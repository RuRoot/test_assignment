////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Строитель объекта геометрической модели.
         \en Constructor of object of the geometric model. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CREATOR_H
#define __CREATOR_H

#include <mb_operation_result.h>
#include <mb_cart_point3d.h>
#include <space_item.h>
#include <topology_faceset.h>
#include <name_item.h>

class   MbVertex;
class   MbCurveEdge;
class   MbPointFrame;
class   MbWireFrame;
class   MbCreator;


namespace c3d // namespace C3D
{
typedef SPtr<MbCreator>                             CreatorSPtr;
typedef SPtr<const MbCreator>                       ConstCreatorSPtr;

typedef std::vector<MbCreator *>                    CreatorsVector;
typedef std::vector<const MbCreator *>              ConstCreatorsVector;

typedef std::vector<CreatorSPtr>                    CreatorsSPtrVector;
typedef std::vector<ConstCreatorSPtr>               ConstCreatorsSPtrVector;

typedef std::set<MbCreator *>                       CreatorsSet;
typedef CreatorsSet::iterator                       CreatorsSetIt;
typedef CreatorsSet::const_iterator                 CreatorsSetConstIt;
typedef std::pair<CreatorsSetConstIt, bool>         CreatorsSetRet;

typedef std::set<CreatorSPtr>                       CreatorsSPtrSet;
typedef CreatorsSPtrSet::iterator                   CreatorsSPtrSetIt;
typedef CreatorsSPtrSet::const_iterator             CreatorsSPtrSetConstIt;
typedef std::pair<CreatorsSPtrSetConstIt, bool>     CreatorsSPtrSetRet;

typedef std::set<const MbCreator *>                 ConstCreatorsSet;
typedef ConstCreatorsSet::iterator                  ConstCreatorsSetIt;
typedef ConstCreatorsSet::const_iterator            ConstCreatorsSetConstIt;
typedef std::pair<ConstCreatorsSetConstIt, bool>    ConstCreatorsSetRet;

typedef std::set<ConstCreatorSPtr>                    ConstCreatorsSPtrSet;
typedef ConstCreatorsSPtrSet::iterator                ConstCreatorsSPtrSetIt;
typedef ConstCreatorsSPtrSet::const_iterator          ConstCreatorsSPtrSetConstIt;
typedef std::pair<ConstCreatorsSPtrSetConstIt, bool>  ConstCreatorsSPtrSetRet;
}


//------------------------------------------------------------------------------
/** \brief \ru Типы строителей.
           \en Types of creators. \~
  \details \ru Типы строителей оболочек тел, точечных и проволочных каркасов геометрической модели. \n
           \en Types of constructors of shells of solids, point-frames and wire-frames of the geometric model. \n \~
  \ingroup Model_Creators
 */
// ---
enum MbeCreatorType {

  ct_Undefined                =   0,  ///< \ru Неизвестный объект. \en Unknown object.
  ct_Creator                  =   1,  ///< \ru Строитель объекта. \en Constructor of object. \n
  ct_DisplaceMaker            =  10,  ///< \ru Строитель изменённого объекта. \en Constructor of a changed object. \n 
  ct_MotionMaker              =  11,  ///< \ru Строитель перемещенного объекта. \en Constructor of a moved object. \n 
  ct_RotationMaker            =  12,  ///< \ru Строитель Повёрнутого вокруг оси объекта. \en Constructor of a rotated object. \n 
  ct_TransformationMaker      =  13,  ///< \ru Строитель трансформированного объекта. \en Constructor of a transformed object. \n 

  // \ru Строители точек. \en Creators of points. 
  ct_PointsCreator            = 101,  ///< \ru Строитель точечного каркаса. \en Constructor of point-frame. \n 

  // \ru Строители кривых. \en Creators of curves. 
  ct_SimpleWireFrameCreator   = 200,  ///< \ru Строитель каркаса без истории. \en Constructor of wire frame without history. 
  ct_Curve3DCreator           = 201,  ///< \ru Строитель кривой. \en Constructor of curve. 
  ct_Nurbs3DCreator           = 202,  ///< \ru Строитель сплайна с сопряжениями. \en Constructor of spline with tangents. 
  ct_SurfaceSplineCreator     = 203,  ///< \ru Строитель сплайна на поверхности с сопряжениями. \en Constructor of spline on a surface with tangents. 
  ct_ProjectionCurveCreator   = 204,  ///< \ru Строитель проекционной кривой. \en Constructor of the projection curve. 
  ct_OffsetCurveCreator       = 205,  ///< \ru Строитель эквидистантной кривой. \en Constructor of the offset curve. 
  ct_IntersectionCurveCreator = 206,  ///< \ru Строитель кривой пересечения. \en Constructor of the intersection curve. 
  ct_ConnectingCurveCreator   = 207,  ///< \ru Строитель кривой скругления двух кривых. \en Constructor of the curve connecting two curves. \n
  ct_ExtensionCurveCreator    = 208,  ///< \ru Строитель продленной кривой. \en Constructor of the extended curve. \n
  ct_FairBaseCreator          = 209,  ///< \ru Строитель плавной кривой. \en Constructor of fair curve. \n
  ct_FairCurveCreator         = 210,  ///< \ru Строитель плавной кривой по ломаной. \en Constructor of fair curve by a polyline. \n
  ct_FairFilletCreator        = 211,  ///< \ru Строитель изменения плавной кривой по ломаной. \en Constructor of changing a fair curve by a polyline. \n
  ct_FairClothoidCreator      = 212,  ///< \ru Строитель изменения плавной кривой по ломаной. \en Constructor of changing a fair curve by a polyline. \n
  ct_FairChangeCreator        = 213,  ///< \ru Строитель изменения плавной кривой по ломаной. \en Constructor of changing a fair curve by a polyline. \n
  ct_UnwrapCurveCreator       = 214,  ///< \ru Строитель развёрнутой кривой. \en Constructor of the unwrapped curve. \n
  ct_WrapCurveCreator         = 215,  ///< \ru Строитель свёрнутой кривой. \en Constructor of the wrapped curve. \n
  ct_BeamCurveCreator         = 216,  ///< \ru Строитель балочной кривой. \en Constructor of the beam curve. \n
  ct_FilletWireCreator        = 217,  ///< \ru Строитель скругленного каркаса. \en Constructor of the filleted wireframe. \n

  ct_WireFrameIntoNurbsCreator= 218,  ///< \ru Строитель NURBS-кривой по каркасу. \en Constructor of the NURBS-curve by wire frame. \n
  ct_TrimmedWireFrameCreator  = 219,  ///< \ru Строитель усечения каркаса. \en Constructor of the trimmed wireframe. \n

  ct_UnwrapPointCreator       = 220,  ///< \ru Строитель развёрнутой точки. \en Constructor of the unwrapped point. \n
  ct_WrapPointCreator         = 221,  ///< \ru Строитель свёрнутой точки. \en Constructor of the wrapped point. \n

  ct_OffsetWireFrameCreator   = 222,  ///< \ru Строитель эквидистантного каркаса. \en Constructor of the offset wireframe. \n

  // \ru Строители полигональных объектов. \en Creators of polygonal objects. 
  ct_SimpleMeshCreator        = 400,  ///< \ru Строитель полигонального объекта без истории. \en Constructor of a polygonal object without history. 
  ct_ReverseMeshCreator       = 401,  ///< \ru Строитель вывернутого полигонального объекта. \en Constructor of a reversed polygonal object. 
  ct_BooleanMeshCreator       = 402,  ///< \ru Строитель полигонального объекта булевой операции. \en Constructor of a polygonal object of a boolean operation. 
  ct_CuttingMeshCreator       = 403,  ///< \ru Строитель полигонального объекта, разрезанного плоскостью. \en Constructor of a polygonal object cutted by a plane.

  // \ru Строители тел. \en Creators of solids. 
  ct_ShellCreator             = 501,  ///< \ru Строитель оболочки. \en Constructor of shell. 
  ct_SimpleCreator            = 502,  ///< \ru Строитель оболочки без истории. \en Constructor of a shell without history. 
  ct_ElementarySolid          = 503,  ///< \ru Строитель оболочки в форме: блока, клина, цилиндра, конуса, шара, тора. \en Constructor of a shell as: a block, a wedge, a cylinder, a cone, a sphere, a torus. 
  ct_CurveSweptSolid          = 504,  ///< \ru Строитель оболочки движения. \en Constructor of a swept shell. 
  ct_CurveExtrusionSolid      = 505,  ///< \ru Строитель оболочки выдавливания. \en Constructor of a shell of extrusion. 
  ct_CurveRevolutionSolid     = 506,  ///< \ru Строитель оболочки вращения. \en Constructor of a shell of revolution. 
  ct_CurveEvolutionSolid      = 507,  ///< \ru Строитель кинематической оболочки. \en Constructor of a shell of evolution. 
  ct_CurveLoftedSolid         = 508,  ///< \ru Строитель оболочки по плоским сечениям. \en Constructor of lofted shell. 
  ct_BooleanSolid             = 509,  ///< \ru Строитель оболочки булевой операции. \en Constructor of a shell of boolean operation. 
  ct_CuttingSolid             = 510,  ///< \ru Строитель разрезанной поверхностью оболочки. \en Constructor of a shell cut by surface. 
  ct_SymmetrySolid            = 511,  ///< \ru Строитель симметричной оболочки. \en Constructor of a symmetric shell. 
  ct_HoleSolid                = 512,  ///< \ru Строитель оболочки отверстия, кармана или фигурного паза. \en Constructor of a shell of a hole, a pocket or a groove. 
  ct_SmoothSolid              = 513,  ///< \ru Строитель оболочки с фаской или скруглением ребер. \en Constructor of a shell with a chamfer or with edges fillet. 
  ct_ChamferSolid             = 514,  ///< \ru Строитель оболочки с фаской ребер. \en Constructor of a shell with edges chamfer. 
  ct_FilletSolid              = 515,  ///< \ru Строитель оболочки со скруглением ребер. \en Constructor of a shell with edges fillet. 
  ct_FullFilletSolid          = 516,  ///< \ru Строитель оболочки со скруглением граней. \en Constructor of a shell with a faces fillet. 
  ct_ShellSolid               = 517,  ///< \ru Строитель тонкостенной оболочки, эквидистантной оболочки, придания толщины. \en Constructor of a thin-walled shell, an offset shell, thickening. 
  ct_DraftSolid               = 518,  ///< \ru Строитель оболочки с литейным уклоном. \en Constructor of a shell with a pattern taper. 
  ct_RibSolid                 = 519,  ///< \ru Строитель оболочки с ребром жесткости. \en Constructor of a shell with a rib. 
  ct_SplitShell               = 520,  ///< \ru Строитель оболочки с подразбиением граней. \en Constructor of a shell with faces subdivision. 
  ct_NurbsBlockSolid          = 521,  ///< \ru Строитель оболочки в форме блока из nurbs-поверхностей. \en Constructor of a shell as a block from NURBS surfaces: 
  ct_FaceModifiedSolid        = 522,  ///< \ru Строитель модифицированной оболочки. \en Constructor of a modified shell. 
  ct_ModifiedNurbsItem        = 523,  ///< \ru Строитель модифицированной nurbs-поверхностями оболочки. \en Constructor of a shell with modified NURBS surfaces. 
  ct_NurbsModification        = 524,  ///< \ru Строитель модифицированной контрольными точками оболочки. \en Constructor of a shell modified by control points. 
  ct_TransformedSolid         = 525,  ///< \ru Строитель трансформированной оболочки. \en Constructor of a transformed shell. 
  ct_ThinShellCreator         = 526,  ///< \ru Строитель тонкой оболочки. \en Constructor of a thin shell. 
  ct_UnionSolid               = 527,  ///< \ru Строитель объединённой оболочки. \en Constructor of a united shell. 
  ct_DetachSolid              = 528,  ///< \ru Строитель оболочки из отделяемой части многосвязной оболочки. \en Constructor of a shell from the detached part of a multiply connected shell. 
  ct_DuplicationSolid         = 529,  ///< \ru Строитель множества тел, построенных из исходного. \en Constructor of set of solids built from the original. \n
  ct_ReverseCreator           = 530,  ///< \ru Строитель вывернутого "наизнанку" тела. \en Constructor of a reversed solid. \n
  ct_DividedShell             = 531,  ///< \ru Строитель разделенной на части оболочки \en Constructor of a divided shell. 

  // \ru Строители листовых тел. \en Creators of sheet solids. 
  ct_SheetMetalSolid          = 601,  ///< \ru Строитель листовой оболочки. \en Constructor of a sheet shell. 
  ct_BendOverSegSolid         = 602,  ///< \ru Строитель оболочки со сгибом относительно отрезка. \en Constructor of a shell with a bend at the segment. 
  ct_JogSolid                 = 603,  ///< \ru Строитель оболочки с подсечкой. \en Constructor of a shell with a jog. 
  ct_BendsByEdgesSolid        = 604,  ///< \ru Строитель оболочки со сгибом по ребру. \en Constructor of a shell with a bend at the edge. 
  ct_BendUnbendSolid          = 605,  ///< \ru Строитель оболочки с выполненным сгибом или разгибом. \en Constructor of a shell with bending or unbending. 
  ct_ClosedCornerSolid        = 606,  ///< \ru Строитель оболочки с замыканием угла. \en Constructor of a shell with corner enclosure. 
  ct_StampSolid               = 607,  ///< \ru Строитель оболочки с штамповкой. \en Constructor of a shell with stamping. 
  ct_SphericalStampSolid      = 608,  ///< \ru Строитель оболочки со сферической штамповкой. \en Constructor of a shell with spherical stamping. 
  ct_BeadSolid                = 609,  ///< \ru Строитель оболочки с буртиком. \en Constructor of a shell with a bead. 
  ct_JalousieSolid            = 610,  ///< \ru Строитель оболочки с жалюзи. \en Constructor of a shell with jalousie. 
  ct_JointBendSolid           = 611,  ///< \ru Строитель оболочки с комбинированным сгибом. \en Constructor of a shell with a composite bend. 
  ct_StitchedSolid            = 612,  ///< \ru Строитель оболочки, сшитой из нескольких граней или оболочек. \en Constructor of a shell stitched from several faces or shells. 
  ct_RuledSolid               = 613,  ///< \ru Строитель линейчатой оболочки (обечайки). \en Constructor of a ruled shell (shell ring). 
  ct_RestoredEdgesSolid       = 614,  ///< \ru Строитель листовой оболочки с восстановленными боковыми рёбрами. \en Constructor of a sheet shell with restored lateral edges. 
  ct_SheetUnionSolid          = 615,  ///< \ru Строитель объединения двух листовых тел по торцу. \en Constructor of two sheet solids union by the side.
  ct_StampRibSolid            = 616,  ///< \ru Строитель ребра жесткости листового тела. \en Constructor of sheet solid rib. \n
  ct_BendAnySolid             = 617,  ///< \ru Строитель оболочки с выполненным сгибом нелистового тела. \en Constructor of a shell with bending of non-sheet solid
  ct_SimplifyFlatSolid        = 618,  ///< \ru Строитель упрощения развёртки листового тела. \en Constructor of the sheet solid flat pattern simplification.
  ct_UserStampSolid           = 619,  ///< \ru Строитель оболочки с штамповкой телом. \en Constructor of a shell with stamping by solid. 
  ct_RemoveOperationSolid     = 620,  ///< \ru Строитель удаления операции листового тела. \en Constructor of removing of the sheet solid.
  ct_BuildSheetMetalSolid     = 621,  ///< \ru Строитель листового тела по произвольному телу. \en Constructor of building sheet metal solid based on an arbitary solid.
  ct_NormalizeHolesSolid      = 622,  ///< \ru Строитель нормализации отверстий листовых тел. \en Constructor of the normalization of the holes of sheet solid.

  // \ru Строители оболочек. \en Creators of shells. 
  ct_JoinShell                = 701,  ///< \ru Строитель оболочки соединения. \en Constructor of a joint shell. 
  ct_MeshShell                = 702,  ///< \ru Строитель оболочки по поверхностям на сетках кривых. \en Constructor of a shell by surfaces constructed by the grid curves. 
  ct_RuledShell               = 703,  ///< \ru Строитель оболочки по набору линейчатых поверхностей. \en Constructor of a shell by a set of ruled surfaces.   
  ct_NurbsSurfacesShell       = 704,  ///< \ru Строитель NURBS-оболочки на двумерном массиве точек. \en Constructor of a NURBS-shell on a two-dimensional array of points. 
  ct_TruncatedShell           = 705,  ///< \ru Строитель оболочки, усеченная геометрическими объектами. \en Constructor of a shell truncated by geometric objects. 
  ct_ExtensionShell           = 706,  ///< \ru Строитель продолженной оболочки. \en Constructor of an extended shell. 
  ct_PatchSetCreator          = 707,  ///< \ru Строитель заплатки по кривым на оболочке. \en Constructor of a patch by curves on the shell. 
  ct_FilletShell              = 708,  ///< \ru Строитель оболочки грани соединения. \en Constructor of a shell of a fillet face. 
  ct_MedianShell              = 709,  ///< \ru Строитель срединной оболочки тела. \en Constructor of a median shell of solid. \n
  ct_SectionShell             = 710,  ///< \ru Строитель оболочки на поверхности переменного сечения. \en Constructor of the shell on swept mutable section surface. \n

  // \ru Строители других объектов (вставлять новые типы перед этим типом). \en Creators of the other objects (insert new types before this type). 
  ct_AttributeProvider        = 801,  ///< \ru Поставщик атрибутов для примитивов оболочки. \en Attribute provider for the shell primitives.
  
  ct_FreeItem                 = 900,  ///< \ru Тип для объектов, созданных пользователем. \en Type for the user-defined objects.

};


//------------------------------------------------------------------------------
/** \brief \ru Строитель объекта геометрической модели.
           \en Constructor of object of the geometric model. \~
  \details \ru Строитель выполняет одно действие при построении объекта геометрической модели и
    содержит необходимую для этого информацию. \n
    Множество строителей образует журнал построения геометрической модели.\n
    Строители наиболее востребованы при создании оболочки тела MbSolid.
    Изначально оболочки тела строятся на основе кривых или поверхностей.
    Многие оболочки можно построить путем движения составной кривой, 
    называемой образующей по траектории, заданной направляющей кривой.
    Можно построить оболочку, проходящую по заданному набору кривых.
    На основе поверхности можно построить оболочку в форме листа конечной толщины. 
    Подобные оболочки назовём простыми. \n
    Более сложные оболочки можно получить с помощью операций.
    Операцией называется совокупность действий над оболочками, 
    которая приводит к рождению новой оболочки. \n
    Примерами операций служат булевы операции, 
    построение тонкостенной оболочки,
    построение скруглений или фасок рёбер.
           \en The constructor performs one operation while creating the object of a geometric model and
    contains the information necessary for this. \n
    Array of constructors forms the history tree of geometric model.\n
    Creators are mostly required while creating a shell of solid MbSolid.
    Initially solid are created on the base of curves or surfaces.
    Most shells can be created by moving the composite curve 
    which is called generatrix, along the trajectory specified by the guide curve.
    A shell can be constructed passing through a given set of curves.
    A shell in the form of a sheet of finite thickness can be constructed on the base of a surface. 
    Let's call such shells simple. \n
    More complex shells can be obtained by operations.
    Operation is a set of actions on the shells 
    which result in creation of a new shell. \n
    Examples of operations are boolean operations, 
    thin-walled shell construction,
    edges fillet or chamfers creation. \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbCreator : public MbRefItem, public TapeBase {
protected :
  SPtr<MbSNameMaker>      names;  ///< \ru Именователь создаваемых строителем элементов и объектов. \en An object defining the names of elements and objects created by the constructor. 
  mutable MbeProcessState status; ///< \ru Состояние строителя и результата выполнения операции. \en State of the constructor and of the operation result. 

protected :
  /// \ru Конструктор по именователю. \en Constructor by name-maker. 
  MbCreator( MbSNameMaker * );
  /// \ru Конструктор по именователю. \en Constructor by name-maker. 
  MbCreator( const MbSNameMaker & );
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbCreator( const MbCreator & );
public :
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbCreator();

  /** \ru \name Общие функции строителя оболочки.
      \en \name Common functions of the shell creator.
      \{ */
  /// \ru Получить регистрационный тип (для копирования, дублирования). \en Get the registration type (for copying, duplication). 
  MbeRefType          RefType() const override;
  /// \ru Получить тип объекта. \en Get the object type. 
  virtual MbeCreatorType  IsA()  const = 0;
  /// \ru Получить групповой тип объекта. \en Get the group type of the object. 
  virtual MbeCreatorType  Type() const;

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
           \en Create a copy of the object using the registrator. 
    The registrator is used to prevent multiple copying of an object.
    If the object contains references to other objects, then the included objects are copied too.
    It is allowed not to pass the registrator to a function. Then the new copy of the object will be created.
    It is allowed not to use the registrator while copying a single object or a set of disconnected objects.
    The registrator must be used to copy several correlated objects successively.
    It is possible that the relation between objects means that the objects contain references on the common objects. 
    Then, while copying without using the registrator, one can get a set of copies 
    which contain references to the different copies of a single embedded object, what leads to loss of relationship between the copies. \~
  \param[in] iReg - \ru Регистратор.
                    \en Registrator. \~
  \return \ru Копия объекта.
          \en The object copy. \~
  */
  virtual MbCreator & Duplicate( MbRegDuplicate * iReg = nullptr ) const = 0;

  /** \brief \ru Преобразовать согласно матрице.
             \en Transform according to the matrix. \~
  \details \ru Преобразовать исходный объект согласно матрице c использованием регистратора. 
    Если объект содержит ссылки на другие геометрические объекты, то вложенные объекты так же преобразуются согласно матрице.
    Регистратор служит для предотвращения многократного преобразования объекта.
    Допустимо использовать функцию без регистратора, если надо преобразовать одиночный объект.
    Если надо преобразовать набор взаимосвязанных объектов, необходимо использовать регистратор для 
    предотвращения повторного преобразования вложенных объектов, поскольку не исключена ситуация,
    когда несколько объектов из набора содержат ссылки на один или несколько общих объектов, подлежащих трансформации.
           \en Transform the initial object according to the matrix using the registrator. 
    If the object contains references to the other geometric objects, then the nested objects are transformed according to the matrix.
    The registrator is used for preventing multiple transformation of the object.
    The function can be used without the registrator to transform a single object.
    The registrator must be used to transform a set of interdependent objects to 
    prevent repeated transformation of the nested objects, since it is not ruled out
    that several objects from the set contain references to one or several common objects subject to transformation. \~
  \param[in] matr - \ru Матрица преобразования.
                    \en A transformation matrix. \~
  \param[in] iReg - \ru Регистратор.
                    \en Registrator. \~
  */
  virtual void        Transform( const MbMatrix3D & matr, MbRegTransform * iReg = nullptr ) = 0;

  /** \brief \ru Сдвинуть вдоль вектора.
             \en Translate along a vector. \~
  \details \ru Сдвинуть геометрический объект вдоль вектора с использованием регистратора. 
    Если объект содержит ссылки на другие геометрические объекты, то к вложенным объектам так же применяется операция сдвига.
    Регистратор служит для предотвращения многократного преобразования объекта.
    Допустимо использовать функцию без регистратора, если надо преобразовать одиночный объект.
    Если надо преобразовать набор взаимосвязанных объектов, необходимо использовать регистратор для 
    предотвращения повторного преобразования вложенных объектов, поскольку не исключена ситуация,
    когда несколько объектов из набора содержат ссылки на один или несколько общих объектов, подлежащих сдвигу.
           \en Move a geometric object along the vector using the registrator. 
    If the object contains references to the other objects, then the translation operation is applied to the nested objects.
    The registrator is used for preventing multiple transformation of the object.
    The function can be used without the registrator to transform a single object.
    The registrator must be used to transform a set of interdependent objects to 
    prevent repeated transformation of the nested objects, since it is not ruled out
    that several objects from the set contain references to one or several common objects subject to moving. \~
  \param[in] to   - \ru Вектор сдвига.
                    \en Movement vector. \~
  \param[in] iReg - \ru Регистратор.
                    \en Registrator. \~
  */
  virtual void        Move     ( const MbVector3D & to, MbRegTransform * iReg = nullptr ) = 0;

  /** \brief \ru Повернуть объект вокруг оси.
             \en Rotate an object about the axis. \~
  \details \ru Повернуть объект вокруг оси на заданный угол с использованием регистратора. 
    Если объект содержит ссылки на другие геометрические объекты, то к вложенным объектам так же применяется операция поворота.
    Регистратор служит для предотвращения многократного преобразования объекта.
    Допустимо использовать функцию без регистратора, если надо преобразовать одиночный объект.
    Если надо преобразовать набор взаимосвязанных объектов, необходимо использовать регистратор для 
    предотвращения повторного преобразования вложенных объектов, поскольку не исключена ситуация,
    когда несколько объектов из набора содержат ссылки на один или несколько общих объектов, подлежащих повороту.
           \en Rotate an object about the axis by the given angle using the registrator. 
    If the object contains references to the other geometric objects, then the rotation operation is applied to the nested objects too.
    The registrator is used for preventing multiple transformation of the object.
    The function can be used without the registrator to transform a single object.
    The registrator must be used to transform a set of interdependent objects to 
    prevent repeated transformation of the nested objects, since it is not ruled out
    that several objects from the set contain references to one or several common objects subject to rotation. \~
  \param[in] axis - \ru Ось поворота.
                    \en The rotation axis. \~
  \param[in] angle - \ru Угол поворота.
                     \en The rotation angle. \~
  \param[in] iReg - \ru Регистратор.
                    \en Registrator. \~
  */
  virtual void        Rotate   ( const MbAxis3D & axis, double angle, MbRegTransform * iReg = nullptr ) = 0;

  /** \brief \ru Являются ли объекты равными?
             \en Determine whether an object is equal? \~
    \details \ru Равными считаются однотипные объекты, все данные которых одинаковы (равны).
             \en Still considered objects of the same type, all data is the same (equal). \~
    \param[in] other -    \ru Объект для сравнения.
                          \en The object to compare. \~
    \param[in] accuracy - \ru Точность сравнения.
                          \en The accuracy to compare. \~
    \return \ru Равны ли объекты.
            \en Whether the objects are equal.
  */
  virtual bool        IsSame( const MbCreator & other, double accuracy ) const = 0; 

  /** \brief \ru Являются ли объекты подобными?
             \en Determine whether an object is similar? \~
    \details \ru Подобными считаются однотипные объекты, данные которых можно приравнять или данные так же являются подобными (указатели).
                 Подобный объект можно инициализировать по данным подобного ему объекта (приравнять один другому без изменения адресов).
             \en Such are considered the same objects whose data are similar. \~
    \param[in] item - \ru Объект для сравнения.
                      \en The object to compare. \~
    \return \ru Подобны ли объекты.
            \en Whether the objects are similar.
  */
  virtual bool        IsSimilar( const MbCreator & item ) const;

  /// \ru Сделать объекты равными, если они подобны. \en Make the objects equal if they are similar. 
  virtual bool        SetEqual( const MbCreator & ) = 0;

  /** \brief \ru Построить оболочку по исходным данным.
             \en Create a shell from the initial data. \~
  \details \ru Построение новой или модификация присланной оболочки по исходным данным согласно строителю.
           \en Construction of a new shell or modification of the given one from the source data according to the constructor. \~
  \param[in] shell     - \ru Подлежащая модификации или новая оболочка.
                         \en A shell to be modified or a new shell. \~
  \param[in] sameShell - \ru Полнота копирования элементов при построении.
                         \en Whether to perform complete copying of elements while constructing. \~
  \param[in] items     - \ru Контейнер для складывания элементов невыполненных построений (может быть nullptr).
                         \en Container for the elements of not performed constructions (can be nullptr). \~
  \return \ru Выполнено ли построение.
          \en Whether the construction is performed. \~
  */
  virtual bool        CreateShell( MbFaceShell *& shell, MbeCopyMode sameShell,
                                   RPArray<MbSpaceItem> * items = nullptr );

  /** \brief \ru Построить оболочку по исходным данным.
             \en Create a shell from the initial data. \~
  \details \ru Построение новой или модификация присланной оболочки по исходным данным согласно строителю.
           \en Construction of a new shell or modification of the given one from the source data according to the constructor. \~
  \param[in] shell     - \ru Подлежащая модификации или новая оболочка.
                         \en A shell to be modified or a new shell. \~
  \param[in] sameShell - \ru Полнота копирования элементов при построении.
                         \en Whether to perform complete copying of elements while constructing. \~
  \param[in] items     - \ru Контейнер для складывания элементов невыполненных построений (может быть nullptr).
                         \en Container for the elements of not performed constructions (can be nullptr). \~
  \return \ru Выполнено ли построение.
          \en Whether the construction is performed. \~
  */
          bool        CreateShell( c3d::ShellSPtr & shell, MbeCopyMode sameShell, 
                                   RPArray<MbSpaceItem> * items = nullptr );

  /** \brief \ru Построить проволочный каркас по исходным данным.
             \en Create a wire-frame from the source data. \~
  \details \ru Построение нового каркаса (кривых) или модификация присланного каркаса по исходным данным согласно строителю.
           \en Construction of a new frame (of curves) or modification of the given one from the source data according to the constructor. \~
  \param[in] frame     - \ru Подлежащий модификации или новый каркас.
                         \en A frame to be modified or a new frame. \~
  \param[in] sameShell - \ru Полнота копирования элементов при построении.
                         \en Whether to perform complete copying of elements while constructing. \~
  \param[in] items     - \ru Контейнер для складывания элементов невыполненных построений (может быть nullptr).
                         \en Container for the elements of not performed constructions (can be nullptr). \~
  \return \ru Выполнено ли построение.
          \en Whether the construction is performed. \~
  */
  virtual bool        CreateWireFrame( MbWireFrame *& frame, MbeCopyMode sameShell, 
                                       RPArray<MbSpaceItem> * items = nullptr );

  /** \brief \ru Построить проволочный каркас по исходным данным.
             \en Create a wire-frame from the source data. \~
  \details \ru Построение нового каркаса (кривых) или модификация присланного каркаса по исходным данным согласно строителю.
           \en Construction of a new frame (of curves) or modification of the given one from the source data according to the constructor. \~
  \param[in] frame     - \ru Подлежащий модификации или новый каркас.
                         \en A frame to be modified or a new frame. \~
  \param[in] sameShell - \ru Полнота копирования элементов при построении.
                         \en Whether to perform complete copying of elements while constructing. \~
  \return \ru Выполнено ли построение.
          \en Whether the construction is performed. \~
  */
          bool        CreateWireFrame( SPtr<MbWireFrame> & frame, MbeCopyMode sameShell );

  /** \brief \ru Построить точечный каркас по исходным данным.
             \en Create a point-frame from the source data. \~
  \details \ru Построение нового каркаса (точек) или модификация присланного каркаса по исходным данным согласно строителю.
           \en Creation of a new frame (of points) or modification of the given one from the given data according to the constructor. \~
  \param[in] frame     - \ru Подлежащий модификации или новый каркас.
                         \en A frame to be modified or a new frame. \~
  \param[in] sameShell - \ru Полнота копирования элементов при построении.
                         \en Whether to perform complete copying of elements while constructing. \~
  \param[in] items     - \ru Контейнер для складывания элементов невыполненных построений (может быть nullptr).
                         \en Container for the elements of not performed constructions (can be nullptr). \~
  \return \ru Выполнено ли построение.
          \en Whether the construction is performed. \~
  */
  virtual bool        CreatePointFrame( MbPointFrame *& frame, MbeCopyMode sameShell, 
                                        RPArray<MbSpaceItem> * items = nullptr );

  /** \brief \ru Построить точечный каркас по исходным данным.
             \en Create a point-frame from the source data. \~
  \details \ru Построение нового каркаса (точек) или модификация присланного каркаса по исходным данным согласно строителю.
           \en Creation of a new frame (of points) or modification of the given one from the given data according to the constructor. \~
  \param[in] frame     - \ru Подлежащий модификации или новый каркас.
                         \en A frame to be modified or a new frame. \~
  \param[in] sameShell - \ru Полнота копирования элементов при построении.
                         \en Whether to perform complete copying of elements while constructing. \~
  \return \ru Выполнено ли построение.
          \en Whether the construction is performed. \~
  */
          bool        CreatePointFrame( SPtr<MbPointFrame> & frame, MbeCopyMode sameShell );

  /** \brief \ru Создать полигональный объект по исходным данным.
             \en Create a polygonal object from the source data. \~
  \details \ru Построение нового полигонального объекта или модификация присланного объекта по исходным данным согласно строителю.
           \en Creation of a new polygonal object or modification of the given one from the given data according to the constructor. \~
  \param[in] mesh      - \ru Подлежащий модификации или новый полигональный объект.
                         \en A polygonal object to be modified or a new one. \~
  \param[in] sameShell - \ru Полнота копирования элементов при построении.
                         \en Whether to perform complete copying of elements while constructing. \~
  \param[in] items     - \ru Контейнер для складывания элементов невыполненных построений (может быть nullptr).
                         \en Container for the elements of not performed constructions (can be nullptr). \~
  \return \ru Выполнено ли построение.
          \en Whether the construction is performed. \~
  */
  virtual bool        CreateMesh( MbMesh *& mesh, MbeCopyMode sameShell,
                                  RPArray<MbSpaceItem> * items = nullptr );

  /** \brief \ru Создать полигональный объект по исходным данным.
             \en Create a polygonal object from the source data. \~
  \details \ru Построение нового полигонального объекта или модификация присланного объекта по исходным данным согласно строителю.
           \en Creation of a new polygonal object or modification of the given one from the given data according to the constructor. \~
  \param[in] mesh      - \ru Подлежащий модификации или новый полигональный объект.
                         \en A polygonal object to be modified or a new one. \~
  \param[in] sameShell - \ru Полнота копирования элементов при построении.
                         \en Whether to perform complete copying of elements while constructing. \~
  \return \ru Выполнено ли построение.
          \en Whether the construction is performed. \~
  */
          bool        CreateMesh( SPtr<MbMesh> & mesh, MbeCopyMode sameShell );

  /// \ru Выдать свойства объекта. \en Get properties of the object. 
  virtual void        GetProperties( MbProperties & );
  /// \ru Записать свойства объекта. \en Set properties of the object. 
  virtual void        SetProperties( const MbProperties & ); 
  /// \ru Выдать заголовок свойства объекта. \en Get a name of object property. 
  virtual MbePrompt   GetPropertyName() = 0; 
  /// \ru Обновить форму оболочки. \en Update the shell shape. 
  virtual void        Refresh( MbFaceShell & ); 
  /// \ru Обновить форму каркаса. \en Update the frame shape. 
  virtual void        Refresh( MbWireFrame & ); 
  /// \ru Дать базовые объекты. \en Get the basis objects. 
  virtual void        GetBasisItems ( RPArray<MbSpaceItem> & ); 
  /// \ru Выдать контрольные точки объекта. \en Get control points of object.
  virtual void        GetBasisPoints( MbControlData3D & ) const;
  /// \ru Изменить объект по контрольным точкам. \en Change the object by control points.
  virtual void        SetBasisPoints( const MbControlData3D & );
  /// \ru Посчитать внутренние построители по типу. \en Count internal creators by type.
  virtual size_t      GetCreatorsCount( MbeCreatorType ct ) const; // { return (IsA() == ct) ? 1 : 0; }
  /// \ru Получить внутренние построители по типу. \en Get internal creators by type.
  virtual bool        GetInternalCreators( MbeCreatorType, c3d::ConstCreatorsSPtrVector & ) const;
  /// \ru Получить внутренние построители по типу. \en Get internal creators by type.
  virtual bool        SetInternalCreators( MbeCreatorType, c3d::CreatorsSPtrVector & );
  /// \ru Переместить/Изменить строитель. \en Displace/Change the creator.
  virtual bool        Perform( MbCreator * ) const;

  /// \ru Установить версию объектов. \en Set the objects version. 
  virtual void        SetYourVersion( VERSION version, bool forAll );
  /// \ru Выдать версию объекта. \en Get the object version. 
          VERSION     GetYourVersion() const { return names->GetMathVersion(); }
  /// \ru Выдать точность построения. \en Get the precision of object construction.
  virtual const MbPrecision & GetPrecision() const { return MbPrecision::defaultPrecision; }

  /// \ru Выдать именователь объекта. \en Get the name-maker. 
  const MbSNameMaker & GetYourNameMaker() const { return *names; }
  /// \ru Выдать именователь объекта для редактирования. \en Get the object's name-maker for editing. 
  MbSNameMaker &      SetYourNameMaker() { return *names; }
  /// \ru Установить именователь объекта. \en Set the object's name-maker. 
          void        SetNameMaker( const MbSNameMaker & n ) { names->SetNameMaker( n, true ); }

  /// \ru Выдать главное имя объекта. \en Get the main name of the object. 
          SimpleName  GetMainName() const          { return names->GetMainName(); }
  /// \ru Установить главное имя объекта. \en Set the main name of the object. 
          void        SetMainName( SimpleName n )  { names->SetMainName(n);       }
  /// \ru Выдать флаг состояния. \en Get the flag of state. 
  MbeProcessState     GetStatus() const  { return status; } 
  /// \ru Установить флаг состояния. \en Set the flag of state. 
          void        SetStatus( MbeProcessState l ) { status = l;    } 

  /** \brief \ru Регистрировать объект.
              \en Register the object. \~
  \details \ru Регистрация объекта для предотвращения его многократной записи.
    Другие объекты могут содержать указатель на данный объект. 
    Функция взводит флаг, который позволяет записывать объект один раз, а в остальных записях ссылаться на записанный экземпляр.
    Чтение так же выполняется один раз, а в остальных случаях чтения подставляется адрес уже прочитанного объекта.
            \en Object registration for preventing its multiple writing.
    Other objects may contain a pointer to the given object. 
    The function sets a flag that allow to write the object once and to use the references to the recorded instance in the other records.
    Reading is performed once too, in other cases of reading the address of the already read object is used. \~
  */
          void        PrepareWrite() const { SetRegistrable( (GetUseCount() > 1) ? registrable : noRegistrable ); }

  /** \} */

private:
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию \en Declaration without implementation of the assignment operator to prevent an assignment by default 
          MbCreator & operator = ( const MbCreator & );

  DECLARE_PERSISTENT_CLASS( MbCreator )
}; // MbCreator

IMPL_PERSISTENT_OPS( MbCreator )


//------------------------------------------------------------------------------
/** \brief \ru Создать оболочку по протоколу построения.
           \en Create a shell by history tree. \~
  \details \ru Создать оболочку по протоколу построения.\n
           \en Create a shell by history tree.\n \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (bool) CreateShell( MbFaceShell *& shell, const RPArray<MbCreator> & creators, MbeCopyMode copyMode );


//------------------------------------------------------------------------------
/** \brief \ru Создать оболочку по протоколу построения.
           \en Create a shell by history tree. \~
  \details \ru Создать оболочку по протоколу построения.\n
           \en Create a shell by history tree.\n \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (bool) CreateShell( MbFaceShell *& shell, const std::vector< SPtr<MbCreator> > & creators, MbeCopyMode copyMode );


#endif // __CREATOR_H
