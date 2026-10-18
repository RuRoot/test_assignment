////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Результат операции.
         \en Operation result. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MB_OPERATION_RESULT_H
#define __MB_OPERATION_RESULT_H


//------------------------------------------------------------------------------
/** \brief \ru Код результата операции.
           \en Operation result code. \~
  \details \ru Код результата операции. \n
           \en Operation result code. \n \~
  \ingroup Base_Items
*/
// \ru ДОБАВЛЯТЬ ТОЛЬКО В КОНЕЦ (ИДЕНТИФИКАТОРЫ СООБЩЕНИЙ СОХРАНЯЮТСЯ В ФАЙЛ) \en TO ADD ONLY TO THE END (IDENTIFIERS OF MESSAGES ARE SAVED TO FILE) 
// ---
enum MbResultType {
  rt_Success = 0,                 ///< \ru Нормальная работа. \en Normal work. 

  // \ru Типы ошибок образующих контуров \en Generating contours error types 
  rt_Empty,                       ///< \ru Пустой результат. \en Empty result. 
  rt_ToManyAxis,                  ///< \ru Слишком много осей. \en Too many axes. 
  rt_ToFewAxis,                   ///< \ru Не хватает осей. \en Too few axes. 
  rt_ToManyContours,              ///< \ru Слишком много контуров. \en Too many contours. 
  rt_Stars,                       ///< \ru Есть "звезда". \en Has "star". 
  rt_SelfIntersection,            ///< \ru Самопересечение контура. \en Contour is self-intersecting. 
  rt_SelfIntWhenExtended,         ///< \ru Самопересечение в продолжении контура. \en Contour extension is self-intersecting. 
  rt_Intersection,                ///< \ru Пересечение контуров. \en Contours intersection. 
  rt_NoIntersectSolid,            ///< \ru Образующий контур не пересекает тела. \en Generating contour does not intersect solids. 
  rt_NoIntersectSection,          ///< \ru Образующий контур не пересекает сечения (для операции построения тела по сечениям). \en Generating contour does not intersect sections (for loft solid construction operation). 
  rt_LibNotFound,                 ///< \ru Фрагмент не найден в библиотеке. \en Fragment is not found in library. 
  rt_MustBeClosed,                ///< \ru Должен быть замкнут. \en Must be closed. 
  rt_MustBeOpen,                  ///< \ru Должен быть разомкнут. \en Must be opened. 
  rt_AxisIntersection,            ///< \ru Пересечение с осью. \en Intersection with axis. 
  rt_DegenerateAxis,              ///< \ru Вырожденная ось. \en Degenerate axis. 

  // \ru Типы ошибок тела \en Solid error types 
  rt_MultiSolid,                  ///< \ru Тело состоит из отдельных частей. \en Solid consists of separate parts. 
  rt_CurveError,                  ///< \ru Ошибочная кривая. \en Wrong curve. 
  rt_ContourError,                ///< \ru Ошибочный контур. \en Wrong contour. 
  rt_SurfaceError,                ///< \ru Ошибочная поверхность. \en Wrong surface. 
  rt_SolidError,                  ///< \ru Ошибочное тело. \en Wrong solid. 
  rt_ParameterError,              ///< \ru Ошибочный параметр. \en Wrong parameter. 
  rt_ThicknessError,              ///< \ru Неправильно задана толщина. \en Wrong  thickness. 
  rt_NoSequenceCurveAndSections,  ///< \ru Не последовательное расположение сечений вдоль кривой (для операции построения тела по сечениям). \en Non-sequential arrangement of sections along the curve (for loft solid construction operation). 
  rt_SelfIntersect,               ///< \ru Объект самопересекается. \en Self-intersecting object. 
  rt_NoIntersect,                 ///< \ru Объекты не пересекаются. \en Objects are not crossed. 
  rt_OffsetIntersectError,        ///< \ru Невозможно построить эквидистанту с данными параметрами. \en Cannot create offset with the given parameters. 
  rt_BooleanError,                ///< \ru Ошибка в булевой операции. \en Boolean operation error. 
  rt_NoEdges,                     ///< \ru Ребра не найдены. \en Edges not found. 
  rt_PrepareError,                ///< \ru Ошибка при подготовке операции. \en Operation preparation error. 
  rt_ChamferError,                ///< \ru Ошибка при создании фаски ребра. \en Error at creating chamfer of edge. 
  rt_FilletError,                 ///< \ru Ошибка при создании скругления ребра. \en Error at creating fillet of edge. 
  rt_PartlyChamfer,               ///< \ru Созданы фаски не на всех ребрах. \en Chamfers are created not for all the edges. 
  rt_PartlyFillet,                ///< \ru Скруглены не все ребра. \en Fillets are created not for all the edges. 
  rt_ChamferSurfaceError,         ///< \ru Ошибка при создании поверхности фаски ребра. \en Error of a chamfer surface creation for an edge. 
  rt_FilletSurfaceError,          ///< \ru Ошибка при создании поверхности скругления ребра. \en Error of a fillet surface creation for an edge. 
  rt_TooLargeChamfer,             ///< \ru Слишком большие катеты фаски. \en Too big cathetuses of chamfer. 
  rt_TooLargeFillet,              ///< \ru Слишком большой радиус скругления. \en Too big radius of chamfer. 
  rt_SemiChamfer,                 ///< \ru Фаски построены не для всех ребер. \en Chamfers are created not for all the edges. 
  rt_SemiFillet,                  ///< \ru Скруглены не все ребра. \en Fillets are created not for all the edges. 
  rt_CuttingError,                ///< \ru Ошибка резки поверхностью. \en Error cutting by surface. 
  rt_ThinError,                   ///< \ru Ошибка при создании тонкостенного тела. \en Error of a thin-walled solid creation. 
  rt_OffsetError,                 ///< \ru Слишком большая толщина стенки при создании тонкостенного тела. \en Too big wall thickness while creating thin-walled solid. 
  rt_FaceError,                   ///< \ru Ошибочная грань. \en Wrong face. 
  rt_RibError,                    ///< \ru Неизвестная ошибка постановки ребра жесткости. \en Unknown error at rib statement. 
  rt_DraftError,                  ///< \ru Неизвестная ошибка уклона граней тела. \en Unknown error of inclining of solid faces. 
  rt_NoObjectForDirection,        ///< \ru В выбранном направлении отсутствует поверхность. \en No surface in the choosen direction. 
  rt_AbsorptionSolid,             ///< \ru Локальное тело поглощает результат. \en Local solid absorbs result. 
  rt_Error,                       ///< \ru Неизвестная ошибка. \en Unknown error. 
  rt_None,                        ///< \ru Нет сообщений. \en No messages. 
  rt_Intersect,                   ///< \ru Объекты пересекаются. \en Objects are crossed. 
  rt_InvalidType,                 ///< \ru Ненадлежащий тип кривой. \en Wrong type of curve. 
  rt_NoConvertTextToNurbs,        ///< \ru Преобразование текста выполнить невозможно. \en Unable to convert text. 
  rt_SplitWireNotSplitFace,       ///< \ru Контур не разбивает ни одну из граней или совпадает с кромкой грани. \en The contour does not split any face or coincides with a face boundary. 
  rt_SplitWireNotIntersectFace,   ///< \ru Контур не пересекает выбранное множество граней или совпадает с кромкой грани. \en The contour does not intersect selected set of faces or coincides with a face boundary. 
  rt_MustBeOnlyOnePoint,          ///< \ru Для данной операции в эскизе должна быть только одна точка. \en There must be only one point in sketch for current operation. 
  rt_InvalidPoleUsage,            ///< \ru Эскиз с одной точкой может использоваться только для крайнего сечения. \en Sketch with one point can be used only for the last section. 
  rt_ThinWithPole,                ///< \ru Построение тонкой стенки невозможно, если одно из сечений представляет собой точку. \en Creation of a thin wall is impossible if one of sections represents a point. 
  rt_TopologyError,               ///< \ru Ошибочная топология. \en Wrong topology. 

  // \ru Начало "нездорового" диапазона ошибок, НИКОГДА НЕ ВЗВОДИТЬ ЭТУ ОШИБКУ \en Beginning of "unhealthy" range of errors, NEVER TO RAISE THIS ERROR 
  rt_BeginOfInvalidRange,   

  // \ru K8+из 3D, там хранить нельзя так как они записываются и потом читаются \en K8+from 3D, it is forbidden to store there since they register and then are read 
  rt_ErBodyCloosed  = rt_TopologyError + 2,   ///< \ru Тело детали не определено. \en Solid of part is undefined. 
  rt_OneEdge        = rt_ErBodyCloosed + 2,   ///< \ru У выбранного угла нет общего ребра. \en The selected corner has no common edge. 
  rt_SomeEdge       = rt_OneEdge + 1,         ///< \ru У одного из выбранных углов нет общего ребра. \en One of chosen corners has no common edge. 
  rt_NoSuitedSketch = rt_OneEdge,             ///< \ru Неподходящий эскиз для операции. \en Improper sketch for operation. 
  // \ru K12 rt_SketchNoSheet  = rt_OneEdge,      // Эскиз должен располагаться только на внешней или внутренней плоской грани листового тела \en K12 rt_SketchNoSheet  = rt_OneEdge,      // Sketch should be located only on inner or outer flat face of a sheet solid 
  rt_SketchNoProj   = rt_OneEdge + 1,         ///< \ru Эскиз не проецируется на базовую грань. \en Sketch cannot be projected on a basic face. 
  rt_NoSheetFace    = rt_OneEdge,             ///< 
  rt_ErHeight       = rt_OneEdge,             ///< \ru Полный размер высоты должен быть больше толщины листового материала. \en Full size of height has to be greater than thickness of a sheet material. 
  rt_ErSketch       = rt_ErHeight + 1,        ///< \ru Эскиз должен располагаться только на внешней или внутренней плоской грани листового тела. \en Sketch should be located only on inner or outer flat face of a sheet solid. 
  rt_UpdatePlaceWrong = rt_TopologyError + 2,
  rt_ErDegree         = rt_TopologyError + 2,

  // \ru Окончание "нездорового" диапазона ошибок, НИКОГДА НЕ ВЗВОДИТЬ ЭТУ ОШИБКУ \en Ending of "unhealthy" range of errors, NEVER TO RAISE THIS ERROR  
  rt_EndOfInvalidRange = rt_BeginOfInvalidRange + 20,     

  rt_NoObjectInDirNormal,         ///< \ru Нет объекта в прямом направлении. \en No object in forward direction. 
  rt_NoObjectInDirReverse,        ///< \ru Нет объекта в обратном направлении. \en No object in reverse direction. 
  rt_SolidAffectedByBoolean,      ///< \ru Тела изменены булевой операцией. \en Solids changed by boolean operation. 
  rt_HaveDegenerateSegment,       ///< \ru Кривая содержит сегменты нулевой длины. \en Curve has segments with zero length. 
  rt_NotAllSourcesFound,          ///< \ru Не найдены одна или несколько исходных операций. \en One or several source operations are not found. 
  rt_InvalidEmptyContour,         ///< \ru Контур состоит из 2 отрезков, проходящих друг по другу. \en Contour consists of two segments passing through each other. 

  rt_UnnecessaryVariables,        ///< \ru Избыточное количество переменных. \en Excessive variables. 
  rt_DomainMismatch,              ///< \ru Область определения не соответствует заданным значениям. \en Domain doesn't match to given values. 
  rt_UnknownTranslatorError,      ///< \ru Неизвестная ошибка при работе транслятора. \en Unknown translator error. 
  rt_UnknownParserError,          ///< \ru Неизвестная ошибка при работе синтаксического анализатора. \en Unknown parser error. 
  rt_UnknownSymbol,               ///< \ru В строке присутствует неизвестный символ. \en String contains unknown symbol. 
  rt_NoClosingBracket,            ///< \ru Не хватает закрывающей скобки. \en Missed closing bracket. 
  rt_NoOpeningBracket,            ///< \ru Не хватает открывающей скобки. \en Missed opening bracket. 
  rt_ImpossibleOperation,         ///< \ru Невозможная операция. \en Impossible operation. 
  rt_LostObject,                  ///< \ru Операция потеряла опорные объекты. \en Operation lost support object. 
  rt_NotAllBendsProcessed,        ///< \ru Не все сгибы согнуты/разогнуты. \en Not all bends are bent/unbent. 
  rt_ValueScalingError,           ///< \ru Масштабирование с заданным коэффициентом невозможно. \en Scaling with the given factor is impossible. 
  rt_RatioScalingError,           ///< \ru Масштабирование с заданным соотношением коэффициентов невозможно. \en Scaling with given factors ratio is impossible. 
  rt_MultiSolidDeflected,         ///< \ru Данная операция не применима к телам из частей. \en Current operation cannot be applied to solids consisting of parts. 
  rt_MultiSolidDefused,           ///< \ru Результатом данной операции не может быть тело из частей. \en Result of current operation cannot be a solid consisting of parts. 
  rt_TransitionError,             ///< \ru Перестало выполняться граничное условие сопряжения. \en The boundary condition of conjugation is not satisfied. 
  rt_MeshCrossingError,           ///< \ru Точки пересечения кривых не образуют регулярной сетки. \en Points of curves intersection don't form a regular mesh. 
  rt_ClosedError,                 ///< \ru Нельзя выполнить замыкание. \en Impossible to carry out closure. 
  rt_TooGreatCurve,               ///< \ru Кривая, через которую происходит сопряжение, больше, чем сама поверхность. \en Curve to conjugate through is bigger than surface. 
  rt_ClosedOrUnClosedError,       ///< \ru Все кривые должны быть либо замкнуты, либо разомкнуты. \en All the curves must be closed or open simultaneously. 
  rt_SketchNoSheet,               ///< \ru Эскиз должен располагаться только на внешней или внутренней плоской грани листового тела. \en Sketch should be located only on inner or outer flat face of a sheet solid. 
  rt_BadSketch,                   ///< \ru Эскиз не удовлетворяет требованиям операции. \en Sketch doesn't meet the operation requirements. 
  rt_ConnectionError,             ///< \ru Нарушена связность объектов. \en Broken connectivity of objects. 
  rt_MeshSmoothError,             ///< \ru В точках пересечения кривые из противоположных семейств касаются. \en Intersection points are tangent points of curves from opposite families. 
  rt_NoIntersectContour,          ///< \ru Нет контуров пересечения. \en No intersection contours. 
  rt_ErShMtHeight,                ///< \ru Полный размер высоты должен быть больше толщины листового материала. \en Full size of height has to be greater than thickness of a sheet material. 
  rt_DegenerateSurface,           ///< \ru Поверхность вырождена. \en Degenerate surface. 
  rt_SurfaceEdgesIntersect,       ///< \ru Невозможно создать поверхность: нарушен порядок внутренних ребер разбиения. \en Cannot create surface: broken order of internal edges of splitting. 
  rt_TooLargeExtension,           ///< \ru Величина удлинения слишком велика. \en Too large extension. 
  rt_TooSmallExtension,           ///< \ru Величина удлинения слишком мала. \en Too small extension. 
  rt_VertexExtensionError,        ///< \ru Не удалось продлить поверхность до данной вершины. \en Cannot extend the surface up to a given vertex. 
  rt_SurfaceExtensionError,       ///< \ru Не удалось продлить поверхность до данной поверхности. \en Cannot extend surface to a given surface. 
  rt_NoEdgesConection,            ///< \ru Ребра не образуют связную цепочку. \en Edges are not connected. 
  rt_TooManyPoints,               ///< \ru Большое количество точек. Уменьшите количество. \en Too many points. Reduce them. 
  rt_TooManyPoints_1,             ///< \ru Слишком большое количество точек. \en Too many points. 
  rt_DirectionExtensionError,     ///< \ru Не удалось продлить поверхность в данном направлении. \en Cannot extend surface in the given direction. 
  rt_ExtensionPoleError,          ///< \ru Грань содержит полюс: укажите другую кромку или выберите другой тип продления. \en Face contains a pole: specify another boundary or select another extension type. 
  rt_MustBeOpenOrClosed,          ///< \ru Контуры должны быть либо все замкнуты, либо все разомкнуты. \en All the contours must be closed or open simultaneously. 
  rt_TooComplicatedItemsSet,      ///< \ru Слишком сложный набор элементов для обработки. \en Too complicated set of elemnts to process. 
  rt_NoAxesIntersection,          ///< \ru Оси не пересекаются. \en Axes are not crossed. 
  rt_TooFarItems,                 ///< \ru Объекты слишком далеко. \en Objects too far. 
  rt_ProcessIsStopped,            ///< \ru Процесс остановлен. \en Process is stopped. 
  rt_ContourSweptError,           ///< \ru Контур невозможно использовать для заданного перемещения. \en Contour cannot be used for given movement. 
  rt_SomeContourError,            ///< \ru Один из контуров невозможно использовать для заданного построения. \en Contour cannot be used for given construction. 
  rt_SplitWireNotAllFaces,        ///< \ru Линии разъема (ребра пересечения) созданы не на всех выбранных гранях или не доходят до границ граней. \en Parting lines (cross edges) created not on all selected faces or do not reach the boundaries of the faces.
  rt_GeneratrixColinearGuide,     ///< \ru В некоторых точках образующая параллельна направляющей. \en Generatrix parallel to the giude at some points. 
  rt_NotEnoughMemory,             ///< \ru Недостаточно памяти. \en Not enough memory. 
  rt_BorderColinearCurve,         ///< \ru Направление боковой границы параллельно касательной на конце образующей кривой. \en Direction of lateral border is parallel to a tangent at the end of a guide curve. 
  rt_ObjectNotFound,              ///< \ru Объект не найден. \en Object not found. 
  rt_PoleBrokenError,             ///< \ru В сплайновой поверхности часть точек из полюса передвинута, часть осталась совпадающей. \en Some points of spline surface moved from pole, some remained coincident. 
  rt_ApproxError,                 ///< \ru Аппроксимация не выполнена. \en No approximation performed. 
  rt_AccuracyError,               ///< \ru Не выполнены условия по точности построения. \en The construction accuracy conditions are not satisfied. 
  rt_BadVariable,                 ///< \ru Ошибочное значение переменной. \en Wrong value of variable. 
  rt_BadEdgesForChamfer,          ///< \ru Невозможно построить фаску на указанных ребрах. \en Cannot create chamfer on specified edges. 
  rt_RevokeStopFillet,            ///< \ru Остановка скругления невозможна, скругление выполнено без остановки. \en Can't stop fillet, the fillet without stopping was done.
  rt_AdjacentTransitionError,     ///< \ru Не согласованы сопряжения на смежных границах. \en The boundary condition of conjugation is not satisfied along adjacent boundaries. 
  rt_ObjectAccessDenied,          ///< \ru Доступ к объекту запрещен. \en Access to the object is denied. 
  rt_TooManySegments,             ///< \ru Количество сегментов слишком велико. \en The number of segments is too large.
  rt_GapShiftError,               ///< \ru Недопустимое положение зазора обечайки. \en Wrong gapShift parameter value.
  rt_CutBySilhouetteError,        ///< \ru Силуэтная грани линия не разрезает грань. \en Silhouette curve of face do not cut the face.
  rt_DegeneratedProjection,       ///< \ru Вырожденная проекция опорного объекта. \en The projection is degenerated for the reference object.
  rt_NotAllContoursUsed,          ///< \ru Использованы не все контура (кривые). \en Not all contours (curves) were used.
  rt_ChangedParameter,            ///< \ru Параметр операции был изменен. \en Parameter was changed.
  rt_NotAllObjectsUsed,           ///< \ru Использованы не все объекты. \en Not all objects were used.
  rt_ZeroJumperError,             ///< \ru Ошибка из-за образования перемычки нулевой толщины. \en Zero jumper error.
  rt_FullFilletError,             ///< \ru Ошибка при создании скругления грани. \en Error at creating full fillet. 

  // \ru Ошибки построения плавных сплайнов. \en Build failure of fair splines. 
  rt_IncorrectData,               ///<  1 \ru Некорректные данные. \en Incorrect data.
  rt_IncorrectPolylines,          ///<  2 \ru Некорректные формы ломаных / направления касательных. \en Incorrect polylines / tangent directions.
  rt_IncorrectStructure,          ///<  3 \ru Некорректная структура ломаной с прямолинейными участками. \en Incorrect structure of 3d poly with straight sites.
  rt_TooFewPoints,                ///<  4 \ru Слишком мало точек. \en Too few points.
  rt_CoincidentPoints,            ///<  5 \ru Совпадение точек. \en Coincidence of points.
  rt_TooSharpAngle,               ///<  6 \ru Слишком острый угол между сегментами ломаной. \en Too acute angle between segments of polyline.
  rt_ReverseMotion,               ///<  7 \ru Обратный ход сегмента. \en Return motion of segment.
  rt_SharpTorsion,                ///<  8 \ru Резкое кручение ломаной. \en Sharp torsion of spatial polyline.
  rt_IncorrectFirstDirection,     ///<  9 \ru Некорректное направление касательной в начальной точке. \en Incorrect direction of first tangent vector.
  rt_IncorrectLastDirection,      ///< 10 \ru Некорректное направление касательной в конечной точке. \en Incorrect direction of last tangent vector.
  rt_FirstTangentVector,          ///< 11 \ru Первая касательная задана к прямолинейному участку. \en First tangent vector is set to a rectilinear site of a polyline.
  rt_LastTangentVector,           ///< 12 \ru Последняя касательная задана к прямолинейному участку. \en Last tangent vector is set to a rectilinear site of a polyline.
  rt_TangentVectorsUnsuitable,    ///< 13 \ru Касательные векторы не корректны для локально-выпуклого участка. \en Tangent vectors are not suitable to convex shape of curve.
  rt_IncorrectBezier,             ///< 14 \ru Некорректная структура исходной кривой Безье. \en Incorrect structure of the initial Bezier curve.
  rt_NoIntersection,              ///< 15 \ru Нет пересечения касательных. Некорректная структура ломаной. \en No intersection of tangents. Incorrect structure of polyline.
  rt_CriticalConfiguration,       ///< 16 \ru Критическая конфигурация для B-сплайновой аппроксимации. \en Critical configuration for B-Spline approximation.
  rt_IncorrectInitialData,        ///< 17 \ru Некорректные исходные данные. \en Incorrect initial data.
  rt_NotImplemented,              ///< 18 \ru Данная команда не реализована. \en This command is not implemented.
  rt_InsufficientMemory,          ///< 19 \ru Недостаточно памяти. \en Insufficient memory. 
  rt_IncorrectFirstTangent,       ///< 20 \ru Некорректное направление первой касательной. \en Incorrect direction of first tangent.  
  rt_IncorrectLastTangent,        ///< 21 \ru Некорректное направление последней касательной. \en Incorrect direction of last tangent.  
  rt_StraightenLastSite,          ///< 22 \ru Конечный прямолинейный участок. Касательный вектор не нужен. \en Straighten last site. Last tangent Ignored.  
  rt_IncorrectPolylineStructure,  ///< 23 \ru Некорректная структура ломаной с прямолинейными участками. \en Incorrect structure of polyline with straight sites.
  rt_IncorrectDataStructure,      ///< 24 \ru Некорректная структура данных в модели. \en Incorrect data structure in the model.
  rt_ObjectIsNotPolyline,         ///< 25 \ru Объект - не пространственная ломаная. \en Object is not a 3D polyline.
  rt_MissingKnots,                ///< 26 \ru Отсутствует вектор узлов. \en Missing knots vector.
  rt_ClosedSpline,                ///< 27 \ru Замкнутый сплайн во внешнем файле. \en Closed spline in an external file.
  rt_ObjectNotSelected,           ///< 28 \ru Не выбран нужный объект. \en Object is not selected.
  rt_SharpChangeCurvature,        ///< 29 \ru Резкое изменение кривизны на участке перегиба \en A sharp change in curvature at the inflection site.
  rt_CloseToStraightness,         ///< 30 \ru Близость к прямолинейности. \en Close to straightness.
  rt_TooFewStartPoints,           ///< 31 \ru Слишком мало точек на выпуклом участке начального участка. \en Too few points on convex start site of curve.
  rt_BanClosedConfiguration,      ///< 32 \ru Запрет на замкнутую конфигурацию при количестве точек < 5. \en A ban on a closed configuration when the number of points < 5.
  rt_IncorrectDeterminant,        ///< 33 \ru Некорректная структура исходного геометрического определителя. \en Incorrect structure of the initial geometric determinant.
  rt_DimensionsPointsArrays,      ///< 34 \ru Размеры массивов точек. \en Dimensions of points arrays.
  rt_TangentsNotIntersect,        ///< 35 \ru Касательные не пересекаются. \en Tangents do not intersect.
  rt_PointsCoincided,             ///< 36 \ru Точки совпадают. \en Points coincide.
  rt_FewPointsForClosed,          ///< 37 \ru Мало точек для замкнутой ломаной. \en Few points for a closed polyline.
  rt_TooFewPointsOnSite,          ///< 38 \ru Слишком мало точек на выпуклом участке. \en Too few points on convex end site of curve.
  rt_PointsOnLine,                ///< 39 \ru Точки на прямой. \en Points on the straight line.
  rt_TooManyApexes,               ///< 40 \ru Слишком много точек. \en Too many points.
  rt_SiteNotConvex,               ///< 41 \ru Трехсегментный участок не выпуклый. \en Three-linked site is not convex.
  rt_CurvatureRange,              ///< 42 \ru Кривизна за пределами допустимого диапазона. \en Curvatute out of range.
  rt_ObjectNotHermiteGD,          ///< 43 \ru Объект не ГО Эрмита. \en The object is not Hermite GD.
  rt_ObjectNotNURBS,              ///< 44 \ru Объект не NURBzS. \en The object is not NURBzS.
  rt_DerivativeContinuityErr,     ///< 45 \ru Ошибка обеспечения непрерывности производной. \en Error ensuring derivative continuity.
  rt_NotEnoughBuildData,          ///< 46 \ru Недостаточно данных для построения. \en Not enough data to build.
  rt_InternalError,               ///< 47 \ru Неизвестная ошибка. \en Unknown error.

  rt_DerivativeGap = 198,         ///<  \ru Отсутствует непрерывность производной кривой по параметру. \en There is no continuity of the curve derivatives.
  rt_DerivativeGapU,              ///<  \ru Отсутствует непрерывность производной поверхности по первому параметру. \en There is no continuity of the surface derivatives on first parameter.
  rt_DerivativeGapV,              ///<  \ru Отсутствует непрерывность производной поверхности по второму параметру. \en There is no continuity of the surface derivatives on second parameter.

  rt_ContourGapError,             ///<  \ru В контуре есть разрывы между сегментами. \en There are gaps between the segments in the contour.
  rt_ContourSegmentsOverlapError, ///<  \ru В контуре есть наложения сегментов. \en There are segment overlays in the contour.
  rt_ContourSegmentsNoTangentJoint,///< \en В контуре есть сегменты с не гладкой стыковкой. \ru In the contour there are segments with a non - smooth connection.

  rt_ExtensionOutOfRange,         ///< \ru Удлинение не может быть ограничено заданной поверхностью. \en Extension cannot be limited given surface.

  rt_BuildSheetBySolidError,      ///< \ru Ошибка построения листового тела по произвольному телу. \en Build sheet solid based on an arbitary solid error.
  rt_BendEdgeError,               ///< \ru Ребро не может быть использовано как ребро сгиба. \en The edge can't be processed as bend edge.

  rt_StampToolPositionError,      ///< \ru Ошибочное положение инструмента относительно листового тела. \en Illegal position of the tool body related to the sheet body.

  // \ru Ошибки построения поверхности переменного сечения. \en Build failure of the swept mutable section surface. 
  rt_NoEdgesForBuild,             ///< \ru Отсутствуют рёбра для построения. \en No edges for build.
  rt_NoCurvesForBuild,            ///< \ru Отсутствуют кривые для построения. \en No curves for build.
  rt_NotCorrectDataForBuild,      ///< \ru Некорректные данные. \en Not correct data for build.
  rt_NotEnoughDataForBuild,       ///< \ru Не хватает данных для построения. \en Not enough data for build.
  rt_ReferenceCurveError,         ///< \ru Ошибочная опорная кривая. \en Illegal reference curve.
  rt_GuideCurveError,             ///< \ru Ошибочная направляющая кривая. \en Illegal guide curve.
  rt_ApexCurveError,              ///< \ru Ошибочная вершинная кривая. \en Illegal apex curve.
  rt_DiscriminantFunctionError,   ///< \ru Ошибочная дискриминантная функция. \en Illegal discriminant function.
  rt_DiscriminantCurveError,      ///< \ru Ошибочная дискриминантная кривая. \en Illegal discriminant curve.
  rt_DiscriminantSurfaceError,    ///< \ru Ошибочная дискриминантная поверхность. \en Illegal discriminant surface.
  rt_AngleFunctionError,          ///< \ru Ошибочная функция угла. \en Illegal angle function.
  rt_ReferenceCurveBuildFailed,   ///< \ru Опорную кривую не удалось построить. \en Reference curve build was failed.
  rt_GuideCurveBuildFailed,       ///< \ru Направляющую кривую не удалось построить. \en Guide curvebuild was failed.
  rt_ApexCurveBuildFailed,        ///< \ru Вершинную кривую не удалось построить. \en Apex curve build was failed.
  rt_VertexCurveBuildFailed,      ///< \ru Контрольную кривую не удалось построить. \en Vertex curve build was failed.
  rt_ApexDiscriminantBuildFailed, ///< \ru Вершинную кривую и дискриминант не удалось построить. \en Apex curve and discriminant build was failed.
  rt_DiscriminantBuildFailed,     ///< \ru Дискриминантную функцию не удалось построить. \en Discriminant function build was failed.
  rt_PatternCurveError,           ///< \ru Ошибочная образующая кривая. \en Illegal generatrix curve.
  rt_SynchronizeParameterError,   ///< \ru Не удалось синхронизировать параметры. \en Failed to synchronize parameters.
  rt_ReparamBuildFailed,          ///< \ru Не удалось выполнить репараметризацию. \en Reparameter build was failed.
  rt_TriangleSectionFailed,       ///< \ru Треугольник сечения поверхности не может быть определен. \ en The triangle section of the surface cannot be determined.
  rt_DiscriminantFunctionWrong,   ///< \ru Функция дискриминанта для задания формы сечения не обеспечивает построение поверхности. \en The discriminant function for setting the cross section shape does not provide the surface construction.
  rt_DiscriminantCurveWrong,      ///< \ru Кривая для задания формы сечения не обеспечивает построение поверхности. \en The curve for setting the cross section shape does not provide the surface construction.
  rt_DiscriminantSurfaceWrong,    ///< \ru Касательная поверхность для задания формы сечения не обеспечивает построение поверхности. \en The tangent surface for setting the cross section shape does not provide the surface construction.
  rt_SectionMovementWrong,        ///< \ru Движение сечения не совместимо с направляющими. \en The cross section movement is not compatible with the guides.
  rt_DiscriminantSurfaceFar,      ///< \ru Дискриминантная поверхность далеко от сечения. \en The discriminant surface far from section.
  rt_CrossOrderError,             ///< \ru Нарушен порядок взаимного пересечения кривых. \en The order of mutual intersection of curves is violated.

  rt_WrongExtensionLength,        ///< \ru Некорректная длина продления. \en Incorrect extension length.
  rt_WrongExtensionWayValue,      ///< \ru Неизвестный способ продления. \en Unknown extension way.
  rt_StampToolHoleError,          ///< \ru Ошибка, в области штамповки не должно быть вырезов. \en Error, the stamping area contains holes.
  rt_CurveClosedAtStart,          ///< \ru Кривая замкнулась в начале. \en The curve has been closed at start point.
  rt_CurveClosedAtEnd,            ///< \ru Кривая замкнулась в конце. \en The curve has been closed at end point.
  rt_CurveClosedBothSides,        ///< \ru Кривая замкнулась с двух сторон. \en The curve has been closed at both sides.
  rt_BeyondLimitsExtension,       ///< \ru Продленная поверхностная кривая вышла за границы поверхности. \en Extended surface curve abandons surface boundary.

  rt_NonBijectiveFunc,            ///< \ru Отображение множества значений параметра функции во множество значений параметра кривой не является взаимно однозначным. \en A map from function parameter range to curve parameter range is not bijective.

  rt_LicenseError,                ///< \ru Ошибка лицензии: неправильная или истекшая лицензия. \en License error: wrong or expired license.

  rt_NoRayIntersection,           ///< \ru В выбранном направлении нет пересечения луча со вторым объектом. \en There is no intersection of a ray and a second object in the selected direction.
  rt_ExtremesRequestExceeded,     ///< \ru Количество экстремумов превышает количество измерений. \en The number of extremes exceeds the number of measurements.

  rt_ConstraintSurfaceDomainError,///< \ru В области определения элемента обнаружена недопустимая ситуация относительно внешнего объекта ограничения - поверхности. \en In the range of spine curve's parameter an unacceptable situation with respect to a surface used in a constraint was encountered.
  rt_ConstraintCurveEmptyDomain,  ///< \ru Пустая область определения относительно внешнего объекта ограничения - кривой. \en The range of spine curve's parameter is empty with respect to a curve used in a constraint.
  rt_ConstraintCurveDomainError,  ///< \ru В области определения элемента обнаружена недопустимая ситуация относительно внешнего объекта ограничения - кривой. \en In the range of spine curve's parameter an unacceptable situation with respect to a curve used in a constraint was encountered.
  rt_ConstraintCurveNoBijection,  ///< \ru Нарушение биекции между траекторией и внешним объектом ограничения - кривой. \en Violation of the bijection between the spine's curve and a curve used in a constraint.

  rt_FacesGroupsOverlapError,     ///< \ru Массивы заменяющих и заменяемых граней пересекаются. \en Replacement faces and replaced faces arrays are overlapping.
  rt_StitchShellsError,           ///< \ru Ошибка сшивки оболочек. \en Stitch shells error.
  rt_NoFacesForModification,      ///< \ru Отсутствуют грани для модификации. \en There are no faces for modification.
  rt_NoReplacementFaces,          ///< \ru Отсутствуют заменяющие грани. \en There are no replacement faces.

  rt_InvalidMathVersion,          ///< \ru Недопустимая математическая версия. \en Invalid mathematical version.

  rt_InvalidTruncChainError,      /// < \ru Секущая цепочка не полностью покрывает оболочку. \en Incomplete shell coverage by truncating chain.

  rt_RadiusFunctionWrong,         ///< \ru Недопустимая функция радиуса. \en Incorrect radius function.

  // \ru !!! СТРОКИ ВСТАВЛЯТЬ СТРОГО ПЕРЕД ЭТОЙ СТРОКОЙ !!!! \en !!! INSERT LINES STRICTLY BEFORE THIS LINE !!!! 
  rt_ErrorTotal           // \ru НИЖЕ НЕ ДОБАВЛЯТЬ! \en DON'T ADD BELOW! 
};


//------------------------------------------------------------------------------
/** \brief \ru Результат операции "Сшивка".
           \en The "stitching" operation result. \~
  \ingroup Data_Structures
*/
// ---
enum MbeStitchResType {
  stch_Success = 0,         ///< \ru Нет ошибок. \en No errors. 
  stch_PrepareError,        ///< \ru Ошибка при подготовке операции сшивки. \en "Stitching" operation preparation error. 
  stch_CoorientFaceError,   ///< \ru Невозможно выставить согласованную ориентацию граней. \en Unable to set the matched orientation of faces. 
  stch_SomeEdgesUnstitched, ///< \ru Некоторые рёбра остались несшитыми. \en Some edges are still unstitched. 
  stch_OutwardOrientError,  ///< \ru Не удалось установить нормали граней наружу тела. \en Can't set the normals of faces oriented outside the solid. 
  stch_NoEdgeWasStitched,   ///< \ru Не было сшито ни одного ребра. \en No edge was stitched. 
  stch_SeparatePartsResult, ///< \ru После сшивки остались несвязанные между собой куски. \en There are separate parts after stitching. 
  stch_EdgeStitchError,     ///< \ru Ошибка сшивки ребра. \en Edge stitching error. 
  stch_InputTopologyError   ///< \ru Критические ошибки топологии во входных оболочках. \en Critical topology errors in input shells.
};


#endif // __MB_OPERATION_RESULT_H
