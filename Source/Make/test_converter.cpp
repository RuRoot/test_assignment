#include <optional>
#include <test_converter.h>
#include <test_manager.h>
#include <test_service.h>
#include <test_computation.h>
#include <test.h>
#include <test_variables.h>
#include <system_atomic.h>
#include <action_solid.h>
#include <action_shell.h>
#include <assembly.h>
#include <topology_iterator.h>
#include <instance_item.h>
#include <space_instance.h>
#include <plane_instance.h>
#include <reference_item.h>
#include <surface.h>
#include <solid.h>
#include <wire_frame.h>
#include <point_frame.h>
#include <cur_polyline.h>
#include <conv_model_exchange.h>
#include <conv_exchange_settings.h>
#include <conv_model_document.h>
#include <conv_requestor.h>
#include <attr_color.h>
#include <attr_common_attribute.h>
#include <check_geometry.h>
#include <alg_draw.h>
#include <list>
#include <sstream>
#include <stdio.h>
#include <last.h>
#include <fstream>

using namespace c3d;

//------------------------------------------------------------------------------
// \ru Раскомментируйте этот define, чтобы установить нужную локаль при экспорте
// \en Uncomment this define to specify locale for export
// ---

//#define SET_LOCALE_OBVIOUSLY

//------------------------------------------------------------------------------
// \ru Эта локаль бутед явно установлена при экспорте при определённом SET_LOCALE_OBVIOUSLY
//     Например: "russian"
// \en This locale will be used for export if SET_LOCALE_OBVIOUSLY is defined
//     Example: "russian"
// ---
#define SPECIFIC_LOCALE "C"



//------------------------------------------------------------------------------
// Деструктор
// Destructor
// ---
StrSpyCommon::~StrSpyCommon() {
}

//------------------------------------------------------------------------------
// Посетитель, извлекающий строку
// Visitor extracting the string
// ---
void StrSpyCommon::Visit( MbeProgBarId_Common & strId )
{
  switch ( strId ) {
    case pbarId_Read_Data     : Data().assign( IDS_PBAR_READ_DATA );    break;
    case pbarId_Prepare_Data  : Data().assign( IDS_PBAR_PREPARE_DATA ); break;
    case pbarId_Process_Data  : Data().assign( IDS_PBAR_PROCESS_DATA ); break;
    case pbarId_Finish_Data   : Data().assign( IDS_PBAR_FINISH_DATA );  break;
    case pbarId_Draw_Data     : Data().assign( IDS_PBAR_DRAW_DATA );    break;
    case pbarId_Write_Data    : Data().assign( IDS_PBAR_WRITE_DATA );   break;
    default: break;
  }
}


//------------------------------------------------------------------------------
// Деструктор
// Destructor
// ---
StrSpyPointsSurface::~StrSpyPointsSurface() {
}


//------------------------------------------------------------------------------
// Посетитель, извлекающий строку
// Visitor extracting the string
// ---
void StrSpyPointsSurface::Visit( MbeProgBarId_PointsSurface & strId )
{
  switch ( strId ) {
    case pbarId_Solve_LinearEquationsSystem : Data().assign(IDS_PBAR_SOLVE_LINEAR_EQUATIONS_SYSTEM); break;
    case pbarId_Remove_RedundantPoints      : Data().assign(IDS_PBAR_REMOVE_REDUNDANT_POINTS);       break;
    case pbarId_Build_ShellByPointsMesh     : Data().assign(IDS_PBAR_BUILD_SHELL_BY_POINTS_MESH);    break;
    case pbarId_Build_PointsCloudMesh       : Data().assign(IDS_PBAR_BUILD_POINTS_CLOUD_MESH);       break;
    case pbarId_Build_TriangleFaces         : Data().assign(IDS_PBAR_BUILD_TRIANGLE_FACES);          break;
    case pbarId_Find_AdjacentEdges          : Data().assign(IDS_PBAR_FIND_ADJACENT_EDGES);           break;
    case pbarId_Build_TrianglesShell        : Data().assign(IDS_PBAR_BUILD_TRIANGLES_SHELL);         break;
    case pbarId_Prepare_SurfaceData         : Data().assign(IDS_PBAR_PREPARE_SURFACE_DATA);          break;
    case pbarId_Check_Surface               : Data().assign(IDS_PBAR_CHECK_SURF);                    break;
    default: break;
  }
}


//------------------------------------------------------------------------------
// Деструктор
// Destructor
// ---
StrSpyMesh::~StrSpyMesh() {
}


//------------------------------------------------------------------------------
// Посетитель, извлекающий строку
// Visitor extracting the string
// ---
void StrSpyMesh::Visit( MbeProgBarId_Mesh & strId )
{
  switch ( strId ) {
  case pBar_SimplifyMesh_byTrianglesCount : Data().assign( IDS_PBAR_SIMPLIFY_TRIANGULATION_BY_TRIANGLES_COUNT ); break;
  case pBar_SimplifyMesh_byTolerance      : Data().assign( IDS_PBAR_SIMPLIFY_TRIANGULATION_BY_TOLERANCE );       break;
  case pBar_AlignObjects                  : Data().assign( IDS_PBAR_ALIGN_OBJECTS );                             break;
  case pBar_Remesh                        : Data().assign( IDS_PBAR_REMESH );                                    break;
  case pBar_Drap                          : Data().assign( IDS_PBAR_DRAP );                                      break;
  default: break;
  }
}


//------------------------------------------------------------------------------
// Деструктор
// Destructor
// ---
StrSpyConverters::~StrSpyConverters() {
}


//------------------------------------------------------------------------------
// Посетитель, извлекающий строку
// Visitor extracting the string
// ---
void StrSpyConverters::Visit( MbeProgBarId_Converters & strId )
{
  switch ( strId ) {
    case pbarId_Cnv_Parse_Data         : Data().assign(IDS_PBAR_CNV_PARSE_DATA);          break;
    case pbarId_Cnv_Create_Objects     : Data().assign(IDS_PBAR_CNV_CREATE_OBJECTS);      break;
    case pbarId_Cnv_Process_Surfaces   : Data().assign(IDS_PBAR_CNV_PROCESS_SURFACES);    break;
    case pbarId_Cnv_Process_Annotation : Data().assign(IDS_PBAR_CNV_PROCESS_ANNOTATION);  break;
    case pbarId_Cnv_Create_Model       : Data().assign(IDS_PBAR_CNV_CREATE_MODEL);        break;
    case pbarId_Cnv_Write_Model        : Data().assign(IDS_PBAR_CNV_WRITE_MODEL);         break;
    default: break;
  }
}


//------------------------------------------------------------------------------
// Деструктор
// Destructor
// ---
StrSpyTriangulation::~StrSpyTriangulation() {
}


//------------------------------------------------------------------------------
// Посетитель, извлекающий строку
// Visitor extracting the string
// ---
void StrSpyTriangulation::Visit( MbeProgBarId_Triangulation & strId )
{
  switch ( strId ) {
    case pbarId_Calc_Triangulation : Data().assign( IDS_PBAR_CALC_TRIANGULATION ); break;
    default: break;
  }
}


//------------------------------------------------------------------------------
// Деструктор
// Destructor
// ---
StrSpyMIP::~StrSpyMIP() {
}


//------------------------------------------------------------------------------
// Посетитель, извлекающий строку
// Visitor extracting the string
// ---
void StrSpyMIP::Visit( MbeProgBarId_MassInertiaProperties & strId )
{
  switch ( strId ) {
    case pbarId_Calc_MassInertiaProperties : Data().assign( IDS_PBAR_CALC_MASS_INERTIA_PROPERTIES ); break;
    default: break;
  }
}


//------------------------------------------------------------------------------
// Реализация получения строки
// Implementation of the string getter
// ---
class GetMsgImp {
protected:
  BaseStrVisitor * strSpy;
public:
  GetMsgImp() : strSpy( new BaseStrVisitor ) {}
  virtual ~GetMsgImp();
public:
  virtual const TCHAR * Msg( IStrData & ) const;
          void          SetStrSpy( BaseStrVisitor * );
};


//------------------------------------------------------------------------------
// Деструктор
// Destructor
// ---
GetMsgImp::~GetMsgImp()
{
  if ( strSpy != nullptr ) {
    delete strSpy;
    strSpy = nullptr;
  }
}


//------------------------------------------------------------------------------
// Получение строки
// Get message string
// ---
const TCHAR * GetMsgImp::Msg( IStrData & strData ) const {
  return strData.Accept( *strSpy ) ? strSpy->GetString() : nullptr;
}


//------------------------------------------------------------------------------
// Установка посетителя
// Set the visitor
// ---
void GetMsgImp::SetStrSpy( BaseStrVisitor * newStrSpy )
{
  if ( newStrSpy != nullptr && strSpy != newStrSpy ) {
    delete strSpy;
    strSpy = newStrSpy;
  }
}


//------------------------------------------------------------------------------
// Реализация индикатора прогресса выполнения
// Implementation of the process and feedback indicator
// ---
class ProgressIndicatorImp : public IProgressIndicator, public GetMsgImp, public MbSyncItem {
private:
  serial_type count;
  string_t    resMsg;
  string_t    curMsg;
  atomic_bool cancel;
public:
  ProgressIndicatorImp() : count( 0 ), resMsg(), curMsg(), cancel( false ) {}
  virtual ~ProgressIndicatorImp();

public:
  virtual bool          Initialize( size_t range, size_t delta, IStrData & msg ); // Установка диапазона индикации, сброс состояния
  virtual bool          Progress  ( size_t n );       // Обработать прогресс на n у.е. Вернет false - пора останавливаться
  virtual void          Success   ();                 // Ликвидация ошибок округления дорастим прогресс бар до 100%
  virtual bool          IsCancel  ();                 // Проверка не пора ли остановиться
  virtual void          SetCancel ( bool c );         // Скажем, что пора остановиться
  virtual void          Stop      ();                 // Команда пора остановиться
  virtual const TCHAR * Msg( IStrData & msg ) const;  // Получить строку

private:
  ProgressIndicatorImp( class ProgressIndicatorImp & );
  void operator =  ( class ProgressIndicatorImp & );
};


//------------------------------------------------------------------------------
// Деструктор
// Destructor
// ---
ProgressIndicatorImp::~ProgressIndicatorImp() {
}


//------------------------------------------------------------------------------
// Получить строку
// Get a string to indicate a state
// ---
const TCHAR * ProgressIndicatorImp::Msg( IStrData & strData ) const {
  return GetMsgImp::Msg( strData );
}

//------------------------------------------------------------------------------
// Послать строку в виде сообщения (потокобезопасная версия)
// Send a string as a message (thread-safe version)
// ---
void ShowStringSafe( const TCHAR * str )
{
  if ( !omp_in_parallel() || omp_get_thread_num() == 0 /*главный поток*/ )
    ::ShowString( str );
}

//------------------------------------------------------------------------------
// Установка диапазона индикации, сбросить состояние в 0
//
// Initialize range of the indicator and set it to the beginning
// ---
bool ProgressIndicatorImp::Initialize( size_t, size_t, IStrData & strData )
{
  Lock();
  count = 0;
  cancel = false;
  const TCHAR* rawMsg = Msg( strData );
  if ( nullptr != rawMsg )
    curMsg.assign( rawMsg );
  if ( curMsg.length() > 0 ) {
    TCHAR buff[20];
    resMsg = curMsg;
    resMsg += _T(" : ");
    _sntprintf( buff, 20, F_TD, count.load() );
    resMsg += string_t( buff );
  }
  Unlock();
  if ( curMsg.length() > 0 )
    ::ShowStringSafe( (const TCHAR *)resMsg.c_str() );
  return true;
}


//------------------------------------------------------------------------------
// Обработать прогресс на n единиц. Вернет false - пора останавливаться
//
// Indicate progress by n units. If the returned value is false, converter 
// will stop the process
// ---
bool ProgressIndicatorImp::Progress( size_t n )
{
  Lock();
  count += n;
  TCHAR buff[20];
  resMsg = curMsg;
  resMsg += _T(" : ");
  _sntprintf( buff, 20, F_TD, count.load() );
  resMsg += string_t( buff );
  Unlock();
  ::ShowStringSafe( (const TCHAR *)resMsg.c_str() );

  return !IsCancel();
}


//------------------------------------------------------------------------------
// Ликвидация ошибок округления дорастим прогресс бар до 100%
// Set the indicator to the "complete" state
// ---
void ProgressIndicatorImp::Success()
{
  Lock();
  count = 0;
  resMsg = curMsg;
  resMsg += string_t( RT_IS_FINISIED );
  Unlock();
  ::ShowStringSafe( (const TCHAR *)resMsg.c_str() );
}


//------------------------------------------------------------------------------
// Команда пора остановиться
// Здесь очистка строки состояния
// Set the indicator to the "stop" state
// ---
void ProgressIndicatorImp::Stop()
{
  if ( cancel )
    return;
  string_t emptyMsg;
  size_t strLen = resMsg.length();
  for ( size_t k = 0; k <= strLen; k++ )
    emptyMsg += _T(" ");

  ::ShowStringSafe( emptyMsg.c_str() );
}


//------------------------------------------------------------------------------
// Проверка не пора ли остановиться
// Is it necessary to cancel the operation
// ---
bool ProgressIndicatorImp::IsCancel()
{
  if ( !cancel ) {
    if ( PeekEscape() )
      SetCancel( true );
  }
  return cancel;
}


//------------------------------------------------------------------------------
// Скажем, что пора остановиться.
// Set the indicator to the "stop" state
// ---
void ProgressIndicatorImp::SetCancel( bool c ) {
  cancel = c;
}


//------------------------------------------------------------------------------
// Создать индикатор прогресса выполнения
// Create the progress indicatior
// ---
IProgressIndicator & CreateProgressIndicator( BaseStrVisitor * strSpy ) {
  ProgressIndicatorImp * progInd = new ProgressIndicatorImp;
  progInd->SetStrSpy( strSpy );
  return *progInd;
}


////////////////////////////////////////////////////////////////////////////////
//
// Интерфейс запроса масштаба
//
// Scale requestor
//
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
// Интерфейс запроса масштаба
// Scale requestor
// ---
class ScaleRequestorImp : public IScaleRequestor {
public:
  ScaleRequestorImp()  { }
  virtual double        ScaleRequest   () { return 1.0; }
};


////////////////////////////////////////////////////////////////////////////////
//
// WrConvertor3D
//
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//
// ---
class LogReporter {
  string_t fileName;  ///< \ru Имя файла. \en File name. \~
  t_ofstream errOut;  ///< \ru Рабочий поток. \en Stream to operate with. \~
  std::map< eMsgDetail, string_t > errorMessages; ///< \ru Строки сообщений трансляции. \en Translation mesages strings. \~

public:

  // Конструктор
  LogReporter( const string_t &ofn );
  ~LogReporter();
  bool InitErrOut();
  void LogReport( ptrdiff_t id, eMsgType msgType, eMsgDetail msgText );

  void LogReport( std::vector<ConvConvertorProperty3D::LogRecord> records );
};


//------------------------------------------------------------------------------
// Конструктор
// Constructor
// ---
LogReporter::LogReporter( const string_t &ofn ) 
  : fileName( ofn )
  , errOut()
  , errorMessages()
{
  // Общие сообщения
  // Common messages
  errorMessages[emd_Title] =                          RT_EMD_TITLE;
  errorMessages[emd_HEADError] =                      RT_EMD_HEAD_ERROR;
  errorMessages[emd_HEADWarinig] =                    RT_EMD_HEAD_WARNING;
  errorMessages[emd_HEADInfo] =                       RT_EMD_HEAD_INFO;
  errorMessages[emd_HEADDefaultMsg] =                 RT_EMD_HEAD_DEFAULT_MSG;

  // Прекращение импорта
  // Errors which cause immediate stop of the import process
  errorMessages[emd_STOPFileOpenError] =              RT_EMD_STOP_FILE_OPEN_ERROR;
  errorMessages[emd_STOPFileOpenErrorOrEmpty] =       RT_EMD_STOP_FILE_OPEN_ERROR_OR_EMPTY;
  errorMessages[emd_STOPHeaderReadError] =            RT_EMD_STOP_HEADER_READ_ERROR;
  errorMessages[emd_STOPNoOrBadData] =                RT_EMD_STOP_NO_OR_BAD_DATA;
  errorMessages[emd_STOPIncorrectStructure] =         RT_EMD_STOP_INCORRECT_STRUCTURE;
  errorMessages[emd_STOPAddressConflict] =            RT_EMD_STOP_ADDRESS_CONFLICT;

  // Общеконвертерные ошибки
  // Common errors which do not cause immediate stop of the import process
  errorMessages[emd_ErrorNoRootObject] =              RT_EMD_ERROR_NO_ROOT_OBJECT;
  errorMessages[emd_ErrorSyntaxIncorrectFormFloat] =  RT_EMD_ERROR_SYNTAX_INCORRECT_FORM_FLOAT;
  errorMessages[emd_ErrorEmptyLoop] =                 RT_EMD_ERROR_EMPTY_LOOP;
  errorMessages[emd_ErrorEmptyQueriesList] =          RT_EMD_ERROR_EMPTY_QUERIES_LIST;
  errorMessages[emd_ErrorEmptyObjectsList] =          RT_EMD_ERROR_EMPTY_OBJECTS_LIST;
  errorMessages[emd_ErrorEmptyGeomObjectsList] =      RT_EMD_ERROR_EMPTY_GEOM_OBJECTS_LIST;
  errorMessages[emd_ErrorEmptyShellsList] =           RT_EMD_ERROR_EMPTY_SHELLS_LIST;
  errorMessages[emd_ErrorEmptyCurveCompositesList] =  RT_EMD_ERROR_EMPTY_CURVE_COMPOSITIES_LIST;
  errorMessages[emd_ErrorEmptyBoundCurvesList] =      RT_EMD_ERROR_EMPTY_BOUND_CURVES_LIST;
  errorMessages[emd_ErrorEmptyEdgeList] =             RT_EMD_ERROR_EMPTY_EDGE_LIST;
  errorMessages[emd_ErrorEmptyFacesList] =            RT_EMD_ERROR_EMPTY_FACES_LIST;
  errorMessages[emd_ErrorEmptyReferencesList] =       RT_EMD_ERROR_EMPTY_REFERENCES_LIST;
  errorMessages[emd_ErrorEmptyListOfWrieframes] =     RT_EMD_ERROR_EMPTY_LIST_OF_WIREFRAMES;
  errorMessages[emd_ErrorEmptyOrMore2ReferencesList] =RT_EMD_ERROR_EMPTY_OR_MORE_2_REFERENCES_LIST;
  /*  errorMessages[emd_ErrorUndefinedRef] =              RT_EMD_ERROR_UNDIFINED_REF;*/
  errorMessages[emd_ErrorUndefinedFaceSurfaceRef] =   RT_EMD_ERROR_UNDIFINED_FACE_SURFACE_REF;
  errorMessages[emd_ErrorUndefinedBaseCurveRef] =     RT_EMD_ERROR_UNDIFINED_BASE_CURVE_REF;
  errorMessages[emd_ErrorRadiusTooCloseToZero] =      RT_EMD_ERROR_RADIUS_TOO_CLOSE_TO_ZERO;
  errorMessages[emd_ErrorRadiusValueNegative] =       RT_EMD_ERROR_RADIUS_VALUE_NEGATIVE;
  errorMessages[emd_ErrorEllipseAxisTooCloseToZero] = RT_EMD_ERROR_ELLIPSE_AXIS_TOO_CLOSE_TO_ZERO;
  errorMessages[emd_ErrorEllipseAxisNegative] =       RT_EMD_ERROR_ELLIPSE_AXIS_NEGATIVE;
  errorMessages[emd_ErrorNegativeDegree] =            RT_EMD_ERROR_NEGATIVE_DEGREE;
  errorMessages[emd_ErrorNegativeUDegree] =           RT_EMD_ERROR_NEGATIVE_U_DEGREE;
  errorMessages[emd_ErrorNegativeVDegree] =           RT_EMD_ERROR_NEGATIVE_V_DEGREE;
  errorMessages[emd_ErrorDegreeFixImpossible] =       RT_EMD_ERROR_DEGREE_FIX_IMPOSIBLE;
  errorMessages[emd_ErrorPolylinePointListLess2] =    RT_EMD_ERROR_POLYLINE_POINT_LIST_LESS_2;
  errorMessages[emd_ErrorPointListLess2] =            RT_EMD_ERROR_POINT_LIST_LESS_2;
  errorMessages[emd_ErrorKnotsListLess2] =            RT_EMD_ERROR_KNOTS_LIST_LESS_2;
  errorMessages[emd_ErrorWeightsListLess2] =          RT_EMD_ERROR_WEIGHTS_LIST_LESS2;
  errorMessages[emd_ErrorUPointListLess2] =           RT_EMD_ERROR_U_POINTS_LIST_LESS_2;
  errorMessages[emd_ErrorUKnotsListLess2] =           RT_EMD_ERROR_U_KNOTS_LEST_LESS_2;
  errorMessages[emd_ErrorUWeightsListLess2] =         RT_EMD_ERROR_U_WEIGTHS_LIST_LESS_2;
  errorMessages[emd_ErrorVPointListLess2] =           RT_EMD_ERROR_V_POINTS_LIST_LESS_2;
  errorMessages[emd_ErrorVKnotsListLess2] =           RT_EMD_ERROR_V_KNOTS_LEST_LESS_2;
  errorMessages[emd_ErrorVWeightsListLess2] =         RT_EMD_ERROR_V_WEIGTHS_LIST_LESS_2;
  errorMessages[emd_ErrorListsSizeMismatch] =         RT_EMD_ERROR_LISTS_SIZE_MISMATCH;
  errorMessages[emd_ErrorKnotsWeightsListsOrderMismatch] =               RT_EMD_ERROR_KNOTS_WEIGHTS_LISTS_ORDER_MISMATCH;
  errorMessages[emd_ErrorKnotsWeightsListsSizeMismatch] =                RT_EMD_ERROR_KNOTS_WEIGHTS_LISTS_SIZE_MISMATCH;
  errorMessages[emd_ErrorUKnotsWeightsListsSizeMismatch] =               RT_EMD_ERROR_U_KNOTS_WEIGHTS_LISTS_SIZE_MISMATCH;
  errorMessages[emd_ErrorVKnotsWeightsListsSizeMismatch] =               RT_EMD_ERROR_V_KNOTS_WEIGHTS_LISTS_SIZE_MISMATCH;
  errorMessages[emd_ErrorSplineCurveNotCreatedUndefinedKnotsVector] =    RT_EMD_ERROR_SPLINE_CURVE_NOTCREATED_UNDIFINED_KNOTS_VECTOR;
  errorMessages[emd_ErrorSplineSurfaceNotCreatedUndefinedKnotsVectors] = RT_EMD_ERROR_SPLINE_SURFACE_NOT_CREATED_UNDIFINED_KNOTSVECTORS;
  errorMessages[emd_ErrorInCorrectSplineSurfaceData] =                   RT_EMD_ERROR_INCORRECT_SPLINE_SURFACE_DATA;  

  // Общеконвертерные предупреждения
  // Common warning messages
  errorMessages[emd_WarningNoSectionTerminator] =             RT_EMD_WARNING_NO_SECTION_TERMINATOR;
  errorMessages[emd_WarningSyntaxMultipleDotInFloat] =        RT_EMD_WARNING_SYNTAX_MULTIPLE_DOT_IN_FLOAT;
  errorMessages[emd_WarningSyntaxMultipleEInFloat] =          RT_EMD_WARNING_SYNTAX_MULTIPLE_E_IN_FLOAT;
  errorMessages[emd_WarningLoopNotClosed] =                   RT_EMD_WARNING_LOOP_NOT_CLOSED;
  errorMessages[emd_WarningContourNotClosed] =                RT_EMD_WARNING_CONTOUR_NOT_CLOSED;  
  errorMessages[emd_WarningUndefinedRef] =                    RT_EMD_WARNING_UNDIFINED_REF;
  errorMessages[emd_WarningToroidalSurfaceDegenerated] =      RT_EMD_WARNING_TOROIDAL_SURFACE_DEGENERATED;
  errorMessages[emd_WarningUndefinedBasisCurve] =             RT_EMD_WARNING_UNDIFINED_BASIS_CURVE;
  errorMessages[emd_WarningUndefinedSweptCurve] =             RT_EMD_WARNING_UNDIFINED_SWEPT_CURVE;
  errorMessages[emd_WarningUndefinedExtrusionDirection] =     RT_EMD_WARNING_UNDIFINED_EXTURSION_DIRECTION;
  errorMessages[emd_WarningUndefinedAxis] =                   RT_EMD_WARNING_UNDIFINED_AXIS;
  errorMessages[emd_WarningUndefinedAxisOfRevolution] =       RT_EMD_WARNING_UNDIFINED_AXIS_OF_REVOLUTION;
  errorMessages[emd_WarningUndefinedBasisSurface] =           RT_EMD_WARNING_UNDIFINED_BASIS_SURFACE;
  errorMessages[emd_WarningUndefinedRepresentation] =         RT_EMD_WARNING_UNDIFINED_REPRESENTATIOV;
  errorMessages[emd_WarningUndefinedTransformationOperator] = RT_EMD_WARNING_UNDIFINED_TRANSFORMATION_OPERATOR;
  errorMessages[emd_WarningUndefinedObjectTransformBy] =      RT_EMD_WARNING_UNDIFINED_OBJECT_TRANSFORM_BY;
  errorMessages[emd_WarningUndefinedObjectToTransform] =      RT_EMD_WARNING_UNDIFINED_OBJECT_TO_TRANSFORM;
  errorMessages[emd_WarningUndefinedCurve] =                  RT_EMD_WARNING_UNDIFINED_CURVE;
  errorMessages[emd_WarningUndefinedCompositeSegment] =       RT_EMD_WARNING_UNDIFINED_COMPOSITE_SEGMENT;
  errorMessages[emd_WarningUndefinedDirection] =              RT_EMD_WARNING_UNDIFINED_DIRECTION;
  errorMessages[emd_WarningUndefinedAxisDirection] =          RT_EMD_WARNING_UNDIFINED_AXIS_DIRECTION;
  errorMessages[emd_MessageWeightsFilled] =                   RT_EMD_MESSAGE_WEIGHTS_FILLED;
  errorMessages[emd_WarningFloatParceFailureDefaultUsed] =    RT_EMD_MESSAGE_FLOAT_PARSE_FAILURE_REPLACE_DEFAULT;
  errorMessages[emd_WarningIncorrectFaceWasNotAddedToShell] = RT_EMD_WARNING_INCORRECT_FACE_WAS_NOT_ADDED_TO_SHELL;
  errorMessages[emd_WarningBoundsNotConnectedWithSeams] =     RT_EMD_WARNING_FACE_BOUNDS_NOT_CONNECTED_TO_SEAMS;
  errorMessages[emd_WarningSameShapeEdgeTwiceInLoop] =        RT_EMD_WARNING_SAME_EDGES_TWICE_IN_LOOP;
  errorMessages[emd_WarningVRMLGridDuplicatesInMeshes] =      RT_EMD_WARNING_VRML_GRID_DUPLICATES_IN_MESHES;
  errorMessages[emd_WarningACISLawIntCurveIsNotCreated] =     RT_EMD_WARNING_ACIS_LAW_INT_CURVE_IS_NOT_CREATED;
  errorMessages[emd_WarningIntCurveWasReplacedBySegment] =    RT_EMD_WARNING_INT_CURVE_WAS_REPLACED_BY_SEGMENT;
  errorMessages[emd_WarningCreateEdgeBadInput] =              RT_EMD_WARNING_INTERSECTION_CURVE_BAD_UNPUT;
  errorMessages[emd_WarningOpenSolidsNotSupported] =          RT_EMD_WARNING_OPEN_SOLIDS_NOT_SUPPORTED;
  errorMessages[emd_WarningOpenMeshesNotSupported] =          RT_EMD_WARNING_OPEN_MESHES_NOT_SUPPORTED;
  errorMessages[emd_WarningClosedSolidBecameOpenMesh] =       RT_EMD_WARNING_CLOSED_SOLID_BECAME_OPEN_MESH;
  errorMessages[emd_WarningEmptyMeshSkipped] =                RT_EMD_WARNING_EMPTY_MESH_SKIPPED;
  errorMessages[emd_WarningTriangulationFailed] =             RT_EMD_WARNING_TRIANGULATION_FAILED;

  // Сообщения конвертера STEP
  // Common info messages
  errorMessages[emd_ErrorEdgeCurveFlagTSingleRedefinition] =  RT_EMD_ERROR_EDGE_CUVE_FLAG_T_SINGLE_REDEFINITION;
  errorMessages[emd_ErrorEdgeCurveFlagFSingleRedefinition] =  RT_EMD_ERROR_EDGE_CUVE_FLAG_F_SINGLE_REDEFINITION;
  errorMessages[emd_ErrorSTEPEdgeCurveFlagTMultipleRedefinition] =RT_EMD_ERROR_STEP_EDGE_CUVE_FLAG_T_MULTIPLE_REDEFINITION;
  errorMessages[emd_ErrorSTEPEdgeCurveFlagFMultipleRedefinition] =RT_EMD_ERROR_STEP_EDGE_CUVE_FLAG_F_MULTIPLE_REDEFINITION;
  errorMessages[emd_ErrorSTEPUndefinedFaceGeometry] =             RT_EMD_ERROR_STEPUNDIFINED_FACE_GEOMETRY;
  errorMessages[emd_ErrorSTEPSyntaxMultipleDotInEnum] =           RT_EMD_ERROR_STEP_SYNTAX_MULTIPLE_DOT_IN_ENUM;
  errorMessages[emd_WarningSTEPPointCorrection] =                 RT_EMD_WARNING_STEP_POINT_CORRECTION;
  errorMessages[emd_WarningSTEPEdgeCurveByVertices] =             RT_EMD_WARNING_STEP_EDGE_CERVE_BY_VERTICES;
  errorMessages[emd_MessageSTEPFlagChangedToF] =                  RT_EMD_MESSAGE_STEP_FLAG_CHANGED_TO_F;
  errorMessages[emd_MessageSTEPFlagChangedToT] =                  RT_EMD_MESSAGE_STEP_FLAG_CHANGED_TO_T;

  // Сообщения конвертера SAT
  // SAT converter messages
  errorMessages[emd_WarningACISUnsupportedInterpoleCurveType] =   RT_EMD_WARNING_ACIS_UNSUPPORTED_INTERPOLE_CURVE_TYPE;
  errorMessages[emd_WarningACISUnsupportedParametricCurveType] =  RT_EMD_WARNING_ACIS_UNSUPPORTED_PARAMETRIC_CURVE_TYPE;
  errorMessages[emd_ErrorACISUnsupportedVersion] =                RT_EMD_ERROR_ACIS_UNSUPPORTED_VERSION;
  errorMessages[emd_WarningACISCannotImportEntityId] =            RT_EMD_WARNING_ACIS_CANNOT_IMPORT_ENTITY_ID;
  errorMessages[emd_WarningACISIncorrectIntAttribute] =           RT_EMD_WARNING_ACIS_INCORRECT_INT_ATTRIBUTE;
  
  // Сообщения конвертера IGES
  // IGES converter messages
  errorMessages[ emd_ErrorIGESIncorrectExternalReference ] =      RT_EMD_ERROR_IGES_INCORRECT_EXTERNAL_REFERENCE;

  // Сообщения конвертера XT
  // XT converter messages
  errorMessages[ emd_ErrorXTUnsupportedVersion ] =                RT_EMD_ERROR_XT_UNSUPPORTED_VERSION;

  // Сообщения конвертера JT
  // JT converter messages
  errorMessages[emd_ErrorJTDuplicatedTriangles] =                 RT_EMD_ERROR_JT_DUPLICATED_TRIANGLES;

  // Сообщения подсистемы работы с плагинами
  // Converter plugin messages
  errorMessages[emd_PluginGeneralProblem] =                       RT_EMD_ERROR_PLUGIN_GENERAL_PROBLEM;
  errorMessages[emd_PluginLicenceProblem] =                       RT_EMD_ERROR_PLUGIN_ACTIVATION_PROBLEM;
  errorMessages[emd_PluginFunctionalityLimitation] =              RT_EMD_ERROR_PLUGIN_LIMITATION;
  errorMessages[emd_PluginImportPartialSuccess] =                 RT_EMD_WARNING_PLUGIN_PARTIAL_IMPORT;
  errorMessages[emd_ErrorModelHasNoGeometry] =                    RT_EMD_ERROR_MODEL_HAS_NO_GEOMETRY;
  errorMessages[emd_ErrorReferencedModelFileNotFound] =           RT_EMD_ERROR_REFERENCED_MODEL_FILE_NOT_FOUND;

  errorMessages[emd_InfoMetadataSuccessfullyImported] =           RT_EMD_INFO_METADATA_SUCCESSFULLY_IMPORTED;
  errorMessages[emd_WarningMetadataPartiallyImported] =           RT_EMD_WARNING_METADATA_PARTIALLY_IMPORTED;
  errorMessages[emd_InfoMetadataNotSupported] =                   RT_EMD_INFO_METADATA_NOT_SUPPORTED;
  errorMessages[emd_InfoModelContainsNoMetadata] =                RT_EMD_INFO_MODEL_CONTAINS_NO_METADATA;
  errorMessages[emd_ErrorInvalidTargetDirectoryForMetadata] =     RT_EMD_ERROR_INVALID_TARGET_DIRECTORY_FOR_METADATA;
  errorMessages[emd_ErrorMetadataFileWriteFailure] =              RT_EMD_ERROR_METADATA_FILE_WRITE_FAILURE;
  errorMessages[emd_ErrorMetadataImportError] =                   RT_EMD_ERROR_METADATA_IMPORT_ERROR;

}

//------------------------------------------------------------------------------
// Деструктор
// Destructor
// ---
LogReporter::~LogReporter() {
  try {
    errOut.close();
  } catch (std::exception const &) {
    std::cerr << "Failed to write messages to log file" << std::endl;
  }
}


//------------------------------------------------------------------------------
// Инициализировать поток сообщений
// Initialise stream of the log
// ---
bool LogReporter::InitErrOut() {
  if ( !errOut.good() )
    return false;

  if ( errOut.rdbuf()->is_open() )
    return true;

  //OV_Lnx errOut.imbue( std::locale( "Russian" ) );
  //OV_Lnx std::wstring errName ( GetFileName().get_wcs() ), fName( errName );

  string_t errName ( fileName ), fName( errName );

  if ( errName.length() == 0 )
    return false;

  ptrdiff_t pointPos = errName.find_last_of( _T(".") );
  errName.erase( errName.begin()+pointPos, errName.end() );
  errName += _T(".log");

  errOut.open( c3d::C3DToPathstring(errName).c_str(), std::ios::out ); // Открыть файл на запись

  if ( !errOut.good() )
    return false;

  errOut << std::endl << std::endl;
  errOut << errorMessages[emd_Title].c_str() << fName.c_str() << _T(".");
  errOut << std::endl << std::endl;

  return errOut.good();
}


//------------------------------------------------------------------------------
// Сделать запись в журнал
// Log the message
// ---
void LogReporter::LogReport( ptrdiff_t id, eMsgType msgType, eMsgDetail msgText )
{
  if ( !!InitErrOut() ) {
    switch ( msgType ) {
    case emt_ErrorNoId:
      errOut << errorMessages[emd_HEADError].c_str();
      break;
    case emt_TextOnly:
      ;
      break;
    case emt_Info:
      errOut << _T("#") << id << _T(" ") << errorMessages[emd_HEADInfo].c_str();
      break;
    case emt_Warning:
      errOut << _T("#") << id << _T(" ") << errorMessages[emd_HEADWarinig].c_str();
      break;
    case emt_Error:
      errOut << _T("#") << id << _T(" ") << errorMessages[emd_HEADError].c_str();
      break;
    default:  // на случай внесения дополнительных изменений в enum;
      // In case of the enum extension
      errOut << _T("#") << id << _T(" ") << errorMessages[emd_HEADDefaultMsg].c_str();
    }
    errOut << _T("") << errorMessages[msgText].c_str() << _T(".") << std::endl;
  }
}


//------------------------------------------------------------------------------
// Сделать запись в журнал
// Log all the messages
// ---
void LogReporter::LogReport( std::vector<ConvConvertorProperty3D::LogRecord> records ) {
  for( size_t i = 0, sz = records.size(); i < sz; i++ )
    LogReport( records[i].id, records[i].msgType, records[i].msgText );
}


////////////////////////////////////////////////////////////////////////////////
//
//                 Методы общения конвертеров и менеджера.
//
//                      Model management utilities
//
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
// Передача конвертированной модели менеджеру.
// Transfer the converted model to manager.
// ---
void AddContentToManager( MbItem& content, bool adjustView )
{
  //if ( /*replaceItems*/true )
  TestVariables::viewManager->DeleteModel();

  if ( adjustView ) {
    MbCube gab;
    content.AddYourGabaritTo( gab );
    // подстроить активное окно и стрелку прогиба для размеров объекта.
    // Adjust the scale of the model to display the entire model
    if ( !gab.IsEmpty() )
      TestVariables::viewManager->SetWindowScale( gab );
  }
  const MbVisual * colorAttr = static_cast<const MbVisual *>( content.GetSimpleAttribute( at_Visual ) );
  if ( colorAttr != nullptr )
    TestVariables::viewManager->AddItem( content );  // C3D-14412
  else
    TestVariables::viewManager->AddItem( content, Style(1, MB_C3DCOLOR) );
}


//------------------------------------------------------------------------------
// Сформировать контент на основе модели
// Build the model for export
// ---
MbItem* ModelToContent( MbModel& model ) {
  MbItem * content = nullptr;
  RPArray<MbItem> objs( 2, 1 );
  model.GetItems( objs );
  size_t objSz = objs.Count();
  if ( objSz > 0 ) {
    if ( objSz > 1 ) {
      std::vector< MbItem* > itemsOfContent;
      for( size_t  i = 0; i < objSz; i++ ) {
        MbItem* curIt = objs[i];
        if ( curIt == nullptr )
          continue;
        if ( curIt->IsA() == st_Instance ) {
          MbInstance& curInst = static_cast<MbInstance&>( *curIt );
          if ( curInst.GetItem() != nullptr ) {
            MbInstance& asInst = *( new MbInstance( *curInst.SetItem(), curInst.GetPlacement() ) );// static_cast<MbInstance&>( curIt->Duplicate() );
            curIt->AttributesCopy( asInst );
            itemsOfContent.push_back( &asInst );
          }
        } else
          //itemsOfContent.push_back( new MbInstance( *curIt, MbPlacement3D::global ) );
          itemsOfContent.push_back( curIt );
      }
      content = new MbAssembly( itemsOfContent );
    }
    else {
      content = objs[0];
    }
  }
  if ( content != nullptr )
    model.AttributesCopy( *content );

  return content;
}


//------------------------------------------------------------------------------
// Распознать булево значение и обновить в соответствие с нотацией
// ---
bool UpdateBool( const c3d::string_t& key, bool& value )
{
  const c3d::string_t trueBoolean( CONFIG_CONVERTER_DEBUG_SETTING_TRUE );
  const c3d::string_t falseBoolean( CONFIG_CONVERTER_DEBUG_SETTING_FALSE );
  bool result(false);
  if ( trueBoolean == key )
  {
    value = true;
    result = true;
  }
  else if ( falseBoolean == key )
  {
    value = false;
    result = true;
  }
  return  result;
}

//------------------------------------------------------------------------------
// Получить пользовательские настройки конвертера
// Файл c3d_test.config должен лежать рядом с файлом лицензии.
// Возвращает true, если хотя бы одна опция была задана.
// ---
bool UpdateConverterSettings( ConvConvertorProperty3D & convProperties)
{
  bool result (false);
  bool filter = false;
  const c3d::string_t repairMeshSettingPrefix( CONFIG_CONV_SETTINGS_REPAIR_MESH );
  const c3d::string_t repairMeshEnable( CONFIG_CONV_SETTINGS_REPAIR_MESH_ENABLE );
  const c3d::string_t orientOutward( CONFIG_CONV_SETTINGS_REPAIR_MESH_ORIENT_OUTWARD );
  const c3d::string_t toleranceRM( CONFIG_CONV_SETTINGS_REPAIR_MESH_TOLERANCE );
  const c3d::string_t angleSharp( CONFIG_CONV_SETTINGS_REPAIR_MESH_ANGLE_SHARP );
  const c3d::string_t normalAveraging( CONFIG_CONV_SETTINGS_REPAIR_MESH_NORMAL_AVERAGING );

  const c3d::string_t debugSettingPrefix( CONFIG_CONV_SETTINGS_DEBUG );
  const c3d::string_t enableCERRout( CONFIG_CONVERTER_DEBUG_SETTING_ENABLE_CERR_OUT );
  const c3d::string_t cerrOutUserTreeTraverse( CONFIG_CONVERTER_DEBUG_SETTING_USER_TREE );
  const c3d::string_t cerrOutIntermediateTreeTraverse( CONFIG_CONVERTER_DEBUG_SETTING_IM_TREE );
  const c3d::string_t cerrOutImportStatistic( CONFIG_CONVERTER_DEBUG_SETTING_OUT_STATISTICS );
  const c3d::string_t saveModelTwin( CONFIG_CONVERTER_DEBUG_SETTING_SAVE_TWIN );
  const c3d::string_t elementIdFineDebug( CONFIG_CONVERTER_DEBUG_SETTING_ID_FINE_DEBUG );
  const c3d::string_t attachHitsIdAttr( CONFIG_CONVERTER_DEBUG_SETTING_ATTHACH_THISID_ATTRIBUTE );
  const c3d::string_t pathFineDebug( CONFIG_CONVERTER_DEBUG_SETTING_OUT_FINE_DEBUG );
  const c3d::string_t loggingMode( CONFIG_CONVERTER_DEBUG_SETTING_LOGGING_MODE );

  const c3d::string_t cnvSettingPrefix( CONFIG_CONV_SETTINGS );
  const c3d::string_t stitchOn( CONFIG_CONVERTER_SETTING_STITCH_ON );
  const c3d::string_t stitchPrec( CONFIG_CONVERTER_SETTING_STITCH_PRECISION );
  const c3d::string_t joinSimilar( CONFIG_CONVERTER_SETTING_JOIN_SIMILAR_FACES );
  const c3d::string_t configToRead( CONFIG_CONVERTER_SETTING_CONFIGURATION_TO_READ );
  const c3d::string_t visualPropertiesStrategy( CONFIG_CONVERTER_SETTING_VISUAL_PROPERTIES_STRATEGY );
  const c3d::string_t importMeshesWithTopology( CONFIG_CONVERTER_SETTING_IMPORT_MESHES_WITH_TOPOLOGY );
  const c3d::string_t importInstanceAttributes( CONFIG_CONVERTER_SETTING_IMPORT_INSTANCE_ATTRIBUTES );

  const c3d::string_t filterSolid( CONFIG_FILTER_SOLID );
  const c3d::string_t filterSurface( CONFIG_FILTER_SURFACE );
  const c3d::string_t filterCurve( CONFIG_FILTER_CURVE );
  const c3d::string_t filterPoint( CONFIG_FILTER_POINT );
  const c3d::string_t filterPolygonal( CONFIG_FILTER_POLYGONAL );
  const c3d::string_t filterBrep( CONFIG_FILTER_BREP);
  const c3d::string_t filterVisual( CONFIG_FILTER_VISUAL );
  const c3d::string_t filterAttribute( CONFIG_FILTER_ATTRIBUTE );
  const c3d::string_t filterPMI( CONFIG_FILTER_PMI );
  const c3d::string_t filterDensity( CONFIG_FILTER_DENSITY );
  const c3d::string_t filterValidation( CONFIG_FILTER_VALIDATION );

  for ( const std::pair<c3d::string_t, c3d::string_t>& curSettingPair :  ::TestVariables::userProp )
  {
    const bool isDebugSetting{ debugSettingPrefix == curSettingPair.first.substr( 0, debugSettingPrefix.length() ) };
    const bool isRepairMeshSetting{ repairMeshSettingPrefix == curSettingPair.first.substr( 0, repairMeshSettingPrefix.length() ) };
    if ( !isDebugSetting && !isRepairMeshSetting )
    {
      if ( cnvSettingPrefix != curSettingPair.first.substr( 0, cnvSettingPrefix.length() ) )
        continue;
      const string_t debugSettingKey( curSettingPair.first.substr( cnvSettingPrefix.length() ) );
      if ( debugSettingKey == stitchOn )
        result = UpdateBool( curSettingPair.second, convProperties.enableAutostitch ) || result;
      else if ( debugSettingKey == joinSimilar )
        result = UpdateBool( curSettingPair.second, convProperties.joinSimilarFaces ) || result;
      else if ( debugSettingKey == stitchPrec )
        result = sscanf( c3d::ToSTDstring( curSettingPair.second ).c_str(), "%lf", &convProperties.autostitchPrecision ) == 1 || result;
      else if ( debugSettingKey == filterSolid )
      {
        result = UpdateBool( curSettingPair.second, filter ) || result;
        convProperties.SetIoPermission( iop_rSolid, filter );
        convProperties.SetIoPermission( iop_wSolid, filter );
      }
      else if ( debugSettingKey == filterSurface )
      {
        result = UpdateBool( curSettingPair.second, filter ) || result;
        convProperties.SetIoPermission( iop_rSurface, filter );
        convProperties.SetIoPermission( iop_wSurface, filter );
      }
      else if ( debugSettingKey == filterCurve )
      {
        result = UpdateBool( curSettingPair.second, filter ) || result;
        convProperties.SetIoPermission( iop_rCurve, filter );
        convProperties.SetIoPermission( iop_wCurve, filter );
      }
      else if ( debugSettingKey == filterPoint )
      {
        result = UpdateBool( curSettingPair.second, filter ) || result;
        convProperties.SetIoPermission( iop_rPoint, filter );
        convProperties.SetIoPermission( iop_wPoint, filter );
      }
      else if ( debugSettingKey == filterPolygonal )
      {
        result = UpdateBool( curSettingPair.second, filter ) || result;
        convProperties.SetIoPermission( iop_rPolygonal, filter );
        convProperties.SetIoPermission( iop_wPolygonal, filter );
      }
      else if ( debugSettingKey == filterBrep )
      {
        result = UpdateBool( curSettingPair.second, filter ) || result;
        convProperties.SetIoPermission( iop_rBRep, filter );
        convProperties.SetIoPermission( iop_wBRep, filter );
      }
      else if ( debugSettingKey == filterVisual )
      {
        result = UpdateBool( curSettingPair.second, filter ) || result;
        convProperties.SetIoPermission( iop_rStyle, filter );
        convProperties.SetIoPermission( iop_wStyle, filter );
      }
      else if ( debugSettingKey == filterAttribute )
      {
        result = UpdateBool( curSettingPair.second, filter ) || result;
        convProperties.SetIoPermission( iop_rAttributes, filter );
        convProperties.SetIoPermission( iop_wAttributes, filter );
      }
      else if ( debugSettingKey == filterPMI )
      {
        result = UpdateBool( curSettingPair.second, filter ) || result;
        convProperties.SetIoPermission( iop_rDimensions, filter );
        convProperties.SetIoPermission( iop_wDimensions, filter );
        convProperties.SetIoPermission( iop_rTextDescription, filter );
        convProperties.SetIoPermission( iop_wTextDescription, filter );
      }
      else if ( debugSettingKey == filterDensity )
      {
        result = UpdateBool( curSettingPair.second, filter ) || result;
        convProperties.SetIoPermission( iop_rDensity, filter );
        convProperties.SetIoPermission( iop_wDensity, filter );
      }
      else if ( debugSettingKey == filterValidation )
      {
        result = UpdateBool( curSettingPair.second, filter ) || result;
        convProperties.SetIoPermission( iop_rValidationProperties, filter );
        convProperties.SetIoPermission( iop_wValidationProperties, filter );
      }
      else if ( debugSettingKey == configToRead )
      {
        int valInt{0};
        if ( sscanf( c3d::ToSTDstring( curSettingPair.second ).c_str(), "%d", &valInt ) > 0 )
          TestVariables::configurationToRead = valInt;
      }   
      else if ( debugSettingKey == visualPropertiesStrategy )   
      {
        int valInt{0};
        if ( sscanf( c3d::ToSTDstring( curSettingPair.second ).c_str(), "%d", &valInt ) > 0 )
        { 
          switch ( valInt )
          {
          case static_cast<int>(ColorProcessing::cr_Color):
            convProperties.visualPropertiesFormation = ColorProcessing::cr_Color;
            break;
          case static_cast<int>(ColorProcessing::cr_Visual):
            convProperties.visualPropertiesFormation = ColorProcessing::cr_Visual;
            break;
          case static_cast<int>(ColorProcessing::cr_Both):
            convProperties.visualPropertiesFormation = ColorProcessing::cr_Both;
            break;
          default:
            break;
          } 
        }
      }
      else if ( debugSettingKey == importMeshesWithTopology )
      {
        result = UpdateBool( curSettingPair.second, filter ) || result;
        convProperties.importMeshesWithTopology = filter;
      }
      else if ( debugSettingKey == importInstanceAttributes )
      {
        result = UpdateBool( curSettingPair.second, filter ) || result;
        convProperties.importInstanceAttributes = filter;
      }
      continue;
    }
    if ( isRepairMeshSetting )
    {
      const string_t repairMeshSettingKey( curSettingPair.first.substr( repairMeshSettingPrefix.length() ) );
      if ( repairMeshSettingKey == repairMeshEnable )
      {
        bool val{ convProperties.repairInconsistentMesh };
        if ( UpdateBool( curSettingPair.second, val ) )
        {
          convProperties.repairInconsistentMesh = val;
          result = true;
        }
      }
      else if ( repairMeshSettingKey == orientOutward )
      {
        bool val{ convProperties.repairInconsistentMeshParams.IsOrientOutward() };
        if ( UpdateBool( curSettingPair.second, val ) )
        {
          convProperties.repairInconsistentMeshParams.SetOrientOutward( val );
          result = true;
        }
      }
      else if ( repairMeshSettingKey == toleranceRM )
      {
        double val{ convProperties.repairInconsistentMeshParams.GetTolerance() };
        if ( sscanf( c3d::ToSTDstring( curSettingPair.second ).c_str(), "%lf", &val ) == 1 )
        {
          convProperties.repairInconsistentMeshParams.SetTolerance( val );
          result = true;
        }
      }
      else if ( repairMeshSettingKey == angleSharp )
      {
        double val{ convProperties.repairInconsistentMeshParams.GetSharpAngle() };
        if ( sscanf( c3d::ToSTDstring( curSettingPair.second ).c_str(), "%lf", &val ) == 1 )
        {
          convProperties.repairInconsistentMeshParams.SetSharpAngle( val );
          result = true;
        }
      }
      else if ( repairMeshSettingKey == normalAveraging )
      {
        bool val{ convProperties.repairInconsistentMeshParams.NeedNormalsAveraging() };
        if ( UpdateBool( curSettingPair.second, val ) )
        {
          convProperties.repairInconsistentMeshParams.SetNormalsAveraging( val );
          result = true;
        }
      }
      continue;
    }
    const string_t debugSettingKey( curSettingPair.first.substr( debugSettingPrefix.length() ) );
    if ( debugSettingKey == enableCERRout )
      result = UpdateBool( curSettingPair.second, convProperties.debugSettings.enableCERRout ) || result;
    else if ( debugSettingKey == cerrOutUserTreeTraverse )
      result = UpdateBool( curSettingPair.second, convProperties.debugSettings.cerrOutUserTreeTraverse ) || result;
    else if ( debugSettingKey == cerrOutIntermediateTreeTraverse)
      result = UpdateBool( curSettingPair.second, convProperties.debugSettings.cerrOutIntermediateTreeTraverse ) || result;
    else if ( debugSettingKey == cerrOutImportStatistic )
      result = UpdateBool( curSettingPair.second, convProperties.debugSettings.cerrOutImportStatistic ) || result;
    else if ( debugSettingKey == saveModelTwin)
      result = UpdateBool( curSettingPair.second, convProperties.debugSettings.saveModelTwin ) || result;
    else if ( debugSettingKey == attachHitsIdAttr )
      result = UpdateBool( curSettingPair.second, convProperties.debugSettings.attachThisIdAttribute ) || result;
    else if ( debugSettingKey == elementIdFineDebug )
    {
      ptrdiff_t id( 0 );
      if ( 1 == sscanf( c3d::ToSTDstring( curSettingPair.second ).c_str(), "%td", &id ) )
      {
        convProperties.debugSettings.elementIdFineDebug = id;
        result = true;
      }
    }
    else if ( debugSettingKey == loggingMode )
    {
      int valInt{0};
      if ( sscanf( c3d::ToSTDstring( curSettingPair.second ).c_str(), "%d", &valInt ) > 0 )
      {
        switch (valInt)
        {
        case static_cast<int>(eLoggingMode::elm_LoggingOff):
          convProperties.debugSettings.loggingMode = eLoggingMode::elm_LoggingOff;
          break;
        case static_cast<int>(eLoggingMode::elm_LoggingAll):
          convProperties.debugSettings.loggingMode = eLoggingMode::elm_LoggingAll;
          break;
        case static_cast<int>(eLoggingMode::elm_LoggingForId):
          convProperties.debugSettings.loggingMode = eLoggingMode::elm_LoggingForId;
          break;          
        default:
        ;
        }
      }
    }
    else if ( debugSettingKey == pathFineDebug  )
    {
      convProperties.debugSettings.pathFineDebug = curSettingPair.second;
      result = true;
    }    
  }
  return  result;
}


//------------------------------------------------------------------------------
// получить единицы измерения длины при экспорте
// Get the length units for export
// ---
//static MbeModelLengthUnits GetLengthUnits() {
//  MbeModelLengthUnits result(mlu_Mm);
//  int lengthUnits( 0 );
//  ::GetInt( IDS_UNIT_OF_MEASURE, lengthUnits );
//  switch( lengthUnits ) {
//    case 1:
//      result = mlu_Cm;
//      break;
//    case 2:
//      result = mlu_Dm;
//      break;
//    case 3:
//      result = mlu_Meter;
//      break;
//    case 4:
//      result = mlu_Inch;
//      break;
//    default:
//      result = mlu_Mm;
//      break;
//  }
//  return result;
//}


//------------------------------------------------------------------------------
// Получить параметры автосшивки
// Get the parameters for automatic stitch
// ---
bool GetAutostitchData( double & precision )
{
  int sType = 1;

  if ( !TestVariables::checkReadTime ) // не спрашиваем при замере времени
    ::GetInt( IDS_ENTER_STITCH_SURFACES, sType );

  bool enableAutostitch = sType != 0;
  if ( enableAutostitch ) {
    double autostitchVal( DELTA_MID );
    if ( !TestVariables::checkReadTime ) // не спрашиваем при замере времени
      ::GetDouble( IDS_ENTER_STITCH_PRECISION, autostitchVal );
    if ( autostitchVal > METRIC_REGION )
      precision = autostitchVal;
    else
      enableAutostitch = false;
  }
  return enableAutostitch;
}


//------------------------------------------------------------------------------
// Получить флаг слияния подобных граней
// Get the join similar faces flag
// ---
bool GetJoinSimilarFacesFlag()
{
  int sType = 1;
  if ( !TestVariables::checkReadTime ) // не спрашиваем при замере времени
    ::GetInt( IDS_ENTER_JOIN_SIMILAR_FACES, sType );

  return ( sType != 0 );
}


//------------------------------------------------------------------------------
// Получить флаг добавления удаленных граней в качестве оболочек
// Get the join similar faces flag
// ---
bool GetAddRemovedFacesAsShellsFlag()
{
  int sType = 0;
  if ( !TestVariables::checkReadTime ) // не спрашиваем при замере времени
    ::GetInt( IDS_ENTER_ADD_REMOVED_FACES_AS_SHELLS, sType );

  return ( sType != 0 );
}


////------------------------------------------------------------------------------
//// Поменять расширение имени файла
//// ---
//static
//void ChangeFileNameExt( string & name, TCHAR * ext )
//{
//  size_t len = name.length();
//
//  if ( len ) {
//    size_t pointPos = name.find_last_of( _T(".") );
//
//    if ( pointPos != NPOS )
//      name.remove( pointPos );
//
//    name += _T("."); 
//    name += string( ext );
//  }
//}


//------------------------------------------------------------------------------
// Установка имени модели
// Set the name of the model based on the name of the imported one
// ---
bool WrConvertor3D::SetModelName( const TCHAR * fileName )
{
  bool changed = false;

  if ( TestVariables::viewManager != nullptr ) {
    WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow();
    if ( wnd != nullptr && wnd->IsModelNameEmpty() ) {
      wnd->SetModelName( fileName );
      changed = true;
    }
  }
  return changed;
}

//------------------------------------------------------------------------------
// Записать в буфер.
// Write into buffer.
//---
MbeConvResType WriteIntoBuffer(const TCHAR * fileName, ConvConvertorProperty3D& convProperties) {
  if (fileName == nullptr)
    return cnv_Error;
  SPtr<MbItem> content(::ModelToContent(TestVariables::viewManager->GetActiveWindow()->SetGeomModel()));
  C3DExchangeBuffer buffer;
  C3dModelDocument modelDoc;
  SetWaitCursor( true );
  std::unique_ptr<IProgressIndicator> progInd ( &::CreateProgressIndicator( new StrSpyConverters{} ) );
  modelDoc.SetContent(content);

#ifdef SET_LOCALE_OBVIOUSLY
  std::string const prevLocale { ::setlocale(LC_CTYPE, nullptr) };
  ::setlocale(LC_CTYPE, SPECIFIC_LOCALE);
#endif // SET_LOCALE_OBVIOUSLY

  MbeConvResType importCode = ::ExportIntoBuffer(modelDoc, mxf_autodetect, buffer, &convProperties, progInd.get());
  LogReporter(c3d::string_t(fileName)).LogReport(convProperties.logRecords);
  if (importCode == cnv_Success)
  {
    std::ofstream test(c3d::C3DToPathstring(fileName), std::ios::out);
    test.write(buffer.Data(), buffer.Count());
  }

#ifdef SET_LOCALE_OBVIOUSLY
  ::setlocale(LC_CTYPE, prevLocale.c_str());
#endif // SET_LOCALE_OBVIOUSLY

  SetWaitCursor(false);
  return importCode;
}

//------------------------------------------------------------------------------
// Функтор получения пользовательской настройки плагина
// пример настройки: converter.plugin.settings.C3D_CAPVIDIA_TRACE_OUT=C3D_CAPVIDIA_BOOLEAN_TRUE
//---
struct PluginPropGetter
{
  optionNameValuePairs_t & pluginSpecificSettings;

public:
  PluginPropGetter( optionNameValuePairs_t & _pluginSpecificSettings ) : pluginSpecificSettings( _pluginSpecificSettings ) {}

public:
  void operator() ( const std::pair<c3d::string_t, c3d::string_t> & prop )
  {
    if ( prop.first.find( CONFIG_CONV_PLUGIN_SETTINGS_PREFIX ) == 0 )
    {
      c3d::string_t keyPlagin = prop.first.substr( string_t(CONFIG_CONV_PLUGIN_SETTINGS_PREFIX).length() );
      pluginSpecificSettings[keyPlagin] = prop.second;
    }
  }
};


//------------------------------------------------------------------------------
// Получить пользовательские настройки плагина
// Файл c3d_test.config должен лежать рядом с файлом лицензии.
// ---
static bool GetPluginSettings( c3d::path_string & fullPathToPlugin, optionNameValuePairs_t & pluginSpecificSettings )
{
  std::map<c3d::string_t, c3d::string_t>::iterator userPropIter = ::TestVariables::userProp.find( CONFIG_CONV_PLUGIN_FULL_PATH );
  if ( userPropIter != ::TestVariables::userProp.end() )
  {
    fullPathToPlugin = c3d::C3DToPathstring( userPropIter->second );

    //специфичные настройки плагина
    std::for_each( ::TestVariables::userProp.begin(), ::TestVariables::userProp.end(), PluginPropGetter( pluginSpecificSettings ) );
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
// Выбор конфигурации для импорта
// Configuration to read selector
// ---
class ConfigurationSelector : public IConfigurationSelector
{  
  size_t config;
  bool useActiveConfiguration;
public:

  ConfigurationSelector()
  : config{ static_cast<size_t>(-2) }
  , useActiveConfiguration { true }
  {
  }

  ConfigurationSelector( size_t conf ) 
  : config{ conf }
  , useActiveConfiguration { false }
  {
  }
  
  virtual ~ConfigurationSelector() = default;

  void AddConfiguration ( const c3d::string_t& configurationName ) override final
  {
    std::cout << ToSTDstring( configurationName ) << std::endl;
  }
  void AddEmptyConfiguration( const c3d::string_t & configurationName ) override final
  {
    std::cout << "Empty: " << ToSTDstring(configurationName) << std::endl;
  }

  void SetActiveConfiguration  ( const size_t index ) override final
  { 
    if ( useActiveConfiguration )
      config = index;
    std::cout << "Active: " << index << std::endl; 
  }
  
  size_t GetConfiguration () const override final
  { 
    std::cout << "Chosen: " << config << std::endl;
    return config;
  }
};


//------------------------------------------------------------------------------
// Получатель метаданных
// ---
class MetadataReciever final : public IConverterMetadataReceiver
{
  public:
  bool InitMetadataForItem(const c3d::string_t& category, const c3d::string_t& , const c3d::string_t&) final
  {
    std::cerr << "Start metadata transfer for " << ToSTDstring( category ) << std::endl; return true;
  };
  void CloseMetadataForItem(const c3d::string_t& category, const c3d::string_t& ) final 
  {
    std::cerr << "End metadata transfer for " << ToSTDstring( category ) << std::endl;
  };
  void TransferMetadataForItem(const c3d::string_t& category, const c3d::string_t&, const char* metadataBuffer, unsigned long int bufferCapacity) final
  {
    std::cerr << "Metadata for " << ToSTDstring( category ) << " buffer capacity: " << bufferCapacity << " address is " << (metadataBuffer == nullptr ? "null" : "valid") << std::endl;
  }
};

namespace {

//----------------------------------------------------------------------------------------
// Добавить с строке список других строк, разделяя их содержимым указанной строки
// ---
void JoinStringsInto(string_t & target, std::vector<string_t> const & parts, string_t const & delimiter, std::optional<size_t> maxCount = std::nullopt) {
  size_t partIndex = 0;
  for (auto const & part : parts) {
    target.append(part);
    if (partIndex != parts.size() - 1) {
      target.append(delimiter);
    }

    // Отсечь максимальное число элементов если задано
    if (maxCount.has_value()) {
      if (partIndex + 1 >= maxCount.value()) {
        if (maxCount.value() != parts.size()) {
          target.append(_T("..."));
          break;
        }
      }
    }

    partIndex++;
  }
}

//----------------------------------------------------------------------------------------
// Сформировать формат, включающий в себя несколько форматов
// ---
c3d::FormatInfo MergeManyFormatsIntoOne(std::vector<c3d::FormatInfo> const & formats) {
  string_t const title {_T("All supported formats")};
  std::vector<string_t> filters {};
  for (auto const & format : formats) {
    for (auto const & filter : format.GetFilters()) {
      filters.push_back(filter);
    }
  }
  return FormatInfo {std::move(title), std::move(filters)};
}

}

//----------------------------------------------------------------------------------------
// Преобразовать список поддерживаемых форматов в вид для диалога выбора файла
// ---
string_t FormatFiltersForFileSelectionDialog(std::vector<c3d::FormatInfo> const & formats) {
  #ifdef __USE_QT__
    c3d::string_t const maskBegin = _T(" (");
    c3d::string_t const maskEnd = _T(")");
    c3d::string_t const maskSeparator = _T(" ");
    c3d::string_t const formatSeparator = _T(";;");
    c3d::string_t const terminator = _T("");
  #else
    // Для MFC нужно разделять части строки-фильтра при помощи символа \0.
    // std::string умеет хранить строки с \0 в середине, но инициализировать их нужно с явным указанием размера,
    // чтобы конструктор не воспринимал \0 как окончание строки.
    c3d::string_t const nullSymbol { _T("\0"), 1UL };

    c3d::string_t const maskBegin = nullSymbol;
    c3d::string_t const maskEnd = _T("");
    c3d::string_t const maskSeparator = _T(";");
    c3d::string_t const formatSeparator = nullSymbol;
    c3d::string_t const terminator = nullSymbol;
  #endif

  string_t filter {};
  for (auto const & format : formats) {
    filter.append(format.GetTitle());
    filter.append(_T(" ("));
    JoinStringsInto(filter, format.GetFilters(), _T(", "), 5);
    filter.push_back(')');
    filter.append(maskBegin);
    JoinStringsInto(filter, format.GetFilters(), maskSeparator);
    filter.append(maskEnd);
    filter.append(formatSeparator);
  }
  filter.append(_T("All files (*.*)"));
  filter.append(maskBegin);
  filter.append(_T("*.*"));
  filter.append(maskEnd);
  filter.append(terminator);
  return filter;
}

//----------------------------------------------------------------------------------------
// Получить фильтр имён файлов исходя из настроенного режима импорта
// ---
c3d::string_t GetFilenameFilter(PluginLoader * const pluginLoader) {
  std::vector<FormatInfo> supportedFormats {};

  bool const readOnlyViaPlugin = TestVariables::readToModelDocument && TestVariables::forceUsePlugin;

  // Добавить форматы, поддерживаемые ядром, если чтение через встроенные конвертеры предполагается
  if (!readOnlyViaPlugin) {
    for ( const auto format : ::GetSupportedFormats() ) {
      supportedFormats.push_back(format);
    }
  }

  // Добавить форматы, поддерживаемые плагином, если чтение через плагин предполагается
  if ( pluginLoader != nullptr && TestVariables::readToModelDocument ) {
    for ( const auto format : pluginLoader->GetConverter().GetSupportedImportForeignFormats() ) {
      supportedFormats.push_back(format);
    }
  }

  // Добавить фильтр для всех поддерживаемых форматов
  if (supportedFormats.size() >= 2) {
    supportedFormats.insert(supportedFormats.begin(), MergeManyFormatsIntoOne(supportedFormats));
  }

  return FormatFiltersForFileSelectionDialog(supportedFormats);
}

//------------------------------------------------------------------------------
// Загрузить плагин если ещё не загружен
// ---
void PluginLoader::LoadPlugin() {
  if (loadStatus_ == LoadStatus::Pending) {
    loadStatus_ = LoadStatus::Error;
    optionNameValuePairs_t pluginSpecificSettings;
    c3d::path_string fullPathToPlugin;
    if ( nullptr != converter_ && GetPluginSettings( fullPathToPlugin, pluginSpecificSettings ) ) {
      SPtr<IConfigurationSelector> cs{ ( TestVariables::configurationToRead == -2 ) ? new ConfigurationSelector{} : new ConfigurationSelector{ static_cast<size_t>( TestVariables::configurationToRead ) } };
      if (converter_->LoadForeignReader( fullPathToPlugin, pluginSpecificSettings, cs.get() ) == cnv_Success) {
        loadStatus_ = LoadStatus::Success;
      }
    }
  }
}

//------------------------------------------------------------------------------
// Успешна ли загрузка плагина
// ---
bool PluginLoader::Loaded() {
  LoadPlugin();
  return loadStatus_ == LoadStatus::Success;
}

//------------------------------------------------------------------------------
// Получить конвертер
// ---
IConverter3D & PluginLoader::GetConverter() {
  LoadPlugin();
  return *converter_;
}

//------------------------------------------------------------------------------
// Импортировать с использованием плагина
// Import using plugin
// ---
MbeConvResType WrConvertor3D::PluginRead( const TCHAR* fileName, PluginLoader & pluginLoader ) {
  MbeConvResType result { cnv_Error };
  if ( nullptr == fileName )
    return result;

  if (pluginLoader.Loaded()) {
    ConvConvertorProperty3D ccp3d;
    UpdateConverterSettings( ccp3d );
    ccp3d.fileName = C3DToPathstring( fileName );

    C3dModelDocument mDoc;
    std::unique_ptr<IProgressIndicator> progInd ( &::CreateProgressIndicator( new StrSpyConverters{} ) );
    result = pluginLoader.GetConverter().ImportForeign( C3DToPathstring( fileName ), mDoc, &ccp3d, progInd.get() );
    LogReporter(c3d::string_t( fileName )).LogReport( ccp3d.logRecords );
    if ( (result == cnv_Success) || ( result == cnv_PartialSuccess ) ) {
      MbItem* content = mDoc.GetContent();
      if ( nullptr != content ) {
        ::AddRefItem( content );
        AddContentToManager( *content, true );
        ::ReleaseItem( content );
      }
    }
  } else {
    MessageBoxEx( RT_PLUGIN_LOAD_FAULT, IDS_TITLE_ERR );
  }

  return result;
}


//------------------------------------------------------------------------------
// Результат диагностики тела
// ---
struct SolidInspectionData
{
  //id проблемных элементов
  /*
  std::vector<ptrdiff_t> badLoopsFacesId;
  std::vector<ptrdiff_t> boundaryEdgesId;
  std::vector<ptrdiff_t> incorrectEdgesId;
  std::vector<ptrdiff_t> inexactEdgesId;
  std::vector<ptrdiff_t> inexactVerticesId;
  */
  //индексы проблемных элементов
  std::vector<size_t> badLoopsFacesIndex;
  double badLoopsFacesMaxLenTol;
  double badLoopsFacesMaxParamTol;
  std::vector<size_t> boundaryEdgesIndex;
  std::vector<size_t> incorrectEdgesIndex;
  std::vector<size_t> inexactEdgesIndex;
  std::vector<size_t> inexactVerticesIndex;
  //double inexactVerticesMaxTol;

  SolidInspectionData() 
    : /*badLoopsFacesId   {}
    , boundaryEdgesId   {}
    , incorrectEdgesId  {}
    , inexactEdgesId    {}
    , inexactVerticesId {}
    
    ,*/ badLoopsFacesIndex{}
    , badLoopsFacesMaxLenTol{ 0.0 }
    , badLoopsFacesMaxParamTol{ 0.0 }
    , boundaryEdgesIndex{}
    , incorrectEdgesIndex{}
    , inexactEdgesIndex {}
    , inexactVerticesIndex{}
    //, inexactVerticesMaxTol{ 0.0 }

    {}
};


//------------------------------------------------------------------------------
// Выдать идентификаторы граней с некорректными циклами
// ---
void GetBadLoopsFacesId( std::vector<MbFace *> const & faces, SolidInspectionData & solidInspData )
{
  int faceIndex(0);
  for( std::vector<MbFace *>::const_iterator itrFace = faces.cbegin(); itrFace != faces.cend(); ++itrFace, faceIndex++) {
    MbFace * face = *itrFace;
    if( !face )
      continue;
    for ( size_t i = 0, loopsCnt = face->GetLoopsCount(); i < loopsCnt; i++ ) {
      MbLoop * loop = face->GetLoop( i );
      if( !loop )
        continue;
      double lenTol = LENGTH_EPSILON;
      double paramTol = PARAM_EPSILON;
      if ( !::CheckLoopConnection( *face, *loop, lenTol, &paramTol ) ) {
        solidInspData.badLoopsFacesIndex.push_back( faceIndex );
        if ( solidInspData.badLoopsFacesMaxLenTol < lenTol )
          solidInspData.badLoopsFacesMaxLenTol = lenTol;
        if ( solidInspData.badLoopsFacesMaxParamTol < paramTol )
          solidInspData.badLoopsFacesMaxParamTol = paramTol;
        break;
      }
    }
  }
}


//------------------------------------------------------------------------------
// Выдать идентификаторы краевых ребер
// ---
void GetBoundaryEdgesId( std::vector<const MbCurveEdge *> const & edges, SolidInspectionData & solidInspData )
{
  int index( 0 );
  for( auto itrEdge = edges.cbegin(); itrEdge != edges.cend(); ++itrEdge,++index) {
    const MbCurveEdge * edge = *itrEdge;
    if( edge == nullptr )
      continue;
    if ( edge->IsBoundaryFace() ) {
      solidInspData.boundaryEdgesIndex.push_back( index );
    }
  }
}


//------------------------------------------------------------------------------
// Индекс в массиве
// ---
template< typename ItemType >
static size_t IndexInArray( const ItemType& ver, std::vector<ItemType > const & vertices ) {
  return std::distance( vertices.cbegin(), std::find( vertices.cbegin(), vertices.cend(), ver ) );
}


//------------------------------------------------------------------------------
// Выдать идентификаторы некорректных ребер
// ---
void GetIncorrectEdgesId( std::vector<const MbCurveEdge *> const & edges, SolidInspectionData & solidInspData )
{
  c3d::ConstEdgesVector allEdges, badEdges;
  for( auto itrEdge = edges.cbegin(); itrEdge != edges.cend(); ++itrEdge ) {
    if( !*itrEdge )
      continue;
    allEdges.push_back( *itrEdge );
  }
  if ( ::CheckBadEdges(allEdges, &badEdges) ) {
    for ( size_t i = 0, badEdgesCnt = badEdges.size(); i < badEdgesCnt; i++ ) {
      if( !badEdges[i] )
        continue;
      //ptrdiff_t edgeId = 0;
      const auto index = IndexInArray< const MbCurveEdge* >( badEdges[i], edges );
      solidInspData.incorrectEdgesIndex.push_back( index );
    }
  }
}


//------------------------------------------------------------------------------
// Выдать идентификаторы неточных ребер
// ---
void GetInexactEdgesId( std::vector<const MbCurveEdge *> const & edges, SolidInspectionData & solidInspData )
{
  int index( 0 );
  const double mAcc = DELTA_MIN;
  for( auto itrEdge = edges.cbegin(); itrEdge != edges.cend(); ++itrEdge, ++index ) {
    const MbCurveEdge * edge = *itrEdge;
    if( !edge )
      continue;
    if ( ::IsInexactEdge(*edge, mAcc) ) {
      //ptrdiff_t edgeId = 0;
      solidInspData.inexactEdgesIndex.push_back(index);
    }
  }
}


//------------------------------------------------------------------------------
// Выдать идентификаторы неточных вершин
// ---
void GetInexactVerticesId( std::vector<MbVertex *> & vertices, SolidInspectionData & solidInspData )
{
  std::vector<MbVertex *> inexactVertices;
  const double mAcc = DELTA_MIN;
  if ( ::CheckInexactVertices(vertices, mAcc, &inexactVertices) ) {
    for( ItemPtrVectorIterator<std::vector<MbVertex *>, MbVertex> itrVert( inexactVertices ); itrVert.undone(); itrVert.next() ) {
      const auto index = IndexInArray< MbVertex* >( &(*itrVert), vertices );
      solidInspData.inexactVerticesIndex.push_back( index );
      // for ( size_t i = 0, icnt = vertices.size(); i < icnt; ++i ) {
      //   if ( vertices[i] != nullptr && vertices[i]->GetTolerance() > solidInspData.inexactVerticesMaxTol )
      //     solidInspData.inexactVerticesMaxTol = vertices[i]->GetTolerance();
      // }
    }
  }
}


//------------------------------------------------------------------------------
// Получить для тела индексы проблемныхтопологических объектов
// ---
void InspectSolid( MbSolid& solid, SolidInspectionData & solidInspData )
{
  std::vector<MbFace *> faces;
  solid.GetFaces( faces );
  std::vector<const MbCurveEdge *> edges;
  solid.GetEdges( edges );
  std::vector<MbVertex *> vertices;
  solid.GetVertices( vertices );
  GetBadLoopsFacesId( faces, solidInspData );
  GetBoundaryEdgesId( edges, solidInspData );
  GetIncorrectEdgesId( edges, solidInspData );
  GetInexactEdgesId( edges, solidInspData );
  GetInexactVerticesId( vertices, solidInspData );

}


//------------------------------------------------------------------------------
// Покрасить проблемные топологические объекты в теле
// ---
void ApplyPermanentDiagnosticVisual( MbSolid& solidToSave )
{
  SolidInspectionData solidData;
  InspectSolid( solidToSave, solidData );

  solidToSave.RemoveSimpleAttribute( at_Color );
  solidToSave.SetColor( TRGB_LIGHTGRAY );

  // убрать все визуальные атрибуты
  FacesVector faces;
  solidToSave.GetFaces( faces );
  for ( auto face : faces )
  {
    if( face != nullptr )
      face->RemoveSimpleAttribute( at_Color );
  }
  EdgesVector edges;
  solidToSave.GetEdges( edges );
  for ( auto edge : edges )
  {
    if ( edge != nullptr )
      edge->RemoveSimpleAttribute( at_Color );
  }
  VerticesVector vertices;
  solidToSave.GetVertices( vertices );
  for ( auto vrt : vertices )
  {
    if ( vrt != nullptr )
      vrt->RemoveSimpleAttribute( at_Color );
  }
  // Проблемные грани
  for ( auto ind : solidData.badLoopsFacesIndex)
  {
    MbFace* face = solidToSave.GetFace( ind );
    if ( face != nullptr )
      face->SetColor( TRGB_LIGHTCYAN );
  }
  // Неточные рёбра
  for ( auto ind : solidData.inexactEdgesIndex )
  {
    MbCurveEdge* edge = solidToSave.GetEdge( ind );
    if ( edge != nullptr )
      edge->SetColor( TRGB_ORANGE );
  }
  // Некорректные рёбра
  for ( auto ind : solidData.incorrectEdgesIndex )
  {
    MbCurveEdge* edge = solidToSave.GetEdge( ind );
    if ( edge != nullptr )
      edge->SetColor( TRGB_RED );
  }
  // Краевые рёбра
  for ( auto ind : solidData.boundaryEdgesIndex )
  {
    MbCurveEdge* edge = solidToSave.GetEdge( ind );
    if ( edge != nullptr )
      edge->SetColor( TRGB_MAGENTA );
  }
  // Неточные вершины
  for ( auto ind : solidData.inexactVerticesIndex )
  {
    MbVertex* vrt = solidToSave.GetVertex( ind );
    if ( vrt != nullptr )
      vrt->SetColor( TRGB_ORANGE );
  }
}
