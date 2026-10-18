// \ru Пустая строка необходима для ресурсного компилятора MSVC \en An empty string is necessary for MSVC resource compiler

#ifndef __TEST_PROMPT_H
#define __TEST_PROMPT_H

#include <math_x.h>
#include <system_types.h>


///////////////////////////////////////////////////////////////////////////////////////////
//
// \ru Информация в окне подсказки. \en Information in a prompt window.
//
///////////////////////////////////////////////////////////////////////////////////////////

#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_POINT                  _T("Укажите точку")
#else
#define IDS_SELECT_POINT                  _T("Specify a point")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_VECTOR                  _T("Укажите вектор")
#else
#define IDS_SELECT_VECTOR                  _T("Specify a vector")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_CENTER                 _T("Укажите центр")
#else
#define IDS_SELECT_CENTER                 _T("Specify center")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_VECTOR_X                _T("Укажите вектор X")
#else
#define IDS_SELECT_VECTOR_X                _T("Specify a vector X")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_VECTOR_Y                _T("Укажите вектор Y")
#else
#define IDS_SELECT_VECTOR_Y                _T("Specify a vector Y")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_VECTOR_Z                _T("Укажите вектор Z")
#else
#define IDS_SELECT_VECTOR_Z                _T("Specify a vector Z")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_CURVE                  _T("Укажите кривую")
#else
#define IDS_SELECT_CURVE                  _T("Specify a curve")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_CONTOUR                _T("Укажите контур")
#else
#define IDS_SELECT_CONTOUR                _T("Specify a contour")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_DISCRIMINANT_CURVE            _T("Укажите дискриминантную кривую")
#else
#define IDS_DISCRIMINANT_CURVE            _T("Specify a discriminant curve")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_DISCRIMINANT_SURFACE         _T("Укажите дискриминантную поверхность")
#else
#define IDS_DISCRIMINANT_SURFACE          _T("Specify a discriminant surface")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_DISCRIMINANT_FACE             _T("Укажите дискриминантную грань")
#else
#define IDS_DISCRIMINANT_FACE             _T("Specify a discriminant face")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_APEX_CURVE             _T("Укажите кривую вершин")
#else
#define IDS_SELECT_APEX_CURVE             _T("Specify an apex curve")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_FACE_PLUS_MINUS               _T("\n\r Грань: Plus - 1; Minus - 0")
#else
#define IDS_FACE_PLUS_MINUS               _T("\n\r Face: Plus - 1; Minus - 0")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_TORUS_CENTER           _T("Укажите центр тора")
#else
#define IDS_SELECT_TORUS_CENTER           _T("Specify torus center")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_SURFACE                _T("Укажите поверхность")
#else
#define IDS_SELECT_SURFACE                _T("Specify a surface")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_ELEM_SURFACE           _T("Укажите элементарную поверхность")
#else
#define IDS_SELECT_ELEM_SURFACE           _T("Specify an elementary surface")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_FIRST_CURVE            _T("Укажите первую кривую")
#else
#define IDS_SELECT_FIRST_CURVE            _T("Specify the first curve")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_SECOND_CURVE           _T("Укажите вторую кривую")
#else
#define IDS_SELECT_SECOND_CURVE           _T("Specify the second curve")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_ROTATE_AXIS            _T("Укажите ось (линию) вращения")
#else
#define IDS_SELECT_ROTATE_AXIS            _T("Specify rotation axis (line)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_GUIDE_LINE             _T("Укажите направляющую прямую")
#else
#define IDS_SELECT_GUIDE_LINE             _T("Specify a guide curve")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_START_POINT            _T("Укажите начальную точку")
#else
#define IDS_SELECT_START_POINT            _T("Specify a start point")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_FINAL_POINT            _T("Укажите конечную точку")
#else
#define IDS_SELECT_FINAL_POINT            _T("Specify an end point")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_SKETCH_POINT           _T("Укажите точку эскиза")
#else
#define IDS_SELECT_SKETCH_POINT           _T("Specify sketch point")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_SURFACE_POINT          _T("Укажите точку поверхности")
#else
#define IDS_SELECT_SURFACE_POINT          _T("Specify surface point")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_EDIT_ELEMENT           _T("Укажите элемент для редактирования")
#else
#define IDS_SELECT_EDIT_ELEMENT           _T("Specify an element for editing")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_INTERS_CURVE           _T("Укажите кривую пересечения")
#else
#define IDS_SELECT_INTERS_CURVE           _T("Specify an intersection curve")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_POINT_ON_CRV           _T("Укажите точку на кривой")
#else
#define IDS_SELECT_POINT_ON_CRV           _T("Specify a point on the curve")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_CURVE_ON_SFR           _T("Укажите кривую на поверхности")
#else
#define IDS_SELECT_CURVE_ON_SFR           _T("Specify a curve on the surface")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_VERTEX                 _T("Укажите вершину тела")
#else
#define IDS_SELECT_VERTEX                 _T("Specify a vertex of the solid")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_NEW_PALCE              _T("Укажите новое положение")
#else
#define IDS_SELECT_NEW_PALCE              _T("Specify a new position")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_PLANE                  _T("Укажите плоскость")
#else
#define IDS_SELECT_PLANE                  _T("Specify a plane")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_SHELL                  _T("Укажите оболочку")
#else
#define IDS_SELECT_SHELL                  _T("Specify a shell")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_REFER_CURVE            _T("Укажите опорную кривую")
#else
#define IDS_SELECT_REFER_CURVE            _T("Specify a reference curve")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_GUIDE_EDGE             _T("Укажите направляющее ребро")
#else
#define IDS_SELECT_GUIDE_EDGE             _T("Specify a guide edge")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_GUIDE_CURVE            _T("Укажите направляющую кривую")
#else
#define IDS_SELECT_GUIDE_CURVE            _T("Specify a guide curve")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_ADDING_CURVE            _T("Укажите дополнительную кривую")
#else
#define IDS_SELECT_ADDING_CURVE            _T("Specify a additional curve")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_2D_CURVE               _T("Укажите двумерную кривую")
#else
#define IDS_SELECT_2D_CURVE               _T("Specify a two-dimentional curve")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_GENER_CURVE            _T("Укажите образующую кривую")
#else
#define IDS_SELECT_GENER_CURVE            _T("Specify a generating curve")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_GENERATRIC_PREPARED           _T("Образующая из проволочных каркасов подготовлена")
#else
#define IDS_GENERATRIC_PREPARED           _T("The wire frame generating is prepared")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_SOLID                  _T("Укажите твердое тело")
#else
#define IDS_SELECT_SOLID                  _T("Specify a solid")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_MESH                   _T("Укажите полигональный объект")
#else
#define IDS_SELECT_MESH                   _T("Specify a polygonal object")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_FIXED_OBJECT           _T("Укажите фиксированный объект")
#else
#define IDS_SELECT_FIXED_OBJECT           _T("Specify a fixed object")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_MOVING_MESH            _T("Укажите движущуюся триангуляцию")
#else
#define IDS_SELECT_MOVING_MESH            _T("Specify a moving mesh")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_HINT_USAGE_MODE               _T("Рассчитать подсказку автоматически: 0 - нет, 1 - да")
#else
#define IDS_HINT_USAGE_MODE               _T("Calculate a hint automatically: 0 - no, 1 - yes")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_POLY_CURVE_CUT_MODE           _T("Режим: 0 - врезать, 1 - разбить сегментацию, 2 - оставить внешнюю часть, 2 - оставить внутреннюю часть")
#else
#define IDS_POLY_CURVE_CUT_MODE           _T("Mode: 0 - insert, 1 - split segm, 2 - keep outer, 2 - keep inner")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_POINT_REJECTION_MODE          _T("Отбраковать пары точек: 0 - нет, 1 - да")
#else
#define IDS_POINT_REJECTION_MODE          _T("Reject point pairs: 0 - no, 1 - yes")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_POINT_PAIRS_MAX               _T("Введите размер выборки точек")
#else
#define IDS_POINT_PAIRS_MAX               _T("Enter the number of sample points")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_FIRST_SOLID            _T("Укажите первое твердое тело")
#else
#define IDS_SELECT_FIRST_SOLID            _T("Specify first solid")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_SECOND_SOLID           _T("Укажите второе твердое тело")
#else
#define IDS_SELECT_SECOND_SOLID           _T("Specify second solid")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_ORIENTATION            _T("Укажите ориентацию")
#else
#define IDS_SELECT_ORIENTATION            _T("Specify orientation")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_OUTER_SHELL            _T("Укажите внешнюю оболочку")
#else
#define IDS_SELECT_OUTER_SHELL            _T("Specify outer shell")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_VACUUM_SHELLS          _T("Укажите оболочки пустот")
#else
#define IDS_SELECT_VACUUM_SHELLS          _T("Specify void shells")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_GEOM_OBJECT            _T("Укажите геометрический объект")
#else
#define IDS_SELECT_GEOM_OBJECT            _T("Specify a geometric object")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_EDGE                   _T("Укажите ребро тела")
#else
#define IDS_SELECT_EDGE                   _T("Specify an edge of solid")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_OBJECT                 _T("Укажите объект")
#else
#define IDS_SELECT_OBJECT                 _T("Specify an object")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_TITLE_PLACE            _T("Укажите положение надписи")
#else
#define IDS_SELECT_TITLE_PLACE            _T("Specify the title position")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_DEPENDENCE             _T("Укажите зависимость")
#else
#define IDS_SELECT_DEPENDENCE             _T("Specify dependence")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_NEW_PLACE              _T("Укажите новое положение")
#else
#define IDS_SELECT_NEW_PLACE              _T("Specify a new position")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_OBJECT_POINT           _T("Укажите точку объекта")
#else
#define IDS_SELECT_OBJECT_POINT           _T("Specify a point of the object")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_MATH_OBJECT            _T("Укажите составной объект")
#else
#define IDS_SELECT_MATH_OBJECT            _T("Specify a compound object")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_FIRST_POINT            _T("Укажите первую точку")
#else
#define IDS_SELECT_FIRST_POINT            _T("Specify first point")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_SECOND_POINT           _T("Укажите вторую точку")
#else
#define IDS_SELECT_SECOND_POINT           _T("Specify second point")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_OBJ_ON_PLANE           _T("Укажите объект на плоскости")
#else
#define IDS_SELECT_OBJ_ON_PLANE           _T("Specify an object on the plane")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_PNT_ON_CIRCLE          _T("Укажите точку на окружности")
#else
#define IDS_SELECT_PNT_ON_CIRCLE          _T("Specify a point on circle")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_PLACEMENT              _T("Укажите положение")
#else
#define IDS_SELECT_PLACEMENT              _T("Specify position")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_LINE                   _T("Укажите прямую")
#else
#define IDS_SELECT_LINE                   _T("Specify a line")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_CRCL_ON_PLANE          _T("Укажите окружность на плоскости")
#else
#define IDS_SELECT_CRCL_ON_PLANE          _T("Specify a circle on the plane")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_THIRD_POINT            _T("Укажите третью точку")
#else
#define IDS_SELECT_THIRD_POINT            _T("Specify third point")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_CNTR_ON_PLANE          _T("Укажите контур на плоскости")
#else
#define IDS_SELECT_CNTR_ON_PLANE          _T("Specify a contour on the plane")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_SPLN_ON_PLANE          _T("Укажите сплайн на плоскости")
#else
#define IDS_SELECT_SPLN_ON_PLANE          _T("Specify a spline on the plane")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_NEAREST_POINT          _T("Укажите ближайшую точку")
#else
#define IDS_SELECT_NEAREST_POINT          _T("Specify the closest point")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_NEW_PNT_PLC            _T("Укажите новое положение точки")
#else
#define IDS_SELECT_NEW_PNT_PLC            _T("Specify new position of the point")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_CRV_ON_PLANE           _T("Укажите кривую на плоскости")
#else
#define IDS_SELECT_CRV_ON_PLANE           _T("Specify a curve on the plane")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_DIMTITLE_PLC           _T("Укажите положение размерой надписи")
#else
#define IDS_SELECT_DIMTITLE_PLC           _T("Specify position of dimensional text")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_ARC_CENTER             _T("Укажите центр дуги")
#else
#define IDS_SELECT_ARC_CENTER             _T("Specify a center of the arc")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_AREA_ON_PLANE          _T("Укажите область на плоскости")
#else
#define IDS_SELECT_AREA_ON_PLANE          _T("Specify a region on the plane")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_TEXT                   _T("Укажите текст")
#else
#define IDS_SELECT_TEXT                   _T("Specify a text")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_FIRST_CRVPNT           _T("Укажите точку на первой кривой")
#else
#define IDS_SELECT_FIRST_CRVPNT           _T("Specify a point on the first curve")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_SECOND_CRVPNT          _T("Укажите точку на второй кривой")
#else
#define IDS_SELECT_SECOND_CRVPNT          _T("Specify a point on the second curve")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_CRV_DEL_PART           _T("Укажите удаляемую часть кривой")
#else
#define IDS_SELECT_CRV_DEL_PART           _T("Specify a part of the curve to delete")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_SCALE_CENTER           _T("Укажите центр масштабирования")
#else
#define IDS_SELECT_SCALE_CENTER           _T("Specify scaling center")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_ALIGN_BORDER           _T("Укажите границу выравнивания")
#else
#define IDS_SELECT_ALIGN_BORDER           _T("Specify alignment border")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_LINE_ON_PLANE          _T("Укажите линию на плоскости")
#else
#define IDS_SELECT_LINE_ON_PLANE          _T("Specify a line on the plane")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_LINE_SEGMENT           _T("Укажите отрезок прямой")
#else
#define IDS_SELECT_LINE_SEGMENT           _T("Specify a line segment")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_CIRCLE                 _T("Укажите окружность")
#else
#define IDS_SELECT_CIRCLE                 _T("Specify a circle")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_CONIC_SECTION          _T("Укажите коническое сечение")
#else
#define IDS_SELECT_CONIC_SECTION          _T("Specify a conic section")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_FACE                   _T("Укажите грань тела")
#else
#define IDS_SELECT_FACE                   _T("Specify a face of the solid")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_FACE_FOR_SET           _T("Укажите грань тела для набора #%d")
#else
#define IDS_SELECT_FACE_FOR_SET           _T("Specify a face of the solid for the set #%d")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_NURBS_CURVE            _T("Укажите NURBS кривую")
#else
#define IDS_SELECT_NURBS_CURVE            _T("Specify NURBS curve")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_INCLINE_FACE           _T("Укажите уклоняемую грань")
#else
#define IDS_SELECT_INCLINE_FACE           _T("Specify a face to incline")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_CENTER_LINE            _T("Укажите осевую линию")
#else
#define IDS_SELECT_CENTER_LINE            _T("Specify a center line")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_PLANE_FACE             _T("Укажите плоскую грань")
#else
#define IDS_SELECT_PLANE_FACE             _T("Specify a planar face")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_LNSEG_PLFACE           _T("Укажите отрезок на плоской грани")
#else
#define IDS_SELECT_LNSEG_PLFACE           _T("Specify a segment on the planar face")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_INNER_FACE             _T("Укажите внутреннюю грань сгиба")
#else
#define IDS_SELECT_INNER_FACE             _T("Specify internal face of the bend")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_OUTER_FACE             _T("Укажите внешнюю грань сгиба")
#else
#define IDS_SELECT_OUTER_FACE             _T("Specify outer face of the bend")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_CYLCONE_FACE           _T("Укажите цилиндическую или коническую грань")
#else
#define IDS_SELECT_CYLCONE_FACE           _T("Specify a cylindrical or a conical face")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_FIRST_FACE             _T("Укажите первую грань")
#else
#define IDS_SELECT_FIRST_FACE             _T("Specify a first face in pair")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_SECOND_FACE            _T("Укажите вторую грань")
#else
#define IDS_SELECT_SECOND_FACE            _T("Specify a second face in pair")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_ORTO_PLANE             _T("Укажите плоскость, перпендикулярную оси цилиндра или конуса")
#else
#define IDS_SELECT_ORTO_PLANE             _T("Specify a plane perpendicular to the axis of a cylinder or a cone")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_GAP_VALUE               _T("Введите величину зазора")
#else 
#define IDS_ENTER_GAP_VALUE               _T("Enter a gap value")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_DISTANCE_RATIO          _T("Введите долю расстояния")
#else 
#define IDS_ENTER_DISTANCE_RATIO          _T("Enter a distance ratio")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_VIEW_PLANE             _T("Укажите плоскость вида")
#else
#define IDS_SELECT_VIEW_PLANE             _T("Specify view plane")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_THREAD                 _T("Укажите резьбу")
#else
#define IDS_SELECT_THREAD                 _T("Specify a thread")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_EQUATION               _T("Укажите уравнение")
#else
#define IDS_SELECT_EQUATION               _T("Specify an equation")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_EQUATION_VAR           _T("Укажите переменную уравнения")
#else
#define IDS_SELECT_EQUATION_VAR           _T("Specify a variable of the equation")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_TOPO_OBJECT            _T("Укажите топологический объект (вершину, ребро, грань)")
#else
#define IDS_SELECT_TOPO_OBJECT            _T("Specify a topological object (a vertex, an edge, a face)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_MULTILINE              _T("Укажите плоскую мультилинию")
#else
#define IDS_SELECT_MULTILINE              _T("Specify a planar multiline")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_WIREFRAME              _T("Укажите проволочный каркас")
#else
#define IDS_SELECT_WIREFRAME              _T("Specify a wireframe")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_WIREFRAME_EDGE_INDEX    _T("Укажите номер ребра в проволочном каркасе")
#else
#define IDS_SELECT_WIREFRAME_EDGE_INDEX    _T("Specify the edge number in the wire frame")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SPECIFY_BY_PROJECTIONS         _T("Указать границы проецированием")
#else
#define IDS_SPECIFY_BY_PROJECTIONS         _T("Specify boundaries by projection")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_PAR_ON_EDGE              _T("Введите параметр на ребре ( 0 <= t <= 1 )")
#else
#define IDS_ENTER_PAR_ON_EDGE              _T("Enter parameter on the ребре ( 0 <= t <= 1 )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_ROTATE_ANGLE            _T("Введите угол поворота")
#else
#define IDS_ENTER_ROTATE_ANGLE            _T("Enter the rotation angle")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_SPLINE_ORDER            _T("Введите порядок сплайна")
#else
#define IDS_ENTER_SPLINE_ORDER            _T("Enter the spline order")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_SPHERE_RADIUS           _T("Введите радиус сферы")
#else
#define IDS_ENTER_SPHERE_RADIUS           _T("Enter sphere radius")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_CYLINDER_RADIUS         _T("Введите радиус цилиндра")
#else
#define IDS_ENTER_CYLINDER_RADIUS         _T("Enter cylinder radius")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_CONE_ANGLE              _T("Введите угол конуса")
#else
#define IDS_ENTER_CONE_ANGLE              _T("Enter cone angle")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_TORUS_RAD_BG            _T("Введите большой радиус тора")
#else
#define IDS_ENTER_TORUS_RAD_BG            _T("Enter the major radius of torus")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_TORUS_RAD_SM            _T("Введите малый радиус тора")
#else
#define IDS_ENTER_TORUS_RAD_SM            _T("Enter the minor radius of torus")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_DEPTH_INCREM            _T("Введите приращение глубины")
#else
#define IDS_ENTER_DEPTH_INCREM            _T("Enter the depth increpent")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_NUMBER                  _T("Введите число ")
#else
#define IDS_ENTER_NUMBER                  _T("Enter a number ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_SURF_LENGHT             _T("Введите длину поверхности")
#else
#define IDS_ENTER_SURF_LENGHT             _T("Enter surface length")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_DISTANCE                _T("Введите расстояние")
#else
#define IDS_ENTER_DISTANCE                _T("Enter the distance")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_FIBER_ANG_TYPE          _T("Введите угол для основной нити (0 - 0, 1 - 45, 2 - -45)")
#else
#define IDS_ENTER_FIBER_ANG_TYPE          _T("Enter the fiber warp angle (0 - 0, 1 - 45, 2 - -45)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_PRECISION               _T("Введите точность аппроксимации")
#else
#define IDS_ENTER_PRECISION               _T("Enter the approximation precision")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_DEGREE                  _T("Введите порядок сплайна")
#else
#define IDS_ENTER_DEGREE                  _T("Enter the spline degree")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_FIRST_SRF_DST           _T("Ведите первое расстояние")
#else
#define IDS_ENTER_FIRST_SRF_DST           _T("Enter the first distance")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_SECOND_SRF_DST          _T("Ведите второе расстояние")
#else
#define IDS_ENTER_SECOND_SRF_DST          _T("Enter the second distance")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_RADIUS                  _T("Введите радиус")
#else
#define IDS_ENTER_RADIUS                  _T("Enter the radius")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_FILLET_RADIUS           _T("Введите радиус скругления")
#else
#define IDS_ENTER_FILLET_RADIUS           _T("Enter the fillet radius")
#endif

#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_PHASE                  _T("Введите фазовый сдвиг")
#else
#define IDS_ENTER_PHASE                  _T("Enter the phase shift")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_AMPLITUDE              _T("Введите амплитуду")
#else
#define IDS_ENTER_AMPLITUDE              _T("Enter the amplitude")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_WAVELENGTH              _T("Введите длину волны")
#else
#define IDS_ENTER_WAVELENGTH              _T("Enter the wavelength")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_WAVESCOUNT              _T("Введите количество волн")
#else
#define IDS_ENTER_WAVESCOUNT              _T("Enter the number of waves")
#endif

#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_TENSION                 _T("Введите натяжение (0.0 - 1.0)")
#else
#define IDS_ENTER_TENSION                 _T("Enter the tension (0.0 - 1.0)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_U_DEGREE                _T("Введите порядок сплайна по U")
#else
#define IDS_ENTER_U_DEGREE                _T("Enter the spline degree by U")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_V_DEGREE                _T("Введите порядок сплайна по V")
#else
#define IDS_ENTER_V_DEGREE                _T("Enter the spline degree by V")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_POINT_NUMB_1_4          _T("Введите номер точки от 1 до 4")
#else
#define IDS_ENTER_POINT_NUMB_1_4          _T("Enter a point number from 1 to 4")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_SIDE_NUMB_1_4           _T("Введите номер стороны от 1 до 4")
#else
#define IDS_ENTER_SIDE_NUMB_1_4           _T("Enter a side number from 1 to 4")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_STRETCHING              _T("Введите удлинение")
#else
#define IDS_ENTER_STRETCHING              _T("Enter stretching")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_SPLIT_COUNT_U           _T("Количество разбиений по U ")
#else
#define IDS_ENTER_SPLIT_COUNT_U           _T("Splitting count by U ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_SPLIT_COUNT_V           _T("Количество разбиений по V ")
#else
#define IDS_ENTER_SPLIT_COUNT_V           _T("Splitting count by V ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_MAP_SCALE               _T("Введите масштаб отображения вида")
#else
#define IDS_ENTER_MAP_SCALE               _T("Enter the scale of a view mapping")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_POINT_INDEX             _T("Введите индекс точки")
#else
#define IDS_ENTER_POINT_INDEX             _T("Enter a point index")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_TEXT                    _T("Введите текст")
#else
#define IDS_ENTER_TEXT                    _T("Enter a text")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_INITIAL_PRM             _T("Начальный параметр")
#else
#define IDS_ENTER_INITIAL_PRM             _T("Initial parameter")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_FINAL_PRM               _T("Конечный параметр")
#else
#define IDS_ENTER_FINAL_PRM               _T("Final parameter")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_NUMBER_N                _T("Введите номер")
#else
#define IDS_ENTER_NUMBER_N                _T("Enter a number")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_SCALE                   _T("Введите масштаб")
#else
#define IDS_ENTER_SCALE                   _T("Enter a scale")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_QUANTITY                _T("Введите количество")
#else
#define IDS_ENTER_QUANTITY                _T("Enter quantity")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_THICKNESS               _T("Введите толщину")
#else
#define IDS_ENTER_THICKNESS               _T("Enter a thickness")
#endif

#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_DOUBLE_MESH                   _T("Полигональные объекты? 1 - double; 0 - float")
#else
#define IDS_DOUBLE_MESH                   _T("Polygon data? 1 - double; 0 - float")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_MERGING                 _T("Объединять элементы?")
#else
#define IDS_ENTER_MERGING                 _T("Merge items?")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_MERGING_FACES           _T("Объединять грани?")
#else
#define IDS_ENTER_MERGING_FACES           _T("Merge faces?")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_MERGING_EDGES           _T("Объединять рёбра?")
#else
#define IDS_ENTER_MERGING_EDGES           _T("Merge edges?")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_NEW_PAR_VALUE           _T("Введите новое значение параметра")
#else
#define IDS_ENTER_NEW_PAR_VALUE           _T("Enter a new value of parameter")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_COORD_NUMBER            _T("Использовать координату номер: 0-x, 1-y, 2-z")
#else
#define IDS_ENTER_COORD_NUMBER            _T("Use a coordinate number: 0-x, 1-y, 2-z")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_STATE_NUMBER            _T("Угол от хорды: 0, поверхности: +1, нормали: -1")
#else
#define IDS_ENTER_STATE_NUMBER            _T("Angle from chord: 0, surface tangent: +1, surface normals: -1")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_FREE_CRD_NUMB           _T("Введите номер свободной координаты")
#else
#define IDS_ENTER_FREE_CRD_NUMB           _T("Enter a free coordinate number")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_DIMENSION               _T("Введите размер")
#else
#define IDS_ENTER_DIMENSION               _T("Enter dimension")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_SENSE                   _T("Введите направление (+1 по или -1 против)")
#else
#define IDS_ENTER_SENSE                   _T("Enter the sense (+1: forward, -1: reversed)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_CONN_TYPE               _T("Введите тип сопряжения для кривой 0-4")
#else
#define IDS_ENTER_CONN_TYPE               _T("Enter connection type for a curve 0-4")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_CLOSED                  _T("Введите замкутость? ( +1:да -1:нет )")
#else
#define IDS_ENTER_CLOSED                  _T("Enter closure? ( +1:yes -1:no )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_CLOSED_BOOL             _T("Введите замкутость?")
#else
#define IDS_ENTER_CLOSED_BOOL             _T("Enter closure?")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_NEW_DIM_VALUE           _T("Введите новое значение размера")
#else
#define IDS_ENTER_NEW_DIM_VALUE           _T("Enter a new value of dimension")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_NEW_AQUATION            _T("Введите новое уравнение")
#else
#define IDS_ENTER_NEW_AQUATION            _T("Enter a new equation")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_ANGLE                   _T("Введите угол")
#else
#define IDS_ENTER_ANGLE                   _T("Enter an angle")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_X_COORDINATE            _T("Введите координату X")
#else
#define IDS_ENTER_X_COORDINATE            _T("Enter the coordinate by X")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_Y_COORDINATE            _T("Введите координату Y")
#else
#define IDS_ENTER_Y_COORDINATE            _T("Enter the coordinate by Y")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_Z_COORDINATE            _T("Введите координату Z")
#else
#define IDS_ENTER_Z_COORDINATE            _T("Enter the coordinate by Z")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_EL_THICKNESS            _T("Введите толщину элемента")
#else
#define IDS_ENTER_EL_THICKNESS            _T("Enter the element thickness")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_ACCURACY                _T("Введите точность")
#else
#define IDS_ENTER_ACCURACY                _T("Enter accuracy")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_BAN_CORRECTION          _T("Запрет коррекции точности: 0 - нет, 1 - да")
#else
#define IDS_ENTER_BAN_CORRECTION          _T("Ban accuracy correction: 0 - false, 1 - true")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_POINT_WEIGHT            _T("Введите вес точки")
#else
#define IDS_ENTER_POINT_WEIGHT            _T("Enter a point weight")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_PAR_ON_CURVE            _T("Введите параметр на кривой ( 0 <= t <= 1 )")
#else
#define IDS_ENTER_PAR_ON_CURVE            _T("Enter parameter on the curve ( 0 <= t <= 1 )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_JOINT_ANALYSIS          _T("Отклонение: 0 - нормалей, 1 - кривизны")
#else
#define IDS_ENTER_JOINT_ANALYSIS          _T("Deviation: 0 - normals, 1 - curvatures")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_APPROX_ACCURACY         _T("Введите точность аппроксимации")
#else
#define IDS_ENTER_APPROX_ACCURACY         _T("Enter approximation accuracy")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_FIXED_POINT_ID          _T("Введите индекс фиксируемой точки (если уже есть, то будет удалена)")
#else
#define IDS_ENTER_FIXED_POINT_ID          _T("Enter index of a point to be fixed (will be deleted if already exists)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_FIXED_DERIVATIVE        _T("Зафиксировать производную? (можно редактировать)")
#else
#define IDS_ENTER_FIXED_DERIVATIVE        _T("Fix derivative? (can be edited)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_USE_CONSTRAINTS         _T("Использовать текущие ограничения? (да - использовать, нет - удалить)")
#else
#define IDS_ENTER_USE_CONSTRAINTS         _T("Use current constraints? (Yes - use, No - delete)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_PARAMETER               _T("Введите параметр")
#else
#define IDS_ENTER_PARAMETER               _T("Enter parameter")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_SAG_ARROW               _T("Введите стрелку прогиба")
#else
#define IDS_ENTER_SAG_ARROW               _T("Enter a sag arrow")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_ANG_TOLERANCE           _T("Введите угловую толерантность")
#define IDS_ENTER_LEN_TOLERANCE           _T("Введите допуск в единицах длины")
#else
#define IDS_ENTER_ANG_TOLERANCE           _T("Enter the angular tolerance")
#define IDS_ENTER_LEN_TOLERANCE           _T("Enter tolerance in units of length")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_LENGHT_ACR              _T("Введите точность вычисления длины кривой")
#else
#define IDS_ENTER_LENGHT_ACR              _T("Enter a curve length calculation tolerance")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_SOLUTION_ACR            _T("Введите точность решения")
#else
#define IDS_ENTER_SOLUTION_ACR            _T("Enter solution tolerance")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_ITER_MAX_NUMB           _T("Введите максимальное количество итераций")
#else
#define IDS_ENTER_ITER_MAX_NUMB           _T("Enter the maximal number of iterations")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_CURSOR_AREA             _T("Введите область захвата курсора")
#else
#define IDS_ENTER_CURSOR_AREA             _T("Enter the cursor capture region")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_BYPASS_TYPE             _T("Введите способ обхода углов (0-дугой 1-срезом)")
#else
#define IDS_ENTER_BYPASS_TYPE             _T("Enter a method of angles bypass (0-by arc 1-by cut)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_BYPASS_SIDE             _T("Выберете сторону обхода (0-слева 1-справа 2-с двух сторон )")
#else
#define IDS_ENTER_BYPASS_SIDE             _T("Choose a side of bypass (0-from the left 1-from the right 2-from the both sides )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_MAJOR_RADIUS            _T("Введите большой радиус A")
#else
#define IDS_ENTER_MAJOR_RADIUS            _T("Enter the major radius A")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_MINOR_RADIUS            _T("Введите малый радиус B")
#else
#define IDS_ENTER_MINOR_RADIUS            _T("Enter the minor radius B")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_FACET_LENGHT            _T("Введите размер фаски")
#else
#define IDS_ENTER_FACET_LENGHT            _T("Enter the chamfer size")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_POINT_NUMBER            _T("Введите количество точек")
#else
#define IDS_ENTER_POINT_NUMBER            _T("Enter points number")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_FASTENER_TYPE           _T("Введите тип крепежа ( 0 - заклепка с потайной головкой, 1 - с универсальной головкой, 2 - с полукруглой коловкой, 3 - с плоской головкой )")
#else
#define IDS_ENTER_FASTENER_TYPE           _T("Enter fastener type ( 0 - countersunk head rivet, 1 - universal head, 2 - round head, 3 - flat head )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_DIAMETER                _T("Введите диаметр")
#else
#define IDS_ENTER_DIAMETER                _T("Enter diameter")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_HOLES_ONLY              _T("Только отверстия? -1, Только крепёж? 0, Все +1")
#else
#define IDS_ENTER_HOLES_ONLY              _T("Holes only? -1, Rivet only? 0, Both +1")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_TRACING_MODE            _T("0-вписанный, 1-описанный")
#else
#define IDS_ENTER_TRACING_MODE            _T("0-inscribed, 1-circumscribed")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_DELETE_AREA             _T("Где удалять ( 0-снаружи; 1-внутри )")
#else
#define IDS_ENTER_DELETE_AREA             _T("Where to delete ( 0-outside; 1-inside )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_PARAM_VALUE             _T("Введите значение параметра")
#else
#define IDS_ENTER_PARAM_VALUE             _T("Enter the parameter value")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_EQUATION                _T("Введите уравнение")
#else
#define IDS_ENTER_EQUATION                _T("Enter an equation")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_NEW_EQUATION            _T("Введите новое уравнение")
#else
#define IDS_ENTER_NEW_EQUATION            _T("Enter a new equation")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_FORMULA                 _T("Введите формулу")
#else
#define IDS_ENTER_FORMULA                 _T("Enter a formula")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_PARAM_NUMBER            _T("Введите номер параметра")
#else
#define IDS_ENTER_PARAM_NUMBER            _T("Enter a parameter number")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_SHAPE                   _T("Введите форму ( 0-сфероид; 1-тороид )")
#else
#define IDS_ENTER_SHAPE                   _T("Enter a shape ( 0-spheroid; 1-toroid )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_BINDING                 _T("Введите привязку ( 0-образующая; 1-направляющая )")
#else
#define IDS_ENTER_BINDING                 _T("Enter a binding ( 0-generating; 1-guide )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_METHOD                  _T("Введите способ ( 0-парраллельно; 1-жестко; 2-ортогонально )")
#else
#define IDS_ENTER_METHOD                  _T("Enter a method ( 0-parallel; 1-rigidly; 2-orthogonal )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_MEMORY                  _T("Копирование: 0-нет, 1-только новое, 2-кроме носителей; 4-полное")
#else
#define IDS_ENTER_MEMORY                  _T("Copying: 0-none, 1-only new, 2-except carriers; 4-full")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_IM_SHIFT_STEP           _T("Введите шаг смещения изображения")
#else
#define IDS_ENTER_IM_SHIFT_STEP           _T("Enter image shift step")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_CURVATURE_PLOT          _T("Введите масштаб рисования кривизны")
#else
#define IDS_ENTER_CURVATURE_PLOT          _T("Enter curvature scale for plot")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_IM_SHIFT_ANG            _T("Введите шаг поворота изображения")
#else
#define IDS_ENTER_IM_SHIFT_ANG            _T("Enter image rotation step")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_BACKGROUND              _T("Введите общий фон объекта")
#else
#define IDS_ENTER_BACKGROUND              _T("Enter the object background")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_DIFFUSION               _T("Введите диффузию объекта")
#else
#define IDS_ENTER_DIFFUSION               _T("Enter object's diffusion")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_REFLECTIVITY            _T("Введите зеркальность объекта")
#else
#define IDS_ENTER_REFLECTIVITY            _T("Enter object's reflectivity")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_BRIGHTNESS              _T("Введите блеск объекта")
#else
#define IDS_ENTER_BRIGHTNESS              _T("Enter object's brightness")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_OPACITY                 _T("Введите непрозрачность объекта")
#else
#define IDS_ENTER_OPACITY                 _T("Enter object's opacity")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_EMISSION                _T("Введите излучение объекта")
#else
#define IDS_ENTER_EMISSION                _T("Enter object's emission")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_HEIGHT                  _T("Введите высоту")
#else
#define IDS_ENTER_HEIGHT                  _T("Enter a height")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_CONTRACTION             _T("Введите сужение")
#else
#define IDS_ENTER_CONTRACTION             _T("Enter tapering")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_OFFSET                  _T("Введите величину расширения")
#else
#define IDS_ENTER_OFFSET                  _T("Enter offset distance")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_SLOPE_ANGLE             _T("Введите угол наклона")
#else
#define IDS_ENTER_SLOPE_ANGLE             _T("Enter a slope angle")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_START_DIRECTION         _T("Введите направление в начале (0-по умолчанию; 1-по нормали)")
#else
#define IDS_ENTER_START_DIRECTION         _T("Enter directions at the start (0-default; 1-by the normal)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_END_DIRECTION           _T("Введите направление в конце (0-по умолчанию; 1-по нормали)")
#else
#define IDS_ENTER_END_DIRECTION           _T("Enter directions at the end (0-default; 1-by the normal)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SET_START_NORMAL              _T("Построить купол в начале? (0-нет; 1-да)")
#else
#define IDS_SET_START_NORMAL              _T("Build a dome at the start? (0-no; 1-yes)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SET_END_NORMAL                _T("Построить купол в конце? (0-нет; 1-да)")
#else
#define IDS_SET_END_NORMAL                _T("Build a dome at the end? (0-no; 1-yes)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_DERIVE_FACTOR_AT_START        _T("Введите модификатор купола в начале. (>1.0 - менее острый ; <1.0 - более острый)")
#else
#define IDS_DERIVE_FACTOR_AT_START        _T("Enter the dome modifier at the beginning. (>1.0 - less acute; < 1.0 - more acute)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_DERIVE_FACTOR_AT_END          _T("Введите модификатор купола в конце. (>1.0 - менее острый ; <1.0 - более острый)")
#else
#define IDS_DERIVE_FACTOR_AT_END          _T("Enter the dome modifier at the end. (>1.0 - less acute; < 1.0 - more acute)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_START_PLANE             _T("Укажите плокость для определения нормали в начале тела")
#else
#define IDS_ENTER_START_PLANE             _T("Enter plane to determine the normal at start solid")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_END_PLANE               _T("Укажите плоскость для определения нормали в конце тела")
#else
#define IDS_ENTER_END_PLANE               _T("Enter plane to determine the normal at end solid")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_DIRECTION_SEG           _T("Укажите отрезок, задающий направление выдавливания")
#else
#define IDS_ENTER_DIRECTION_SEG           _T("Enter a segment specifying the extrusion direction")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_PLACE_NORMAL_DIR              _T("Направление выдавливания совпадает с нормалью плоскости образующей")
#else
#define IDS_PLACE_NORMAL_DIR              _T("The extrusion direction coincides with the normal of the generatrix plane")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_SIDE                    _T("Введите сторону 0-Left, 1-Right, 2-Up, 3-Down")
#else
#define IDS_ENTER_SIDE                    _T("Enter a side 0-Left, 1-Right, 2-Up, 3-Down")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_BUILD_METHOD            _T("Введите способ 0-до тела, 1-до грани")
#else
#define IDS_ENTER_BUILD_METHOD            _T("Enter a method 0-up to a solid, 1-up to a face")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_KEEP_EDGE               _T("Что сохранять: поверхность(-1), кромку(1), auto(0) ?")
#else
#define IDS_ENTER_KEEP_EDGE               _T("What to keep: surface(-1), border(1), auto(0) ?")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_BUILD_TYPE              _T("0-по толщине, 1-на расстояние, 2-до поверхности")
#else
#define IDS_ENTER_BUILD_TYPE              _T("0-by thickness, 1-at a distance, 2-up to a surface")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_CONSTR_NAME             _T("Введите имя ограничения ?")
#else
#define IDS_ENTER_CONSTR_NAME             _T("Enter the constraint name ?")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_BUILD_NUMBER            _T("Введите число построений")
#else
#define IDS_ENTER_BUILD_NUMBER            _T("Enter the number of constructions")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_STEP_TYPE               _T("0 - по прогибу, 1 - по углу, 2 - по прогибу и углу, 3 - по длине, \n4 - все вместе, 5 - МЦХ")
#else
#define IDS_ENTER_STEP_TYPE               _T("0 - sag, 1 - angle, 2 - sag+angle, 3 - length, \n4 - all together, 5 - MIP")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_FILLET_TYPE             _T(" Форма поверхности: -1, 0, 1 ")
#else
#define IDS_ENTER_FILLET_TYPE             _T(" Shape of surface: -1, 0, 1 ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_SIDE_SHAPE              _T(" Обрезка края поверхности: 0, 1, 2, 3, 4 ")
#else
#define IDS_ENTER_SIDE_SHAPE              _T(" Surface sides cropping: 0, 1, 2, 3, 4 ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_FACE_HANDLING           _T(" Обработка опорных граней: 0, 1, 2 ")
#else
#define IDS_ENTER_FACE_HANDLING           _T(" Initial faces processing: 0, 1, 2 ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_FACE_DIVIDE             _T(" Разделять оболочку на грани: 0, 1 ")
#else
#define IDS_ENTER_FACE_DIVIDE             _T(" Division the shell into faces: 0, 1 ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_EDIT_GROUP              _T(" Изменить: первую группу - 1, вторую группу - 2 ")
#else
#define IDS_ENTER_EDIT_GROUP              _T(" Edit: first group - 1, second group - 2 ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_MAKE_GRID               _T("Строить триангуляцию модели \n(1 - да, 0 - нет)")
#else
#define IDS_ENTER_MAKE_GRID               _T("Whether to build triangulation of the model \n(1 - yes, 0 - no)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_STEP                    _T("Введите шаг")
#else
#define IDS_ENTER_STEP                    _T("Enter a step")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_DO_EXACT_OBJECT              _T("Построить объект на числах double (1 - да, 0 - нет)")
#else
#define IDS_DO_EXACT_OBJECT              _T("Object will created on double data (1 - yes, 0 - no)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_DUAL_SEAMS              _T("Сохранить совпадающие точки швов (1 - да, 0 - нет)")
#else
#define IDS_ENTER_DUAL_SEAMS              _T("Keep dual points of seam edges (1 - yes, 0 - no)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_BVT_LEVEL                     _T("Уровень дерева объемов от 1 до n.")
#else
#define IDS_BVT_LEVEL                     _T("Level of bounding volume tree from 1 to n.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_THREAD_LENGTH           _T("Введите длину резьбы")
#else
#define IDS_ENTER_THREAD_LENGTH           _T("Enter thread length")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_THREAD_STEP             _T("Введите шаг резьбы")
#else
#define IDS_ENTER_THREAD_STEP             _T("Enter thread step")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_THR_MAP_TYPE            _T("Введите тип отоб. резьбы 0-полный, 1-разрез, 2-сечение")
#else
#define IDS_ENTER_THR_MAP_TYPE            _T("Enter a method of thread mapping 0-full, 1-profile, 2-section")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_ADAPT_THREAD            _T("Адаптировать резьбу к телу?")
#else
#define IDS_ENTER_ADAPT_THREAD            _T("Adapt the thread to a solid?")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_OBJECT_TYPE             _T("Введите: 0-поверхность, 1-тело")
#else
#define IDS_ENTER_OBJECT_TYPE             _T("Enter: 0-surface, 1-solid")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_PROJECTION_TYPE         _T("Проекция: 0-параллельная, 1-перспективная")
#else
#define IDS_ENTER_PROJECTION_TYPE         _T("Projection: 0-parallel, 1-perspective")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_ADAPT_TYPE              _T("Введите тип адаптации")
#else
#define IDS_ENTER_ADAPT_TYPE              _T("Enter adapting type")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_STAMP_TYPE              _T("Введите тип штамповки ( 0 - закрытая, 1 - открытая )")
#else
#define IDS_ENTER_STAMP_TYPE              _T("Enter a type of stamping  ( 0 - closed, 1 - open )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_SIMPLIFY_COEF           _T("Введите коэффициент упрощения 0-100")
#else
#define IDS_ENTER_SIMPLIFY_COEF           _T("Enter simplification coefficient 0-100")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_BEAD_TYPE               _T("Введите тип буртика ( 0 - круглый, 1 - V-образный, 2 - U-образный )")
#else
#define IDS_ENTER_BEAD_TYPE               _T("Enter bead type ( 0 - circular, 1 - V-shaped, 2 - U-shaped )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_HIDE_EDGE               _T("Выводить невидимые линии ? ( 0-нет 1-да )")
#else
#define IDS_ENTER_HIDE_EDGE               _T("Show hidden lines ? ( 0-no 1-yes )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_SHADE_SPCURVES          _T("Затенять пространственные кривые телами ?")
#else
#define IDS_ENTER_SHADE_SPCURVES          _T("Shade spatial curves by solids ?")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_ADD_CENTERLINES         _T("Выводить осевые линии ?")
#else
#define IDS_ENTER_ADD_CENTERLINES         _T("Show center lines ?")
#endif

#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_HOLE_TYPE               _T("Введите тип отверстия: 0,1,2,3,4,5,6")
#else
#define IDS_ENTER_HOLE_TYPE               _T("Enter hole type: 0,1,2,3,4,5,6")
#endif

#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_SURF_CURVATURE_TYPE     _T("Тип кривизны: norm(0), min(1), max(2), gauss(3), mean(4), normU(5), normV(6).")
#else
#define IDS_ENTER_SURF_CURVATURE_TYPE     _T("Curvature type: norm(0), min(1), max(2), gauss(3), mean(4), normU(5), normV(6).")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_EXTREMES_SEARCH_ALG     _T("Алгоритм: градиентый спуск(1), выделение линий(2)")
#else
#define IDS_ENTER_EXTREMES_SEARCH_ALG     _T("Algorithm: gradient descent(1), lines segregation(2)")
#endif

#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_CAP_DIAMETER            _T("Введите диаметр шляпки")
#else
#define IDS_ENTER_CAP_DIAMETER            _T("Enter the cap diameter")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_CAP_DIPTH               _T("Введите глубину шляпки")
#else
#define IDS_ENTER_CAP_DIPTH               _T("Enter the cap depth")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_HOLE_ANGLE              _T("Введите угол конуса")
#else
#define IDS_ENTER_HOLE_ANGLE              _T("Enter a taper angle")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_HOLE_DIAMETER           _T("Введите диаметр отверстия")
#else
#define IDS_ENTER_HOLE_DIAMETER           _T("Enter the hole diameter")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_HOLE_DIPTH              _T("Введите глубину отверстия")
#else
#define IDS_ENTER_HOLE_DIPTH              _T("Enter the hole depth")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_HOLE_PROLONG            _T("Продлевать отверстие вверх?")
#else
#define IDS_ENTER_HOLE_PROLONG            _T("Whether to extend the hole upwards?")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_SPIKE_ANGLE             _T("Введите угол законцовки")
#else
#define IDS_ENTER_SPIKE_ANGLE             _T("Enter the tip angle")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_SHOW_ALL                _T("Показывать все? ( 1-да, 0-нет )")
#else
#define IDS_ENTER_SHOW_ALL                _T("Whether to show everything? ( 1-yes, 0-no )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_EXT_FLAG                _T("На продолжении? ( 1-да, 0-нет )")
#else
#define IDS_ENTER_EXT_FLAG                _T("On the extension? ( 1-yes, 0-no )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_EXT_FLAG_BOOL           _T("На продолжении?")
#else
#define IDS_ENTER_EXT_FLAG_BOOL           _T("On the extension?")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_USE_RANGE               _T("Использовать диапазон? ( 1-да, 0-нет )")
#else
#define IDS_ENTER_USE_RANGE               _T("Is the range to be used? ( 1-yes, 0-no )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_USE_OR_NOT_BOOL         _T("Использовать?") // _T("Использовать? ( 1-да, 0-нет )")
#else
#define IDS_ENTER_USE_OR_NOT_BOOL         _T("Is to be used?") // _T("Is to be used? ( 1-yes, 0-no )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_USE_YOURSELF            _T("Использовать открытые тела? (1 - да, 0 - нет)")
#else                                    
#define IDS_ENTER_USE_YOURSELF            _T("Use open solids? (1 - yes, 0 - no)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_NUMBER_X                _T("Введите количество ячеек по Х (0 < число < 150)")
#else                                    
#define IDS_ENTER_NUMBER_X                _T("Enter number of cells by X (0 < number < 150)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_NUMBER_Y                _T("Введите количество ячеек по Y (0 < число < 150)")
#else                                        
#define IDS_ENTER_NUMBER_Y                _T("Enter number of cells by Y (0 < number < 150)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_NUMBER_Z                _T("Введите количество ячеек по Z (0 < число < 150)")
#else                                        
#define IDS_ENTER_NUMBER_Z                _T("Enter number of cells by Z (0 < number < 150)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_KIND_OF_SOLID           _T("Введите тип блоков для отрисовки (-2 - пересекающиеся, -1 - внешние, 0 - внутренние)")
#else                                     
#define IDS_ENTER_KIND_OF_SOLID           _T("Enter type of blocks to draw (0 - intersecting, 1 outside, 2 - inside, 3 - all)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_DRAW_SECTION            _T("В каком сечении рисовать блоки (0 - середина X, 1 - середина Y, 2 - середина Z, 3 - везде)")
#else                                    
#define IDS_ENTER_DRAW_SECTION            _T("In which section to draw blocks (0 - middle X, 1 - middle Y, 2 - middle Z, 3 - all)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_USE_OR_NOT              _T("Использовать? ( 1-да, 0-нет )")
#else
#define IDS_ENTER_USE_OR_NOT              _T("Is to be used? ( 1-yes, 0-no )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_VIS_OR_NOT              _T("Видимый объект? ( 1-да, 0-нет )")
#else
#define IDS_ENTER_VIS_OR_NOT              _T("Is it visible? ( 1-yes, 0-no )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_PLACE_TYPE              _T("Левая система координат? ( 1-да, 0-нет )")
#else
#define IDS_ENTER_PLACE_TYPE              _T("Is the coordinate system left? ( 1-yes, 0-no )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_BUILD_FROM_PFE          _T("Строить от плоскости (0), грани(1) или ребра? (2)")
#else
#define IDS_ENTER_BUILD_FROM_PFE          _T("Build from a plane (0), a face(1) or an edge? (2)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_SLANT                   _T("Введите катет")
#else
#define IDS_ENTER_SLANT                   _T("Enter a cathetus")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_SPAN                    _T("Введите хорду")
#else
#define IDS_ENTER_SPAN                    _T("Enter a chord")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_SOLIDMASS               _T("Введите массу тела ")
#else
#define IDS_ENTER_SOLIDMASS               _T("Enter mass of the solid ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_SOLIDDENSITY            _T("Введите плотность тела ")
#else
#define IDS_ENTER_SOLIDDENSITY            _T("Enter density of the solid ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_ASSEMBLYMASS            _T("Введите массу сборки ")
#else
#define IDS_ENTER_ASSEMBLYMASS            _T("Enter mass of the assembly ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_ASSEMBLYDENSITY         _T("Введите плотность сборки ")
#else
#define IDS_ENTER_ASSEMBLYDENSITY         _T("Enter density of the assembly ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_BOOLEANOPERTYPE         _T("Тип операции (0 - объед., 1 - пересеч., 2 - разн.)")
#else
#define IDS_ENTER_BOOLEANOPERTYPE         _T("Operation type (0 - union, 1 - intersection, 2 -subtraction)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_TRACING_TYPE            _T("Тип обхода вершины (0 - срезом, 1 - скр., 2 - спец.скр.)")
#else
#define IDS_ENTER_TRACING_TYPE            _T("Type of vertex bypass (0 - by cutting, 1 - by rounding, 2 - by a special rounding)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_IN_TIP_TYPE             _T("Тип внутр. законцовки (0-нет, 1-м/д верш, 2-лин, 3-дуг)")
#else
#define IDS_ENTER_IN_TIP_TYPE             _T("Type of the internal tip (0-none, 1-between vertices, 2-linear, 3-arc)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_OUT_TIP_TYPE            _T("Тип внешн. законцовки (0-нет, 1-лин, 2-дуг, 3-лом, 4-накл.)")
#else
#define IDS_ENTER_OUT_TIP_TYPE            _T("Type of the external tip (0-none, 1-linear, 2-arc, 3-polyline, 4-inclined)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_SMOOTH_JOINT            _T("Гладкий стык сегментов? ( 1-да, 0-нет )")
#else
#define IDS_ENTER_SMOOTH_JOINT            _T("Is segments joint smooth? ( 1-yes, 0-no )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_SEGMENT_INDEX           _T("Введите индекс сегмента")
#else
#define IDS_ENTER_SEGMENT_INDEX           _T("Enter a segment index")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_CURVE_INDEX             _T("Введите индекс кривой")
#else
#define IDS_ENTER_CURVE_INDEX             _T("Enter a curve index")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_ALL_CORNERS             _T("На всех углах? ( 1-да, 0-нет )")
#else
#define IDS_ENTER_ALL_CORNERS             _T("All the corners? ( 1-yes, 0-no )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_CHAMFER_TYPE            _T("Введите способ задания ( 0-длина+угол, 1-длина+длина )")
#else
#define IDS_ENTER_CHAMFER_TYPE            _T("Enter a method of definition ( 0-length+angle, 1-length+length )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_CHANGE_DIR              _T("Изменить направление? ( 1 - да, 0 - нет )")
#else
#define IDS_ENTER_CHANGE_DIR              _T("Is the direction to be changed? ( 1 - yes, 0 - no )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_TIMERESULTFILE          _T("Печатать файл замеров времени? (0-нет 1-да )")
#else
#define IDS_ENTER_TIMERESULTFILE          _T("Print the file of time measurements? (0-no 1-yes )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_CHECK_READ_TIME         _T("Выводить время чтения? (0-нет 1-да )")
#else
#define IDS_ENTER_CHECK_READ_TIME         _T("Show read time? (0-no 1-yes )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_CHECK_WRITE_TIME         _T("Выводить время записи? (0-нет 1-да )")
#else
#define IDS_ENTER_CHECK_WRITE_TIME         _T("Show write time? (0-no 1-yes )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_CHECK_PROC_TIME         _T("Выводить время выполнения? (0-нет 1-да )")
#else
#define IDS_ENTER_CHECK_PROC_TIME         _T("Show processing time? (0-no 1-yes )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_THROUGH_ITEMS           _T("Через элементы? ( 1-да, 0-нет )")
#else
#define IDS_ENTER_THROUGH_ITEMS           _T("Through items? ( 1-yes, 0-no )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_THROUGH_ITEMS_BOOL      _T("Через элементы?")
#else
#define IDS_ENTER_THROUGH_ITEMS_BOOL      _T("Through items?")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_CHECKSELFINT            _T("Проверять самопересечения? ( 1-да, 0-нет )")
#else
#define IDS_ENTER_CHECKSELFINT            _T("Check self-intersections? ( 1-yes, 0-нет )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_CHECKINT                _T("Проверять на пересечения? ( 1-да, 0-нет )")
#else
#define IDS_ENTER_CHECKINT                _T("Check intersections? ( 1-yes, 0-нет )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_INVERSE                 _T("Инвертировать? ( 1-да, 0-нет )")
#else
#define IDS_ENTER_INVERSE                 _T("Is to be inverted? ( 1-yes, 0-no )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_CALC_POINTS_DERS        _T("Рассчитывать точки и производные? ( 1-да, 0-нет )")
#else
#define IDS_ENTER_CALC_POINTS_DERS        _T("Calculate points and derivatives? ( 1-yes, 0-no )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_DERIVATIVE_NUMBER       _T("Введите номер производной ( 1-u, 2-v, 3-uu, 4-vv, 5-uv, else-n )")
#else
#define IDS_ENTER_DERIVATIVE_NUMBER       _T("Enter derivative number ( 1-u, 2-v, 3-uu, 4-vv, 5-uv, else-n )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_EXTENSIONTYPE           _T("Тип? ( 0 - как есть, 1 - по касательно, 2 - по направлению )")
#else
#define IDS_ENTER_EXTENSIONTYPE           _T("Type? ( 0 - as is, 1 - tangentially, 2 - in the direction )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_EXTENSIONWAY            _T("Способ? ( 0 - на расстояние, 1 - до вершины, 2 - до грани оболочки )")
#else
#define IDS_ENTER_EXTENSIONWAY            _T("Method? ( 0 - by a distance, 1 - up to a vertex, 2 - up to shell face )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_EXTENSIONKIND           _T("Боковые ребра? ( 0 - по нормали, 1 - продлить исходные )")
#else
#define IDS_ENTER_EXTENSIONKIND           _T("Border edges? ( 0 - by the normal, 1 - extend initial ones )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_EXTENSION_SOT           _T("Тип оболочки-препятствия? ( 0 - до оболочки, 1 - через слой оболочки )")
#else
#define IDS_ENTER_EXTENSION_SOT           _T("Shell obstacle type? ( 0 - up to shell, 1 - through layer )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_EXTENSIONNEW            _T("Новую грань? ( 1 - да, 0 - нет )")
#else
#define IDS_ENTER_EXTENSIONNEW            _T("New face? ( 1 - yes, 0 - no )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_ITEM_STEP_TYPE          _T("Введите тип шага (0 - по параметру, 1 - по прогибу, 2 - по углу, 3 - по длине)")
#else
#define IDS_ENTER_ITEM_STEP_TYPE          _T("Enter a step type(0 - by parameter, 1 - by sag, 2 - by angle, 3 - by length)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_CHECKRESULT             _T("Проверить результат ( 1 - да, 0 - нет )")
#else
#define IDS_ENTER_CHECKRESULT             _T("Check the result ( 1 - yes, 0 - no )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_SURF_CONN_TYPE          _T("Тип сопряжения с поверхностью (0 - 6)")
#else
#define IDS_ENTER_SURF_CONN_TYPE          _T("Type of connection with the surface (0 - 6)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_TRUNCATE                _T("Усекать результат? ( 1 - да, 0 - нет)")
#else
#define IDS_ENTER_TRUNCATE                _T("Truncate the result? ( 1 - yes, 0 - no)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_TRUNCATE_BOOL           _T("Усекать результат?")
#else
#define IDS_ENTER_TRUNCATE_BOOL           _T("Truncate the result?")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_EDGE_CONNECTING         _T("Продолжить исходных боковые ребра? ( 1-да, 0-нет )")
#else
#define IDS_ENTER_EDGE_CONNECTING         _T("Extend the initial border edges? ( 1-yes, 0-no )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_U_POINT_NUMBER          _T("Введите количество точек по U")
#else
#define IDS_ENTER_U_POINT_NUMBER          _T("Enter a number of points by U")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_V_POINT_NUMBER          _T("Введите количество точек по V")
#else
#define IDS_ENTER_V_POINT_NUMBER          _T("Enter a number of points by V")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_ELEMENTWISE             _T("Поэлементно? ( 1-да, 0-нет )")
#else
#define IDS_ENTER_ELEMENTWISE             _T("Elementwise? ( 1-yes, 0-no )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_ELEMENTWISE_BOOL        _T("Поэлементно?")
#else
#define IDS_ENTER_ELEMENTWISE_BOOL        _T("Elementwise?")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_ROTATIOBAXIS            _T("Перемещение (0) или вращение (1)? )")
#else
#define IDS_ENTER_ROTATIOBAXIS            _T("Translation (0) or rotation (1)? )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_DIR                     _T("Введите вектор направления (покомпонентно - нажмите TAB)")
#else
#define IDS_ENTER_DIR                     _T("Enter a vector of direction (componentwise - press TAB)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_SPIRAL_TYPE             _T("Тип (0-кивая радиусов, 1-осевая кивая)")
#else
#define IDS_ENTER_SPIRAL_TYPE             _T("Type (0-curve of radii, 1-axial curve)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_JOIN_BY_VERTICES        _T("Соединять через вершины? ( 1-да, 0-нет )")
#else
#define IDS_ENTER_JOIN_BY_VERTICES        _T("Join by vertices? ( 1-yes, 0-no )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SET_PROPORTIONAL_PARAMS       _T("Установить псевдо-натуральную параметризацию? ( 1-да, 0-нет )")
#else
#define IDS_SET_PROPORTIONAL_PARAMS       _T("Set pseudo-natural parameterization? ( 1-yes, 0-no )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SET_DIVIDE_SHELL              _T("Разделять оболочку на грани? ( 1-да, 0-нет )")
#else
#define IDS_SET_DIVIDE_SHELL              _T("Divide the shell into faces? ( 1-yes, 0-no )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SET_PERMISSIBLE_MISMATCH      _T("Допустимое несовпадение противолежащих точек")
#else
#define IDS_SET_PERMISSIBLE_MISMATCH      _T("Permissible mismatch of opposite points")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_USE_APPROX              _T("Использовать аппроксимацию? ( 1-да, 0-нет )")
#else
#define IDS_ENTER_USE_APPROX              _T("Use approximation? ( 1-yes, 0-no )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_U_USE_APPROX            _T("Использовать аппроксимацию по U? ( 1-да, 0-нет )")
#else
#define IDS_ENTER_U_USE_APPROX            _T("Use approximation by U? ( 1-yes, 0-no )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_V_USE_APPROX            _T("Использовать аппроксимацию по V? ( 1-да, 0-нет )")
#else
#define IDS_ENTER_V_USE_APPROX            _T("Use approximation by V? ( 1-yes, 0-no )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_AXIS_IND                _T("Введите индекс оси координат (1-ox, 2-oy, 3-oz)")
#else
#define IDS_ENTER_AXIS_IND                _T("Enter an index of coordinate axis (1-ox, 2-oy, 3-oz)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_BOTH_SIDES              _T("В обе стороны? (1-да, 0-нет)")
#else
#define IDS_ENTER_BOTH_SIDES              _T("To the both sides? (1-yes, 0-no)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_ITEM_GRID_TYPE          _T("Введите тип сетки (0 - прямоугольная, 1 - концентрическая, 2 - гексагональная)")
#else
#define IDS_ENTER_ITEM_GRID_TYPE          _T("Enter the grid type (0 - rectangular, 1 - concentric, 2 - hexagonal)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_DISCR                   _T("Введите дискриминант 0-1")
#else
#define IDS_ENTER_DISCR                   _T("Enter discriminant 0-1")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_CURVE_IS_AXIS           _T("Трактовать кривую как ось? (1 - да, 0 - нет)")
#else
#define IDS_ENTER_CURVE_IS_AXIS           _T("Treat the curve as axis? (1 - yes, 0 - no)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_SURFACE_PARAM           _T("Сохранить параметризацию поверхности-образца? (1 - да, 0 - нет)")
#else
#define IDS_ENTER_SURFACE_PARAM           _T("Keep the parametrization of a sample surface? (1 - yes, 0 - no)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_USE_PARAM_POINTS              _T("Использовать ли параметрические точки? (1 - да, 0 - нет)")
#else
#define IDS_USE_PARAM_POINTS              _T("Whether to use parametric points? (1 - yes, 0 - no)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_SURFACE_NORMAL          _T("Сохранить направление нормали (1 - да, 0 - нет)")
#else
#define IDS_ENTER_SURFACE_NORMAL          _T("Keep the normal direction (1 - yes, 0 - no)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_DOWN                    _T("Строить вниз (1 - да, 0 - нет)")
#else
#define IDS_ENTER_DOWN                    _T("Build downwards (1 - yes, 0 - no)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_DOWN_BOOL               _T("Строить вниз")
#else
#define IDS_ENTER_DOWN_BOOL               _T("Build downwards")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_BUILD_PFANTOM                 _T("Строить фантом? 1")
#else
#define IDS_BUILD_PFANTOM                 _T("Build pfantom? 1")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_STEP_PROTOCOL           _T("Укажите протокол для экспорта ( 0 - 203, другое - 214 )")
#else
#define IDS_ENTER_STEP_PROTOCOL           _T("Specify a protocol for export ( 0 - 203, other - 214 )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_STITCH_SURFACES         _T("Сшивать поверхности? ( 0 - нет, другое - да )")
#else
#define IDS_ENTER_STITCH_SURFACES         _T("Stitch the surfaces? ( 0 - no, other - yes )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_JOIN_SIMILAR_FACES      _T("Объединять подобные грани? ( 0 - нет, другое - да )")
#else
#define IDS_ENTER_JOIN_SIMILAR_FACES      _T("Join similar faces? ( 0 - no, other - yes )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_ADD_REMOVED_FACES_AS_SHELLS   _T("Добавлять удаленные грани в качестве оболочек? ( 0 - нет, другое - да )")
#else
#define IDS_ENTER_ADD_REMOVED_FACES_AS_SHELLS   _T("Add removed faces as shells? ( 0 - no, other - yes )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_IMPORT_BREP             _T("Импортировать граничное представлеие пи наличии? ( 0 - нет, другое - да )")
#else
#define IDS_ENTER_IMPORT_BREP             _T("Prefer B-rep, if exists? ( 0 - no, other - yes )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_STITCH_GRIDS            _T("Сшивать сетки триангуляции? ( 0 - нет, другое - да )")
#else
#define IDS_ENTER_STITCH_GRIDS            _T("Stitch the grids? ( 0 - no, other - yes )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_STITCH_PRECISION        _T("Укажите точность сшивки")
#else
#define IDS_ENTER_STITCH_PRECISION        _T("Specify stitch percision")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_USE_GRIDSURFACE         _T("Использовать поверхность триангуляции? ( 0 - нет, другое - да )")
#else
#define IDS_ENTER_USE_GRIDSURFACE         _T("Use the surface of grid? ( 0 - no, other - yes )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_WRITE_FILETYPE          _T("Укажите тип файла для записи ( 0 - текстовый, другое - бинарный )")
#else
#define IDS_ENTER_WRITE_FILETYPE          _T("Specify file type for export ( 0 - text file, other - binary file )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_STRING_SMAP_PROP              _T("1-отобр.без сеч.,2-только сеч.,3-отоб.в рассч.виде")
#else
#define IDS_STRING_SMAP_PROP              _T("1-show without sections,2-sections only,3-in sectioned form")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_STRING_SENSE_VALUE            _T("Направление ?(-1-обратное,0-оба,1-прямое)")
#else
#define IDS_STRING_SENSE_VALUE            _T("Direction ?(-1-reversed,0-both,1-forward)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_OGL_VIEWTYPE                  _T("Способ отображения OGL")
#else
#define IDS_OGL_VIEWTYPE                  _T("Method of viewing OGL")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_USE_BOUNDS_BOOL               _T("Учитывать границы поверхности ?")
#else
#define IDS_USE_BOUNDS_BOOL               _T("Use surface boundaries ?")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_DELETE_LASTITEM_BOOL          _T("Удалить созданный последним объект?")
#else
#define IDS_DELETE_LASTITEM_BOOL          _T("Delete the last created object?")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SAVE_TO_FILE                  _T("Записать в файл?")
#else
#define IDS_SAVE_TO_FILE                  _T("Write to file?")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_TANGENT_PROLONG               _T("Продолжать по касательной? (1 - да, 0 - нет)")
#else
#define IDS_TANGENT_PROLONG               _T("Extend by a tangent? (1 - yes, 0 - no)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_GENERAL_PROLONG               _T("Продлить по общему участку")
#else
#define IDS_GENERAL_PROLONG               _T("Extend by the common region")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_DERIVATIVES_CONTINUOUS        _T("Сделать производные непрерывными? ( 1 - да, 0 - нет )")
#else
#define IDS_DERIVATIVES_CONTINUOUS         _T("Make the derivatives continuous? ( 1 - yes, 0 - no )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_NUMBER_OR_SHIFT         _T("Введите индекс или смещение")
#else
#define IDS_ENTER_NUMBER_OR_SHIFT         _T("Enter an index or a shift")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_CONIC                  _T("Коэффициент полноты: 0.0 (для дуги окружности) или 0.05-0.95")
#else
#define IDS_SELECT_CONIC                  _T("Coefficient of completeness: 0.0 (for circle arc) or 0.05-0.95")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_BEG_FILLET_LENGTH             _T("Остановка от начала скругления")
#else
#define IDS_BEG_FILLET_LENGTH             _T("Termination form the fillet start")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_END_FILLET_LENGTH             _T("Остановка до конца  скругления")
#else
#define IDS_END_FILLET_LENGTH             _T("Termination to the end of fillet")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_FILLET_STOP_FACES             _T("Указать грани другого тела для остановки скругления? (1 - да, 0 - нет)")
#else
#define IDS_FILLET_STOP_FACES             _T("Specify faces of other solid to bound the fillet? (1 - yes, 0 - no)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_FILLET_VERTEX_FACES             _T("Указать грани этого же тела для обработки вершины скругления? (1 - да, 0 - нет)")
#else
#define IDS_FILLET_VERTEX_FACES             _T("Specify faces of the same solid to process the vertex of the fillet ? (1 - yes, 0 - no)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_NOTHING                _T("Ничего не указано")
#else
#define IDS_SELECT_NOTHING                _T("Nothing is specified")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_MISMATCH               _T("Указан объект несоответсвующего типа")
#else
#define IDS_SELECT_MISMATCH               _T("Specified object of inconsistent type")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_MISMATCH_PAR           _T("Указан объект с несоответствующими параметрами")
#else
#define IDS_SELECT_MISMATCH_PAR           _T("Specified object with inconsistent parameters")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_POCKET_TYPE                   _T("Введите тип объекта (1  бобышка, -1 карман )")
#else
#define IDS_POCKET_TYPE                   _T("Enter the object type (1  boss, -1 pocket )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_POCKET_WIDTH                  _T("Введите ширину")
#else
#define IDS_POCKET_WIDTH                  _T("Enter a width")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_POCKET_LENGTH                 _T("Введите длину")
#else
#define IDS_POCKET_LENGTH                 _T("Enter a length")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_POCKET_CURVATURE              _T("Введите кривизну максимальную")
#else
#define IDS_POCKET_CURVATURE              _T("Enter a curvature maximum")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_POCKET_FILLET                 _T("Введите радиус скругления боковых ребер ")
#else
#define IDS_POCKET_FILLET                 _T("Enter fillet radius for side edges ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_POCKET_ANGLE                  _T("Введите угол наклона")
#else
#define IDS_POCKET_ANGLE                  _T("Enter a slope angle")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_POCKET_FLOOR                  _T("Введите радиус скругления у основания")
#else
#define IDS_POCKET_FLOOR                  _T("Enter a radius of rounding at the base")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_POCKET_DEPTH                  _T("Введите глубину/высоту кармана/бобышки")
#else
#define IDS_POCKET_DEPTH                  _T("Enter depth/height of a pocket/a boss")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define ERR_NOINTERSECT                   _T("Нет пересечения")
#else
#define ERR_NOINTERSECT                   _T("No intersection")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define ERR_NOTCLOSED                     _T("Кривая незамкнута")
#else
#define ERR_NOTCLOSED                     _T("Curve is not closed")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define ERR_NOTBOUND                      _T("Кривая не может быть границей")
#else
#define ERR_NOTBOUND                      _T("The curve cannot be a boundary")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define ERR_NOTSURFCRV                    _T("Это не кривая на поверхности")
#else
#define ERR_NOTSURFCRV                    _T("It is not a curve on a surface")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_RIGASSAMBLY_TYPE        _T("Введите тип жесткой сборки 1 - линия, 2 - стенка, 3 - куб")
#else
#define IDS_ENTER_RIGASSAMBLY_TYPE        _T("Enter a rigid assembly type 1 - line, 2 - side, 3 - cube")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_BRICK_COUNT_SIDE        _T("Введите число кирпичей для каждой из сторон")
#else
#define IDS_ENTER_BRICK_COUNT_SIDE        _T("Enter a number of bricks for each side")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_BLOCK_COUNT             _T("Введите число элементарных блоков в сборке")
#else
#define IDS_ENTER_BLOCK_COUNT             _T("Enter a number of elementary blocks in the assembly")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_RINGS_COUNT             _T("Введите число колец")
#else
#define IDS_ENTER_RINGS_COUNT             _T("Enter a number of rings")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_SHOW_ASSEMBLY           _T("Показать сборку?")
#else
#define IDS_ENTER_SHOW_ASSEMBLY           _T("Show assembly?")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SPECIFY_RIGID_STATUS          _T("Жесткая связь?")
#else
#define IDS_SPECIFY_RIGID_STATUS          _T("Rigid link?")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_RIGID_TYPE              _T("1 - 1 кирпич, 2 - 3 кирпича, 3 - 2 оси и 1 кирпич")
#else
#define IDS_ENTER_RIGID_TYPE              _T("1 - 1 brick, 2 - 3 bricks 3 - 2 axes и 1 brick")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_NONRIGID_TYPE           _T("1 - 2 оси, 2 - ось и расстояние, 3 - 1 ось")
#else
#define IDS_ENTER_NONRIGID_TYPE           _T("1 - 2 axes, 2 - an axis and a distance , 3 - 1 axis")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_MATES_NUMBER                  _T("Число сопряжений в сборке")
#else
#define IDS_MATES_NUMBER                  _T("Number of matings in the assembly")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_CORNER_TYPE                   _T("Введите тип поверхности скругления вершины (0 - сфера, 1 - гладкая, 3 - треугольная, 4,5,6 - четырехугольная)")
#else
#define IDS_CORNER_TYPE                   _T("Enter a type of surface of vertex rounding (0 - sphere, 1 - smooth, 3 - triangular, 4,5,6 - quadrangular)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_UNIT_OF_MEASURE               _T("Единицы измерения длины в миллиметрах")
#else
#define IDS_UNIT_OF_MEASURE               _T("Specify of length units in millimeters")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_VISUAL_TOL                    _T("Задайте погрешность прогиба для расчёта триангуляции")
#else
#define IDS_VISUAL_TOL                    _T("Specify sag for mesh calculation")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_DEVIATE_TOL                   _T("Задайте угловую погрешность для расчёта триангуляции")
#else
#define IDS_DEVIATE_TOL                   _T("Specify angular tolerance for mesh calculation")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_METRIC_TOL                    _T("Задайте допустимую длину для расчёта триангуляции")
#else
#define IDS_METRIC_TOL                    _T("Specify max length for mesh calculation")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SAVE_TOPOLIGY                 _T("Сохранять топологию? ( 0 - нет, другое - да )")
#else
#define IDS_SAVE_TOPOLIGY                 _T("Save topology? ( 0 - no, other - yes )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SAVE_LENGTH                   _T("Сохранять длину? ( 0 - нет, другое - да )")
#else
#define IDS_SAVE_LENGTH                   _T("Save length? ( 0 - no, other - yes )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_LICENSE_KEY             _T("Введите лицензионный ключ ")
#else
#define IDS_ENTER_LICENSE_KEY             _T("Enter a license key ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_SIGNATURE               _T("Введите подпись ")
#else
#define IDS_ENTER_SIGNATURE               _T("Enter a signature ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_IS_POLAR                      _T("Сетка полярная?")
#else
#define IDS_IS_POLAR                      _T("Is it a polar grid?")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_IS_ALONG_AXIS                 _T("Тела расположены вдоль радиальной оси?")
#else
#define IDS_IS_ALONG_AXIS                 _T("Are bodies oriented along polar axis?")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_IS_CENTER_GIVEN               _T("Будет задан центр системы координат?")
#else
#define IDS_IS_CENTER_GIVEN               _T("Will the center of the coordinate system be given?")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SPECIFY_DIRECTION             _T("1 - доп. spine, 2 - вращ. масш., 3 - эквидистанта, 0 - ничего")
#else
#define IDS_SPECIFY_DIRECTION             _T("1 - add spine, 2 - rot. scale, 3 - offset,  0 - nothing")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SPECIFY_VS_PROTOTYPE          _T("0 - нет, 1 - sin масш.")
#else
#define IDS_SPECIFY_VS_PROTOTYPE          _T("0 - no, 1 - sin scale")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_METRIC_PRECISION        _T("Задайте метрическую точность.")
#else
#define IDS_ENTER_METRIC_PRECISION        _T("Specify metric precision.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SPECIFY_SURFACE_SECTION       _T("0 - дуга, 1 - линия, 2 - коник, 3 - кубик, 4 - сплайн, 5 - диск, 6 - шар")
#else
#define IDS_SPECIFY_SURFACE_SECTION       _T("0 - arc, 1 - line, 2 - conic, 3 - cubic, 4 - spline, 5 - disk, 6 - ball")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SPECIFY_SURFACE_SECTION_SHORT _T("0 - шар, 1 - диск")
#else
#define IDS_SPECIFY_SURFACE_SECTION_SHORT _T("0 - ball, 1 - disk")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SPECIFY_SURFACE_SECTION_SUBFORM _T("0 - отрезок, 1 - коническая кривая, 2 - кубический сплайн")
#else
#define IDS_SPECIFY_SURFACE_SECTION_SUBFORM _T("0 - line, 1 - conic, 2 - cubic")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SHELL_OR_FACE_SELECTION       _T("0 - вся оболочка, 1 - только выбранная грань")
#else
#define IDS_SHELL_OR_FACE_SELECTION       _T("0 - shell, 1 - selected face")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_SEGMENTATION_NUBER      _T("Введите количество отрезков сегментации ")
#else
#define IDS_ENTER_SEGMENTATION_NUBER      _T("Enter a number of the segmentation segments ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_CURVATURE_TYPE          _T("Кривизна (0 - средняя, 1 - гауссова, 2 - 1ая главная, 3 - 2ая главная)")
#else
#define IDS_ENTER_CURVATURE_TYPE          _T("Curvature (0 - mean, 1 - gausssian, 2 - 1st principal, 3 - 2nd principal)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_CURVATURE_DIR_TYPE      _T("Главное направление изменения кривизны (0 - 1ое, 1 - 2ое)")
#else
#define IDS_ENTER_CURVATURE_DIR_TYPE      _T("Principal curvature direction (0 - 1st, 1 - 2nd)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_MESH2BREP_RELTOLERANCE  _T("Использовать относительную точность при распознавании?")
#else
#define IDS_ENTER_MESH2BREP_RELTOLERANCE  _T("Use relative tolerance while reconstructing?")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_MESH2BREP_MODE          _T("Выберите тип генерируемых поверхностей: 0 - все, 1 - без MbGridSurface, 2 - только элементарные.")
#else
#define IDS_ENTER_MESH2BREP_MODE          _T("Choose types of surfaces to generate: 0 - all, 1 - without MbGridSurface, 2 - only elementary.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_OFFSET_TYPE             _T("Введите тип: 0-пустой, 1-констнантный, 2-линейный, 3-кубический")
#else
#define IDS_ENTER_OFFSET_TYPE             _T("Enter type: 0-none, 1-const, 2-line, 3-cubic")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_FUNCTION_TYPE                 _T("Тип функции: 0 - постоянный, 1-линейный, 2-кубический")
#else
#define IDS_FUNCTION_TYPE                 _T("Function type: 0 - const, 1-line, 2-cubic")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_MORE                    _T("Подробнее? 0 - нет, 1 - да ")
#else
#define IDS_ENTER_MORE                    _T("More? 0 - no, 1 - yes")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_LOWERLIMIT              _T("Введите минимальное допустимое расстояние ")
#else
#define IDS_ENTER_LOWERLIMIT              _T("Enter the minimum allowed distance")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_UPPERLIMIT              _T("Введите максимальное допустимое расстояние ")
#else
#define IDS_ENTER_UPPERLIMIT              _T("Enter the maximum allowed distance")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_TILL_FIRST                    _T("Искать до первого минимального расстояния удовлетворяющего допустимому? 0 - все расстояния, 1 - только первое")
#else
#define IDS_TILL_FIRST                    _T("Search for the first minimum distance satisfying acceptable? 0 - all distanse 1 - only first")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_TWO_CONTOURS_BEND             _T("Два контура? 0 - нет, 1 - да")
#else
#define IDS_TWO_CONTOURS_BEND             _T("Two contours? 0 - no 1 - yes")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_FIX_FIRST_PT                  _T("Зафиксировать первую точку? 0 - нет, 1 - да")
#else                                     
#define IDS_FIX_FIRST_PT                  _T("Fix first point? 0 - no 1 - yes")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_FIX_LAST_PT                  _T("Зафиксировать последнюю точку? 0 - нет, 1 - да")
#else                                     
#define IDS_FIX_LAST_PT                  _T("Fix last point? 0 - no 1 - yes")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SMOOTH_COEF                  _T("Коэффициент сглаживания (1e-8 - 1.)")
#else                                     
#define IDS_SMOOTH_COEF                  _T("Smoothing coefficient (1e-8 - 1.)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_EXTEND_CRV_BEG                  _T("Сдвиг начала кривой, в процентах от длины")
#else                                     
#define IDS_EXTEND_CRV_BEG                  _T("Extension for the curve beginning in percents w.r.t length")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_EXTEND_CRV_END                  _T("Сдвиг конца кривой, в процентах от длины")
#else                                     
#define IDS_EXTEND_CRV_END                  _T("Extension for the curve end in percents w.r.t length")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SELECT_POLYLINE                  _T("Укажите полилинию")
#else
#define IDS_SELECT_POLYLINE                  _T("Specify a polyline")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SPECIFY_FUNCTION_CASE            _T("Введите вариант используемой аналитической функции\n(0 - MdCharacterFunction(); 1 - MdAnalyticalFunction() )")
#else
#define IDS_SPECIFY_FUNCTION_CASE            _T("Enter the kind of the analytical function used\n(0 - MdCharacterFunction(); 1 - MdAnalyticalFunction() )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SPECIFY_REBUILD_FILLETS          _T("Перестраивать прилегающие скругления ? (0/1)")
#else
#define IDS_SPECIFY_REBUILD_FILLETS          _T("Whether to rebuild the ajacent fillets ? (0/1)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SPECIFY_CREATE_PHANTOM           _T("Создать фантом ? 0 - нет 1 - да")
#else
#define IDS_SPECIFY_CREATE_PHANTOM           _T("Create a phantom ? 0 - no 1 - yes")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_GENERAL_HANDLE_NONTANGENT_JOINT  _T("Обрабатывать некасательные стыки? 0 - нет 1 - да")
#else
#define IDS_GENERAL_HANDLE_NONTANGENT_JOINT  _T("Handle nontangent joint? 0 - no 1 - yes")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_POLYGONAL_ORIENT_NORMALS_OUTWARD  _T("Ориентировать нормали вовне? 0 - нет 1 - да")
#else
#define IDS_POLYGONAL_ORIENT_NORMALS_OUTWARD  _T("Set normals outwards? 0 - no 1 - yes")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_POLYGONAL_NORMAL_AVERAGING  _T("Усреднять ли нормали?")
#else
#define IDS_POLYGONAL_NORMAL_AVERAGING  _T("Set normal vectors averaging mode")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_EXTEND_SHELL  _T("Строить каркас на расширении граней?")
#else
#define IDS_EXTEND_SHELL  _T("Set the wireframe build mode to expand faces")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_EXTEND_WIRE_FRAME  _T("Продлить каркас до границ граней?")
#else
#define IDS_EXTEND_WIRE_FRAME  _T("Set extend wireframe to the edges of the faces mode")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_CHOOSE_LIMIT_DEPTH_MODE  _T("Выберите настройки проецирования объектов по глубине видимимости. ( 0 - off, 1 - on, 2 - on_exclude, 3 - cut )")
#else
#define IDS_CHOOSE_LIMIT_DEPTH_MODE  _T("Choose limit depth mode. ( 0 - off, 1 - on, 2 - on_exclude, 3 - cut )")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_VISIBILITY_DEPTH  _T("Введите глубину видимости")
#else
#define IDS_ENTER_VISIBILITY_DEPTH  _T("Enter the visibility depth")
#endif
////////////////////////////////////////////////////////////////////////////////
//
// \ru Заголовки окон, сообщения в окнах. \en Titles of windows, messages in windows
//
////////////////////////////////////////////////////////////////////////////////


#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_TITLE_ERR                     _T("Ошибка")
#else
#define IDS_TITLE_ERR                     _T("Error")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_TITLE_WRN                     _T("Предупреждение")
#else
#define IDS_TITLE_WRN                     _T("Warning")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_TITLE_MSG                     _T("Сообщение")
#else
#define IDS_TITLE_MSG                     _T("Message")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_TITLE_INFO                     _T("Информация")
#else
#define IDS_TITLE_INFO                     _T("Information")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_TITLE_ALL_RIGHT               _T("Все в порядке")
#else
#define IDS_TITLE_ALL_RIGHT               _T("All right")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_TITLE_DIAG_ERR                _T("Диагностика: Ошибка")
#else
#define IDS_TITLE_DIAG_ERR                _T("Diagnostics: Error")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_TITLE_DIAG_WRN                _T("Диагностика: Предупреждение")
#else
#define IDS_TITLE_DIAG_WRN                _T("Diagnostics: Warning")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_TITLE_DIAG_ALL_RIGHT          _T("Диагностика: Все в порядке")
#else
#define IDS_TITLE_DIAG_ALL_RIGHT          _T("Diagnostics: All right")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_TITLE_CALC_RES                _T("Результат вычисления")
#else
#define IDS_TITLE_CALC_RES                _T("Calculation result")
#endif

#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_TITLE_VIEWFRONT               _T("Вид спереди")
#else
#define IDS_TITLE_VIEWFRONT               _T("Front view")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_TITLE_VIEWREAR                _T("Вид сзади")
#else
#define IDS_TITLE_VIEWREAR                _T("Rear view")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_TITLE_VIEWUP                  _T("Вид сверху")
#else
#define IDS_TITLE_VIEWUP                  _T("Top view")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_TITLE_VIEWDOWN                _T("Вид снизу")
#else
#define IDS_TITLE_VIEWDOWN                _T("Bottom view")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_TITLE_VIEWLEFT                _T("Вид слева")
#else
#define IDS_TITLE_VIEWLEFT                _T("Left-side view")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_TITLE_VIEWRIGHT               _T("Вид справа")
#else
#define IDS_TITLE_VIEWRIGHT               _T("Right-side view")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_TITLE_VIEWISOMETRY            _T("Изометрия XYZ")
#else
#define IDS_TITLE_VIEWISOMETRY            _T("Isometry XYZ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_TITLE_VIEWARBITRARY           _T("Произвольный вид")
#else
#define IDS_TITLE_VIEWARBITRARY           _T("User view")
#endif

#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_TITLE_WRITE                   _T("Записать")
#else
#define IDS_TITLE_WRITE                   _T("Write")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_TITLE_APPLY                   _T("Применить")
#else
#define IDS_TITLE_APPLY                   _T("Apply")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_TITLE_CANCEL                  _T("Отменить")
#else
#define IDS_TITLE_CANCEL                  _T("Cancel")
#endif

#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_TITLE_SELECT_APPLY            _T("Прочитать")
#define IDS_TITLE_SELECT                  _T("Выбрать текущий объект")
#define IDS_TITLE_SELECT_CHILD            _T("Выбрать дочерний объект")
#define IDS_TITLE_SELECT_CHILD2           _T("Выбрать объект")
#define IDS_TITLE_SELECT_ALL              _T("Выбрать все")
#define IDS_TITLE_OBJECT                  _T("Объект")
#define IDS_TITLE_EMBODIMENT              _T("Исполнение")
#define IDS_TITLE_OBJECT_TYPE             _T("Тип:")
#define IDS_TITLE_OBJECT_NAME             _T("Имя:")
#define IDS_TITLE_OBJECT_ID               _T("Идентификатор:")
#define IDS_TITLE_OBJECT_GABARIT          _T("Габарит:")
#define IDS_TITLE_OBJECT_CHILDREN         _T("Потомки")
#define IDS_TITLE_OBJECT_SELECTION        _T("Выбран")
#define IDS_TITLE_MODEL_CATALOG           _T("Каталог модели")
#define IDS_TITLE_EMBODIMENT_CATALOG      _T("Каталог исполнений")
#define IDS_TITLE_SELECT_TYPE             _T("Выбрать тип")
#define IDS_TITLE_SELECT_BY_TYPE          _T("Выбрать объекты по типу")
#define IDS_TITLE_SELECT_BY_GABARIT       _T("Выбрать объекты по габариту")
#define IDS_TITLE_SELECT_BY_ATTRIBUTE     _T("Выбрать объекты по атрибуту")
#define IDS_TITLE_SELECT_MESSAGE_FORMAT   _T("Выбрано %d объектов. Читаю...")
#define IDS_TITLE_SELECT_MESSAGE_DRAW     _T("Рисую...")
#define IDS_TITLE_SELECT_MESSAGE_NO       _T("Объекты не найдены")
#define IDS_TITLE_SELECT_MESSAGE_READ     _T("Читаю...")
#define IDS_TITLE_SELECT_MESSAGE_EMBODI   _T("Операция не разрешена для Каталога модели типа Исполнение")
#define IDS_TITLE_CATALOG_MESSAGE_EMBODI  _T("Каталог модели типа Исполнение открыт только для просмотра")
#define IDS_CATALOG_MESSAGE_EMBODI_ERR    _T("В каталоге модели нет исполнений")
#else
#define IDS_TITLE_SELECT_APPLY            _T("Read")
#define IDS_TITLE_SELECT                  _T("Select current object")
#define IDS_TITLE_SELECT_CHILD            _T("Select child object")
#define IDS_TITLE_SELECT_CHILD2           _T("Select object")
#define IDS_TITLE_SELECT_ALL              _T("Select all")
#define IDS_TITLE_OBJECT                  _T("Object")
#define IDS_TITLE_EMBODIMENT              _T("Embodiment")
#define IDS_TITLE_OBJECT_TYPE             _T("Type:")
#define IDS_TITLE_OBJECT_NAME             _T("Name:")
#define IDS_TITLE_OBJECT_ID               _T("Identifier:")
#define IDS_TITLE_OBJECT_GABARIT          _T("Gabarit:")
#define IDS_TITLE_OBJECT_CHILDREN         _T("Children")
#define IDS_TITLE_OBJECT_SELECTION        _T("Selected")
#define IDS_TITLE_MODEL_CATALOG           _T("Model Catalog")
#define IDS_TITLE_EMBODIMENT_CATALOG      _T("Embodiment Catalog")
#define IDS_TITLE_SELECT_TYPE             _T("Select type")
#define IDS_TITLE_SELECT_BY_TYPE          _T("Select objects by type")
#define IDS_TITLE_SELECT_BY_GABARIT       _T("Select objects by gabarit")
#define IDS_TITLE_SELECT_BY_ATTRIBUTE     _T("Select objects by attribute")
#define IDS_TITLE_SELECT_MESSAGE_FORMAT   _T("Selected %d objects. Reading...")
#define IDS_TITLE_SELECT_MESSAGE_DRAW     _T("Drawing...")
#define IDS_TITLE_SELECT_MESSAGE_READ     _T("Reading...")
#define IDS_TITLE_SELECT_MESSAGE_NO       _T("No objects found")
#define IDS_TITLE_SELECT_MESSAGE_EMBODI   _T("Operation not applicable for Model Catalog\n of Embodiment type")
#define IDS_TITLE_CATALOG_MESSAGE_EMBODI  _T("Model catalog of Embodiment type opened for viewing only")
#define IDS_CATALOG_MESSAGE_EMBODI_ERR    _T("Model catalog contains no embodiments")
#endif

#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_INFO_FILENAME                 _T("Имя файла:")
#else
#define IDS_INFO_FILENAME                 _T("File name:")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_INFO_ITEMS_CREATED            _T("Объект(ы) создан(ы).")
#else
#define IDS_INFO_ITEMS_CREATED            _T("Object(s) created.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_INFO_ITEMS_NOT_CREATED        _T("Объект(ы) не создан(ы).")
#else
#define IDS_INFO_ITEMS_NOT_CREATED        _T("Object(s) not created.")
#endif

#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_LINE_RENDER_DONE              _T("Закраска модели. Выполнено - %.1f%%")
#else
#define IDS_LINE_RENDER_DONE              _T("Model rendering. Done - %.1f%%")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_PRINT_3D_MODEL                _T("Печать 3D модели")
#else
#define IDS_PRINT_3D_MODEL                _T("Print 3D model")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_MATES                         _T("Сопряжения")
#else
#define IDS_MATES                         _T("Matings")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ERROR                         _T("Ошибка!")
#else
#define IDS_ERROR                         _T("Error!")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_MESSAGE                       _T("Сообщение")
#else
#define IDS_MESSAGE                       _T("Message")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ASSEMBLY_TRIANGULATION        _T("Триангуляция сборки тел")
#else
#define IDS_ASSEMBLY_TRIANGULATION        _T("Triangulation of an assembly")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_BODY_TRIANGULATION            _T("Триангуляция тела")
#else
#define IDS_BODY_TRIANGULATION            _T("Solid triangulation")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_KEEP_EDGES                    _T("Сохранять ребра оболочки?")
#else
#define IDS_KEEP_EDGES                    _T("Keep shell edges?")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_NEED_REMESH                   _T("Ремешировать?")
#else
#define IDS_NEED_REMESH                   _T("Remesh?")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_REMESH_METHOD                 _T("Метод ремешинга: 1 - однородный, 2 - по кривизне")
#else
#define IDS_REMESH_METHOD                 _T("Remesh method: 1 - uniform, 2 - by curvature")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SMOOTH_METHOD                 _T("Метод сглаживания: 0 - Таубин, 1 - Лаплас")
#else
#define IDS_SMOOTH_METHOD                 _T("Smooth method: 0 - Taubin, 1 - Laplas")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SMOOTH_LAMBDA                 _T("Коэффициент сглаживания: ")
#else
#define IDS_SMOOTH_LAMBDA                 _T("Smoothing coefficient: ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SMOOTH_MU                 _T("Коэффициент сглаживания: ")
#else
#define IDS_SMOOTH_MU                 _T("Smoothing coefficient: ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SMOOTH_ITER                 _T("Количество итераций: ")
#else
#define IDS_SMOOTH_ITER                 _T("Number of iterations: ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SMOOTH_SAVE_BORDERS                 _T("Сохранять границы: ")
#else
#define IDS_SMOOTH_SAVE_BORDERS                 _T("Save borders: ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SEGMENTATION_ACCOUNTING             _T("Учет сегментации: ")
#else
#define IDS_SEGMENTATION_ACCOUNTING             _T("Segmentation accounting: ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_REMESH_TOLERANCE              _T("Относительная точность ремешинга (~ 0.01 - 0.001)")
#else
#define IDS_REMESH_TOLERANCE              _T("Relative remesh tolerance (~ 0.01 - 0.001)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_REMESH_UNIFORM_SIZE           _T("Целевой размер ребра треугольника")
#else
#define IDS_REMESH_UNIFORM_SIZE           _T("Target triangle edge length")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_GRIDS_TO_SHELL                _T("Преобразование триангуляции в оболочку")
#else
#define IDS_GRIDS_TO_SHELL                _T("Triangulation conversion into shell")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_RESULTS                       _T("Результаты расчета")
#else
#define IDS_RESULTS                       _T("Calculation results")
#endif

#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_COLLISION_DETECTION    _T("Определение коллизий тел")
#define IDS_COLLISION_PAIRS        _T("Найдено пар граней пересечения")
#define CDM_MESSAGE_NO_RESULT           _T("Детектор столкновений не дал резульататов")
#define CDM_MESSAGE_MEASURE_FULFILLED   _T("Запрос на измерения исполнен")
#define CDM_MESSAGE_NO_COLLISION        _T("Столкновений не обнаружено")
#define CDM_MESSAGE_COLLISION_DETECTED  _T("Есть столкновения")
#define CDM_MESSAGE_ERROR               _T("Неспецифицированная ошибка детектора")

#else
#define IDS_COLLISION_DETECTION         _T("Solids collision detection")
#define IDS_COLLISION_PAIRS             _T("Collision face pairs founded")
#define CDM_MESSAGE_NO_RESULT           _T("No results from collision detection module")
#define CDM_MESSAGE_MEASURE_FULFILLED   _T("Measuring query fullfiled successfully")
#define CDM_MESSAGE_NO_COLLISION        _T("No Collision")
#define CDM_MESSAGE_COLLISION_DETECTED  _T("Collision detected")
#define CDM_MESSAGE_ERROR               _T("Unknown error of collision detection")

#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_FILE_NAME_AFTER_CHANGING_EXT  _T("Имя файла после изменения расширения")
#else
#define IDS_FILE_NAME_AFTER_CHANGING_EXT  _T("File name after changing the extension")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_FILE_NAME_BEFORE_CHANGING_EXT _T("Имя файла до изменения расширения")
#else
#define IDS_FILE_NAME_BEFORE_CHANGING_EXT _T("File name before changing the extension")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_REBUILD_OBJECT                _T("Перестроение объекта")
#else
#define IDS_REBUILD_OBJECT                _T("Object rebuilding")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_OBJECTS_ADDED                 _T("Добавлены объекты")
#else
#define IDS_OBJECTS_ADDED                 _T("Objects added")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_WERE_ERRORS                   _T("Были ошибки")
#else
#define IDS_WERE_ERRORS                   _T("Errors occurred")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_EMPTY_VECTOR                  _T("Вектор пуст")
#else
#define IDS_EMPTY_VECTOR                  _T("Vector is empty")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_RESULT                        _T("Результат")
#else
#define IDS_RESULT                        _T("Result")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_INFORMATION                   _T("Информация")
#else
#define IDS_INFORMATION                   _T("Information")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_FAILED                        _T("Не выполнено")
#else
#define IDS_FAILED                        _T("Failed")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_DONE                          _T("Выполнено")
#else
#define IDS_DONE                          _T("Done")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_FACE_NUMBER_IN_SOLID          _T("Номер грани в оболочке тела ")
#else
#define IDS_FACE_NUMBER_IN_SOLID          _T("Number of a face in solid ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_FACE_FILLET_TYPE              _T("Тип скругления грани")
#else
#define IDS_FACE_FILLET_TYPE              _T("Type of fillet face")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_EDGE_NUMBER_IN_SOLID          _T("Номер ребра в оболочке тела ")
#else
#define IDS_EDGE_NUMBER_IN_SOLID          _T("Number of an edge in solid ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_OUR_CASE                      _T("Наш случай")
#else
#define IDS_OUR_CASE                      _T("Our case")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_DISGNOSTICS                   _T("Диагностика")
#else
#define IDS_DISGNOSTICS                   _T("Diagnostics")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_EDIT_RESULT                   _T("Результат редактирования")
#else
#define IDS_EDIT_RESULT                   _T("Result of editing")
#endif

#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SKIP_ASSERT_ON                _T("Ассерты перенаправлены в окно Output")
#else
#define IDS_SKIP_ASSERT_ON                _T("Asserts redirected to Output")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SKIP_ASSERT_OFF               _T("Ассерты перенаправлены во всплывающее окно")
#else
#define IDS_SKIP_ASSERT_OFF               _T("Asserts redirected to Dialog")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SKIP_DEBUG_DRAW_OFF           _T("Отладочная отрисовка отключена")
#else
#define IDS_SKIP_DEBUG_DRAW_OFF           _T("DEBUG drawing is switched OFF")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SKIP_DEBUG_DRAW_ON            _T("Отладочная отрисовка включена")
#else
#define IDS_SKIP_DEBUG_DRAW_ON             _T("DEBUG drawing is switched ON")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_MULTITHREADED_MODE            _T("Установить режим многопоточности")
#else
#define IDS_MULTITHREADED_MODE            _T("Set multithreaded mode")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_FACE_SUBDIVISION              _T("Подразбиение грани")
#else
#define IDS_FACE_SUBDIVISION              _T("Face subdivision")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_CREATING_ANGLE                _T("Создание уклона: ")
#else
#define IDS_CREATING_ANGLE                _T("Creation a slope: ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_RIB_ERROR                     _T("Ошибка создания ребра жёсткости")
#else
#define IDS_RIB_ERROR                     _T("Error of a rib construction")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_LOFT_ERROR                    _T("Ошибка создания тела по сечениям")
#else
#define IDS_LOFT_ERROR                    _T("Error of a lofted solid construction")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_LOCAL_SECTION_ERROR           _T("Местный разрез не получен")
#else
#define IDS_LOCAL_SECTION_ERROR           _T("Local section is not obtained")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_CURRENT_SECTION               _T("Текущее сечение: ")
#else
#define IDS_CURRENT_SECTION               _T("Current section: ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_PROJECTION_BODIES             _T("Проецирование тел")
#else
#define IDS_PROJECTION_BODIES             _T("Solids projection")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_FAILED_TO_APPROXIMATE         _T("Не удалось аппроксимировать")
#else
#define IDS_FAILED_TO_APPROXIMATE         _T("Failed to approximate")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_WRITING_MODEL_TREE_OFF        _T("Запись дерева модели выключена")
#define IDS_WRITING_MODEL_TREE_ON         _T("Запись дерева модели включена")
#else
#define IDS_WRITING_MODEL_TREE_OFF        _T("Writing model tree is off")
#define IDS_WRITING_MODEL_TREE_ON         _T("Writing model tree is on")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_WRITING_VIEW_DATA_OFF        _T("Запись атрибутов данных вида выключена")
#define IDS_WRITING_VIEW_DATA_ON         _T("Запись атрибутов данных вида включена")
#else
#define IDS_WRITING_VIEW_DATA_OFF        _T("Writing view data attributes is off")
#define IDS_WRITING_VIEW_DATA_ON         _T("Writing view data attributes is on")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_CONSTRUCTORS_ENABLED          _T("Работа построителей включена")
#define IDS_CONSTRUCTORS_DISABLED         _T("Работа построителей выключена")
#define IDS_BUILD_LOG_IS_INACTIVE         _T("История построения отключена") 
#else
#define IDS_CONSTRUCTORS_ENABLED          _T("Constructors enabled")
#define IDS_CONSTRUCTORS_DISABLED         _T("Constructors disabled")
#define IDS_BUILD_LOG_IS_INACTIVE         _T("Build log is inactive")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_HIGHLIGHTINHG_OBJ_OFF       _T("Подсветка подобъектов меню выключена")
#define IDS_HIGHLIGHTINHG_OBJ_ON        _T("Подсветка подобъектов меню включена")
#else
#define IDS_HIGHLIGHTINHG_OBJ_OFF        _T("Menu sub-object highlighting off")
#define IDS_HIGHLIGHTINHG_OBJ_ON         _T("Menu sub-object highlighting on")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_STAMP_TYPE_BOOL               _T("Штамповка пуансоном(Да) или матрицей(Нет) ?")
#else
#define IDS_STAMP_TYPE_BOOL               _T("Stamping with a punch(YES) or a die(NO) ?")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_BOOL_RESET_NAMES              _T("Очистить имена")
#else
#define IDS_BOOL_RESET_NAMES              _T("Reset names")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_BOOL_USE_SURFACE_ONE          _T("Использовать 1-ю пересекающуюся поверхность?")
#else
#define IDS_BOOL_USE_SURFACE_ONE          _T("Use 1st intersected surface?")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SHELL_BOOL_USE_MESH           _T("Использовать анализ сеток?")
#else
#define IDS_SHELL_BOOL_USE_MESH           _T("Use meshes analysis?")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_BOOL_LOCK_ASPECT_RATIO        _T("Разрешить изменение пропорций по осям?")
#else
#define IDS_BOOL_LOCK_ASPECT_RATIO        _T("Allow change an aspect ratio?")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_BOOL_USE_CNTR_ON_SURFACE      _T("Использовать контур на поверхности?")
#else
#define IDS_BOOL_USE_CNTR_ON_SURFACE      _T("Using contour on surface?")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_SPLINE_TYPE             _T("Сплайн (1-ломаная, 2-кубический, 3-Эрмита, 4- Nurbs)")
#else
#define IDS_ENTER_SPLINE_TYPE             _T("Spline (1-polyline, 2-cubic, 3-Hermite, 4- Nurbs)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_KNOTS_MODE              _T("Введите тип узлового вектора (0, 1, 2)")
#else
#define IDS_ENTER_KNOTS_MODE              _T("Enter a knots vector tmode (0, 1, 2)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_TRANSLATION_MODE              _T("Адаптировать ли скругления?")
#else
#define IDS_TRANSLATION_MODE              _T("Adapt fillets?")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_MODIFY_BASE_OBJECTS           _T("Модифицировать базовые объекты скруглением?")
#else
#define IDS_MODIFY_BASE_OBJECTS           _T("Modify base objects by the fillet?")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_PART_INDEX              _T("Введите индекс части оболочки результата")
#else
#define IDS_ENTER_PART_INDEX              _T("Enter index of the result shell part")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_CUT_FACES                     _T("Подрезать грани: 0 - первый набор, 1 - второй, 2 - оба и сшить")
#else
#define IDS_CUT_FACES                     _T("Cut: 0 - first faces, 1 - second faces, 2 - sew both")
#endif


////////////////////////////////////////////////////////////////////////////////
//
// \ru Сообщения о результатах операции \en Messages about operation results
//
////////////////////////////////////////////////////////////////////////////////


// \ru сообщения эскиза о причинах провала построения контуров \en messages of a sketch about reasons of contours creation failure
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_OK                                   _T("") // sf_Ok,
#else
#define  RT_OK                                   _T("") // sf_Ok,
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_EMPTY                                _T("Пустой эскиз. ")
#else
#define  RT_EMPTY                                _T("Empty sketch. ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_TOMANYAXIS                           _T("Слишком много осей. ")
#else
#define  RT_TOMANYAXIS                           _T("Too many axes. ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_TOFEWAXIS                            _T("В эскизе должна присутствовать строго одна ось. ")
#else
#define  RT_TOFEWAXIS                            _T("There should be exactly one axis. ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_TOMANYCONTOURS                       _T("В эскизе должен быть строго один контур. ")
#else
#define  RT_TOMANYCONTOURS                       _T("There should be exactly one contour. ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_STARS                                _T("Контуры не должны пересекаться и/или иметь общие точки. ")//"Есть 'звезда'. "
#else
#define  RT_STARS                                _T("Contours should not intersect each other and/or have common points. ")//"There is a 'star'. "
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_SELFINTERSECTION                     _T("Самопересечение контура. ")
#else
#define  RT_SELFINTERSECTION                     _T("Contour self-intersection. ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_NOTCLOSEDSCETCH                      _T("Эскиз замкнут. ")
#else
#define  RT_NOTCLOSEDSCETCH                      _T("The sketch is closed. ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_NOTSECTIONCUBE                       _T("Эскиз не пересекает деталь. ")
#else
#define  RT_NOTSECTIONCUBE                       _T("The sketch does not intersect the detail. ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_INTERSECTION                         _T("Пересечение контуров. ")
#else
#define  RT_INTERSECTION                         _T("Contours intersection. ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_CONTOUR_GAB_ERROR                    _T("В контуре есть разрывы между сегментами. ")
#else
#define  RT_CONTOUR_GAB_ERROR                    _T("There are gaps between the segments in the contour. ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_CONTOUR_SEGMENTS_OVERLAP_ERROR       _T("В контуре есть наложения сегментов. ")
#else
#define  RT_CONTOUR_SEGMENTS_OVERLAP_ERROR       _T("There are segment overlays in the contour. ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_SKETCH_CHECK                         _T("Проверка эскиза ")
#else
#define  RT_SKETCH_CHECK                         _T("Sketch check ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_NO_INTERSECTION_SOLID                _T("Нет пересечения образующего контура с телом")
#else
#define  RT_NO_INTERSECTION_SOLID                _T("There is no intersection of generating contour with the solid")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_LIBNOTFOUND                          _T("Фрагмент не найден в библиотеке. ")
#else
#define  RT_LIBNOTFOUND                          _T("The fragment is not found in the library. ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_MUST_BE_CLOSED                       _T("Все контура должны быть замкнуты.")
#else
#define  RT_MUST_BE_CLOSED                       _T("All the contours should be closed.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_MUST_BE_OPEN                         _T("Все контура должны быть разомкнуты. ")
#else
#define  RT_MUST_BE_OPEN                         _T("All the contours should be open. ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_AXIS_INTERSECTION                    _T("Ось пересекает контур. ")
#else
#define  RT_AXIS_INTERSECTION                    _T("The axis intersects the contour. ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_ERROR_                               _T("Невозможно выполнить операцию. ") //"Неизвестная ошибка. "
#else
#define  RT_ERROR_                               _T("The operation cannot be done. ") //"Unknown error. "
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_NO_INTERSECT_SECTION                 _T("Осевая линия не пересекает сечения")
#else
#define  RT_NO_INTERSECT_SECTION                 _T("Axial line does not intersect sections")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_INVALID_TYPE                         _T("Тип кривой не подходит для данной операции")
#else
#define  RT_INVALID_TYPE                         _T("The curve type does not suit for the this operation")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_ACCURACY                             _T("Достигнутая точность аппроксимации")
#else
#define  RT_ACCURACY                             _T("Attained approximation accuracy")
#endif

// \ru Сообщения о причинах отказа построения тела \en Messages about the reasons of solid construction failure
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_MULTISOLID                           _T("Тело состоит из отдельных частей. ")
#else
#define  RT_MULTISOLID                           _T("The solid consists of separate parts. ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_MULTISOLID_CHECK                     _T("Проверка разделения тела на части")
#else
#define  RT_MULTISOLID_CHECK                     _T("Parts splitting check")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_MULTISOLID_SPLIT                     _T("Выполнение разделения тела на части")
#else
#define  RT_MULTISOLID_SPLIT                     _T("Parts splitting process")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_CURVEERROR                           _T("Ошибочная кривая. ")
#else
#define  RT_CURVEERROR                           _T("Invalid curve. ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_CONTOURERROR                         _T("Ошибочный контур. ")
#else
#define  RT_CONTOURERROR                         _T("Invalid contour. ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_SURFACEERROR                         _T("Ошибочная поверхность. ")
#else
#define  RT_SURFACEERROR                         _T("Invalid surface. ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_SOLIDERROR                           _T("Ошибочное тело. ")
#else
#define  RT_SOLIDERROR                           _T("Invalid solid. ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_PARAMETERERROR                       _T("Ошибочный параметр. ")
#else
#define  RT_PARAMETERERROR                       _T("Invalid parameter. ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_THICKNESSERROR                       _T("Неправильно задана толщина.")
#else
#define  RT_THICKNESSERROR                       _T("Incorrectly specified thickness.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_SELFINTERSECT                        _T("Объект самопересекается. ")
#else
#define  RT_SELFINTERSECT                        _T("The object has self-intersections. ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_SELFINTWHENEXTENDED                  _T("Объект самопересекается на продолжении. ") 
#else
#define  RT_SELFINTWHENEXTENDED                  _T("The object has self-intersections on the extension.") // for a rib
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_INTERSECT                            _T("Объекты пересекаются.")
#else
#define  RT_INTERSECT                            _T("The objects have intersections.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_NOINTERSECT                          _T("Объекты не пересекаются.")
#else
#define  RT_NOINTERSECT                          _T("The objects have no intersections.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_NO_SEQUENCE_CURVE_AND_SECTIONS       _T("Не послед.расположение сечений вдоль кривой.")
#else
#define  RT_NO_SEQUENCE_CURVE_AND_SECTIONS       _T("Nonsequential arrangement of sections along the curve.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_ERRORINTERSECT                       _T("Объекты пересекается с ошибкой.")
#else
#define  RT_ERRORINTERSECT                       _T("Objects intersection error.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_BOOLEAN_OPERATION                    _T("Булева операция.")
#else
#define  RT_BOOLEAN_OPERATION                    _T("Boolean operation.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_BOOLEANERROR                         _T("Ошибка в булевой операции.")
#else
#define  RT_BOOLEANERROR                         _T("Error in the Boolean operation.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_NOEDGES                              _T("Ребра не найдены. ")
#else
#define  RT_NOEDGES                              _T("Edges not found. ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_PREPAREERROR                         _T("Ошибка при подготовке операции. ")
#else
#define  RT_PREPAREERROR                         _T("Error while preparing the operation. ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_CHAMFERERROR                         _T("Ошибка при создании фаски ребра. ")
#else
#define  RT_CHAMFERERROR                         _T("Error while creation an edge chamfer. ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_FILLETERROR                          _T("Ошибка при скруглении ребра. ")
#else
#define  RT_FILLETERROR                          _T("Error while filleting an edge. ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_CHAMFER_SURFACE_ERROR                _T("Ошибка при создании поверхности фаски ребра. ")
#else
#define  RT_CHAMFER_SURFACE_ERROR                _T("Error while creation an edge chamfer surface. ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_FILLET_SURFACE_ERROR                 _T("Ошибка при создании поверхности скругления ребра. ")
#else
#define  RT_FILLET_SURFACE_ERROR                 _T("Error while creating an edge fillet surface. ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_TOO_LARGE_CHAMFER                    _T("Слишком большые катеты фаски. ")
#else
#define  RT_TOO_LARGE_CHAMFER                    _T("Too large cathetus of a chamfer. ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_TOO_LARGE_FILLET                     _T("Слишком большой радиус скругления. ")
#else
#define  RT_TOO_LARGE_FILLET                     _T("Too large fillet radius. ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_SEMI_CHAMFER                         _T("Фаски построены на для всех ребер. ")
#else
#define  RT_SEMI_CHAMFER                         _T("Chamfers created not for all edges. ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_SEMI_FILLET                          _T("Скруглены не все ребра. ")
#else
#define  RT_SEMI_FILLET                          _T("Not all the edges are filleted. ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_CUTTINGERROR                         _T("Ошибка резки поверхностью. ")
#else
#define  RT_CUTTINGERROR                         _T("Error of cutting by surface. ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_THINERROR                            _T("Ошибка при создании тонкостенного тела. ")
#else
#define  RT_THINERROR                            _T("Error of a thin-walled solid creation. ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_RIBERROR                             _T("Ошибка постановки ребра жесткости. ")
#else
#define  RT_RIBERROR                             _T("Error of a rib construction. ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_DRAFTERROR                           _T("Ошибка уклона граней тела")
#else
#define  RT_DRAFTERROR                           _T("Error of solid's faces drafting ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_CUTBYSILHOUETTE                       _T("Ошибка разреза силуэтной линией. ")
#else
#define  RT_CUTBYSILHOUETTE                       _T("Error of cutting by silhouette curve. ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_OPENFILEERROR                        _T(": файл не найден. ")
#else
#define  RT_OPENFILEERROR                        _T(": file not found. ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_EMPTYDETAIL                          _T("Пустая деталь. ")
#else
#define  RT_EMPTYDETAIL                          _T("Empty detail. ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_ERRORDETAIL                          _T(": деталь содержит ошибку. ")
#else
#define  RT_ERRORDETAIL                          _T(": the detail contains an error. ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_SPLIT_WIRE_NOT_SPLIT_FACE            _T("Линия разъема не пересекает грани")
#else
#define  RT_SPLIT_WIRE_NOT_SPLIT_FACE            _T("Parting line does not intersect faces")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_SPLIT_WIRE_NOT_INTERSECT_FACE        _T("Линия разъема не разделяет грани")
#else
#define  RT_SPLIT_WIRE_NOT_INTERSECT_FACE        _T("Parting line does not split faces")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_COORIENTFACEERROR                    _T("Невозможно выставить согласованную ориентацию граней.")
#else
#define  RT_COORIENTFACEERROR                    _T("A consistent orientation of faces cannot be specified.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_SOMEEDGESUNSTITCHED                  _T("Некоторые рёбра остались несшитыми.")
#else
#define  RT_SOMEEDGESUNSTITCHED                  _T("Some edges remained unstitched.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_OUTWARDORIENTERROR                   _T("Не удалось установить нормали граней наружу тела.")
#else
#define  RT_OUTWARDORIENTERROR                   _T("Failed to set the outer normals of faces.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_NOEDGEWASSTITCHED                    _T("Не было сшито ни одного ребра.")
#else
#define  RT_NOEDGEWASSTITCHED                    _T("No edges were stitched.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_SEPARATEPARTSRESULT                  _T("После сшивки остались несвязанные между собой куски.")
#else
#define  RT_SEPARATEPARTSRESULT                  _T("Disconnected parts remained after stitching.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_CURVE_IS_NOT_ADDED                   _T("Кривая не добавлена.")
#else
#define  RT_CURVE_IS_NOT_ADDED                   _T("The curve is not added.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_NOT_ALL_ITEMS_WERE_USED              _T("Не все объекты были использованы.")
#else
#define  RT_NOT_ALL_ITEMS_WERE_USED              _T("Not all objects were used.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_PROCESS_INTERRUPTED                  _T("Процесс остановлен.")
#else
#define  RT_PROCESS_INTERRUPTED                  _T("Process is stopped.")
#endif

// \ru Общие сообщения об ошибках конвертеров \en Common messages about converter errors
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_TITLE                             _T("\tЖурнал регистрации ошибок, импортирование файла ")
#else
#define RT_EMD_TITLE                             _T("\tError log, file importing ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_HEAD_ERROR                        _T("Ошибка: ")
#else
#define RT_EMD_HEAD_ERROR                        _T("Error: ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_HEAD_WARNING                      _T("Предупреждение: ")
#else
#define RT_EMD_HEAD_WARNING                      _T("Warning: ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_HEAD_INFO                         _T("Информация: ")
#else
#define RT_EMD_HEAD_INFO                         _T("Information: ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_HEAD_DEFAULT_MSG                  _T("Сообщение: ")
#else
#define RT_EMD_HEAD_DEFAULT_MSG                  _T("Message: ")
#endif

// \ru Прекращение импорта \en Termination of import
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_STOP_FILE_OPEN_ERROR              _T("ошибка открытия файла")
#else
#define RT_EMD_STOP_FILE_OPEN_ERROR              _T("file opening error")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_STOP_FILE_OPEN_ERROR_OR_EMPTY     _T("ошибка открытия файла или файл пуст")
#else
#define RT_EMD_STOP_FILE_OPEN_ERROR_OR_EMPTY     _T("file opening error or the file is empty")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_STOP_HEADER_READ_ERROR            _T("не удалось прочитать заголовок файла либо анализ заголовка показал невозможность импорта")
#else
#define RT_EMD_STOP_HEADER_READ_ERROR            _T("failed to read the file title or analysis of the title revealed impossibility of import")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_STOP_NO_OR_BAD_DATA               _T("файл не содержит данных или их не удалось распознать")
#else
#define RT_EMD_STOP_NO_OR_BAD_DATA               _T("the file does not contain any data or they were not recognized")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_STOP_INCORRECT_STRUCTURE          _T("неверная структура файла")
#else
#define RT_EMD_STOP_INCORRECT_STRUCTURE          _T("incorrect file structure")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_STOP_ADDRESS_CONFLICT             _T("данный адрес имеют два различных объекта")
#else
#define RT_EMD_STOP_ADDRESS_CONFLICT             _T("the given address corresponds to two different objects")
#endif

// \ru Общеконвертерные ошибки \en Converters common errors
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_NO_ROOT_OBJECT                      _T("не найден корневой объект")
#else
#define RT_EMD_ERROR_NO_ROOT_OBJECT                      _T("the root object not found")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_SYNTAX_INCORRECT_FORM_FLOAT         _T("невозможно прочитать действительную константу")
#else
#define RT_EMD_ERROR_SYNTAX_INCORRECT_FORM_FLOAT         _T("a float constant cannot be read")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_EMPTY_LOOP                          _T("цикл грани пуст")
#else
#define RT_EMD_ERROR_EMPTY_LOOP                          _T("a face loop is empty")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_EMPTY_QUERIES_LIST                  _T("список запросов пуст")
#else
#define RT_EMD_ERROR_EMPTY_QUERIES_LIST                  _T("the query list is empty")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_EMPTY_OBJECTS_LIST                  _T("список объектов пуст")
#else
#define RT_EMD_ERROR_EMPTY_OBJECTS_LIST                  _T("the object list is empty")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_EMPTY_GEOM_OBJECTS_LIST             _T("список геометрических объектов пуст")
#else
#define RT_EMD_ERROR_EMPTY_GEOM_OBJECTS_LIST             _T("the geometric object list is empty")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_EMPTY_SHELLS_LIST                   _T("список оболочек пуст")
#else
#define RT_EMD_ERROR_EMPTY_SHELLS_LIST                   _T("the shell list is empty")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_EMPTY_CURVE_COMPOSITIES_LIST        _T("список компонент составной кривой пуст")
#else
#define RT_EMD_ERROR_EMPTY_CURVE_COMPOSITIES_LIST        _T("the list of a compound curve components is empty")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_EMPTY_BOUND_CURVES_LIST             _T("список граничных кривых пуст")
#else
#define RT_EMD_ERROR_EMPTY_BOUND_CURVES_LIST             _T("the list of boundary curves is empty")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_EMPTY_EDGE_LIST                     _T("список рёбер пуст")
#else
#define RT_EMD_ERROR_EMPTY_EDGE_LIST                     _T("edge list is empty")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_EMPTY_FACES_LIST                    _T("список граней пуст")
#else
#define RT_EMD_ERROR_EMPTY_FACES_LIST                    _T("face list is empty")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_EMPTY_REFERENCES_LIST               _T("список ссылок пуст")
#else
#define RT_EMD_ERROR_EMPTY_REFERENCES_LIST               _T("list of references is empty")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_EMPTY_LIST_OF_WIREFRAMES            _T("список каркасов пуст")
#else
#define RT_EMD_ERROR_EMPTY_LIST_OF_WIREFRAMES            _T("list of wireframes is empty")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_EMPTY_OR_MORE_2_REFERENCES_LIST     _T("список ссылок пуст или содрежит более 2 элементов")
#else
#define RT_EMD_ERROR_EMPTY_OR_MORE_2_REFERENCES_LIST     _T("list of references is empty or contains more than 2 elements")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_UNDIFINED_REF                       _T("ссылка не определена")
#else
#define RT_EMD_ERROR_UNDIFINED_REF                       _T("the reference is undefined")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_UNDIFINED_FACE_SURFACE_REF          _T("ссылка на базовую поверхность грани не определена")
#else
#define RT_EMD_ERROR_UNDIFINED_FACE_SURFACE_REF          _T("reference to face's base surface is undefined")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_UNDIFINED_BASE_CURVE_REF            _T("ссылка на базовую кривую не определена")
#else
#define RT_EMD_ERROR_UNDIFINED_BASE_CURVE_REF            _T("reference to the base curve is undefined")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_RADIUS_TOO_CLOSE_TO_ZERO            _T("радиус слишком мал")
#else
#define RT_EMD_ERROR_RADIUS_TOO_CLOSE_TO_ZERO            _T("the radius is too small")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_RADIUS_VALUE_NEGATIVE               _T("отрицательное значение радиуса")
#else
#define RT_EMD_ERROR_RADIUS_VALUE_NEGATIVE               _T("negative value of radius")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_ELLIPSE_AXIS_TOO_CLOSE_TO_ZERO      _T("длина полуоси эллипса слишком мала")
#else
#define RT_EMD_ERROR_ELLIPSE_AXIS_TOO_CLOSE_TO_ZERO      _T("ellipse semiaxis is too small")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_ELLIPSE_AXIS_NEGATIVE               _T("отрицательная длина полуоси эллипса")
#else
#define RT_EMD_ERROR_ELLIPSE_AXIS_NEGATIVE               _T("negative length of ellipse semiaxis")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_NEGATIVE_DEGREE                     _T("отрицательный порядок сплайна")
#else
#define RT_EMD_ERROR_NEGATIVE_DEGREE                     _T("negative degree of the spline")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_NEGATIVE_U_DEGREE                   _T("отрицательный порядок сплайновой поверхности по U")
#else
#define RT_EMD_ERROR_NEGATIVE_U_DEGREE                   _T("negative degree of spline surface by U")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_NEGATIVE_V_DEGREE                   _T("отрицательный порядок сплайновой поверхности по V")
#else
#define RT_EMD_ERROR_NEGATIVE_V_DEGREE                   _T("negative degreee of spline surface by V")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_DEGREE_FIX_IMPOSIBLE                _T("невозможно исправить порядок сплайна")
#else
#define RT_EMD_ERROR_DEGREE_FIX_IMPOSIBLE                _T("the spline degree cannot be fixed")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_POLYLINE_POINT_LIST_LESS_2          _T("список точек ломаной содержит менее 2 элементов")
#else
#define RT_EMD_ERROR_POLYLINE_POINT_LIST_LESS_2          _T("list of polyline points contains less than 2 elements")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_POINT_LIST_LESS_2                   _T("список точек содержит менее 2 элементов")
#else
#define RT_EMD_ERROR_POINT_LIST_LESS_2                   _T("list of points contains less than 2 elements")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_KNOTS_LIST_LESS_2                   _T("список узлов содержит менее 2 элементов")
#else
#define RT_EMD_ERROR_KNOTS_LIST_LESS_2                   _T("list of knots contains less than 2 elements")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_WEIGHTS_LIST_LESS2                  _T("список весов содержит менее 2 элементов")
#else
#define RT_EMD_ERROR_WEIGHTS_LIST_LESS2                  _T("list of weights contains less than 2 elements")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_U_POINTS_LIST_LESS_2                _T("список точек по U содержит менее 2 элементов")
#else
#define RT_EMD_ERROR_U_POINTS_LIST_LESS_2                _T("list of points by U contains less than 2 elements")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_U_KNOTS_LEST_LESS_2                 _T("список узлов по U содержит менее 2 элементов")
#else
#define RT_EMD_ERROR_U_KNOTS_LEST_LESS_2                 _T("list of knots by U contains less than 2 elements")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_U_WEIGTHS_LIST_LESS_2               _T("список весов по U содержит менее 2 элементов")
#else
#define RT_EMD_ERROR_U_WEIGTHS_LIST_LESS_2               _T("list of weights by U contains less than 2 elements")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_V_POINTS_LIST_LESS_2                _T("список точек по V содержит менее 2 элементов")
#else
#define RT_EMD_ERROR_V_POINTS_LIST_LESS_2                _T("list of points by V contains less than 2 elements")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_V_KNOTS_LEST_LESS_2                 _T("список узлов по V содержит менее 2 элементов")
#else
#define RT_EMD_ERROR_V_KNOTS_LEST_LESS_2                 _T("list of knots by V contains less than 2 elements")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_V_WEIGTHS_LIST_LESS_2               _T("список весов по V содержит менее 2 элементов")
#else
#define RT_EMD_ERROR_V_WEIGTHS_LIST_LESS_2               _T("list of weights by V contains less than 2 elements")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_LISTS_SIZE_MISMATCH                 _T("размеры списков не согласуются")
#else
#define RT_EMD_ERROR_LISTS_SIZE_MISMATCH                 _T("lists sizes mismatch")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_KNOTS_WEIGHTS_LISTS_ORDER_MISMATCH                  _T("размеры списков узлов и весов не согласуются с порядком сплайна")
#else
#define RT_EMD_ERROR_KNOTS_WEIGHTS_LISTS_ORDER_MISMATCH                  _T("sizes of knot list and weight list do not match the spline order")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_KNOTS_WEIGHTS_LISTS_SIZE_MISMATCH                   _T("размеры списков узлов и весов не согласуются")
#else
#define RT_EMD_ERROR_KNOTS_WEIGHTS_LISTS_SIZE_MISMATCH                   _T("sizes of lists of knots and weights do not match")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_U_KNOTS_WEIGHTS_LISTS_SIZE_MISMATCH                 _T("размеры списков узлов и весов по U не согласуются")
#else
#define RT_EMD_ERROR_U_KNOTS_WEIGHTS_LISTS_SIZE_MISMATCH                 _T("sizes of lists of knots and weights by U do not match")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_V_KNOTS_WEIGHTS_LISTS_SIZE_MISMATCH                 _T("размеры списков узлов и весов по V не согласуются")
#else
#define RT_EMD_ERROR_V_KNOTS_WEIGHTS_LISTS_SIZE_MISMATCH                 _T("sizes of lists of knots and weights by V do not match")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_SPLINE_CURVE_NOTCREATED_UNDIFINED_KNOTS_VECTOR      _T("сплайновая кривая не создана - не определёны узлы")
#else
#define RT_EMD_ERROR_SPLINE_CURVE_NOTCREATED_UNDIFINED_KNOTS_VECTOR      _T("spline curve is not created - knots are not defined")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_SPLINE_SURFACE_NOT_CREATED_UNDIFINED_KNOTSVECTORS   _T("сплайновая поверхность не создана - не определёны узлы")
#else
#define RT_EMD_ERROR_SPLINE_SURFACE_NOT_CREATED_UNDIFINED_KNOTSVECTORS   _T("spline surface is not created - knots are not defined")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_INCORRECT_SPLINE_SURFACE_DATA                       _T("неверно заданы параметры NURBS поверхности")
#else
#define RT_EMD_ERROR_INCORRECT_SPLINE_SURFACE_DATA                       _T("parameters of NURBS surface are incorrectly specified")
#endif

// \ru Общеконвертерные предупреждения \en Common warnings of converters
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_WARNING_NO_SECTION_TERMINATOR             _T("Маркер завершения раздела не обнаружен. Возможно наличие дефектов в импортированной модели из-за отсутствующих данных.")
#else
#define RT_EMD_WARNING_NO_SECTION_TERMINATOR             _T("Section terminator is not found. There are possible defects in the imported model because of missing data.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_WARNING_SYNTAX_MULTIPLE_DOT_IN_FLOAT      _T("повторяющаяся точка в действительном числе")
#else
#define RT_EMD_WARNING_SYNTAX_MULTIPLE_DOT_IN_FLOAT      _T("multiple dot in a float number")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_WARNING_SYNTAX_MULTIPLE_E_IN_FLOAT        _T("повторяющаяся E в действительном числе")
#else
#define RT_EMD_WARNING_SYNTAX_MULTIPLE_E_IN_FLOAT        _T("muliple E in a float number")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_WARNING_LOOP_NOT_CLOSED                   _T("цикл не замкнут")
#else
#define RT_EMD_WARNING_LOOP_NOT_CLOSED                   _T("loop is not closed")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_WARNING_CONTOUR_NOT_CLOSED                _T("контур не замкнут")
#else
#define RT_EMD_WARNING_CONTOUR_NOT_CLOSED                _T("contour is not closed")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_WARNING_UNDIFINED_REF                     _T("ссылка не определена")
#else
#define RT_EMD_WARNING_UNDIFINED_REF                     _T("reference is undefined")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_WARNING_TOROIDAL_SURFACE_DEGENERATED      _T("тороидальная поверхность вырождена")
#else
#define RT_EMD_WARNING_TOROIDAL_SURFACE_DEGENERATED      _T("toroidal surface is degenerated")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_WARNING_UNDIFINED_BASIS_CURVE             _T("не определена базовая кривая")
#else
#define RT_EMD_WARNING_UNDIFINED_BASIS_CURVE             _T("the base curve is undefined")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_WARNING_UNDIFINED_SWEPT_CURVE             _T("не определена образующая кривая")
#else
#define RT_EMD_WARNING_UNDIFINED_SWEPT_CURVE             _T("generating curve is undefined")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_WARNING_UNDIFINED_EXTURSION_DIRECTION     _T("не определено направление выдавливания")
#else
#define RT_EMD_WARNING_UNDIFINED_EXTURSION_DIRECTION     _T("extrusion direction is undefined")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_WARNING_UNDIFINED_AXIS                    _T("не определана ось")
#else
#define RT_EMD_WARNING_UNDIFINED_AXIS                    _T("axis is undefined")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_WARNING_UNDIFINED_AXIS_OF_REVOLUTION      _T("не определана ось вращения")
#else
#define RT_EMD_WARNING_UNDIFINED_AXIS_OF_REVOLUTION      _T("rotation axis is undefined")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_WARNING_UNDIFINED_BASIS_SURFACE           _T("не определена базовая поверхность")
#else
#define RT_EMD_WARNING_UNDIFINED_BASIS_SURFACE           _T("the base surface is undefined")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_WARNING_UNDIFINED_REPRESENTATIOV          _T("не определено представление")
#else
#define RT_EMD_WARNING_UNDIFINED_REPRESENTATIOV          _T("representation is undefined")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_WARNING_UNDIFINED_TRANSFORMATION_OPERATOR _T("не определён оператор преобразования")
#else
#define RT_EMD_WARNING_UNDIFINED_TRANSFORMATION_OPERATOR _T("transformation operator is undefined")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_WARNING_UNDIFINED_OBJECT_TRANSFORM_BY     _T("не определён объект, по которому ведётся преобразование")
#else
#define RT_EMD_WARNING_UNDIFINED_OBJECT_TRANSFORM_BY     _T("undefined object to transform by")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_WARNING_UNDIFINED_OBJECT_TO_TRANSFORM     _T("не определён преобразуемый объект")
#else
#define RT_EMD_WARNING_UNDIFINED_OBJECT_TO_TRANSFORM     _T("undefined object to transform")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_WARNING_UNDIFINED_CURVE                   _T("не определена кривая")
#else
#define RT_EMD_WARNING_UNDIFINED_CURVE                   _T("curve is undefined")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_WARNING_UNDIFINED_COMPOSITE_SEGMENT       _T("не определён сегмент составной кривой")
#else
#define RT_EMD_WARNING_UNDIFINED_COMPOSITE_SEGMENT       _T("a segment of a composite curve is undefined")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_WARNING_UNDIFINED_DIRECTION               _T("не определено направление")
#else
#define RT_EMD_WARNING_UNDIFINED_DIRECTION               _T("direction is undefined")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_WARNING_UNDIFINED_AXIS_DIRECTION          _T("не определено направление оси")
#else
#define RT_EMD_WARNING_UNDIFINED_AXIS_DIRECTION          _T("axis direction is undefined")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_MESSAGE_WEIGHTS_FILLED                    _T("веса заданы")
#else
#define RT_EMD_MESSAGE_WEIGHTS_FILLED                    _T("weights are specified")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_MESSAGE_FLOAT_PARSE_FAILURE_REPLACE_DEFAULT _T("ошибка разпознавания числа с плавающей точкой, подставлено значение по умолчанию.")
#else
#define RT_EMD_MESSAGE_FLOAT_PARSE_FAILURE_REPLACE_DEFAULT _T("floating point value couldn't be parced; default value was used.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_WARNING_INCORRECT_FACE_WAS_NOT_ADDED_TO_SHELL  _T("Некорректная грань не была добавлена в оболочку.")
#else
#define RT_EMD_WARNING_INCORRECT_FACE_WAS_NOT_ADDED_TO_SHELL  _T("Incorrect face was not added to shell.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_WARNING_SAME_EDGES_TWICE_IN_LOOP  _T("В цикле дважды встречается одинаковое ребро.")
#else
#define RT_EMD_WARNING_SAME_EDGES_TWICE_IN_LOOP  _T("Edge based on same curves twice enters a loop.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_WARNING_FACE_BOUNDS_NOT_CONNECTED_TO_SEAMS  _T("Границы замкнутой грани не стыкуются со швами.")
#else
#define RT_EMD_WARNING_FACE_BOUNDS_NOT_CONNECTED_TO_SEAMS  _T("Bounds of periodic face not connected with seams.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_WARNING_VRML_GRID_DUPLICATES_IN_MESHES  _T("Присутствуют дубликаты объектов в сетках.")
#else
#define RT_EMD_WARNING_VRML_GRID_DUPLICATES_IN_MESHES  _T("There are grid duplicates in meshes.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_WARNING_ACIS_LAW_INT_CURVE_IS_NOT_CREATED  _T("Кривая по закону не создана.")
#else
#define RT_EMD_WARNING_ACIS_LAW_INT_CURVE_IS_NOT_CREATED  _T("Law intersection curve is not created.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_WARNING_INT_CURVE_WAS_REPLACED_BY_SEGMENT  _T("Кривая пересечения была заменена отрезком.")
#else
#define RT_EMD_WARNING_INT_CURVE_WAS_REPLACED_BY_SEGMENT  _T("Intersection curve was replaced by segment.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_WARNING_INTERSECTION_CURVE_BAD_UNPUT  _T("Некорректные исходные данные для кривой пересечения.")
#else
#define RT_EMD_WARNING_INTERSECTION_CURVE_BAD_UNPUT  _T("Incorrect input data for intersection curve")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_WARNING_OPEN_SOLIDS_NOT_SUPPORTED  _T("Незамкнутые тела не поддерживаются форматом. Объект пропущен.")
#else
#define RT_EMD_WARNING_OPEN_SOLIDS_NOT_SUPPORTED  _T("Open solids are not supported by the format. Object has been skipped.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_WARNING_OPEN_MESHES_NOT_SUPPORTED  _T("Незамкнутые полигональные объекты не поддерживаются форматом. Объект пропущен.")
#else
#define RT_EMD_WARNING_OPEN_MESHES_NOT_SUPPORTED  _T("Open meshes are not supported by the format. Object has been skipped.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_WARNING_CLOSED_SOLID_BECAME_OPEN_MESH  _T("В результате триангуляции замкнутого тела сформировался незамкнутый полигональный объект, которые не поддерживаются форматом. Объект пропущен.")
#else
#define RT_EMD_WARNING_CLOSED_SOLID_BECAME_OPEN_MESH  _T("Triangulation of closed solid created an open mesh, which are not supported by the format. Object has been skipped.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_WARNING_EMPTY_MESH_SKIPPED  _T("Пропущен полигональный объект, не содержащий триангуляций.")
#else
#define RT_EMD_WARNING_EMPTY_MESH_SKIPPED  _T("Mesh without grids is skipped.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_WARNING_TRIANGULATION_FAILED  _T("Не удалось выполнить триангуляцию. Объект пропущен.")
#else
#define RT_EMD_WARNING_TRIANGULATION_FAILED  _T("Triangulation failed. Object is skipped.")
#endif

// \ru сообщения конвертера STEP \en messages of STEP converter
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_EDGE_CUVE_FLAG_T_SINGLE_REDEFINITION    _T("кривая дважды входит в ребро c флагом .T.")
#else
#define RT_EMD_ERROR_EDGE_CUVE_FLAG_T_SINGLE_REDEFINITION    _T("a curve is defined in edge twice with flag .T.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_EDGE_CUVE_FLAG_F_SINGLE_REDEFINITION    _T("кривая дважды входит в ребро c флагом .F.")
#else
#define RT_EMD_ERROR_EDGE_CUVE_FLAG_F_SINGLE_REDEFINITION    _T("a curve is defined in edge twice with flag .F.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_STEP_EDGE_CUVE_FLAG_T_MULTIPLE_REDEFINITION  _T("кривая более чем дважды входит в ребро c флагом .T.")
#else
#define RT_EMD_ERROR_STEP_EDGE_CUVE_FLAG_T_MULTIPLE_REDEFINITION  _T("a curve is defined in edge more than twice with flag .T.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_STEP_EDGE_CUVE_FLAG_F_MULTIPLE_REDEFINITION  _T("кривая более чем дважды входит в ребро c флагом .F.")
#else
#define RT_EMD_ERROR_STEP_EDGE_CUVE_FLAG_F_MULTIPLE_REDEFINITION  _T("a curve is defined in edge more than twice with flag .F.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_STEPUNDIFINED_FACE_GEOMETRY                  _T("")
#else
#define RT_EMD_ERROR_STEPUNDIFINED_FACE_GEOMETRY                  _T("")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_STEP_SYNTAX_MULTIPLE_DOT_IN_ENUM             _T("")
#else
#define RT_EMD_ERROR_STEP_SYNTAX_MULTIPLE_DOT_IN_ENUM             _T("")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_WARNING_STEP_POINT_CORRECTION                      _T("Скорректированы коодинаты точки.")
#else
#define RT_EMD_WARNING_STEP_POINT_CORRECTION                      _T("Coordinates of the point corrected.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_WARNING_STEP_EDGE_CERVE_BY_VERTICES                _T("Кривая ребра скорректирована с учётом коодинат вершин.")
#else
#define RT_EMD_WARNING_STEP_EDGE_CERVE_BY_VERTICES                _T("The curve of the ribs adjusted coordinates of the vertices.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_MESSAGE_STEP_FLAG_CHANGED_TO_F                     _T("заменяем флаг на .F.")
#else
#define RT_EMD_MESSAGE_STEP_FLAG_CHANGED_TO_F                     _T("change the flag to .F.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_MESSAGE_STEP_FLAG_CHANGED_TO_T                     _T("заменяем флаг на .T.")
#else
#define RT_EMD_MESSAGE_STEP_FLAG_CHANGED_TO_T                     _T("change the flag to .T.")
#endif

// \ru сообщения конвертера SAT \en SAT converter messages
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_WARNING_ACIS_UNSUPPORTED_INTERPOLE_CURVE_TYPE      _T("Данный подтип ACIS интерполяционной кривой не поддерживается")
#else
#define RT_EMD_WARNING_ACIS_UNSUPPORTED_INTERPOLE_CURVE_TYPE      _T("This subtype of ACIS interpolation curve is not supported")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_WARNING_ACIS_UNSUPPORTED_PARAMETRIC_CURVE_TYPE     _T("Данный подтип ACIS параметрической кривой не поддерживается")
#else
#define RT_EMD_WARNING_ACIS_UNSUPPORTED_PARAMETRIC_CURVE_TYPE     _T("This subtype of ACIS parametric curve is not supported")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_ACIS_UNSUPPORTED_VERSION                     _T("Данная версия ACIS NT не поддерживается")
#else
#define RT_EMD_ERROR_ACIS_UNSUPPORTED_VERSION                     _T("This version of ACIS NT is not supported")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_XT_UNSUPPORTED_VERSION                       _T("Данная версия фомата Parasolid не поддерживается")
#else
#define RT_EMD_ERROR_XT_UNSUPPORTED_VERSION                       _T("This version of Parasolid format is not supported")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_WARNING_ACIS_CANNOT_IMPORT_ENTITY_ID               _T("Не удалось импортироват объект с данным Id.")
#else
#define RT_EMD_WARNING_ACIS_CANNOT_IMPORT_ENTITY_ID               _T("Failed to import the object with the given Id.")
#endif

#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_WARNING_ACIS_INCORRECT_INT_ATTRIBUTE               _T("Некорректный целочисленный атрибут")
#else
#define RT_EMD_WARNING_ACIS_INCORRECT_INT_ATTRIBUTE               _T("Incorrect integer attribute")
#endif

// \ru сообщения конвертера IGES \en IGEC converter messages
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_IGES_INCORRECT_EXTERNAL_REFERENCE            _T("Неверное имя внешней ссылки")
#else
#define RT_EMD_ERROR_IGES_INCORRECT_EXTERNAL_REFERENCE            _T("Incorrect external reference name")
#endif

// \ru сообщения конвертера JT \en JT converter messages
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_JT_DUPLICATED_TRIANGLES            _T("Сетка содержит дублирующиеся треугольники и не может быть записана")
#else
#define RT_EMD_ERROR_JT_DUPLICATED_TRIANGLES            _T("Mesh contains duplicate triangles and cannot be written")
#endif


// \ru Сообщение плагина об общей проблеме \en General plugin problem notification
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_PLUGIN_GENERAL_PROBLEM            _T("Сбой общего характера в плагине")
#else
#define RT_EMD_ERROR_PLUGIN_GENERAL_PROBLEM            _T("General plugin fault")
#endif

// \ru Сообщение плагина об проблеме активации \en Activation plugin problem notification
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_PLUGIN_ACTIVATION_PROBLEM            _T("Ошибка активации плагина")
#else
#define RT_EMD_ERROR_PLUGIN_ACTIVATION_PROBLEM            _T("Plugin couldn't be activated")
#endif

// \ru Сообщение плагина об ограниченной или неподдерживаемой функциональности \en Limited plugin problem notification
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_PLUGIN_LIMITATION            _T("Плагин не обладает требуемой функциональностью")
#else
#define RT_EMD_ERROR_PLUGIN_LIMITATION            _T("Plugin' funtionality doesn't cover the required case")
#endif

// \ru Сообщение плагина о частичном успехе при импорте \en Partial import success plugin notification
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_WARNING_PLUGIN_PARTIAL_IMPORT            _T("Модель частично импортирована через плагин")
#else
#define RT_EMD_WARNING_PLUGIN_PARTIAL_IMPORT            _T("Model was partially imported via the plugin")
#endif

// \ru Модель не содержит геометрии \en Model does not contain geometry
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_MODEL_HAS_NO_GEOMETRY            _T("Модель не содержит геометрии")
#else
#define RT_EMD_ERROR_MODEL_HAS_NO_GEOMETRY            _T("Model does not contain geometry")
#endif

// \ru Файл, на который ссылается модель не найден \en File which is referenced by the model not found
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_REFERENCED_MODEL_FILE_NOT_FOUND            _T("Файл, на который ссылается модель не найден")
#else
#define RT_EMD_ERROR_REFERENCED_MODEL_FILE_NOT_FOUND            _T("File which is referenced by the model not found")
#endif

// \ru Метаданные импортированы успешно. \en Metadata imported successfully.
#if defined ( __NATIVE_LANGUAGE__ )
#define  RT_EMD_INFO_METADATA_SUCCESSFULLY_IMPORTED           _T("Метаданные импортированы успешно")
#else
#define  RT_EMD_INFO_METADATA_SUCCESSFULLY_IMPORTED           _T("Metadata imported successfully")
#endif

// \ru Метаданные импортированы не полностью. \en Metadata imported partially.
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_WARNING_METADATA_PARTIALLY_IMPORTED            _T("Метаданные импортированы не полностью")
#else
#define RT_EMD_WARNING_METADATA_PARTIALLY_IMPORTED            _T("Metadata imported partially")
#endif

// \ru Импорт метаданных не поддерживается. \en Metadata import not supported.
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_INFO_METADATA_NOT_SUPPORTED            _T("Импорт метаданных не поддерживается")
#else
#define RT_EMD_INFO_METADATA_NOT_SUPPORTED            _T("Metadata import not supported")
#endif

// \ru Модель не содержит метаданных. \en Model contains no metadata.
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_INFO_MODEL_CONTAINS_NO_METADATA            _T("Модель не содержит метаданных")
#else
#define RT_EMD_INFO_MODEL_CONTAINS_NO_METADATA            _T("Model contains no metadata")
#endif

// \ru Не удалось записать метаданные в указанную директорию. \en Failed to write metadata into specified directory.
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_INVALID_TARGET_DIRECTORY_FOR_METADATA            _T("Не удалось записать метаданные в указанную директорию")
#else
#define RT_EMD_ERROR_INVALID_TARGET_DIRECTORY_FOR_METADATA            _T("Failed to write metadata into specified directory")
#endif

// \ru Не удалось записать файл с метаданными. \en Failed to write metadata file.
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_METADATA_FILE_WRITE_FAILURE            _T("Не удалось записать файл с метаданными")
#else
#define RT_EMD_ERROR_METADATA_FILE_WRITE_FAILURE            _T("Failed to write metadata file")
#endif

// \ru Сбой импорта метаданных. \en Metadata import failure.
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EMD_ERROR_METADATA_IMPORT_ERROR            _T("Сбой импорта метаданных")
#else
#define RT_EMD_ERROR_METADATA_IMPORT_ERROR            _T("Metadata import failure")
#endif
 
// \ru результаты расчетов \en calculations results
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_THREAD_NOT_ON_BODY                                 _T("Резьба не принадлежит телу!")
#else
#define RT_THREAD_NOT_ON_BODY                                 _T("Thread does not belong to the solid!")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_THREAD_ON_BODY                                     _T("Резьба принадлежит телу!")
#else
#define RT_THREAD_ON_BODY                                     _T("Thread belongs to the solid!")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_INCORRECT_VIEW_PLANE                               _T("Неправильно выбрана плоскость вида!")
#else
#define RT_INCORRECT_VIEW_PLANE                               _T("The view plane is chosen incorrectly!")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_NOT_MAP_CURVES                                     _T("Нет кривых отображения!")
#else
#define RT_NOT_MAP_CURVES                                     _T("No mapping curves!")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_DEGENERATED_THREAD                                 _T("Резьба вырождена!")
#else
#define RT_DEGENERATED_THREAD                                 _T("The thread is degenerated!")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_THREAD_ADDED                                       _T("Резьба добавлена!")
#else
#define RT_THREAD_ADDED                                       _T("The thread is added!")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_THREAD_IS_NOT_ADDED                                _T("Сбой проецирования! Резьба не добавлена!")
#else
#define RT_THREAD_IS_NOT_ADDED                                _T("Projection failure! The thread is not added!")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_THREAD_IS_ADAPTED                                  _T("Подгонка выполнена!")
#else
#define RT_THREAD_IS_ADAPTED                                  _T("Adapting is done!")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_THREAD_IS_NOT_ADAPTED                              _T("Подгонка не удалась!")
#else
#define RT_THREAD_IS_NOT_ADAPTED                              _T("Adapting is failed!")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_NOT_IMPLEMENTED                                    _T("Не реализовано")
#else
#define RT_NOT_IMPLEMENTED                                    _T("Not implemented")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_THREAD_NOT_ON_BODY_OR_CAN_NOT_ADAPT                _T("Резьба не принадлежит телу или ее нельзя адаптировать")
#else
#define RT_THREAD_NOT_ON_BODY_OR_CAN_NOT_ADAPT                _T("The thread does not belong to the solid or it cannot be adapted")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_THREAD_ON_BODY_AND_ADAPTED_TO_IT                   _T("Резьба принадлежит телу и адаптирована к нему")
#else
#define RT_THREAD_ON_BODY_AND_ADAPTED_TO_IT                   _T("The thread belong to the solid and adapted to it")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_THREADS_NOT_CONJUGATE                              _T("Резьбы не сопрягаются \n\r")
#else
#define RT_THREADS_NOT_CONJUGATE                              _T("The treads are not conjugated \n\r")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_THREADS_IDENTICAL                                  _T("Резьбы идентичны \n\r")
#else
#define RT_THREADS_IDENTICAL                                  _T("The threads are identical \n\r")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_THREADS_CONJUGATE                                  _T("Резьбы сопрягаются \n\r")
#else
#define RT_THREADS_CONJUGATE                                  _T("The threads are conjugated \n\r")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_OBJECT_HAS_NOT_GRID                                _T("Объект не имеет сетки")
#else
#define RT_OBJECT_HAS_NOT_GRID                                _T("The object does not have a grid")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_INACCURACY                                         _T("Неточность")
#else
#define RT_INACCURACY                                         _T("Inaccuracy")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_OBJECT_NAS_NO_POLYCURVE                            _T("Объект не имеет характеристической ломаной")
#else
#define RT_OBJECT_NAS_NO_POLYCURVE                            _T("The object does not have a characteristic polyline")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_PRINCIPAL_CENTRAL_MOMENTS_OF_INERTIA_AND_ANGLE     _T("Главные центральные моменты инерции \n\r J1=  %18.10e   J2=  %18.10e   \n\rЛюбая ось является главной  Угол = %18.10e град.  \n\r")
#else
#define RT_PRINCIPAL_CENTRAL_MOMENTS_OF_INERTIA_AND_ANGLE     _T("The principal central moments of inertia  \n\r J1=  %18.10e   J2=  %18.10e   \n\rAny axis is a principal axis  Angle = %18.10e degrees.  \n\r")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_PRINCIPAL_CENTRAL_MOMENTS_OF_INERTIA_AND_ANGLE_X   _T("Главные центральные моменты инерции \n\r J1=  %18.10e   J2=  %18.10e   \n\rУгол между первой главной осью и осью x  %18.10e град. \n\r")
#else
#define RT_PRINCIPAL_CENTRAL_MOMENTS_OF_INERTIA_AND_ANGLE_X   _T("The principal central moments of inertia \n\r J1=  %18.10e   J2=  %18.10e   \n\rAngle between the first principal axis and x-axis   %18.10e degrees. \n\r")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_LENGTH_AREA_WC_AND_CENTRAL_MOMENTS_OF_INERTIA      _T("Длина кривой = %18.10e   \n\rПлощадь, ограниченная кривой = %18.10e \n\rКоординаты центра масс \n\r x= %18.10e  y= %18.10e \n\rЦентральные моменты инерции \n\r Jxx= %18.10e   Jyy= %18.10e   Jxy= %18.10e\n\r")
#else
#define RT_LENGTH_AREA_WC_AND_CENTRAL_MOMENTS_OF_INERTIA      _T("Curve length = %18.10e   \n\rArea bounded by the curve = %18.10e \n\rCenter mass coordinates \n\r x= %18.10e  y= %18.10e \n\rCentral moments of inertia \n\r Jxx= %18.10e   Jyy= %18.10e   Jxy= %18.10e\n\r")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define TOLERANCE_VALUE                                       _T("\n\n\r   Угловая толерантность %15.10e радиан")
#define RT_TIME_VALUE                                         _T("\n\r   Время расчёта   %lu mc")
#define RT_TIME_AND_TOLERANCE                                 _T("\n\n\r   Время расчёта   %lu mc \n\r   Угловая толерантность %15.10e радиан")
#else
#define TOLERANCE_VALUE                                       _T("\n\n\r   Angular tolerance %15.10e radians")
#define RT_TIME_VALUE                                         _T("\n\r   Computation time   %lu mc")
#define RT_TIME_AND_TOLERANCE                                 _T("\n\n\r   Computation time   %lu mc \n\r   Angular tolerance %15.10e radians")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_TIME_AND_AREA                                      _T("\n\n\r   Время расчёта   %lu mc \n\r   Площадь поверхности %15.10e ")
#else
#define RT_TIME_AND_AREA                                      _T("\n\n\r   Computation time   %lu mc \n\r   Area of surface %15.10e ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_PRINCIPAL_CENTRAL_MOMENTS_OF_INERTIA_AND_ANY_AXIS  _T("Главные центральные моменты инерции \n\r J1= %18.10e   J2= %18.10e   J3= %18.10e  \n\rЛюбая ось является главной ")
#else
#define RT_PRINCIPAL_CENTRAL_MOMENTS_OF_INERTIA_AND_ANY_AXIS  _T("The principal central moments of inertia \n\r J1= %18.10e   J2= %18.10e   J3= %18.10e  \n\rAny axis is a principal axis ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_PRINCIPAL_CENTRAL_MOMENTS_OF_INERTIA_AND_AXIS_3    _T("Главные центральные моменты инерции \n\r J1= %18.10e   J2= %18.10e   J3= %18.10e  \n\rНаправления главных осей \n\r ось 3  %18.10e  %18.10e  %18.10e    \n\r ось 1 и ось 2 - любые оси, ортогональные оси 3 ")
#else
#define RT_PRINCIPAL_CENTRAL_MOMENTS_OF_INERTIA_AND_AXIS_3    _T("The principal central moments of inertia \n\r J1= %18.10e   J2= %18.10e   J3= %18.10e  \n\rDirections of the principal axes \n\r axis 3  %18.10e  %18.10e  %18.10e    \n\r axis 1 and axis 2 are any axes orthogonal to axis 3 ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_PRINCIPAL_CENTRAL_MOMENTS_OF_INERTIA_AND_AXIS_1    _T("Главные центральные моменты инерции \n\r J1= %18.10e   J2= %18.10e   J3= %18.10e  \n\rНаправления главных осей \n\r ось 1  %18.10e  %18.10e  %18.10e    \n\r ось 2 и ось 3 - любые оси, ортогональные оси 1 ")
#else
#define RT_PRINCIPAL_CENTRAL_MOMENTS_OF_INERTIA_AND_AXIS_1    _T("The principal central moments of inertia \n\r J1= %18.10e   J2= %18.10e   J3= %18.10e  \n\rDirections of the principal axes \n\r axis 1  %18.10e  %18.10e  %18.10e    \n\r axis 2 and axis 3 are any axes orthogonal to axis 1 ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_DIRECTIONS_OF_PRINCIPAL_AXES                       _T("  \n\rНаправления главных осей \n\r ось 1  %18.10e  %18.10e  %18.10e    \n\r ось 2  %18.10e  %18.10e  %18.10e    \n\r ось 3  %18.10e  %18.10e  %18.10e    ")
#else
#define RT_DIRECTIONS_OF_PRINCIPAL_AXES                       _T("  \n\rDirections of the principal axes \n\r axis 1  %18.10e  %18.10e  %18.10e    \n\r axis 2  %18.10e  %18.10e  %18.10e    \n\r axis 3  %18.10e  %18.10e  %18.10e    ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_PRINCIPAL_CENTRAL_MOMENTS_OF_INERTIA               _T("Главные центральные моменты инерции \n\r J1= %18.10e   J2= %18.10e   J3= %18.10e")
#else
#define RT_PRINCIPAL_CENTRAL_MOMENTS_OF_INERTIA               _T("The principal central moments of inertia \n\r J1= %18.10e   J2= %18.10e   J3= %18.10e")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_MATRIX_OF_CENTRAL_MOMENTS_OF_INERTIA               _T(" \n\rМатрица центральных моментов инерции \n\r   %18.10e   %18.10e   %18.10e    \n\r   %18.10e   %18.10e   %18.10e    \n\r   %18.10e   %18.10e   %18.10e \n\r")
#else
#define RT_MATRIX_OF_CENTRAL_MOMENTS_OF_INERTIA               _T(" \n\rMatrix of central moments of inertia \n\r   %18.10e   %18.10e   %18.10e    \n\r   %18.10e   %18.10e   %18.10e    \n\r   %18.10e   %18.10e   %18.10e \n\r")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_MATRIX_OF_MOMENTS_OF_INERTIA_IN_CURRENT_PLACE      _T(" \n\rМатрица моментов инерции в текущей системе координат \n\r   %18.10e   %18.10e   %18.10e    \n\r   %18.10e   %18.10e   %18.10e    \n\r   %18.10e   %18.10e   %18.10e \n\r")
#else
#define RT_MATRIX_OF_MOMENTS_OF_INERTIA_IN_CURRENT_PLACE      _T(" \n\rMatrix of moments of inertia in the current coordinate system \n\r   %18.10e   %18.10e   %18.10e    \n\r   %18.10e   %18.10e   %18.10e    \n\r   %18.10e   %18.10e   %18.10e \n\r")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_WEIGHT_CENTRE                                      _T(" \n\rКоординаты центра масс \n\r          x= %18.10e \n\r          y= %18.10e \n\r          z= %18.10e")
#else
#define RT_WEIGHT_CENTRE                                      _T(" \n\rCoordinates of center of mass \n\r          x= %18.10e \n\r          y= %18.10e \n\r          z= %18.10e")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_AREA_VOLUME_MASS_AND_DENSITY                       _T("Площадь поверхности = %18.10e \n\rОбъем                       = %18.10e \n\rМасса                        = %18.10e \n\rПлотность                  = %18.10e")
#else
#define RT_AREA_VOLUME_MASS_AND_DENSITY                       _T("Surface area = %18.10e \n\rVolume                       = %18.10e \n\rMass                        = %18.10e \n\rDensity                  = %18.10e")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_CURVATURE_OF_CURVE                                 _T("радиус кривизны кривой %22.15e\n\rкривизна кривой %22.15e")
#else
#define RT_CURVATURE_OF_CURVE                                 _T("radius of curve curvature %22.15e\n\rcurve curvature %22.15e")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_CURVE_LENGTH_THE_ARC                               _T("Длина кривой (по дуге) %22.15e")
#else
#define RT_CURVE_LENGTH_THE_ARC                               _T("Curve length (by arc) %22.15e")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_SAG                                                _T("Введенное значение стрелки прогиба = %5.10lf \n\n\rРезультаты: \n\rМинимальный прогиб = %5.10lf \n\rМаксимальный прогиб = %5.10lf \n\rСредний прогиб = %5.10lf")
#else
#define RT_SAG                                                _T("Entered sag = %5.10lf \n\n\rResults: \n\rMinimum sag = %5.10lf \n\rMaximum sag = %5.10lf \n\rAverage sag = %5.10lf")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_ERROR_RESULT_NOT_OBTAINED                          _T("Ошибка! Результат не получен")
#else
#define RT_ERROR_RESULT_NOT_OBTAINED                          _T("Error! Result is not obtained")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_ERROR_BAD_ALLOC                                    _T("Исключение! Не удалось выделить память.")
#else
#define RT_ERROR_BAD_ALLOC                                    _T("Exception! Memory allocation failed.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_ERROR_GENERAL_EXCEPTION                            _T("Исключение! Неизвестная ошибка.")
#else
#define RT_ERROR_GENERAL_EXCEPTION                            _T("Exception! Unknown error.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_DISTANCE_BETWEEN_OBJECTS                           _T("Расстояние между объектами %15.8e")
#else
#define RT_DISTANCE_BETWEEN_OBJECTS                           _T("Distance between the objects is %15.8e")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_DISTANCE_IS_NOT_FOUND                              _T("Расстояние не найдено!")
#else
#define RT_DISTANCE_IS_NOT_FOUND                              _T("Distance is not found!")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_DISTANCE                                           _T("Расстояние: %15.8f")
#else
#define RT_DISTANCE                                           _T("Distance: %15.8f")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_MIN_DISTANCE                                       _T("Минимальное расстояние: %15.8f")
#else
#define RT_MIN_DISTANCE                                       _T("Minimal distance: %15.8f")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_MAX_DISTANCE                                       _T("Максимальное расстояние: %15.8f")
#else
#define RT_MAX_DISTANCE                                       _T("Maximal distance: %15.8f")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_MID_VALUE                                          _T("Среднее значение: %15.8f")
#else
#define RT_MID_VALUE                                          _T("Average value: %15.8f")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_SIGMA_VALUE                                        _T("Среднеквадратичное отклонение: %15.8f")
#else
#define RT_SIGMA_VALUE                                        _T("Standard deviation: %15.8f")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_INIT_DISTANCE                                      _T("Исходное расстояние: %15.8f")
#else
#define RT_INIT_DISTANCE                                      _T("Initial distance: %15.8f")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_NOT_SELECTED_FIRTS_OBJECT                          _T("Не выбран первый объект")
#else
#define RT_NOT_SELECTED_FIRTS_OBJECT                          _T("The first object is not selected")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_NOT_SELECTED_SECOND_OBJECT                         _T("Не выбран второй объект")
#else
#define RT_NOT_SELECTED_SECOND_OBJECT                         _T("The second object is not selected")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_SELECT_POINT_CURVE_OR_SURFACE                      _T("Надо выбрать точку, кривую или поверхность")
#else
#define RT_SELECT_POINT_CURVE_OR_SURFACE                      _T("A point, a curve or a surface should be chosen")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_POINT_AND_ANGLE                                    _T("В точке %.3f \n\r угол между гранями  %22.15e град.")
#else
#define RT_POINT_AND_ANGLE                                    _T("At point %.3f \n\r angle between faces  %22.15e degrees")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_POINT_AND_ANGLE_F                                  _T("В точке %f \n\r угол между гранями %f град.")
#else
#define RT_POINT_AND_ANGLE_F                                  _T("At point %f \n\r angle between faces %f degrees")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_MIN_ANGLE_AND_PARAM                                _T("Минимальный угол %f град. \n\r находится в точке с параметром %f")
#else
#define RT_MIN_ANGLE_AND_PARAM                                _T("Minimum angle %f degrees \n\r is located at point with parameter %f")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_MAX_ANGLE_AND_PARAM                                _T("Максимальный угол %f град. \n\r находится в точке с параметром %f")
#else
#define RT_MAX_ANGLE_AND_PARAM                                _T("Maximum angle %f degrees \n\r is located at point with parameter %f")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EDGES_ANGLE                                        _T("Угол между рёбрами  %22.15e град.")
#else
#define RT_EDGES_ANGLE                                        _T("Angle between edges  %22.15e degrees")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_POINT_ANGLE_AND_DEGENERATE_EDGE                    _T("В точке %.3f \n\r угол между гранями  %22.15e град. \n\r ребро является вырожденным  ")
#else
#define RT_POINT_ANGLE_AND_DEGENERATE_EDGE                    _T("At point %.3f \n\r angle between faces  %22.15e dergees \n\r the edge is degenerate  ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_POINT_ANGLE_AND_TRANSITION_LINE                    _T("В точке %.3f \n\r угол между гранями  %22.15e град. \n\r ребро является линией перехода  ")
#else
#define RT_POINT_ANGLE_AND_TRANSITION_LINE                    _T("At point %.3f \n\r angle between faces  %22.15e dergees \n\r the edge is a transition line  ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_POINT_ANGLE_AND_LINE_OF_PARTITION                  _T("В точке %.3f \n\r угол между гранями  %22.15e град. \n\r ребро является линией разбиения  ")
#else
#define RT_POINT_ANGLE_AND_LINE_OF_PARTITION                  _T("At point %.3f \n\r angle between faces  %22.15e dergees \n\r the edge is a partition line  ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_POINT_ANGLE_AND_SEAM                               _T("В точке %.3f \n\r угол между гранями  %22.15e град. \n\r ребро является швом  ")
#else
#define RT_POINT_ANGLE_AND_SEAM                               _T("At point %.3f \n\r angle between faces  %22.15e dergees \n\r the edge is a seam  ")
#endif

#if defined ( __NATIVE_LANGUAGE__ )
#define RT_MIN_CURVATURE_CHAR_VALUE                            _T("Минимальное значение характеристики кривизны = %f")
#else
#define RT_MIN_CURVATURE_CHAR_VALUE                            _T("Minimum curvature characteristic value = %f")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_MAX_CURVATURE_CHAR_VALUE                           _T("Максимальное значение характеристики кривизны = %f")
#else
#define RT_MAX_CURVATURE_CHAR_VALUE                           _T("Maximum curvature characteristic value = %f")
#endif

#if defined ( __NATIVE_LANGUAGE__ )
#define RT_POINT_OBJECT_DISTANCE                              _T("Расстояние от точки до объекта %22.15e")
#else
#define RT_POINT_OBJECT_DISTANCE                              _T("Distance from a point to the object %22.15e")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_DISTANCE_IS_DEGENERATE                             _T("Расстояние вырожденное!")
#else
#define RT_DISTANCE_IS_DEGENERATE                             _T("Distance is degenerated!")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_DISTANCE_LESS_THAN_ERROR                           _T("Расстояние меньше погрешности!")
#else
#define RT_DISTANCE_LESS_THAN_ERROR                           _T("The distance is less than tolerance!")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_UNABLE_TO_COMPUTE                                  _T("Не удалось вычислить")
#else
#define RT_UNABLE_TO_COMPUTE                                  _T("Unable to compute")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_MAX_AND_MIN_CURVATURE                              _T("Максимальная положительная кривизна: %15.8f\nМинимальная отрицательная кривизна:  %15.8f")
#else
#define RT_MAX_AND_MIN_CURVATURE                              _T("Maximum positive curvature: %15.8f\nMinimum negative curvature: %15.8f")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_LENGTH_MAX_CURVATURE                               _T("Длина кривой: %15.8f \nМаксимальная кривизна: %15.8f")
#else
#define RT_LENGTH_MAX_CURVATURE                               _T("Curve length: %15.8f \nCurvature max: %15.8f")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_ERROR_IN_PARAMS                                    _T("Ошибка в параметрах")
#else
#define RT_ERROR_IN_PARAMS                                    _T("Error in parameters")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_PROCESS_IS_STOPPED                                 _T("Процесс остановлен до полного завершения")
#else
#define RT_PROCESS_IS_STOPPED                                 _T("Process stopped before ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_NOT_SELECT_GUIDE_SEGMENT                           _T("Не выбран направляющий отрезок")
#else
#define RT_NOT_SELECT_GUIDE_SEGMENT                           _T("Guide segment is not selected")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_NOT_SELECTED_ONE_OF_THE_SURFACES                   _T("Не выбрана одна из поверхностей")
#else
#define RT_NOT_SELECTED_ONE_OF_THE_SURFACES                   _T("One of surfaces is not selected")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_SURFACES_ARE_NOT_SELECTED                          _T("Поверхности не выбраны")
#else
#define RT_SURFACES_ARE_NOT_SELECTED                          _T("Surfaces are not chosen")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_SELECT_SAME_SURFACE                                _T("Выбрали ту же поверхность")
#else
#define RT_SELECT_SAME_SURFACE                                _T("The same surface is chosen")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_BOTH_SURFACES_NO_AXES                              _T("У обоих поверхностей нет осей вращения")
#else
#define RT_BOTH_SURFACES_NO_AXES                              _T("There is no rotation axes of both surfaces")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_SECOND_SURFACE_NO_AXIS                             _T("У второй поверхности нет оси вращения")
#else
#define RT_SECOND_SURFACE_NO_AXIS                             _T("There is no rotation axis of the second surface")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_FIRST_SURFACE_NO_AXIS                              _T("У первой поверхности нет оси вращения")
#else
#define RT_FIRST_SURFACE_NO_AXIS                              _T("There is no rotation axis of the first surface")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_ANGLE_BETWEEN_AXES                                 _T("Угол между осями: %16.8f \n")
#else
#define RT_ANGLE_BETWEEN_AXES                                 _T("Angle between axes: %16.8f \n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_DISTANCE_BETWEEN_AXES                              _T("Расстояние между осями: %16.8f \n")
#else
#define RT_DISTANCE_BETWEEN_AXES                              _T("Distance between axes: %16.8f \n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_CURVE_SELFINTERSECTION                             _T("Самопересечение двумерной кривой")
#else
#define RT_CURVE_SELFINTERSECTION                             _T("Two dimensional curve self-intersection")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_SURFACE_SELFINTERSECTION                             _T("Самопересечение поверхности")
#else
#define RT_SURFACE_SELFINTERSECTION                             _T("Surface self-intersection")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_SHELL_SELFINTERSECTION                             _T("Самопересечение оболочки")
#else
#define RT_SHELL_SELFINTERSECTION                             _T("Shell self-intersection")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_SELFINTERSECTING_FACES_COUNT                       _T("Количество самопересекающихся граней")
#else
#define RT_SELFINTERSECTING_FACES_COUNT                       _T("Self-intersecting faces count: ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_INTERSECTING_FACES_COUNT                           _T("Количество пересекающихся граней")
#else
#define RT_INTERSECTING_FACES_COUNT                           _T("Intersecting faces count:      ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_INTERSECTING_CURVES_COUNT                           _T("Количество кривых пересечения")
#else
#define RT_INTERSECTING_CURVES_COUNT                           _T("Intersecting curves count:    ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_IS_SELFINTERSECTION                                _T("Есть самопересечение")
#else
#define RT_IS_SELFINTERSECTION                                _T("There is a self-intersection")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_NO_SELFINTERSECTION                                _T("Нет самопересечения")
#else
#define RT_NO_SELFINTERSECTION                                _T("There is no self-intersection")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_CURVE2D_SPEED                                      _T("Скорость работы двумерной кривой")
#else
#define RT_CURVE2D_SPEED                                      _T("Two-dimensional curve speed")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_CURVE3D_SPEED                                      _T("Скорость работы простанственной кривой")
#else
#define RT_CURVE3D_SPEED                                      _T("Spatial curve speed")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_SURFACE_SPEED                                      _T("Скорость работы поверхности")
#else
#define RT_SURFACE_SPEED                                      _T("Surface speed")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_POLYGON_POINTS_COUNT                               _T(" - Количество точек полигона: %d \n\n")
#else
#define RT_POLYGON_POINTS_COUNT                               _T(" - Number of points of a polygon: %d \n\n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_TIME_MSEC                                          _T(" - Время выполнения: %d, мс \n")
#else
#define RT_TIME_MSEC                                          _T(" - Execution time: %d, ms \n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_TIME_SEC                                           _T(" - Время выполнения: %f, сек. \n")
#else
#define RT_TIME_SEC                                           _T(" - Execution time: %f, sec. \n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_TIME_SEC1                                          _T("Время выполнения: %f, сек. \n")
#else
#define RT_TIME_SEC1                                          _T("Execution time: %f, sec. \n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_TEST_TIME_MSEC                                     _T("Время выполнения:       %s = %d милисек.")
#else
#define RT_TEST_TIME_MSEC                                     _T("Execution time:       %s = %d msec.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_SIZE_DEFLECTION                                    _T(" - Размер допустимого прогиба: %f, мм \n")
#else
#define RT_SIZE_DEFLECTION                                    _T(" - Size of permissible deflection: %f, мм \n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_MAX_AVERAGE_DISTANCE                                _T("Отклонение: макимальное %f , среднее %f")
#else
#define RT_MAX_AVERAGE_DISTANCE                                _T("Deviation: maximum %f , average %f")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_REQUESTS_COUNT                                     _T(" - Количество запросов: %d \n\n")
#else
#define RT_REQUESTS_COUNT                                     _T(" - Count of requests: %d \n\n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_REQUESTS_COUNT1                                    _T("Количество запросов: %d")
#else
#define RT_REQUESTS_COUNT1                                    _T("Count of requests: %d")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_SAMPLES_COUNT                                     _T(" - Количество запросов: %d \n\n")
#else
#define RT_SAMPLES_COUNT                                     _T(" - Count of samples: %d \n\n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_SAMPLES_COUNT1                                     _T("Количество запросов: %d")
#else
#define RT_SAMPLES_COUNT1                                     _T("Count of samples: %d")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_GAB_DIAGONAL                                       _T(" - Диагональ габарита: %f, мм \n")
#else
#define RT_GAB_DIAGONAL                                       _T(" - Diagonal of bounding box: %f, мм \n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_CALCULATE_POLYGON                                  _T(" Расчёт отрисовочного полигона\n")
#else
#define RT_CALCULATE_POLYGON                                  _T(" Calculation of polygon for drawing\n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_TEST_RESULTS                                       _T("Результаты проверки: \n\n")
#else
#define RT_TEST_RESULTS                                       _T("Check results: \n\n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_AVERAGE_SPEED_MSEC                                 _T(" - Средняя скорость: %f, мс/тыс. \n")
#else
#define RT_AVERAGE_SPEED_MSEC                                 _T(" - Average speed: %f, ms/thousand. \n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_CALCULATE_POINT_AND_DERIVE                         _T(" Расчёт точки и первой производной\n")
#else
#define RT_CALCULATE_POINT_AND_DERIVE                         _T(" Calculation of a point and first derivative\n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_PROJECTING_BOUNDING_POINTS                         _T(" Проецирование точек граней габаритного куба\n")
#else
#define RT_PROJECTING_BOUNDING_POINTS                         _T(" Projection of points of bounding box faces\n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_PROJECTING_BOUNDING_POINTS_IN_DIRECTION            _T(" Проецирование точек граней габаритного куба по направлению\n")
#else
#define RT_PROJECTING_BOUNDING_POINTS_IN_DIRECTION            _T(" Projection of points of bounding box faces in direction\n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_CALCULATE_SURFACE_TESSELATION                      _T(" Расчёт разбивки поверхности для визуализации и вычислений\n")
#else
#define RT_CALCULATE_SURFACE_TESSELATION                      _T(" Calculation of surface tesselation for visualization and computations\n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_CALCULATE_SURFACE_GAB                              _T(" Расчёт габарита поверхности\n")
#else
#define RT_CALCULATE_SURFACE_GAB                              _T(" Calculation of surface bounding box\n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_CUBE_PHANTOME_ERROR                                _T("Ошибка при создании фантома локального куба")
#else
#define RT_CUBE_PHANTOME_ERROR                                _T("Error of a local cube phantome creation")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_QUADRANGLES_COUNT                                  _T("Число четырехугольников:      ")
#else
#define RT_QUADRANGLES_COUNT                                  _T("Count of quadrangles:      ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_TRIANGLES_COUNT                                    _T("Число треугольников:          ")
#else
#define RT_TRIANGLES_COUNT                                    _T("Count of triangles:          ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_POINTS_COUNT                                       _T("Число точек:                  ")
#else
#define RT_POINTS_COUNT                                       _T("Count of points:                  ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_TOTAL_FACES_COUNT                                  _T("Общее число граней:           ")
#else
#define RT_TOTAL_FACES_COUNT                                  _T("Total count of faces:           ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_BODIES_COUNT                                       _T("Число тел:                    ")
#else
#define RT_BODIES_COUNT                                       _T("Count of solids:                    ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_SOLVING_CODE                                       _T("Код решения: %u")
#else
#define RT_SOLVING_CODE                                       _T("Solution code: %u")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_BLOCKS_COUNT                                       _T("Число блоков:     ")
#else
#define RT_BLOCKS_COUNT                                       _T("Count of blocks:     ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_MATES_COUNT                                        _T("Число сопряжений: ")
#else
#define RT_MATES_COUNT                                        _T("Count of matings: ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_HIDE_LINES_COUNT                                   _T("Число скрытых линий    :  ")
#else
#define RT_HIDE_LINES_COUNT                                   _T("Count of hidden lines    :  ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_VISIBLE_LINES_COUNT                                _T("Число видимых линий    :  ")
#else
#define RT_VISIBLE_LINES_COUNT                                _T("Count of visible lines    :  ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_FACES_COUNT                                        _T("Число граней     :  ")
#else
#define RT_FACES_COUNT                                        _T("Count of faces    :  ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_OPERATIONS_COUNT                                   _T("Число операций   :  ")
#else
#define RT_OPERATIONS_COUNT                                   _T("Count of operations   :  ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_WRITE_ERR_LICENSE                                  _T("Файл не записан.\nНе найдена лиценция.")
#else
#define RT_WRITE_ERR_LICENSE                                  _T("File not written.\nLicense not found.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_FILE_NOT_WRITTEN                                   _T("Файл не записан")
#else
#define RT_FILE_NOT_WRITTEN                                   _T("File not written")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_FILE_WRITE_ERR                                   _T("Ошибка записи файла")
#else
#define RT_FILE_WRITE_ERR                                   _T("File export error")
#endif

#if defined ( __NATIVE_LANGUAGE__ )
#define RT_FILE_NOT_OPEN                                      _T("Файл не открыт")
#else
#define RT_FILE_NOT_OPEN                                      _T("File not open")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_FORMAT_NOT_SUPPORTED                               _T("Форрмат файла не поддерживается")
#else
#define RT_FORMAT_NOT_SUPPORTED                               _T("File format not supported")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_PLUGIN_LOAD_FAULT                                  _T("Сбой загрузки плагина")
#else
#define RT_PLUGIN_LOAD_FAULT                                  _T("Plugin load fault")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_FORMAT_VERSION_NOT_SUPPORTED                       _T("Неподдерживаемая версия формата")
#else
#define RT_FORMAT_VERSION_NOT_SUPPORTED                       _T("Format version not supported")
#endif

#if defined ( __NATIVE_LANGUAGE__ )
#define RT_CANNOT_LOAD_FORMAT_SPECIFIC_PLUGIN                 _T("Не удалось загрузить плагин для чтения этого формата")
#else
#define RT_CANNOT_LOAD_FORMAT_SPECIFIC_PLUGIN                 _T("Cannot load the plugin for reading this format")
#endif

#if defined ( __NATIVE_LANGUAGE__ )
#define RT_LICENCE_CHECK_FAILURE                              _T("Сбой активации")
#else
#define RT_LICENCE_CHECK_FAILURE                              _T("Licence check failure")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_USER_CANCELED                                      _T("Отменено пользователем")
#else
#define RT_USER_CANCELED                                      _T("User canceled")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_FILE_NO_OBJECT                                     _T("Файл не содержит объектов")
#else
#define RT_FILE_NO_OBJECT                                     _T("File without objects")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_FILE_WITH_UNKNOWN_OBJECTS                          _T("Файл содержит неизвестные объекты.\nВыполнено частичное чтение")
#else
#define RT_FILE_WITH_UNKNOWN_OBJECTS                          _T("File contains unknown objects.\nPerformed partial reading")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_FILE_WITH_UNKNOWN_ATTR                             _T("Файл содержит неизвестные атрибуты.\nВыполнено частичное чтение")
#else
#define RT_FILE_WITH_UNKNOWN_ATTR                             _T("File contains unknown attributes.\nPerformed partial reading")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_FILE_FAIL_READ_CATALOG                          _T("Файл не открыт.\nОшибка чтения каталога модели.")
#else
#define RT_FILE_FAIL_READ_CATALOG                          _T("File not open.\nModel catalog read error.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_FILE_BAD_FORMAT                                 _T("Неверный формат файла.")
#else
#define RT_FILE_BAD_FORMAT                                 _T("Incorrect file format")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_FILE_BAD_CLASSID                                 _T("Ошибка чтения.\nФайл содержит неизвестные объекты.")
#else
#define RT_FILE_BAD_CLASSID                                 _T("Reading error.\nFile contains unknown objects.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_FILE_OUT_OF_MEM                                  _T("Ошибка чтения.\nНехватка памяти.")
#else
#define RT_FILE_OUT_OF_MEM                                  _T("Reading error.\nRun out of memory.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_OUT_OF_MEM_WARN                                  _T("Недостаточно памяти для расчета визуализации.")
#else
#define RT_OUT_OF_MEM_WARN                                  _T("Not enough memory to show.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_OUT_OF_MEM_ERR                                   _T("Недостаточно памяти для операции.")
#else
#define RT_OUT_OF_MEM_ERR                                   _T("Not enough memory for the operation.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_ERR_LICENSE                                      _T("Ошибка чтения.\nЛицензия не найдена.")
#else
#define RT_ERR_LICENSE                                      _T("Reading error.\nLicense not found.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_ERR_FILE_VERSION                                 _T("Ошибка чтения.\nФайл создан в более новой версии C3D.")
#else
#define RT_ERR_FILE_VERSION                                 _T("Reading error.\nFile created in newer C3D version.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_WAR_FILE_VERSION                                 _T("Файл создан в более новой версии C3D.")
#else
#define RT_WAR_FILE_VERSION                                 _T("File created in newer C3D version.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_VIEW_MEMORY_ERROR                                _T("Ошибка визуализации.\nНехватка памяти.")
#else
#define RT_VIEW_MEMORY_ERROR                                _T("Visualization error.\nRun out of memory.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_VIEW_ERROR                                       _T("Ошибка при расчете визуализации.")
#else
#define RT_VIEW_ERROR                                       _T("Error while calculating visualization.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_NULL_FILE                                        _T("Файл нулевой длины.")
#else
#define RT_NULL_FILE                                        _T("Zero-length file.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_FILE_NOT_FOUND                                   _T("Файл не найден.")
#else
#define RT_FILE_NOT_FOUND                                    _T("File not found.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_ERROR_READING_PARTLY                               _T("Ошибка при чтении.\nЧтение выполнено частично")
#else
#define RT_ERROR_READING_PARTLY                               _T("Error of reading.\nReading is performed partially")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_WRITTING_ERROR                                     _T("Файл записан с введением ошибки")
#else
#define RT_WRITTING_ERROR                                     _T("File is written with error")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_WRITTING_IN_MODEL_FILE                             _T("Запись в файл модели\0")
#else
#define RT_WRITTING_IN_MODEL_FILE                             _T("Writing to model file\0")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_ISOCLINES_NO_SURFACAE                              _T("Изоклины поверхности отсутствуют")
#else
#define RT_ISOCLINES_NO_SURFACAE                              _T("There are no isoclines")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_TOUCHING_PLANE_FAILED                              _T("Касание плоскости не выполнено")
#else
#define RT_TOUCHING_PLANE_FAILED                              _T("Touching of plane is not satisfied")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_PASSAGE_OF_POINTS_NOT_EXECUTED                     _T("Проход через характеристические точки не выполнен")
#else
#define RT_PASSAGE_OF_POINTS_NOT_EXECUTED                     _T("Passing through control points is not satisfied")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_CONVERT_TO_NURBS_FAILED                            _T("Преобразование в NURBS не выполнено")
#else
#define RT_CONVERT_TO_NURBS_FAILED                            _T("Failed conversion to NURBS")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EDGE_PROJECT_ERROR                                 _T("Не удалось спроецировать все ребра на NURBS-поверхность")
#else
#define RT_EDGE_PROJECT_ERROR                                 _T("Failed to project all the edges to NURBS-surface")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_CUTTING_MODEL                                      _T("Резка модели")
#else
#define RT_CUTTING_MODEL                                      _T("Cutting the model")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_CUTTING_SOLID                                      _T("Разрез тела")
#else
#define RT_CUTTING_SOLID                                      _T("Cutting the solid")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_THICKENING_SOLID                                   _T("Придание толщины")
#else
#define RT_THICKENING_SOLID                                   _T("Thickening")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_BOOLEAN_SOLID                                      _T("Составное тело")
#else
#define RT_BOOLEAN_SOLID                                      _T("Boolean solid")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_CUTTING_SHELL                                      _T("Разрез оболочки")
#else
#define RT_CUTTING_SHELL                                      _T("Cutting the shell")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_BASIS_OBJECTS_ERROR                                _T("Не удалось получить базовые объекты")
#else
#define RT_BASIS_OBJECTS_ERROR                                _T("Failed to get the base objects")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_COMPILER_VERSION                                   _T("Данный функционал доступен только при сборке приложения компилятором, поддерживающим C++17")
#else
#define RT_COMPILER_VERSION                                   _T("This functionality is only available when building the application with a compiler that supports C++17")
#endif

#if defined ( __NATIVE_LANGUAGE__ )
#define RT_REBUILDING_WITH_CHANGED                _T("Перестроение выполнено с изменениями")
#else
#define RT_REBUILDING_WITH_CHANGED                _T("Rebuilding is performed with changes")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_OBJECT_REBUILD_WITH_CHANGED            _T("Объект перестроен с изменениями")
#else
#define RT_OBJECT_REBUILD_WITH_CHANGED            _T("Object is rebuilt with changes")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_REBUILDING_NOT_COMPLETELY              _T("Перестроение выполнено не полностью")
#else
#define RT_REBUILDING_NOT_COMPLETELY              _T("Rebuilding is performed not completely")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_OBJECT_REBUILD_NOT_COMPLETELY          _T("Объект перестроен не полностью")
#else
#define RT_OBJECT_REBUILD_NOT_COMPLETELY          _T("Object is rebuilt not completely")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_OBJECT_BUILD_NOT_COMPLETELY            _T("Тело построено не полностью")
#else
#define RT_OBJECT_BUILD_NOT_COMPLETELY            _T("Solid is rebuilt not completely")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_SPIRAL_SURFACES_NOT_FOUND              _T("Спиральных поверхностей не найдено")
#else
#define RT_SPIRAL_SURFACES_NOT_FOUND              _T("Spiral surfaces were not found")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_SAME_NAMES                             _T("Существуют одинаковые имена")
#else
#define RT_SAME_NAMES                             _T("There are same names")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_WRONG_MAIN_NAMES                       _T("Есть несоответствующие главные имена")
#else
#define RT_WRONG_MAIN_NAMES                       _T("There are unrelated main names")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_FACE_PLUS_NUMBER                       _T("\n\rНомер грани Plus  (слева)  ")
#else
#define RT_FACE_PLUS_NUMBER                       _T("\n\rFace number Plus  (on the left)  ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_NOT_FILLET                             _T("Не скругление ")
#else
#define RT_NOT_FILLET                             _T("Not fillet")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_FILLET                                 _T("Скругление ")
#else
#define RT_FILLET                                 _T("Fillet")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_CONST_RADIUS_FILLET                    _T("Скругление с постоянным радиусом ")
#else
#define RT_CONST_RADIUS_FILLET                    _T("Fillet with const radius")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_CORNER_FILLET                          _T("Угловое скругление ")
#else
#define RT_CORNER_FILLET                          _T("Corner Fillet")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_VARIABLE_RADIUS_FILLET                 _T("Скругление с переменным радиусом")
#else
#define RT_VARIABLE_RADIUS_FILLET                 _T("Fillet with variable radius")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_NOT_RECOGNIZED                         _T("Не распознанный тип скругления")
#else
#define RT_NOT_RECOGNIZED                         _T("Not recognized fillet type")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_FACE_MINUS_NUMBER                      _T("\n\rНомер грани Minus (справа)  ")
#else
#define RT_FACE_MINUS_NUMBER                      _T("\n\rFace number Minus (on the right)  ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EDGE_AND_FACES_FROM_DIFF_SHELLS        _T("Ребро и грани из разных оболочек")
#else
#define RT_EDGE_AND_FACES_FROM_DIFF_SHELLS        _T("Edge and faces are from different shells")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EDGE_NOT_BELONG_FACE                   _T("Ребро не принадлежит выбранной грани")
#else
#define RT_EDGE_NOT_BELONG_FACE                   _T("Edge does not belong to the selected face")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EDGE_NUMBER_IN_LOOP                    _T(" Номер ребра в цикле - ")
#else
#define RT_EDGE_NUMBER_IN_LOOP                    _T(" Edge number in the loop - ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_LOOP_NUMBER_IN_FACE                    _T(" Номер цикла в грани - ")
#else
#define RT_LOOP_NUMBER_IN_FACE                    _T(" Number of loop in the face - ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_TANGENT_NOT_ROUND_COLLINEAR            _T("RoundColinear: касательные НЕ параллельны \n")
#else
#define RT_TANGENT_NOT_ROUND_COLLINEAR            _T("RoundColinear: tangents are NOT parallel \n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_TANGENT_ROUND_COLLINEAR                _T("RoundColinear: касательные параллельны \n")
#else
#define RT_TANGENT_ROUND_COLLINEAR                _T("RoundColinear: tangents are parallel \n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_TANGENT_NOT_COLLINEAR                  _T("Colinear: касательные НЕ параллельны \n")
#else
#define RT_TANGENT_NOT_COLLINEAR                  _T("Colinear: tangents are NOT parallel \n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_TANGENT_COLLINEAR                      _T("Colinear: касательные параллельны \n")
#else
#define RT_TANGENT_COLLINEAR                      _T("Colinear: tangents are parallel \n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_ANGLE_BETWEEN_TANGENT                  _T("Угол между касательными: %16.8f, град \n")
#else
#define RT_ANGLE_BETWEEN_TANGENT                  _T("Angle between tangents: %16.8f, degrees \n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_PROJ_MEASUREMENTS                      _T("Проекционные замеры \n\n")
#else
#define RT_PROJ_MEASUREMENTS                      _T("Projection measurements \n\n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_PROJ_DIRECTION_INVERTED                _T("Направление проекции инверсное")
#else
#define RT_PROJ_DIRECTION_INVERTED                _T("Projection direction is inverted")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_DISTANCE_BETWEEN_POINTS                _T("Расстояние между точкам: %16.8f, мм \n")
#else
#define RT_DISTANCE_BETWEEN_POINTS                _T("Distance between two points: %16.8f, mm \n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_SPACE_MEASUREMENT                      _T("Пространственные замеры \n\n")
#else
#define RT_SPACE_MEASUREMENT                      _T("Spatial measurements \n\n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_CURVES_NOT_DOCKED                      _T("Кривые не состыкованы")
#else
#define RT_CURVES_NOT_DOCKED                      _T("Curves are not joined")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_VERTICES_COUNT                         _T("Количество вершин = ")
#else
#define RT_VERTICES_COUNT                         _T("Number of vertices = ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EDGES_COUNT                            _T("Количество ребер = ")
#else
#define RT_EDGES_COUNT                            _T("Number of edges = ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_MATCHES_NOT_FOUND                      _T("Соответствий не найдено")
#else
#define RT_MATCHES_NOT_FOUND                      _T("Matches not found")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_OPERATION_NAME                         _T("Имя операции\0")
#else
#define RT_OPERATION_NAME                         _T("Operation name\0")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_RUNNING_TIME                           _T("Время исполнения\0")
#else
#define RT_RUNNING_TIME                           _T("Execution time\0")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_AVERAGE_TIME                           _T("Среднее время\0")
#else
#define RT_AVERAGE_TIME                           _T("Average time\0")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_CALLS_COUNT                            _T("Кол. вызовов\0")
#else
#define RT_CALLS_COUNT                            _T("Number of calls\0")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_FUNC_NAME_TIME                         _T("Имя функции               Время исполнения       Среднее       Вызовов\r\n")
#else
#define RT_FUNC_NAME_TIME                         _T("Function name               Execution time       Average       Calls\r\n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_IS_FINISIED                            _T(" : закончено!")
#else
#define RT_IS_FINISIED                            _T(" : finished!")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_SHELL_DEFECTIVE                        _T("Оболочка дефектная")
#define RT_SOME_SHELLS_DEFECTIVE                  _T("Некоторые оболочки дефектные")
#else
#define RT_SHELL_DEFECTIVE                        _T("The shell is defective")
#define RT_SOME_SHELLS_DEFECTIVE                  _T("The some shells is defective")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_DEFECTS_NOT_FOUND                      _T("Дефекты не обнаружены")
#else
#define RT_DEFECTS_NOT_FOUND                      _T("Defects were not found")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_BOUNDARY_EDGES_IN_MOST_PART            _T("Есть краевые ребра у наибольшей из частей")
#define RT_BOUNDARY_EDGES_IN_SOME_SHELLS_MOST_PART _T("Есть краевые ребра у наибольшей из частей некоторых оболочек")
#else
#define RT_BOUNDARY_EDGES_IN_MOST_PART            _T("There are boundary edges in the largest part")
#define RT_BOUNDARY_EDGES_IN_SOME_SHELLS_MOST_PART _T("There are boundary edges in the some shells largest part")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_NOT_BROKEN_INTO_PARTS                  _T("Но не распалась на части")
#define RT_SOME_SHELLS_NOT_BROKEN_INTO_PARTS      _T("Некоторые оболочки могут распасться на части, но не распались")
#else
#define RT_NOT_BROKEN_INTO_PARTS                  _T("But not decomposed")
#define RT_SOME_SHELLS_NOT_BROKEN_INTO_PARTS      _T("The some shells can be decomposed, but not decomposed")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_SHELL_BROKE_INTO_PARTS                 _T("Оболочка распалась на части")
#define RT_SOME_SHELLS_BROKE_INTO_PARTS           _T("Некоторые оболочки распалась на части")
#else
#define RT_SHELL_BROKE_INTO_PARTS                 _T("The shell has been decomposed")
#define RT_SOME_SHELLS_BROKE_INTO_PARTS           _T("The some shells has been decomposed")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_SHELL_CAN_BREAK_APART                  _T("Оболочка может распасться на части")
#define RT_SOME_SHELLS_CAN_BREAK_APART            _T("Некоторые болочки могут распасться на части")
#else
#define RT_SHELL_CAN_BREAK_APART                  _T("The shell can be decomposed")
#define RT_SOME_SHELLS_CAN_BREAK_APART            _T("The some shells can be decomposed")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_INEXACT_EDGES                          _T("Есть неточные ребра - %d")
#else
#define RT_INEXACT_EDGES                          _T("There are inexact edges - %d")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_INCORRECT_VERTICES_IN_EDGES            _T("В ребрах неправильно выставлены вершины")
#else
#define RT_INCORRECT_VERTICES_IN_EDGES            _T("There are incorrect vertices in edges")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_BREAK_CONNECT_LOOPS                    _T("Нарушена связность в циклах")
#else
#define RT_BREAK_CONNECT_LOOPS                    _T("Connectivity is broken in loops")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_SELFINTERSECTING_LOOPS                 _T("Найдены (само)пересечения циклов")
#else
#define RT_SELFINTERSECTING_LOOPS                 _T("There are (self)intersections of loops")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_WRONG_LOOPS_INTERPOSITION              _T("Некорректное взаимное расположение циклов")
#else
#define RT_WRONG_LOOPS_INTERPOSITION              _T("Wrong loops interposition")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_WRONG_AREA_SIGN                        _T("Неправильный знак площади")
#else
#define RT_WRONG_AREA_SIGN                        _T("Wrong area sign")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_VERTICES_WITH_TOLERANCE                _T("Есть вершины с толерантностью более %8.6f - %d")
#else
#define RT_VERTICES_WITH_TOLERANCE                _T("There are vertices with tolerance greater than %8.6f - %d")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_BOUNDARY_EDGES_OPEN_SHELL              _T("Граничные ребра незамкнутой оболочки")
#else
#define RT_BOUNDARY_EDGES_OPEN_SHELL              _T("Boundary edges of an open shell")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_BOUNDARY_EDGES                         _T("Есть краевые ребра")
#else
#define RT_BOUNDARY_EDGES                         _T("There are boundary edges")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_BOUNDARY_EDGES_COUNT                   _T("Есть краевые ребра - %d")
#else
#define RT_BOUNDARY_EDGES_COUNT                   _T("There are boundary edges - %d")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_BOUNDARY_EDGE                          _T("\n\rКраевое ребро")
#else
#define RT_BOUNDARY_EDGE                          _T("\n\rBoundary edge")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_INCORRECT_EDGES                        _T("Есть некорректные ребра")
#else
#define RT_INCORRECT_EDGES                        _T("There are incorrect edges")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_INVALID_REFERENCE_TO_FACE              _T("Некорректное указание на грани")
#else
#define RT_INVALID_REFERENCE_TO_FACE              _T("There is incorrect reference to faces")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_FACES_HAVE_COMMON_SUBSTRATE            _T("У граней есть общие подложки")
#else
#define RT_FACES_HAVE_COMMON_SUBSTRATE            _T("There are faces with common substrate")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_FACES_SAME_POINTERS                    _T("Есть одинаковые указатели на грани")
#else
#define RT_FACES_SAME_POINTERS                    _T("There are the same face pointers")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_EDGES_SAME_POINTERS                    _T("Есть одинаковые указатели на ребра")
#else
#define RT_EDGES_SAME_POINTERS                    _T("There are the same edge pointers")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_VERTICES_SAME_POINTERS                 _T("Есть одинаковые указатели на вершины")
#else
#define RT_VERTICES_SAME_POINTERS                 _T("There are the same vertex pointers")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_CHECK_OPEN_SHELL                       _T("Проверяем незамкнутую оболочку")
#else
#define RT_CHECK_OPEN_SHELL                       _T("An open shell is being checked")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_SOME_FACES_MERGED                      _T("Часть граней объединена")
#else
#define RT_SOME_FACES_MERGED                      _T("Some faces are merged")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define FACES_WERE_CORRECTED                      _T("Поверхности граней исправлены")
#else
#define FACES_WERE_CORRECTED                      _T("Surfaces were corrected")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_DO_NOT_DO                              _T("Ничего не сделали")
#else
#define RT_DO_NOT_DO                              _T("Nothing is done")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_ONE_PART                               _T("Все одна часть")
#else
#define RT_ONE_PART                               _T("There is only one part")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_NOT_CREATE_BODY_OF_PARTS               _T("Не смогли создать тело из частей")
#else
#define RT_NOT_CREATE_BODY_OF_PARTS               _T("Failed to create a solid from parts")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_BUT_CREATE_BODY_OF_PARTS               _T("Но разделили тело на части")
#else
#define RT_BUT_CREATE_BODY_OF_PARTS               _T("But created a solid from parts")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_BODY_IS_MADE_OF_PARTS                  _T("Тело состоит из отдельных частей")
#else
#define RT_BODY_IS_MADE_OF_PARTS                  _T("The solid consists from separate parts")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_REBUILDING_SHELL_WITH_FULL_COPY        _T("Перестроение тела с полным копированием")
#else
#define RT_REBUILDING_SHELL_WITH_FULL_COPY        _T("Rebuilding of the solid with full copying")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_REBUILDING_SHELL_WITH_SAME_SURF        _T("Перестроение тела на тех же поверхностях")
#else
#define RT_REBUILDING_SHELL_WITH_SAME_SURF        _T("Rebuilding of the solid on the same surfaces")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_REBUILDING_SHELL_WITH_CONST_FACES      _T("Перестроение тела с сохранением неизменных граней")
#else
#define RT_REBUILDING_SHELL_WITH_CONST_FACES      _T("Rebuilding of the solid with keeping unmodified faces")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_REBUILDING_SHELL_WITHOUT_COPYING_FACES _T("Перестроение тела без копирования граней")
#else
#define RT_REBUILDING_SHELL_WITHOUT_COPYING_FACES _T("Rebuilding the solid without copying the faces")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_REBUILDING_BODY                        _T("Перестроение тела")
#else
#define RT_REBUILDING_BODY                        _T("Rebuilding of the solid")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_REBUILDING_WIREFRAME_WITH_FULL_COPY    _T("Перестроение проволочного каркаса с полным копированием")
#else
#define RT_REBUILDING_WIREFRAME_WITH_FULL_COPY    _T("Rebuilding of the wire frame with full copying")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_REBUILDING_WIREFRAME_WITH_SAME_CURV    _T("Перестроение проволочного каркаса на тех же кривых")
#else
#define RT_REBUILDING_WIREFRAME_WITH_SAME_CURV    _T("Rebuilding of the wire frame on the same curves")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_REBUILDING_WIREFRAME_WITH_CONST_EDGES  _T("Перестроение проволочного каркаса с сохранением неизменных ребер")
#else
#define RT_REBUILDING_WIREFRAME_WITH_CONST_EDGES  _T("Rebuilding of the wire frame with keeping unmodified edges")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_REBUILDING_WIREFRAME_WITHOUT_COPYING_EDGES _T("Перестроение проволочного каркаса без копирования ребер")
#else
#define RT_REBUILDING_WIREFRAME_WITHOUT_COPYING_EDGES _T("Rebuilding of the wire frame without copying the edges")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_REBUILDING_WIREFRAME                   _T("Перестроение проволочного каркаса")
#else
#define RT_REBUILDING_WIREFRAME                   _T("Rebuilding of the wire frame")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_RADIUS_CURVE_TO_INDEX                  _T("Радиусу соответствует кривая с индексом: ")
#else
#define RT_RADIUS_CURVE_TO_INDEX                  _T("Radius corresponds to a curve with index: ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_INSERTED_VERTEX_IN_INDEX               _T("Вставлена вершина с индексом: ")
#else
#define RT_INSERTED_VERTEX_IN_INDEX               _T("Inserted vertex with index: ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_COMPOSITE_BODY_NOT_CREATED             _T("Составное тело не создано!")
#else
#define RT_COMPOSITE_BODY_NOT_CREATED             _T("A composite solid is not created!")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_NOT_SELECTED_BODY                      _T("Не выбрано тело")
#else
#define RT_NOT_SELECTED_BODY                      _T("A solid is not selected")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_FACES_TO_MODIFY_NOT_SELECTED           _T("Грани для модификации толщины не выбраны")
#else
#define RT_FACES_TO_MODIFY_NOT_SELECTED           _T("Faces for thickness modification are not selected")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_SELECT_FACES_TO_MODIFY                 _T("Выберите грани для модификации толщины")
#else
#define RT_SELECT_FACES_TO_MODIFY                 _T("Choose faces for thickness modification")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_OPENED_FACES_NOT_SELECTED              _T("Вскрываемые грани не выбраны")
#else
#define RT_OPENED_FACES_NOT_SELECTED              _T("Faces to open are not selected")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_SELECT_FACES                           _T("Выберите грани")
#else
#define RT_SELECT_FACES                           _T("Select faces")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_SELECT_OPENED_FACES                    _T("Выберите вскрываемые грани")
#else
#define RT_SELECT_OPENED_FACES                    _T("Select faces to open")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_SELECT_MEDIAN_FACES                    _T("Выберите грани между которыми строится срединная оболочка")
#else
#define RT_SELECT_MEDIAN_FACES                    _T("Select faces to create median shell")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_BODY_IS_NOT_BUILD                      _T("Тело не построено")
#else
#define RT_BODY_IS_NOT_BUILD                      _T("A solid is not built")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_UNKNOWN_ERROR                          _T("Неизвестная ошибка")
#else
#define RT_UNKNOWN_ERROR                          _T("Unknown error")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_CACHED_DATA_ERROR                      _T("Ошибка кэширования данных")
#else
#define RT_CACHED_DATA_ERROR                      _T("Cached data error")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_ITEM_LABEL_ERROR                       _T("Некоррекный объект или его метка")
#else
#define RT_ITEM_LABEL_ERROR                       _T("Damaged object or its wrong label")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_BASE_BBOX_ERROR                        _T("Ошибка габарита базового объекта")
#else
#define RT_BASE_BBOX_ERROR                        _T("Base bounding box error")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_BASE_BBOX_ERROR_REQUEST                _T("Проверять корректность габарита\nбазового объекта?")
#else
#define RT_BASE_BBOX_ERROR_REQUEST                _T("Do you need to check\nthe base bounding box?")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_TOPOLOGY_ERROR                         _T("Тело с нарушенной топологией")
#else
#define RT_TOPOLOGY_ERROR                         _T("The solid has bad topology")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_PARAMETER_ERROR                        _T("Неверные параметры")
#else
#define RT_PARAMETER_ERROR                        _T("Wrong parameters")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_NO_OBJECT_TO_DIRECTION                 _T("Нет объекта в заданном направлении")
#else
#define RT_NO_OBJECT_TO_DIRECTION                 _T("There is no object in the given direction")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_DONE_IN_PART                           _T("Выполнено частично")
#else
#define RT_DONE_IN_PART                           _T("Partially done")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_FACE_NOT_BELONG_BODY                   _T("Грань не принадлежит телу")
#else
#define RT_FACE_NOT_BELONG_BODY                   _T("Face does not belong to the solid")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_BODY_NOT_CUT                           _T("Тело не разрезано")
#else
#define RT_BODY_NOT_CUT                           _T("The solid is not cut")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_PROJECTIONS_IN_DIRECTION_NOT_FOUND     _T("Проекций по направлению не найдено")
#else
#define RT_PROJECTIONS_IN_DIRECTION_NOT_FOUND     _T("There are no projections in a direction found")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_FOUND_NORMAL_PROJECTION                _T("Найдена нормальная проекция")
#else
#define RT_FOUND_NORMAL_PROJECTION                _T("Normal projection is found")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_NOT_FOUND_NORMAL_PROJECTION            _T("Не найдена нормальная проекция")
#else
#define RT_NOT_FOUND_NORMAL_PROJECTION            _T("Normal projection is not found")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_SMALL_RANGE_SURFACE                    _T("Слишком маленький диапазон, будут взяты параметры с поверхности")
#else
#define RT_SMALL_RANGE_SURFACE                    _T("Too small range, parameters will be taken from the surface")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_SMALL_RANGE_CURVE                      _T("Слишком маленький диапазон, будут взяты параметры с кривой")
#else
#define RT_SMALL_RANGE_CURVE                      _T("Too small range, parameters will be taken from the curve")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_CHECK_PERMUTATION_PARAMETERS           _T("Будет проведена перестановка параметров т.к. tMin > tMax")
#else
#define RT_CHECK_PERMUTATION_PARAMETERS           _T("Parameters will be rearranged since tMin > tMax")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_PROJECTION_ERROR                       _T("Ошибка проецирования")
#else
#define RT_PROJECTION_ERROR                       _T("Projection error")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_CROSS_LINE_NOT_FOUND                   _T("Линия пересечения не найдена")
#else
#define RT_CROSS_LINE_NOT_FOUND                   _T("Intersection line is not found")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_CURVE_NOT_SELECTED                     _T("Кривая не выбрана")
#else
#define RT_CURVE_NOT_SELECTED                     _T("A curve is not selected")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_SELECT_CURVE_ERROR                     _T("Ошибка в функции селектирования кривой")
#else
#define RT_SELECT_CURVE_ERROR                     _T("Error in function of a curve selection")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_NOT_SELECTED_FACE_OR_SURFACE           _T("Не было выбрано ни одной грани или поверхности")
#else
#define RT_NOT_SELECTED_FACE_OR_SURFACE           _T("There is no face or surface selected")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_MISMATCH_IN_OPERATIONS_RESULTS         _T("Рассогласование в результатах операции")
#else
#define RT_MISMATCH_IN_OPERATIONS_RESULTS         _T("Mismatch in the operation results")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_PROJ_CURVE_ERROR                       _T("Ошибка при создании проекционной кривой")
#else
#define RT_PROJ_CURVE_ERROR                       _T("Error of a projection curve creation")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_PROJ_CURVE_OUT_FACES                   _T("Проекция кривой не попадает в область оперделения граней (поверхностей)")
#else
#define RT_PROJ_CURVE_OUT_FACES                   _T("Projection curve is out of the faces definition domain (of surfaces domain)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_PROJ_CURVES_CREATED                    _T("Проекционные кривые созданы")
#else
#define RT_PROJ_CURVES_CREATED                    _T("Projection curves are created")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_SURFACE_NOT_SELECTED                   _T("Не указана поверхность")
#else
#define RT_SURFACE_NOT_SELECTED                   _T("A surface is not specified")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_SELECTED_OBJECT_ERROR                  _T("Ошибка при селектировании объекта")
#else
#define RT_SELECTED_OBJECT_ERROR                  _T("Error of an object selection")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_TOO_FEW_POINTS                         _T("Слишком мало точек")
#else
#define RT_TOO_FEW_POINTS                         _T("Too few points")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_CURVE_NOT_CREATED                      _T("Не получилось создать кривую")
#else
#define RT_CURVE_NOT_CREATED                      _T("Failed to create a curve")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_MATES_CREATE_ERROR                     _T("Ошибка при задании сопряжения")
#else
#define RT_MATES_CREATE_ERROR                     _T("Error of a mating creation")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_SET_MATES                              _T("Задавать сопряжения?")
#else
#define RT_SET_MATES                              _T("To set mates?")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_SET_CURVE_MATE_TYPE                    _T("Укажите тип сопряжения")
#else
#define RT_SET_CURVE_MATE_TYPE                    _T("Set the type of mate")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_MATE_NOT_IMPLEMENTED                   _T("Сопряжение не реализовано")
#else
#define RT_MATE_NOT_IMPLEMENTED                   _T("Mating is not implemented")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_CONTOUR_CREATE_ERROR                   _T("Ошибка при создании контура")
#else
#define RT_CONTOUR_CREATE_ERROR                   _T("Error of a contour creation")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_CURVE_REMOVED_FROM_CONTOUR             _T("Кривая удалена из контура")
#else
#define RT_CURVE_REMOVED_FROM_CONTOUR             _T("A curve is removed from the contour")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_NOTHING_SELECTED                       _T("Ничего не выбрано")
#else
#define RT_NOTHING_SELECTED                       _T("Nothing is selected")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_CURVE2D_CREATE_ERROR                   _T("Не получилось создать двумерную кривую.")
#else
#define RT_CURVE2D_CREATE_ERROR                   _T("Failed to create a two-dimensional curve.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_CURVE3D_NOT_PLANAR                     _T("Пространственная кривая не плоская?")
#else
#define RT_CURVE3D_NOT_PLANAR                     _T("Is a spatial curve not planar?")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_MISMATCH_FUNCTION_PLANARITY            _T("Рассогласование функций планарности!")
#else
#define RT_MISMATCH_FUNCTION_PLANARITY            _T("Planarity functions mismatch!")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_INCORRECT_REGION                       _T("Некорректный регион")
#else
#define RT_INCORRECT_REGION                       _T("Incorrect region")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_OVERLAPPING_FACES                      _T("Потенциальное наложение граней")
#else
#define RT_OVERLAPPING_FACES                      _T("Potential overlapping of faces")
#endif

#if defined ( __NATIVE_LANGUAGE__ )
#define RT_CLOSED_MESH                              _T("Замкнутый полигональный объект")
#else
#define RT_CLOSED_MESH                              _T("A closed mesh")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_OPEN_MESH                              _T("Незамкнутый полигональный объект")
#else
#define RT_OPEN_MESH                              _T("An open mesh")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_BOUNDARY_EDGES_OPEN_MESH              _T("Граничные ребра незамкнутого полигонального объекта")
#else
#define RT_BOUNDARY_EDGES_OPEN_MESH              _T("Boundary edges of the open mesh")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_BOUNDARY_POLY_OPEN_MESH               _T("Граничные полигоны незамкнутого полигонального объекта")
#else
#define RT_BOUNDARY_POLY_OPEN_MESH               _T("Boundary polygons of the open mesh")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_INCONSINSTENT_POLY_MESH               _T("Обнаружена несогласованная ориентация полигонов в полигональном объекте")
#else
#define RT_INCONSINSTENT_POLY_MESH               _T("Found inconsistent orientation of polygons in the mesh")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_INCONSINSTENT_GRID_MESH               _T("Обнаружена несогласованная ориентация триангуляций в полигональном объекте")
#else
#define RT_INCONSINSTENT_GRID_MESH               _T("Found inconsistent orientation of grids in the mesh")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_DEGENERATE_POLY_MESH                  _T("Обнаружены вырожденные полигоны в полигональном объекте")
#else
#define RT_DEGENERATE_POLY_MESH                  _T("There are degenerated polygons in the mesh")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_SURFACE_TYPE                   _T("0 - плоскость, 1 - цилиндр, 2 - сфера, 3 - конус, 4 - тор, \n5 - NURBS, 6 - пов-ть вращения, 7 - пов-ть выдавливания, другое - автоопределение")
#else
#define IDS_ENTER_SURFACE_TYPE                   _T("0 - plane, 1 - cylinder, 2 - sphere, 3 - cone, 4 - torus, \n5 - NURBS, 6 - revolution, 7 - extrusion, other - autodetect")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_SURFACE_TYPE_CONSTRAINED       _T("0 - плоскость, 1 - цилиндр, 2 - сфера, 3 - конус, 4 - тор")
#else
#define IDS_ENTER_SURFACE_TYPE_CONSTRAINED       _T("0 - plane, 1 - cylinder, 2 - sphere, 3 - cone, 4 - torus")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_OUTLIER_REJECT_TYPE            _T("0 - нет, 1 - N Sigma, 2 - процент, 3 - максимальное отклонение")
#else
#define IDS_ENTER_OUTLIER_REJECT_TYPE            _T("0 - no, 1 - N Sigma, 2 - percent, 3 - max distance")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_SPHERE_X_COORDINATE            _T("Введите координату X центра сферы")
#else
#define IDS_ENTER_SPHERE_X_COORDINATE            _T("Enter the X coordinate of a sphere's center")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_SPHERE_Y_COORDINATE            _T("Введите координату Y центра сферы")
#else
#define IDS_ENTER_SPHERE_Y_COORDINATE            _T("Enter the Y coordinate of a sphere's center")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_SPHERE_Z_COORDINATE            _T("Введите координату Z центра сферы")
#else
#define IDS_ENTER_SPHERE_Z_COORDINATE            _T("Enter the Z coordinate of a sphere's center")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_AXIS_X_COORDINATE              _T("Введите координату X точки на оси объекта")
#else
#define IDS_ENTER_AXIS_X_COORDINATE              _T("Enter the X coordinate of a point of an object's axis")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_AXIS_Y_COORDINATE              _T("Введите координату Y точки на оси объекта")
#else
#define IDS_ENTER_AXIS_Y_COORDINATE              _T("Enter the Y coordinate of a point of an object's axis")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_PHI_PARAMETER                  _T("Введите угол между проекцией оси объекта на плоскость OXY и осью OX заданной СК из промежутка [0; 360)")
#else
#define IDS_ENTER_PHI_PARAMETER                  _T("Enter the angle between the projection of an object's axis onto the OXY plane and the OX axis of a given CS from [0; 360)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_THETA_PARAMETER                _T("Введите угол между осью объекта и осью OZ заданной СК из промежутка [0; 90]")
#else
#define IDS_ENTER_THETA_PARAMETER                _T("Enter the angle between an object's axis and the OZ axis of a given CS from [0; 90]")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_CONSTRAINT_SIZE1               _T("Укажите первый фиксированный размер > 0 (если <= 0 - размер не фиксирован)")
#else
#define IDS_ENTER_CONSTRAINT_SIZE1               _T("Input the first size constraint > 0 (if <= 0 - the size is not fixed)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_CONSTRAINT_SIZE2               _T("Укажите второй фиксированный размер > 0 (если <= 0 - размер не фиксирован)")
#else
#define IDS_ENTER_CONSTRAINT_SIZE2               _T("Input the second size constraint > 0 (if <= 0 - the size is not fixed)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_DIST_PARAMETER                 _T("Укажите расстояние от оси объекта до оси OZ заданной СК > 0 ( если <= 0 - расстояние не фиксировано)")
#else
#define IDS_ENTER_DIST_PARAMETER                 _T("Input the distance between an object's axis and the OZ axis of a given CS > 0 (if <= 0 - distance is not fixed)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_BEGIN_OR_END                         _T("Удалить начало или конец ребра? (ДА - начало, НЕТ - конец)")
#else
#define IDS_BEGIN_OR_END                         _T("Delete start or the end of an edge? (YES - start, NO - end)")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_SUFRACE_SELFINT_MODE          _T("Опция поиска самопересечений : 0 - авторежим, 1 - сегментированное пересечение, 2 - вдоль UV линий")
#else
#define IDS_ENTER_SUFRACE_SELFINT_MODE          _T("Surface self-intersection mode : 0 - auto mode, 2 - segmented intersection, 2 - along UV-lines")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_ENTER_SHELL_SELFINT_MODE             _T("Опция поиска самопересечений : 0 - авторежим, 1 - самопересечение граней, 2 - пересечение различных граней")
#else
#define IDS_ENTER_SHELL_SELFINT_MODE             _T("Shell self-intersection mode : 0 - auto mode, 1 - self-intersection of faces, 2 - intersection of different faces")
#endif

#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_NUMBER_UQINUE_SURFACE                _T("Количество уникальных поверхностей")
#else
#define IDS_NUMBER_UQINUE_SURFACE                _T("Number of unique surfaces")
#endif 

#if defined ( __NATIVE_LANGUAGE__ )
#define THREAD_TEST_TITLE                         _T("Построение полигональных объектов")
#define THREAD_PROJECTION_TEST_TITLE              _T("Проекция базовых точек")
#define THREAD_PROJECTION_TEST_FAILED             _T("Проекции базовых точек не совпадают")
#define THREAD_TEST_SERIAL                        _T("Последовательные вычисления")
#define THREAD_TEST_MULTITHREAD                   _T("Параллельные вычисления")
#define THREAD_PROJECTION_POINTS                  _T("Количество базовых точек: ")
#define THREAD_PROJECTION_FACES                   _T("Количество граней: ")
#define THREAD_PROJECTION_SURFACES                _T("Количество поверхностей: ")
#define THREAD_PROJECTION_FORMAT1                 _T("%d\n")
#define THREAD_PROJECTION_FORMAT2                 _T("%d\n----\n")
#define THREAD_PROJECTION_FORMAT3                 _T("\n----\n")
#define THREAD_MESH_TEST_FAILED                   _T("ОШИБКА! Вычисленные полигональные объекты не совпадают")
#define THREAD_MESH_TEST_COPY_FAILED              _T("Не удалось создать дубликаты объектов")
#define THREAD_MESH_TEST_NUM                      _T("Количество полигональных объектов: ")
#define MUTLTHREADING_MODES                       _T("Режимы многопоточности")
#define MUTLTHREADING_MODE_OFF                    _T("Многопоточность отключена - 0\n")
#define MUTLTHREADING_MODE_STANDARD               _T("Стандартная многопоточность ядра - 1 \n\t(параллельная обработка независимых данных)\n")
#define MUTLTHREADING_MODE_ITEMS                  _T("Потокобезопасный доступ к объектам - 2 \n")
#define MUTLTHREADING_MODE_PARALLEL_ITEMS         _T("Расширенная многопоточность ядра - 3\n\t(параллельная обработка зависимых объектов)\n")
#define MUTLTHREADING_MODE_MAX                    _T("Максимальная многопоточность - 31\n")
#define MUTLTHREADING_MODE_OFF_EX                 _T("Многопоточность отключена")
#define MUTLTHREADING_MODE_STANDARD_EX            _T("Стандартная многопоточность")
#define MUTLTHREADING_MODE_ITEMS_EX               _T("Потокобезопасный доступ к объектам")
#define MUTLTHREADING_MODE_PARALLEL_ITEMS_EX      _T("Расширенная многопоточность")
#define MUTLTHREADING_MODE_MAX_EX                 _T("Максимальная многопоточность")
#define MUTLTHREADING_MODE_INCORRECT              _T("Некорректный режим\n Многопоточность отключена")
#define INTERNAL_MUTLTHREADING_IS_OFF             _T("Внутренняя многопоточность отключена!\n")
#define INTERNAL_MUTLTHREADING_IS_ON              _T("Внутренняя многопоточность включена!\n")
#define CONFID_FILE_UPDATED                       _T("Опция обновлена из файла конфигурации")
#define CONFID_FILE_NOT_FOUND                     _T(" не был найден")
#define OPENMP_IS_OFF                             _T("=== Внимание! Многопоточность OpenMP отключена! ===\n")
#define SYSTEM_MUTEX_ON                           _T("=== Внимание! Используется системный мютекс! ===\n")
#define PARALLEL_CYCLE_OMP                        _T("Параллельный цикл OpenMP" )
#define PARALLEL_CYCLE_STD_THREAD                 _T("Параллельный цикл std::thread")
#define SEQUENTIAL_CALCULATION                    _T("Последовательные вычисления: ")
#define PARALLEL_CALCULATION                      _T("Параллельные вычисления: ")
#define STANDARD_CALCULATION                      _T("Стандартные вычисления: ")
#define PARALLEL_CYCLE_CALCULATION                _T("Вычисления в параллельном цикле: ")
#define ONE_FUNCTION_ALL_VIEWS                    _T("Одна функция для всех видов: ")
#define FOUR_VIEWS_CREATION                       _T("Создание четырех видов")
#define VIEW_PROJECTION_CREATION                  _T("Создание проекции вида")
#define FIT_FIRST_LOADING_ON                      _T("Адаптация модели включена")
#define FIT_FIRST_LOADING_OFF                     _T("Адаптация модели выключена")

#else
#define THREAD_TEST_TITLE                         _T("Mesh calculation")
#define THREAD_PROJECTION_TEST_TITLE              _T("Basic points projection")
#define THREAD_PROJECTION_TEST_FAILED             _T("Basic points projections do not match")
#define THREAD_TEST_SERIAL                        _T("Serial calculations")
#define THREAD_TEST_MULTITHREAD                   _T("Multithreaded calculations")
#define THREAD_PROJECTION_POINTS                  _T("Number of basic points: ")
#define THREAD_PROJECTION_FACES                   _T("Number of faces: ")
#define THREAD_PROJECTION_SURFACES                _T("Number of surfaces: ")
#define THREAD_PROJECTION_FORMAT1                 _T("%d\n")
#define THREAD_PROJECTION_FORMAT2                 _T("%d\n----\n")
#define THREAD_PROJECTION_FORMAT3                 _T("\n----\n")
#define THREAD_MESH_TEST_FAILED                   _T("ERROR! Calculated meshes do not match")
#define THREAD_MESH_TEST_COPY_FAILED              _T("Failed to create sets of duplicates")
#define THREAD_MESH_TEST_NUM                      _T("Number of meshes: ")
#define MUTLTHREADING_MODES                       _T("Multithreading modes")
#define MUTLTHREADING_MODE_OFF                    _T("Multithreading is off - 0\n")
#define MUTLTHREADING_MODE_STANDARD               _T("Standard multithreading mode - 1 \n\t(parallel processing of independent objects)\n")
#define MUTLTHREADING_MODE_ITEMS                  _T("Thread-safe access to items - 2\n")
#define MUTLTHREADING_MODE_PARALLEL_ITEMS         _T("Extended multithreading mode - 3\n\t(parallel processing of dependent objects)\n")
#define MUTLTHREADING_MODE_MAX                    _T("Maximal multithreading mode - 31")
#define MUTLTHREADING_MODE_OFF_EX                 _T("Multithreading is off")
#define MUTLTHREADING_MODE_STANDARD_EX            _T("Standard multithreading mode")
#define MUTLTHREADING_MODE_ITEMS_EX               _T("Thread-safe access to items")
#define MUTLTHREADING_MODE_PARALLEL_ITEMS_EX      _T("Extended multithreading mode")
#define MUTLTHREADING_MODE_MAX_EX                 _T("Maximal multithreading mode")
#define MUTLTHREADING_MODE_INCORRECT              _T("Wrong mode\n Multithreading is off")
#define INTERNAL_MUTLTHREADING_IS_OFF             _T("Internal multithreading is OFF!\n")
#define INTERNAL_MUTLTHREADING_IS_ON              _T("Internal multithreading is ON!\n")
#define CONFID_FILE_UPDATED                       _T("Options were updated from the config file")
#define CONFID_FILE_NOT_FOUND                     _T(" was not found")
#define OPENMP_IS_OFF                             _T("=== Warning! OpenMP multithreading is OFF! ===\n")
#define SYSTEM_MUTEX_ON                           _T("=== Warning! Using system mutex! ===\n")
#define PARALLEL_CYCLE_OMP                        _T("Parallel cycle using OpenMP")
#define PARALLEL_CYCLE_STD_THREAD                 _T("Parallel cycle using std::thread")
#define SEQUENTIAL_CALCULATION                    _T("Sequential calculations: ")
#define PARALLEL_CALCULATION                      _T("Parallel calculations: ")
#define STANDARD_CALCULATION                      _T("Standard calculations: ")
#define PARALLEL_CYCLE_CALCULATION                _T("Calculation in parallel cycle: ")
#define ONE_FUNCTION_ALL_VIEWS                    _T("One function for all views: ")
#define FOUR_VIEWS_CREATION                       _T("Creation of 4 views")
#define VIEW_PROJECTION_CREATION                  _T("Creation of view projection")
#define FIT_FIRST_LOADING_ON                      _T("Enable model adaptation")
#define FIT_FIRST_LOADING_OFF                     _T("Disable model adaptation")
#endif

#if defined ( __NATIVE_LANGUAGE__ )
#define RT_LOC_UNDEF    _T("Не определялось")
#define RT_LOC_UNKNOWN  _T("Не получилось определить")
#define RT_LOC_OUT      _T("Вне объекта")
#define RT_LOC_ON       _T("На объекте")
#define RT_LOC_IN       _T("Внутри объекта")
#define RT_LOC_BY       _T("Условно внутри объекта")
#else
#define RT_LOC_UNDEF    _T("Not defined")
#define RT_LOC_UNKNOWN  _T("Failed to define")
#define RT_LOC_OUT      _T("Out the object")
#define RT_LOC_ON       _T("On the object")
#define RT_LOC_IN       _T("Inside the object")
#define RT_LOC_BY       _T("Conditionally inside the object")
#endif

#if defined ( __NATIVE_LANGUAGE__ )
#define CURVE_AS_WIREFRAME                         _T("Создавать кривую в виде проволочного каркаса?")
#else
#define CURVE_AS_WIREFRAME                        _T("Create a curve as a wire frame?")
#endif

#if defined ( __NATIVE_LANGUAGE__ )
#define POINTSET_AS_POINTFRAME                    _T("Набор точек в виде каркаса?")
#else
#define POINTSET_AS_POINTFRAME                    _T("Set of points as a point frame?")
#endif

#if defined ( __NATIVE_LANGUAGE__ )
#define RT_MAKE_USER_COMAND_0                     _T("Допишите свой код в функцию MakeUserCommand0    ")
#else
#define RT_MAKE_USER_COMAND_0                     _T("Add your code to function MakeUserCommand0    ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_MAKE_USER_COMAND_1                     _T("Допишите свой код в функцию MakeUserCommand1    ")
#else
#define RT_MAKE_USER_COMAND_1                     _T("Add your code to function MakeUserCommand1    ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_MAKE_USER_COMAND_2                     _T("Допишите свой код в функцию MakeUserCommand2    ")
#else
#define RT_MAKE_USER_COMAND_2                     _T("Add your code to function MakeUserCommand2    ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_MAKE_USER_COMAND_3                     _T("Допишите свой код в функцию MakeUserCommand3    ")
#else
#define RT_MAKE_USER_COMAND_3                     _T("Add your code to function MakeUserCommand3    ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_MAKE_USER_COMAND_4                     _T("Допишите свой код в функцию MakeUserCommand4    ")
#else
#define RT_MAKE_USER_COMAND_4                     _T("Add your code to function MakeUserCommand4    ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_MAKE_USER_COMAND_5                     _T("Допишите свой код в функцию MakeUserCommand5    ")
#else
#define RT_MAKE_USER_COMAND_5                     _T("Add your code to function MakeUserCommand5    ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_MAKE_USER_COMAND_6                     _T("Допишите свой код в функцию MakeUserCommand6    ")
#else
#define RT_MAKE_USER_COMAND_6                     _T("Add your code to function MakeUserCommand6    ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_MAKE_USER_COMAND_7                     _T("Допишите свой код в функцию MakeUserCommand7    ")
#else
#define RT_MAKE_USER_COMAND_7                     _T("Add your code to function MakeUserCommand7    ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_MAKE_USER_COMAND_8                     _T("Допишите свой код в функцию MakeUserCommand8    ")
#else
#define RT_MAKE_USER_COMAND_8                     _T("Add your code to function MakeUserCommand8    ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_MAKE_USER_COMAND_9                     _T("Допишите свой код в функцию MakeUserCommand9    ")
#else
#define RT_MAKE_USER_COMAND_9                     _T("Add your code to function MakeUserCommand9    ")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_LICENSE_DISABLED                       _T("Ключ лицензии не найден в %s\nДля активации C3D необходимо\nдобавить в файл строку содержащую ключ лицензии,\nстроку содержащую подпись лицензии или вызвать\nкоманду меню Помощь/Лицензионный ключ")
#else
#define RT_LICENSE_DISABLED                       _T("The license key not found in the %s\nTo activate C3D please add a line with a license key and\na line with a license signature to the file\nor call Help/License Key menu command")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_LICENSE                                _T("Отсутствует лицензия")
#else
#define RT_LICENSE                                _T("License not found")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_LICENSE_INVALID_KEY                    _T("Недействительный лицензионный ключ")
#else
#define RT_LICENSE_INVALID_KEY                    _T("Invalid license key")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_SAVING_LICENCE_ERROR                   _T("Ошибка записи")
#else
#define RT_SAVING_LICENCE_ERROR                   _T("Writing error")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_SAVING_LICENCE_ERROR_DESCRIPTION       _T("Ошибка записи. Ключ лицензии не сохранен.")
#else
#define RT_SAVING_LICENCE_ERROR_DESCRIPTION       _T("Writing error. The license key has not been saved.")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define RT_LICENSE_STATUS                         _T("Статус лицензии")
#else
#define RT_LICENSE_STATUS                         _T("License status")
#endif



////////////////////////////////////////////////////////////////////////////////
//
// \ru Горячие клавиши \en Hot keys
//
////////////////////////////////////////////////////////////////////////////////


#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_HOTKEYS                       _T("Горячие клавиши")
#else
#define IDS_HOTKEYS                       _T("Hot keys")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_KEYBOARD                      _T("  КЛАВИАТУРА : \n")
#else
#define IDS_KEYBOARD                      _T("  KEYBOARD : \n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_KEYS_ARROWS                   _T("- <Клавиши-стрелки> - смещение точки обзора\n")
#else
#define IDS_KEYS_ARROWS                   _T("- <Arrow keys> - shifting of the view point\n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_CTRL_KEYS_ARROWS              _T("- <Ctrl>  + <Клавиши-стрелки> - вращение точки обзора\n")
#else
#define IDS_CTRL_KEYS_ARROWS              _T("- <Ctrl>  + <Arrow keys> - rotation of the view point\n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SHIFT_KEYS_ARROWS             _T("- <Shift> + <Клавиши-стрелки> - вращение точки обзора в плоскости экрана\n")
#else
#define IDS_SHIFT_KEYS_ARROWS             _T("- <Shift> + <Arrow keys> - rotation of the view point in the plane of screen\n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_CTRL_O                        _T("- <Ctrl> + <O> - открыть файл\n")
#else
#define IDS_CTRL_O                        _T("- <Ctrl> + <O> - open file\n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_CTRL_S                        _T("- <Ctrl> + <S> - сохранить файл\n")
#else
#define IDS_CTRL_S                        _T("- <Ctrl> + <S> - save file\n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_TAB_LEFT_MOUSE_BUTTON         _T("- <Tab> - левая  кнопка мыши \n")
#else
#define IDS_TAB_LEFT_MOUSE_BUTTON         _T("- <Tab> - left mouse button\n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_INSERT_RIGHT_MOUSE_BUTTON     _T("- <Insert> - правая кнопка мыши \n")
#else
#define IDS_INSERT_RIGHT_MOUSE_BUTTON     _T("- <Insert> - right mouse button \n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_BACKSPACE                     _T("- <BackSpace> - отмена предыдущей команды \n")
#else
#define IDS_BACKSPACE                     _T("- <BackSpace> - previous command cancellation \n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_F2                            _T("- <F2> - записать модель активного окна \n")
#else
#define IDS_F2                            _T("- <F2> - write a model by active window \n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_F3                            _T("- <F3> - прочитать модель в активное окно \n")
#else
#define IDS_F3                            _T("- <F3> - read a model by active window \n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_F4                            _T("- <F4> - закраска модели с помощью трассировки лучей \n")
#else
#define IDS_F4                            _T("- <F4> - rendering of the model by ray tracing \n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SHIFT_F4                      _T("- <Shift> + <F4> - настроить цвет объенкта \n")
#else
#define IDS_SHIFT_F4                      _T("- <Shift> + <F4> - setup object color \n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_F5                            _T("- <F5> - развернуть/восстановить активное окно \n")
#else
#define IDS_F5                            _T("- <F5> - expand/restore the active window \n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SHIFT_F5                      _T("- <Shift> + <F5> - сдвинуть конец дерева \n")
#else
#define IDS_SHIFT_F5                      _T("- <Shift> + <F5> - move end of build log \n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_F6                            _T("- <F6> - поменять активное окно \n")
#else
#define IDS_F6                            _T("- <F6> - change the active window \n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SHIFT_F6                      _T("- <Shift> + <F6> - выполнить пропущенную олперацию \n")
#else
#define IDS_SHIFT_F6                      _T("- <Shift> + <F6> - build an omitted operation \n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_F7                            _T("- <F7> - повторить предыдущую команду \n")
#else
#define IDS_F7                            _T("- <F7> - repeat the previous command \n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SHIFT_F7                      _T("- <Shift> + <F7> - перестроить тело \n")
#else
#define IDS_SHIFT_F7                      _T("- <Shift> + <F7> - rebuild solid or shell \n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_F8                            _T("- <F8> - отображение модели с помощью GDI или OGL \n")
#else
#define IDS_F8                            _T("- <F8> - rendering of a model using GDI or OGL \n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_F9                            _T("- <F9> - перерисовать с учетом масштаба\n")
#else
#define IDS_F9                            _T("- <F9> - redraw using a scale\n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_F11                           _T("- <F11> - перспективное отображение\n")
#else
#define IDS_F11                           _T("- <F11> - perspective drawing \n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_F12                           _T("- <F12> - не занятно\n")
#else
#define IDS_F12                           _T("- <F12> - no command\n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_NUMPAD_ASTERISK               _T("- NumPad <*> - центрировать \n")
#else
#define IDS_NUMPAD_ASTERISK               _T("- NumPad <*> - align \n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_NUMPAD_MINUS                  _T("- NumPad <-> - уменьшить масштаб \n")
#else
#define IDS_NUMPAD_MINUS                  _T("- NumPad <-> - decrease the scale \n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_NUMPAD_PLUS                   _T("- NumPad <+> - увеличить масштаб \n")
#else
#define IDS_NUMPAD_PLUS                   _T("- NumPad <+> - increase the scale \n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_CTRL_NUMPAD_ASTERISK          _T("- <Ctrl> + NumPad <*> - центрировать c подгонкой масштаба \n")
#else
#define IDS_CTRL_NUMPAD_ASTERISK          _T("- <Ctrl> + NumPad <*> - align with the scale fitting \n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_CTRL_F8                       _T("- <Ctrl> + <F8> - отображение модели с помощью OGL (каркас или закраска)\n")
#else
#define IDS_CTRL_F8                       _T("- <Ctrl> + <F8> - rendering of a model using OGL (frame or shading)\n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SHIFT_F8                      _T("- <Shift> + <F8> - закраска модели с помощью трассировки лучей\n")
#else
#define IDS_SHIFT_F8                      _T("- <Shift> + <F8> - rendering of the model by ray tracing\n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_CAPSLOCK_F8                   _T("- <CapsLock> + <F8> - закраска модели с помощью упрощенной триангуляции и OGL\n")
#else
#define IDS_CAPSLOCK_F8                   _T("- <CapsLock> + <F8> - rendering of the model using simplified triangulation and OGL\n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_MOUSE                         _T("  МЫШЬ : \n")
#else
#define IDS_MOUSE                         _T("  MOUSE : \n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_MOUSE_MOVE                    _T("- Движение мыши: \n")
#else
#define IDS_MOUSE_MOVE                    _T("- Mouse move: \n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SHIFT                         _T("- + <Shift> - подсветка элементов объекта\n")
#else
#define IDS_SHIFT                         _T("- + <Shift> - object's elements highlighting\n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_CENTRE_MOUSE_KEY              _T("- + Нажать среднюю клавишу - вращение точки обзора\n")
#else
#define IDS_CENTRE_MOUSE_KEY              _T("- + Press a central button - rotation of a view point\n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SHIFT_CENTRE_MOUSE_KEY        _T("- + <Shift> + Нажать среднюю клавишу - смещение точки обзора\n")
#else
#define IDS_SHIFT_CENTRE_MOUSE_KEY        _T("- + <Shift> + Press a central button - shifting of a view point\n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_LEFT_MOUSE_KEY                _T("- Нажать левую клавишу - выделить объект\n")
#else
#define IDS_LEFT_MOUSE_KEY                _T("- Press left button - highlight the object\n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_SHIFT_LEFT_MOUSE_KEY          _T("- <Shift> + Нажать левую клавишу - выделить элемент объекта\n")
#else
#define IDS_SHIFT_LEFT_MOUSE_KEY          _T("- <Shift> + Press left button - highlight the object element\n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_DBL_LEFT_MOUSE_KEY            _T("- Двойное нажатие левой клавиши - режим редактирования объекта\n")
#else
#define IDS_DBL_LEFT_MOUSE_KEY            _T("- Double click of left button - the object editing mode\n")
#endif
#if defined ( __NATIVE_LANGUAGE__ )
#define IDS_RIGHT_MOUSE_KEY               _T("- Нажать правую клавишу - свойства объекта\n")
#else
#define IDS_RIGHT_MOUSE_KEY               _T("- Press right button - object's properties\n")
#endif


#ifndef __USE_QT__

  #if defined ( __NATIVE_LANGUAGE__ )
    #define C3D_EXT_INFO     _T("Модели(*.C3D)\0*.c3d\0\0")
    #define MAT_EXT_INFO     _T("Объекта(*.MAT)\0*.mat\0\0")
    #define BMP_EXT_INFO     _T("Объекта(*.BMP)\0*.bmp\0\0")
    #define SAT_EXT_INFO     _T("ACIS(*.SAT)\0*.sat\0\0")
    #define SAB_EXT_INFO     _T("ACIS(*.SAB)\0*.sab\0\0")
    #define IGS_EXT_INFO     _T("IGES(*.IGS,*IGES)\0*.igs;*.iges\0\0")
    #define X_T_EXT_INFO     _T("PARASOLID(*.X_T)\0*.x_t;*.x_b\0\0")
    #define X_B_EXT_INFO     _T("PARASOLID(*.X_B)\0*.x_b\0\0")
    #define STP_EXT_INFO     _T("STEP(*.STP,*STEP)\0*.stp;*.step\0\0")
    #define JT_EXT_INFO      _T("JT(*.JT)\0*.JT\0\0")
    #define STL_EXT_INFO     _T("STL(*.STL)\0*.stl\0\0")
    #define VRML_EXT_INFO    _T("WRL(*.WRL)\0*.wrl\0\0")
    #define OBJ_EXT_INFO     _T("OBJ(*.OBJ)\0*.obj\0\0")
    #define THREEMF_EXT_INFO _T("3MF(*.3MF)\0*.3mf\0\0")
    #define GRDECL_EXT_INFO  _T("GRDECL(*.GRDECL)\0*.grdecl\0\0")
    #define ASCIIPC_EXT_INFO _T("ASCII Point Cloud(*.TXT,*ASC,*XYZ)\0*.txt;*.asc;*.xyz\0\0")
  #else
    #define C3D_EXT_INFO     _T("Models(*.C3D)\0*.c3d\0\0")
    #define MAT_EXT_INFO     _T("Of object(*.MAT)\0*.mat\0\0")
    #define BMP_EXT_INFO     _T("Of object(*.BMP)\0*.bmp\0\0")
    #define SAT_EXT_INFO     _T("ACIS(*.SAT)\0*.sat\0\0")
    #define SAB_EXT_INFO     _T("ACIS(*.SAB)\0*.sab\0\0")
    #define IGS_EXT_INFO     _T("IGES(*.IGS,*IGES)\0*.igs;*.iges\0\0")
    #define X_T_EXT_INFO     _T("PARASOLID(*.X_T)\0*.x_t;*.x_b\0\0")
    #define X_B_EXT_INFO     _T("PARASOLID(*.X_B)\0*.x_b\0\0")
    #define STP_EXT_INFO     _T("STEP(*.STP,*STEP)\0*.stp;*.step\0\0")
    #define JT_EXT_INFO      _T("JT(*.JT)\0*.JT\0\0")
    #define STL_EXT_INFO     _T("STL(*.STL)\0*.stl\0\0")
    #define VRML_EXT_INFO    _T("WRL(*.WRL)\0*.wrl\0\0")
    #define OBJ_EXT_INFO     _T("OBJ(*.OBJ)\0*.obj\0\0")
    #define THREEMF_EXT_INFO _T("3MF(*.3MF)\0*.3mf\0\0")
    #define GRDECL_EXT_INFO  _T("GRDECL(*.GRDECL)\0*.grdecl\0\0")  
    #define ASCIIPC_EXT_INFO _T("ASCII Point Cloud(*.TXT,*ASC,*XYZ)\0*.txt;*.asc;*.xyz\0\0")
  #endif

  #if defined ( __NATIVE_LANGUAGE__ )
    #define SAVE_EXT_INFO \
      _T("Модели(*.C3D)\0*.c3d\0ACIS(*.SAT)\0*.sat\0IGES(*.IGS,*IGES)\0*.igs;*.iges\0PARASOLID(*.X_T)\0*.x_t;*.x_b\0STEP(*.STP,*STEP)\0*.stp;*.step\0JT(*.jt)\0*.jt\0STL(*.STL)\0*.stl\0WRL(*.WRL)\0*.wrl\0GRDECL(*.GRDECL)\0*.grdecl\0ASCII Point Cloud(*.TXT,*ASC,*XYZ)\0*.txt;*.asc;*.xyz\0\x33MF(*.3MF)\0*.3mf\0ACIS(*.SAB)\0*.sab\0PARASOLID(*.X_B)\0*.x_b\0")
    #else
    #define SAVE_EXT_INFO \
      _T("Models(*.C3D)\0*.c3d\0ACIS(*.SAT)\0*.sat\0IGES(*.IGS,*IGES)\0*.igs;*.iges\0PARASOLID(*.X_T)\0*.x_t;*.x_b\0STEP(*.STP,*STEP)\0*.stp;*.step\0JT(*.jt)\0*.jt\0STL(*.STL)\0*.stl\0WRL(*.WRL)\0*.wrl\0GRDECL(*.GRDECL)\0*.grdecl\0ASCII Point Cloud(*.TXT,*ASC,*XYZ)\0*.txt;*.asc;*.xyz\0ThreeMF(*.3MF)\0*.3mf\0ACIS(*.SAB)\0*.sab\0PARASOLID(*.X_B)\0*.x_b\0")
  #endif

  // номера в списке фильтров
  #define SAVE_NUMBER_C3D    1
  #define SAVE_NUMBER_SAT    2
  #define SAVE_NUMBER_IGS    3
  #define SAVE_NUMBER_X_T    4
  #define SAVE_NUMBER_STP    5
  #define SAVE_NUMBER_JT     6
  #define SAVE_NUMBER_STL    7
  #define SAVE_NUMBER_WRL    8
  #define SAVE_NUMBER_GRDECL 9
  #define SAVE_NUMBER_ASCII  10
  #define SAVE_NUMBER_3MF    11
  #define SAVE_NUMBER_SAB    12
  #define SAVE_NUMBER_X_B    13
  #define SAVE_NUMBER_OBJ    14  


#else // __USE_QT__

  #if defined ( __NATIVE_LANGUAGE__ )
    #define C3D_EXT_INFO     _T("Модели (*.c3d);;Все файлы (*.*)")
    #define MAT_EXT_INFO     _T("Объекта (*.mat);;Все файлы (*.*)")
    #define BMP_EXT_INFO     _T("Объекта (*.bmp);;Все файлы (*.*)")
    #define SAT_EXT_INFO     _T("ACIS (*.sat);;Все файлы (*.*)")
    #define SAB_EXT_INFO     _T("ACIS (*.sab);;Все файлы (*.*)")
    #define IGS_EXT_INFO     _T("IGES (*.igs);;Все файлы (*.*)")
    #define X_T_EXT_INFO     _T("PARASOLID (*.x_t);;Все файлы (*.*)")
    #define X_B_EXT_INFO     _T("PARASOLID (*.x_b);;Все файлы (*.*)")
    #define STP_EXT_INFO     _T("STEP (*.stp);;Все файлы (*.*)")
    #define JT_EXT_INFO      _T("JT(*.JT)\0*.jt\0\0")
    #define STL_EXT_INFO     _T("STL(*.STL)\0*.stl\0\0")
    #define VRML_EXT_INFO    _T("WRL(*.WRL)\0*.wrl\0\0")
    #define OBJ_EXT_INFO     _T("OBJ(*.OBJ)\0*.obj\0\0")
    #define THREEMF_EXT_INFO _T("3MF(*.3MF)\0*.3mf\0\0")
    #define GRDECL_EXT_INFO  _T("GRDECL(*.GRDECL)\0*.grdecl\0\0")
    #define ASCIIPC_EXT_INFO _T("ASCII Point Cloud(*.TXT,*ASC,*XYZ)\0*.txt;*.asc;*.xyz\0\0")
  #else
    #define C3D_EXT_INFO     _T("Models (*.c3d);;All files (*.*)")
    #define MAT_EXT_INFO     _T("Of object (*.mat);;All files (*.*)")
    #define BMP_EXT_INFO     _T("Of object (*.bmp);;All files (*.*)")
    #define SAT_EXT_INFO     _T("ACIS (*.sat);;All files (*.*)")
    #define SAB_EXT_INFO     _T("ACIS (*.sab);;All files (*.*)")
    #define IGS_EXT_INFO     _T("IGES (*.igs);;All files (*.*)")
    #define X_T_EXT_INFO     _T("PARASOLID (*.x_t);;All files (*.*)")
    #define X_B_EXT_INFO     _T("PARASOLID (*.x_b);;All files (*.*)")
    #define STP_EXT_INFO     _T("STEP (*.stp);;All files (*.*)")
    #define JT_EXT_INFO      _T("JT(*.JT)\0*.jt\0\0")
    #define STL_EXT_INFO     _T("STL(*.STL)\0*.stl\0\0")
    #define VRML_EXT_INFO    _T("WRL(*.WRL)\0*.wrl\0\0")
    #define OBJ_EXT_INFO     _T("OBJ(*.OBJ)\0*.obj\0\0")
    #define THREEMF_EXT_INFO _T("3MF(*.3MF)\0*.3mf\0\0")
    #define GRDECL_EXT_INFO  _T("GRDECL(*.GRDECL)\0*.grdecl\0\0")
    #define ASCIIPC_EXT_INFO _T("ASCII Point Cloud(*.TXT,*ASC,*XYZ)\0*.txt;*.asc;*.xyz\0\0")
  #endif

#endif // __USE_QT__


#if defined ( __NATIVE_LANGUAGE__ )
#define SELECT_FILE_INFO  _T("Открыть файл\0")
#define WRITE_FILE_INFO   _T("Сохранить файл\0")
#else
#define SELECT_FILE_INFO  _T("Open\0")
#define WRITE_FILE_INFO   _T("Save\0")
#endif


#ifndef __USE_QT__
#ifdef C3D_DEBUG
#if defined ( __NATIVE_LANGUAGE__ )
  #ifdef PLATFORM_64
    #define TITLE_MAIN_WND   _T("Тестирование геометрического ядра C3D [Visual C++ Debug (x64)]")
  #else
    #define TITLE_MAIN_WND   _T("Тестирование геометрического ядра C3D [Visual C++ Debug (x86)]")
  #endif
#else
  #ifdef PLATFORM_64
    #define TITLE_MAIN_WND   _T("Testing of geometrical kernel C3D [Visual C++ Debug (x64)]")
  #else
    #define TITLE_MAIN_WND   _T("Testing of geometrical kernel C3D [Visual C++ Debug (x86)]")
  #endif
#endif
#else
#if defined ( __NATIVE_LANGUAGE__ )
  #ifdef PLATFORM_64
    #define TITLE_MAIN_WND   _T("Тестирование геометрического ядра C3D [Visual C++ Release (x64)]")
  #else
    #define TITLE_MAIN_WND   _T("Тестирование геометрического ядра C3D [Visual C++ Release (x86)]")
  #endif
#else
  #ifdef PLATFORM_64
    #define TITLE_MAIN_WND   _T("Testing of geometrical kernel C3D [Visual C++ Release (x64)]")
  #else
    #define TITLE_MAIN_WND   _T("Testing of geometrical kernel C3D [Visual C++ Release (x86)]")
  #endif
#endif
#endif
#else // __USE_QT__
#ifdef C3D_WINDOWS // _MSC_VER
#ifdef C3D_DEBUG
#if defined ( __NATIVE_LANGUAGE__ )
#define TITLE_MAIN_WND   _T("Тестирование геометрического ядра C3D [Qt & Visual C++ Debug]")
#else
#define TITLE_MAIN_WND   _T("Testing of geometrical kernel C3D [Qt & Visual C++ Debug]")
#endif
#else
#if defined ( __NATIVE_LANGUAGE__ )
#define TITLE_MAIN_WND   _T("Тестирование геометрического ядра C3D [Qt & Visual C++ Release]")
#else
#define TITLE_MAIN_WND   _T("Testing of geometrical kernel C3D [Qt & Visual C++ Release]")
#endif
#endif
#else // _WIN32
#ifdef C3D_DEBUG
#if defined ( __NATIVE_LANGUAGE__ )
#define TITLE_MAIN_WND   _T("Тестирование геометрического ядра C3D [Qt & g++ Debug]")
#else
#define TITLE_MAIN_WND   _T("Testing of geometrical kernel C3D [Qt & g++ Debug]")
#endif
#else
#if defined ( __NATIVE_LANGUAGE__ )
#define TITLE_MAIN_WND   _T("Тестирование геометрического ядра C3D [Qt & g++ Release]")
#else
#define TITLE_MAIN_WND   _T("Testing of geometrical kernel C3D [Qt & g++ Release]")
#endif
#endif
#endif // _WIN32
#endif // __USE_QT__


#endif // __TEST_PROMPT_H
