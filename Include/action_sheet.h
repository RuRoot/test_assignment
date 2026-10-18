////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Функции для работы с телом из листового металла.
         \en Functions for operating with a sheet metal solid. \~

*/
////////////////////////////////////////////////////////////////////////////////


#ifndef __ACTION_SHEET_H
#define __ACTION_SHEET_H


#include <templ_sptr.h>
#include <mb_operation_result.h>
#include <cur_contour_on_plane.h>
#include <cur_plane_curve.h>
#include <cr_stamp_remove_solid.h>
#include <sheet_metal_param.h>
#include <topology_faceset.h>


class MbLineSegment;
class MbLineSegment3D;
class MbLine3D;
class MbSolid;



//------------------------------------------------------------------------------
/** \brief \ru Способ сегментации эскиза.
           \en The method of contour segmentation. \~
  \details \ru Способ сегментации эскиза. \n
           \en The method of contour segmentation. \n \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
enum MbeSegmentationMethod {
  sm_Quantity, ///< \ru По количеству сегментов. \en By quantity of segments. 
  sm_Length,   ///< \ru По длине сегментов. \en By length of segments 
  sm_Angle,    ///< \ru По углу. \en By angle 
  sm_Height    ///< \ru По величине отклонения от хорды. \en By the deviation from the chord. 
};


//------------------------------------------------------------------------------
/** \brief \ru Способ задания ширины сгиба.
           \en The method of bend width definition. \~
  \details \ru Способ задания ширины сгиба. \n
           \en The method of bend width definition. \n \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
enum MbeBendWidthType {
  bwt_KFactor,   ///< \ru Ширина сгиба рассчитывается через коэффициент нейтрального слоя. \en The bending width is computed using neutral layer coefficient. 
  bwt_Allowance, ///< \ru Ширина сгиба задана непосредственно. \en The bend width is defined explicitly. 
  bwt_Deduction, ///< \ru Задано уменьшение сгиба. \en The bend tapering is defined. 
  bwt_Table      ///< \ru Ширина сгиба рассчитывается по таблице. \en The bend width is computed from a table. 
};


//------------------------------------------------------------------------------
/** \brief \ru Вспомогательные функции для построения комбинированного сгиба (сгиба по эскизу).
           \en Auxiliary functions for a composite bend (a bend from a sketch). \~
  \details \ru Вспомогательные функции для построения комбинированного сгиба (сгиба по эскизу). \n
           \en Auxiliary functions for a composite bend (a bend from a sketch). \n \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
class MATH_CLASS MbJointBendUtils {
  double                 thickness;         ///< \ru Толщина листа. \en The sheet thickness. 
  SPtr<MbContourOnPlane> contour;           ///< \ru Эскиз присоединяемых сгибов. \en A sketch of attachable bends. 
  MbOrientedEdge *       orEdge;            ///< \ru Ориентированное ребро, к которому присоединяются сгибы. \en An oriented edge to attach the bends to. 
  bool                   offsetToLeft;      ///< \ru Придавать толщину слева от эскиза. \en Whether to thicken to the left from the sketch. 
  bool                   edgePlaceCoorient; ///< \ru Совпадение направления ребра orEdge и оси Z локальной системы координат эскиза. \en Whether the direction of edge orEdge is equal to the direction of Z-axis of the sketch local coordinate system. 

public:
  /// \ru Способ построения комбинированного сгиба. \en The method of a composite bend construction. 
  enum MbeConstructionMethod {
    cmToEnd,      ///< \ru До конца ребра \en To the edge end. 
    cmByWidth,    ///< \ru На определённую ширину \en With a specified width. 
    cmByManyEdges ///< \ru По нескольким рёбрам \en From several edges. 
  };

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \param[in] placement - \ru Локальная система координат эскиза.
                           \en The local coordinate system of the sketch. \~
    \param[in] contour   - \ru Эскиз создаваемых сгибов.
                           \en The sketch of bends to create. \~
    \param[in] curveEdge - \ru Неориентированное ребро к которому прикрепляются создаваемые сгибы.
                           \en A non-oriented edge to attach the created bend to. \~
  */
  MbJointBendUtils( const MbPlacement3D & placement,
                    const MbContour &     contour,
                    const MbCurveEdge &   curveEdge );
  ~MbJointBendUtils();

  /** \brief \ru Рассчитать отступы от концов ребра.
             \en Calculate the distances from the edge ends. \~
    \details \ru Отступы имеют положительное значение в случае расширения сгибов относительно исходного тела, отрицательное - в случае сужения.
             \en The distances are positive if the bends extend relative to the source solid and negative if they narrow. \~
    \param[in]  method              - \ru Способ построения комбинированного сгиба.
                                      \en The method of a composite bend construction. \~
    \param[in]  widthAlongPlaceNorm - \ru Ширина сгиба в направлении нормали ЛСК контура для способа cmByWidth.
                                      \en The bend width in the direction of contour LCS normal for method cmByWidth. \~
    \param[in]  widthRevPlaceNorm   - \ru Ширина сгиба в направлении, противоположном нормали ЛСК контура для способа cmByWidth.
                                      \en The bend width in the opposite direction of contour LCS normal for method cmByWidth. \~
    \param[in]  alongPlaceNorm      - \ru Направление придания ширины сгиба для способа cmToEnd.
                                      \en The direction in which to thicken the bend for method cmToEnd. \~
    \param[out] begDistance         - \ru Возвращаемое значение отступа сгиба от начала ориентированного ребра.
                                      \en Returned value of the distance from the start of oriented edge to the bend. \~
    \param[out] endDistance         - \ru Возвращаемое значение отступа сгиба от конца ориентированного ребра.
                                      \en Returned value of the distance from the end of oriented edge to the bend. \~
    \return \ru true - в случае успеха операции, false - в противном случае.
            \en True if the operation succeeded, otherwise false. \~
  */
  bool        CalculateDistances( MbeConstructionMethod method,               // \ru Способ построения комбинированного сгиба \en The method of the composite bend construction 
                                  const double                widthAlongPlaceNorm,  // \ru Ширина сгиба в направлении нормали ЛСК контура для способа cmByWidth \en The bend width in the direction of contour LCS normal for method cmByWidth 
                                  const double                widthRevPlaceNorm,    // \ru Ширина сгиба в направлении, противоположном нормали ЛСК контура для способа cmByWidth \en The bend width in the opposite direction of contour LCS normal for method cmByWidth 
                                  const bool                  alongPlaceNorm,       // \ru Направление придания ширины сгиба для способа cmToEnd \en The direction in which to thicken the bend for method cmToEnd 
                                        double &              begDistance,          // \ru Возвращаемое значение отступа сгиба от начала ориентированного ребра \en Returned value of the distance from the start of oriented edge to the bend 
                                        double &              endDistance ) const;  // \ru Возвращаемое значение отступа сгиба от конца ориентированного ребра \en Returned value of the distance from the end of oriented edge to the bend 
  /** \brief \ru Рассчитать угол стыковки эскиза с листовым телом.
             \en Compute the angle of connection of a sketch with a sheet solid. \~ 
    \param[out] angle - \ru Угол стыковки эскиза с листовым телом.
                        \en The angle of connection of a sketch with a sheet solid. \~
    \return \ru true - в случае успеха операции, false - в противном случае.
            \en True if the operation succeeded, otherwise false. \~
  */
  bool        GetConnectionAngle(       double &              angle       ) const;
  /** \brief \ru Построить дугу, содержащую хот-точку радиуса сгиба.
             \en Create an arc that contains a hot point of the bend radius. \~
    \details \ru Дуга проходит по центру ширины присоединяемой пластины.
             \en The arc passes through the center (by width) of the sheet being attached. \~
    \param[in] curveEdge   - \ru Неориентированное ребро присоединения.
                             \en A non-oriented edge of the joint. \~
    \param[in] begDistance - \ru Отступ от начального края соответствующего curveEdge ориентированного ребра.
                             \en The distance from the start point of the oriented edge corresponding to curveEdge. \~
    \param[in] endDistance - \ru Отступ от конечного края соответствующего curveEdge ориентированного ребра.
                             \en The distance from the end point of the oriented edge corresponding to curveEdge. \~
    \param[in] radius      - \ru Радиус создаваемой дуги.
                             \en The arc radius. \~
    \param[in] bendIndex   - \ru Порядковый номер присоединяемого сгиба, ноль обозначает сгиб, которым формируемая пластина присоединяется к листовому телу.
                             \en The index of the bend to be attached. Zero value indicates the bend by which the sheet being constructed is attached to the sheet solid. \~
    \return \ru Построенную дугу.
            \en The created arc. \~
  */
  MbCurve3D * CreateHotPointArc ( const MbCurveEdge &         curveEdge,
                                  const double                begDistance,
                                  const double                endDistance,
                                  const double                radius,
                                  const size_t                bendIndex   ) const;

  /** \brief \ru Построить контур в системе координат, связанной с ребром.
             \en Create a contour in the coordinate system associated with the edge. \~
  */
  static MbContour * CreateInitialContour ( const MbCurveEdge &   curveEdge,
                                            const bool            orient,
                                            const MbContour &     contour,
                                            const MbPlacement3D & placement );
  /** \brief \ru Построить связанную с ребром систему координат.
             \en Create a coordinate system associated with the edge. \~
    \details \ru Связанная с ребром система координат начинается в точке ребра, находящейся на расстоянии begDistance от начала ребра в случае begOrient равным true или
            от конца в случае begOrient равном false. Расстояние отсчитывается внутрь ребра, если begDistance меньше нуля, и наружу, если begDistance больше нуля.
            Ось Z направлена вдоль ребра curveEdge и сонаправлена с осью Z modifiedPlacement. Ось X совпадает с нормалью к листовой грани, содержащей ребро curveEdge.
            Ось Y дополняет систему до правой.
             \en The coordinate system associated with the edge has the origin at the edge point located at distance begDistance from the edge start point if begOrient is equal to true or
            at distance begDistance from edge end point if begDistance is equal to false. The distance is measured to inside of the edge if begDistance is negative and outside the edge if begDistance is positive.
            Z-axis is directed along edge curveEdge and codirected with Z-axis of modifiedPlacement. X-axis is equal to the normal of the sheet face that contains edge curveEdge.
            Y-axis makes the system right-handed. \~
    \param[in] curveEdge  - \ru Неориентированное ребро присоединения сгибов.
                            \en The non-oriented edge of bends joint. \~
    \param[in] edgeOrient - \ru Ориентация ориентированного ребра, принадлежащего листовой грани и содержащего curveEdge.
                            \en The orientation of the oriented edge that belongs to the sheet face and contains curveEdge. \~
    \param[in] begDistance - \ru Расстояние от начала ориентированного ребра, определяемого параметрами curveEdge и edgeOrient.
                             \en The distance from the beginning of the oriented edge defined by parameters curveEdge and edgeOrient. \~
    \param[in,out] modifiedPlacement - \ru Искомая система координат.
                                       \en The required coordinate system. \~
    \return \ru true - в случае успеха операции, false - в противном случае.
            \en True if the operation succeeded, otherwise false. \~
  */
  static bool        SetPlacementToEdge   ( const MbCurveEdge &   curveEdge,
                                            const bool            edgeOrient,
                                            const double          begDistance,
                                                  MbPlacement3D & modifiedPlacement );

private:
  MbCurve *    CreateTwoSegmentsArc  ( const MbLineSegment & prevSegment,
                                       const MbLineSegment & nextSegment,
                                       const double          radius       ) const;

  bool           GetBendFormingCurves  ( const size_t bendIndex,
                                               MbCurve *&    prevCurve,
                                               MbCurve *&    nextCurve    ) const;

  bool           ContourConnectionPoint( const double          t            ) const;

  bool           PointOnOrientedEdge   ( const MbCartPoint3D & point        ) const;


  static bool    SmoothConnection      ( const MbLineSegment & prevSegment,
                                         const MbLineSegment & nextSegment  );

  MbJointBendUtils( const MbJointBendUtils & );               // \ru Не реализовано \en Not implemented 
  MbJointBendUtils & operator = ( const MbJointBendUtils & ); // \ru Не реализовано \en Not implemented 
};


//------------------------------------------------------------------------------
/** \brief \ru Расчётчик зависимого параметра буртика.
           \en The calculator of the dependent parameter of a bead. \~
  \details \ru Расчётчик зависимого параметра буртика по ширине основания и остальным параметрам. \n
           \en The calculator of the dependent parameter of a bead from the base width and the other parameters. \n \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
class MATH_CLASS MbBeadParamCalculator {
public:
  /// \ru Рассчитать зависимый параметр буртика. \en Compute the dependent parameter of the bead. 
  static bool CalculateBeadParam   ( const double baseWidth, MbBeadValues & parameters );

private:
  static bool CalculateHight       ( const double baseWidth, MbBeadValues & parameters );
  static bool CalculateBottomRadius( const double baseWidth, MbBeadValues & parameters );
  static bool CalculateAngle       ( const double baseWidth, MbBeadValues & parameters );
  static bool CalculateBottomWidth ( const double baseWidth, MbBeadValues & parameters );
  static bool CheckConsistency     ( const double baseWidth, MbBeadValues & parameters );
};


//------------------------------------------------------------------------------
/** \brief \ru Расчётчик коэффициента нейтрального слоя.
           \en Calculator of the neutral layer coefficient. \~
  \details \ru Расчётчик коэффициента нейтрального слоя для различных способов задания ширины разогнутого сгиба. \n
           \en Calculator of the neutral layer coefficient for different methods of the unbended bend width definition. \n \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
class MATH_CLASS MbKCalculator {
  double thickness; ///< \ru Толщина сгиба. \en The width of the bend. 
  double radius;    ///< \ru Внутренний радиус сгиба. \en The internal radius of the bend. 
  double angle;     ///< \ru Угол сгиба. \en The bend angle. 

public:
  /// \ru Конструктор. \en Constructor. 
  MbKCalculator( double thick, double rad, double ang );
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbKCalculator( const MbKCalculator & init )
    : thickness( init.thickness )
    , radius( init.radius )
    , angle( init.angle )
  {}
  /// \ru Оператор присваивания. \en An assignment operator. 
  MbKCalculator & operator = ( const MbKCalculator &init ) {
    thickness = init.thickness; radius = init.radius; angle = init.angle; return *this; }

  /// \ru Рассчитать коэффициент нейтрального слоя с возвратом кода ошибки. \en Calculate the neutral layer coefficient and return the error code. 
  MbResultType CalcK( const double l, const MbeBendWidthType type, double & k ) const;
  /// \ru Рассчитать коэффициент нейтрального слоя. \en Calculate the neutral layer coefficient. 
  double CalcK( double l, MbeBendWidthType type ); 
};


//------------------------------------------------------------------------------
/** \brief \ru Расчётчик смещения сгиба.
           \en The bend displacement calculator. \~
  \details \ru Расчётчик смещения сгиба. \n
           \en The bend displacement calculator. \n \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
class MATH_CLASS MbDisplacementCalculator {
public:
  /// \ru Тип смещения. \en A displacement type. 
  enum MbeDisplacementType {
    dt_InIn,    ///< \ru Пересечение касательных к внутренним сторонам сгиба. \en Intersection of the tangents to the bend inner sides. 
    dt_OutIn,   ///< \ru Пересечение касательных к внешней и внутренней сторонам сгиба. \en Intersection of the tangents to the outer and the inner sides of the bend. 
    dt_InOut,   ///< \ru Пересечение касательных к внутренней и внешней сторонам сгиба. \en Intersection of the tangents to the inner and the outer sides of the bend. 
    dt_OutOut,  ///< \ru Пересечение касательных к внешним сторонам сгиба. \en Intersection of the tangents to the outer sides of the bend. 
    dt_OutProj, ///< \ru Проекция внешней стороны на касательную к внутренней стороне сгиба. \en The projection of the outer side onto the tangent to the inner side of the bend. 
    dt_InProj,  ///< \ru Проекция внутренней стороны не касательную к внутренней стороне сгиба. \en The projection of the inner side onto the tangent to the outer side of the bend.
    dt_Center   ///< \ru Сгиб по осевой линии. \en Bend Centerline.
  };

private:
  double thickness; ///< \ru Толщина листа. \en The sheet thickness. 
  double radius;    ///< \ru Внутренний радиус сгиба. \en The internal radius of the bend. 
  double angle;     ///< \ru Угол сгиба. \en The bend angle. 
  double k;         ///< \ru Коэффициент нейтрального слоя. \en K-Factor value.

public:
  /// \ru Конструктор. \en Constructor. 
  MbDisplacementCalculator( double thick, double rad, double ang, double coef = 0.4 )
    : thickness( ::fabs(thick) )
    , radius   ( ::fabs(rad) )
    , angle    ( ::fabs(ang) )
    , k        (        coef )
  {}
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbDisplacementCalculator( const MbDisplacementCalculator &init )
    : thickness( init.thickness )
    , radius( init.radius )
    , angle( init.angle )
    , k( init.k )
  {}
  /// \ru Оператор присваивания. \en An assignment operator. 
  MbDisplacementCalculator & operator = ( const MbDisplacementCalculator &init ) {
    thickness = init.thickness; radius = init.radius; angle = init.angle; k = init.k; return *this; }

  /// \ru Рассчитать смещение сгиба. \en Calculate the bend displacement. 
  double CalcDisplacement( MbDisplacementCalculator::MbeDisplacementType type );
  /// \ru Рассчитать радиус по смещению сгиба. \en Calculate the radius by bend displacement. 
  double CalcRadiusByDisplacement( MbeDisplacementType type, double displacement );
};


//------------------------------------------------------------------------------
/** \brief \ru Расчётчик параметров подрезанных жалюзи.
           \en The calculator of trimmed jalousie parameters. \~
  \details \ru Расчётчик параметров подрезанных жалюзи. \n
           \en The calculator of trimmed jalousie parameters. \n \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
class MATH_CLASS MbJalousieParamCalculator {
  MbJalousieValues parameters; ///< \ru Параметры жалюзи. \en Jalousie parameters. 
  double           thickness;  ///< \ru Толщина. \en The thickness. 

public:
  /// \ru Конструктор. \en Constructor. 
  MbJalousieParamCalculator( const MbJalousieValues & params, const double thick ) : parameters( params ), thickness( ::fabs(thick) ) {}

  /// \ru Инициализация. \en Initialization. 
  void Init                      ( const MbJalousieValues & params, const double thick ) { parameters = params; thickness = ::fabs(thick); }
  /// \ru Рассчитать высоту подрезанных жалюзи по высоте зазора. \en Calculate the height of trimmed jalousie given the gap height. 
  bool CalculateHeight           ( const double             gapHeight,
                                         double &           height ) const;
  /// \ru Рассчитать угол наклона жалюзи. \en Calculate the slope angle of jalousie. 
  bool CalculateAngle            (       double &           angle ) const;
  /// \ru Проверить возможность построения жалюзи с заданным углом наклона. \en Check if the construction of jalousie with the given slope angle is possible. 
  bool CanCreateJalousieWithAngle( const double             angle ) const;

private:
  bool CalculateAngle            ( const double             height,
                                   const bool               heightIsGap,
                                         double &           angle ) const;

  bool CalcAngleFunction( const double   angle,
                          const double   height,
                          const bool     heightIsGap,
                                double & f,
                                double & df ) const;

  MbJalousieParamCalculator( const MbJalousieParamCalculator & );               // \ru Не реализовано \en Not implemented 
  MbJalousieParamCalculator & operator = ( const MbJalousieParamCalculator & ); // \ru Не реализовано \en Not implemented 
};


//------------------------------------------------------------------------------
/** \brief \ru Расчётчик расположения хот-точки для зазора замыкания углов.
           \en Calculator of hot point location for the gap of corner closure. \~
  \details \ru Расчётчик расположения хот-точки для зазора замыкания углов. \n
           \en Calculator of hot point location for the gap of corner closure. \n \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
class MATH_CLASS MbCloseCornerGapHotPointCalc {
private:
  /// \ru Тип ребра замыкания. \en A corner edge type. 
  enum MbCornerEdgeType {
    cet_BendEdge = 0, ///< \ru Торцевое ребро сгиба листового тела. \en Butt edge of sheet solid bend. 
    cet_RipEdge  = 1  ///< \ru Ребро разъема нелистового тела(для операции преобразования в листовое тело). \en Rip edge of non sheet solid (for operation of converting to sheet solid). 
  };

private:
  const MbCornerEdgeType       edgeType;    ///< \ru Тип ребра. \en The type of edge. 
  const MbCurveEdge &          curveEdge;   ///< \ru Ребро. \en The edge. 
  const MbClosedCornerValues & parameters;  ///< \ru Параметры замыкания сгиба. \en The bend closure parameters. 
  const double                 thickness;   ///< \ru Толщина листового тела. \en The thickness of sheet solid.
  const bool                   sheetSense;  ///< \ru Направление придания толщины. \en The sense of thickness of sheet solid.
        bool                   edgeSense;   ///< \ru Флаг совпадения направления ребра разъема с направлением замыкания угла. \en The sense of the rip edge relative to corner direction.
  const VERSION                version;     ///< \ru Версия математики. \en Math version.

public:
  /// \ru Конструктор. \en Constructor. 
  MbCloseCornerGapHotPointCalc( const MbCurveEdge &          edge,
                                const MbClosedCornerValues & params,
                                const VERSION                mathVersion = Math::DefaultMathVersion() );

  /// \ru Конструктор расчетчика хот-точки зазора замыкания углов для операции распознавания в листовое тело. \en Constructor for case of operation of converting to sheet solid. 
  MbCloseCornerGapHotPointCalc( const bool                   sense,
                                const double                 sheetThickness,
                                const MbCurveEdge &          ripEdge,
                                const RPArray<MbCurveEdge> & ripChainEdges,
                                const RPArray<MbCurveEdge> & bendEdges,
                                const MbClosedCornerValues & params,
                                const VERSION                mathVersion = Math::DefaultMathVersion() );

  /// \ru Рассчитать положение "хот"-точки. \en Calculate the hot point location. 
  bool CalcHotPoint( MbCartPoint3D & point ) const;

private:
         bool FindAlongOrEdge( const bool              plus,
                                     MbOrientedEdge *& alongOrEdge,
                                     bool &            begin     ) const;

         bool SetPlacement   ( const MbOrientedEdge &  baseOrEdge,
                               const MbOrientedEdge &  refOrEdge,
                               const bool              refBegin,
                                     MbPlacement3D &   placement ) const;

  static bool SetLines       ( const MbOrientedEdge &  orEdge,
                               const bool              begin,
                                     MbLine3D &        line1,
                                     MbLine3D &        line2 );
         bool CalcByRipEdge  ( MbCartPoint3D & pnt )               const;

  MbCloseCornerGapHotPointCalc( const MbCloseCornerGapHotPointCalc & );               // \ru Не реализовано \en Not implemented 
  MbCloseCornerGapHotPointCalc & operator = ( const MbCloseCornerGapHotPointCalc & ); // \ru Не реализовано \en Not implemented 
};


//------------------------------------------------------------------------------
/** \brief \ru Расчётчик расположения хот-точки для зазора замыкания углов.
           \en Calculator of hot point location for the gap of corner closure. \~
  \details \ru Расчётчик расположения хот-точки для зазора замыкания углов. \n
           \en Calculator of hot point location for the gap of corner closure. \n \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
class MATH_CLASS MbRuledShellGapCalc {
  const MbRuledSolidValues &  parameters; ///< \ru Параметры обечайки. \en The ruled shell parameters. 

public:
  /// \ru Конструктор. \en Constructor. 
  MbRuledShellGapCalc( const MbRuledSolidValues & params ) : parameters( params ) {}
  /// \ru Рассчитать хот-точки зазора. \en Calculate hot points of the gap. 
  bool   CalcHotPoints   ( const MbContour &             filletedContour,
                           const MbPlacement3D &         placement,
                                 MbAxis3D &              centerAxis,
                                 MbAxis3D &              oppositeAxis,
                                 MbCartPoint3D &         edgePoint      );
  /// \ru Рассчитать новое значение зазора по "хот"-точке. \en Calculate the new value of the gap given the hot point. 
  double CalcGapValue    ( const MbContour &             filletedContour,
                           const MbPlacement3D &         placement,
                           const MbCartPoint3D &         point          ) const;
  /// \ru Рассчитать новое расположение зазора. \en Calculate new position of the gap. 
  double CalcGapPosition ( const MbContour &             filletedContour,
                           const MbPlacement3D &         placement,
                           const MbCartPoint3D &         newPoint       ) const;
  /// \ru Рассчитать новое расположение зазора. \en Calculate new position of the gap. 
  double CalcGapPosition ( const MbContour &             filletedContour,
                           const MbPlacement3D &         placement,
                           const MbCartPoint3D &         oldPoint,
                           const MbVector3D &            moveVector     ) const;
  /// \ru Найти параметр центра зазора. \en Calculate the parameters of the gap center. 
  double FindGapParameter( const MbContourOnPlane &      contourOnPlane ) const;
  /// \ru Найти параметры границ зазора. \en Calculate the gap boundary parameters. 
  bool   FindGapLimits   ( const MbContourOnPlane &      contourOnPlane,
                           const double                  tGapMiddle,
                                 double &                tGapMin,
                                 double &                tGapMax        ) const;

private:
  MbRuledShellGapCalc( const MbRuledShellGapCalc & );               // \ru Не реализовано \en Not implemented 
  MbRuledShellGapCalc & operator = ( const MbRuledShellGapCalc & ); // \ru Не реализовано \en Not implemented 
};


//------------------------------------------------------------------------------
/** \brief \ru Сгиб листового тела по линии.
           \en The bend of a sheet solid along a line. \~
  \details \ru Линией может быть отрезок, лежащий на плоских гранях bendingFaces, либо прямая.
           Грани bendingFaces располагаются на общей для них плоскости.\n
           \en The line is a line segment on the planar faces bendingFaces or a line.
           Faces bendingFaces lies on the common plane.\n \~
  \param[in] solid - \ru Исходное тело.
                     \en The source solid. \~
  \param[in] sameShell - \ru Флаг удаления оболочки исходного тела.
                         \en Whether to delete the shell of the source solid. \~
  \param[in] bendingFaces - \ru Изгибаемые грани.
                            \en The faces to bend. \~
  \param[in] curve - \ru Прямолинейная кривая, вдоль которой гнуть.
                     \en A straight line along which to bend. \~
  \param[in] unbended - \ru Флаг построения элемента в разогнутом состоянии.
                        \en Whether to construct the element unbended. \~
  \param[in] params - \ru Параметры листового тела.
                      \en A sheet solid parameters. \~
  \param[in] nameMaker - \ru Именователь.
                         \en An object for naming the new objects. \~
  \param[out] result - \ru Результирующее тело.
                       \en The resultant solid. \~
  \result \ru - Код результата операции.
          \en - The operation result code. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (MbResultType) BendSheetSolidOverSegment(       MbSolid &             solid,
                                                          MbeCopyMode           sameShell,
                                                    const RPArray<MbFace> &     bendingFaces,
                                                          MbCurve3D &           curve,
                                                          bool                  unbended,
                                                    const MbBendOverSegValues & params,
                                                    const MbSNameMaker &        nameMaker,
                                                          MbSolid *&            result );


//------------------------------------------------------------------------------
/** \brief \ru Подсечка.
           \en A jog. \~
  \details \ru Линией может быть отрезок, лежащий на плоских гранях bendingFaces, либо прямая.
           Грани bendingFaces располагаются на общей для них плоскости. Подсечка выполняется
           в виде двух смещённых друг относительно друга сгиба по линии.
           Формируемые при этом листовые грани сгибов возвращаются в массивах:\n
           firstBendFaces - грани сгибов, примыкающие к неподвижной части базовых граней,\n
           secondBendFaces - грани сгибов, поднятых над базовыми гранями.\n
           \en The line is a line segment on the planar faces bendingFaces or a line.
           Faces bendingFaces lies on the common plane. A jog is performed
           as two bends by a line shifted relative to each other.
           The sheet faces of bends generated during this operation are returned in the arrays: \n
           firstBendFaces - the bend faces adjacent to the fixed part of the base faces,\n
           secondBendFaces - the bend faces raised above the base faces.\n \~
  \param[in] solid - \ru Исходное тело.
                     \en The source solid. \~
  \param[in] sameShell - \ru Флаг удаления оболочки исходного тела.
                         \en Whether to delete the shell of the source solid. \~
  \param[in] bendingFaces - \ru Изгибаемые грани.
                            \en The faces to bend. \~
  \param[in] curve - \ru Прямолинейная кривая, вдоль которой гнуть.
                     \en A straight line along which to bend. \~
  \param[in] unbended - \ru Флаг построения элемента в разогнутом состоянии.
                        \en Whether to construct the element unbended. \~
  \param[in] jogParams - \ru Параметры подсечки и первого сгиба.
                         \en The parameters of a jog and the first bend. \~
  \param[in] secondBendParams - \ru Параметры второго сгиба.
                                \en The second bend parameters. \~
  \param[in] nameMaker - \ru Именователь.
                         \en An object for naming the new objects. \~
  \param[out] firstBendFaces - \ru Грани первого сгиба подсечки.
                               \en The faces of the first bend of the jog. \~
  \param[out] secondBendFaces - \ru Грани второго сгиба подсечки.
                                \en The faces of the second bend of the jog. \~
  \param[out] result - \ru Результирующее тело.
                       \en The resultant solid. \~
  \result \ru - Код результата операции.
          \en - The operation result code. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (MbResultType) SheetSolidJog(       MbSolid &         solid,
                                              MbeCopyMode       sameShell,
                                        const RPArray<MbFace> & bendingFaces,
                                              MbCurve3D &       curve,
                                              bool              unbended,
                                        const MbJogValues &     jogParams,
                                        const MbBendValues &    secondBendParams,
                                        const MbSNameMaker &    nameMaker,
                                              RPArray<MbFace> & firstBendFaces,
                                              RPArray<MbFace> & secondBendFaces,
                                              MbSolid *&        result );


//------------------------------------------------------------------------------
/** \brief \ru Согнуть сгибы листового тела.
           \en Rebend the sheet solid bends. \~
  \details \ru Сгибаются разогнутые сгибы bends относительно неподвижной грани fixedFace.
           Если fixedFace - это листовая грань, принадлежащая одному из сгибов bends,
           то сгиб осуществляется так, чтобы неподвижной осталась плоскость, касательная
           к поверхности, лежащей под fixedFace, в точке fixedPoint.\n
           \en The unbended bends 'bends' are bended relative to the fixed face 'fixedFace'.
           If 'fixedFace' is a sheet face that belongs to one of bends 'bends',
           then bending is performed such that the plane tangent to
           the underlying surface of 'fixedFace' at point 'fixedPoint' remains fixed.\n \~
  \param[in] solid - \ru Исходное тело.
                     \en The source solid. \~
  \param[in] sameShell - \ru Флаг удаления оболочки исходного тела.
                         \en Whether to delete the shell of the source solid. \~
  \param[in] bends - \ru Множество сгибов, состоящих из пар граней - внутренней и внешней граней сгиба.
                     \en An array of bends which consist of face pairs - inner and outer faces of the bend. \~
  \param[in] fixedFace - \ru Грань, остающаяся неподвижной.
                         \en The face that remains fixed. \~
  \param[in] fixedPoint - \ru Точка в параметрической области поверхности, лежащей под гранью fixedFace, в случае, если она сгибовая.
                          \en A point in the domain of the underlying surface of face 'fixedFace' if this face is a face of the bend. \~
  \param[in] nameMaker - \ru Именователь.
                         \en An object for naming the new objects. \~
  \param[out] result - \ru Результирующее тело.
                       \en The resultant solid. \~
  \result \ru - Код результата операции.
          \en - The operation result code. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (MbResultType) BendSheetSolid(       MbSolid &                   solid,
                                               MbeCopyMode                 sameShell,
                                         const RPArray<MbSheetMetalBend> & bends,
                                         const MbFace &                    fixedFace,
                                         const MbCartPoint &               fixedPoint,
                                         const MbSNameMaker &              nameMaker,
                                               MbSolid *&                  result );


//------------------------------------------------------------------------------
/** \brief \ru Разогнуть сгибы листового тела.
           \en Unbend the bends of a sheet solid. \~
  \details \ru Разгибаются сгибы bends относительно неподвижной грани fixedFace.
           Если fixedFace - это листовая грань, принадлежащая одному из сгибов bends,
           то разгиб осуществляется так, чтобы неподвижной осталась плоскость, касательная
           к поверхности, лежащей под fixedFace, в точке fixedPoint.\n
           \en Bends 'bends' are to be unbended relative to the fixed face 'fixedFace'.
           If 'fixedFace' is a sheet face that belongs to one of bends 'bends',
           unbending is performed such that the plane tangent to
           the underlying surface of 'fixedFace' at point 'fixedPoint' remains fixed.\n \~
  \param[in] solid - \ru Исходное тело.
                     \en The source solid. \~
  \param[in] sameShell - \ru Флаг удаления оболочки исходного тела.
                         \en Whether to delete the shell of the source solid. \~
  \param[in] bends - \ru Множество сгибов, состоящих из пар граней - внутренней и внешней граней сгиба.
                     \en An array of bends which consist of face pairs - inner and outer faces of the bend. \~
  \param[in] fixedFace - \ru Грань, остающаяся неподвижной.
                         \en The face that remains fixed. \~
  \param[in] fixedPoint - \ru Точка в параметрической области грани fixedFace, в случае, если она сгибовая.
                          \en A point in the domain of face 'fixedFace' if it is a face of the bend. \~
  \param[in] nameMaker - \ru Именователь.
                         \en An object for naming the new objects. \~
  \param[out] result - \ru Результирующее тело.
                       \en The resultant solid. \~
  \param[out] ribContours - \ru Набор контуров содержащих кривые границ ребер жесткости(при их наличии) в разогнутом виде.
                            \en The set of contours, which are containing edges of stamp rib in unfolded state. \~
  \result \ru - Код результата операции.
          \en - The operation result code. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (MbResultType) UnbendSheetSolid(       MbSolid &                   solid,
                                                 MbeCopyMode                 sameShell,
                                           const RPArray<MbSheetMetalBend> & bends,
                                           const MbFace &                    fixedFace,
                                           const MbCartPoint &               fixedPoint,
                                           const MbSNameMaker &              nameMaker,
                                                 MbSolid *&                  result,
                                                 RPArray<MbContour3D> *      ribContours = nullptr );


//------------------------------------------------------------------------------
/** \brief \ru Создать листовое тело.
           \en Create a sheet solid. \~
  \details \ru Листовое тело создаётся выдавливанием одного незамкнутого контура или нескольких замкнутых контуров.\n
           В случае замкнутых контуров, один контур должен быть внешним, а остальные внутренними, и выдавливание
           производится на толщину листового тела.\n
           В случае незамкнутого контура, ему придаётся толщина листового тела в ту или иную в зависимости от параметров сторону,
           а затем результат выдавливается на заданные расстояния.\n
           \en A sheet solid is created by extrusion of one open contour or several closed contours.\n
           In the case of several closed contours one contour should be outer, the others should be inner; extrusion
           is performed by a distance equal to the sheet solid thickness.\n
           In the case of open contour it is supplied with the sheet solid thickness in one or another direction subject to the parameters,
           then the result is extruded by the specified distances.\n \~
  \param[in] placement - \ru Плейсмент эскиза.
                         \en A sketch placement. \~
  \param[in] contours - \ru Контуры листового тела.
                        \en The sheet solid contours. \~
  \param[in] unbended - \ru Флаг построения элемента в разогнутом состоянии.
                        \en Whether to construct the element unbended. \~
  \param[in] params - \ru Параметры листового тела.
                      \en A sheet solid parameters. \~
  \param[in] nameMakers - \ru Именователи.
                          \en Objects for naming the new objects. \~
  \param[out] resultBends - \ru Формируемые сгибы.
                            \en The resultant bends. \~
  \param[out] result - \ru Результирующее тело.
                       \en The resultant solid. \~
  \result \ru - Код результата операции.
          \en - The operation result code. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (MbResultType) CreateSheetSolid( const MbPlacement3D &          placement,
                                                 RPArray<MbContour> &     contours,
                                                 bool                     unbended,
                                           const MbSheetMetalValues &     params,
                                                 RPArray<MbSNameMaker> *  nameMakers,
                                                 RPArray<MbSMBendNames> & resultBends,
                                                 MbSolid *&               result );


//------------------------------------------------------------------------------
/** \brief \ru Добавление пластины к листовому телу.
           \en Addition of a plate to the sheet solid. \~
  \details \ru Пластина строится по одному или нескольким замкнутым непересекающимся контурам,
           Причём среди них может быть несколько внешних. \n
           \en A plate is constructed from one or several closed non-intersecting contours;
           And several contours can be outer. \n \~
  \param[in] solid - \ru Исходное тело.
                     \en The source solid. \~
  \param[in] sameShell - \ru Флаг удаления оболочки исходного тела.
                         \en Whether to delete the shell of the source solid. \~
  \param[in] placement - \ru Локальная система координат эскиза.
                         \en The local coordinate system of the sketch. \~
  \param[in] contours - \ru Замкнутый контур пластины.
                        \en The closed contour of the plate. \~
  \param[in] params - \ru Параметры листового тела.
                      \en A sheet solid parameters. \~
  \param[in] nameMakers - \ru Именователь.
                          \en An object for naming the new objects. \~
  \param[out] result - \ru Результирующее тело.
                       \en The resultant solid. \~
  \result \ru - Код результата операции.
          \en - The operation result code. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (MbResultType) SheetSolidPlate(       MbSolid &               solid,
                                                MbeCopyMode             sameShell,
                                          const MbPlacement3D &         placement,
                                                RPArray<MbContour> &    contours,
                                          const MbSheetMetalValues &    params,
                                                RPArray<MbSNameMaker> * nameMakers,
                                                MbSolid *&              result );


//------------------------------------------------------------------------------
/** \brief \ru Вырез отверстия в листовом теле.
           \en Create a hole in a sheet solid. \~
  \details \ru Вырез строится по замкнутому контуру. \n
           \en A hole is constructed by a closed contour. \n \~
  \param[in] solid - \ru Исходное тело.
                     \en The source solid. \~
  \param[in] sameShell - \ru Флаг удаления оболочки исходного тела.
                         \en Whether to delete the shell of the source solid. \~
  \param[in] placement - \ru Плейсмент эскиза.
                         \en A sketch placement. \~
  \param[in] contours - \ru Замкнутый контур выреза/пересечения.
                        \en A closed contour of a hole/intersection. \~
  \param[in] params - \ru Параметры листового тела.
                      \en A sheet solid parameters. \~
  \param[in] diff - \ru Отверстие (diff = true), пересечение (diff = false).
                    \en The hole (diff = true), the intersection (diff = false). \~
  \param[in] nameMakers - \ru Именователи.
                          \en Objects for naming the new objects. \~
  \param[out] result - \ru Результирующее тело.
                       \en The resultant solid. \~
  \result \ru - Код результата операции.
          \en - The operation result code. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (MbResultType) SheetSolidHole(       MbSolid &               solid,
                                               MbeCopyMode             sameShell,
                                         const MbPlacement3D &         placement,
                                               RPArray<MbContour> &    contours,
                                         const MbSheetMetalValues &    params,
                                               bool                    diff,
                                               RPArray<MbSNameMaker> * nameMakers,
                                               MbSolid *&              result );


//------------------------------------------------------------------------------
/** \brief \ru Сгиб на ребре.
           \en A bend on an edge. \~
  \details \ru Сгиб строится на одном или нескольких рёбрах, принадлежащих плоской листовой грани,
           согласно заданным параметрам. \n
           \en A bend is constructed on one or several edges that belong to a planar sheet face
           from the specified parameters. \n \~
  \param[in] solid - \ru Исходное тело.
                     \en The source solid. \~
  \param[in] sameShell - \ru Флаг удаления оболочки исходного тела.
                         \en Whether to delete the shell of the source solid. \~
  \param[in] params - \ru Параметры сгиба на ребре.
                      \en The parameters of a bend on an edge. \~
  \param[in,out] bends - \ru Параметры формируемых сгибов и имена созданных граней.
                         \en Bends parameters and names of the bends faces. \~
  \param[out] resultSolid - \ru Результирующее тело.
                            \en The resultant solid. \~
  \result \ru - Код результата операции.
          \en - The operation result code. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (MbResultType) BendSheetSolidByEdges( const c3d::SolidSPtr &                 solid,
                                                const MbeCopyMode                      sameShell,
                                                const MbBendSheetSolidByEdgesParams &  params,
                                                      std::vector<MbSMBendNames *> &   bends,
                                                      c3d::SolidSPtr &                 resultSolid );


//------------------------------------------------------------------------------
/** \brief \ru Сгиб на ребре.
           \en A bend on an edge. \~
  \details \ru Сгиб строится на одном или нескольких рёбрах, принадлежащих плоской листовой грани,
           согласно заданным параметрам. \n
           \en A bend is constructed on one or several edges that belong to a planar sheet face
           from the specified parameters. \n \~
  \deprecated \ru Функция устарела, взамен использовать #BendSheetSolidByEdges с параметрами #MbBendSheetSolidByEdgesParams.
              \en The function is deprecated, instead use #BendSheetSolidByEdges with parameters #MbBendSheetSolidByEdgesParams. \~
  \param[in] solid - \ru Исходное тело.
                     \en The source solid. \~
  \param[in] sameShell - \ru Флаг удаления оболочки исходного тела.
                         \en Whether to delete the shell of the source solid. \~
  \param[in] edges - \ru Рёбра сгибов.
                     \en The edges of bends. \~
  \param[in] unbended - \ru Флаг построения элемента в разогнутом состоянии.
                        \en Whether to construct the element unbended. \~
  \param[in] params - \ru Параметры сгибов.
                      \en The bends parameters. \~
  \param[in] nameMaker - \ru Именователь.
                         \en An object for naming the new objects. \~
  \param[in,out] resultBends - \ru Параметры формируемых сгибов и имена созданных граней.
                               \en Bends parameters and names of the bends faces. \~
  \param[out] result - \ru Результирующее тело.
                       \en The resultant solid. \~
  \result \ru - Код результата операции.
          \en - The operation result code. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
//DEPRECATE_DECLARE_REPLACE( BendSheetSolidByEdges with MbBendSheetSolidByEdgesParams )
MATH_FUNC (MbResultType) BendSheetSolidByEdges(       MbSolid &                solid,
                                                const MbeCopyMode              sameShell,
                                                const RPArray<MbCurveEdge> &   edges,
                                                const bool                     unbended,
                                                const MbBendByEdgeValues &     params,
                                                const MbSNameMaker &           nameMaker,
                                                      RPArray<MbSMBendNames> & resultBends,
                                                      MbSolid *&               result );


// устаревшая
//DEPRECATE_DECLARE_REPLACE( BendSheetSolidByEdges with MbBendSheetSolidByEdgesParams )
MATH_FUNC (MbResultType) BendSheetSolidByEdges(       MbSolid &               solid,
                                                const MbeCopyMode             sameShell,
                                                const RPArray<MbCurveEdge> &  edges,
                                                const bool                    unbended,
                                                const MbBendByEdgeValues &    params,
                                                const MbSNameMaker &          nameMaker,
                                                      MbSolid *&              result );


//------------------------------------------------------------------------------
/** \brief \ru Комбинированный сгиб листового тела.
           \en A composite bend of a sheet solid. \~
  \details \ru Комбинированный сгиб листового тела или другими словами сгиб по эскизу может строиться
           на одном или нескольких соседних прямолинейных рёбрах одной листовой грани или нескольких,
           расположенных через сгиб. Эскиз, состоящий из отрезков и дуг должен лежать в плоскости,
           перпендикулярной одному из рёбер построения и одним концом располагаться на его проекции на эту плоскость.
           Данный эскиз применяется к каждому ребру, участвующему в построении. По нему и его копиям для всех рёбер
           строятся листовые тела со скруглениями негладких стыковок прямолинейных сегментов контура и гладкой стыковкой к
           базовой листовой грани. Построенные тела объединяются с базовым (исходным) телом, и затем
           осуществляются замыкания углов согласно заданным параметрам. После выполнения операции в массиве resultBends
           записаны все созданные ей сгибы.\n
           \en A composite bend of a sheet solid or, in the other words, a bend from a sketch can be constructed
           on one or several neighboring linear edges of one or several sheet faces
           from the different sides of the bend. A sketch consisting of line segments and arcs should lie on a plane
           perpendicular to one of edges of the construction; one of its ends should lie on the edge projection onto this plane.
           The specified sketch is applied to each edge involved in the construction. From this sketch and its copies for all the edges 
           sheet solids are created with rounding of non-smooth (G0) joints of line segments of the contour and smoothly connecting with
           the base sheet face. The constructed faces are united with the base (source) solid, and, then
           the corners are to be closed according to the specified parameters. After finishing the operation
           all the created edges are stored in array 'resultBends'.\n \~
  \param[in] solid - \ru Исходное тело.
                     \en The source solid. \~
  \param[in] sameShell - \ru Флаг удаления оболочки исходного тела.
                         \en Whether to delete the shell of the source solid. \~
  \param[in] placement - \ru Плейсмент контура сгиба.
                         \en The bend contour placement. \~
  \param[in] params - \ru Параметры сгиба.
                      \en The bend parameters. \~
  \param[in,out] bends - \ru Параметры формируемых сгибов и имена их граней.
                         \en Bends parameters and their faces names. \~
  \param[out] resultSolid - \ru Результирующее тело.
                            \en The resultant solid. \~
  \result \ru - Код результата операции.
          \en - The operation result code. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (MbResultType) SheetSolidJointBend( const c3d::SolidSPtr &                            solid,
                                              const MbeCopyMode                                 sameShell,
                                              const MbSheetSolidJointBendParams &               params,
                                                    std::vector<std::vector<MbSMBendNames *>> & bends,
                                                    c3d::SolidSPtr &                            resultSolid );


//------------------------------------------------------------------------------
/** \brief \ru Комбинированный сгиб листового тела.
           \en A composite bend of a sheet solid. \~
  \details \ru Комбинированный сгиб листового тела или другими словами сгиб по эскизу может строиться
           на одном или нескольких соседних прямолинейных рёбрах одной листовой грани или нескольких,
           расположенных через сгиб. Эскиз, состоящий из отрезков и дуг должен лежать в плоскости,
           перпендикулярной одному из рёбер построения и одним концом располагаться на его проекции на эту плоскость.
           Данный эскиз применяется к каждому ребру, участвующему в построении. По нему и его копиям для всех рёбер
           строятся листовые тела со скруглениями негладких стыковок прямолинейных сегментов контура и гладкой стыковкой к
           базовой листовой грани. Построенные тела объединяются с базовым (исходным) телом, и затем
           осуществляются замыкания углов согласно заданным параметрам. После выполнения операции в массиве resultBends
           записаны все созданные ей сгибы.\n
           \en A composite bend of a sheet solid or, in the other words, a bend from a sketch can be constructed
           on one or several neighboring linear edges of one or several sheet faces
           from the different sides of the bend. A sketch consisting of line segments and arcs should lie on a plane
           perpendicular to one of edges of the construction; one of its ends should lie on the edge projection onto this plane.
           The specified sketch is applied to each edge involved in the construction. From this sketch and its copies for all the edges 
           sheet solids are created with rounding of non-smooth (G0) joints of line segments of the contour and smoothly connecting with
           the base sheet face. The constructed faces are united with the base (source) solid, and, then
           the corners are to be closed according to the specified parameters. After finishing the operation
           all the created edges are stored in array 'resultBends'.\n \~
  \deprecated \ru Функция устарела, взамен использовать #SheetSolidJointBend с параметрами #MbSheetSolidJointBendParams.
              \en The function is deprecated, instead use #SheetSolidJointBend with parameters #MbSheetSolidJointBendParams. \~
  \param[in] solid - \ru Исходное тело.
                     \en The source solid. \~
  \param[in] sameShell - \ru Флаг удаления оболочки исходного тела.
                         \en Whether to delete the shell of the source solid. \~
  \param[in] placement - \ru Плейсмент контура сгиба.
                         \en The bend contour placement. \~
  \param[in] contour - \ru Контур сгиба.
                       \en The bend contour. \~
  \param[in] edges - \ru Рёбра сгиба.
                     \en The bend edges. \~
  \param[in] unbended - \ru Флаг построения элемента в разогнутом состоянии.
                        \en Whether to construct the element unbended. \~
  \param[in] params - \ru Параметры сгиба.
                      \en The bend parameters. \~
  \param[in] nameMaker - \ru Именователь.
                         \en An object for naming the new objects. \~
  \param[out] resultBends - \ru Формируемые сгибы.
                            \en The resultant bends. \~
  \param[out] result - \ru Результирующее тело.
                       \en The resultant solid. \~
  \result \ru - Код результата операции.
          \en - The operation result code. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
//DEPRECATE_DECLARE_REPLACE( SheetSolidJointBend with MbSheetSolidJointBendParams )
MATH_FUNC (MbResultType) SheetSolidJointBend(       MbSolid &                           solid,
                                              const MbeCopyMode                         sameShell,
                                              const MbPlacement3D &                     placement,
                                              const MbContour &                         contour,
                                              const RPArray<MbCurveEdge> &              edges,
                                              const bool                                unbended,
                                              const MbJointBendValues &                 params,
                                              const MbSNameMaker &                      nameMaker,
                                                    RPArray< RPArray<MbSMBendNames> > & resultBends,
                                                    MbSolid *&                          result );


//------------------------------------------------------------------------------
/** \brief \ru Замыкание угла.
           \en A corner enclosure. \~
  \details \ru Если на соседних рёбрах листовой грани построены два сгиба, то между ними образуется угол,
           который можно затянуть материалом, расширив соответствующие стороны этих сгибов,
           что и осуществляет данная операция. В параметрах можно выставить величину зазора и
           виды замыкания отдельно для сгибов и отдельно для их плоских продолжений.\n
           \en If two bends are created on the neighboring edges of a sheet face, a corner appears between them
           which can be covered by the material by extending the corresponding sides of these bends,
           that's what the operation does. The specified parameters can include a gap size and 
           types of enclosure separately for the bends and for their planar extensions.\n \~
  \param[in] solid - \ru Исходное тело.
                     \en The source solid. \~
  \param[in] sameShell - \ru Флаг удаления оболочки исходного тела.
                         \en Whether to delete the shell of the source solid. \~
  \param[in] curveEdgePlus - \ru Ребро сгиба, условно принятое за положительное.
                             \en The bend edge assumed to be positive. \~
  \param[in] curveEdgeMinus - \ru Ребро сгиба, условно принятое за отрицательное.
                              \en The bend edge assumed to be negative. \~
  \param[in] params - \ru Параметры замыкания.
                      \en The enclosure parameters. \~
  \param[in] nameMaker - \ru Именователь.
                         \en An object for naming the new objects. \~
  \param[out] result - \ru Результирующее тело.
                       \en The resultant solid. \~
  \result \ru - Код результата операции.
          \en - The operation result code. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (MbResultType) CloseCorner(       MbSolid &              solid,
                                            MbeCopyMode            sameShell,
                                            MbCurveEdge *          curveEdgePlus,
                                            MbCurveEdge *          curveEdgeMinus,
                                      const MbClosedCornerValues & params,
                                      const MbSNameMaker &         nameMaker,
                                            MbSolid *&             result );


//------------------------------------------------------------------------------
/** \brief \ru Подрезка массива тела solidArray контурами плоских листовых граней тела sheetSolid из params.
           \en Cutting the solidArray with contours of plane sheet faces of sheetSolid form params. \~
  \details \ru Для подрезания используются только грани, компланарные хотя бы одной ЛСК из массива placements.
               Контурные тела граней строятся выдавливанием ограничивающих контуров в обе стороны на величину depth.\n
           \en For cutting are used only those faces whose placements are complanar to the ones from placements array.
               The faces contours solids is built by extrusion of bounding contours of the faces in both directions on "depth" value.\n \~
  \param[in] solidArray - \ru Подрезаемое тело.
                          \en The solid to be cut. \~
  \param[in] sameShell - \ru Флаг удаления оболочки исходного тела.
                         \en Whether to delete the shell of the source solid. \~
  \param[in] params - \ru Параметры операции.
                      \en Operation parameters. \~
  \param[out] result - \ru Результирующее тело.
                       \en Result solid. \~
  \result \ru - Код результата операции.
          \en - The operation result code. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (MbResultType) CutSolidArrayByBorders(       MbSolid &                        solidArray,
                                                 const MbeCopyMode                      sameShells,
                                                 const MbCutSolidArrayByBordersParams & params,
                                                       c3d::SolidSPtr &                 resultSolid );


//------------------------------------------------------------------------------
/** \brief \ru Создание составляющих частей штамповки.
           \en Creation of stamping's components. \~
  \details \ru Штамповка строится посредством добавления к пластине выпуклой части и последующим вычитанием вогнутой.
               Данная функция возвращает обе эти части в качестве отдельных тел. Подрезка краями пластины, на которой находятся эскизы, не производится.\n
           \en The spherical stamp is created by adding a convex part to the plate and subtructing a concave part from it. This function returns these parts as separate solids.
               It does not cut them with the edges of the face on which the contours lay.\n \~
  \param[in] params - \ru Параметры штамповки.
                      \en The parameters of stamping. \~
  \param[out] resultSolids - \ru  Добавляемая и вычитаемая части штамповки.
                             \en Added and deductible parts of the stamp. \~
  \result \ru - Код результата операции.
          \en - The operation result code. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (MbResultType) CreateStampParts( const MbStampPartsParams & params,
                                                 MbStampPartsResult & resultSolids );


//------------------------------------------------------------------------------
/** \brief \ru Создание составляющих частей штамповки одного тела другим телом.
           \en Stamping with a tool solid (punch or die). \~
  \details \ru Штамповка строится на основе произвольного тела-инструмента и заданной плоской листовой грани.
               Штамповка подрезается границами листовой грани, которую пересекает тело.\n
           \en The stamping is created based on a tool body and a flat sheet face.
               The stamping is trimmed by the boundary of the sheet face which contains the sketch.\n \~
  \param[in] solid - \ru Листовое тело со штамповкой.
                     \en The sheet solid with stamp. \~
  \param[in] sameShell - \ru Флаг удаления оболочки исходного тела.
                         \en Whether to delete the shell of the source solid. \~
  \param[in] toolSolid - \ru Оболочка тела-инструмента.
                         \en A shell of tool solid. \~
  \param[in] sameShellTool - \ru Флаг удаления оболочки тела-инструмента.
                             \en Whether to delete the shell of the tool solid. \~
  \param[in] params - \ru Параметры штамповки.
                      \en The parameters of stamping. \~
  \param[out] resultSolids - \ru  Добавляемая и вычитаемая части штамповки.
                             \en Added and deductible parts of the stamp. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC(MbResultType) CreateStampWithToolSolidParts( const c3d::SolidSPtr &             solid,
                                                             MbeCopyMode                  sameShell, 
                                                       const c3d::SolidSPtr &             toolSolid, 
                                                             MbeCopyMode                  sameShellTool, 
                                                       const MbStampWithToolPartsParams & params,
                                                             MbStampWithToolPartsResult & resultSolids );


//------------------------------------------------------------------------------
/** \brief \ru Нормализовать кромки выреза листового тела.
           \en Normalize cut sides of the sheet solid. \~
  \details \ru Нормализовать кромки выреза листового тела. \n
           \en Normalize cut sides of the sheet solid. \n \~
  \param[in] solid - \ru Исходное тело.
                     \en The source solid. \~
  \param[in] sameShell - \ru Флаг удаления оболочки исходного тела.
                         \en Whether to delete the shell of the source solid. \~
  \param[in] normParam - \ru Набор параметров.
                         \en Parameters list. \~
  \param[out] result - \ru Результирующее тело.
                       \en The resultant solid. \~
  \result \ru - Код результата операции.
          \en - The operation result code. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC(MbResultType) NormalizeCutSides (       MbSolid &                   solid,
                                                  MbeCopyMode                 sameShell, 
                                            const MbNormalizeCutSidesParams & normParam,
                                                  c3d::SolidSPtr &            result );


//------------------------------------------------------------------------------
/** \brief \ru Штамповка.
           \en Stamping. \~
  \details \ru Штамповка строится по одному замкнутому или незамкнутому контуру, лежащему на плоской листовой грани.
           Замкнутый эскиз может лежать на листовой грани полностью или частично,
           а незамкнутый должен начинаться и заканчиваться за пределами грани.
           Штамповка подрезается границами листовой грани, на которой располагается эскиз.\n
           \en The stamping is created from one closed or open contour lying on a flat sheet face.
           A closed sketch can lie on the sheet face entirely or partially,
           and an open sketch must have the start and the end points outside the face.
           The stamping is trimmed by the boundary of the sheet face which contains the sketch.\n \~
  \param[in] solid - \ru Исходное тело.
                     \en The source solid. \~
  \param[in] sameShell - \ru Флаг удаления оболочки исходного тела.
                         \en Whether to delete the shell of the source solid. \~
  \param[in] params - \ru Параметры штамповки.
                      \en The parameters of stamping. \~
  \param[out] result - \ru Результирующее тело.
                       \en The resultant solid. \~
  \result \ru - Код результата операции.
          \en - The operation result code. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (MbResultType) Stamp( const c3d::SolidSPtr & solid,
                                      MbeCopyMode      sameShell,
                                const MbStampParams &  params,
                                      c3d::SolidSPtr & result );


//------------------------------------------------------------------------------
/** \brief \ru Штамповка телом-инструментом (пуансоном или матрицей).
           \en Stamping with a tool solid (punch or die). \~
  \details \ru Штамповка строится на основе произвольного тела-инструмента и заданной плоской листовой грани.
               Штамповка подрезается границами листовой грани, которую пересекает тело.\n
           \en The stamping is created based on a tool body and a flat sheet face.
               The stamping is trimmed by the boundary of the sheet face which contains the sketch.\n \~
  \param[in] solid - \ru Исходное листовое тело.
                     \en The source sheet solid. \~
  \param[in] sameShell - \ru Флаг удаления оболочки исходного тела.
                         \en Whether to delete the shell of the source solid. \~
  \param[in] toolSolid - \ru Оболочка тела-инструмента.
                         \en A shell of tool solid. \~
  \param[in] sameShellTool - \ru Флаг удаления оболочки тела-инструмента.
                             \en Whether to delete the shell of the tool solid. \~
  \param[in] params - \ru Параметры штамповки.
                      \en The parameters of stamping. \~
  \param[out] result - \ru Результирующее тело.
                       \en The resultant solid. \~
  \result \ru - Код результата операции.
          \en - The operation result code. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC( MbResultType ) StampWithToolSolid( const c3d::SolidSPtr &        solid,
                                                    MbeCopyMode             sameShell,
                                              const c3d::SolidSPtr &        toolSolid,
                                                    MbeCopyMode             sameShellTool,
                                              const MbStampWithToolParams & params,
                                                    c3d::SolidSPtr &        result );


//------------------------------------------------------------------------------
/** \brief \ru Создание составляющих частей сферической штамповки.
           \en Creation of spherical stamping's components. \~
  \details \ru Штамповка строится посредством добавления к пластине выпуклой части и последующим вычитанием вогнутой.
               Данная функция возвращает обе эти части в качестве отдельных тел. Подрезка краями пластины, на которой находятся эскизы, не производится.\n
           \en The spherical stamp is created by adding a convex part to the plate and subtructing a concave part from it. This function returns these parts as separate solids.
               It does not cut them with the edges of the face on which the contours lay.\n \~
  \param[in] params - \ru Параметры штамповки.
                      \en The parameters of stamping. \~
  \param[out] resultSolids - \ru  Добавляемая и вычитаемая части штамповки.
                             \en Added and deductible parts of the stamp. \~
  \result \ru - Код результата операции.
          \en - The operation result code. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (MbResultType) CreateSphericalStampParts( const MbSphericalStampPartsParams & params,
                                                          MbSphericalStampPartsResult & resultSolids );


//------------------------------------------------------------------------------
/** \brief \ru Сферическая штамповка.
           \en Spherical stamping. \~
  \details \ru Штамповка строится по параметрам и центру, лежащему на плоской листовой грани.
           Штамповка подрезается границами листовой грани, на которой располагается центр.\n
           \en The stamping is created by the parameters and a center lying on a flat sheet face.
           The stamping is trimmed by the boundary of the sheet face which contains the sketch.\n \~
  \param[in] solid - \ru Исходное тело.
                     \en The source solid. \~
  \param[in] sameShell - \ru Флаг удаления оболочки исходного тела.
                         \en Whether to delete the shell of the source solid. \~
  \param[in] params - \ru Параметры штамповки.
                      \en The parameters of stamping. \~
  \param[out] result - \ru Результирующее тело.
                       \en The resultant solid. \~
  \result \ru - Код результата операции.
          \en - The operation result code. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (MbResultType) SphericalStamp( const c3d::SolidSPtr &         solid,
                                               MbeCopyMode              sameShell,
                                         const MbSphericalStampParams & params,
                                               c3d::SolidSPtr &         result );


//------------------------------------------------------------------------------
/** \brief \ru Создание составляющих частей буртика.
           \en Creation of bead's components. \~
  \details \ru Буртик строится посредством добавления к пластине выпуклой части и последующим вычитанием вогнутой.
               Данная функция возвращает обе эти части в качестве отдельных тел. Подрезка краями пластины, на которой находятся эскизы, не производится.\n
           \en A bead is created by adding a convex part to the plate and subtructing a concave part from it. This function returns these parts as separate solids.
               It does not cut them with the edges of the face on which the contours lay.\n \~
  \param[in] face - \ru Грань, контуром которой надо подрезать буртик.
                    \en A face by which bounding contours the bead should be cutted. \~
  \param[in] placement - \ru Локальная система координат контуров.
                         \en The local coordinate system of the contours. \~
  \param[in] contours - \ru Контуры буртика.
                        \en The bead contours. \~
  \param[in] centers - \ru Центры сферических штамповок.
                       \en The spherical stamps centers. \~
  \param[in] params - \ru Параметры буртика.
                      \en The bead parameters. \~
  \param[in] thickness - \ru Толщина листа.
                         \en The sheet metal thickness. \~
  \param[in] nameMaker - \ru Имена контуров.
                         \en The contours names. \~
  \param[out] partToAdd - \ru Добавляемая часть буртика.
                          \en Added part of the bead. \~
  \param[out] partToSubtract - \ru Вычитаемая часть буртика.
                               \en Deductible part of the bead. \~
  \result \ru - Код результата операции.
          \en - The operation result code. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (MbResultType) CreateBeadParts( const MbFace *              face,
                                          const MbPlacement3D &       placement,
                                          const RPArray<MbContour> &  contours,
                                          const SArray<MbCartPoint> & centers,
                                          const MbBeadValues &        params,
                                          const double                thinkness,
                                          const MbSNameMaker &        nameMaker,
                                                MbSolid *&            partToAdd,
                                                MbSolid *&            partToSubtract );


//------------------------------------------------------------------------------
/** \brief \ru Буртик.
           \en A bead. \~
  \details \ru Буртик строится по одному или нескольким замкнутым или незамкнутым эскизам, лежащим на плоской листовой грани, а также по точкам.
           Если эскиз выходит за пределы этой грани, то буртик подрезается её границами.
           Буртик по незамкнутому эскизу на в начале и конце имеет законцовки, вид которых задаётся в параметрах операции. Буртик по точке имеет вид сферической штамповки.\n
           \en A bead is created from one or several closed or open sketches lying on a flat sheet face, as well as by points.
           If the sketch goes out of the face, the bead is trimmed by its boundary.
           A bead from an open sketch has two tips at the start and at the end points; the tips type is specified in the operation parameters. The bead by point looks like spherical stamp.\n \~
  \param[in] solid - \ru Исходное тело.
                     \en The source solid. \~
  \param[in] sameShell - \ru Флаг удаления оболочки исходного тела.
                         \en Whether to delete the shell of the source solid. \~
  \param[in] face - \ru Грань буртика.
                    \en The bead face. \~
  \param[in] placement - \ru Локальная система координат контуров.
                         \en The local coordinate system of the contours. \~
  \param[in] contours - \ru Контуры буртика.
                        \en The bead contours. \~
  \param[in] centers - \ru Центры сферических штамповок.
                       \en The spherical stamps centers. \~
  \param[in] params - \ru Параметры буртика.
                      \en The bead parameters. \~
  \param[in] nameMaker - \ru Имена контуров.
                         \en The contours names. \~
  \param[out] result - \ru Результирующее тело.
                       \en The resultant solid. \~
  \result \ru - Код результата операции.
          \en - The operation result code. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (MbResultType) CreateBead(       MbSolid &             solid,
                                           MbeCopyMode           sameShell,
                                     const MbFace &              face,
                                     const MbPlacement3D &       placement,
                                     const RPArray<MbContour> &  contours,
                                     const SArray<MbCartPoint> & centers,
                                     const MbBeadValues &        params,
                                     const MbSNameMaker &        nameMaker,
                                           MbSolid *&            result );


//------------------------------------------------------------------------------
/** \brief \ru Создание составляющих частей жалюзи.
           \en Creation of jalousie's components. \~
  \details \ru Вытянутые жалюзи строятся посредством добавления к пластине выпуклой части и последующим вычитанием вогнутой. Подрезанные жалюзи - наоборот
               сначала вычитанием прямоугольной заготовки из пластины, а затем добавлением к ней отогнутой части.
               Данная функция возвращает обе эти части в качестве отдельных тел. При выходе за края пластины жалюзи не строятся.\n
           \en A stratched jalousie is created by adding a convex part to the plate and subtructing a concave part from it.
               A cutted jalousie - vice versa by subtracting the rectangular part from the plate and then by adding the bent part to it.
               This function returns these parts as separate solids. If any part of the jalousie lay outside the plate it does not build.\n \~
  \param[in] face - \ru Грань, контуром которой надо подрезать буртик.
                    \en A face by which bounding contours the bead should be cutted. \~
  \param[in] placement - \ru Локальная система координат отрезка.
                         \en A local coordinate system of the segment. \~
  \param[in] segments - \ru Отрезки жалюзи.
                        \en The segments of jalousie. \~
  \param[in] params - \ru Параметры жалюзи.
                      \en The parameters of jalousie. \~
  \param[in] nameMaker - \ru Именователь.
                         \en An object for naming the new objects. \~
  \param[out] partToAdd - \ru Добавляемая часть жалюзи.
                          \en Added part of the jalousie. \~
  \param[out] partToSubtract - \ru Вычитаемая часть жалюзи.
                               \en Deductible part of the jalousie. \~
  \result \ru - Код результата операции.
          \en - The operation result code. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (MbResultType) CreateJalousieParts( const MbFace *                 face,
                                              const MbPlacement3D &          placement,
                                              const RPArray<MbLineSegment> & segments,
                                              const MbJalousieValues &       params,
                                              const double                   thickness,
                                              const MbSNameMaker &           nameMaker,
                                                    MbSolid *&               partToAdd,
                                                    MbSolid *&               partToSubtract );


//------------------------------------------------------------------------------
/** \brief \ru Жалюзи.
           \en Jalousie. \~
  \details \ru Жалюзи строятся на одном или нескольких отрезках, лежащих на плоской листовой грани.
           Жалюзи не могут выходить за пределы грани и пересекаться сами с собой. Жалюзи бывают двух видов:
           вытяжка и подрезка. Вытяжка имеет вид половины, разрезанного вдоль прямолинейного буртика,
           а подрезка имеет вид отогнутой пластины.\n
           \en Jalousie are crated from one or several line segments on a flat sheet face.
           Jalousie can't go out of the face boundary and can't crossed with itself. Jalousie can be of two types:
           stretch and cutting. A stretch looks like a half of a linear bead splitted lengthwise,
           and a cutting looks like a deflected slice.\n \~
  \param[in] solid - \ru Исходное тело.
                     \en The source solid. \~
  \param[in] sameShell - \ru Флаг удаления оболочки исходного тела.
                         \en Whether to delete the shell of the source solid. \~
  \param[in] face - \ru Грань жалюзи.
                    \en A face of jalousie. \~
  \param[in] placement - \ru Локальная система координат отрезка.
                         \en A local coordinate system of the segment. \~
  \param[in] segments - \ru Отрезки жалюзи.
                        \en The segments of jalousie. \~
  \param[in] params - \ru Параметры жалюзи.
                      \en The parameters of jalousie. \~
  \param[in] nameMaker - \ru Именователь.
                         \en An object for naming the new objects. \~
  \param[out] result - \ru Результирующее тело.
                       \en The resultant solid. \~
  \result \ru - Код результата операции.
          \en - The operation result code. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (MbResultType) CreateJalousie(       MbSolid &                solid,
                                               MbeCopyMode              sameShell,
                                         const MbFace &                 face,
                                         const MbPlacement3D &          placement,
                                         const RPArray<MbLineSegment> & segments,
                                         const MbJalousieValues &       params,
                                         const MbSNameMaker &           nameMaker,
                                               MbSolid *&               result );


//------------------------------------------------------------------------------
/** \brief \ru Обечайка по контуру.
           \en A ruled shell from a contour. \~
  \details \ru Обечайка строится по одному плоскому контуру выдавливанием с уклоном до, в общем случае, линейчатой поверхности
           и дальнейшим приданием её толщины.\n
           \en A ruled shell is created from a planar contour by extrusion with a slope to form a ruled surface in the general case,
           and then by supplying it with a thickness.\n \~
  \param[in] parameters - \ru Параметры обечайки.
                          \en A ruled shell parameters. \~
  \param[in] nameMaker - \ru Именователь с главным именем операции.
                         \en An object defining the main name of the operation. \~
  \param[out] resultBends - \ru Формируемые сгибы.
                            \en The resultant bends. \~
  \param[out] resultContour - \ru Contour, скруглённый по параметрам из resultBends.
                              \en 'Contour' rounded according to the parameters from 'resultBends'. \~
  \param[out] resultSolid - \ru Результирующее тело.
                            \en The resultant solid. \~
  \result \ru - Код результата операции.
          \en - The operation result code. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (MbResultType) CreateRuledSolid(       MbRuledSolidValues &     parameters,
                                           const MbSNameMaker &           nameMaker,
                                                 RPArray<MbSMBendNames> & resultBends,
                                                 MbContour *&             resultContour,
                                                 MbSolid *&               resultSolid );


//------------------------------------------------------------------------------
/** \brief \ru Объединение листовых тел по торцевой грани.
           \en A union of sheet solids by a side face. \~
  \details \ru Объединяет два листовых тела, если они касаются друг друга одной единственной боковинкой.\n
           \en Connects the two sheet solids, if they touch each other by an only side face.\n \~
  \param[in] solid1 - \ru Первое листовое тело.
                      \en The first sheet solid. \~
  \param[in] sameShell1 - \ru Способ использования первого листового тела.
                          \en Whether to delete the shell of the first source solid. \~
  \param[in] solid2 - \ru Второе листовое тело.
                      \en The second sheet solid. \~
  \param[in] sameShell2 - \ru Способ использования второго листового тела.
                          \en Whether to delete the shell of the second source solid. \~
  \param[in] names - \ru Именователь с версией операции.
                     \en An object defining the main name of the operation. \~
  \param[out] result - \ru Объединённое листовое тело.
                       \en The resultant solid. \~
  \result \ru - Код результата операции.
          \en - The operation result code. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (MbResultType) SheetSolidUnion(       MbSolid &      solid1,
                                          const MbeCopyMode    sameShell1,
                                                MbSolid &      solid2,
                                          const MbeCopyMode    sameShell2,
                                          const MbSNameMaker & names,
                                                MbSolid *&     result );


//------------------------------------------------------------------------------
/** \brief \ru Проверяет, что тела листовые и их можно объединить по торцевой грани.
           \en Checks that solids are sheet and they can be connected by a side face. \~
  \details \ru Функция завершается успешно, если находит единственную совпадающую в пространстве пару боковинок первого и второго тела.
           \en The function completed successfully if it finds an only overlapping in space pair of side faces belonging to the first and the second solids. \~
  \param[in] solid1 - \ru Первое листовое тело.
                      \en The first sheet solid. \~
  \param[in] solid2 - \ru Второе листовое тело.
                      \en The second sheet solid. \~
  \result \ru - true, если листовые тела можно объединить, false - в противном случае.
          \en - true if the sheet solids can be connected, false - otherwise. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (bool) CanUnionSheetSolids( const MbSolid & solid1,
                                      const MbSolid & solid2 );


//------------------------------------------------------------------------------
/** \brief \ru Восстановить боковые рёбра сгибов.
           \en Restore the side edges of the bends. \~
  \details \ru Операция служит для восстановления боковых границ сгибов после построений, которые могли их удалить,
           таких как вырез или скругление.\n
           \en The operation is used for restoring of the side boundaries of bends after the constructions which could delete them,
           such as cutting or fillet.\n \~
  \param[in] solid - \ru Исходное тело.
                     \en The source solid. \~
  \param[in] sameShell - \ru Флаг удаления оболочки исходного тела.
                         \en Whether to delete the shell of the source solid. \~
  \param[in] outerFaces - \ru Внешние грани сгибов, у которых восстанавливаем боковые рёбра.
                          \en The external faces whose side edges are to be restored. \~
  \param[in] strict - \ru При false - восстанавливаем боковые рёбра только там, где возможно без сообщений об ошибке.
                      \en If 'strict' = false, the side edges are to be restored only when the error message is not generated during the operation. \~
  \param[out] bends - \ru Сгибы, у которых восстановили боковые рёбра.
                      \en The bends for which the side edges has been restored. \~
  \param[in] nameMaker - \ru Именователь.
                         \en An object for naming the new objects. \~
  \param[out] result - \ru Результирующее тело.
                       \en The resultant solid. \~
  \result \ru - Код результата операции.
          \en - The operation result code. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (MbResultType) RestoreSideEdges(       MbSolid &                   solid,
                                                 MbeCopyMode                 sameShell,
                                           const RPArray<MbFace> &           outerFaces,
                                           const bool                        strict,
                                                 RPArray<MbSheetMetalBend> & bends,
                                           const MbSNameMaker &              nameMaker,
                                                 MbSolid *&                  result );


//------------------------------------------------------------------------------
/** \brief \ru Разделить сгибы по подоболочкам.
           \en Disjoint the bends by the common subshells. \~
  \details \ru Сгибы из bends группируются по принадлежности разным подоболочкам листового тела solid.
           В результате работы функции формируется взаимно однозначное соответствие групп сгибов
           и соответствующих этим группам неподвижных граней.\n
           \en The bends form 'bends' are grouped by belonging to different subshells of sheet solid 'solid'.
           As a result of the function a one-to-one correspondence is formed for the groups of bends
           and fixed faces corresponding to the groups.\n \~
  \param[in] solid - \ru Листовое тело.
                     \en A sheet solid. \~
  \param[in] bends - \ru Сгибы.
                     \en The bends. \~
  \param[in] fixedFaceName - \ru Имя неподвижной грани.
                             \en A fixed face name. \~
  \param[out] bendsGroups - \ru Сгибы, разделённые на группы по принадлежности разным подоболочкам.
                            \en The bends subdivided into groups by belonging to different subshells. \~
  \param[out] fixedFaces - \ru Соответствующие этим подоболочкам неподвижные грани.
                           \en The fixed faces corresponding to these subshells. \~
  \return \ru true - в случае успеха операции, false - в противном случае.
          \en True if the operation succeeded, otherwise false. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (bool) SeparateBendsBySubshells( const MbSolid &                              solid,
                                           const RPArray<MbSheetMetalBend> &            bends,
                                           const MbName &                               fixedFaceName,
                                                 RPArray< RPArray<MbSheetMetalBend> > & bendsGroups,
                                                 RPArray<const MbFace> &                fixedFaces );


//------------------------------------------------------------------------------
/** \brief \ru Разбить грани сгибов по парам.
           \en Collect the pairs of faces of the bends. \~
  \details \ru Ищутся составляющие сгиб внутренняя и внешняя грань среди
           неупорядоченного набора внешних и внутренних граней сгибов,
           по ним формируется сгиб, который добавляется в массив bends.\n
           \en The inner and the outer faces of a bend are searched among
           unordered set of external and internal faces of the bends;
           given these faces a bend is constructed and added to array 'bends'.\n \~
  \param[in] faceShell - \ru Набор граней листового тела.
                         \en A face set of the sheet solid. \~
  \param[in] innerFaces - \ru Внутренние грани сгибов.
                          \en The inner faces of the bends. \~
  \param[in] outerFaces - \ru Внешние грани сгибов.
                          \en The outer faces of the bends. \~
  \param[out] result - \ru Найденные пары граней, составляющие сгибы.
                       \en The face pairs forming the bends. \~
  \return \ru true - в случае успеха операции, false - в противном случае.
          \en True if the operation succeeded, otherwise false. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (bool) CollectBends( const MbFaceShell &               faceShell,
                               const RPArray<MbFace> &           innerFaces,
                               const RPArray<MbFace> &           outerFaces,
                                     RPArray<MbSheetMetalBend> & result );


//------------------------------------------------------------------------------
/** \brief \ru Проверить, что грань может быть выбрана в качестве фиксированной при сгибе/разгибе.
           \en Determine whether a face can be chosen as a fixed face for bending/unbending. \~
  \details \ru Проверяется, что указанная грань при сгибе/разгибе всех сгибов не изменится. \n
           \en The specified face is checked to be invariant while bending/unbending of all the bends. \n \~
  \param[in] - \ru Проверяемая грань.
               \en A face to check. \~
  \return \ru true - грань может быть выбрана в качестве фиксированной, false - в противном случае.
          \en True - the face can be chosen as a fixed face, false - otherwise. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (bool) IsSuitableForFixed( const MbFace & face );


//------------------------------------------------------------------------------
/** \brief \ru Найти грани, на которых лежит кривая.
           \en Find the faces containing the specified curve. \~
  \details \ru Кривая curve должна быть прямолинейной.\n
           \en The curve 'curve' should be linear.\n \~
  \param[in] faces - \ru Набор граней для поиска.
                     \en A face set for search. \~
  \param[in] curve - \ru Кривая, лежащая на некоторых из них.
                     \en A curve lying on some of the faces. \~
  \param[out] result - \ru Грани, на которых лежит кривая curve.
                           \en The faces containing the curve. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (void) FindCurveFaces( const RPArray<MbFace> & faces,
                                 const MbCurve3D &       curve,
                                       RPArray<MbFace> & result );


//------------------------------------------------------------------------------
/** \brief \ru Найти верхнюю/нижнюю грань листового тела, содержащую ребро.
           \en Find the upper/lower face of a sheet solid that contains the specified edge. \~
  \details \ru Поиск среди двух стыкующихся в ребре edge граней верхней или нижней грани листового тела. \n
           \en Find the upper or the lower face of a sheet solid among two faces adjacent by edge 'edge'. \n \~
  \param[in] edge - \ru Неориентированное ребро листовой грани.
                    \en A non-oriented edge of a sheet face. \~
  \return \ru Найденную листовую грань.
          \en A sheet face that has been found. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (MbFace *) FindSheetFace( const MbCurveEdge & edge );


//------------------------------------------------------------------------------
/** \brief \ru Найти парную грань сгиба.
           \en Find the pair face of a bend. \~
  \details \ru Поиск по листовой грани сгиба противоположной ей листовой грани.\n
           \en Find a sheet face opposite to the specified sheet face of a bend.\n \~
  \param[in] face - \ru Листовая грань сгиба.
                    \en A sheet face of a bend. \~
  \return \ru Искомую парную ей грань.
          \en The required pair face. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (MbFace *) FindPairBendFace( const MbFace & face );


//------------------------------------------------------------------------------
/** \brief \ru Найти плоскую парную листовую грань по ребру.
           \en Find a planar pair sheet face given en edge. \~
  \details \ru Функция поиска парной листовой грани для операции сгиб на ребре.
           Применяется для многотолщинных листовых тел в условиях,
           когда выбранной листовой грани соответствует несколько парных ей граней,
           находящихся на разном расстоянии от неё.
           Положительные расстояния begDistance и endDistance означают отступ наружу от ребра, а отрицательные - внутрь.\n
           \en The function of searching of a pair sheet face for the bend-on-edge operation.
           It is applied for multithickness sheet faces if
           the specified face corresponds to several pair faces
           at the different distances from it.
           The positive distances 'begDistance' and 'endDistance' mean that the distance is measured outside the edge, the negative ones means the distance inside the edge.\n \~
  \param[in] curveEdge - \ru Ребро, по которому искать.
                         \en The edge for which to search. \~
  \param[in] begDistance - \ru Расстояние от начала ребра.
                           \en The distance from the beginning of the edge. \~
  \param[in] endDistance - \ru Расстояние от конца ребра.
                           \en The distance from the edge end. \~
  \return \ru Искомую грань.
          \en The required face. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (MbFace *) GetPairPlanarFaceByEdge( const MbCurveEdge & curveEdge,
                                              const double        begDistance,
                                              const double        endDistance );


//------------------------------------------------------------------------------
/** \brief \ru Найти плоскую парную листовую грань по трёхмерной кривой.
           \en Find a planar pair sheet face given a three-dimensional curve. \~
  \details \ru Функция поиска парной листовой грани для операции сгиб по линии.
           Применяется для многотолщинных листовых тел в условиях,
           когда выбранной листовой грани соответствует несколько парных ей граней,
           находящихся на разном расстоянии от неё.
           \en The function of searching of a pair sheet face for the bend-along-a-line operation.
           It is applied for multithickness sheet faces if
           the specified face corresponds to several pair faces
           at the different distances from it. \~
  \param[in] sheetFace - \ru Плоская листовая грань.
                         \en A planar sheet face. \~
  \param[in] curve - \ru Лежащая на ней прямолинейная кривая.
                     \en A linear curve lying on the face. \~
  \return \ru Искомую грань.
          \en The required face. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (MbFace *) GetPairPlanarFaceByCurve( const MbFace &    sheetFace,
                                               const MbCurve3D & curve );


//------------------------------------------------------------------------------
/** \brief \ru Найти плоскую парную листовую грань по контуру.
           \en Find a planar pair sheet face given a contour. \~
  \details \ru Функция поиска парной листовой грани для операций базирующихся на контурах.
           Применяется для многотолщинных листовых тел в условиях,
           когда выбранной листовой грани соответствует несколько парных ей граней,
           находящихся на разном расстоянии от неё.
           \en The function of searching of a pair sheet face for the operations based on contours.
           It is applied for multithickness sheet faces if
           the specified face corresponds to several pair faces
           at the different distances from it. \~
  \param[in] shell - \ru Оболочка листового тела.
                     \en A shell of a sheet solid. \~
  \param[in] sheetFace - \ru Базовая листовая грань.
                         \en A base sheet face. \~
  \param[in] place - \ru Локальная система координат, лежащая на грани sheetFace.
                     \en A local coordinate system on the face sheetFace. \~
  \param[in] segments - \ru Кривые, лежащие в плоскости XY локальной системы координат placement.
                        \en Curves on XY-plane of the local coordinate system 'placement'. \~
  \return \ru Искомую грань.
          \en The required face. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (MbFace *) GetPairPlanarFaceByContour( const MbFaceShell &            shell,
                                                 const MbFace &                 sheetFace,
                                                 const MbPlacement3D &          place,
                                                 const RPArray<const MbCurve> & segments );


//------------------------------------------------------------------------------
/** \brief \ru Найти плоскую парную листовую грань.
           \en Find the planar pair sheet face. \~
  \details \ru Поиск осуществляется сначала через рёбра внешнего цикла грани sheetFace,
           в случае неудачи - через вершины этого цикла, и если грань не найдена,
           то перебором по всем связным граням или граням из набора faceShell.
           В последнем случае предпочтение отдаётся более близко расположенным граням.\n
           \en The search is firstly performed among the faces adjacent by the edges of outer loop of face 'sheetFace',
           and, if it fails, among the faces adjacent by the vertices of the loop, and, if the face is not found again,
           the search among all the connected faces or faces from set 'faceShell' is used.
           In the last case the closest faces are preferable.\n \~
  \param[in] faceShell - \ru Набор граней для поиска.
                         \en A face set for search. \~
  \param[in] sheetFace - \ru Исходная плоская грань.
                         \en The source planar face. \~
  \return \ru - Искомую плоскую грань.
          \en - The required planar face. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (MbFace *) GetPairPlanarFace( const MbFaceShell * faceShell,
                                        const MbFace &      sheetFace );


//------------------------------------------------------------------------------
/** \brief \ru Рассчитать расстояние между гранями.
           \en Compute the distance between faces. \~
  \details \ru Определяет расстояние между парой подобных граней.
           Подобными считаются пары плоских, цилиндрических и конических граней,
           у которых нормали коллинеарны и противоположно направлены.
           Расстояние считается положительным, если грани располагаются со стороны,
           противоположной направлению нормали, и отрицательным в противном случае.
           В случае ошибки возвращается 0.0.\n
           \en Determines the distance between a pair of a similar faces.
           Similar faces is a pair of a planar, cylindrical or conic faces
           which normals are collinear and have the opposite directions.
           The distance is considered to be positive if the faces are located on the side
           opposite to the normal direction, and to be negative otherwise.
           If an error occurred, returns 0.0.\n \~
  \param[in] face1 - \ru Первая грань.
                     \en The first face. \~
  \param[in] face2 - \ru Вторая грань.
                     \en The second face. \~
  \result \ru - Значение расстояния между гранями.
          \en - The distance between the faces. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (double) GetDistanceIfSameAndOpposite( const MbFace & face1,
                                                 const MbFace * face2 );


//------------------------------------------------------------------------------
/** \brief \ru Найти подобные сгибы.
           \en Find the similar bends. \~
  \details \ru В листовом теле shell ищутся согнутые цилиндрические/конические сгибы,
           которые надо добавить к сгибам из bends, чтобы они могли разогнуться, то есть
           сгибы разгибаемые только совместно.\n
           \en Bended cylindrical/conic bends are searched in sheet solid 'shell'
           which have to be added to bends 'bends' such that it will be possible to unbend the bends, i.e.
           the bends can be unbend only together.\n \~
  \param[in] shell - \ru Набор граней поиска.
                     \en The face set for the search. \~
  \param[in,out] bends - \ru Множество подобных сгибов.
                         \en An array of similar bends. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (void) GetSimilarCylindricBends( const MbFaceShell &               shell,
                                                 RPArray<MbSheetMetalBend> & bends );


//------------------------------------------------------------------------------
/** \brief \ru Рассчитать касательную точку для сгиба/разгиба.
           \en Compute a tangent point for bend/unbend. \~
  \details \ru В точке tangentPoint либо координаты точки касания внутри грани, то есть 0.0<=x<=1.0 и 0.0<=y<=1.0, или за её пределами.
           В первом случае точка касания пересчитывается в координаты лежащей под гранью поверхности,
           во втором находится одна из точек касания поверхности, лежащей под гранью face и плоскости plane.\n
           \en The tangent point is inside the face, i.e. 0.0<=x<=1.0 and 0.0<=y<=1.0, or outside the face.
           In the first case the tangent point is recomputed in the coordinates of the underlying surface of the face,
           in the second case one of the touching points of underlying surface of face 'face' and plane 'plane' is calculated.\n \~
  \param[in] face - \ru Грань, содержащая точку касания.
                    \en The face containing the tangent point. \~
  \param[in] plane - \ru Касательная плоскость.
                     \en The tangent plane. \~
  \param[in,out] tangentPoint - \ru Точка касания.
                                \en The tangent point. \~
  \return \ru true - в случае успеха операции, false - в противном случае.
          \en True if the operation succeeded, otherwise false. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (bool) CalculateTangentPoint( const MbFace &      face,
                                        const MbPlane &     plane,
                                              MbCartPoint & tangentPoint );


//------------------------------------------------------------------------------
/** \brief \ru Рассчитать осевую линию разогнутого конического сгиба.
           \en Calculate the centerline of an unbended conic bend. \~
  \details \ru Возвращает осевую линию в координатах параметрической области плоскости, лежащей под гранью face.\n
           \en Returns the centerline in the coordinates of the parametric domain of the underlying plane of the face 'face'.\n \~
  \param[in] face - \ru Листовая грань разогнутого конического сгиба.
                    \en A sheet face of the unbended conic bend. \~
  \param[out] axisLineSegment - \ru Искомая осевая линия.
                                \en The required centerline. \~
  \return \ru true - в случае успеха операции, false - в противном случае.
          \en True if the operation succeeded, otherwise false. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (bool) CalculateConicAxisLine( const MbFace &        face,
                                               MbLineSegment & axisLineSegment );


//------------------------------------------------------------------------------
/** \brief \ru Рассчитать осевые линии разогнутых сгибов.
           \en Calculate the centerlines of unfolded bends. \~
  \details \ru Возвращает трёхмерные осевые линии, лежащие на разогнутых гранях сгибов.\n
           \en Returns the 3D centerlines that lies on the unbent faces of the bends.\n \~
  \param[in] params   - \ru Исходные данные для расчета осевых линий разогнутых сгибов.
                        \en Input data for calculating centerlines of unfolded bends. \~
  \param[out] result  - \ru Искомые осевые линии.
                        \en The required centerlines. \~
  \return \ru true - в случае успеха операции, false - в противном случае.
          \en True if the operation succeeded, otherwise false. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (bool) BuildBends3DCenterlines( const MbBends3DLinesParams & params,
                                                MbBends3DLinesResult & result );


//------------------------------------------------------------------------------
/** \brief \ru Рассчитать параметры для замыкания угла.
           \en Calculate the parameters for the corner closure. \~
  \details \ru Находит общее ребро угла или пару рёбер для замыканий через сгиб.\n
           \en Find common edge for corner closure or two basic edges for corner closure across bend.\n \~
  \param[in] params   - \ru Выбранные данные (ребра или грани) для сторон угла, условно принятых за положительную и отрицательную.
                        \en Selected data (edges or faces) for the sides of the corner assumed to be positive and negative. \~
  \param[out] result  - \ru Рассчитанные параметры замыкания.
                        \en Calculated closure parameters. \~
  \return \ru true - в случае успеха операции, false - в противном случае.
          \en True if the operation succeeded, otherwise false. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (bool) GetParamsForCloseCorner( const MbCloseCornerParams &   params,
                                                MbCloseCornerResults &  result );


//------------------------------------------------------------------------------
/** \brief \ru Вычислить параметр сегментации кривой.
           \en Calculate parameter of segmentation. \~
  \details \ru Вычислить параметр сегментации для заданного метода. 
               При изменении метода результат сегментации не меняется.\n
           \en Calculate parameter of segmentation for the new method.
               Result of the segmentation doesn't change. \n \~
  \param[in] curve      - \ru Кривая (дуга).
                          \en Curve (arc).\~
  \param[in] method     - \ru Метод сегментации.
                          \en Segmentation method.\~
  \param[in] param      - \ru Параметр сегментации.
                          \en Segmentation parameter.\~
  \param[in] newMethod  - \ru Метод сегментации, для которого нужно вычислить значение параметра.
                          \en Segmentation method to calculate parameter for.\~
  \result newParam - \ru Вычисленный параметр.
                     \en Calculated parameter. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (double) CalculateSegmentationParameter( const MbCurve &             curve,
                                                   const MbeSegmentationMethod method,
                                                   const double                param,
                                                   const MbeSegmentationMethod newMethod );


//------------------------------------------------------------------------------
/** \brief \ru Аппроксимировать дуги контура ломаной.
           \en Split every arc of the contour into segments. \~
  \details \ru Аппроксимировать дуги контура ломаной.\n
           \en Split every arc of the contour into segments.\n \~
  \param[in] contour     - \ru Кривая или контур.
                           \en A curve or a contour.\~
  \param[in] segmNumber  - \ru Количество сегментов аппроксимации каждой дуги контура.
                           \en Number of segments for splitting every arc in the contour.\~
  \param[out] resultContour  - \ru Аппроксимированный отрезками контур.
                               \en Segmented contour. \~
  \result \ru - Код результата операции.
          \en - The operation result code. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (MbResultType) SplitContourIntoSegments( const MbCurve &    curve,
                                                   const size_t       segmNumber,
                                                         MbContour *& resultContour );


//------------------------------------------------------------------------------
/** \brief \ru Аппроксимировать участки контуров (дуги) ломаной.
           \en Split a part of a contours (an arc) into segments. \~
  \details \ru Аппроксимировать участки контуров (дуги) ломаной.\n
           \en Split a part of a contours (an arc) into segments.\n \~
  \param[in,out] contour1 -     \ru Первый контур.
                                \en First contour.\~
  \param[in,out] breaks1 -      \ru Массив параметров разбиения первого контура.
                                \en Parameters of a partition of the first contour. \~
  \param[in,out] contour2 -     \ru Второй контур.
                                \en Second contour.\~
  \param[in,out] breaks2 -      \ru Массив параметров разбиения второго контура.
                                \en Parameters of a partition of the second contour. \~
  \param[in,out] names -        \ru Именователь.
                                \en Name maker.
  \param[in]     segmNumbers1 - \ru Количество сегментов аппроксимации для каждого сегмента обечайки после применения разбиения по breaks1.
                                \en Number of the linear segments for each segment of the lofted bend after splitting by breaks1.
  \param[in]     segmNumbers2 - \ru Количество сегментов аппроксимации для каждого сегмента обечайки после применения разбиения по breaks2.
                                \en Number of the linear segments for each segment of the lofted bend after splitting by breaks2.
  \param[in]     defSegmNumb -  \ru Количество сегментов аппроксимации, если не задано в segmNumbers1 и segmNumbers2.
                                \en Number of segments after splitting, if not defined in segmNumbers1 и segmNumbers2.\~
  \param[in]     gapValue -     \ru Ширина зазора.
                                \en The gap width.
  \result \ru - Код результата операции.
          \en - The operation result code. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (MbResultType) SplitContoursIntoSegments(       MbContour &      contour1,
                                                          SArray<double> & breaks1,
                                                          MbContour &      contour2,
                                                          SArray<double> & breaks2,
                                                          MbSNameMaker &   names,
                                                    const SArray<size_t> & segmNumbers1,
                                                    const SArray<size_t> & segmNumbers2,
                                                    const size_t           defSegmNumb,
                                                    const double           gapValue );


//------------------------------------------------------------------------------
/** \brief \ru Заполнить массив сгибов.
           \en Fill the array of MbSMBendNames. \~
  \details \ru Заполнить массив сгибов.
           \en Fill the array of MbSMBendNames. \~
  \param[in] contour1 - \ru Первый контур.
                        \en First contour. \~
  \param[in] placement1 - \ru ЛСК первого контура.
                          \en Placement if the first contour. \~
  \param[in] breaks1 - \ru Массив параметров разбиения первого контура.
                       \en Parameters of a partition of the first contour. \~
  \param[in] contour2 - \ru Второй контур.
                        \en Second contour. \~
  \param[in] placement2 - \ru ЛСК второго контура.
                          \en Placement if the second contour. \~
  \param[in] breaks2 - \ru Массив параметров разбиения второго контура.
                       \en Parameters of a partition of the second contour. \~
  \param[in] nameMaker - \ru Массив имён граней линейчатой поверхности.
                         \en Names of the faces of the ruled surface. \~
  \param[out] bendNames - \ru Массив параметров сгибов.
                          \en Array of the bends parameters. \~
  \return \ru true - в случае успеха операции, false - в противном случае.
          \en True if the operation succeeded, otherwise false. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (bool) FillBendNamesArray( const MbContour &              contour1,
                                     const MbPlacement3D &          placement1,
                                     const SArray<double> &         breaks1,
                                     const MbContour &              contour2,
                                     const MbPlacement3D &          placement2,
                                     const SArray<double> &         breaks2,
                                     const MbSNameMaker &           nameMaker,
                                           RPArray<MbSMBendNames> & bendNames );


//------------------------------------------------------------------------------
/** \brief \ru Создать трехмерный контур по двумерному с учётом разбиения.
           \en To create 3D contour by 2D contour with splitting. \~
  \details \ru Разбивает присланный контур точками, заданными параметрами breaks,
               и по нему создаёт трёхмерный контур, лежащий на плоскости, заданной ЛСК placement.
           \en Splits the contour by points corresponding parameters breaks and makes 3D contour that lies on the placement. \~
  \param[in] placement - \ru ЛСК контура.
                         \en Placement of the contour. \~
  \param[in] contour -   \ru Контур.
                         \en The contour. \~
  \param[in] breaks -    \ru Массив параметров разбиения контура.
                         \en Parameters of a partition of the contour. \~
  \param[in,out] names - \ru Массив имён сегментов контура.
                         \en Names of the segments of the contour. \~
  \return \ru Разбитый с помощью массива breaks на сегменты трёхмерный контур.
          \en Splitted with array breaks 3D contour. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (MbContour3D *) MakeContour ( const MbPlacement3D &  placement,
                                        const MbContour &      contour,
                                        const SArray<double> & breaks,
                                              MbSNameMaker &   names );


//------------------------------------------------------------------------------
/** \brief \ru Создать контур ребра жесткости по двум сторонам.
           \en Create the contour of rib by side distances. \~
  \details \ru Создать двумерный контур ребра жесткости и его ЛСК по двум сторонам.\n
               Начало координат ЛСК лежит на прямой пересечения плоских внутренних сторон сгиба.
           \en Create the 2D contour of rib and placement by side distances. \n
               The placement origin is placed on intersection of plane internal sides of bend.\~
  \param[in] bendEdge - \ru Ребро на внутренней грани сгиба.
                        \en The edge on internal bend face. \~
  \param[in] bendAngle - \ru Угол сгиба листового тела. 
                         \en The bend angle.  \~
  \param[in] l1 - \ru Длина отступа вдоль первой стороны угла профиля.
                  \en The length along first side of rib section. \~
  \param[in] l2 - \ru Длина отступа вдоль второй стороны угла профиля.
                  \en The length along second side of rib section. \~
  \param[in] bRatio - \ru Относительная глубина прогиба контура в диапазоне от 0 до 1 (0 - нет прогиба, 1 - максимальный прогиб).
                      \en Relative value of contour bending defined in the range from 0 to 1 (0 - no bend, 1 - maximum bend). \~
  \param[in] rad - \ru Радиус скругления при прогибе профиля.
                   \en The contour fillet radius. \~
  \param[in] dir - \ru Направление выбора первой стороны угла профиля.
                   \en Direction of first side of rib section. \~
  \param[in] t - \ru Параметр точки на ребре сгиба.
                 \en Parameter on internal bend edge. \~
  \param[out] placement - \ru ЛСК контура.
                          \en Placement of the contour. \~
  \param[out] contour - \ru Контур листового ребра усиления.
                        \en The contour of sheet rib solid. \~
  \param[out] bMax - \ru Расстояние по биссектрисе угла сгиба от контура без прогиба до листового тела.
                     \en The distance along bisectrix of bend angle between unbended contour and sheet solid. \~
  \return \ru True - в случае успеха операции, false - в противном случае.
          \en True if the operation succeeded, otherwise false. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC( bool ) MakeSheetRiContourByTwoSides( const MbCurveEdge &   bendEdge,
                                                const double          bendAngle,
                                                const double          l1,
                                                const double          l2,
                                                const double          bRatio,
                                                const double          rad,
                                                const bool            dir,
                                                const double          t,
                                                      MbPlacement3D & placement,
                                                      MbContour &     contour,
                                                      double &        bMax );


//------------------------------------------------------------------------------
/** \brief \ru Создать контур ребра жесткости по стороне и углу.
           \en Create the contour of rib by distance and slope angle. \~
  \details \ru Создать двумерный контур ребра жесткости и его ЛСК по стороне и углу наклона.\n
               Начало координат ЛСК лежит на прямой пересечения плоских внутренних сторон сгиба. \~
           \en Create the 2D contour of rib and placement by distance and slope angle. \n
               The placement origin is placed on intersection of plane internal sides of bend.\~
  \param[in] bendEdge - \ru Ребро на внутренней грани сгиба.
                        \en The edge on internal bend face. \~
  \param[in] bendAngle - \ru Угол сгиба листового тела.
                         \en The bend angle.  \~
  \param[in] l1 - \ru Длина отступа вдоль первой стороны угла профиля.
                  \en The length along first side of rib section. \~
  \param[in] a  - \ru Угол наклона профиля.
                  \en The contour slope angle. \~
  \param[in] bRatio - \ru Относительная глубина прогиба контура в диапазоне от 0 до 1 (0 - нет прогиба, 1 - максимальный прогиб).
                      \en Relative value of contour bending defined in the range from 0 to 1 (0 - no bend, 1 - maximum bend). \~
  \param[in] rad - \ru Радиус скругления при прогибе профиля.
                   \en The contour fillet radius. \~
  \param[in] dir - \ru Направление выбора первой стороны угла профиля.
                   \en Direction of first side of rib section. \~
  \param[in] t - \ru Параметр точки на ребре сгиба.
                 \en Parameter on internal bend edge. \~
  \param[out] placement - \ru ЛСК контура.
                          \en Placement of the contour. \~
  \param[out] contour - \ru Контур листового ребра усиления.
                        \en The contour of sheet rib solid. \~
  \param[out] bMax - \ru Расстояние по биссектрисе угла сгиба от контура без прогиба до листового тела.
                     \en The distance along bisectrix of bend angle between unbended contour and sheet solid. \~
  \return \ru True - в случае успеха операции, false - в противном случае.
          \en True if the operation succeeded, otherwise false. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC( bool ) MakeSheetRiContourBySideAndAngle( const MbCurveEdge &   bendEdge,
                                                    const double          bendAngle,
                                                    const double          l1,
                                                    const double          a,
                                                    const double          bRatio,
                                                    const double          rad,
                                                    const bool            dir,
                                                    const double          t,
                                                          MbPlacement3D & placement,
                                                          MbContour &     contour,
                                                          double &        bMax );


//------------------------------------------------------------------------------
/** \brief \ru Создать контур ребра жесткости по глубине и углу.
           \en Create the contour of rib by depth and slope angle. \~
  \details \ru Создать двумерный контур ребра жесткости и его ЛСК по глубине и углу наклона.\n
               Начало координат ЛСК лежит на прямой пересечения плоских внутренних сторон сгиба. \~
           \en Create the 2D contour of rib and placement by depth and slope angle. \n
               The placement origin is placed on intersection of plane internal sides of bend.\~
  \param[in] bendEdge - \ru Ребро на внутренней грани сгиба.
                        \en The edge on internal bend face. \~
  \param[in] bendAngle - \ru Угол сгиба листового тела.
                         \en The bend angle.  \~
  \param[in] h - \ru Глубина профиля ребра.
                 \en The depth of rib section. \~
  \param[in] a  - \ru Угол наклона профиля.
                  \en The contour slope angle. \~
  \param[in] bRatio - \ru Относительная глубина прогиба контура в диапазоне от 0 до 1 (0 - нет прогиба, 1 - максимальный прогиб).
                      \en Relative value of contour bending defined in the range from 0 to 1 (0 - no bend, 1 - maximum bend). \~
  \param[in] rad - \ru Радиус скругления при прогибе профиля.
                   \en The contour fillet radius. \~
  \param[in] dir - \ru Направление выбора первой стороны угла профиля.
                   \en Direction of first side of rib section. \~
  \param[in] t - \ru Параметр точки на ребре сгиба.
                 \en Parameter on internal bend edge. \~
  \param[out] placement - \ru ЛСК контура.
                          \en Placement of the contour. \~
  \param[out] contour - \ru Контур листового ребра усиления.
                        \en The contour of sheet rib solid. \~
  \param[out] bMax - \ru Расстояние по биссектрисе угла сгиба от контура без прогиба до листового тела.
                     \en The distance along bisectrix of bend angle between unbended contour and sheet solid. \~
  \return \ru True - в случае успеха операции, false - в противном случае.
          \en True if the operation succeeded, otherwise false. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC( bool ) MakeSheetRiContourByDepthAndAngle( const MbCurveEdge &   bendEdge,
                                                     const double          bendAngle,
                                                     const double          h,
                                                     const double          a,
                                                     const double          bRatio,
                                                     const double          rad,
                                                     const bool            dir,
                                                     const double          t,
                                                           MbPlacement3D & placement,
                                                           MbContour &     contour,
                                                           double &        bMax );


//------------------------------------------------------------------------------
/** \brief \ru Расчётчик расположения хот-точек для ребра жесткости листового тела.
           \en Calculator of hot point location for the sheet rib. \~
  \details \ru Расчётчик расположения хот-точек для ребра жесткости листового тела.\n
               Контур должен быть рассчитан с помощью одной из функций MakeSheetRiContourByTwoSides, \n
               MakeSheetRiContourBySideAndAngle или MakeSheetRiContourByDepthAndAngle.
           \en Calculator of hot point location for the metal sheet rib. \n
               The contour should be generated by functions MakeSheetRiContourByTwoSides, \n
               MakeSheetRiContourBySideAndAngle or MakeSheetRiContourByDepthAndAngle. \n \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
class MATH_CLASS MbSheetRibHotPointCalculator {
  MbPlaneCurve   planeCurve;        ///< \ru Образующий контур ребра жесткости. \en The generating contour of a sheet rib.
  SheetRibValues parameters;        ///< \ru Параметры листового ребра жёсткости. \en Parameters of a sheet rib.
  MbPlacement3D  secPlace;          ///< \ru ЛСК плоскости, проходящей через ось сгиба перпендикулярно контуру. \en Placement of the plane passing throught the bend axis orthogonally to the contour.
  MbPlacement3D  rightFlankPlace;   ///< \ru ЛСК боковой плоской грани ребра жёсткости. \en Placement of the flank plane face of a sheet rib.
  MbCartPoint3D  begPoint;          ///< \ru Начальная точка контура ребра жесткости. \en The begin point of the generating contour. 
  MbCartPoint3D  endPoint;          ///< \ru Концевая точка контура ребра жесткости. \en The end point of the generating contour. 
  MbCartPoint3D  rightCornerPoint;  ///< \ru Правая вершина ребра жесткости. \en The right vertex of rib. 
  double         sheetThickness;    ///< \ru Толщина листа. \en The sheet thickness. 
  double         depth;             ///< \ru Глубина профиля ребра. \en The depth of rib section.
  bool           isContourStraight; ///< \ru Прямолинейная форма контура . \en The straight contour form.
  bool           isContourBent;     ///< \ru Прогнутая форма контура . \en The bent contour form.
  bool           isContourValid;    ///< \ru Корректность контура . \en The contour validness.

public:
  /// \ru Конструктор. \en Constructor. 
  MbSheetRibHotPointCalculator( const MbPlacement3D & place, const MbContour & contour, const SheetRibValues & params, double thickness, bool first = true );
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbSheetRibHotPointCalculator( const MbSheetRibHotPointCalculator &init );
  /// \ru Оператор присваивания. \en An assignment operator. 
  MbSheetRibHotPointCalculator & operator = ( const MbSheetRibHotPointCalculator &init );
  /// \ru Рассчитать положение "хот"-точки : длина отступа вдоль первой стороны угла профиля (L1). \en Calculate the hot point location : the length along first side of rib section. 
  bool CalcContourL1HotPoint( MbCartPoint3D & point, MbVector3D & dir ) const;
  /// \ru Рассчитать положение "хот"-точки : длина отступа вдоль второй стороны угла профиля (L2). \en Calculate the hot point location : the length along second side of rib section. 
  bool CalcContourL2HotPoint( MbCartPoint3D & point, MbVector3D & dir ) const;
  /// \ru Рассчитать положение "хот"-точки : угол наклона профиля в способе «По стороне и углу» (a). \en Calculate the hot point location: the contour slope angle. 
  bool CalcContourAngleHotPoint( MbCartPoint3D & point, MbVector3D & dir ) const;
  /// \ru Рассчитать положение "хот"-точки : глубина профиля (H) в способе «По глубине и углу». \en Calculate the hot point location : the depth of rib section.
  bool CalcContourDepthHotPoint( MbCartPoint3D & point, MbVector3D & dir ) const;
  /// \ru Рассчитать положение "хот"-точки : радиус скругления ребра (дна формы) в сечении (R). \en Calculate the hot point location : fillet radius of convex part of rib.
  bool CalcRibRadHotPoint( MbCartPoint3D & point, MbVector3D & dir ) const;
  /// \ru Рассчитать положение "хот"-точки : радиус скругления основания. \en Calculate the hot point location : fillet radius of base part of rib.
  bool CalcRibRadBaseHotPoint( MbCartPoint3D & point, MbVector3D & dir ) const;
  /// \ru Рассчитать положение "хот"-точки : ширина ребра в сечении. \en Calculate the hot point location : width of base of rib.
  bool CalcRibWidthHotPoint( MbCartPoint3D & point, MbVector3D & dir ) const;
  /// \ru Рассчитать положение "хот"-точки : угол наклона боковой грани. \en Calculate the hot point location : the slope angle of flank face of rib.
  bool CalcRibSlopeHotPoint( MbCartPoint3D & point, MbAxis3D & rotationAxis ) const;
  /// \ru Рассчитать положение "хот"-точки : глубина прогиба профиля. \en Calculate the hot point location : the contour bending depth.
  bool CalcContourBendDepthHotPoint( MbCartPoint3D & point, MbVector3D & dir ) const;
  /// \ru Рассчитать положение "хот"-точки : радиус скругления при прогибе профиля. \en Calculate the hot point location : the contour fillet radius.
  bool CalcContourRadHotPoint( MbCartPoint3D & point, MbVector3D & dir ) const;

private:
  bool CalcFilletHotPoint( const MbCartPoint & p0,
                           const MbCartPoint & p1,
                           const MbCartPoint & p2,
                                 double        rad,
                                 MbCartPoint & hotPoint,
                                 MbVector &    hotDir ) const;
};


//------------------------------------------------------------------------------
/** \brief \ru Создание элементов ребра жёсткости листового тела.
           \en Create rib parts of a sheet solid. \~
  \details \ru Создание элементов ребра жёсткости листового тела. \n
               По заданному контуру функция строит ребро жёсткости, а затем вычленяет составляющие его элементы, заделывая места разрыва. \n
           \en Create rib parts of a sheet solid. \n
               The function creates a rib from a given contour and then extructs its elements from the body, healing rip borders with patches. \n \~
  \param[in]  solid          - \ru Исходное листовое тело.
                               \en The source sheet solid. \~
  \param[in]  sameShell      - \ru Режим копирования входного листового тела.
                               \en Whether to copy the input sheet solid. \~
  \param[in]  params         - \ru Параметры построения ребра жесткости для листового тела.
                               \en Parameters for creating a sheet solid with rib. \~
  \param[out] partToAdd      - \ru Добавляемый элемент ребра.
                               \en The adding rib element. \~
  \param[out] partToSubtract - \ru Вычитаемый элемент ребра.
                               \en The subtracting rib element. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (MbResultType) SheetRibParts( const c3d::SolidSPtr &        solid,
                                              MbeCopyMode             sameShell,
                                        const MbSheetRibSolidParams & params,
                                              c3d::SolidSPtr &        partToAdd,
                                              c3d::SolidSPtr &        partToSubtract );


//------------------------------------------------------------------------------
/** \brief \ru Создание тела с листовым ребром жесткости.
           \en Create a sheet solid with rib. \~
  \details \ru Создать тело с листовым ребром жёсткости. \n
               По заданному контуру функция строит ребро жёсткости и объединяет его с исходным листовым телом.
               Сегмент контура с указанным номером устанавливает вектор уклона. \n
           \en Create a sheet solid with a sheet rib. \n
               The function creates a rib from a given contour and unites it with the source sheet solid.
               The segment of the contour with the given number determines the slope vector. \n \~
  \param[in]  solid     - \ru Исходное листовое тело.
                          \en The source sheet solid. \~
  \param[in]  sameShell - \ru Режим копирования входного листового тела.
                          \en Whether to copy the input sheet solid. \~
  \param[in]  params    - \ru Параметры построения ребра жесткости для листового тела.
                          \en Parameters for creating a sheet solid with rib. \~
  \param[out] result    - \ru Построенное листовое тело.
                          \en The resultant sheet solid. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (MbResultType) SheetRibSolid( const c3d::SolidSPtr &        solid,
                                              MbeCopyMode             sameShell,
                                        const MbSheetRibSolidParams & params,
                                              c3d::SolidSPtr &        result );


//------------------------------------------------------------------------------
/** \brief \ru Разрезать тело секущими плоскостями и согнуть получившиеся сегменты согласно заданным параметрам.
           \en Cut the solid with the cutting planes and bend the resulting parts according to given parameters. \~
  \details \ru cutPlane - это граница-начало цепочки сгибов. На ней с помощью bends[0].origin и bends[0].vector определена ось цилиндрического сгиба, \n
               причём направление оси выбрано так, что при повороте на 90 градусов (именно повороте в положительном направлении) она будет направлена в сторону сгибаемого тела. \n
               bends[0].wideness - ширина сгиба в разогнутом положении, а bends[0].neutralRadius - это расстояние от оси сгиба до слоя, который при сгибе не будет деформироваться. \n
               Граница следующего сгиба - это cutPlane смещённая на bends[0].wideness вдоль нормали, то есть противоположная граница предыдущего сгиба. \n
               На этой новой плоскости аналогичным образом задаются параметры MbAnyBend для второго сгиба, и так далее для всей цепочки сгибов. \n
               Если между идущими друг за другом сгибами нужно указать несгибаемый (плоский) участок, то надо в соответствующем ему bend[i] занулить координаты origin, vector и значение neutralRadius, \n
               а в поле wideness положить ширину несгибаемого участка.\n
           \en The cutPlane is the the beginning of a chain of bends borders. The axis of the cylindrical bend is defined using bends[0].origin and bends[0].vector on it. \n
               The direction of the axis is chosen so that when turning it 90 degrees (turning in the positive direction) it will be directed towards the bendable body. \n
               bends[0].width is the width of the bend in the unfolded position, and bends[0].neutralRadius is the distance from the bend axis to the layer, which will not deform when folded. \n
               The border of the next fold is a cutPlane offset by bends[0].width along the normal, that is, the opposite border of the previous bend. \n
               On this new plane, the MbAnyBend parameters for the second bend are set in the same way, and so on for the entire chain of bends. \n
               If it is necessary to specify an unbendable (flat) section between the bends following each other, then it is necessary to zero the coordinates of origin, vector and the value of neutralRadius, \n
               and in the width field, put the width of the flat section. \~
  \param[in] solid - \ru Исходное тело.
                     \en The source solid. \~
  \param[in] sameShell - \ru Флаг удаления оболочки исходного тела.
                         \en Whether to delete the shell of the source solid. \~
  \param[in] cutPlane - \ru Первая секущая плоскость.
                        \en The first cutting plane. \~
  \param[in] bends - \ru Множество сгибов, состоящих из секущей плоскости и параметров сгиба.
                     \en An array of bends which consist of cutting plane and bend parameters. \~
  \param[in] nameMaker - \ru Именователь.
                         \en An object for naming the new objects. \~
  \param[out] result - \ru Результирующее тело.
                       \en The resultant solid. \~
  \result \ru - Код результата операции.
          \en - The operation result code. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (MbResultType) BendAnySolid(       MbSolid &           solid,
                                       const MbeCopyMode         sameShell,
                                       const MbPlane &           cutPlane,
                                       const SArray<MbAnyBend> & bends,
                                       const MbSNameMaker &      nameMaker,
                                             MbSolid *&          result );


//------------------------------------------------------------------------------
/** \brief \ru Упростить развёртку листового тела.
           \en Simplify flattened sheet solid. \~
  \details \ru Упростить развёртку листового тела. \n
           \en Simplify flattened sheet solid. \n \~
  \param[in] solid - \ru Исходное тело.
                     \en The source solid. \~
  \param[in] sameShell - \ru Флаг удаления оболочки исходного тела.
                         \en Whether to delete the shell of the source solid. \~
  \param[in] params - \ru Параметры операции.
                      \en The operation parameters. \~
  \param[in] nameMaker - \ru Именователь.
                         \en An object for naming the new objects. \~
  \param[out] result - \ru Результирующее тело.
                       \en The resultant solid. \~
  \result \ru - Код результата операции.
          \en - The operation result code. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (MbResultType) SimplifyFlatPattern(       MbSolid &                     solid,
                                              const MbeCopyMode                   sameShell,
                                              const MbSimplifyFlatPatternValues & params,
                                              const MbSNameMaker &                nameMaker,
                                                    MbSolid *&                    result );


//------------------------------------------------------------------------------
/** \brief \ru Удалить из тела результат операции с именем removeName .
           \en Remove the result operation with name "removeName". \~
  \details \ru Операция удаляет грани с главным именем removeName и потом заделывает образовавшиеся дыры.\n
           \en The operation deletes the faces that have main name equal to "mainName" and then closes up the holes that remain after the first stage of the operation.\n \~
  \param[in] solid - \ru Исходное тело.
                     \en The source solid. \~
  \param[in] sameShell - \ru Флаг удаления оболочки исходного тела.
                         \en Whether to delete the shell of the source solid. \~
  \param[in] params - \ru Параметры операции.
                      \en Operation parameters. \~
  \param[out] result - \ru Результирующее тело.
                       \en The resultant solid. \~
  \result \ru - Код результата операции.
          \en - The operation result code. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (MbResultType) RemoveOperationResult(       MbSolid &                       solid,
                                                const MbeCopyMode                     sameShell,
                                                const MbRemoveOperationResultParams & params,
                                                      SPtr<MbSolid> &                 result );


//------------------------------------------------------------------------------
/** \brief \ru Преобразовать тело в листовой металл.
           \en Construct sheet metal solid based on an arbitrary solid. \~
  \details \ru Операция строит листовое тело на базе произвольного тела.\n
           \en The operation builds a sheet metal solid based on an arbitrary solid.\n \~
  \param[in] solid - \ru Исходное тело.
                     \en The source solid. \~
  \param[in] sameShell - \ru Флаг удаления оболочки исходного тела.
                         \en Whether to delete the shell of the source solid. \~
  \param[in,out] parameters - \ru Параметры операции.
                              \en Operation parameters. \~
  \param[out] result - \ru Результирующее тело.
                       \en The resultant solid. \~
  \result \ru - Код результата операции.
          \en - The operation result code. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC( MbResultType ) ConvertSolidToSheetMetal(       c3d::SolidSPtr &            solid,
                                                    const MbeCopyMode                 sameShell,
                                                          MbSolidToSheetMetalParams & parameters,
                                                          c3d::SolidSPtr &            result );
#endif // __ACTION_SHEET_H
