////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __IGES_BASIC_H
#define __IGES_BASIC_H

#include <math_define.h>

#include <string>

#define IGS_DOUBLE_TO_STRING_NDEC 15


//------------------------------------------------------------------------------
// типы IGS псевдо объектов
// ---
typedef enum   {
  igs_CopiousData11                   =  11,
  igs_MetalHatch                      =  31,
  igs_CeramicHatch                    =  32,
  igs_Hatch33                         =  33,
  igs_Hatch34                         =  34,
  igs_Hatch35                         =  35,
  igs_Hatch36                         =  36,
  igs_NonMetalHatch                   =  37,
  igs_BrickHatch                      =  38,
  igs_WitnesLine                      =  40,  

  igs_ItArcOrCircleIGES               = 100,
  igs_ItContourIGES                   = 102,
  igs_ItConicIGES                     = 104,
  igs_ItCopiousDataIGES               = 106,
  igs_ItPlaneIGES                     = 108,
  igs_ItLineSegIGES                   = 110,
  igs_ItSplineIGES                    = 112,
  igs_ItParametricSplineSurfaceIGES   = 114,
  igs_ItPointIGES                     = 116,
  igs_ItRuledSurfaceIGES              = 118,
  igs_ItSurfaceOfRevolutionIGES       = 120,
  igs_ItTabulatedCylinderIGES         = 122,
  igs_ItDirectionIGES                 = 123,
  igs_ItTransformMatrixIGES           = 124,
  igs_ItRationalBSplineCurveIGES      = 126,
  igs_ItRationalBSplineSurfaceIGES    = 128,
  igs_ItOffsetSurfaceIGES             = 140,
  igs_ItBoundaryIGES                  = 141,
  igs_ItCurveOnParametricSurfaceIGES  = 142,
  igs_ItBoundedSurfaceIGES            = 143,
  igs_ItTrimmedParametricSurfaceIGES  = 144,
  igs_ItManifoldSolidBRepIGES         = 186,
  igs_ItPlaneSurfaceIGES              = 190,
  igs_ItRCCylindricalSurfaceIGES      = 192,
  igs_ItRCConicalSurfaceIGES          = 194,
  igs_ItSphericalSurfaceIGES          = 196,
  igs_ItToroidalSurfaceIGES           = 198,

  igs_AngularDim                      = 202,
  igs_DiamDim                         = 206,
  igs_Text                            = 212,
  igs_Leader                          = 214,
  igs_LinDim                          = 216,
  igs_RadDim                          = 222,

  igs_ItSubfigureIGES                 = 308,
  igs_ItColorIGES                     = 314,
  igs_ItBlockIGES                     = 402,
  igs_ItPropertyIGES                  = 406,
  igs_ItSingularSubfigureInstanceIGES = 408,
  igs_ItExternalReferenceIGES         = 416,
  igs_ItVertexListIGES                = 502,
  igs_ItEdgeListIGES                  = 504,
  igs_ItLoopIGES                      = 508,
  igs_ItFaceIGES                      = 510,
  igs_ItShellIGES                     = 514,
  igs_Surface,
  igs_SpaceCurve,
} IGSGConverterType;


//-----------------------------------------------------------------------------
//
// ---
typedef enum  {          // секция
  UndefSection,
  FlagSection,           // not always present
  StartSection,
  GlobalSection,
  DirectoryEntrySection,
  ParametrDataSection,
  TerminateSection
} NameSectionIGES;

#define LENGTH_STRING_FILE 80
#define LENGTH_STRING_IGES 72

#define IGS_WIDTH                             1000   // число градаций толщины

#define LENGTH_ARRAY_IGES_SPEC_SYMBOLS          58
#define LENGTH_ARRAY_IGES_SPEC_SYMBOLS_GOST      6
//#define LENGTH_ARRAY_IGES_SPEC_SYMBOLS1001      30
#define LENGTH_ARRAY_IGES_SPEC_SYMBOLS1001      29
#define LENGTH_ARRAY_IGES_SPEC_SYMBOLS1002      34
#define LENGTH_ARRAY_IGES_SPEC_SYMBOLS1003      32
#define LENGTH_ARRAY_IGES_SPEC_SYMBOLS800        7


#define IGS_BLOCK_GROUP                          1   // +          форма блока 1 
#define IGS_BLOCK_GROUP_WO_BACK_POINTERS         7   // +          форма блока 7 - без обратных указателей

#define IGS_DOUBLE_UNDEF                     -1e32   //

#define ENTITY_LABEL_COUNT 8 ///< Длина поля entityLabel

#define VDE_VISIBLE_YES     0 ///< Объект видим
#define VDE_VISIBLE_NO      1 ///< Объект невидим

#define VDE_DEPEND_NO       0 ///< Объект независим
#define VDE_DEPEND_PHYS     1 ///< Объект зависим физически
#define VDE_DEPEND_LOG      2 ///< Объект зависим логически
#define VDE_DEPEND_BOTH     3 ///< Зависим физически и логически

#define VDE_USE_GEOMETRY    0 ///< Объект геометрический
#define VDE_USE_ANNOT       1 ///< Объект аннотационный
#define VDE_USE_DEF         2 ///< Объект определение
#define VDE_USE_OTHER       3 ///< Объект вне классификации
#define VDE_USE_LOG_POS     4 ///< Объект логический / позиционирование
#define VDE_USE_2D_PARAM    5 ///< Объект параметрический 2D
#define VDE_USE_CONSTR_GM   6 ///< Объект конструктивной геометрии

#define VDE_HIER_TOP_DOWN   0  ///< Сверху вниз
#define VDE_HIER_GLOB_DEFER 1  ///< Глобально
#define VDE_HIER_PROPERTY   2  ///< Свойство


//-------------------------------------------------------------------------------
// 
// ---
struct VectorDE{
  unsigned short visible           :  1;// 9 вектор состояния
  unsigned short depend            :  2;// 9
  unsigned short geometry          :  3;// 9
  unsigned short hierarchy         :  2;// 9
};


//-----------------------------------------------------------------------------
//
// ---
struct DirEntryParameter {
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// горячо рекомендую все изменения в структуре согласовывать с процедурой чтения
// ProcessingOneEntityDirEntrySection(), иначе есть очень реальный шанс все порушить,
// еще рекомендую крепко подумать, прежде чем делать виртуальные функции и наследников
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  int32     typeNumber;            // 1 номер типа
// поле   typeNumber должно идти первым - см. функцию Init этой структуры
// !!!!!!!!!!!!!!!!!!!!
  ptrdiff_t numbParDataString;     // 2 номер строки данных в секции Parametr Data
  ptrdiff_t structure;             // 3 инвертированный указатель на на строку в секции DirEntry обычно 0
  ptrdiff_t lineFontPattern;       // 4 номер стандартного типа линии или указатель на строку с описанием нестандартного
  ptrdiff_t level;                 // 5 номер слоя или  указатель на строку с описанием слоя в секции DirEntry
  ptrdiff_t view;                  // 6 указатель на строку с описанием вида в секции DirEntry
  ptrdiff_t transformMatrix;       // 7 указатель на строку с описанием матрицы преобразования в секции DirEntry
  ptrdiff_t labelDisplayAssociativ;// 8 указатель на строку с  ???? в секции DirEntry
  // 9 вектор состояния
  union {
    VectorDE       def;
    unsigned short vector;
  };
  ptrdiff_t numbString;            // 10 номер строки
  ptrdiff_t lineWeight;            // 12 - 11 пропущен  - градация толщины
  ptrdiff_t color;                 // 13 номер цвета или  указатель на строку с описанием цвета
  ptrdiff_t lineCount;             // 14 число строк под описание параметров в секции Parametr Data
  int32     formNumber;            // 15
  std::string entityLabel;         // 18 метка
   // SpAG K14 BUG_65022 - избавляемся от низкоуровневых абстракций
  ptrdiff_t numerLabel;            // 19 числовая метка

  DirEntryParameter( int32 type = 0 ): typeNumber( type ), numbParDataString( 0 ), structure( 0 ), lineFontPattern( 0 ), level( 0 ),
                                      view( 0 ), transformMatrix( 0 ), labelDisplayAssociativ( 0 ), numbString( 0 ), lineWeight( 0 ), 
                                      color( 0 ), lineCount( 0 ), formNumber( 0 ), entityLabel( ENTITY_LABEL_COUNT, ' ' ), numerLabel( 0 ) { Init(); } // SpAG K13 SP1 анализаторы кода

  void Init() {
    // SpAG - С точки зрения cppCheck должно быть гораздо более пристойно
    def.visible       = VDE_VISIBLE_YES;
    def.depend        = VDE_DEPEND_NO;
    def.geometry      = VDE_USE_DEF;
    def.hierarchy     = VDE_HIER_TOP_DOWN;
  }

  void operator = ( const DirEntryParameter & o ) {
    typeNumber = o.typeNumber;
    numbParDataString = o.numbParDataString;
    structure = o.structure;
    lineFontPattern = o.lineFontPattern;
    level = o.level;
    view = o.view;
    transformMatrix = o.transformMatrix;
    labelDisplayAssociativ = o.labelDisplayAssociativ;
    numbString = o.numbString; // BUG_71099
    lineWeight = o.lineWeight; 
    color = o.color;
    lineCount = o.lineCount;
    formNumber = o.formNumber;
    entityLabel = o.entityLabel; // SpAG K14 BUG_65022
    numerLabel = o.numerLabel;
    vector = o.vector;
  }

  void Zero() { 
    typeNumber = 0;
    numbParDataString = structure = lineFontPattern = level = view = transformMatrix = labelDisplayAssociativ = numbString = lineWeight = color = lineCount = numerLabel = 0;
    formNumber = 0;
    vector = 0;
    entityLabel.assign( ENTITY_LABEL_COUNT, ' ' );
  }

  bool operator == ( DirEntryParameter & o ) const {  return numbString == o.numbString; }
  bool operator <  ( DirEntryParameter & o ) const {  return numbString <  o.numbString; }
};


//-----------------------------------------------------------------------------
// Данные общей секции.
// ---
struct GlobalSectionParameter {
  int32       delimiter;            // ограничитель параметров 1
  int32       recordDelimiter;      // ограничитель записей 2
  std::string identifSendingSystem; // версия системы откуда 3
  std::string fileName;             // 4
  std::string systemID;             // идентификатор системы откуда 5
  std::string verPreProc;           // версия препроцессора( на хрен она упала?) 6
  int32       numbBitsOnInt;        // разрядность целого откуда 7
  int32       maxPowerFloat;        // максимальная степень float откуда 8
  int32       numbSignFloat;        // число значащих цифр float откуда 9
  int32       maxPowerDouble;       // максимальная степень double откуда 10
  int32       numbSignDouble;       // число значащих цифр double откуда 11
  std::string identifReceivngSystem;// версия системы куда 12
  double      scale;                // масштаб 13
  int32       unitFlag;             // 14  1- дюймы 2 -мм 3- 4 -футы 5 -мили 6 -м 7 -км 8 -милидюймы 9- мкм 10- см 11 - микродюйм
  std::string nameUnit;             // 15
  int32       maxNumberOfLineWeightGrad; // 16
  double      widthOfMaxLineWeight; // 17
  std::string dateAndTime;          // 18   YYMMMDD.HHNNSS
  double      minResolution;        // 19 - мин. разрешение системы
  double      maxAbsCoord;          // 20 максимальное координата по модулю
  std::string nameOfAuthor;         // 21 автор
  std::string authorsOrg;           // 22 организация
  int32       intVer;               // 23
  int32       intDraftStandart;     // 24
  std::string dateAndTimeMod;       // 25   YYMMMDD.HHNNSS

  GlobalSectionParameter () { Init(); }
  void Init() {
    delimiter                 = ',';      // ограничитель параметров 1
    recordDelimiter           = ';';      // ограничитель записей 2
    identifSendingSystem      = " ";      // версия системы откуда 3
    fileName                  = " ";      // 4
    systemID                  = " ";      // идентификатор системы откуда 5
    verPreProc                = " ";      // версия препроцессора( на хрен она упала?) 6
    numbBitsOnInt             = 32;       // разрядность целого откуда 7                       //-V112
    maxPowerFloat             = 38;       // максимальная степень float откуда 8
    numbSignFloat             = 6;        // число значащих цифр float откуда 9
    maxPowerDouble            = 307;      // максимальная степень double откуда 10
    numbSignDouble            = 15;       // число значащих цифр double откуда 11
    identifReceivngSystem     = " ";      // версия системы куда 12
    scale                     = 1.0;      // масштаб 13
    unitFlag                  = 2;        // 14  1- дюймы 2 -мм 3- 4 -футы 5 -мили 6 -м 7 -км 8 -милидюймы 9- мкм 10- см 11 - микродюйм
    nameUnit                  = "MM";     // 15
    maxNumberOfLineWeightGrad = IGS_WIDTH;// 16
    widthOfMaxLineWeight      = 1;        // 17
    dateAndTime               = "";       // 18   YYMMMDD.HHNNSS
    minResolution             = 0.001;    // 19 - мин. разрешение системы
    maxAbsCoord               = 10000;    // 20 максимальное координата по модулю
    nameOfAuthor              = " ";      // 21 автор
    authorsOrg                = " ";      // 22 организация
    intVer                    = 0/*3*/;        // 23
    intDraftStandart          = 0;        // 24
    dateAndTimeMod            = " ";      // 25
  }
};


//-------------------------------------------------------------------------------
// структура для формирования отчета о записи в IGES
// ---
struct ReportEntity {
  int32      kompasResNumHigh; // номер в ресурсе строки наименования того, что пришло из Компаса
  int32      kompasResNumBase; // номер в ресурсе строки наименования подложки, которая пришла из Компаса
  ptrdiff_t igesResNum;       // номер в ресурсе строки наименования того, чем записали в IGES
  ReportEntity( ptrdiff_t _igesResNum = 0 )
    : kompasResNumHigh( 0 )
    , kompasResNumBase( 0 )
    , igesResNum( _igesResNum )
  {}
};


//-------------------------------------------------------------------------------
/// Базовый класс для IGES объектов 
// ---
class CONV_CLASS BasicIGES {
public :
  ptrdiff_t    numStr;    ///< место хранения - номер строки в секции DE
  union {
    VectorDE       def;
    unsigned short vector;
  };
  ptrdiff_t    color;
  ptrdiff_t    level;
  ptrdiff_t    matrix;
  ptrdiff_t    form;
  ReportEntity report;

private:
  ptrdiff_t numType;   // номер типа

public :
  BasicIGES( ptrdiff_t _numType, ptrdiff_t _form  = 0 )
    : numStr (0)
    , vector (0)
    , color  (0)
    , level  (0)
    , matrix (0)
    , form   ( _form    )
    , report ( _numType )
    , numType( _numType )
  {}

  BasicIGES( const BasicIGES & o )
    : numStr ( o.numStr  )
    , vector ( o.vector  )
    , color  ( o.color   )
    , level  ( o.level   )
    , matrix ( o.matrix  )
    , form   ( o.form    )
    , report ( o.numType )
    , numType( o.numType )
  {}

  virtual ~BasicIGES() {}

          bool      Less( const BasicIGES & o ) const { return numType <  o.numType ? true : numType > o.numType ? false : form < o.form ? true : form > o.form ? false : matrix < o.matrix;}
          bool      Eq  ( const BasicIGES & o ) const { return numType == o.numType && form == o.form && matrix == o.matrix;}
  const   ptrdiff_t GetTypeIGES()               const { return numType; }
  const   ptrdiff_t GetFormIGES()               const { return form; }
  virtual bool operator == ( const BasicIGES & o ) const { return Eq( o ); }
  virtual bool operator <  ( const BasicIGES & o ) const { return Less( o ); }
};


#endif // __IGES_BASIC_H
