///////////////////////////////////////////////////////
//
// Функции вычисления
//
// Computation functions
//
///////////////////////////////////////////////////////

#ifndef __TEST_COMPUTATION_H
#define __TEST_COMPUTATION_H

#include <test_variables.h>


class    MbSurface; 
class    MbFace; 


//   вычисление геометрических свойств объектов
//   calculation of the geometric characteristics of objects

// Положение точки относительно тела
//Location of a point relative to body 
bool PointLocation();

// Расстояние от точки до точки
// Distance between points
bool PointDistanceToPoint();

// Вычислить расстояние от точки до объекта
// Calculate distance from a point to the object
bool DistanceToPointObject();

// Вычислить длину кривой
// Calculate length of the curve
bool CurveLength();

// Вычислить длину каркаса
// Calculate length of the wire frame
bool WireFrameLength();

// Вычислить радиус кривизны кривой
// Calculate curveture radius
bool CurveRadius();

// Угол между гранями ребра 
// Angle between the faces adjacent to the edge
bool GetEdgeFacesAngle();

// Угол между двумя ребрами. 
// Angle between two edges.
bool GetEdgeEdgeAngle();

// Определение массо-центровочных характеристик тела
// Calculation of the solid's mass-inertia characteristics
void SolidMassInertia();

// Определение массо-центровочных характеристик полигональных объектов
// Calculation of mass-inertia characteristics of polygonal objects
void MeshMassInertia();

// Расчет центра поверхности грани.
// Calculaion of the center of the face's surface
void FaceCentre();

// Расчет элементов дерева объемов
// Compute elements of the bounding volume tree
void ComputeBVTree();

// Расчет коллизий тел в статике
// Calculate collisions
void CollisionDistanceQuery();

// Расчет коллизий тел.
// Calculate collisions.
void CollisionInterferenceSolids();

// Расчет коллизий полигональных моделей.
// Calculate collisions of the meshes.
void CollisionInterferenceMeshes();

// Расчет коллизий методами булевых операций.
// Calculate collisions by the boolean operation method.
void InterferenceSolids();

//------------------------------------------------------------------------------
// Метод теста на интерференции.
//---
enum InterferenceTest
{
    IT_Undefined = 0
  , IT_FindTouchedFaces = 2
  , IT_SplitTouchedFaces = 3
  , IT_TouchedSolidsMerging = 4
};

//------------------------------------------------------------------------------
// Тесты на интерференцию геометрических объектов.
// Interference tests of solids and subassemblies.
//---
void InterferenceDetection( InterferenceTest test );

// Определение площади замкнутой кривой
// Calculate the area, bounded by the curve
void ContourArea();

// Проверить скорость работы поверхности
// Test the performance of the surface
bool CheckSurfaceSpeed();

// Проверить скорость работы пространственной кривой
// Test the performance of the spatial curve
bool CheckCurve3DSpeed();

// Проверить скорость работы двумерной кривой
// Test the performance of the planar curve
bool CheckCurve2DSpeed();

// Проверить эскиз.
// Check sketch.
bool CheckSketch();

// Проверить двумерную кривую на самопересечение
// Self-intersection test of the planar curve
bool CheckCurve2DSelfInt();

// Проверить трехмерную кривую на самопересечение
// Self-intersection test of the space curve
bool CheckCurve3DSelfInt();

// Проверить поверхность на самопересечение.
// Self-intersection test of the surface.
bool CheckSurfaceSelfInt();

// Проверить оболочку на самопересечение.
// Self-intersection test of the shell.
bool CheckShellSelfInt();

// Создать операционную разбивку поверхности
// Create the surface's tesselation for operations
bool CreateSurfaceOperTesselation();

// Расстояние между осями поверхностей, имеющих оси
// Distance between the axes of the objects, which have ones
bool SurfAxesDistance();             


// Минимальное расстояние между геометрическими объектами
// Minimal distance between the geometrical objects
bool GeomItemsMinDistance();

// Минимальное расстояние между топологическими объектами
// Minimal distance between the topological objects
bool TopoItemsMinDistance();

// Минимальное расстояние между телами
// Minimal distance between the solids
bool SolidsMinDistance();

// Максимальное расстояние между геометрическими объектами
// Maximal distance between the geometrical objects
bool GeomItemsMaxDistance( const MbSpaceItem * gitem1, 
                           const MbSpaceItem * gitem2 );

// Максимальное расстояние между топологическими объектами
// Maximal distance between the topological objects
bool TopoItemsMaxDistance();

// Расстояния между поверхностями по сетке
// Distance between the objects' grids
bool GriddedSurfMinMaxDistances();

// Максимальные по модулю кривизны поверхности.
// Maximum modulus of surface curvature.
void SurfaceMaxMinCurvatures();

// Максимальные по модулю кривизны оболочки.
// Maximum modulus of shell curvature.
void FacesMaxMinCurvatures();

// Максимальные по модулю кривизны кривой.
// Maximum modulus of curve curvature.
void CurveMaxMinCurvatures();

//   габарит объектов
//   Dimensions of objects

// Габарит кривой
// Dimensions of the curve
bool PutCurveGabarit();

// Габарит поверхности
// Dimensions of the surface
bool PutSurfaceGabarit();

// Габарит грани тела
// Dimensions of the solid's face
bool PutFaceGabarit();

// Габарит тела
// Dimensions of the solid
bool PutSolidGabarit();


// Габарит кривой относительно л.с.к.
// Dimensions of the planar curve relative to the placement
bool PutCurve2DLocalGabarit();

// Габарит кривой относительно л.с.к.
// Dimensions of the spatial curve relative to the placement
bool PutCurve3DLocalGabarit();

// Габарит поверхности относительно л.с.к.
// Dimensions of the surface relative to the placement
bool PutSurfaceLocalGabarit();

// Габарит ребра относительно л.с.к.
// Dimensions of the edge relative to the placement
bool PutEdgeLocalGabarit();

// Габарит грани относительно л.с.к.
// Dimensions of the solids's face relative to the placement
bool PutFaceLocalGabarit();

// Габарит тела относительно л.с.к.
// Dimensions of the solid relative to the placement
bool PutSolidLocalGabarit();


// Характерные точки и вектора объектов
// Characteristical points and vectors of objects

// Нормали поверхности
// Normals of the surface
bool PutSurfNormals( const MbSurface * surf, bool isSameSense );

// Нормали грани
// Normals of the face
bool PutFaceNormals( const MbFace * face );

// Нормали сеток
// Normals of the mesh
bool PutMeshNormals( const MbMesh * mesh );

//   очерк объектов
//   shiluette lines of objects

// Получение очерка поверхности
// Get the shiluelle lines of the surface
bool SurfacesHide();

// Получение очерка грани
// Get the shiluelle lines of the face
bool FaceHide();

// Получение очерка тела
// Get the shiluelle lines of the body
bool SolidsHide();


//   триангуляция объектов
//   tesselation of objects

// Запросить параметры для триангуляции
// Ask user for the tesselation parameters
bool InitStepData( MbStepData & resStepData, bool createParams, int defaultMode );

// Преобразовать поверхность в множество треугольных поверхностей
// Transform the surface into triangles
bool TriangulateSurface();

// Преобразовать тело в множество треугольных поверхностей
// Transform the body into triangles
bool TriangulateSolid();

// Преобразовать все тела в массивы треугольных поверхностей
// Transform all the bodies into containers of triangles
bool TriangulateAllSolids();

// Преобразовать грань тела в множество треугольных поверхностей
// Transform the face into triangles
bool TriangulateFace();

// Построить триангуляцию по облаку точек.
// Build triangulation by point cloud.
bool TriangulatePointCloud();

// Аппроксимировать грань тела треугольными пластинами и нарисовать а области параметров
// Approximate the face by triangles and draw the domain splitting
bool ParamTriangulateFace();

//   упрощение триангуляции
//   Simplification of tesseltation

// Упростить триангуляцию тела
// Simplify the solid's tesseltaion
bool SimplifySolidTrgn();


//   тонирование объектов
//   rendering of objects

// Раскрасить поверхности в отдельном окне
// Render surfaces in another window
void SurfaceRendering();

// Раскрасить твердые тела в отдельном окне
// Render solids in another window
void SolidRendering();

// Раскрасить модель в отдельном окне
// Render the model in another window
void ModelRendering();

//   развертки объектов
//   Domain unfolds

// Развертка кривых на поверхности
// Domain unfold of curves
void CurveOnSurfaceMap();


// Развертка множества кривых на поверхности
// Domain unfold of set of curves
void SetCurvesOnSurfaceMap();


// Развертка поверхности
// Domain unfold of the surface
void SurfaceBorderMap();

// Развертка твердого тела
// Domain unfold of the solid
void SolidFaceLoopMap();

// Развертка грани твердого тела
// Domain unfold of the solid's face
bool FaceLoopMap( const MbFace * = nullptr );


// Построение характеристической ломаной NURBS
// Constuction of the characteristic polyline of NURBS objects

// Построение характеристической ломаной NURBS
// Constuction of the characteristic polyline of NURBS curve
void LineSegmentNURBSCurve();

// Построение характеристической ломаной NURBS
// Constuction of the characteristic polyline of NURBS surface
void LineSegmentNURBSSurface();


// Построение каркасов
// Constuction of wire frames

// Построение каркаса
// Cunstruct a wire frame
void MakeSpaceWire();

// Построение плоского каркаса
// Cunstruct a planar wire frame
void MakePlaneWire();

// Построение каркаса на плоскости
// Make a wire frame on the plane
void MakeWireOnPlane();

// Построение каркаса из точек триангуляции
// Construct a wire frame based on the tesselation points
void MakeTriangulationPoints();

// Построение каркаса из линий триангуляции
// Construct a wire frame based on the tesselation lines
void MakeTriangulationWire();

// Построение каркаса из линий триангуляции
// Construct a wire frame based on the tesselation polygons
void MakeTriangulationPlates();

// Поднятие флага
// Set the flag
void UpPlotFlag();

// Векторный вывод
// Vector drawing
void PutPlot ( POINT point0, POINT point1 );

// Поднятие флага
// Set the raster flag
void UpRastrFlag();                          

// Растровый вывод
// Raster drawing
void PutRastr( POINT point0, POINT point1 );

// Векторный вывод в растр
// Vector drawing into raster
void PutVect ( POINT point0, POINT point1 );


// Построение резьбы
// Thread construction

// Без привязки к телу
// Thread without bind to the solid
void CreateNewThreadInSpace();

// С привязкой к телу
// Thread bind to the solid
void CreateNewThreadOnSolid();

// Построение отображения условного обозначения резьбы на плоскость
// Construction ot the thread proxy on the plane
void CreateNewThreadMappingByParams();

// Построение отображения условного обозначения резьбы на плоскость
// Construction ot the thread proxy on the plane
void CreateNewThreadMappingByThread();

// Сопрягаются ли резьбы
// A the thread mate
void ModifyThreadIsMatedThreads();

// Принадлежит ли резьба телу
// Is the thread bind to the solid
void ModifyThreadIsBodyThread();

// Адаптировать резьбу к телу
// Adapt the thread to the solid
void ModifyThreadAdaptToBody();


// Построение графиков
// Trends construction

// Построение эпюры кривизны кривой на самой кривой.
// Plotting curvature curve on the curve itself.
void CurveCurvatureProfile();

// Построение графика кривизны кривой
// Build the curve's curvature trend
void PlotCurveCurvature();

// Построение графика кручения кривой
// Build the curve's torsion trend
void PlotCurveTorsion();

// Построение эпюр производных поверхности по кривой на этой поверхности.
// The surface derivative plotting by the curve on this surface.
void SurfaceDerivativeProfile();

// Построение объема заполненного сосуда.
// The construction of the volume of flood fill.
void FloodFill();

// Количество уникальных объектов заданного типа.
void NumberOfUniqueObjects( MbeSpaceType type );

#endif // __TEST_COMPUTATION_H
