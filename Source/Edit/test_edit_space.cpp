#include <test.h>
#include <test_manager.h>
#include <test_service.h>
#include <test_property.h>
#include <test_draw.h>
#include <test_converter.h>
#include <test_computation.h>
#include <tool_time_test.h>
#include <templ_dptr.h>
#include <mb_class_traits.h>
#include <cur_contour.h>
#include <cur_line3d.h>
#include <cur_plane_curve.h>
#include <creator.h>
#include <attr_color.h>
#include <op_shell_parameter.h>
#include <op_mesh_parameter.h>
#include <action_mesh.h>
#include <action_polymesh.h>
#include <action_solid.h>
#include <solid.h>
#include <collection.h>
#include <point_frame.h>
#include <space_instance.h>
#include <plane_instance.h>
#include <attr_geometric_attribute.h>
#include <op_shell_parameter.h>
#include <op_polymesh_parameter.h>
#include <region.h>
#include <topology_iterator.h>
#include <point3d.h>
#include <mip_solid_area_volume.h>
#include <cur_polyline3d.h>
#include <vector>
#include <polymesh.h>
#include <halfedge_range_iterators.h>
#include <polymesh_topo.h>
#include <last.h>

using namespace c3d;

//------------------------------------------------------------------------------
// Перестроить элемент.
// Rebuild the element.
// ---
void RebuildObject()
{
  MbItem * obj = nullptr;

  if ( GetObj( IDS_SELECT_GEOM_OBJECT, &obj ) ) {
    WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
    if ( wind != nullptr )
      wind->EraseObject( obj );
    
    RPArray<MbSpaceItem> spaceItems( 0, 1 );
    MbeCopyMode copyMode = cm_Copy;

    SetWaitCursor( true );
    uint32 miliSec = ::GetTickCountEx();

#ifdef ORIGINAL_MAIN_NAME
    MbNamePairList nameList; // Таблица соответствия имён оригиналов и их копий. // Table of compliance of the names of originals and their copies.
    MbItem * copyObj = nullptr;
    if ( obj->IsA() == st_Solid ) {
      copyObj = (MbItem *)&obj->Duplicate();
      copyObj->AddRef();
    }
    RPArray<MbCreator> creators( 0, 1 );
    obj->GetCreators( creators );
    nameList.Reserve( 5 * creators.Count() ); // Таблица соответствия имён оригиналов и их копий. // Table of compliance of the names of originals and their copies.

    for ( size_t i = 0, iCount = creators.Count(); i < iCount; i++ ) {
      MbSNameMaker & nameMaker = creators[i]->SetYourNameMaker();
      SimpleName original = nameMaker.GetMainName(); // Исходное главное имя. // The original main name.
      SimpleName mainName = (SimpleName)( (size_t)original + COUNT_MAX + i); 
      nameMaker.SetMainName( mainName );
      nameMaker.SetOriginalMainName( original, &nameList );
    }
#endif //  ORIGINAL_MAIN_NAME

    bool rebuildRes = obj->RebuildItem( copyMode, &spaceItems, nullptr );

    miliSec = ::GetTickCountEx() - miliSec;
    SetWaitCursor( false );

    if ( !rebuildRes || (spaceItems.Count() > 0) ) {
      MessageBoxEx( RT_OBJECT_REBUILD_NOT_COMPLETELY, IDS_WERE_ERRORS );
      if ( spaceItems.Count() > 0 ) {
        MessageBoxEx( RT_OBJECT_REBUILD_WITH_CHANGED, IDS_OBJECTS_ADDED );
        bool showItem = false; // вылет под Release: нельзя перерисовывать всю модель (OpenGL), т.к. отрисовочная модель может ссылаться на убитые объекты.
        // Release crash: we can't redraw the entire model (OpenGL), because the draw model can refer to dead objects.
        for ( size_t i = 0; i < spaceItems.Count(); i++ ) {
          TestVariables::viewManager->AddObject( TestVariables::SURFACECURVE_Style, spaceItems[i], showItem );
        }
      }
    }
    else {
      size_t facesCount = 0;
      if ( obj->IsA() == st_Solid ) {
        const MbSolid * solid = (const MbSolid *)obj;
        facesCount = solid->GetFacesCount();

#ifdef ORIGINAL_MAIN_NAME
        const MbSolid * originSolid = (const MbSolid *)copyObj;
        bool result = false;

        if ( facesCount == originSolid->GetFacesCount() ) {
          for ( size_t i = 0; i < facesCount; i++ ) {
            MbFace * origin = originSolid->GetFace( i );
            const MbName & originalName = origin->GetName();
            const MbFace * pure = solid->FindFaceByName( originalName );
            const MbName * copyName = nameList.FindCopyByOriginal( originalName );
            if ( copyName != nullptr ) {
              const MbFace * find = solid->FindFaceByName( *copyName );
              if ( pure == nullptr && find != nullptr ) {
                size_t ind = solid->GetFaceIndex( *find );
                if ( ind == i )
                  result = true;
              }
            }
          }
        }
        RPArray<MbVertex>    vertices( 0, 1 );
        RPArray<MbCurveEdge> edges   ( 0, 1 );
        // Выдать множество ребер и вершин. // Get the set of edges and vertices.
        solid->GetItems( vertices, edges ); 

        RPArray<MbVertex>    s_vertices( 0, 1 );
        RPArray<MbCurveEdge> s_edges   ( 0, 1 );
        // Выдать множество ребер и вершин. // Get the set of edges and vertices.
        originSolid->GetItems( s_vertices, s_edges ); 

        if ( edges.Count() == s_edges.Count() ) {
          for ( size_t i = 0, iCount = s_edges.Count(); i < iCount; i++ ) {
            MbCurveEdge * origin = s_edges[ i ];
            const MbName & originalName = origin->GetName();
            const MbCurveEdge * pure = solid->FindEdgeByName( originalName );
            const MbName * copyName = nameList.FindCopyByOriginal( originalName );
            if ( copyName != nullptr ) {
              const MbCurveEdge * find = solid->FindEdgeByName( *copyName );
              if ( pure == nullptr && find != nullptr ) {
                size_t ind = edges.FindIt( find );
                if ( ind == i )
                  result = true;
              }
            }
          }
        }

        if ( vertices.Count() == s_vertices.Count() ) {
          for ( size_t i = 0, iCount = s_vertices.Count(); i < iCount; i++ ) {
            MbVertex * origin = s_vertices[ i ];
            const MbName & originalName = origin->GetName();
            const MbVertex * pure = solid->FindVertexByName( originalName );
            const MbName * copyName = nameList.FindCopyByOriginal( originalName );
            if ( copyName != nullptr ) {
              const MbVertex * find = solid->FindVertexByName( *copyName );
              if ( pure == nullptr && find != nullptr ) {
                size_t ind = vertices.FindIt( find );
                if ( ind == i )
                  result = true;
              }
            }
          }
        }
#endif //  ORIGINAL_MAIN_NAME
      }

      size_t operationsCount =  obj->GetCreatorsCount();

      ShowTimeTestResult( nullptr, operationsCount, facesCount, miliSec, st_Solid, IDS_REBUILD_OBJECT );
    }

#ifdef ORIGINAL_MAIN_NAME
    for ( size_t i = 0, iCount = creators.Count(); i < iCount; i++ ) {
      MbSNameMaker & nameMaker = creators[i]->SetYourNameMaker();
      SimpleName original = nameMaker.GetMainName(); // Исходное главное имя. // The original main name.
      nameMaker.SetOriginalMainName( original, nullptr );
    }
    ::ReleaseItem( copyObj );
#endif //  ORIGINAL_MAIN_NAME

    TestVariables::viewManager->RefreshObjectModel( obj );
  }
}


//------------------------------------------------------------------------------
// Сдвиг элемента.
// Move the element.
// ---
bool MoveObject()
{ 
  PathMatrix objPathFrom;
  if ( GetInstance( IDS_SELECT_GEOM_OBJECT, objPathFrom ) != nullptr ) 
  {
    TestVariables::viewManager->ToggleObject( objPathFrom.first );
    MbCartPoint3D p0, p1;
    if ( GetPoint( IDS_SELECT_START_POINT, p0 ) ) 
    {
      while ( GetPoint( IDS_SELECT_FINAL_POINT, p1 ) ) 
      {
        TestVariables::viewManager->MoveObject( objPathFrom.first, p1 - p0 );
        p0 = p1;
      }
    }

    TestVariables::viewManager->ToggleObject( objPathFrom.first );
    TestVariables::viewManager->ShowModel();
    return true;
  }
  return false;
}


//------------------------------------------------------------------------------
// Поворот элемента в плоскости.
// Rotate the element in the plane.
// ---
bool PlaneRotateObject()
{
  MbCartPoint3D point;

  if ( GetPoint( IDS_SELECT_POINT, point ) ) 
  {
    WorkWindow * currentView = TestVariables::viewManager->GetActiveWindow();
    MbPlacement3D plane( currentView->GetNullPlacement() );
    MbAxis3D rotAxis( point, plane.GetAxisZ() );
    double angle = 0;

    while ( GetDouble( IDS_ENTER_ROTATE_ANGLE, angle ) ) 
    {
      /*
        МА: Вставочную геометрию (повторно-используемую) не трогаем, а вместо этого 
            трансформируем конкретно выбранную вставку.
      */      
      PathMatrix pathMat;
      while ( MbItem * obj = GetInstance(IDS_SELECT_GEOM_OBJECT, pathMat) )
      {
        MbMatrix3D invMat;
        rotAxis.Transform( pathMat.second.Div(invMat) );

        TestVariables::viewManager->ToggleObject( obj );
        TestVariables::viewManager->RotateObject( obj, rotAxis, angle * Math::DEGRAD );
        TestVariables::viewManager->ToggleObject( obj );
        TestVariables::viewManager->ShowModel();
      }
    }

    return true;
  }
  return false;
}


//------------------------------------------------------------------------------
// Поворот элемента вокруг линии.
// Rotate the element around the line.
// ---
bool LineRotateObject() 
{ 
  PathMatrix pathMat;
  if ( MbItem * obj = GetInstance( IDS_SELECT_GEOM_OBJECT, pathMat ) ) 
  {
    TestVariables::viewManager->ToggleObject(obj);

    double angle = 0;
    MbItem * nearest = 0;
    while ( GetGObj( IDS_SELECT_ROTATE_AXIS, st_Line3D, &nearest ) &&
            GetDouble( IDS_ENTER_ROTATE_ANGLE, angle ) ) 
    {
      if ( const MbSpaceInstance * sInst = isa_cast<const MbSpaceInstance *>(nearest) )
      {
        const MbLine3D * line = static_cast<const MbLine3D *>( sInst->GetSpaceItem() );
        MbAxis3D rotAxis( line->GetOrigin(), line->GetDirection() );
        MbMatrix3D invMat;
        rotAxis.Transform( pathMat.second.Div(invMat) );
        TestVariables::viewManager->RotateObject( obj, rotAxis, angle * Math::DEGRAD );
      }      
    }
    TestVariables::viewManager->ToggleObject(obj);
    TestVariables::viewManager->ShowModel();
    return true;
  }
  return false;
}


//------------------------------------------------------------------------------
// Масштабировать элемент
// Scale the element.
// ---
void ScaleObject()
{
  MbCartPoint3D p0;
  if ( GetPoint( IDS_SELECT_POINT, p0 ) ) {
    double s1 = 2;
    if ( GetDouble( IDS_ENTER_SCALE, s1 ) ) {
      double s2 = s1;
      if ( GetDouble( IDS_ENTER_SCALE, s2 ) ) {
        double s3 = s2;
        if ( GetDouble( IDS_ENTER_SCALE, s3 ) ) {

          MbMatrix3D matr;
          {
            MbMatrix3D a, b;
            a.Move( p0.x, p0.y, p0.z );
            a.Div( b );
            matr.ScaleX(s1);
            matr.ScaleY(s2);
            matr.ScaleZ(s3);
            matr = b * matr;
            matr = matr * a;
          }
          MbItem * obj = nullptr;
          WorkWindow * currentView = TestVariables::viewManager->GetActiveWindow();
          MbPlacement3D plane( currentView->GetOwnPlacement() );
          MbCartPoint3D origin( plane.GetOrigin() );
          MbVector3D direct( plane.GetAxisZ() );

          while ( GetObj( IDS_SELECT_GEOM_OBJECT, &obj ) ) {
            if ( obj->IsA() == st_PlaneInstance ) {
              MbPlaneItem * planeItem = ((MbPlaneInstance *)obj)->SetPlaneItem(); // Дать двумерный геометрический объект. // Give a two-dimensional geometric object.
              if ( planeItem != nullptr ) {
                MbPlacement3D place( ((MbPlaneInstance *)obj)->GetPlacement() );
                MbCartPoint p;
                if ( place.DirectPointProjection( origin, direct, p.x, p.y ) ) {
                  MbMatrix m, a, b;
                  a.Move( p.x, p.y );
                  a.Div( b );
                  m.ScaleX(s1);
                  m.ScaleY(s2);
                  m = b * m;
                  m = m * a;
                  planeItem->Transform( m );
                  MbMatrix3D mat;
                  TestVariables::viewManager->TransformObject( obj, mat );
                }
              }
              else
                TestVariables::viewManager->TransformObject( obj, matr );
            }
            else {
              if ( obj->IsA() == st_Solid ) {
                MbSolid * solid = static_cast<MbSolid *>(obj);

                MbDuplicationSolidParams params( matr );
                SolidSPtr result;

                SetWaitCursor( true );

                MbResultType res = ::DuplicationSolid( *solid, params, result );

                SetWaitCursor( false );

                if ( result == nullptr )
                  ::PutErrorMessage( res, IDS_TITLE_ERR );
                if ( result != nullptr ) {
                  Style objStyle( obj->GetWidth(), obj->GetColor() );
                  TestVariables::viewManager->DeleteObject( obj );
                  TestVariables::viewManager->AddObject( objStyle, result );
                }
              }
              else {
                TestVariables::viewManager->TransformObject( obj, matr );
              }
            }
          }
          TestVariables::viewManager->ShowModel();
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Перевести в локальную систему. Transform by local system.
// ---
void TransformObject()
{
  MbCartPoint3D p0, p;
  if ( GetPoint( IDS_SELECT_CENTER, p0 ) ) {
    MbVector3D axisX( 1.0, 0.0, 0.0 ), axisY( 0.0, 1.0, 0.0 ), axisZ( 0.0, 0.0, 1.0 );
    p = axisX;
    if ( GetPoint( IDS_SELECT_VECTOR_X, p ) ) {
      axisX = p;
      p = axisY;
      if ( GetPoint( IDS_SELECT_VECTOR_Y, p ) ) {
        axisY = p;
        p = axisZ;
        if ( GetPoint( IDS_SELECT_VECTOR_Y, p ) ) {
          axisZ = p;

          MbPlacement3D place;
          place.SetOrigin() = p0;
          place.SetAxisX() = axisX;
          place.SetAxisY() = axisY;
          place.SetAxisZ() = axisZ;
          MbMatrix3D matr;
          place.GetMatrixFrom( matr );

          if ( !matr.IsSingle() ) {
            MbItem * obj = nullptr;

            while ( GetObj( IDS_SELECT_GEOM_OBJECT, &obj ) ) {
              MbSolid * solid = static_cast< MbSolid * >( obj );
              SolidSPtr result;
              if ( solid != nullptr && solid->IsA() == st_Solid ) {
                SetWaitCursor( true );
                MbDuplicationSolidParams params ( matr );
                MbResultType res = ::DuplicationSolid( *solid, params, result );

                if ( result == nullptr )
                  ::PutErrorMessage( res, IDS_TITLE_ERR );
                if ( result != nullptr ) {
                  Style objStyle( obj->GetWidth(), obj->GetColor() );
                  TestVariables::viewManager->DeleteObject( obj );
                  TestVariables::viewManager->AddObject( objStyle, result );
                }  
                SetWaitCursor( false );
              }
            }
          }
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Дублировать элемент.
// Duplicate the element.
// ---
void DuplicateObject()
{
  MbItem * obj = nullptr;
  while ( GetObj( IDS_SELECT_GEOM_OBJECT, &obj ) ) {
    TestVariables::viewManager->ToggleObject(obj);
    TestVariables::viewManager->DuplicateObject( obj );
  }
}


//------------------------------------------------------------------------------
// Зеркально дублировать элемент.
// Mirror duplicate the element.
// ---
void MirrorDuplicateObject() {
  MbCartPoint3D p0, p1;

  if ( GetPoint( IDS_SELECT_START_POINT, p0 ) &&
       GetPoint( IDS_SELECT_FINAL_POINT, p1 ) ) {

    MbLine3D line( p0, p1 );
//#ifdef _DRAWGI
    TestVariables::drawImp->DrawItem( &line, TRGB_LIGHTRED );
//#endif // _DRAWGI
    WorkWindow *currentView = TestVariables::viewManager->GetActiveWindow();
    MbVector3D vx( p0, p1 );
    MbMatrix3D matr;
    matr.Symmetry(p0,vx,currentView->GetAxisZ());

    RPArray <MbItem> objs(0,1);
    TestVariables::viewManager->GetSelectedObjects( objs ); // Нахождение выделенных объектов. // Finding selected objects.
    ptrdiff_t count = objs.Count();

    if ( count > 0 ) {
      for ( ptrdiff_t i = 0; i < count; i++ ) {
        TestVariables::viewManager->TransformDuplicateObject( objs[i], matr );
      }
    }

    MbItem *obj;

    while ( GetObj( IDS_SELECT_GEOM_OBJECT, &obj ) ) {
      TestVariables::viewManager->ToggleObject(obj);
      TestVariables::viewManager->TransformDuplicateObject( obj, matr );
    }
  }
}


//------------------------------------------------------------------------------
// Удалить элемент.
// Delete the element.
// ---
void DeleteObject()
{
  MbItem * obj = nullptr;

  PathMatrix objPathFrom;
  while ( GetObj( IDS_SELECT_GEOM_OBJECT, &obj, objPathFrom ) ) {
    TestVariables::viewManager->DeleteObject( objPathFrom.first ); //TestVariables::viewManager->DeleteObject( obj );
  }
  if ( obj == nullptr ) {
    WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
    if ( wind != nullptr ) {
      MbModel & model = wind->SetGeomModel();
      RPArray<MbItem> items( 0, 1 );
      RPArray<MbItem> items_delete( 0, 1 );
      // Дать все объекты.  // Give all objects. 
      model.GetItems( items ); 
      for ( size_t i = model.ItemsCount(); i--;  ) {
        MbItem * obj_i = items[i];
        for ( size_t j = i; j--;  ) {
          MbItem * obj_j = items[j];
          if ( (obj_i != nullptr) && (obj_j != nullptr) && obj_i->IsSame(*obj_j) ) {
            items_delete.Add( obj_i );
          }
        }
      }
      for ( size_t j = 0, jCount = items_delete.Count(); j < jCount; j++ ) {
        TestVariables::viewManager->DeleteObject( items_delete[j] );
      }
    }
  }
}


//------------------------------------------------------------------------------
// Добавить атрибут color или visual.
// Add a color or visual attribute.
// ---
template <typename Item>
void SetColorForObject( Item & item, COLORREF color )
{
  MbSolid * solid = dynamic_cast<MbSolid *>( &item );
  if ( solid != nullptr ) {
    MbFaceShell * shell = dynamic_cast<MbSolid *>( &item )->GetShell();
    for ( size_t i = 0; i < shell->GetFacesCount(); i++ ) {
      MbFace * face = shell->GetFace( i );
      if ( face != nullptr ) {
        const MbAttribute * attr = face->GetSimpleAttribute( at_Visual );
        if ( attr != nullptr && static_cast<const MbVisual *>( attr )->GetColor() != color )
          face->RemoveAttribute( attr );
      }
    }
  }
  item.SetColor( color );
}


//------------------------------------------------------------------------------
// Добавить атрибут color или visual у выбранных объектов.
// Add a color or visual attribute to the selected objects.
// ---
void SetAllObjectColorOrVisual( MbItem & obj, MbSolid & solid, COLORREF color )
{
  SetWaitCursor( true );
  for ( SolidFacesIterator faceItr( solid ); faceItr.undone(); faceItr.next() ) {
    MbFace & face( *faceItr );
    SetColorForObject( face, color );
  }
  TestVariables::viewManager->RedrawObject( &obj ); // Перерисовать объект.
  SetWaitCursor( false );
}


//------------------------------------------------------------------------------
// Изменить цвет элементов. 
// Change the elements color.
// ---
void SetObjectColor( bool all ) 
{
  MbItem * obj = nullptr;
  MbRefItem * item = nullptr;
  COLORREF color;// = BLACK;

  while ( GetObj( IDS_SELECT_GEOM_OBJECT, &obj, &item ) )  {
    TestVariables::viewManager->UnselectObject( obj );
    if ( CMUColor( color ) ) {
      MbAttributeContainer & workItem = item != nullptr ? *dynamic_cast< MbAttributeContainer * >( item ) : *obj;
      SetColorForObject( workItem, color );
      if ( ( all && item != nullptr && item->RefType() == rt_SpaceItem ) || ( item == nullptr && obj->IsA() == st_Solid ) ) {
        MbSolid & solid = *static_cast< MbSolid * >( &workItem );
        SetAllObjectColorOrVisual( *obj, solid, color );
      }
    }
    else {
      return;
    }
  }
}


//------------------------------------------------------------------------------
// Изменить толщину линий элементов.
// Change the line thickness of the elements.
// ---
void SetObjectWidth()
{
  MbItem * obj = nullptr;
  int w = 1;
  if ( GetInt( IDS_ENTER_THICKNESS, w ) ) {
    while ( GetObj( IDS_SELECT_GEOM_OBJECT, &obj ) ) {
      WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
      if ( wind != nullptr )
        wind->SetObjectWidth( obj, w );
    }
  }
}


//------------------------------------------------------------------------------
// Создавать полигональные объекты на числах double(true)/float(false). 
// Create polygonal object on double(true)/float(false) data.
// ---
void SetupDoubleMesh()
{
  int m = TestVariables::exactMesh ? 1 : 0;

  if ( GetInt( IDS_DOUBLE_MESH, m ) ) {
    if ( m > 0 ) 
      TestVariables::exactMesh = true;  // double data
    else
      TestVariables::exactMesh = false; // float data
  }
}


//------------------------------------------------------------------------------
// Слияние элементов.
// Merge the elements.
// ---
void SetupItemsMerging()
{
  int m = TestVariables::mergeFaces ? 1 : 0;

  if ( GetInt( IDS_ENTER_MERGING_FACES, m ) ) {
    if ( m > 0 ) 
      TestVariables::mergeFaces = true;
    else
      TestVariables::mergeFaces = false; // Объединять ли грани в операциях?  // Combine whether faces in operations?

    m = TestVariables::mergeEdges ? 1 : 0;

    if ( GetInt( IDS_ENTER_MERGING_EDGES, m ) ) {
      if ( m > 0 ) 
        TestVariables::mergeEdges = true;
      else
        TestVariables::mergeEdges = false; // Объединять ли грани в операциях?  // Combine whether faces in operations?
    }
  }
}


//------------------------------------------------------------------------------
// Сделать объект невидимым.
// Make object invisible.
// ---
void InvisibleObject()
{
  bool goNext = true;

  while ( goNext ) {
    goNext = false;

    MbItem * obj = nullptr;
    MbRefItem * item = nullptr;

    //if ( GetObj( IDS_SELECT_GEOM_OBJECT, &obj ) ) {
    if ( GetObj( IDS_SELECT_EDIT_ELEMENT, &obj, &item ) ) {
      WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
      if ( wind != nullptr ) {
        if ( item != nullptr ) { // выбран элемент объекта
          if ( item->RefType() == rt_TopItem ) {
            MbTopologyItem * tItem = dynamic_cast<MbTopologyItem *>(item);
            if ( tItem != nullptr ) {
              tItem->SetVisible( false );
              wind->RefreshObjectModel( obj );
              goNext = true;
            }
          }
        }
        else {
          wind->SetObjectVisible( obj, false );
          goNext = true;
        }
      }
    }
  }
}
       

//------------------------------------------------------------------------------
// Показать все объекты.
// Show all objects.
// ---
void VisibleAllObjects()
{
  TestVariables::viewManager->SetVisibleByType( st_SpaceItem, true );
  TestVariables::viewManager->SetVisibleByType( pt_PlaneItem, true );
}


//------------------------------------------------------------------------------
// Показать точки.
// Show points.
// ---
void VisiblePoints()
{
  TestVariables::viewManager->SetVisibleByType( st_Point3D, true );
  TestVariables::viewManager->SetVisibleByType( st_PointFrame, true );
}


//------------------------------------------------------------------------------
// Показать кривые.
// Show curves.
// ---
void VisibleCurves()
{
  TestVariables::viewManager->SetVisibleByType( st_Curve3D, true );
  TestVariables::viewManager->SetVisibleByType( pt_Curve, true );
}


//------------------------------------------------------------------------------
// Показать поверхности.
// Show surfaces.
// ---
void VisibleSurfaces()
{
  TestVariables::viewManager->SetVisibleByType( st_Surface, true );
}


//------------------------------------------------------------------------------
// Показать тела.
// Show solids.
// ---
void VisibleBodies()
{
  TestVariables::viewManager->SetVisibleByType( st_Solid, true );
}


//------------------------------------------------------------------------------
// Редактирование параметров элемента.
// Edit parameters of the element.
// ---
bool EditProperty()  
{
  MbItem * editObj = nullptr;
  if ( GetObj( IDS_SELECT_EDIT_ELEMENT, &editObj ) ) 
  {
    TestVariables::viewManager->CreateProperty( editObj );
    return true;
  }
  return false;
}


//------------------------------------------------------------------------------
// Сдвиг выделенных элементов.
// Move selected elements.
// ---
bool MoveSelectedObjects()
{
  MbCartPoint3D points[2];

  if ( GetPoint( IDS_SELECT_START_POINT, points[0] ) &&
       GetPoint( IDS_SELECT_FINAL_POINT, points[1] ) ) {
    MbVector3D to( points[0], points[1] );
    TestVariables::viewManager->MoveSelected( to );
    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
// Поворот выделенных элементов в плоскости.
// Rotate selected elements in the plane.
// ---
bool PlaneRotateSelectedObjects()
{
  double angle = 0;
  MbCartPoint3D point;

  if ( GetPoint( IDS_SELECT_POINT, point ) &&
       GetDouble( IDS_ENTER_ROTATE_ANGLE, angle ) ) {

    WorkWindow *currentView = TestVariables::viewManager->GetActiveWindow();
    MbPlacement3D plane( currentView->GetNullPlacement() );
    MbAxis3D rotAxis( point, plane.GetAxisZ() );
    TestVariables::viewManager->RotateSelected( rotAxis, angle * Math::DEGRAD );
    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
// Поворот выделенных элементов вокруг линии.
// Rotate selected elements around the line.
// ---
bool LineRotateSelectedObjects()
{
  double angle = 0;
  MbItem * nearest = nullptr;

  if ( GetGObj( IDS_SELECT_ROTATE_AXIS, st_Line3D, &nearest ) &&
       GetDouble( IDS_ENTER_ROTATE_ANGLE, angle ) ) {
    MbLine3D *line = (MbLine3D *)((MbSpaceInstance *)nearest)->GetSpaceItem();
    MbAxis3D rotAxis( line->GetOrigin(), line->GetDirection() );
    TestVariables::viewManager->RotateSelected( rotAxis, angle * Math::DEGRAD );
    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
// Удалить выделенные элементы. Delete selected elements.
// ---
void DeleteSelected()
{
  // Запомненный объект и его элемент
  TestVariables::ResetHLighted();
  WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
  if ( wind != nullptr )
    wind->DeleteSelected();
}


//------------------------------------------------------------------------------
// Удаление все невидимые объекты. Delete all invisible objects.
// ---
void DeleteInvisible()
{
  WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
  if ( wind != nullptr )
    wind->DeleteInvisible();
}


//------------------------------------------------------------------------------
// Масштабировать выделенные элементы.
// Scale selected elements.
// ---
void ScaleSelectedObjects()
{
  MbCartPoint3D p0;

  if ( GetPoint( IDS_SELECT_POINT, p0 ) ) {
    double s1 = 2;
    if ( GetDouble( IDS_ENTER_SCALE, s1 ) ) {
      double s2 = s1;
      if ( GetDouble( IDS_ENTER_SCALE, s2 ) ) {
        double s3 = s2;
        if ( GetDouble( IDS_ENTER_SCALE, s3 ) ) {
          MbMatrix3D matr, a, b;
          a.Move( p0.x, p0.y, p0.z );
          a.Div( b );
          matr.ScaleX(s1);
          matr.ScaleY(s2);
          matr.ScaleZ(s3);
          matr = b * matr;
          matr = matr * a;
          TestVariables::viewManager->TransformSelected( matr );
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Удалить объект, оставив базовые объекты.
// Remove the object, leaving the base objects.
// ---
bool RemoveBasis()
{
  MbItem * nearest = nullptr;
  bool res( false );

  if ( GetGObj( IDS_SELECT_GEOM_OBJECT, st_SpaceItem, &nearest ) ) {
    TestVariables::viewManager->ToggleObject(nearest);

    RPArray<MbSpaceItem> s( 0, 1 );
    nearest->GetBasisItems( s );
    size_t sCnt = s.Count();
    bool noBasis = (sCnt < 1);

    if ( noBasis ) {
      if ( nearest->IsA() == st_SpaceInstance ) {
        const MbSpaceItem * spaceItem = (static_cast<MbSpaceInstance *>(nearest))->GetSpaceItem();
        if ( spaceItem != nullptr ) {
          if ( spaceItem->IsA() == st_PlaneCurve ) {
            const MbPlaneCurve * item = static_cast<const MbPlaneCurve *>(spaceItem);
            MbPlacement3D pl(item->GetPlacement() );
            MbCurve * curv = static_cast<MbCurve *>(&item->GetCurve().Duplicate());
            TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, curv, &pl );
            noBasis = false;
          }
          else {
            (const_cast<MbSpaceItem *>(spaceItem))->GetBasisItems( s );
            sCnt = s.Count();
            noBasis = (sCnt < 1);
          }
        }
      }
      else if ( nearest->IsA() ==  st_PlaneInstance ) {
        const MbPlaneInstance * planeInst = static_cast<MbPlaneInstance *>(nearest);
        size_t itemsCnt = planeInst->PlaneItemsCount();
        if ( itemsCnt > 0 ) {
          const MbPlacement3D & place = planeInst->GetPlacement();
          for ( size_t j = 0; j < itemsCnt; j++ ) {
            const MbPlaneItem * item = planeInst->GetPlaneItem( j );
            if ( item != nullptr ) {
              if ( item->IsA() == pt_Region ) {
                const MbRegion * region = static_cast<const MbRegion *>(item);
                size_t contoursCnt = region->GetContoursCount();
                if ( contoursCnt > 0 ) {
                  for ( size_t k = 0; k < contoursCnt; k++ ) {
                    const MbContour * contour = region->GetContour(k);
                    if ( (contour != nullptr) && (contour->GetSegmentsCount() > 0) ) {
                      TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, &contour->Duplicate(), &place );
                      noBasis = false;
                    }
                  }
                }
              }
              else if ( itemsCnt > 1 ) {
                TestVariables::viewManager->AddObject( TestVariables::CURVE_Style, &item->Duplicate(), &place );
                noBasis = false;
              }
            }
          }
        }
      }
    }

    TestVariables::viewManager->ToggleObject(nearest);
    ::AddRefItems( s );

    if ( sCnt > 0 ) {
      for ( size_t k = 0; k < sCnt; k++ ) {
        TestVariables::viewManager->AddObject( TestVariables::SURFACE_Style, &s[k]->Duplicate() ); // копия из-за распаралелливания отрисовки по MbItem
      }
    }
    else if ( noBasis ) {
      MessageBoxEx( RT_BASIS_OBJECTS_ERROR, IDS_TITLE_WRN );
    }

    TestVariables::viewManager->DeleteObject( nearest ); // Удаление элемента. // Delete element.

    ::ReleaseItems( s );
    res = true;
  }
  else {
    if ( WorkWindow * wind = TestVariables::viewManager->GetActiveWindow() ) {
      RPArray<MbSpaceItem> s( 0, 1 );
      MbModel & geomModel = wind->SetGeomModel(); // Выдать геометрическую модель.
      AttrVector attributes;
      // Выдать атрибуты заданного типа. \en Get attributes of a given type. 
      geomModel.GetAttributes( attributes, at_CommonAttribute, at_Undefined );
      size_t iCount = attributes.size();
      for ( size_t i = 0; i < iCount; ++i ) {
        MbAttribute * attr = attributes[i];
        if ( attr != nullptr && attr->AttributeType() == at_GeomAttribute ) {
          MbGeomAttribute * geomAttr = static_cast<MbGeomAttribute*>( attr );
          if ( geomAttr != nullptr ) {
            s.Add( geomAttr->SetSpaceItem() );
            res = true;
          }
        }
      }
      size_t sCnt = s.Count();
      if ( sCnt > 0 ) {
        for ( size_t k = 0; k < sCnt; k++ ) {
          TestVariables::viewManager->AddObject( TestVariables::SOLID_Style, &s[k]->Duplicate() ); // копия из-за распаралелливания отрисовки по MbItem
        }
      }
    }
  }

  return res;
}


//------------------------------------------------------------------------------
// Дать базовые точки объекта.
// Give the base points of the object.
// ---
void AddBasisPoints() {
  MbItem * object0;

  if ( GetGObj( IDS_SELECT_GEOM_OBJECT, st_SpaceItem, &object0 ) ) {
    TestVariables::viewManager->ToggleObject(object0);

    MbControlData3D pnts;
    MbCartPoint3D pnt;

    object0->GetBasisPoints( pnts ); // Дать базовые точки. // Give the base points.

    for ( size_t i = 0, n = pnts.Count(); i < n; i++ ) {
      pnts.GetPoint( i, pnt );
      TestVariables::viewManager->AddObject( TestVariables::BACKGROUND_Style, new MbPointFrame(pnt) );
    }
    TestVariables::viewManager->ToggleObject(object0);
  }
}


//------------------------------------------------------------------------------
// Разрезать модель
// Cut the model.
// ---
bool CutModelObjects()
{
  MbCartPoint3D points[2];
  MbVector3D to;

  if ( GetPoint( IDS_SELECT_START_POINT, points[0] ) ) {
    if  ( GetPoint( IDS_SELECT_FINAL_POINT, points[1] ) ) {
      TestVariables::distance = 0.0; // Внешняя. // Outside.
      //if ( GetDouble( IDS_ENTER_DISTANCE, distance ) ) 
      {
        WorkWindow * vc = TestVariables::viewManager->GetActiveWindow();
        MbVector3D axisX( points[0], points[1] ), axisY( -vc->GetNullPlacement().GetAxisZ() );
        MbPlacement3D place( axisX, axisY, points[0] ); 
        SetWaitCursor( true );

        MbCube r;
        TestVariables::viewManager->GetDrawModelGabarit( r ); // Дать габарит полигональной модели. // Give the gabarit of the polygonal model.

        size_t operationCount = 0;
/*
        size_t trianglesCount = viewManager->GetTrianglesCount();

#if defined(C3D_DEBUG)
        SET_TIME_TEST(true);
#endif // C3D_DEBUG
        uint32 miliSec = ::GetTickCountEx();
*/
        TestVariables::viewManager->CutModel( place, TestVariables::distance, operationCount );
/*
        miliSec = ::GetTickCountEx() - miliSec;
#if defined(C3D_DEBUG)
        CHECK_TIME_TEST();
#endif // C3D_DEBUG
*/
        SetWaitCursor( false );
/*
        TimeTest * testResult = nullptr;
#if defined(C3D_DEBUG)
        testResult = ::GetTimeTestResults();
#endif // C3D_DEBUG

        string rebuildMsg( RT_CUTTING_MODEL );

        ::ShowTimeTestResult( testResult, operationCount, trianglesCount, miliSec, true, rebuildMsg.c_str() );

#if defined(C3D_DEBUG)
        SET_TIME_TEST(false);
#endif // C3D_DEBUG
*/
        TestVariables::viewManager->ShowModel();
        return true;
      }
    }
  }
  return false;
}


//------------------------------------------------------------------------------
// Превратить в слиток.
// Convert to ingot.
// ---
void MakeIngot() 
{
  WorkWindow * vc = TestVariables::viewManager->GetActiveWindow();
  if ( vc != nullptr ) {
    RPArray<MbItem> rpItems;
    vc->GetGeomModel().GetItems( rpItems ); // Сразу в SPtr нельзя
    ItemsSPtrVector items;
    ::InitSPtrArray( rpItems, items ); // Переводим RPArray в vector, без проверок, конструктор параметров ниже всё обработает.

    SetWaitCursor( true );
    uint32 miliSec = ::GetTickCountEx();
    SolidSPtr ingotSolid ( nullptr );
    DPtr<IProgressIndicator> progBar( &::CreateProgressIndicator( new StrSpyCommon ) );
    MbIngotSolidParams params( items, MbSNameMaker(st_Assembly, MbSNameMaker::i_SideNone, 0), progBar );
    bool copy ( true ); // false - заменяем всё слитком, true - создаём слиток, не удаляя оболочки.
    MbResultType returnResult = ::IngotSolid( copy, params, ingotSolid );
    miliSec = ::GetTickCountEx() - miliSec;
    SetWaitCursor( false );

    size_t itemsCount = items.size();
    size_t facesCount = ( ingotSolid != nullptr ) ? ingotSolid->GetFacesCount() : 0;

    ShowTimeTestResult( nullptr, itemsCount, facesCount, miliSec, st_Solid, IDS_RESULT );

    if ( ingotSolid != nullptr ) {
      if ( !copy )
        for ( size_t i = 0, ssize = items.size(); i < ssize; ++i )
          TestVariables::viewManager->DeleteObject( items[i].detach() );
      TestVariables::viewManager->AddObject( *ingotSolid.detach() );
    }
    if( returnResult != rt_Success )
      ::PutErrorMessage( returnResult, IDS_TITLE_ERR );
  }
}


#define CHECK_MESH_TO_BREP_TIME
//------------------------------------------------------------------------------
// Преобразовать полигональную модель в тело.
// Convert the polygonal model into the solid.
// ---
void MeshToSolid() 
{
  MbItem * obj = nullptr;

  if ( GetObj( IDS_SELECT_GEOM_OBJECT, &obj ) ) {
    TestVariables::viewManager->ToggleObject(obj);

    SPtr<MbMesh> mesh;
    SPtr<MbCollection> collection;
    if ( obj->IsA() == st_Mesh )
      mesh = &static_cast<MbMesh &>( *obj );
    else if ( obj->IsA() == st_Collection ) {
      collection = &static_cast<MbCollection &>( *obj );
    }
    else if ( obj->IsA() == st_Solid ) {
      const MbSolid & sol = static_cast<MbSolid &>( *obj );

      bool createParams = true;
      {
        int iCreateParams = createParams ? 1 : 0;
        GetInt( IDS_USE_PARAM_POINTS, iCreateParams );
        if ( iCreateParams == 0 )
          createParams = false;
      }
      MbStepData stepData;
      int mode = TestVariables::operateType;
      ::InitStepData( stepData, createParams, mode );
      SPtr<MbItem> meshItem( sol.CreateMesh( stepData, MbFormNote(false, true, true, TestVariables::exactMesh), nullptr ) );
      if ( (meshItem != nullptr) && (meshItem->IsA() == st_Mesh) )
        mesh = &static_cast<MbMesh &>( *meshItem );
    }

    SolidSPtr solid;
    MbResultType res = rt_Error;

    if ( mesh != nullptr ) {
      if ( mesh->GridsCount() > 0 ) {
        DPtr<IProgressIndicator> progBar( &::CreateProgressIndicator( new StrSpyPointsSurface ) );

#ifdef CHECK_MESH_TO_BREP_TIME
        size_t pointsCount = mesh->AllPointsCount();
        size_t trianglesCount = mesh->AllTrianglesCount();
        size_t quadranglesCount = mesh->AllQuadranglesCount();
#endif
        bool sewGrids = true;
        {
          int iSewGrids = sewGrids ? 1 : 0;
          GetInt( IDS_ENTER_STITCH_GRIDS, iSewGrids );
          sewGrids = (iSewGrids != 0) ? true : false;
        }
        SetupItemsMerging();
        bool mergeFaces = TestVariables::mergeFaces;

        bool useGridSurface = false;
        {
          int iGridSurface = useGridSurface ? 1 : 0;
          GetInt( IDS_ENTER_USE_GRIDSURFACE, iGridSurface );
          useGridSurface = (iGridSurface != 0) ? true : false;
        }
     
        MbSNameMaker snMaker( SimpleName(-1), MbSNameMaker::i_SideNone, 0 );
        MbGridShellParams params( sewGrids, mergeFaces, useGridSurface, false, snMaker, progBar );

        SetWaitCursor( true );
        uint miliSec = ::GetTickCountEx();

        res = ::MeshSolid( *mesh, params, solid );

        miliSec = ::GetTickCountEx() - miliSec;
        SetWaitCursor( false );

#ifdef CHECK_MESH_TO_BREP_TIME
        size_t solidsCount = 0; 
        size_t facesCount = 0;
        if ( solid != nullptr ) {
          solidsCount = 1; 
          facesCount = solid->GetFacesCount();
        }
        if ( TestVariables::checkOperationTime ) {
          ::ShowTimeTestResult( nullptr, solidsCount, facesCount, pointsCount, trianglesCount, quadranglesCount,
                                miliSec, IDS_GRIDS_TO_SHELL );
        }
#endif
      }
    }

    if ( collection != nullptr ) {
      if ( collection->TrianglesCount() > 0 ) {
        DPtr<IProgressIndicator> progBar( &::CreateProgressIndicator( new StrSpyPointsSurface ) );

#ifdef CHECK_MESH_TO_BREP_TIME
        size_t pointsCount = collection->PointsCount();
        size_t trianglesCount = collection->TrianglesCount();
        size_t quadranglesCount = collection->QuadranglesCount();
#endif
        MbSNameMaker snMaker( SimpleName(-1), MbSNameMaker::i_SideNone, 0 );
        MbGridShellParams params( true, true, false, false, snMaker, progBar );
        SetWaitCursor( true );
        uint miliSec = ::GetTickCountEx();
        res = ::CollectionSolid( *collection, params, solid );
        miliSec = ::GetTickCountEx() - miliSec;
        SetWaitCursor( false );

#ifdef CHECK_MESH_TO_BREP_TIME
        size_t solidsCount = 0; 
        size_t facesCount = 0;
        if ( solid != nullptr ) {
          solidsCount = 1; 
          facesCount = solid->GetFacesCount();
        }
        if ( TestVariables::checkOperationTime ) {
          ::ShowTimeTestResult( nullptr, solidsCount, facesCount, pointsCount, trianglesCount, quadranglesCount,
                                miliSec, IDS_GRIDS_TO_SHELL );
        }
#endif
      }
    }

    if ( solid != nullptr ) {
      Style st( obj->GetWidth(), obj->GetColor() );
      TestVariables::viewManager->AddObject( st, solid, false );
      if ( obj != solid )
        TestVariables::viewManager->DeleteObject( obj );
      TestVariables::viewManager->ShowModel();
    }
    else {
      if ( res == rt_ProcessIsStopped )
        MessageBoxEx( RT_PROCESS_IS_STOPPED, IDS_RESULT );
      else {
        MessageBoxEx( RT_OBJECT_HAS_NOT_GRID, IDS_RESULT );
      }
    }
  }
}


//------------------------------------------------------------------------------
// Удалить дублирующие с заданной точностью друг друга точки.
// Remove redundant points.
// ---
static
bool RemoveRedundantPoints( MbMesh & mesh, bool deleteNormals )
{ 
  bool isDone = false;

  for ( size_t k = 0, gridsCnt = mesh.GridsCount(); k < gridsCnt; ++k ) {
    MbGrid * grid = mesh.SetGrid( k );
    if ( grid->RemoveRedundantPoints( deleteNormals ) )
      isDone = true;
  }

  return isDone;
}


//------------------------------------------------------------------------------
// Упростить полигональную модель.
// Simplify a mesh.
// ---
void SimplifyMesh()
{
  MbItem * obj = nullptr;

  if ( GetObj( IDS_SELECT_GEOM_OBJECT, &obj ) ) {
    TestVariables::viewManager->ToggleObject( obj );

    SPtr<MbMesh> mesh;
    SPtr<MbCollection> coll;

    if ( obj->IsA() == st_Mesh )
      mesh = &static_cast<MbMesh &>(*obj);
    else if ( obj->IsA() == st_Collection ) 
      coll = &static_cast<MbCollection &>(*obj);
    else if ( obj->IsA() == st_Solid ) {
      const MbSolid & sol = static_cast<MbSolid &>(*obj);

      bool createParams = true;
      {
        int iCreateParams = createParams ? 1 : 0;
        GetInt( IDS_USE_PARAM_POINTS, iCreateParams );
        if ( iCreateParams == 0 )
          createParams = false;
      }
      MbStepData stepData;
      int mode = TestVariables::operateType;
      ::InitStepData( stepData, createParams, mode );
      SPtr<MbItem> meshItem( sol.CreateMesh( stepData, MbFormNote( false, true, true, TestVariables::exactMesh ), nullptr ) );
      if ( (meshItem != nullptr) && (meshItem->IsA() == st_Mesh) )
        mesh = &static_cast<MbMesh &>(*meshItem);
    }

    if ( (mesh != nullptr) && (mesh->GridsCount() > 0) ) {
      static bool deleteNormals = false;
      if ( ::RemoveRedundantPoints( *mesh, deleteNormals ) ) {
        MessageBoxEx( IDS_DONE, IDS_RESULT );
        TestVariables::viewManager->RefreshModel();
      }
    }

    if ( (coll != nullptr) && (coll->GetType() == MbCollection::coll_Tessellation) ) {
      static bool deleteNormals = false;
      if ( coll->RemoveRedundantPoints( deleteNormals ) ) {
        MessageBoxEx( IDS_DONE, IDS_RESULT );
        TestVariables::viewManager->RefreshModel();
      }
    }

  }
}


//------------------------------------------------------------------------------
// \ru Преобразовать объект в полигоны. \en Convert an object to the mesh. \~
// ---
void ConvertToMesh()
{
  MbItem * obj = nullptr;

  if ( GetObj( IDS_SELECT_GEOM_OBJECT, &obj ) ) {
    TestVariables::viewManager->ToggleObject( obj );

    if ( obj != nullptr && obj->IsA() != st_Mesh ) {
      SetWaitCursor( true );
      bool createParams = false;
      MbStepData stepData;
      int mode = TestVariables::operateType;
      ::InitStepData( stepData, createParams, mode );
      SPtr<MbMesh> mesh;

      SPtr<MbItem> meshItem( obj->CreateMesh( stepData, MbFormNote( false, true, true, TestVariables::exactMesh ), nullptr ) );

      if ( (meshItem != nullptr) && (meshItem->IsA() == st_Mesh) )
        mesh = &static_cast<MbMesh &>(*meshItem);

      if ( (mesh != nullptr) && (mesh->GridsCount() > 0) ) {
        Style st( obj->GetWidth(), obj->GetColor() );
        TestVariables::viewManager->DeleteObject( obj );
        TestVariables::viewManager->AddObject( st, mesh );
      }
      SetWaitCursor( false );
    }
    else {
      if ( obj != nullptr && obj->IsA() == st_Mesh ) {
        SetWaitCursor( true );
        SPtr<MbMesh> mesh;
        mesh = &static_cast<MbMesh &>(*obj);
        MbCollection * collection = new MbCollection( *mesh );

        if ( collection != nullptr ) {
          Style st( obj->GetWidth(), obj->GetColor() );
          TestVariables::viewManager->DeleteObject( obj );
          TestVariables::viewManager->AddObject( st, collection );
        }
        SetWaitCursor( false );
      }
    }
  }
}


//------------------------------------------------------------------------------
// Развалить полиглнальный объект на отдельные триангуляции.
// Break mesh to grids and delete it.
// ---
bool TranslateMeshToGrids()
{
  MbItem * meshItem = nullptr;

  if ( GetGObj( IDS_SELECT_MESH, st_Mesh, &meshItem ) && 
       meshItem != nullptr && meshItem->IsA() == st_Mesh ) {
    MbMesh * mesh = static_cast<MbMesh *>( meshItem );

    if ( mesh != nullptr ) {
      int width = meshItem->GetWidth();
      size_t gridsCount = mesh->GridsCount();

      TestVariables::viewManager->ToggleObject( meshItem );
      SetWaitCursor( true );

      if ( gridsCount > 0 ) {
        for ( size_t i = gridsCount; i--; ) {
          MbGrid * gr = mesh->DetachGrid( i );
          if ( gr != nullptr ) {
            MbMesh * mesh_i = new MbMesh( mesh->IsExact() );
            mesh_i->AddGrid( *gr );
            Style ns( width, TestVariables::colors[9+(i%5)] );
            TestVariables::viewManager->AddObject( ns, mesh_i );
          }
        }
        TestVariables::viewManager->DeleteObject( meshItem );
      }
      SetWaitCursor( false );
      TestVariables::viewManager->ShowModel();
    }
    return true;
  }
  return false;
}


//------------------------------------------------------------------------------
// Оставить в объекте только пластины триангуляции (0), ломаные (1), апексы (2).
// Keep in item only grids (0), polys (1), apexs (2).
// ---
bool KeepOnlyItems( int type )
{ 
  MbItem * meshItem = nullptr;

  if ( GetGObj( IDS_SELECT_MESH, st_Mesh, &meshItem ) && 
       meshItem != nullptr && meshItem->IsA() == st_Mesh ) {
    MbMesh * mesh = static_cast<MbMesh *>( meshItem );

    if ( mesh != nullptr ) {
      TestVariables::viewManager->ToggleObject( meshItem );
      SetWaitCursor( true );

      switch ( type ) {
        case 0 : {
          RPArray<MbPolygon3D> wires;  // \ru Множество указателей на полигоны. \en A set of pointers to polygons. 
          RPArray<MbApex3D>    peaks;  // \ru Множество указателей на апексы. \en A set of pointers to apexes. 
          mesh->DetachAllPolygons( wires );
          mesh->DetachAllApexes( peaks );
          ::DeleteItems( wires );
          ::DeleteItems( peaks );
        } break;
        case 1 : {
          RPArray<MbGrid>      grids;  // \ru Множество указателей на триангуляции. \en A set of pointers to triangulations. 
          RPArray<MbApex3D>    peaks;  // \ru Множество указателей на апексы. \en A set of pointers to apexes. 
          mesh->DetachAllGrids( grids );
          mesh->DetachAllApexes( peaks );
          ::DeleteItems( grids );
          ::DeleteItems( peaks );
        } break;
        case 2 : {
          RPArray<MbGrid>      grids;  // \ru Множество указателей на триангуляции. \en A set of pointers to triangulations. 
          RPArray<MbPolygon3D> wires;  // \ru Множество указателей на полигоны. \en A set of pointers to polygons. 
          mesh->DetachAllGrids( grids );
          mesh->DetachAllPolygons( wires );
          ::DeleteItems( grids );
          ::DeleteItems( wires );
        } break;
      }

      SetWaitCursor( false );
      TestVariables::viewManager->ToggleObject( meshItem );
      TestVariables::viewManager->RedrawObject( meshItem );
    }
    return true;
  }
  return false;
}


//------------------------------------------------------------------------------
// Заменить одинаковые меши на вставки.
// ---
void ConvertMeshToInstance()
{
  ItemsSPtrVector meshContainerOld;
  std::vector<MbMatrix3D> dummyMatr;
  ::ShowString( _T("Getting objects from the model") );
  TestVariables::viewManager->GetObjects( st_Mesh, meshContainerOld, dummyMatr );
  double accuracy = 0.05;
  bool absAcc = true;
  ::GetBool( IDS_ENTER_MESH2BREP_RELTOLERANCE, absAcc ); // use relative ?
  absAcc = !absAcc;
  ::GetDouble( IDS_ENTER_ACCURACY, accuracy );
  if ( accuracy < NULL_EPSILON )
    accuracy = Math::metricRegion;
  uint32 convertTime = ::GetTickCountEx();
  ::ShowString( _T("Converting meshes: ") );
  MbMeshToInstanceParams p ( accuracy, absAcc );
  MbMeshToInstanceResults convertRes;
  if ( rt_Success != ::ConvertMeshToInstance( meshContainerOld, p, convertRes ) || convertRes._nReplaced == 0 )
    return;
  ItemsSPtrVector & meshContainer( convertRes._meshContainer );
  convertTime = ::GetTickCountEx() - convertTime;

  size_t size = meshContainer.size();
  if ( size == meshContainerOld.size() ) // Должно быть всегда true.
  {
    DPtr<IProgressIndicator> progInd( &::CreateProgressIndicator( new StrSpyCommon ) );
    StrData<const TCHAR*> data (_T("Replacing meshes "));
    progInd->Initialize( size, 1, data);
    uint32 replaceTime = ::GetTickCountEx();
    int changed = 0;
    for ( size_t i = 0; i < size; ++i )
      if ( meshContainerOld[i] != meshContainer[i] ) // Заменяем объект только если он изменился.
      {
        #ifdef _DRAWGI
          DrawGI::DrawMesh( static_cast< MbMesh * >(meshContainerOld[i].get()), TRGB_PURPLE );
        #endif
        //TestVariables::viewManager->ReplaceItem( *meshContainerOld[i], *(meshContainer[i]), true ); // утечка памяти, нет параметра показа тела
        TestVariables::viewManager->DeleteObject( meshContainerOld[i].detach() );
        TestVariables::viewManager->AddObject( *(meshContainer[i].detach()), false ); // false нужен, т.к. иначе это долгая операция
        if ( ++changed % 5 == 0 )
          progInd->Progress( 5 );
      }
    replaceTime = ::GetTickCountEx() - replaceTime;
    TCHAR text[256];
    _sntprintf(text, 256, _T("number of changed objects: %d/%zu;\nconversion time: %u ms;\nreplacement time: %u ms;"),
               changed, size, convertTime, replaceTime );
    MessageBoxEx(text, _T("finished"));
  }
  else
    MessageBoxEx( _T("Error"), _T("Error") );
}


//------------------------------------------------------------------------------
// Упростить триангуляцию до заданного количества треугольников.
// ---
void SimplifyMeshTrianglesCount()
{
  MbItem * obj = nullptr;

  if ( GetGObj( IDS_SELECT_MESH, st_Mesh, &obj ) )
  {
    MbMesh * mesh = static_cast<MbMesh *>( obj );
    int nTrTarget = 0;
    for ( size_t i = 0, nGrid = mesh->GridsCount(); i < nGrid; ++i )
    {
      const auto * pGrid = mesh->GetGrid( i );
      if ( pGrid != nullptr )
        nTrTarget += static_cast<int>( pGrid->TrianglesCount() );
    }

    if ( GetInt( IDS_ENTER_NUMBER, nTrTarget ) )
    {
      c3d::IndicesVector counts;
      counts.push_back( nTrTarget );
      DPtr<IProgressIndicator> progBar( &::CreateProgressIndicator( new StrSpyMesh ) );
      MbMeshSimplificationParams params( counts, progBar );
      SimplifyTriangularMesh( params, *mesh );
      WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
      if ( wind != nullptr )
        wind->RedrawObject( mesh );
    }
  }
}


//------------------------------------------------------------------------------
// Упростить триангуляцию с заданной точностью.
// ---
void SimplifyMeshTolerance()
{
  MbItem * obj = nullptr;

  if ( GetGObj( IDS_SELECT_MESH, st_Mesh, &obj ) )
  {
    MbMesh * mesh = static_cast<MbMesh *>( obj );
    MbCube bb;
    mesh->AddYourGabaritTo( bb );
    double tol = bb.GetDiagonal() * 0.01;

    if ( GetDouble( IDS_ENTER_ACCURACY, tol ) )
    {
      c3d::DoubleVector  toleranceLevels;
      toleranceLevels.push_back( tol );
      DPtr<IProgressIndicator> progBar( &::CreateProgressIndicator( new StrSpyMesh ) );
      MbMeshSimplificationParams params( toleranceLevels, progBar );
      SimplifyTriangularMesh( params, *mesh );
      WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
      if ( wind != nullptr )
        wind->RedrawObject( mesh );
    }
  }
}


//------------------------------------------------------------------------------
// Совместить триангуляцию с другим объектом.
// ---
void AlignMeshWithObject()
{
  MbItem * itemFixed = nullptr;
  MbItem * itemMoving = nullptr;
  ptrdiff_t hintUsageModeVal = 0; // Соответствует MbeIcpHintUsageMode::noHint.
  ptrdiff_t pointRejectionModeVal = 1; // Соответствует MbeIcpPointRejectionMode::automatic.
  ptrdiff_t pointPairsMax = 10000;
  ptrdiff_t iterationMax = 10000;

  if ( GetObj( IDS_SELECT_FIXED_OBJECT, &itemFixed ) && ( itemFixed->IsA() == st_Mesh || itemFixed->IsA() == st_Solid ) ) {
    if ( GetGObj( IDS_SELECT_MOVING_MESH, st_Mesh, &itemMoving ) &&
         GetInt( IDS_HINT_USAGE_MODE, hintUsageModeVal ) &&
         GetInt( IDS_POINT_REJECTION_MODE, pointRejectionModeVal ) &&
         GetInt( IDS_POINT_PAIRS_MAX, pointPairsMax ) &&
         GetInt( IDS_ENTER_ITER_MAX_NUMB, iterationMax ) ) {
      MbeIcpHintUsageMode hintUsageMode( MbeIcpHintUsageMode::noHint );
      switch ( hintUsageModeVal ) {
        case 0: hintUsageMode = MbeIcpHintUsageMode::noHint; break;
        case 1: hintUsageMode = MbeIcpHintUsageMode::autoHint; break;
        case 2: hintUsageMode = MbeIcpHintUsageMode::givenHint; break;
      }
      MbeIcpPointRejectionMode pointRejectionMode( MbeIcpPointRejectionMode::automatic );
      switch ( pointRejectionModeVal ) {
        case 0: pointRejectionMode = MbeIcpPointRejectionMode::noReject; break;
        case 1: pointRejectionMode = MbeIcpPointRejectionMode::automatic; break;
        case 2: pointRejectionMode = MbeIcpPointRejectionMode::constant; break;
      }
      std::unique_ptr<IProgressIndicator> progBar( &::CreateProgressIndicator( new StrSpyMesh ) );
      MbObjectAlignmentParams params( hintUsageMode, progBar.get() );
      params.SetPointRejectionMode( pointRejectionMode );
      params.SetPointPairsMax( static_cast<size_t>( pointPairsMax ) );
      params.SetIterationMax( static_cast<size_t>( iterationMax ) );
      MbObjectAlignmentResult result;
      SetWaitCursor( true );
      AlignObjects( *itemFixed, *itemMoving, params, result );
      itemMoving->Transform( result.GetMatrix() );
      SetWaitCursor( false );
      WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
      if ( wind != nullptr )
        wind->RedrawObject( itemMoving );
      const size_t bufLen = 1024;
      TCHAR buf[bufLen];
      _sntprintf( buf, bufLen, _T( "Alignment error:  %e" ), result.GetAlignmentErrorValue() );
      ::MessageBoxEx( buf, IDS_RESULTS );
    }
  }
}


//------------------------------------------------------------------------------
// Создать полигональную сетку по объекту MbMesh. 
// ---
void CreatePolymeshByMesh()
{
  MbItem * obj = nullptr;

  if ( GetGObj( IDS_SELECT_MESH, st_Mesh, &obj ) ) {
    TestVariables::viewManager->ToggleObject( obj );

    MbMesh * mesh = static_cast<MbMesh *>( obj );
    if ( WorkWindow * wind = TestVariables::viewManager->GetActiveWindow() ) {
      auto & model = wind->SetGeomModel();
      MbAccurateTimer at;
      at.Begin();
      MbPolymeshTopo * pTopo = MbPolymeshTopo::Create( *mesh, 1e-8 );
      if ( pTopo != nullptr ) {
        for ( size_t e = 0, n = pTopo->EdgeSegmCount(); e < n; ++e ) {
          const auto & ed = pTopo->EdgeSegm( e );
          if ( ed.IsActive() ) {
            auto * pCont = pTopo->GetSegmEdgeModelAttributes( e, true );
            if ( pCont != nullptr )
              pCont->SetColor( 255, 0, 0 );
          }
        }
        MbPolymesh * pMesh = new MbPolymesh( pTopo );
        at.End();

        TestVariables::viewManager->DeleteObject( obj );
        model.AddItem( *pMesh );
        TestVariables::viewManager->RefreshModel();
      }
      else
        at.End();

      if ( TestVariables::checkOperationTime ) {
        const size_t bufLen = 1024;
        TCHAR buf[bufLen];
        size_t len = 0;
        len += (size_t)_sntprintf( buf + len, bufLen - len, RT_TIME_SEC1, at.GetLast() );
        ::MessageBoxEx( buf, IDS_RESULTS );
      }
    }

    TestVariables::viewManager->ToggleObject( obj );
  }
}


//------------------------------------------------------------------------------
// Обрезать полигональную сетку плоскостью.
// ---
void CutPolymeshByPlane()
{
  MbItem * obj = nullptr;
  PathMatrix surfacePathFrom;

  if ( GetGObj( IDS_SELECT_PLANE, st_Plane, &obj, surfacePathFrom ) ) {
    TestVariables::viewManager->ToggleObject( surfacePathFrom.first );
    bool res = false;
    MbPlacement3D place;
    if ( obj->IsA() == st_SpaceInstance ) {
      MbSpaceInstance * sInst = static_cast<MbSpaceInstance *>( obj );
      if ( sInst->GetSpaceItem() != nullptr ) {
        const MbSpaceItem * sItem = sInst->GetSpaceItem();
        if ( sItem->IsA() == st_Plane ) {
          const MbPlane * sPlane = static_cast<const MbPlane *>( sItem );
          place.Init( sPlane->GetPlacement() );
          if ( !surfacePathFrom.second.IsSingle() )
            place.Transform( surfacePathFrom.second );
          res = true;
        }
      }
    }

    if (res && GetGObj( IDS_SELECT_MESH, st_Polymesh, &obj ) ) {
      SetWaitCursor( true );

      MbPolymesh * pMesh = static_cast<MbPolymesh *>( obj );
      if ( pMesh != nullptr ) {
        if ( TestVariables::viewManager->GetActiveWindow() != nullptr )
        {
          const bool bTriangulate = GetBool( IDS_ENTER_MAKE_GRID, true );
          MbAccurateTimer at;
          at.Begin();
          const bool bCut = CutPolymeshByPlane( place, bTriangulate, *pMesh );
          at.End();
          if ( bCut )
            TestVariables::viewManager->RefreshModel();

          if ( TestVariables::checkOperationTime ) {
            const size_t bufLen = 1024;
            TCHAR buf[bufLen];
            size_t len = 0;
            len += (size_t)_sntprintf( buf + len, bufLen - len, RT_TIME_SEC1, at.GetLast() );
            ::MessageBoxEx( buf, IDS_RESULTS );
          }
        }
      }
      SetWaitCursor( false );
    }
  }
}


//------------------------------------------------------------------------------
// Булева операция с полигональными объектами. 
// ---
void MakePolymeshBoolean( size_t oper  )
{
  MbPolymeshBooleanParams::OpType type = MbPolymeshBooleanParams::OpType::opUnion;
  if( oper == 1 )
    type = MbPolymeshBooleanParams::OpType::opIntersection;
  else if ( oper == 2 )
    type = MbPolymeshBooleanParams::OpType::opSubtraction;

  MbItem * obj0 = nullptr;
  MbItem * obj1 = nullptr;
  if ( GetGObj( IDS_SELECT_MESH, st_Polymesh, &obj0 ) ) {
    TestVariables::viewManager->ToggleObject( obj0 );
    if ( GetGObj( IDS_SELECT_MESH, st_Polymesh, &obj1 ) ) {
      TestVariables::viewManager->ToggleObject( obj1 );
      MbPolymesh * pMesh0 = static_cast<MbPolymesh *>( obj0 );
      MbPolymesh * pMesh1 = static_cast<MbPolymesh *>( obj1 );

      MbPolymeshBooleanParams params( type, METRIC_EPSILON );
      MbPolymeshBooleanResults results;
      MbAccurateTimer ac;

      SetWaitCursor( true );
      ac.Begin();

      MbResultType res = PolymeshBoolean( params, results, *pMesh0, *pMesh1 );

      ac.End();
      SetWaitCursor( false );

      if ( res == rt_Success ) {
        TestVariables::viewManager->DeleteObject( obj1 );
        TestVariables::viewManager->RefreshModel();
        TestVariables::viewManager->ToggleObject( obj0 );
      }
      else {
        ::PutErrorMessage( res, _T( "Operation error" ) );
        TestVariables::viewManager->ToggleObject( obj1 );
        TestVariables::viewManager->ToggleObject( obj0 );
      }

      if ( TestVariables::checkOperationTime ) {
        const size_t bufLen = 1024;
        TCHAR buf[bufLen];
        _sntprintf( buf, bufLen, RT_TIME_SEC1, ac.GetLast() );
        ::MessageBoxEx( buf, IDS_RESULTS );
      }
    }
  }
}


//------------------------------------------------------------------------------
// Драпировать оболочку.
// ---
void DrapeShell()
{
  MbItem * solid = nullptr;

  // Запросить исходное тело
  // Select an initial solid.
  auto * pWnd = TestVariables::viewManager->GetActiveWindow();
  if ( pWnd != nullptr ) {
    if ( ::GetGObj( IDS_SELECT_SOLID, st_Solid, &solid, TestVariables::itemPathFrom ) ) {

      TestVariables::viewManager->ToggleObject( solid );
      const MbSolid & sourceSolid = *(MbSolid *)solid;

      MbCartPoint3D point;
      if ( GetPoint( IDS_SELECT_POINT, point ) ) {

        MbCartPoint3D center;
        double size = CalculateAreaCentre( *sourceSolid.GetShell(), Math::deviateSag, center );
        const double lenMax = ::sqrt( size * 0.01 );
        size *= 0.001;
        size = ::sqrt( size );
        int angFiberType = 0;
        if ( GetDouble( IDS_ENTER_DISTANCE, size ) && GetInt( IDS_ENTER_FIBER_ANG_TYPE, angFiberType ) ) {
          
          MbStepData stepData;
          int mode = TestVariables::operateType;
          ::InitStepData( stepData, false, mode );
          stepData.SetLength( lenMax );

          size_t iF;
          double u, v;
          sourceSolid.GetShell()->NearPointProjection( point, iF, u, v );
          MbCartPoint3D org;
          MbVector3D normal;
          const auto * pFace = sourceSolid.GetShell()->GetFace( iF );
          if ( pFace != nullptr ) {
            pFace->PointOn( u, v, org );
            pFace->Normal( u, v, normal );
            MbVector3D axisX( pWnd->GetAxisX().NormalComponent( normal ) );
            MbPlacement3D place( org, normal, axisX );

            MbDrapeShellParams::AngleFromCurveType type = MbDrapeShellParams::AngleFromCurveType::degrees0;
            if ( angFiberType == 1 )
              type = MbDrapeShellParams::AngleFromCurveType::degreesPlus45;
            else if ( angFiberType == 2 )
              type = MbDrapeShellParams::AngleFromCurveType::degreesMinus45;
            MbDrapeShellParams params( *sourceSolid.GetShell(), stepData );
            params.Init( place, MbDrapeShellParams::ConstraintCurveType::geodesic, type, size );
            std::unique_ptr<IProgressIndicator> progBar( &::CreateProgressIndicator( new StrSpyMesh ) );
            params._progress = progBar.get();
            MbDrapeShellResults results;
            if ( DrapeShell( params, results ) == rt_Success ) {
              const auto & halfedge = results.GetMesh()->GetTopo()->GetMeshTopo();
              
              const double angMax = 30. * M_DEGRAD;
              const double angWarning = angMax * c3d::ONE_HALF;
              std::vector<std::vector<SpacePointsVector>> colored( 3 );
              for ( const auto & ed : halfedge.AllActiveEdges() ) {
                const size_t id = results.GetSideId( ed.Id() );
                if ( id < SYS_MAX_T ) {
                  const double val = results.GetEdgeAngle( ed.Id() );
                  if ( val >= 0. ) {
                    if ( val < angWarning )
                      colored[0].emplace_back( results.GetCellSides()[id] );
                    else if ( val < angMax )
                      colored[1].emplace_back( results.GetCellSides()[id] );
                    else
                      colored[2].emplace_back( results.GetCellSides()[id] );
                  }
                }
              }

              for ( size_t s = 0; s < 3; ++s ) {
                const auto & vec = colored[s];
                if ( !vec.empty() ) {
                  MbWireFrame * pW = new MbWireFrame;
                  for ( const auto & side : vec ) {
                    MbPolyline3D * pPoly = new MbPolyline3D( side, false );
                    MbEdge * pEdge0 = new MbEdge( *pPoly, true );
                    pW->AddEdge( *pEdge0, true );
                  }

                  if ( s == 0 )
                    pW->SetColor( 0, 0, 255 );
                  else if ( s == 1 )
                    pW->SetColor( 255, 255, 0 );
                  else
                    pW->SetColor( 255, 0, 0 );
                  TestVariables::viewManager->AddObject( *pW );
                }
              }

              double tolApprox = size * 0.1;
              FaceSPtr unwrapped;
              results.GetUnwrapped( tolApprox, unwrapped );
              if ( unwrapped != nullptr ) {
                MbFaceShell * pShell = new MbFaceShell( *unwrapped );
                MbSolid * pSolid = new MbSolid( pShell, nullptr );
                MbMatrix3D matr;
                place.GetMatrixFrom( matr );
                pSolid->Transform( matr );
                TestVariables::viewManager->AddObject( *pSolid );
              }

              std::vector<c3d::SpacePointsVector> polys;
              MbCartPoint uv;
              const double shift = 3;
              uv.Add( MbVector( 1., 0 ), shift * size );
              const MbVector dir( 0., 1 );
              results.GetSplitPolylines( uv, dir, polys );
              if ( !polys.empty() ) {
                MbWireFrame * pW = new MbWireFrame;
                for ( const auto & poly : polys ) {
                  MbPolyline3D * pPoly = new MbPolyline3D( poly, false );
                  MbEdge * pEdge0 = new MbEdge( *pPoly, true );
                  pW->AddEdge( *pEdge0, true );
                }
                pW->SetColor( 255, 0, 0 );
                pW->SetWidth( 4 );
                TestVariables::viewManager->AddObject( *pW );

                if ( !results.GetWarp()[0].empty() && !results.GetWarp()[1].empty() ) {
                  pW = new MbWireFrame;
                  MbPolyline3D * pPoly = new MbPolyline3D( results.GetWarp()[0], false );
                  MbEdge * pEdge0 = new MbEdge( *pPoly, true );
                  pW->AddEdge( *pEdge0, true );
                  pPoly = new MbPolyline3D( results.GetWarp()[1], false );
                  pEdge0 = new MbEdge( *pPoly, true );
                  pW->AddEdge( *pEdge0, true );
                  pW->SetColor( 0, 255, 0 );
                  pW->SetWidth( 6 );
                  TestVariables::viewManager->AddObject( *pW );
                }
              }
            }
          }
        }
      }
      TestVariables::viewManager->ToggleObject( solid );
    }
  }
}


//------------------------------------------------------------------------------
// Ремешировать полигональный объект.
// ---
void RemeshPolymesh()
{
  MbItem * obj = nullptr;

  if ( GetGObj( IDS_SELECT_MESH, st_Polymesh, &obj ) ) {
    MbPolymesh * pMesh = static_cast<MbPolymesh *>( obj );
    if ( pMesh != nullptr ) {
      if ( WorkWindow * wind = TestVariables::viewManager->GetActiveWindow() ) {
        int remeshMethod = 1;
        double value = 0.;
        remeshMethod = 1;
        ::GetInt( IDS_REMESH_METHOD, remeshMethod );
        if ( remeshMethod == 2 ) {
          value = 0.01;
          GetDouble( IDS_REMESH_TOLERANCE, value );
        }
        else {
          MbCube bb;
          pMesh->AddYourGabaritTo( bb );
          value = bb.GetDiagonal() * 0.01;
          GetDouble( IDS_REMESH_UNIFORM_SIZE, value );
        }

        double angEdge = -1.;
        if ( pMesh->GetSubstrateShell() == nullptr )
          GetDouble( IDS_ENTER_ANGLE, angEdge );

        SetWaitCursor( true );
        std::unique_ptr<IProgressIndicator> progBar( &::CreateProgressIndicator( new StrSpyMesh ) );
        MbRemeshParams params( progBar.get() );
        if ( remeshMethod == 2 ) {
          auto * pAttrCurv = pMesh->GetTopo()->GetMeshHalfedgeAttribute<MbHalfedgeAttrCurvaturesVertex>();
          if ( pAttrCurv == nullptr ) {
            MbMeshCurvatureParams paramCurv;
            paramCurv.InitPolynomMethod( 3 );
            CalculatePolymeshCurvatures( paramCurv, *pMesh );
          }
          params.InitIncrementalCurvature( value, c3d::IndicesVector(), angEdge );
        }
        else
          params.InitIncrementalUniform( value, c3d::IndicesVector(), angEdge );
        MbAccurateTimer at;
        at.Begin();
        auto res = Remesh( params, *pMesh );
        at.End();

        if ( res == rt_Success ) {
          TestVariables::viewManager->RefreshModel();
          const size_t bufLen = 1024;
          TCHAR buf[bufLen];
          size_t len = 0;
          len += (size_t)_sntprintf( buf + len, bufLen - len, RT_TIME_SEC1, at.GetLast() );
          ::MessageBoxEx( buf, IDS_RESULTS );
        }
        SetWaitCursor( false );
      }
    }
  }
}


//------------------------------------------------------------------------------
// Сгладить полигональную сетку.
// ---
void SmoothPolymesh()
{
  MbItem * obj = nullptr;

  if ( GetGObj( IDS_SELECT_MESH, st_Polymesh, &obj ) ) {
    MbPolymesh * pMesh = static_cast<MbPolymesh *>( obj );
    if ( pMesh != nullptr ) {
      if ( WorkWindow * wind = TestVariables::viewManager->GetActiveWindow() ) {
        int smoothMethod = 0;
        ::GetInt( IDS_SMOOTH_METHOD, smoothMethod );

        double lambda = 0.33;
        ::GetDouble( IDS_SMOOTH_LAMBDA, lambda );

        double mu = -0.34;
        if ( smoothMethod == 0 )
          ::GetDouble( IDS_SMOOTH_MU, mu );

        int nIter = 100;
        ::GetInt( IDS_SMOOTH_ITER, nIter );

        bool bSaveBorders{ true };
        ::GetBool( IDS_SMOOTH_SAVE_BORDERS, bSaveBorders );

        SetWaitCursor( true );

        MbMeshSmoothingParams params( static_cast<MbMeshSmoothingParams::MethodSmoothMesh>( smoothMethod ), lambda, mu, nIter, bSaveBorders, {}, {} );

        const auto res = SmoothPolymesh( params, *pMesh );

        if ( res == rt_Success )
          TestVariables::viewManager->RefreshModel();

        SetWaitCursor( false );
      }
    }
  }
}


//------------------------------------------------------------------------------
// Рассчитать нормали в вершинах сетки. 
// ---
void CalculateMeshNormals()
{
  MbItem * obj = nullptr;

  if ( GetGObj( IDS_SELECT_MESH, st_Polymesh, &obj ) ) {
    MbPolymesh * pMesh = static_cast<MbPolymesh *>( obj );
    if ( pMesh != nullptr ) {
      if ( WorkWindow * wind = TestVariables::viewManager->GetActiveWindow() ) {
        bool bSegmentation = ::GetBool( IDS_SEGMENTATION_ACCOUNTING, true );

        SetWaitCursor( true );

        MbPolymeshNormalsParams params( bSegmentation );
        const auto res = CalculatePolymeshNormals( params, *pMesh );
        if ( res == rt_Success )
          TestVariables::viewManager->RefreshModel();

        SetWaitCursor( false );
      }
    }
  }
}


//------------------------------------------------------------------------------
// Создать полигональный объект из оболочки.
// ---
void CreatePolymeshBySolid()
{
  MbItem * solid = nullptr;

  // Запросить исходное тело
  // Select an initial solid.
  auto * pWnd = TestVariables::viewManager->GetActiveWindow();
  if ( pWnd != nullptr ) {
    if ( ::GetGObj( IDS_SELECT_SOLID, st_Solid, &solid, TestVariables::itemPathFrom ) ) {

      TestVariables::viewManager->ToggleObject( solid );
      const MbSolid & sourceSolid = *(MbSolid *)solid;
      MbStepData stepData;
      int mode = TestVariables::operateType;
      
      if ( ::InitStepData( stepData, false, mode ) ) {
        bool bKeepEdges = ::GetBool( IDS_KEEP_EDGES, true );
        bool bRemesh = ::GetBool( IDS_NEED_REMESH, false );
        int remeshMethod = 0;
        std::vector<c3d::IndicesVector> faceGroups;
        double value = 0.;
        if ( bRemesh ) {
          remeshMethod = 1;
          ::GetInt( IDS_REMESH_METHOD, remeshMethod );
          if ( remeshMethod == 2 ) {
            value = 0.01;
            GetDouble( IDS_REMESH_TOLERANCE, value );
          }
          else {
            MbCartPoint3D center;
            value = CalculateAreaCentre( *sourceSolid.GetShell(), Math::deviateSag, center );
            value *= 0.001;
            value = ::sqrt( value );
            GetDouble( IDS_REMESH_UNIFORM_SIZE, value );
          }
        }

        if ( !bKeepEdges ) {
          const size_t nFace = sourceSolid.GetShell()->GetFacesCount();
          faceGroups.resize( 1 );
          auto & vec = faceGroups[0];
          vec.resize( nFace );
          for ( size_t i = 0; i < nFace; ++i )
            vec[i] = i;
        }

        SetWaitCursor( true );
        SPtr<MbPolymesh> pPoly( new MbPolymesh() );
        IndicesVector fixedEdges;
        MbShellLinearizationParams paramsLin( stepData, bRemesh, false, remeshMethod == 2, faceGroups, fixedEdges );
        MbShellLinearizationResults results;
        auto res = CreatePolymeshByShell( paramsLin, *sourceSolid.GetShell(), *pPoly, results );

        if ( bRemesh && res == rt_Success ) {
          std::unique_ptr<IProgressIndicator> progBar( &::CreateProgressIndicator( new StrSpyMesh ) );
          MbRemeshParams params( progBar.get() );
          if ( remeshMethod == 2 )
            params.InitIncrementalCurvature( value, c3d::IndicesVector() );
          else
            params.InitIncrementalUniform( value, c3d::IndicesVector() );
          res = Remesh( params, *pPoly );
          pPoly->DeleteSubstrateShell();
        }

        if ( res == rt_Success ) {
          TestVariables::viewManager->DeleteObject( solid );
          TestVariables::viewManager->AddObject( *pPoly );
        }

        SetWaitCursor( false );
      }
    }
  }
}


//------------------------------------------------------------------------------
// Обрезать полигональную сетку кривыми.
// ---
void CutPolymeshByCurves()
{
  MbPolymesh * pMesh = SelectPolymesh();
  if ( pMesh == nullptr )
    return;

  SpaceCurvesSPtrVector crvs;
  IndicesVector types;
  MbItem * object1;
  while ( GetAnyCurve( IDS_SELECT_CURVE, &object1 ) ) {
    int type = 1;
    GetInt( IDS_POLY_CURVE_CUT_MODE, type );
    TestVariables::viewManager->ToggleObject( object1 );
    crvs.push_back( ::GetCurve3D( object1, true ) );
    types.push_back( type );
  }

  if ( !crvs.empty() ) {
    MbStepData stepData;
    int mode = TestVariables::operateType;
    ::InitStepData( stepData, false, mode );

    MbCurvePolymeshProjectParams params;
    for ( size_t i = 0; i < types.size(); ++i ) {
      const size_t t = types[i];
      uint val = 0;
      if ( t >= 0 && t < 4 )
        val = static_cast<uint>( t );
      ProjProcessingMethod type = static_cast<ProjProcessingMethod>( val );
      params.AddCurve( crvs[i], type );
    }

    MbPolymeshProjectResults results;
    CutPolymeshByCurves( params, results, *pMesh );

    TestVariables::viewManager->RefreshModel();
  }
}
