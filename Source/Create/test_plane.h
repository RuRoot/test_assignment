//////////////////////////////////////////////////////////////
//
// Функции создания новых объектов на конструктивной плоскости
//
//////////////////////////////////////////////////////////////

#ifndef __TEST_PLANE_H
#define __TEST_PLANE_H


// Способы построения точки

bool CreateSimplePoint();           // Новая точка по координатам
bool CreateCrossPoint();            // Точка как пересечение двух элементов
bool CreatePointOnCurve();          // Точка на кривой
bool CreateNPointsOnCurve();        // Много точек на кривой
bool CreateCentrePoint();            // Выдать центр тяжести кривой
bool CreatePointsOnParamCurve();    // Много точек на кривой по параметру
bool IntersectCurves();             // Пересечение двух выбранных элементов
bool SelfIntersectCurve();          // Самопересечение произвольной кривой
bool TruncateCurvesByIntersection();// Разбить кривые точками пересечения. // Truncate curves by intersection points.

// Способы построения прямой линии

bool CreateGorisontalLine();        // Горизонтальная прямая
bool CreateVerticalLine();          // Вертикальная прямая
void CreateLinePointPoint();        // Прямая точка-точка
void CreateLinePoint0X();           // Прямая точка-угол с осью 0X
void CreateLinePointAngle();        // Прямая через точку под углом к прямой
void CreateLineParallelPoint();     // Прямая параллельная прямой через точку
void CreateLineParallelDistance();  // Прямая параллельная на расстоянии
bool CreateLinePointPerpCurve();    // Прямая перпендикулярная кривой через точку
void CreateLinePointTangentCurve(); // Прямая точка-касательная к кривой
void CreateLineTangentTwoCurves();  // Прямая касательная к двум окружностям
void CreateLineAngleTangentCurve(); // Прямые, проходящие под углом к оси 0X и касательные к кривой
void CreateLineBisector();          // Биссектрисса
bool CreateLineSegMinDistance();    // Отрезок - минимальное расстояние
bool CreateSegmentPointPoint();     // Отрезок точка-точка

// Способы построения окружности

void CreateCircleCentreRadius();            // Центр-Радиус
void CreateCircleCentrePoint();             // Центр-Точка на окружности
void CreateCircleCentreOnCurveRadPointOn(); // Центр на элементе-Радиус-точка
void CreateCircleCentreOnCurveTwoPoints();  // Центр на элементе-Две точки на окружности
void CreateCircleCentreTanCurve();          // Касание элемента-Центр
void CreateCircleTangentCurveTwoPoints();   // Касание элемента-Две точки на окр
void CreateCircleTangentCurveRPointOn();    // Касание элемента-Радиус-точка на окр
void CreateCircleTanTwoCurvesRadius();      // Касание двух элементов-Радиус
void CreateCircleTanTwoCurvesPointOn();     // Касание двух элементов-Точка на окр
void CreateCircleTanThreeCurves();          // Касание трех элементов
void CreateCircleRad2Points();              // Радиус-две точки
void CreateCircle3Points();                 // Три точки

// Способы построения дуги окружности

void CreateArc2PointsRadius();           // Две точки-Радиус
void CreateArc3Points();                 // Дуга по трем точкам
void CreateArcTangentCurveTwoPoints();   // Касание элемента-Две точки на окр
void CreateArcTangentCurveRPointOn();    // Касание элемента-Радиус-точка на окр
bool CreateArcCentre2Points();           // Дуга центр, начальная точка, конечная точка, направление
bool CreateArcCircle2Points();           // Дуга окружность, начальная точка, конечная точка, направление

// Способы построения кривых

bool CreateRectangle();                  // Прямоугольник
bool CreateRegularPolygon();             // Правильный многоугольник
bool CreateNurbs();                       // В-сплайн
bool CreateSpline( int curveType );      // Построение сплайна.
bool CreateTrimmSpline();                // Построение усеченного сплайна

// Построение конических сечений

bool CreateEllipseCentreAB();            // Эллипс - Центр - радиусы A и B
bool CreateEllipse3Points();             // Эллипс - Три точки
bool CreateEllipseCentre3Points();       // Эллипс - Центр - 3 точки
bool CreateEllipseTangent3Curves();      // Эллипс - Касание 2-х кривых
bool CreateEllipseArc();                 // Построение дуги эллипса
bool CreateConicArc();                   // Построение дуги коника

// Построение кривых на базе кривых

void Spline_Equid();                     // Эквидистанта к сплайну
void OffsetExtendedCurve();              // Продолженная эквидистанта
void PlaneCurveBySpace();                // Проекция пространственной кривой на конструктивную плоскость
void NurbsSpline();                      // Преобразование в NURBS
void CreateMedianCurve();                // Создание срединной кривой

// Построение кривых на базе тел

bool CreateEdgeProjection( bool plan );  // Проецирование ребра на плоскую грань

// Способы построения контура

bool NewContour();                 // Новый контур
bool ContourEquid();               // Построить эквидистанту к контуру
bool ContourFillet();              // Скруглить контур
bool ContourChamfer();             // Сделать фаски
bool ContourBreak();               // Разбить контур кривой на несколько новых контуров
bool ContourUnite();               // Объединение контуров
bool ContourCross();               // Пересечение контуров
bool ContourSubstr();              // Вычитание контуров
bool CreateAllEmbracingContours(); // Построение всех контуров
bool CreateEncloseContours();      // Построение контуров вокруг точки
bool CreateOuterContours();        // Построение контуров
bool ReconstructContours();        // Построение контуров
bool MakeOffsetContour();          // Построение эквидистантных контуров
bool MakeSlopeContour();           // Построение эквидистантных контуров для выдавливания под углом
bool MakeOffsetThinContour();      // Построение эквидистантных контуров для вращения вокруг оси
bool RegionsBooleanOper();         // Выполнение булевой операции над регионами
bool MakeUnionRegions();           // Объединение регионов (контуров)
bool TriangulateRegion();          // Триангуляция региона
bool MakeAllRegions();             // Разбиение произвольного набора контуров на связные группы контуров

// Создание составных объектов
// ...

// Построение мультилинии

bool CreateCntrsMultiline();       // Создание мультилинии на базе контура
bool CreateMultiline();            // Создание мультилинии построением базовой кривой

// Контур с разрывами

bool CreateContourWithBreaks();    // Создание контура с разрывами
bool AddBreakInContour();          // Добавить невидимый участок в контур с разрывами

// Геометричесике ограничения 2d
void AddCoincident2Points();       // Совпадение пары точек
void AddTangency();
void AddPerpendicular();


#endif // __TEST_PLANE_H
