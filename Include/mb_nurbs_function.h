////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Модуль геометрических построений.
         \en The module of geometric constructions. \~
  \details \ru Базовые алгоритмы Nurbs кривых и поверхностей.
           \en The base algorithms for NURBS curves and surfaces. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MB_NURBS_FUNCTION_H
#define __MB_NURBS_FUNCTION_H


#include <templ_s_array.h>
#include <templ_array2.h>
#include <templ_sparse_array2.h>
#include <templ_dptr.h>
#include <math_define.h>
#include <mb_enum.h>
#include <curve3d.h>
#include <surface.h>
#include <mb_rect1d.h>
#include <mb_matrixnn.h>
#include <mb_vector3d.h>
#include <mb_cart_point3d.h>
#include <op_binding_data.h>
#include <alg_base.h>
#include <vector>

class   MbContour;
class   MbContour3D;
class   MbNurbs;
class   MbNurbs3D;

//------------------------------------------------------------------------------
/** \brief \ru Флаг, определяющий построение сплайна, проходящего через точки.
           \en Flag defining creation of spline passing through points. \~
  \details \ru Флаг, определяющий построение сплайна, проходящего через точки. Связан с версией. \n
           \en Flag defining creation of spline passing through points. Related to the version. \n \~
  \ingroup Data_Structures
*/
// ---
enum MbeSplineCreateType {
  sct_Version0  = 0,  ///< \ru Используется в версиях < V13 (центростремительная параметризация). \en Used in versions < V13 (centripetal parameterization). 
  sct_Version1  = 1,  ///< \ru Используется в версии V13 (параметризация по длине хорды). \en Used in version V13 (parameterization by chord length). 
  sct_Version2  = 2,  ///< \ru Используется в версии V13+. \en Used in version V13+. 
};


//-------------------------------------------------------------------------------
/** \brief \ru Дополнительная информация для преобразования кривой или поверхности в Nurbs.
           \en Additional information for transformation of a curve or surface to NURBS. \~
  \details \ru Дополнительная информация для преобразования кривой или поверхности в Nurbs. \n
           \en Additional information for transformation of a curve or surface to NURBS. \n \~
  \ingroup Data_Structures
*/
// ---
class MATH_CLASS MbCurveIntoNurbsInfo {
private:
  double  tbeg;         ///< \ru Параметр начала участка кривой. \en A parameter of the curve piece start. 
  double  tend;         ///< \ru Параметр конца участка кривой. \en A parameter of the curve piece end. 
  int     sense;        ///< \ru Направление сплайн-кривой. \en Direction of spline-curve. 
  bool    matchParams;  ///< \ru Сохранять ли при преобразовании однозначное соответствие параметрических областей. \en Whether to save correspondence of parametric regions while transforming or not. 
  bool    extendRange;  ///< \ru Строится ли преобразование на продолжении для незамкнутой подложки. \en Whether transformation is constructed on the extension for a non-closed substrate. 
  double  tRegion;      ///< \ru Параметрическая точность. \en Parametric accuracy.
  VERSION version;      ///< \ru Версия исполнения. \en The version of execution. 
private:
  MbCurveIntoNurbsInfo();
public:
  /// \ru Конструктор. \en Constructor. 
  template <class Curve>
  MbCurveIntoNurbsInfo( const Curve & c, bool match, bool ext, VERSION ver = Math::DefaultMathVersion() ) 
    : tbeg       ( c.GetTMin()   )
    , tend       ( c.GetTMax()   )
    , sense      ( 1             )
    , matchParams( match         )
    , extendRange( ext           )
    , tRegion    ( PARAM_EPSILON )
    , version    ( ver           )
  {}
  /// \ru Конструктор. \en Constructor. 
  MbCurveIntoNurbsInfo( double t1, double t2, int s, bool match, bool ext, VERSION ver = Math::DefaultMathVersion() ) 
    : tbeg       ( t1            )
    , tend       ( t2            )
    , sense      ( s             )
    , matchParams( match         )
    , extendRange( ext           )
    , tRegion    ( PARAM_EPSILON )
    , version    ( ver           )
  {}
  /// \ru Конструктор. \en Constructor. 
  MbCurveIntoNurbsInfo( double t1, double t2, int s, bool match, bool ext, double tRegion, VERSION ver = Math::DefaultMathVersion() ) 
    : tbeg       ( t1      )
    , tend       ( t2      )
    , sense      ( s       )
    , matchParams( match   )
    , extendRange( ext     )
    , tRegion    ( tRegion )
    , version    ( ver     )
  {}
  /// \ru Конструктор. \en Constructor. 
  MbCurveIntoNurbsInfo( const MbCurveIntoNurbsInfo & other, double t1, double t2, int s ) 
    : tbeg       ( t1                )
    , tend       ( t2                )
    , sense      ( s                 )
    , matchParams( other.matchParams )
    , extendRange( other.extendRange )
    , tRegion    ( PARAM_EPSILON     )
    , version    ( other.version     )
  {}
  /// \ru Функция присвоения. \en Assignment function. 
  void Assign( const MbCurveIntoNurbsInfo & other )
  {
    tbeg = other.tbeg;
    tend = other.tend;
    sense = other.sense;
    matchParams = other.matchParams;
    extendRange = other.extendRange;
    version = other.version;
  }
  /// \ru Функция инициализации. \en The initialization function. 
  bool Init( double t1, double t2, int s )
  {
    tbeg = t1;
    tend = t2;
    sense = s;
    C3D_ASSERT( ((sense == 1) || (sense == -1)) );
    return ((sense == 1) || (sense == -1));
  }
  /// \ru Функция инициализации. \en The initialization function. 
  bool Init( double t1, double t2, int s, bool match, bool ext ) {    
    matchParams = match;
    extendRange = ext;
    return Init( t1, t2, s );
  }

public:
  /// \ru Получить параметр начала участка кривой. \en Get the parameter of the start curve region. 
  double    GetTBeg()  const { return tbeg;  }
  /// \ru Получить параметр конца участка кривой. \en Get the parameter of the end curve region. 
  double    GetTEnd()  const { return tend;  }
  /// \ru Получить направление сплайн-кривой. \en Get the direction of spline-curve. 
  int       GetSense() const { return sense; }
  /// \ru Сохранять ли при преобразовании однозначное соответствие параметрических областей. \en Whether to save correspondence of parametric regions by transformation or not. 
  bool      MatchParams() const { return matchParams; }
  /// \ru Строится ли преобразование на продолжении для незамкнутой подложки. \en Whether transformation is constructed on the extension for a non-closed substrate or not. 
  bool      ExtendRange() const { return extendRange; }
  /// \ru Получить версию исполнения. \en Get the version of execution. 
  VERSION   GetMathVersion() const { return version; }
  /// \ru Получить парметрическую точность. \en Get the parametric accuracy.
  double    GetTRegion() const { return tRegion; }

OBVIOUS_PRIVATE_COPY( MbCurveIntoNurbsInfo )
};


//-------------------------------------------------------------------------------
/** \brief \ru Параметры построения NURBS копии объекта.
           \en Parameters for the construction of a NURBS copy of the object. \~
  \details \ru Параметры построения NURBS копии объекта. \n
           \en Parameters for the construction of a NURBS copy of the object. \n \~
  \ingroup Data_Structures
*/
// ---
struct MATH_CLASS MbNurbsParameters: public MbPrecision {
public:
  size_t         degree;      ///< \ru Порядок NURBS копии. \en Order of NURBS copy. 
  size_t         pointsCount; ///< \ru Количество контрольных точек (при 0 параметр игнорируется). \en The number of control points (if there is no control points, parameter is ignored).  
  MbRect1D       region;      ///< \ru Область объекта, подлежащая копированию: [0 1] соответствует [tMin tMax] объекта. \en Region of the object to be copied: [0, 1] corresponds to [tMin tMax] object. 
  SArray<double> knots;       ///< \ru Узловой вектор. \en Knot vector. 
  mutable bool   useApprox;   ///< \ru Не пытаться построить точную поверхность. \en Don't try to create the exact surface. 

public:
  /// \ru Конструктор по умолчанию. \en Default constructor. 
  MbNurbsParameters()
    : MbPrecision()
    , degree     ( c3d::NURBS_DEGREE )
    , pointsCount( 0 )
    , region     ( 0.0, 1.0 )
    , knots      ( 0, 1 )
    , useApprox  ( true )
  {}
  /// \ru Конструктор по параметрам (без узлов) построения NURBS. \en The constructor of NURBS by parameters (without knots). 
  MbNurbsParameters( size_t d, size_t c, double zmin, double zmax, bool approx )
    : MbPrecision()
    , degree     ( d )
    , pointsCount( c )
    , region     ( zmin, zmax )
    , knots      ( 0, 1 )
    , useApprox  ( approx )
  {
    C3D_ASSERT( d > 1 && d < SYS_MAX_UINT16 );
    degree = std_max( d, (size_t)2 );
    degree = std_min( d, (size_t)SYS_MAX_UINT16 );
  }
  /// \ru Конструктор по полному набору параметров построения NURBS. \en The constructor of NURBS by a complete set of parameters. 
  MbNurbsParameters( size_t d, size_t c, double zmin, double zmax, bool approx, const SArray<double> & aKnots )
    : MbPrecision()
    , degree     ( d )
    , pointsCount( c )
    , region     ( zmin, zmax )
    , knots      ( aKnots )
    , useApprox  ( approx )
  {
    C3D_ASSERT( d > 1 && d < SYS_MAX_UINT16 );
    degree = std_max( d, (size_t)2 );
    degree = std_min( d, (size_t)SYS_MAX_UINT16 );
  }
  /// \ru Конструктор копирования. \en The copy constructor. 
  MbNurbsParameters( const MbNurbsParameters & other )
    : MbPrecision( other )
    , degree     ( other.degree )
    , pointsCount( other.pointsCount )
    , region     ( other.region )
    , knots      ( other.knots )
    , useApprox  ( other.useApprox )
  {}
  /// \ru Деструктор. \en Destructor. 
  ~MbNurbsParameters() {}

  /// \ru Инициализировать по другим параметрам построения NURBS копии объекта. \en Initialize by another parameters. 
  void        Init( const MbNurbsParameters & other ) { 
    degree = other.degree;
    pointsCount = other.pointsCount;
    region = other.region;
    knots = other.knots; 
    useApprox = other.useApprox;
  }
  /// \ru Являются ли объекты равными? \en Determine whether an object is equal? 
  bool        IsSame( const MbNurbsParameters & other, double accuracy ) const; 
  /// \ru Оператор присваивания. \en The assignment operator. 
  MbNurbsParameters & operator = ( const MbNurbsParameters & other ) {
    MbPrecision::operator=( other );
    degree = other.degree;
    pointsCount = other.pointsCount;
    region = other.region;
    knots = other.knots; 
    useApprox = other.useApprox;
    return (*this);
  }

public:
  size_t                  GetDegree()      const { return degree; }
  size_t                  GetPointsCount() const { return pointsCount; }
  const MbRect1D &        GetRegion()      const { return region; }
  const SArray<double> &  GetKnots()       const { return knots; }
  bool                    UseApprox()      const { return useApprox; }

  void                    SetDegree( size_t c ) { if ( c > 1 ) degree = c; }
  void                    SetPointsCount( size_t c ) { if ( c > 1 ) pointsCount = c; }
  SArray<double> &        SetKnots() { return knots; }
  void                    SetKnots( SArray<double> & c ) { knots = c; }
  void                    SetKnots( std::vector<double> & c ) { knots = c; }
  void                    SetApprox( bool c ) { useApprox = c; }

KNOWN_OBJECTS_RW_REF_OPERATORS( MbNurbsParameters ) // \ru Для работы со ссылками и объектами класса \en For working with references and objects of the class 
}; // MbNurbsParameters


//------------------------------------------------------------------------------
/** \brief \ru Выходные данные для операции создания NURBS копии кривой.
           \en Output data for create NURBS copy of curve. \~
  \details \ru Класс содержит значение максимального отклонения NURBS копии кривой от исходной кривой и NURBS копию кривой. \n
           \en The output data contains value of maximum deviation of NURBS copy of curve from the original curve and NURBS copy of the curve. \n \~
  \ingroup Data_Structures
*/
// ---
class MATH_CLASS MbNurbsCopyCurveResult {
private:
  double              _maxDeviation; ///< \ru Значение максимального отклонения NURBS копии кривой от исходной кривой.\en Value of maximum deviation of NURBS copy of curve from the original curve. \~ 
  c3d::SpaceCurveSPtr _nurbsCopy;    ///< \ru NURBS копия кривой.\en NURBS copy of the curve. \~ 
public:
  /// \ru Конструктор по умолчанию. \en Default constructor. \~
  MbNurbsCopyCurveResult()
  : _maxDeviation( 0 )
  , _nurbsCopy   ()
  {}
  /// \ru Получить значение максимального отклонения NURBS копии кривой. \en Get value of maximum deviation of NURBS copy of curve. \~
  double GetMaxDeviation() { return _maxDeviation; }
  /// \ru Получить NURBS копию кривой. \en Get NURBS copy of the curve. \~
  c3d::SpaceCurveSPtr & SetNurbsCopy() { return _nurbsCopy; } 
  /// \ru Задать значение максимального отклонения NURBS копии кривой. \en Set value of maximum deviation of NURBS copy of curve. \~
  void InitMaxDeviation ( double maxDeviation ) { _maxDeviation = maxDeviation; }
  /// \ru Задать NURBS копию кривой. \en Set NURBS copy of the curve. \~
  void InitNurbsCopy ( const c3d::SpaceCurveSPtr & nurbsCopy ) { _nurbsCopy = nurbsCopy; }
  OBVIOUS_PRIVATE_COPY( MbNurbsCopyCurveResult )
};


//------------------------------------------------------------------------------
/** \brief \ru Выходные данные для операции создания NURBS копии поверхности.
           \en Output data for create NURBS copy of surface. \~
  \details \ru Класс содержит значение максимального отклонения NURBS копии поверхности от исходной поверхности и NURBS копию поврехности. \n
           \en The output data contains value of maximum deviation of NURBS copy of surface from the original surface and NURBS copy of the surface. \n \~
  \ingroup Data_Structures
*/
// ---
class MATH_CLASS MbNurbsCopySurfaceResult {
private:
  double           _maxDeviation; ///< \ru Значение максимального отклонения NURBS копии поверхности от исходной поверхности.\en Value of maximum deviation of NURBS copy of surface from the original surface. \~ 
  c3d::SurfaceSPtr _nurbsCopy;    ///< \ru NURBS копия поверхности.\en NURBS copy of the surface. \~ 
public:
  /// \ru Конструктор по умолчанию. \en Default constructor. \~
  MbNurbsCopySurfaceResult()
    : _maxDeviation( 0 )
    , _nurbsCopy   ()
  {}
  /// \ru Получить значение максимального отклонения NURBS копии поверхности. \en Get value of maximum deviation of NURBS copy of surface. \~
  double GetMaxDeviation() const { return _maxDeviation; }
  /// \ru Получить NURBS копию поверхности. \en Get NURBS copy of the surface. \~
  c3d::SurfaceSPtr & SetNurbsCopy() { return _nurbsCopy; } 
  /// \ru Задать значение максимального отклонения NURBS копии поверхности. \en Set value of maximum deviation of NURBS copy of surface. \~
  void InitMaxDeviation ( double maxDeviation ) { _maxDeviation = maxDeviation; }
  /// \ru Задать NURBS копию поверхности. \en Set NURBS copy of the surface. \~
  void InitNurbsCopy ( const c3d::SurfaceSPtr & nurbsCopy ) { _nurbsCopy = nurbsCopy; }
  OBVIOUS_PRIVATE_COPY( MbNurbsCopySurfaceResult )
};


//-------------------------------------------------------------------------------
/** \brief \ru Параметры узловой точки сплайновой копии объекта.
           \en Parameters of knot point of the object spline copy. \~
  \details \ru Параметры узловой точки сплайновой копии объекта. \n
           \en Parameters of knot point of the object spline copy. \n \~
  \ingroup Data_Structures
*/
// ---
struct MATH_CLASS MbNurbsPointInfo {
public:
  MbCartPoint3D     point;        ///< \ru Узловая точка сплайновой поверхности. \en A knot point of a spline surface. 
  bool              visible;      ///< \ru Флаг видимости точки. \en A point visibility flag.   
  int8              poleLocation; ///< \ru Расположение полюса в параметрической области. \en The location of a pole in the parametric region.  
public:
  /// \ru Конструктор по умолчанию. \en Default constructor. 
  MbNurbsPointInfo()
    : point       (                 )
    , visible     ( true            )
    , poleLocation( (uint8)pln_None )
  {}
  /// \ru Конструктор по точке и флагу видимости. \en The constructor by a point and visibility flag. 
  MbNurbsPointInfo( MbCartPoint3D aPount, bool aVisible, MbePoleLocation aPoleLocation )
    : point       ( aPount               )
    , visible     ( aVisible             )
    , poleLocation( (uint8)aPoleLocation )
  {}
  /// \ru Конструктор копирования. \en The copy constructor. 
  MbNurbsPointInfo( const MbNurbsPointInfo & other )
    : point       ( other.point               )
    , visible     ( other.visible             )
    , poleLocation( (uint8)other.poleLocation )
  {}
   /// \ru Деструктор. \en Destructor. 
  ~MbNurbsPointInfo() {}

  /// \ru Инициализировать по точке и флагу видимости. \en Initialize by a point and visibility flag. 
  void Init( const MbNurbsPointInfo & other )
  { 
    point = other.point;
    visible = other.visible;
    poleLocation = other.poleLocation;
  }
  /// \ru Оператор присваивания. \en The assignment operator. 
  MbNurbsPointInfo & operator = ( const MbNurbsPointInfo & other )
  {
    point = other.point;
    visible = other.visible;
    poleLocation = other.poleLocation;
    return (*this);
  }
  /// \ru Определить, является ли точка полюсом. \en Define whether the point is a pole. 
  MbePoleLocation GetPoleLocation() { return (MbePoleLocation)poleLocation; }

  // \ru Закомментировала строчку ниже, поскольку объект служит только для передачи дополнительной информации об узлах сплайна \en Commented out the line below, because the object is used only to pass additional information about spline knots 
  // \ru В процессе прямого моделирования. Не пишется и не читается. \en In the direct modeling. Not written and not read. 
  // \ru KNOWN_OBJECTS_RW_REF_OPERATORS( MbNurbsPointInfo ) // для работы со ссылками и объектами класса \en KNOWN_OBJECTS_RW_REF_OPERATORS( MbNurbsPointInfo ) // for working with references and objects of the class 
}; //MbNurbsPointInfo


//------------------------------------------------------------------------------
/** \brief \ru Веса NURBS кривой.
           \en Weights of a NURBS curve. \~
  \details \ru Веса NURBS кривой.
           \en Weights of a NURBS curve. \~
  \ingroup Data_Structures
*/ // ---
class MATH_CLASS MbWeightVector {
private:
  size_t              m_count;        ///< \ru Количество весов контрольных точек.  \en A number of weights of the control points.
  double              m_commonWeight; ///< \ru Общий вес контрольных точек.         \en A common weight of control points. 
  c3d::DoubleVector   m_weights;      ///< \ru Множество весов контрольных точек.   \en Set of weights of the control points.
public:
  /// \ru Конструктор по умолчанию. \en Default constructor. 
  MbWeightVector() : m_count( 0 ), m_commonWeight( UNDEFINED_DBL ), m_weights() {}
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbWeightVector( const MbWeightVector & wv ) : m_count( wv.m_count ), m_commonWeight( wv.m_commonWeight ), m_weights( wv.m_weights ) {}
  ~MbWeightVector() {}

  /// \ru Количество весов. \en Number of weights.
  size_t  Count() const { return m_count; }
  /// \ru Количество весов. \en Number of weights.
  size_t  size() const { return m_count; }
  /// \ru Общий вес? \en Is it used a common weight?
  bool    IsCommonWeight() const {
    C3D_ASSERT( ( m_commonWeight != UNDEFINED_DBL ) == ( m_weights.size() < 1 ) );
    return ( m_commonWeight != UNDEFINED_DBL );
  }

  /// \ru Инициализация по общему весу. \en Initialize by common weight.
  bool  InitWeights( double wt, size_t cnt ) {
    ClearVector();
    m_count = cnt;
    m_commonWeight = wt;
    return ( m_commonWeight > -MB_MAXDOUBLE && m_commonWeight < MB_MAXDOUBLE );
  }

  /// \ru Инициализация по массиву весов. \en Initialize by weights array.
  template<class Vector>
  bool  InitWeights( const Vector & wts, size_t count = 0 ) {
    bool res = false;
    size_t wtsCount = wts.size();
    m_count = count == 0 ? wtsCount : count < wtsCount ? count : wtsCount;
    m_commonWeight = UNDEFINED_DBL;

    if ( CanAdjust(wts) ) {
      m_commonWeight = wts[0];
      ClearVector();
      res = true;
    }
    else {
      m_weights.assign( wts.begin(), wts.begin() + m_count );
      res = wts.size() > 0;
    }
    return res;
  }

  /// \ru  Инициализация по вектору весов. \en Initialize by weights vector.
  bool  InitWeights( const MbWeightVector & wv ) {
    m_count = wv.m_count;
    m_commonWeight = wv.m_commonWeight;
    m_weights = wv.m_weights;
    return true;
  }

  /// \ru Получить веса. \en Get weights array.
  template<class Vector>
  void  GetWeights( Vector & wts, bool clear = false ) const {
    C3D_ASSERT( (m_commonWeight != UNDEFINED_DBL) == (m_weights.size() < 1) );
    if ( clear )
      wts.clear();
    wts.reserve( m_count + wts.size() );
    if ( m_commonWeight != UNDEFINED_DBL )
      wts.resize( m_count + wts.size(), m_commonWeight);
    else
      std::copy( m_weights.begin(), m_weights.end(), std::back_inserter( wts ) );
  }
  /// \ru Установить веса. \en Get weights array.
  template<class Vector>
  bool  SetWeights( const Vector & wts ) { return InitWeights( wts ); }

  /// \ru Получить вес по индексу. \en Get weight by index.
  double   GetWeight( size_t index ) const { return index < m_weights.size() ? m_weights[index] : m_commonWeight; }
  /// \ru Установить вес по индексу. \en Set weight by index.
  bool     SetWeight( size_t index, double wt );
  /// \ru Добавить вес. \en Add weight.
  void     AddWeight( double wt );
  /// \ru Вставить вес. \en Insert weight.
  void     InsertWeight( size_t index, double wt );
  /// \ru Удалить вес по индексу. \en Remove weight at index.
  bool RemoveWeight( size_t index );
  /// \ru Удалить группу весов в интервале [index1, index2). \en Remove group of weights in the [index1, index2) interval.
  bool RemoveWeights( size_t index1, size_t index2 );

  /// \ru Получить общий вес (использовать после проверки IsCommonWeight). \en Get common weight (to be used after checking IsCommonWeight).
  double   GetCommonWeight() const { return m_commonWeight; }

  /// \ru Инвертировать веса. \en Reverse weights.
  void Reverse() { if ( m_commonWeight == UNDEFINED_DBL ) std::reverse( m_weights.begin(), m_weights.end() ); }
  /// \ru Сбросить данные. \en Flush data.
  void FlushData() { m_count = 0; m_commonWeight = UNDEFINED_DBL; ClearVector(); }

  /// \ru Можно ли оптимизировать расход памяти? \en Is it possible to adjust memory of weights array?
  template<class Vector>
  bool  CanAdjust( const Vector & wts, double eps = DOUBLE_REGION ) const {
    bool res = wts.size() > 0;
    if ( res ) {
      const double & w0 = wts[0];
      for ( size_t i = 1; res && i < m_count; i++ )
        res = ::fabs( wts[i] - w0 ) < eps;
    }
    return res;
  }
  /// \ru Оптимизировать расход памяти. \en Adjust memory of weights array.
  bool  Adjust( double eps = DOUBLE_REGION );

  /// \ru Оператор присваивания. \en Assignment operator.
  MbWeightVector & operator = ( const MbWeightVector & wv ) { InitWeights( wv ); return *this; }
  /// \ru Оператор присваивания. \en Assignment operator.
  template<class Vector>
  MbWeightVector & operator = ( const Vector & wv ) { InitWeights( wv ); return *this; }
  /// \ru Получить вес по индексу. \en Get weight by index.
  double   operator[]( size_t index ) const { return index < m_weights.size() ? m_weights[index] : m_commonWeight; }

  /// \ru Сравнение. \en Comparison.
  bool IsSame( const MbWeightVector & wv, double eps = DOUBLE_REGION ) const;
  /// \ru Сравнение. \en Comparison.
  template<class WeightVector>
  bool IsSame( const WeightVector & wv, double eps = DOUBLE_REGION ) const;

private:
  bool FillVector() {
    if ( m_count > 0 && m_commonWeight != UNDEFINED_DBL ) {
      m_weights.resize( m_count, m_commonWeight );
      m_commonWeight = UNDEFINED_DBL;
      return true;
    }
    return false;
  }
  void ClearVector() {
    if ( m_weights.size() ) {
      m_weights.clear();
      m_weights.shrink_to_fit();
    }
  }
  MbWeightVector( const c3d::DoubleVector & wv ); // Запрещено.
  MbWeightVector( const SArray<double> & wv );    // Запрещено.
};

//------------------------------------------------------------------------------
/** \brief \ru Дать меру расстояния.
           \en Get a measure of the distance. \~
  \details \ru Дать меру расстояния.
           \en Get a measure of the distance. \~
  \param[in] p1, p2 - \ru Точки между которыми ищется расстояние.
                      \en Points between which the distance is computed. \~
  \param[in] spType - \ru Тип параметризации сплайновых объектов.
                      \en The parametrization type of spline objects. \~
  \return \ru Расстояние.
          \en Distance. \~
  \ingroup Base_Algorithms
*/
// ---
template <class Type>
double GetParamDistance( const Type & p1, const Type & p2, MbeSplineParamType spType )
{
  switch ( spType ) {
    case spt_Unstated      :
    case spt_EquallySpaced :
      return 1.0;
    case spt_ChordLength :
      return p1.DistanceToPoint( p2 );
    case spt_Centripetal:
      return ::sqrt( p1.DistanceToPoint( p2 ) );
  }

  return 1.0;
}


//------------------------------------------------------------------------------
/** \brief \ru Проверить параметры инициализации nurbs-объекта.
           \en Check initialization parameters of a nurbs-object. \~
  \details \ru Проверить параметры инициализации nurbs-объекта.
           \en Check initialization parameters of a nurbs-object. \~
  \param[in] degree - \ru Порядок B-сплайна.
                      \en B-spline degree. \~
  \param[in] closed - \ru Признак замкнутости.
                      \en A closedness attribute. \~
  \param[in] pcnt -   \ru Число точек.
                      \en Number of points. \~
  \return \ru true, если параметры согласованы.
          \en Returns true if parameters are consistent. \~
  \ingroup Base_Algorithms
*/
// ---
inline
bool IsValidNurbsParams( size_t degree, bool closed, size_t pcnt )
{
  // \ru 1.  Порядок B-сплайна должен быть не менее 2.                          \en 1.  The order of B-spline must be at least 2. 
  // \ru 2а. Для незамкнутой кривой количество точек не меньше порядка сплайна. \en 2a. The number of open curve points isn't less than the order of spline. 
  // \ru 2б. Для замкнутой кривой должно быть как минимум 3 различных точки.    \en 2b. Closed curve must have at least 3 different points. 

  return ( (degree > 1) && (pcnt >= (closed ? 3 : degree)) );
}

//------------------------------------------------------------------------------
/** \brief \ru Проверить параметры инициализации nurbs-объекта.
           \en Check initialization parameters of a nurbs-object. \~
  \details \ru Проверить параметры инициализации nurbs-объекта.
           \en Check initialization parameters of a nurbs-object. \~
  \param[in] degree - \ru Порядок B-сплайна.
                      \en B-spline degree. \~
  \param[in] closed - \ru Признак замкнутости.
                      \en A closedness attribute. \~
  \param[in] pcnt -   \ru Число точек.
                      \en Number of points. \~
  \param[in] wcnt -   \ru Число весов.
                      \en Number of weights. \~
  \return \ru true, если параметры согласованы.
          \en Returns true if parameters are consistent. \~
  \ingroup Base_Algorithms
*/
// ---
inline
bool IsValidNurbsParams( size_t degree, bool closed, size_t pcnt, size_t wcnt )
{
  // \ru 1.  Порядок B-сплайна должен быть не менее 2.                          \en 1.  The order of B-spline must be at least 2. 
  // \ru 2а. Для незамкнутой кривой количество точек не меньше порядка сплайна. \en 2a. The number of open curve points isn't less than the order of spline. 
  // \ru 2б. Для замкнутой кривой должно быть как минимум 3 различных точки.    \en 2b. Closed curve must have at least 3 different points. 
  // \ru 3.  Количество точек и количество весов должны быть согласованы.       \en 3.  Number of points and number of weights must be equal. 

  bool res = ::IsValidNurbsParams( degree, closed, pcnt ) && (wcnt == pcnt);
  return res;
}

//------------------------------------------------------------------------------
/** \brief \ru Проверить параметры инициализации nurbs-объекта.
           \en Check initialization parameters of a nurbs-object. \~
  \details \ru Проверить параметры инициализации nurbs-объекта.
           \en Check initialization parameters of a nurbs-object. \~
  \param[in] degree - \ru Порядок B-сплайна.
                      \en B-spline degree. \~
  \param[in] closed - \ru Признак замкнутости.
                      \en A closedness attribute. \~
  \param[in] pcnt -   \ru Число точек.
                      \en Number of points. \~
  \param[in] wcnt -   \ru Число весов.
                      \en Number of weights. \~
  \param[in] kcnt -   \ru Число узлов.
                      \en Number of knots. \~
  \return \ru true, если параметры согласованы.
          \en Returns true if parameters are consistent. \~
  \ingroup Base_Algorithms
*/
// ---
inline
bool IsValidNurbsParams( size_t degree, bool closed, size_t pcnt, size_t wcnt, size_t kcnt )
{
  // \ru 1.  Порядок B-сплайна должен быть не менее 2.                          \en 1.  The order of B-spline must be at least 2. 
  // \ru 2а. Для незамкнутой кривой количество точек не меньше порядка сплайна. \en 2a. The number of open curve points isn't less than the order of spline. 
  // \ru 2б. Для замкнутой кривой должно быть как минимум 3 различных точки.    \en 2b. Closed curve must have at least 3 different points. 
  // \ru 3.  Количество точек и количество весов должны быть согласованы.       \en 3.  Number of points and number of weights must be equal. 
  // \ru 4.  Количество узлов должно быть согласовано по остальным параметрам.  \en 4.  Number of knots must be consistent with the other parameters. 

  bool res = ::IsValidNurbsParams( degree, closed, pcnt, wcnt ) &&
             ( kcnt == (degree + pcnt + (closed ? (degree - 1) : 0)) );
  return res;
}

//------------------------------------------------------------------------------
/** \brief \ru Проверить параметры инициализации nurbs-кривой.
           \en Check initialization parameters of a nurbs-curve. \~
  \details \ru Проверить параметры инициализации nurbs-кривой.
           \en Check initialization parameters of a nurbs-curve. \~
  \param[in] degree - \ru Порядок B-сплайна.
                      \en B-spline degree. \~
  \param[in] closed - \ru Признак замкнутости.
                      \en A closedness attribute. \~
  \param[in] pcnt -   \ru Число точек.
                      \en Number of points. \~
  \param[in] knots -  \ru Узловой вектор.
                      \en Knots vector. \~
  \return \ru true, если параметры согласованы.
          \en Returns true if parameters are consistent. \~
  \ingroup Base_Algorithms
*/
// ---
template <class KnotsVector>
bool IsValidNurbsParamsExt( size_t degree, bool closed, size_t pcnt,
                            const KnotsVector & knots )
{
  // \ru 1.  Порядок B-сплайна должен быть не менее 2.                          \en 1.  The order of B-spline must be at least 2. 
  // \ru 2а. Для незамкнутой кривой количество точек не меньше порядка сплайна. \en 2a. The number of open curve points isn't less than the order of spline. 
  // \ru 2б. Для замкнутой кривой должно быть как минимум 3 различных точки.    \en 2b. Closed curve must have at least 3 different points. 
  // \ru 3.  Количество точек и количество весов должны быть согласованы.       \en 3.  Number of points and number of weights must be equal. 
  // \ru 4.  Количество узлов должно быть согласовано по остальным параметрам.  \en 4.  Number of knots must be consistent with the other parameters. 

  bool res = ::IsValidNurbsParams( degree, closed, pcnt ) &&
             ( knots.size() == (degree + pcnt + (closed ? (degree - 1) : 0)) );

  if ( res ) {
    if ( !c3d::IsMonotonic( knots, true, true ) )
      res = false; // SD#7118498 
  }

  return res;
}

//------------------------------------------------------------------------------
/** \brief \ru Проверить параметры инициализации nurbs-кривой.
           \en Check initialization parameters of a nurbs-curve. \~
  \details \ru Проверить параметры инициализации nurbs-кривой.
           \en Check initialization parameters of a nurbs-curve. \~
  \param[in] degree - \ru Порядок B-сплайна.
                      \en B-spline degree. \~
  \param[in] closed - \ru Признак замкнутости.
                      \en A closedness attribute. \~
  \param[in] pnts -   \ru Точки.
                      \en Points vector. \~
  \param[in] wts -    \ru Веса точек.
                      \en Weights vector. \~
  \param[in] knots -  \ru Узловой вектор.
                      \en Knots vector. \~
  \return \ru true, если параметры согласованы.
          \en Returns true if parameters are consistent. \~
  \ingroup Base_Algorithms
*/
// ---
template <class PointVector, class DoubleVector, class WeightVector>
bool IsValidNurbsParamsExt( size_t degree, bool closed, const PointVector & pnts, 
                            const WeightVector * wts,
                            const DoubleVector * knots )
{
  // \ru 1.  Порядок B-сплайна должен быть не менее 2.                          \en 1.  The order of B-spline must be at least 2. 
  // \ru 2а. Для незамкнутой кривой количество точек не меньше порядка сплайна. \en 2a. The number of open curve points isn't less than the order of spline. 
  // \ru 2б. Для замкнутой кривой должно быть как минимум 3 различных точки.    \en 2b. Closed curve must have at least 3 different points. 
  // \ru 3.  Количество точек и количество весов должны быть согласованы.       \en 3.  Number of points and number of weights must be equal. 
  // \ru 4.  Количество узлов должно быть согласовано по остальным параметрам.  \en 4.  Number of knots must be consistent with the other parameters. 
  
  size_t pcnt = pnts.size();

  bool res = ::IsValidNurbsParams( degree, closed, pcnt ) &&
             ( (wts == nullptr) || (wts->size() == pcnt) ) &&
             ( (knots == nullptr) || (knots->size() == (degree + pcnt + (closed ? (degree - 1) : 0))) );

  if ( res && (knots != nullptr) ) {
    if ( !c3d::IsMonotonic( *knots, true, true ) )
      res = false; // SD#7118498 
  }

  return res;
}


//------------------------------------------------------------------------------
/** \brief \ru Проверить параметры инициализации nurbs-кривой.
           \en Check initialization parameters of a nurbs-curve. \~
  \details \ru Проверить параметры инициализации nurbs-кривой.
           \en Check initialization parameters of a nurbs-curve. \~
  \param[in] degree - \ru Порядок B-сплайна.
                      \en B-spline degree. \~
  \param[in] closed - \ru Признак замкнутости.
                      \en A closedness attribute. \~
  \param[in] pnts -   \ru Точки.
                      \en Points vector. \~
  \param[in] wts -    \ru Веса точек.
                      \en Weights vector. \~
  \param[in] knots -  \ru Узловой вектор.
                      \en Knots vector. \~
  \return \ru true, если параметры согласованы.
          \en Returns true if parameters are consistent. \~
  \ingroup Base_Algorithms
*/
// ---
template <class PointVector, class WeightVector>
bool IsValidNurbsParamsExt( size_t degree, bool closed, const PointVector & pnts, 
                            const WeightVector * wts )
{
  // \ru 1.  Порядок B-сплайна должен быть не менее 2.                          \en 1.  The order of B-spline must be at least 2. 
  // \ru 2а. Для незамкнутой кривой количество точек не меньше порядка сплайна. \en 2a. The number of open curve points isn't less than the order of spline. 
  // \ru 2б. Для замкнутой кривой должно быть как минимум 3 различных точки.    \en 2b. Closed curve must have at least 3 different points. 
  // \ru 3.  Количество точек и количество весов должны быть согласованы.       \en 3.  Number of points and number of weights must be equal. 
  // \ru 4.  Количество узлов должно быть согласовано по остальным параметрам.  \en 4.  Number of knots must be consistent with the other parameters. 
  
  size_t pcnt = pnts.size();
  return ::IsValidNurbsParams( degree, closed, pcnt ) && ( (wts == nullptr) || (wts->size() == pcnt) );
}


//------------------------------------------------------------------------------
/** \brief \ru Проверить узловой вектор nurbs-объекта.
           \en Check knots vector of a nurbs-object. \~
  \details \ru Проверить узловой вектор nurbs-объекта.
           \en Check knots vector of a nurbs-object. \~
  \param[in] knots - \ru Узловой вектор.
                     \en Knots vector. \~
  \return \ru true, если параметры согласованы.
          \en Returns true if parameters are consistent. \~
  \ingroup Base_Algorithms
*/
// ---
template <class KnotsVector>
bool IsValidNurbsKnots( const KnotsVector & knots, double eps = EXTENT_EPSILON )
{
  // \ru 1 . Количество узлов должно быть не менее 4 \en 1 . The number of knots must be at least 4
  // \ru 2 . Последовательность узлов должна быть неубывающей \en 2 . Sequence of nodes must be nondecreasing
  // \ru 3 . Первый и последний узлы должны быть различны \en 3 . The first and the last nodes must be different

  size_t cnt = knots.size();
  if ( cnt < 4 )
    return false;

  eps = ::fabs(eps);
  if ( ::fabs( knots.back() - knots.front() ) < eps )
    return false;

  size_t maxInd = cnt - 1;
  for ( size_t i = 0; i < maxInd; ++i ) {
    if ( knots[i+1] < knots[i] - eps )
      return false;
  }

  return true;
}


//------------------------------------------------------------------------------
/** \brief \ru Проверить и попытаться починить обрезанный узловой вектор nurbs-объекта.
           \en Check and try to restore knots vector of a nurbs-object. \~
  \details \ru Проверить и попытаться починить обрезанный узловой вектор nurbs-объекта.
           \en Check and try to restore knots vector of a nurbs-object. \~
  \param[in] degree - \ru Порядок B-сплайна.
                      \en B-spline degree. \~
  \param[in] closed - \ru Признак замкнутости.
                      \en A closedness attribute. \~
  \param[in] pointsCnt - \ru Количество точек.
                         \en Number of points. \~
  \param[in,out] knots - \ru Узловой вектор.
                         \en Knots vector. \~
  \return \ru true, если параметры согласованы.
          \en Returns true if parameters are consistent. \~
  \ingroup Base_Algorithms
*/
// ---
template <class KnotsVector>
bool RestoreClippedKnots( size_t degree, bool closed, size_t pointsCnt, KnotsVector & knots )
{
  bool isValid = false;

  if ( ::IsValidNurbsParamsExt( degree, closed, pointsCnt, knots ) )
    isValid = true;
  else if ( !closed ) {
    if ( IsValidNurbsParams( degree, closed, pointsCnt ) ) {
      if ( c3d::IsMonotonic( knots, true, true ) ) {
        const double knotEps = EXTENT_EQUAL;

        KnotsVector initKnots;
        std::copy( knots.begin(), knots.end(), std::back_inserter( initKnots ) );

        const double begKnot = knots.front();
        size_t begCnt = 1;
        {
          bool check = true;
          for ( size_t i = 1; i < degree && check; ++i ) {
            check = false;
            if ( ::fabs( begKnot - knots[i] ) < knotEps ) {
              check = true;
              begCnt++;
            }
          }
        }

        const double endKnot = knots.back();
        size_t endCnt = 1;
        {
          bool check = true;
          size_t knotsCnt = knots.size();
          for ( size_t i = 1; i < degree && check; ++i ) {
            check = false;
            if ( ::fabs( endKnot - knots[knotsCnt - i - 1] ) < knotEps ) {
              check = true;
              endCnt++;
            }
          }
        }

        if ( begCnt < degree ) {
          while ( begCnt < degree ) {
            knots.insert( knots.begin(), begKnot );
            begCnt++;
          }
        }
        if ( endCnt < degree ) {
          while ( endCnt < degree ) {
            knots.push_back( endKnot );
            endCnt++;
          }
        }

        if ( ::IsValidNurbsParamsExt( degree, closed, pointsCnt, knots ) )
          isValid = true;
        else {
          knots.clear();
          std::copy( initKnots.begin(), initKnots.end(), std::back_inserter( knots ) );
        }
      }
    }
  }

  return isValid;
}


//------------------------------------------------------------------------------
/** \brief \ru Определение индекса узла left для первой ненулевой функции.
           \en Definition of "left" knot index for the first non-zero function. \~
  \details \ru Определение индекса узла left для первой ненулевой функции (knots[mid] <= t < knots[mid + 1]).
           \en Definition of "left" knot index for the first non-zero function (knots[mid] <= t < knots[mid + 1]). \~
  \param[in] degree - \ru Порядок B-сплайна.
                      \en B-spline degree. \~
  \param[in] knots - \ru Множество узлов.
                     \en Knots. \~
  \param[in, out] t - \ru Значение параметра.
                      \en A parameter value. \~
  \return \ru Индекс узла.
          \en A knot index. \~
  \ingroup Base_Algorithms
*/
// ---
template <class KnotsVector>
ptrdiff_t KnotIndex( size_t degree, const KnotsVector & knots, double & t )
{
  size_t low  = degree - 1;
  size_t high = knots.size() - degree;
  ptrdiff_t mid  = low;
  double knot_low = knots[low];
  double knot_high = knots[high];

  if ( t <= knot_low ) {
    t = knot_low;
    size_t countKnt = knots.size();
    size_t lowP = low;
    lowP++;
    // \ru Исправление ошибки BUG_21185 while ( (lowP < countKnt) && (t == knots[lowP]) ) { \en Bugfix BUG_21185 while ( (lowP < countKnt) && (t == knots[lowP]) ) { 
    while ( (lowP + degree < countKnt) && (t == knots[lowP]) ) { //-V550
      low = lowP;
      lowP++;
    }
    mid = low;
  }
  else if ( t >= knot_high ) {
    t = knot_high;
    // BUG_82980 while ( (high > 0) && (knots[high] == t) ) {
    while ( (high + 1 > degree) && (knots[high] == t) ) {
      high--;
    }
    mid = high;
  }
  else { 
    // \ru Поиск выполнятеся быстрее, если значение индекса близко к искомому.
    // \en The search perform faster if the index value close to the one looking for.
    size_t count = high - low;
    if ( count > c3d::LIMIT_COUNT ) {
      double average = ( knot_high - knot_low ) / count;
      if ( average > NULL_EPSILON )
        mid = (ptrdiff_t)( (t - knot_low) / average );
    }
    if ( c3d::ArFind(knots, t, mid) ) {
      while ( knots[mid] == t ) {
        mid++;      
      }
      mid--;
    }
  }
  return mid;
}


//------------------------------------------------------------------------------
/** \brief \ru Определить узловой вектор (равномерная параметризация).
           \en Define knot vector (uniform parameterization). \~
  \details \ru Определить узловой вектор (равномерная параметризация).
           \en Define knot vector (uniform parameterization). \~
  \param[in] degree - \ru Порядок B-сплайна.
                      \en B-spline degree. \~
  \param[in] closed - \ru Признак замкнутости.
                      \en A closedness attribute. \~
  \param[in] uppPointsIndex - \ru Индекс последней точки.
                              \en Last point index. \~
  \param[in] knots - \ru Узловой вектор.
                     \en Knots vector. \~
  \ingroup Base_Algorithms
*/
// ---
template <class KnotsVector>
ptrdiff_t DefineKnotsVector( ptrdiff_t degree, bool closed, ptrdiff_t uppPointsIndex,
                             KnotsVector & knots )
{
  if ( (degree < 2) || (uppPointsIndex < 1) ) {
    knots.clear();
    return -1;
  }

  ptrdiff_t pointsCount = uppPointsIndex + 1;
  ptrdiff_t power = degree - 1;

  ptrdiff_t knotsCount = degree + pointsCount + (closed ? power : 0);
  knots.clear();
  knots.reserve( knotsCount );

  ptrdiff_t i = 0;

  if ( closed ) { // замкнутый В-сплайн  
    double knot = 0.0;
    for ( i = 0; i < knotsCount; i++ ) {
      knot = (double)(i - power);
      knots.push_back( knot );
    }
  }
  else {
    double knot = 0.0;
    for ( i = 0; i < degree; i++ )
      knots.push_back( knot );

    ptrdiff_t cnt = pointsCount - degree;
    for ( i = 0; i < cnt; i++ ) {
      knot += 1.0;
      knots.push_back( knot );
    }

    knot += 1.0;
    for ( i = 0; i < degree; i++ )
      knots.push_back( knot );
  }

  return (knotsCount - 1);
}


//------------------------------------------------------------------------------
/** \brief \ru Определить узловой вектор.
           \en Define knot vector. \~
  \details \ru Определить узловой вектор.
           \en Define knot vector. \~
  \param[in] degree - \ru Порядок B-сплайна.
                      \en B-spline degree. \~
  \param[in] closed - \ru Признак замкнутости.
                      \en A closedness attribute. \~
  \param[in] count  - \ru Количество точек.
                      \en Number of points. \~
  \param[in] params - \ru Параметры точек (для замкнутого count+1).
                      \en Points parameters (for closed spline - "count"+1). \~
  \param[in] kMode  - \ru Способ построения узлового вектора при наличии параметров.
                      \en A method for constructing a knot vector in the presence of parameters. \~
  \param[out] knots - \ru Узловой вектор.
                      \en Knots vector. \~
  \return \ru true, если набор параметров сформирован.
          \en Returns true if result is success. \~
  \ingroup Base_Algorithms
*/
// ---
template <class ParamsVector, class KnotsVector>
bool DefineKnotsVector(       size_t         degree, 
                              bool           closed, 
                              size_t         count,
                        const ParamsVector * paramsPtr,
                              size_t         kMode,
                              KnotsVector &  knots )
{
  bool isDone = false;

  if ( degree > 1 && count >= (closed ? 3 : degree) ) {
    knots.clear();
    size_t knotsCount = (degree + count + (closed ? (degree - 1) : 0));
    knots.reserve( knotsCount );

    double knot = 0.0;

    if ( (paramsPtr == nullptr) || (*paramsPtr).empty() ) { // Вектор параметров не задан.
      // Назначаеттся (по умолчанию) целочисленный узловой вектор.
      for ( size_t i = 0; i < knotsCount; ++i ) {
        if ( closed ) // Узловой вектор замкнутого сплайна.
          knot = (double)(ptrdiff_t)(i - degree + 1);
        else { // Узловой вектор незамкнутого сплайна.
          if ( i < degree )
            knot = 0.0;
          else if ( i < count + 1 )
            knot = (knots[i - 1] + 1.0);
          else
            knot = knots[i - 1];
        }
        knots.push_back( knot );
      }
      isDone = true;
    }
    else if ( paramsPtr->size() == count + (closed ? 1 : 0) ) { // Задан вектор параметров.
      const ParamsVector & params = (*paramsPtr);

      double pmin = params.front();
      double pmax = params.back();
      double prng = pmax - pmin;

      // Узловой вектор связывается с заданными параметрами.
      if ( closed ) { // Узловой вектор замкнутого сплайна.
        /* BUG_45280
        for ( size_t i = 0; i < knotsCount; ++i ) {
          knot = pmin + (prng / count) * (i - degree + 1);
          knots.push_back( knot );
        } */

        size_t cnt = count + 1;

        for ( size_t i = 0; i < knotsCount; ++i ) {
          ptrdiff_t ind = (ptrdiff_t)(i - degree + 1);

          if ( i < degree )
            knot = params[(ind + count) % cnt] - prng;
          else if ( i < count + degree )
            knot = params[ind];
          else
            knot = prng + params[(ind + 1) % cnt];

          knots.push_back( knot );
        }
      }
      else { // Узловой вектор незамкнутого сплайна.
        size_t d_2 = degree % 2;
        size_t i_2 = degree / 2;
        for ( size_t i = 0; i < knotsCount; ++i ) {
          if ( i < degree )
            knot = pmin;
          else if ( i < count ) {
            if ( kMode == 0 ) { // \ru Усреднение ближайших параметров в узловом векторе. \en Averaging of the nearest parameters for the knots vector.
              knot = 0.0;
              for ( size_t j = i - degree + 1; j < i; ++j )
                knot += params[j];
              knot /= (double)(degree - 1);
            }
            else { // \ru Использование параметров в узловом векторе. \en Using parameters for the knots vector.
              if ( d_2 == 0 )
                knot = params[i - i_2];
              else
                knot = ( params[i - 1 - i_2] + params[i - i_2] ) * c3d::ONE_HALF;
            }
          }
          else
            knot = pmax;

          knots.push_back( knot );
        }
        if ( kMode == 2 ) { // Поправить узловой вектор. 
          if ( degree > 1 && knotsCount >= 3 * degree ) {
            double a_2 = (double)i_2;
            for ( size_t i = degree; i < degree + i_2; ++i ) { // Узлы в начале.
              double d = ( knots[i+1] - knots[i] ) * ( a_2 - (i - degree) ) / ( a_2 + 1 );
              knots[i] += d;
            }
            ptrdiff_t uppind = knotsCount - 1 - degree;
            for ( ptrdiff_t i = uppind; i > (ptrdiff_t)(uppind - i_2); --i ) { // Узлы в конце.
              double d = ( knots[i] - knots[i-1] ) * ( a_2 + (i - uppind) ) / ( a_2 + 1 );
              knots[i] -= d;
            }
          }
        }
      }
      isDone = true;
    }
    else {
      C3D_ASSERT_UNCONDITIONAL( false );
    }
  }

  return isDone;
}


//------------------------------------------------------------------------------
/** \brief \ru Определить параметрическое распределение точек.
           \en Define parametric distribution of points. \~
  \details \ru Определить параметрическое распределение точек.
           \en Define parametric distribution of points. \~
  \param[in] points - \ru Массив точек.
                      \en Points vector. \~
  \param[in] spType - \ru Тип параметризации сплайновых объектов.
                      \en The parameterization type of spline objects. \~
  \param[in] closed - \ru Признак замкнутости.
                      \en A closedness attribute. \~
  \param[out] params - \ru Параметрическое распределение точек.
                       \en Parametric distribution of points. \~
  \return \ru true, если набор параметров сформирован.
          \en Returns true if result is success. \~
  \ingroup Base_Algorithms
*/
// ---
template <class PointsVector, class ParamsVector>
bool DefineThroughPointsParams( const PointsVector & points, MbeSplineParamType spType, bool closed,
                                ParamsVector & params )
{
  bool res = false;

  ptrdiff_t count = (ptrdiff_t)points.size();

  if ( count > (closed ? 2 : 1) ) {
    double param = 0.0;
    ptrdiff_t extCount = closed ? (count + 1) : count;
    params.clear();
    params.resize( extCount, param );

    double paramSum = 0.0;
    for ( ptrdiff_t j = 1; j < extCount; j++ ) {
      ptrdiff_t jp = (j - 1 + count) % count;
      ptrdiff_t jc = j % count;
      param = ::GetParamDistance( points[jp], points[jc], spType );
      params[j] = ( params[j-1] + param );
      paramSum += param;
    }

    if ( paramSum > METRIC_PRECISION ) {
      for ( ptrdiff_t j = 0; j < extCount; j++ ) {
        params[j] /= paramSum;
        params[j] *= (double)(extCount-1);
      }
      res = true;
    }
    else if ( spType != spt_EquallySpaced ) {
      C3D_ASSERT_UNCONDITIONAL( false );
      paramSum = 0.0;
      for ( ptrdiff_t j = 1; j < extCount; j++ ) {
        ptrdiff_t jp = (j - 1 + count) % count;
        ptrdiff_t jc = j % count;
        param = ::GetParamDistance( points[jp], points[jc], spt_EquallySpaced );
        params[j] = ( params[j-1] + param );
        paramSum += param;
      }
      if ( paramSum > METRIC_PRECISION ) {
        for ( ptrdiff_t j = 0; j < extCount; j++ ) {
          params[j] /= paramSum;
          params[j] *= (double)(extCount-1);
        }
        res = true;
      }
    }
  }

  return res;
}


//------------------------------------------------------------------------------
/** \brief \ru Вычисление B - базиса (degree - порядок B-сплайна, p = (degree - 1) - степень полинома(B-сплайна)).
           \en The calculation of B - basis ("degree" - order of B-spline, p = (degree - 1) - the degree of the polynomial (B-spline)). \~
  \details \ru Вычисление B - базиса (degree - порядок B-сплайна, p = (degree - 1) - степень полинома(B-сплайна)).
               Для ускорения используется рабочий вектор lr = { left[p+1], right[p+1] } ).
           \en The calculation of B - basis ("degree" - order of B-spline, p = (degree - 1) - the degree of the polynomial (B-spline)). 
               To speed up vector lr = { left[p+1], right[p+1] } is used. \~
  \param[in] i - \ru Индекс в массиве узлов, получаемый с помощью функции KnotIndex().
                 \en Index of knots vector obtained by the function KnotIndex(). \~
  \param[in] t - \ru Параметр.
                 \en Parameter. \~
  \param[in] p - \ru p = degree - 1, где degree - порядок B-сплайна.
                 \en p = degree - 1, where degree is B-spline degree. \~
  \param[in] knots - \ru Узловой вектор.
                     \en Knots vector. \~
  \param[out] nsplines - \ru Массив размерности degree, заполняется значениями сплайна в поля 0..degree-2; nsplines[degree-1] = 0.
                         \en Array of B-spline values. \~
  \param[in,out] lrVect - \ru Массив размерности 2*(p+1) = 2*degree, содержимое игнорируется и будет перезаписано. Нужен для ускорения работы функции. В результате работы в нем останется мусор..
                          \en Temporary working vector with dimension 2*(p+1) = 2*degree. \~
  \ingroup Base_Algorithms
*/
// ---
template <class KnotsVector, class DoubleVector>
bool BasisFuns( ptrdiff_t i, double t, ptrdiff_t p, const KnotsVector & knots, DoubleVector & nsplines,
                DoubleVector & lrVect )
{
  C3D_ASSERT( (p > 0) && (i >= 0 && i < (ptrdiff_t)knots.size()) );

  if ( (p > 0) && (i >= 0 && i < (ptrdiff_t)knots.size()) ) {
    ptrdiff_t d = p + 1;

    if ( nsplines.size() != (size_t)d )
      nsplines.resize( d );

    size_t d2x = 2 * d;
    if ( lrVect.size() < d2x )
      lrVect.resize( d2x );

    nsplines[0] = 1.0;

    for ( ptrdiff_t j = 1; j < d; j++ ) { // внешний цикл
      ptrdiff_t jm1 = j - 1;
      double saved = 0.0;
      double temp = 0.0;

      lrVect[j] = t - knots[i - jm1];
      lrVect[j + d] = knots[i + j] - t;

      for ( ptrdiff_t r = 0; r < j; r++ ) { // внутренний цикл
        double & lcur = lrVect[j - r];
        double & rcur = lrVect[r + 1 + d];
        double & nspline = nsplines[r];
        temp = (rcur + lcur);

        if ( temp != 0.0 ) { //-V550
          temp = nspline / temp;
          nspline = rcur * temp;
          nspline += saved;
          saved = lcur * temp;
        }
        else {
          C3D_ASSERT_UNCONDITIONAL( false );
          nspline = 0.0;
        }
      }
      nsplines[j] = saved;
    }

    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
/** \brief \ru Вычислить базисный сплайн по параметру t и узловому вектору.
           \en Calculate basic spline by "t" parameter and knots vector. \~
  \details \ru Вычислить базисный сплайн по параметру t и узловому вектору.
           \en Calculate basic spline by "t" parameter and knots vector. \~
  \ingroup Base_Algorithms
*/
// ---
template <class DoubleVector>
bool CalcBsplvb( const DoubleVector & knots, double t, ptrdiff_t leftIndex, ptrdiff_t degree,
                 DoubleVector & biatx, DoubleVector & lrVect )
{
  return ::BasisFuns( leftIndex, t, degree-1, knots, biatx, lrVect );
}


//------------------------------------------------------------------------------
/** \brief \ru Вычислить все разностные формы кривой.
           \en Calculate all difference forms of curve. \~
  \details \ru Вычислить все (или опционально некоторые) разностные формы кривой (характеристические производные).
           \en Calculate all (or some, optionally) difference forms of curve (characteristic derivatives). \~
  \ingroup Base_Algorithms
*/
// ---
template <class Point, class NurbsVector, class KnotsVector>
void CurveDeriveCpts( ptrdiff_t p, const KnotsVector & U, const Point * P, const double * W, size_t pointCount,
                      const NurbsVector * PW, ptrdiff_t d, ptrdiff_t r1, ptrdiff_t r2, NurbsVector * PK )
{
  C3D_ASSERT( (P != nullptr && W != nullptr) != (PW != nullptr) );

  ptrdiff_t r = ( r2 - r1 );
  ptrdiff_t degree = ( p + 1 );
  ptrdiff_t i, k, icount;
  NurbsVector & PK0 = PK[0];

  if ( PW != nullptr ) {
    for ( i = 0; i <= r; i++ )
      PK0.Set( i, *PW, (r1 + i) );
  }
  else {
    if ( !PK0.UseWeights() && ( r1 + r ) < pointCount ) {
      for ( i = 0; i <= r; i++ ) {
        PK0[i] = P[r1 + i];
      }
    }
    else {
      for ( i = 0; i <= r; i++ ) {
        k = ( ( r1 + i ) % pointCount );
        PK0.Init( i, P[k], W[k] );
      }
    }
  }

  for ( k = 1; k <= d; k++ ) {
    NurbsVector & PKMin = PK[k - 1];
    NurbsVector & PKPls = PK[k];
    double tmp = (double)( degree - k );
    ptrdiff_t r1i = r1;
    for ( i = 0, icount = (r - k); i <= icount; i++ ) {
      r1i = ( r1 + i );
      C3D_ASSERT( ( ::fabs( U[r1i + degree] - U[r1i + k] ) > DOUBLE_EPSILON ) ); // \ru Проверка на сбой \en Check of failure 
      PKPls.Dec( i, PKMin, i, PKMin, (i + 1), (tmp / (U[r1i + degree] - U[r1i + k])) );
    }
  }
}

//------------------------------------------------------------------------------
/** \brief \ru Вычислить все разностные формы кривой.
           \en Calculate all difference forms of curve. \~
  \details \ru Вычислить все (или опционально некоторые) разностные формы кривой (характеристические производные).
           \en Calculate all (or some, optionally) difference forms of curve (characteristic derivatives). \~
  \ingroup Base_Algorithms
*/
// ---
template <class Point, class NurbsVector, class KnotsVector>
void CurveDeriveCpts( ptrdiff_t p, const KnotsVector & U, const Point * P, const double w, size_t pointCount,
                      const NurbsVector * PW, ptrdiff_t d, ptrdiff_t r1, ptrdiff_t r2, NurbsVector * PK )
{
  C3D_ASSERT( (P != nullptr) != (PW != nullptr) );

  ptrdiff_t r = ( r2 - r1 );
  ptrdiff_t degree = ( p + 1 );
  ptrdiff_t i, k, icount;
  NurbsVector & PK0 = PK[0];

  if ( PW != nullptr ) {
    for ( i = 0; i <= r; i++ )
      PK0.Set( i, *PW, (r1 + i) );
  }
  else {
    if ( !PK0.UseWeights() && ( r1 + r ) < pointCount ) {
      for ( i = 0; i <= r; i++ ) {
        PK0[i] = P[r1 + i];
      }
    }
    else {
      for ( i = 0; i <= r; i++ ) {
        k = ( ( r1 + i ) % pointCount );
        PK0.Init( i, P[k], w );
      }
    }
  }

  for ( k = 1; k <= d; k++ ) {
    NurbsVector & PKMin = PK[k - 1];
    NurbsVector & PKPls = PK[k];
    double tmp = (double)( degree - k );
    ptrdiff_t r1i = r1;
    for ( i = 0, icount = (r - k); i <= icount; i++ ) {
      r1i = ( r1 + i );
      C3D_ASSERT( ( ::fabs( U[r1i + degree] - U[r1i + k] ) > DOUBLE_EPSILON ) ); // \ru Проверка на сбой \en Check of failure 
      PKPls.Dec( i, PKMin, i, PKMin, (i + 1), (tmp / (U[r1i + degree] - U[r1i + k])) );
    }
  }
}

//------------------------------------------------------------------------------
  /** \brief \ru Вычислить все разностные формы кривой.
             \en Calculate all difference forms of curve. \~
    \details \ru Вычислить все (или опционально некоторые) разностные формы кривой (характеристические производные).
             \en Calculate all (or some, optionally) difference forms of curve (characteristic derivatives). \~
    \ingroup Base_Algorithms
*/
// ---
template <class Point, class NurbsVector, class KnotsVector, class DoubleTriple>
void CurveDeriveCpts( ptrdiff_t p, const KnotsVector & U, const Point * P, const double * W, size_t pointCount,
                      const NurbsVector * PW, ptrdiff_t d, ptrdiff_t r1, ptrdiff_t r2, DoubleTriple ** DT, double ** WT )
{
  C3D_ASSERT( ( P != nullptr && W != nullptr ) != ( PW != nullptr ) );

  ptrdiff_t r = ( r2 - r1 );
  ptrdiff_t degree = ( p + 1 );
  ptrdiff_t i, k, icount;
  DoubleTriple * DT0 = DT[0];
  double * WT0 = WT[0];
  bool useWeight = WT0 != nullptr;

  if ( PW != nullptr ) {
    if ( !useWeight ) {
      for ( i = 0; i <= r; i++ )
        DT0[i].Init( (*PW)[r1 + i] );
    }
    else {
      for ( i = 0; i <= r; i++ ) {
        DT0[i].Init( (*PW)[r1 + i] );
        WT0[i] = PW->w( r1 + i );
      }
    }
  }
  else {
    if ( !useWeight && ( r1 + r ) < (ptrdiff_t)pointCount ) {
      for ( i = 0; i <= r; i++ ) {
        DT0[i].Init( P[r1 + i].x, P[r1 + i].y, P[r1 + i].z ); // SKIP_SA
      }
    }
    else {
      for ( i = 0; i <= r; i++ ) {
        k = ( ( r1 + i ) % pointCount );
        DT0[i].Init( P[k], W[k] ); // SKIP_SA
        if ( useWeight )
          WT0[i] = W[k];
      }
    }
  }

  double * WTMin = nullptr;
  double * WTPls = nullptr;
  for ( k = 1; k <= d; k++ ) {
    DoubleTriple * DTMin = DT[k - 1];
    DoubleTriple * DTPls = DT[k];
    if ( useWeight ) {
      WTMin = WT[k - 1];
      WTPls = WT[k];
    }
    
    double tmp = (double)( degree - k );
    ptrdiff_t r1i = r1;
    for ( i = 0, icount = ( r - k ); i <= icount; i++ ) {
      r1i = ( r1 + i );
      double tmp2 = U[r1i + degree] - U[r1i + k];
      C3D_ASSERT( ( ::fabs( tmp2 ) > DOUBLE_EPSILON ) ); // \ru Проверка на сбой \en Check of failure 
      DTPls[i].Dec( DTMin[i], DTMin[i + 1], tmp / (tmp2) );
      if ( useWeight )
        WTPls[i] = ( WTMin[i + 1] - WTMin[i] ) * ( tmp / (tmp2) );
    }
  }
}

//------------------------------------------------------------------------------
  /** \brief \ru Вычислить все разностные формы кривой.
             \en Calculate all difference forms of curve. \~
    \details \ru Вычислить все (или опционально некоторые) разностные формы кривой (характеристические производные).
             \en Calculate all (or some, optionally) difference forms of curve (characteristic derivatives). \~
    \ingroup Base_Algorithms
*/
// ---
template <class Point, class NurbsVector, class KnotsVector, class DoubleTriple>
void CurveDeriveCpts( ptrdiff_t p, const KnotsVector & U, const Point * P, const double w, size_t pointCount,
                      const NurbsVector * PW, ptrdiff_t d, ptrdiff_t r1, ptrdiff_t r2, DoubleTriple ** DT, double ** WT )
{
  C3D_ASSERT( ( P != nullptr ) != ( PW != nullptr ) );

  ptrdiff_t r = ( r2 - r1 );
  ptrdiff_t degree = ( p + 1 );
  ptrdiff_t i, k, icount;
  DoubleTriple * DT0 = DT[0];
  double * WT0 = WT[0];
  bool useWeight = WT0 != nullptr;

  if ( PW != nullptr ) {
    if ( !useWeight ) {
      for ( i = 0; i <= r; i++ )
        DT0[i].Init( (*PW)[r1 + i] );
    }
    else {
      for ( i = 0; i <= r; i++ ) {
        DT0[i].Init( (*PW)[r1 + i] );
        WT0[i] = PW->w( r1 + i );
      }

    }
  }
  else {
    if ( !useWeight && ( r1 + r ) < (ptrdiff_t)pointCount ) {
      for ( i = 0; i <= r; i++ ) {
        DT0[i].Init( P[r1 + i].x, P[r1 + i].y, P[r1 + i].z );
      }
    }
    else {
      for ( i = 0; i <= r; i++ ) {
        k = ( ( r1 + i ) % pointCount );
        DT0[i].Init( P[k], w );
        if( useWeight )
          WT0[i] = w;
      }
    }
  }

  double * WTMin = nullptr;
  double * WTPls = nullptr;
  for ( k = 1; k <= d; k++ ) {
    DoubleTriple * DTMin = DT[k - 1];
    DoubleTriple * DTPls = DT[k];
    if ( useWeight ) {
      WTMin = WT[k - 1];
      WTPls = WT[k];
    }
    double tmp = (double)( degree - k );
    ptrdiff_t r1i = r1;
    for ( i = 0, icount = ( r - k ); i <= icount; i++ ) {
      r1i = ( r1 + i );
      double tmp2 = U[r1i + degree] - U[r1i + k];
      C3D_ASSERT( ( ::fabs( tmp2 ) > DOUBLE_EPSILON ) ); // \ru Проверка на сбой \en Check of failure 
      DTPls[i].Dec( DTMin[i], DTMin[i + 1], tmp / (tmp2) );
      if ( useWeight )
        WTPls[i] = ( WTMin[i + 1] - WTMin[i] ) * ( tmp / ( tmp2 ) );
    }
  }
}

//------------------------------------------------------------------------------
/** \brief \ru Вычислить все разностные формы кривой.
           \en Calculate all difference forms of curve. \~
  \details \ru Вычислить все (или опционально некоторые) разностные формы кривой (характеристические производные).
           \en Calculate all (or some, optionally) difference forms of curve (characteristic derivatives). \~
  \ingroup Base_Algorithms
*/
// ---
template <class Point, class Homogeneous>
void CurveDeriveCpts( ptrdiff_t p, const double * U, const Point * P, const double * W, size_t pointCount,
                      ptrdiff_t r1, ptrdiff_t r2, 
                      Homogeneous * H0, Homogeneous * H1, Homogeneous * H2, Homogeneous * H3 )
{
  ptrdiff_t r = ( r2 - r1 );
  ptrdiff_t degree = ( p + 1 );
  ptrdiff_t i, k, icount;

  for ( i = 0; i <= r; i++ ) {
    k = ( (r1 + i) % pointCount );
    if ( W != nullptr )
      H0[i].Init( P[k], W[k] );
    else
      H0[i].Init( P[k], 1.0 );
  }
  k = 1;
  {
    Homogeneous * PKMin = H0;
    Homogeneous * PKPls = H1;
    double tmp = (double)( degree - k );
    ptrdiff_t r1i = r1;
    for ( i = 0, icount = (r - k); i <= icount; i++ ) {
      r1i = ( r1 + i );
      C3D_ASSERT( ( ::fabs( U[r1i + degree] - U[r1i + k] ) > DOUBLE_EPSILON ) ); // \ru Проверка на сбой \en Check of failure 
      PKPls[i].Dec( PKMin[i], PKMin[i + 1], (tmp / (U[r1i + degree] - U[r1i + k])) );
    }
  }
  k = 2;
  {
    Homogeneous * PKMin = H1;
    Homogeneous * PKPls = H2;
    double tmp = (double)( degree - k );
    ptrdiff_t r1i = r1;
    for ( i = 0, icount = (r - k); i <= icount; i++ ) {
      r1i = ( r1 + i );
      C3D_ASSERT( ( ::fabs( U[r1i + degree] - U[r1i + k] ) > DOUBLE_EPSILON ) ); // \ru Проверка на сбой \en Check of failure 
      PKPls[i].Dec( PKMin[i], PKMin[i + 1], (tmp / (U[r1i + degree] - U[r1i + k])) );
    }
  }
  k = 3;
  {
    Homogeneous * PKMin = H2;
    Homogeneous * PKPls = H3;
    double tmp = (double)( degree - k );
    ptrdiff_t r1i = r1;
    for ( i = 0, icount = (r - k); i <= icount; i++ ) {
      r1i = ( r1 + i );
      C3D_ASSERT( ( ::fabs( U[r1i + degree] - U[r1i + k] ) > DOUBLE_EPSILON ) ); // \ru Проверка на сбой \en Check of failure 
      PKPls[i].Dec( PKMin[i], PKMin[i + 1], (tmp / (U[r1i + degree] - U[r1i + k])) );
    }
  }
}


//------------------------------------------------------------------------------
/// \ru Загнать параметр t в параметрическую область кривой. \en Reduce "t" to parameter in the parametric domain of the curve. 
/**
  \param[in] tMin, tMax - \ru Параметры, задающие параметрическую область кривой.
                          \en Parameters which define the parametric region of the curve. \~
  \param[in] closed - \ru Признак замкнутости кривой.
                      \en An attribute of curve closedness. \~
  \param[in, out] t - \ru Исходный параметр.
                      \en Initial parameter. \~
  \ingroup Base_Algorithms
*/
// ---
inline
void CheckParam( const double & tMin, const double & tMax, bool closed, double & t ) 
{
  if ( (t < tMin) || (t > tMax) ) {
    if ( closed ) { // \ru Сплайн кривая замкнута \en Spline curve is closed 
      double period = tMax - tMin;
      t -= ::floor( (t - tMin) / period ) * period;
    }
    else if ( t < tMin ) 
      t = tMin;
    else if ( t > tMax ) 
      t = tMax;
  }
}


//------------------------------------------------------------------------------
/// \ru Рассчитать ненулевые сплайны при данном параметре. \en Calculate non-zero splines with a given parameter. 
/**
  \param[in] degree - \ru Порядок B-сплайна.
                      \en B-spline degree. \~
  \param[in] knots - \ru Множество узлов.
                     \en Knots. \~
  \param[in] closed - \ru Признак замкнутости.
                      \en A closedness attribute. \~
  \param[in, out] t - \ru Параметр
                      \en A parameter \~
  \param[out] nsplines - \ru Множество размерности degree, заполняется значениями сплайна.
                         \en Array (dimension is "degree") is filled by spline values. \~
  \param[out] lrVect - \ru Вспомогательный массив (содержит мусор).
                       \en An assisting array (contains garbage). \~
  \return \ru Номер первого ненулевого B-сплайна.
          \en The number of the first non-zero B-spline. \~
  \ingroup Base_Algorithms
*/
// ---
template <class KnotsVector, class DoubleVector1, class DoubleVector2>
ptrdiff_t CalculateSplines(       size_t          degree, 
                            const KnotsVector &   knots, 
                                  bool            closed, 
                                  double &        t, 
                                  DoubleVector1 & nsplines,
                                  DoubleVector2 & lrVect )
{
  ptrdiff_t begInd = -1; // \ru Возвращаемое значение = номер первого ненулевого B-сплайна. \en The return value = the number of the first non-zero B-spline. 

  if ( (degree > 1) && (knots.size() > degree) ) {
    size_t power = degree - 1;
    size_t lastKnotInd = knots.size() - 1;

    // \ru Загнать параметр t в параметрическую область кривой. \en Reduce "t" to a parameter in the parametric domain of the curve 
    ::CheckParam( knots[power], knots[lastKnotInd - power], closed, t );

    ptrdiff_t tspan = ::KnotIndex( degree, knots, t );
    begInd = tspan - (ptrdiff_t)power;
    if ( begInd >= 0 && begInd + power <= lastKnotInd )
      ::BasisFuns( tspan, t, power, knots, nsplines, lrVect );
    else {
      begInd = SYS_MAX_T; // \ru Ошибка. \en Error 
      for ( size_t  i = 0; i < degree; i++ )
        nsplines[i] = 0.0;
    }

    C3D_ASSERT( (size_t)begInd != SYS_MAX_T );
  }

  return begInd;
}


//------------------------------------------------------------------------------
// \ru Вычисление характеристических точек pointList для прохождения NURBS-кривой через points[i] при params[i] \en Calculation of characteristic points "pointList" of NURBS-curve passing through points[i] with params[i] 
// ---
template <class Point, class ParamsVector, class SrcPointsVector, class KnotsVector, class DstPointsVector>
MbeNewtonResult CalculatePointList( const ParamsVector &    params, 
                                    const SrcPointsVector & points,
                                          size_t            degree, 
                                          bool              closed,
                                    const KnotsVector &     knots,
                                          DstPointsVector & pointList )
{
  MbeNewtonResult res = nr_Failure;

  size_t pointsCount = points.size();
  C3D_ASSERT( points.size() > 1 );

  if ( pointsCount > 1 && degree > 1 && knots.size() > 1 ) {
    // \ru Инициализация опорных точек. \en Initialization of support points. 
    if ( reinterpret_cast<ptrdiff_t>( &pointList ) != reinterpret_cast<ptrdiff_t>( &points ) ) {
      pointList.clear();
      pointList = points;
    }
    ptrdiff_t uppIndex = (ptrdiff_t)pointList.size() - 1; // \ru Количество точек. \en The count of points 
    if ( closed && (uppIndex > 1) &&
         c3d::EqualPoints( pointList.front(), pointList.back(), METRIC_REGION ) ) {
      pointList.erase( pointList.begin() + uppIndex );
      pointsCount = pointList.size();
    }

    DPtr<MatrixNN> matrixPtr( MatrixNN::Create(pointsCount) ); // \ru Матрица системы уравнений для прохождения NURBS при params[i] через points[i] \en Matrix of equation system for constructing the NURBS-curve passing through the points[i] with params[i]  

    if ( matrixPtr != nullptr && ::IsValidNurbsParamsExt(degree, closed, pointList.size(), knots) ) {
      MatrixNN & matrix = *matrixPtr;

      c3d::DoubleVector bSplines; // \ru Ненулевые B-сплайны \en Non-zero B-splines 
      bSplines.resize( degree );

      c3d::DoubleVector lrVect;

      for ( size_t i = 0; i < pointsCount; ++i ) { // \ru Заполняем строки матрицы \en Fills matrix rows 
        double t = params[i];
        ptrdiff_t k = 0;
        ptrdiff_t ind = ::CalculateSplines( degree, knots, closed, t, bSplines, lrVect );
        // \ru Заполняем i-ю строку \en Fill the i-th row 
        for ( k = 0; k < (ptrdiff_t)pointsCount; ++k )
          matrix( i, k ) = 0.0;
        for ( k = ind; k < ind + (ptrdiff_t)degree; ++k )
          matrix( i, k % pointsCount ) += bSplines[k - ind]; // \ru Ненулевые элементы строки. \en Non-zero elements of row 
      }

      double epsilon = PARAM_EPSILON;
      // \ru Решаем систему уравнений относительно характеристических точек pointList \en Solve the system of equations for the characteristic points "pointList" 
      // \ru Правая часть системы = адрес начала массива опорных точек pointList.begin(). \en The right system part = address of beginning of support point array pointList.begin(). 
      res = ::TypedGaussEquation( matrix, &pointList[0], epsilon );
    }
  }

  return res;
}


//------------------------------------------------------------------------------
// \ru Вычисление характеристических точек pointList для прохождения NURBS-кривой через points[i] при params[i]. \en Calculation of characteristic points "pointList" of NURBS-curve passing through points[i] with params[i].
// ---
template <class Point, class ParamsVector, class SrcPointsVector, class KnotsVector, class DstPointsVector>
MbeNewtonResult CalculatePointListWithBandMatrix( const ParamsVector &    params, 
                                                  const SrcPointsVector & points,
                                                        size_t            degree, 
                                                        bool              closed, 
                                                  const KnotsVector &     knots,
                                                        DstPointsVector & pointList )
{
  MbeNewtonResult res = nr_Failure;

  size_t pointsCount = points.size();

  if ( pointsCount > 1 && degree > 1 && knots.size() > 1 ) {
    // \ru Инициализация опорных точек. \en Initialization of support points. 
    if ( reinterpret_cast<ptrdiff_t>( &pointList ) != reinterpret_cast<ptrdiff_t>( &points ) ) {
      pointList.clear();
      pointList = points;
    }
    ptrdiff_t uppIndex = (ptrdiff_t)pointList.size() - 1; // \ru Количество точек. \en The count of points. 
    if ( closed && (uppIndex > 1) &&
         c3d::EqualPoints( pointList.front(), pointList.back(), METRIC_REGION ) ) {
      pointList.erase( pointList.begin() + uppIndex );
      pointsCount = pointList.size();
    }

    // \ru Матрица системы уравнений для прохождения NURBS при params[i] через points[i]. \en Matrix of equation system for constructing the NURBS-curve passing through the points[i] with params[i].  
    DPtr< SparseArray2<double> > matrixPtr( SparseArray2<double>::Create( pointsCount, pointsCount ) );

    if ( matrixPtr != nullptr && ::IsValidNurbsParamsExt( degree, closed, pointList.size(), knots ) ) {
      SparseArray2<double> & matrix = *matrixPtr;

      c3d::DoubleVector bSplines; // \ru Ненулевые B-сплайны. \en Non-zero B-splines. 
      bSplines.resize( degree );

      c3d::DoubleVector lrVect;
      std::vector<c3d::IndexDouble> nzElems;

      c3d::DoubleVector nzBuffer;
      for ( size_t i = 0; i < pointsCount; ++i ) { // \ru Заполняем строки матрицы. \en Fills matrix rows. 
        double t = params[i];

        ptrdiff_t ind = ::CalculateSplines( degree, knots, closed, t, bSplines, lrVect );

        nzElems.clear();
        nzBuffer.assign( pointsCount, 0.0 );

        // \ru Заполняем i-ю строку. \en Fill the i-th row. 
        ptrdiff_t k;
        for ( k = ind; k < ind + (ptrdiff_t)degree; ++k ) {
          size_t curInd = k % pointsCount;
          nzBuffer[curInd] += bSplines[k - ind];
        }
        for ( k = 0; k < (ptrdiff_t)pointsCount; ++k ) {
          if ( nzBuffer[k] != 0.0 ) {
            nzElems.push_back( std::make_pair( k, nzBuffer[k] ) );
          }
        }
        matrix.SetLine( i, nzElems );
      }

      double epsilon = PARAM_EPSILON;
      // \ru Решаем систему уравнений относительно характеристических точек pointList. \en Solve the system of equations for the characteristic points "pointList". 
      // \ru Правая часть системы = адрес начала массива опорных точек pointList.begin(). \en The right system part = address of beginning of support point array pointList.begin(). 
      res = ::TypedGaussEquationWithBandMatrix<Point>( matrix, &pointList[0], epsilon );
    }
  }

  return res;
}


//------------------------------------------------------------------------------
// \ru Установить касательность сплайна к вектору \en Set tangency of spline to vector 
//---
template<class TypedNurbs, class TypedPoint, class TypedVector>
bool AttachNurbsG1(       TypedNurbs &  nurbs, // \ru Модифицируемый сплайн \en Modifiable spline 
                    const TypedVector & tang, // \ru Касательный вектор \en Tangent vector 
                          bool          begin,   // \ru Сопряжение выставлено в начале \en Conjugation is defined for the start 
                          bool          modify,  // \ru Можно ли менять существующие полюса \en Whether it is possible to modify the existing pole 
                          bool          isC1 )   // \ru Нужно сохранить длину касательного вектора \en Need to save the length of the tangent vector 
{
  bool res = false;

  if ( !nurbs.IsClosed() && nurbs.GetPointListCount() > 2 && nurbs.GetDegree() > 2 ) {
    bool needRebuild = false;

    TypedVector normTang( tang );
    double tangLen = tang.Length();
    if ( tangLen > LENGTH_EPSILON )
      normTang /= tangLen;

    SArray<TypedPoint>  points ( 0, 1 );
    SArray<double>      weights( 0, 1 );
    SArray<double>      knots  ( 0, 1 );
    nurbs.GetPointList( points );
    nurbs.GetWeights( weights );
    nurbs.GetKnots( knots );
    ptrdiff_t degree = nurbs.GetDegree();
    bool closed = false;

    // \ru Дополнительная точка, которая обеспечивает визуальную касательность \en Additional point which provides a visual tangency 
    TypedPoint point, add( points[begin ? 0 : points.MaxIndex()] );
    TypedVector curTang, curVect;
    double dist   = 0.0;
    //double dKnots = 0.0; 

    double part        = modify ? 1.0 : 0.5; // \ru Параметрическая доля \en Parametric part  
    double pointKnot   = 0.0;                // \ru Узел точки, обеспечивающий сопряжение \en Point knot which provides a conjugation 
    double pointWeight = 1.0;                // \ru Вес точки \en A point weight 

    if ( begin ) { // \ru Стыковка производится в начале \en Connection is performed in the beginning 
      nurbs._Tangent( nurbs.GetTMin(), curTang );
      if ( !curTang.Colinear(tang) ||
            curTang * tang < -ANGLE_EPSILON ) // BUG_55564
      { // \ru Если еще не установлено сопряжение \en If conjugation is not set 
        curVect.Set( points[1], 1.0, points[0], -1.0 );

        if ( isC1 || !modify || tang.Orthogonal( curTang, Math::metricNear ) ) {
        // BUG_53978 if ( !modify || tang.Orthogonal( curTang, Math::metricNear ) ) {
          pointKnot = modify ? knots[(size_t)degree] : 
                               knots[(size_t)degree] * part + knots[0] * ( 1.0 - part ); 

          pointWeight = modify ? weights[1] : weights[0];

          dist = points[0].DistanceToPoint( points[1] ) * part; // \ru Длина производной \en Derivative length 
          
          if ( isC1 )
            add.Add( tang, (pointKnot - knots[0]) / (double)(degree - 1) * weights[0] / pointWeight );
          else
            add.Add( normTang, dist / (double)(degree - 1) * weights[0] / pointWeight );

          if ( modify ) 
            points[1] = add;
          else {
            points.AddAt( add, 1 );
            weights.AddAt( pointWeight, 1 );
            knots.AddAt( pointKnot, (size_t)degree );
          }
        }
        else {
          // \ru BUG_50080 dist = (normTang * curVect); // МСГ К12 решено не учитывать знак производной \en BUG_50080 dist = (normTang * curVect); // МСГ К12 ignore the sign of the derivative 
          dist = ::fabs(normTang * curVect);
          points[1] = points[0] + normTang * dist;
        }
        needRebuild = true;
      }

      res = true;
    }
    else { // \ru Стыковка производится в конце \en Connection is performed at the end 
      nurbs._Tangent( nurbs.GetTMax(), curTang );
      if ( !curTang.Colinear(tang) ||
           curTang * tang < -ANGLE_EPSILON ) // BUG_55564) 
      { // \ru Если еще не установлено сопряжение \en If conjugation is not set 
        ptrdiff_t shear = points.MaxIndex(); 
        curVect.Set( points[shear], 1.0, points[shear - 1], -1.0 );

        if ( isC1 || !modify || tang.Orthogonal( curTang, Math::metricNear ) ) {
        // BUG_53978 if ( !modify || tang.Orthogonal( curTang, Math::metricNear ) ) {
          pointKnot = modify ? knots[shear] : 
                               knots[shear] * ( 1.0 - part ) + knots[shear + (size_t)degree] * part;

          pointWeight = modify ? weights[shear - 1] : weights[shear];

          dist = points[shear].DistanceToPoint( points[shear - 1] ) * part; // \ru Длина производной \en Derivative length 
          
          if ( isC1 )
            add.Add( tang, -(knots[shear + degree] - pointKnot) / (double)(degree - 1) * weights[shear] / pointWeight );
          else {
            // BUG_49940 add.Add( tang, -dist / (degree - 1) * weights[shear] / pointWeight );
            add.Add( normTang, -dist / (double)(degree - 1) * weights[shear] / pointWeight );
          }

          if ( modify )
            points[shear - 1] = add;
          else { 
            points.AddAt( add, shear );
            weights.AddAt( pointWeight, shear );
            knots.AddAt( pointKnot, shear + 1 );
          }
        }
        else {
          // \ru BUG_50080 dist = (normTang * curVect); // МСГ К12 решено не учитывать знак производной \en BUG_50080 dist = (normTang * curVect); // МСГ К12 ignore the sign of the derivative 
          dist = ::fabs(normTang * curVect);
          points[shear - 1] = points[shear] - normTang * dist;
        }
        needRebuild = true;
      }

      res = true;
    }

    if ( res && needRebuild )
      nurbs.Init( degree, closed, points, weights, knots, ncf_Unspecified );
/*#if defined(C3D_DEBUG)
    if ( res ){ // \ru Отладка \en Debugging
      TypedVector vect;
      if ( begin ) nurbs._FirstDer( nurbs.GetTMin(), vect );
      else         nurbs._FirstDer( nurbs.GetTMax(), vect );
      double vectLen = vect.Length();
      C3D_ASSERT( tang.Colinear( vect ) && tang * vect > ANGLE_EPSILON );
      C3D_ASSERT( !isC1 || ::fabs(tangLen - vectLen) < METRIC_EPSILON );
    }
#endif
*/
  }

  return res;
}


//------------------------------------------------------------------------------
// \ru Установить касательные на краях \en Set tangents at the ends 
// ---
template<class Curve, class Nurbs, class Point, class Vector>
bool SetLimitFirstDerivatives( const Curve & curve, bool setBeg, bool setEnd, Nurbs & nurbs, bool setLen )
{ // BUG_59596
  bool changed = false;

  if ( setBeg ) {
    Vector fd;
    curve._FirstDer( curve.GetTMin(), fd );
    if ( ::AttachNurbsG1<Nurbs,Point,Vector>( nurbs, fd, true,  false, setLen ) )
      changed = true;
  }
  if ( setEnd ) {
    Vector fd;
    curve._FirstDer( curve.GetTMax(), fd );
    if ( ::AttachNurbsG1<Nurbs,Point,Vector>( nurbs, fd, false, false, setLen ) )
      changed = true;
  }

  return changed;
}


//------------------------------------------------------------------------------
// \ru Установить точки так, чтобы совпала касательная и главная нормаль \en Set points such that tangent and principal normal are coincident 
//---
template<class TypedNurbs, class TypedPoint, class TypedVector>
bool AttachNurbsG2(       TypedNurbs &  nurbs, // \ru Модифицируемый сплайн \en Modifiable spline 
                    const TypedVector & tang, // \ru Касательный вектор \en Tangent vector 
                    const TypedVector & tangDiff, // \ru Производная касательного вектора \en The derivative of a tangent vector 
                          bool          begin,  // \ru Сопряжение выставлено в начале \en Conjugation is defined for the start 
                          bool          modify, // \ru Можно ли менять существующие полюса \en Whether it is possible to modify the existing poles 
                          double *      wDiff1,
                          double *      wDiff2 )
{
  bool res = false;

  if ( !nurbs.IsClosed() && nurbs.GetPointListCount() > 3 && nurbs.GetDegree() > 3 && 
       ::AttachNurbsG1<TypedNurbs, TypedPoint, TypedVector>( nurbs, tang, begin, modify, false ) ) // \ru Стыкуем сначала по касательной \en Join by a tangent at first 
  { 
    bool needRebuild = false;   // \ru Нужно ли перестраивать кривую \en Whether to rebuild the curve 

    TypedVector normTang( tang );
    double tangLen = tang.Length();
    if ( tangLen > LENGTH_EPSILON )
      normTang /= tangLen;

    SArray<TypedPoint>  points ( 0, 1 );
    SArray<double>      weights( 0, 1 );
    SArray<double>      knots  ( 0, 1 );
    nurbs.GetPointList( points );
    nurbs.GetWeights( weights );
    nurbs.GetKnots( knots );
    ptrdiff_t degree = nurbs.GetDegree();
    bool closed = nurbs.IsClosed();

    double eps = Math::metricEpsilon;

    //double dKnots    = 0.0; 
    ptrdiff_t degm      = degree - 1;
    double curCurv   = 0.0;
    double curvature = tangDiff.Length();
    
    TypedVector curNormal;          // \ru Текущая нормаль в стыке \en Current normal at the joint  
    TypedVector firstDer;           // \ru Первая производная сплайна  в точке \en The first spline derivative at the point 
    TypedVector secndDer;           // \ru Старая производная сплайна в точке \en The old spline derivative at the point 
    TypedVector secDer( tangDiff ); // \ru Вторая производная, с которой фактически устанавливается равенство \en The second derivative which the equality is set with 

    TypedPoint add; // \ru Точка, обеспечивающая равенство вторых производных \en Point which provides the equality of second derivatives 
    TypedPoint wp0, wp1;   // \ru Взвешенные точки \en Weighted points 

    double part        = modify ? 1.0 : 0.5;  // \ru Параметрическая доля \en Parametric part  
    double pointKnot   =                0.0;  // \ru Узел точки, обеспечивающий сопряжение \en Point knot which provides a conjugation 
    double pointWeight =                1.0;  // \ru Вес точки \en A point weight 

    double weightDiff1 = 0.0, weightDiff2 = 0.0; // \ru Первая и вторая производная весов \en The first and the second derivative of weights 
    if ( begin ) { // \ru Стыковка производится в начале \en Connection is performed at the start 
      nurbs._Normal( nurbs.GetTMin(), curNormal );
      curCurv = nurbs.Curvature( nurbs.GetTMin() );

      pointKnot = modify ? knots[(size_t)degree + 1] : 
                           knots[(size_t)degree + 1] * part + knots[degree] *( 1.0 - part );
      pointWeight = modify ? weights[2] : weights[0] ;
      weightDiff1 = (double)degm * ( weights[1] - weights[0] ) / ( knots[degree] - knots[1] );
      weightDiff2 = ( (double)degm - 1 ) * (double)degm / ( knots[degree] - knots[2] ) *
                    ( (pointWeight - weights[1]) / (pointKnot     - knots[2]) -
                      (weights[1]  - weights[0]) / (knots[degree] - knots[1]) );

      if ( !(curNormal.Colinear(tangDiff) && 
           ::fabs(curCurv - curvature) < eps) ) // \ru Еще нет необходимой гладкости стыка \en The required smoothness of a joint is not provided yet 
      {
        nurbs._FirstDer( nurbs.GetTMin(), firstDer );
        secDer *= firstDer * firstDer; 
        nurbs._SecondDer( nurbs.GetTMin(), secndDer );
        secDer += normTang * ( part * normTang * secndDer ); // \ru Сохранение старой проекции \en Saving of the old projection  

        wp0 += points[0] * weights[0];
        wp1 += points[1] * weights[1];

        // \ru Обрабатываем вторую производную \en Process the second derivative 
        secDer *= weights[0];
        secDer.Add( points[0], weightDiff2, firstDer, 2.0 * weightDiff1 );

        double dK  = pointKnot - knots[2];
        double dK1 = 1.0 / dK + 1.0 / ( knots[(size_t)degree] - knots[1] );

        add.Set( wp0, 1.0, wp1 - wp0, dK1 * dK );
        add.Add( secDer,  (knots[(size_t)degree] - knots[2]) * dK / ((double)degm * (double)(degm - 1)) );
        add /= pointWeight;

        if ( modify ) 
          points[2] = add;
        else {
          knots.AddAt( pointKnot, (size_t)degree + 1 );
          points.AddAt( add, 2 );
          weights.AddAt( pointWeight, 2 );
        }
        needRebuild = true;
      }

      res = true;
    }
    else { // \ru Стыковка производится в конце \en Connection is performed at the end 
      nurbs._Normal( nurbs.GetTMax(), curNormal );
      curCurv = nurbs.Curvature( nurbs.GetTMax() );

      ptrdiff_t shear = points.MaxIndex();

      pointKnot = modify ? knots[shear - 1] : 
                           knots[shear] * ( 1.0 - part ) + knots[shear - 1] * part ;

      pointWeight = modify ? weights[shear - 2] : weights[shear];

      weightDiff1 = (double)degm * ( weights[shear] - weights[shear - 1] ) / 
                           ( knots[shear + (size_t)degree] - knots[shear] );
      weightDiff2 = (double)(degm - 1) * (double)degm / ( knots[shear + (size_t)degm] - knots[shear] ) *
                    ( (weights[shear]     - weights[shear - 1]) / (knots[shear + (size_t)degree]     - knots[shear]) -
                      (weights[shear - 1] - pointWeight       ) / (knots[shear + (size_t)degree - 1] - pointKnot   ) );

      if ( !(curNormal.Colinear(tangDiff) &&
            ::fabs(curCurv - curvature) < eps) ) // \ru Еще нет необходимой гладкости стыка \en The required smoothness of a joint is not provided yet 
      {
        nurbs. _FirstDer( nurbs.GetTMax(), firstDer );
        secDer *= firstDer * firstDer;
        nurbs._SecondDer( nurbs.GetTMax(), secndDer );
        secDer += normTang * ( part * normTang * secndDer ); // \ru Сохранение старой проекции \en Saving of the old projection  

        wp0 += points[shear]     * weights[shear];
        wp1 += points[shear - 1] * weights[shear - 1];
        
        // \ru Обрабатываем вторую производную \en Process the second derivative 
        secDer *= weights[shear];
        secDer.Add( points[shear], weightDiff2, firstDer, 2.0 * weightDiff1 );

        double dK  = knots[shear + (size_t)degm] - pointKnot;
        double dK1 = 1.0 / ( knots[shear + (size_t)degm] - knots[shear] ) + 
                     1.0 / ( knots[shear + (size_t)degm] - pointKnot );

        add.Set( wp0, 1.0, wp1 - wp0, dK1 * dK );
        add.Add( secDer,  (knots[shear + (size_t)degm] - knots[shear]) * dK / ((double)degm * (double)(degm - 1)) );
        add /= pointWeight; 

        if ( modify )
          points[shear - 2] = add;
        else {
          knots.AddAt( pointKnot, shear );
          points.AddAt( add, shear - 1 );
          weights.AddAt( pointWeight, shear - 1 );
        }
        needRebuild = true;
      }

      res = true;
    }

    // \ru Сохраняем вычисленные производные \en Save the calculated derivatives 
    if ( res ) {
      if ( wDiff1 != nullptr )
        *wDiff1 = weightDiff1;
      if ( wDiff2 != nullptr && res )
        *wDiff2 = weightDiff2;
    }

    if ( res && needRebuild )
      nurbs.Init( degree, closed, points, weights, knots, ncf_Unspecified );
  }

/*#if defined(C3D_DEBUG)
  if ( res ) { // \ru Отладка \en Debugging
    double t = begin ? nurbs.GetTMin() : nurbs.GetTMax();

    MbVector3D vect;
    nurbs._Tangent( t, vect );
    C3D_ASSERT( vect.Colinear( tang ) );

    MbVector3D normal;
    nurbs._Normal( t, normal );
    C3D_ASSERT( normal.Colinear(tangDiff) );

    double curvature = tangDiff.Length();
    double curCurvature = nurbs.Curvature( t );
    C3D_ASSERT( ::fabs(curCurvature - curvature) < Math::metricAccuracy );
  }
#endif
*/

  return res;
}


//------------------------------------------------------------------------------
// \ru Создать замкнутый NURBS, проходящий через точки с заданными параметрами. \en Calculate closed NURBS by points which it passes through and points parameters.
//---
template <class Nurbs, class Point>
bool CreateClosedNURBS4( Nurbs & nurbs, const SArray<Point> & initPoints, const SArray<double> & initParams )
{
  bool bRes = ( (initParams.Count() > 3) && (initParams.Count() == initPoints.Count() + 1) );

  if ( bRes ) {
    nurbs.Refresh(); // должен стоять первым, т.к. освобождается выделенная память

    const ptrdiff_t degree = 4; // степень В-сплайна     //-V112
    const bool closed = true; // признак замкнутости
    const MbeNurbsCurveForm form = ncf_Unspecified; // форма B - сплайна
    
    // скопировать массив характеристических точек
    SArray<Point> points( initPoints );

    // установить единичные веса
    SArray<double> weights( points.Count(), 1 );
    {
      const double weight0 = 1.0;
      weights.Fill( points.Count(), weight0 );
    }

    const ptrdiff_t degm = degree - 1;

    // заполнить массив параметров (на концах используем условие "отсутствия узла"), обеспечивающий замыкание
    SArray<double> knots ( (initParams.MaxIndex() + 2 * degree - 1), 1 );
    {
      ptrdiff_t uppKnotsIndex = (ptrdiff_t)initParams.Count() - 1;
      ptrdiff_t i = 0; // индекс

      // вставляем начальные узлы 0.. degree - 2
      for ( i = 0; i < degm; i++ ) {
        ptrdiff_t startIndex = uppKnotsIndex - degm;
        knots.Add( initParams[0] - initParams[uppKnotsIndex] + initParams[startIndex + i] );
      }

      knots += initParams;

      // вставляем конечные узлы
      const double tmax = initParams[initParams.MaxIndex()]; 
      ptrdiff_t i0 = degm;
      for ( i = i0; i < i0 + degm; i++ ) 
        knots.Add( tmax + knots[i + 1] - knots[i0] );

      uppKnotsIndex = knots.MaxIndex();
    }

    points.Adjust();
    weights.Adjust();
    knots.Adjust();

    const ptrdiff_t pointsCnt = initPoints.Count();
    const ptrdiff_t uppIndex = pointsCnt - 1;
    // для приведения матрицы к матрице с диагональным преобладанием необходимо
    // последнее уравнение исключать первым 
    // далее решаем систему уравнений методом Гаусса без выбора ведущего элемента
    SArray<double> biatx( degree, 1 ); // не нулевые B - сплайны
    SArray<double> d    ( pointsCnt ); // массив диагоналей
    SArray<Point>  ar   ( pointsCnt ); // массив правых частей 
    SArray<double> n    ( pointsCnt ); // последний столбец в результирующей матрице

    SArray<double> lrVect;
    lrVect.resize( 2*degree );

    ar.Add( initPoints[initPoints.MaxIndex()] );
    ar += initPoints;
    ar.RemoveInd( ar.MaxIndex() );

    ptrdiff_t lastIndex = initParams.MaxIndex() - 1;
    ptrdiff_t lastKnot  = knots.Count() - degree - 1;

    // первое уравнение
    ::CalcBsplvb( knots, initParams[lastIndex], lastKnot, degree, biatx, lrVect );

    double norm     = 1 / biatx[1];
    double normLast = 1.0;

    ar[0] *= norm;

    d.Add( biatx[2] * norm );
    n.Add( biatx[0] * norm );

    // последнее уравнение
    ::CalcBsplvb( knots, initParams[lastIndex - 1], lastKnot - 1, degree, biatx, lrVect );
    norm = 1.0 / biatx[2];
    // в последнем уравнении, соответственно, внедиагональный и диагональный элемент 
    double a = biatx[0] * norm;
    double b = biatx[1] * norm; 

    ar[lastIndex] *= norm;
    Point & sn = ar[lastIndex];

    // прямой ход
    ptrdiff_t crLeft = degm;
    ptrdiff_t i = 1, im = 0, ip = 1;
    for ( ; i < lastIndex - 1; i++, im++, crLeft++ ) {
      ::CalcBsplvb( knots, initParams[i - 1], crLeft, degree, biatx, lrVect );

      norm = 1 / ( biatx[1] - d[im] * biatx[0] );

      ar[i] = ( ar[i] - ar[im] * biatx[0] ) * norm;
      d [i] = biatx[2] * norm;
      // исключаем из последнего уравнения ведущую 1 и меняем последний столбец
      normLast = -1.0 / d[im];

      a *= normLast;
      b = ( b - n[im] ) * normLast;

      sn = ( sn - ar[im] ) * normLast;

      n.Add( -norm * biatx[0] * n[im] );
    }

    // исключаем из 2-х последних уравнений 3-е с конца
    ::CalcBsplvb( knots, initParams[i - 1], crLeft, degree, biatx, lrVect );

    norm              = 1.0 / ( biatx[1] - d[im] * biatx[0] );
    d [lastIndex - 1] = ( biatx[2] - n[im] * biatx[0] ) * norm;
    ar[lastIndex - 1] = ( ar[lastIndex - 1] - ar[im] * biatx[0] ) * norm;

    normLast = 1.0 / ( a - d[im] );
    b  = ( b  -  n[im] ) * normLast;
    a  = 1.0; 
    sn = ( sn - ar[im] ) * normLast;

    // исключаем из последнего предпоследнее
    im++;   
    points[uppIndex] = ( sn - ar[im] ) / ( b  -  d[im] );

    ptrdiff_t prevLast = uppIndex - 1;
    points[prevLast] = ar[lastIndex - 1] - points[uppIndex] * d[prevLast];

    // обратный ход
    for ( i = prevLast - 1, ip = prevLast; i >= 0; i--, ip-- ) {
      points[i] = ar[i] - points[ip]       * d[i]  
                        - points[uppIndex] * n[i];
    }

    bRes = nurbs.Init( degree, closed, points, weights, knots, form );
    C3D_ASSERT( bRes );
  }

  return bRes;
}


//------------------------------------------------------------------------------
// \ru Получить массив параметров по точкам \en Get an array of parameters given the points 
// ---
template <class PointsVector, class DoubleVector>
bool CreateSplineParameters( const PointsVector &     points, 
                                   MbeSplineParamType spType, 
                                   bool               cls,
                                   DoubleVector &     params ) 
{
  bool isDone = false;
  params.clear();
  isDone = ::DefineThroughPointsParams( points, spType, cls, params );
 
  // \ru Нормализация \en Normalization 
  if ( isDone )
    c3d::SetLimitParam( params, 0.0, (double)((ptrdiff_t)points.size() - 1 + (cls ? 1 : 0)) );

  return isDone;

}


//------------------------------------------------------------------------------
// \ru Выбрать точки на кривой для аппроксимации замкнутой nurbs \en Select points on the curve for approximation of closed nurbs 
// ---
template <class Curve, class Point, class KnotsVector>
size_t DefineApproxPointsClosed( const Curve & curve, size_t pCount, double pmin, double pmax, ptrdiff_t degree, SArray<Point> & points, const KnotsVector & aKnots, const SArray<size_t> & pCounts )
{
  size_t pCountActual = pCount;
  if ( pCount < 1 )
    return 0;

  points.clear();
  size_t stepCount = 0;
  double factor = 1.0 / ( (double)pCount );
  const double epsilon = curve.GetTRegion( METRIC_REGION );
  const double angle = Math::deviateSag;

  size_t segmCount = curve.GetSegmentsCount();
  bool notZeroLen = pmin < pmax;

  SArray<size_t> segmPointsCnt( segmCount, 2 );
  SArray<double> tList, restList;

  double tmin, tmax;
  ptrdiff_t first_Segm = curve.FindSegment( pmin, tmin );
  ptrdiff_t last_Segm = curve.FindSegment( pmax, tmax );

  if ( segmCount == 1 && notZeroLen && !(tmin < tmax) ) {
    if ( ::fabs(pmax - pmin) < NULL_REGION ) {
      double period = curve.GetPeriod();
      pmax = pmin + period;
      tmax = tmin + period;
    }
  }

  ptrdiff_t i = 0;

  size_t freePntsCount = pCount, j = 0;

  // \ru Найдем количество точек для каждого сегмента. \en Find the number of points for each segment. 
  size_t totalCount = 0;
  for ( j = 0; j < pCounts.size(); ++j )
    totalCount += pCounts[j];
  if ( totalCount < 1 )
    totalCount = 1;

  for ( j = 0; j < pCounts.size(); j++ ){
    double part = ((double)pCounts[j]) / ((double)totalCount);
    size_t segmPCnt = (size_t)(pCount * part);   //-V113
    if ( j == (size_t)last_Segm )
      segmPCnt = freePntsCount;
    else {
      segmPCnt = std_max( segmPCnt, (size_t)1);
      segmPCnt = std_min( segmPCnt, freePntsCount );
    }
    segmPointsCnt.push_back( segmPCnt );
    freePntsCount -= segmPCnt;
  }

  Point p;
  double plusT = 0.0;
  for ( i = first_Segm; i <= last_Segm; i++ ) {
    tList.clear();
    double smin, smax;
    smin = curve.GetSegment(i)->GetTMin();
    smax = curve.GetSegment(i)->GetTMax();

    if ( first_Segm == last_Segm ) {
      plusT = pmin + smin - tmin;
      smin = tmin;
      smax = tmax;
    }
    else if ( i == first_Segm ){
      plusT = pmin + smin - tmin;
      smin = tmin;
    }
    else if ( i == last_Segm ) {
      smax = tmax;
    }
    double t = smin;
    size_t tempCount = segmPointsCnt[i - first_Segm];
    factor = 1.0 / ( (double)tempCount );
    stepCount = 0;
    while ( t < (smax - epsilon) ) {
      double step = curve.GetSegment(i)->DeviationStep( t, angle );
      if ( (t + step) >= (smax - epsilon) ) 
        step = smax - t;

      for ( size_t k = 0; k < tempCount; k++ ) 
        tList.push_back( plusT + t + (step * factor * (double)k) );
      t += step;
      stepCount++;
    }
    tList.push_back(plusT + smax);
    plusT += smax;

    for ( j = 0; j < tempCount; j++ ) {
      t = tList[j * stepCount];
      curve._PointOn( t, p );
      points.push_back( p );
      restList.push_back( t );
    }
  }

  restList.push_back( pmax );

  // \ru Если задан узловой вектор, надо проверить, что между любыми 2 узлами есть хотя бы одна точка. \en If the knot vector is given, then it is necessary to check that there is at least one point between any two knots.  
  // \ru Если нет, то добавим точек, сохранив все уже набранные \en If not, then add the points and save old points 
  if ( aKnots.size() > 0 ) {
    SArray<double> pParams( 0, 1 );
    ::CreateSplineParameters( points, spt_ChordLength, true, pParams );
    c3d::SetLimitParam( pParams, aKnots[degree - 1], aKnots[aKnots.size() - degree] );
    double t1, t2;
    bool bRes = false;
    while ( !bRes ) {
      bRes = true;
      for ( i = degree - 1; i < aKnots.MaxIndex() - degree + 1 && bRes; i++ ) {
        t1 = aKnots[i];
        t2 = aKnots[i + 1];
        if ( ::fabs(t2 - t1) > NULL_EPSILON ) {
          size_t il = 0, ir = pParams.MaxIndex();
          size_t itemp = 0;
          size_t ires1 = 0;
          size_t ires2 = 0;
          // \ru Левая граница \en The left boundary 
          bool goOn = true;
          while ( goOn ) {
            if ( ::fabs(pParams[il] - t1) < NULL_EPSILON ){
              ires1 = il;
              goOn = false;
              break;
            }
            else if ( ::fabs(pParams[ir] - t1) < NULL_EPSILON ) {
              ires1 = ir;
              goOn = false;
              break;
            }
            else {
              itemp = ( il + ir ) / 2;
              if ( ::fabs(pParams[itemp] - t1) < NULL_EPSILON ) {
                ires1 = itemp;
                goOn = false;
                break;
              }
              if ( pParams[itemp] < t1 ) 
                il = itemp;
              else if ( pParams[itemp] > t1 )
                ir = itemp;
              if ( ir - il < 2) {
                ires1 = il;
                goOn = false;
                break;
              }
            }
          }
          // \ru Правая граница \en The right boundary 
          il = ires1;
          ir = pParams.MaxIndex();
          goOn = true;
          while ( goOn ) {
            if ( ::fabs(pParams[il] - t2) < NULL_EPSILON ){
              ires2 = il;
              goOn = false;
              break;
            }
            else if ( ::fabs(pParams[ir] - t2) < NULL_EPSILON ) {
              ires2 = ir;
              goOn = false;
              break;
            }
            else {
              itemp = ( il + ir ) / 2;
              if ( ::fabs(pParams[itemp] - t2) < NULL_EPSILON ) {
                ires2 = itemp;
                goOn = false;
                break;
              }
              if ( pParams[itemp] < t2 ) 
                il = itemp;
              else if ( pParams[itemp] > t2 )
                ir = itemp;
              if ( ir - il < 2) {
                ires2 = ir;
                goOn = false;
                break;
              }
            }
          }
          if ( ires2 - ires1 < 2 ) {
            bRes = false;
            // \ru Вставим среднюю точку между ires1 и ires2 и пересчитаем параметры \en Insert mid-point between ires1 and ires2 and recalculate parameters 
            double t = 0.5 * (restList[ ires1 ] + restList[ ires2 ]);
            restList.AddAt( t, ires1 + 1 );

            curve._PointOn( t, p );
            points.AddAt( p, ires1 + 1);

            pParams.clear();
            ::CreateSplineParameters( points, spt_ChordLength, false, pParams );
            c3d::SetLimitParam( pParams, aKnots[degree - 1], aKnots[aKnots.size() - degree] );
            pCountActual++;
          }
        }
      }
    }
  }

  return pCountActual;
}


//------------------------------------------------------------------------------
// \ru Выбрать точки на кривой для аппроксимации незамкнутой nurbs \en Select points on the curve for approximation of non-closed nurbs 
// ---
template <class Curve, class Point, class KnotsVector>
size_t DefineApproxPointsOpen( const Curve & curve, size_t pCount, double pmin, double pmax, SArray<Point> & points, const KnotsVector & aKnots, const SArray<size_t> & pCounts )
{
  size_t pCountActual = pCount;
  if ( pCount < 1 )
    return 0;

  points.clear();
  size_t stepCount = 0;
  double factor = 1.0 / ( (double)(pCount - 1) );
  const double epsilon = curve.GetTRegion( METRIC_REGION );
  const double angle = Math::deviateSag;

  size_t segmCount = curve.GetSegmentsCount();
  if ( pCounts.size() != segmCount )
    return 0;

  SArray<double> tList, restList;
  SArray<size_t> segmPointsCnt( segmCount, 2 );

  double tpmin = pmin;
  double tpmax = pmax;

  double tmin, tmax;
  ptrdiff_t first_Segm = curve.FindSegment( tpmin, tmin );
  ptrdiff_t last_Segm = curve.FindSegment( tpmax, tmax );
  ptrdiff_t i;

  size_t freePntsCount = pCount - 1, j = 0;

  // \ru Найдем количество точек для каждого сегмента. \en Find the number of points for each segment. 
  size_t totalCount = 0;
  for ( j = 0; j < pCounts.size(); ++j )
    totalCount += pCounts[j];
  if ( totalCount == 0 )
    totalCount = 1;

  for ( j = 0; j < pCounts.size(); j++ ){
    double part = ((double)pCounts[j]) / ((double)totalCount);
    size_t segmPCnt = (size_t)(pCount * part);   //-V113
    if ( j == (size_t)last_Segm )
      segmPCnt = freePntsCount;
    else {
      segmPCnt = std_max( segmPCnt, (size_t)1);
      segmPCnt = std_min( segmPCnt, freePntsCount );
    }
    segmPointsCnt.push_back( segmPCnt );
    freePntsCount -= segmPCnt;
  }

  Point p;
  double plusT = 0.0;
  for ( i = first_Segm; i <= last_Segm; i++ ) {
    tList.clear();

    double smin = curve.GetSegment(i)->GetTMin();
    double smax = curve.GetSegment(i)->GetTMax();

    if ( first_Segm == last_Segm ) {
      plusT = tpmin + smin - tmin;
      smin = tmin;
      if (pmin < tpmin - PARAM_EPSILON )
        smin += pmin - tpmin;
      smax = tmax;
      if (pmax > tpmax + PARAM_EPSILON )
        smax += pmax - tpmax;
    }
    else if ( i == first_Segm ){
      plusT = tpmin + smin - tmin;
      smin = tmin;
      if (pmin < tpmin - PARAM_EPSILON )
        smin += pmin - tpmin;
    }
    else if ( i == last_Segm ) {
      smax = tmax;
      if (pmax > tpmax + PARAM_EPSILON )
        smax += pmax - tpmax;
    }
    double t = smin;
    size_t tempCount = segmPointsCnt[i - first_Segm];
    factor = 1.0 / ( (double)tempCount );
    stepCount = 0;
    while ( t < (smax - epsilon) ) {
      double step = curve.GetSegment(i)->DeviationStep( t, angle );
      if ( (t + step) >= (smax - epsilon) ) 
        step = smax - t;

      for ( j = 0; j < tempCount; j++ ) 
        tList.push_back( plusT + t + (step * factor * (double)j) );
      t += step;
      stepCount++;
    }
    tList.push_back(plusT + smax);
    plusT += smax;

    for ( size_t k = 0; k < tempCount; k++ ) {
      t = tList[k * stepCount];
      curve._PointOn ( t, p );
      points.push_back( p );
      restList.push_back( t );
    }
  }

  double t = pmax;
  curve._PointOn ( t, p );
  points.push_back( p );
  restList.push_back( t );

  // \ru Если задан узловой вектор, надо проверить, что между любыми 2 узлами есть хотя бы одна точка. \en If the knot vector is given, then it is necessary to check that there is at least one point between any two knots.  
  // \ru Если нет, то добавим точек, сохранив все уже набранные \en If not, then add the points and save old points 
  if ( aKnots.size() > 0 ) {
    SArray<double> pParams( 0, 1 );
    ::CreateSplineParameters( points, spt_ChordLength, false, pParams );
    c3d::SetLimitParam( pParams, aKnots.front(), aKnots.back() );

    double t1, t2;
    bool bRes = false;
    while ( !bRes ) {
      bRes = true;
      for ( i = 0; i < aKnots.MaxIndex() && bRes; i++ ) {
        t1 = aKnots[i];
        t2 = aKnots[i + 1];
        if ( ::fabs(t2 - t1) > NULL_EPSILON ) {
          size_t il = 0, ir = pParams.MaxIndex();
          size_t itemp = 0;
          size_t ires1 = 0;
          size_t ires2 = 0;
          // \ru Левая граница \en The left boundary 
          bool goOn = true;
          while ( goOn ) {
            if ( ::fabs(pParams[il] - t1) < NULL_EPSILON ){
              ires1 = il;
              goOn = false;
              break;
            }
            else if ( ::fabs(pParams[ir] - t1) < NULL_EPSILON ) {
              ires1 = ir;
              goOn = false;
              break;
            }
            else {
              itemp = ( il + ir ) / 2;
              if ( ::fabs(pParams[itemp] - t1) < NULL_EPSILON ) {
                ires1 = itemp;
                goOn = false;
                break;
              }
              if ( pParams[itemp] < t1 ) 
                il = itemp;
              else if ( pParams[itemp] > t1 )
                ir = itemp;
              if ( ir - il < 2) {
                ires1 = il;
                goOn = false;
                break;
              }
            }
          }
          // \ru Правая граница \en The right boundary 
          il = ires1;
          ir = pParams.MaxIndex();
          goOn = true;
          while ( goOn ) {
            if ( ::fabs(pParams[il] - t2) < NULL_EPSILON ){
              ires2 = il;
              goOn = false;
              break;
            }
            else if ( ::fabs(pParams[ir] - t2) < NULL_EPSILON ) {
              ires2 = ir;
              goOn = false;
              break;
            }
            else {
              itemp = ( il + ir ) / 2;
              if ( ::fabs(pParams[itemp] - t2) < NULL_EPSILON ) {
                ires2 = itemp;
                goOn = false;
                break;
              }
              if ( pParams[itemp] < t2 ) 
                il = itemp;
              else if ( pParams[itemp] > t2 )
                ir = itemp;
              if ( ir - il < 2) {
                ires2 = ir;
                goOn = false;
                break;
              }
            }
          }
          if ( ires2 - ires1 < 2 ) {
            bRes = false;
            // \ru Вставим среднюю точку между ires1 и ires2 и пересчитаем параметры \en Insert mid-point between ires1 and ires2 and recalculate parameters 
            t = 0.5 * (restList[ ires1 ] + restList[ ires2 ]);
            restList.AddAt( t, ires1 + 1 );

            curve._PointOn( t, p );
            points.AddAt( p, ires1 + 1 );

            pParams.clear();
            ::CreateSplineParameters( points, spt_ChordLength, false, pParams );
            c3d::SetLimitParam( pParams, aKnots.front(), aKnots.back() );
            pCountActual++;
          }
        }
      }
    }
  }
  return pCountActual;
}


//------------------------------------------------------------------------------
/** \brief \ru Построение замкнутого сплайна.
           \en Construction of closed spline. \~
  \details \ru Построение замкнутого сплайна, аппроксимирующего набор точек, с помощью метода наименьших квадратов. \n
           \en Construction of closed spline which approximates a set of points by the method of least squares. \n \~
  \param[in,out] nurbs - \ru Модифицируемый сплайн.
                         \en Modifiable spline. \~
  \param[in] aDegree   - \ru Порядок сплайна.
                         \en The spline order. \~
  \param[in] pCount    - \ru Количество точек.
                         \en Count of points. \~
  \param[in] aPoints   - \ru Набор точек для аппроксимации.
                         \en Point set for approximation. \~
  \param[in] aKnots    - \ru Предустановленный узловой вектор.
                         \en The predefined knots vector. \~
  \param[in] aParams   - \ru Параметры для набора точек.
                         \en The parameters for point set. \~
  \result \ru Возвращает true при успешном построении, в противном случае - false.
          \en Returns true if success, false - on other case. \~
  \ingroup Curve_Modeling
*/
// ---
template <class Nurbs, class PointsVector, class DoubleVector>
MATH_FUNC (bool) CreateNurbsLSMClosed(       SPtr<Nurbs> &  nurbs, // \ru Модифицируемый сплайн \en Modifiable spline  
                                       const ptrdiff_t      degree, 
                                       const ptrdiff_t      pCount,
                                       const PointsVector & aPoints,
                                       const DoubleVector & aKnots,
                                       const DoubleVector * aParams = nullptr );


//------------------------------------------------------------------------------
/** \brief \ru Построение незамкнутого сплайна.
           \en Construction of non-closed spline. \~
  \details \ru Построение незамкнутого сплайна, аппроксимирующего набор точек, с помощью метода наименьших квадратов. \n
           \en Construction of non-closed spline which approximates a set of points by the method of least squares. \n \~
  \param[in,out] nurbs - \ru Модифицируемый сплайн.
                         \en Modifiable spline. \~
  \param[in] aDegree   - \ru Порядок сплайна.
                         \en The spline order. \~
  \param[in] pCount    - \ru Количество точек.
                         \en Count of points. \~
  \param[in] aPoints   - \ru Набор точек для аппроксимации.
                         \en Point set for approximation. \~
  \param[in] aKnots    - \ru Предустановленный узловой вектор.
                         \en The predefined knots vector. \~
  \param[in] aParams   - \ru Параметры для набора точек.
                         \en The parameters for point set. \~
  \result \ru Возвращает true при успешном построении, в противном случае - false.
          \en Returns true if success, false - on other case. \~
  \ingroup Curve_Modeling
*/
// ---
template <class Nurbs, class PointsVector, class DoubleVector>
MATH_FUNC (bool) CreateNurbsLSM(       SPtr<Nurbs> &  nurbs, // \ru Модифицируемый сплайн \en Modifiable spline  
                                 const ptrdiff_t      degree, 
                                 const ptrdiff_t      pCount,
                                 const PointsVector & aPoints,
                                 const DoubleVector & aKnots,
                                 const DoubleVector * aParams = nullptr );


//-------------------------------------------------------------------------------
/// \ru Построение приямолинейногло сплайна. \en Construction of line segment spline. \~
// ---
template <class Curve, class Nurbs, class Point>
Nurbs * CreateLineOutRgn( const Curve & curve, double tn1, double tn2, double t1, double t2,
                          const MbCurveIntoNurbsInfo & nci )
{
  Nurbs * nurbs = nullptr;

  if ( !curve.IsClosed() && nci.ExtendRange() && ((tn2 - tn1) > nci.GetTRegion()) ) {
    SArray<Point>   points ( 2, 1 );
    SArray<double>  weights( 2, 1 );
    SArray<double>  knots  ( 4, 1 ); //-V112

    curve._PointOn( t1, *(points.Add()) );
    curve._PointOn( t2, *(points.Add()) );

    weights.Add( 1.0 );
    weights.Add( 1.0 );

    knots.Add( tn1 );
    knots.Add( tn1 );
    knots.Add( tn2 );
    knots.Add( tn2 );

    nurbs = Nurbs::Create( 2, false, points, weights, knots, ncf_Unspecified );
  }

  return nurbs;
}

#endif // __MB_NURBS_FUNCTION_H
