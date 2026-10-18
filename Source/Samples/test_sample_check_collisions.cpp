//////////////////////////////////////////////////////////////////////////////////////////
/*
  The file contains a code demonstrating how to apply collision detection utility to 
  show interferencies of solids grouped into components of the assembly. Each component 
  can contain  a sub-hierarchy of solids and subassemblies. In the sample, collisions 
  between solids from different components are searched for, and checks between solids of
  the same component are skipped.
*/
//////////////////////////////////////////////////////////////////////////////////////////

#include <test_samples.h>
//
#include <test_manager.h>
#include <test_service.h>
#include <test_variables.h>
#include <alg_draw.h>
#include <solid.h>
#include <instance_item.h>
#include <mb_class_traits.h>
#include <cdet_utility.h>
#include <last.h>

using namespace c3d;

#ifndef _UNUSED_VAR
#define _UNUSED_VAR( var ) (std::ignore = var)
#endif

//----------------------------------------------------------------------------------------
// Hierarhical representation of C3D geometric model.
/*  
  components_tree provides for tree-representation of instancies of reusable solids 
  in nested assemblies.
*/
//---
class components_tree
{
public:
  class component: public MtRefItem 
  {
  public:
    component( const MbItem * item ) 
      : MtRefItem()
      , parent( nullptr )
      , item( item )
    {}

  public:
    const component * Parent() const { return parent; }    
    void              SetParent( const component * p ) { parent = p; }
    // The model item which is represented by the component instance.
    const MbItem *    GetGeomModel() const { return item; }
    // Local coordinate system positioning the item in the world.
    MbPlacement3D     PlacementToWorld() const;

  private:
    const component *   parent;
    SPtr<const MbItem>  item;
    OBVIOUS_PRIVATE_COPY( component );
  };

  typedef std::vector<SPtr<component>>    components_list;
  typedef components_list::const_iterator components_iterator;

public:
  components_tree( const MbItem * root )
    : m_root( nullptr )
    , m_comps()
  {
    m_root = _Build( root );
  }
  
  void GetComponents( components_iterator & iter, components_iterator & last )
  { 
    iter = m_comps.begin(); last = m_comps.end(); 
  }
  const component * Root() const { return m_root; }

private:
  component * _Build( const MbItem* item )
  {  
    SPtr<component> comp( new component(item) );
    m_comps.push_back( comp );

    if ( const MbAssembly * ass = isa_cast<const MbAssembly*>(item) )
    {
      for ( std::size_t i = 0, items_count = ass->ItemsCount(); i < items_count; ++i )
      {        
        if ( component * subComp = _Build(ass->GetItem(i)) )
        {
          subComp->SetParent( comp );
        }
      }
    }
    else if ( const MbInstance * inst = isa_cast<const MbInstance*>(item) )
    {
      if ( component * subComp = _Build(inst->GetItem()) )
      {
        subComp->SetParent( comp );
      }
    }
    return comp;
  }

private:
  const component * m_root;
  components_list   m_comps;
  
  OBVIOUS_PRIVATE_COPY( components_tree );
};

//----------------------------------------------------------------------------------------
// Local coordinate system positioning the item in The World.
//---
MbPlacement3D components_tree::component::PlacementToWorld() const
{      
  MbPlacement3D lcs;
  item->GetPlacement( lcs );
  if ( parent )
  {
    lcs.Transform( parent->PlacementToWorld().GetMatrixFrom() );
  }
  return lcs;
}


typedef components_tree::component Component;

//----------------------------------------------------------------------------------------
// The structure of the query to find collision faces.
//---
struct cdet_collided_faces_and_app_items: public cdet_query
{ 
  typedef std::pair<cdet_app_item,const MbRefItem*> item_face_t;  // Represents a face of component.
  typedef std::pair<cdet_app_item,cdet_app_item>    items_pair_t; 
  typedef std::set<item_face_t>   collided_faces_t;
  typedef std::set<items_pair_t>  collided_app_items_t;
  
  collided_faces_t      collided_faces;
  collided_app_items_t  collided_app_items;
  cdet_app_item         root; // The main assembly.

public:
  cdet_collided_faces_and_app_items()
    : cdet_query( _QueryFunc )
    , collided_faces()
    , collided_app_items()
    , root( nullptr )
  {}
  
  void Reset()
  {
    collided_faces.clear();
    collided_app_items.clear();
  }

private:
  static cback_res _QueryFunc( cdet_query * query, message code, cback_data & cData );

  // Component of the main assembly including a given 'appItem' (top sub item the assembly in the component tree).
  cdet_app_item _Parent( cdet_app_item appItem ) const
  {
    const Component * comp = static_cast<const Component *>( appItem );
    while ( comp && (comp->Parent() != root) )
    {
      comp = comp->Parent();
    }
    return comp;   
  }

  bool  _SameGroups( cdet_app_item fst, cdet_app_item snd ) const
  {    
    return _Parent( fst ) == _Parent( snd );
  }

  OBVIOUS_PRIVATE_COPY( cdet_collided_faces_and_app_items );
};

//----------------------------------------------------------------------------------------
//
//---
cdet_query::cback_res cdet_collided_faces_and_app_items::_QueryFunc( cdet_query* query, message code, cback_data& cData )
{
  if ( cdet_collided_faces_and_app_items * q = static_cast<cdet_collided_faces_and_app_items*>(query) )
  {
    switch ( code )
    {
    case CDET_QUERY_STARTED:
    {
      q->collided_faces.clear();
      q->collided_app_items.clear();
      return CBACK_VOID;
    }
    case CDET_STARTED:
    {
      if ( q->_SameGroups(cData.first.appItem, cData.second.appItem) )
      {
        return CBACK_SKIP;
      }
      return CBACK_VOID;
    }

    case CDET_FINISHED: // a pair of solids was finished.
      return CBACK_SEARCH_MORE;

    case CDET_INTERSECTED:
    {
      if ( cData.first.refItem )
      {
        q->collided_faces.insert( item_face_t(cData.first.appItem, cData.first.refItem) );
      }
      if ( cData.second.refItem )
      {
        q->collided_faces.insert( item_face_t(cData.second.appItem, cData.second.refItem) );
      }

      if ( cData.first.appItem && cData.second.appItem )
      {
        q->collided_app_items.insert( std::make_pair(cData.first.appItem, cData.second.appItem) );
      }

      return CBACK_SEARCH_MORE;
    }
    default:
      return CBACK_VOID;
    }
  }
  return CBACK_VOID;
}


//----------------------------------------------------------------------------------------
// Wrapper of the collision detection utility
//---
class CollisionDetector
{
  CdCollisionDetection m_cdet;
  cdet_collided_faces_and_app_items m_col_query;  

public:
  CollisionDetector( const components_tree & gTree )
    : m_cdet()
    , m_col_query()
  {
    m_col_query.root = gTree.Root();
  }

public:
  // Add a component to the analisys.
  void        AddComponent( Component * comp );
  // Calculate collision between components.
  cdet_result CheckCollisions();

  OBVIOUS_PRIVATE_COPY( CollisionDetector );
};

//----------------------------------------------------------------------------------------
// Add a component to the analisys.
//---
void CollisionDetector::AddComponent( Component * comp )
{
  if ( const MbSolid * solid = isa_cast<const MbSolid*>(comp->GetGeomModel()) )
  {
    cdet_item cdetItem = m_cdet.AddItem( *solid, comp->PlacementToWorld(), comp );
    PRECONDITION( cdetItem );
    _UNUSED_VAR( cdetItem );
  }
}

//----------------------------------------------------------------------------------------
// Highlight faces involved in the collisions.
//---
template<class _Iter>
void DrawCollisionFaces( _Iter iter, _Iter last )
{
  for ( ; iter!=last; ++iter )
  {
    if ( const Component * comp = static_cast<const Component*>(iter->first) )
    {
      const MbMatrix3D from = comp->PlacementToWorld().GetMatrixFrom();
      // Highlight a face involved in the collision.
      TestVariables::drawImp->DrawItem( isa_cast<const MbFace*>(iter->second), TRGB_ORANGE, from );
    }
  }
}

//----------------------------------------------------------------------------------------
// Calculate collision between components.
//---
cdet_result CollisionDetector::CheckCollisions()
{
  cdet_result res = m_cdet.CheckCollisions( m_col_query );
  if ( res == CDM_RESULT_CollisionDetected )
  { 
    DrawCollisionFaces( m_col_query.collided_faces.begin(), m_col_query.collided_faces.end() );
  }
  return res;
}

//----------------------------------------------------------------------------------------
// Get an assembly from model items sequance, the first founded.
//---
template<class _ItemIter>
_ItemIter FirstAssembly( _ItemIter iter, const _ItemIter last )
{
  for ( ; iter != last; ++iter )
  {
    if ( (*iter)->IsA() == st_Assembly )
    {
      return iter;
    }
  }
  return last;
}

//----------------------------------------------------------------------------------------
// Demonstrating sample: check collisions between instancies of an assembly.
/*
  This sample demonstrates how to apply collision detection utility to show interferencies 
  of solids grouped into components  of the assembly.  Each component can contain  a 
  sub-hierarchy of solids and subassemblies. In the sample, collisions between solids from 
  different components are searched for,  and checks between solids of the same component
  are skipped.
*/
// ---
void SampleCheckCollisions()
{  
  if ( const WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow() )
  {    
    const MbModel & gModel = wnd->GetGeomModel();
    // The main assembly is at top level of the model hierarhy.
    MbModel::ItemConstIterator mainAssembly = FirstAssembly( gModel.CBegin(), gModel.CEnd() );
    if ( mainAssembly == gModel.CEnd() )
    {
      MessageBoxEx( _T("The model has no assemblies."), _T("Check collisions") );
      return;
    }
    size_t compNb = 0; // The counter of component instancies in the main assembly.
    components_tree gModelTree( *mainAssembly );
    CollisionDetector cdet( gModelTree );
    components_tree::components_iterator iter, last;
    for ( gModelTree.GetComponents(iter,last); iter!=last; ++iter )
    { 
      cdet.AddComponent( *iter );
      if ( (*iter)->Parent() == gModelTree.Root() )
        ++compNb;
    }
    if ( compNb > 1 )
      cdet.CheckCollisions();      
    else
      MessageBoxEx( _T("The main assembly must have two or more components (sub-items)!"), _T("Check collisions") );
  }
}

// eof