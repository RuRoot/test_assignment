///////////////////////////////////////////////////////
//
// Функции редактирования объектов в пространстве.
// Functions for editing objects in space.
//
///////////////////////////////////////////////////////

#ifndef __TEST_EDIT_SPACE_H
#define __TEST_EDIT_SPACE_H


// Редактирование объектов модели.  // Editing of the model objects.

bool MoveObject();                   // Сдвиг элемента.  // Move the element.
bool PlaneRotateObject();            // Поворот элемента в плоскости.  // Rotate the element in the plane.
bool LineRotateObject();             // Поворот элемента вокруг линии.  // Rotate the element around the line.
void ScaleObject();                  // Масштабировать элемент.  // Scale the element.
void TransformObject();              // Перевести в локальную систему. Transform by local system.
void DuplicateObject();              // Дублировать элемент.  // Duplicate the element.
void MirrorDuplicateObject();        // Зеркально дублировать элемент.  // Mirror duplicate the element.
void DeleteObject();                 // Удалить элемент.  // Delete the element.

void SetObjectColor( bool all );     // Изменить цвет элементов.  // Change the elements color.
void SetObjectWidth();               // Изменить толщину линий элементов.  // Change the line thickness of the elements.
void SetupDoubleMesh();              // Создавать полигональные объекты на числах double(true)/float(false). // Create polygonal object on double(true)/float(false) data.
void SetupItemsMerging();            // Установить слияние элементов.  // Merge the elements.
void InvisibleObject();              // Сделать объект невидимым.  // Make object invisible. 
void VisibleAllObjects();            // Показать все объекты.  // Show all objects.
void VisiblePoints();                // Показать точки.  // Show points.
void VisibleCurves();                // Показать кривые.  // Show curves.
void VisibleSurfaces();              // Показать поверхности.  // Show surfaces.
void VisibleBodies();                // Показать тела.  // Show solids.
bool EditProperty();                 // Редактирование параметров элемента.  // Edit parameters of the element.
bool RemoveBasis();                  // Удалить объект, оставив базовые объекты.  // Remove the object, leaving the base objects.
void AddBasisPoints();               // Дать базовые точки объекта.  // Give the base points of the object.
bool CutModelObjects();              // Разрезать модель.  // Cut the model.

void RebuildObject();                // Перестроить элемент.  // Rebuild the element.
void MakeIngot();                    // Превратить в слиток.  // Convert to ingot.
void MeshToSolid();                  // Преобразовать полигональную модель в тело.  // Convert the polygonal model into the solid.
void SimplifyMesh();                 // Упростить полигональную модель. // Simplify a mesh.
void ConvertToMesh();                // \ru Преобразовать объект в полигоны. \en Convert an object to the mesh. \~
bool TranslateMeshToGrids();         // Развалить полиглнальный объект на отдельные триангуляции. // Break mesh to grids and delete it.
bool KeepOnlyItems( int type );      // Оставить в объекте только пластины триангуляции (0), ломаные (1), апексы (2). // Keep in item only grids (0), polys (1), apexs (2).
void ConvertMeshToInstance();        // Заменить повторяющиеся меши на вставки. // Convert repetitive meshes to instances.
void SimplifyMeshTrianglesCount();   // Упростить триангуляцию до заданного количества треугольников. // Simplify mesh up to specified triangles count.
void SimplifyMeshTolerance();        // Упростить триангуляцию с заданной точностью. // Simplify mesh with specified tolerance.
void AlignMeshWithObject();          // Совместить триангуляцию с другим объектом. // Align a mesh with another object.

// Редактирование выделенных объектов модели.  // Editing of the selected model objects.

bool MoveSelectedObjects();          // Сдвиг выделенных элементов.  // Move selected elements.
bool PlaneRotateSelectedObjects();   // Поворот выделенных элементов в плоскости.  // Rotate selected elements in the plane.
bool LineRotateSelectedObjects();    // Поворот выделенных элементов вокруг линии.  // Rotate selected elements around the line.
void DeleteSelected();               // Удалить выделенные элементы.  // Delete selected elements.
void DeleteInvisible();              // Удаление все невидимые объекты. Delete all invisible objects.
void ScaleSelectedObjects();         // Масштабировать выделенные элементы.  // Scale selected elements.

// Редактирование точек.  // Editing of the points.

bool MovePointAlongLine();           // Сдвиг точки вдоль направления кривой.  // Shift of the point along the curve direction.

// Редактирование 3D кривых.  // Editing of the curves.

bool TranslateCurveToNurbs();                   // Преобразовать кривую в NURBS.  // Convert curve to NURBS.
bool AddSplineCurvePoint();                     // Задание новой точки кривой.  // Specify a new point on the curve.
bool ChangeSpaceSplinePoint();                  // Замена точки кривой.  // Change point of the curve.
void SpaceSplineSetClosed();                    // Замкнуть / разомкнуть кривую.  // Close / unclose a curve.
bool CurveInverse();                            // Изменить направление кривой на противоположное.  // Reverse curve direction.
bool CreateMidlePart();                         // Дать часть кривой.  // Give a part of the curve.
bool DeleteMidlePart();                         // Дать концевые части кривой.  // Give end parts of the curve.
bool CreateBreakParts();                        // Разбить кривую.  // Break a curve.
bool RemoveCurveBasis();                        // Удалить кривую, оставив базовые объекты.  // Remove the curve, leaving the base objects.
bool ConvertSpaceNurbsIntoOpen();               // Переопределение базисных точек NURBS из Close в Open при замкнутом узловом векторе.  // Overriding NURBS basis points from Close to Open with a closed nodal vector.
bool ConvertSpaceNurbsIntoClose();              // Переопределение базисных точек NURBS из Open в Close при замкнутом узловом векторе.  // Overriding NURBS basis points from Open to Close with a closed nodal vector.
bool AttachSpaceNurbsToObject();                // Состыковать сплайн с объектом.  // Attach NURBS to Object.
bool AddSpaceNurbsToNurbs();                    // Объединить два сплайна. Add NURBS to NURBS.
bool AddNurbsKnot();                            // Задание новый узел кривой.  // Add a new node to the curve.
bool ChangeSpaceNurbsDegree( bool degreeDown ); // Уменьшить\увеличить порядок nurbs кривой на 1. // Reduce\raise the order of the nurbs curve at 1.
bool RemoveNurbsKnote();                        // Удаление точки кривой.  // Delete a point of the curve.
bool ConjugateContourSegments();                // Гладко сопрячь сегменты контура. // Conjugate the contour segments smoothly.
bool CurveExtension();                          // Продлить кривую с одного из концов. // Extend the curve from one of its ends.

// Редактирование поверхностей.  // Editing of the surfaces.

bool TranslateSurfaceToNurbs();      // Преобразовать поверхность в NURBS.  // Translate surface to NURBS.
bool GoSurfaceThroughPoint();        // Провести поверхность через заданную точку.  // Conduct a NURBS surface through a given point.
bool GoSurfaceThroughPlane();        // Коснуться поверхностью заданной плоскости.  // Touch the NURBS surface to the specified plane.
bool RemoveSurfaceBasis();           // Удалить поверхность, оставив базовые объекты.  // Remove the surface, leaving the base objects.
bool ConvertNURBSSurfaceIntoOpen();  // Переопределение базисных точек NURBS из Close в Open при замкнутом узловом векторе.  // Overriding NURBS basis points from Close to Open with a closed nodal vector.
bool ConvertNURBSSurfaceIntoClose(); // Переопределение базисных точек NURBS из Open в Close при замкнутом узловом векторе.  // Overriding NURBS basis points from Open to Close with a closed nodal vector.
void SurfaceBasisPointProjection( bool multithreading = false);  // Спроецировать базовые точки объекта на поверхность.  // Project base points of the object on a surface or body.

// Редактирование твердых тел.  // Editing of the solids.

bool TranslateSolidToSurfaces();     // Развалить тело на поверхности граней и ликвидировать его.  // Break solid to surfaces and delete it.
bool TranslateSolidToFaces();        // Развалить тело на грани и ликвидировать его.  // Break solid to faces and delete it.
bool BreakSolidToBasis();            // Развалить тело на составные части.  // Break solid to parts.
bool DeleteItemCreators( bool exceptLast ); // Удалить историю построения.  // Delete item (solid, wireframe, pointframe) creators.
bool BreakSolidOnParts();            // Разделение тела на части.  // Break solid to parts.
bool RemoveInternalVolumes();        // Удалить внутренние объемы тела // Remove of internal body volumes
bool CreatePartSolid();              // Создать тело из частей тела.  // Create solid from parts.
bool CheckIdenticalBaseSufaces();    // Проверить и устранить наличие общих подложек.  // Check and correct identical base surfaces.
bool UnifyOwnComplanarFaces();       // Объединить компланарные грани тела.  // Unify coplanar solid faces.
bool RebuildSolid( bool selectCopyMode ); // Перестроить тело.  // Rebuild a solid.
bool RebuildAllSolids( bool selectCopyMode ); // Перестроить все тела и оболочки. // Rebuild all solids and shells.
bool RemoveSolidBasis();             // Удалить тело, оставив базовые объекты.  // Remove the solid, leaving the base objects.
bool CheckShellGeometry();           // Проверить оболочку тела.  // Check solid shell.
bool CheckAllShellsGeometry();       // Проверить оболочки всех тел.  // Check all solids shells.
void CheckShellsPointers();          // Проверить совпадение указателей в оболочках
void ShellReverse();                 // Вывернуть оболочку.  // Reverse a shell.
bool ResetSurfaceGabarit();          // Reset bounding boxes of surfaces.
bool ResetCurvesTemporaryData();     // Reset all temporary data of curves.
bool ResetIntCurveApproximation();   // Reset approximation of surfaces intersection curve
void ForcedChangeShellClosedness();  // Forced change closedness of a shell
bool RemoveInnerLoop();              // Remove inner loop
bool RepairInexactEdges();           // Try to repair inexact edges
bool RepairVertices();               // Try to repair vertices
bool SolidRepairing();               // Починить тело. Solid repairing. 
void DecomposeAllAssemblies();       // Разрушить сборки с подсборками на составляющие.  // Decompose assemblies and subassemblies.
void RemoveAllColorAndVisualAttributes(); // Удалить все визуальные свойства объектов.  // Remove all visual properties of objects.
bool IsSolidsIntersection();         // Предпроверка тел на пересечение.  // Precheck solids at the intersection.
bool IsConvexHullsIntersection();    // Проверка абрис триангуляций тел на пересечение.  // Check the outline of triangulation at the intersection.
void FindOverlappingSimilarFaces();  // Найти родительские грани оболочки.  // Find overlapping similar faces.
void FindFacesEdgesCarriers();       // Найти грани - потенциальные носители ребер.  // Find faces - potential carriers of edges.
void FindNearestSolid();             // Найти ближайшее тело. Для операции выдавливания до ближайшей оболочки. //  Find nearest solid. For an extrusion operation to the nearest shell.
void CheckFaceTriangulation();       // Проверить триангуляцию грани // Check triangulation of a face.
void AddVertexEdgesAsCurves();       // Показать все ребра, стыкующиеся в вершине, добавив в модель копии кривых.  // Show all edges connected in the vertex, adding to the model copies of the curves.
void AddSolidEdgesAsCurves();        // Показать все ребра тела, добавив в модель копии кривых.  // Show all edges of the solid, adding to the model copies of the curves.
void AddSolidVertsAsPoints();        // Показать все вершины тела, добавив в модель копии точек.  // Show all vertices of the solid, adding to the model copies of the points.
bool SetSolidLastVersion();          // Выставить последнюю версию.  // Set solid last version.
void CheckCurvesTangentMating();     // Проверить касание кривых.  // Check curves tangent mating.
void CheckCurvePolygonAccuracy();    // Проверить точность полигона кривой.  // Check curve polygon accuracy.
bool ShowCurveDirection();           // Показать направление кривой.  // Show curve direction.
bool ShowSurfaceDirections();        // Показать направления поверхности.  // Show surface directions.
bool ShowDegeneratedTrianglesAndGaps();      // Показать вырожденные треугольники сетки. // Show the degenerate triangles of the mesh.   
bool SolidRebuildTimeFunction();     // Перестроить тело и засечь время.  // Rebuild solid and detect the time.
void SolidEndMove( bool cut );       // Сдвинуть конец дерева построения не обрезая или обрезать. // Move end of construction tree without cutting, or cut.
void SolidFinish();                  // Достроить тело для последней невыполненной операции в журнале построения. // Build a body by the last omitted operation in the build log.
void SolidCreatorDelete();           // Удалить элемент дерева построения.  // Delete a construction tree element.
void SolidCreatorSetLastVersion();   // Установить последнюю версию элементу дерева. Set the latest version of the tree element.
void SolidFilletsSetLastVersion();   // Установить последнюю версию скруглениям. Set the latest version for fillets.
void InsertAttribToSolid();          // Добавить атрибут телу или его примитиву.  // Insert attribute to solid or its primitive.
bool RemoveSolidAttributes();        // Удалить атрибуты тела.  // Remove solid attributes.

// Редактирование проволочных каркасов.  // Editing of the wire frames.

bool RebuildWireFrame( bool selectCopyMode ); // Перестроить проволочный каркас.  // Rebuild a wire frame.
bool RebuildAllWireFrames( bool selectCopyMode ); // Перестроить все проволочные каркасы. // Rebuild all wire frames.

// Работа с именами объектов.  // Working with object names.       

bool GetEdgeNumberInFace();          // Номер ребра в грани с указанием номера цикла.  // Get edge number in the face indicating the loop number.
bool GetFaceNumber();                // Определение номера грани.  // Face number definition.
bool GetFaceFilletType();            // Определение типа грани скругления.  // Fillet face type definition.
bool GetEdgeNumber();                // Определение номера ребра.  // Edge number definition.
bool GetFaceByNumber();              // Определение грани по номеру.  // Define face by number. 
bool GetEdgeByNumber();              // Определение ребра по номеру.  // Define edge by number.  
bool RecognitionFilletFaces();       // Распознование граней скругления. // Recognition fillet faces. 
void RecognitionSpiralSurfaces();    // Распознавание спиральных поверхностей в теле.  // Recognition of spiral surfaces in the solid.
void SetShellNames();                // Выполнить именование оболочки   // Perform shell naming
void CheckShellNames();              // Проверить имена на совпадение.  // Check shell names for a match.
void CheckPointFrameNames();         // Проверить имена на совпадение.  // Check point frame names for a match.
void CheckWireFrameNames();          // Проверить имена на совпадение.  // Check wire frame names for a match.
bool CheckSurface();                 // Проверить производные поверхности. // Check surface derivatives.
void CheckShellMainNames();          // Проверить главные имена точек и рёбер. // Check main names of edges and vertices.
// Редактирование плоских объектов.  // Editing of the flat objects.

bool ViewRotateInObjectPlane();      // Повернуть плоскость вида плоскость в объекта.  // Rotate the view plane in the object plane.
void DrapeShell();                    // Драпировать оболочку.  // Drape shell.

void CreatePolymeshByMesh();             // Создать полигональную сетку по объекту MbMesh.  // Create polymesh by MbMesh object.
void CutPolymeshByPlane();               // Обрезать полигональную сетку плоскостью.  // Cut polymesh by plane.
void MakePolymeshBoolean( size_t oper ); // Булева операция с полигональными объектами.  // Boolean operation with polymeshes.
void RemeshPolymesh();                   // Ремешировать полигональную сетку.  // Remesh polymesh.
void SmoothPolymesh();                   // Сгладить полигональную сетку.  // Smooth polymesh.
void CalculateMeshNormals();             // Рассчитать нормали в вершинах сетки.  // Calculate normals at mesh vertices.
void CreatePolymeshBySolid();            // Создать полигональную сетку по объекту MbSolid.  // Create polymesh by MbSolid object.
void CutPolymeshByCurves();              // Обрезать полигональную сетку кривыми.  // Cut polymesh by curves.

#endif // __TEST_EDIT_SPACE_H
