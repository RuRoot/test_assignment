//////////////////////////////////////////////////////////////////////////////////////////
//
//    The file contains source codes demonstrating samples how to work with polymesh.
//
//    WARNING: This code is only an example. The real application can require more 
//             functionality and data types for implementation.
// 
//////////////////////////////////////////////////////////////////////////////////////////

#include <test_samples.h>
#include <test_variables.h>
#include <test_manager.h>
#include <test_service.h>
//
#include <action_polymesh.h>
#include <action_shell.h>
#include <polymesh.h>
#include <polymesh_topo.h>
#include <halfedge_iterators.h>
#include <tuple>
#include <surf_plane.h>
#include <op_polymesh_parameter.h>

#include <last.h>

using namespace c3d;


//------------------------------------------------------------------------------
// \ru Создать икосаэдр. \en Create icosahedron.
// ---
static
void CreateIcosahedron( SPtr<MbPolymesh> & icosahedron )
{
  MbPolymeshTopo * pTopo = MbPolymeshTopo::Create();
  if ( pTopo == nullptr )
    return;

  const double scale = 100.;
  const double c0 = 0.809 * scale;
  const double c1 = 0.5 * scale;
  const double c2 = 0.588 * scale;
  const double c3 = 0.309 * scale;
  const double c4 = 0.951 * scale;
  const double c5 = 1. * scale;
  const double c6 = 1.118 * scale;

  SpacePointsVector points;
  points.emplace_back( c0, c1, c2 );
  points.emplace_back( c3, -c1, c4 );
  points.emplace_back( -c3, c1, c4 );
  points.emplace_back( -c0, -c1, c2 );
  points.emplace_back( -c5, c1, 0 );
  points.emplace_back( -c0, -c1, -c2 );
  points.emplace_back( -c3, c1, -c4 );
  points.emplace_back( c3, -c1, -c4 );
  points.emplace_back( c0, c1, -c2 );
  points.emplace_back( c5, -c1, 0 );
  points.emplace_back( 0, c6, 0 );
  points.emplace_back( 0, -c6, 0 );

  std::vector<std::tuple<size_t, size_t, size_t>> ids;
  ids.reserve( 20 );
  ids.emplace_back( 0, 10, 2 );
  ids.emplace_back( 2, 10, 4 );
  ids.emplace_back( 4, 10, 6 );
  ids.emplace_back( 6, 10, 8 );
  ids.emplace_back( 8, 10, 0 );
  ids.emplace_back( 0, 2, 1 );
  ids.emplace_back( 1, 2, 3 );
  ids.emplace_back( 2, 4, 3 );
  ids.emplace_back( 3, 4, 5 );
  ids.emplace_back( 4, 6, 5 );
  ids.emplace_back( 5, 6, 7 );
  ids.emplace_back( 6, 8, 7 );
  ids.emplace_back( 7, 8, 9 );
  ids.emplace_back( 8, 0, 9 );
  ids.emplace_back( 9, 0, 1 );
  ids.emplace_back( 1, 11, 9 );
  ids.emplace_back( 3, 11, 1 );
  ids.emplace_back( 5, 11, 3 );
  ids.emplace_back( 7, 11, 5 );
  ids.emplace_back( 9, 11, 7 );

  // Add polymesh vertices.
  MbHalfedgeAttrPoint3D * pAttrVx = pTopo->AddMeshHalfedgeAttribute<MbHalfedgeAttrPoint3D>();
  pTopo->Reserve( points.size(), ids.size() * 3, ids.size() );
  for ( size_t i = 0; i < points.size(); ++i )
    pAttrVx->SetPoint( pTopo->AddVertex(), points[i] );

  // Add polymesh facets (triangles).
  IndicesVector aux( 3 );
  for ( size_t i = 0; i < ids.size(); ++i )
  {
    const auto & vec = ids[i];
    aux[0] = std::get<0>(vec);
    aux[1] = std::get<1>(vec);
    aux[2] = std::get<2>( vec );
    pTopo->AddFacet( aux );
  }

  pTopo->SewByVertices(); // Sew by internal function.
  pTopo->Initialize(); // Initialize simple segmentation.

  icosahedron = new MbPolymesh( pTopo );
}


//------------------------------------------------------------------------------
// \ru Создать додэкаэдр, дуальный икосаэдру. \en Create dodecahedron dual to the given icosahedron.
// ---
static
void CreateDodecahedronByIcosahedron( SPtr<MbPolymesh> & icosahedron, SPtr<MbPolymesh> & dodecahedron )
{
  // Dodecahadron and Icosahedron are dual polyhedrons which means that every vertex of one polyhedron can be uniquilely defined to a facet of dual polyhedron.
  // This property is used to show some examples how to build new topology based on existing one.

  MbPolymeshTopo * pTopo1 = MbPolymeshTopo::Create();
  if ( pTopo1 == nullptr )
    return;

  MbPolymeshTopo * pTopo = icosahedron->SetTopo();
  const MbHalfedgeAttrPoint3D * pAttrVx = pTopo->GetMeshHalfedgeAttribute<MbHalfedgeAttrPoint3D>(); // Get icosahedron's 3D points.
  MbHalfedgeAttrPoint3D * pAttrVx1 = pTopo1->AddMeshHalfedgeAttribute<MbHalfedgeAttrPoint3D>(); // Create dodecahedron's 3D points.
  pTopo1->Reserve( 20, 60, 12 );

  // Add temporary icosahedron's facet atribute to store ID of the dodecahedron's dual vertex to this facet.
  MbHalfedgeAttrSizetFacet * pAttrF = pTopo->AddMeshHalfedgeAttribute<MbHalfedgeAttrSizetFacet>();
  pAttrF->SetSize( pTopo->GetMeshTopo().FacetCount() );
  for ( size_t i = 0; i < pTopo->GetMeshTopo().FacetCount(); ++i )
  {
    MbCartPoint3D center;
    if ( pAttrVx->GetTriangleCenter( i, center ) )
    {
      const auto v = pTopo1->AddVertex();
      pAttrVx1->SetPoint( v, center );
      pAttrF->SetValue( i, v ); // Store dodecahedron vertex ID to the icosahedron facet attribute.
    }
  }
  
  // Add temporary icosahedron's edge attribute to store ID of dodecahedron's dual edge.
  MbHalfedgeAttrSizetEdge * pAttrE = pTopo->AddMeshHalfedgeAttribute<MbHalfedgeAttrSizetEdge>();
  pAttrE->SetSize( pTopo->GetMeshTopo().EdgeCount() );
  IndicesVector vertices( 5 );
  IndicesVector edges( 5 );
  for ( size_t i = 0; i < pTopo->GetMeshTopo().VertexCount(); ++i )
  {
    // Create dodecahedron facet loop from vertex indices (vertices array).
    size_t ind = 0;
    for ( EdgeFanIterator it( pTopo->GetMeshTopo(), i, true ); !it.Done(); ++it )
    {
      const auto e = it.EdgePair().first; // Icosahedron edge ID (outgoing from the vertex).
      const auto & ed = pTopo->GetMeshTopo().Edge( e ); // Icosahedron edge.
      const auto v = pAttrF->GetValue( ed.Facet() ); // Dodecahedron vertex ID for this facet.
      vertices[ind] = v;
      edges[ind] = e; // Store icosahedron fan edges.
      ++ind;
    }

    const auto f = pTopo1->AddFacet( vertices ); // Create new dodecahedron's facet (pentagon)
    ind = 0;
    for ( EdgeFacetIterator it( pTopo1->GetMeshTopo(), f ); !it.Done(); ++it )
    {
      pAttrE->SetValue( edges[ind], it.Edge() ); // Store dodecahedron edge ID to the icosahedron edge attribute.
      ++ind;
    }
  }

  // Sew dodecahedron (restore twin information).
  for ( size_t i = 0; i < pTopo->GetMeshTopo().EdgeCount(); ++i ) // Iterate icosahedron edges.
  {
    const auto & edNext = pTopo->GetMeshTopo().Edge( pTopo->GetMeshTopo().NextFanEdge( i, false ) );
    const auto e1 = pAttrE->GetValue( i ); // Dodecahedron edge ID associated with this icosahedron edge.
    const auto & ed1 = pTopo1->GetMeshTopo().Edge( e1 );
    if ( !ed1.IsTwinDefined() )
    {
      const auto t1 = pAttrE->GetValue( edNext.Next() ); // Twin ID found by the icosahedron topology and the information stored in the auxilliary attribute.
      pTopo1->SewEdges( e1, t1 ); // Sew dodecahedron edges manually.
    }
  }

  pTopo->DeleteMeshHalfedgeAttribute( pAttrF ); // Delete temporary attribute.
  pTopo->DeleteMeshHalfedgeAttribute( pAttrE ); // Delete temporary attribute.
  pTopo1->Initialize(); // Initialize simple segmentation.

  dodecahedron = new MbPolymesh( pTopo1 );
}


//------------------------------------------------------------------------------
// \ru Создать звездчатый икосаэдр. \en Create stellate icosahedron.
// ---
static
void CreateIcosahedronStar( const SPtr<MbPolymesh> & icosahedron, SPtr<MbPolymesh> & star )
{
  MbPolymeshTopo * pTopo = icosahedron->GetTopo()->Clone();
  MbHalfedgeAttrPoint3D * pAttrVx = pTopo->GetMeshHalfedgeAttribute<MbHalfedgeAttrPoint3D>();
  pTopo->Reserve( 32, 180, 60);

  MbCartPoint uvCenter( ONE_THIRD, ONE_THIRD );
  const double shift = 80.;
  for ( size_t i = 0, n = pTopo->GetMeshTopo().FacetCount(); i < n; ++i )
  {
    if ( pTopo->GetMeshTopo().Facet( i ).IsActive() )
    {
      MbVector3D normal;
      if ( pAttrVx->CalculateTriangleNormal( i, normal ) )
      {
        // Split triangle on three by inserting a new vertex in the triangle center.
        const auto iVx = pTopo->InsertVertexTriangle( i, uvCenter.x, uvCenter.y );
        if ( iVx < SYS_MAX_T )
        {
          MbCartPoint3D point( pAttrVx->GetPoint( iVx ) );
          point.Move( normal * shift ); // Move new vertex along normal direction.
          pAttrVx->SetPoint( iVx, point );
        }
      }
    }
  }

  star = new MbPolymesh( pTopo );
}


//------------------------------------------------------------------------------
// \ru Создать звездчатый додэкаэдр. \en Create stellate dodecahedron.
// ---
static
void CreateDodecahedronStar( const SPtr<MbPolymesh> & dodecahedron, SPtr<MbPolymesh> & star )
{
  MbPolymeshTopo * pTopo = dodecahedron->GetTopo()->Clone();
  MbHalfedgeAttrPoint3D * pAttrVx = pTopo->GetMeshHalfedgeAttribute<MbHalfedgeAttrPoint3D>();
  pTopo->Reserve( 32, 180, 60 );

  MbCartPoint uvCenter( ONE_THIRD, ONE_THIRD );
  const double shift = 80.;
  std::array<size_t, 3> edgesInterpolate;
  IndicesVector edges( 5 );
  for ( size_t i = 0, n = pTopo->GetMeshTopo().FacetCount(); i < n; ++i )
  {
    if ( pTopo->GetMeshTopo().Facet( i ).IsActive() )
    {
      pTopo->GetMeshTopo().GetFacetEdges( i, edges );
      for ( size_t s = 0; s < 3; ++s )
        edgesInterpolate[s] = edges[s];

      // To add a new vertex in a pentagon facet it's necessary to define a virtual triangle to deal with. 
      // We take first three sequential edge around facet and add a vertex in a geometrical center of this triangle.
      // Note that this vertex will be inserted in the existing facet that means it splits the whole pentagon facet  
      // into five triangual facets gruiped in fan of this vertex.
      const auto iVx = pTopo->InsertVertexFacet( edges, edgesInterpolate, uvCenter.x, uvCenter.y );
      if ( iVx < SYS_MAX_T )
      {
        const auto & vertex = pTopo->GetMeshTopo().Vertex( iVx );
        const auto & edge = pTopo->GetMeshTopo().Edge( vertex.Edge() );
        MbVector3D normal;
        if ( pAttrVx->CalculateTriangleNormal( edge.Facet(), normal) ) // edge.Facet() is a new triangle.
        {
          MbCartPoint3D point( pAttrVx->GetPoint( iVx ) );
          point.Move( normal * shift ); // Move new vertex along normal direction.
          pAttrVx->SetPoint( iVx, point );
        }
      }
    }
  }

  star = new MbPolymesh( pTopo );
}


//------------------------------------------------------------------------------
// \ru Построение икосаэдра и додэкаэдра. \en Icosahedron and dodecahedron creation.
// ---
void SamplePolymeshIcosahedron()
{
  SPtr<MbPolymesh> icosahedron;
  CreateIcosahedron( icosahedron );
  if ( icosahedron.is_null() )
    return;

  TestVariables::viewManager->AddItem( *icosahedron );

  SPtr<MbPolymesh> dodecahedron;
  CreateDodecahedronByIcosahedron( icosahedron, dodecahedron );

  MbVector3D shift( 300, 0, 0 );
  dodecahedron->Move( shift );
  TestVariables::viewManager->AddItem( *dodecahedron );

  SPtr<MbPolymesh> icosahedronStar;
  CreateIcosahedronStar( icosahedron, icosahedronStar );
  icosahedronStar->Move( -shift );
  TestVariables::viewManager->AddItem( *icosahedronStar );

  SPtr<MbPolymesh> dodecahedronStar;
  CreateDodecahedronStar( dodecahedron, dodecahedronStar );
  dodecahedronStar->Move( shift );
  TestVariables::viewManager->AddItem( *dodecahedronStar );
}


//------------------------------------------------------------------------------
// \ru Создать квадратный полигональный объект. \en Create square polymesh.
// ---
static
bool CreatePlanePolymesh( const MbStepData & stepData, double width, SPtr<MbPolymesh> & pPolymesh )
{
  bool bOk = false;
  SPtr<MbPlane> pPlane( new MbPlane );
  pPlane->SetLimit( -width, -width, width, width );

  SPtr<MbSolid> pSolid;
  MbSNameMaker names;
  MbSolid * pAux = nullptr;
  if ( ::ElementaryShell( *pPlane, names, pAux ) == rt_Success )
  {
    pSolid = pAux;

    MbPolymeshTopo * pTopo = MbPolymeshTopo::Create();
    pPolymesh = new MbPolymesh( pTopo );

    MbShellLinearizationParams params( stepData, false, false, false, {} );
    MbShellLinearizationResults results;
    if ( CreatePolymeshByShell( params, *pSolid->GetShell(), *pPolymesh, results ) == rt_Success )
      bOk = true;
  }
  return bOk;
}


//------------------------------------------------------------------------------
// \ru Создать окружности. \en Create arcs.
// ---
static
bool GetDartboardArcs( size_t nArc, double radStep, std::vector<SPtr<MbArc3D>> & arcs )
{
  bool bOk = true;
  arcs.resize( nArc );
  const MbCartPoint3D org;
  for ( size_t i = 0; i < nArc; ++i )
  {
    MbCartPoint3D p1( radStep * i + 1., 0., 0. );
    MbCartPoint3D p2( 0., radStep * i + 1., 0. );
    arcs[i] = MbArc3D::Create( org, p1, p2, 0, true );
    if ( arcs[i].is_null() )
    {
      bOk = false;
      break;
    }
  }
  return bOk;
}


//------------------------------------------------------------------------------
// \ru Создать мишень путем разделения сегментации. \en Create dartboard by segmentation splitting.
// ---
static
bool CreateDartboardWhole( const MbStepData & stepData, const std::vector<SPtr<MbArc3D>> & arcs, SPtr<MbPolymesh> & pPolymesh )
{
  bool bOk = false;
  const size_t nArc = arcs.size();
  MbCurvePolymeshProjectParams paramsSplit;
  paramsSplit.SetStepData( stepData );
  for ( size_t i = 0; i < nArc; ++i )
    paramsSplit.AddCurve( arcs[i], ProjProcessingMethod::splitSegmentation );

  MbPolymeshProjectResults results;
  if ( CutPolymeshByCurves( paramsSplit, results, *pPolymesh ) == rt_Success )
  {
    IndicesVector blacks, whites;
    for ( size_t i = 0; i < nArc; ++i )
    {
      const auto & pair = results.GetEdgePairPath( i, 0 ).front();
      const auto f = pPolymesh->GetTopo()->GetMeshTopo().Edge( pair.first ).Facet();
      size_t iReg, iSub;
      pPolymesh->GetTopo()->GetFacetRegionId( f, iReg, iSub );
      pPolymesh->SetTopo()->ExtractSubRegionAsRegion( iSub );
      pPolymesh->GetTopo()->GetFacetRegionId( f, iReg, iSub );
      if ( i % 2 == 0 )
        blacks.push_back( iReg );
      else
        whites.push_back( iReg );

      if ( i == nArc - 1 )
      {
        const auto f1 = pPolymesh->GetTopo()->GetMeshTopo().Edge( pair.second ).Facet();
        pPolymesh->GetTopo()->GetFacetRegionId( f1, iReg, iSub );
        pPolymesh->SetTopo()->ExtractSubRegionAsRegion( iSub );
        pPolymesh->GetTopo()->GetFacetRegionId( f1, iReg, iSub );
        whites.push_back( iReg );
      }
    }

    for ( auto r : blacks )
    {
      auto * cont = pPolymesh->SetTopo()->GetSegmRegionModelAttributes( r, true );
      cont->SetColor( 10, 10, 10 );
    }

    for ( auto r : whites )
    {
      auto * cont = pPolymesh->SetTopo()->GetSegmRegionModelAttributes( r, true );
      cont->SetColor( 230, 230, 230 );
    }
    bOk = true;
  }

  return bOk;
}


//------------------------------------------------------------------------------
// \ru Создать мишень путем удаления регионов сегментации. \en Create dartboard by deleting segmentation regions.
// ---
static
bool CreateDartboard( const MbStepData & stepData, const std::vector<SPtr<MbArc3D>> & arcs, SPtr<MbPolymesh> & pPolymesh )
{
  bool bOk = false;
  const size_t nArc = arcs.size();
  MbCurvePolymeshProjectParams paramsSplit;
  paramsSplit.SetStepData( stepData );
  for ( size_t i = 0; i < nArc; ++i )
  {
    if ( i % 2 == 0 )
      paramsSplit.AddCurve( arcs[i], ProjProcessingMethod::keepInnerPart );
    else
      paramsSplit.AddCurve( arcs[i], ProjProcessingMethod::keepOuterPart );
  }

  MbPolymeshProjectResults results;
  if ( CutPolymeshByCurves( paramsSplit, results, *pPolymesh ) == rt_Success )
  {
    for ( size_t r = 0, nReg = pPolymesh->GetTopo()->RegionCount(); r < nReg; ++r )
    {
      if ( pPolymesh->GetTopo()->IsRegionActive( r ) )
      {
        auto * cont = pPolymesh->SetTopo()->GetSegmRegionModelAttributes( r, true );
        cont->SetColor( 10, 10, 10 );
      }
    }
    
    bOk = true;
  }

  return bOk;
}


//------------------------------------------------------------------------------
// \ru Попытаться создать мишень с учетом отрезка. \en Try to create dartboard taking the segment into account.
// ---
static
bool CreateDartboardCross( const MbStepData & stepData, const std::vector<SPtr<MbArc3D>> & arcs, const SPtr<MbLineSegment3D> & pSeg, SPtr<MbPolymesh> & pPolymesh )
{
  bool bOk = false;
  const size_t nArc = arcs.size();
  MbCurvePolymeshProjectParams paramsSplit;
  paramsSplit.SetStepData( stepData );
  for ( size_t i = 0; i < nArc; ++i )
  {
    if ( i % 2 == 0 )
      paramsSplit.AddCurve( arcs[i], ProjProcessingMethod::keepInnerPart );
    else
      paramsSplit.AddCurve( arcs[i], ProjProcessingMethod::keepOuterPart );
  }
  paramsSplit.AddCurve( pSeg, ProjProcessingMethod::splitSegmentation );

  MbPolymeshProjectResults results;
  if ( CutPolymeshByCurves( paramsSplit, results, *pPolymesh ) == rt_Success )
  {
    if ( results.HasIntersections() || results.HasSelfIntersections() )
    {
      for ( size_t r = 0, nReg = pPolymesh->GetTopo()->RegionCount(); r < nReg; ++r )
      {
        if ( pPolymesh->GetTopo()->IsRegionActive( r ) )
        {
          auto * cont = pPolymesh->SetTopo()->GetSegmRegionModelAttributes( r, true );
          cont->SetColor( 230, 230, 230 );
        }
      }
    }

    bOk = true;
  }

  return bOk;
}


//------------------------------------------------------------------------------
// \ru Построение мишени. \en Dartboard creation.
// ---
void SamplePolymeshDartboard()
{
  const size_t nArc = 5;
  const double radStep = 1.;
  const double width = radStep * nArc + radStep;
  MbStepData stepData;
  stepData.SetLength( radStep );
  stepData.SetStepType( MbeStepType::ist_MetricStep );
  
  SPtr<MbPolymesh> pPolymesh;
  if ( !CreatePlanePolymesh( stepData, width, pPolymesh ) )
    return;

  std::vector<SPtr<MbArc3D>> arcs;
  if ( !GetDartboardArcs( nArc, radStep, arcs ) )
    return;

  SPtr<MbPolymesh> pPolymeshDartWhole( new MbPolymesh( pPolymesh->GetTopo()->Clone() ) );
  if ( !CreateDartboardWhole( stepData, arcs, pPolymeshDartWhole ) )
    return;

  stepData.SetAngle( DELTA_MOD * Math::deviateSag );
  stepData.SetStepType( MbeStepType::ist_DeviationStep );
  SPtr<MbPolymesh> pPolymeshDartWholeFine( new MbPolymesh( pPolymesh->GetTopo()->Clone() ) );
  if ( !CreateDartboardWhole( stepData, arcs, pPolymeshDartWholeFine ) )
    return;

  SPtr<MbPolymesh> pPolymeshDart( new MbPolymesh( pPolymesh->GetTopo()->Clone() ) );
  if ( !CreateDartboard( stepData, arcs, pPolymeshDart ) )
    return;

  const double shift = ( radStep * nArc + radStep ) * 2.;
  MbCartPoint3D corner0( shift * c3d::ONE_HALF, shift * c3d::ONE_HALF, 0. );
  MbCartPoint3D corner1( -shift * c3d::ONE_HALF, -shift * c3d::ONE_HALF, 0. );
  SPtr<MbLineSegment3D> pSeg( new MbLineSegment3D( corner0, corner1 ) );

  SPtr<MbPolymesh> pPolymeshCross( new MbPolymesh( pPolymesh->GetTopo()->Clone() ) );
  if ( !CreateDartboardCross( stepData, arcs, pSeg, pPolymeshCross ) )
    return;

  pPolymeshDartWhole->Move( MbVector3D::xAxis * shift );
  pPolymeshDartWholeFine->Move( MbVector3D::yAxis * shift );
  pPolymeshDart->Move( -MbVector3D::xAxis * shift );
  pPolymeshCross->Move( -MbVector3D::yAxis * shift );

  TestVariables::viewManager->AddItem( *pPolymesh );
  TestVariables::viewManager->AddItem( *pPolymeshDartWhole );
  TestVariables::viewManager->AddItem( *pPolymeshDartWholeFine );
  TestVariables::viewManager->AddItem( *pPolymeshDart );
  TestVariables::viewManager->AddItem( *pPolymeshCross );
  for ( size_t i = 0; i < nArc; ++i )
    TestVariables::viewManager->AddObject( *arcs[i] );
  TestVariables::viewManager->AddObject( *pSeg );
  TestVariables::viewManager->GetActiveWindow()->FitModelToWindow();
  TestVariables::viewManager->RefreshModel();
}


//------------------------------------------------------------------------------
// \ru Сделать оффсет открытой границы полигонального объекта. \en Offset an open polymesh boundary.
// ---
static
bool CreateOffsetOpenBound( double offset, SPtr<MbPolymesh> & pPolymeshOffsetBound )
{
  std::vector<c3d::IndicesVector> bounds;
  GetPolymeshOpenBoundaries( *pPolymeshOffsetBound, bounds );
  const auto & bound = bounds[0];
  IndicesPairsVector path;
  path.reserve( bound.size() );

  for ( auto e : bound )
    path.emplace_back( e, SYS_MAX_T );

  MbPathPolymeshOffsetParams params;
  params.AddPath( path, offset, -1., MbPathPolymeshOffsetParams::EndProcessingMethod::doNothing, MbPathPolymeshOffsetParams::EndProcessingMethod::doNothing, ProjProcessingMethod::splitSegmentation );
  MbPolymeshProjectResults results;
  if ( OffsetPolymeshPath( params, results, *pPolymeshOffsetBound ) != rt_Success )
    return false;

  auto & topoOffsetBound = pPolymeshOffsetBound->GetTopo()->GetMeshTopo();
  const auto e0 = bound[0];
  const auto f0 = topoOffsetBound.Edge( e0 ).Facet();
  size_t iReg, iSub;
  pPolymeshOffsetBound->GetTopo()->GetFacetRegionId( f0, iReg, iSub );
  pPolymeshOffsetBound->SetTopo()->ExtractSubRegionAsRegion( iSub );
  pPolymeshOffsetBound->GetTopo()->GetFacetRegionId( f0, iReg, iSub );
  auto * contOffsetBound = pPolymeshOffsetBound->SetTopo()->GetSegmRegionModelAttributes( iReg, true );
  contOffsetBound->SetColor( 255, 0, 0 );
  return true;
}


//------------------------------------------------------------------------------
// \ru Сделать оффсет проекции сегмента. \en Offset a segment projection result.
// ---
static
bool CreateOffsetSegment( double offset, const SPtr<MbLineSegment3D> & pSeg, SPtr<MbPolymesh> & pPolymeshOffsetSeg )
{
  MbPolymeshProjectResults resultsProj;
  MbCurvePolymeshProjectParams paramsProj;
  paramsProj.AddCurve( pSeg, ProjProcessingMethod::insert );
  if ( CutPolymeshByCurves( paramsProj, resultsProj, *pPolymeshOffsetSeg ) != rt_Success )
    return false;

   MbPathPolymeshOffsetParams params;
   params.AddPath( resultsProj.GetEdgePairPath( 0, 0 ), offset, offset, MbPathPolymeshOffsetParams::EndProcessingMethod::connect, MbPathPolymeshOffsetParams::EndProcessingMethod::connect, ProjProcessingMethod::splitSegmentation );
  MbPolymeshProjectResults results;
  if ( OffsetPolymeshPath( params, results, *pPolymeshOffsetSeg ) != rt_Success )
    return false;

  auto & topoOffsetBound = pPolymeshOffsetSeg->GetTopo()->GetMeshTopo();
  const auto e0 = results.GetEdgePairPath( 0, 0 ).front().first;
  const auto f0 = topoOffsetBound.Edge( e0 ).Facet();
  size_t iReg, iSub;
  pPolymeshOffsetSeg->GetTopo()->GetFacetRegionId( f0, iReg, iSub );
  pPolymeshOffsetSeg->SetTopo()->ExtractSubRegionAsRegion( iSub );
  pPolymeshOffsetSeg->GetTopo()->GetFacetRegionId( f0, iReg, iSub );
  auto * contOffsetBound = pPolymeshOffsetSeg->SetTopo()->GetSegmRegionModelAttributes( iReg, true );
  contOffsetBound->SetColor( 0, 0, 255 );
  return true;
}


//------------------------------------------------------------------------------
// \ru Сделать оффсет проекции сегмента с удалением части сетки. \en Offset a segment projection result with removing of the mesh part.
// ---
static
bool CreateDartSegment( double offset, ProjProcessingMethod method, const SPtr<MbLineSegment3D> & pSeg, SPtr<MbPolymesh> & pPolymeshOffsetSeg )
{
  MbPolymeshProjectResults resultsProj;
  MbCurvePolymeshProjectParams paramsProj;
  paramsProj.AddCurve( pSeg, ProjProcessingMethod::insert );
  if ( CutPolymeshByCurves( paramsProj, resultsProj, *pPolymeshOffsetSeg ) != rt_Success )
    return false;

  MbPathPolymeshOffsetParams params;
  params.AddPath( resultsProj.GetEdgePairPath( 0, 0 ), offset, offset, MbPathPolymeshOffsetParams::EndProcessingMethod::growToBoundary, MbPathPolymeshOffsetParams::EndProcessingMethod::connect, method );
  MbPolymeshProjectResults results;
  if ( OffsetPolymeshPath( params, results, *pPolymeshOffsetSeg ) != rt_Success )
    return false;

  return true;
}


//------------------------------------------------------------------------------
// \ru Построение оффсетов по поверхности полигонального объекта. \en Offset creation.
// ---
void SamplePolymeshOffsetPath()
{
  const double width = 5.;
  MbStepData stepData;
  stepData.SetLength( width * 0.1 );
  stepData.SetStepType( MbeStepType::ist_MetricStep );

  SPtr<MbPolymesh> pPolymesh;
  if ( !CreatePlanePolymesh( stepData, width, pPolymesh ) )
    return;

  MbCartPoint3D corner0( width * c3d::ONE_HALF, width * c3d::ONE_HALF, 0. );
  MbCartPoint3D corner1( -width * c3d::ONE_HALF, -width * c3d::ONE_HALF, 0. );
  SPtr<MbLineSegment3D> pSeg( new MbLineSegment3D( corner0, corner1 ) );

  SPtr<MbPolymesh> pPolymeshOffsetBound( new MbPolymesh( pPolymesh->GetTopo()->Clone() ) );
  if ( !CreateOffsetOpenBound( width * 0.1, pPolymeshOffsetBound ) )
    return;
  
  SPtr<MbPolymesh> pPolymeshOffsetSeg( new MbPolymesh( pPolymeshOffsetBound->GetTopo()->Clone() ) );
  if ( !CreateOffsetSegment( width * 0.05, pSeg, pPolymeshOffsetSeg ) )
    return;

  SPtr<MbPolymesh> pPolymeshDartSeg( new MbPolymesh( pPolymeshOffsetBound->GetTopo()->Clone() ) );
  if ( !CreateDartSegment( width * 0.05, ProjProcessingMethod::keepOuterPart, pSeg, pPolymeshDartSeg ) )
    return;

  SPtr<MbPolymesh> pPolymeshDartSegInv( new MbPolymesh( pPolymeshOffsetBound->GetTopo()->Clone() ) );
  if ( !CreateDartSegment( width * 0.05, ProjProcessingMethod::keepInnerPart, pSeg, pPolymeshDartSegInv ) )
    return;

  const double shift = width * 2.;
  pPolymeshOffsetBound->Move( MbVector3D::xAxis * shift );
  pPolymeshOffsetSeg->Move( MbVector3D::xAxis * shift * 2. );
  pPolymeshDartSeg->Move( MbVector3D::xAxis * shift * 3. );
  pPolymeshDartSegInv->Move( MbVector3D::xAxis * shift * 4. );

  TestVariables::viewManager->AddItem( *pPolymesh );
  TestVariables::viewManager->AddObject( *pSeg );
  TestVariables::viewManager->AddItem( *pPolymeshOffsetBound );
  TestVariables::viewManager->AddItem( *pPolymeshOffsetSeg );
  TestVariables::viewManager->AddItem( *pPolymeshDartSeg );
  TestVariables::viewManager->AddItem( *pPolymeshDartSegInv );
  TestVariables::viewManager->GetActiveWindow()->FitModelToWindow();
  TestVariables::viewManager->RefreshModel();
}
