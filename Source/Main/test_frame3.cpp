#include <test.h>
#include <test_space.h>
#include <test_service.h>
#include <test_edit_space.h>
#include <test_computation.h>
#include <test_manager.h>
#include <test_mesh.h>
#include <test_edit_space.h>
#include <test_samples.h>
#include <op_direct_mod_parameter.h>
#include <last.h>

using namespace c3d;


//------------------------------------------------------------------------------
// Построение и редактирование объектов
// ---
bool CreateSpaceObjects( uint wParam ) 
{
  if ( TestVariables::viewManager == nullptr )
    return false;

  switch ( wParam ) 
  {
    // ******************************** создание точек

    case CM_NEWPOINT : // Точка
      CreateCartPoints();
      break;

    case CM_NEWPOINTONCURVE : // Проекция точки на кривую
      CreateCurvePointProjection();
      break;

    case CM_NEWNEARPOINTONCURVE : // Ближайшая проекция точки на кривую
      CreateCurveNearPointProjection();
      break;

    case CM_NEWCURVECURVATUREPOINT:
      CreateCurveCurvaturePoints();
      break;

    case CM_NEWPOINTONSURFACE : // Проекция точки на поверхность
      CreateSurfacePointProjection();
      break;

    case CM_NEWNEARPOINTONSURFACE : // Ближайшая проекция точки на поверхность
      CreateSurfaceNearPointProjection();
      break;

    case CM_NEWDIRECTPOINTONSURFACE : // Создание проекции точки на поверхность по направлению
      CreateSurfaceDirectPointProjection();
      break;

    case CM_NEWPOINTS_ON_SURFACE : // Создать группу точек на поверхности
      CreateSurfacePoints();
      break;

    case CM_NEWPOINTONSOLID : // Проекция точки на тело
      CreateSolidPointProjection();
      break;

    case CM_NEWPOINTS_BY_SOLID : // Создание точечного каркаса по вершинам тела // Create point frame by solid vertices
      while ( CreatePointFrameBySolidVertices() ) {}
      break;

    case CM_MERGE_POINTS : // Объединить точечные каркасы // Create point frames union
      CreatePointFramesUnion();
      break;

    case CM_NEWPOINTCURVECROSSING : // Точки пересечения или скрещивания кривых
      CreateCurveCurveInters();
      break;

    case CM_NEWPOINTWIREFRAMEINTERSECT: // Точки пересечения двух проволочных каркасов
      CreateWireFrameWireFrameInters();
      break;

    case CM_NEWPOINTCURVESURFACE : // Точки пересечения кривой с поверхностью
      CreateCurveSurfaceInters();
      break;

    case CM_NEWPOINTCURVESOLID : // Точки пересечения кривой с телом
      CreateCurveSolidInters();
      break;

    case CM_NEWEVENPARAM : // Выдать n точек кривой с равными интервалами по параметру
      CreateCurveEvenParamPoints();
      break;

    case CM_NEWEVENLENGTH : // Выдать n точек кривой с равными интервалами по по длине дуги
      CreateCurveEvenLengthPoints();
      break;

    case CM_DISTANCEALONG :
      CreateCurvePointDistanceAlong(); // Дать точки на кривой, отстоящие на расстояние len по дуге
      break;

    case CM_CREATECURVEISOPOINTS :
      CreateCurveIsoPoints();        // Дать точки касания кривой по отношению к направляющим вращения \ выдавливания   
      break;

    case CM_CUTMESH_BYPLANE :
      CutMeshByPlane();              // Создание разрезанной полигональной модели. Cut a mesh model off by a plane.
      break;

    case CM_CUTMESH_BYLINE :
      CutMeshByLine( false );        // Создание разрезанной полигональной модели. Cut a mesh model off by a line. 
      break;

    case CM_MESH_SECTION :
      CutMeshByLine( true );         // Создание контура сечения полигонального объекта. Cut a section contour of mesh object by a line.
      break;

    case CM_NEWMESH_BY_UNION:
      MeshBoolean( bo_Union );      // Операция объединения двух полигональных объектов. Perform union of two polygonal objects.
      break;

    case CM_NEWMESH_BY_INTERSECT:
      MeshBoolean( bo_Intersect );  // Операция пересечения двух полигональных объектов. Perform intersection of two polygonal objects.
      break;

    case CM_NEWMESH_BY_SUBSTRACT:
      MeshBoolean( bo_Difference ); // Операция вычитания двух полигональных объектов. Perform substraction of two polygonal objects.
      break;

    // ******************************** создание кривых

    case CM_NEWPLANENURBS : // B-сплайн в плоскости
      CreatePlaneNurbs();
      break;

    case CM_NEWPLANECBSPLN : // Кубический сплайн
      CreatePlaneSpline( (int)pt_CubicSpline );
      break;

    case CM_NEWPLANEOFFSET : // Плоская эквидистантная кривая
      CreatePlaneOffsetCurve();
      break;

    case CM_NEWCURVEBYPLANE : // Создание плоской кривой по двумерной кривой
      CreateCurveByCurveOnPlane();   
      break;

    case CM_NEWSPACECURVEBYPLANE : // Создание пространственной кривой по двумерной кривой
      CreateSpaceCurveByCurveOnPlane();
      break;

    case CM_GETPLANECURVE : // Создать кривую по плоской пространственной
      CreateGetPlaneCurve();
      break;

    case CM_NEWLINE3D : // Прямая точка-точка
      CreateLine3DPointPoint();
      break;

    case CM_NEWLINEPLANEPLANE :       // Прямая пересечения плоскостей
      CreateLine3DPlanesIntersection();
      break;

    case CM_NEWCIRCLE3D : // Окружность
      CreateCircle3D();
      break;

    case CM_NEWCIRCLE3DBYCENTRE : // Окружность
      CreateArc3D( 0, true );
      break;

    case CM_NEWCIRCLE3DBYPOINTS : // Окружность
      CreateArc3D( 1, true );
      break;

    case CM_NEWELLIPSE3D : // Эллипс
      CreateEllipse3D();
      break;

    case CM_NEWLINESEGMENT3D : // отрезок прямой линии точка-точка
      CreateLineSeg3D();
      break;

    case CM_NEWARC3D : // Дуга окружности
      CreateArc3D( 1, false );
      break;

    case CM_NEWARC3DBYPOINTS : // Дуга окружности
      CreateArc3D( 1, false );
      break;

    case CM_NEWSPIRALCURVE : // спираль
      CreateSpiral();
      break;

    case CM_NEWPOLYLINE3D : // Полилиния
      while ( ::CreateSpline3D( st_Polyline3D ) ) {}
      break;

    case CM_NEWCUBICSPLINE3D : // Кубический сплайн
      while ( ::CreateSpline3D( st_CubicSpline3D ) ) {}
      break;

    case CM_NEWBEZIER3D : // Bezier сплайн в пространстве
      while ( ::CreateSpline3D( st_Bezier3D ) ) {}
      break;

    case CM_NEWNURBS3D : // B-сплайн в пространстве
      CreateNurbs3D( false );
      break;

    case CM_NEWNURBS3D_WITH_MATING: // Построить сплайн по точкам и производным
      CreateNurbs3D( true );
      break;

    case CM_CURVE_B_SPLINE : // B-сплайн
      CreateBaseSpline();
      break;

    case CM_NEWHERMIT3D : // Hermit-сплайн
      while ( ::CreateSpline3D( st_Hermit3D ) ) {}
      break;

    case CM_NEWSURFACEBORDER : // Граничная кривая поверхности
      CreateSurfaceBorder();
      break;

    case CM_NEWCONTOUR3D : // Новый контур
      CreateContour3D();
      break;

    case CM_NEWCONTOUR3DBYCURVES : // Создание контура по существующим кривым
      CreateContour3DByCurves();
      break;

    case CM_NEWCONTOURS3DBYWIREFRAME : // Создание контуров по ребрам проволочного каркаса.
      CreateContours3DByWireFrame();
      break;

    case CM_NEWFILLETCONTOUR3DBYCURVES : // Создание скругленного контура по существующим кривым
      CreateFilletContour3DByCurves();
      break;

    case CM_NEWCHAMFERCONTOUR3DBYCURVES : // Создание фаски существующего контура
      CreateChamferContour3DByCurves();
      break;

    case CM_NEWCONTOURONPLANE : // Новый контур на плоскости
      CreateContourOnPlane();
      break;

    case CM_RECTANGLEONPLANE : // Новый прямоугольник
      CreateRectangleOnPlane();
      break;

    case CM_NEWTRIMMEDCURVE3D : // Усеченная кривая
      CreateTrimmedCurve3D();
      break;

    case CM_NEWEXTENDEDCURVE3D : // Создание удлиненной кривой
      CreateExtendedCurve3D();
      break;

    case CM_NEWCHARCURVE3D : // Создание символьной кривой
      CreateCharCurve3D();
      break;

    case CM_NEWOFFSETCURVE3D : // Создание эквидистантной кривой 
      CreateOffsetCurve3D();
      break;

    case CM_NEWCURVESBRIDGE3D : // Создание кривой-мостика, соединяющего две кривые 
      CreateCurvesBridge3D();
      break;

    case CM_NEWCURVESCOUPLE3D : // Создание кривой, соединяющей две осевые кривые трубопроводов
      CreateCurvesCouple3D();
      break;

    case CM_NEWCURVESFILLET3D : // Создание кривой, скругляющей две кривые 
      CreateCurvesFillet3D();
      break;

    case CM_NEWCURVESCONNCURVE3D: // Создание кривой, соединяющей две кривые
      CreateConnectingCurve3D();
      break;

    case CM_NEWEVOLUTIONSECTION : // Создание кривой, трансфорированной по закону кинематической операции
      CreateEvolutionSection();
      break;

    case CM_NEWCURVEONSURFACE : // Кривая на поверхности
      CreateSurfaceCurveProjection();
      break;

    case CM_NEWCURVEDIRECTVECTOR : // Кривая на поверхности по направлению
      CreateSurfaceCurveDirectProjection();
      break;

    case CM_NEWPROJECTIONCURVE_ON_SURFACE : // Создать проекционную кривую на поверхности
      CreateProjectionCurveOnSurface();
      break;

    case CM_TRANSLATECURVETONURBS : // Преобразовать кривую в NURBS
      TranslateCurveToNurbs();
      break;

    case CM_TRANSLATE_SPACECURVE_TO_SURFACECURVE : // Translate space curve into curve-on-surface form
      TranslateSpaceCurveToSurfaceCurve();
      break;

    case CM_TRANSLATECURVE3DTOCURVE : // Создание кривой на плоскости кривой по пространственной кривой
      TranslateCurve3DToPlaneCurve( false );
      break;

    case CM_TRANSLATECURVETOCURVE3D : // Создание пространственной кривой по кривой на плоскости
      TranslatePlaneCurveToCurve3D( false );
      break;

    case CM_TRANSLATECURVETOCONTOURON : // Создание контура на плоскости по кривой на плоскости
      TranslatePlaneCurveToContourOnPlane( false );
      break;

    case CM_TRANSLATECONTOURTOCONTOURON : // Создание контура на плоскости по контуру на плоскости
      TranslatePlaneContourToContourOnPlane( false );
      break;

    case CM_NEWEDGESCURVE :       // Создание контура по ребрам тела
      while ( CreateEdgeContour() ) {}
      break;

    case CM_NEWFACECONTOUR :       // Создание контура по ребрам грани
      while ( CreateFaceEdgeContour() ) {}    
      break;

    case CM_NEWFACEPROJECTION :   // Проекция грани на плоскость
      while ( CreateFaceProjection() ) {}
      break;

    case CM_NEWCURVE_BY_TWO_PROJECTIONS : // Создать кривую по двум плоским проекциям
      CreateCurveByTwoProjections();
      break;

    case CM_NEWCURVE_ON_SURFACE_BY_POINTS : { // Создать кривую на поверхности по точкам
        const MbItem * modelItem = nullptr;
        while ( CreateCurveOnSurfaceByPoints( modelItem ) ) { modelItem = nullptr; }
    } break;

    case CM_NEWCURVESWRAPPING : // Обертывание кривыми поверхности. Surface curves wrapping.
      SurfaceCurvesWrapping();
      break;

    case CM_WRAPPOINTS:
      SurfacePointsWrapping();  // Перенести точки с плоскости на поверхность.
      break;

    case CM_NEWCURVE_CENTER_LINE: // Создать осевые линии для грани оболочки.
      CreateFaceCenterLines();
      break;

    case CM_NEWCURVE_BY_VERTICES : // Кривая между вершинами грани. Segment by vertexes of a face.
      CreateCurveBetweenVertices();
      break;

    case CM_NEWNURBSCONIC3D_1 : // Построить коническое сечение по 2-ум точкам вершине и дискриминанту
      CreateNurbsConic_1();
      break;

    case CM_NEWNURBSCONIC3D_2 : // Построить коническое сечение по 3-ем точкам и вершине
      CreateNurbsConic_2();
      break;

    case CM_NEWNURBSCONIC3D_3 : // Построить коническое сечение по 3-ем точкам и 2-ум наклонам
      CreateNurbsConic_3();
      break;

    case CM_NEWNURBSCONIC3D_4 : // Построить коническое сечение по 2-ум точкам, 2-ум наклонам и дискриминанту
      CreateNurbsConic_4();
      break;

    case CM_NEWNURBSCONIC3D_5 : // Построить коническое сечение по 4-ем точкам и наклону
      CreateNurbsConic_5();
      break;

    case CM_NEWNURBSCONIC3D_6 : // Построить коническое сечение по 5-ти точкам
      CreateNurbsConic_6();
      break;

    case CM_NEWCOSINUSOID_BY_3_POINTS : // Создать косинусоиду по трем точкам и параметрам 
      CreatePlaneCosinusoid( true );
      break;
    case CM_NEWCOSINUSOID_BY_ORIGIN   : // Создать косинусоиду по началу и параметрами
      CreatePlaneCosinusoid( false );
      break;

    case CM_APPROXBYARCS :      // Аппроксимировать кривую дугами и отрезками
      MakeArcsApproximation();
      break;

    case CM_UNWRAPCURVE :       // Построить развертку кривой на плоскости
      CreateUnwrappingCurve();
      break;

    case CM_NEWCURVESUNWRAPPING : // Построить развертку кривых на плоскости
      SurfaceCurvesUnwrapping();
      break;

    case CM_UNWRAPPOINTS:
      SurfacePointsUnwrapping();  // Перенести точки на с поверхности на плоскость.
      break;

    case CM_FIT_CURVE_TOLERANCE:
      FitSmoothNurbs( 0 );
      break;

    case CM_FIT_CURVE_UNIFORM:
      FitSmoothNurbs( 1 );
      break;

    case CM_FIT_CIRCLE_LS:
      FitCurveLs( 0 );
      break;

    case CM_FIT_LINE_LS:
      FitCurveLs( 1 );
      break;

    case CM_FIT_ELLIPSE_LS:
      FitCurveLs( 2 );
      break;

    case CM_NEWLENGTHCURVE :    // Создать сплайн между заданными точками заданной длины. Create a spline between specified points of the specified length.
      CreateCablingCurve();     
      break;

    case CM_NEWFACECURVE :      // Создать сплайн на поверхности грани между между заданными точками. Create a spline on the surface of the face between between the specified points.
      CreateFaceCurve();       
      break;

    case CM_CURVE_FACEPROJECTION : // Создать пролекцию кривой на поверхности грани. Create a curve projection on the surface of the face.
      while( ::CreateCurveFaceProjection() );
      break;

    // ******************************** линии пересечения

    case CM_NEWSURFACEINTERSECTION : // Кривая пересечения поверхностей в общем случае
      CreateSurfaceIntersectionCurve();
      break;

    case CM_NEWSURFACEINTERSECTIONONEPOINT: // Кривая пересечения поверхностей по точке и вектору направления
      CreateSurfaceIntersectionCurveOnePoint();
      break;

    case CM_NEWSURFACESOLIDINTERSECT : // Создание кривой пересечения поверхности и тела
      CreateSurfaceSolidIntersection();
      break;

    case CM_NEWSOLIDINTERSECTION : // Построение линий пересечения тел
      CreateSolidIntersectionCurve();
      break;

    case CM_NEWFACESINTERSECTION : // Построение линий пересечения двух граней тел
      CreateFacesIntersectionCurve();
      break;

    case CM_MESH_MESH_INTERSECTION:
      MeshMeshIntersection();      // Контур пересечения 2-х полигональных объектов. Intersection contour of 2 mesh objects.
      break;

    // ******************************** проволочный каркас
    case CM_NEW_WIREFRAME_BY_CURVES : // Создать проволочный каркас на основе кривых
      CreateWireFrameByCurves();
      break;

    case CM_NEW_WIREFRAME_BY_POINTS: // Создать проволочный каркас по характерным точкам
      CreateWireFrameByPoints();
      break;

    case CM_NEW_WIREFRAME_BY_FACE: // Создать проволочный каркас по грани тела.
      CreateWireFrameByFace();
      break;

    case CM_NEW_NURBS_WIREFRAME: // Создание каркаса NURNS кривых.
      CreateNurbsCurveWireFrame();
      break;
      
    case CM_NEW_TRIMMED_WIREFRAME: // Создание усеченного каркаса.
      CreateTrimmedEdgeWireFrame();
      break;

    case CM_NEW_EXTENDED_WIREFRAME: // Создание продленного каркаса.
      CreateExtendedWireFrame();
      break;

    case CM_NEW_OFFSET_WIREFRAME: // Создание эквидистантного каркаса.
      CreateOffsetWireFrame();
      break;

    case CM_NEW_WIREFRAME_CONTOURS3D: // Контуры в виде каркасов по ребрам проволочного каркаса.
      CreateWireFramesByWireFrame();
      break;

    case CM_NEW_WIREFRAME_PARTS: // Отделение частей каркаса.
      CreatePartsByWireFrame();
      break;

    case CM_MERGE_EDGES_WIREFRAME: // Слияние ребер каркаса.
      MergeEdgesWireFrame();
      break;

    // ******************************** создание поверхностей

    case CM_NEWPLANE : // Плоскость
      CreatePlane();
      break;

    case CM_NEWPLANE3POINTS : // Плоскость по точкам
      CreatePlane3Points();
      break;

    case CM_NEWPLANEEDGE :
      CreateEdgePlane();     // Создать плоскость через ребро параллельно оси координат
      break;

    case CM_NEWCYLINDER3POINTS : // Цилиндр по точкам
      CreateCylinder3Points();
      break;

    case CM_NEWCONE3POINTS :     // Конус по точкам
      CreateCone3Points();
      break;

    case CM_NEWSPHERE3POINTS :   // Сфера по точкам
      CreateSphere3Points();
      break;

    case CM_NEWTORUS3POINTS :    // Тор по точкам
      CreateTorus3Points();
      break;

    case CM_NEWSPLINESURFACE : // NURBS-поверхность по точкам
      CreateSplineSurface();
      break;

    case CM_NEWRECTANGLENURBS :
      CreateQuadrangleNURBSSurface(); // Создание четырехугольной NURBS поверхности
      break;

    case CM_NEWLOFTEDSURFACE : // Hermit поверхность по кривым
      CreateLoftedSurface();
      break;

    case CM_NEWELEVATIONSURFACE : // Создание кинематической поверхности по сечениям 
      CreateElevationSurface(); 
      break;

    case CM_NEWRULEDSURFACE : // Поверхность линейчатая
      CreateRuledSurface();
      break;

    case CM_NEWREVOLUTION : // Поверхность вращения
      CreateRevolutionSurface();
      break;

    case CM_NEWPLANE_BYCURVE : // Плоскость по плоской кривой
      while ( CreatePlaneByCurve() ) {}
      break;

    case CM_NEWEXTRUSION : // Поверхность движения
      CreateExtrusionSurface();
      break;

    case CM_NEWEVOLUTION : // Кинематическая поверхность
      CreateEvolutionSurface();
      break;

    case CM_NEWSPIRALSURFACE : // Спиральная поверхность
      CreateSpiralSurface();
      break;

    case CM_NEWEXPANSION : // Кинематическая поверхность
      CreateExpansionSurface();
      break;

    case CM_NEWOFFSETSURFACE : // Эквидистантная поверхность
      CreateOffsetSurface();
      break;
      
    case CM_NEWDEFORMEDSURFACE : // Деформированная поверхность
      CreateDeformedSurface();
      break;

    case CM_NEWSECTORSURFACE : // Секториальная поверхность
      CreateSectorSurface();
      break;

    case CM_NEWCORNERSURFACE : // Поверхность по трем кривым
      CreateCornerSurface();
      break;

    case CM_NEWCOVERSURFACE : // Поверхность по четырем кривым
      CreateCoverSurface();
      break;

    case CM_NEWMESHSURFACE : // Поверхность на сетке кривых
      CreateMeshSurface();
      break;

    case CM_NEWEXTENDEDSURFACE : // Расширенная поверхность
      CreateExtendedSurface();
      break;

    case CM_NEWCURVEBOUNDEDSURFACE : // Поверхность, усеченная кривыми на поверхности
      CreateCurveBoundedSurface();
      break;

    case CM_TRANSLATESURFACETONURBS : // Преобразовать поверхность в NURBS
      TranslateSurfaceToNurbs();
      break;

    case CM_NEWTRIBEZIERSURFACE :
      CreateTriBezierSurface();      // Создание треугольного Безье
      break;

    case CM_NEWSIMPLEXSURFACE :
      CreateSimplexSurface();        // Создание симплексной поверхности. Create a simplex surface.
      break;

    case CM_NEWSIMPLEXSPLINESURFACE :
      CreateSimplexSplineSurface(); // Создание поверхности симплексного сплайна 
      break;

    case CM_NEWJOINSURFACE :
      CreateJoinSurface();          // Создание поверхности соединения  
      break;

    case CM_SWEPTSECTIONSURFACE :
      CreateSectionSurface();       // Поверхность переменного сечения. // Swept section surface.
      break;

    case CM_COONSPATCHSURFACE :
      CoonsPatchSurface();          // Построение поверхности Кунса касательной к четырём кривым на прверхностях. // The construction of Coons surface tangent to four curves on surfaces.
      break;

    case CM_SPLINEPATCHSURFACE :
      SplinePatchSurface();         // Построение поверхности-заплатки для заданных рёбер касательной к четырём кривым на поверхностях. // The construction of a surface-patch by the edges.
      break;

    case CM_UNBENDPLATE :
      RectifiedSurface();           // Построение развёртки поверхности на плоскость. // Construction of a surface sweep on a plane.
      break;

    case CM_NEWGRIDSURFACE :
      CreateGridSurface();           // Создание поверхности на точках триангуляции
      break;

    case CM_FITSURFACE:
      FitSurfaceToGrid();           // Вписывание поверхности в множество полигонов.
      break;

    case CM_FITSURFACE_AXIS_CONSTRAINT:
      FitSurfaceToGridWithConstraints(); // Вписывание поверхностиь в множество полигонов триангуляции с ограничениями.
      break;

    // ******************************** создание твердых тел

    case CM_NEWCYLINDERSOLID : // Цилиндр - твердое тело
      CreateElementarySolid( et_Cylinder );
      break;

    case CM_NEWCONESOLID : // Конус - твердое тело
      CreateElementarySolid( et_Cone );
      break;

    case CM_NEWSPHERESOLID : // Сфера - твердоге тело
      CreateElementarySolid( et_Sphere );
      break;

    case CM_NEWTORUSSOLID : // Тор - твердое тело
      CreateElementarySolid( et_Torus );
      break;

    case CM_NEWBLOCKSOLID : // Блок по трем точкам
      CreateElementarySolid( et_Block );
      break;

    case CM_NEWWEDGESOLID : // Клин по трем точкам
      CreateElementarySolid( et_Wedge );
      break;

    case CM_NEWPRISMSOLID : // Призма
      CreateElementarySolid( et_Prism );
      break;

    case CM_NEWPYRAMIDSOLID : // Пирамида
      CreateElementarySolid( et_Pyramid );
      break;

    case CM_NEWPLATESOLID : // Пластину по трем точкам
      CreateElementarySolid( et_Plate );
      break;

    case CM_NEWTETRAELEMENT : { // Create an element of lattice shell by points.  
      CreateElementarySolid( et_Tetrapipe );
    } break;

    case CM_NEWOCTAELEMENT : { // Create an element of lattice shell by points.  
      CreateElementarySolid( et_Octapipe );
    } break;

    case CM_NEWELEMENTARYSOLID : // Тело по поверхности
      CreateElementarySolid();
      break;

    //case CM_NEWPLATECONTOURSOLID : // Создание тела по плоскому контуру
    //  CreatePlateSolid();

    case CM_NEWNURBSBLOCKSOLID : // Создание nurbs-блока по трем точкам
      CreateNurbsBlockSolid3Points();
      break;

    case CM_NEWCURVEEXTRUSIONSOLID : // Тело выдавливания
      CreateCurveExtrusionSolid( 0, true );
      break;

    case CM_NEWCURVE3DEXTRUSIONSOLID :  // Тело выдавливания по трехмерной кривой
      CreateCurve3DExtrusionSolid( 0, false );
      break;
    
    case CM_NEWSOLIDEXTRUSIONSOLID :  // Тело выдавливания по трехмерной кривой
      CreateSolidExtrusionSolid( );
      break;

    case CM_NEWSOLIDEVOLUTIONSOLID :  // Кинематическое тело
      CreateSolidEvolutionSolid( );
      break;

    case CM_NEWSOLIDREVOLUTIONSOLID :  // Тело вращения
      CreateSolidRevolutionSolid( );
      break;

    case CM_NEWCURVEREVOLUTIONSOLID : // Тело вращения
      CreateCurveRevolutionSolid( 0, true );
      break;

    case CM_NEWCURVE3DREVOLUTIONSOLID : // Тело вращения по трехмерной кривой
      CreateCurve3DRevolutionSolid( 0, false );
      break;

    case CM_NEWCURVEEVOLUTIONSOLID : // Кинематическое тело
      CreateCurveEvolutionSolid( 0, true );
      break;

    case CM_NEWWIREFRAMEEXTRUSIONSOLID:  // Тело выдавливания проволочного каркаса.
      CreateWireFrameExtrusionSolid( true );
      break;

    case CM_NEWWIREFRAMEREVOLUTIONSOLID: // Тело вращения проволочного каркаса.
      CreateWireFrameRevolutionSolid( true );
      break;

    case CM_NEWWIREFRAMEEVOLUTIONSOLID: // Кинематическое тело по проволочному каркасу.
      CreateWireFrameEvolutionSolid( true );
      break;

    case CM_NEWCURVELOFTEDSOLID:     // Тело по плоским сечениям
      CreateCurveLoftedSolid( 0, true );
      break;

    case CM_NEWCURVELOFTEDSOLID_CL:  // Тело по плоским сечениям по осевой линии
      CreateCurveLoftedSolidCentralLine( 0, true );
      break;
    
    case CM_NEWRULEDSOLID : // Создание линейчатой оболочки по двум кривым
      CreateRuledShell();
      break;

    case CM_NEWUNIONSOLID :
      CreateUnionSolids( bo_Base );        // Создать составное тело
      break;

    case CM_BOOLEANUNION : // Объединение двух тел
      CreateBooleanSolid( bo_Union );
      break;

    case CM_BOSSCURVEEXTRUSIONSOLID :
      CreateCurveExtrusionSolid( 1, true ); // Изменить телом выдавливания
      break;

    case CM_BOSSCURVEREVOLUTIONSOLID : // Изменить телом вращения
      CreateCurveRevolutionSolid( 1, true );
      break;

    case CM_BOSSCURVEEVOLUTIONSOLID : // Изменить кинематическим телом 
      CreateCurveEvolutionSolid( 1, true );
      break;

    case CM_BOSSCURVELOFTEDSOLID:     // Изменить телом по плоским сечениям
      CreateCurveLoftedSolid( 1, true );
      break;

    case CM_BOSSCURVELOFTEDSOLID_CL:  // Изменить телом по плоским сечениям по осевой линии
      CreateCurveLoftedSolidCentralLine( 1, true );
      break;

    case CM_BOSSUNIONSOLID :
      CreateUnionSolids( bo_Union );        // Объединить с составным телом
      break;

    case CM_BOOLEANDIFFERENCE : // Вычитание двух тел
      CreateBooleanSolid( bo_Difference );
      break;

    case CM_CUTCURVEEXTRUSIONSOLID :
      CreateCurveExtrusionSolid(-1, true ); // Изменить телом выдавливания 
      break;

    case CM_CUTCURVEREVOLUTIONSOLID : // Изменить телом вращения
      CreateCurveRevolutionSolid(-1, true );
      break;

    case CM_CUTCURVEEVOLUTIONSOLID : // Изменить кинематическим телом 
      CreateCurveEvolutionSolid(-1, true );
      break;

    case CM_CUTCURVELOFTEDSOLID:     // Изменить телом по плоским сечениям
      CreateCurveLoftedSolid(-1, true );
      break;

    case CM_CUTCURVELOFTEDSOLID_CL:  // Изменить телом по плоским сечениям по осевой линии
      CreateCurveLoftedSolidCentralLine(-1, true );
      break;

    case CM_CUTUNIONSOLID :
      CreateUnionSolids( bo_Difference ); // Вырезать составное тело
      break;

    case CM_BOOLEANINTERSECTION : // Пересечение двух тел
      CreateBooleanSolid( bo_Intersect );
      break;

    case CM_INTERSECTCURVEEXTRUSIONSOLID :
      CreateCurveExtrusionSolid(-2, true ); // Изменить телом выдавливания 
      break;

    case CM_INTERSECTCURVEREVOLUTIONSOLID : // Изменить телом вращения
      CreateCurveRevolutionSolid(-2, true );
      break;

    case CM_INTERSECTCURVEEVOLUTIONSOLID : // Изменить кинематическим телом 
      CreateCurveEvolutionSolid(-2, true );
      break;

    case CM_INTERSECTCURVELOFTEDSOLID:     // Изменить телом по плоским сечениям
      CreateCurveLoftedSolid( -2, true );
      break;

    case CM_INTERSECTCURVELOFTEDSOLID_CL:  // Изменить телом по плоским сечениям по осевой линии
      CreateCurveLoftedSolidCentralLine( -2, true );
      break;

    case CM_INTERSECTUNIONSOLID :
      CreateUnionSolids( bo_Intersect );        // Пересечь с составным телом
      break;

    case CM_SOLID_CONTOUR_CUT: // Cut solid by a planar contour
      CutSolidByContour( true );
      break;

    case CM_SOLID_SURFACE_CUT : // Cut solid by a surface
      CutSolidBySurface( true );
      break;

    case CM_SOLID_SHELL_CUT: // Cut solid by a shell
      CutSolidByShell( true );
      break;

    case CM_NEWSHEETSOLID : // Листовое тело
      while ( CreateSurfaceSolid( false ) );
      break;

    case CM_OFFSET_SOLID :    // Создание эквидистантного тела. Create an offset solid.
      CreateOffsetSolid();
      break;

    case CM_NEWTHINSOLID_ONESIDE_OFFSET :    // Создание однотолщинного эквидистантного тела. Create a thin-walled solid.
      CreateMonoThickOffsetSolid( true );
      break;

    case CM_NEWTHINSOLID_ONESIDE_MONOTHICK : // Создание однотолщинного тонкостенного тела с удалением граней
      CreateMonoThickThinSolid( true );
      break;

    case CM_NEWTHINSOLID_ONESIDE_POLYTHICK : // Создание разнотолщинного тонкостенного тела с удалением граней
      CreatePolyThickThinSolid();
      break;

    case CM_SOLIDCHAMFER : // Тело с фаской ребра
      CreateSolidChamfer( st_Chamfer );
      break;

    case CM_SOLIDSLANT1 : // Тело с фаской ребра по катету и углу 
      CreateSolidChamfer( st_Slant1 );
      break;

    case CM_SOLIDSLANT2 : // Тело с фаской ребра по углу и катету 
      CreateSolidChamfer( st_Slant2 );
      break;

    case CM_SOLIDFILLET : // Тело со скругленным ребром
      CreateSolidFillet( st_Fillet );
      break;

    case CM_SOLIDSPAN : // Тело со скругленным ребром с заданной хордой
      CreateSolidFillet( st_Span );
      break;

    case CM_SOLIDVERTEXFILLET : // Тело со скругленной вершиной
      CreateSolisVertexFillet();
      break;
      
    case CM_FULLFILLETSOLID : // Тело со скругленной гранью
      CreateFullFilletSolid();
      break;

    case CM_SOLIDCHANNEL :
      CreateSolidChannel( st_Fillet );
      break;

    case CM_SOLIDSMOOTH : // Создание тела со скруглением по опорной кривой (переменного радиуса). Fillet edges with a supporting curves (variable radius).
      CreateSolidSmooth();
      break;

    case CM_NEW_SPLIT_SOLID :
      SplitSolidFaces(); // Создание тела c подразбиением грани
      break;

    case CM_NEWRIBSHELL :
      CreateRibOpenShell();    // Создание элемента приклеивания ребра жёсткости
      break;

    case CM_NEWRIBSOLID :
      CreateRibSolid();        // Создание тела c ребром жёсткости
      break;

    case CM_SOLIDNPDRAFT :
      CreateDraftSolidNP();    // Создание тела c уклоном от нейтральной плоскости
      break;

    case CM_NEWSYMMETRYSOLID : // Симметричное тело
      CreateMirrorSolid();
      break;

    case CM_NEWHOLESOLID :     // Создание тела с отверстием
      CreateHoleSolid( 0 );             
      break;

    case CM_NEWPOCKETSOLID :   // Создание тела с карманом или бобышкой
      CreateHoleSolid( 1 );             
      break;

    case CM_NEWSLOTSOLID :     // Создание тела с пазом
      CreateHoleSolid( 2 );             
      break;

    case CM_THINSOLIDBYSHELL : // Создание однотолщинного эквидистантного тела. Create a thin-walled solid.
      CreateMonoThickOffsetSolid( false ); 
      break;

    case CM_NEWJOINSOLID : // Создание грани на основе поверхности соединения
      CreateJoinShell();          
      break;

    case CM_DUPLICATION_BYGRID : // Размножение по сетке.
      DuplicationSolid( false );       
      break;

    case CM_DUPLICATION_BYMATRICES : // Размножение матрицами.
      DuplicationSolid( true );
      break;

    case CM_SOLIDSEW : // Тело сшивкой тел.
      CreateSolidSew();
      break;

    case CM_MEDIAN_SHELL : // Создание срединной оболочки по паре граней тела.
      CreateMedianShell();
      break;

    case CM_LATTICE_SHELL : 
      CreateLattice();     // Создать решетчатую оболочку. Create a lattice shell. 
      break;

    case CM_DIVIDED_SHELL : // Разделить оболочку на части по заданному набору ребер.
      CreateDividedShell();
      break;

    case CM_SECTION_SHELL : // Построить оболочку переменного сечения. Mutable section shell construction.
      CreateSectionShell(); 
      break;

    case CM_SECTION_SHELL_CUT: // Построить оболочку переменного сечения с подрезкой граней. Mutable section shell construction with faces cutting.
      CreateSectionShellCut(); 
      break;

    case CM_FACESMERGINGSOLID : // Создание тела слиянием граней в одну грань. // Create a solid with one face instead selected faces
      CreateFacesMergingSolid();

    // ******************************** создание тел прямого редактирования

    case CM_TRANSFORMEDSOLID : // Трансформация тела по матрице
      TransformedSolid();
      break;

    case CM_REMOVEMODIFIEDSOLID : // Модификация тела удалением выбранных граней с окружением
      ModifySolid( dmm_RemoveFacesWithAdjacentFillets );
      break;

    case CM_CREATEMODIFIEDSOLID : // Создание тела из выбранных граней с окружением
      ModifySolid( dmm_Create );
      break;

    case CM_ACTIONMODIFIEDSOLID : // Модификация тела перемещением выбранных граней с окружением относительно оставшихся граней
      ModifySolid( dmm_MoveFacesByVector );
      break;

    case CM_OFFSETMODIFIEDSOLID : // Модификация тела эквидистантным изменением граней 
      ModifySolid( dmm_OffsetFaces );
      break;

    case CM_FILLETMODIFIEDSOLID : // Модификация тела изменением граней скругления
      ModifySolid( dmm_FilletsRadiusChange );
      break;

    case CM_FILLETRADIUSSET: // Изменение радиуса скруглений
      ModifySolid( dmm_FilletsRadiusSet );
      break;

    case CM_SUPPLEMODIFIEDSOLID : // Замена выбранных граней тела деформируемыми гранями (превращение в NURBS)
      ModifySolid( dmm_ConvertFacesToNurbs );
      break;

    case CM_PURIFYMODIFIEDSOLID : // Удаление указанных граней скругления тела
      ModifySolid( dmm_PurifyFillets );
      break;

    case CM_ROTATEMODIFIEDSOLID : // Поворот граней тела
      ModifySolid( dmm_RotateFaces );
      break;

    case CM_GETHERMODIFIEDSOLID : // Слиянием гладко стыкующихся граней. Replacing smoothly joined faces with one face.
      while ( ModifySolid( dmm_UniteFaces ) ) {
      }
      break;

    case CM_REPLACEMODIFIEDSOLID : // Замена граней тела
      ModifySolid( dmm_ReplaceFaces );
      break;

    case CM_MERGERMODIFIEDSOLID : // Удаление указанных рёбер тела. Direct modeling (modification) of a body by removing edges. 
      ModifySolid( dmm_DeleteEdges );
      break;

    case CM_SUPPLESETCONJUGATION : // Модификация тела c измененными гранями установкой сопряжений по ребрам
      FaceModifiedSolidConj();
      break;

    case CM_SUPPLESETSIMILARITY : // Модификация тела c измененными гранями подобием указанной поверхности
      FaceModifiedSolidSimilarity();
      break;

    case CM_SUPPLESETSMOOTHING : // Модификация тела c измененными гранями сглаживанием
      FaceModifiedSolidSmoothing();
      break;

    case CM_REPLACEFACESGROUP : // Модификация тела заменой группы граней
      ModifySolid( dmm_ReplaceGroupFaces );
      break;

    case CM_RESIZE_CYLINDER : // Модификация тела изменением размера цилиндрических граней
      ModifySolid( dmm_CylinderRadiusSet );
      break;

    case CM_RESIZE_SPHERE : // Модификация тела изменением размера сферических граней
      ModifySolid( dmm_SphereRadiusSet );
      break;

    // ******************************** создание тел из листового материала

    case CM_CREATESHEETSOLID : // Создание тела из листового материала
      CreateSheetSolid( 0 );           
      break;

    case CM_SHEETSOLIDPLATE :  // Операция "Пластина" тела из листового материала
      CreateSheetSolid( 1 );           
      break;

    case CM_SHEETSOLIDHOLE :   // Операция "Отверстие" или "Вырез" тела из листового материала
      CreateSheetSolid(-1 );           
      break;

    case CM_SHEETSOLIDINTERSECT : // Операция пересечения тела из листового материала с контуром
      CreateSheetSolid( -2 );
      break;

    case CM_BENDOVERSEGMENT :  // Сгиб вдоль отрезка тела из листового материала
      BendSheetSolidOverSegment();
      break;

    case CM_JOGSHEETSOLID :  // Подсечка тела из листового материала
      SheetSolidJog();
      break;

    case CM_BENDBYEDGES :       // Сгиб по ребру тела из листового материала
      BendSheetSolidByEdges();       
      break;

    case CM_JOINTBEND : // Комбинированный сгиб
      SheetSolidJointBend();
      break;

    case CM_BEND_ANY : // Сгиб тела из листового материала
      BendAnySolid();
      break;

    case CM_SIMPLIFY_SHEET_PATTERN : // Упрощение развёртки листового тела
      SimplifySheetPattern();
      break;

    case CM_NORMALIZE_SHEET_HOLES: // Нормализация вырезов листового тела
      NormalizeSheetHoles();
      break;

    case CM_BUILD_BENDS_3D_AXIS_LINES : // Построение линий сгибов
      BuildBends3DAxisLines();
      break;

    case CM_BEND : // Сгиб тела из листового материала
      BendSheetSolid();
      break;

    case CM_UNBEND : // Выпрямление сгиба тела из листового материала
      UnbendSheetSolid();
      break;

    case CM_UNBENDALL : // Выпрямление всех сгибов тела из листового материала
      UnbendAllBends();
      break;

    case CM_ADD_ATTTRIBUTE : // Добавить атрибут телу или его примитиву
      InsertAttribToSolid();
      break;

    case CM_REMOVE_ATTTRIBUTES : // Удалить атрибуты тела
      while ( RemoveSolidAttributes() ) {}
      break;

    case CM_CLOSECORNER : // Замыкание угла при сгибе
      CloseCorner();
      break;

    case CM_STAMPSHEETSOLID : // Штамповка
      SheetSolidStamp();
      break;

    case CM_USERSTAMPSHEETSOLID: // Штамповка телом-инструментом.
      SheetSolidStampWithTool();
      break;

    case CM_BEADSHEETSOLID : // Буртик
      SheetSolidBead();
      break;

    case CM_JALOUSIESHEETSOLID : // Жалюзи
      SheetSolidJalousie();
      break;

    case CM_CREATERULEDSOLID : // Обечайка
      CreateRuledSolid();
      break;

    case CM_CREATESHEETRIB: // Ребро жесткости для листового тела.
      CreateSheetRib();
      break;

    case CM_SHEETMETAL_BYSOLID: // Построить листовое тела на основе произвольного тела.
      BuildSheetMetalByArbitarySolid();
      break;

    // ******************************** создание оболочек

    case CM_NEWNURBSSURFACESHELL : // Создать оболочку на основе nurbs-поверхности по точкам
      CreateNurbsSurfacesSolid();
      break;

    case CM_NEWCURVEEXTRUSIONSHELL : // создание оболочки выдавливания
      CreateCurveExtrusionSolid( 0, false );
      break;

    case CM_NEWCURVEREVOLUTIONSHELL : // создание оболочки вращения
      CreateCurveRevolutionSolid( 0, false );
      break;

    case CM_NEWCURVEEVOLUTIONSHELL : // создание оболочки кинематически
      CreateCurveEvolutionSolid( 0, false );
      break;

    case CM_NEWCURVELOFTEDSHELL:     // создание оболочки по плоским сечениям
      CreateCurveLoftedSolid( 0, false );
      break;

    case CM_NEWCURVELOFTEDSHELL_CL:  // создание оболочки по плоским сечениям по осевой линии
      CreateCurveLoftedSolidCentralLine( 0, false );
      break;

    case CM_NEWWIREFRAMEEXTRUSIONSHELL : // Создание оболочки выдавливания проволочного каркаса.
      CreateWireFrameExtrusionSolid( false );
      break;

    case CM_NEWWIREFRAMEREVOLUTIONSHELL:  // Создание оболочки вращения проволочного каркаса.
      CreateWireFrameRevolutionSolid( false );
      break;

    case CM_NEWWIREFRAMEEVOLUTIONSHELL: // Создание оболочки кинематики проволочного каркаса.
      CreateWireFrameEvolutionSolid( false );
      break;

    case CM_SOLIDVARIETY : // Объединение двух оболочек
      CreateBooleanSolid( bo_Variety );
      break;

    case CM_SOLIDEXTERNAL : // Вычитание двух оболочек
      CreateBooleanSolid( bo_External );
      break;

    case CM_SOLIDINTERNAL : // Пересечение двух оболочек
      CreateBooleanSolid( bo_Internal );     //-V112
      break;

    case CM_SHELLOFSOLID :  // Создание тонкой оболочки с удалением граней тела
      CreateMonoThickThinSolid( false );
      break;

    case CM_CUTSHELL_BYCONTOUR: // Cut unclosed shell off by a planar contour
      CutSolidByContour( false );
      break;

    case CM_CUTSHELL_BYSURFACE : // Cut unclosed shell off by a surface
      CutSolidBySurface( false );
      break;

    case CM_CUTSHELL_BYSHELL: // Cut unclosed shell off by a shell
      CutSolidByShell( false );
      break;

    case CM_CUTSHELL_BYOBJECTS : // Cut unclosed shell off by objects
      CutShellByObjects();
      break;

    case CM_OFFSETSHELLBYFACES :
      CreateOffsetShells();          // Создание эквидистантной оболочки на указанных гранях тела
      break;

    case CM_LOFTEDSHELLBYPOINTS :
      CreateLoftedShellByPoints();   // Создание Lofted оболочки по точкам 
      break;
      
    case CM_LOFTEDSHELLBYCURVES :
      CreateLoftedShellByCurves();   // Создание Lofted оболочки по кривым
      break;

    case CM_SHELLSURFACE : // Оболочка по поверхности
      while ( CreateSurfaceSolid( true ) );
      break;

    case CM_SHELLPATCH : // Создание новой заплатки
      CreatePatchShell( 1 ); 
      break;
    
    case CM_DRAP_SHELL: // Создание новой заплатки
      DrapeShell();
      break;

    case CM_CURVEPATCH : // Создание новой заплатки
      CreatePatchShell( 2 ); 
      break;
    
    case CM_NEWMESHSHELL : // Создание оболочки на сетке кривых
      CreateMeshShell();
      break;
    
    case CM_NEWRULSHELL : // Создание линейчатой оболочки по двум кривым
      CreateRuledShell();
      break;
    
    case CM_NEWEXTENSIONSHELL : // Создание продлённой грани
      CreateExtensionShell();
      break;

    case CM_FACESFILLET : // Создание тела по граням скругления между двумя наборами граней
      CreateFacesFillet( st_Fillet );
      break;

    case CM_THREEFACESFILLET : // Создание тела по граням скругления между тремя наборами граней
      CreateThreeFacesFillet();
      break;

    case CM_FILLET_BY_BORDERS: // Создание поверхности скргуления по граничным кривым.
      CreateFilletByBorders();
      break;

    case CM_SHELLSEW : // Оболочка сшивкой
      CreateShellSew();
      break;

    case CM_SILHOUETTECONTOURSOLID : // Создание оболочек на основе разрезки тела силуэтным контуром
      MakeSolidsSilhouetteContour();
      break;

    case CM_LATHESECTIONCURVES : { // Построение токарного сечения тела. 
      MbPlacement3D position;
      if ( MakeLatheElements( position ) ) {
        while ( MakeLatheElement( position, Math::DefaultMathVersion() ) ) {
        }
        MakeLatheContour( position );
      }
    } break;

    // ******************************** создание вставок и сборок

    case CM_NEWASSEMBLY : 
      CreateAssembly();                  // Создать сборку. 
      break;

    case CM_NEWINSTANCE : 
      CreateInstance();                  // Создать вставку. 
      break;

    case CM_NEWMESH_BYSHELL:
      CreateMesh();                      // Создать полигональный объект вместо указанного. // Create polygonal object instead a pointed.
      break;

    case CM_NEWMESH_BYFACE:
      CreateGrid();                      // Создать полигональный объект по грани тела. // Create polygonal object by solid face.

      break;

    case CM_NEWCONVEXPOLYHEDRON : 
      CreateConvexPolyhedron();          // Создать абрис объект.
      break;

    // ******************************** создание плоских объектов

    case CM_NEWCURVEHIDEMAP : // Проекция кривой
      while (MakeCurveHideMap());
      break;

    case CM_NEWSURFACEHIDEMAP : // Плоская проекция поверхности
      MakeSurfaceHideMap();
      break;

    case CM_NEWSOLIDHIDEMAP : // Плоская проекция тела
      MakeSolidHideMap( true );
      break;

    case CM_NEWSOLIDSHIDEMAP : // Плоская проекция тел
      MakeSolidHideMap( false );
      break;

    case CM_NEWALLSOLIDSHIDEMAP : // Плоская проекция тела
      MakeAllSolidsHideMap( );
      break;

    case CM_NEWOUTLINESOLIDS : // Создание очерка силуэтов тел
       MakeOutlineOfSolids( );
      break;

    case CM_NEWEDGEMAP : // Проецирование ребра на плоскую грань
      while ( CreateEdgeProjection( false ) );        
      break;

    case CM_NEW_SOLID_CONTOUR_SECTION_MAP : // Сечение тела
      MakeSolidContourSectionMap();
      break;

    case CM_NEW_SOLID_LOCAL_SECTION_MAP :  // Местный разрез тв.тела
      MakeSolidLocalSectionMap();
      break;

//    case CM_NEW_CONSTRUCTIVE_BY_POINTS :   // Создать вспомогательный объект по трем точкам
//      CreateConstuctiveByPoints();
//      break;

    // ******************************** создание крепежа

    case CM_FASTENERBYPOINT :
      CreateFastenerByPoint();
      break;

    case CM_FASTENERBYCURVE :
      CreateFastenerByCurve();
      break;

    // ******************************** создание резьбы

    case CM_NEWTHREAD_INSPACE : // Без привязки к телу
      CreateNewThreadInSpace();
      break;

    case CM_NEWTHREAD_ONSOLID : // С привязкой к телу
      CreateNewThreadOnSolid();
      break;

    case CM_NEWTHREAD_MAPPING_BYPARAMS : // Отображение резьбы
      CreateNewThreadMappingByParams();
      break;

    case CM_NEWTHREAD_MAPPING_BYTHREAD : // Отображение резьбы
      CreateNewThreadMappingByThread();
      break;

    // ******************************** воксельное предщставление

    case CM_VOXEL_REPRESENTATION :
      MakeVoxelRepresentation();
      break;

    // ******************************** редактирование резьбы

    case CM_MODTHREAD_ISMATEDTHREADS : // Сопрягаются ли резьбы
      ModifyThreadIsMatedThreads();
      break;
    case CM_MODTHREAD_ISBODYTHREAD   : // Принадлежит ли резьба телу
      ModifyThreadIsBodyThread();
      break;
    case CM_MODTHREAD_ADAPTTOBODY    : // Адаптировать резьбу к телу
      ModifyThreadAdaptToBody();
      break;

    // ******************************** редактирование точки

    case CM_MOVEPOINTALONGLINE : // Сдвиг точки вдоль направления кривой
      while ( MovePointAlongLine() ) ;
      break;

    // ******************************** редактирование кривой

    case CM_CURVEADDPOINT : // Задание новой точки кривой
      while ( ::AddSplineCurvePoint() ) ;
      break;

    case CM_CURVEADDKNOTE : // Задание новой точки кривой
      while ( AddNurbsKnot() ) ;
      break;

    case CM_CURVEREMOVEKNOTE : //  удаление точки кривой
      while ( RemoveNurbsKnote() ) ;
      break;

    case CM_REDUCE_NURBS_DEGREE: // Уменьшить порядок nurbs кривой на 1
      while ( ChangeSpaceNurbsDegree( true ) ) ;
      break;

    case CM_RAISE_NURBS_DEGREE: // Увеличить порядок nurbs кривой на 1
      while ( ChangeSpaceNurbsDegree( false ) );
      break;

    case CM_MOVECURVEPOINT : // Сдвиг точки NURBS кривой
      while ( ::ChangeSpaceSplinePoint() ) ;
      break;

    case CM_CURVESETCLOSED : // Замкнуть / разомкнуть кривую
      ::SpaceSplineSetClosed();
      break;

    case CM_CURVEINVERSE : // Изменить направление кривой на противоположное
      while ( CurveInverse() ) ;
      break;
    
    case CM_ATTACHNURBSTOOBJECT : // Состыковать сплайн с объектом
      ::AttachSpaceNurbsToObject();  
      break;
    
    case CM_ADDNURBSTONURBS : // Объединить два NURBS-сплайна. Add NURBS to NURBS.
      while ( ::AddSpaceNurbsToNurbs() ) {}  
      break;
    
    case CM_CREATEMIDLEPART : // Дать часть кривой
      while (CreateMidlePart());
      break;

    case CM_DELETEMIDLEPART : // Дать концевые части кривой
      while (DeleteMidlePart());
      break;

    case CM_CREATEBREAKPARTS : // Разбить кривую
      while (CreateBreakParts());
      break;

    case CM_TRANSLATECURVETOBASIS : // Удалить кривую, оставив базовые объекты
      RemoveCurveBasis();
      break;

    case CM_CONVERTNURBSINTOOPEN : 
      ConvertSpaceNurbsIntoOpen();  // Переопределение базисных точек NURBS из Close в Open при замкнутом узловом векторе
      break;

    case CM_CONVERTNURBSINTOCLOSE : 
      ConvertSpaceNurbsIntoClose(); // Переопределение базисных точек NURBS из Open в Close при замкнутом узловом векторе
      break;

    case CM_CONJUGATE_CONTOUR_SEGMENTS : 
      while( ::ConjugateContourSegments() ); // Гладко сопрячь сегменты контура. // Conjugate the contour segments smoothly.
      break;

    // ******************************** редактирование поверхности

    case CM_GOSURFACETHROUGHPOINT : // Провести поверхность через заданную точку
      GoSurfaceThroughPoint();
      break;

    case CM_GOSURFACETHROUGHPLANE : // Коснуться поверхностью заданной плоскости
      SurfaceBasisPointProjection(); // Спроецировать базовые точки объекта на поверхность 
//      GoSurfaceThroughPlane();
      break;

    case CM_TRANSLATESURFACETOBASIS : // Удалить поверхность, оставив базовые объекты
      RemoveSurfaceBasis();
      break;

    case CM_CONVERTNURBSSURFACEINTOOPEN : 
      ConvertNURBSSurfaceIntoOpen();       // Переопределение базисных точек NURBS из Close в Open при замкнутом узловом векторе
      break;

    case CM_CONVERTNURBSSURFACEINTOCLOSE : 
      ConvertNURBSSurfaceIntoClose();      // Переопределение базисных точек NURBS из Open в Close при замкнутом узловом векторе
      break;


    // ******************************** редактирование проволочного каркаса

    case CM_REBUILD_WIRE_FRAME: // Перестроить проволочные каркас.  // Rebuild wire frame.
      while ( RebuildWireFrame( false ) ) {}
      break;

    case CM_REBUILD_ALL_WIRE_FRAMES: // Перестроить все проволочные каркасы. // Rebuild all wire frames.
      RebuildAllWireFrames( false );
      break;

    // ******************************** редактирование твердого тела

    case CM_REBUILD_SHELL : // Перестроить тело.  // Rebuild solid or shell.
      while ( RebuildSolid( false ) ) {}
      break;

    case CM_REBUILD_ALL_SHELLS : // Перестроить все тела и оболочки. // Rebuild all solids and shells.
      RebuildAllSolids( false );
      break;

    case CM_REBUILD_SHELL_WITHCOPYMODE : // Перестроить тело с выбором режима копирования
      RebuildSolid( true );
      break;

    case CM_TRANSLATESOLIDTOBASIS : // Удалить тело, оставив базовые объекты
      while ( RemoveSolidBasis() ) {}
      break;

    case CM_TRANSLATESOLIDTOSURFACES : // Развалить тело на поверхности граней и ликвидировать его. // Break solid to surfaces and delete it.
      while ( TranslateSolidToSurfaces() ) {}
      break;

    case CM_TRANSLATESOLIDTOFACES : // Развалить тело на грани и ликвидировать его
      while ( TranslateSolidToFaces() ) {}
      break;

    case CM_BREAKSOLIDTOPARTS : // Рассыпать в базовые объекты
      BreakSolidToBasis();
      break;

    case CM_DELETE_ITEM_CREATORS : // Delete build log
      while ( DeleteItemCreators( false ) ) {}
      break;

    case CM_LEAVE_LAST_ITEM_CREATORS_ONLY : // Оставить только последние элементы истории построения
      DeleteItemCreators( true );
      break;

    case CM_BREAKSOLIDONPARTS : // Разделение тела на части
      BreakSolidOnParts();
      break;

    case CM_REMOVEINTERNALVOLUMES : // Удалить внутренние объемы тела
      RemoveInternalVolumes();
      break;

    case CM_CREATE_PARTSOLID : // Создать тело из частей тела
      CreatePartSolid();
      break;

    case CM_CHECKIDENTICALBASESURFACES: // Проверить и устранить наличие общих подложек
      CheckIdenticalBaseSufaces();
      break;

    case CM_UNIFYOWNCOMPLANARFACES : // Объединить компланарные грани тела
      UnifyOwnComplanarFaces();
      break;

    case CM_SOLIDENDMOVE : // Сдвинуть конец дерева построения. Move the end of the build log. 
      SolidEndMove( false );
      break;

    case CM_SOLIDFINISH : // Достроить тело для последней невыполненной операции в журнале построения. // Build a body by the last omitted operation in the build log.
      SolidFinish();
      break;

    case CM_SOLIDENDCUT :  // Обрезать конец дерева построения. Cut the the build log. 
      SolidEndMove( true );
      break;

    case CM_SOLIDCREATORDELETE : // Удалить элемент дерева построени. Delete the item of the build log.
      SolidCreatorDelete();
      break;

    case CM_SOLIDCREATORSETLASTVERS : // Установить последнюю версию элементу. Set last version for component.
      SolidCreatorSetLastVersion();
      break;

    case CM_SOLIDFILLETSSETLASTVERS : // Установить последнюю версию скруглениям. Set the latest version for fillets.
      SolidFilletsSetLastVersion();
      break;

    case CM_CHECK_SHELL_GEOMETRY : // Проверить оболочку тела
      while ( CheckShellGeometry() ) {}
      break;

    case CM_CHECK_SHELL_SELFINT: // Проверить оболочку тела на самопересечение
      CheckShellSelfInt();
      break;

    case CM_CHECK_ALL_SHELLS_GEOMETRY: /// Проверить оболочки всех тел
      CheckAllShellsGeometry();
      break;

    case CM_CHECK_SHELL_NAMES : // Проверить оболочку тела
      CheckShellNames();
      break;

    case CM_PERFORM_SHELL_NAMING : // Perform shell naming
      SetShellNames();
      break;

    case CM_CHECK_SHELL_MAIN_NAMES: // Проверить главные имена
      CheckShellMainNames();
      break;

    case CM_CHECK_POINTFRAME_NAMES : // Проверить имена на совпадение. // Check point frame names for a match.
      CheckPointFrameNames();
      break;

    case CM_CHECK_WIREFRAME_NAMES: // Проверить имена на совпадение. // Check wire frame names for a match.
      CheckWireFrameNames();
      break;

    case CM_CHECK_SHELLS_PTRS_COINCIDENCE : // Проверить совпадение указателей в оболочках
      CheckShellsPointers();
      break;

    case CM_CHECK_MESH_CLOSENESS: // Проверить замкнутость полигонального объекта. // Check closeness of a polygonal object
      CheckMeshClosed();
      break;

    case CM_CHECK_SURFACE_DERIVES : // Проверить производные поверхности.
      while ( CheckSurface() ) {};
      break;

    case CM_CHECK_SOLIDS_INTERSECT : // Предпроверка тел на пересечение
      while ( IsSolidsIntersection() ) {};
      break;

    case CM_CHECK_CONVEX_HULL_INTERSECT : // Проверка абрис триангуляций тел на пересечение
      IsConvexHullsIntersection();
      break;

    case CM_FIND_OVERLAPPING_SIMILAR_FACES : // Найти подобные грани с наложением.
      FindOverlappingSimilarFaces();
      break;

    case CM_FIND_FACES_EDGES_CARRIERS : // Найти грани - потенциальные носители ребер.
      FindFacesEdgesCarriers();
      break;

    case CM_FIND_NEAREST_SOLID: // Найти ближайшее тело.
      FindNearestSolid();
      break;

    case CM_CHECK_FACE_TRIANGULATION : // Проверить триангуляцию грани
      CheckFaceTriangulation();
      break;

    case CM_ADD_VERTEXEDGES_AS_CURVES :
      AddVertexEdgesAsCurves(); // Показать все ребра, ствкующиеся в вершине, добавив в модель копии кривых.
      break;

    case CM_ADD_SOLIDEDGES_AS_CURVES : // Показать все ребра тела, добавив в модель копии кривых.
      AddSolidEdgesAsCurves();
      break;

    case CM_ADD_SOLIDVERTS_AS_POINTS : // Показать все вершины тела, добавив в модель копии точек.
      AddSolidVertsAsPoints();
      break;

    case CM_RECOGNITIONSPIRALSURFACES :
      RecognitionSpiralSurfaces(); // Распознавание спиральных поверхностей в теле
      break;

    case CM_RECOGNITIONFILLETFACES:
      RecognitionFilletFaces(); // Распознавание граней скругления в теле
      break;

    case CM_SHELL_REVERSE : // Вывернуть оболочку
      ShellReverse();
      break;

    case CM_RESET_SURFACE_BBOX : // Reset bounding boxes of surfaces
      while ( ResetSurfaceGabarit() ) {}
      break;

    case CM_RESET_CURVES_TEMPDATA: // Reset temporary data of curves
      while ( ResetCurvesTemporaryData() ) {}
      break;

    case CM_RESET_INTCURVE_APPROX : // Reset approximation of surfaces intersection curve
      while ( ResetIntCurveApproximation() ) {}
      break;

    case CM_CHANGE_SHELL_CLOSEDNESS : // Forced change closedness of a shell
      ForcedChangeShellClosedness();
      break;

    case CM_REMOVE_INNER_LOOP : // Remove inner loop
      while ( RemoveInnerLoop() ) {}
      break;

    case CM_REPAIR_INEXACT_EDGES : // Try to repair inexact edges
      while ( RepairInexactEdges() ) {}
      break;

    case CM_REPAIR_VERTICES : // Try to repair vertices
      while ( RepairVertices() ) {}
      break;

    case CM_SOLID_REPAIRING : // Починить тело. Solid repairing.
      SolidRepairing();
      break;

    case CM_DECOMPOSE_ALL_ASSEMBLIES : // Разрушить сборки с подсборками на составляющие
      DecomposeAllAssemblies();
      break;

    case CM_REMOVE_COLOR_AND_VISUAL_ATTRIBUTES:
      RemoveAllColorAndVisualAttributes();
      break;

    case CM_CONVERT_TO_MESH : // \ru Преобразовать объект в полигоны. \en Convert an object to the mesh. \~
      ConvertToMesh();
      break;

    case CM_MESH_TO_GRIDS : // Развалить полиглнальный объект на отдельные триангуляции. // Break mesh to grids and delete it.
      TranslateMeshToGrids();
      break;

    case CM_SIMPLIFY_MESH_TRI_COUNT: // Упростить триангуляцию до заданного количества треугольников. // Simplify mesh up to specified triangles count.
      SimplifyMeshTrianglesCount();
      break;

    case CM_SIMPLIFY_MESH_TOLERANCE: // Упростить триангуляцию с заданной точностью. // Simplify mesh with specified tolerance.
      SimplifyMeshTolerance();
      break;

    case CM_ALIGN_MESH: // Совместить триангуляцию с другим объектом. // Align a mesh with another object.
      AlignMeshWithObject();
      break;

    case CM_KEEP_ONLY_GRIDS : // Оставить только пластины. // Keep only grids.
      KeepOnlyItems( 0 );
      break;

    case CM_KEEP_ONLY_POLYS : // Оставить только ломаные.  // Keep only polys.
      KeepOnlyItems( 1 );
      break;

    case CM_KEEP_ONLY_APEXS : // Оставить только апексы.   // Keep only apexs.
      KeepOnlyItems( 2 );
      break;

    case CM_REPAIRINCONSISTENTMESH: // Исправить несогласованную ориентацию сетки. // Repair inconsistent orientation.
      RepairInconsistentMesh();
      break;

    case CM_MESH_REVERSE: // Вывернуть полигональный объект. // Reverse a polygonal object.
      ReverseMesh();
      break;

    case CM_REBUILD_MESH: // Перестроить полигональный объект. // Rebuild a polygonal object.
      RebuildMesh();
      break;

    case CM_TRANSLATEMESHTOBASIS: // Один шаг назад в построении полигонального объекта. // Step back in building of a polygonal object.
      RemoveMeshBasis();
      break;

    case  CM_SET_SOLID_LAST_VERSION : // Выставить последнюю версию
      while( SetSolidLastVersion() );
      break;

    case  CM_CHECK_CURVES_TANGENT_MATING : // Проверить касание кривых
      CheckCurvesTangentMating();
      break;

    case CM_CHECK_CURVE_POLYGON_ACCURACY : // Проверить точность полигона кривой
      CheckCurvePolygonAccuracy();
      break;

    case  CM_SHOW_CURVE_DIRECTION : // Показать направление кривой
      while ( ShowCurveDirection() ) {};
      break;

    case  CM_SHOW_SURFACE_DIRECTIONS : // Показать направления поверхности
      while ( ShowSurfaceDirections() ) {};
      break;
    case  CM_SHOW_DEGENERATE_TRIANGLES : // Показать вырожденные треугольники сетки
      while ( ShowDegeneratedTrianglesAndGaps() ) {};
      break;

    case  CM_GETEDGENUMBER_INFACE : // Номер ребра в грани с указанием номера цикла
      while ( GetEdgeNumberInFace() ) {};
      break;

    case CM_GETFACENUMBER :
      while ( GetFaceNumber() ) {}   // Определение номера грани 
      break;

    case CM_FACEFILLETTYPE:
      while ( GetFaceFilletType() ) {}  // Определение типа скругления грани 
      break;

    case CM_GETEDGENUMBER :
      while (GetEdgeNumber()) {}   // Определение номера ребра
      break;

    case CM_GETFACEBYNUMBER :
      while (GetFaceByNumber()) {} // Определение грани по номеру 
      break;

    case CM_GETEDGEBYNUMBER :
      while (GetEdgeByNumber()) {} // Определение ребра по номеру  
      break;

    case CM_NUMBER_UNIQUE_SURFACE:
      NumberOfUniqueObjects( st_Surface );  // Количество уникальных поверхностей
      break;

    // ******************************** создание копий объектов

    case CM_DUPLICATEOBJECT : // Дублировать объектов
      DuplicateObject();
      break;

    case CM_MIRRORDUPLICATEOBJECT : // Зеркально дублировать объектов
      MirrorDuplicateObject();
      break;

    /*
      Группа редактирующих команд
      The group of editing commands
    */

    case CM_REBUILD_ITEM : // Перестроение элемента
      RebuildObject();
      break;

    case CM_MOVEOBJECT :   // Сдвиг элемента
      while ( MoveObject() );
      break;

    case CM_PLANEROTATEOBJECT : // Поворот элемента в плоскости
      while ( PlaneRotateObject() );
      break;

    case CM_LINEROTATEOBJECT : // Поворот элемента вокруг линии
      while ( LineRotateObject() );
      break;

    case CM_SCALEOBJECT : // Масштабировать элемент
      ScaleObject();
      break;

    case CM_TRANSFORMOBJECT : // Перевести в локальную систему. Transform by local system.
      TransformObject();
      break;

    case CM_TRANSLATETOBASIS : // Дать базовые объекты
      while (RemoveBasis());
      break;

    case CM_TRANSLATETOINGOT : // Превратить в слиток
      MakeIngot();
      break;

    case CM_MESH_TO_BREP : // Преобразовать полигональную модель в тело (Convert a mesh to the solid)
      MeshToSolid();
      break;

    case CM_SIMPLIFY_MESH : // Упростить полигональный объект (Simplify a mesh)
      SimplifyMesh();
      break;

    case CM_CONVERT_MESH_TO_INSTANCE : // Заменить повторяющиеся меши на вставки.
      ConvertMeshToInstance();
      break;

    case CM_NEW_ICOSAHEDRON_MESH : // Создать икосаэдр (Create Icosahedron)
      CreateIcosahedron( false );
      break;

    case CM_NEW_ICOSAHEDRON_SOLID : // Создать икосаэдр (Create Icosahedron)
      //CreateIcosahedron( true );
      CreateElementarySolid( et_Icosahedron );
      break;

    case CM_NEW_SPHERE_POLYHEDRON_MESH : // Создать аппроксимацию сферы многогранником (полигональный объект)
      CreateSpherePolyhedron( false );
      break;

    case CM_NEW_MESH_CONVEX_HULL: // Создать выпуклую оболочку (полигональный объект)
      CreateConvexHull();
      break;

    case CM_NEW_SPHERE_POLYHEDRON_SOLID : // Создать аппроксимацию сферы многогранником (тело)
      //CreateSpherePolyhedron( true );
      CreateElementarySolid( et_Polyhedron );
      break;

    case CM_ADDBASISPOINTS : // Дать базовые точки объекта
      AddBasisPoints();             
      break;

    case CM_DELETEOBJECT : // Удалить элемент
      DeleteObject();
      break;

    case CM_MESH_CURVATURES: // Рассчитать и отрисовать кривизны в вершинах сетки (Calculate and draw curvature map at mesh verticies).
      CreateMeshCurvatureMap( false );
      break;

    case CM_MESH_CURVATURES_DIR: // Рассчитать и отрисовать главные направления изменения кривизн в вершинах сетки (Calculate and draw principal curvature direction map at mesh verticies).
      CreateMeshCurvatureMap( true );
      break;

    case CM_MESH_SEGMENTATION_MAP: // Рассчитать и отрисовать сегментированную сетку (Calculate and draw segmentation map).
      CreateSegmentMap();  
      break;

    case CM_MESH_TO_BREP_SURF_RECON_AUTO: // Преобразовать полигональную модель в тело (Convert a mesh to the solid).
      ConvertMeshToBRep();
      break;

#if defined(C3D_DEBUG) // МА - устранение свойств для внеотладочной версии
    case CM_EDITPROPERTY : // Редактирование параметров элемента
      EditProperty();
      break;
#endif

    /*
      Группа команд: Редактирование выделенных объектов.
      The group: Edition of selected objects.
    */

    case CM_EDITMOVE : // Сдвиг выделенных элементов
      while ( MoveSelectedObjects() ) {}
      break;

    case CM_EDITPLANEROTATE : // Поворот выделенных элементов в плоскости
      while ( PlaneRotateSelectedObjects() ) {}
      break;

    case CM_EDITLINEROTATE : // Поворот выделенных элементов вокруг линии
      while ( LineRotateSelectedObjects() ) {}
      break;

    case CM_EDITSCALESELECTED : // Масштабировать выделенные элементы
      ScaleSelectedObjects();
      break;

    case CM_DELETESELECTED : // Удалить все выбранные объекты. Delete all selected objects.
      DeleteSelected();
      break;

    case CM_DELETEINVISIBLE : // Удаление все невидимые объекты. Delete all invisible objects.
      DeleteInvisible();
      break;

    case CM_SETOBJECTCOLOR : // Изменить цвет элементов. Change objects color.
      SetObjectColor( false );
      break;

    case CM_SETOBJECTWIDTH : // Установить толщину линий объектов
      SetObjectWidth();
      break;

    case CM_DOUBLE_MESH : // Создавать полигональные объекты на числах double(true)/float(false). Create polygonal object on double(true)/float(false) data.
      SetupDoubleMesh();
      break;

    case CM_ITEMSMERGING :  // Настроить слияние элементов в операциях
      SetupItemsMerging();
      break;

    case CM_INVISIBLEOBJECT :       // Сделать объект невидимым 
      InvisibleObject();
      break;   

    case CM_VISIBLEALLOBJECTS :     // Показать все объекты
      VisibleAllObjects();
      break;  

    case CM_VISIBLEPOINTS :         // Показать точки
      VisiblePoints();
      break;  

    case CM_VISIBLECURVES :         // Показать кривые
      VisibleCurves();
      break;  

    case CM_VISIBLESURFACES :       // Показать поверхности
      VisibleSurfaces();
      break;  

    case CM_VISIBLEBODIES :         // Показать тела
      VisibleBodies();
      break;

    case CM_PLACEMENT_VISIBILITY : // Coordinate system visibility
      {
        bool b = TestVariables::enableWCS;
        ::GetBoolean( IDS_ENTER_VIS_OR_NOT, b, false );
        TestVariables::enableWCS = b;
        TestVariables::viewManager->RefreshModel();
      }
      break;

    /*
      Группа команд создания пользователя.
      The group of user commands.
    */    

    case CM_MAKEUSERCOMAND0 :
      MakeUserCommand0(); // Создание пользователя
      break;  

    case CM_MAKEUSERCOMAND1 :
      MakeUserCommand1(); // Создание пользователя
      break;  

    case CM_MAKEUSERCOMAND2 :
      MakeUserCommand2(); // Создание пользователя
      break;  

    case CM_MAKEUSERCOMAND3 :
      MakeUserCommand3(); // Создание пользователя
      break;  

    case CM_MAKEUSERCOMAND4 :
      MakeUserCommand4(); // Создание пользователя
      break;  

    case CM_MAKEUSERCOMAND5 :
      MakeUserCommand5(); // Создание пользователя
      break;  

    case CM_MAKEUSERCOMAND6 :
      MakeUserCommand6(); // Создание пользователя
      break;  

    case CM_MAKEUSERCOMAND7 :
      MakeUserCommand7(); // Создание пользователя
      break;  

    case CM_MAKEUSERCOMAND8 :
      MakeUserCommand8(); // Создание пользователя
      break;  

    case CM_MAKEUSERCOMAND9 :
      MakeUserCommand9(); // Создание пользователя
      break;

    case CM_BETAMULTITHREADING1 :
      BetaMultithreaded0(); 
      break; 

    case CM_SHELLMULTITHREADING :
      MultithreadingTest ();
      break; 

    case CM_SHELLMULTITHREADING2 :
      MultithreadingTest ( true );
      break; 

    case CM_PROJECTIONMULTITHREADING :
      MultithreadingProjection();
      break; 

    case CM_THREADSAFETY :
      TestThreadSafety ();
      break; 

    case CM_PARALLELPROJECTION :
      MultithreadedGetVestiges( 2 );
      break; 

    case CM_PARALLELPROJECTION2 :
      MultithreadedGetVestiges2( 3 );
      break; 

    case CM_PARALLELINERTIA:
      MassInertiaCompare();
      break; 


    /*
      Группа команд тестовых примеров.
      The group of testing samples.
    */    
    
    // Modeler
    case CM_SAMPLE_ATTRIBUTES:
      SampleAttributes(); // How to use attributes
      break;
    case CM_SAMPLE_SOLID_TORUS_SPLINE:
      SampleTorusSplineSolid(); // How to create a torus solid based on the toroidal spline-surface
      break;
    case CM_SAMPLE_SOLID_SPLIT_BALL:
      SampleSplitBallIntoEightPieces(); // How to split the ball into 8 pieces
      break;
    case CM_SAMPLE_CYLINDER_MANUAL_SPLIT:
      SampleSplitCylinderManualSplit();  // How to create a split face on the cylinder face.
      break;
    case CM_SAMPLE_ANIMATION_OF_BENDING: // Анимация гибки
      SampleAnimationOfBending();
      break;
    case CM_SAMPLE_LOFTED_BEND: // Обечайка
      SampleLoftedBend();
      break;
    case CM_SAMPLE_SOLIDS_CLASSIFICATION: // Множественая классификация тел.
      SampleSolidsClassification();
      break;
    case CM_SAMPLE_CLOSE_CORNER: // Замыкание угла.
      SampleCloseCorner();
      break;
    case CM_SAMPLE_SEVERAL_BENDS: // Сгиб нескольких сгибов одновременно.
      SampleSeveralBends();
      break;

    // Solver
    case CM_SAMPLE_PARAMETRIC_SKETCH:
      SampleParametricSketch(); // Create parametric sketch.
      break;
    case CM_SAMPLE_WRITE_SKETCH:
      SampleWriteSketch(); // Write parametric sketch.
      break;
    case CM_SAMPLE_READ_SKETCH:
      SampleReadSketch(); // Read parametric sketch.
      break;
    case CM_SAMPLE_SPINNING_BLOCK:
      SampleSpinningBlock(); // How to spin a block using constraints.
      break;      
    case CM_SAMPLE_DODECAHEDRON:
      SampleDodecahedron(); // How to assembly dodecahedron using C3D Solver.
      break;
    case CM_SAMPLE_COLLISION_DETECTION:
      SampleCollisionDetection(); // How to detect collisions of solids.
      break;      
    case CM_SAMPLE_CHECK_COLLISIONS:
      SampleCheckCollisions(); // How to detect collisions between instances of an assembly.
      break;
    case CM_SAMPLE_CHECK_COLLISIONS_GROUPS: // How to detect collisions of mixed objects (solids or meshes) grouped into components, and how to skip collisions in components.
      SampleCollisionDetectionGroups();
      break;
    case CM_SAMPLE_POLYMESH_BUILD:
      SamplePolymeshIcosahedron();
      break;
    case CM_SAMPLE_POLYMESH_CUT:
      SamplePolymeshDartboard();
      break;
    
    case CM_SAMPLE_POLYMESH_OFFSET_PATH:
      SamplePolymeshOffsetPath();
      break;

    case CM_CURVEXTEND:
      CurveExtension();
      break;

    case CM_EXTRACT_BEAM:
      ExtractBeam();
      break;

    case CM_CREATE_POLYMESH:
      CreatePolymeshByMesh();
      break;
    
    case CM_POLYMESH_UNION:
      MakePolymeshBoolean( 0 );
      break;
    
    case CM_POLYMESH_INTERSECT:
      MakePolymeshBoolean( 1 );
      break;
    
    case CM_POLYMESH_SUBTRACT:
      MakePolymeshBoolean( 2 );
      break;

    case CM_CUT_PLANE_POLYMESH:
      CutPolymeshByPlane();
      break;

    case CM_CREATE_POLYMESH_BY_SHELL:
      CreatePolymeshBySolid();
      break;

    case CM_REMESH_POLYMESH:
      RemeshPolymesh();
      break;

    case CM_SMOOTH_POLYMESH:
      SmoothPolymesh();
      break;

    case CM_CALC_NORMALS_POLYMESH:
      CalculateMeshNormals();
      break;
    
    case CM_CUT_POLYMESH_BY_CURVES:
      CutPolymeshByCurves();
      break;

    default:
      return false;
  }


  return true;
}


