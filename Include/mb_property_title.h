////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Свойства математических объектов.
         \en Properties of mathematical objects. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MB_PROPERTY_TITLE_H
#define __MB_PROPERTY_TITLE_H


//------------------------------------------------------------------------------
/** \brief \ru Свойства математических объектов.
           \en Properties of mathematical objects. \~
  \attention  \ru Целочисленные значения данного перечислительного типа могут быть изменены!
              \en Integer values of the enum can be changed!
  \ingroup Base_Items
*/
// ---
enum MbePrompt 
{
  IDS_ITEM_0000 = 0, ///< \ru Неопределенный объект.

// \ru Базовые объекты двумерной математики. \en Base 2D objects.

  IDS_ITEM_0001,     ///< \ru Двумерная точка. \en  A two-dimensional point.
  IDS_ITEM_0002,     ///< \ru Двумерный вектор. \en Two-dimensional vector.
  IDS_ITEM_0003,     ///< \ru Двумерная матрица преобразования. \en Two-dimensional matrix of transformation.
  IDS_ITEM_0004,     ///< \ru Двумерная локальная система. \en Two-dimensional local system.
  IDS_ITEM_0005,     ///< \ru Двумерный единичный вектор. \en Two-dimensional unit vector

// \ru Типы двумерных кривых. \en Types of two-dimensional curves. 

  IDS_ITEM_0011,     ///< \ru Двумерная кривая. \en Two-dimensional curve

  IDS_ITEM_0013,     ///< \ru Двумерная прямая. \en Two-dimensional line.
  IDS_ITEM_0014,     ///< \ru Двумерный отрезок. \en Two-dimensional segment.
  IDS_ITEM_0015,     ///< \ru Двумерный отрезок прямой. \en  Two-dimensional line segment.
  IDS_ITEM_0016,     ///< \ru Двумерная дуга окружности. \en Two-dimensional circular arc.
  IDS_ITEM_0017,     ///< \ru Двумерная усеченная кривая. \en Two-dimensional truncated curve.
  IDS_ITEM_0018,     ///< \ru Двумерная эквидистантная кривая. \en Two-dimensional offset curve.
  IDS_ITEM_0019,     ///< \ru Двумерная эквидистанта. \en Two-dimensional equidistant.
  IDS_ITEM_0020,     ///< \ru Двумерная окружность. \en Two-dimensional circle.
  IDS_ITEM_0021,     ///< \ru Двумерный эллипс. \en Two-dimensional ellipse.
  IDS_ITEM_0022,     ///< \ru Двумерная парабола. \en Two-dimensional parabola.
  IDS_ITEM_0023,     ///< \ru Двумерная дуга эллипса. \en Two-dimensional elliptical arc.
  IDS_ITEM_0024,     ///< \ru Двумерная ломаная. \en Two-dimensional polyline.
  IDS_ITEM_0025,     ///< \ru Двумерная NURBS кривая. \en Two-dimensional NURBS curve.
  IDS_ITEM_0026,     ///< \ru Двумерный сплайн Эрмита. \en Two-dimensional Hermite spline.
  IDS_ITEM_0027,     ///< \ru Двумерный сплайн Безье. \en Two-dimensional Bezier spline.
  IDS_ITEM_0028,     ///< \ru Двумерный кубический сплайн. \en Two-dimensional cubic spline.
  IDS_ITEM_0029,     ///< \ru Двумерная репараметризованная кривая. \en Two-dimensional reparametrized curve.
  IDS_ITEM_0030,     ///< \ru Двумерный контур. \en Two-dimensional contour.
  IDS_ITEM_0031,     ///< \ru Двумерная косинусоида. \en Two-dimensional cosine curve.
  IDS_ITEM_0032,     ///< \ru Двумерная точечная кривая. \en Two-dimensional point curve.
  IDS_ITEM_0040,     ///< \ru Двумерная область. \en Two-dimensional region.
  IDS_ITEM_0050,     ///< \ru Двумерный объект. \en Two-dimensional object.
  IDS_ITEM_0051,     ///< \ru Двумерная мультилиния. \en Two-dimensional multiline.
  IDS_ITEM_0052,     ///< \ru Двумерная кривая на конусе, соответствующая кривой на коническом изгибе плоскости. \en Two-dimensional curve on cone corresponding to a curve on conic bend of a plane.
  IDS_ITEM_0053,     ///< \ru Двумерная кривая на плоскости, соответствующая кривой на изгибе конуса. \en Two-dimensional curve on plane corresponding to a curve on a bend of cone
  IDS_ITEM_0054,     ///< \ru Двумерная кривая, координатные функции которой заданы в символьном виде. \en Functionally defined two-dimensional curve.
  IDS_ITEM_0055,     ///< \ru Образ трехмерной кривой на поверхности при движении по направляющей. \en Image of a three-dimensional curve on surface while moving along the guide curve.

// \ru Типы полигональных объектов. \en Types of simplified forms of an object. 

  IDS_ITEM_0060,     ///< \ru Параметр. \en Parameter.
  IDS_ITEM_0061,     ///< \ru Вершина. \en Vertex.
  IDS_ITEM_0062,     ///< \ru Нормаль. \en Normal.
  IDS_ITEM_0063,     ///< \ru Треугольник. \en Triangle.
  IDS_ITEM_0064,     ///< \ru Четырехугольник. \en Quadrangle.
  IDS_ITEM_0071,     ///< \ru Полигональный объект на числах double. \en Polygonal object on double data.
  IDS_ITEM_0072,     ///< \ru Апекс на числах double. \en Apex on double data.
  IDS_ITEM_0073,     ///< \ru Полигон на числах double. \en Polygon on double data.
  IDS_ITEM_0074,     ///< \ru Триангуляция на числах double. \en Triangulation on double data.
  IDS_ITEM_0075,     ///< \ru Полигональный объект на числах float. \en Polygonal object on float data.
  IDS_ITEM_0076,     ///< \ru Апекс на числах float. \en Apex on float data.
  IDS_ITEM_0077,     ///< \ru Полигон на числах float. \en Polygon on float data.
  IDS_ITEM_0078,     ///< \ru Триангуляция на числах float. \en Triangulation on float data.
  IDS_ITEM_0079,     ///< \ru "Вывернутый" полигональный объект. \en Reversed polygonal object.
  IDS_ITEM_0080,     ///< \ru Разрезанный полигональный объект. \en Cutted polygonal object.
  IDS_ITEM_0081,     ///< \ru Только сечение. \en Only section.
  IDS_ITEM_0082,     ///< \ru Полигональный объект с топологией. \en Polymesh.
  IDS_ITEM_0083,     ///< \ru Регион полигонального объекта с топологией. \en Polymesh region.
  IDS_ITEM_0084,     ///< \ru Подрегион полигонального объекта с топологией. \en Polymesh subregion.
  IDS_ITEM_0085,     ///< \ru Цикл полигонального объекта с топологией. \en Polymesh loop.
  IDS_ITEM_0086,     ///< \ru Ребро полигонального объекта с топологией. \en Polymesh edge.
  IDS_ITEM_0087,     ///< \ru Вершина полигонального объекта с топологией. \en Polymesh vertex.

// \ru Базовые объекты трехмерной математики. \en Base objects of three-dimensional mathematics. 

  IDS_ITEM_0101,     ///< \ru Точка. \en Point.
  IDS_ITEM_0102,     ///< \ru Вектор. \en Vector.
  IDS_ITEM_0103,     ///< \ru Матрица преобразования. \en Transformation matrix.
  IDS_ITEM_0104,     ///< \ru Локальная система координат. \en Local coordinate system.
  IDS_ITEM_0105,     ///< \ru Ось. \en Axis.

  IDS_ITEM_0107,     ///< \ru Перемещение. \en Translation.
  IDS_ITEM_0108,     ///< \ru Вращение. \en Rotation.
  IDS_ITEM_0109,     ///< \ru Трансформация. \en Transformation.

// \ru Типы функций \en Types of functions 

  IDS_ITEM_0111,     ///< \ru Константная функция. \en Constant Function.
  IDS_ITEM_0112,     ///< \ru Линейная функция. \en Linear Function.
  IDS_ITEM_0113,     ///< \ru Кубическая функция. \en Cubic Function.
  IDS_ITEM_0114,     ///< \ru Кубическай сплайн-функция. \en Cubic spline Function.
  IDS_ITEM_0115,     ///< \ru Символьная функция. \en Symbolic Function.
  IDS_ITEM_0116,     ///< \ru Степенная функция. \en Power Function.
  IDS_ITEM_0117,     ///< \ru Синус функция. \en Sinus Function.
  IDS_ITEM_0118,     ///< \ru Служебная функция. \en Service function.
  IDS_ITEM_0119,     ///< \ru Монотонная функция. \en Monotonous function.
  IDS_ITEM_0120,     ///< \ru NURBS функция. \en NURBS function. 
  IDS_ITEM_0121,     ///< \ru Функция координаты кривой. \en Function by curve coordinate. 

  IDS_ITEM_0191,     ///< \ru Символьная функция. \en Symbolic function. 
  IDS_ITEM_0192,     ///< \ru Символьная функция на модельном выражении. \en Symbolic function in model expression. 

// \ru Типы трехмерных кривы.х \en Types of three-dimensional curves. 

  IDS_ITEM_0201,     ///< \ru Кривая. \en Curve.
  IDS_ITEM_0202,     ///< \ru B-сплайн. \en B-spline.
  IDS_ITEM_0213,     ///< \ru Прямая линия. \en Straight Line.
  IDS_ITEM_0214,     ///< \ru Отрезок. \en Segment.
  IDS_ITEM_0215,     ///< \ru Дуга эллипса. \en Elliptic Arc.
  IDS_ITEM_0216,     ///< \ru Дуга окружности. \en Circular Arc.
  IDS_ITEM_0217,     ///< \ru Усеченная кривая. \en Truncated Curve.
  IDS_ITEM_0218,     ///< \ru Эквидистантная кривая. \en Offset Curve.
  IDS_ITEM_0219,     ///< \ru Коническая спираль. \en Conical Spiral.
  IDS_ITEM_0220,     ///< \ru Окружность. \en Circle.
  IDS_ITEM_0221,     ///< \ru Эллипс. \en Ellipse.
  IDS_ITEM_0222,     ///< \ru Парабола. \en Parabola.
  IDS_ITEM_0223,     ///< \ru Гипербола. \en Hyperbola.
  IDS_ITEM_0224,     ///< \ru Ломаная линия. \en Polyline.
  IDS_ITEM_0225,     ///< \ru NURBS кривая. \en NURBS Curve.
  IDS_ITEM_0226,     ///< \ru Сплайн Эрмита. \en Hermite Spline.
  IDS_ITEM_0227,     ///< \ru Сплайн Безье. \en Bezier Spline.
  IDS_ITEM_0228,     ///< \ru Кубический сплайн. \en Cubic Spline.
  IDS_ITEM_0229,     ///< \ru Репараметризованная кривая. \en Reparametrized Curve.
  IDS_ITEM_0231,     ///< \ru Плоская кривая. \en Plane curve.
  IDS_ITEM_0232,     ///< \ru Спираль с переменым радиусом. \en Spiral with Variable Radius.
  IDS_ITEM_0233,     ///< \ru Спираль с криволинейной осью. \en Spiral with a Curved Axis.
  IDS_ITEM_0234,     ///< \ru Кривая-мостик. \en Bridge Curve.
  IDS_ITEM_0235,     ///< \ru Символьная кривая. \en Functionally Defined Curve.
  IDS_ITEM_0236,     ///< \ru Кривая на поверхности. \en Curve on a Surface.
  IDS_ITEM_0237,     ///< \ru Линия пересечения поверхностей. \en Intersection Curve of Surfaces.
  IDS_ITEM_0238,     ///< \ru Контур на поверхности. \en Contour on a Surface.
  IDS_ITEM_0239,     ///< \ru Контур на плоскости. \en Contour on a Plane.
  IDS_ITEM_0240,     ///< \ru Контур. \en Contour.
  IDS_ITEM_0241,     ///< \ru Проекционная кривая. \en Projection Curve.
  IDS_ITEM_0242,     ///< \ru Силуэтная кривая. \en Silhouette Curve.
  IDS_ITEM_0243,     ///< \ru Кривая сопряжения кривых. \en Curve of Curves Conjugation.
  IDS_ITEM_0244,     ///< \ru Кривая производных поверхности Кунса. \en Curve of Coons Surface Derivetives.
  IDS_ITEM_0249,     ///< \ru Направляющая кривая. \en Guide Curve.
  IDS_ITEM_0250,     ///< \ru Кривая пересечения. \en Intersection Curve.

  IDS_ITEM_0251,     ///< \ru Первое направляющее ребро.  \en First Guide Edge.
  IDS_ITEM_0252,     ///< \ru Первая направляющая кривая. \en First Guide Curve.
  IDS_ITEM_0253,     ///< \ru Первая опорная грань.       \en First Reference Face.
  IDS_ITEM_0254,     ///< \ru Первая контрольная кривая.  \en First Control Curve.
  IDS_ITEM_0255,     ///< \ru Первая функция.             \en First Function.
  IDS_ITEM_0256,     ///< \ru Второе направляющее ребро.  \en Second Guide Edge.
  IDS_ITEM_0257,     ///< \ru Вторая направляющая кривая. \en Second Guide Curve.
  IDS_ITEM_0258,     ///< \ru Вторая опорная грань.       \en Second Reference Face.
  IDS_ITEM_0259,     ///< \ru Вторая контрольная кривая.  \en Second Control Curve.
  IDS_ITEM_0260,     ///< \ru Вторая функция.             \en Second Function.
  IDS_ITEM_0261,     ///< \ru Угол к хорде.                   \en Angle from Chord.
  IDS_ITEM_0262,     ///< \ru Угол к касательной поверхности. \en Angle from Surface Tangent.
  IDS_ITEM_0263,     ///< \ru Угол к нормали поверхности.     \en Angle from Surface Normal.
  IDS_ITEM_0264,     ///< \ru Удлиненная кривая. \en Extended curve.
  IDS_ITEM_0265,     ///< \ru Плавная кривая. \en Fair curve.
  IDS_ITEM_0266,     ///< \ru Измененная плавная кривая. \en Modified Fair curve.
  IDS_ITEM_0267,     ///< \ru Плавная кривая сопряжения. \en Fair Fillet curve.
  IDS_ITEM_0268,     ///< \ru Клотоида. \en Clothoid.
  IDS_ITEM_0269,     ///< \ru Развернутая кривая. \en Unwrapped curve.
  IDS_ITEM_0270,     ///< \ru Свёрнутая кривая. \en Wrapped curve.
  IDS_ITEM_0271,     ///< \ru Балочная кривая. \en Beam curve.
  IDS_ITEM_0272,     ///< \ru Скругленный каркас. \en Filleted wire frame.
  IDS_ITEM_0273,     ///< \ru NURBS-копия каркаса. \en Nurbs-copy by wire frame.
  IDS_ITEM_0274,     ///< \ru Усеченный каркас. \en Trimmed wire frame.
  IDS_ITEM_0275,     ///< \ru Развернутая точка. \en Unwrapped point.
  IDS_ITEM_0276,     ///< \ru Свёрнутая точка. \en Wrapped point.
  IDS_ITEM_0277,     ///< \ru Эквидистантный каркас. \en Offset wire frame.


// \ru Типы параметрических поверхностей. \en Types of parametric surfaces. 

  IDS_ITEM_0301,     ///< \ru Поверхность. \en Surface.
  IDS_ITEM_0302,     ///< \ru Поверхность заметания. \en Sweep Surface.
  IDS_ITEM_0303,     ///< \ru Поверхность сдвига. \en Motion Surface.
  IDS_ITEM_0304,     ///< \ru Поверхность выдавливания. \en Extrusion Surface.
  IDS_ITEM_0305,     ///< \ru Поверхность вращения. \en Revolution Surface.
  IDS_ITEM_0306,     ///< \ru Линейчатая поверхность. \en Ruled Surface.
  IDS_ITEM_0307,     ///< \ru Поверхность по кривой и точке. \en Surface Defined by a Curve and a Point.
  IDS_ITEM_0308,     ///< \ru Четырехугольная поверхность. \en Quadrangular Surface.
  IDS_ITEM_0309,     ///< \ru Треугольная поверхность. \en Triangular Surface.
  IDS_ITEM_0310,     ///< \ru Поверхность движения с доворотом. \en Sweep with Guide Curve Surface with Rotating Ends.
  IDS_ITEM_0311,     ///< \ru Поверхность на семействе кривых и напрвляющей. \en Loft Surface with Guide Curve.
  IDS_ITEM_0312,     ///< \ru Спиральная поверхность. \en Spiral Surface.
  IDS_ITEM_0313,     ///< \ru Цилиндрически согнутая поверхность. \en Cylindrically Bent Surface.
  IDS_ITEM_0314,     ///< \ru Цилиндрически разогнутая поверхность. \en Cylindrically Unbent Surface.
  IDS_ITEM_0315,     ///< \ru Конически согнутая поверхность. \en Conically Bent Surface.
  IDS_ITEM_0316,     ///< \ru Конически разогнутая поверхность. \en Conically Unbent Surface.
  IDS_ITEM_0317,     ///< \ru Поверхность заметания с изменением образующей. \en Sweep Surface with Changin Generatin.
  IDS_ITEM_0318,     ///< \ru Линейчато согнутая/разогнутая поверхность. \en Ruled Bent/Unbent Surface.
  IDS_ITEM_0319,     ///< \ru Плоскость. \en Plane.
  IDS_ITEM_0320,     ///< \ru Сферическая поверхность. \en Spherical Surface.
  IDS_ITEM_0321,     ///< \ru Тороидальная поверхность. \en Toroidal Surface.
  IDS_ITEM_0322,     ///< \ru Цилиндрическая поверхность. \en Cylindrical Surface.
  IDS_ITEM_0323,     ///< \ru Коническая поверхность. \en Conical Surface.
  IDS_ITEM_0325,     ///< \ru NURBS поверхность. \en NURBS Surface.
  IDS_ITEM_0326,     ///< \ru Треугольная NURBS поверхность. \en Triangular NURBS Surface.
  IDS_ITEM_0327,     ///< \ru Поверхность Безье. \en Bezier Surface.
  IDS_ITEM_0328,     ///< \ru Эквидистантная поверхность. \en Offset Surface.
  IDS_ITEM_0329,     ///< \ru Деформированная поверхность. \en Deformed Surface.
  IDS_ITEM_0330,     ///< \ru Поверхность Грегори. \en Gregory Surface.
  IDS_ITEM_0331,     ///< \ru Поверхность соединения. \en Joint Surface.
  IDS_ITEM_0332,     ///< \ru Поверхность объединения. \en Join Surface.
  IDS_ITEM_0333,     ///< \ru Поверхность на трех кривых. \en Surface Based on Three Curves.
  IDS_ITEM_0334,     ///< \ru Поверхность на четырех кривых. \en Surface Based on Four Curves.
  IDS_ITEM_0335,     ///< \ru Поверхность-фаска. \en Chamfer Surface.
  IDS_ITEM_0336,     ///< \ru Поверхность скругления. \en Fillet Surface.
  IDS_ITEM_0337,     ///< \ru Переменная поверхность скругления. \en Variable Fillet Surface.
  IDS_ITEM_0338,     ///< \ru Поверхность на семействе кривых. \en Lofted Surface.
  IDS_ITEM_0339,     ///< \ru Поверхность на сетке кривых. \en Surface defined on a mesh of curves.
  IDS_ITEM_0340,     ///< \ru Поверхность скругления по кромке. \en Surface of Fillet by Border.
  IDS_ITEM_0341,     ///< \ru Поверхность по замкнутому контуру. \en Surface on Closed Contour.
  IDS_ITEM_0342,     ///< \ru Плазовая поверхность. \en Spiling Surface.
  IDS_ITEM_0343,     ///< \ru Поверхность Кунса. \en Coons Surface.
  IDS_ITEM_0345,     ///< \ru Поверхность на сетке точек. \en Surface Based on a Point Grid.
  IDS_ITEM_0346,     ///< \ru Треугольная поверхность Безье. \en Triangular Bezier Surface.
  IDS_ITEM_0349,     ///< \ru Усеченная контурами поверхность. \en Curve bounded Surface.
  IDS_ITEM_0350,     ///< \ru Поверхность-копия. \en Copy Surface.
  IDS_ITEM_0351,     ///< \ru Поверхность соединения. \en Joint Surface.
  IDS_ITEM_0352,     ///< \ru Поверхность полного скругления. \en Full Fillet Surface.
  IDS_ITEM_0353,     ///< \ru Поверхность заметания с масштабированием. \en Swept Surface with Scaling.
  IDS_ITEM_0354,     ///< \ru Поверхность переменного сечения. \en The Swept Mutable Section Surface.
  IDS_ITEM_0355,     ///< \ru Пользовательская поверхность. \en The User Defined Surface.

// \ru Типы тел \en Types of solids 

  IDS_ITEM_0401,     ///< \ru Тело. \en Solid.
  IDS_ITEM_0402,     ///< \ru Оболочка. \en Shell.
  IDS_ITEM_0403,     ///< \ru Проволочный каркас. \en Wire Frame.
  IDS_ITEM_0404,     ///< \ru Точечный каркас. \en Point Frame.
  IDS_ITEM_0405,     ///< \ru Коллекция элементов. \en Collection of Elements.

  IDS_ITEM_0411,     ///< \ru Тело первой группы. \en Solid of The First Set.
  IDS_ITEM_0412,     ///< \ru Тело второй группы. \en Solid of The Second Set.

// \ru Типы строителей. \en Types of creators.  

  IDS_ITEM_0501,     ///< \ru Журнал построения. \en Build log.
  IDS_ITEM_0502,     ///< \ru Шар. \en Sphere.
  IDS_ITEM_0503,     ///< \ru Тор. \en Torus.
  IDS_ITEM_0504,     ///< \ru Цилиндр. \en Cylinder.
  IDS_ITEM_0505,     ///< \ru Конус. \en Cone.
  IDS_ITEM_0506,     ///< \ru Блок. \en Block.
  IDS_ITEM_0507,     ///< \ru Клин. \en Wedge.
  IDS_ITEM_0508,     ///< \ru Призма. \en Prism.
  IDS_ITEM_0509,     ///< \ru Пирамида. \en Pyramid.
  IDS_ITEM_0510,     ///< \ru Твёрдое тело. \en Solid.
  IDS_ITEM_0511,     ///< \ru Плита. \en Plate.
  IDS_ITEM_0512,     ///< \ru Икосаэдр. \en Icosahedron.
  IDS_ITEM_0513,     ///< \ru Многогранник. \en Polyhedron.

  IDS_ITEM_0515,     ///< \ru Объединение оболочек. \en Shells union.
  IDS_ITEM_0516,     ///< \ru Пересечение оболочек. \en Shells intersection.
  IDS_ITEM_0517,     ///< \ru Разность оболочек. \en Shells subtraction.
  IDS_ITEM_0518,     ///< \ru Нормализация отверстия. \en Normalization of hole.

  IDS_ITEM_0520,     ///< \ru Отверстие. \en Hole.
  IDS_ITEM_0521,     ///< \ru Карман/Бобышка. \en Pocket/Boss.
  IDS_ITEM_0522,     ///< \ru Паз. \en Groove.
  IDS_ITEM_0523,     ///< \ru Заплатка. \en Patch.
  IDS_ITEM_0524,     ///< \ru Тонкая оболочка. \en Thin Shell.
  IDS_ITEM_0525,     ///< \ru Листовое тело на базе произвольного тела. \en Sheet solid based on an arbitary solid.
  IDS_ITEM_0526,     ///< \ru Оболочка на семействе кривых. \en Shell Defined by a Set of Curves.
  IDS_ITEM_0527,     ///< \ru Продолженная оболочка. \en Extended Shell.
  IDS_ITEM_0528,     ///< \ru Эквидистантная оболочка. \en Offset Shell.
  IDS_ITEM_0529,     ///< \ru Срединная оболочка. \en Median Shell.
  IDS_ITEM_0530,     ///< \ru Оболочка переменного сечения. \en The Swept Mutable Section Shell.
  IDS_ITEM_0531,     ///< \ru Булево объединение тел. \en Boolean Union of Solids.
  IDS_ITEM_0532,     ///< \ru Булево пересечение тел. \en Boolean Intersection of Solids.
  IDS_ITEM_0533,     ///< \ru Булева разность тел. \en Boolean Subtraction of Solids.
  IDS_ITEM_0534,     ///< \ru Разрезанное тело. \en Cut Solid.
  IDS_ITEM_0535,     ///< \ru Фаски ребер. \en Edges Chamfers.
  IDS_ITEM_0536,     ///< \ru Скругление ребер. \en Edges Fillets.
  IDS_ITEM_0537,     ///< \ru Симметричное тело. \en Symmetric Solid.
  IDS_ITEM_0538,     ///< \ru Оболочечное тело. \en Thin Shell Solid.
  IDS_ITEM_0539,     ///< \ru Тело приданием толщины. \en Solid of Thickening.
  IDS_ITEM_0540,     ///< \ru Оболочка с удалёнными гранями. \en Shell with Removed Faces.
  IDS_ITEM_0541,     ///< \ru Коробчатое тело. \en Box-like Solid.
  IDS_ITEM_0542,     ///< \ru Кинематическое тело. \en Sweeping Solid.
  IDS_ITEM_0543,     ///< \ru Тело заметания. \en Swept Solid.
  IDS_ITEM_0544,     ///< \ru Тело выдавливания. \en Extrusion Solid.
  IDS_ITEM_0545,     ///< \ru Тело вращения. \en Revolution Solid.
  IDS_ITEM_0546,     ///< \ru Тело по сечениям. \en Loft Solid.
  IDS_ITEM_0547,     ///< \ru Простое тело. \en Simple Solid.
  IDS_ITEM_0548,     ///< \ru Ребро жесткости тела. \en Rib of a Solid.
  IDS_ITEM_0549,     ///< \ru Набор тел. \en Set of solids.
  IDS_ITEM_0550,     ///< \ru Часть набора тел. \en Set of solids part.
  IDS_ITEM_0551,     ///< \ru Клон граней тела. \en Solid's faces drafting.
  IDS_ITEM_0552,     ///< \ru Разбивка граней тела. \en Splitting of Solid's faces.
  IDS_ITEM_0553,     ///< \ru Сшитое из оболочек тело. \en Stitched Solid.
  IDS_ITEM_0554,     ///< \ru Сшитая из оболочек оболочка. \en Shell stitched from shells.
  IDS_ITEM_0555,     ///< \ru Оболочка из NURBS-поверхностей. \en Shell from NURBS-surfaces.
  IDS_ITEM_0556,     ///< \ru Трансформированное тело. \en Transformed Solid.
  IDS_ITEM_0557,     ///< \ru Модифицированное тело. \en Modified Solid.
  IDS_ITEM_0558,     ///< \ru Оболочка из линейчатых поверхностей. \en Shell from ruled surfaces.
  IDS_ITEM_0559,     ///< \ru Усеченная оболочка. \en Truncated Shell.
  IDS_ITEM_0560,     ///< \ru Оболочка соединения. \en Joint Shell.
  IDS_ITEM_0561,     ///< \ru Тело с восстановленными боковыми рёбрами. \en Solid with restored lateral edges.
  IDS_ITEM_0562,     ///< \ru Объединение с кинематическим телом. \en Union with a sweeping Solid.
  IDS_ITEM_0563,     ///< \ru Модифицированное тело NURBS-поверхностями. \en Solid modified with NURBS-surfaces.
  IDS_ITEM_0564,     ///< \ru Объединение с телом выдавливания. \en Union with an extrusion Solid.
  IDS_ITEM_0565,     ///< \ru Объединение с телом вращения. \en Union with a revolution Solid.
  IDS_ITEM_0566,     ///< \ru Объединение с телом по сечениям. \en Union with a lofted Solid.
  IDS_ITEM_0567,     ///< \ru Объединение с простым телом. \en Union with an elementary Solid.
  IDS_ITEM_0568,     ///< \ru Модифицированная NURBS-поверхность грани. \en Modified NURBS-surface of the face.
  IDS_ITEM_0569,     ///< \ru Объединение с набором тел. \en Union with a Solid set.
  IDS_ITEM_0570,     ///< \ru Оболочка грани соединения. \en Joint face Shell.
  IDS_ITEM_0571,     ///< \ru Скругление граней. \en Faces fillet.
  IDS_ITEM_0572,     ///< \ru Разность с кинематическим телом. \en Subtraction with a sweeping Solid.
  IDS_ITEM_0573,     ///< \ru Удаление результата операции. \en Delete the result of the operation.
  IDS_ITEM_0574,     ///< \ru Разность с телом выдавливания. \en Subtraction with an extrusion Solid.
  IDS_ITEM_0575,     ///< \ru Разность с телом вращения. \en Subtraction with a revolution Solid.
  IDS_ITEM_0576,     ///< \ru Разность с телом по сечениям. \en Subtraction with a lofted Solid.
  IDS_ITEM_0577,     ///< \ru Разность с простым телом. \en Subtraction with an elementary Solid.
  IDS_ITEM_0578,     ///< \ru Упрощение развёртки. \en The flat pattern simplification.
  IDS_ITEM_0579,     ///< \ru Разность с набором тел. \en Subtraction with a Set of Solids.
  IDS_ITEM_0580,     ///< \ru Вывернутое тело. \en Reversed Solid.
  IDS_ITEM_0581,     ///< \ru Сгиб нелистового тела. \en Bend of a non-sheet Solid.
  IDS_ITEM_0582,     ///< \ru Пересечение с кинематическим телом. \en Intersection with a sweeping Solid.
  IDS_ITEM_0583,     ///< \ru Сферическая штамповка. \en Spherical stamping.
  IDS_ITEM_0584,     ///< \ru Пересечение с телом выдавливания. \en Intersection with an extrusion Solid.
  IDS_ITEM_0585,     ///< \ru Пересечение с телом вращения. \en Intersection with a revolution Solid.
  IDS_ITEM_0586,     ///< \ru Пересечение с телом по сечениям. \en Intersection with a lofted Solid.
  IDS_ITEM_0587,     ///< \ru Пересечение с простым телом. \en Intersection with an elementary Solid.
  IDS_ITEM_0588,     ///< \ru Обечайка. \en Ruled Shell.
  IDS_ITEM_0589,     ///< \ru Пересечение с набором тел. \en Intersection with a set of solids.
  IDS_ITEM_0590,     ///< \ru Оболочка по сети кривых. \en Shell defined by a mesh of curves.
  IDS_ITEM_0591,     ///< \ru Комбинированный сгиб. \en Combined bend.
  IDS_ITEM_0592,     ///< \ru Кинематическая оболочка. \en Sweep with guide curve Shell.
  IDS_ITEM_0593,     ///< \ru Жалюзи. \en Jalousie.
  IDS_ITEM_0594,     ///< \ru Оболочка выдавливания. \en Extrusion Shell.
  IDS_ITEM_0595,     ///< \ru Оболочка вращения. \en Revolution Shell.
  IDS_ITEM_0596,     ///< \ru Оболочка по сечениям. \en Loft Shell.
  IDS_ITEM_0597,     ///< \ru Тонкая оболочка. \en Thin Shell.
  IDS_ITEM_0598,     ///< \ru Разрезанная оболочка. \en Cut Shell.
  IDS_ITEM_0599,     ///< \ru Буртик. \en Bead.
  IDS_ITEM_0600,     ///< \ru Сгиб/разгиб листового тела. \en Bend/Unbend of a Sheet Solid.
  IDS_ITEM_0601,     ///< \ru Сгиб листового тела по отрезку. \en Bend of a sheet Solid by a segment.
  IDS_ITEM_0602,     ///< \ru Сгиб листового тела по рёбрам. \en Bend of a sheet Solid along edges.
  IDS_ITEM_0603,     ///< \ru Замыкание угла листового тела. \en Closure of a sheet Solid corner.
  IDS_ITEM_0604,     ///< \ru Листовое тело. \en Sheet Solid.
  IDS_ITEM_0605,     ///< \ru Пластина листового тела. \en Sheet Solid plate.
  IDS_ITEM_0606,     ///< \ru Вырез листового тела. \en Cut of a sheet Solid.
  IDS_ITEM_0607,     ///< \ru Пересечение листового тела. \en Sheet Solid intersection.
  IDS_ITEM_0608,     ///< \ru Подсечка листового тела. \en Jog of a sheet Solid.
  IDS_ITEM_0609,     ///< \ru Штамповка. \en Stamping.

// \ru Способы построения оболочек. \en Shells construction methods. 

  IDS_ITEM_0610,     ///< \ru Оболочка. \en Shell.
  IDS_ITEM_0611,     ///< \ru Оболочка на базе поверхности. \en Shell based on a surface.

  IDS_ITEM_0614,     ///< \ru Оболочка тела. \en Shell.
  IDS_ITEM_0615,     ///< \ru Журнал построения. \en Build log.
  IDS_ITEM_0616,     ///< \ru Атрибуты объекта. \en Object Attributes.

  IDS_ITEM_0620,     ///< \ru Оболочка тела. \en Shell.
  IDS_ITEM_0621,     ///< \ru Ориентированное ребро цикла. \en Oriented edge of a loop.
  IDS_ITEM_0622,     ///< \ru Цикл грани. \en Face loop.
  IDS_ITEM_0623,     ///< \ru Грань оболочки. \en Face of a Shell.
  IDS_ITEM_0624,     ///< \ru Вершина. \en Vertex.
  IDS_ITEM_0625,     ///< \ru Ребро оболочки. \en Edge of a Shell.
  IDS_ITEM_0626,     ///< \ru Ребро каркаса. \en Edge of a frame.

  IDS_ITEM_0627,     ///< \ru Вершина. \en Vertex.

  IDS_ITEM_0628,     ///< \ru Разделенная оболочка. \en Divided Shell.

  IDS_ITEM_0630,     ///< \ru Оболочка кругового сечения.    \en Round Section Shell.
  IDS_ITEM_0631,     ///< \ru Оболочка лигнейчатого сечения. \en Ruled Section Shell.
  IDS_ITEM_0632,     ///< \ru Оболочка конического сечения.  \en Conic Section Shell.
  IDS_ITEM_0633,     ///< \ru Оболочка кубического сечения.  \en Cubic Section Shell.
  IDS_ITEM_0634,     ///< \ru Оболочка сплайнового сечения.  \en Spline Section Shell.
  IDS_ITEM_0635,     ///< \ru Оболочка скругления диском.    \en Disk Fillet Shell.
  IDS_ITEM_0636,     ///< \ru Оболочка скругления сферой.    \en Sphere Fillet Shell.

// \ru Способы построения проекций тела\оболочки. \en Solid/Shell projections creation methods. 

  IDS_ITEM_0650,     ///< \ru Проекция тела. \en Solid Projection.
  IDS_ITEM_0651,     ///< \ru Разрез тела. \en Solid Cutting.
  IDS_ITEM_0652,     ///< \ru Сечение тела. \en Solid Section.

  IDS_ITEM_0653,     ///< \ru Копия объекта. \en Object copy.

// \ru Вспомогательный объект. \en The helper object.

  IDS_ITEM_0669,     ///< \ru Вспомогательный объект. \en The Helper Object.

// \ru Резьба. \en A thread. 

  IDS_ITEM_0670,     ///< \ru Резьба. \en Thread.

// \ru Обозначение \en Notation 

  IDS_ITEM_0671,     ///< \ru Условное обозначение. \en Symbolic Notation.
  IDS_ITEM_0672,     ///< \ru Элемент аннотации.  \en PMI object.
  IDS_ITEM_0673,     ///< \ru Тип элемента аннотации.  \en PMI type.
  IDS_ITEM_0674,     ///< \ru Текстовый элемент.  \en Text item.
  IDS_ITEM_0675,     ///< \ru Составной текст.    \en Composite text.

// \ru Объекты. \en Objects. 

  IDS_ITEM_0700,     ///< \ru Геометрический объект. \en Geometric Object.
  IDS_ITEM_0701,     ///< \ru Переменная уравнения. \en Equation Variable.
  IDS_ITEM_0702,     ///< \ru Объект на плоскости. \en Object on a Plane.
  IDS_ITEM_0703,     ///< \ru Объект в пространстве. \en Object in Space.
  IDS_ITEM_0704,     ///< \ru Объект модели. \en Model Object.
  IDS_ITEM_0705,     ///< \ru Сборочная единица. \en Assembly Unit.
  IDS_ITEM_0706,     ///< \ru Вспомогательный объект. \en Auxiliary Object.
  IDS_ITEM_0707,     ///< \ru Вставка объекта. \en Object Instance.
  IDS_ITEM_0708,     ///< \ru Количество элементов. \en Number of Elements.
  IDS_ITEM_0709,     ///< \ru Геометрическая модель. \en Geometric Model.

// \ru Атрибуты \en Attributes 

  IDS_ITEM_0729,     ///< \ru Атрибуты модели. \en Model Attributes.
  IDS_ITEM_0730,     ///< \ru Поставщик атрибутов. \en Attributes Provider.
  IDS_ITEM_0731,     ///< \ru Атрибуты объекта. \en Object Attributes.
  IDS_ITEM_0732,     ///< \ru Атрибут. \en Attribute.
  IDS_ITEM_0733,     ///< \ru Имя примитива. \en Primitive Name.
  IDS_ITEM_0734,     ///< \ru Поведение атрибутов. \en Attributes Behavior.

  IDS_ITEM_0751,     ///< \ru Механические характеристики. \en Mechanical Properties. 
  IDS_ITEM_0754,     ///< \ru Деформации. \en Strains. 

  IDS_ITEM_0761,     ///< \ru Исполнение (вариант реализации модели). \en Embodiment (Variant of Model Implementation). 
  IDS_ITEM_0762,     ///< \ru Количество u-линий и v-линий отрисовочной сетки. \en The Number of u-mesh and v-mesh Lines. 
  IDS_ITEM_0763,     ///< \ru Плотность. \en Density.
  IDS_ITEM_0764,     ///< \ru Цвет. \en Color.
  IDS_ITEM_0765,     ///< \ru Толщина. \en Thickness.
  IDS_ITEM_0766,     ///< \ru Стиль. \en Style.
  IDS_ITEM_0767,     ///< \ru Визуальные свойства. \en Visual Properties.
  IDS_ITEM_0768,     ///< \ru Идентификатор. \en Identifier.
  IDS_ITEM_0769,     ///< \ru Селектированность. \en Selectivity.
  IDS_ITEM_0770,     ///< \ru Видимость. \en Visibility.
  IDS_ITEM_0771,     ///< \ru Измененность. \en Modification.
  IDS_ITEM_0772,     ///< \ru Топологическое имя. \en Topological Name.
  IDS_ITEM_0773,     ///< \ru Якорь. \en Anchor.
  IDS_ITEM_0774,     ///< \ru Геометрический атрибут. \en Geometric Attribute.
  IDS_ITEM_0775,     ///< \ru Метка времени обновления. \en Label of Update Time.
  IDS_ITEM_0776,     ///< \ru Уникальность ключей. \en Keys Uniqueness.
  IDS_ITEM_0777,     ///< \ru Имя объекта в модели. \en Name of object in the model.
  IDS_ITEM_0778,     ///< \ru Данные об изделии. \en Product data.
  IDS_ITEM_0779,     ///< \ru Атрибут ребра жесткости листового тела. \en Attribute of stamp rib of sheet solid.
  IDS_ITEM_0780,     ///< \ru Атрибут отбортовки листового тела. \en Swept flange attribute of a sheet solid.
  IDS_ITEM_0781,     ///< \ru Атрибут Uuid для вставок. \en Uuid attribute for instances.

  IDS_ITEM_0782,     ///< \ru Атрибут пользовательский. \en Custom Attribute.
  IDS_ITEM_0783,     ///< \ru Атрибут обобщенный. \en Generalized Attribute.
  IDS_ITEM_0784,     ///< \ru Атрибут булев. \en Boolean Attribute.
  IDS_ITEM_0785,     ///< \ru Атрибут целочисленный (32-битный). \en (32 bit ) Integer Attribute.
  IDS_ITEM_0786,     ///< \ru Атрибут действительный. \en Real Attribute.
  IDS_ITEM_0787,     ///< \ru Атрибут строковый. \en String Attribute.
  IDS_ITEM_0788,     ///< \ru Атрибут элементарный. \en Elementary Attribute.
  IDS_ITEM_0789,     ///< \ru Пояснение. \en Prompt.
  IDS_ITEM_0790,     ///< \ru Атрибут int64. \en Int64 Attribute.
  IDS_ITEM_0791,     ///< \ru Атрибут бинарный. \en Binary Attribute.
  IDS_ITEM_0792,     ///< \ru Атрибут массив целочисленных значений типа int32. \en Array of integer (int32) values attribute.
  IDS_ITEM_0793,     ///< \ru Атрибут массив целочисленных значений типа int64. \en Array of integer (int64) values attribute.
  IDS_ITEM_0794,     ///< \ru Атрибут массив действительных чисел типа double. \en Array of real (double) values attribute.
  IDS_ITEM_0795,     ///< \ru Атрибут хот-точки. \en Attribute of hot-point. 
  IDS_ITEM_0796,     ///< \ru Атрибут диапазона действительных чисел типа double. \en Double precision range attribute.
  IDS_ITEM_0797,     ///< \ru Атрибут ссылочного объекта. \en Reference-counted object attribute.

// \ru Сообщения. \en Messages.

  IDS_ITEM_0900,     ///< \ru ! Ошибка !. \en ! Error !
  IDS_ITEM_0901,     ///< \ru  Остановлено. \en  Stopped.
  IDS_ITEM_0902,     ///< \ru  Пропущено. \en  Missed.

// \ru Состав объектов \en Structure of objects 

  IDS_PROP_0000,     ///< \ru Пусто. \en Empty.
                             
  IDS_PROP_0001,     ///< \ru Кривая на плоскости. \en Curve on a plane.
  IDS_PROP_0002,     ///< \ru Параметр кривой. \en Parameter of a curve.
  IDS_PROP_0003,     ///< \ru Кривая 1 на плоскости. \en Curve 1 on the plane.
  IDS_PROP_0004,     ///< \ru Кривая 2 на плоскости. \en Curve 2 on the plane.
  IDS_PROP_0005,     ///< \ru Параметр кривой 1. \en Parameter of curve 1.
  IDS_PROP_0006,     ///< \ru Параметр кривой 2. \en Parameter of curve 2.
  IDS_PROP_0007,     ///< \ru Начальная точка. \en Start point.
  IDS_PROP_0008,     ///< \ru Направление. \en Direction.
  IDS_PROP_0009,     ///< \ru Конечная  точка. \en End point.
  IDS_PROP_0014,     ///< \ru Начальный параметр усечения. \en Start parameter of truncation.
  IDS_PROP_0015,     ///< \ru Конечный  параметр усечения. \en End parameter of truncation.
  IDS_PROP_0016,     ///< \ru Совпадение направления. \en Coincidence of direction.
  IDS_PROP_0017,     ///< \ru Циклическая частота. \en Cyclic frequency.
  IDS_PROP_0018,     ///< \ru Начальная фаза (град). \en Initial phase (degrees).
  IDS_PROP_0019,     ///< \ru Амплитуда. \en Amplitude.
  IDS_PROP_0020,     ///< \ru Перевернуть направление. \en Reverse the direction.
  IDS_PROP_0021,     ///< \ru Неподвижная точка. \en Fixed point.
  IDS_PROP_0022,     ///< \ru Использовать точку. \en Use a point.
  IDS_PROP_0023,     ///< \ru Равномерное преобразование. \en Uniform transformation.
  IDS_PROP_0024,     ///< \ru Точность. \en Tolerance.
  IDS_PROP_0025,     ///< \ru Начальное значение. \en Start value.
  IDS_PROP_0026,     ///< \ru Коэффициент усиления. \en Scale gain. 
  IDS_PROP_0027,     ///< \ru Амплитуда. \en Amplitude. 
  IDS_PROP_0028,     ///< \ru Сдвиг параметра. \en Parameter shift.
  IDS_PROP_0029,     ///< \ru Степень возведения. \en Exponent parameter. 
  IDS_PROP_0030,     ///< \ru Циклическая частота. \en Frequency. 
  IDS_PROP_0031,     ///< \ru Эквидистантное смещение. \en The offset range. 
  IDS_PROP_0032,     ///< \ru Тип кривой. \en Type of curve.
  
  IDS_PROP_0033,     ///< \ru Прогиб. \en The sag.
  IDS_PROP_0034,     ///< \ru Угловое отклонение. \en The angular deviation.
  IDS_PROP_0035,     ///< \ru Расстояние. \en The distance.

  IDS_PROP_0101,     ///< \ru Координата X. \en Coordinate X.
  IDS_PROP_0102,     ///< \ru Координата Y. \en Coordinate Y.
  IDS_PROP_0103,     ///< \ru Координата Z. \en Coordinate Z.
  IDS_PROP_0104,     ///< \ru Матрица. \en Matrix.
  IDS_PROP_0107,     ///< \ru Угол с осью X. \en Angle with axis X.
  IDS_PROP_0108,     ///< \ru Угол с осью Y. \en Angle with axis Y.
  IDS_PROP_0109,     ///< \ru Угол с осью Z. \en Angle with axis Z.
  IDS_PROP_0110,     ///< \ru Точка. \en Point.
  IDS_PROP_0111,     ///< \ru Компонента 1.X. \en Component 1.X.
  IDS_PROP_0112,     ///< \ru Компонента 1.Y. \en Component 1.Y.
  IDS_PROP_0113,     ///< \ru Компонента 1.Z. \en Component 1.Z.
  IDS_PROP_0114,     ///< \ru Компонента 2.X. \en Component 2.X.
  IDS_PROP_0115,     ///< \ru Компонента 2.Y. \en Component 2.Y.
  IDS_PROP_0116,     ///< \ru Компонента 2.Z. \en Component 2.Z.
  IDS_PROP_0117,     ///< \ru Компонента 3.X. \en Component 3.X.
  IDS_PROP_0118,     ///< \ru Компонента 3.Y. \en Component 3.Y.
  IDS_PROP_0119,     ///< \ru Компонента 3.Z. \en Component 3.Z.
  IDS_PROP_0120,     ///< \ru Вектор. \en Vector.
  IDS_PROP_0121,     ///< \ru Сдвиг по X. \en Shift by X.
  IDS_PROP_0122,     ///< \ru Сдвиг по Y. \en Shift by Y.
  IDS_PROP_0123,     ///< \ru Сдвиг по Z. \en Shift by Z.
  IDS_PROP_0124,     ///< \ru Ось X. \en Axis X.
  IDS_PROP_0125,     ///< \ru Ось Y. \en Axis Y.
  IDS_PROP_0126,     ///< \ru Ось Z. \en Axis Z.
  IDS_PROP_0127,     ///< \ru Длина оси X. \en Length of axis X.
  IDS_PROP_0128,     ///< \ru Длина оси Y. \en Length of axis Y.
  IDS_PROP_0129,     ///< \ru Длина оси Z. \en Length of axis Z.
  IDS_PROP_0130,     ///< \ru Центр. \en Center.
  IDS_PROP_0131,     ///< \ru Левая система. \en Left system.
  IDS_PROP_0132,     ///< \ru Прямоугольная. \en Rectangular.
  IDS_PROP_0133,     ///< \ru Базовая точка. \en Base point.
  IDS_PROP_0134,     ///< \ru Строка. \en String.
  IDS_PROP_0135,     ///< \ru Шрифт. \en Font.
  IDS_PROP_0136,     ///< \ru Позиция начала. \en Start position.
  IDS_PROP_0137,     ///< \ru Высота. \en Height.
  IDS_PROP_0138,     ///< \ru Сужение. \en Taper.
  IDS_PROP_0139,     ///< \ru Угол наклона. \en Slope angle.
  IDS_PROP_0140,     ///< \ru Радиус. \en Radius.
  IDS_PROP_0141,     ///< \ru Масштаб по X. \en Scale by X.
  IDS_PROP_0142,     ///< \ru Масштаб по Y. \en Scale by Y.
  IDS_PROP_0143,     ///< \ru Масштаб по Z. \en Scale by Z.
  IDS_PROP_0144,     ///< \ru Функция масштабирования. \en The function of scaling.
  IDS_PROP_0145,     ///< \ru Функция вращения. \en The function of rotation.
  IDS_PROP_0146,     ///< \ru Толерантность. \en The tolerance.

  IDS_PROP_0150,     ///< \ru Угол. \en Angle.
  IDS_PROP_0151,     ///< \ru Шаг. \en Step.
  IDS_PROP_0152,     ///< \ru Смещение зазора. \en Gap displacement.
  IDS_PROP_0153,     ///< \ru Перемещение. \en Translation.
  IDS_PROP_0154,     ///< \ru Вращение. \en Rotation.
  IDS_PROP_0155,     ///< \ru Масштабирование. \en Scaling.
  IDS_PROP_0156,     ///< \ru Зеркальность. \en Specularity.
  IDS_PROP_0157,     ///< \ru Только ортогональность. \en Orthogonality only.
  IDS_PROP_0158,     ///< \ru Трансформация. \en Transformation.
  IDS_PROP_0159,     ///< \ru Перспектива. \en Perspective.
  IDS_PROP_0160,     ///< \ru Локальная система координат. \en Local coordinate system.
  IDS_PROP_0161,     ///< \ru Начальное значение. \en Start value.
  IDS_PROP_0162,     ///< \ru Конечное  значение. \en End value.
  IDS_PROP_0163,     ///< \ru Значение. \en Value.
  IDS_PROP_0164,     ///< \ru Функция изменения радиусов. \en Radius function.
  IDS_PROP_0165,     ///< \ru Функция изменения веса. \en Weight function.
  IDS_PROP_0166,     ///< \ru Функция. \en Function.
  IDS_PROP_0167,     ///< \ru Минимум. \en Minimum.
  IDS_PROP_0168,     ///< \ru Максимум. \en Maximum

  IDS_PROP_0169,     ///< \ru Продлевать вверх?. \en Extend upwards?.
  IDS_PROP_0170,     ///< \ru Поворот оси вокруг нормали. \en Rotation of axis about the normal.
  IDS_PROP_0171,     ///< \ru Угол между осью и нормалью. \en Angle between the axis and the normal.
  IDS_PROP_0172,     ///< \ru Диаметр головки. \en Cap diameter.
  IDS_PROP_0173,     ///< \ru Глубина под головку. \en Depth for a cap.
  IDS_PROP_0174,     ///< \ru Угол фаски под головку. \en Chamfer angle for a cap.
  IDS_PROP_0175,     ///< \ru Диаметр отверстия под резьбу. \en Diameter of hole for a cap.
  IDS_PROP_0176,     ///< \ru Глубина отверстия под резьбу. \en Depth of hole for a cap.
  IDS_PROP_0177,     ///< \ru Угол конусности отверстия. \en Taper angle of hole.
  IDS_PROP_0178,     ///< \ru Угол раствора конца отверстия. \en Apical angle of hole end.
  IDS_PROP_0179,     ///< \ru Тип отверстия. \en Hole type.
  IDS_PROP_0180,     ///< \ru Способ модификации. \en Method of modification.

  IDS_PROP_0181,     ///< \ru Тип(true-бобышка, false- карман). \en Type(true-boss, false- pocket).
  IDS_PROP_0182,     ///< \ru Тип. \en Type.
  IDS_PROP_0183,     ///< \ru Ширина. \en Width.
  IDS_PROP_0184,     ///< \ru Ширина. \en Width.
  IDS_PROP_0185,     ///< \ru Способ. \en Method.
  IDS_PROP_0186,     ///< \ru Новая грань. \en New face.

  IDS_PROP_0188,     ///< \ru Направление (вниз/вверх). \en Direction (down/up).
  IDS_PROP_0189,     ///< \ru Радиус дуги. \en Arc radius.
  IDS_PROP_0190,     ///< \ru Дискриминантная функция. \en Discriminant function.
  IDS_PROP_0191,     ///< \ru Дискриминантная скривая. \en Discriminant cueve.
  IDS_PROP_0192,     ///< \ru Дискриминантная поверхность. \en Discriminant surface.
  IDS_PROP_0193,     ///< \ru Дискриминантная оболочка. \en Discriminant shell.

// \ru Параметры. \en Parameters.  

  IDS_PROP_0201,     ///< \ru Объект на числах double. \en Object on double data.
  IDS_PROP_0202,     ///< \ru Объект на числах float.  \en Object on float data.
  IDS_PROP_0203,     ///< \ru Кривая 1. \en Curve 1.
  IDS_PROP_0204,     ///< \ru Кривая 2. \en Curve 2.
  IDS_PROP_0205,     ///< \ru Кривая 3. \en Curve 3.
  IDS_PROP_0206,     ///< \ru Кривая 4. \en Curve 4
  IDS_PROP_0207,     ///< \ru Начальная точка. \en Start point.
  IDS_PROP_0208,     ///< \ru Направляющий вектор. \en Direction vector.
  IDS_PROP_0209,     ///< \ru Конечная точка. \en End point.
  IDS_PROP_0210,     ///< \ru Нормаль к плоскости.\en Normal to surface.
  IDS_PROP_0211,     ///< \ru Первая полуось. \en First semiaxis.
  IDS_PROP_0212,     ///< \ru Вторая полуось. \en Second semiaxis.
  IDS_PROP_0213,     ///< \ru Фокусное расстояние. \en Focal distance.
  IDS_PROP_0214,     ///< \ru Параметр min. \en Parameter min.
  IDS_PROP_0215,     ///< \ru Параметр max. \en Parameter max.
  IDS_PROP_0216,     ///< \ru Действительная полуось. \en Real semiaxis.
  IDS_PROP_0217,     ///< \ru Мнимая полуось. \en Imaginary semiaxis.
  IDS_PROP_0218,     ///< \ru Приращение начального параметра. \en Increment of start parameter.
  IDS_PROP_0219,     ///< \ru Приращение конечного параметра. \en Increment of end parameter.
  IDS_PROP_0220,     ///< \ru Замкнутость. \en Closedness.
  IDS_PROP_0221,     ///< \ru Порядок. \en Order.
  IDS_PROP_0222,     ///< \ru Количество точек. \en Number of points.
  IDS_PROP_0223,     ///< \ru Точка. \en Point.
  IDS_PROP_0224,     ///< \ru Начальный параметр. \en Start parameter.
  IDS_PROP_0225,     ///< \ru Производная параметра. \en Derivative of parameter.
  IDS_PROP_0226,     ///< \ru Параметрическая длина. \en Parametric length.
  IDS_PROP_0227,     ///< \ru Положительное направление. \en Positive direction.
  IDS_PROP_0228,     ///< \ru Вес. \en Weight.
  // IDS_PROP_0229,     ///< \ru Касание. \en Tangency. (МА 2019 Лишнее?)
  IDS_PROP_0230,     ///< \ru Расстояние. \en Distance.
  IDS_PROP_0232,     ///< \ru Число кривых контура. \en Number of curves of contour.
  IDS_PROP_0233,     ///< \ru Кривая. \en Curve.
  IDS_PROP_0234,     ///< \ru Начальный параметр кривой. \en Start parameter of a curve.
  IDS_PROP_0235,     ///< \ru Конечный параметр кривой. \en End parameter of a curve.
  IDS_PROP_0236,     ///< \ru Число узлов. \en Number of knots.
  IDS_PROP_0237,     ///< \ru Значение узла. \en Knot value.
  IDS_PROP_0238,     ///< \ru Вторая производная в точке. \en Second derivative at a point.
  IDS_PROP_0239,     ///< \ru Производная. \en Derivative.
  IDS_PROP_0240,     ///< \ru Опорная кривая. \en Reference curve.
  IDS_PROP_0241,     ///< \ru Зависимость по X. \en Dependency by X.
  IDS_PROP_0242,     ///< \ru Зависимость по Y. \en Dependency by Y.
  IDS_PROP_0243,     ///< \ru Зависимость по Z. \en Dependency by Z.
  IDS_PROP_0244,     ///< \ru Расширение minPar. \en Extension minPar.
  IDS_PROP_0245,     ///< \ru Расширение maxPar. \en Extension maxPar.
  IDS_PROP_0246,     ///< \ru Количество сплайнов. \en Number of splines.
  IDS_PROP_0250,     ///< \ru Базовая кривая. \en Base curve.
  IDS_PROP_0260,     ///< \ru Двумерная кривая. \en Two-dimensional curve.
  IDS_PROP_0263,     ///< \ru Угол между OX плоскости и прямой касания ее с конусом. \en Angle of OX axis of a plane and the line of its tangency with a cone.
  IDS_PROP_0264,     ///< \ru Расстояние от листовой грани до нейтрального слоя. \en Distance from a sheet face to the neutral layer.
  IDS_PROP_0265,     ///< \ru Угол образующей конуса, касательной к плоскости при разгибе. \en Angle of a cone generatrix tangent to the plane while unbending.
  IDS_PROP_0266,     ///< \ru Количество образующих кривых. \en Number of generating lines.
  IDS_PROP_0267,     ///< \ru Объект модифицирован. \en Object is modified.
  IDS_PROP_0268,     ///< \ru Количество нормалей. \en Number of normals.
  IDS_PROP_0269,     ///< \ru Нормаль. \en Normal.
  IDS_PROP_0270,     ///< \ru Аппроксимационная кривая. \en Approximation curve.

  IDS_PROP_0271,     ///< \ru Удаление выбранных граней. \en Remove selected faces.
  IDS_PROP_0272,     ///< \ru Создание тела из выбранных граней. \en Solid creation by selected faces.
  IDS_PROP_0273,     ///< \ru Перемещение выбранных граней. \en Move selected faces.
  IDS_PROP_0274,     ///< \ru Эквидистантное смещение выбранных граней. \en Offset selected faces.
  IDS_PROP_0275,     ///< \ru Изменение радиусов выбранных скруглений. \en Change selected fillets.
  IDS_PROP_0276,     ///< \ru Замена выбранных граней деформируемыми. \en Replace selected faces by deformed.
  IDS_PROP_0277,     ///< \ru Удаление выбранных скруглений. \en Remove selected features.
  IDS_PROP_0278,     ///< \ru Слияние вершин выбранных ребер. \en Merging vertices of selected edges.
  IDS_PROP_0279,     ///< \ru Замена гладко стыкующихся граней одной гранью. \en Replacing smoothly joined faces with one face.
  IDS_PROP_0280,     ///< \ru Поворот граней. \en Faces rotation.
  IDS_PROP_0281,     ///< \ru Замена граней. \en Faces replacement.

  IDS_PROP_0282,     ///< \ru Вектор модификации. \en The vector of modification.
  IDS_PROP_0283,     ///< \ru Количество модифицированных граней. \en Number of modified faces.
  IDS_PROP_0284,     ///< \ru Положение срединной оболочки тела. \en Position of median shell.
  IDS_PROP_0285,     ///< \ru Минимальное расстояние между гранями. \en Minimal equidistant value.
  IDS_PROP_0286,     ///< \ru Максимальное расстояние между гранями. \en Maximal equidistant value.

  IDS_PROP_0290,     ///< \ru Изменение размера цилиндрической или сферической грани. \en Cylinder or sphere radius change.
  IDS_PROP_0291,     ///< \ru Замена группы граней. \en Replacement group of faces.
  IDS_PROP_0292,     ///< \ru Удаление скруглений меньших радиуса. \en Remove fillets less then radius.
  IDS_PROP_0293,     ///< \ru Удаление замкнутых граней вращения меньших радиуса. \en Remove the closed faces of revolution less then radius.
  IDS_PROP_0294,     ///< \ru Удаление замкнутых граней вращения меньших радиуса c прилегающими скруглениями. \en Remove the closed faces of revolution less then radius with adjacent fillets.
  IDS_PROP_0295,     ///< \ru Удаление выбранных граней c прилегающими скруглениями. \en Remove selected faces with adjacent fillets.
  IDS_PROP_0296,     ///< \ru Изменение радиусов скруглений на заданную величину. \en Change fillet radius by value.
  IDS_PROP_0297,     ///< \ru Установка радиуса для цепочки скруглений. \en Set radius for fillets chain.
  IDS_PROP_0298,     ///< \ru Удаление цепочек скруглений. \en Fillets chain removal.
  IDS_PROP_0299,     ///< \ru Поворот выбранных граней без адаптации скруглений. \en Face rotation without fillet adaptation.
  IDS_PROP_0300,     ///< \ru Перемещение выбранных граней без адаптации скруглений. \en Move selected faces without fillet adaptation.

  IDS_PROP_0301,     ///< \ru Начальный параметр U. \en Start parameter U.
  IDS_PROP_0302,     ///< \ru Начальный параметр V. \en Start parameter V.
  IDS_PROP_0303,     ///< \ru Положительное направление U. \en Positive direction by U.
  IDS_PROP_0304,     ///< \ru Положительное направление V. \en Positive direction by V.
  IDS_PROP_0305,     ///< \ru Параметрическая длина U. \en Parametric length by U.
  IDS_PROP_0306,     ///< \ru Параметрическая длина V. \en Parametric length by V.
  IDS_PROP_0307,     ///< \ru Положительное направление 1. \en Positive direction 1.
  IDS_PROP_0308,     ///< \ru Положительное направление 2. \en Positive direction 2.
  IDS_PROP_0309,     ///< \ru Тип сопряжения. \en Conjugation type.
  IDS_PROP_0310,     ///< \ru Точка. \en Point.
  IDS_PROP_0311,     ///< \ru Замкнутость по U. \en Closedness by U.
  IDS_PROP_0312,     ///< \ru Замкнутость по V. \en Closedness by V.
  IDS_PROP_0313,     ///< \ru Число порций по U. \en Number of portions by U.
  IDS_PROP_0314,     ///< \ru Число порций по V. \en Number of portions by V.
  IDS_PROP_0315,     ///< \ru Порядок по U. \en Order by U.
  IDS_PROP_0316,     ///< \ru Порядок по V. \en Order by V.
  IDS_PROP_0317,     ///< \ru Число точек по U. \en Number of points by U.
  IDS_PROP_0318,     ///< \ru Число точек по V. \en Number of points by V.
  IDS_PROP_0320,     ///< \ru Локальная система. \en Local system.
  IDS_PROP_0321,     ///< \ru Радиус основания. \en Radius of base.
  IDS_PROP_0322,     ///< \ru Высота. \en Height.
  IDS_PROP_0323,     ///< \ru Половина угла. \en Half-angle.
  IDS_PROP_0324,     ///< \ru Большой радиус. \en Major radius.
  IDS_PROP_0325,     ///< \ru Малый радиус. \en Minor radius.
  IDS_PROP_0326,     ///< \ru Длина. \en Length.
  IDS_PROP_0327,     ///< \ru Смещение. \en Shift.
  IDS_PROP_0328,     ///< \ru Форма. \en Shape.
  IDS_PROP_0329,     ///< \ru Закрепление границы поверхности. \en Surface boundary fixation.
  IDS_PROP_0330,     ///< \ru Отличается от базовой поверхности. \en Differs from the base surface.
  IDS_PROP_0331,     ///< \ru Граничное значение Xmin. \en Boundary value Xmin.
  IDS_PROP_0332,     ///< \ru Граничное значение Ymin. \en Boundary value Ymin.
  IDS_PROP_0333,     ///< \ru Граничное значение Xmax. \en Boundary value Xmax.
  IDS_PROP_0334,     ///< \ru Граничное значение Ymax. \en Boundary value Ymax.
  IDS_PROP_0336,     ///< \ru Число узлов по U. \en Number of knots by U.
  IDS_PROP_0337,     ///< \ru Значение U узла. \en Value of U knot.
  IDS_PROP_0338,     ///< \ru Число узлов по V. \en Number of knots by V.
  IDS_PROP_0339,     ///< \ru Значение V узла. \en Value of V knot.
  IDS_PROP_0340,     ///< \ru Поверхность. \en Surface.
  IDS_PROP_0341,     ///< \ru Направляющая кривая. \en Guide curve .
  IDS_PROP_0342,     ///< \ru Образующая кривая. \en Generating curve.
  IDS_PROP_0343,     ///< \ru Вектор смещения. \en Translation vector.
  IDS_PROP_0344,     ///< \ru Вектор направления. \en Direction vector.
  IDS_PROP_0345,     ///< \ru Точка оси вращения. \en Point of the rotation axis.
  IDS_PROP_0346,     ///< \ru Направление оси. \en Axis direction.
  IDS_PROP_0347,     ///< \ru Угол вращения. \en Rotation angle.
  IDS_PROP_0348,     ///< \ru Вершина. \en Vertex.
  IDS_PROP_0350,     ///< \ru Базовая поверхность. \en Base surface.
  IDS_PROP_0351,     ///< \ru Поверхность 1. \en Surface 1.
  IDS_PROP_0352,     ///< \ru Поверхность 2. \en Surface 2.
  IDS_PROP_0353,     ///< \ru Контур. \en Contour.
  IDS_PROP_0354,     ///< \ru Число контуров. \en Number of contours.
  IDS_PROP_0355,     ///< \ru Двумерный контур. \en Two-dimensional contour.
  IDS_PROP_0356,     ///< \ru Двумерная кривая. \en Two-dimensional curve.
  IDS_PROP_0357,     ///< \ru Контур 1. \en Contour 1.
  IDS_PROP_0358,     ///< \ru Контур 2. \en Contour 2.
  IDS_PROP_0360,     ///< \ru Плоскость. \en Plane.
  IDS_PROP_0361,     ///< \ru Вес поверхности 1. \en Weight of surface 1.
  IDS_PROP_0362,     ///< \ru Вес поверхности 2. \en Weight of surface 2.
  IDS_PROP_0363,     ///< \ru Производная в начале. \en Derivative at the beginning.
  IDS_PROP_0364,     ///< \ru Производная в конце. \en Derivative at the end.
  IDS_PROP_0365,     ///< \ru Нормаль в начале. \en The normal at the beginning.
  IDS_PROP_0366,     ///< \ru Нормаль в конце. \en The normal at the end.
  IDS_PROP_0367,     ///< \ru Множитель длины производной в начале. \en The derivative length modifier at the beginning.
  IDS_PROP_0368,     ///< \ru Множитель длины производной в конце. \en The derivative length modifier at the end.
  IDS_PROP_0370,     ///< \ru Кривая на поверхности 0. \en Curve on surface 0.
  IDS_PROP_0371,     ///< \ru Кривая на поверхности 1. \en Curve on surface 1.
  IDS_PROP_0372,     ///< \ru Кривая на поверхности 2. \en Curve on surface 2.
  IDS_PROP_0373,     ///< \ru Кривая вершин. \en Apex Curve.
  IDS_PROP_0374,     ///< \ru Параметр Umin. \en Parameter Umin.
  IDS_PROP_0375,     ///< \ru Параметр Umax. \en Parameter Umax.
  IDS_PROP_0376,     ///< \ru Параметр Vmin. \en Parameter Vmin.
  IDS_PROP_0377,     ///< \ru Параметр Vmax. \en Parameter Vmax.
  IDS_PROP_0378,     ///< \ru Производная dU. \en Derivative dU.
  IDS_PROP_0379,     ///< \ru Производная dV. \en Derivative dV.
  IDS_PROP_0380,     ///< \ru Кромка проходит по Vmin. \en Boundary passes through Vmin.
  IDS_PROP_0384,     ///< \ru Расширение minUPar. \en Extension minUPar.
  IDS_PROP_0385,     ///< \ru Расширение maxUPar. \en Extension maxUPar.
  IDS_PROP_0386,     ///< \ru Расширение minVPar. \en Extension minVPar.
  IDS_PROP_0387,     ///< \ru Расширение maxVPar. \en Extension maxVPar.
  IDS_PROP_0390,     ///< \ru Число кривых. \en Number of curves.
  IDS_PROP_0391,     ///< \ru Кривая по U. \en Curve by U
  IDS_PROP_0392,     ///< \ru Кривая по V. \en Curve by V.
  IDS_PROP_0393,     ///< \ru Число кривых по U. \en Number of curves by U.
  IDS_PROP_0394,     ///< \ru Число кривых по V. \en Number of curves by V.
  IDS_PROP_0395,     ///< \ru Тип поверхности. \en Type of surface.
  IDS_PROP_0397,     ///< \ru Нейтральная плоскость. \en Neutral plane.
  IDS_PROP_0398,     ///< \ru Плоскость контура. \en Plane of the contour.
  IDS_PROP_0399,     ///< \ru Через элементы. \en Through elements.
  IDS_PROP_0400,     ///< \ru Сопряжение на границе. \en Conjugation on the boundary.
  IDS_PROP_0401,     ///< \ru Натяжение на границе. \en Tension on the boundary.
  IDS_PROP_0402,     ///< \ru Параметр определения длины производных. \en Parameter of derivatives length definition.
  IDS_PROP_0403,     ///< \ru Не сохранять длину производной. \en Do not keep the derivative length.
  IDS_PROP_0404,     ///< \ru Тип сопряжения (0-4). \en Conjugation type (0-4).
  IDS_PROP_0405,     ///< \ru Использовать готовый узловой вектор. \en Use prepared knot vector.
  IDS_PROP_0406,     ///< \ru Количество узлов. \en Number of knots.
  IDS_PROP_0407,     ///< \ru Проверка самопересечений. \en Check for self-intersections.
  IDS_PROP_0408,     ///< \ru Используется общий вес точек. \en The common weight of points is used.
  IDS_PROP_0409,     ///< \ru Построена по пласту точек. \en Build from a cloud of points.
  IDS_PROP_0410,     ///< \ru Построена по сети точек. \en Build from a mesh of points.
  IDS_PROP_0411,     ///< \ru В виде набора треугольников. \en As a set of triangles.
  IDS_PROP_0412,     ///< \ru Использовать проекционную кривую. \en Use projection curve.
  IDS_PROP_0413,     ///< \ru Усекать границами. \en Truncate by bounds.
  IDS_PROP_0414,     ///< \ru Привязка к началу. \en Binding to the beginning.
  IDS_PROP_0415,     ///< \ru Соединять скруглениями.\en Join by fillets.
  IDS_PROP_0416,     ///< \ru Сохранять радиус. \en Keep the radius.
  IDS_PROP_0417,     ///< \ru Притуплять острый угол. \en Blunt a sharp angle.
  IDS_PROP_0418,     ///< \ru Проверка пересечений. \en Check for intersections.
  IDS_PROP_0419,     ///< \ru Слияние подобных граней. \en Similar faces merging.
  IDS_PROP_0420,     ///< \ru Слияние подобных ребер. \en Similar edges merging.

  IDS_PROP_0421,     ///< \ru Номер соседнего объекта. \en The number of neighbour object.

  IDS_PROP_0422,     ///< \ru Слияние подобных кривых. \en Similar curves merging.
  IDS_PROP_0423,     ///< \ru Резка кривых. \en Curves cutting.
  IDS_PROP_0424,     ///< \ru Резка ребер. \en Edges cutting.
  IDS_PROP_0425,     ///< \ru Резка поверхностей. \en Surfaces cutting.
  IDS_PROP_0426,     ///< \ru Резка граней. \en Faces cutting.

  IDS_PROP_0427,     ///< \ru Вдоль поверхности. \en Along the surface.

  IDS_PROP_0429,     ///< \ru Площадь. \en Area.

  IDS_PROP_0430,     ///< \ru Способ продления начала. \en Extension way to the start point.
  IDS_PROP_0431,     ///< \ru Способ продления конца. \en Extension way from the end point.
  IDS_PROP_0432,     ///< \ru Длина в начале. \en Length at start point.
  IDS_PROP_0433,     ///< \ru Длина в конце. \en Length at end point.

  IDS_PROP_0434,     ///< \ru Величина изменения радиуса скругления. \en Value of a fillet radius change.

  IDS_PROP_0440,     ///< \ru Круговое сечение поверхности. \en Round section surface.
  IDS_PROP_0441,     ///< \ru Линейное сечение поверхности. \en Line section surface.
  IDS_PROP_0442,     ///< \ru Коническое сечение поверхности. \en Conic section surface.
  IDS_PROP_0443,     ///< \ru Кубическое сечение поверхности. \en Cubic section surface.
  IDS_PROP_0444,     ///< \ru Сплайновое сечение поверхности. \en Spline section surface .
  IDS_PROP_0445,     ///< \ru Поверхность скругления движением диска. \en Fillet disk movement surface.
  IDS_PROP_0446,     ///< \ru Поверхность скругления движением сферы. \en Fillet sphere movement surface.

  IDS_PROP_0450,     ///< \ru Начальный радиус (поверхность). \en Start radius (surface).
  IDS_PROP_0451,     ///< \ru Конечный радиус  (резьба). \en End radius  (thread).
  IDS_PROP_0452,     ///< \ru Длина резьбы. \en Thread length.
  IDS_PROP_0453,     ///< \ru Угол коничности резьбы. \en Taper angle of the thread.

  IDS_PROP_0459,     ///< \ru Количество регионов. \en Number of regions.
  IDS_PROP_0460,     ///< \ru Количество подрегионов. \en Number of subregions.
  IDS_PROP_0461,     ///< \ru Триангуляция. \en Triangulation.
  IDS_PROP_0462,     ///< \ru Количество точек триангуляции. \en Number of points of triangulation.
  IDS_PROP_0463,     ///< \ru Количество двумерных точек. \en Number of two-dimension points.
  IDS_PROP_0464,     ///< \ru Количество точек полигонов. \en Number of points of polygons.
  IDS_PROP_0465,     ///< \ru Количество фасетов. \en Number of facets.

  IDS_PROP_0466,     ///< \ru Форма поверхности сопряжения. \en Shape of blend surface.
  IDS_PROP_0467,     ///< \ru Форма обрезки боков поверхности. \en Shape of cropping the surface sides.
  IDS_PROP_0468,     ///< \ru Обработка опорных граней. \en Initial faces processing.
  IDS_PROP_0469,     ///< \ru Разделять оболочку на грани. \en Division the shell into faces.
  IDS_PROP_0470,     ///< \ru Обрезать опору по касательной. \en Trim the support faces tangentially.

  IDS_PROP_0471,     ///< \ru Сохранять параметризацию. \en Save parameterization.
  IDS_PROP_0472,     ///< \ru Строить на продолжении. \en Build on continuation.

  IDS_PROP_0499,     ///< \ru Есть оболочка-подложка. \en Has the substrate shell.
  IDS_PROP_0500,     ///< \ru Число фасетов. \en Number of facets.
  IDS_PROP_0501,     ///< \ru Число вершин. \en Number of vertices.
  IDS_PROP_0502,     ///< \ru Число ребер. \en Number of edges.
  IDS_PROP_0503,     ///< \ru Число граней. \en Number of faces.
  IDS_PROP_0504,     ///< \ru Ориентация вершины. \en Vertex orientation.
  IDS_PROP_0505,     ///< \ru Ориентация ребра. \en Edge orientation.
  IDS_PROP_0506,     ///< \ru Ориентация грани. \en Face orientation.
  IDS_PROP_0508,     ///< \ru Ребро. \en Edge.
  IDS_PROP_0509,     ///< \ru Грань. \en Face.
  IDS_PROP_0510,     ///< \ru Число циклов. \en Number of loops.
  IDS_PROP_0511,     ///< \ru Цикл. \en Loop.
  IDS_PROP_0512,     ///< \ru Автоопределение. \en Automatic identification.
  IDS_PROP_0513,     ///< \ru Автоматическое. \en Automatic.

  IDS_PROP_0514,     ///< \ru Тип размножения. \en Type of duplication.
  IDS_PROP_0515,     ///< \ru Кол-во шагов. \en Number of steps.
  IDS_PROP_0516,     ///< \ru Шаг. \en Step.
  IDS_PROP_0517,     ///< \ru Количество угловых шагов. \en Number of angular step.
  IDS_PROP_0518,     ///< \ru Элемент. \en Element.
  IDS_PROP_0519,     ///< \ru Сегмент полигональной сетки. \en Segment of polygonal mesh.
  IDS_PROP_0520,     ///< \ru Направляющая поверхность. \en Guide surface.

  IDS_PROP_0521,     ///< \ru Длина  Lx. \en Length  Lx.
  IDS_PROP_0522,     ///< \ru Ширина Ly. \en Width Ly.
  IDS_PROP_0523,     ///< \ru Высота Lz. \en Height Lz.
  IDS_PROP_0524,     ///< \ru Малая длина lx. \en Minor length lx.
  IDS_PROP_0525,     ///< \ru Толщина. \en Thickness.
  IDS_PROP_0526,     ///< \ru Толщина стенки. \en Wall thickness.
  IDS_PROP_0527,     ///< \ru Число вскрытых граней. \en Number of opened faces.
  IDS_PROP_0528,     ///< \ru Форма. \en Shape.
  IDS_PROP_0529,     ///< \ru Сохранять кромку, поверхность или режим автоопределения. \en Keep the boundary, surface or auto mode.
  IDS_PROP_0530,     ///< \ru Продолжить далее. \en Continue.
  IDS_PROP_0531,     ///< \ru Катет 1. \en Cathetus 1.
  IDS_PROP_0532,     ///< \ru Катет 2. \en Cathetus 2.
  IDS_PROP_0533,     ///< \ru Число фасок. \en Number of chamfers.
  IDS_PROP_0534,     ///< \ru Число скруглений. \en Number of fillets.
  IDS_PROP_0535,     ///< \ru Радиус скругления. \en Fillet radius.
  IDS_PROP_0536,     ///< \ru Номер грани. \en Face number.
  IDS_PROP_0537,     ///< \ru Параметр U. \en Parameter U.
  IDS_PROP_0538,     ///< \ru Параметр V. \en Parameter V.
  IDS_PROP_0539,     ///< \ru Число модифицированных граней. \en Number of modified faces.
  IDS_PROP_0540,     ///< \ru Тело. \en Solid.
  IDS_PROP_0541,     ///< \ru Строитель тела. \en Construct solid.
  IDS_PROP_0542,     ///< \ru Количество четырёхугольников. \en Number of quadrangles.
  IDS_PROP_0543,     ///< \ru Остановка от начала. \en Termination from the start.
  IDS_PROP_0544,     ///< \ru Остановка до конца. \en Termination to the end.
  IDS_PROP_0545,     ///< \ru Радиус скругления 1. \en Fillet radius 1.
  IDS_PROP_0546,     ///< \ru Радиус скругления 2. \en Fillet radius 2.
  IDS_PROP_0547,     ///< \ru Коэффициент полноты. \en Coefficient of completeness.
  IDS_PROP_0548,     ///< \ru Способ обработки углов стыковки рёбер. \en Method of processing corners of edges connection. 
  IDS_PROP_0549,     ///< \ru Четырехугольник. \en Quadrangle.

  IDS_PROP_0550,     ///< \ru Базовое тело. \en Base solid.
  IDS_PROP_0551,     ///< \ru Тело 1. \en Solid 1.
  IDS_PROP_0552,     ///< \ru Тело 2. \en Solid 2.
  IDS_PROP_0553,     ///< \ru Исходное тело. \en Initial solid.
  IDS_PROP_0554,     ///< \ru Режущая поверхность. \en Cutting surface.
  IDS_PROP_0555,     ///< \ru Оставляемая часть. \en A part to keep.
  IDS_PROP_0556,     ///< \ru Точка симметрии. \en Symmetry point.
  IDS_PROP_0557,     ///< \ru Ось X симметрии. \en Axis X of symmetry.
  IDS_PROP_0558,     ///< \ru Ось Y симметрии. \en Axis Y of symmetry.
  IDS_PROP_0559,     ///< \ru Поверхность. \en Surface.
  IDS_PROP_0560,     ///< \ru Внешняя оболочка. \en Outer shell.
  IDS_PROP_0561,     ///< \ru Пустотная оболочка. \en Void shell.
  IDS_PROP_0562,     ///< \ru Число пустот. \en Number of voids.
  IDS_PROP_0563,     ///< \ru Глубина 1. \en Depth 1.
  IDS_PROP_0564,     ///< \ru Глубина 2. \en Depth 2.
  IDS_PROP_0565,     ///< \ru Угол уклона 1. \en Slope angle 1.
  IDS_PROP_0566,     ///< \ru Угол уклона 2. \en Slope angle 2.
  IDS_PROP_0567,     ///< \ru Толщина стенки 1. \en Wall thickness 1.
  IDS_PROP_0568,     ///< \ru Толщина стенки 2. \en Wall thickness 2.
  IDS_PROP_0569,     ///< \ru Толщина. \en Thickness.
  IDS_PROP_0570,     ///< \ru Глубина. \en Depth.

  IDS_PROP_0571,     ///< \ru Способ построения. \en Method of construction.
  IDS_PROP_0572,     ///< \ru Число оболочек. \en Number of shells.
  IDS_PROP_0573,     ///< \ru Количество треугольников. \en Number of triangles.
  IDS_PROP_0575,     ///< \ru Угол вращения 1. \en Rotation angle 1.
  IDS_PROP_0576,     ///< \ru Угол вращения 2. \en Rotation angle 2.

  IDS_PROP_0577,     ///< \ru Первая вершина.    \en The first  vertex.
  IDS_PROP_0578,     ///< \ru Вторая вершина.    \en The second vertex.
  IDS_PROP_0579,     ///< \ru Третья вершина.    \en The third  vertex.
  IDS_PROP_0580,     ///< \ru Четвертая вершина. \en The fourth vertex.

  IDS_PROP_0581,     ///< \ru Способ построения 1. \en Method of construction 1.
  IDS_PROP_0582,     ///< \ru Способ построения 2. \en Method of construction 2.
  IDS_PROP_0583,     ///< \ru Расстояние 1. \en Distance 1.
  IDS_PROP_0584,     ///< \ru Расстояние 2. \en Distance 2.
  IDS_PROP_0585,     ///< \ru Треугольник. \en Triangle.
  IDS_PROP_0586,     ///< \ru Количество апексов. \en Number of apices.
  IDS_PROP_0587,     ///< \ru Количество полигонов. \en Number of polygons.
  IDS_PROP_0588,     ///< \ru Количество триангуляций. \en Number of triangulations.
  IDS_PROP_0589,     ///< \ru Контур. \en Contour.
  IDS_PROP_0590,     ///< \ru Число сечений. \en Number of sections.
  IDS_PROP_0591,     ///< \ru Сечение. \en Section.
  IDS_PROP_0592,     ///< \ru Параллельность. \en Parallelization.
  IDS_PROP_0593,     ///< \ru Ориентация образующей. \en Generatrix orientation.
  IDS_PROP_0594,     ///< \ru Положение образующей. \en Generatrix position.
  IDS_PROP_0595,     ///< \ru Сфероид (0) или тороид (1). \en Spheroid (0) or toroid (1).
  IDS_PROP_0596,     ///< \ru Полюс в начале. \en Pole at the beginning.
  IDS_PROP_0597,     ///< \ru Полюс в конце. \en Pole at the end.
  IDS_PROP_0598,     ///< \ru Продолжение. \en Extension.
  IDS_PROP_0599,     ///< \ru Смещение. \en Shift.

  IDS_PROP_0600,     ///< \ru Оболочка тела. \en Shell.
  IDS_PROP_0601,     ///< \ru Ориентация ребра в цикле. \en Sense of edge in the loop.
  IDS_PROP_0602,     ///< \ru Ориентация кривой ребра. \en Edge curve orientation.
  IDS_PROP_0603,     ///< \ru Ориентация нормали оболочки. \en Orientation of a shell normal.
  IDS_PROP_0604,     ///< \ru Кривая ребра. \en Edge curve.
  IDS_PROP_0605,     ///< \ru Двумерная кривая ребра. \en Two-dimensional edge curve.
  IDS_PROP_0606,     ///< \ru Поверхность грани. \en Surface of a face.
  IDS_PROP_0607,     ///< \ru Вершина-начало. \en Start vertex.
  IDS_PROP_0608,     ///< \ru Вершина-конец. \en End vertex.
  IDS_PROP_0609,     ///< \ru Количество ссылок. \en References count.
  IDS_PROP_0611,     ///< \ru Грань плюс. \en Face plus.
  IDS_PROP_0612,     ///< \ru Грань минус. \en Face minus.
  IDS_PROP_0613,     ///< \ru Указатель на грань. \en Pointer to a face.
  IDS_PROP_0614,     ///< \ru Номер по порядку. \en Number by and index.
  IDS_PROP_0615,     ///< \ru Сортировка. \en Sorting.
  IDS_PROP_0616,     ///< \ru Пуансон или матрица. \en Punch or die.
  IDS_PROP_0617,     ///< \ru Скруглять острые ребра инструмента. \en Fillet sharp edges of tool solid.
  IDS_PROP_0651,     ///< \ru Разрезанное тело. \en Cutting solid.
  IDS_PROP_0652,     ///< \ru Плоскость раскроя. \en Cutting plane.
  IDS_PROP_0654,     ///< \ru Наличие штриховки. \en Whether there is hatching.
  IDS_PROP_0655,     ///< \ru Шаг штриховки. \en Hatching step.
  IDS_PROP_0656,     ///< \ru Угол штриховки. \en Hatching angle.
  IDS_PROP_0657,     ///< \ru Проекционная плоскость. \en Projection plane.
  IDS_PROP_0658,     ///< \ru Наличие невидимых линий. \en Whether there are invisible lines.
  IDS_PROP_0659,     ///< \ru Hash имени. \en Hash of name.

  IDS_PROP_0660,     ///< \ru Коэффициент нейтрального слоя. \en Neutral layer coefficient.
  IDS_PROP_0661,     ///< \ru Радиус сгиба. \en Bend radius.
  IDS_PROP_0662,     ///< \ru Угол сгиба. \en Bend angle.
  IDS_PROP_0663,     ///< \ru Длина продолжения сгиба. \en Bend extension length.
  IDS_PROP_0664,     ///< \ru Смещение сгиба. \en Bend shift.
  IDS_PROP_0665,     ///< \ru Отступ от края сгиба 1. \en Distance from the bound of bend 1.
  IDS_PROP_0666,     ///< \ru Отступ от края сгиба 2. \en Distance from the bound of bend 2.
  IDS_PROP_0667,     ///< \ru Угол уклона края сгиба 1. \en Slope angle of the bound of bend 1.
  IDS_PROP_0668,     ///< \ru Угол уклона края сгиба 2. \en Slope angle of the bound of bend 2.
  IDS_PROP_0669,     ///< \ru Угол уклона продолжения сгиба 1. \en Slope angle of bend extension 1.
  IDS_PROP_0670,     ///< \ru Угол уклона продолжения сгиба 2. \en Slope angle of bend extension 2.
  IDS_PROP_0671,     ///< \ru Расширение продолжения сгиба 1. \en Expansion of extension of bend 1.
  IDS_PROP_0672,     ///< \ru Расширение продолжения сгиба 2. \en Expansion of extension of bend 2.
  IDS_PROP_0673,     ///< \ru Ширина разгрузки сгиба. \en Width of bend relief.
  IDS_PROP_0674,     ///< \ru Глубина разгрузки сгиба. \en Depth of bend relief.
  IDS_PROP_0675,     ///< \ru Радиус скругления разгрузки. \en Radius of relief rounding.
  IDS_PROP_0676,     ///< \ru Способ освобождения углов. \en Method of freeing the corners.
  IDS_PROP_0677,     ///< \ru Фиксированная часть грани слева. \en Fixed part of a face on the left.
  IDS_PROP_0678,     ///< \ru Строить разогнутым. \en Build in unbent state.
  IDS_PROP_0679,     ///< \ru Зазор. \en Gap.
  IDS_PROP_0680,     ///< \ru Перехлёстывающая сторона слева. \en Overlapping side on the left.
  IDS_PROP_0681,     ///< \ru С добавлением материала. \en With addition of material.
  IDS_PROP_0682,     ///< \ru Высота. \en Height.
  IDS_PROP_0683,     ///< \ru Коэффициент сгиба 1. \en Coefficient of bend 1.
  IDS_PROP_0684,     ///< \ru Радиус сгиба 1. \en Radius of bend 1.
  IDS_PROP_0685,     ///< \ru Коэффициент сгиба 2. \en Coefficient of bend 2.
  IDS_PROP_0686,     ///< \ru Радиус сгиба 2. \en Radius of bend 2.
  IDS_PROP_0687,     ///< \ru Радиус скругления эскиза. \en Radius of a sketch fillet.
  IDS_PROP_0688,     ///< \ru Радиус скругления основания. \en Radius of a base fillet.
  IDS_PROP_0689,     ///< \ru Радиус скругления дна. \en Radius of a bottom fillet.
  IDS_PROP_0690,     ///< \ru Открытая штамповка. \en Open stamping.
  IDS_PROP_0691,     ///< \ru Боковая стенка внутри. \en Side wall is inside.
  IDS_PROP_0692,     ///< \ru Ширина основания. \en Width of base.
  IDS_PROP_0693,     ///< \ru Ширина выпуклой части. \en Width of a salient part.
  IDS_PROP_0694,     ///< \ru Зазор рубленой законцовки. \en Gap of a cropped tip.
  IDS_PROP_0695,     ///< \ru Тип буртика. \en Bead type.
  IDS_PROP_0696,     ///< \ru Тип законцовки. \en Type of a tip.
  IDS_PROP_0697,     ///< \ru Вытяжка. \en Stretch.
  IDS_PROP_0698,     ///< \ru По нормали к толщине. \en By the normal to thickness.
  IDS_PROP_0699,     ///< \ru Способ замыкания цилиндрических частей. \en Method of cylindric parts closure.
  IDS_PROP_0700,     ///< \ru Разрешение на замыкание углов. \en Permission for corners closure.

  IDS_PROP_0701,     ///< \ru Имя. \en Name.
  IDS_PROP_0702,     ///< \ru Значение. \en Value.
  IDS_PROP_0703,     ///< \ru Положение. \en Position.
  IDS_PROP_0704,     ///< \ru Число. \en Number.
  IDS_PROP_0705,     ///< \ru Ориентация. \en Orientation.
  IDS_PROP_0706,     ///< \ru Длина. \en Length.
  IDS_PROP_0707,     ///< \ru Толщина. \en Thickness.
  IDS_PROP_0708,     ///< \ru Угол. \en Angle.
  IDS_PROP_0709,     ///< \ru Параметр. \en Parameter.
  IDS_PROP_0710,     ///< \ru Геометрический объект. \en Geometric object.
  IDS_PROP_0711,     ///< \ru Точка. \en Point.
  IDS_PROP_0712,     ///< \ru Контрольная кривая. \en Control curve.
  IDS_PROP_0713,     ///< \ru Поверхность. \en Surface.
  IDS_PROP_0714,     ///< \ru Вершина. \en Vertex.
  IDS_PROP_0715,     ///< \ru Ребро грани. \en Edge of a face.
  IDS_PROP_0716,     ///< \ru Цикл грани. \en Face loop.
  IDS_PROP_0717,     ///< \ru Грань. \en Face.
  IDS_PROP_0718,     ///< \ru Полюс при umin. \en Pole at umin.
  IDS_PROP_0719,     ///< \ru Полюс при umax. \en Pole at umax.
  IDS_PROP_0720,     ///< \ru Полюс при vmin. \en Pole at vmin.
  IDS_PROP_0721,     ///< \ru Полюс при vmax. \en Pole at vmax.
  IDS_PROP_0724,     ///< \ru Вершина. \en Vertex.
  IDS_PROP_0725,     ///< \ru Ребро. \en Edge.
  IDS_PROP_0726,     ///< \ru Цикл. \en Loop.
  IDS_PROP_0727,     ///< \ru Грань. \en Face.
  IDS_PROP_0729,     ///< \ru Количество граней. \en Number of faces.
  IDS_PROP_0730,     ///< \ru Количество операций. \en Number of operations.
  IDS_PROP_0731,     ///< \ru Количество объектов. \en Number of objects.
  IDS_PROP_0732,     ///< \ru Объединение граней. \en Faces unification.
  IDS_PROP_0733,     ///< \ru Обработка углов. \en Corners treatment.
  IDS_PROP_0734,     ///< \ru Операция объединения. \en Union operation.
  IDS_PROP_0735,     ///< \ru Операция пересечения. \en Intersection operation.
  IDS_PROP_0736,     ///< \ru Операция разности. \en Subtraction operation.
  IDS_PROP_0737,     ///< \ru Базовая операция. \en Base operations.
  IDS_PROP_0738,     ///< \ru Флаг состояния. \en Flag of state.
  IDS_PROP_0739,     ///< \ru Параметр полюса по U. \en Parameter of a pole by U.
  IDS_PROP_0740,     ///< \ru Номер грани. \en Number of a face.
  IDS_PROP_0741,     ///< \ru Номер ребра. \en Number of an edge.
  IDS_PROP_0742,     ///< \ru Номер грани плюс. \en Number of a face plus.
  IDS_PROP_0743,     ///< \ru Номер грани минус. \en Number of a face minus.
  IDS_PROP_0744,     ///< \ru Формировать твёрдое тело. \en Create a solid.
  IDS_PROP_0745,     ///< \ru Точность сшивки. \en Stitching tolerance.
  IDS_PROP_0746,     ///< \ru Через сгиб. \en Through a bend.
  IDS_PROP_0747,     ///< \ru Полюс. \en Pole.
  IDS_PROP_0748,     ///< \ru Край. \en Border.
  IDS_PROP_0749,     ///< \ru Шов. \en Seam.
  IDS_PROP_0750,     ///< \ru Линия перехода. \en Transition line.
  IDS_PROP_0751,     ///< \ru Адрес начальной вершины. \en Start vertex address.
  IDS_PROP_0752,     ///< \ru Адрес конечной  вершины. \en End vertex address.
  IDS_PROP_0753,     ///< \ru Адрес грани слева. \en Address of a face on the left.
  IDS_PROP_0754,     ///< \ru Адрес грани справа. \en Address of a face on the right.
  IDS_PROP_0755,     ///< \ru Примитив разрезан. \en Primitive is cut.
  IDS_PROP_0756,     ///< \ru Листовой примитив. \en Sheet primitive.
  IDS_PROP_0757,     ///< \ru Внутренняя грань сгиба. \en Internal face of a bend.
  IDS_PROP_0758,     ///< \ru Внешняя грань сгиба. \en External face of a bend.
  IDS_PROP_0759,     ///< \ru Угол раствора конуса. \en Cone angle.

// \ru Версии \en Versions 

  IDS_PROP_0760,     ///< \ru Версия. \en Version.
  IDS_PROP_0761,     ///< \ru Версия имени. \en Version of name.
  IDS_PROP_0762,     ///< \ru Версия операции. \en Version of operation.
  IDS_PROP_0763,     ///< \ru Версия объекта. \en Version of object.

// \ru Информация от геометрической модели \en Information from geometric model 

  IDS_PROP_0771,     ///< \ru Количество вершин. \en Number of vertices.
  IDS_PROP_0772,     ///< \ru Количество кривых. \en Number of curves.
  IDS_PROP_0773,     ///< \ru Количество поверхностей. \en Number of surfaces.
  IDS_PROP_0774,     ///< \ru Количество тел. \en Number of solids.
  IDS_PROP_0775,     ///< \ru Количество полигональных объектов. \en Number of polygonal objects.
  IDS_PROP_0776,     ///< \ru Количество проволочных каркасов. \en Number of wireframes.
  IDS_PROP_0777,     ///< \ru Количество точечных каркасов. \en Number of point frames.
  IDS_PROP_0778,     ///< \ru Количество сборочных единиц. \en Number of assembly units.
  IDS_PROP_0779,     ///< \ru Количество вставок. \en Number of instances.
  IDS_PROP_0780,     ///< \ru Количество других объектов. \en Number of other objects.
  IDS_PROP_0781,     ///< \ru Количество регионов. \en Number of regions.
  IDS_PROP_0782,     ///< \ru Количество элементов. \en Number of elements.
  IDS_PROP_0783,     ///< \ru Количество сегментов. \en Number of segments.
  IDS_PROP_0784,     ///< \ru Количество всех граней. \en Number of all faces.
  IDS_PROP_0785,     ///< \ru Количество всех уникальных граней. \en Number of all unique faces.

  IDS_PROP_0791,     ///< \ru Первая деформация. \en First strain. 
  IDS_PROP_0792,     ///< \ru Вторая деформация. \en Second strain. 
  IDS_PROP_0793,     ///< \ru Третья деформация. \en Third strain. 

  IDS_PROP_0797,     ///< \ru Модуль Юнга. \en Young's modulus. 
  IDS_PROP_0798,     ///< \ru Коэффициент Пуассона. \en Poisson's ratio.

  IDS_PROP_0830,     ///< \ru Число радиусов эквидистант. \en Number of offsets radii.
  IDS_PROP_0831,     ///< \ru Гладкий стык. \en Smooth joint.
  IDS_PROP_0832,     ///< \ru Тип обхода угла. \en Type of corner bypass.
  IDS_PROP_0833,     ///< \ru Радиус специального скругления. \en Radius of a special fillet.
  IDS_PROP_0834,     ///< \ru Тип законцовки. \en Type of a tip.
  IDS_PROP_0835,     ///< \ru Законцовка первого сегмента. \en Tip of the first segment.
  IDS_PROP_0836,     ///< \ru Тип законцовки в начале. \en Type of tip at the beginning.
  IDS_PROP_0837,     ///< \ru Тип законцовки в конце. \en Type of tip at the end.
  IDS_PROP_0838,     ///< \ru Параметр законцовки. \en Parameter of a tip.

  IDS_PROP_0839,     ///< \ru Параметр построения заплатки. \en Parameter of a patch construction.
  IDS_PROP_0840,     ///< \ru Число кривых образующего контура. \en Number of curves of a generating contour.
  IDS_PROP_0841,     ///< \ru Даны образующие грани. \en The generating faces are given.
  IDS_PROP_0842,     ///< \ru Кривая образующего контура. \en Curve of a generating contour.
  IDS_PROP_0843,     ///< \ru Ориентация кривой контура. \en Orientation of a curve of the contour.
  IDS_PROP_0844,     ///< \ru Сторона существующей грани оболочки. \en Side of an existent face of the shell.
  IDS_PROP_0845,     ///< \ru Да. \en Yes.
  IDS_PROP_0846,     ///< \ru Нет. \en No.

// \ru Контейнер атрибутов \en Attribute container 

  IDS_PROP_0847,     ///< \ru Поставщик атрибутов. \en Attribute provider.
  IDS_PROP_0848,     ///< \ru Количество контейнеров. \en Number of containers.
  IDS_PROP_0849,     ///< \ru Контейнер. \en Container.

  IDS_PROP_0851,     ///< \ru Количество атрибутов. \en Number of attributes.

  IDS_PROP_0853,     ///< \ru При изменении. \en While changing.
  IDS_PROP_0854,     ///< \ru При конвертации. \en While convertation.
  IDS_PROP_0855,     ///< \ru При трансформировании. \en While transforming.
  IDS_PROP_0856,     ///< \ru При копировании. \en While copying.
  IDS_PROP_0857,     ///< \ru При объединении. \en While joining.
  IDS_PROP_0858,     ///< \ru При замене. \en While replacing.
  IDS_PROP_0859,     ///< \ru При разделении. \en While splitting.
  IDS_PROP_0860,     ///< \ru При удалении. \en While deleting.
  IDS_PROP_0861,     ///< \ru Объект свободен. \en The object is free.
  IDS_PROP_0862,     ///< \ru Копируемость. \en Whether it can be copied.
  IDS_PROP_0863,     ///< \ru Количество u-линий. \en The number of u-lines. 
  IDS_PROP_0864,     ///< \ru Количество v-линий. \en The number of v-lines. 

  IDS_PROP_0869,     ///< \ru Имя исполнения. \en Embodiment name.
  IDS_PROP_0870,     ///< \ru Имя родительского исполнения. \en Parent embodiment name.
  IDS_PROP_0871,     ///< \ru Красный. \en Red.
  IDS_PROP_0872,     ///< \ru Зелёный. \en Green.
  IDS_PROP_0873,     ///< \ru Синий. \en Blue.
  IDS_PROP_0874,     ///< \ru Толщина. \en Thickness.
  IDS_PROP_0875,     ///< \ru Стиль. \en Style.
  IDS_PROP_0876,     ///< \ru Плотность. \en Density.
  IDS_PROP_0877,     ///< \ru Идентификатор. \en Identifier.
  IDS_PROP_0878,     ///< \ru Селектированность. \en Selectivity.
  IDS_PROP_0879,     ///< \ru Видимость. \en Visibility.
  IDS_PROP_0880,     ///< \ru Изменённость. \en Modified.
  IDS_PROP_0881,     ///< \ru Общий фон. \en Background.
  IDS_PROP_0882,     ///< \ru Диффузное  отражение (компонент red). \en Diffuse reflection (red component).
  IDS_PROP_0883,     ///< \ru Зеркальное отражение. \en Specular reflection.
  IDS_PROP_0884,     ///< \ru Блеск. \en Shininess.
  IDS_PROP_0885,     ///< \ru Непрозрачность. \en Opacity.
  IDS_PROP_0886,     ///< \ru Излучение. \en Emission.
  IDS_PROP_0887,     ///< \ru Количество родительских объектов. \en Number of parent objects.
  IDS_PROP_0888,     ///< \ru Родительский объект. \en Parent object.
  IDS_PROP_0889,     ///< \ru Имя топологического объекта. \en Topological object name.
  IDS_PROP_0890,     ///< \ru Имя объекта. \en Object name.
  IDS_PROP_0891,     ///< \ru Хэш имени объекта. \en Object name hash.
  IDS_PROP_0892,     ///< \ru Диффузное  отражение (компонент green). \en Diffuse reflection (green component).
  IDS_PROP_0893,     ///< \ru Диффузное  отражение (компонент blue). \en Diffuse reflection (blue component).
  IDS_PROP_0894,     ///< \ru Диффузное  отражение (компонент alpha). \en Diffuse reflection (alpha component).
  IDS_PROP_0895,     ///< \ru Зеркальное отражение объекта. \en Specular reflection for an object.

  IDS_PROP_0900,     ///< \ru Сопряжение в точке. \en Conjugation at point.
  IDS_PROP_0901,     ///< \ru Тип сопряжения. \en Conjugation type.
  IDS_PROP_0902,     ///< \ru Без сопряжения. \en Without conjugation.
  IDS_PROP_0903,     ///< \ru По позиции. \en By position.
  IDS_PROP_0904,     ///< \ru По касательной. \en By tangent.
  IDS_PROP_0905,     ///< \ru По нормали. \en By normal.
  IDS_PROP_0906,     ///< \ru По G2. \en By G2.
  IDS_PROP_0907,     ///< \ru По G3. \en By G3.
  IDS_PROP_0908,     ///< \ru Касательный вектор. \en Tangent vector.
  IDS_PROP_0909,     ///< \ru Первая производная касательного вектора. \en First derivative of tangent vector.
  IDS_PROP_0910,     ///< \ru Вторая производная касательного вектора. \en Second derivative of tangent vector.
  IDS_PROP_0911,     ///< \ru Можно ли двигать точки. \en Whether points can be moved.
  IDS_PROP_0912,     ///< \ru Только по направлению. \en Only along the direction.
  IDS_PROP_0913,     ///< \ru Сопряжение. \en Conjugation.

  IDS_PROP_0920,     ///< \ru Тип параметризации. \en Parametrization type.
  IDS_PROP_0921,     ///< \ru Пользовательская. \en Custom.
  IDS_PROP_0922,     ///< \ru Равномерная. \en Uniform.
  IDS_PROP_0923,     ///< \ru По длине хорды. \en By chord length.
  IDS_PROP_0924,     ///< \ru Центростремительная. \en Centripetal.

  IDS_PROP_0925,     ///< \ru Через вершины. \en Through vertices.
  IDS_PROP_0926,     ///< \ru Равномерная параметризация. \en Uniform parametrization.
  IDS_PROP_0927,     ///< \ru Признак текущего исполнения. \en Mark of current embodiment.
  IDS_PROP_0928,     ///< \ru Упрощение граней. \en Faces simplification.
  IDS_PROP_0929,     ///< \ru Разделять оболочку на грани. \en Divide the shell into faces.
  IDS_PROP_0930,     ///< \ru Допустимое несовпадение. \en Permissible mismatch.
  IDS_PROP_0931,     ///< \ru Совпадение направлений. \en Directions matching.

  IDS_PROP_0932,     ///< \ru Исходный проволочный каркас. \en Initial wire frame.
  IDS_PROP_0933,     ///< \ru Признак строгого построения уклона 1. \en Sign of strict construction a slope angle 1.
  IDS_PROP_0934,     ///< \ru Признак строгого построения уклона 2. \en Sign of strict construction a slope angle 2.
  IDS_PROP_0935,     ///< \ru Направляющий проволочный каркас. \en Guide wire frame.

// \ru Конвертеры \en Converters 

  IDS_PROP_1000,     ///< \ru Заголовок. \en Title.
  IDS_PROP_1001,     ///< \ru Название. \en Name.
  IDS_PROP_1002,     ///< \ru Дата и время. \en Date and time.
  IDS_PROP_1003,     ///< \ru Автор(ы). \en Author(s)).
  IDS_PROP_1004,     ///< \ru Организация(и). \en Organization(s).
  IDS_PROP_1005,     ///< \ru Процессор STEP. \en Processor STEP.
  IDS_PROP_1006,     ///< \ru Система. \en System.
  IDS_PROP_1007,     ///< \ru Авторизация. \en Authorization.

  IDS_PROP_1010,     ///< \ru Лицо и организация. \en Person and organization.
  IDS_PROP_1011,     ///< \ru Идентификатор лица. \en Identifier of a person.
  IDS_PROP_1012,     ///< \ru Фамилия. \en Surname.
  IDS_PROP_1013,     ///< \ru Имя. \en Name.
  IDS_PROP_1014,     ///< \ru Средние имена. \en Middle names.
  IDS_PROP_1015,     ///< \ru Титулы предшествующие. \en Prefix titles.
  IDS_PROP_1016,     ///< \ru Титулы завершающие. \en Suffix titles.
  IDS_PROP_1017,     ///< \ru Идентификатор организации. \en Identifier of organization.
  IDS_PROP_1018,     ///< \ru Название организации. \en Name of organization.
  IDS_PROP_1019,     ///< \ru Описание организации. \en Description of organization.

  IDS_PROP_1030,     ///< \ru Изделие. \en Product.
  IDS_PROP_1031,     ///< \ru Идентификатор. \en Identifier.
  IDS_PROP_1032,     ///< \ru Название. \en Name.
  IDS_PROP_1033,     ///< \ru Описание. \en Description.

  IDS_PROP_1043,     ///< \ru Элемент описания. \en Description element.

// \ru 1100 .. 1199 зарезервированы для решателя геометрических ограничений. \en 1100 .. 1199 is a range for C3D Solver.

  IDS_PROP_1100,     ///< \ru Геометрический решатель. \en Geom solver.
  IDS_PROP_1101,     ///< \ru Схема сопряжений. \en Scheme of matings.
  IDS_PROP_1102,     ///< \ru Система ограничений. \en Constraint system.    
  IDS_PROP_1103,     ///< \ru Ограничение. \en Constraint.
  IDS_PROP_1104,     ///< \ru Тип сопряжения. \en Type of mating.
  IDS_PROP_1105,     ///< \ru Тип ограничения. \en Type of constraint.
  IDS_PROP_1106,     ///< \ru Выравнивание. \en Alignment.
  IDS_PROP_1107,     ///< \ru Количество ограничений. \en Number of constraints.
  IDS_PROP_1108,     ///< \ru Тип взаимоориентации. \en Coorientation type.  
  IDS_PROP_1109,     ///< \ru Сопряжение. \en Mate.
  IDS_PROP_1110,     ///< \ru Базовый объект. \en Base object.
  IDS_PROP_1111,     ///< \ru Объект 1. \en Object 1.
  IDS_PROP_1112,     ///< \ru Объект 2. \en Object 2.
  IDS_PROP_1113,     ///< \ru Вещественный параметр. \en Real parameter.
  IDS_PROP_1114,     ///< \ru Величина взаимоориентации. \en Value of coorientation.

// \ru Типы геометрических ограничений. \en Types of geometric constraint.

  IDS_PROP_1130,     ///< \ru Совпадение \en Coincident  
  IDS_PROP_1131,     ///< \ru Параллельность \en Parallel
  IDS_PROP_1132,     ///< \ru Перпендикулярность \en Perpendicular
  IDS_PROP_1133,     ///< \ru Касание \en Tangent.
  IDS_PROP_1134,     ///< \ru Концентричность \en Concentric
  IDS_PROP_1135,     ///< \ru На расстоянии \en Distance
  IDS_PROP_1136,     ///< \ru По углом \en Angle
  IDS_PROP_1137,     ///< \ru По месту \en In place
  IDS_PROP_1138,     ///< \ru Механическая передача \en Transmittion
  IDS_PROP_1139,     ///< \ru Кулачковый механизм \en Cam mechanism
  IDS_PROP_1140,     ///< \ru Радиальный размер. \en Radial dimension.
  IDS_PROP_1141,     ///< \ru Точка по проценту. \en Percent point.
  IDS_PROP_1142,     ///< \ru Размер смещения. \en Offset dimension.
  IDS_PROP_1145,     ///< \ru Симметричность. \en Symmetric.
  IDS_PROP_1146,     ///< \ru Зависимый объект. \en Dependent.
  IDS_PROP_1147,     ///< \ru Элемент паттерна. \en Patterned.
  IDS_PROP_1148,     ///< \ru Линейный паттерн. \en Linear pattern.
  IDS_PROP_1149,     ///< \ru Угловой паттерн. \en Angular pattern.
  IDS_PROP_1150,     ///< \ru Фиксация. \en Fixing.
  IDS_ITEM_1151,     // "Горизонтальность"
  IDS_ITEM_1152,     // "Вертикальность"
  IDS_ITEM_1153,     // "Длина"
  IDS_ITEM_1154,     // "Угол наклона"
  IDS_PROP_1155,     // "СК паттерн."
  IDS_PROP_1156,     // "Координата СК паттерна."
  IDS_PROP_1157,     // "Опция масштабируемости паттерна GCM_scale."
  IDS_PROP_1158,     // "Паттерн для пары кривых"
  IDS_PROP_1159,     ///< \ru Линейный размер \en Linear dimension
  IDS_PROP_1199,     // The last id for C3D Solver

// \ru Новые описания без группировки \en New unsorted descriptions

  IDS_PROP_2001,     ///< \ru Внимание: \en Attention:
  IDS_PROP_2002,     ///< \ru Начало общих операций. \en Beginning of the shared operations.
  IDS_PROP_2003,     ///< \ru Начало группы операций. \en Beginning of the operations group.
  IDS_PROP_2004,     ///< \ru Начало первой группы операций. \en Beginning of the first operations group.
  IDS_PROP_2005,     ///< \ru Начало второй группы операций. \en Beginning of the second operations group.
  IDS_PROP_2006,     ///< \ru Начало объекта. \en Beginning of an object.
  IDS_PROP_2007,     ///< \ru Начало первого объекта. \en Beginning of the first object.
  IDS_PROP_2008,     ///< \ru Начало второго объекта. \en Beginning of the second object.
  IDS_PROP_2009,     ///< \ru Копировать атрибуты. \en Copy attributes.
  IDS_PROP_2010,     ///< \ru Количество общих групп операций. \en Number of shared operations groups.
  IDS_PROP_2011,     ///< \ru Количество выбранных граней. \en Number of selected faces.
  IDS_PROP_2012,     ///< \ru Количество выбранных ребёр. \en Number of selected edges.
  IDS_PROP_2013,     ///< \ru Количество выбранных вершин. \en Number of selected vertices.
  IDS_PROP_2014,     ///< \ru Секущий эскиз. \en Cutting sketch.
  IDS_PROP_2015,     ///< \ru Секущие 3D-кривые. \en Cutting 3D-curves.
  IDS_PROP_2016,     ///< \ru Секущие поверхности. \en Cutting surfaces.
  IDS_PROP_2017,     ///< \ru Секущее тело. \en Cutting solid.

  IDS_PROP_2018,     ///< \ru Поверхность сопряжения на границе 0. \en Adjacent surface on the border 0.
  IDS_PROP_2019,     ///< \ru Поверхность сопряжения на границе 1. \en Adjacent surface on the border 1.
  IDS_PROP_2020,     ///< \ru Поверхность сопряжения на границе 2. \en Adjacent surface on the border 2.
  IDS_PROP_2021,     ///< \ru Поверхность сопряжения на границе 3. \en Adjacent surface on the border 3.
  IDS_PROP_2022,     ///< \ru Поверхность сопряжения на границе 4. \en Adjacent surface on the border 4.
  IDS_PROP_2023,     ///< \ru Сопряжение на границе 0. \en Conjugation on the boundary 0.
  IDS_PROP_2024,     ///< \ru Сопряжение на границе 1. \en Conjugation on the boundary 1.
  IDS_PROP_2025,     ///< \ru Сопряжение на границе 2. \en Conjugation on the boundary 2.
  IDS_PROP_2026,     ///< \ru Сопряжение на границе 3. \en Conjugation on the boundary 3.
  IDS_PROP_2027,     ///< \ru Сопряжение на границе 4. \en Conjugation on the boundary 4.
  
  IDS_PROP_2028,     ///< \ru Количество ребер сгиба. \en Count of bend edges.
  IDS_PROP_2029,     ///< \ru Количество ребер стыка. \en Count of rip edges.

  IDS_ITEM_2030,     ///< \ru Тип входных данных. \en Type of input data.
  IDS_ITEM_2031,     ///< \ru Тип изменений. \en Change type.

  IDS_PROP_2032,     ///< \ru Число индексов частей скругления. \en Number of indices of fillet parts.
  IDS_PROP_2033,     ///< \ru Индекс части скругления. \en Index of fillet part.
  IDS_PROP_2034,     ///< \ru Опция выбора результата. \en Option for the result selection.

  IDS_PROP_2035,     ///< \ru Режим адаптации. \en Adaptation mode.

  IDS_PROP_2036,     ///< \ru Создавать проекции на поверхностях параллельных вектору проецирования. \en Project onto surfaces parallel to a direction of the vector projection.

  IDS_PROP_LAST = 9999, ///< \ru Наибольшее значение. \en The greatest value.
};


#endif // __MB_PROPERTY_TITLE_H
