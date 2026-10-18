////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Геометрический объект в трехмерном пространстве.
         \en Geometrical object in three-dimensional space.

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __SPACE_ITEM_H
#define __SPACE_ITEM_H


#include <io_tape_define.h>
#include <reference_item.h>
#include <templ_rp_array.h>
#include <item_registrator.h>
#include <mb_variables.h>
#include <mb_data.h>
#include <math_define.h>
#include <mb_property_title.h>


class   MbVector3D;
class   MbCartPoint3D;
class   MbMatrix3D;
class   MbAxis3D;
class   MbCube;
class   MbProperty;
class   MbProperties;
class   MbMesh;
struct  MbControlData3D;
class   MbStepData;
class             MbRegTransform;
class             MbRegDuplicate;


class   MbSpaceItem;
namespace c3d // namespace C3D
{
typedef SPtr<MbSpaceItem>                        SpaceItemSPtr;             ///< \ru Умный указатель на геометрический объект. \en Smart pointer of an geometrical object. 
typedef SPtr<const MbSpaceItem>                  ConstSpaceItemSPtr;        ///< \ru Умный указатель на геометрический объект. \en Smart pointer of an geometrical object. 
typedef std::pair<MbSpaceItem *, MbSpaceItem *>  SpaceItemPair;             ///< \ru Пара геометрических объектов. \en Pair of geometrical objects. 

typedef std::vector<MbSpaceItem *>               SpaceItemsVector;          ///< \ru Вектор геометрических объектов. \en Vector of geometrical objects. 
typedef std::vector<const MbSpaceItem *>         ConstSpaceItemsVector;     ///< \ru Вектор геометрических объектов. \en Vector of geometrical objects. 

typedef std::vector<SpaceItemSPtr>               SpaceItemsSPtrVector;      ///< \ru Вектор геометрических объектов. \en Vector of geometrical objects. 
typedef std::vector<ConstSpaceItemSPtr>          ConstSpaceItemsSPtrVector; ///< \ru Вектор геометрических объектов. \en Vector of geometrical objects. 
}


//------------------------------------------------------------------------------
/** \brief \ru Типы геометрических объектов в трёхмерном пространстве.
           \en Types of spatial geometric objects. \~
  \details \ru Геометрические объекты группируются в семейства:
               точки, кривые, поверхности объекты модели и вспомогательные объекты.
           \en Geometric objects are grouped into families:
               points, curves, surfaces, model objects and ancillary facilities. \~
  \ingroup Geometric_Items
*/
// ---
enum MbeSpaceType {

  st_Undefined                =   0,  ///< \ru Неизвестный объект. \en Unknown object.
  st_SpaceItem                =   1,  ///< \ru Геометрический объект. \en Geometric object. \n 

  // \ru Типы точек. \en Point types. 
  st_Point3D                  = 101,  ///< \ru Точка. \en Point.
  st_FreePoint3D              = 200,  ///< \ru Тип для точек, созданных пользователем. \en Type for the user-defined points. \n 

  // \ru Типы кривых. \en Curve types. 
  st_Curve3D                  = 201,  ///< \ru Кривая. \en Curve.
  st_Line3D                   = 202,  ///< \ru Прямая. \en Line.
  st_LineSegment3D            = 203,  ///< \ru Отрезок прямой. \en Line segment.
  st_Arc3D                    = 204,  ///< \ru Окружность, эллипс, дуга. \en Circle, ellipse, arc.
  st_Spiral                   = 205,  ///< \ru Спираль. \en Spiral.
  st_ConeSpiral               = 206,  ///< \ru Коническая спираль. \en Conical spiral.
  st_CurveSpiral              = 207,  ///< \ru Спираль по образующей кривой. \en Spiral curve constructed by generatrix.
  st_CrookedSpiral            = 208,  ///< \ru Спираль по направляющей кривой. \en Spiral along the guide curve.
  st_PolyCurve3D              = 209,  ///< \ru Кривая, построенная по точкам. \en Curve constructed by points.
  st_Polyline3D               = 210,  ///< \ru Полилиния. \en Polyline.
  st_Nurbs3D                  = 211,  ///< \ru NURBS кривая. \en NURBS curve.
  st_Bezier3D                 = 212,  ///< \ru Кривая Безье. \en Bezier curve.
  st_Hermit3D                 = 213,  ///< \ru Составной кубический сплайн Эрмита. \en Composite Hermit cubic spline.
  st_CubicSpline3D            = 214,  ///< \ru Кубический сплайн. \en Cubic spline.
  st_PlaneCurve               = 215,  ///< \ru Плоская кривая в пространстве. \en Plane curve in space.
  st_OffsetCurve3D            = 216,  ///< \ru Эквидистантная кривая. \en Offset curve.
  st_TrimmedCurve3D           = 217,  ///< \ru Усеченная кривая. \en Truncated curve.
  st_ReparamCurve3D           = 218,  ///< \ru Репараметризованная кривая. \en Reparametrized curve.
  st_BridgeCurve3D            = 219,  ///< \ru Кривая-мостик, соединяющая две кривые. \en Curve as a bridge connecting two curves.
  st_CharacterCurve3D         = 220,  ///< \ru Кривая, координатные функции которой заданы в символьном виде. \en Functionally defined curve.
  st_ContourOnSurface         = 221,  ///< \ru Контур на поверхности. \en Contour on the surface.
  st_ContourOnPlane           = 222,  ///< \ru Контур на плоскости. \en Contour on the plane.
  st_SurfaceCurve             = 223,  ///< \ru Кривая на поверхности. \en Curve on the surface.
  st_SilhouetteCurve          = 224,  ///< \ru Силуэтная кривая. \en Silhouette curve.
  st_SurfaceIntersectionCurve = 225,  ///< \ru Кривая пересечения поверхностей. \en Curve as intersection of surfaces.
  st_BSpline                  = 226,  ///< \ru В-сплайн. \en B-spline.
  st_Contour3D                = 227,  ///< \ru Контур. \en Contour.
  st_CoonsDerivative          = 228,  ///< \ru Кривая производных поверхности Кунса. \en Curve of Coons surface derivetives.
  st_FreeCurve3D              = 300,  ///< \ru Тип для кривых, созданных пользователем. \en Type for the user-defined curves. \n 

  // \ru Типы поверхностей. \en Surface types. 
  st_Surface                  = 301,  ///< \ru Поверхность. \en Surface.
  st_ElementarySurface        = 302,  ///< \ru Элементарная поверхность. \en Elementary surface.
  st_Plane                    = 303,  ///< \ru Плоскость. \en Plane.
  st_ConeSurface              = 304,  ///< \ru Коническая поверхность. \en Conical surface.
  st_CylinderSurface          = 305,  ///< \ru Цилиндрическая поверхность. \en Cylindrical surface.
  st_SphereSurface            = 306,  ///< \ru Сфера. \en Sphere.
  st_TorusSurface             = 307,  ///< \ru Тор. \en Torus.
  st_SweptSurface             = 308,  ///< \ru Поверхность движения. \en Swept surface.
  st_ExtrusionSurface         = 309,  ///< \ru Поверхность перемещения. \en Extrusion surface.
  st_RevolutionSurface        = 310,  ///< \ru Поверхность вращения. \en Revolution surface.
  st_EvolutionSurface         = 311,  ///< \ru Поверхность заметания. \en Swept surface with guide curve.
  st_ExactionSurface          = 312,  ///< \ru Поверхность заметания с поворотными торцами. \en Swept surface with rotating ends.
  st_ExpansionSurface         = 313,  ///< \ru Плоскопараллельная поверхность. \en Plane-parallel swept surfaces.
  st_SpiralSurface            = 314,  ///< \ru Спиральная поверхность. \en Spiral surface.
  st_RuledSurface             = 315,  ///< \ru Линейчатая поверхность. \en Ruled surface.
  st_SectorSurface            = 316,  ///< \ru Секториальная поверхность. \en Sectorial surface.
  st_PolySurface              = 317,  ///< \ru Поверхность, определяемая точками. \en Surface constructed by points.
  st_HermitSurface            = 318,  ///< \ru Hermit поверхность, определяемая точками. \en Hermit surface.
  st_SplineSurface            = 319,  ///< \ru NURBS поверхность, определяемая точками. \en NURBS surface.
  st_GridSurface              = 320,  ///< \ru Поверхность, определяемая точками. \en Surface defined by points.
  st_TriBezierSurface         = 321,  ///< \ru Треугольная Bezier поверхность, определяемая точками. \en Triangular Bezier surface.
  st_SimplexSurface           = 322,  ///< \ru Треугольная NURBS поверхность, определяемая точками. \en Triangular NURBS surface.
  st_OffsetSurface            = 323,  ///< \ru Эквидистантная поверхность. \en Offset surface.
  st_DeformedSurface          = 324,  ///< \ru Деформированная поверхность. \en Deformed surface.
  st_NurbsSurface             = 325,  ///< \ru NURBS поверхность, определяемая кривыми. \en NURBS surface defined by curves.
  st_CornerSurface            = 326,  ///< \ru Поверхность по трем кривым. \en The surface based on three curves.
  st_CoverSurface             = 327,  ///< \ru Поверхность по четырем кривым. \en The surface based on the four curves.
  st_CoonsPatchSurface        = 328,  ///< \ru Бикубическая поверхность Кунса по четырем кривым. \en Bicubic Coons surface constructed by four curves.
  st_GregoryPatchSurface      = 329,  ///< \ru Поверхность Грегори по четырем кривым. \en Gregory surface constructed by four curves.
  st_LoftedSurface            = 330,  ///< \ru Поверхность, проходящая через заданное семейство кривых. \en Lofted surface.
  st_ElevationSurface         = 331,  ///< \ru Поверхность, проходящая через заданное семейство кривых, с направляющей. \en Lofted surface with the guide.
  st_MeshSurface              = 332,  ///< \ru Поверхность на сетке кривых. \en The surface constructed by the grid curves.
  st_GregorySurface           = 333,  ///< \ru Поверхность на ограничивающем контуре. \en The surface on the bounding contour.
  st_SmoothSurface            = 334,  ///< \ru Поверхность сопряжения. \en Conjugation surface.
  st_ChamferSurface           = 335,  ///< \ru Поверхность фаски. \en The surface of the bevel.
  st_FilletSurface            = 336,  ///< \ru Поверхность скругления. \en Fillet surface.
  st_ChannelSurface           = 337,  ///< \ru Поверхность скругления с переменным радиусом. \en Fillet surface with variable radius.
  st_FullFilletSurface        = 338,  ///< \ru Поверхность полного скругления. \en Full fillet surface.
  st_JoinSurface              = 339,  ///< \ru Поверхность соединения. \en The surface of the joint.
  st_CurveBoundedSurface      = 340,  ///< \ru Ограниченная кривыми поверхность. \en The surface bounded by curves.
  st_BendedUnbendedSurface    = 341,  ///< \ru Поверхность, полученная сгибом/разгибом. \en Surface constructed by fold / unbending.
  st_CylindricBendedSurface   = 342,  ///< \ru Поверхность, полученная цилиндрическим сгибом. \en Surface constructed by cylindrical fold.
  st_CylindricUnbendedSurface = 343,  ///< \ru Поверхность, полученная цилиндрическим разгибом. \en Surface constructed by cylindrical unbending.
  st_ConicBendedSurface       = 344,  ///< \ru Поверхность, полученная коническим сгибом. \en Surface constructed by conical fold.
  st_ConicUnbendedSurface     = 345,  ///< \ru Поверхность, полученная коническим разгибом. \en Surface constructed by conical unbending.
  st_GregoryRibbonPatchSurface= 346,  ///< \ru Поверхность Грегори с граничными условиями. \en Gregory patch surface with ribbons.
  st_ExplorationSurface       = 347,  ///< \ru Поверхность заметания с масштабированием и поворотом образующей кривой. \en Swept surface with scaling and winding of generating curve.
  st_SectionSurface           = 348,  ///< \ru Поверхность заметания переменного сечения. \en The swept mutable section surface.
  st_SimplexSpline            = 349,  ///< \ru Поверхность, моделирующая симплексный сплайн.
  st_RuledBendedUnbendedSurface=350,  ///< \ru Поверхность, полученная линейчатым сгибом/разгибом. \en Surface constructed by ruled fold / unbending.
  st_FreeSurface              = 400,  ///< \ru Тип для поверхностей, созданных пользователем. \en Type for the user-defined surfaces. \n 

  // \ru Типы вспомогательных объектов. \en Helper object types. 
  st_Legend                   = 401,  ///< \ru Вспомогательный объект. \en The helper object.
  st_Marker                   = 402,  ///< \ru Точка и двойка ортонормированных векторов (применяется в сопряжениях, в кинематике). \en Point and two orthonormal vectors.
  st_Thread                   = 403,  ///< \ru Резьба. \en Thread.
  st_Symbol                   = 404,  ///< \ru Условное обозначение. \en Symbol.
  st_PointsSymbol             = 405,  ///< \ru Условное обозначение на базовых точках. \en Symbol on the basic points.
  st_Rough                    = 406,  ///< \ru Обозначение шероховатости. \en Designation of roughness.
  st_Leader                   = 407,  ///< \ru Обозначение линии выноски. \en Designation of the leader line.
  st_Dimension3D              = 408,  ///< \ru Размер. \en Dimension
  st_LinearDimension3D        = 409,  ///< \ru Линейный размер. \en Linear dimension.
  st_DiameterDimension3D      = 410,  ///< \ru Диаметральный размер. \en Diameter dimension. 
  st_RadialDimension3D        = 411,  ///< \ru Радиальный размер. \en Radial dimension. 
  st_AngularDimension3D       = 412,  ///< \ru Угловой размер. \en Angular dimension.
  st_PMI                      = 413,  ///< \ru Элемент аннотации. \en Annotation item..
  st_FreeLegend               = 500,  ///< \ru Тип для вспомогательных объектов, созданных пользователем. \en Type for the user helper objects. \n 

  // \ru Типы объектов модели геометрического ядра с журналом построения и атрибутами. \en Model object types with history tree and attributes. 
  st_Item                     = 501,  ///< \ru Геометрический объект модели. \en Model object.
  st_AssistedItem             = 502,  ///< \ru Локальная система координат. \en The local coordinate system.
  st_PointFrame               = 503,  ///< \ru Точечный каркас. \en Point frame.
  st_WireFrame                = 504,  ///< \ru Проволочный каркас. \en Wire frame.
  st_Solid                    = 505,  ///< \ru Твердое тело. \en Solid.
  st_Instance                 = 506,  ///< \ru Объект модели в локальной системе координат. \en The model object in the local coordinate system.
  st_Assembly                 = 507,  ///< \ru Сборочная единица объектов модели. \en Assembly unit of model objects.
  st_Mesh                     = 508,  ///< \ru Полигональный объект в виде точек, ломаных и пластин. \en Polygonal form of an object as a set of points, polylines, and plates.
  st_SpaceInstance            = 509,  ///< \ru Обертка над геометрическим объектом MbSpaceItem. \en Wrapper over a geometry MbSpaceItem.
  st_PlaneInstance            = 510,  ///< \ru Обертка над плоским объектом MbPlaneItem. \en Wrapper over a flat object MbPlaneItem.
  st_Collection               = 511,  ///< \ru Коллекция элементов. \en Collection of elements. \n
  st_Polymesh                 = 512,  ///< \ru Полигональный объект с топологией. \en Polygonal object with topology.

  st_FreeItem                 = 600,  ///< \ru Тип для объектов, созданных пользователем. \en Type for the user-defined objects.

};


//------------------------------------------------------------------------------
/** \brief \ru Геометрический объект в трехмерном пространстве. 
           \en Geometrical object in three-dimensional space. \~
  \details \ru Родительский класс геометрических объектов в трехмерном пространстве.
               Наследниками являются: точка MbPoint3D, кривая MbCurve3D, поверхность MbSurface, 
               объект геометрической модели MbItem и другие вспомогательные объекты.\n
               Объект имеет счетчик ссылок и умеет записываться и читаться.
           \en The parent class of geometric objects in three dimensions.
               Heirs are: point MbPoint3D, curve MbCurve3D, surface MbSurface, 
               geometric object model and MbItem other ancillary facilities.\n
               The object has a reference count and can be written and read.\n \~
  \ingroup Geometric_Items
*/
// ---
class MATH_CLASS MbSpaceItem : public TapeBase, public MbRefItem {
protected :
  /// \ru Конструктор без параметров. \en Default constructor.
  MbSpaceItem();
public :
  /// \ru Деструктор. \en Destructor.
  virtual ~MbSpaceItem();

  /** \ru \name Общие функции геометрического объекта.
      \en \name General functions of a geometric object.
  \{ */  
  /// \ru Получить регистрационный тип (для копирования, дублирования). \en Get a registration type (for copying, duplication).
  MbeRefType   RefType() const override;
  /// \ru Получить тип объекта. \en Get the type of the object.
  virtual MbeSpaceType IsA() const = 0;
  /// \ru Получить групповой тип объекта. \en Get the group object type.
  virtual MbeSpaceType Type() const = 0;
  /// \ru Получить семейство объекта. \en Get family of objects.
  virtual MbeSpaceType Family() const = 0;

  /// \ru Принадлежит ли объект к регистрируемому семейству. \en Whether the object belongs to a registrable family.
  bool IsFamilyRegistrable() const override;

  /** \brief \ru Создать копию объекта.
             \en Create a copy of the object. \~
    \details \ru Создать копию объекта с использованием регистратора. 
                 Регистратор используется для предотвращения многократного копирования объекта.
                 Если объект содержит ссылки на другие объекты, то вложенные объекты так же копируются.
                 Допустимо не передавать регистратор в функцию. Тогда будет создана новая копия объекта.
                 При копировании одиночного объекта или набора не связанных между собой объектов допустимо не использовать регистратор.
                 Регистратор необходимо использовать, если надо последовательно копировать несколько взаимосвязанных объектов.
                 Возможно, что связь объектов обусловлена наличием в них ссылок на общие объекты. 
                 Тогда, при копировании без использования регистратора, можно получить набор копий, 
                 содержащих ссылки на разные копии одного и того же вложенного объекта, что ведет к потере связи между копиями.
             \en Create a copy of an object using the registrator. 
                 Registrator is used to prevent multiple copy of the object.
                 If the object contains references to the other objects, then nested objects are copied as well.
                 It is allowed not to pass registrator into the function. Then new copy of object will be created.
                 While copying of single object or set of not connected objects, it is allowed not to use registrator.
                 Registrator should be used if it is required to copy several connected objects.
                 It is possible, that objects connection is based on the references to common objects.
                 Then, while copying without using of registrator, it is possible to obtain set of copies, 
                 that contain references to the different copies of the same nested object, that leads to loss of connection between copies.\~
    \param[in] iReg - \ru Регистратор.
                      \en Registrator. \~
    \return \ru Копия объекта.
            \en A copy of the object.
  */
  virtual MbSpaceItem & Duplicate( MbRegDuplicate * iReg = nullptr ) const = 0;

  /** \brief \ru Преобразовать объект согласно матрице.
             \en Convert the object according to the matrix. \~
    \details \ru Преобразовать исходный объект согласно матрице c использованием регистратора. 
                 Если объект содержит ссылки на другие геометрические объекты, то вложенные объекты так же преобразуются согласно матрице.
                 Регистратор служит для предотвращения многократного преобразования объекта.
                 Допустимо использовать функцию без регистратора, если надо преобразовать одиночный объект.
                 Если надо преобразовать набор взаимосвязанных объектов, необходимо использовать регистратор для 
                 предотвращения повторного преобразования вложенных объектов, поскольку не исключена ситуация,
                 когда несколько объектов из набора содержат ссылки на один или несколько общих объектов, подлежащих трансформации.
             \en Convert the original object according to the matrix using the registrator. 
                 If object contains references to the other geometric objects, then nested objects are transformed according to the matrix.
                 Registrator is needed to prevent multiple object copying.
                 It is allowed to use function without registrator, if it is needed to transform single object.
                 If it is needed to transform a set of connected objects, then one should use registrator 
                 to prevent repeating transformation of nested objects, because of the possible situation 
                 when several objects contain references to the same common objects, that require to be transformed.\~
    \param[in] matr - \ru Матрица преобразования.
                      \en Transformation matrix. \~
    \param[in] iReg - \ru Регистратор.
                      \en Registrator.
  */
  virtual void    Transform( const MbMatrix3D & matr, MbRegTransform * iReg = nullptr ) = 0;

  /** \brief \ru Сдвинуть объект вдоль вектора.
             \en Move an object along a vector. \~
    \details \ru Сдвинуть геометрический объект вдоль вектора с использованием регистратора. 
                 Если объект содержит ссылки на другие геометрические объекты, то к вложенным объектам так же применяется операция сдвига.
                 Регистратор служит для предотвращения многократного преобразования объекта.
                 Допустимо использовать функцию без регистратора, если надо преобразовать одиночный объект.
                 Если надо преобразовать набор взаимосвязанных объектов, необходимо использовать регистратор для 
                 предотвращения повторного преобразования вложенных объектов, поскольку не исключена ситуация,
                 когда несколько объектов из набора содержат ссылки на один или несколько общих объектов, подлежащих сдвигу.
             \en Move an object along a geometric vector using the registrator.
                 If object contains references to the other geometric objects then the move operation is applied to the nested objects.
                 Registrator is needed to prevent multiple copying of the object.
                 It is allowed to use function without registrator, if it is needed to transform a single object.
                 If it is needed to transform a set of connected objects, then one should use registrator 
                 to prevent repeating transformation of nested objects, because of the possible situation 
                 when several objects contain references to the same common objects, that require to be moved.\~
    \param[in] to   - \ru Вектор сдвига.
                      \en Shift vector. \~
    \param[in] iReg - \ru Регистратор.
                      \en Registrator.
  */
  virtual void    Move     ( const MbVector3D & to, MbRegTransform * iReg = nullptr ) = 0;

  /** \brief \ru Повернуть объект вокруг оси на заданный угол.
             \en Rotate an object around an axis at a given angle. \~
    \details \ru Повернуть объект вокруг оси на заданный угол с использованием регистратора. 
                 Если объект содержит ссылки на другие геометрические объекты, то к вложенным объектам так же применяется операция поворота.
                 Регистратор служит для предотвращения многократного преобразования объекта.
                 Допустимо использовать функцию без регистратора, если надо преобразовать одиночный объект.
                 Если надо преобразовать набор взаимосвязанных объектов, необходимо использовать регистратор для 
                 предотвращения повторного преобразования вложенных объектов, поскольку не исключена ситуация,
                 когда несколько объектов из набора содержат ссылки на один или несколько общих объектов, подлежащих повороту.
             \en Rotate an object around an axis at a given angle with the registrator.
                 If object contains references to the other geometric objects then the rotation operation is applied to the nested objects.
                 Registrator is needed to prevent multiple copying of the object.
                 It is allowed to use function without registrator, if it is needed to transform a single object.
                 If it is needed to transform a set of connected objects, then one should use registrator 
                 to prevent repeating transformation of nested objects, because of the possible situation 
                 when several objects contain references to the same common objects, that require to be rotated.\~
    \param[in] axis  - \ru Ось поворота.
                       \en The axis of rotation. \~
    \param[in] angle - \ru Угол поворота.
                       \en Rotation. \~
    \param[in] iReg  - \ru Регистратор.
                       \en Registrator.
  */
  virtual void    Rotate   ( const MbAxis3D & axis, double angle, MbRegTransform * iReg = nullptr ) = 0;

  /** \brief \ru Определить, являются ли объекты равными.
             \en Determine whether an object is equal. \~
    \details \ru Равными считаются однотипные объекты, все данные которых одинаковы (равны).
             \en Still considered objects of the same type, all data is the same (equal). \~
    \param[in] item -     \ru Объект для сравнения.
                          \en The object to compare. \~
    \param[in] accuracy - \ru Точность сравнения.
                          \en The accuracy to compare. \~
    \return \ru Равны ли объекты.
            \en Whether the objects are equal.
  */
  virtual bool    IsSame   ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const = 0; 

  /** \brief \ru Определить, являются ли объекты подобными.
             \en Determine whether an object is similar. \~
    \details \ru Подобными считаются однотипные объекты, данные которых можно приравнять или данные так же являются подобными (указатели).
                 Подобный объект можно инициализировать по данным подобного ему объекта (приравнять один другому без изменения адресов).
             \en Such are considered the same objects whose data are similar. \~
    \param[in] item - \ru Объект для сравнения.
                      \en The object to compare. \~
    \return \ru Подобны ли объекты.
            \en Whether the objects are similar.
  */
  virtual bool    IsSimilar( const MbSpaceItem & item ) const;

  /** \brief \ru Сделать объекты равным, если они подобны.
             \en Make objects equal if they are similar. \~
    \details \ru Равными можно сделать только подобные объекты.
                 Подобный объект приравнивается присланному путем изменения численных данных.
             \en You can still make only a similar objects. \~
    \param[in] item - \ru Объект для инициализации.
                      \en The object to initialize. \~
    \return \ru Сделан ли объект равным присланному.
            \en Object is changed.
  */
  virtual bool    SetEqual ( const MbSpaceItem & item ) = 0;

  /** \brief \ru Определить расстояние до точки.
             \en Determine the distance to the point. \~
    \details \ru Определить расстояние до точки.
             \en Determine the distance to the point. \~
    \param[in] point - \ru Точка.
                       \en Point. \~
    \return \ru Расстояние от объекта до точки.
            \en Distance to point
  */
  virtual double  DistanceToPoint ( const MbCartPoint3D & point ) const = 0;

  /** \brief \ru Расширить присланный габаритный куб так, чтобы он включал в себя данный объект.
             \en Expand sent bounding box (a.k.a. gabarit), so that it included the object. \~
    \details \ru Расширить присланный габаритный куб так, чтобы он включал в себя данный объект.
             \en Expand sent bounding box, so that it included the object. \~
    \param[in, out] cube - \ru Принимающий габаритный куб с информацией по габаритам.
                           \en The bounding box to expand.
  */
  virtual void    AddYourGabaritTo( MbCube & cube ) const = 0; 

  /** \brief \ru Рассчитать габарит в локальной системы координат.
             \en To compute bounding box in a local coordinate system\~
    \details \ru Для получения габарита объекта относительно локальной системы координат, 
                 присланный куб делается пустым. Затем вычисляются габариты объекта в локальной системе координат
                 и сохраняются в кубе cube. 
             \en To obtain bounding box of object with regar to a local coordinate system, 
                 sent box is made to be empty. Then it is computed bounding box of object in a local coordinate system \
                 and it is stored in box 'cube' \~
    \param[in]      into - \ru Матрица перехода от текущей для объекта системы координат к локальной системе координат.
                           \en Transformation matrix from object's current coordinate system to a local coordinate system \~
    \param[in, out] cube - \ru Куб с информацией по габаритам.
                           \en Information on bounding box \~
  */
  virtual void    CalculateLocalGabarit( const MbMatrix3D & into, MbCube & cube ) const = 0; 

  /// \ru Перевести все временные (mutable) данные объекта в неопределённое (исходное) состояние. \en Translate all the time (mutable) data objects in an inconsistent (initial) state.
  virtual void    Refresh(); 
  
  /// \ru Создать собственное свойство с заданием его имени. \en Create your own property with the name.
  virtual MbProperty & CreateProperty( MbePrompt name ) const = 0;

  /** \brief \ru Выдать свойства объекта.
             \en Outstanding properties of the object. \~
    \details \ru Выдать внутренние данные (свойства) объекта для их просмотра и модификации.
             \en Issue internal data (properties) of the object for viewing and modification. \~
    \param[in] properties - \ru Контейнер для внутренних данных объекта.
                            \en Container for the internal data of the object.
  */
  virtual void    GetProperties( MbProperties & properties ) = 0;

  /** \brief \ru Изменить свойства объекта.
             \en Change the properties of an object. \~
    \details \ru Изменение внутренних данных (свойств) объекта выполняется копированием соответствующих значений из присланного объекта.
             \en Changing the internal data (properties) of the object you are copying the corresponding values from the sent object. \~
    \param[in] properties - \ru Контейнер для внутренних данных объекта.
                            \en Container for the internal data of the object.
  */
  virtual void    SetProperties( const MbProperties & properties ) = 0;

  /** \brief \ru Построить полигональную копию mesh.
             \en Build polygonal copy mesh. \~
    \details \ru Построить полигональную копию данного объекта, представленную полигонами, или/и плоскими пластинами.
             \en Build a polygonal copy of the object that is represented by polygons or/and fasets. \~
    \param[in] stepData -  \ru Данные для вычисления шага при построении полигонального.
                           \en Data for еру step calculation for polygonal object. \~
    \param[in] note     -  \ru Способ построения полигонального объекта.
                           \en Way for polygonal object constructing. \~
    \param[in, out] mesh - \ru Построенный полигональный объект.
                           \en The builded polygonal object.
  */
  virtual void    CalculateMesh( const MbStepData & stepData, const MbFormNote & note, MbMesh & mesh ) const = 0;
          void    CalculateWire( const MbStepData & stepData, MbMesh & mesh ) const { CalculateMesh( stepData, MbFormNote(true, false), mesh ); }
          void    CalculateWire( double sag, MbMesh & mesh ) const                  { CalculateMesh( MbStepData(ist_SpaceStep,sag), MbFormNote(true, false), mesh ); }

  /// \ru Выдать базовые объекты в присланный контейнер. \en Outstanding reference objects in a container sent.
  virtual void    GetBasisItems  ( RPArray<MbSpaceItem> & );
  /// \ru Выдать контрольные точки объекта. \en Get control points of object.
  virtual void    GetBasisPoints( MbControlData3D & ) const;
  /// \ru Изменить объект по контрольным точкам. \en Change the object by control points.
  virtual void    SetBasisPoints( const MbControlData3D & );
  /** \} */
          /** \brief \ru Регистрация объекта.
                     \en Register object. \~
            \details \ru Регистрация объекта для предотвращения его многократной записи.
                         Другие объекты могут содержать указатель на данный объект. 
                         Функция взводит флаг, который позволяет записывать объект один раз, 
                         а в остальных записях ссылаться на записанный экземпляр.
                         Чтение так же выполняется один раз, а в остальных случаях чтения подставляется адрес уже прочитанного объекта.
                     \en Register object to prevent its rewritable.
                         Other objects may contain a pointer to the given object.
                         Function sets a flag to 'true', which allows to write object for one time,
                         and make a reference to the written instance of the other records.
                         Reading is performed once, and there is substitution of already read object for all the remaining reading cases.
          */
          void    PrepareWrite() const { SetRegistrable( (GetUseCount() > 1) ? registrable : noRegistrable ); }

DECLARE_PERSISTENT_CLASS( MbSpaceItem ) 
OBVIOUS_PRIVATE_COPY( MbSpaceItem )
};

IMPL_PERSISTENT_OPS( MbSpaceItem )


#endif // __SPACE_ITEM_H
