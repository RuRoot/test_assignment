#include <test.h>
#include <test_variables.h>
#include <test_plane.h>
#include <test_edit_plane.h>
#include <last.h>

using namespace c3d;


//------------------------------------------------------------------------------
// Построение и редактирование объектов на плоскости
// ---
bool CreatePlaneObjects( uint wParam ) {

  if ( TestVariables::viewManager==nullptr )
    return false;

  switch ( wParam ) {

    // ************************** различные способы задания точек

    case CM_POINTPOSITION : // Новая точка по координатам
      while ( CreateSimplePoint() ) ;
      break;

    case CM_POINTINTERSECT : // Точка как пересечение двух элементов
      while ( CreateCrossPoint() ) ;
      break;

    case CM_POINTON_CURVE : // Точка на кривой
      while ( CreatePointOnCurve() ) ;
      break;

    case CM_N_POINTS_ON_CURVE : // Много точек на кривой
      while ( CreateNPointsOnCurve() ) ;
      break;

    case CM_POINTONPARAMCURVE : // Много точек на кривой по параметру
      while ( CreatePointsOnParamCurve() ) ;
      break;

    case CM_POINTWEIGHTCENTRE : // Выдать центр тяжести кривой
      while ( CreateCentrePoint() ) ;
      break;

    // ************************** построение пересечений

    case CM_EDITINTERSECT : // Пересечение двух выбранных элементов
      while ( IntersectCurves() ) ;
      break;

    case CM_EDITSELF_INTERSECT : // Самопересечение произвольной кривой
      while ( SelfIntersectCurve() ) ;
      break;

    // ************************** различные способы задания прямых

    case CM_GORISONTALLINE :    // Горизонтальная прямая
      while ( CreateGorisontalLine() ) ;
      break;

    case CM_VERTICALLINE :      // Вертикальная прямая
      while ( CreateVerticalLine() ) ;
      break;

    case CM_LINEPNTPNT :    // Прямая точка-точка
      CreateLinePointPoint();
      break;

    case CM_LINEPNTANLGE0X :  // Прямая точка-угол с осью 0X
      CreateLinePoint0X();
      break;

    case CM_LINEPOINTANGLE : // Прямая через точку под углом к прямой
      CreateLinePointAngle();
      break;

    case CM_LINEPRLPNT :    // Прямая параллельная прямой через точку
      CreateLineParallelPoint();
      break;

    case CM_LINEPNTDIST :   // Прямая параллельная на расстоянии
      CreateLineParallelDistance();
      break;

    case CM_LINEPRPPNT :    // Прямая перпендикулярная кривой через точку
      while ( CreateLinePointPerpCurve() ) ;
      break;

    case CM_LINEPNTTANG :   // Прямая точка-касательная к кривой
      CreateLinePointTangentCurve();
      break;

    case CM_LINETAN2CIR :   // Прямая касательная к двум окружностям
      CreateLineTangentTwoCurves();
      break;

    case CM_LINEANGLETAN_CIRCLE : // Прямые, проходящие под углом к оси 0X и касательные к кривой
      CreateLineAngleTangentCurve();
      break;

    case CM_LINEBISECTOR : // Биссектрисса
      CreateLineBisector();
      break;

    // ************************** различные способы построения окружностей

    case CM_CIRCLECENTRERADIUS : // Центр-Радиус
//      DWORD err = GetLastError();
      CreateCircleCentreRadius();
      break;

    case CM_CIRCLECENTREPOINTON : // Центр-Точка на окружности
      CreateCircleCentrePoint();
      break;

    case CM_CIRCLECENTREONRADIUSPOINTON : // Центр на элементе-Радиус-точка
      CreateCircleCentreOnCurveRadPointOn();
      break;

    case CM_CIRCLECENTREONTWOPOINTS : // Центр на элементе-Две точки на окружности
      CreateCircleCentreOnCurveTwoPoints();
      break;

    case CM_CIRCLETANENTITYCENTRE : // Касание элемента-Центр
      CreateCircleCentreTanCurve();
      break;

    case CM_CIRCLETANENTITYTWOPOINTS : // Касание элемента-Две точки на окр
      CreateCircleTangentCurveTwoPoints();
      break;

    case CM_CIRCLETANENTITYRADIUSPOINTON : // Касание элемента-Радиус-точка на окр
      CreateCircleTangentCurveRPointOn();
      break;

    case CM_CIRCLETANTWOENTITIESRADIUS : // Касание двух элементов-Радиус
      CreateCircleTanTwoCurvesRadius();
      break;

    case CM_CIRCLETANTWOENTITIESPOINTON : // Касание двух элементов-Точка на окр
      CreateCircleTanTwoCurvesPointOn();
      break;

    case CM_CIRCLETANTHREEENTITIES : // Касание трех элементов
      CreateCircleTanThreeCurves();
      break;

    case CM_CIRCLERADIUSTWOPOINTS : // Радиус-две точки
      CreateCircleRad2Points();
      break;

    case CM_CIRCLETHREEPOINTS : // Три точки
      CreateCircle3Points();
      break;

    // ************************** различные способы построения отрезков

    case CM_NEWSEGMENTPOINTPOINT : // Отрезок точка-точка
      while ( CreateSegmentPointPoint() ) ;
      break;

    case CM_NEWLINESEGDMIN : // Отрезок - минимальное расстояние
      while ( CreateLineSegMinDistance() ) ;
      break;

    // ************************** построение усеченной кривой

    case CM_FILLETCURVES :  
      while ( FilletCurves() );         // Скруглить кривые
      break;

    case CM_CHAMFERCURVES :  
      while ( ChamferCurves() );        // Фаска на кривых
      break;

    case CM_DELETEPART :  // Удалить часть
      while ( DelCrvPart() ) ;
      break;

    case CM_DELETEPARTP1P2 :  // Удалить часть от точки p1 до точки p2
      while ( DelCrvPartP1P2() ) ;

    case CM_NEWTRIMMEDCURVE : // Вырезать
      while ( CreateTrimmCurve() ) ;
      break;

    case CM_BREAKCURVE :  // Разбить
      while ( CreateBreakCurve() ) ;
      break;

    case CM_TRUNCATECURVES : // Разбить кривые точками пересечения. // Truncate curves by intersection points.
      while ( TruncateCurvesByIntersection() ) ;
      break;

    // ************************** различные способы построения точечно-заданных кривых

    case CM_NEWPOLYLINE : // Полилиния по точкам
      while ( ::CreateSpline( (int)pt_Polyline ) ) {}
      break;

    case CM_RECTANGLE : // Прямоугольник
      while ( CreateRectangle() ) ;
      break;

    case CM_REGULARPOLYGON : // Правильный многоугольник
      while ( CreateRegularPolygon() ) ;
      break;

    case CM_NEWBSPLINE : // В-сплайн
      while ( CreateNurbs() ) ;
      break;

    case CM_NEWCBSPLN2D : // Кубический снлайн
      while ( ::CreateSpline( (int)pt_CubicSpline) ) {}
      break;

    case CM_NEWHERMIT : //  Сплайн Эрмита
      while ( ::CreateSpline( (int)pt_Hermit ) ) {}
      break;

    case CM_NEWBEZIER : // Безье-сплайн
      while ( ::CreateSpline( (int)pt_Bezier ) ) {}
      break;

    // ************************** различные способы построения дуги окружности

    case CM_ARC2POINTSRADIUS : // Две точки-Радиус
      CreateArc2PointsRadius();
      break;

    case CM_NEWARC_3_POINTS : // Дуга по трем точкам
      CreateArc3Points();
      break;

    case CM_ARCTANGENTCURVE2POINTS : // Касание элемента-Две точки на окр
      CreateArcTangentCurveTwoPoints();
      break;

    case CM_ARCTANGENTCURVERADPOINTON : // Касание элемента-Радиус-точка на окр
      CreateArcTangentCurveRPointOn();
      break;

    case CM_ARCCENTRE2POINTS : // Дуга центр, начальная точка, конечная точка, направление
      while ( CreateArcCentre2Points() ) ;
      break;

    case CM_ARCCIRCLE2POINTS : // Дуга окружность, начальная точка, конечная точка, направление
      while ( CreateArcCircle2Points() ) ;
      break;

    // ************************** построение конических сечений

    case CM_ELLIPSECENTRE_AB        : // Центр - радиусы A и B
      while ( CreateEllipseCentreAB() ) ;
      break;

    case CM_ELLIPSE3POINTS          : // Три точки
      while ( CreateEllipse3Points() ) ;
      break;

    case CM_ELLIPSECENTRE3POINTS    : // Центр - 3 точки
      while ( CreateEllipseCentre3Points() ) ;
      break;

    case CM_ELLIPSE_TANGENT2CURVES  : // Касание 2-х кривых
      while ( CreateEllipseTangent3Curves() ) ;
      break;

    case CM_NEWELLIPSEARC : // Дуга эллипса
      while( CreateEllipseArc() );
      break;

    case CM_NEWCONIC : // Дуга коника
      while ( CreateConicArc() ) ;
      break;

    case CM_SPLINE_EQUID : // Эквидистанта к сплайну
      Spline_Equid();
      break;

    case CM_NEWOFFSETEXTENDED :
      OffsetExtendedCurve(); // Продолженная эквидистанта
      break;

    case CM_NEWPLANECURVEBYSPACE :
      PlaneCurveBySpace();   // Проекция пространственной кривой на конструктивную плоскость
      break;

    case CM_SPLINENURBS : // Преобразование в NURBS
      NurbsSpline();
      break;

    case CM_MEDIANCURVE : // Создание срединной кривой.
      CreateMedianCurve();
      break;

    case CM_NEWEDGEMAPONPLANE : // Проецирование ребра на плоскую грань
      while ( CreateEdgeProjection( true ) );        
      break;


    // ************************** построение контуров

    case CM_NEWCONTOUR : // Новый контур
      while ( NewContour() ) ;
      break;

    case CM_HANDENVELOPE      : // Выбор по стрелке
      while ( CreateEnvelopeContour( (int)wParam ) ) ;
      break;

    case CM_CONTOUREQUID : // Построить эквидистанту к контуру
      while ( ContourEquid() ) ;
      break;

    case CM_MAKEOFFSETCONTOUR :
      while (MakeOffsetContour() );  // Построение эквидистантных контуров
      break;

    case CM_MAKESLOPECONTOUR :
      while (MakeSlopeContour() );   // Построение эквидистантных контуров для выдавливания под углом
      break;

    case CM_MAKEOFFSETTHINCONTOUR :
      while (MakeOffsetThinContour() );// Построение эквидистантных контуров для вращения вокруг оси
      break;

    case CM_CONTOURFILLET : // Скруглить контур
      while ( ContourFillet() ) ;
      break;

    case CM_CONTOURCHAMFER : // Сделать фаски
      while ( ContourChamfer() ) ;
      break;

    case CM_CONTOURBREAK : // Разбить контур кривой на несколько новых контуров
      while ( ContourBreak() ) ;
      break;

    case CM_CONTOURUNITE : // Объединение контуров
      while ( ContourUnite() ) ;
      break;

    case CM_CONTOURCROSS : // Пересечение контуров
      while ( ContourCross() ) ;
      break;

    case CM_CONTOURSUBSTR : // Вычитание контуров
      while ( ContourSubstr() ) ;
      break;

    case CM_CONTOURSENSE : //  установить направление обхода контура
      while ( ContourSense() ) ;
      break;

    case CM_ELIMINATE_CONTOUR_GAPS : // Устранить разрывы в контуре
      EliminateContourGaps();
      break;

    case CM_DISASSEMBLY :  // Разрушить контур на сегменты.
      while ( DisassemblyContour() ) ;
      break;

    case CM_INSERTPOINTINCONTOUR :   // Вставить точечную кривую в контур на плоскости.
      while ( InsertPointInContour() ); 

    case CM_ALL_EMBRACING_CNTRS : // Построение всех контуров
      CreateAllEmbracingContours();
      break;

    case CM_NEWENVELOPCONTOUR : // Построение охватывающего контура
    case CM_ENCLOSECONTOURS : // Построение контуров вокруг точки
      while (CreateEncloseContours() );
      break;

    case CM_OUTERCONTOURS : // Построение контуров
      CreateOuterContours();
      break;

    case CM_RECONSTRUCTCONTOURS : // Построение контуров
      ReconstructContours();
      break;

    // ************************** построение областей

    case CM_REGIONSBOOLEANOPER :
      RegionsBooleanOper(); // Выполнение булевой операции над регионами
      break;

    case CM_MAKE_UNION_REGIONS :
      MakeUnionRegions(); // Объединение регионов (контуров)
      break;

    case CM_MAKE_ALL_REGIONS :
      MakeAllRegions(); // Разбиение произвольного набора контуров на связные группы контуров
      break;

    case CM_CREATECNTRSMULTILINE :
      CreateCntrsMultiline(); // Создание мультилинии на базе контура
      break;

    case CM_CREATEMULTILINE :
      CreateMultiline(); // Создание мультилинии построением базовой кривой
      break;

    case CM_NEWCONTOURWITHBREAKS :
      CreateContourWithBreaks(); // Создание контура с разрывами на базе контура
      break;

    case CM_ADDINVISIBLEPART :
      AddBreakInContour(); // Добавить невидимый участок в контур с разрывами
      break;

    case CM_REPLACEMLTLINESBASISCURVE :
      ReplaceMltLinesBasisCurve(); // Заменить базовую кривую мультилинии
      break;

    case CM_INSERTMLTLINESVERTEX :
      InsertMltLinesVertex(); // Вставить вершину в мультилинию
      break;

    case CM_REMOVEMLTLINESVERTEX :
      RemoveMltLinesVertex(); // Удалить вершину мультилинии
      break;

    case CM_SETMLTLINESHOTPOINT :
      SetMltLinesHotPoint(); // Сместить hot-точку базовой кривой мультилинии
      break;

    case CM_DELMLTLINESHOTPOINT :
      DelMltLinesHotPoint(); // Удалить hot-точку базовой кривой мультилинии
      break;

    case CM_SETMLTLINESVERTSMOOTHJOINT :
      SetMltLinesVertSmoothJoint(); // Выставить флаг гладкий стык в вершине мультилинии
      break;

    case CM_SETMLTLINESVERTTRACINGTYPE :
      SetMltLinesVertTracingType(); // Выставить тип обхода угла в вершине мультилинии
      break;

    case CM_SETMLTLINESVERTSPECFILLETRAD :
      SetMltLinesVertSpecFilletRad(); // Выставить радиус специального скругления в вершине мультилинии
      break;

    case CM_SETMLTLINESVERTTIPTYPE :
      SetMltLinesVertTipType(); // Выставить тип законцовки в вершине мультилинии
      break;

    case CM_SETMLTLINESVERTTIPDIR :
      SetMltLinesVertTipDirection(); // Выставить направление законцовки в вершине мультилинии
      break;

    case CM_SETMLTLINESRADIUS :
      SetMltLinesRadius(); // Выставить радиус эквидистанты мультилинии
      break;

    case CM_CHANGEMLTLINESRADIUS :
      ChangeMltLinesRadius(); // Изменить радиус эквидистанты мультилинии
      break;

    case CM_ADDMLTLINESRADIUS :
      AddMltLinesRadius(); // Добавить радиус эквидистанты мультилинии
      break;

    case CM_REMOVEMLTLINESRADIUS :
      RemoveMltLinesRadius(); // Удалить радиус эквидистанты мультилинии
      break;

    case CM_SETMLTLINESBEGTIPTYPE :
      SetMltLinesBegTipType(); // Выставить тип законцовки в начале мультилинии
      break;

    case CM_SETMLTLINESBEGTIPPARAM :
      SetMltLinesBegTipParam(); // Выставить параметр законцовки в начале мультилинии
      break;

    case CM_SETMLTLINESENDTIPTYPE :
      SetMltLinesEndTipType(); // Выставить тип законцовки в конце мультилинии
      break;

    case CM_SETMLTLINESENDTIPPARAM :
      SetMltLinesEndTipParam(); // Выставить параметр законцовки в конце мультилинии
      break;

    case CM_SETMLTLINESPROCESSCLOSED :
      SetMltLinesProcessClosed(); // Выставить использование замкнутости
      break;

    case CM_FILLETMLTLINESBASISCURVE :
      FilletMltLinesBasisCurve(); // Построить скругление базовой кривой мультилинии
      break;

    case CM_CHAMFERMLTLINESBASISCURVE :
      ChamferMltLinesBasisCurve(); // Построить фаску базовой кривой мультилинии
      break;

    case CM_BREAKMLTLINE :
      BreakMultiline();  // Разбить мультилинию на две части
      break;

    case CM_BREAKMLTLINENPARTS :
      BreakMultilineNParts();  // Разбить мультилинию на N частей
      break;

    case CM_DELMLTLINESLASTSEG :
      DelMltLinesLastBasisSegment();  // Удалить последний сегмент мультилинии
      break;

    case CM_DELETEPARTMLTLINEP1P2 :  
      DelMltPartP1P2(); // Удалить часть кривой мультилинии между точками
      break;

      // ************************** построение

    case CM_SPLINESET_WEIGHTS : // Установить вес вершины
      Splineset_Weights();
      break;

    case CM_SPLINENEW_POINT : // Новая вершина
      Splinenew_Point();
      break;

    case CM_SPLINETANGENT : // Касательная в точке
      Splinetangent();
      break;

    case CM_SPLINETRIMM : // Построение усеченного сплайна
      while ( CreateTrimmSpline() ) ;
      break;

    case CM_SPLINEINVERSE : // Инверсия сплайна
      while ( CreateInverseSpline() ) ;
      break;

    case CM_SPLINECHANGEPOINT : // Замена точки
      Splinechangepoint();
      break;

    case CM_SPLINESETCLOSED : // Замкнуть / разомкнуть кривую
      SplineSetClosed();
      break;

    case CM_SETDEGREE : // Изменить порядок
      SetNurbsDegree();
      break;

    case CM_SPLINEINSERTKNOT : // Вставить узел
      while ( InsertSplineKnot() ) ;
      break;

    case CM_DELETEALLPLANEOBJECT :
      DeletePlaneObjects();
      break;

    case CM_NORMALIZEOBJECT : // Нормализовать двумерный объект
      Normalize();
      break;

    case CM_APPARCCURVE :     // Апроксимация кривой дугами окружности
      CreateArcsByCurve(); 
      break;

    case CM_SELECTBYCPLANE : // Выделить элементы конструктивной плоскости
      SelectCPlane();
      break;

    case CM_MOVECPLANE :    // Переместить конструктивную плоскость
      MoveCPlane();
      break;

    case CM_EDITOWNPLANEROTATE : // Повернуть в своей плоскости. Rotate in the own plane.
      RotateCPlane();
      break;

    case CM_ROTATEPLANEINVIEW : // Совместить конструктивную плоскость с плоскостью вида. Combine the constructive plane with the view plane.
      ComplanarCPlane();
      break;

    case CM_TRANSFORMPLANEOBJECT: // Перевести в локальную плоскость. Transform by local plane.
      TransformPlaneObject();
      break;

    case CM_DELETECONSTRUCTIVEPLANE : // Удаление всех объектов плоскости
      DeletePlaneObjects();
      break;

    case CM_CONVERTPLANENURBSINTOOPEN : 
      ConvertPlaneNURBSIntoOpen();       // Переопределение базисных точек NURBS из Close в Open при замкнутом узловом векторе
      break;

    case CM_CONVERTPLANENURBSINTOCLOSE : 
      ConvertPlaneNURBSIntoClose();      // Переопределение базисных точек NURBS из Open в Close при замкнутом узловом векторе
      break;

    default:
      return false;

  }

  return true;

}

