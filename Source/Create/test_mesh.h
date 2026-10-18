///////////////////////////////////////////////////////
//
// Функции преобразования сетки в объект BRep.
//
// Functions to perform mesh to BRep conversion.
//
///////////////////////////////////////////////////////


#ifndef __TEST_MESH_H
#define __TEST_MESH_H


//------------------------------------------------------------------------------
// Рассчитать и отрисовать поле кривизн или главных направлений по сетке.
// Calculate and draw mesh curvature or principal curvature direction map.
// ---
void CreateMeshCurvatureMap( bool isCurvatureDir = false );


//------------------------------------------------------------------------------
// Удалить вспомогательную сетку.
// Delete a temporal mesh.
// ---
void DeleteTemporalMesh( bool );


//------------------------------------------------------------------------------
// Рассчитать и отрисовать карту сегментации.
// Calculate and draw segmentation map.
// ---
void CreateSegmentMap();


//------------------------------------------------------------------------------
// Создать по полигональной сетке объект BRep.
// Create the BRep object by a polygonal mesh.
// ---
void ConvertMeshToBRep();


//------------------------------------------------------------------------------
// Вписать поверхность в множество полигонов триангуляции.
// Fit the surface to polygon set.
// ---
void FitSurfaceToGrid();


//------------------------------------------------------------------------------
// Вписать поверхность в множество полигонов триангуляции с XYW ограничением.
// Fit the surface to polygon set with XYW constraint.
// ---
void FitSurfaceToGridXYW();


//------------------------------------------------------------------------------
// Вписать поверхность в множество полигонов триангуляции с Z ограничением.
// Fit the surface to polygon set with Z constraint.
// ---
void FitSurfaceToGridZ();


//------------------------------------------------------------------------------
// Вписать поверхность в множество полигонов триангуляции с XYZ ограничением.
// Fit the surface to polygon set with XYZ constraint.
// ---
void FitSurfaceToGridXYZ();


//------------------------------------------------------------------------------
// Вписать поверхность в множество полигонов триангуляции с ZW ограничением.
// Fit the surface to polygon set with ZW constraint.
// ---
void FitSurfaceToGridZW();


//------------------------------------------------------------------------------
// Вписать поверхность в множество полигонов триангуляции с ограничениями.
// Fit the surface to polygon set with constraints.
// ---
void FitSurfaceToGridWithConstraints();


//------------------------------------------------------------------------------
// Исправить несогласованную ориентацию полигонов сетки. 
// Repair inconsistent mesh polygons orientation.
// ---
void RepairInconsistentMesh();

//------------------------------------------------------------------------------
// Вывернуть полигональный объект.
// Reverse a polygonal object.
// ---
void ReverseMesh();

//------------------------------------------------------------------------------
// Перестроить полигональный объект. 
// Rebuild a polygonal object.
// ---
void RebuildMesh();

//------------------------------------------------------------------------------
// Удалить полигональный объект, оставив базовые объекты.
// Remove the polygonal object, leaving the base objects.
// ---
bool RemoveMeshBasis();

//------------------------------------------------------------------------------
// Проверить замкнутость полигонального объекта. 
// Check closeness of a polygonal object
// ---
void CheckMeshClosed();

#endif //__TEST_MESH_H
