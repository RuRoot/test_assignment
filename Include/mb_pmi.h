////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Элемент аннотации.
         \en Annotation item.
  \details \ru Элемент аннотации и сопутствующие классы: текстовые элементы, выноски,
               хранилище номинала и диапазона.
           \en Annotation item and related classes: text elements, callouts,
               value and range storage.

*/
////////////////////////////////////////////////////////////////////////////////


#ifndef __MB_PMI_H
#define __MB_PMI_H

#include <legend.h>
#include <topology_item.h>
#include <model_item.h>
#include <mb_placement.h>
#include <curve.h>
#include <curve3d.h>

//#define PMI_RAD_DIAM_USE_CALLOUT_INSTEAD_OF_DIM_PLUS_PROJ_CRV

//----------------------------------------------------------------------------------------
/** \brief \ru Элемент текста объекта аннотации.
           \en Reference-counted object.  \~
  \details \ru Базовый класс текстовых элементов - составного текста или несущего содержание. \n
           \en Base class for text items - composite or readable. \n\~

  \note \ru Текст размещается в плоскости элемента аннотации и может быть либо группой текстовых
            элементов, либо строкой либо специальным символом.
        \en Text is located in the plane of annotation and can be either formatter or unformatted
            text or symbol or a group of text elements. \~
  \ingroup Legend
*/
// ---
class MATH_CLASS MbTextItem : public TapeBase, public MbRefItem
{
public:
  /// \ru Создать копию объекта. \en Create a copy of an object.
  virtual SPtr<MbTextItem> Clone( MbRegDuplicate * = nullptr ) const = 0;
  /// \ru Являются ли объекты равными. \en Are the objects equal. 
  virtual bool IsSame( const MbTextItem & to, double accuracy ) const = 0;

  DECLARE_PERSISTENT_CLASS( MbTextItem );

  /// \ru Присутствует ли тэг. \en Wether a tag present. 
  //  \ru Метод устарел. \en The method is deprecated.
  DEPRECATE_DECLARE  bool IsTagPresent( const c3d::string_t & tag ) const;
  /// \ru Присутствуют ли тэги. \en Wether tags present. 
  //  \ru Метод устарел. \en The method is deprecated.
  DEPRECATE_DECLARE bool IsHasTags() const;
  ///< \ru Назначено ли менее 2 тэгов. \en If less than two tags assinged.   
  //  \ru Метод устарел. \en The method is deprecated.
  DEPRECATE_DECLARE bool TagUniqueOrUndefined() const;
  ///< \ru Получить тэг, если он единственный. \en Get the tag provided it id qnique.
  //  \ru Метод устарел. \en The method is deprecated.
  DEPRECATE_DECLARE bool GetTagIfUnique( c3d::string_t & tag ) const;
  ///< \ru Сбросить тэг. \en Erase a tag.
  //  \ru Метод устарел. \en The method is deprecated.
  DEPRECATE_DECLARE void EraseTag( const c3d::string_t & tag );
  /// \ru Добавить тэг, если он не пустой. \en Add a non-empty tag.
  //  \ru Метод устарел. \en The method is deprecated.
  DEPRECATE_DECLARE void AddTag( const c3d::string_t & tag );

  /// \ru Принять посетителя. \en Accept a visitior. 
  virtual void Accept( Visitor & ) = 0;

  /// \ru Принять посетителя. \en Accept a visitior. 
  virtual void Accept( Visitor & ) const = 0;

protected:

  /// \ru Пользователские тэги, не предназначенные для сериализации. \en User tags not for serialization.
  std::vector<c3d::string_t> m_userTags;

  /// \ru Конструктор по умолчанию. \en Default constructor.
  MbTextItem();
  MbTextItem( const MbTextItem & it );
};


IMPL_PERSISTENT_OPS( MbTextItem )


//----------------------------------------------------------------------------------------
/** \brief \ru Интерпретация текста.
           \en Text content type.  \~
  \details \ru Текст или специальный символ. \n
           \en Object counting number of its owners. \n\~
  \ingroup Legend
*/
// ---
enum class MbeTextLiteralForm
{ 
  unformattedString = 0,  ///< \ru Текст без кодов форматирования. \en Text as is.
  preDefinedCode    = 1,  ///< \ru Код специального символа. \en Text is a code of a symbol.
  formattedString   = 2   ///< \ru Текст с элементами форматирования форматирования, дублирует содержание неформатированных строк внутри одного MbPmi. \en Formatted text, duplicates unformatted text from the same MbPmi.
};



#ifdef PMI_RAD_DIAM_USE_CALLOUT_INSTEAD_OF_DIM_PLUS_PROJ_CRV 
//----------------------------------------------------------------------------------------
/** \brief \ru Тип кривой аннотации.
  dimensionCurve и projectionCurve используются совместно для линейных и диаметральных размеров,
  для всех остальных аннотаций используется callout,  в том числе для диаметрального размера, 
  изображенного с помощью одной кривой типа callout с двумя терминаторами.
  callout содержит 0/1/2 терминатора, dimensionCurve - 1/2, projectionCurve - 0.
  callout и projectionCurve ссылаются на геометрический объект, dimensionCurve - нет.
           \en Type of curve with terminators.  \~
  \ingroup Legend
*/
// ---
enum class MbeCalloutCurveType
{ 
  callout = 0,            ///< \ru Кривая-выноска, связанная с характеризуемым геометрическим объектом. \en Callout.
  dimensionCurve    = 1,  ///< \ru Размерная кривая, связанная с проекционной кривой. \en Dimension curve.
  projectionCurve   = 2   ///< \ru Проекционная кривая размера, связанная с характеризуемым геометрическим объектом, не содержит терминаторы. \en Projection curve.
};

#else
//----------------------------------------------------------------------------------------
/** \brief \ru Тип кривой с терминаторами.
           \en Type of curve with terminators.  \~
  \ingroup Legend
*/
// ---
enum class MbeCalloutCurveType
{
  callout = 0,            ///< \ru Кривая-выноска. \en Callout.
  dimensionCurve = 1,  ///< \ru Размерная кривая. \en Dimension curve.
  projectionCurve = 2   ///< \ru Кривая к характеризуемому объекту. \en Projection curve.
};

#endif // PMI_RAD_DIAM_USE_CALLOUT_INSTEAD_OF_DIM_PLUS_PROJ_CRV 

//----------------------------------------------------------------------------------------
/** \brief \ru Тип численной характеристики.
           \en Type of numerical characteristic.  \~
  \ingroup Legend
*/
// ---
enum class MbeNumericCharacteristicType
{
  linearDimension,    ///< \ru Линейный размер. \en Linear dimension.
  angularDimension,   ///< \ru Угловой размер. \en Angular dimension.
  radialDimension,    ///< \ru Радиальный размер. \en Radial dimension.
  diameterDimension,  ///< \ru Диаметральный размер. \en Diameter dimension.
  shapeTolerance,     ///< \ru Погрешность формы. \en Shape tolerance.
  surfaceRoughness,   ///< \ru Шероховатость поверхности. \en Surface roughness.
  voidNumeric         ///< \ru Численное значение игнорируется. \en Numeric characteristics ignored.
};


/// \ru Предопределённый тип терминатора - закрашенная стрелка. \en Pre-defined type of terminator - filled arrow. 
const c3d::string_t c3d_terminatorFilledArrow = _T("C3D TERMINATOR FILLED ARROW");

/// \ru Предопределённый тип терминатора - закрашенный квадрат. \en Pre-defined type of terminator - filled square. 
const c3d::string_t c3d_terminatorFilledBox = _T("C3D TERMINATOR FILLED BOX");

/// \ru Предопределённый тип терминатора - закрашенная окружность. \en Pre-defined type of terminator - filled circle. 
const c3d::string_t c3d_terminatorFilledCircle = _T("C3D TERMINATOR FILLED CIRCLE");

/// \ru Предопределённый тип терминатора - незакрашенная стрелка. \en Pre-defined type of terminator - unfilled arrow. 
const c3d::string_t c3d_terminatorUnfilledArrow = _T("C3D TERMINATOR UNFILLED ARROW");

/// \ru Предопределённый тип терминатора - незакрашенный прямоугольник. \en Pre-defined type of terminator - unfilled box. 
const c3d::string_t c3d_terminatorUnfilledBox = _T("C3D TERMINATOR UNFILLED BOX");

/// \ru Предопределённый тип терминатора - незакрашенная окружность. \en Pre-defined type of terminator - unfilled circle. 
const c3d::string_t c3d_terminatorUnfilledCircle = _T("C3D TERMINATOR UNFILLED CIRCLE");

/// \ru Предопределённый тип терминатора - открытая стрелка . \en Pre-defined type of terminator - open arrow. 
const c3d::string_t c3d_terminatorOpenArrow = _T("C3D TERMINATOR OPEN ARROW");

/// \ru Предопределённый тип терминатора - косая черта. \en Pre-defined type of terminator - slash. 
const c3d::string_t c3d_terminatorSlash = _T("C3D TERMINATOR SLASH");

/// \ru Предопределённый тип терминатора - знак интеграла. \en Pre-defined type of terminator - integral circle. 
const c3d::string_t c3d_terminatorIntegral = _T("C3D TERMINATOR INTEGRAL");

/// \ru Предопределённый тип терминатора - начало размера - незакрашенная окружность. \en Pre-defined type of terminator - dimension origin  - unfilled circle. 
const c3d::string_t c3d_terminatorDimensionOrigin = _T("C3D TERMINATOR DIMENSION ORIGIN");

/// \ru Предопределённый тип терминатора - стрелка базы отсчета. \en Pre-defined type of terminator - datum arrow. 
const c3d::string_t c3d_terminatorDatumArrow = _T( "C3D TERMINATOR DATUM ARROW" );

/// \ru Предопределённый тип терминатора - незакрашенная стрелка базы отсчета. \en Pre-defined type of terminator - unfilled datum arrow. 
const c3d::string_t c3d_terminatorDatumUnfilledArrow = _T( "C3D TERMINATOR DATUM UNFILLED ARROW" );


//----------------------------------------------------------------------------------------
/** \brief \ru Текстовый элемент, несущий содержание.
           \en Text element which has content.  \~
  \details \ru Текст или специальный символ. \n
           \en Either text or pre-defined simbol. \n\~
  \ingroup Legend
*/
// ---
class MATH_CLASS MbTextLiteral final : public MbTextItem
{
  c3d::string_t       m_text; ///< \ru Текст или код символа. \en Text or symbol's code.
  c3d::string_t       m_font; ///< \ru Шрифт. \en Font.
    /** \brief \ru Положение в плоскости аннотации.
             \en Location in annotation plane. \~
    \details \ru Начало системы координат соответствует левому нижнему углу.
             \en The origin of the location is position of the left-bottom corner. \~
  */
  MbPlacement         m_location;
  double              m_width;    ///< \ru Ширина. \en Width.
  double              m_height;   ///< \ru Высота. \en Height.
  MbeTextLiteralForm  m_textForm; ///< \ru Способ интерпретации текста. \en Meaning of the text field..
public:
  
  /** \brief \ru Конструктор.
             \en Constructor. \~
       \param[in] initText - \ru Текст, 
                             \en Text, \~
       \param[in] place -    \ru Расположение в плоскости,
                             \en Location in plane, \~
       \param[in] textForm - \ru Тип текстового элемента.
                             \en Type of text element. \~
        */
  explicit MbTextLiteral( const c3d::string_t & initText, const MbPlacement & place, MbeTextLiteralForm textForm );
  
  /// \ru Получить текст. \en Get text. 
  c3d::string_t GetText() const;
  /// \ru Получить положение в плоскости аннотации. \en Get location in annotation plane. 
  MbPlacement   GetLocation() const;
  /// \ru Получить ширину. \en Get width. 
  double        GetWidth() const;
  /// \ru Получить высоту. \en Get height. 
  double        GetHeight() const;
  /// \ru Задать ширину. \en Set width. 
  bool          SetWidth( double w );
  /// \ru Задать высоту. \en Set height.
  bool          SetHeight( double h );
  /// \ru Получить тип текста. \en Get text form. 
  MbeTextLiteralForm GetTextForm() const;
  /// \ru Получить шрифт. \en Get font. 
  c3d::string_t GetFont() const;
  /// \ru Задать шрифт. \en Set font. 
  void SetFont( const c3d::string_t & font );
  
  /// \ru Создать копию объекта. \en Create a copy of an object.
  SPtr<MbTextItem> Clone( MbRegDuplicate * = nullptr ) const final;
  /// \ru Являются ли объекты равными. \en Are the objects equal. 
  bool IsSame( const MbTextItem & to, double accuracy ) const final;

  VISITING_CLASS( MbTextLiteral )

  void Accept( Visitor & visitor ) const override
  {
    auto * impl = dynamic_cast<VisitorImpl<const MbTextLiteral> *>(&visitor);
    if( impl )
      impl->Visit( *this );
  }

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbTextLiteral )
  OBVIOUS_PRIVATE_COPY( MbTextLiteral )
};

IMPL_PERSISTENT_OPS( MbTextLiteral )


//----------------------------------------------------------------------------------------
/** \brief \ru Составной текст.
           \en Composite text.  \~
  \details \ru Группа из более, чем одного текстового элемента. \n
           \en Group of two or more text items. \n\~
  \ingroup Legend
*/
// ---
class MATH_CLASS MbCompositeText final : public MbTextItem
{
  /// \ru Элементы группы текстовых элементов. \en Grouped items.
  std::vector<SPtr<MbTextItem>> m_textItems;

  /// \ru Конструктор. \en Constructor. 
  explicit MbCompositeText( std::vector<SPtr<MbTextItem>> && textItems );
public:

  /** \brief  \ru Создать составной текстовый элемент.
            \en Create composite text element. \~
    \details \ru Элемент создаётся только в том случае, если в исходных данных будет два или более ненулевых элемента.
             \en New element is created if two or more text items found in the source array. \~
  */
  static SPtr<MbCompositeText> Create( const std::vector<SPtr<MbTextItem>> & textItems );

  /// \ru Получить количество текстовых элементов. \en Get count of text items. 
  size_t GetTextItemsCount() const;

  /// \ru Получить текстовый элемент по индексу. \en Get text item at the specified postion. 
  SPtr<MbTextItem> GetTextItem( size_t index ) const;

  /// \ru Получить текстовые элементы. \en Get text items. 
  void GetTextItems( std::vector<SPtr<MbTextItem>> & txt ) const;

  /// \ru.Заменить текстовые элементы. \en Reset text elements. 
  void ResetItems( const std::vector<SPtr<MbTextItem>> & textItems );

  /// \ru Создать копию объекта. \en Create a copy of an object.
  SPtr<MbTextItem> Clone( MbRegDuplicate * = nullptr ) const final;
  /// \ru Являются ли объекты равными. \en Are the objects equal. 
  bool IsSame( const MbTextItem & to, double accuracy ) const final;

  VISITING_CLASS( MbCompositeText )

  void Accept( Visitor & visitor ) const override
  {
    auto * impl = dynamic_cast<VisitorImpl<const MbCompositeText> *>(&visitor);
    if( impl )
      impl->Visit( *this );
  }
  
  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbCompositeText )
  OBVIOUS_PRIVATE_COPY( MbCompositeText )
};


IMPL_PERSISTENT_OPS( MbCompositeText )


//----------------------------------------------------------------------------------------
/** \brief \ru Значение и диапазон изменения величины.
           \en Value and range.  \~
  \details \ru Может быть определён либо номинал, либо диапазон изменения, либо и то и другое. Контроль данных не производится. \n
           \en Value or range or both can be defined. The check of falues is not performed. \n\~
  \ingroup Legend
*/
// ---
class MATH_CLASS MbValueRange final : public TapeBase {
  double m_nominalValue;    ///< \ru Номинал. \en Nominal value.
  double m_upperValue;      ///< \ru Верхняя граница диапазона. \en Upper value of range.
  double m_lowerValue;      ///< \ru Нижняя граница диапазона. \en Lower value of range.
  bool   m_nominalDefined;  ///< \ru Признак, определён ли номинал. \en Flag if the value is defined.
  bool   m_rangeDefined;    ///< \ru Признак, определён ли диапазон. \en Flag if the range is defined.
public:
  /** \brief \ru Конструктор по умолчанию.
             \en Default constructor. \~
        */
  MbValueRange();
  
  /** \brief \ru Конструктор.
             \en Constructor. \~
       \param[in] valie - \ru Номинал. 
                          \en Value. \~
        */
  MbValueRange( double value );

  /** \brief \ru Конструктор.
             \en Constructor. \~
       \param[in] lower - \ru Нижняя граница диапазона,
                          \en Lower value of range, \~
       \param[in] upper - \ru Верхняя граница диапазона.
                          \en Upper value of range. \~
        */

  MbValueRange( double lower, double upper );
  /** \brief \ru Конструктор.
             \en Constructor. \~
       \param[in] value - \ru Номинал, 
                          \en Value, \~
       \param[in] lower - \ru Нижняя граница диапазона,
                          \en Lower value of range, \~
       \param[in] upper - \ru Верхняя граница диапазона.
                          \en Upper value of range. \~
        */
  MbValueRange( double value, double lower, double upper );

  /// \ru Конструктор копирования реализован по умолчанию. \en Copy constructor has default implementation. 
  MbValueRange( const MbValueRange & ) = default;

  /// \ru Оператор присваивания реализован по умолчанию. \en Assignment operators has default implementation. 
  MbValueRange( MbValueRange && ) = default;
  
  /** \brief \ru Определён ли номинал.
             \en Wether the value is defined. \~
       \param[out] value - \ru Номинал, если определён, иначе значение не меняется.
                           \en Value if defined, not changed otherwise. \~
       \return \ru Флаг, задан ли номинал.
               \en Flag wether the value is defined. \~ 
        */
  bool IsValueDefined( double & value ) const;


  /** \brief \ru Определён ли диапазон.
             \en Wether the range is defined. \~
       \param[out] lower - \ru Нижняя граница диапазона, иначе значение не меняется, 
                           \en Lower value of range, not changed otherwise, \~
       \param[out] upper - \ru Верхняя граница диапазона, иначе значение не меняется.
                           \en Upper value of range, not changed otherwise. \~
       \return \ru Флаг, задан ли диапазон.
               \en Flag wether the range is defined. \~ 
        */
  bool IsRangeDefined( double & lower, double & upper ) const;

  /** \brief \ru Задать номинал, сбросить диапазон.
             \en Set value, reset range. \~
       \param[in] valie - \ru Номинал. 
                          \en Value. \~
        */
  void Init( double value );

  /** \brief \ru Задать диапазон, сбросить номинал.
             \en Set range, reset value. \~
       \param[in] lower - \ru Нижняя граница диапазона,
                          \en Lower value of range, \~
       \param[in] upper - \ru Верхняя граница диапазона.
                          \en Upper value of range. \~
        */
  void Init( double lower, double upper );
  
  /** \brief \ru Задать номинал и диапазон.
             \en Set value and range. \~
       \param[in] value - \ru Номинал, 
                          \en Value, \~
       \param[in] lower - \ru Нижняя граница диапазона,
                          \en Lower value of range, \~
       \param[in] upper - \ru Верхняя граница диапазона.
                          \en Upper value of range. \~
        */
  void Init( double value, double lower, double upper );

  /// \ru Являются ли объекты равными. \en Are the objects equal. 
  bool IsSame( const MbValueRange & to, double accuracy ) const;


  DECLARE_PERSISTENT_CLASS( MbValueRange )
};

IMPL_PERSISTENT_OPS( MbValueRange )

//----------------------------------------------------------------------------------------
/** \brief \ru Отображаемый знак.
           \en Terminator.  \~
  \details \ru Предназначен для отображения специально выделенных точек на кривых PMI. \n
           \en Designed to dispay points on PMI curves with special meaning. \n\~
  \ingroup Legend
*/
// ---
class MATH_CLASS MbTerminator final : public MbRefItem, public TapeBase
{
  MbPlacement m_location;         ///< \ru Расположение терминатора в плоскости отображения. \en Location of terminator in the display plane.
  c3d::string_t m_terminatorType; ///< \ru Тип терминатора. \en Terminator type.
  double m_sizeX;                 ///< \ru Размер терминатора по координате x расположения. \en Size of the terminator in the x direction of the location.
  double m_sizeY;                 ///< \ru Размер терминатора по координате y расположения. \en Size of the terminator in the y direction of the location.

  /** \brief \ru Конструктор.
             \en Constructor. \~
       \param[in] location - \ru Расположение терминатора в плоскости отображения, 
                             \en Location of terminator in the display plane, \~
       \param[in] type -     \ru Тип терминатора.
                             \en Terminator type. \~
       \param[in] sizeX -    \ru Размер терминатора по координате x расположения.
                             \en Size of the terminator in the x direction of the location.. \~
       \param[in] sizeY -    \ru Размер терминатора по координате y расположения.
                             \en Size of the terminator in the y direction of the location.. \~
        */
  MbTerminator( const MbPlacement & location, const c3d::string_t & type, const double sizeX, const double sizeY );
public:
  /** \brief \ru Создать терминатор.
             \en Create terminator. \~
       \param[in] location - \ru Расположение терминатора в плоскости отображения, 
                             \en Location of terminator in the display plane, \~
       \param[in] type -     \ru Тип терминатора,
                             \en Terminator type, \~
       \param[in] sizeX -    \ru Размер терминатора по координате x расположения,
                             \en Size of the terminator in the x direction of the location, \~
       \param[in] sizeY -    \ru Размер терминатора по координате y расположения.
                             \en Size of the terminator in the y direction of the location. \~
       \return \ru Экземпляр терминатора, если строка типа не пуста и размеры больше Math::lengthEpsilon, иначе нулевой указатель.
               \en Instance of terminator, if the type string not empty and sizses greater than Math::lengthEpsilon, otherwise null pointer. \~ 
        */
  static SPtr<MbTerminator> Create( const MbPlacement & location, const c3d::string_t & type, const double sizeX, const double sizeY );

  /// \ru Получить положение терминатора. \en Get terminator's location. 
  MbPlacement GetLocation() const;

  /// \ru Получить тип терминатора. \en Get terminator's type. 
  c3d::string_t GetTerminatorType() const;

  /// \ru Получить ширину терминатора. \en Get terminator's width. 
  double GetWidth() const;

  /// \ru Получить высоту терминатора. \en Get terminator's height. 
  double GetHeight() const;

  /// \ru Инициализировтаь терминатор по исходному. \en Initialize terminator. 
  void Init( const MbTerminator& );

  /// \ru Являются ли объекты равными. \en Are the objects equal. 
  bool IsSame( const MbTerminator & to, double accuracy ) const;

  /// \ru Создать копию объекта. \en Create a copy of an object.
  SPtr<MbTerminator> Clone() const;

  // Посчитать сетку для терминатора
  static void CalculateMeshPmiTerminator( MbMesh & mesh, const c3d::string_t& termType , const MbPlacement3D & location, const double sizeX, const double sizeY );

  DECLARE_PERSISTENT_CLASS( MbTerminator )
};

IMPL_PERSISTENT_OPS( MbTerminator )


//----------------------------------------------------------------------------------------
/** \brief \ru Кривая с терминаторами.
           \en Curve with terminators.  \~
  \details \ru Может быть связана с характеризуемым объектом. \n
           \en Can have reference to the characterized object. \n\~
  \ingroup Legend
*/
// ---
class MATH_CLASS MbCalloutCurve final : public MbRefItem, public TapeBase
{
  SPtr<MbCurve> m_curve;                          ///< \ru Кривая-выноска. \en Callout curve.
  std::vector<SPtr<MbTerminator>> m_terminators;  ///< \ru Терминаторы. \en Termniators.
  SPtr<MbTopologyItem> m_characterizedObject;     ///< \ru Характеризуемый объект. \en Characterized object.
  MbeCalloutCurveType m_curveType;                ///< \ru Вид кривой. \en Callout type.
public:

  /** \brief \ru Конструктор.
             \en Constructor. \~
       \param[in] curveType -       \ru Вид кривой, 
                                    \en Callout type, \~
       \param[in] curveToObject -   \ru Кривая-выноска,
                                    \en Callout curve. \~
       \param[in] terminators -     \ru Терминаторы.
                                    \en Terminators, \~
       \param[in] objectCalloutTo - \ru Характеризуемый объект.
                                    \en Charactrrized object. \~
        */
  MbCalloutCurve( MbeCalloutCurveType curveType, SPtr<MbCurve> curveToObject, const std::vector<SPtr<MbTerminator>> & terminators, SPtr<MbTopologyItem> objectCalloutTo  );
  
  /** \brief \ru Получить кривую-выноску.
             \en Get callout curve. \~
       \return \ru Кривая-выноска.
               \en Callout curve. \~ 
        */
  SPtr<MbCurve> GetCurve() const;

  /** \brief \ru Получить количество терминаторов.
             \en Get count of terminators. \~
       \return \ru Количество терминаторов.
               \en Count of terminators. \~ 
        */
  size_t TerminatorsCount() const;

  /** \brief \ru Получить терминатор по индексу.
             \en Get terminator by index. \~
       \return \ru Терминатор по указанному индексу, если тот не превышает количество, иначе нулевой указатель.
               \en Terminator at the index if the index less than count, null pointer otherwise. \~ 
        */
  SPtr<MbTerminator> GetTerminator( size_t index ) const;

  /** \brief \ru Получить характеризуемые объект.
           \en Get characterized object. \~
     \return \ru Характеризуемый объект.
             \en Characterized object. \~
      */
  SPtr<MbTopologyItem> GetCharacterizedObject() const {  return m_characterizedObject;  }

  /** \brief \ru Задать характеризуемые объект.
           \en Set characterized object. \~
     \return \ru Характеризуемый объект.
             \en Characterized object. \~
      */
  void SetCharacterizedObject( SPtr<MbTopologyItem> obj );

/** \brief \ru Получить вид кривой-выноски.
             \en Get type of callout curve. \~
       \return \ru Вид кривой-выноски.
               \en Type of callout curve. \~ 
        */
  MbeCalloutCurveType GetCurveType() const;

#ifdef PMI_RAD_DIAM_USE_CALLOUT_INSTEAD_OF_DIM_PLUS_PROJ_CRV 
  bool ChangeDimensionCurveTypeToCallout();
#endif // PMI_RAD_DIAM_USE_CALLOUT_INSTEAD_OF_DIM_PLUS_PROJ_CRV 


  /// \ru Создать копию объекта. \en Create a copy of an object.
  SPtr<MbCalloutCurve> Clone( MbRegDuplicate * = nullptr ) const;
  
  /// \ru Являются ли объекты равными. \en Are the objects equal. 
  bool IsSame( const MbCalloutCurve & to, double accuracy ) const;

  DECLARE_PERSISTENT_CLASS( MbCalloutCurve )
};

IMPL_PERSISTENT_OPS( MbCalloutCurve )


//----------------------------------------------------------------------------------------
/** \brief \ru Численная характеристика объекта.
           \en Numerical characteristic of an object.  \~
  \details \ru Группа из более, чем одного текстового элемента. \n
           \en Group of two or more text items. \n\~
  \ingroup Legend
*/
// ---
class MATH_CLASS MbNumericalCharacteristic final : public MbRefItem, public TapeBase
{
  MbValueRange m_valueWithRange;                      ///< \ru Численные значения. \en Numerical values.
  MbeNumericCharacteristicType m_CharacteristicType;  ///< \ru Тип характеристики. \en Type of characteristics.
  std::vector<SPtr<MbCalloutCurve>> m_callouts;       ///< \ru Линии выноски. \en Callout lines.
  SPtr<MbCurve3D>       path;                         ///< \ru Кривая, вдоль которой проводится измерение. Если не задана, то размер есть кратчайший. \en A curve along which the measurement is performed. If not specified, then the size is shortest. 

  /** \brief \ru Конструктор.
             \en Constructor. \~
              \param[in] type -             \ru Тип характеристики, 
                                            \en Type of characteristics, \~
              \param[in] rangeValue -       \ru Численные значения,
                                            \en Numerical values, \~
              \param[in] callouts -         \ru Линии-выноски.
                                            \en Callout lines. \~
          */
  MbNumericalCharacteristic( MbeNumericCharacteristicType type, MbValueRange&&rangeValue, std::vector< SPtr<MbCalloutCurve> > && callouts );
public:

  
  /** \brief \ru Сформировать размер.
             \en Create annotation item. \~
              \param[in] dimensionType -    \ru Тип размера, 
                                            \en Type of dimension, \~
              \param[in] rangeValue -       \ru Численные значения,
                                            \en Numerical values, \~
              \param[in] callouts -         \ru Линии-выноски.
                                            \en Callout lines. \~
              \return \ru Численное значение линейного размера с объектами привязки.
                      \en Numerical value of a linear dimension with bind objects. \~
              \note \ru В контейнере выносных линий обязательно должна присутствовать размерная линия. Количество проекционных линий:
                  - В случае линейного, углового или диаметрального размера размера две или ни одной.
                  - В случае радиального размера ни одной.
                    \en . \~
          */
  static SPtr<MbNumericalCharacteristic> CreateDimension( MbeNumericCharacteristicType dimensionType, MbValueRange && rangeValue, std::vector<SPtr<MbCalloutCurve>> && callouts );

  /** \brief \ru Сформировать шероховатость поверхности.
             \en Create surface condition. \~
              \param[in] rangeValue -       \ru Численные значения,
                                            \en Numerical values, \~
              \param[in] callouts -         \ru Линии-выноски.
                                            \en Callout lines. \~
              \return \ru Численное значение шероховатости поверхности.
                      \en Numerical value of surface condition. \~
          */
  static SPtr<MbNumericalCharacteristic> CreateSurfaceRoughness( MbValueRange && rangeValue, std::vector<SPtr<MbCalloutCurve>> && callouts );


  /** \brief \ru Сформировать допуск формы.
             \en Create shape tolerance. \~
              \param[in] rangeValue -       \ru Численные значения,
                                            \en Numerical values, \~
              \param[in] callouts -         \ru Линии-выноски.
                                            \en Callout lines. \~
              \return \ru Численное значение линейного допуска формы.
                      \en Numerical value of shape tolerance. \~
          */
  static SPtr<MbNumericalCharacteristic> CreateShapeTolerance( MbValueRange && rangeValue, std::vector<SPtr<MbCalloutCurve>> && callouts );

  /** \brief \ru Сформировать носитель выносных линий.
             \en Create callouts holder. \~
              \param[in] callouts -         \ru Линии-выноски.
                                            \en Callout lines. \~
              \return \ru Численное значение типа "без характеристики".
                      \en Numerical value of the void type. \~
              \note \ru В контейнере выносных линий не должно быть размерных линий.
                    \en . \~
          */
  static SPtr<MbNumericalCharacteristic> CreateCallout( std::vector<SPtr<MbCalloutCurve>> && callouts );
  
  /// \ru Получить тип характеристики. \en Get the type of characteristic.
  MbeNumericCharacteristicType GetType() const;

  /// \ru Опредлено ли значение. \en Whether the value is defined.
  bool IsValueDefined( double & value ) const;

  /// \ru Опредлен ли диапазон. \en Whether the range is defined.
  bool IsRangeDefined( double & lower, double & upper ) const;

  /// \ru Получить количество выносных линий. \en Get the number of callout curves.
  size_t GetDecoratedCurvesCount() const;

  /// \ru Получить выносную линию по индексу. \en Get the callout curve by index.
  SPtr<MbCalloutCurve> GetDecoratedCurve( const size_t index ) const;

  /// \ru Получить характеризуемый объект выносной линии по индексу выносной линии. \en Get the characterized object of the callout curve by curve's index.
  SPtr<MbTopologyItem> GetCalloutCharacterizedObject( const size_t index ) const;

  /// \ru Получить геометрические объекты привязки. \en
  void GetCalloutCharacterizedObjects( std::vector<SPtr<MbTopologyItem>> & aTopo ) const;

  /// \ru Получить выносные линии размера, пристыкованные к геометрии. \en
  void GetCalloutCurvesProjectionCurves( std::vector<SPtr<MbCalloutCurve>> & aCrv ) const;

  /// \ru Получить размерную линию размера, имеющую стыковку с выносными линиями. \en 
  SPtr<MbCalloutCurve> GetCalloutCurvesDimensionCurve() const;

  /// \ru Получить обычные выносные линии аннотации. \en 
  void GetCalloutCurvesGeneral( std::vector<SPtr<MbCalloutCurve>> & aCrv ) const;

  /// \ru Являются ли объекты равными. \en Are the objects equal. 
  bool IsSame( const MbNumericalCharacteristic & to, double accuracy ) const;

  /// \ru Задать кривую, вдоль которой проводится измерение. \en Set the curve  the measurement is performed along. 
  void       SetPath( MbCurve3D * inPath );

  /// \ru Получить кривую, вдоль которой проводится измерение. \en Get the curve the measurement is performed along. 
  SPtr<MbCurve3D> GetPath() const;

  /// \ru Создать копию объекта. \en Create a copy of an object.
  SPtr<MbNumericalCharacteristic> Clone( MbRegDuplicate * = nullptr ) const;

  DECLARE_PERSISTENT_CLASS( MbNumericalCharacteristic )
};


IMPL_PERSISTENT_OPS( MbNumericalCharacteristic )


//----------------------------------------------------------------------------------------
/** \brief \ru Тип элемента аннотации.
           \en Type of PMI.  \~
  \details \ru Текст или специальный символ. \n
           \en Object counting number of its owners. \n\~
  \ingroup Legend
*/
// ---
enum class MbePMIType
{ 
  general                 = 0,  ///< \ru Общего вида. \en General type.
  numericalCharacteristic = 1,  ///< \ru Численная характеристика. \en Numerical characteristics.
  technicalRequiremets    = 2,  ///< \ru Технические требования. Предназначены для передачи преимущественно текста вне окна модели. \en Technical requirements. Should be used to store mainly text items not to be displaied in the model view.
  callout                 = 3,  ///< \ru Выносной элемент. \en Callout element.
  calloutMarking          = 4,  ///< \ru Выносной элемент - маркировка базы отсчета. \en Datum marking callout element.
  calloutDatum            = 5,  ///< \ru Выносной элемент - база. \en Datum callout element.
  calloutNote             = 6,  ///< \ru Выносной элемент - заметка. \en Note callout element.
  calloutCenterLine       = 7,  ///< \ru Выносной элемент - центральная линия. \en Center line callout element.
  calloutFeatureControlFrame = 8, ///< \ru Выносной элемент - рамка управления характеристиками. \en Feature control frame callout element.
  calloutReferencePoint   = 9   ///< \ru Выносной элемент - точка отсчёта. \en Reference point callout element.
};


//----------------------------------------------------------------------------------------
/** \brief \ru Элемент аннотации.
           \en Reference-counted object.  \~
  \ingroup Legend
*/
// ---
class MATH_CLASS MbPMI final : public MbLegend
{
  /// \ru Тип элемента аннотации. \en Type of PMI.
  MbePMIType                   m_PMIType;
  /// \ru Плоскость для отображения текста. \en The plane text is shown in.
  MbPlacement3D                 m_planeTextDisplay;
  /// \ru Заголовок элемента аннотации. \en The title of the annotation item.
  c3d::string_t                 m_title;
  /// \ru Элементы для непосредственного отображения. \en The elements to be displaied directly.
  c3d::ItemsSPtrVector          m_nonTextItems;
  /// \ru Текстовые элементы. \en The text items of the annotation.
  std::vector<SPtr<MbTextItem>> m_textItems;
  /// \ru Численная характеристика с элементами оформления. \en Numerical characteristics with visual apearance.
  SPtr<MbNumericalCharacteristic> m_numericalCharacteristics;
public:

/** \brief \ru Создать элемент аннотации общего вида.
           \en Create annotation item of general type. \~
              \param[in] plane -            \ru Плоскость для отображения плоских элементов,
                                            \en Plane for planar elements transformation into space, \~
              \param[in] pmiName -          \ru Название элемента аннотации,
                                            \en Captrion of the annotation element, \~
              \param[in] pmiVisual -        \ru Геометрические компоненты элемента аннотации,
                                            \en Geometric items ot the annotation element, \~
              \param[in] pmiText            \ru Текстовые компоненты элемента аннотации.
                                            \en Text items ot the annotation element. \~
              \return \ru Возвращает указатель на элемент аннотации, если передаётся хотя бы один
                         ненулевой текстовый или геометрический элемент, иначе нулевой указатель.
                      \en Returns pointer to new annotation element if only at least one text or geometric 
                         element is given, otherwise null pointer. \~
          */
  static SPtr<MbPMI> CreateGeneral ( const MbPlacement3D & plane   = MbPlacement3D::global,
                              const c3d::string_t & pmiName = c3d::string_t(),
                              const c3d::ItemsSPtrVector &  pmiVisual = c3d::ItemsSPtrVector(),
                              const std::vector<SPtr<MbTextItem>> & pmiText = std::vector<SPtr<MbTextItem>>() );

  /** \brief \ru Создать элемент аннотации вида "Технические требования".
           \en Create annotation item of the "Tehcnical requirements" type. \~
              \param[in] plane -            \ru Плоскость для отображения плоских элементов,
                                            \en Plane for planar elements transformation into space, \~
              \param[in] pmiName -          \ru Название элемента аннотации,
                                            \en Captrion of the annotation element, \~
              \param[in] pmiVisual -        \ru Геометрические компоненты элемента аннотации,
                                            \en Geometric items ot the annotation element, \~
              \param[in] pmiText            \ru Текстовые компоненты элемента аннотации.
                                            \en Text items ot the annotation element. \~
              \return \ru Возвращает указатель на элемент аннотации, если передаётся хотя бы один
                         ненулевой текстовый или геометрический элемент, иначе нулевой указатель.
                      \en Returns pointer to new annotation element if only at least one text or geometric 
                         element is given, otherwise null pointer. \~
          */
  static SPtr<MbPMI> CreateTechnicalRequirements ( const MbPlacement3D & plane   = MbPlacement3D::global,
                                                   const c3d::string_t & pmiName = c3d::string_t(),
                                                   const c3d::ItemsSPtrVector &  pmiVisual = c3d::ItemsSPtrVector(),
                                                   const std::vector<SPtr<MbTextItem>> & pmiText = std::vector<SPtr<MbTextItem>>() );  


  /** \brief \ru Создать элемент аннотации вида шероховатость поверхности.
             \en Create annotation item of the surface condition type. \~
              \param[in] rangeValue -       \ru Численные значения,
                                            \en Numerical values, \~
              \param[in] callouts -         \ru Линии-выноски.
                                            \en Callout lines. \~
              \return \ru Численное значение линейного размера с объектами привязки.
                      \en Numerical value of a linear dimension with bind objects. \~
              \param[in] plane -            \ru Плоскость для отображения плоских элементов,
                                            \en Plane for planar elements transformation into space, \~
              \param[in] pmiName -          \ru Название элемента аннотации,
                                            \en Captrion of the annotation element, \~
              \param[in] pmiVisual -        \ru Геометрические компоненты элемента аннотации,
                                            \en Geometric items ot the annotation element, \~
              \param[in] pmiText            \ru Текстовые компоненты элемента аннотации.
                                            \en Text items ot the annotation element. \~
              \return \ru Возвращает указатель на элемент аннотации, если передаётся хотя бы один
                         ненулевой текстовый или геометрический элемент, иначе нулевой указатель.
                      \en Returns pointer to new annotation element if only at least one text or geometric
                         element is given, otherwise null pointer. \~
                    \en . \~
          */
  static SPtr<MbPMI> CreateSurfaceRoughness( MbValueRange && rangeValue,
                                                                 std::vector<SPtr<MbCalloutCurve>> && callouts,
                                                                 const MbPlacement3D & plane = MbPlacement3D::global,
                                                                 const c3d::string_t & pmiName = c3d::string_t(),
                                                                 const c3d::ItemsSPtrVector & pmiVisual = c3d::ItemsSPtrVector(),
                                                                 const std::vector<SPtr<MbTextItem>> & pmiText = std::vector<SPtr<MbTextItem>>() );

  /** \brief  \ru Создать элемент аннотации вида допуск формы.
              \en Create annotation item of the shape tolerance type. \~
              \param[in] rangeValue -       \ru Численные значения,
                                            \en Numerical values, \~
              \param[in] callouts -         \ru Линии-выноски.
                                            \en Callout lines. \~
              \return \ru Численное значение линейного размера с объектами привязки.
                      \en Numerical value of a linear dimension with bind objects. \~
              \param[in] plane -            \ru Плоскость для отображения плоских элементов,
                                            \en Plane for planar elements transformation into space, \~
              \param[in] pmiName -          \ru Название элемента аннотации,
                                            \en Captrion of the annotation element, \~
              \param[in] pmiVisual -        \ru Геометрические компоненты элемента аннотации,
                                            \en Geometric items ot the annotation element, \~
              \param[in] pmiText            \ru Текстовые компоненты элемента аннотации.
                                            \en Text items ot the annotation element. \~
              \return \ru Возвращает указатель на элемент аннотации, если передаётся хотя бы один
                         ненулевой текстовый или геометрический элемент, иначе нулевой указатель.
                      \en Returns pointer to new annotation element if only at least one text or geometric
                         element is given, otherwise null pointer. \~
                    \en . \~
          */
  static SPtr<MbPMI> CreateShapeTolerance( MbValueRange && rangeValue,
                                                               std::vector<SPtr<MbCalloutCurve>> && callouts,
                                                               const MbPlacement3D & plane = MbPlacement3D::global,
                                                               const c3d::string_t & pmiName = c3d::string_t(),
                                                               const c3d::ItemsSPtrVector & pmiVisual = c3d::ItemsSPtrVector(),
                                                               const std::vector<SPtr<MbTextItem>> & pmiText = std::vector<SPtr<MbTextItem>>() );

  /** \brief \ru Создать элемент аннотации вида численная характеристика. 
           \en Create annotation item of the "Numerical Characteristic" type. \~           
              \param[in] numericalCharacteristics - \ru Численная характеристика,
                                                    \en Numerical characteristics, \~
              \param[in] plane -            \ru Плоскость для отображения плоских элементов,
                                            \en Plane for planar elements transformation into space, \~
              \param[in] pmiName -          \ru Название элемента аннотации,
                                            \en Captrion of the annotation element, \~
              \param[in] pmiVisual -        \ru Геометрические компоненты элемента аннотации,
                                            \en Geometric items ot the annotation element, \~
              \param[in] pmiText            \ru Текстовые компоненты элемента аннотации.
                                            \en Text items ot the annotation element. \~
              \return \ru Возвращает указатель на элемент аннотации, если передаётся хотя бы один
                         ненулевой текстовый или геометрический элемент, иначе нулевой указатель.
                      \en Returns pointer to new annotation element if only at least one text or geometric 
                         element is given, otherwise null pointer. \~
          */
  static SPtr<MbPMI> CreateNumericalCharacteristics ( SRef<MbNumericalCharacteristic> numericalCharacteristics,
                                                      const MbPlacement3D & plane   = MbPlacement3D::global,
                                                      const c3d::string_t & pmiName = c3d::string_t(),
                                                      const c3d::ItemsSPtrVector &  pmiVisual = c3d::ItemsSPtrVector(),
                                                      const std::vector<SPtr<MbTextItem>> & pmiText = std::vector<SPtr<MbTextItem>>() );

   /** \brief \ru Создать элемент вида "Выноска".
           \en Create annotation item of the "Callout" type. \~
              \param[in] calloutSubType -   \ru Уточнение типа выноски,
                                            \en Exact callout type, \~
              \param[in] calloutStorage -   \ru Хранилище выносных линий,
                                            \en Callout lines storage, \~
              \param[in] plane -            \ru Плоскость для отображения плоских элементов,
                                            \en Plane for planar elements transformation into space, \~
              \param[in] pmiName -          \ru Название элемента аннотации,
                                            \en Captrion of the annotation element, \~
              \param[in] pmiVisual -        \ru Геометрические компоненты элемента аннотации,
                                            \en Geometric items ot the annotation element, \~
              \param[in] pmiText            \ru Текстовые компоненты элемента аннотации.
                                            \en Text items ot the annotation element. \~
              \return \ru Возвращает указатель на элемент аннотации, если передаётся хотя бы один
                         ненулевой текстовый или геометрический элемент, иначе нулевой указатель.
                      \en Returns pointer to new annotation element if only at least one text or geometric 
                         element is given, otherwise null pointer. \~
          */
  static SPtr<MbPMI> CreateCallout ( MbePMIType calloutSubType,
                                     SRef<MbNumericalCharacteristic> calloutStorage,
                                     const MbPlacement3D & plane   = MbPlacement3D::global,
                                     const c3d::string_t & pmiName = c3d::string_t(),
                                     const c3d::ItemsSPtrVector &  pmiVisual = c3d::ItemsSPtrVector(),
                                     const std::vector<SPtr<MbTextItem>> & pmiText = std::vector<SPtr<MbTextItem>>() );   

  /** \brief \ru Создать выносные линии размера: 
                 одна размерная кривая типа MbeCalloutCurveType::dimensionCurve
                 и две проекционных кривых типа MbeCalloutCurveType::projectionCurve.
             \en Create callout curves:
                 one dimension curve of type MbeCalloutCurveType::dimensionCurve
                 and two projection curves of type MbeCalloutCurveType::projectionCurve. \~
             \param[inout] addTo -            \ru Хранилище выносных линий,
                                              \en Callout lines storage, \~
             \param[in] dimLocation -         \ru Плоскость для отображения плоских элементов,
                                              \en Plane for planar elements transformation into space,  \~
             \param[in] dimCurve -            \ru Кривая для построения размерной выносной линии,
                                              \en Curves for creation dimension callout curves, \~
             \param[in] projectionCurves -    \ru Кривые для построения проекционных выносных линий,
                                              \en Curves for creation projection callout curves, \~
             \param[in] projectionCurvesObj - \ru Характеризуемые объекты топологии, на которые ссылаются проекционные кривые,
                                              \en Characterized topology objects referenced by projection curves, \~
             \param[in] dimCurveTerminators - \ru Терминаторы размерной кривой.
                                              \en Dimension curve terminators. \~
             \return \ru Возвращает true в случае корректного создания выносных линий.
                     \en Returns true if callout curves are created correctly. \~
         */
  static bool CreateDimensionCalloutCurves( std::vector<SPtr<MbCalloutCurve>> & addTo,
                                            const MbPlacement3D dimLocation,
                                            const MbCurve3D * dimCurve,
                                            const std::vector<MbCurve3D *> & projectionCurves,
                                            const std::vector < SPtr<MbTopologyItem>> & projectionCurvesObj,
                                            const std::vector<SPtr<MbTerminator>> & dimCurveTerminators = std::vector<SPtr<MbTerminator>>() );
   
  /** \brief \ru Создать выносную линию типа MbeCalloutCurveType::callout.
             \en Create callout curve of type MbeCalloutCurveType::callout. \~
           \param[inout] addTo -            \ru Хранилище выносных линий,
                                            \en Callout lines storage, \~
           \param[in] location -            \ru Плоскость для отображения плоских элементов,
                                            \en Plane for planar elements transformation into space,  \~
           \param[in] calloutCurve -        \ru Кривая для построения выносной линии,
                                            \en Curves for creation callout curves, \~
           \param[in] obj -                 \ru Характеризуемый объект топологии,
                                            \en Characterized topology object, \~
           \param[in] terminators -         \ru Терминаторы выносной линии.
                                            \en Callout curve terminators. \~
           \return \ru Возвращает true в случае корректного создания выносной линии.
                   \en Returns true if callout curve is created correctly. \~
       */
  static bool CreateGeneralCalloutCurve( std::vector<SPtr<MbCalloutCurve>> & addTo,
                                         const MbPlacement3D location,
                                         const MbCurve3D * calloutCurve,
                                         const SPtr<MbTopologyItem> & obj,
                                         const std::vector<SPtr<MbTerminator>> & terminators = std::vector<SPtr<MbTerminator>>() );


  /** \brief \ru Создать терминатор выносной линии.
             \en Create terminator of callouc curve. \~
           \param[in] pmiLocation -         \ru Плоскость для отображения плоских элементов,
                                            \en Plane for planar elements transformation into space,  \~
           \param[in] dimensionCurve -      \ru Кривая выносной линии,
                                            \en Callout curve, \~
           \param[in] type -                \ru Тип терминатора,
                                            \en Termitanor type, \~
           \param[in] parameterOnCurve -    \ru  Значение параметра на размерной кривой. Если не указан, должен быть равен UNDEFINED_DBL,
                                            \en Parameter value on the dimensional curve. If not known, must be equal UNDEFINED_DBL, \~
           \param[in] sizeX -               \ru Размер терминатора по координате x расположения,
                                            \en Size of the terminator in the x direction of the location, \~
           \param[in] sizeY -               \ru Размер терминатора по координате y расположения,
                                            \en Size of the terminator in the y direction of the location, \~
           \param[in] sameDirection -       \ru Признак сонаправленности с касательной к кривой в точке размещения. 
                                                В случае неопределённого значения параметра - признак направленности внутрь.
                                            \en Flag of the same direction with the tangent to the curve at the location point. 
                                                In case parameter id undefined it shows if the arrow's direction is inner. \~
       */
  static SPtr<MbTerminator> CreateTerminator( const MbPlacement3D & pmiLocation, const MbCurve3D & dimensionCurve, const c3d::string_t & type,  double parameterOnCurve,
                                              const double sizeX, const double sizeY, const bool sameDirection );
protected:
/** \brief \ru Конструктор.
           \en Constructor. \~
              \param[in] plane -            \ru Плоскость для отображения плоских элементов,
                                            \en Plane for planar elements transformation into space, \~
              \param[in] pmiName -          \ru Название элемента аннотации,
                                            \en Captrion of the annotation element, \~
              \param[in] pmiVisual -        \ru Геометрические компоненты элемента аннотации,
                                            \en Geometric items ot the annotation element, \~
              \param[in] pmiText            \ru Текстовые компоненты элемента аннотации.
                                            \en Text items ot the annotation element. \~
          */
  MbPMI( MbePMIType pmiType, const MbPlacement3D & plane, const c3d::string_t & pmiName, SPtr<MbNumericalCharacteristic>, c3d::ItemsSPtrVector &&, std::vector<SPtr<MbTextItem>> && pmiText );

  /** \brief \ru Конструктор глубокого копирования.
           \en Deepcoly constructor. \~
              \param[in] init -             \ru Копируемый образец,
                                            \en Item to copy, \~
              \param[in] iReg -             \ru Регистратор дублей.
                                            \en Replica registry. \~
          */
  explicit MbPMI( const MbPMI & init, MbRegDuplicate * iReg = nullptr );
public:

  /// \ru Получить тип элемента аннотации. \en Get PMI type.
  MbePMIType GetPMIType() const;

  /// \ru Получить плоскость для отображения плоских элементов. \en Get plane for planar elements transformation into space.
  MbPlacement3D GetLocation() const;
  /// \ru Задать плоскость для отображения плоских элементов. \en Set plane for planar elements transformation into space.
  void SetLocation( const MbPlacement3D & loc );

  /// \ru Получить название элемента аннотации. \en Get captrion of the annotation element.
  c3d::string_t GetTitle() const;

  /// \ru Задать название элемента аннотации. \en Set captrion of the annotation element.
  void SetTitle( const c3d::string_t & t );

  /// \ru Получить количество геометрических элементов. \en Get count of geometric items.
  size_t GeometricElementsCount() const;

  /** \brief \ru Получить геометрический элемент с указанным индексом.
             \en Get geometric item at the specified index. \~
              \param[in] elementIndex -     \ru Индекс элемента,
                                            \en Element's index, \~
              \return \ru Элемент по указанному индексу, если индекс допустим, иначе нулевой указатель.
                      \en Element if index is valid, null pointer otherwise. \~
          */
  c3d::ItemSPtr GetGeometricElement( size_t elementIndex ) const;

  /// \ru Добавить геометрические элементы. \en Add geometric items.
  void AddGeometricElements( const c3d::ItemsSPtrVector & elems );

  /// \ru Получить количество текстовых элементов. \en Get count of text items.
  size_t TextElementsCount() const;

  /** \brief \ru Получить текстовый элемент с указанным индексом.
             \en Get text item at the specified index. \~
              \param[in] elementIndex -     \ru Индекс элемента,
                                            \en Element's index, \~
              \return \ru Элемент по указанному индексу, если индекс допустим, иначе нулевой указатель.
                      \en Element if index is valid, null pointer otherwise. \~
          */
  SPtr<MbTextItem> GetTextElement( size_t elementIndex ) const;

  /// \ru Задать текстовые элементы. \en Reset text items.
  bool ResetTextElemets( const std::vector<SPtr<MbTextItem>> & texts );

  /// \ru Задать геометрические элементы. \en Reset geometric elements.
  bool ResetGeometricElemets( const std::vector<c3d::ItemSPtr> & elems );

  /// \ru Получить текстовые элементы. \en Reset text items.
  void GetTextElements( std::vector<SPtr<MbTextItem>> & texts );

  /// \ru Задать численную характеристику. \en Set numerical characteristics.
  void SetNumericalCharacteristics( SRef<MbNumericalCharacteristic> numericahCharacteristics );

  /// \ru Получить численную характеристику. \en Get numerical characteristics.
  SPtr<MbNumericalCharacteristic> GetNumericalCharacteristics() const;

  /// \ru Заданы ли численные значения. \en Is numeric values defined.
  bool IsValueOrRangeDefined() const;

  /// \ru Это размер. \en Is it dimension.
  const bool IsDimension() const;

  /// \ru Преобразовать в элемент Технические Требования, если это возможно. \en Convert to Technical Requirements element if it is possible.
  bool ConvertToTechnicalRequirements();
  
  MbeSpaceType    IsA      () const final; // \ru Тип объекта. \en A type of an object.
  MbeSpaceType    Type     () const final; // \ru Групповой тип объекта. \en Group type of object. 
  MbSpaceItem &   Duplicate( MbRegDuplicate * = nullptr ) const final; // \ru Создать копию. \en Create a copy.
  bool            IsSame   ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const final; // \ru Являются ли объекты равными? \en Are the objects equal? 
  bool            SetEqual ( const MbSpaceItem & ) final; // \ru Сделать объекты равным. \en Make the objects equal. 
  
  void    Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) final; // \ru Преобразовать согласно матрице. \en Transform according to the matrix.
  void    Move     ( const MbVector3D &, MbRegTransform * = nullptr ) final; // \ru Сдвинуть вдоль вектора. \en Translate along a vector.
  void    Rotate   ( const MbAxis3D   &, double angle, MbRegTransform * = nullptr ) final; // \ru Повернуть вокруг оси. \en Rotate around an axis.
  bool    IsSimilar( const MbSpaceItem & init ) const final; // \ru Являются ли объекты подобными? \en Determine whether the objects are similar.
  double  DistanceToPoint ( const MbCartPoint3D & ) const final; // \ru Вычислить расстояние до точки. \en Calculate the distance to a point.
  void    AddYourGabaritTo( MbCube & r ) const final; // \ru Добавь свой габарит в куб. \en Add bounding box into a cube.
  
  void    CalculateMesh( const MbStepData & stepData, const MbFormNote & note, MbMesh & mesh ) const final; // \ru Построить полигональную копию mesh. \en Build polygonal copy mesh.

  MbProperty & CreateProperty( MbePrompt n ) const final;     // \ru Создать собственное свойство. \en Create a custom property.
  void    GetProperties( MbProperties & properties ) final; // \ru Выдать свойства объекта. \en Get properties of the object.
  void    SetProperties( const MbProperties & properties ) final; // \ru Установить свойства объекта. \en Set properties of the object.
  
  DECLARE_PERSISTENT_CLASS( MbPMI )
private:
  const MbPMI& operator=( const MbPMI & ) = delete;
};

IMPL_PERSISTENT_OPS( MbPMI )


/** \brief \ru Контейнер текстовых блоков.
\en Container of text blocks. \~
\ingroup Exchange_Base
*/
typedef std::vector< SPtr<MbTextItem> > vector_of_mbtext;

typedef SPtr<MbPMI> PmiSPtr;

/** \brief \ru Контейнер объектов аннотации.
\en Container of annotation objects. \~
\ingroup Exchange_Base
*/
typedef std::vector<PmiSPtr> vector_of_pmi;
typedef std::vector<PmiSPtr> PmiSptrVector;

#endif /* __MB_PMI_H */
