//////////////////////////////////////////////////////////////////////////////////////////
//
//    The file contains source codes demonstrating samples how to make animation of bending.
//
//    WARNING: This code is only an example. The real application can require more 
//             functionality and data types for implementation.
// 
//////////////////////////////////////////////////////////////////////////////////////////

#include <test_manager.h>
#include <test_service.h>

#include <action_sheet.h>
#include <creator.h>

#include <solid.h>
#include <space_instance.h>
#include <instance_item.h>

#include <last.h>

using namespace c3d;

// Загрузка файл с моделью, необходимой для примера
// ! Модели и тела не создаются в данном примере, берутся готовые из файла
inline bool LoadModel( MbModel & model )
{
  bool fileIsValid = false;

  TCHAR nameBuffer[ 0x7FFF ] = _T( "Sample_animation_of_bending_src_data.c3d" );

  if( _GetOpenFileName( nameBuffer, _T( "Open file" ), _T( "Модели(*.c3d)\0*.c3d\0\0" ), _T( ".c3d" ) ) )
  {
    ::ReadModelItems( *reader_ex::CreateReaderEx( std::unique_ptr<iobuf_Seq>( &::createiobuf( nameBuffer ) ), io::in ), model );

    // Защита от загрузки неправильного файла
    // ! код примера находит нужные item по номерам

    if( model.ItemsCount() == 7 )
    {
      std::vector<MbItem*> items;

      model.GetItems( items );

      fileIsValid =
        ( st_SpaceInstance == items[ 0 ]->IsA() ) &&
        ( st_SpaceInstance == items[ 1 ]->IsA() ) &&
        ( st_SpaceInstance == items[ 2 ]->IsA() ) &&
        ( st_SpaceInstance == items[ 3 ]->IsA() ) &&
        ( st_Solid == items[ 4 ]->IsA() ) &&
        ( st_Solid == items[ 5 ]->IsA() ) &&
        ( st_Solid == items[ 6 ]->IsA() );

      if( fileIsValid )
      {
        fileIsValid =
          ( 12 == ( (MbSolid*) items[ 4 ] )->GetFacesCount() ) &&
          ( 8 == ( (MbSolid*) items[ 5 ] )->GetFacesCount() ) &&
          ( 6 == ( (MbSolid*) items[ 6 ] )->GetFacesCount() );
      }
    }
  }

  return fileIsValid;
}

// Найти грань тела, ближайшую к указанной точке
MbFace * FindFace( const MbSolid * const solid, const MbCartPoint3D & facePoint )
{
  MbFaceShell * shell = solid->GetShell();

  size_t n; // out - faceIndex
  double u, v;

  shell->NearPointProjection( facePoint, n, u, v );

  return shell->GetFace( n );
}

// Создать одиночный сгиб по отрезку, (!) но тело оставить в разогнутом состоянии
SolidSPtr CreateBendOverSegment( MbSolid *srcSheetSolid, MbeCopyMode mode, MbCurve3D * curve, const MbBendOverSegValues & params, SimpleName operationMainName )
{
  RPArray<MbFace> bendingFaces( 1, 1 );
  bendingFaces.Add( FindFace( srcSheetSolid, MbCartPoint3D( 0, 10, 0.5 ) ) );

  MbSolid *newSheetSolid = nullptr;

  MbSNameMaker nameMarker( operationMainName, MbSNameMaker::i_SideNone/*sideAdd*/, 0/*buttAdd*/ );

  ::BendSheetSolidOverSegment( *srcSheetSolid,
                               mode,
                               bendingFaces,
                               *curve,
                               true/*unbended*/,
                               params,
                               nameMarker,
                               newSheetSolid );

  return SolidSPtr( newSheetSolid );
}

// Создать все сгибы на теле
SolidSPtr CreateSolidWithBendFaces( const ItemsVector itemsPtr )
{
  // ! Дубликатов наименований граней быть не должно, надо менять operationMainName при каждой операции
  SimpleName operationMainName = (SimpleName) rand();

  // Создаем грани сгибов (создаем разогнутый сгиб), кривые берем из модели, по номерам item-ов

  // Сгиб для боковины
  SolidSPtr sheetSolid = CreateBendOverSegment( static_cast<MbSolid *>( itemsPtr[ 4 ] ),
                                                  cm_Copy,
                                                  static_cast<MbCurve3D *>( static_cast<MbSpaceInstance *>( itemsPtr[ 0 ] )->SetSpaceItem() ),
                                                  MbBendOverSegValues( 0.4/*k*/, 0.4999, M_PI_2, 0.0/*coneAngle*/, 0.0, true, rt_Only ),
                                                  operationMainName );
  // Большой сгиб
  sheetSolid = CreateBendOverSegment( sheetSolid,
                                      cm_Same,
                                      static_cast<MbCurve3D *>( static_cast<MbSpaceInstance *>( itemsPtr[ 1 ] )->SetSpaceItem() ),
                                      MbBendOverSegValues( 0.4/*k*/, 25.0, M_PI, 0.0/*coneAngle*/, 0.0, true, rt_Only ),
                                      ++operationMainName );

  // Два сгиба, разделенные площадкой
  // Чтобы в CreateBendOverSegment точку поиска bendingFaces не менять. создаем сначала "дальний" сгиб.
  MbBendOverSegValues params( 0.4/*k*/, 15.0, M_PI_2, 0.0/*coneAngle*/, 0.0, false, rt_Only );

  sheetSolid = CreateBendOverSegment( sheetSolid,
                                      cm_Same,
                                      static_cast<MbCurve3D *>( static_cast<MbSpaceInstance *>( itemsPtr[ 3 ] )->SetSpaceItem() ),
                                      params,
                                      ++operationMainName );

  sheetSolid = CreateBendOverSegment( sheetSolid,
                                      cm_Same,
                                      static_cast<MbCurve3D *>( static_cast<MbSpaceInstance *>( itemsPtr[ 2 ] )->SetSpaceItem() ),
                                      params,
                                      operationMainName );

  return SolidSPtr( sheetSolid );
}

// Пошаговый сгибатель - углы гиба зависят от шага
// ! Сгибаемое тело должно быть заранее поделено на грани сгибов.
class SheetBender
{
  SolidSPtr                 m_sheetSolid;
  PArray<MbSheetMetalBend>  m_bends;
  MbFace *                  m_fixedFace;
  MbCartPoint               m_surfaceParam;
  MbSNameMaker              m_nameMaker;

  // Создать сгиб
  MbSheetMetalBend * CreateBend( const MbSolid * solid, const MbCartPoint3D facePoint, const double radius, const double angle )
  {
    RPArray<MbFace> innerFaces( 1, 1 );
    innerFaces.Add( FindFace( solid, facePoint ) );

    RPArray<MbFace> outerFaces( 1, 1 );
    outerFaces.Add( FindPairBendFace( *innerFaces[ 0 ] ) );

    return new MbSheetMetalBend( innerFaces, outerFaces, 0.4/*k*/, radius, angle/*angle*/, 0.0 );
  }

public:
  SheetBender( SolidSPtr sheetSolid )
    : m_sheetSolid( sheetSolid )
    , m_bends( 4, 1 )
    , m_fixedFace( FindFace( sheetSolid, MbCartPoint3D( 0.0, 10.0, 0.5 ) ) )
    , m_nameMaker( (SimpleName) rand(), MbSNameMaker::i_SideNone/*sideAdd*/, 0/*buttAdd*/ )
  {
    m_bends.Add( CreateBend( sheetSolid, MbCartPoint3D( 50.0, 10.0, 0.5 ), 25, M_PI ) );
    m_bends.Add( CreateBend( sheetSolid, MbCartPoint3D( -55.0, 10.0, 0.5 ), 15, M_PI_2 ) );
    m_bends.Add( CreateBend( sheetSolid, MbCartPoint3D( -25.0, 10.0, 0.5 ), 15, M_PI_2 ) );
    m_bends.Add( CreateBend( sheetSolid, MbCartPoint3D( 0.0, 1.0, 0.5 ), 0.4999, M_PI_2 ) );

    m_fixedFace->GetSurfaceParam( 0.5, 0.5, m_surfaceParam.x, m_surfaceParam.y );
  }

  // Выполнить сгибание тела для шага stepNum (1 - 10)
  SolidSPtr Bend( unsigned int stepNum )
  {
    // ! Степень сгибания управляется не углом, а радиусом сгиба. От начального большого радиуса, к нужному - конечному
    ( (MbSheetMetalBend*&) m_bends[ 0 ] )->radius = 250.0 / stepNum;
    ( (MbSheetMetalBend*&) m_bends[ 1 ] )->radius = 150.0 / stepNum;
    ( (MbSheetMetalBend*&) m_bends[ 2 ] )->radius = 150.0 / stepNum;
    ( (MbSheetMetalBend*&) m_bends[ 3 ] )->radius = 4.999 / stepNum;

    MbSolid * resultSolid = nullptr;

    ::BendSheetSolid( *m_sheetSolid, cm_Copy, m_bends, *m_fixedFace, m_surfaceParam, m_nameMaker, resultSolid );

    return SolidSPtr( resultSolid );
  }
};

// Обертка над API системы ограничений
class ConstraintSolver
{
  GCM_system m_gcSystem;

public:
  ConstraintSolver()
  {
    m_gcSystem = GCM_CreateSystem();
    GCM_SetJournal( m_gcSystem, "SampleAnimationOfBending.jrn" );
  }

  ~ConstraintSolver()
  {
    GCM_RemoveSystem( m_gcSystem );
  }

  GCM_geom AddGeom( const MbPlacement3D placement )
  {
    return GCM_AddGeom( m_gcSystem, GCM_SolidLCS( placement ) );
  }

  void AddConstraint( GCM_c_type cType,
                      GCM_geom g1,
                      GCM_geom g2,
                      GCM_alignment aVal = GCM_CLOSEST,
                      GCM_tan_choice tVar = GCM_TAN_POINT )
  {
    GCM_AddBinConstraint( m_gcSystem, cType, g1, g2, aVal, tVar );
  }

  void FixGeom( GCM_geom g )
  {
    GCM_FreezeGeom( m_gcSystem, g );
  }

  GCM_result Evaluate()
  {
    return GCM_Evaluate( m_gcSystem );
  }

  MbPlacement3D GetNewPlacement3D( GCM_geom g )
  {
    return GCM_Placement( m_gcSystem, g );
  }

  void SetPlacement( GCM_geom g, const MbPlacement3D & place )
  {
    GCM_SetPlacement( m_gcSystem, g, place );
  }
};

/*
Класс, обеспечивающий пересчет исходного placement соответственно изгибу/смещением отслеживаемой грани тела.
! Грани отслеживаются по имени, поскольку при изгибе тела имена его граней не меняются.

Пересчет исходного placement производится следующим образом:
1) При создании объекта класса, в точке проекции начала исходного placement на отслеживаемую грань создается ЛСК.
Координаты начала ЛСК, в виде u v координат грани, запоминаются.
По исходному placement создается локальный placement в этой ЛСК и запоминается.

2) После модификации грани, на неё вновь создается ЛСК, в запомненной точке (u v координаты грани не поменялись)
и по запомненному локальному placement создается новый глобальный placement.
*/
class Placement3DTransformator
{
  MbName        m_faceName;           // Имя отслеживаемой грани
  MbPlacement3D m_srcLocalPlacement;  // Локальный placement в ЛСК на грани
  MbCartPoint   m_faceUV;             // contactPoint в координатах грани

  // Создать ЛСК на грани
  MbPlacement3D CreateLSC( const MbFace & face )
  {
    MbCartPoint surfaceUV;
    face.GetSurfaceParam( m_faceUV.x, m_faceUV.y, surfaceUV.x, surfaceUV.y );

    MbVector3D lscNormal;
    face.GetSurface().Normal( surfaceUV, lscNormal );

    // Нормаль поверхности грани и нормаль грани могут не совпадать по направлению, 
    // проверяем - если не совпадают - инвертируем lscNormal
    if( !face.IsSameSense() )
      lscNormal.Invert();

    MbVector3D lcsXVector;
    face.GetSurface().DeriveU( surfaceUV.x, surfaceUV.y, lcsXVector );

    MbCartPoint3D faceContactPoint;
    face.Point( m_faceUV.x, m_faceUV.y, faceContactPoint );

    return MbPlacement3D( faceContactPoint, lscNormal, lcsXVector );
  }

public:
  Placement3DTransformator( const MbSolid & srcBody, const MbPlacement3D & contactPlace )
  {
    // находим грань и uv точки контакта на грани
    const MbFaceShell &shell = *srcBody.GetShell();

    size_t n; // out - faceIndex
    double surfaceU, surfaceV;

    shell.NearPointProjection( contactPlace.GetOrigin(), n, surfaceU, surfaceV );

    const MbFace & face = *shell.GetFace( n );

    // получаем параметры грани
    face.GetFaceParam( surfaceU, surfaceV, m_faceUV.x, m_faceUV.y );

    // запоминаем имя грани, оно сохраняется при выполнении гиба грани
    m_faceName = face.GetName();

    // создаем на грани ЛСК и получим матрицу трансформации
    MbMatrix3D localTransform;
    CreateLSC( face ).GetMatrixInto( localTransform );

    // по оригинальному глобальному placement создаем локальный placement в ЛСК на грани
    m_srcLocalPlacement.Init( contactPlace );
    m_srcLocalPlacement.Transform( localTransform );
  }

  // Получить новый contactPlace для согнутого тела
  MbPlacement3D GetNewPlacement3D( const MbSolid & newBody )
  {
    // создаем ЛСК на грани, найденной по имени
    MbPlacement3D lcs( CreateLSC( *( newBody.GetShell()->FindFaceByName( m_faceName ) ) ) );

    // матрица трансформации из ЛСК в глобальный placement
    MbMatrix3D toGlobalTransform;
    lcs.GetMatrixFrom( toGlobalTransform );

    // Из запомненного локального placement формируем глобальный
    MbPlacement3D newPlacement( m_srcLocalPlacement );
    newPlacement.Transform( toGlobalTransform );

    return newPlacement;
  }
};

// Подготовка мест присоединения (contactPointPlacement) для коробок
inline MbPlacement3D PrepareBoxContactPointPlacement( const MbPlacement3D & srcPlacement, double angle /*radian*/ )
{
  MbPlacement3D placement( srcPlacement );

  placement.Rotate( MbAxis3D( placement.GetOrigin(), placement.GetAxisX() ), M_PI );
  placement.Rotate( MbAxis3D( placement.GetOrigin(), placement.GetAxisZ() ), angle );
  placement.Normalize();

  return placement;
}

// Пример простой анимации гибки
void SampleAnimationOfBending()
{
  // Очищаем модель (для повторного запуска примера)
  TestVariables::viewManager->DeleteModel();

  // ! Модели и тела не создаются в данном примере, берутся готовые из файла

  // Сообщение-запрос о необходимости загрузки файла
  MessageBoxEx( _T( "To execute this example you need to load the file \"Sample_animation_of_bending_src_data.с3d\".\nBy default, the file is located in the folder \"C3D_Toolkit\\C3D_Kernel\\Example\\Module\\Sample\"" ),
                _T( "SampleAnimationOfBending" ) );

  SolidSPtr tapeSolid;
  SolidSPtr sideSolid;
  SolidSPtr box1Solid;
  SolidSPtr box2Solid;
  SolidSPtr box3Solid;
  SolidSPtr box4Solid;
  SolidSPtr box5Solid;
  SolidSPtr box6Solid;

  {
    MbModel model;

    if( LoadModel( model ) )
    {
      ItemsVector itemsPtr;
      model.DetachItems( itemsPtr );

      // Лента - из исходного листового тела создаем тело с гранями сгибов 
      tapeSolid = CreateSolidWithBendFaces( itemsPtr );

      // Боковина
      sideSolid = (MbSolid*) itemsPtr[ 5 ];

      // Коробки
      box1Solid = (MbSolid*) itemsPtr[ 6 ];
      box2Solid = ( MbSolid* )&( (MbSolid*) itemsPtr[ 6 ] )->Duplicate();
      box3Solid = ( MbSolid* )&( (MbSolid*) itemsPtr[ 6 ] )->Duplicate();
      box4Solid = ( MbSolid* )&( (MbSolid*) itemsPtr[ 6 ] )->Duplicate();
      box5Solid = ( MbSolid* )&( (MbSolid*) itemsPtr[ 6 ] )->Duplicate();
      box6Solid = ( MbSolid* )&( (MbSolid*) itemsPtr[ 6 ] )->Duplicate();

      for( ItemsVector::iterator i = itemsPtr.begin(); i != itemsPtr.end(); ++i )
        ::DeleteItem( *i );
    }

    else
    {
      MessageBoxEx( _T( "File not found or no valid file was selected!" ), _T( "Error" ) );
      return;
    }
  }

  // Instance боковины 
  // ! Instance используется для упрощения управления положением в модели его внутренних объектов 
  InstanceSPtr side( new MbInstance( *sideSolid, MbPlacement3D( MbPlacement3D( MbCartPoint3D( 0.0, 0.0, 0.0 ) ) ) ) );

  // Instances коробок
  InstanceSPtr box1( new MbInstance( *box1Solid, MbPlacement3D( MbCartPoint3D( 0.0, 0.0, 0.0 ) ) ) );
  InstanceSPtr box2( new MbInstance( *box2Solid, MbPlacement3D( MbCartPoint3D( 0.0, 0.0, 0.0 ) ) ) );
  InstanceSPtr box3( new MbInstance( *box3Solid, MbPlacement3D( MbCartPoint3D( 0.0, 0.0, 0.0 ) ) ) );
  InstanceSPtr box4( new MbInstance( *box4Solid, MbPlacement3D( MbCartPoint3D( 0.0, 0.0, 0.0 ) ) ) );
  InstanceSPtr box5( new MbInstance( *box5Solid, MbPlacement3D( MbCartPoint3D( 0.0, 0.0, 0.0 ) ) ) );
  InstanceSPtr box6( new MbInstance( *box6Solid, MbPlacement3D( MbCartPoint3D( 0.0, 0.0, 0.0 ) ) ) );

  // Добавляем объекты в отрисовочную модель
  TestVariables::viewManager->AddObject( Style(), tapeSolid );
  TestVariables::viewManager->AddObject( Style(), side );
  TestVariables::viewManager->AddObject( Style(), box1 );
  TestVariables::viewManager->AddObject( Style(), box2 );
  TestVariables::viewManager->AddObject( Style(), box3 );
  TestVariables::viewManager->AddObject( Style(), box4 );
  TestVariables::viewManager->AddObject( Style(), box5 );
  TestVariables::viewManager->AddObject( Style(), box6 );

  // Задаем ограничения
  ConstraintSolver cs;

  GCM_geom sideId = cs.AddGeom( side->GetPlacement() );
  GCM_geom sideContactPointPlacementId = cs.AddGeom( MbPlacement3D( MbCartPoint3D( 0.0, 0.0, 0.0 ) ) );

  GCM_geom box1Id = cs.AddGeom( box1->GetPlacement() );
  GCM_geom box1ContactPointPlacementId = cs.AddGeom( PrepareBoxContactPointPlacement( MbPlacement3D( MbCartPoint3D( -25.0, 5.0, -0.5 ) ), M_PI / 4 ) );

  GCM_geom box2Id = cs.AddGeom( box2->GetPlacement() );
  GCM_geom box2ContactPointPlacementId = cs.AddGeom( PrepareBoxContactPointPlacement( MbPlacement3D( MbCartPoint3D( -45.0, 10.0, -0.5 ) ), M_PI / 5 ) );

  GCM_geom box3Id = cs.AddGeom( box3->GetPlacement() );
  GCM_geom box3ContactPointPlacementId = cs.AddGeom( PrepareBoxContactPointPlacement( MbPlacement3D( MbCartPoint3D( -85.0, 15.0, -0.5 ) ), M_PI / 3 ) );

  GCM_geom box4Id = cs.AddGeom( box4->GetPlacement() );
  GCM_geom box4ContactPointPlacementId = cs.AddGeom( PrepareBoxContactPointPlacement( MbPlacement3D( MbCartPoint3D( 65.0, 10.0, -0.5 ) ), M_PI / 6 ) );

  GCM_geom box5Id = cs.AddGeom( box5->GetPlacement() );
  GCM_geom box5ContactPointPlacementId = cs.AddGeom( PrepareBoxContactPointPlacement( MbPlacement3D( MbCartPoint3D( 95.0, 5.0, -0.5 ) ), M_PI / 7 ) );

  GCM_geom box6Id = cs.AddGeom( box6->GetPlacement() );
  GCM_geom box6ContactPointPlacementId = cs.AddGeom( PrepareBoxContactPointPlacement( MbPlacement3D( MbCartPoint3D( -65.0, 5.0, -0.5 ) ), M_PI / 9 ) );

  cs.FixGeom( sideContactPointPlacementId );

  cs.FixGeom( box1ContactPointPlacementId );
  cs.FixGeom( box2ContactPointPlacementId );
  cs.FixGeom( box3ContactPointPlacementId );
  cs.FixGeom( box4ContactPointPlacementId );
  cs.FixGeom( box5ContactPointPlacementId );
  cs.FixGeom( box6ContactPointPlacementId );

  cs.AddConstraint( GCM_COINCIDENT, sideContactPointPlacementId, sideId );
  cs.AddConstraint( GCM_COINCIDENT, box1ContactPointPlacementId, box1Id );
  cs.AddConstraint( GCM_COINCIDENT, box2ContactPointPlacementId, box2Id );
  cs.AddConstraint( GCM_COINCIDENT, box3ContactPointPlacementId, box3Id );
  cs.AddConstraint( GCM_COINCIDENT, box4ContactPointPlacementId, box4Id );
  cs.AddConstraint( GCM_COINCIDENT, box5ContactPointPlacementId, box5Id );
  cs.AddConstraint( GCM_COINCIDENT, box6ContactPointPlacementId, box6Id );

  // Решение ограничений
  cs.Evaluate();

  // Задаем объектам насчитанные в решателе placement
  side->SetPlacement( cs.GetNewPlacement3D( sideId ) );
  box1->SetPlacement( cs.GetNewPlacement3D( box1Id ) );
  box2->SetPlacement( cs.GetNewPlacement3D( box2Id ) );
  box3->SetPlacement( cs.GetNewPlacement3D( box3Id ) );
  box4->SetPlacement( cs.GetNewPlacement3D( box4Id ) );
  box5->SetPlacement( cs.GetNewPlacement3D( box5Id ) );
  box6->SetPlacement( cs.GetNewPlacement3D( box6Id ) );

  //Обновляем отрисовочную модель - видно, что все коробки встали на свои места
  TestVariables::viewManager->RefreshModel();
  TestVariables::viewManager->ShowModel();

  // Сгибатель тела с гранями сгибов
  // ! Сгибатель выдает разные варианты согнутого тела, в зависимости от шага анимации
  SheetBender sheetBender( tapeSolid );

  // "Трансформаторы" placement-ов точек контакта коробок для согнутых вариантов тела
  // ! "Трансформатор" пересчитывает placement коробок и боковины на новые положения на согнутом теле
  Placement3DTransformator sidePlacementTransformator( *tapeSolid, cs.GetNewPlacement3D( sideContactPointPlacementId ) );
  Placement3DTransformator box1PlacementTransformator( *tapeSolid, cs.GetNewPlacement3D( box1ContactPointPlacementId ) );
  Placement3DTransformator box2PlacementTransformator( *tapeSolid, cs.GetNewPlacement3D( box2ContactPointPlacementId ) );
  Placement3DTransformator box3PlacementTransformator( *tapeSolid, cs.GetNewPlacement3D( box3ContactPointPlacementId ) );
  Placement3DTransformator box4PlacementTransformator( *tapeSolid, cs.GetNewPlacement3D( box4ContactPointPlacementId ) );
  Placement3DTransformator box5PlacementTransformator( *tapeSolid, cs.GetNewPlacement3D( box5ContactPointPlacementId ) );
  Placement3DTransformator box6PlacementTransformator( *tapeSolid, cs.GetNewPlacement3D( box6ContactPointPlacementId ) );

  // Начинаем анимацию

  SetWaitCursor( true );
  ::SleepCP( 300 ); // чтобы исходную сцену успели разглядеть

  TestVariables::viewManager->DeleteObject( tapeSolid ); // удаляем из отрисовочной сцены еще не согнутую ленту

  for( unsigned int i = 1; i <= 10; ++i )
  {
    // Создаем согнутое тело
    SolidSPtr solid = sheetBender.Bend( i );
    TestVariables::viewManager->AddObject( Style(), solid );

    // Пересчитываем placement положение боковины и коробок для согнутого тела
    cs.SetPlacement( sideContactPointPlacementId, sidePlacementTransformator.GetNewPlacement3D( *solid ) );
    cs.SetPlacement( box1ContactPointPlacementId, box1PlacementTransformator.GetNewPlacement3D( *solid ) );
    cs.SetPlacement( box2ContactPointPlacementId, box2PlacementTransformator.GetNewPlacement3D( *solid ) );
    cs.SetPlacement( box3ContactPointPlacementId, box3PlacementTransformator.GetNewPlacement3D( *solid ) );
    cs.SetPlacement( box4ContactPointPlacementId, box4PlacementTransformator.GetNewPlacement3D( *solid ) );
    cs.SetPlacement( box5ContactPointPlacementId, box5PlacementTransformator.GetNewPlacement3D( *solid ) );
    cs.SetPlacement( box6ContactPointPlacementId, box6PlacementTransformator.GetNewPlacement3D( *solid ) );

    // Решение ограничений
    cs.Evaluate();

    // Задаем объектам насчитанные в решателе placement
    side->SetPlacement( cs.GetNewPlacement3D( sideId ) );
    box1->SetPlacement( cs.GetNewPlacement3D( box1Id ) );
    box2->SetPlacement( cs.GetNewPlacement3D( box2Id ) );
    box3->SetPlacement( cs.GetNewPlacement3D( box3Id ) );
    box4->SetPlacement( cs.GetNewPlacement3D( box4Id ) );
    box5->SetPlacement( cs.GetNewPlacement3D( box5Id ) );
    box6->SetPlacement( cs.GetNewPlacement3D( box6Id ) );

    //Обновляем отрисовочную модель
    TestVariables::viewManager->RefreshModel();
    TestVariables::viewManager->ShowModel();

    ::SleepCP( 300 );

    if( i < 10 )
      TestVariables::viewManager->DeleteObject( solid ); // чтобы модель осталась видна после завершения примера
  }

  SetWaitCursor( false );
}
