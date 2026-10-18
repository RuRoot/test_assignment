//////////////////////////////////////////////////////////////
//
// Функции редактирования объектов на конструктивной плоскости.
// Functions for editing objects on a constructive plane.
//
//////////////////////////////////////////////////////////////

#ifndef __TEST_EDIT_PLANE_H
#define __TEST_EDIT_PLANE_H


// Редактирование кривых.  // Editing of the curves.

bool FilletCurves();                 // Скруглить кривые.  // Fillet curves.
bool ChamferCurves();                // Фаска на кривых.  // Chamfer on the curves.
bool DelCrvPart();                   // Удалить часть кривой.  // Delete a curve part.
bool DelCrvPartP1P2();               // Удалить часть от точки p1 до точки p2.  // Delete part from the point p1 to the point p2.
bool CreateTrimmCurve();             // Построение усеченной кривой.  // Create trimmed curve. 
bool InsertSplineKnot();             // Вставить узел.  // Insert a knot.
bool CreateInverseSpline();          // Инверсия сплайна.  // Create inverse spline.
void Splineset_Weights();            // Установить вес вершины.  // Set weight of the vertex.
void Splinenew_Point();              // Новая вершина.  // A new vertex.
void Splinechangepoint();            // Замена точки.  // Change point.
void SplineSetClosed();              // Замкнуть / разомкнуть кривую.  // Close / unclose a curve.
void SetNurbsDegree();               // Изменить порядок двумерной кривой NURBS. // Change degree of two-dimensional curve NURBS.
bool CreateBreakCurve();             // Разбить на две кривые.  // Break into two curves.
void CreateArcsByCurve();            // Апроксимация кривой дугами окружности.  // Approximation of the curve by arcs.
void Splinetangent();                // Касательная в точке.  // Tangent at the point.
bool ConvertPlaneNURBSIntoOpen();    // Переопределение базисных точек NURBS из Close в Open при замкнутом узловом векторе.  // Overriding NURBS basis points from Close to Open with a closed nodal vector.
bool ConvertPlaneNURBSIntoClose();   // Переопределение базисных точек NURBS из Open в Close при замкнутом узловом векторе.  // Overriding NURBS basis points from Open to Close with a closed nodal vector.

// Редактирование всех кривых конструктивной плоскости.  // Editing all curves on constructive plane.

void SelectCPlane();                 // Выделить элементы конструктивной плоскости.  // Select elements of the constructive plane.
void MoveCPlane();                   // Переместить конструктивную плоскость.  // Move a constructive plane.
void RotateCPlane();                 // Повернуть в своей плоскости.  // Rotate in the own plane.
void ComplanarCPlane();              // Совместить конструктивную плоскость с плоскостью вида.  // Combine the constructive plane with the view plane.
void TransformPlaneObject();         // Перевести в локальную плоскость. Transform by local plane.
void DeletePlaneObjects();           // Удаление всех объектов плоскости.  // Delete all objects of plane.
void Normalize();                    // Нормализовать двумерный объект.  // Normalize a two-dimensional object.

// Редактирование контура.  // Editing of the contour.

bool ContourSense();                 // Установить направление обхода контура.  // Set the direction of the contour bypass.
bool EliminateContourGaps();         // Устранить разрывы в контуре
bool DisassemblyContour();           // Разрушить контур на сегменты.  // Break contour into segments.
bool InsertPointInContour();         // Вставить точечную кривую в контур на плоскости.  // Insert a point curve in the contour on the plane.
bool CreateEnvelopeContour( int wParam );  // Построение охватывающего контура или выбор по стрелке.  //  Building a covering loop or selection of the arrow.

// Редактирование плоской мультилинии.  // Editing of the flat multiline.

bool ReplaceMltLinesBasisCurve   (); // Заменить базовую кривую мультилинии.  // Change the base curve of multiline.
bool InsertMltLinesVertex        (); // Вставить вершину в мультилинию.  // Insert a vertex in multiline.
bool RemoveMltLinesVertex        (); // Удалить вершину мультилинии.  // Remove the multiline vertex.
bool SetMltLinesHotPoint         (); // Сместить hot-точку базовой кривой мультилинии.  // Move the hot-point of the base curve of multiline.
bool DelMltLinesHotPoint         (); // Удалить hot-точку базовой кривой мультилинии.  // Remove the hot-point of the base curve of multiline.
bool SetMltLinesVertSmoothJoint  (); // Выставить флаг гладкий стык в вершине мультилинии.  // Set the flag smooth joint at the multiline vertex.
bool SetMltLinesVertTracingType  (); // Выставить тип обхода угла в вершине мультилинии.  // Set type of bypass of the angle at the vertex of multiline.
bool SetMltLinesVertSpecFilletRad(); // Выставить радиус специального скругления в вершине мультилинии.  // Set radius of the special fillet at the vertex of multiline.
bool SetMltLinesVertTipType      (); // Выставить тип законцовки в вершине мультилинии.  // Set type of ending at the vertex of multiline.
bool SetMltLinesVertTipDirection (); // Выставить направление законцовки в вершине мультилинии.  // Set direction of ending at the vertex of multiline.
bool SetMltLinesRadius           (); // Выставить радиус эквидистанты мультилинии.  // Set radius of the offset of multiline.
bool ChangeMltLinesRadius        (); // Изменить радиус эквидистанты мультилинии.  // Change radius of the offset of multiline.
bool AddMltLinesRadius           (); // Добавить радиус эквидистанты мультилинии.  // Add radius of the offset of multiline.
bool RemoveMltLinesRadius        (); // Удалить радиус эквидистанты мультилинии.  // Remove radius of the offset of multiline.
bool SetMltLinesBegTipType       (); // Выставить тип законцовки в начале мультилинии.  // Set type of the ending at the begin of multiline.
bool SetMltLinesBegTipParam      (); // Выставить параметр законцовки в начале мультилинии.  // Set parameter of the ending at the begin of multiline.
bool SetMltLinesEndTipType       (); // Выставить тип законцовки в конце мультилинии.  // Set type of the ending at the end of multiline.
bool SetMltLinesEndTipParam      (); // Выставить параметр законцовки в конце мультилинии.  // Set parameter of the ending at the end of multiline.
bool SetMltLinesProcessClosed    (); // Выставить использование замкнутости.  // Set the use of closeness.
bool FilletMltLinesBasisCurve    (); // Построить скругление базовой кривой мультилинии.  // Build the fillet of the base curve of multiline.
bool ChamferMltLinesBasisCurve   (); // Построить фаску базовой кривой мультилинии.  // Build the chamfer of the base curve of multiline.
bool BreakMultiline              (); // Разбить мультилинию на две части.  // Break multiline into two parts.
bool BreakMultilineNParts        (); // Разбить мультилинию на N частей.  // Break multiline into N parts.
bool DelMltLinesLastBasisSegment (); // Удалить последний сегмент мультилинии.  // Delete last segment of multiline.
bool DelMltPartP1P2              (); // Удалить часть мультилинии между точками.  // Delete part of the multiline between points.


#endif // __TEST_EDIT_PLANE_H
