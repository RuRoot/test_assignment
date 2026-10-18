#include <test_samples.h>
#include <test_manager.h>
#include <action_solid.h>
#include <creator.h>
#include <assembly.h>
#include <constraint.h>
#include <solid.h>
#include <instance_item.h>
#include <attr_common_attribute.h>
#include <templ_sptr.h>
#include <alg_draw.h>
#include <generic_utility.h>
#include <ctime>
#include <last.h>

using namespace c3d;


//------------------------------------------------------------------------------
//
// ---
template< class PointsArray >
static void Get4BoxVertices( PointsArray & points )
{
  const double d1 = 100;
  const double d2 = 40;
  const double d3 = 0.62*(d1+d2);
  points.clear();
  MbPlacement3D place;
  MbCartPoint3D origin( -0.5 * d1, -0.5 * d2, - 0.5 * d3 );
  place.GetPointFrom( origin );
  points.push_back( origin );

  MbCartPoint3D pnt;
  pnt.Init( 0.5 * d1, -0.5 * d2, -0.5 * d3 );
  place.GetPointFrom( pnt );
  points.push_back( pnt );
  pnt.Init( -0.5 * d1, 0.5 * d2, -0.5 * d3 );
  place.GetPointFrom( pnt );
  points.push_back( pnt );
  pnt.Init( -0.5 * d1, -0.5 * d2, 0.5 * d3 );
  place.GetPointFrom( pnt );
  points.push_back( pnt );
}


//----------------------------------------------------------------------------------------
//
// ---
template< class PointsArray >
void Get3PointsForCylinder( PointsArray & points )
{
  points.clear();
  MbPlacement3D place;
  const double h = 50.0;
  MbCartPoint3D origin( -h, 0., 0. );
  place.GetPointFrom( origin );
  points.push_back( origin );

  MbCartPoint3D pnt;
  pnt.Init( 2. * h, 0., 0. ); //  точка на оси OZ, по которой будет задана высота
  place.GetPointFrom( pnt );
  points.push_back( pnt );
  pnt.Init( -h, 0., h/3 );// Точка на оси OX, по которой будет задан радиус
  place.GetPointFrom( pnt );
  points.push_back( pnt );
}

//----------------------------------------------------------------------------------------
// Return a solid of a brick with a hole in the middle
// ---
static SPtr<MbSolid> HoledBrick()
{
  MbElementarySolidResults results1, results2;
  MbSNameMaker names1( 1, MbSNameMaker::i_SideNone, 0 );
  SArray<MbCartPoint3D> points;
  Get4BoxVertices( points );
  {
    SpacePointsVector auxPoints( points.begin(), points.end() );
    ::ElementarySolid( MbElementarySolidParams(et_Block, auxPoints, names1), results1 ); // ElementarySolid( points, et_Block, names1, box );   
  }

  MbSNameMaker names2( 2, MbSNameMaker::i_SideNone, 0 );
  Get3PointsForCylinder( points );
  {
    SpacePointsVector auxPoints( points.begin(), points.end() );
    ::ElementarySolid( MbElementarySolidParams(et_Cylinder, auxPoints, names2), results2 ); // ElementarySolid( points, et_Cylinder, names2, cyl );
  }

  MbSNameMaker operNames( ct_BooleanSolid, MbSNameMaker::i_SideNone, 0 );
  SolidSPtr newSolid;
  ::BooleanResult( results1.GetResultSolid(), cm_Copy, results2.GetResultSolid(), cm_Copy, MbBooleanOperationParams( bo_Difference, true, operNames ),
                   newSolid ); // BooleanSolid( *box, cm_Copy, *cyl, cm_Copy, bo_Difference, operNames, solid );

  return newSolid;
}

//----------------------------------------------------------------------------------------
// Demonstrating sample: spinning block
/*
  This sample demonstrates how to apply an angular driving dimension to rotate an assembly part.
*/
// ---
void SampleSpinningBlock()
{
  TestVariables::viewManager->DeleteModel();

  SPtr<MbSolid> brick( HoledBrick() );
  MbPlacement3D lcs;
  SPtr<MbInstance> comp1( new MbInstance(*brick, lcs) );
  SPtr<MbInstance> comp2( new MbInstance(*brick, lcs.Move(MbVector3D::zAxis*100.0)) );
  static_array<SPtr<MbInstance>,2> pair( comp1, comp2 );
  SPtr<MbAssembly> assm( new MbAssembly(pair) );  
  //assm->SetJournal( "C:\\Logs\\Journals\\spinning.jrn" );
  MtGeomArgument face1( brick->GetFace(0), comp1 );
  MtGeomArgument face2( brick->GetFace(1), comp2 );
  assm->AddConstraint( GCM_COINCIDENT, face1, face2 );
  MtGeomArgument hole1( brick->GetFace(2), comp1 );
  MtGeomArgument hole2( brick->GetFace(2), comp2 );
  assm->AddConstraint( GCM_CONCENTRIC, hole1, hole2 );
  MtGeomArgument plane1( brick->GetFace(5), comp1 );
  MtGeomArgument plane2( brick->GetFace(5), comp2 );
  MtGeomConstraint angDim = assm->AddConstraint( GCM_ANGLE, plane1, plane2, 0.0 );

  assm->EvaluateConstraints();
  /*
  SPtr<MbStringAttribute> aData( new MbStringAttribute(_T("Animation"), true, _T("360 degree")) );
  assm->AddAttribute( aData );
  */
  TestVariables::viewManager->AddObject( TestVariables::ELEMENTARY_Style, assm );
  
  const clock_t duration = CLOCKS_PER_SEC*2; // 2 seconds
  const double delta = CLOCKS_PER_SEC/20.0;
  const clock_t startClock = std::clock();
  while ( std::clock() < (startClock + duration) )
  {    
    const clock_t t = std::clock();
    assm->ChangeDimension( angDim, (t - startClock)*M_PI2/duration );
    TestVariables::viewManager->RefreshModel();
    TestVariables::viewManager->ShowModel();
    while ( std::clock() - t < delta ); // delay
  }

  assm->ChangeDimension( angDim, 0.0 );
  TestVariables::viewManager->RefreshModel();
  TestVariables::viewManager->ShowModel();
}

//#define __ASSEMBLY_ANIMATION_SAMPLE
#ifdef  __ASSEMBLY_ANIMATION_SAMPLE

class MtAssemblyAnimator;

//----------------------------------------------------------------------------------------
// Event handler of the assembly
//---
class MtAssemblyReactor: public ItAssemblyReactor
{
  // Capture the reactor instance by the assembly for further work.
  virtual  void Capture( const MbAssembly * );
  // Release this instance by the assembly, stop working with it.
  virtual  void Release();
  // This called when geometric solver failed to try for constraint satisfaction.
  virtual void  EvaluationFailed( const MbAssembly * ) const;
  // The geometric solver found a new position of a constrained sub-object belonging the assembly.
  virtual void  PositionChanged( const MbAssembly *, const MbItem * subItem ) const;

public:
  MtAssemblyReactor( const MtAssemblyAnimator & );

private:
  MtAssemblyAnimator &  animator; 
  const MbAssembly *    assembly;  

private:
  OBVIOUS_PRIVATE_COPY( MtAssemblyReactor );
};

//----------------------------------------------------------------------------------------
// Функциональность для анимации сборки с кинематикой.
// Functionality to animate cinematic assemblies.
//---
class MtAssemblyAnimator
{
public:
  MtAssemblyAnimator( MbAssembly & );
 ~MtAssemblyAnimator();

public:
  void Start() {}
  void Stop()  {}
  void Pause() {}
  void Run()   {}
  // The period of time to replay full cycle of animation.
  double  Period() const;

private:
  // Посчитать сцену на момент времени t (millisec).
  void    _Eval( double t );
  // Initialization of the animation job
  void    _Init( MbAssembly & );
  // Значение размера на момент времени t.
  double  _DimValue( const MtGeomConstraint & dim, double t ) const;

private:
  SPtr<MbAssembly>              m_assembly; // Animated assembly.
  std::vector<MtGeomConstraint> m_dims;     // Dimensional constraints.
  MtAssemblyReactor             m_reactor;  // Event handler of an assembly solver

  OBVIOUS_PRIVATE_COPY( MtAssemblyAnimator );
};

//----------------------------------------------------------------------------------------
//
//---
MtAssemblyAnimator::~MtAssemblyAnimator()
{
  m_assembly->SetReactor( nullptr ); // Обязательно освободить сборку от реактора
}

//----------------------------------------------------------------------------------------
// Capture the reactor instance by the assembly for further work.
//---
void MtAssemblyReactor::Capture( const MbAssembly * assm )
{
  PRECONDITION( assembly == nullptr );
  assembly = assm;
}

//----------------------------------------------------------------------------------------
// Release this instance by the assembly, stop working with it.
//---
void MtAssemblyReactor::Release()
{
  assembly = nullptr;
}

//----------------------------------------------------------------------------------------
// This called when geometric solver failed to try for constraint satisfaction.
//---
void MtAssemblyReactor::EvaluationFailed( const MbAssembly * assm ) const
{
  PRECONDITION( assembly == assm );
  if ( assembly == assm )
  {
    animator.Stop(); // Аварийная остановка анимации.
  }
}

//----------------------------------------------------------------------------------------
// The geometric solver found a new position of a constrained sub-object belonging the assembly.
//---
void MtAssemblyReactor::PositionChanged( const MbAssembly * assm, const MbItem * subItem ) const
{
  PRECONDITION( assembly == assm );
  if ( assembly == assm )
  {
    // Перерисовка объектов сцены или накопление изменившихся объектов в буфере для последующей перерисовки.
  }
}

//----------------------------------------------------------------------------------------
// Посчитать сцену на момент времени t.
//---
void MtAssemblyAnimator::_Eval( double t )
{
  std::vector<MtGeomConstraint>::iterator dIter = m_dims.begin();
  std::vector<MtGeomConstraint>::iterator dLast = m_dims.end();
  for ( ; dIter != dLast; ++dIter )
  {
    if ( !OK(m_assembly->ChangeDimension(*dIter, _DimValue(*dIter, t))) )
    {
      return;
    }
  }
  m_assembly->EvaluateConstraints();
}

//----------------------------------------------------------------------------------------
//
//---
double MtAssemblyAnimator::Period() const
{
  return 5000.0;
}

//----------------------------------------------------------------------------------------
// Значение размера на момент времени t (реализация функции времени).
//---
double MtAssemblyAnimator::_DimValue( const MtGeomConstraint & dim, double t ) const
{
  if ( dim.ConstraintType() == GCM_ANGLE )
  {
    return M_PI*2*t/Period();
  }

  return dim.DimValue();
}

//----------------------------------------------------------------------------------------
// Initialization of the animation job
//---
void MtAssemblyAnimator::_Init( MbAssembly & assembly )
{
  m_assembly = &assembly;
  m_assembly->SetReactor( &m_reactor );
  
  MtConstraintIter iter, last;
  for ( assembly.GetConstraints(iter,last); iter!=last; ++iter )
  {
    switch ( (*iter).ConstraintType() )
    {
    case GCM_ANGLE:
    case GCM_DISTANCE:
      m_dims.push_back ( *iter );
      break;
    }
  }
}

#endif //__ASSEMBLY_ANIMATION_SAMPLE

// eof