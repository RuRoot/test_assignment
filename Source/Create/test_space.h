////////////////////////////////////////////////////////////////////////////////
//
// Функции создания новых объектов в пространстве
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEST_SPACE_H
#define __TEST_SPACE_H

#include <mb_placement3d.h>
#include <space_item.h>
#include <op_direct_mod_parameter.h>

class    MbItem;
class    MbPlacement3D;


// Создание точек в пространстве

void CreateCartPoints();                   // Создание точки
void CreateCurvePointProjection();         // Создание проекции точки на кривую
void CreateCurveNearPointProjection();     // Создание проекции точки на кривую
void CreateCurveCurvaturePoints();         // Создание особых точек функции кривизны кривой.
void CreateSurfacePointProjection();       // Создание проекции точки на поверхность
void CreateSurfaceNearPointProjection();   // Создание проекции точки на поверхность
void CreateSurfacePoints();                // Создание группы точек на поверхности
void CreateSurfaceDirectPointProjection(); // Создание проекции точки на поверхность по направлению
void CreateSolidPointProjection();         // Создание проекции точки на тело
bool CreatePointFrameBySolidVertices();    // Создание точечного каркаса по вершинам тела // Create point frame by solid vertices
void CreatePointFramesUnion();             // Объединить точечные каркасы // Create point frames union

void CreateWireFrameWireFrameInters(); // Создание точки пересечения двух проволочных каркасов
void CreateCurveCurveInters();         // Создание точки пересечения или скрещивания кривых
void CreateCurveSurfaceInters();       // Создание точки пересечения кривой с поверхностью
void CreateCurveSolidInters();         // Создание точки пересечения кривой с телом

void CreateCurveEvenParamPoints();  // Выдать n точек кривой с равными интервалами по параметру
void CreateCurveEvenLengthPoints(); // Выдать n точек кривой с равными интервалами по по длине дуги
void CreateCurvePointDistanceAlong(); // Дать точки на кривой, отстоящие на расстояние len по дуге
void CreateCurveIsoPoints();        // Дать точки касания кривой по отношению к направляющим вращения \ выдавливания   
void CutMeshByPlane();              // Создание разрезанной полигональной модели. Cut a mesh model by a plane.
void CutMeshByLine( bool poly );    // Создание разрезанной полигональной модели / контура сечения(poly==true). Cut a mesh object by a line / a section contour(poly==true). 
void MeshMeshIntersection();        // Контур пересечения двух полигональных объектов. Intersection contour of two polygonal objects.
void MeshBoolean ( OperationType operation ); // Выполнить операцию объединения/пересечения/вычитания для двух полигональных объектов. Perform a Boolean operation on two polygonal objects.

// Создание кривых в пространстве

void CreatePlaneNurbs();            // Создание В-сплайна в плоскости
void CreatePlaneSpline( int curveType ); // Создание сплайна в плоскости
void CreatePlaneOffsetCurve();      // Создание эквидистантной кривой
void CreatePlaneCosinusoid( bool byThreePoints ); // Создание косинусоиды
void CreateCurveByCurveOnPlane();   // Создание плоской кривой по двумерной кривой
void CreateSpaceCurveByCurveOnPlane(); // Создание пространственной кривой по двумерной кривой
void CreateGetPlaneCurve();         // Создать кривую по плоской пространственной
void CreateLine3DPointPoint();      // Создание линии

void CreateLine3DPlanesIntersection(); // Создание линии пересечения плоскостей
void CreateCircle3D();              // Создание окружности
void CreateEllipse3D();             // Создание эллипса
void CreateLineSeg3D();             // Создание отрезка прямой линии
void CreateArc3D( int n, bool closed ); // Создание окружности или дуги окружности
void CreateSpiral();                // Создание спирали
bool CreateSpline3D( MbeSpaceType curveType ); // Создание сплайна
void CreateBaseSpline();               // Создание BaseSpline
void CreateNurbs3D( bool useMating );  // Создание В-сплайна
void CreateContour3D();                // Create space contour
void CreateContour3DByCurves();        // Create space contours by space curves
void CreateFilletContour3DByCurves();  // Создание контура со скруглениями по существующим кривым
void CreateChamferContour3DByCurves(); // Создание фаски существующего контура
void CreateContourOnPlane();           // Создание контура на плоскости
void CreateTrimmedCurve3D();           // Создание усеченной кривой
void CreateExtendedCurve3D();          // Создание удлиненной кривой

void CreateCharCurve3D();           // Создание символьной кривой
void CreateOffsetCurve3D();         // Создание эквидистантной кривой 
void CreateCurvesBridge3D();        // Создание кривой-мостика, соединяющего две кривые 
void CreateCurvesCouple3D();        // Создание кривой, соединяющей две осевые кривые трубопроводов
void CreateCurvesFillet3D();        // Создание кривой, скругляющей две кривые 
void CreateConnectingCurve3D();     // Создание соединяющей кривой
void CreateEvolutionSection();      // Создание кривой, трансформированной по закону кинематической операции

void CreateContours3DByWireFrame(); // Создание контуров по ребрам проволочного каркаса.
void CreateWireFramesByWireFrame(); // Создание контуров в виде отдельных каркасов по ребрам проволочного каркаса.
void CreatePartsByWireFrame();      // Отделение частей каркаса.

bool TranslateSpaceCurveToSurfaceCurve();                // Создание поверхностной кривой по пространственной кривой
void TranslatePlaneCurveToCurve3D( bool same );          // Создание пространственной кривой по кривой на плоскости
void TranslateCurve3DToPlaneCurve( bool same );          // Создание плоской кривой по пространственной кривой
void TranslatePlaneCurveToContourOnPlane  ( bool same ); // Создание контура на плоскости по кривой на плоскости
void TranslatePlaneContourToContourOnPlane( bool same ); // Создание контура на плоскости по контуру на плоскости

void CreateRectangleOnPlane();      // Создание прямоугольника на плоскости
bool CreateEdgeContour();            // Создание контура по ребрам тела
bool CreateFaceEdgeContour();        // Создание контура по ребрам грани
bool CreateFaceProjection();         // Проекция грани на плоскость
bool CreateCurveByTwoProjections();  // Создать кривую по двум плоским проекциям
bool CreateFaceCenterLines();        // Создать осевые (центральные) линии для грани оболочки
void CreateCurveBetweenVertices();   // Кривая между вершинами грани. Segment by vertexes of a face.
void CreateFaceCurve();              // Создать сплайн на поверхности грани между между заданными точками. Create a spline on the surface of the face between between the specified points.
bool CreateCurveFaceProjection();    // Создать пролекцию кривой на поверхности грани. Create a curve projection on the surface of the face.
void MakeArcsApproximation();        // Аппроксимация кривой дугами и отрезками

void CreateUnwrappingCurve();        // Создать развертку кривой
void SurfaceCurvesWrapping();        // Создать кривые, оборачивающие поверхность. Construct curves that wrap the surface.
void SurfaceCurvesUnwrapping();      // Создать развёртку кривых с поверхности на плоскость. Unwrap curves from a surface to a plane.
void SurfacePointsWrapping();        // Перенести точки с плоскости на поверхность. Translate points from plane into the surface.
void SurfacePointsUnwrapping();      // Перенести точки на с поверхности на плоскость. Translate points from the surface into the plane.

void FitSmoothNurbs( size_t mode );  // Аппроксимация гладкой нурбс кривой. Fit smooth NUIRBS.
void FitCurveLs( size_t mode );      // Аппроксимация аналитической кривой. Fit analytic curve.
void CreateCablingCurve();           // Создать сплайн между заданными точками заданной длины. Create a spline between specified points of the specified length.


// Конические сечения

void CreateNurbsConic_1();           // Создание конического сечения по 2-ум точкам, вершине и дискриминанту
void CreateNurbsConic_2();           // Создание конического сечения по 3-ем точкам и вершине
void CreateNurbsConic_3();           // Создание конического сечения по 3-ем точкам и 2-ум наклонам
void CreateNurbsConic_4();           // Создание конического сечения по 2-ум точкам, 2-ум наклонам и дискриминанту
void CreateNurbsConic_5();           // Создание конического сечения по 4-ем точкам и наклону
void CreateNurbsConic_6();           // Создание конического сечения по 5-ти точкам

// Создание кривых на поверхности и телах

void CreateSurfaceBorder();                                     // Создание граничной кривой поверхности.
void CreateContourOnPlane();                                    // Создание контура на плоскости.
void CreateSurfaceCurveProjection();                            // Создание проекции кривой на поверхности.
void CreateSurfaceCurveDirectProjection();                      // Создание проекции кривой на поверхность по направлению.
void CreateSurfaceIntersectionCurve();                          // Создание кривой пересечения поверхностей в общем случае.
void CreateSurfaceIntersectionCurveOnePoint();                  // Создание кривой пересечения поверхностей по начальной точке и направлению.
void CreateSurfaceSolidIntersection();                          // Создание кривой пересечения поверхности и тела.
void CreateSolidIntersectionCurve();                            // Создание кривой пересечения двух тел.
bool CreateFacesIntersectionCurve();                            // Создание кривой пересечения граней двух тел.
bool CreateCurveOnSurfaceByPoints( const MbItem *& modelItem ); // Кривую на поверхности по точкам.
bool CreateProjectionCurveOnSurface();                          // Создать проекционную кривую на поверхности.
void CreateWireFrameByCurves();                                 // Создать проволочный каркас на основе кривых.
void CreateWireFrameByPoints();                                 // Создать проволочный каркас по точкам (со скруглениями).
void CreateWireFrameByFace();                                   // Создать проволочный каркас по грани тела.
void CreateNurbsCurveWireFrame();                               // Создание проволочного каркаса NURNS кривых.
void CreateTrimmedEdgeWireFrame();                              // Создание усеченного проволочного каркаса.
void CreateExtendedWireFrame();                                 // Создание продленного проволочного каркаса.
void CreateOffsetWireFrame();                                   // Создание эквидистантного проволочного каркаса.
void ExtractBeam();                                             // Распознавание балочных элементов.
void MergeEdgesWireFrame();                                     // Слияние ребер каркаса.

// Создание элементарных поверхностей

void CreatePlane();                  // Создание плоскости
void CreatePlane3Points();           // Создание плоскости по трем точкам
void CreateEdgePlane();              // Создание плоскости по ребру параллельно оси координат
bool CreatePlaneByCurve();           // Создание плоскости по плоской кривой
void CreateCylinder3Points();        // Создание цилиндра по трем точкам
void CreateCone3Points();            // Создание конуса по трем точкам
void CreateSphere3Points();          // Создание сферы по трем точкам
void CreateTorus3Points();           // Создание тора по трем точкам

// Создание поверхностей

void CreateSectorSurface();          // Создание секториальной поверхности
void CreateSplineSurface();          // Создание NURBS поверхности по точкам
void CreateQuadrangleNURBSSurface(); // Создание четырехугольной NURBS поверхности
void CreateRuledSurface();           // создание линейчатой поверхности
void CreateExtrusionSurface();       // создание поверхности выдавливания
void CreateRevolutionSurface();      // создание поверхности вращения
void CreateEvolutionSurface();       // создание кинематической поверхности
void CreateSpiralSurface();          // создание спиральной поверхности
void CreateExpansionSurface();       // создание кинематической поверхности
void CreateOffsetSurface();          // создание эквидистантной поверхности
void CreateDeformedSurface();        // Создание деформированной поверхности
void CreateLoftedSurface();          // Создание Hermit сплайн поверхности по кривым
void CreateElevationSurface();       // Создание кинематической поверхности по сечениям 
void CreateExtendedSurface();        // создание расширенной поверхности
void CreateCurveBoundedSurface();    // Создание новой поверхности, усеченной кривыми на поверхности
void CreateCornerSurface();          // создание поверхности по трем кривым
void CreateBottomSurface();          // Создание поверхности по кривой
void CreateCoverSurface();           // создание поверхности по четырем кривым
void CreateMeshSurface();            // создание поверхности по сетке кривых
void CreateTriBezierSurface();       // Создание треугольного Безье
void CreateSimplexSurface();         // Создание симплексной поверхности. Create a simplex surface.
void CreateSimplexSplineSurface();   // Создание поверхности симплексного сплайна 
void CreateJoinSurface();            // Создание поверхности соединения  
void CreateSectionSurface();         // Поверхность заметания переменного сечения. // Swept mutable section surface.
void CoonsPatchSurface();            // Построение поверхности Кунса касательной к четырём кривым на прверхностях. // The construction of Coons surface tangent to four curves on surfaces.
void SplinePatchSurface();           // Построение поверхности-заплатки для заданных рёбер касательной к четырём кривым на поверхностях. // The construction of a surface-patch by the edges.
void RectifiedSurface();             // Построение развёртки поверхности на плоскость. // Construction of a surface sweep on a plane.
void CreateGridSurface();            // Создание поверхности на точках триангуляции

// Создание конструктивных тел

void CreateElementarySolid( int solidType ); // Создание тела по трем точкам
void CreateElementarySolid();                // Создание тела по поверхности
void CreateNurbsBlockSolid3Points();         // Создание nurbs-блока по трем точкам
void CreateIcosahedron( bool doSolid );      // Создание икосаэдра
void CreateSpherePolyhedron( bool doSolid ); // Создание аппроксимации сферы многогранником
void CreateConvexHull();                     // Создание выпуклой оболочки

// Создание твердых тел

void CreateCurveExtrusionSolid ( int type, bool closed );         // Создание тела выдавливания
void CreateCurve3DExtrusionSolid ( int type, bool closed );       // Создание тела выдавливания по неплоской кривой
void CreateSolidExtrusionSolid ();                                // Создание тела выдавливания из тела
void CreateCurveRevolutionSolid( int type, bool closed );         // Создание тела вращения
void CreateCurve3DRevolutionSolid( int type, bool closed );       // Создание тела вращения по неплоской кривой
void CreateSolidRevolutionSolid();                                // Создание тела вращения
void CreateCurveEvolutionSolid ( int type, bool closed );         // Создание кинематического тела
void CreateSolidEvolutionSolid();                                 // Создание кинематического тела
void CreateCurveLoftedSolid    ( int type, bool closed );         // Создание тела по плоским сечениям
void CreateCurveLoftedSolidCentralLine( int type, bool closed );  // Создание тела по плоским сечениям вдоль осевой линии
void CreateWireFrameExtrusionSolid( bool closed );                // Создание тела выдавливания по проволочному каркасу.
void CreateWireFrameRevolutionSolid( bool closed );               // Создание тела вращения по проволочному каркасу.
void CreateWireFrameEvolutionSolid( bool closed );                // Создание тела движения проволочного каркаса.

bool CreateSurfaceSolid( bool thin );                // Создание листового тела по поверхности.
void CreateOffsetSolid();                            // Создание эквидистантного тела. Create an offset solid.
void CreateMonoThickOffsetSolid( bool closed );      // Создание однотолщинного эквидистантного тела. Create a thin-walled solid.
void CreateMonoThickThinSolid( bool closed );        // Создание однотолщинного тонкостенного тела с удалением граней. Create a thin-walled solid.
void CreatePolyThickThinSolid();                     // Создание разнотолщинного тонкостенного тела с удалением граней. Create a thin-walled solid.

void CreateUnionSolids( OperationType );  // Создание составного тела. Create a solid with or without union of the intersecting solids.
void CreateSolidChamfer( int form );      // Создание тела с фаской ребра. Create chamfers for edges of the solid.
void CreateSolidFillet ( int form );      // Создание тела со скруглением ребра. Fillet edges with a constant radius
void CreateSolidChannel( int form );      // Создание тела со скруглением ребра. Fillet edges with a variable radius.
void CreateSolidSmooth();                 // Создание тела со скруглением по опорной кривой (переменного радиуса). Fillet edges with a supporting curves (variable radius).
void CreateSolisVertexFillet();           // Создание тела со скругленной вершиной. Create fillets on vertices and the edges adjacent to these vertices with a constant radius.
void CreateFullFilletSolid();             // Создание тела со скруглением цепочки граней. Create a fillet on the specified faces of the solid.

void CutSolidByContour( bool closed );    // Создание разрезанного плоским контуром тела (Cut solid by a planar contour)
void CutSolidBySurface( bool closed );    // Создание разрезанного поверхностью тела (Cut solid by a surface)
void CutSolidByShell  ( bool closed );    // Создание разрезанного поверхностью тела (Cut solid by a shell)
void CutShellByObjects();                 // Усечение оболочки выбранными объектами (Truncate shell by objects)

void CreateRibSolid();                    // Создание тела c ребром жёсткости
void CreateDraftSolidNP();                // Создание тела c уклоном от нейтральной плоскости. Slope the specified faces of the solid at the specified angle relative to the neutral isoplane.
void CreateRibOpenShell();                // Создание элемента приклеивания ребра жёсткости. Create a rib shell without union with initial solid.
void CreateBooleanSolid( OperationType ); // Создание результата объединения/пересечения/вычитания двух тел. Perform a Boolean operation.
void CreateMirrorSolid();                 // Создание симметричного тела. Create a mirror copy of a solid relative to a plane.

void CreateHoleSolid( int ind );      // Создание тела с отверстием, кармана или бобышки. Create a hole, a pocket, a groove in the solid.
void CreateJoinShell();               // Создание нового тела на основе поверхности соединения. Create a joint solid.
void DuplicationSolid( bool byMatr ); // Размножение тела. Duplicate the solid by the parameters and unite copies in one solid.
void CreateMedianShell();             // Срединная оболочка по парам граней тела. Median shell by faces of solid.
void CreateLattice();                 // Создать решетчатую оболочку. Create a lattice shell. 
void CreateDividedShell();            // Создать разделенную на части оболочку. Create divided shell.
void CreateSectionShell();            // Построить оболочку переменного сечения. Mutable section shell construction.
void CreateSectionShellCut();         // Построить оболочку переменного сечения с подрезкой граней. Mutable section shell construction with faces cutting.
void CreateFacesMergingSolid();       // Создание тела слиянием граней в одну грань. // Create a solid with one face instead selected faces.

// Создание тел прямого редактирования. Modify a shell by the methods of direct modeling.

void TransformedSolid();              // Модификация тела по матрице. Transform a copy of the initial solid using a given matrix
void EdgeModifiedSolid();             // Прямое моделирование (модификация) тела удалением рёбер. Direct modeling (modification) of a body by removing edges. 
void FaceModifiedSolidConj();         // Модификация тела c измененными гранями установкой сопряжений по ребрам 
void FaceModifiedSolidSimilarity();   // Модификация тела c измененными гранями подобием указанной поверхности 
void FaceModifiedSolidSmoothing();    // Модификация тела c измененными гранями сглаживанием
bool ModifySolid( MbeModifyingMode modifyingMode );// Модификация тела прямым редактированием. Modify solid with the direct editing.

// Создание крепежа

void CreateFastenerByPoint();         // Создать крепеж по точке. Create fastener using a point.
void CreateFastenerByCurve();         // Создать крепеж по кривой. Create fastener using a curve.
void MakeVoxelRepresentation();       // Построение воксельного представления. Voxel representation of the model.

// Создание оболочек

void CreateNurbsSurfacesSolid();                // Создать оболочку на основе nurbs-поверхности по точкам
void CreateOffsetShells();                      // Создание эквидистантной оболочки на указанных гранях тела
void CreateLoftedShellByPoints();               // Создание Lofted оболочки по точкам 
void CreateLoftedShellByCurves();               // Создание Lofted оболочки по кривым
void CreatePatchShell( int mode );              // Создание новой заплатки
void CreateMeshShell();                         // Создание оболочки по сетке кривых     
void CreateRuledShell();                        // Создание линейчатой оболочки по двум кривым
void CreateExtensionShell();                    // Продлить грань вдоль указанных рёбер 
void CreateFacesFillet ( MbeSmoothForm form );  // Создание тела по грани скругления между двумя гранями 
void CreateThreeFacesFillet();                  // Создание тела по граням скругления между тремя наборами граней
void CreateFilletByBorders();                   // Создание тела на основе граней скругления по заданным граничным кривым.
void CreateShellSew();                          // Создание новой оболочки сшивкой. Stitch faces to a shell.
void CreateSolidSew();                          // Создание нового тела сшивкой. Stitch solids to a solid.
bool MakeSolidsSilhouetteContour();             // Создание оболочек, путем разрезки тела силуэтным контуром.
bool MakeLatheElements( MbPlacement3D & position ); // Построение элементов токарного сечения и оси вращения.
bool MakeLatheElement( const MbPlacement3D & position, VERSION version ); // Построение следа от вращения ребра тела. 
bool MakeLatheContour( const MbPlacement3D & position ); // Построение контура по элементам токарного сечения.

// Создание плоских объектов

void MakeSolidContourSectionMap();      // Создание вида контурного сечения
void MakeSolidLocalSectionMap();        // Создание вида местного разреза
bool MakeCurveHideMap();                // Создание проекции кривой
void MakeSurfaceHideMap();              // Создание плоской проекции поверхности
void MakeSolidHideMap( bool one );      // Создание плоской проекции тела
void MakeAllSolidsHideMap();            // Создание плоской проекции всех тел
void MakeOutlineOfSolids();               // Создание очерка силуэтов тел
bool CreateEdgeProjection( bool plan ); // Проецирование ребра на плоскую грань

// Создание вспомогательных(конструктивных) объектов

bool CreateConstuctiveByPoints();       // Создание вспомогательного объекта по трем точкам

// Создание объектов

void SplitSolidFaces();                 // Создание тела c подразбиением грани
void CreateAssembly();                  // Создать сборку. 
void CreateInstance();                  // Создать вставку. 
void CreateMesh();                      // Создать полигональный объект вместо указанного. // Create polygonal object instead a pointed.
void CreateGrid();                      // Создать полигональный объект по триангуляции грани тела. // Create polygonal object by solid face triangulation.
void CreateConvexPolyhedron();          // Создать абрис объект.

// Ограничения

void CreateMateConstraint( int mateType );  // Создать сопряжение
void AddCoincident2Points();            // Совпадение пары точек
void AddTangency();
void AddPerpendicular();

// Создание объектов из листового материала

void CreateSheetSolid( int type );      // Создание тела из листового материала
void BendSheetSolidOverSegment();       // Сгиб вдоль отрезка тела из листового материала
void SheetSolidJog();                   // Подсечка листового тела
void BendSheetSolidByEdges();           // Сгиб по рёбрам тела из листового материала
void SheetSolidJointBend();             // Комбинированный сгиб (сгиб по эскизу)
void BendAnySolid();                    // Сгиб нелистового тела
void BendSheetSolid();                  // Сгиб тела из листового материала
void UnbendSheetSolid();                // Выпрямление сгиба тела из листового материала
void UnbendAllBends();                  // Выпрямление всех сгибов тела из листового материала
void CloseCorner();                     // Замыкание угла при сгибе
void SheetSolidStamp();                 // Штамповка листового тела
void SheetSolidStampWithTool();         // Штамповка листового тела телом-инструментом.
void SheetSolidBead();                  // Буртик
void SheetSolidJalousie();              // Жалюзи
void CreateRuledSolid();                // Обечайка
void CreateRadiusDimension3D();         // 
void CreateSheetRib();                  // Создание ребра жёсткости для листового тела.
void SimplifySheetPattern();            // Упрощение развёртки листового тела
void NormalizeSheetHoles();             // Нормализация вырезов листового тела
void BuildBends3DAxisLines();           // Построение линии сгибов
void BuildSheetMetalByArbitarySolid();  // Построить листовое тело на основе произвольного тела.

// Создание объектов пользователя, слоты для испытания примеров

void MakeUserCommand0(); // Создание пользователя
void MakeUserCommand1(); // Создание пользователя
void MakeUserCommand2(); // Создание пользователя
void MakeUserCommand3(); // Создание пользователя
void MakeUserCommand4(); // Создание пользователя
void MakeUserCommand5(); // Создание пользователя
void MakeUserCommand6(); // Создание пользователя
void MakeUserCommand7(); // Создание пользователя
void MakeUserCommand8(); // Создание пользователя
void MakeUserCommand9(); // Создание пользователя

// Тестирование многопоточности

void BetaMultithreaded0( bool internalParallel = false ); // Многопоточная команда
// Тест для многопоточного вычисления сеток (если compareSerial == true, сравнить результаты двух последовательных вычислениий)
void MultithreadingTest( bool compareSerial = false );
void MultithreadingProjection(); // Тест для многопоточного проецирования базовых точек объекта на поверхность.
void MultithreadedGetVestiges( int mode = 2 ); // Распараллеливание построения проекций вида
void MultithreadedGetVestiges2( int mode = 2 ); // Многопоточное построение нескольких проекций

void TestThreadSafety();

void MassInertiaCompare();

#endif // __TEST_SPACE_H
