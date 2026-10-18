////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Объекты, используемые при импорте и экспорте аннотации и размеров.
         \en Objects used for import and export of annotation and dimensions \~
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CONV_ANNOTATION_ITEM_H
#define __CONV_ANNOTATION_ITEM_H

#include <reference_item.h>
#include <mb_placement.h>
#include <mb_placement3d.h>
#include <mb_pmi.h>
#include <conv_pmi_depricated_convert.h>

#include <vector>
#include <set>

class  MbLineSegment3D;
class  MbArc3D;
class  MbItem;
class  MbPlaneItem;
class  MbRegDuplicate;

//------------------------------------------------------------------------------
/** \brief \ru Тип элемента аннотации.
           \en Type of annotation element. \~
*/
// ---
enum Mae_AnnotationType {
  nt_AnnotationItem,    ///< \ru Аннотация без объектов привязки. \en Annotation without binding objects. 
  nt_Dimension,         ///< \ru Размер. \en Dimension 
  nt_LinearDimension,   ///< \ru Линейный размер. \en Linear dimension. 
  nt_DiameterDimension, ///< \ru Диаметральный размер. \en Diameter dimension. 
  nt_RadialDimension,   ///< \ru Радиальный размер. \en Radial dimension. 
  nt_AngularDimension,  ///< \ru Угловой размер. \en Angular dimension. 
  nt_Callout,           ///< \ru Выноска. \en Callout.
  nt_Marking,           ///< \ru Обозначение. \en Marking.
  nt_Datum,             ///< \ru База. \en Datum.
  nt_Note,              ///< \ru Примечание. \en Note.
  nt_Centreline,        ///< \ru Осевая линия. \en Centreline.
  nt_FeatureControlFrame,  ///< \ru Рамка управления характеристиками. \en Feature Control Frame.
  nt_ReferencePoint,    ///< \ru Точка отсчета. \en Reference Point.
  nt_SurfaceRoughness,  ///< \ru Шероховатость поверхности. \en Surface roughness.
  nt_ShapeTolerance     ///< \ru Допуск формы.
};


//------------------------------------------------------------------------------
/** \brief \ru Тип текстового объекта.
           \en Type of a text object. \~
*/
// ---
enum MaeTextType {
  xt_CompositeText,     ///< \ru Набор текстовых блоков. \en Set of text blocks. 
  xt_TextLiteral,       ///< \ru Текст с указанием ЛСК, шрифта, выравнивания. \en Text with specification of LCS, font, alignment. 
  xt_TextLiteralExtent, ///< \ru Текст с указанием ЛСК, шрифта, выравнивания, геометрического размера. \en Text with specification of LCS, font, alignment, geometric dimension. 
  xt_SpecificSymbol     ///< \ru Спецсимвол. \en Specific symbol. 
};


//------------------------------------------------------------------------------
/** \brief \ru Тэг, определяющий назначение текстового блока.
           \en Purpose tag of a text object. \~
*/
// ---
enum MaeTextFormatTag {
  xft_Enumeration,  ///< \ru Перечисление. \en Enumeration. 
  xft_Paragraph,    ///< \ru Параграф. \en Paragraph.
  // Тэги в следующей группе являются взаимосиключающими. Tags of the next group are mutually exclusive.
  xft_Ground,       ///< \ru Положение текста на базовом уровне. \en Ground level text position.
  xft_Upper,        ///< \ru Верхний индекс или числитель. \en Upper index or numerator. 
  xft_Lower,        ///< \ru Нижний индекс или знаменатель. \en Lower index or denominator. 
  // Следующая группа тэгов уточняет смысл тэгов предыдущей группы. Next group of tags gives the exact meaning to the tags frem the previosu group.
  xft_Fraction,     ///< \ru Дробь. \en Fraction. 
  xft_Index,        ///< \ru Наличие индекс. \en Indexed item.
  xft_OverUnder,    ///< \ru Наличие надстрочного и подстрочного текста. \en Overline and underline text present.

  xft_Undefined,    ///< \ru Неопределённое значение тэга, не назначается. \en Undefined can be never assigned to items.
};


//------------------------------------------------------------------------------
/** \brief \ru Направление текста.
           \en Text direction. \~
*/
// ---
enum eTextPath {
  txp_Left, ///< \ru Налево. \en To the left. 
  txp_Right,///< \ru Направо. \en To the right. 
  txp_Up,   ///< \ru Вверх. \en Upward. 
  txp_Down  ///< \ru Вниз. \en Downward. 
};


//------------------------------------------------------------------------------
/** \brief \ru Спецсимволы.
           \en Special symbols. \~
*/
enum MbeDefinedDimensionSymbol {
  dds_ArcLength,          ///< \ru Длина дуги. \en The arc length. 
  dds_ConicalTaper,       ///< \ru Конусность. \en Conicity. 
  dds_Counterbore,        ///< \ru Зенковка. \en Counterbore. 
  dds_Countersink,        ///< \ru Циковка. \en Countersink. 
  dds_Depth,              ///< \ru Глубина. \en Depth. 
  dds_Diameter,           ///< \ru Диаметр. \en Diameter. 
  dds_PlusMinus,          ///< \ru Одинаковая двусторонняя погрешность. \en Equal double-sided tolerance. 
  dds_Radius,             ///< \ru Радиус. \en Radius. 
  dds_Slope,              ///< \ru Склон. \en Slope. 
  dds_SphericalDiameter,  ///< \ru Сферический диаметр. \en Spherical diameter. 
  dds_SphericalRadius,    ///< \ru Сферический радиус. \en Spherical radius. 
  dds_Square,             ///< \ru Квадрат. \en Square. 
  dds_MetricThread,       ///< \ru Метрическая резьба (при экспорте в STEP преобразуется в букву M). \en Metric thread ( in STEP it corresponds M letter ).
  
  dds_SurfaceCondition,     ///< \ru Шереховатость поверхности в нотации STEP (ISO 10303). \en Surface condition in STEP (ISO 10303) codes.
  dds_SurfaceCondition_010,
  dds_SurfaceCondition_020,
  dds_SurfaceCondition_030,
  dds_SurfaceCondition_040,
  dds_SurfaceCondition_050,
  dds_SurfaceCondition_060,
  dds_SurfaceCondition_070,

  dds_SurfaceCondition_001,
  dds_SurfaceCondition_011,
  dds_SurfaceCondition_021,
  dds_SurfaceCondition_031,
  dds_SurfaceCondition_041,
  dds_SurfaceCondition_051,
  dds_SurfaceCondition_061,
  dds_SurfaceCondition_071,

  dds_SurfaceCondition_100,
  dds_SurfaceCondition_110,
  dds_SurfaceCondition_120,
  dds_SurfaceCondition_130,
  dds_SurfaceCondition_140,
  dds_SurfaceCondition_150,
  dds_SurfaceCondition_160,
  dds_SurfaceCondition_170,

  dds_SurfaceCondition_101,
  dds_SurfaceCondition_111,
  dds_SurfaceCondition_121,
  dds_SurfaceCondition_131,
  dds_SurfaceCondition_141,
  dds_SurfaceCondition_151,
  dds_SurfaceCondition_161,
  dds_SurfaceCondition_171,
  
  dds_SurfaceCondition_200,
  dds_SurfaceCondition_210,
  dds_SurfaceCondition_220,
  dds_SurfaceCondition_230,
  dds_SurfaceCondition_240,
  dds_SurfaceCondition_250,
  dds_SurfaceCondition_260,
  dds_SurfaceCondition_270,
   
  dds_SurfaceCondition_201,
  dds_SurfaceCondition_211,
  dds_SurfaceCondition_221,
  dds_SurfaceCondition_231,
  dds_SurfaceCondition_241,
  dds_SurfaceCondition_251,
  dds_SurfaceCondition_261,
  dds_SurfaceCondition_271,

  dds_Angularity,               ///< \ru Допуск наклона. \en Angularity.
  dds_CircularRunout,           ///< \ru Допуск биения. \en Circular runout.
  dds_Circularity,              ///< \ru Допуск круглости. \en Circularity.
  dds_Concentricity,            ///< \ru Допуск соосности. \en Concentricity.
  dds_Cylindricity,             ///< \ru Допуск цилиндричности. \en Cylindricity.
  dds_DiameterTol,              ///< \ru Допуск диаметра. \en Diameter.
  dds_Flatness,                 ///< \ru Допуск плоскостности. \en Flatness.
  dds_LeastMaterialCondition,   ///< \ru Требование минимума материала. \en Least material condition.
  dds_MaximumMaterialCondition, ///< \ru Требование максимума материала. \en Maximum material condition.
  dds_Parallelism,              ///< \ru Допуск параллельности. \en Parallelism.
  dds_Perpendicularity,         ///< \ru Допуск перпендикулярности. \en Perpendicularity.
  dds_Position,                 ///< \ru Позиционный допуск. \en Position.
  dds_LineProfile,              ///< \ru Допуск формы заданного профиля. \en Line profile.
  dds_SurfaceProfile,           ///< \ru Допуск формы заданной поверхности. \en Surface profile.
  dds_ProjectedToleranceZone,   ///< \ru Выступающее поле допуска. \en ProejectedToleranceZone.
  dds_RegardlessOfFeatureSize,  ///< \ru  . \en .
  dds_Straightness,             ///< \ru Допуск прямолинейности. \en Straightness.
  dds_Symmetry,                 ///< \ru Допуск симметричности. \en .Symmetry
  dds_TotlaRunout,              ///< \ru Допуск полного радиального (либо торцевого) биения. \en Full radial (or face) runout tolerance.
  dds_StringDefinedSymbol       ///< \ru Для интерпретации следует использовать строковое поле. \en String field should be used to identify the symbol.
};


//------------------------------------------------------------------------------
/** \brief \ru Тип законцовки.
           \en Type of tip. \~
*/
enum MbeDefinedTerminatorSymbol {
  dts_BlankedArrow,     ///< \ru Незакрашенная стрелка.   \en Blank arrow. 
  dts_BlankedBox,       ///< \ru Незакрашенный квадрат.   \en Blank square. 
  dts_BlankedDot,       ///< \ru Незакрашенная точка.     \en Blank point. 
  dts_DimensionOrigin,  ///< \ru Базовый объект.          \en Base object. 
  dts_FilledArrow,      ///< \ru Закрашенная стрелка.     \en Filled arrow. 
  dts_FilledBox,        ///< \ru Закрашенный квадрат.     \en Filled square. 
  dts_FilledDot,        ///< \ru Закрашенная точка.       \en Filled point. 
  dts_IntegralSymbol,   ///< \ru Знак интеграла.          \en Integral symbol. 
  dts_OpenArrow,        ///< \ru Открытая стрелка.        \en Open arrow. 
  dts_Slash,            ///< \ru Косая черта.             \en Slash. 
  dts_UnfilledArrow     ///< \ru Стрелка без заполнения.  \en Unfilled arrow. 
};


//------------------------------------------------------------------------------
/** \brief \ru Тип кривой с терминаторами.
\en Type of curve with terminators. \~
*/
enum MbeDecoratedCurveRole {
  dcr_ProjectionCurve,   ///< \ru Проекционная кривая размера. \en Projection curve of dimension. 
  dcr_DimensionCurve,    ///< \ru Размерная кривая. \en Dimension curve. 
  dcr_LeaderCurve,       ///< \ru Линия выноски. \en Leader curve. 
};


//------------------------------------------------------------------------------
/** \brief \ru Текстовый объект.
           \en Text object. \~
*/
// ---
class CONV_CLASS MaTextItem : public MbRefItem {
protected:
  bool visibility;                        // \ru Признак видимости. \en Visibility.
  std::set<MaeTextFormatTag> purposeTags; // \ru Тэги форматирования. \en Gormat tags.
public:

  MaTextItem(); ///< \ru Конструктор по умолчанию. \en Default constructor. 

  void SetVisibility( bool v );  ///< \ru Задать видимость; \en Set visibility.
  bool IsVisible() const; ///< \ru Получить видимость; \en Get visibility.

  bool IsTag( MaeTextFormatTag tag ) const;  ///< \ru Установлен ли тэг. \en Is a tag set.
  bool GetTagIfUnique( MaeTextFormatTag& tag ) const;  ///< \ru получить тэг, если он единственный. \en Get the tag provided it id qnique.
  void SetTag( MaeTextFormatTag tag );       ///< \ru Установить тэг. \en Set a tag.
  void ResetTag( MaeTextFormatTag tag );     ///< \ru Сбросить тэг. \en reset a tag.
  bool TagUniqueOrUndefined() const;         ///< \ru Назначено ли менее 2 тэгов. \en If less than two tags assinged.
  bool NoTag() const;                        ///< \ru Отсутствуют ли тэги. \en If threre are no tags.
  
  virtual MaeTextType IsA() const = 0;
  virtual SPtr<MaTextItem> Duplicate() const = 0;
  virtual ~MaTextItem(); ///< \ru Деструктор. \en Destructor. 

  OBVIOUS_PRIVATE_COPY( MaTextItem )
};


//------------------------------------------------------------------------------
/** \brief \ru Набор текстовых блоков.
           \en Set of text blocks. \~
*/
// ---
class CONV_CLASS MaCompositeText : public MaTextItem {
  std::vector< SPtr<MaTextItem> > items; ///< \ru Текстовый блок. \en The text block. 

public:

  MaCompositeText(); ///< \ru Конструктор по умолчанию. \en Default constructor. 

  std::vector< SPtr<MaTextItem> > GetItems() const; ///< \ru Получить элементы. \en Get elements. 
  void SetItems( const std::vector< SPtr<MaTextItem> >& it ); ///< \ru Задать элементы. \en Set elements. 
  void AddItem( MaTextItem* item ); ///< \ru Добавить элемент \en Add an element. 
  size_t ItemsSize() const; ///< \ru Получить число элементов \en Get count of elements. 
  MaTextItem* GetItem( size_t idx ); ///< \ru Получить элемент. \en Get element. 
  const MaTextItem* GetItem( size_t idx ) const; ///< \ru Получить элемент. \en Get element. 

  virtual MaeTextType IsA() const; ///< \ru Выдать тип элемента. \en Get element type. 
  virtual SPtr<MaTextItem> Duplicate() const;

  /** \brief \ru Вставить объект перед всеми вхождениями указанного.
           \en Insert an object before all instances of the specified one. \~
  */
  void    InsertBefore( const SPtr<MaTextItem>& itemToInsert, const SPtr<const MaTextItem>& beforeThis );

  OBVIOUS_PRIVATE_COPY( MaCompositeText )
};


//------------------------------------------------------------------------------
/** \brief \ru Текст с указанием ЛСК, шрифта, выравнивания.
           \en Text with specification of LCS, font, align. \~
*/
// ---
class CONV_CLASS MaTextLiteral : public MaTextItem {
protected:
  c3d::string_t text;    ///< \ru Текст. \en A text. 
  MbPlacement location;  ///< \ru Положение в аннотационной плоскости \en Position in annotation plane 
  std::string alignment; ///< \ru Выравнивание. \en Alignment. 
  eTextPath   path;      ///< \ru Направление текста. \en Text direction. 
  std::string font;      ///< \ru Шрифт текста. \en Text font. 
  bool isFontExternal;   ///< \ru Является ли шрифт нестандартным. \en Is font non-standard.

public:

  MaTextLiteral(); ///< \ru Конструктор по умолчанию. \en Default constructor. 

        MbPlacement & SetLocation(); ///< \ru Получить положение с возможностью модификации. \en Get position with possibility of modification. 
  const MbPlacement & GetLocation() const; ///< \ru Получить положение. \en Get position. 
        eTextPath & SetPath(); ///< \ru Получить направление с возможностью модификации. \en Get direction with possibility of modification. 
        eTextPath   GetPath() const; ///< \ru Получить направление. \en Get direction. 
        void        SetFontExternal( bool value ); ///< \ru Задать признак нестандартного шрифта. \en Set the flag of external font.
        bool        GetFontExternal() const;  ///< \ru Получить признак нестандартного шрифта. \en Get the flag of external font.

        void        SetC3DText( const c3d::string_t& ); ///< \ru Получить текст. \en Get text. 
        void        GetC3DText( c3d::string_t& ) const; ///< \ru Задать текст. \en Set text. 
        void        SetText( const std::string& ); ///< \ru Получить текст. \en Get text. 
        void        GetText( std::string& ) const; ///< \ru Задать текст. \en Set text. 
        void        SetAlignment( const std::string& ); ///< \ru Получить выравнивание. \en Get alignment. 
        void        GetAlignment( std::string& ) const; ///< \ru Задать выравнивание. \en Set alignment. 
        void        SetFont( const std::string& ); ///< \ru Получить шрифт. \en Get font. 
        void        GetFont( std::string& ) const; ///< \ru Задать шрифт. \en Set font. 

  virtual MaeTextType IsA() const;
  virtual SPtr<MaTextItem> Duplicate() const;

  OBVIOUS_PRIVATE_COPY( MaTextLiteral )
};


//------------------------------------------------------------------------------
/** \brief \ru Текст с указанием ЛСК, шрифта, выравнивания, размера.
           \en Text with specification of LCS, font, alignment, size. \~
*/
// ---
class CONV_CLASS MaTextLiteralExtent : public MaTextLiteral {
  double sizeX, sizeY; ///< \ru Размеры по x и у. \en Size by x and size by y.
public:

  MaTextLiteralExtent(); ///< \ru Конструктор по умолчанию. \en Default constructor. 

  double & SetSizeX(); ///< \ru Получить размер по x. \en Get size by x with possibility of modification. 
  double & SetSizeY(); ///< \ru Получить размер по y. \en Get size by y with possibility of modification. 
  double   GetSizeX() const; ///< \ru Получить размер по x. \en Get size by x. 
  double   GetSizeY() const; ///< \ru Получить размер по y. \en Get size by y. 

  virtual MaeTextType IsA() const;
  virtual SPtr<MaTextItem> Duplicate() const;

  OBVIOUS_PRIVATE_COPY( MaTextLiteralExtent )
};


//------------------------------------------------------------------------------
/** \brief \ru Спецсимвол.
           \en Specific symbol. \~
*/
// ---
class CONV_CLASS MaSpecificSymbol : public MaTextItem {  
  MbPlacement location;  ///< \ru Положение в аннотационной плоскости \en Position in annotation plane.
  double      sizeX; ///< \ru Размер по X. \en Size by x. 
  double      sizeY; ///< \ru Размер по Y. \en Size by Y. 
  MbeDefinedDimensionSymbol preDefinedSym; ///< \ru Код предопределённого символа. \en The predefined symbol code. 
  c3d::string_t symbolDefinition; ///< \ru Определение символа, выходящее за пределвы кодов. \en The definition of symbol beyound the pre-defined code. 
public:

  MaSpecificSymbol( MbeDefinedDimensionSymbol symbol, double szX, double szY );

  MaSpecificSymbol( const c3d::string_t definition, double szX, double szY );

  MbeDefinedDimensionSymbol GetSymbol() const; ///< \ru Получить код предопределённого символа. \en Get the predefined symbol code. 
  c3d::string_t GetSymbolDefintion() const; ///< \ru Получить определение символа. \en Get the definition of symbol. 
  bool IsSymbolDimension() const; ///< \ru Является ли символ размерным. \en Is symbol dimension. 
  bool IsSymbolSurfaceCondition() const; ///< \ru Является ли символ обозначением шероховатости. \en Is symbol surface condition. 
  bool IsSymbolShapeTolerance() const; ///< \ru Является ли символ допуском формы. \en Is symbol shape tolerance. 
  MbPlacement& SetLocation(); ///< \ru Получить положение с возможностью модификации. \en Get position with possibility of modification. 
  const MbPlacement& GetLocation() const; ///< \ru Получить положение. \en Get position. 
  double GetSizeX() const; ///< \ru Получить размер по x. \en Get size by x. 
  double GetSizeY() const; ///< \ru Получить размер по y. \en Get size by y. 
  void GetSize( double& x, double& y ) const; ///< \ru Получить размеры. \en Get sizes. 
  
  virtual MaeTextType IsA() const;
  virtual SPtr<MaTextItem> Duplicate() const;

  OBVIOUS_PRIVATE_COPY( MaSpecificSymbol )
};


//------------------------------------------------------------------------------
/** \brief \ru Описание законцовочного символа.
\en Description of the terminator symbol. \~
*/
struct MaTerminatorSymbol {
  MbeDefinedTerminatorSymbol type; ///< \ru Тип символа \en Symbol type 
  double parameter;                ///< \ru Значение параметра на размерной кривой. Если не указан, должен быть равен UNDEFINED_DBL. \en Parameter value on the dimensional curve. If not known, must be equal UNDEFINED_DBL.
  double sizeX;                    ///< \ru Размер по x. \en Size by x. 
  double sizeY;                    ///< \ru Размер по у. \en Size by y. 
  /// \ru Признак сонаправленности с касательной к кривой в точке размещения. В случае неопределённого значения параметра - признак направленности внутрь. 
  /// \en Flag of the same direction with the tangent to the curve at the location point. In case parameter id undefined it shows if the arrow's direction is inner. 
  bool sameDirection;              

  MbCartPoint3D location;           ///< \ru Положение в пространстве. \en Location in space.
};


//------------------------------------------------------------------------------
/** \brief \ru Кривая с терминаторами.
           \en Curve and terminators. \~
*/
class CONV_CLASS MaDecoratedCurve : public MbRefItem {
  SPtr<MbCurve3D>                   curve;
  std::vector< MaTerminatorSymbol > terminators;
  MbeDecoratedCurveRole             curveType;
public:
  MaDecoratedCurve( MbeDecoratedCurveRole crvType ); ///< \ru Конструктор. \en Constructor.
  MaDecoratedCurve( const MaDecoratedCurve& );       ///< \ru Конструктор копирования. \en Copy constructor.
  const MaDecoratedCurve& operator= ( const MaDecoratedCurve& ); ///< \ru Оператор присваивания. \en Assignment operator.

  SPtr<MbCurve3D> GetCurve() const; ///< \ru Получить кривую. \en Get curve.
  bool CurveEmpty() const; ///< \ru Пуста ли кривая. \en If curve is empty.
  void SetCurve( MbCurve3D* crv ); ///< \ru Задать кривую. \en Set curve.
  size_t TerminatorsCount() const; ///< \ru Получить число законцовок. \en Set number of terminators.
  bool TerminatorInfo( size_t terminatorIndex, MaTerminatorSymbol& term ) const; ///< \ru Получить законцовку с указанным индексом. \en Get terminator.
  void AddTerminator( const MaTerminatorSymbol& term ); ///< \ru Добавить законцовку. \en Add terminator.

  bool IsA( MbeDecoratedCurveRole ) const; ///< \ru Проверка типа кривой. \en Check curve type.
  MbeDecoratedCurveRole IsA() const; ///< \ru Проверка типа кривой. \en Check curve type.

  void DuplicateCurve( const MbMatrix3D& transform ); ///< \ru Заменить кривую на преобразованный по матрице дубликат. \en Replace curve by transformed replica.
};


//------------------------------------------------------------------------------
/** \brief \ru Объект аннотации.
           \en Annotation object. \~
*/
class CONV_CLASS MaAnnotationItem : public MbRefItem {
protected:
  MbPlacement3D                         location;           ///< \ru Локальная система координат (ЛСК), в плоскости XY которой расположены объекты аннотации. \en Local coordinate system (LCS) the annotation objects are located in XY plane of. 
  std::vector< const MbItem* >          annotationGeometry; ///< \ru Геометрические объекты аннотации. \en Geometric objects of annotation. 
  std::vector< SPtr<const MaTextItem> > annotationText;     ///< \ru Текстовые аннотационные объекты. \en Text annotation objects. 
  c3d::string_t                         name;               ///< \ru Имя. \en Name.
  bool                                  visible;            ///< \ru Видим ли объект. \en If object is vivible.
  // \ru Аналогичным образом реализовать и символьное представление \en Implement symbolic representation similarly. 
public:
  /// \ru Конструктор по плоскости аннотации. \en Constructor by annotation plane. 
  MaAnnotationItem( const MbPlacement3D& loc ); 
  /// \ru Деструктор. \en Destructor. 
  virtual ~MaAnnotationItem(); 

public:
  /// \ru Получить тип объекта. \en Get the object type. 
  virtual Mae_AnnotationType IsA() const; 
  /// \ru Получить групповой тип объекта. \en Get the group type of the object. 
  virtual Mae_AnnotationType Type() const; 

  /// \ru Пусто ли визуальное представление. \en Whether the visual representation is empty. 
  virtual bool VisualItemsEmpty() const; 

          /// \ru Отсутствуют ли геометрические элементы. \en Whether there are no geometric items.
          bool GeometryEmpty() const;

          /// \ru Отсутствуют ли текстовые элементы. \en Whether there are no text items.
          bool TextEmpty() const;

          /// \ru Добавить геометрический визуальный аннотационный элемент. \en Add the geometric visual annotation element of the kernel. 
          void AddGeometricAnnotationElement( const MbItem& );

          /// \ru Добавить собственные геометрические визуальные аннотационный элементы в контейнер. \en Add own geometric visual annotation elements to container. 
          void AddAnnotationGeometryTo( std::vector< SPtr<MbItem> >& addTo ) const;

          /// \ru Число текстовых элементов. \en Count of text items. 
          size_t TextItemsCount() const;

          /// \ru Получить текстовый элемент с указанным индексом. \en Get specified text item. 
          SPtr<MaTextItem> TextItem( size_t ) const;

          /// \ru Задать аннотационные объекты ядра. \en Set the annotation objects of the kernel. 
          template< typename In >
          void SetAnnotationGeometry( In first, In last ); 
          /// \ru Выдать аннотационные объекты ядра. У приёмника должен быть определён метод push_back. \en Get the annotation objects of the kernel. Method push_back should be defined for the receiver. 
          template< typename Out >
          void GetAnnotationGeometry( Out dest ) const; 

          /// \ru Получить текстовые аннотационные объекты. \en Get the text annotation object. 
          template< typename In >
          void SetAnnotationText( In first, In last ); 
          /// \ru Выдать текстовые аннотационные объекты. У приёмника должен быть определён метод push_back. \en Get text annotation objects. Method push_back should be defined for the receiver. 
          template< typename Out >
          void GetAnnotationText( Out dest ) const; 

          /// \ru Добавить плоские геометрические объекты, преобразуя их в пространственные, используя текущую ЛСК. \en Add planar objects to geometric objects using current location. 
          void AddPlaneItems( const std::vector<SPtr<MbPlaneItem> >& );

          /// \ru Задать ЛСК. \en Specify LCS. 
          void SetLocation( const MbPlacement3D & loc ); 
          /// \ru Получить ЛСК. \en Get LCS. 
          MbPlacement3D GetLocation() const; 

          /// \ru Задать имя. \en Specify name.
          void          SetC3dName ( const c3d::string_t& nm );

          /// \ru Получить имя. \en Specify name.
          c3d::string_t GetC3dName ( ) const;

          /// \ru Задать видимость. \en Set visibility.
          void SetVisibility( bool v ); 
          /// \ru Видим ли объект. \en Is object vivible.
          bool IsVisible() const; 

          /// \ru Создать дубликат и трансформировать его согласно матрице. \en Create a replica then transform it.
  virtual SPtr<MaAnnotationItem> ShallowDuplicateTransform( const MbMatrix3D &, MbRegDuplicate* partDuplicateRegistry = nullptr );

          /// \ru Инициализировать все поля за исключением ЛСК данными присланного. \en Init all fields except for location according to the specified item.
          void InitExceplLocation( const MaAnnotationItem & init );

protected:

          /// \ru Заменить геометрические элементы трансформированными копиями. \en Replace all geometric items by transformed copies.
  virtual void DuplicateTransformDeometry( const MbMatrix3D & );
};


typedef SPtr<MaAnnotationItem> AnnotationSPtr;

/** \brief \ru Контейнер объектов аннотации.
\en Container of annotation objects. \~
\ingroup Exchange_Base
*/
typedef std::vector<AnnotationSPtr> vector_of_annotation;
typedef std::vector<AnnotationSPtr> AnnotationSptrVector;


/** \brief \ru Ассоциация наборов аннотационных объектов элементам со счётчиком ссылок.
\en Association of sets of annotation objects with elements with reference counter. \~
\ingroup Exchange_Base
*/
typedef std::map< SPtr<const MbItem>, AnnotationSptrVector > map_of_visual_items;


/** \brief \ru Контейнер текстовых блоков.
\en Container of text blocks. \~
\ingroup Exchange_Base
*/
typedef std::vector< SPtr<MaTextItem> > vector_of_text;



//------------------------------------------------------------------------------
/** \brief \ru Размер - родоначальник классов для размеров различных типов.
           \en Dimension is the parent of all classes for dimensions of different types. \~
*/
// ---
class CONV_CLASS MaDimension : public MaAnnotationItem {
  double value;          ///< \ru Значение размера. \en A value of dimension. 
  double valuePlus;      ///< \ru Отклонение размера в сторону увеличения. \en Deviation (increase) of size. 
  double valueMinus;     ///< \ru Отклонение размера в сторону уменьшения. \en Deviation (decrease) of size. 
  bool   isRangeSet;     ///< \ru Если false, то задан только диапазон изменения, иначе можно вычислить погрешности в обе стороны. \en If it equals false, then only the range of changing is specified, else the tolerances in both directions can be computed.   
  bool   isValueDefined; ///< \ru Задан ли номинал. \en Whether the nominal is given. 
protected:
  MaDecoratedCurve dimensionCurve;

  OBVIOUS_PRIVATE_COPY( MaDimension )
protected:
  MaDimension( const MbPlacement3D& loc, MbCurve3D* dimCurve );
  MaDimension( const MbPlacement3D& loc, const MaDecoratedCurve& dimCurve );
public:
  /// \ru Получить тип объекта. \en Get the object type. 
  virtual Mae_AnnotationType IsA() const; 
  /// \ru Получить групповой тип объекта. \en Get the group type of the object. 
  virtual Mae_AnnotationType Type() const; 

          /// \ru Получить размерную кривую. \en Get the dimensional curve. 
          MbCurve3D* GetDimensionCurve() const; 

          /// \ru Задать номинал. \en Set a value. 
          void       SetValue( double v );
          /// \ru Задать диапазон и значение. \en Set a range and a value. 
          void       SetRange( double v, double vPlus, double vMinus ); 
          /// \ru Задать диапазон. \en Set range. 
          void       SetRange( double vPlus, double vMinus ); 
          /// \ru Получить номинал. \en Get value. 
          bool       GetValue( double& v ) const; 
          /// \ru Получить границы диапазона и значение, если они заданы. \en Get bounds of range and a value if they are specified. 
          bool       GetRange( double& v, double& vPlus, double& vMinus ) const; 
          /// \ru Получить границы диапазона, если они заданы. \en Get bounds of the range if they are specified. 
          bool       GetRange( double& vPlus, double& vMinus ) const; 
          /// \ru Заданы ли границы диапазона. \en Whether the bounds of range are specified. 
          bool       IsRangeDefined() const; 
          /// \ru Задано ли значение. \en Whether the value is specified. 
          bool       IsValueDefined() const; 
          /** \brief \ru Добавить законцовочный символ.
                     \en Add a terminator. \~
          \param [in] init - \ru Параметры задаваемого символа.
                             \en Parameters of specified symbol. \~
          \return \ru - true, если задана размерная кривая и хотя бы один из законцовочных символов не был задан.
                  \en - true, if a dimensional curve is specified and at least one of terminators has not been specified. \~
          */
          bool       AddTerminator( const MaTerminatorSymbol& init );
          /// \ru Получить первый законцовочный символ. \en Get the first terminator. 
          bool       GetFirstTerminator( MaTerminatorSymbol& first ) const;
          /// \ru Получить второй законцовочный символ. \en Get the second terminator. 
          bool       GetSecondTerminator( MaTerminatorSymbol& second ) const;

          void       InitValueTerminators( const MaDimension& init );
protected:
          /// \ru Заменить геометрические элементы трансформированными копиями. \en Replace all geometric items by transformed copies.
          virtual void DuplicateTransformDeometry( const MbMatrix3D & );
};


//------------------------------------------------------------------------------
/** \brief \ru Линейный размер.
           \en Linear dimension. \~
*/
// ---
class CONV_CLASS MaLinearDimension : public MaDimension {
private:
  SPtr<const MbRefItem> bindBase;         ///< \ru Первый объект привязки. \en The first binding object. 
  SPtr<const MbRefItem> bindTarget;       ///< \ru Второй объект привязки. \en The second binding object. 
  MaDecoratedCurve projectionBase;   ///< \ru Проекционная кривая к первому объекту привязки в смысле STEP. \en Projection curve to the first binding object in sense of  STEP. 
  MaDecoratedCurve projectionTarget; ///< \ru Проекционная кривая ко второму объекту привязки в смысле STEP. \en Projection curve to the second binding object in sense of STEP. 
  SPtr<MbCurve3D>       path;             ///< \ru Кривая, вдоль которой проводится измерение. Если не задана, то размер есть кратчайший. \en A curve along which the measurement is performed. If not specified, then the size is shortest. 

  OBVIOUS_PRIVATE_COPY( MaLinearDimension )
public:
  MaLinearDimension ( const MbRefItem* base, const MbRefItem* target, 
                      MbLineSegment3D* projBase, MbLineSegment3D* projTarget, 
                      MbLineSegment3D* dimensionCurve, const MbPlacement3D& loc );

  MaLinearDimension ( const MbRefItem* base, const MbRefItem* target,
                      MbLineSegment3D* projBase, MbLineSegment3D* projTarget, 
                      const MaDecoratedCurve dimensionCurve, const MbPlacement3D& loc );

  virtual Mae_AnnotationType IsA() const;

  virtual bool       VisualItemsEmpty() const;

  /// \ru Получить базовый объект привязки. \en Get the base binding object. 
  const   MbRefItem * GetBindBase(); 
  /// \ru Получить второй объект привязки. \en Get the second binding object. 
  const   MbRefItem * GetBindTarget(); 

          /// \ru Получить проекционную кривую к базовому объекту привязки. \en Get projection curve to the base binding object. 
          MbLineSegment3D* GetProjectionBase() const; 
          /// \ru Получить проекционную кривую ко второму объекту привязки. \en Get the projection curve to the second binding object. 
          MbLineSegment3D* GetProjectionTarget() const; 

          /// \ru Задать кривую, вдоль которой проводится измерение. \en Set the curve  the measurement is performed along. 
          void       SetPath( MbCurve3D* inPath );
          /// \ru Получить кривую, вдоль которой проводится измерение. \en Get the curve the measurement is performed along. 
          MbCurve3D* GetPath() const;

          /// \ru Создать дубликат и трансформировать его согласно матрице. \en Create a replica then transform it.
  virtual SPtr<MaAnnotationItem> ShallowDuplicateTransform( const MbMatrix3D &, MbRegDuplicate* partDuplicateRegistry = nullptr );

protected:
  // Заменить геометрические элементы трансформированными копиями.
  virtual void DuplicateTransformDeometry( const MbMatrix3D & );
};


//------------------------------------------------------------------------------
/** \brief \ru Угловой размер.
           \en Angular dimension. \~
*/
// ---
class CONV_CLASS MaAngularDimension : public MaDimension {
private:
  SPtr<const MbRefItem> bindBase;         ///< \ru Первый объект привязки. \en The first binding object. 
  SPtr<const MbRefItem> bindTarget;       ///< \ru Второй объект привязки. \en The second binding object. 
  MaDecoratedCurve projectionBase;   ///< \ru Проекционная кривая к первому объекту привязки в смысле STEP. \en Projection curve to the first binding object in sense of  STEP. 
  MaDecoratedCurve projectionTarget; ///< \ru Проекционная кривая ко второму объекту привязки в смысле STEP. \en Projection curve to the second binding object in sense of STEP. 

  OBVIOUS_PRIVATE_COPY( MaAngularDimension )
public:
  MaAngularDimension( const MbRefItem* base, const MbRefItem* target, 
                      MbLineSegment3D* projBase, MbLineSegment3D* projTarget, 
                      MbArc3D* dimensionCurve, const MbPlacement3D& loc );

  MaAngularDimension( const MbRefItem* base, const MbRefItem* target,
                      MbLineSegment3D* projBase, MbLineSegment3D* projTarget,
                      const MaDecoratedCurve&, const MbPlacement3D& loc );

  virtual Mae_AnnotationType IsA() const ;

  virtual bool       VisualItemsEmpty() const;

  /// \ru Получить базовый объект привязки. \en Get the base binding object. 
  const   MbRefItem * GetBindBase(); 
  /// \ru Получить второй объект привязки. \en Get the second binding object. 
  const   MbRefItem * GetBindTarget();

          /// \ru Получить проекционную кривую к базовому объекту привязки. \en Get projection curve to the base binding object. 
          MbLineSegment3D * GetProjectionBase() const;
          /// \ru Получить проекционную кривую ко второму объекту привязки. \en Get the projection curve to the second binding object. 
          MbLineSegment3D * GetProjectionTarget() const;
          /// \ru Если заданы проекционные кривые и если они не параллельны, получить точку пересечения или скрещивания. Метод работает и за пределеми параметрической области. \en If the projection curves are specified and if they are not parallel, get the point of intersection or crossing. The method works outside the bounds of a parametric region too. 
          bool NearestBetweenProjections( MbCartPoint3D& pnt );
          /// \ru Создать дубликат и трансформировать его согласно матрице. \en Create a replica then transform it.
  virtual SPtr<MaAnnotationItem> ShallowDuplicateTransform( const MbMatrix3D &, MbRegDuplicate* partDuplicateRegistry = nullptr );

protected:
  // Заменить геометрические элементы трансформированными копиями.
  virtual void DuplicateTransformDeometry( const MbMatrix3D & );
};


//------------------------------------------------------------------------------
/** \brief \ru Радиальный размер.
           \en Radial dimension. \~
*/
// ---
class CONV_CLASS MaRadialDimension : public MaDimension {
private:
  SPtr<const MbRefItem> bindBase;       ///< \ru Объект привязки. \en Binding object. 
  MaDecoratedCurve projectionBase; ///< \ru Проекционная кривая к объекту привязки в смысле STEP. \en Projection curve to the binding object in sense of STEP. 

  OBVIOUS_PRIVATE_COPY( MaRadialDimension )
public:
  MaRadialDimension( const MbRefItem* base, MbLineSegment3D* projBase, 
                     MbLineSegment3D* dimensionCurve, const MbPlacement3D& loc );

  MaRadialDimension( const MbRefItem* base, MbLineSegment3D* projBase, 
                     const MaDecoratedCurve& dimensionCurve, const MbPlacement3D& loc );

  virtual Mae_AnnotationType IsA() const;

  virtual bool       VisualItemsEmpty() const;

  /// \ru Получить базовый объект привязки. \en Get the base binding object. 
  const   MbRefItem * GetBindBase(); 
          /// \ru Получить проекционную кривую к базовому объекту привязки. \en Get projection curve to the base binding object. 
          MbLineSegment3D * GetProjectionBase() const; 
          /// \ru Создать дубликат и трансформировать его согласно матрице. \en Create a replica then transform it.
  virtual SPtr<MaAnnotationItem> ShallowDuplicateTransform( const MbMatrix3D &, MbRegDuplicate* partDuplicateRegistry = nullptr );

protected:
  // Заменить геометрические элементы трансформированными копиями.
  virtual void DuplicateTransformDeometry( const MbMatrix3D & );
};


//------------------------------------------------------------------------------
/** \brief \ru Диаметральный размер.
           \en Diameter dimension. \~
*/
// ---
class CONV_CLASS MaDiameterDimension : public MaDimension {
private:
  SPtr<const MbRefItem> bindBase;         ///< \ru Объект привязки. \en Binding object. 
  MaDecoratedCurve projectionBase;   ///< \ru Первая проекционная кривая к объекту привязки в смысле STEP. \en The first projection curve to binding object in sense of STEP. 
  MaDecoratedCurve projectionTarget; ///< \ru Вторая проекционная кривая к объекту привязки в смысле STEP. \en The second projection curve to binding object in sense of STEP. 

  OBVIOUS_PRIVATE_COPY( MaDiameterDimension )
public:
  MaDiameterDimension( const MbRefItem* base, MbLineSegment3D* projBase, 
                       MbLineSegment3D* projTarget, MbLineSegment3D* dimCurve, 
                       const MbPlacement3D& loc );

  MaDiameterDimension( const MbRefItem* base, MbLineSegment3D* projBase, 
                       MbLineSegment3D* projTarget, const MaDecoratedCurve& dimCurve, 
                       const MbPlacement3D& loc );

  virtual Mae_AnnotationType IsA() const;

  virtual bool       VisualItemsEmpty() const;

  /// \ru Получить базовый объект привязки. \en Get the base binding object. 
  const   MbRefItem * GetBindBase(); 

          /// \ru Получить проекционную кривую к базовому объекту привязки. \en Get projection curve to the base binding object. 
          MbLineSegment3D * GetProjectionBase() const; 
          /// \ru Получить вторую проекционную кривую к объекту привязки. \en Get the first projection curve to the binding object. 
          MbLineSegment3D * GetProjectionTarget() const;
          /// \ru Создать дубликат и трансформировать его согласно матрице. \en Create a replica then transform it.
  virtual SPtr<MaAnnotationItem> ShallowDuplicateTransform( const MbMatrix3D &, MbRegDuplicate* partDuplicateRegistry = nullptr );

protected:
  // Заменить геометрические элементы трансформированными копиями.
  virtual void DuplicateTransformDeometry( const MbMatrix3D & );
};


//------------------------------------------------------------------------------
/** \brief \ru Выносной элемент - родоначальник классов для обозначений различных типов.
\en Callout is the parent of all classes for callouts of different types. \~
*/
// ---
class CONV_CLASS MaCallout : public MaAnnotationItem {
  Mae_AnnotationType whatIs; ///< \ru Подтип объекта. \en Object subtype. 
  std::vector<MaDecoratedCurve> leaderLines; ///< \ru Линии выноски. \en Leader lines.
public:
  /// \ru Получить тип объекта. \en Get the object type. 
  virtual Mae_AnnotationType IsA() const;
  /// \ru Получить групповой тип объекта. \en Get the group type of the object. 
  virtual Mae_AnnotationType Type() const;
  /// \ru Создать объект заданного типа объекта. \en Create object of specified type. 
  static MaCallout* Create( const MbPlacement3D& location, Mae_AnnotationType subtype );

  /// \ru Создать дубликат и трансформировать его согласно матрице. \en Create a replica then transform it.
  virtual SPtr<MaAnnotationItem> ShallowDuplicateTransform( const MbMatrix3D&, MbRegDuplicate* = nullptr );

  void AddLeaderLine( const MaDecoratedCurve& leader ); ///< \ru Добавить линию выноски. \en Add leader line.
  void AddLeaderLines( const std::vector<MaDecoratedCurve>& leaders ); ///< \ru Добавить линию выноски. \en Add leader line.
  size_t LeaderLinesCount() const; ///< \ru Получить число линий выноски. \en Get number of leader lines.
  bool LeaderLineInfo( size_t index, MaDecoratedCurve& callout ) const; ///< \ru Получить линию выноски с указанным индексом. \en Get of leader lines at specified index.
protected:
  ///< Заменить геометрические элементы трансформированными копиями.
  virtual void DuplicateTransformDeometry( const MbMatrix3D& matr );

private:
  MaCallout( const MbPlacement3D& location, Mae_AnnotationType subtype ); ///< \ru Конструктор. \en Constructor.

  OBVIOUS_PRIVATE_COPY(MaCallout)
};


//------------------------------------------------------------------------------
/** \brief \ru Шероховатость поверхности.
\en Surface condition. \~
*/
// ---
class CONV_CLASS MaSurfaceCondition : public MaAnnotationItem {
  SPtr< const MbRefItem > baseObject;
  double value;
public:
  /// \ru Конструктор. \en Constructor.
  MaSurfaceCondition( const MbPlacement3D& location );

  /// \ru Создать дубликат и трансформировать его согласно матрице. \en Create a replica then transform it.
  virtual SPtr<MaAnnotationItem> ShallowDuplicateTransform( const MbMatrix3D&, MbRegDuplicate* partDuplicateRegistry = nullptr );

  /// \ru Получить тип объекта. \en Get the object type. 
  virtual Mae_AnnotationType IsA() const;
  /// \ru Получить групповой тип объекта. \en Get the group type of the object. 
  virtual Mae_AnnotationType Type() const;
  /// \ru Получить значение. \en Get the value. 
  double GetValue() const { return value; }
  /// \ru Задать значение. \en Set the value. 
  void SetValue( const double val ) { value = val; }

  OBVIOUS_PRIVATE_COPY( MaSurfaceCondition )
};


//------------------------------------------------------------------------------
/** \brief \ru Допуск формы.
\en Shape tolerance. \~
*/
// ---
class CONV_CLASS MaShapeTolerance : public MaAnnotationItem {
  SPtr< const MbRefItem > baseObject;
  double value;
public:
  /// \ru Конструктор. \en Constructor.
  MaShapeTolerance( const MbPlacement3D& location );

  /// \ru Создать дубликат и трансформировать его согласно матрице. \en Create a replica then transform it.
  virtual SPtr<MaAnnotationItem> ShallowDuplicateTransform( const MbMatrix3D&, MbRegDuplicate* partDuplicateRegistry = nullptr );

  /// \ru Получить тип объекта. \en Get the object type. 
  virtual Mae_AnnotationType IsA() const;
  /// \ru Получить групповой тип объекта. \en Get the group type of the object. 
  virtual Mae_AnnotationType Type() const;
  /// \ru Получить значение. \en Get the value. 
  double GetValue() const { return value; }
  /// \ru Задать значение. \en Set the value. 
  void SetValue( const double val ) { value = val; }

  OBVIOUS_PRIVATE_COPY(MaShapeTolerance)
};


//------------------------------------------------------------------------------
// \ru Задать геометрические объекты аннотации \en Set geometric objects of annotation. 
// ---
template< typename In >
void MaAnnotationItem::SetAnnotationGeometry( In first, In last ) { 
  std::for_each( annotationGeometry.begin(), annotationGeometry.end(), ReleaseItem<const MbItem> ); 
  annotationGeometry.assign( first, last ); 
  std::for_each( annotationGeometry.begin(), annotationGeometry.end(), AddRefItem<const MbItem> ); 
}


//------------------------------------------------------------------------------
// \ru Получить геометрические объекты аннотации \en Get geometric objects of annotation. 
// ---
template< typename Out >
void MaAnnotationItem::GetAnnotationGeometry( Out dest ) const { 
  std::copy( annotationGeometry.begin(), annotationGeometry.end(), dest ); 
}
  

//------------------------------------------------------------------------------
// \ru Задать текстовые объекты аннотации \en Set text objects of annotation. 
// ---
template< typename In >
void MaAnnotationItem::SetAnnotationText( In first, In last ) {   
  annotationText.assign( first, last ); 
}


//------------------------------------------------------------------------------
// \ru Получить текстовые объекты аннотации \en Get text objects of annotation 
// ---
template< typename Out >
void MaAnnotationItem::GetAnnotationText( Out dest ) const { 
  std::copy( annotationText.begin(), annotationText.end(), dest ); 
}

//------------------------------------------------------------------------------
// Получить геометрию PMI
// ---
c3d::ItemsSPtrVector GetPMIGeometry( const MbPMI & it );


//------------------------------------------------------------------------------
// Получить строковое название спецсимвола
// ---
c3d::string_t PredefinedSymbolToString( const MbeDefinedDimensionSymbol & symb );

//------------------------------------------------------------------------------
// Получить спецсимвол из строки
// ---
bool StringToPredefinedSymbol( const c3d::string_t & txt, MbeDefinedDimensionSymbol & predefSymb );

//------------------------------------------------------------------------------
// Получить строковое название тэга форматирования
// ---
c3d::string_t TextTagToString( const MaeTextFormatTag & tag );

//------------------------------------------------------------------------------
// Получить тэг форматирования из строки
// ---
MaeTextFormatTag StringToTextTag( const c3d::string_t & txt );

//------------------------------------------------------------------------------
// Получить строковое название типа терминатора
// ---
c3d::string_t GetTerminatorTypeStr( const MbeDefinedTerminatorSymbol termTrype );

#endif // __CONV_ANNOTATION_ITEM_H
