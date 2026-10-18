////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Параметры операций над полигональными сетками.
         \en Parameters of operations on the polygonal meshes. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __OP_MESH_PARAMETERS_H
#define __OP_MESH_PARAMETERS_H


#include <math_define.h>
#include <mesh_plane_grid.h>
#include <mesh_float_point.h>
#include <model_item.h>
#include <templ_sptr.h>
#include <cur_contour.h>
#include <op_binding_data.h>
#include <curve3d.h>
#include <curve.h>
#include <mesh.h>

class MbFaceShell;
class MbStepData;

//------------------------------------------------------------------------------
/** \brief \ru Параметры операции упрощения триангуляции.
           \en Triangulation simplification parameters. \~
    \details \ru Параметры операции упрощения триангуляции.
                 Упрощение может производиться до достижения заданного количества треугольников,
                 либо до превышения заданной точности.
                 Пользователь может задать порог двугранного угла - рёбра с углом больше порога не участвуют в процедуре упрощения.
                 Пользователь может задать порог угла между соседними граничными рёбрами -
                   участки слома границы не участвуют в процедуре упрощения.
             \en Triangulation simplification parameters.
                 Simplification can be carried out until target triangles count reaching or until
                 exceeding of specified tolerance.
                 The dihedral angle threshold can be specified - edges with a bigger angle will be skipped during simplification.
                 The threshold of the angle between adjacent boundary edges can be specified - 
                   boundary cracks will be skipped during simplification. \~
    \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbMeshSimplificationParams
{
public:
  //------------------------------------------------------------------------------
  /** \brief \ru Метод упрощения триангуляции.
             \en Triangulation simplification method. \~
    \details \ru Метод упрощения триангуляции.
                 На текущий момент пользователь может выбирать между более точной и более быстрой версиями.
             \en Triangulation simplification method.
                 At this moment the user can choose between the more precise version of the algorithm and the faster one. \~
    \warning \ru В разработке.
             \en Under development. \~
  */
  // ---
  enum class MbeMeshSimplificationMethod
  {
    precise, ///< \ru Более точный. \en More precise.
    fast     ///< \ru Более быстрый. \en Faster.
  };

private:
  c3d::DoubleVector           _toleranceLevels;     ///< \ru Уровни точностей для упрощения. \en Tolerance levels for simplification. \~
  c3d::IndicesVector          _triangleCountLevels; ///< \ru Уровни количества треугольников для упрощения. \en Triangle count levels for simplification. \~
  MbeMeshSimplificationMethod _method;              ///< \ru Метод упрощения триангуляции. \en Triangulation simplification method. \~
  double                      _thresholdEdAngle;    ///< \ru Порог двугранного угла (в градусах). \en The dihedral angle threshold (in degrees). \~
  double                      _thresholdVxAngle;    ///< \ru Порог угла между соседними рёбрами границы (в градусах). \en The threshold of the angle between adjacent boundary edges (in degrees). \~

public:
  mutable IProgressIndicator * _progress;  ///< \ru Индикатор прогресса выполнения операции. \en A progress indicator of the operation.

public:
  /// \ru Конструктор для задания целевй точности. \en Constructor for tolerance target. \~
  MbMeshSimplificationParams( const c3d::DoubleVector & toleranceLevels, IProgressIndicator * prog = nullptr )
    : _toleranceLevels    ( toleranceLevels                   )
    , _triangleCountLevels(                                   )
    , _method             ( MbeMeshSimplificationMethod::fast )
    , _thresholdEdAngle   ( -1.                               )
    , _thresholdVxAngle   ( -1.                               )
    , _progress           ( prog                              )
  {}

  /// \ru Конструктор для задания целевого количества треугольников. \en Constructor for triangles count target. \~
  MbMeshSimplificationParams( const c3d::IndicesVector & triangleCountLevels, IProgressIndicator * prog = nullptr )
    : _toleranceLevels    (                                   )
    , _triangleCountLevels( triangleCountLevels               )
    , _method             ( MbeMeshSimplificationMethod::fast )
    , _thresholdEdAngle   ( -1.                               )
    , _thresholdVxAngle   ( -1.                               )
    , _progress           ( prog                              )
  {}

  /// \ru Деструктор. \en Destructor. \~
  ~MbMeshSimplificationParams() {}

  ///< \ru Делать ли упрощение до достижения целевого количества треугольников. \en Is target triangles count. \~
  bool IsTriangleCountTarget() const { return !_triangleCountLevels.empty(); }

  ///< \ru Делать ли упрощение до достижения целевой точности. \en Is target tolerance. \~
  bool IsToleranceTarget() const { return !_toleranceLevels.empty(); }

  ///< \ru Получить уровни упрощения для точности. \en Get simplification levels for tolerance. \~
  const c3d::DoubleVector & ToleranceLevels() const { return _toleranceLevels; }

  ///< \ru Получить уровни упрощения для количества треугольников. \en Get simplification levels for triangles count. \~
  const c3d::IndicesVector & TrianglesCountLevels() const { return _triangleCountLevels; }

  ///< \ru Валидны ли параметры? \en Is parameters valid? \~
  bool IsValid() const
  {
    bool isValid = true;
    if ( IsTriangleCountTarget() )
    {
      if ( _triangleCountLevels.size() > 1 )
      {
        auto valPrev = _triangleCountLevels.front();
        for ( size_t i = 1, n = _triangleCountLevels.size(); i < n; ++i )
        {
          const auto val = _triangleCountLevels[i];
          if ( val >= valPrev )
          {
            isValid = false;
            break;
          }
          valPrev = val;
        }
      }
    }
    else if ( IsToleranceTarget() )
    {
      if ( _toleranceLevels.size() > 1 )
      {
        auto valPrev = _toleranceLevels.front();
        for ( size_t i = 1, n = _toleranceLevels.size(); i < n; ++i )
        {
          const auto val = _toleranceLevels[i];
          if ( val <= valPrev )
          {
            isValid = false;
            break;
          }
          valPrev = val;
        }
      }
    }
    return isValid;
  }

  ///< \ru Установить метод упрощения. \en Set the simplification method. \~
  void SetSimplificationMethod( MbeMeshSimplificationMethod method ) { _method = method; }

  ///< \ru Получить метод упрощения. \en Get the simplification method. \~
  MbeMeshSimplificationMethod GetSimplificationMethod() const { return _method; }

  ///< \ru Установить порог двугранного угла для коллапса рёбер (в градусах). \en Set the dihedral angle threshold for collapsing edges (in degrees). \~
  void SetDihedralAngleThreshold( double angle ) { _thresholdEdAngle = angle; }

  ///< \ru Получить порог двугранного угла для коллапса рёбер (в градусах). \en Get the dihedral angle threshold for collapsing edges (in degrees). \~
  double GetDihedralAngleThreshold() const { return _thresholdEdAngle; }

  ///< \ru Установить порог угла между соседними рёбрами границы (в градусах). \en Set the threshold of the angle between adjacent boundary edges (in degrees). \~
  void SetAdjacentBoundaryEdgesAngleThreshold( double angle ) { _thresholdVxAngle = angle; }

  ///< \ru Получить порог угла между соседними рёбрами границы (в градусах). \en Get the threshold of the angle between adjacent boundary edges (in degrees). \~
  double GetAdjacentBoundaryEdgesAngleThreshold() const { return _thresholdVxAngle; }

  OBVIOUS_PRIVATE_COPY( MbMeshSimplificationParams )
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры операции создания выпуклого многоугольника в 2D.
           \en Parameters for convex hull 2D creation. \~
    \details \ru Параметры операции создания выпуклого многоугольника в 2D.
             \en Parameters for convex hull 2D creation. \~
    \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbConvexHullParams
{
private:
  bool                   _makeGrid;     ///< \ru Создавать ли двумерную триангуляцию. \en Should planar grid be created. \~
  bool                   _makeDelaunay; ///< \ru Создавать ли Делоне триангуляцию. \en Should planar grid be Delaunay triangulation. \~
  bool                   _makeContour;  ///< \ru Создавать ли двумерный контур. \en Should planar contour be created. \~
  double                 _tolerance;    ///< \ru Точность. \en Tolerance. \~
  c3d::ParamPointsVector _points;       ///< \ru Точки. \en Points. \~

public:
  /// \ru Конструктор.. \en Constructor. \~
  MbConvexHullParams( const c3d::ParamPointsVector & points, double tolerance, bool makeContour, bool makeGrid, bool makeDelaunay )
    : _makeGrid    ( makeGrid     )
    , _makeDelaunay( makeDelaunay )
    , _makeContour ( makeContour  )
    , _tolerance   ( tolerance    )
    , _points      ( points       )
  {}
  /// \ru Конструктор.. \en Constructor. \~
  MbConvexHullParams( const std::vector<MbFloatPoint> & points, double tolerance, bool makeContour, bool makeGrid, bool makeDelaunay )
    : _makeGrid    ( makeGrid     )
    , _makeDelaunay( makeDelaunay )
    , _makeContour ( makeContour  )
    , _tolerance   ( tolerance    )
  {
    _points.clear();
    _points.reserve( points.size() );
    for ( const auto & pt : points )
      _points.push_back( MbCartPoint( pt ) );
  }
  /// \ru Конструктор.. \en Constructor. \~
  MbConvexHullParams( const SArray<MbFloatPoint> & points, double tolerance, bool makeContour, bool makeGrid, bool makeDelaunay )
    : _makeGrid    ( makeGrid     )
    , _makeDelaunay( makeDelaunay )
    , _makeContour ( makeContour  )
    , _tolerance   ( tolerance    )
  {
    _points.clear();
    _points.reserve( points.size() );
    for ( const auto & pt : points )
      _points.push_back( MbCartPoint( pt ) );
  }
  /// \ru Конструктор.. \en Constructor. \~
  MbConvexHullParams( const SArray<MbCartPoint> & points, double tolerance, bool makeContour, bool makeGrid, bool makeDelaunay )
    : _makeGrid    ( makeGrid     )
    , _makeDelaunay( makeDelaunay )
    , _makeContour ( makeContour  )
    , _tolerance   ( tolerance    )
  {
    _points.clear();
    _points.reserve( points.size() );
    for ( const auto & pt : points )
      _points.push_back( pt );
  }
  
  /// \ru Деструктор. \en Destructor. \~
  ~MbConvexHullParams() {}

  ///< \ru Создавать ли двумерную триангуляцию. \en Should planar grid be created. \~
  bool   ShouldMakeGrid()     const { return _makeGrid; }
  ///< \ru Создавать ли Делоне триангуляцию. \en Should planar grid be Delaunay triangulation. \~
  bool   ShouldMakeDelaunay() const { return _makeDelaunay; }
  ///< \ru Создавать ли двумерный контур. \en Should planar contour be created. \~
  bool   ShouldMakeContour()  const { return _makeContour; }
  ///< \ru Получить точность. \en Get tolerance. \~
  double GetTolerance()       const { return _tolerance; }
  ///< \ru Получить точки. \en Get points. \~
  const c3d::ParamPointsVector & GetPoints() const { return _points; }

  OBVIOUS_PRIVATE_COPY( MbConvexHullParams )
};


//------------------------------------------------------------------------------
/** \brief \ru Результаты создания выпуклого многоугольника в 2D.
           \en Results of convex hull 2D creating. \~
  \details \ru Результаты создания выпуклого многоугольника в 2D.
           \en Results of convex hull 2D creating. \~
  \ingroup Shell_Building_Parameters
*/
// ---
struct MATH_CLASS MbConvexHullResults
{
public:
  c3d::IndicesVector     _hullIndices; ///< \ru Индексы точек выпуклого многоугольника. \en Indices of convex hull points. \~
  c3d::ParamPointsVector _hull;        ///< \ru Выпуклый многоугольник. \en Convex hull. \~
  SPtr<MbPlanarGrid>     _grid;        ///< \ru Точки. \en Points. \~
  SPtr<MbContour>        _contour;     ///< \ru Контур. \en Contour. \~

  /// \ru Конструктор.. \en Constructor. \~
  MbConvexHullResults()
    : _hullIndices()
    , _hull()
    , _grid()
    , _contour()
  {}
  /// \ru Деструктор. \en Destructor. \~
  ~MbConvexHullResults() {}

  OBVIOUS_PRIVATE_COPY( MbConvexHullResults )
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры операции создания выпуклой оболочки в 3D.
           \en Parameters for convex hull 3D creation. \~
    \details \ru Параметры операции создания выпуклой оболочки в 3D.
             \en Parameters for convex hull 3D creation. \~
    \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbConvexHullParams3D
{
private:
  double                 _tolerance;  ///< \ru Точность. \en Tolerance. \~
  c3d::SpacePointsVector _points;     ///< \ru Точки. \en Points. \~

public:
  /// \ru Конструктор.. \en Constructor. \~
  MbConvexHullParams3D( const c3d::SpacePointsVector & points, double tolerance )
    : _tolerance( tolerance )
    , _points   ( points    )

  {}
  /// \ru Конструктор.. \en Constructor. \~
  MbConvexHullParams3D( const std::vector<MbFloatPoint3D> & points, double tolerance )
    : _tolerance( tolerance )
  {
    _points.clear();
    _points.reserve( points.size() );
    for ( const auto & pt : points )
      _points.push_back( MbCartPoint3D( pt ) );
  }
  /// \ru Конструктор.. \en Constructor. \~
  MbConvexHullParams3D( const SArray<MbFloatPoint3D> & points, double tolerance )
    : _tolerance( tolerance )
  {
    _points.clear();
    _points.reserve( points.size() );
    for ( const auto & pt : points )
      _points.push_back( MbCartPoint3D( pt ) );
  }
  /// \ru Конструктор.. \en Constructor. \~
  MbConvexHullParams3D( const SArray<MbCartPoint3D> & points, double tolerance )
    : _tolerance( tolerance )
  {
    _points.clear();
    _points.reserve( points.size() );
    for ( const auto & pt : points )
      _points.push_back( pt );
  }

  /// \ru Деструктор. \en Destructor. \~
  ~MbConvexHullParams3D() {}

  ///< \ru Получить точность. \en Get tolerance. \~
  double GetTolerance() const { return _tolerance; }
  ///< \ru Получить точки. \en Get points. \~
  const c3d::SpacePointsVector & GetPoints() const { return _points; }

  OBVIOUS_PRIVATE_COPY( MbConvexHullParams3D )
};


//------------------------------------------------------------------------------
/** \brief   \ru Параметры для исправления несогласованных полигональных объектов (#MbMesh).
             \en Parameters for the inconsistent meshes (#MbMesh) repair. \~
    \details \ru Параметры для исправления несогласованных полигональных объектов (#MbMesh).
                 Исправляет несогласованные нормали. Позволяет дополнительно к основному алгоритму
                 ориентировать нормали вовне, если это направление может быть однозначно определено.\n
             \en Parameters for the inconsistent meshes (#MbMesh) repair.
                 Fixes inconsistent normals. In addition to the main flow can orient normals outward.
                 if this direction can be uniquely determined.\n \~
    \ingroup Polygonal_Objects
*/
// ---
class MATH_CLASS MbRepairInconsistentMeshParams
{
private:
  /// \ru Ориентировать нормали вовне, если это возможно. \en Orient normals outward if possible.\~
  bool   _orientOutward;
  /// \ru Точность объединения вершин (не используется, если отрицательная). \en A tolerance for vertices merging (not used if negative).\~
  double _tolerance;
  /// \ru Двуграный угол для расчета нормалей (не используется, если отрицательный). \en A dihedral angle for normal vectors calculation (not used if negative).\~
  double _angleSharp;
  /// \ru Флаг усреднения нормалей по веерам объединенных вершин. \en A flag for normal vectors averaging around merged vertices fans.\~
  bool _bNormalAveraging;

public:
  /// \ru Конструктор по-умолчанию. \en Default constructor.\~
  MbRepairInconsistentMeshParams()
    : _orientOutward   ( false          )
    , _tolerance       ( METRIC_EPSILON )
    , _angleSharp      ( -1.            )
    , _bNormalAveraging( true           )
  {}
  /// \ru Конструктор. \en Constructor. \~;
  MbRepairInconsistentMeshParams( bool orientOutward );
  /// \ru Конструктор. \en Constructor. \~;
  MbRepairInconsistentMeshParams( double tolerance, bool orientOutward, double sharpAngle, bool normalAveraging );
  /// \ru Деструктор. \en Destructor. \~
  ~MbRepairInconsistentMeshParams() {}

  /// \ru Установить ориентирование нормалей вовне. \en Set orienting of normal vectors outwards. \~
  void SetOrientOutward( bool orient ) { _orientOutward = orient; }
  /// \ru Ориентировать ли нормали вовне. \en Whether to orient normal vectors outwards. \~
  bool IsOrientOutward() const { return _orientOutward; }
  /// \ru Установить точность. \en Set tolerance. \~
  void SetTolerance( double tolerance ) { _tolerance = tolerance; }
  /// \ru Получить точность. \en Get tolerance. \~
  double GetTolerance() const { return _tolerance; }
  /// \ru Установить угол. \en Set angle. \~
  void SetSharpAngle( double angle ) { _angleSharp = angle; }
  /// \ru Получить угол. \en Get angle. \~
  double GetSharpAngle() const { return _angleSharp; }
  /// \ru Установить усреднение нормалей. \en Set averaging of normal vectors. \~
  void SetNormalsAveraging( bool flag ) { _bNormalAveraging = flag; }
  /// \ru Усреднять ли нормали. \en Whether to average normal vectors. \~
  bool NeedNormalsAveraging() const { return _bNormalAveraging; }

  OBVIOUS_PRIVATE_COPY( MbRepairInconsistentMeshParams );
};


//------------------------------------------------------------------------------
/** \brief   \ru Параметры для поиска и замены повторяющихся полигональных объектов (#MbMesh) на вставки #MbInstance.
             \en Parameters for the replacement of repetative meshes (#MbMesh) into #MbInstance. \~
    \details \ru Параметры для поиска и замены одинаковых полигональных объектов (#MbMesh) на вставки #MbInstance.
                 Возможно переключение между абсолютной/относительной точностями сравнения точек. Под относительной
                 точностью имеется в виду ограничение сверху по каждой координате на abs( (c1 - c2) / (c1 + c2) ). \n
             \en Parameters for the replacement of repetative meshes (#MbMesh) into #MbInstance.
                 It is possible to switch between absolute/relative accuracy for the points comparison. The relative
                 accuracy is the upper bound of the expression abs( (c1 - c2) / (c1 + c2) ) for each coordinate. \n \~
    \ingroup Polygonal_Objects
*/
// ---
class MATH_CLASS MbMeshToInstanceParams : public MbPrecision
{
private:
  /// \ru Использовать относительную или абсолютную точность при сравнении точек.\en Use absolute or relative tolerance for points comparison.\~
  bool                   _isAbsoluteAccuracy;
  /// \ru Все объекты выходного массива - вставки.\en All objects in the output array are instances.\~
  bool                   _replaceOriginByInstance;
public:
  /// \ru Конструктор по-умолчанию. \en Default constructor.\~
  MbMeshToInstanceParams      ()
    : MbPrecision             (       )
    , _isAbsoluteAccuracy     ( true  )
    , _replaceOriginByInstance( false )
  {}
  /// \ru Конструктор. \en Constructor. \~
  MbMeshToInstanceParams      ( double precision, bool isAbsoluteAccuracy )
    : MbPrecision             ( precision, ANGLE_REGION )
    , _isAbsoluteAccuracy     ( isAbsoluteAccuracy      )
    , _replaceOriginByInstance( false                   )
  {}

  /// \ru Конструктор. \en Constructor. \~
  MbMeshToInstanceParams      ( double precision, bool isAbsoluteAccuracy, bool eachObjectInstancing )
    : MbPrecision             ( precision, ANGLE_REGION )
    , _isAbsoluteAccuracy     ( isAbsoluteAccuracy      )
    , _replaceOriginByInstance( eachObjectInstancing    )
  {}
  /// \ru Деструктор. \en Destructor. \~
  ~MbMeshToInstanceParams() {}

  /// \ru Переключатель между абсолютной и относительной точностью. \en Switch between absolute and relative accuracy. \~
  void   SetAbsoluteAccuracy( bool isAbs )       { _isAbsoluteAccuracy = isAbs; }
  /// \ru Установлена абсолютная или относительная точность. \en Whether the accuracy is set to absolute or relative. \~
  bool   IsAbsoluteAccuracy()              const { return _isAbsoluteAccuracy; }
  /// \ru Все объекты выходного массива - вставки или нет. \en All objects in the output array are instances or not. \~
  bool   IsOriginReplacedByInstance()      const { return _replaceOriginByInstance; }

  OBVIOUS_PRIVATE_COPY( MbMeshToInstanceParams );
};


//------------------------------------------------------------------------------
/** \brief   \ru Результаты поиска и замены повторяющихся полигональных объектов (#MbMesh) на вставки #MbInstance.
             \en Results of the replacement of repetative meshes (#MbMesh) into #MbInstance. \~
    \details \ru Результаты поиска и замены одинаковых полигональных объектов (#MbMesh) на вставки #MbInstance.
                 Содержат присланный массив, где на месте повторяющихся полигональных объектов расположены вставки. \n
             \en Results of the replacement of repetative meshes (#MbMesh) into #MbInstance. 
                 Contain the initial array, where instances replace repeatative meshes.\n \~
    \ingroup Polygonal_Objects
*/
// ---
class MATH_CLASS MbMeshToInstanceResults
{
public:
  c3d::ItemsSPtrVector _meshContainer; ///< \ru Выходной массив. \en Output array \~
  size_t               _nReplaced;     ///< \ru Число заменённых объектов. \en Number of replaced objects. \~

public:
  /// \ru Конструктор. \en Constructor. \~
  MbMeshToInstanceResults()
    : _nReplaced ( 0 )
  {}
  /// \ru Деструктор. \en Destructor. \~
  ~MbMeshToInstanceResults() {}

  /// \ru Обнулить. \en Reset. \~
  void Reset () { _nReplaced = 0; _meshContainer.clear(); }

  OBVIOUS_PRIVATE_COPY( MbMeshToInstanceResults );
};


//------------------------------------------------------------------------------
/** \brief \ru Режим отбраковки выбросов при вписывании поверхности в набор точек.
           \en Outliers rejection mode for surface fitting. \~
  \ingroup Polygonal_Objects
*/
// ---
enum MbeRejectOutliersType
{
  rot_NoReject = 0,     ///< \ru Не отбраковывать выбросы. \en No outliers rejection.
  rot_Sigma = 1,        ///< \ru Отбраковывать выбросы по критерию N*sigma. \en Reject outliers by N*sigma criterion.
  rot_Percent = 2,      ///< \ru Отбраковывать выбросы по процентному критерию. \en Reject outliers by percent criterion.
  rot_MaxDistance = 3   ///< \ru Отбраковывать выбросы по критерию максимального отклонения. \en Reject outliers by maximum distance criterion.
};


//------------------------------------------------------------------------------
/** \brief \ru Базовый класс для параметров вписывания кривых.
           \en Base class for curve fitting parameters. \~
  \ingroup Polygonal_Objects
*/
// ---
class MATH_CLASS MbCurveFitParametersBase : public MbPrecision
{
public:
  /** \brief \ru Тип кривой для вписывания.
             \en Type of a curve to be fit. \~
    \ingroup Polygonal_Objects
  */
  enum MbeCurveFitType
  {
    cft_Undef = 0,    ///< \ru Не определено. \en Undefined.
    cft_Line = 1,     ///< \ru Прямая. \en Line.
    cft_Circle = 2,   ///< \ru Окружность. \en Circle.
    cft_Ellipse = 3   ///< \ru Эллипс. \en Ellipse.
  };

private:
  MbeRejectOutliersType _typeReject;          ///< \ru Способ отбраковки выбросов. \en Outliers rejection mode.
  double                _valueReject;         ///< \ru Пороговое значения для отбраковки выбросов. \en Outliers rejection mode treshold value.
  MbeCurveFitType       _curveType;           ///< \ru Тип кривой. \en A curve type.

public:
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  explicit MbCurveFitParametersBase( double                tolerance,
                                     MbeRejectOutliersType typeReject,
                                     double                valueReject,
                                     MbeCurveFitType       curveType )
    : MbPrecision ( tolerance, tolerance )
    , _typeReject ( typeReject           )
    , _valueReject( valueReject          )
    , _curveType  ( curveType            )
  {}

  /// \ru Выдать метод отбраковки выбросов. \en Get outliers rejection mode.
  MbeRejectOutliersType GetOutliersRejectionMode() const { return _typeReject; }
  /// \ru Выдать пороговое значения для отбраковки выбросов. \en Get outliers rejection mode treshold value. 
  double GetOutliersRejectionValue() const { return _valueReject; }
  /// \ru Выдать тип кривой. \en Get curve type.
  MbeCurveFitType GetCurveType() const { return _curveType; }

  OBVIOUS_PRIVATE_COPY( MbCurveFitParametersBase )
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры вписывания 2Д кривых.
           \en 2D curve fitting parameters. \~
  \ingroup Polygonal_Objects
*/
// ---
class MATH_CLASS MbCurveFitParameters : public MbCurveFitParametersBase
{
private:
  c3d::ParamPointsVector _points;    ///< \ru Точки. \en Points.

public:
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  explicit MbCurveFitParameters( MbeCurveFitType       curveType,
                                 double                tolerance,
                                 MbeRejectOutliersType typeReject,
                                 double                valueReject )
    : MbCurveFitParametersBase( tolerance, typeReject, valueReject, curveType )
  {}

  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  explicit MbCurveFitParameters( MbeCurveFitType       curveType,
                                 double                tolerance )
    : MbCurveFitParametersBase( tolerance, rot_NoReject, 0., curveType )
  {}

  /// \ru Установить точки для вписывания. \en Set points to fit in.
  void SetPoints( const c3d::ParamPointsVector & points ) { _points = points; }
  /// \ru Получить точки для вписывания. \en Get points to fit in.
  const c3d::ParamPointsVector & GetPoints() const { return _points; }

  OBVIOUS_PRIVATE_COPY( MbCurveFitParameters )
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры вписывания 3Д кривых.
           \en 3D curve fitting parameters. \~
  \ingroup Polygonal_Objects
*/
// ---
class MATH_CLASS MbCurveFitParameters3D : public MbCurveFitParametersBase
{
private:
  c3d::SpacePointsVector _points;    ///< \ru Точки. \en Points.

public:
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  explicit MbCurveFitParameters3D( MbeCurveFitType       curveType,
                                   double                tolerance,
                                   MbeRejectOutliersType typeReject,
                                   double                valueReject )
    : MbCurveFitParametersBase( tolerance, typeReject, valueReject, curveType )
  {}

  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  explicit MbCurveFitParameters3D( MbeCurveFitType       curveType,
                                   double                tolerance )
    : MbCurveFitParametersBase( tolerance, rot_NoReject, 0., curveType )
  {}

  /// \ru Установить точки для вписывания. \en Set points to fit in.
  void SetPoints( const c3d::SpacePointsVector & points ) { _points = points; }
  /// \ru Получить точки для вписывания. \en Get points to fit in.
  const c3d::SpacePointsVector & GetPoints() const { return _points; }

  OBVIOUS_PRIVATE_COPY( MbCurveFitParameters3D )
};


//------------------------------------------------------------------------------
/** \brief \ru Базовый класс для результатов вписывания.
           \en Base class for fitting results. \~
  \ingroup Polygonal_Objects
*/
// ---
class MATH_CLASS MbFitResultsBase
{
private:
  double _devMax;    ///< \ru Максимальное получившееся отклонение. \en A maximal deviation. 
  double _devMean;   ///< \ru Среднее получившееся отклонение. \en A mean deviation. 
  double _rmsd;      ///< \ru Среднее квадратическое отклонение. \en A root-mean-square deviation. 

public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  explicit MbFitResultsBase()
    : _devMax ( 0.0 )
    , _devMean( 0.0 )
    , _rmsd   ( 0.0 )
  {}
  /// \ru Инициализировать данные. \en Initialize data.
  void Init( double devMax, double devMean, double rmsd ) {
    _devMax = devMax;
    _devMean = devMean;
    _rmsd = rmsd;
  }
  /// \ru Выдать максимальное отклонение. \en Get maximal deviation.
  double GetTolerance() const { return _devMax; }
  /// \ru Выдать среднее отклонение. \en Get mean deviation.
  double GetMeanDeviation() const { return _devMean; }
  /// \ru Выдать среднее квадратическое отклонение. \en Get root-mean-square deviation.
  double GetRmsd() const { return _rmsd; }

  OBVIOUS_PRIVATE_COPY( MbFitResultsBase )
};


//------------------------------------------------------------------------------
/** \brief \ru Результат вписывания 2Д кривой.
           \en 2D curve fitting results. \~
  \ingroup Polygonal_Objects
*/
// ---
class MATH_CLASS MbCurveFitResults : public MbFitResultsBase
{
private:
  SPtr<MbCurve> _curve; ///< \ru Кривая. \en A curve.

public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  explicit MbCurveFitResults()
    : MbFitResultsBase(         )
    , _curve          ( nullptr )
  {}
  /// \ru Инициализировать данные. \en Initialize data.
  void Init( MbCurve * pCurve, double devMax, double devMean, double rmsd ) 
  {
    _curve = pCurve;
    MbFitResultsBase::Init( devMax, devMean, rmsd );
  }
  /// \ru Выдать поверхность. \en Get surface.
  SPtr<MbCurve> GetCurve() const { return _curve; }

  OBVIOUS_PRIVATE_COPY( MbCurveFitResults )
};


//------------------------------------------------------------------------------
/** \brief \ru Результат вписывания 3Д кривой.
           \en 2D curve fitting results. \~
  \ingroup Polygonal_Objects
*/
// ---
class MATH_CLASS MbCurveFitResults3D : public MbFitResultsBase
{
private:
  SPtr<MbCurve3D> _curve; ///< \ru Кривая. \en A curve.

public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  explicit MbCurveFitResults3D()
    : MbFitResultsBase()
    , _curve( nullptr )
  {}
  /// \ru Инициализировать данные. \en Initialize data.
  void Init( MbCurve3D * pCurve, double devMax, double devMean, double rmsd )
  {
    _curve = pCurve;
    MbFitResultsBase::Init( devMax, devMean, rmsd );
  }
  /// \ru Выдать поверхность. \en Get surface.
  SPtr<MbCurve3D> GetCurve() const { return _curve; }

  OBVIOUS_PRIVATE_COPY( MbCurveFitResults3D )
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры развёртки триангуляции на плоскость.
           \en Parameter for an unwrapping the mesh on a plane. \~
  \details \ru Параметры развёртки триангуляции на плоскость. \n
                 Параметры содержат информацию о положении развёртки и свойствах материала.
           \en Parameter for an unwrapping the mesh on a plane. \n
                 The parameters contain information about the scan position and material properties. \~
  \ingroup Shell_Building_Parameters
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
class MATH_CLASS MbMeshUnwrapParams
{
protected:
  MbPlacement3D _place;        ///< \ru Локальная система координат развернутой поверхности. \en The local coordinate system for result surface. \~
  double        _coefPoisson;  ///< \ru Коэффициент Пуассона материала поверхности. \en The Poisson's ratio of suface material. \~
  SPtr<MbMesh>  _baseMesh;     ///< \ru Базовая триангуляция. \en The base triangulation. \~
  SPtr<MbMesh>  _remeshedMesh; ///< \ru Ремешированная триангуляция. \en The remeshed triangulation. \~

public:
  /** \brief \ru Конструктор по умолчанию.
             \en Default constructor. \~
  */
  MbMeshUnwrapParams();

  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  MbMeshUnwrapParams( const MbPlacement3D & place, double poisson = 0.25 );

  /// \ru Конструктор копирования. \en Copy-constructor.
  MbMeshUnwrapParams( const MbMeshUnwrapParams & that );

  /// \ru Деструктор. \en Destructor.
  ~MbMeshUnwrapParams() {}

public:
  /// \ru Оператор присваивания. \en Assignment operator.
  MbMeshUnwrapParams & operator = ( const MbMeshUnwrapParams & that );
  /// \ru Установить локальную систему координат развернутой поверхности сетки. \en Set local coordinate system for mesh unwrapping. \~
  void SetPlacement( const MbPlacement3D & pl ) { _place.Init( pl ); }
  // \ru Получить локальную систему координат развернутой поверхности сетки. \en Get local coordinate system for mesh unwrapping. \~
  const MbPlacement3D & GetPlacement() const { return _place; }
  /// \ru Установить коэффициент Пуассона материала грани. \en Set the Poisson's ratio of face material. \~
  void SetPoissonsRatio( double poisson ) { _coefPoisson = poisson; }
  /// \ru Получить коэффициент Пуассона материала грани. \en Get the Poisson's ratio of face material. \~
  double GetPoissonsRatio() const { return _coefPoisson; }
  /// \ru Получить базовую сетку. \en Get the base mesh. \~
  const SPtr<MbMesh> & GetBaseMesh() const { return  _baseMesh; }
  /// \ru Получить ремешированную сетку. \en Get the remeshed mesh. \~
  const SPtr<MbMesh> & GetRemeshedMesh() const { return _remeshedMesh; }

  /// \ru Преобразовать объект согласно матрице. \en Transform an object according to the matrix.
  void Transform( const MbMatrix3D & matr );
  /// \ru Сдвинуть объект вдоль вектора. \en Move an object along a vector.
  void Move( const MbVector3D & to );
  /// \ru Повернуть объект вокруг оси на заданный угол. \en Rotate an object at a given angle around an axis.
  void Rotate( const MbAxis3D & axis, double ang );
  /// \ru Являются ли объекты равными? \en Determine whether an object is equal? 
  bool IsSame( const MbMeshUnwrapParams & that, double accuracy ) const;
  /// \ru Инициализировать базовую сетку по оболочке. \en Initialize base mesh by shell.
  bool InitBaseMesh( const MbFaceShell & shell, const MbStepData & stepData, bool bParameterize = true );
  /// \ru Инициализировать ремешированную сетку с заданным размером треугольника (если triangleSize < 0, размер будет оценен автоматически). \en Initialize remeshed mesh with defined triangle size. If triangleSize < 0, then it will be estimated automatically.
  bool InitRemeshedMesh( double triangleSize, bool bReparam = false );

  KNOWN_OBJECTS_RW_REF_OPERATORS( MbMeshUnwrapParams ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class.
}; // MbMeshUnwrapParams


//------------------------------------------------------------------------------
/** \brief \ru Результат развёртки триангуляции на плоскость.
           \en Result of an unwrapping the mesh on a plane. \~
  \details \ru Результат развёртки триангуляции на плоскость. \n
           \en Result of an unwrapping the mesh on a plane. \n
  \ingroup Shell_Building_Parameters
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
class MATH_CLASS MbMeshUnwrapResult
{
protected:
  MbPlacement3D _place;         ///< \ru Локальная система координат развернутой поверхности грани. \en The local coordinate system for result surface. \~
  double        _coefPoisson;   ///< \ru Коэффициент Пуассона материала грани. \en The Poisson's ratio of face material. \~
  size_t        _idNearest;     ///< \ru Индекс ближайшего к началу локальной системы координат треугольника. \en Index of the nearest to the local coordinate system origin triangle. \~
  SPtr<MbMesh>  _unwrappedMesh; ///< \ru Результат - триангуляция (3Д копия сетки из MbMeshUnwrapParams) с заполненными 2Д параметрами точек на плоскости развертки. \en Result - mesh (3D copy of the mesh from MbMeshUnwrapParams ) with filled 2D parameters on the unwrapped plane. \~

public:
  /** \brief \ru Конструктор по умолчанию.
             \en Default constructor. \~
  */
  MbMeshUnwrapResult();

  /// \ru Деструктор. \en Destructor.
  ~MbMeshUnwrapResult() {}

public:
  /// \ru Получить локальную систему координат развернутой поверхности сетки. \en Get local coordinate system for mesh unwrapping. \~
  const MbPlacement3D & GetPlacement() const { return _place; }
  // \ru Получить коэффициент Пуассона материала грани. \en Get the Poisson's ratio of face material. \~
  double GetPoissonsRatio() const { return _coefPoisson; }
  /// \ru Получить  сетку. \en Get the base mesh. \~
  const SPtr<MbMesh> & GetUnwrappedMesh() const { return  _unwrappedMesh; }
  /// \ru Получить индекс ближайшего треугольника. \en Get the nearest triangle index. \~
  size_t GetNearestTriangleId() const { return _idNearest; }

  /// \ru Рассчитать плоскую триангуляцию-развертку. \en Calculate the flat unwrapped mesh. \~
  void GetFlatUnwrappedMesh( SPtr<MbMesh> & flatMesh ) const;
  /// \ru Рассчитать регулярную прямоугольную сетку с заданным размером ячейки. \en Calculate the regular quad mesh with defined cell size. \~
  void GetQuadMesh( double cellSize, SPtr<MbMesh> & quadMesh );
  /// \ru Получить деформации треугольников. \en Get triangles deformations. \~
  void GetTrianglesDeformations( c3d::SpaceVectorsVector & deformations ) const;
  /// \ru Получить напряжения треугольников для заданного модуля Юнга. \en Get triangles strains for defined Young modulus. \~
  void GetTrianglesStrains( double young, c3d::DoubleVector & strains ) const;

  /// \ru Функции для внутреннего использования. \en Functions for internal use. \~
  /// \ru Инициализировать по параметрам. \en Initialize by parameters. \~
  void InitByParams( const MbMeshUnwrapParams & params );
  /// \ru Получить ремешированную сетку. \en Get the remeshed mesh. \~
  SPtr<MbMesh> & SetUnwrappedMesh() { return _unwrappedMesh; }
  /// \ru Установить индекс ближайшего треугольника. \en Set the nearest triangle index. \~
  void SetNearestTriangleId( size_t id ) { _idNearest = id; }

  OBVIOUS_PRIVATE_COPY( MbMeshUnwrapResult );
}; // MbMeshUnwrapResult


//------------------------------------------------------------------------------
/** \brief \ru Режим использования подсказки при совмещении объектов по алгоритму ICP.
           \en Hint usage mode for object alignment by the ICP algorithm. \~
  \details \ru Режим использования подсказки при совмещении объектов по алгоритму ICP.
               Режим без использования подсказки подразумевает совмещение объектов в их исходном положении.
               Другие режимы допускают начальную трансформацию движущегося объекта.
           \en Hint usage mode for object alignment by the ICP algorithm.
               The mode without any hint implies object alignment from their initial positions.
               Other modes assume some initial transformation of a moving object. \~
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
enum class MbeIcpHintUsageMode
{
  noHint,   ///< \ru Не использовать подсказку. \en Do not use any hint.
  autoHint, ///< \ru Автоопределение подсказки по инерционным характеристикам. \en Automatic detection of a hint by means of inertial properties.
  givenHint ///< \ru Использовать подсказку, заданную пользователем. \en Use a hint given by a user.
};


//------------------------------------------------------------------------------
/** \brief \ru Режим отбраковки точек при совмещении объектов по алгоритму ICP.
           \en Point rejection mode for object alignment by the ICP algorithm. \~
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
enum class MbeIcpPointRejectionMode
{
  noReject,  ///< \ru Не отбраковывать точки. \en No point rejection.
  automatic, ///< \ru Автоопределение порога для отбраковки. \en Automatic detection of a rejection threshold.
  constant   ///< \ru Отбраковка заданной доли из общего числа точек. \en Reject a given percentage of points.
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры совмещения объектов.
           \en Parameters of object alignment. \~
  \details \ru Параметры совмещения объектов.
               В соответствии с полем _hintUsageMode возможны три режима работы алгоритма:
               - без использования подсказки (объекты совмещаются в их исходном положении; подходит для объектов с частичным перекрытием),
               - с автоматическим определением подсказки (к движущемуся объекту применяется начальная трансформация, 
                 рассчитанная по инерционным характеристикам объектов; подходит для одинаковых или почти одинаковых по форме объектов),
               - с набором заданных подсказок (алгоритм совмещения запускается для набора начальных трансформаций из массива _hintArray,
                 выбирается лучший результат).
           \en Parameters of object alignment.
               According to _hintUsageMode there are three working options:
               - without any hint (object alignment from their initial positions; most suitable for partially overlapping objects),
               - with automatic hint detection (for a moving object some initial transformation calculated by means of inertial properties is applied;
                 most suitable for objects identical or nearly identical in shape),
               - with a given hint array (having launched the alignment procedure for every given initial transformation, the best result is chosen). \~
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
class MATH_CLASS MbObjectAlignmentParams
{
private:
  MbeIcpHintUsageMode      _hintUsageMode;      ///< \ru Режим использования подсказки. \en Hint usage mode.
  MbeIcpPointRejectionMode _pointRejectionMode; ///< \ru Режим отбраковки точек. \en Point rejection mode.
  size_t                   _pointPairsMax;      ///< \ru Максимальное количество пар точек. \en Maximum count of point pairs.
  size_t                   _iterationMax;       ///< \ru Максимальное количество итераций. \en Maximum iteration count.
  std::vector<MbMatrix3D>  _hintArray;          ///< \ru Набор подсказок от пользователя. \en Hint array given by a user.

public:
  mutable IProgressIndicator * _progress;  ///< \ru Индикатор прогресса выполнения операции. \en A progress indicator of the operation.

public:
  /// \ru Конструктор. \en Constructor.
  MbObjectAlignmentParams( MbeIcpHintUsageMode hintUsageMode, IProgressIndicator * prog = nullptr )
    : _hintUsageMode     ( hintUsageMode                       )
    , _pointRejectionMode( MbeIcpPointRejectionMode::automatic )
    , _pointPairsMax     ( 10000                               )
    , _iterationMax      ( 10000                               )
    , _progress          ( prog                                )
  {}
  /// \ru Конструктор. \en Constructor.
  MbObjectAlignmentParams( const std::vector<MbMatrix3D> & hintArray, IProgressIndicator * prog = nullptr )
    : _hintUsageMode     ( MbeIcpHintUsageMode::givenHint      )
    , _pointRejectionMode( MbeIcpPointRejectionMode::automatic )
    , _pointPairsMax     ( 10000                               )
    , _iterationMax      ( 10000                               )
    , _hintArray         ( hintArray                           )
    , _progress          ( prog                                )
  {}
  /// \ru Деструктор. \en Destructor.
  ~MbObjectAlignmentParams() {}

public:
  /// \ru Задать режим отбраковки точек. \en Set the point rejection mode. \~
  void SetPointRejectionMode( MbeIcpPointRejectionMode pointRejectionMode ) { _pointRejectionMode = pointRejectionMode; }
  /// \ru Задать максимальное количество пар точек. \en Set the maximum count of point pairs. \~
  void SetPointPairsMax( size_t pointPairsMax ) { _pointPairsMax = pointPairsMax; }
  /// \ru Задать максимальное количество итераций. \en Set the maximum iteration count. \~
  void SetIterationMax( size_t iterationMax ) { _iterationMax = iterationMax; }
  /// \ru Задать набор подсказок. \en Set the hint array. \~
  void SetHintArray( const std::vector<MbMatrix3D> & hintArray ) { _hintArray = hintArray; }
  /// \ru Получить режим использования подсказки. \en Get the hint usage mode. \~
  MbeIcpHintUsageMode GetHintUsageMode() const { return _hintUsageMode; }
  /// \ru Получить режим отбраковки точек. \en Get the point rejection mode. \~
  MbeIcpPointRejectionMode GetPointRejectionMode() const { return _pointRejectionMode; }
  /// \ru Получить максимальное количество пар точек. \en Get the maximum count of point pairs. \~
  size_t GetPointPairsMax() const { return _pointPairsMax; }
  /// \ru Получить максимальное количество итераций. \en Get the maximum iteration count. \~
  size_t GetIterationMax() const { return _iterationMax; }
  /// \ru Получить набор заданных подсказок. \en Get the given hint array. \~
  const std::vector<MbMatrix3D> & GetHintArray() const { return _hintArray; }
}; // MbObjectAlignmentParams


//------------------------------------------------------------------------------
/** \brief \ru Результат совмещения объектов.
           \en Result of object alignment. \~
  \details \ru Результат совмещения объектов.
           \en Result of object alignment. \~
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
class MATH_CLASS MbObjectAlignmentResult
{
private:
  MbMatrix3D _matrix; ///< \ru Трансформация, необходимая для выравнивания. \en Transformation needed for alignment.
  double     _error;  ///< \ru Значение ошибки совмещения. \en Alignment error value.

public:
  /// \ru Конструктор. \en Constructor.
  explicit MbObjectAlignmentResult()
    : _matrix(              )
    , _error ( MB_MAXDOUBLE )
  {}
  /// \ru Деструктор. \en Destructor.
  ~MbObjectAlignmentResult() {}

public:
  /// \ru Получить результирующую трансформацию. \en Get the result transformation. \~
  const MbMatrix3D & GetMatrix() const { return _matrix; }
  /// \ru Получить результирующую трансформацию для изменения. \en Get the result transformation for changing. \~
  MbMatrix3D & SetMatrix() { return _matrix; }
  /// \ru Получить значение ошибки совмещения. \en Get alignment error value. \~
  double GetAlignmentErrorValue() const { return _error; }
  /// \ru Получить значение ошибки совмещения для изменения. \en Get alignment error value for changing. \~
  double & SetAlignmentErrorValue() { return _error; }
}; // MbObjectAlignmentResult


//------------------------------------------------------------------------------
/** \brief \ru Тип позиционного ограничения.
           \en Position constraint type. \~
  \details \ru Тип позиционного ограничения.
               Определяет вариант фиксации положения объекта в пространстве.
               Можно ограничить перемещения вдоль и повороты вокруг координатных осей некоторой локальной системы координат в разных комбинациях.
           \en Position constraint type.
               Defines the method of fixing the position of an object.
               There may be different combinations of restrictions to translation along or rotation about the axes of a local coordinate system. \~
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
enum class MbePositionConstraintType
{
  z,    ///< \ru Разрешен только сдвиг вдоль оси OZ плейсмента параметров. \en Translation along the OZ axis of a local coordinate system is only allowed.
  w,    ///< \ru Разрешен только поворот вокруг оси OZ плейсмента параметров. \en Rotation about the OZ axis of a local coordinate system is only allowed.
  zw,   ///< \ru Разрешен только сдвиг вдоль оси OZ и поворот вокруг оси OZ плейсмента параметров. \en Translation along the OZ axis and rotation about the OZ axis of a local coordinate system are only allowed.
  xyz,  ///< \ru Разрешен только параллельный перенос (запрещены повороты вокруг координатных осей плейсмента параметров). \en Translation is only allowed (any rotation is forbidden).
  xyw,  ///< \ru Разрешен только параллельный перенос вдоль осей OX и OY и поворот вокруг оси OZ плейсмента параметров. \en Translation along the OX и OY axes and rotation about the OZ axis of a local coordinate system are only allowed.
  xyzw, ///< \ru Разрешен параллельный перенос вдоль координатных осей и поворот вокруг оси OZ плейсмента параметров. \en Translation along the coordinate axes and rotation about the OZ axis of a local coordinate system are only allowed.
  uvw,  ///< \ru Разрешен только поворот вокруг координатных осей плейсмента параметров (запрещен любой параллельный перенос). \en Rotation is only allowed (any translation is forbidden).
  free, ///< \ru Без ограничений. \en Without constraints.
};


//------------------------------------------------------------------------------
/** \brief \ru Тип размерного ограничения.
           \en Dimensional constraint type. \~
  \details \ru Тип размерного ограничения.
               Определяет вариант фиксации размеров объекта. 
               Можно зафиксировать радиус сферы, радиус цилиндра, уклон конуса, большой и малый радиусы тора (только один их них или сразу оба).
           \en Dimensional constraint type.
               Defines the method of fixing dimensional parameters of an object.
               There may be fixed the radius of a sphere, the radius of a cylinder, the conicity angle of a cone, the major radius and the minor radius of a torus (one or both of them). \~
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
enum class MbeDimensionalConstraintType
{
  none,   ///< \ru Без ограничений. \en Without constraints.
  first,  ///< \ru Зафиксирован только первый размерный параметр объекта. \en The first dimensional parameter of an object is only fixed.
  second, ///< \ru Зафиксирован только второй размерный параметр объекта (корректно только для тора). \en The second dimensional parameter of an object is only fixed (only valid for a torus).
  both,   ///< \ru Зафиксированы оба размерных параметра объекта (корректно только для тора). \en Both dimensional parameters of an object are fixed (only valid for a torus).
};


//------------------------------------------------------------------------------
/** \brief \ru Фиксированные значения параметров поверхности.
           \en Fixed values of surface parameters. \~
  \details \ru Фиксированные значения параметров поверхности:
               - координаты x и y некоторой точки на оси объекта (невалидно для сферы и плоскости),
               - координаты центра сферы,
               - азимутальный угол (_phi): угол между проекцией оси объекта на плоскость OXY 
               и осью OX некоторой системы координат, принадлежит промежутку [0; 2П) (невалидно для сферы),
               - зенитный угол (_theta): угол между осью объекта и осью OZ некоторой 
               системы координат, принадлежит отрезку [0; П/2] (невалидно для сферы),
               - расстояние (_dist) от оси объекта до оси OZ некоторой системы координат (невалидно для сферы),
               - расстояние (_dist) от центра сферы до начала координат некоторой системы,
               - радиус сферы (_size1),
               - радиус цилиндра (_size1),
               - уклон конуса (_size1),
               - большой (_size1) или малый (_size2) радиусы тора (только один их них или сразу оба).
           \en Fixed values of surface parameters:
               - the X and Y coordinates of a point of an object's axis (invalid for a sphere and a plane),
               - coordinates of a sphere's center,
               - azimuthal angle (_phi): the angle between the projection of an object's axis onto the OXY plane
               and the OX axis of a coordinate system, belongs to [0; 2П) (invalid for a sphere),
               - zenith angle (_theta): the angle between an object's axis and the OZ axis 
               of a coordinate system, belongs to [0; П/2] (invalid for a sphere),
               - the distance (_dist) between an object's axis and the OZ axis of a coordinate system (invalid for a sphere),
               - the distance (_dist) between a sphere's center and the origin of a coordinate system,
               - the radius of a sphere (_size1),
               - the radius of a cylinder (_size1),
               - the conicity angle of a cone (_size1),
               - the major radius (_size1) or the minor radius (_size2) of a torus (one or both of them). \~
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
struct MATH_CLASS MbConstraintFixedValues
{
public:
  /** \ru \name Позиционные параметры.
      \en \name Position parameters.
      \{ */
  double _x;     ///< \ru Координата x точки на оси (для сферы - ее центра). \en The X coordinate of an axis point (for a sphere - of its center).
  double _y;     ///< \ru Координата y точки на оси (для сферы - ее центра). \en The Y coordinate of an axis point (for a sphere - of its center).
  double _z;     ///< \ru Координата z точки на оси (для сферы - ее центра). \en The Z coordinate of an axis point (for a sphere - of its center).
  double _phi;   ///< \ru Азимутальный угол (невалидно для сферы). \en Azimuthal angle (invalid for a sphere).
  double _theta; ///< \ru Зенитный угол (невалидно для сферы). \en Zenith angle (invalid for a sphere).
  double _dist;  ///< \ru Расстояние от оси объекта до оси OZ некоторой СК (для сферы - от ее центра до начала СК). \en The distance between an object's axis and the OZ axis in a CS (for a sphere - between its center and the origin of a CS).
  /** \} */
  /** \ru \name Размерные параметры.
    \en \name Dimensional parameters.
    \{ */
  double _size1; ///< \ru Первый размерный параметр объекта (невалидно для плоскости). \en The first dimensional parameter of an object (invalid for a plane).
  double _size2; ///< \ru Второй размерный параметр объекта (валидно только для тора). \en The second dimensional parameter of an object (only valid for a torus).
  /** \} */

public:
  /// \ru Конструктор по умолчанию. \en Default constructor. \~
  MbConstraintFixedValues()
    : _x    ( UNDEFINED_DBL )
    , _y    ( UNDEFINED_DBL )
    , _z    ( UNDEFINED_DBL )
    , _phi  ( UNDEFINED_DBL )
    , _theta( UNDEFINED_DBL )
    , _dist ( UNDEFINED_DBL )
    , _size1( UNDEFINED_DBL )
    , _size2( UNDEFINED_DBL )
  {}
};


//------------------------------------------------------------------------------
/** \brief \ru Ограничение на вписывание примитива.
           \en Constraint for primitive fitting. \~
  \details \ru Ограничение на вписывание примитива.
               Позиционное ограничение действует в заданной локальной системе координат.
               Фиксированные значения для позиционных и размерных параметров заданы структурой.
           \en Constraint for primitive fitting.
               Position constraint operates in a given local coordinate system.
               Fixed values for position and dimensional parameters are given by a structure. \~
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
class MATH_CLASS MbSurfaceFitConstraint
{
private:
  MbePositionConstraintType    _typePos;     ///< \ru Тип позиционного ограничения. \en Position constraint type.
  MbeDimensionalConstraintType _typeDim;     ///< \ru Тип размерного ограничения. \en Dimensional constraint type.
  MbConstraintFixedValues      _fixedValues; ///< \ru Фиксированные значения параметров поверхности. \en Fixed values of surface parameters.
  MbPlacement3D                _place;       ///< \ru Локальная система координат. \en Local coordinate system.

public:
  /// \ru Конструктор по умолчанию. \en Default constructor. \~
  MbSurfaceFitConstraint();

public:
  /** \ru \name Позиционные ограничения.
      \en \name Position constraints.
      \{ */
  /** \brief \ru Ограничить ось примитива.
             \en Add an axis constraint. \~
    \details \ru Ограничить ось цилиндра, конуса или тора, а также нормаль плоскости.
                 По умолчанию ось примитива или нормаль плоскости будет коллинеарна направлению direction.
                 Дополнительно можно задать желаемый угол angle между осью примитива или нормалью плоскости и заданным направлением из промежутка [0; П/2].
                 Для построения плоскости с нормалью, которая перпендикулярна заданному направлению, необходимо задать угол П/2.
                 Функция сбрасывает выставленные ранее ограничения на ось.
             \en Add an axis constraint to a cylinder, cone, torus or plane. 
                 The axis of a primitive or plane normal will be collinear to a given direction by default.
                 Besides there may be given a desired angle between the axis of a primitive or plane normal and a given direction from [0; П/2].
                 Fitting a plane with a normal, which is perpendicular to a given direction, implies an angle П/2.
                 The function resets all previous axis constraints. \~
    \param[in] direction - \ru Эталонное направление.
                           \en Reference direction. \~
    \param[in] angle     - \ru Угол между осью примитива и заданным направлением.
                           \en Angle between the axis of a primitive and a given direction. \~
    \return \ru Возвращает true, если задан корректный угол, и false - иначе.
            \en Returns true, if an angle is correct, or false otherwise. \~
    \ingroup Polygonal_Objects
  */
  bool AddAxisConstraint( const MbVector3D & direction, double angle = 0. );
  /** \brief \ru Зафиксировать ось примитива.
             \en Add a coaxial constraint. \~
    \details \ru Зафиксировать ось цилиндра, конуса, тора или сферы.
                 Вписывается примитив с заданной осью.
                 Если вписывается сфера, ее центр лежит на заданной оси.
                 Функция сбрасывает выставленные ранее ограничения на ось.
             \en Add a coaxial constraint to a cylinder, cone, torus or sphere.
                 A primitive will be fit with a given axis.
                 If a sphere is fit, its center lies on a given axis.
                 The function resets all previous axis constraints. \~
    \param[in] axis - \ru Ось.
                      \en Axis. \~
    \ingroup Polygonal_Objects
  */
  void AddCoaxialConstraint( const MbAxis3D & axis );
  /** \} */
  /** \ru \name Размерные ограничения.
      \en \name Dimensional constraints.
    \{ */
  /** \brief \ru Зафиксировать радиус цилиндра.
             \en Fix cylinder radius. \~
    \details \ru Зафиксировать радиус цилиндра.
                 Радиус должен быть положительным.
             \en Fix cylinder radius.
                 The value has to be positive. \~
    \param[in] radius - \ru Радиус цилиндра.
                        \en Cylinder radius. \~
    \return \ru Возвращает true, если задан корректный радиус, и false - иначе.
            \en Returns true, if a radius is correct, or false otherwise. \~
    \ingroup Polygonal_Objects
  */
  bool AddCylinderRadiusConstraint( double radius );
  /** \brief \ru Зафиксировать радиус сферы.
             \en Fix sphere radius. \~
    \details \ru Зафиксировать радиус сферы.
                 Радиус должен быть положительным.
             \en Fix sphere radius.
                 The value has to be positive. \~
    \param[in] radius - \ru Радиус сферы.
                        \en Sphere radius. \~
    \return \ru Возвращает true, если задан корректный радиус, и false - иначе.
            \en Returns true, if a radius is correct, or false otherwise. \~
    \ingroup Polygonal_Objects
  */
  bool AddSphereRadiusConstraint( double radius );
  /** \brief \ru Зафиксировать угол конуса.
             \en Fix cone angle. \~
    \details \ru Зафиксировать угол конуса.
                 Угол должен быть из промежутка (0; П/2).
             \en Fix cone angle.
                 The angle has to be from (0; П/2). \~
    \param[in] angle - \ru Угол конуса.
                       \en Cone angle. \~
    \return \ru Возвращает true, если задан корректный угол, и false - иначе.
            \en Returns true, if an angle is correct, or false otherwise. \~
    \ingroup Polygonal_Objects
  */
  bool AddConeAngleConstraint( double angle );
  /** \brief \ru Зафиксировать радиусы тора.
             \en Fix torus radii. \~
    \details \ru Зафиксировать радиусы тора.
                 Можно зафиксировать большой и малый радиусы тора (только один их них или сразу оба).
                 Малый радиус должен быть положительным (=0 - значение не зафиксировано).
                 Большой радиус может быть отрицательным для торов типа "лимон" (=0 - значение не зафиксировано).
                 Большой радиус (центров) должен быть больше малого радиуса (трубки), взятого с обратным знаком: majorRadius>–minorRadius.
             \en Fix torus radii.
                 There may be fixed the major radius (_size1) or the minor radius (_size2) of a torus (one or both of them).
                 The minor radius has to be positive (=0 - value is not fixed).
                 The major radius may be negative if a torus is of "lemon" type (=0 - value is not fixed).
                 The major radius (of centers) has to be greater than the opposite of the minor radius (of a tube): majorRadius>–minorRadius. \~
    \param[in] majorRadius - \ru Большой радиус тора.
                             \en Major torus radius. \~
    \param[in] minorRadius - \ru Малый радиус тора.
                             \en Minor torus radius. \~
    \return \ru Возвращает true, если заданы корректные радиусы, и false - иначе.
            \en Returns true, if radii are correct, or false otherwise. \~
    \ingroup Polygonal_Objects
  */
  bool AddTorusRadiiConstraint( double majorRadius, double minorRadius );
  /** \} */

  /** \brief \ru Инициализировать ограничение типа XYW (при условии валидности).
             \en Initialize the XYW-constraint (in case of validity). \~
    \details \ru Инициализировать ограничение типа XYW.
                 Ограничение типа XYW допускает только параллельный перенос вдоль осей OX и OY и поворот вокруг оси OZ заданной системы координат. 
                 Для плоскости, цилиндра, конуса или тора должен быть зафиксирован зенитный угол theta:
                 угол между осью объекта и осью OZ заданной СК, должен принадлежать отрезку [0; П/2].
                 Для сферы должна быть зафиксирована координата z ее центра в заданной СК.
                 Для тора должна быть зафиксирована координата z центра его направляющей окружности в заданной СК.
                 При необходимости можно зафиксировать размерные параметры объекта.
                 Функция сбрасывает выставленные ранее ограничения (например, добавленные с помощью методов "Add*Constraint").
             \en Initialize the XYW-constraint. 
                 Translation along the OX и OY axes and rotation about the OZ axis of a local coordinate system are only allowed.
                 For a plane, a cylinder, a cone or a torus there has to be fixed the zenith angle theta:
                 the angle between an object's axis and the OZ axis of a given CS, has to belong to [0; П/2].
                 For a sphere there has to be fixed the Z coordinate of its center.
                 For a torus there has to be fixed the Z coordinate of its directrix circle.
                 There may be fixed some dimensional parameters of an object if necessary.
                 The function resets all previous constraints (for example, having been set by "Add*Constraint"). \~
    \param[in] typeSurface - \ru Тип поверхности.
                             \en Surface type. \~
    \param[in] typeDim     - \ru Тип размерного ограничения.
                             \en Dimensional constraint type. \~
    \param[in] fixedValues - \ru Фиксированные значения параметров поверхности.
                             \en Structure with fixed values of surface parameters. \~
    \param[in] place       - \ru Локальная система координат.
                             \en Local coordinate system. \~
    \return \ru Возвращает код результата операции.
            \en Returns operation result code. \~
    \ingroup Polygonal_Objects
  */
  MbResultType InitializeXYW(       MbeSpaceType                 typeSurface,
                                    MbeDimensionalConstraintType typeDim,
                              const MbConstraintFixedValues &    fixedValues,
                              const MbPlacement3D &              place );
  /** \brief \ru Инициализировать ограничение типа Z (при условии валидности).
             \en Initialize the Z-constraint (in case of validity). \~
    \details \ru Инициализировать ограничение типа Z.
                 Ограничение типа Z допускает только сдвиг вдоль оси OZ заданной системы координат.
                 Для плоскости, цилиндра, конуса или тора должны быть зафиксированы:
                 - зенитный угол theta: угол между осью объекта и осью OZ заданной СК, должен принадлежать отрезку [0; П/2],
                 - азимутальный угол phi: угол между проекцией оси объекта на плоскость OXY и осью OX заданной СК, 
                 должен принадлежать промежутку [0; 2П).
                 Для цилиндра, конуса или тора должны быть зафиксированы координаты x и y некоторой точки на оси объекта.
                 Для сферы должны быть зафиксированы координаты x и y ее центра в заданной СК.
                 При необходимости можно зафиксировать размерные параметры объекта.
                 Функция сбрасывает выставленные ранее ограничения (например, добавленные с помощью методов "Add*Constraint").
             \en Initialize the Z-constraint.
                 Translation along the OZ axis of a local coordinate system is only allowed.
                 For a plane, a cylinder, a cone or a torus there have to be fixed:
                 - the zenith angle theta: the angle between an object's axis and the OZ axis of a given CS, has to belong to [0; П/2],
                 - the azimuthal angle phi: the angle between the projection of an object's axis onto the OXY plane 
                 and the OX axis of a given CS, has to belong to [0; 2П).
                 For a cylinder, a cone or a torus there have to be fixed the X and Y coordinates of a point of an object's axis.
                 For a sphere there have to be fixed the X and Y coordinates of its center.
                 There may be fixed some dimensional parameters of an object if necessary.
                 The function resets all previous constraints (for example, having been set by "Add*Constraint"). \~
    \param[in] typeSurface - \ru Тип поверхности.
                             \en Surface type. \~
    \param[in] typeDim     - \ru Тип размерного ограничения.
                             \en Dimensional constraint type. \~
    \param[in] fixedValues - \ru Фиксированные значения параметров поверхности.
                             \en Structure with fixed values of surface parameters. \~
    \param[in] place       - \ru Локальная система координат.
                             \en Local coordinate system. \~
    \return \ru Возвращает код результата операции.
            \en Returns operation result code. \~
    \ingroup Polygonal_Objects
  */
  MbResultType InitializeZ(       MbeSpaceType                 typeSurface,
                                  MbeDimensionalConstraintType typeDim,
                            const MbConstraintFixedValues &    fixedValues,
                            const MbPlacement3D &              place );
  /** \brief \ru Инициализировать ограничение типа XYZ (при условии валидности).
             \en Initialize the XYZ-constraint (in case of validity). \~
    \details \ru Инициализировать ограничение типа XYZ.
                 Ограничение типа XYZ допускает только параллельный перенос (запрещены повороты вокруг координатных осей заданной системы координат).
                 Для плоскости, цилиндра, конуса или тора должны быть зафиксированы:
                 - зенитный угол theta: угол между осью объекта и осью OZ заданной СК, должен принадлежать отрезку [0; П/2],
                 - азимутальный угол phi: угол между проекцией оси объекта на плоскость OXY и осью OX заданной СК, 
                 должен принадлежать промежутку [0; 2П).
                 При необходимости можно зафиксировать размерные параметры объекта.
                 Функция сбрасывает выставленные ранее ограничения (например, добавленные с помощью методов "Add*Constraint").
             \en Initialize the XYZ-constraint.
                 Translation is only allowed (any rotation is forbidden).
                 For a plane, a cylinder, a cone or a torus there have to be fixed:
                 - the zenith angle theta: the angle between an object's axis and the OZ axis of a given CS, has to belong to [0; П/2],
                 - the azimuthal angle phi: the angle between the projection of an object's axis onto the OXY plane 
                 and the OX axis of a given CS, has to belong to [0; 2П).
                 There may be fixed some dimensional parameters of an object if necessary.
                 The function resets all previous constraints (for example, having been set by "Add*Constraint"). \~
    \param[in] typeSurface - \ru Тип поверхности.
                             \en Surface type. \~
    \param[in] typeDim     - \ru Тип размерного ограничения.
                             \en Dimensional constraint type. \~
    \param[in] fixedValues - \ru Фиксированные значения параметров поверхности.
                             \en Structure with fixed values of surface parameters. \~
    \param[in] place       - \ru Локальная система координат.
                             \en Local coordinate system. \~
    \return \ru Возвращает код результата операции.
            \en Returns operation result code. \~
    \ingroup Polygonal_Objects
  */
  MbResultType InitializeXYZ(       MbeSpaceType                 typeSurface,
                                    MbeDimensionalConstraintType typeDim,
                              const MbConstraintFixedValues &    fixedValues,
                              const MbPlacement3D &              place );
  /** \brief \ru Инициализировать ограничение типа ZW (при условии валидности).
             \en Initialize the ZW-constraint (in case of validity). \~
    \details \ru Инициализировать ограничение типа ZW.
                 Ограничение типа ZW допускает только сдвиг вдоль оси OZ и поворот вокруг оси OZ заданной системы координат.
                 Для плоскости, цилиндра, конуса или тора должны быть зафиксированы:
                 - зенитный угол theta: угол между осью объекта и осью OZ заданной СК, должен принадлежать отрезку [0; П/2],
                 - начальный азимутальный угол phi: угол между проекцией оси объекта в начальной позиции на плоскость OXY и осью OX заданной СК,
                 должен принадлежать промежутку [0; 2П),
                 - расстояние dist от оси OZ заданной СК до оси объекта.
                 Для сферы должно быть зафиксировано расстояние dist от ее центра до оси OZ заданной СК.
                 При необходимости можно зафиксировать размерные параметры объекта.
                 Функция сбрасывает выставленные ранее ограничения (например, добавленные с помощью методов "Add*Constraint").
             \en Initialize the ZW-constraint.
                 Translation along the OZ axis and rotation about the OZ axis of a local coordinate system are only allowed.
                 For a plane, a cylinder, a cone or a torus there have to be fixed:
                 - the zenith angle theta: the angle between an object's axis and the OZ axis of a given CS, has to belong to [0; П/2],
                 - the initial azimuthal angle phi: the angle between the projection of an object's axis in the initial position onto the OXY plane
                 and the OX axis of a given CS, has to belong to [0; 2П),
                 - the distance between an object's axis the OZ axis of a given CS.
                 For a sphere there has to be fixed the distance between its center and the OZ axis of a given CS.
                 There may be fixed some dimensional parameters of an object if necessary.
                 The function resets all previous constraints (for example, having been set by "Add*Constraint"). \~
    \param[in] typeSurface - \ru Тип поверхности.
                             \en Surface type. \~
    \param[in] typeDim     - \ru Тип размерного ограничения.
                             \en Dimensional constraint type. \~
    \param[in] fixedValues - \ru Фиксированные значения параметров поверхности.
                             \en Structure with fixed values of surface parameters. \~
    \param[in] place       - \ru Локальная система координат.
                             \en Local coordinate system. \~
    \return \ru Возвращает код результата операции.
            \en Returns operation result code. \~
    \ingroup Polygonal_Objects
  */
  MbResultType InitializeZW(       MbeSpaceType                 typeSurface,
                                   MbeDimensionalConstraintType typeDim,
                             const MbConstraintFixedValues &    fixedValues,
                             const MbPlacement3D &              place );
  /// \ru Получить тип позиционного ограничения. \en Get the position constraint type. \~
  MbePositionConstraintType GetPosType() const { return _typePos; }
  /// \ru Получить тип размерного ограничения. \en Get the dimensional constraint type. \~
  MbeDimensionalConstraintType GetDimType() const { return _typeDim; }
  /// \ru Получить структуру фиксированных значений параметров поверхности. \en Get the structure with fixed values of surface parameters. \~
  const MbConstraintFixedValues & GetFixedValues() const { return _fixedValues; }
  /// \ru Получить локальную систему координат. \en Get the local coordinate system. \~
  const MbPlacement3D & GetPlacement() const { return _place; }
  /// \ru Установить тип позиционного ограничения. \en Set the position constraint type. \~
  void SetPosType( MbePositionConstraintType typePos ) { _typePos = typePos; }
  /// \ru Установить тип размерного ограничения. \en Set the dimensional constraint type. \~
  void SetDimType( MbeDimensionalConstraintType typeDim ) { _typeDim = typeDim; }
  /// \ru Установить структуру фиксированных значений параметров поверхности. \en Set the structure with fixed values of surface parameters. \~
  void SetFixedValues( const MbConstraintFixedValues & fixedValues ) { _fixedValues = fixedValues; }
  /// \ru Установить локальную систему координат. \en Set the local coordinate system. \~
  void SetPlacement( const MbPlacement3D & place ) { _place.Init( place ); }
  /// \ru Проверить ограничение на пустоту. \en Check whether the constraint is empty. \~
  bool IsEmpty() const;
  /// \ru Установить пустое ограничение. \en Set empty constraint. \~
  void SetEmpty();

  OBVIOUS_PRIVATE_COPY( MbSurfaceFitConstraint );
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры вписывания поверхности.
           \en Parameters of surface fitting. \~
  \ingroup Polygonal_Objects
*/
// ---
class MATH_CLASS MbSurfaceFitToGridParameters
{
private:
  MbeSpaceType           _surfaceType;      ///< \ru Тип поверхности. \en A surface type.
  double                 _tolerance;        ///< \ru Точность распознавания. \en A fitting tolerance.
  c3d::IndicesVector     _indicies;         ///< \ru Индексы полигонов сетки. \en Indicies of polygons.
  size_t                 _iRegion;          ///< \ru Регион сегментации. \en Segmentation region.
  MbeRejectOutliersType  _typeReject;       ///< \ru Способ отбраковки выбросов. \en Outliers rejection mode.
  double                 _valueReject;      ///< \ru Пороговое значения для отбраковки выбросов. \en Outliers rejection mode treshold value.
  MbSurfaceFitConstraint _fitConstraint;    ///< \ru Ограничение. \en Constraint.
  size_t                 _pointsFittingMax; ///< \ru Максимальное количество точек для вписывания. \en Maximum count of fitting points.
  bool                   _withRefinement;   ///< \ru Уточнять ли поверхность по всем точкам. \en Refine the surface by all the points or not.
  bool                   _closed;           ///< \ru Вписать замкнутую поверхность. \en Fit a closed surface.

  ///< \ru Предельные значения параметров аналитических поверхностей. \en Thresholds for analytical surfaces parameters.
  double _angleConeMin;           ///< \ru Минимально возможный половинный угол конуса (градусы). \en Mininmum allowed cone half-angle ( degrees ).
  double _angleConeMax;           ///< \ru Максимально возможный половинный угол конуса (градусы). \en Maximum allowed cone half-angle( degrees ).
  double _radiusAnalyticShapeMax; ///< \ru Максимально возможный радиальный размер аналитических поверхностей. \en Maximum allowed analytical shapes radial size.

  ///< \ru Параметры для вписывания нурбс поверхности. \en NURBs surface fitting parameters.
  size_t _order;      ///< \ru Порядок нурбс поверхности. \en NURBs surface order.
  size_t _countCpMax; ///< \ru Максимально разрешенное количество контрольных точек. \en Maximum allowed control points count.
  double _smoothCoef; ///< \ru Коэффициент сглаживания [1e-9 - 1e-3]. \en Smoothing coefficient [1e-9 - 1e-3].
  double _angleDelta; ///< \ru Отклонение угла поворота направления изолиний (в градусах). \en Rotation delta for isolines direction (in degrees).

private:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  MbSurfaceFitToGridParameters()
    : _surfaceType           ( st_Undefined                )
    , _tolerance             ( c3d::DELTA_MIN              )
    , _indicies              (                             )
    , _iRegion               ( SYS_MAX_T                   )
    , _typeReject            ( rot_NoReject                )
    , _valueReject           ( 0.                          )
    , _pointsFittingMax      ( 10000                       )
    , _withRefinement        ( false                       )
    , _closed                ( false                       )
    , _angleConeMin          ( 1.                          )
    , _angleConeMax          ( 89.                         )
    , _radiusAnalyticShapeMax( 1500.                       )
    , _order                 ( c3d::NURBS_DEGREE           )
    , _countCpMax            ( c3d::NURBS_POINTS_MAX_COUNT )
    , _smoothCoef            ( METRIC_ACCURACY             )
    , _angleDelta            ( 0.                          )
  {}

public:
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  explicit MbSurfaceFitToGridParameters(       MbeSpaceType         surfaceType,
                                               double               tolerance,
                                         const c3d::IndicesVector & indicies )
    : _surfaceType           ( surfaceType                 )
    , _tolerance             ( tolerance                   )
    , _indicies              ( indicies                    )
    , _iRegion               ( SYS_MAX_T                   )
    , _typeReject            ( rot_NoReject                )
    , _valueReject           ( 0.                          )
    , _pointsFittingMax      ( 10000                       )
    , _withRefinement        ( false                       )
    , _closed                ( false                       )
    , _angleConeMin          ( 1.                          )
    , _angleConeMax          ( 89.                         )
    , _radiusAnalyticShapeMax( 1500.                       )
    , _order                 ( c3d::NURBS_DEGREE           )
    , _countCpMax            ( c3d::NURBS_POINTS_MAX_COUNT )
    , _smoothCoef            ( METRIC_ACCURACY             )
    , _angleDelta            ( 0.                          )
  {}
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  explicit MbSurfaceFitToGridParameters(       MbeSpaceType          surfaceType,
                                               double                tolerance,
                                         const c3d::IndicesVector &  indicies,
                                               MbeRejectOutliersType rejectType,
                                               double                rejectValue )
    : _surfaceType           ( surfaceType                 )
    , _tolerance             ( tolerance                   )
    , _indicies              ( indicies                    )
    , _iRegion               ( SYS_MAX_T                   )
    , _typeReject            ( rejectType                  )
    , _valueReject           ( rejectValue                 )
    , _pointsFittingMax      ( 10000                       )
    , _withRefinement        ( false                       )
    , _closed                ( false                       )
    , _angleConeMin          ( 1.                          )
    , _angleConeMax          ( 89.                         )
    , _radiusAnalyticShapeMax( 1500.                       )
    , _order                 ( c3d::NURBS_DEGREE           )
    , _countCpMax            ( c3d::NURBS_POINTS_MAX_COUNT )
    , _smoothCoef            ( METRIC_ACCURACY             )
    , _angleDelta            ( 0.                          )
  {}
  /// \ru Конструктор по параметрам для вписывания аналитических поверхностей. \en Constructor by parameters for analytic surfaces fitting.
  explicit MbSurfaceFitToGridParameters(       MbeSpaceType          surfaceType,
                                               double                tolerance,
                                         const c3d::IndicesVector &  indicies,
                                               MbeRejectOutliersType rejectType,
                                               double                rejectValue,
                                               double                angleConeMin,
                                               double                angleConeMax,
                                               double                radiusAnalyticShapeMax )
    : _surfaceType           ( surfaceType                 )
    , _tolerance             ( tolerance                   )
    , _indicies              ( indicies                    )
    , _iRegion               ( SYS_MAX_T                   )
    , _typeReject            ( rejectType                  )
    , _valueReject           ( rejectValue                 )
    , _pointsFittingMax      ( 10000                       )
    , _withRefinement        ( false                       )
    , _closed                ( false                       )
    , _angleConeMin          ( angleConeMin                )
    , _angleConeMax          ( angleConeMax                )
    , _radiusAnalyticShapeMax( radiusAnalyticShapeMax      )
    , _order                 ( c3d::NURBS_DEGREE           )
    , _countCpMax            ( c3d::NURBS_POINTS_MAX_COUNT )
    , _smoothCoef            ( METRIC_ACCURACY             )
    , _angleDelta            ( 0.                          )
  {}
  /// \ru Конструктор по параметрам для вписывания нурбс поверхности. \en Constructor by parameters for NURBs surface fitting.
  explicit MbSurfaceFitToGridParameters(       MbeSpaceType         surfaceType,
                                               double               tolerance,
                                         const c3d::IndicesVector & indicies,
                                               size_t               order,
                                               size_t               countCpMax,
                                               double               smoothCoef )
    : _surfaceType           ( surfaceType  )
    , _tolerance             ( tolerance    )
    , _indicies              ( indicies     )
    , _iRegion               ( SYS_MAX_T    )
    , _typeReject            ( rot_NoReject )
    , _valueReject           ( 0.           )
    , _pointsFittingMax      ( 10000        )
    , _withRefinement        ( false        )
    , _closed                ( false        )
    , _angleConeMin          ( 1.           )
    , _angleConeMax          ( 89.          )
    , _radiusAnalyticShapeMax( 1500.        )
    , _order                 ( order        )
    , _countCpMax            ( countCpMax   )
    , _smoothCoef            ( smoothCoef   )
    , _angleDelta            ( 0.           )
  {}

  /// \ru Выдать тип поверхности. \en Get surface type.
  MbeSpaceType GetSurfaceType() const { return _surfaceType; }
  /// \ru Выдать точность распознавания. \en Get fitting tolerance.
  double GetTolerance() const { return _tolerance; }
  /// \ru Выдать индексы полигонов. \en Get indicies of polygons.
  const c3d::IndicesVector & GetIndicies() const { return _indicies; }
  /// \ru Выдать индекс региона сегментации. \en Get the index of a segmentation region.
  size_t GetRegionIndex() const { return _iRegion; }
  /// \ru Задать индекс региона сегментации. \en Set the index of a segmentation region.
  void SetRegionIndex( size_t iRegion ) { _iRegion = iRegion; }
  /// \ru Выдать метод отбраковки выбросов. \en Get outliers rejection mode.
  MbeRejectOutliersType GetOutliersRejectionMode() const { return _typeReject; }
  /// \ru Выдать пороговое значения для отбраковки выбросов. \en Get outliers rejection mode treshold value. 
  double GetOutliersRejectionValue() const { return _valueReject; }
  /// \ru Выдать порядок нурбс поверхности. \en Get NURBs order. 
  size_t GetNurbsOrder() const { return _order; }
  /// \ru Выдать максимально разрешенное количество контрольных точек. \en Get maximum allowed control points count.
  size_t GetCountCpMax() const { return _countCpMax; }
  /// \ru Выдать коэффициент сглаживания. \en Get smoothing coefficient.
  double GetSmoothCoef() const { return _smoothCoef; }
  /// \ru Выдать отклонение угла поворота направления изолиний. \en Get the rotation delta for isolines direction.
  double GetAngleDelta() const { return _angleDelta; }
  /// \ru Выдать минимально возможный половинный угол конуса. \en Get mininmum allowed cone half-angle.
  double GetAngleConeMin() const { return _angleConeMin; }
  /// \ru Выдать максимально возможный половинный угол конуса. \en Get maximum allowed cone half-angle.
  double GetAngleConeMax() const { return _angleConeMax; }
  /// \ru Выдать максимально возможный радиальный размер аналитических поверхностей. \en Get maximum allowed analytical shapes radial size.
  double GetRadiusAnalyticShapeMax() const { return _radiusAnalyticShapeMax; }
  /// \ru Выдать ограничение. \en Get the constraint.
  const MbSurfaceFitConstraint & GetFitConstraint() const { return _fitConstraint; }
  /// \ru Выдать ограничение для редактирования. \en Get the constraint for editing.
  MbSurfaceFitConstraint & GetFitConstraint() { return _fitConstraint; }
  /// \ru Выдать максимальное количество точек для вписывания. \en Get the maximum count of fitting points.
  size_t GetPointsFittingMax() const { return _pointsFittingMax; }
  /// \ru Выдать флаг уточнения поверхности по всем точкам. \en Get the flag of surface refinement by all the points.
  bool GetWithRefinement() const { return _withRefinement; }
  /// \ru Выдать флаг замкнутости вписываемой поверхности. \en Get the flag of surface closedness.
  bool GetClosed() const { return _closed; }
  /// \ru Установить предельные значения параметров аналитических поверхностей. \en Set thresholds for analytical surfaces parameters.
  void SetAnalyticBounds( double angleConeMin, double angleConeMax, double radiusAnalyticShapeMax )
  {
    _angleConeMin = angleConeMin;
    _angleConeMax = angleConeMax;
    _radiusAnalyticShapeMax = radiusAnalyticShapeMax;
  }
  /// \ru Установить параметры для вписывания нурбс поверхности. \en Set NURBs surface fitting parameters.
  void SetNurbsFitParams( size_t order, size_t countCpMax, double smoothCoef )
  {
    _order = order;
    _countCpMax = countCpMax;
    _smoothCoef = smoothCoef;
  }
  /// \ru Установить отклонение угла поворота направления изолиний (в градусах) для вписывания нурбс поверхности. \en Set the rotation delta for isolines direction (in degrees) of a NURBS surface.
  void SetNurbsAngleDelta( double angleDelta )
  {
    _angleDelta = angleDelta;
  }
  /** \brief \ru Ограничить ось примитива.
             \en Add an axis constraint. \~
    \details \ru Ограничить ось цилиндра, конуса или тора, а также нормаль плоскости.
                 По умолчанию ось примитива или нормаль плоскости будет коллинеарна направлению direction.
                 Дополнительно можно задать желаемый угол angle между осью примитива или нормалью плоскости и заданным направлением из промежутка [0; П/2].
                 Для построения плоскости с нормалью, которая перпендикулярна заданному направлению, необходимо задать угол П/2.
                 Функция сбрасывает выставленные ранее ограничения на ось.
             \en Add an axis constraint to a cylinder, cone, torus or plane.
                 The axis of a primitive or plane normal will be collinear to a given direction by default.
                 Besides there may be given a desired angle between the axis of a primitive or plane normal and a given direction from [0; П/2].
                 Fitting a plane with a normal, which is perpendicular to a given direction, implies an angle П/2.
                 The function resets all previous axis constraints. \~
    \param[in] direction - \ru Эталонное направление.
                           \en Reference direction. \~
    \param[in] angle     - \ru Угол между осью примитива и заданным направлением.
                           \en Angle between the axis of a primitive and a given direction. \~
    \return \ru Возвращает true, если задан корректный угол, и false - иначе.
            \en Returns true, if an angle is correct, or false otherwise. \~
    \ingroup Polygonal_Objects
  */
  bool AddAxisConstraint( const MbVector3D & direction, double angle = 0. )
  {
    return _fitConstraint.AddAxisConstraint( direction, angle );
  }
  /** \brief \ru Зафиксировать ось примитива.
             \en Add a coaxial constraint. \~
    \details \ru Зафиксировать ось цилиндра, конуса, тора или сферы.
                 Вписывается примитив с заданной осью.
                 Если вписывается сфера, ее центр лежит на заданной оси.
                 Функция сбрасывает выставленные ранее ограничения на ось.
             \en Add a coaxial constraint to a cylinder, cone, torus or sphere.
                 A primitive will be fit with a given axis.
                 If a sphere is fit, its center lies on a given axis.
                 The function resets all previous axis constraints. \~
    \param[in] axis - \ru Ось.
                      \en Axis. \~
    \ingroup Polygonal_Objects
  */
  void AddCoaxialConstraint( const MbAxis3D & axis )
  {
    return _fitConstraint.AddCoaxialConstraint( axis );
  }
  /** \brief \ru Зафиксировать радиус цилиндра.
             \en Fix cylinder radius. \~
    \details \ru Зафиксировать радиус цилиндра.
                 Радиус должен быть положительным.
             \en Fix cylinder radius.
                 The value has to be positive. \~
    \param[in] radius - \ru Радиус цилиндра.
                        \en Cylinder radius. \~
    \return \ru Возвращает true, если задан корректный радиус, и false - иначе.
            \en Returns true, if a radius is correct, or false otherwise. \~
    \ingroup Polygonal_Objects
  */
  bool AddCylinderRadiusConstraint( double radius )
  {
    return _fitConstraint.AddCylinderRadiusConstraint( radius );
  }
  /** \brief \ru Зафиксировать радиус сферы.
             \en Fix sphere radius. \~
    \details \ru Зафиксировать радиус сферы.
                 Радиус должен быть положительным.
             \en Fix sphere radius.
                 The value has to be positive. \~
    \param[in] radius - \ru Радиус сферы.
                        \en Sphere radius. \~
    \return \ru Возвращает true, если задан корректный радиус, и false - иначе.
            \en Returns true, if a radius is correct, or false otherwise. \~
    \ingroup Polygonal_Objects
  */
  bool AddSphereRadiusConstraint( double radius )
  {
    return _fitConstraint.AddSphereRadiusConstraint( radius );
  }
  /** \brief \ru Зафиксировать угол конуса.
             \en Fix cone angle. \~
    \details \ru Зафиксировать угол конуса.
                 Угол должен быть из промежутка (0; П/2).
             \en Fix cone angle.
                 The angle has to be from (0; П/2). \~
    \param[in] angle - \ru Угол конуса.
                       \en Cone angle. \~
    \return \ru Возвращает true, если задан корректный угол, и false - иначе.
            \en Returns true, if an angle is correct, or false otherwise. \~
    \ingroup Polygonal_Objects
  */
  bool AddConeAngleConstraint( double angle )
  {
    return _fitConstraint.AddConeAngleConstraint( angle );
  }
  /** \brief \ru Зафиксировать радиусы тора.
             \en Fix torus radii. \~
    \details \ru Зафиксировать радиусы тора.
                 Можно зафиксировать большой и малый радиусы тора (только один их них или сразу оба).
                 Малый радиус должен быть положительным (=0 - значение не зафиксировано).
                 Большой радиус может быть отрицательным для торов типа "лимон" (=0 - значение не зафиксировано).
                 Большой радиус (центров) должен быть больше малого радиуса (трубки), взятого с обратным знаком: majorRadius>–minorRadius.
             \en Fix torus radii.
                 There may be fixed the major radius (_size1) or the minor radius (_size2) of a torus (one or both of them).
                 The minor radius has to be positive (=0 - value is not fixed).
                 The major radius may be negative if a torus is of "lemon" type (=0 - value is not fixed).
                 The major radius (of centers) has to be greater than the opposite of the minor radius (of a tube): majorRadius>–minorRadius. \~
    \param[in] majorRadius - \ru Большой радиус тора.
                             \en Major torus radius. \~
    \param[in] minorRadius - \ru Малый радиус тора.
                             \en Minor torus radius. \~
    \return \ru Возвращает true, если заданы корректные радиусы, и false - иначе.
            \en Returns true, if radii are correct, or false otherwise. \~
    \ingroup Polygonal_Objects
  */
  bool AddTorusRadiiConstraint( double majorRadius, double minorRadius )
  {
    return _fitConstraint.AddTorusRadiiConstraint( majorRadius, minorRadius );
  }
  /** \brief \ru Установить ограничение типа XYW.
             \en Set the XYW-constraint. \~
    \details \ru Установить ограничение типа XYW.
                 Ограничение типа XYW допускает только параллельный перенос вдоль осей OX и OY и поворот вокруг оси OZ заданной системы координат.
                 Для плоскости, цилиндра, конуса или тора должен быть зафиксирован угол theta между осью объекта и осью OZ заданной СК.
                 Угол theta должен принадлежать отрезку [0; П/2].
                 Для сферы должна быть зафиксирована координата z ее центра в заданной СК.
                 Для тора должна быть зафиксирована координата z центра его направляющей окружности в заданной СК.
                 При необходимости можно зафиксировать размерные параметры объекта.
                 Функция сбрасывает выставленные ранее ограничения (например, добавленные с помощью методов "Add*Constraint").
             \en Set the XYW-constraint.
                 Translation along the OX и OY axes and rotation about the OZ axis of a local coordinate system are only allowed.
                 For a plane, a cylinder, a cone or a torus there has to be fixed the angle theta between an object's axis and the OZ axis of a given coordinate system.
                 The angle theta has to belong to [0; П/2].
                 For a sphere there has to be fixed the Z coordinate of its center.
                 For a torus there has to be fixed the Z coordinate of its directrix circle.
                 There may be fixed some dimensional parameters of an object if necessary.
                 The function resets all previous constraints (for example, having been set by "Add*Constraint"). \~
    \param[in] typeSurface - \ru Тип поверхности.
                             \en Surface type. \~
    \param[in] typeDim     - \ru Тип размерного ограничения.
                             \en Dimensional constraint type. \~
    \param[in] fixedValues - \ru Фиксированные значения параметров поверхности.
                             \en Structure with fixed values of surface parameters. \~
    \param[in] place       - \ru Локальная система координат.
                             \en Local coordinate system. \~
    \return \ru Возвращает код результата операции.
            \en Returns operation result code. \~
    \ingroup Polygonal_Objects
  */
  MbResultType SetFitConstraintXYW(       MbeSpaceType                 typeSurface,
                                          MbeDimensionalConstraintType typeDim,
                                    const MbConstraintFixedValues &    fixedValues,
                                    const MbPlacement3D &              place )
  {
    return _fitConstraint.InitializeXYW( typeSurface, typeDim, fixedValues, place );
  }
  /** \brief \ru Установить ограничение типа Z.
             \en Set the Z-constraint. \~
    \details \ru Установить ограничение типа Z.
                 Ограничение типа Z допускает только сдвиг вдоль оси OZ заданной системы координат.
                 Для плоскости, цилиндра, конуса или тора должны быть зафиксированы:
                 - зенитный угол theta: угол между осью объекта и осью OZ заданной СК, должен принадлежать отрезку [0; П/2],
                 - азимутальный угол phi: угол между проекцией оси объекта на плоскость OXY и осью OX заданной СК,
                 должен принадлежать промежутку [0; 2П).
                 Для цилиндра, конуса или тора должны быть зафиксированы координаты x и y некоторой точки на оси объекта.
                 Для сферы должны быть зафиксированы координаты x и y ее центра в заданной СК.
                 При необходимости можно зафиксировать размерные параметры объекта.
                 Функция сбрасывает выставленные ранее ограничения (например, добавленные с помощью методов "Add*Constraint").
             \en Set the Z-constraint.
                 Translation along the OZ axis of a local coordinate system is only allowed.
                 For a plane, a cylinder, a cone or a torus there have to be fixed:
                 - the zenith angle theta: the angle between an object's axis and the OZ axis of a given CS, has to belong to [0; П/2],
                 - the azimuthal angle phi: the angle between the projection of an object's axis onto the OXY plane
                 and the OX axis of a given CS, has to belong to [0; 2П).
                 For a cylinder, a cone or a torus there have to be fixed the X and Y coordinates of a point of an object's axis.
                 For a sphere there have to be fixed the X and Y coordinates of its center.
                 There may be fixed some dimensional parameters of an object if necessary.
                 The function resets all previous constraints (for example, having been set by "Add*Constraint"). \~
    \param[in] typeSurface - \ru Тип поверхности.
                             \en Surface type. \~
    \param[in] typeDim     - \ru Тип размерного ограничения.
                             \en Dimensional constraint type. \~
    \param[in] fixedValues - \ru Фиксированные значения параметров поверхности.
                             \en Structure with fixed values of surface parameters. \~
    \param[in] place       - \ru Локальная система координат.
                             \en Local coordinate system. \~
    \return \ru Возвращает код результата операции.
            \en Returns operation result code. \~
    \ingroup Polygonal_Objects
  */
  MbResultType SetFitConstraintZ(       MbeSpaceType                 typeSurface,
                                        MbeDimensionalConstraintType typeDim,
                                  const MbConstraintFixedValues &    fixedValues,
                                  const MbPlacement3D &              place )
  {
    return _fitConstraint.InitializeZ( typeSurface, typeDim, fixedValues, place );
  }
  /** \brief \ru Установить ограничение типа XYZ.
             \en Set the XYZ-constraint. \~
    \details \ru Установить ограничение типа XYZ.
                 Ограничение типа XYZ допускает только параллельный перенос (запрещены повороты вокруг координатных осей заданной системы координат).
                 Для плоскости, цилиндра, конуса или тора должны быть зафиксированы:
                 - зенитный угол theta: угол между осью объекта и осью OZ заданной СК, должен принадлежать отрезку [0; П/2],
                 - азимутальный угол phi: угол между проекцией оси объекта на плоскость OXY и осью OX заданной СК,
                 должен принадлежать промежутку [0; 2П).
                 При необходимости можно зафиксировать размерные параметры объекта.
                 Функция сбрасывает выставленные ранее ограничения (например, добавленные с помощью методов "Add*Constraint").
             \en Set the XYZ-constraint.
                 Translation is only allowed (any rotation is forbidden).
                 For a plane, a cylinder, a cone or a torus there have to be fixed:
                 - the zenith angle theta: the angle between an object's axis and the OZ axis of a given CS, has to belong to [0; П/2],
                 - the azimuthal angle phi: the angle between the projection of an object's axis onto the OXY plane
                 and the OX axis of a given CS, has to belong to [0; 2П).
                 There may be fixed some dimensional parameters of an object if necessary.
                 The function resets all previous constraints (for example, having been set by "Add*Constraint"). \~
    \param[in] typeSurface - \ru Тип поверхности.
                             \en Surface type. \~
    \param[in] typeDim     - \ru Тип размерного ограничения.
                             \en Dimensional constraint type. \~
    \param[in] fixedValues - \ru Фиксированные значения параметров поверхности.
                             \en Structure with fixed values of surface parameters. \~
    \param[in] place       - \ru Локальная система координат.
                             \en Local coordinate system. \~
    \return \ru Возвращает код результата операции.
            \en Returns operation result code. \~
    \ingroup Polygonal_Objects
  */
  MbResultType SetFitConstraintXYZ(       MbeSpaceType                 typeSurface,
                                          MbeDimensionalConstraintType typeDim,
                                    const MbConstraintFixedValues &    fixedValues,
                                    const MbPlacement3D &              place )
  {
    return _fitConstraint.InitializeXYZ( typeSurface, typeDim, fixedValues, place );
  }
  /** \brief \ru Установить ограничение типа ZW.
             \en Set the ZW-constraint. \~
    \details \ru Установить ограничение типа ZW.
                 Ограничение типа ZW допускает только сдвиг вдоль оси OZ и поворот вокруг оси OZ заданной системы координат.
                 Для плоскости, цилиндра, конуса или тора должны быть зафиксированы:
                 - зенитный угол theta: угол между осью объекта и осью OZ заданной СК, должен принадлежать отрезку [0; П/2],
                 - начальный азимутальный угол phi: угол между проекцией оси объекта в начальной позиции на плоскость OXY и осью OX заданной СК,
                 должен принадлежать промежутку [0; 2П),
                 - расстояние dist от оси OZ заданной СК до оси объекта.
                 Для сферы должно быть зафиксировано расстояние dist от ее центра до оси OZ заданной СК.
                 При необходимости можно зафиксировать размерные параметры объекта.
                 Функция сбрасывает выставленные ранее ограничения (например, добавленные с помощью методов "Add*Constraint").
             \en Set the ZW-constraint.
                 Translation along the OZ axis and rotation about the OZ axis of a local coordinate system are only allowed.
                 For a plane, a cylinder, a cone or a torus there have to be fixed:
                 - the zenith angle theta: the angle between an object's axis and the OZ axis of a given CS, has to belong to [0; П/2],
                 - the initial azimuthal angle phi: the angle between the projection of an object's axis in the initial position onto the OXY plane
                 and the OX axis of a given CS, has to belong to [0; 2П),
                 - the distance between an object's axis the OZ axis of a given CS.
                 For a sphere there has to be fixed the distance between its center and the OZ axis of a given CS.
                 There may be fixed some dimensional parameters of an object if necessary.
                 The function resets all previous constraints (for example, having been set by "Add*Constraint"). \~
    \param[in] typeSurface - \ru Тип поверхности.
                             \en Surface type. \~
    \param[in] typeDim     - \ru Тип размерного ограничения.
                             \en Dimensional constraint type. \~
    \param[in] fixedValues - \ru Фиксированные значения параметров поверхности.
                             \en Structure with fixed values of surface parameters. \~
    \param[in] place       - \ru Локальная система координат.
                             \en Local coordinate system. \~
    \return \ru Возвращает код результата операции.
            \en Returns operation result code. \~
    \ingroup Polygonal_Objects
  */
  MbResultType SetFitConstraintZW(       MbeSpaceType                 typeSurface,
                                         MbeDimensionalConstraintType typeDim,
                                   const MbConstraintFixedValues &    fixedValues,
                                   const MbPlacement3D &              place )
  {
    return _fitConstraint.InitializeZW( typeSurface, typeDim, fixedValues, place );
  }
  /// \ru Установить максимальное количество точек для вписывания. \en Set the maximum count of fitting points.
  void SetPointsFittingMax( size_t value ) { _pointsFittingMax = value; }
  /// \ru Установить флаг уточнения поверхности по всем точкам. \en Set the flag of surface refinement by all the points.
  void SetWithRefinement( bool value ) { _withRefinement = value; }
  /// \ru Установить флаг замкнутости вписываемой поверхности. \en Set the flag of surface closedness.
  void SetClosed( bool closed ) { _closed = closed; }

  OBVIOUS_PRIVATE_COPY( MbSurfaceFitToGridParameters )
};


//------------------------------------------------------------------------------
/** \brief \ru Результат вписывания поверхности.
           \en Parameters of surface fitting. \~
  \ingroup Polygonal_Objects
*/
// ---
class MATH_CLASS MbSurfaceFitToGridResults
{
private:
  c3d::SurfaceSPtr _surface;   ///< \ru Поверхность. \en A surface. 
  double           _devMax;    ///< \ru Максимальное получившееся отклонение. \en A maximal deviation. 
  double           _devMean;   ///< \ru Среднее получившееся отклонение. \en A mean deviation. 
  double           _rmsd;      ///< \ru Среднее квадратическое отклонение. \en A root-mean-square deviation. 
public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  MbSurfaceFitToGridResults()
    : _surface( nullptr )
    , _devMax ( 0.0     )
    , _devMean( 0.0     )
    , _rmsd   ( 0.0     )
  {}
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  MbSurfaceFitToGridResults( MbSurface * surface, double tolerance )
    : _surface( surface   )
    , _devMax ( tolerance )
    , _devMean( 0.0       )
    , _rmsd   ( 0.0       )
  {}
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  MbSurfaceFitToGridResults( MbSurface * surface, double devMax, double devMean, double rmsd )
    : _surface( surface )
    , _devMax ( devMax  )
    , _devMean( devMean )
    , _rmsd   ( rmsd    )
  {}
  /// \ru Инициализировать данные. \en Initialize data.
  void Init( MbSurface * surface, double tolerance ) {
    _surface = surface;
    _devMax = tolerance;
    _devMean = 0.;
    _rmsd = 0.;
  }
  /// \ru Инициализировать данные. \en Initialize data.
  void Init( MbSurface * surface, double devMax, double devMean, double rmsd ) {
    _surface = surface;
    _devMax = devMax;
    _devMean = devMean;
    _rmsd = rmsd;
  }
  /// \ru Выдать поверхность. \en Get surface.
  c3d::SurfaceSPtr GetSurface() const { return _surface; }
  /// \ru Выдать максимальное отклонение. \en Get maximal deviation.
  double GetTolerance() const { return _devMax; }
  /// \ru Выдать среднее отклонение. \en Get mean deviation.
  double GetMeanDeviation() const { return _devMean; }
  /// \ru Выдать среднее квадратическое отклонение. \en Get root-mean-square deviation.
  double GetRmsd() const { return _rmsd; }

  OBVIOUS_PRIVATE_COPY( MbSurfaceFitToGridResults )
};


#endif // __OP_MESH_PARAMETERS_H
