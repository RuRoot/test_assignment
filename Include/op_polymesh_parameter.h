////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Параметры операций над полигональными сетками с топологией.
         \en Parameters of operations on the polygonal meshes with topology. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __OP_POLYMESH_PARAMETERS_H
#define __OP_POLYMESH_PARAMETERS_H

#include <math_define.h>
#include <polymesh.h>
#include <mb_placement3d.h>
#include <mb_data.h>
#include <templ_sptr.h>
#include <topology.h>
#include <curve.h>
#include <curve3d.h>
#include <m2b_mesh_curvature.h>

class MbFaceShell;
class MbHalfedgeAttrSizetEdge;
class MbHalfedgeAttrDoubleEdge;
class MbHalfedgeAttrBoolVertex;
class MbPolyline3D;

//------------------------------------------------------------------------------
/** \brief \ru Параметры ремешинга.
           \en Remeshing parameters. \~
    \details \ru Параметры ремешинга.
             \en Remeshing parameters.
    \ingroup Polygonal_Objects
    \warning \ru В разработке.
           \en Under development. \~
*/
// ---
class MATH_CLASS MbRemeshParams
{
public:
  //------------------------------------------------------------------------------
  /** \brief \ru Перечисление методов ремешинга.
             \en Enumeration for remeshing methods. \~
  */
  // ---
  enum class MethodRemesh
  {
    incrementalUniform,   ///< \ru Однородный инкрементальный ремешинг. \en Uniform incremental remeshing.
    incrementalCurvature  ///< \ru Инкрементальный ремешинг по кривизне. \en Curvature incremental remeshing.
  };

private:
  MethodRemesh       _method;       ///< \ru Метод ремешинга. \en A remeshing method.
  double             _targetLength; ///< \ru Целевой размер ребра фасета для однородного метода. \en A target facet edge length for the uniform method.
  double             _tolCurvature; ///< \ru Относительная точность для метода по кривизне. \en A relative tolerance for the curvature method.
  double             _angleEdge;    ///< \ru Угол для автоматической разбивки ребер сегментации по сломам (градусы), если меньше нуля - не учитывается. \en A angle for automatic segmentation edges splitting (degrees). It is not used if negative. \~
  c3d::IndicesVector _fixedEdges;   ///< \ru Фиксированные ребра сетки. \en Mesh edges to be fixed.

public:
  mutable IProgressIndicator * _progress;  ///< \ru Индикатор прогресса выполнения операции. \en A progress indicator of the operation.

public:
  /// \ru Конструктор. \en Constructor.
  MbRemeshParams( IProgressIndicator * prog = nullptr );
  /// \ru Деструктор. \en Destructor.
  ~MbRemeshParams();

  /// \ru Инициализировать как однородный инкрементальный ремешинг. \en Initialize as a uniform incremental remeshing.
  void InitIncrementalUniform( double targetLength, const c3d::IndicesVector & fixedEdges, double angleEdge = -1. );
  /// \ru Инициализировать как инкрементальный ремешинг по кривизне. \en Initialize as an incremental remeshing by curvature.
  void InitIncrementalCurvature( double tolCurvature, const c3d::IndicesVector & fixedEdges, double angleEdge = -1. );
  ///< \ru Получить целевой размер ребра фасета. \en Get the target facet edge length.
  double GetTargetLength() const { return _targetLength; }
  ///< \ru Получить относительная точность. \en Get the relative tolerance.
  double GetCurvatureTolerance() const { return _tolCurvature; }
  ///< \ru Получить угол для разбивки ребер сегментации. \en Get the angle for segmentation edges splitting.
  double GetAngleEdge() const { return _angleEdge; }
  ///< \ru Получить способ вычисления. \en Get the calculation method.
  MethodRemesh GetMethod() const { return _method; }
  ///< \ru Получить фиксированные ребра. \en Get fixed edges.
  const c3d::IndicesVector & GetFixedEdges() const { return _fixedEdges; }
  ///< \ru Зафиксировать открытые границы. \en Fix open boundaries.
  void FixOpenBoundaries( MbPolymesh & mesh );

  OBVIOUS_PRIVATE_COPY( MbRemeshParams );
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры линеаризации оболочки.
           \en Parameters for the shell linearization. \~
    \details \ru Параметры линеаризации оболочки.
                 Для каждой заданной группы граней будет создан один регион в полигональном объекте.
             \en Parameters for the shell linearization.
                 Creates one polymesh region for every given face group. \~
    \ingroup Polygonal_Objects
    \warning \ru В разработке.
           \en Under development. \~
*/
// ---
class MATH_CLASS MbShellLinearizationParams
{
private:
  bool                             _b2d;         ///< \ru Флаг создания атрибута параметров. \en A parametrers attribute creation flag. \~
  bool                             _bNormals;    ///< \ru Флаг создания атрибута нормалей. \en A normals attribute creation flag. \~
  bool                             _bCurvatures; ///< \ru Флаг создания атрибута кривизн. \en A curvatures attribute creation flag. \~
  MbStepData                       _stepData;    ///< \ru Данные для расчета шага. \en Data for the step calculation. \~
  std::vector<c3d::IndicesVector>  _faceGroups;  ///< \ru Группы граней. \en Face groups.
  c3d::IndicesVector               _fixedEdges;  ///< \ru Фиксированные ребра оболочки. \en Constrained shell edges.

public:
  /// \ru Конструктор. \en Constructor.
  MbShellLinearizationParams( const MbStepData & data, bool b2d, bool bNormals, bool bCurvatures, const std::vector<c3d::IndicesVector> & faceGroups, const c3d::IndicesVector & fixedEdges );
  /// \ru Конструктор. \en Constructor.
  MbShellLinearizationParams( const MbStepData & data, bool b2d, bool bNormals, bool bCurvatures, const std::vector<std::vector<MbItemIndex>> & faceGroups );
  /// \ru Конструктор. \en Constructor.
  MbShellLinearizationParams( const MbStepData & data, bool b2d, bool bNormals, bool bCurvatures, const MbFaceShell & shell, const std::vector<std::vector<const MbFace*>> & faceGroups );
  /// \ru Деструктор. \en Destructor.
  ~MbShellLinearizationParams() {}

  // \ru Получить данные для расчета шага. \en Get data for the step calculation. \~
  const MbStepData & GetStepData() const { return _stepData; }
  // \ru Получить группы граней. \en Get face groups. \~
  const std::vector<c3d::IndicesVector> & GetFaceGroups() const { return _faceGroups; }
  // \ru Получить фиксированные ребра. \en Get constrained edges. \~
  const c3d::IndicesVector & GetFixedEdges() const { return _fixedEdges; }
  // \ru Нужно ли создавать атрибут параметров. \en Check if parameters attribute should be created. \~
  bool IsUvNeeded() const { return _b2d; }
  // \ru Нужно ли создавать атрибут нормалей. \en Check if normals attribute should be created. \~
  bool IsNormalsNeeded() const { return _bNormals; }
  // \ru Нужно ли создавать атрибут кривизн. \en Check if curvatures attribute should be created. \~
  bool IsCurvaturesNeeded() const { return _bCurvatures; }

  OBVIOUS_PRIVATE_COPY( MbShellLinearizationParams );
};


//------------------------------------------------------------------------------
/** \brief \ru Результаты линеаризации оболочки.
           \en Shell linearization results. \~
    \details \ru Результаты линеаризации оболочки.
                 Содержат набор граней, которые не удалось линеаризовать.
             \en Shell linearization results.
                 Parameters contain the indices of unlinearized faces. \~
    \ingroup Polygonal_Objects
    \warning \ru В разработке.
           \en Under development. \~
*/
// ---
class MATH_CLASS MbShellLinearizationResults
{
private:
  c3d::IndicesVector _facesUnlinearized; ///< \ru Нелинеаризованные грани. \en Unlinearized faces. \~

public:
  /// \ru Конструктор. \en Constructor.
  MbShellLinearizationResults() : _facesUnlinearized() {}
  /// \ru Деструктор. \en Destructor.
  ~MbShellLinearizationResults() {}

public:
  // \ru Получить нелинеаризованные грани. \en Get the unlinearized faces. \~
  const c3d::IndicesVector & GetUnlinearizedFaces() const { return _facesUnlinearized; }
  // \ru Инициализировать результаты. \en Initialize results. \~
  void Initialize( const c3d::IndicesVector & facesUnlinearized ) { _facesUnlinearized = facesUnlinearized; }

  OBVIOUS_PRIVATE_COPY( MbShellLinearizationResults );
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры операции расчета кривизн полигональной сетки.
           \en Parameters for mesh curvature calculation. \~
    \details \ru Параметры операции расчета кривизн полигональной сетки.
                 Возможны два метода - с помощью дискретного дифференциального оператора, и с помощью вписывания полиномиальной поверхности
                 в окрестность вершины. Количество соседей для второго метода должно быть в пределах от 1 до 10.
             \en Parameters for mesh curvature calculation. 
                 There are two methods are possible - by discrete differential operator and by polynomial surface fitting.
                 Number of neighbors for second method must be in the range from 1 to 10. \~
    \ingroup Polygonal_Objects
*/
// ---
class MATH_CLASS MbMeshCurvatureParams
{
public:
  //------------------------------------------------------------------------------
  /** \brief \ru Перечисление методов расчета кривизн полигональной сетки.
  */
  // ---
  enum class MethodCalcCurvatures
  {
    byDiscrDiffOperators, ///< \ru Дискретный дифференциальный оператор. \en Discrete differential operator.
    byApproxPolynom       ///< \ru Аппроксимация полиномиальной поверхностью. \en Polynomial surface fitting.
  };

private:
  MethodCalcCurvatures _method;        ///< \ru Метод расчета. \en Calculation method.
  size_t               _nNeighborRing; ///< \ru Количество соседей для аппроксимации поверхностью. \en Number of neighbors for surface fitting.

public:
  /// \ru Конструктор. \en Constructor.
  MbMeshCurvatureParams()
    : _method       ( MethodCalcCurvatures::byDiscrDiffOperators )
    , _nNeighborRing( 0                                          )
  {}
  /// \ru Деструктор. \en Destructor.
  ~MbMeshCurvatureParams() {}

  /// \ru Инициализировать метод дискретного оператора. \en Initialize discrete operator method.
  void InitOperatorMethod()
  {
    _method = MethodCalcCurvatures::byDiscrDiffOperators;
    _nNeighborRing = 0;
  }
  /// \ru Инициализировать метод полиномиальной поверхности. \en Initialize polynomial surface method.
  void InitPolynomMethod( size_t nRing )
  {
    _method = MethodCalcCurvatures::byApproxPolynom;
    _nNeighborRing = nRing;
  }

  ///< \ru Получить способ вычисления. \en Get calculation method.
  MethodCalcCurvatures GetMethod() const { return _method; }
  ///< \ru Получить количество соседей. \en Get number of neighbors.
  size_t GetRingsNumber() const { return _nNeighborRing; }

  OBVIOUS_PRIVATE_COPY( MbMeshCurvatureParams )
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры драпировки оболочки тканью.
           \en Parameters for the shell draping. \~
  \details \ru Параметры драпировки оболочки тканью. \n
                 Параметры содержат информацию о положении развёртки и свойствах материала.
           \en Parameters for the shell draping. \n
               Parameters contain information about the scan position and material properties. \~
  \ingroup Shell_Building_Parameters
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
class MATH_CLASS MbDrapeShellParams
{
public:
  //------------------------------------------------------------------------------
  /** \brief \ru Перечисление для типа констрейна для кривой.
             \en Constraint curve type. \~
    \details \ru Перечисление для типа констрейна для кривой.
             \en Constraint curve type. \~
  */
  // ---
  enum class ConstraintCurveType
  {
    geodesic, ///< \ru Геодезическая. \en Geodesic.
    curve     ///< \ru Заданная кривая. \en Given curve.
  };

  //------------------------------------------------------------------------------
  /** \brief \ru Перечисление для угла между основой ткани и кривой-кострейна.
             \en Fiber angle from a constraint curve. \~
    \details \ru Перечисление для угла между основой ткани и кривой-кострейна.
             \en Fiber angle from a constraint curve. \~
  */
  // ---
  enum class AngleFromCurveType
  {
    degrees0,      ///< \ru Ноль градусов. \en Zero degrees.
    degreesPlus45, ///< \ru 45 градусов. \en 45 degrees.
    degreesMinus45 ///< \ru Минус 45 градусов. \en Minus 45 degrees.
  };

private:
  double                     _epsSew;              ///< \ru Точность сшивки сетки. \en Sew tolerance. \~
  double                     _cellSize;            ///< \ru Размер ячейки ткани. \en Fiber cell size. \~
  ConstraintCurveType        _constraintCurveType; ///< \ru Тип кривой-констрейна. \en Constraint curve type. \~
  AngleFromCurveType         _angleFromCurveType;  ///< \ru Угол между основой ткани и кривой. \en Fiber angle from a curve. \~
  MbPlacement3D              _place;               ///< \ru Локальная система координат развернутой поверхности. \en The local coordinate system for the result surface. \~
  MbStepData                 _stepData;            ///< \ru Данные для расчета шага. \en Data for the step calculation. \~
  SPtr<MbPolymesh>           _mesh;                ///< \ru Линеаризация оболочки. \en Shell linearization. \~
  SPtr<const MbCurve3D>      _firstStageRegion;    ///< \ru Кривая для приоритетной области. \en First stage region curve. \~
  SPtr<const MbCurve3D>      _constrainCurve;      ///< \ru Кривая-констрейн. \en Constraint curve. \~
  c3d::IndicesVector         _firstStageFaces;     ///< \ru Массив индексов граней для приоритетной области. \en Face indices for the first stage region. \~
  c3d::SpaceCurvesSPtrVector _markerCurves;        ///< \ru Маркеры-кривые. \en Marker curves. \~
  c3d::SpacePointsVector     _markerPoints;        ///< \ru Маркеры-точки. \en Marker points. \~

public:
  mutable IProgressIndicator * _progress;  ///< \ru Индикатор прогресса выполнения операции. \en A progress indicator of the operation.

public:
  /// \ru Конструктор (только для внутреннего использования ). \en Constructor ( for internal usage only ).
  MbDrapeShellParams();

public:
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  MbDrapeShellParams( const MbFaceShell & shell, const MbStepData & stepData, double epsSew = METRIC_EPSILON );
  /// \ru Деструктор. \en Destructor.
  ~MbDrapeShellParams();

public:
  // \ru Валидны ли данные. \en Is data valid.
  bool IsValid() const;
  // \ru Инициализировать данные. \en Initialize data.
  void Init( const MbPlacement3D &     place, 
                   ConstraintCurveType constrCurveType, 
                   AngleFromCurveType  angFromCurveType, 
                   double              cellSize, 
             const MbCurve3D *         pConstraintCurve = nullptr,
             const MbCurve3D *         pFirstStageRegion = nullptr);

  // \ru Получить линеаризацию оболочки. \en Get shell linearization. \~
  const SPtr<MbPolymesh> & GetMesh() const { return _mesh; }
  // \ru Получить приоритетную область. \en Get the first stage region. \~
  const SPtr<const MbCurve3D> & GetFirstStageRegion() const { return _firstStageRegion; }
  // \ru Получить для изменения приоритетную область. \en Get first stage region. \~
  SPtr<const MbCurve3D> & SetFirstStageRegion() { return _firstStageRegion; }
  // \ru Получить кривую-констрейн. \en Get constraint curve. \~
  const SPtr<const MbCurve3D> & GetConstraintCurve() const { return _constrainCurve; }
  // \ru Получить для изменения кривую-констрейн. \en Get constraint curve. \~
  SPtr<const MbCurve3D> & SetConstraintCurve() { return _constrainCurve; }
  // \ru Получить данные для расчета шага. \en Get data for step calculation. \~
  const MbStepData & GetStepData() const { return _stepData; }
  // \ru Получить массив граней приоритетной области. \en Get the first stage region faces. \~
  const c3d::IndicesVector GetFirstStageRegionFaces() const { return _firstStageFaces; }
  // \ru Установить массив граней приоритетной области. \en Set the first stage region faces. \~
  void SetFirstStageRegionFaces( const c3d::IndicesVector & faces ) { _firstStageFaces = faces; }
 
  /// \ru Установить локальную систему координат развернутой поверхности сетки. \en Set the local coordinate system for a mesh unwrapping. \~
  void SetPlacement( const MbPlacement3D & pl ) { _place.Init( pl ); }
  // \ru Получить локальную систему координат развернутой поверхности сетки. \en Get the local coordinate system for a mesh unwrapping. \~
  const MbPlacement3D & GetPlacement() const { return _place; }
  /// \ru Установить размер ячейки. \en Set cell size. \~
  void SetCellSize( double cellSize ) { _cellSize = cellSize; }
  // \ru Получить размер ячейки. \en Get cell size. \~
  double GetCellSize() const { return _cellSize; }
  /// \ru Установить тип кривой-констрейна. \en Set constraint curve type. \~
  void SetConstraintCurveType( ConstraintCurveType type ) { _constraintCurveType = type; }
  // \ru Получить тип кривой-констрейна. \en Get constraint curve type. \~
  ConstraintCurveType GetConstraintCurveType() const { return _constraintCurveType; }
  /// \ru Установить угол между основой ткани и кривой. \en Set fiber angle from the curve. \~
  void SetAngleFromCurveType( AngleFromCurveType type ) { _angleFromCurveType = type; }
  // \ru Получить угол между основой ткани и кривой. \en Get fiber angle from the curve. \~
  AngleFromCurveType GetAngleFromCurveType() const { return _angleFromCurveType; }

  // \ru Функции только для внутреннего использования. \en Functions for the internal usage only. 

  // \ru Получить для изменения линеаризацию оболочки. \en Get the shell linearization. \~
  SPtr<MbPolymesh> & SetMesh() { return _mesh; }
  // \ru Получить для изменения данные для расчета шага. \en Get data for the step calculation. \~
  MbStepData & SetStepData() { return _stepData; }
  // \ru Инициализировать линеаризацию оболочки. \en Initialize the shell linearization. \~
  void InitMesh( const MbFaceShell & shell, const MbStepData & stepData, double epsSew );
  /// \ru Установить точность сшивки. \en Set sew tolerance. \~
  void SetSewTolerance( double epsSew ) { _epsSew = epsSew; }
  // \ru Получить точность сшивки. \en Get sew tolerance. \~
  double GetSewTolerance() const { return _epsSew; }
  // \ru Получить маркеры-кривые. \en Get the marker curves. \~
  const c3d::SpaceCurvesSPtrVector & GetMarkerCurves() const { return _markerCurves; }
  // \ru Получить маркеры-кривые. \en Get the marker curves. \~
  c3d::SpaceCurvesSPtrVector & SetMarkerCurves() { return _markerCurves; }
  // \ru Получить маркеры-точки. \en Get the marker points. \~
  const c3d::SpacePointsVector & GetMarkerPoints() const { return _markerPoints; }
  // \ru Получить маркеры-точки. \en Get the marker points. \~
  c3d::SpacePointsVector & SetMarkerPoints() { return _markerPoints; }

  
  OBVIOUS_PRIVATE_COPY( MbDrapeShellParams );
}; // MbDrapeShellParams


//------------------------------------------------------------------------------
/** \brief \ru Результаты драпировки оболочки тканью.
           \en Shell draping results. \~
  \details \ru Результаты драпировки оболочки тканью. \n
               Содержат информацию о ячейках ткани в 3D и развертки в 2D.
           \en Shell draping results. \n
               Results contain information about 3D fiber cells and 2D unwrapping. \~
  \ingroup Shell_Building_Parameters
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
class MATH_CLASS MbDrapeShellResults
{
public:
  //------------------------------------------------------------------------------
 /** \brief \ru Перечисление для типа ошибок драпировки.
            \en Error code type. \~
   \details \ru Перечисление для типа ошибок драпировки.
            \en Error code type. \~
 */
 // ---
  enum class DrapErrorType : uint
  {
    unbuiltCells                = 0x01, ///< \ru Невозможно построить некоторые ячейки. \en It is not possible to build some cells.
    fabricOverlapping           = 0x02, ///< \ru Наложение разных частей ткани. \en Overlapping fabric pieces were found.
    coordSystemCenterOnBoundary = 0x03, ///< \ru Точка приложения попадает точно на границу. \en The center of a coordinate system is just on the boundary.
    coordSystemCenterIsOut      = 0x04, ///< \ru Точка приложения за пределами рабочей области. \en The center of a coordinate system is out of the surface.
    unbuiltMarkers              = 0x05, ///< \ru Невозможно построить некоторые маркеры. \en It is not possible to build some markers.
  };

private:
  SPtr<MbPolymesh>                    _mesh;           ///< \ru Результат. \en The result. \~
  std::vector<c3d::SpacePointsVector> _sides;          ///< \ru Полилинии сторон ячеек. \en Cell sides polylines. \~
  std::vector<c3d::SpacePointsVector> _warp;           ///< \ru Полилинии для нулевой нити основы. \en Warp polylines. \~
  std::vector<c3d::SpacePointsVector> _weft;           ///< \ru Полилинии для нулевой нити утка. \en Weft polylines. \~
  std::vector<c3d::ParamPointsVector> _markers;        ///< \ru Полилинии маркеров. \en Marker polylines. \~
  c3d::ParamPointsVector              _markerPoints;   ///< \ru Маркеры точки. \en Marker points. \~
  MbHalfedgeAttrSizetEdge *           _pAttrId;        ///< \ru Атрибут индексов сторон ячеек. \en An attribute for cell sides indices. \~
  MbHalfedgeAttrDoubleEdge *          _pAttrAng;       ///< \ru Атрибут углов сторон ячеек. \en An attribute for cell sides angles. \~
  MbHalfedgeAttrBoolVertex *          _pAttrNode;      ///< \ru Атрибут флагов узлов ткани. \en An attribute for fiber nodes flags. \~
  MbHalfedgeAttrSizetEdge *           _pAttrDir;       ///< \ru Атрибут направлений сторон ячеек ткани. \en An attribute for cell sides directions. \~
  uint                                _errorCodesMask; ///< \ru Коды ошибок драпировки. \en Codes of draping errors. \~

public:
  /// \ru Конструктор. \en Constructor.
  MbDrapeShellResults();
  /// \ru Деструктор. \en Destructor.
  ~MbDrapeShellResults();

  /// \ru Очистить данные. \en Clear data.
  void Clear();
  // \ru Получить результат. \en Get the result. \~
  const SPtr<MbPolymesh> & GetMesh() const { return _mesh; }
  // \ru Получить результат. \en Get the result. \~
  SPtr<MbPolymesh> & SetMesh() { return _mesh; }

  // \ru Является ли вершина узлом ткани. \en Is vertex a fiber node. \~
  bool IsNode( size_t iVx ) const;
  // \ru Получить угол деформации для ребра сетки. \en Get a deformation angle for the mesh edge. \~
  double GetEdgeAngle( size_t iEd ) const;
  // \ru Получить идентификатор полилинии для этого ребра сетки. \en Get a polyline index for the given mesh edge. \~
  size_t  GetSideId( size_t iEd ) const;
  /** \brief \ru Получить полилинии для главной нити основы.
            \en Get a polyline for the main warp thread. \~
   \details \ru Получить полилинии для главной нити основы.
                Эти полилинии идут от точки приложения в положительном и отрицательном направлении
                оси Х локальной системы координат.
            \en Get polyline for maGet a polyline for the main warp thread.
                Polylines go in a positive and negative direction along the X-axis of the local coordinate system.
 */
  const std::vector<c3d::SpacePointsVector> & GetWarp() const { return _warp; }
  /** \brief \ru Получить полилинии для главной нити утка.
             \en Get a polyline for the main weft thread. \~
    \details \ru Получить полилинии для главной нити утка.
                 Эти полилинии идут от точки приложения в положительном и отрицательном направлении
                 оси Y локальной системы координат.
             \en Get a polyline for the main weft thread.
                 Polylines go in a positive and negative direction along the Y-axis of the local coordinate system.
  */
  const std::vector<c3d::SpacePointsVector> & GetWeft() const { return _weft; }
  /** \brief \ru Получить полилинии для главной нити основы.
           \en Get a polyline for the main warp thread. \~
  \details \ru Получить полилинии для главной нити основы.
               Эти полилинии идут от точки приложения в положительном и отрицательном направлении
               оси Х локальной системы координат.
           \en Get a polyline for the main warp thread.
               Polylines go in a positive and negative direction along the X-axis of the local coordinate system.
*/
  std::vector<c3d::SpacePointsVector> & GetWarp() { return _warp; }
  /** \brief \ru Получить полилинии для главной нити утка.
             \en Get a polyline for the main weft thread. \~
    \details \ru Получить полилинии для главной нити утка.
                 Эти полилинии идут от точки приложения в положительном и отрицательном направлении
                 оси Y локальной системы координат.
             \en Get a polyline for the main weft thread. 
                 Polylines go in a positive and negative direction along the Y-axis of the local coordinate system.
  */
  std::vector<c3d::SpacePointsVector> & GetWeft() { return _weft; }
  /// \ru Получить полилинию для стороны ячейки. \en Get a cell side polyline.
  const std::vector<c3d::SpacePointsVector> & GetCellSides() const { return _sides; }
  /// \ru Получить полилинию для стороны ячейки. \en Get a cell side polyline.
  std::vector<c3d::SpacePointsVector> & GetCellSides() { return _sides; }
  /// \ru Получить полилинии маркеров. \en Get the marker polylines.
  std::vector<c3d::ParamPointsVector> & SetMarkersPolylines() { return _markers; }
  /// \ru Получить полилинии маркеров. \en Get the marker polylines.
  const std::vector<c3d::ParamPointsVector> & GetMarkersPolylines() const { return _markers; }
  /// \ru Получить маркеры точки. \en Get the marker points.
  c3d::ParamPointsVector & SetMarkerPoints() { return _markerPoints; }
  /// \ru Получить маркеры точки. \en Get the marker points.
  const c3d::ParamPointsVector & GetMarkerPoints() const { return _markerPoints; }
  /** \brief \ru Получить грань-развертку.
             \en Get an unwrapped face. \~
    \details \ru Получить грань-развертку.
                 Ее контур получается путем аппроксимации границы сеточной развертки.
             \en Get an unwrapped face.
                 This is a contour calculated by an approximation of the unwrapped mesh boundary.
    \param[in] tolApprox - \ru Точность аппроксимации.
                           \en An approximation tolerance. \~
    \param[out] unwrapped - \ru Грань-развертка.
                            \en An unwrapped face. \~
  */
  void GetUnwrapped( double tolApprox, c3d::FaceSPtr & unwrapped ) const;
  /** \brief \ru Получить маркеры.
           \en Get the markers. \~
  \details \ru Получить маркеры.
               Кривые получаются путем аппроксимации полилиний.
           \en Get the markers.
               The curves are calculated by the approximation of the polylines.
  \param[in] tolApprox - \ru Точность аппроксимации.
                         \en An approximation tolerance. \~
  \param[out] approxMarkers - \ru Аппроксимации полилиний.
                              \en A polyline approximations. \~
*/
  void GetMarkerCurves( double tolApprox, c3d::PlaneCurvesSPtrVector & approxMarkers ) const;
  /** \brief \ru Получить полилинию разреза ткани на поверхности оболочки.
            \en Get a fiber cut polyline on the shell surface. \~
   \details \ru Получить полилинию разреза ткани на поверхности оболочки.
            \en Get a fiber cut polyline on the shell surface.
   \param[in] uv - \ru Начало прямой - линии разреза.
                   \en A cut line beginning. \~
   \param[in] dir - \ru Направление прямой - линии разреза.
                    \en A cut line direction. \~
   \param[out] polys - \ru Полилинии - отображение линии разреза на оболочку.
                       \en Polylines represented a line mapping to the shell. \~
 */
  void GetSplitPolylines( const MbCartPoint & uv, const MbVector & dir, std::vector<c3d::SpacePointsVector> & polys ) const;
  /** \brief \ru Получить атрибут направлений сторон ячеек.
             \en Get an attribute for cell sides directions. \~
   \details \ru Получить атрибут направлений сторон ячеек.
                Возможные значения:
                0 - вдоль основы, 1 - вдоль утка, 2 - против основы, 3 - против утка, SYS_MAX_T - не определено.
            \en Get an attribute for cell sides directions.
                Possible values are:
                0 - along warp, 1 - along weft, 2 - against warp, 3 - against weft, SYS_MAX_T - undefined.
 */
  const MbHalfedgeAttrSizetEdge * GetDirectionalAttribute() const { return _pAttrDir; }

  // \ru Очистить коды ошибок. \en Clear error codes. \~
  void ClearErrorCodes() { _errorCodesMask = 0; }
  // \ru Добавить код ошибки. \en Add error code. \~
  void AddErrorCode( DrapErrorType type ) { _errorCodesMask |= static_cast<uint>( type ); }
  // \ru Проверить наличие кода ошибки. \en Check for an error code. \~
  bool HasErrorCode( DrapErrorType type ) const { return _errorCodesMask & static_cast<uint>( type ); }
  // \ru Получить маску кодов ошибок. \en Get an error code mask. \~
  uint GetErrorCodesMask() const { return _errorCodesMask; }

  // \ru Инициализировать атрибуты сетки. \en Initialize the mesh attributes. \~
  void InitAttributes();

  OBVIOUS_PRIVATE_COPY( MbDrapeShellResults );
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры объединения полигональных объектов.
           \en Parameters for polymesh merging. \~
  \details \ru Параметры объединения полигональных объектов. \n
               Параметры содержат информацию о соответствующих рёбрах из объединяемых полигональных объектов, при объединении эти рёбра должны быть склеены.
               Первый компонент пары - индекс ребра топологии полигонального объекта, который копируется.
               Второй компонент пары - индекс ребра топологии полигонального объекта, который аккумулирует результат объединения.
           \en Parameters for polymesh merging. \n
               Parameters may contain information about corresponding edges from polymeshes being merged - these edges will be marked as twins.
               The first pair element is the edge index from the topology of a polymesh being copied.
               The second pair element is the edge index from the topology of a polymesh accumulating merged objects. \~
  \ingroup Polygonal_Objects
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
class MATH_CLASS MbMergePolymeshParams
{
private:
  c3d::IndicesPairsVector _edgePairs; ///< \ru Пары ребер, которые должны быть склеены. \en Pairs of edges being merged with each other. \~

public:
  /// \ru Конструктор. \en Constructor.
  MbMergePolymeshParams() : _edgePairs() {}
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  MbMergePolymeshParams( const c3d::IndicesPairsVector & edgePairs ) : _edgePairs( edgePairs ) {}
  /// \ru Деструктор. \en Destructor.
  ~MbMergePolymeshParams() {}

public:
  // \ru Получить пары соответствующих ребер. \en Get the pairs of corresponding edges. \~
  const c3d::IndicesPairsVector & GetEdgePairs() const { return _edgePairs; }
  // \ru Установить пары соответствующих ребер. \en Set the pairs of corresponding edges. \~
  void SetEdgePairs( const c3d::IndicesPairsVector & edgePairs ) { _edgePairs = edgePairs; }

  OBVIOUS_PRIVATE_COPY( MbMergePolymeshParams );
}; // MbMergePolymeshParams


//------------------------------------------------------------------------------
/** \brief \ru Параметры сглаживания полигональных объектов.
           \en Parameters for the polymesh smoothing. \~
  \details \ru Параметры сглаживания полигональных объектов. \n
               Параметры содержат информацию о коэффициентах сглаживания, методе сглаживания, максимальном числе итераций,
               наборе индексов фасетов для сглаживания.
           \en Parameters for the polymesh smoothing. \n
               The parameters contain information about the smoothing coefficients, the smoothing method, the maximum number of iterations 
               and the set of facet indices for smoothing. \~
  \ingroup Polygonal_Objects
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
class MATH_CLASS MbMeshSmoothingParams
{
public:
  //------------------------------------------------------------------------------
  /** \brief \ru Перечисление методов сглаживания.
             \en Enumeration for smoothing methods. \~
  */
  // ---
  enum class MethodSmoothMesh
  {
    smoothTaubin, ///< \ru Метод Таубина. \en Taubin's method.
    smoothLaplas  ///< \ru Метод Лапласа. \en Laplace method.
  };

private:
  MethodSmoothMesh   _method;         ///< \ru Метод сглаживания сетки. \en Mesh smoothing method.
  double             _lambda;         ///< \ru Коэффициент сжатия, используемый на нечетных шагах в "taubin smoothing". \en The compression factor used in odd steps in "taubin smoothing".
  double             _mu;             ///< \ru Коэффициент расширения, используемый на четных шагах в "taubin smoothing". \en The expansion factor used in even steps in "taubin smoothing".
  size_t             _maxIter;        ///< \ru Максимальное число итераций. \en Maximum iteration number.
  bool               _bFixBoundaries; ///< \ru Сохранять границы. \en Save boundaries.
  c3d::IndicesVector _facets;         ///< \ru Индексы фасетов. Если набор фасетов пустой, то сглаживается вся сетка. \en Facet indices to smooth. If the facet set is empty, the entire mesh is smoothed.
  c3d::IndicesVector _fixVertices;    ///< \ru Индексы фиксированных вершин. \en Fixed vertex indices.

public:
  /// \ru Конструктор. \en Constructor.
  MbMeshSmoothingParams()
    : _method         ( MethodSmoothMesh::smoothTaubin )
    , _lambda         ( 0.33                           )
    , _mu             ( -0.34                          )
    , _maxIter        ( 100                            )
    , _bFixBoundaries ( true                           )
    , _facets         (                                )
    , _fixVertices    (                                )
  {}
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  MbMeshSmoothingParams( MethodSmoothMesh           method, 
                         double                     lambda, 
                         double                     mu, 
                         size_t                     maxIter, 
                         bool                       bFixBoundaries,
                         const c3d::IndicesVector & facets, 
                         const c3d::IndicesVector & fixVertices )
    : _method         ( method         )
    , _lambda         ( lambda         )
    , _mu             ( mu             )
    , _maxIter        ( maxIter        )
    , _bFixBoundaries ( bFixBoundaries )
    , _facets         ( facets         )
    , _fixVertices    ( fixVertices    )
  {}
  /// \ru Деструктор. \en Destructor.
  ~MbMeshSmoothingParams() {}

public:
  // \ru Установить метод сглаживания. \en Set the smoothing method. \~
  void SetMethod( MethodSmoothMesh method ) { _method = method; }
  // \ru Установить коэффициент сжатия. \en Set the compression factor. \~
  void SetLambda( double lambda ) { _lambda = lambda; }
  // \ru Установить коэффициент расширения. \en Set the expansion factor. \~
  void SetMu( double mu ) { _mu = mu; }
  // \ru Установить максимальное число итераций. \en Set the maximum iteration number. \~
  void SetMaxIter( size_t maxIter ) { _maxIter = maxIter; }
  // \ru Установить флаг сохранения границ. \en Set the save boundaries flag. \~
  void SetFixBoundaries( bool bFixBoundaries ) { _bFixBoundaries = bFixBoundaries; }
  // \ru Установить набор индексов фасетов для сглаживания. \en Set facet indices to smooth. \~
  void SetFacets( const c3d::IndicesVector & facets ) { _facets = facets; }
  // \ru Установить набор фиксированных вершин. \en Set the fixed vertex indices. \~
  void SetFixVertices( const c3d::IndicesVector & fixVertices ) { _fixVertices = fixVertices; }
  // \ru Получить метод сглаживания. \en Get the the smoothing method. \~
  MethodSmoothMesh GetMethod() const { return _method; }
  // \ru Получить коэффициент сжатия. \en Get the the compression factor. \~
  double GetLambda() const { return _lambda; }
  // \ru Получить коэффициент расширения. \en Get the expansion factor. \~
  double GetMu() const { return _mu; }
  // \ru Получить максимальное число итераций. \en Get the maximum iteration number. \~
  size_t GetMaxIter() const { return _maxIter; }
  // \ru Получить флаг сохранения границ. \en Get the save boundaries flags. \~
  bool GetFixBoundaries() const { return _bFixBoundaries; }
  // \ru Получить набор индексов фасетов для сглаживания. \en Get facet indices to smooth. \~
  const c3d::IndicesVector & GetFacets() const { return _facets; }
  // \ru Получить набор фиксированных вершин. \en Get the fixed vertex indices. \~
  const c3d::IndicesVector & GetFixVertices() const { return _fixVertices; }

  OBVIOUS_PRIVATE_COPY( MbMeshSmoothingParams );
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры проецирования полилинии на полигональный объект.
           \en Parameters for projecting a polyline on a polymesh. \~
  \details \ru Параметры проецирования полилинии на полигональный объект. \n
               Параметры содержат проецируемую полилинию.
           \en Parameters for projecting a polyline on a polymesh. \n
               Parameters contain a polyline to be projected. \~
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
class MATH_CLASS MbPolylinePolymeshProjectParams : public MbPrecision
{
private:
  const MbPolyline3D & _polyline;  ///< \ru Полилиния. \en Polyline. \~

public:
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  MbPolylinePolymeshProjectParams( const MbPolyline3D & polyline )
    : MbPrecision(          )
    , _polyline  ( polyline )
  {}
  /// \ru Деструктор. \en Destructor.
  ~MbPolylinePolymeshProjectParams() {}

public:
  // \ru Получить полилинию. \en Get the polyline. \~
  const MbPolyline3D & GetPolyline() const { return _polyline; }

  OBVIOUS_PRIVATE_COPY( MbPolylinePolymeshProjectParams );
};


//------------------------------------------------------------------------------
/** \brief \ru Результаты проецирования полилинии на полигональный объект.
           \en Results of projecting a polyline on a polymesh. \~
  \details \ru Результаты проецирования полилинии на полигональный объект. \n
               Результаты содержат массив цепочек пар рёбер вдоль проекции.
           \en Results of projecting a polyline on a polymesh. \n
               Results contain paths of edge pairs along the projection. \~
  \ingroup Polygonal_Objects
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
class MATH_CLASS MbPolylinePolymeshProjectResults
{
private:
  std::vector<c3d::IndicesPairsVector> _paths; ///< \ru Цепочки пар рёбер. \en Paths of edge pairs. \~

public:
  /// \ru Конструктор. \en Constructor.
  MbPolylinePolymeshProjectResults() : _paths() {}
  /// \ru Деструктор. \en Destructor.
  ~MbPolylinePolymeshProjectResults() {}

public:
  // \ru Получить набор цепочек рёбер. \en Get the paths of edge pairs. \~
  const std::vector<c3d::IndicesPairsVector> & GetEdgePairPaths() const { return _paths; }
  // \ru Получить набор цепочек рёбер. \en Get the paths of edge pairs. \~
  std::vector<c3d::IndicesPairsVector> & GetEdgePairPaths() { return _paths; }

  OBVIOUS_PRIVATE_COPY( MbPolylinePolymeshProjectResults );
};


//------------------------------------------------------------------------------
  /** \brief \ru Перечисление для способа обработки результатов проецирования и других операций.
             \en Enumeration for projection results processing method and other operations. \~
  */
  // ---
enum class ProjProcessingMethod : uint
{
  insert,            ///< \ru Просто врезать в сетку. \en Just embed into a mesh.
  splitSegmentation, ///< \ru Разбить сегментацию. \en Split a segmentation.
  keepOuterPart,     ///< \ru Оставить в сетке только внешнюю часть контура. \en Keep only outer part of contour.
  keepInnerPart,     ///< \ru Оставить в сетке только внутреннюю часть контура. \en Keep only inner part of a contour.
  insertCheckOpen    ///< \ru Врезать в сетку с контролем открытых границ. \en Embed into mesh with open boundary control.
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры обрезки полигонального объекта набором кривых.
           \en Parameters for cutting a polymesh with a set of curves. \~
  \details \ru Параметры обрезки полигонального объекта набором кривых. \n
               Сначала кривые линеаризуются с указанной точностью. Далее полученные
               полилинии проецируются на сетку. Результат проецирования обрабатывается
               указанным способом. \n
               1. #insert - в сетку врезаются новые ребра; \n
               2. #splitSegmentation - в сетку врезаются новые ребра, по ним производится разбивка сегментации; \n
               3. #keepOuterPart - в сетку врезаются новые ребра, далее удаляется часть сетки внутри контура; \n
               4. #keepInnerPart - в сетку врезаются новые ребра, далее удаляется часть сетки снаружи контура; \n
               5. #insertCheckOpen - в сетку врезаются новые ребра, при этом кривые, оказавшиеся внутри сетки, продлеваются
                  до открытой границы, а выступающие за границу - обрезаются этой границей. \n
           \en Parameters for cutting a polymesh with a set of curves. \n
               First, curves are linearized with a given tolerance. Then the resulting
               polylines are projected on a mesh. The projection results are processed
               with a given method. \n
               1. #insert - embed new edges along curves; \n
               2. #splitSegmentation - embed new edges along curves with segmentation; \n
               3. #keepOuterPart - embed new edges along curves and remove inner area; \n
               4. #keepInnerPart - embed new edges along curves and remove outer area; \n
               5. #insertCheckOpen - trim or extend curves to open boundaries and then embed new edges along these curves. \n \~
  \ingroup Polygonal_Objects
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
class MATH_CLASS MbCurvePolymeshProjectParams : public MbPrecision
{
protected:
  c3d::ConstSpaceCurvesSPtrVector   _curves;   ///< \ru Кривые. \en Curves. \~
  std::vector<ProjProcessingMethod> _types;    ///< \ru Способ обработки для каждой кривой. \en Processing method for an each curve. \~
  MbStepData                        _stepData; ///< \ru Данные для расчета шага. \en Data for the step calculation. \~

public:
  /// \ru Конструктор. \en Constructor.
  MbCurvePolymeshProjectParams();
  /// \ru Деструктор. \en Destructor.
  ~MbCurvePolymeshProjectParams();

public:
  // \ru Получить кривую по индексу. \en Get the curve by index. \~
  const c3d::ConstSpaceCurveSPtr & GetCurve( size_t iCrv ) const { return _curves[iCrv]; }
  // \ru Получить способ обработки кривой по индексу. \en Get the curve processing method by index. \~
  ProjProcessingMethod GetProcessingType( size_t iCrv ) const { return _types[iCrv]; }
  // \ru Получить количество кривых. \en Get the curves count. \~
  size_t CurveCount() const { return _curves.size(); }
  // \ru Получить данные для расчета шага. \en Get data for the step calculation. \~
  const MbStepData & GetStepData() const { return _stepData; }
  // \ru Добавить кривую. \en Add the curve. \~
  void AddCurve( const c3d::ConstSpaceCurveSPtr & crv, ProjProcessingMethod type );
  // \ru Установить данные для расчета шага. \en Set data for the step calculation. \~
  void SetStepData( const MbStepData & data ) { _stepData = data; }

  OBVIOUS_PRIVATE_COPY( MbCurvePolymeshProjectParams );
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры оффсета цепочки ребер полигонального объекта.
           \en Parameters for polymesh edge pathes offsetting. \~
  \details \ru Параметры оффсета цепочки ребер полигонального объекта. \n
           \en Parameters for polymesh edge pathes offsetting.  \~
  \ingroup Polygonal_Objects
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
class MATH_CLASS MbPathPolymeshOffsetParams
{
public:
  //------------------------------------------------------------------------------
  /** \brief \ru Перечисление для способа обработки концов незамкнутых оффсетированных кривых.
             \en Enumeration for ends processing method for open (non-closed) offset curves. \~
  */
  // ---
  enum class EndProcessingMethod
  {
    doNothing,      ///< \ru Оставить как есть. \en Do nothing.
    growToBoundary, ///< \ru Дорастить до открытой границы. \en Grow to the open mesh boundary.
    connect,        ///< \ru Соединить отрезком. \en Connect by the straight line.
    round           ///< \ru Скруглить. \en Make rounded.
  };

private:
  std::vector<c3d::IndicesPairsVector> _paths;        ///< \ru Цепочки пар рёбер. \en Paths of edge pairs. \~
  c3d::DoubleVector                    _offsetsLeft;  ///< \ru Величина оффсета влево. \en Left offset value. \~
  c3d::DoubleVector                    _offsetsRight; ///< \ru Величина оффсета вправо. \en Right offset value. \~
  std::vector<EndProcessingMethod>     _typesBeg;     ///< \ru Способ обработки начал. \en Beginning processing method. \~
  std::vector<EndProcessingMethod>     _typesEnd;     ///< \ru Способ обработки концов. \en End processing method. \~
  std::vector<ProjProcessingMethod>    _types;        ///< \ru Способ обработки для каждой цепочки. \en Processing method for an each path. \~

public:
  /// \ru Конструктор. \en Constructor.
  MbPathPolymeshOffsetParams();
  /// \ru Деструктор. \en Destructor.
  ~MbPathPolymeshOffsetParams();

  // \ru Получить количество цепочек. \en Get the path count. \~
  size_t PathCount() const { return _paths.size(); }
  // \ru Получить цепочку. \en Get the path. \~
  const c3d::IndicesPairsVector & GetEdgePairPath( size_t iPath ) const { return _paths[iPath]; }
  // \ru Получить величину оффсета влево. \en Get the left offset value. \~
  double GetOffsetLeft( size_t iPath ) const { return _offsetsLeft[iPath]; }
  // \ru Получить величину оффсета вправо. \en Get the right offset value. \~
  double GetOffsetRight( size_t iPath ) const { return _offsetsRight[iPath]; }
  // \ru Получить способ обработки концов. \en Get the end processing method. \~
  EndProcessingMethod GetEndProcessingMethod( size_t iPath, bool bEnd ) const { return bEnd ? _typesEnd[iPath] : _typesBeg[iPath]; }
  // \ru Получить способ обработки цепочки. \en Get the path processing method. \~
  ProjProcessingMethod GetProjProcessingMethod( size_t iPath ) const { return _types[iPath]; }
  /** \brief \ru Добавить цепочку.
             \en Add a path. \~
    \details \ru Добавить цепочку.
             \en Add a path. \~
    \param[in] path - \ru Цепочка. \n
                      \en Path. \~
    \param[in] offsetLeft - \ru Величина оффсета влево. \n
                           \en Left offset value. \~
    \param[in] offsetRight - \ru Величина оффсета вправо. \n
                             \en Right offset value. \~
    \param[in] typeBeg - \ru Способ обработки начала. \n
                         \en Method for the offset beginning processing. \~
    \param[in] typeEnd - \ru Способ обработки конца. \n
                         \en Method for the offset end processing. \~
    \param[in] method - \ru Способ обработки результата оффсета. \n
                        \en Method for the offset results processing. \~
  */
  void AddPath( const c3d::IndicesPairsVector & path, double offsetLeft, double offsetRight, EndProcessingMethod typeBeg, EndProcessingMethod typeEnd, ProjProcessingMethod method );

  // \ru Получить все цепочки. \en Get all pathes. \~
  const std::vector<c3d::IndicesPairsVector> & GetEdgePairPathes() const { return _paths; }

  OBVIOUS_PRIVATE_COPY( MbPathPolymeshOffsetParams );
};


//------------------------------------------------------------------------------
/** \brief \ru Результаты обрезки полигонального объекта набором кривых.
           \en Results of cutting a polymesh with a set of curves. \~
  \details \ru Результаты обрезки полигонального объекта набором кривых. \n
               Результаты содержат массивы цепочек пар рёбер вдоль проекции каждой кривой
               и флаги пересечения и самопересечения.
           \en Results of cutting a polymesh with a set of curves. \n
               Results contain paths of edge pairs along the projection of an each curve,
               and intersection and self-intersection flags. \~
  \ingroup Polygonal_Objects
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
class MATH_CLASS MbPolymeshProjectResults
{
private:
  std::vector<std::vector<c3d::IndicesPairsVector>> _paths;             //< \ru Цепочки пар рёбер. \en Paths of edge pairs. \~
  bool                                              _bIntersection;     ///< \ru Флаг наличия пересечений. \en An intersections flag. \~
  bool                                              _bSelfIntersection; ///< \ru Флаг наличия самопересечений. \en A self-intersections flag. \~

public:
  /// \ru Конструктор. \en Constructor.
  MbPolymeshProjectResults();
  /// \ru Деструктор. \en Destructor.
  ~MbPolymeshProjectResults();

public:
  // \ru Получить количество кривых. \en Get the curves count. \~
  size_t CurveCount() const { return _paths.size(); }
  // \ru Получить количество цепочек для кривой. \en Get the pathes count for a curve. \~
  size_t CurvePathCount( size_t iCrv ) const { return _paths[iCrv].size(); }
  // \ru Получить цепочку для кривой. \en Get the path for a curve. \~
  const c3d::IndicesPairsVector & GetEdgePairPath( size_t iCrv, size_t iPath ) const { return _paths[iCrv][iPath]; }
  // \ru Получить цепочку для кривой. \en Get the path for a curve. \~
  c3d::IndicesPairsVector & GetEdgePairPath( size_t iCrv, size_t iPath ) { return _paths[iCrv][iPath]; }
  // \ru Проверить наличие пересечений. \en Check for intersections. \~
  bool HasIntersections() const { return _bIntersection; }
  // \ru Проверить наличие самопересечений. \en Check for self-intersections. \~
  bool HasSelfIntersections() const { return _bSelfIntersection; }

  // \ru Инициализировать данные. \en Initialize data.
  void Init( const std::vector<std::vector<c3d::IndicesPairsVector>> & paths, bool bIntersect, bool bSelfintersect );

  OBVIOUS_PRIVATE_COPY( MbPolymeshProjectResults );
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры заполнения отверстия, заданного замкнутой кривой.
           \en Parameters for filling a hole defined by a closed curve. \~
  \details \ru Параметры заполнения отверстия, заданного замкнутой кривой. \n
               Параметры содержат точки на границе отверстия.
           \en Parameters for filling a hole defined by a closed curve. \n
               Parameters contain points on the hole's boundary. \~
  \ingroup Polygonal_Objects
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
class MATH_CLASS MbFillClosedCurveParams
{
private:
  c3d::SpacePointsVector _points; ///< \ru Точки на границе отверстия. \en Points on the hole's boundary. \~

public:
  /// \ru Конструктор по кривой. \en Constructor by a curve.
  MbFillClosedCurveParams( const MbPolyline3D & polyline );
  /// \ru Конструктор по точкам. \en Constructor by points.
  MbFillClosedCurveParams( const c3d::SpacePointsVector & points );
  /// \ru Деструктор. \en Destructor.
  ~MbFillClosedCurveParams() {}

public:
  // \ru Получить точки на границе отверстия. \en Get the points on the hole's boundary. \~
  const c3d::SpacePointsVector & GetPoints() const { return _points; }

  OBVIOUS_PRIVATE_COPY( MbFillClosedCurveParams );
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры заполнения отверстия полигонального объекта.
           \en Parameters for filling a polygonal object hole. \~
  \details \ru Параметры заполнения отверстия полигонального объекта. \n
               Параметры содержат индексы рёбер на границе отверстия.
           \en Parameters for filling a polygonal object hole. \n
               Parameters contain the indices of the edges from the hole's boundary. \~
  \ingroup Polygonal_Objects
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
class MATH_CLASS MbFillPolymeshHoleParams
{
public:
  //------------------------------------------------------------------------------
  /** \brief \ru Перечисление методов заполнения отверстия.
           \en Enumeration for filling methods. \~
  */
  // ---
  enum class MethodFillPolymeshHole
  {
    full,    ///< \ru Полный. \en Full.
    partial, ///< \ru Частичный. \en Partial.
  };

private:
  MethodFillPolymeshHole _method;   ///< \ru Метод заполнения отверстия. \en The filling method. \~
  c3d::IndicesVector     _boundary; ///< \ru Список ребер вдоль отверстия. \en The list of edges along the hole's boundary. \~

public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  MbFillPolymeshHoleParams() 
    : _boundary(                              ) 
    , _method  ( MethodFillPolymeshHole::full )
  {}
  /// \ru Деструктор. \en Destructor.
  ~MbFillPolymeshHoleParams() {}

public:
  /// \ru Инициализировать ребром топологии. \en Initialize by the topology edge.
  bool InitializeByTopoEdge( const MbPolymesh & mesh, size_t iEdTopo );
  /** \brief \ru Инициализировать частичное закрытие начальным и конечным ребрами топологии.
             \en Initialize partial filling with the initial and final topology edges. \~
      \param[in] mesh - \ru Полигональный объект.
                      \en A polygonal object. \~
      \param[in] iEdTopoBegin  - \ru Начальное открытое ребро сетки.
                              \en An initial open topology edge. \~
      \param[in] iEdTopoEnd  - \ru Конечное открытое ребро сетки.
                            \en A final open topology edge. \~
  */
  bool InitializePartialByTwoTopoEdges( const MbPolymesh & mesh, size_t iEdTopoBegin, size_t iEdTopoEnd );
  /// \ru Инициализировать ребром сегментации. \en Initialize by the segmentation edge.
  bool InitializeBySegmEdge( const MbPolymesh & mesh, size_t iEdSegm );
  /// \ru Получить границу отверстия. \en Get the hole's boundary. \~
  const c3d::IndicesVector & GetBoundary() const { return _boundary; }
  /// \ru Получить метод заполнения отверстия. \en Get the filling method. \~
  MethodFillPolymeshHole GetMethod() const { return _method; }

  OBVIOUS_PRIVATE_COPY( MbFillPolymeshHoleParams );
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры расчета нормалей в вершинах полигонального объекта.
           \en Parameters for calculation of normal vectors at the vertices of a polygonal object. \~
  \details \ru Параметры расчета нормалей в вершинах полигонального объекта. \n
           \en Parameters for calculation of normal vectors at the vertices of a polygonal object. \~
  \ingroup Polygonal_Objects
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
class MATH_CLASS MbPolymeshNormalsParams
{
private:
  bool _bUseSegmentation; ///< \ru Учет сегментации. \en Use segmentation data. \~

public:
  /// \ru Конструктор. \en Constructor.
  MbPolymeshNormalsParams( bool bUseSegmentation )
    : _bUseSegmentation( bUseSegmentation )
  {}
  /// \ru Деструктор. \en Destructor.
  ~MbPolymeshNormalsParams() {}

public:
  // \ru Получить флаг учета сегментации. \en Segmentation usage flag. \~
  bool GetUseSegmentation() const { return _bUseSegmentation; }

  OBVIOUS_PRIVATE_COPY( MbPolymeshNormalsParams );
};


//------------------------------------------------------------------------------
/** \brief \ru Критерий сегментации полигонального объекта.
           \en A criterion for segmenting a polygonal object. \~
  \ingroup Polygonal_Objects
*/
// ---
enum class MbeSegmentationCriterion
{
  openBoundary = 0, ///< \ru Критерий открытой границы. \en Open boundary criterion.
  sharpEdges   = 1, ///< \ru Критерий острых рёбер. \en Sharp edges criterion.
  curvature     = 2 ///< \ru Критерий кривизны. \en Curvature criterion.
};
//------------------------------------------------------------------------------
/** \brief \ru Параметры сегментации полигонального объекта.
           \en Parameters for segmenting a polygonal object. \~
  \details \ru Параметры сегментации полигонального объекта.
               Есть возможность сегментировать по открытым границам, по острым сломам, по кривизне.
           \en Parameters for segmenting a polygonal object.
               It can be segmented based on open boundaries, on sharp edges, on curvature. \~
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
class MATH_CLASS MbSegmentationParams
{
private:
  MbeSegmentationCriterion _criterion;    ///< \ru Критерий сегментации. \en Segmentation criterion. \~
  double                   _angleSharp;   ///< \ru Порог угла для острых рёбер (в градусах). \en The angle threshold for sharp edges (in degrees). \~
  double                   _curvMaxValue; ///< \ru Значение порога кривизны. \en The curvature threshold value. \~
  MbeCurvatureType         _curvType;     ///< \ru Тип порога кривизны. \en The curvature threshold type. \~
  c3d::IndicesVector       _regions;      ///< \ru Выбранные для сегментации регионы. \en The regions to be segmented. \~
  bool                     _bKeep;        ///< \ru Сохранять имеющуюся сегментацию (при наличии). \en Keep the existing segmentation (if exists). \~

public:
  /// \ru Конструктор. \en Constructor.
  MbSegmentationParams()
    : _criterion   ( MbeSegmentationCriterion::openBoundary )
    , _angleSharp  ( MB_MAXDOUBLE                           )
    , _curvMaxValue( MB_MAXDOUBLE                           )
    , _curvType    ( MbeCurvatureType::mean                 )
    , _regions     (                                        )
    , _bKeep       ( true                                   )
  {
  }
  /// \ru Деструктор. \en Destructor.
  ~MbSegmentationParams() {}

public:
  /// \ru Инициализировать параметры сегментации по острым углам (в градусах). \en Initialize the parameters with sharp edges (in degrees). \~
  void InitializeWithSharpEdges( double angleSharp )
  {
    _criterion = MbeSegmentationCriterion::sharpEdges;
    _angleSharp = angleSharp;
  }
  /// \ru Инициализировать параметры сегментации по кривизне. \en Initialize the parameters with a curvature value. \~
  void InitializeWithCurvature( double curvMaxValue, MbeCurvatureType curvType )
  {
    _criterion = MbeSegmentationCriterion::curvature;
    _curvMaxValue = curvMaxValue;
    _curvType = curvType;
  }
  /// \ru Получить критерий сегментации. \en Get the segmentation criterion. \~
  MbeSegmentationCriterion GetCriterion() const { return _criterion; }
  /// \ru Получить порог угла для острых рёбер (в градусах). \en Get the angle threshold for sharp edges (in degrees). \~
  double GetAngleSharp() const { return _angleSharp; }
  /// \ru Получить порог кривизны. \en Get the curvature threshold value. \~
  double GetMaxCurvatureValue() const { return _curvMaxValue; }
  /// \ru Получить тип порога кривизны. \en Get the curvature threshold type. \~
  MbeCurvatureType GetCurvatureType() const { return _curvType; }
  /// \ru Получить выбранные регионы для редактирования. \en Get the selected regions for editing. \~
  c3d::IndicesVector & GetSelectedRegions() { return _regions; }
  /// \ru Получить выбранные регионы. \en Get the selected regions. \~
  const c3d::IndicesVector & GetSelectedRegions() const { return _regions; }
  /// \ru Установить флаг сохранения имеющейся сегментации. \en Set the flag of keeping the existing segmentation. \~
  void SetKeepExisting( bool bKeep ) { _bKeep = bKeep; }
  /// \ru Получить флаг сохранения имеющейся сегментации. \en Get the flag of keeping the existing segmentation. \~
  bool GetKeepExisting() const { return _bKeep; }
  /// \ru Установить параметры по умолчанию. \en Reset parameters. \~
  void ResetParameters()
  {
    _criterion = MbeSegmentationCriterion::openBoundary;
    _angleSharp = MB_MAXDOUBLE;
    _curvMaxValue = MB_MAXDOUBLE;
    _curvType = MbeCurvatureType::mean;
    _regions.clear();
    _bKeep = true;
  }

  OBVIOUS_PRIVATE_COPY( MbSegmentationParams );
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры лечения сегментации полигонального объекта.
           \en Parameters for repairing the segmentation of a polygonal object. \~
  \details \ru Параметры лечения сегментации полигонального объекта.
               После сегментирования полигонального объекта вероятно появление мелких регионов.
               Их можно устранить за счет объединения с соседними регионами.
               Также есть возможность удалить внутренние рёбра сегментации.
           \en Parameters for repairing the segmentation of a polygonal object.
               Segmenting can generate small regions - they can be fixed through merging with neighboring ones.
               Is it also possible to remove inner segmentation edges. \~
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
class MATH_CLASS MbRepairSegmentationParams
{
private:
  bool   _bFixSmall;     ///< \ru Устранить мелкие регионы. \en Fix small regions. \~
  double _areaThreshold; ///< \ru Порог площади региона. \en Region area threshold. \~
  bool   _bRemoveInner;  ///< \ru Удалить внутренние рёбра. \en Remove inner edges. \~

public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  MbRepairSegmentationParams()
    : _bFixSmall    ( true )
    , _areaThreshold( -1.  )
    , _bRemoveInner ( true )
  {
  }
  /// \ru Конструктор с параметрами. \en Constructor with parameters.
  MbRepairSegmentationParams( bool bFixSmallRegions, double areaThreshold, bool bRemoveInnerEdges )
    : _bFixSmall    ( bFixSmallRegions  )
    , _areaThreshold( areaThreshold     )
    , _bRemoveInner ( bRemoveInnerEdges )
  {
  }
  /// \ru Деструктор. \en Destructor.
  ~MbRepairSegmentationParams() {}

public:
  /// \ru Установить флаг устранения мелких регионов. \en Set the flag of fixing small regions. \~
  void SetFixSmallRegions( bool bFixSmallRegions ) { _bFixSmall = bFixSmallRegions; }
  /// \ru Получить флаг устранения мелких регионов. \en Get the flag of fixing small regions. \~
  bool GetFixSmallRegions() const { return _bFixSmall; }
  /// \ru Установить порог площади региона. \en Set region area threshold. \~
  void SetAreaThreshold( double areaThreshold ) { _areaThreshold = areaThreshold; }
  /// \ru Получить порог площади региона. \en Get region area threshold. \~
  double GetAreaThreshold() const { return _areaThreshold; }
  /// \ru Установить флаг удаления внутренних рёбер. \en Set the flag of removing inner edges. \~
  void SetRemoveInnerEdges( bool bRemoveInnerEdges ) { _bRemoveInner = bRemoveInnerEdges; }
  /// \ru Получить флаг удаления внутренних рёбер. \en Get the flag of removing inner edges. \~
  bool GetRemoveInnerEdges() const { return _bRemoveInner; }  

  OBVIOUS_PRIVATE_COPY( MbRepairSegmentationParams );
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры булевой операции для двух полигональных объектов.
           \en Parameters of a boolean operation with two polygonal objects. \~
  \details \ru Параметры булевой операции для двух полигональных объектов. \n
           \en Parameters of a boolean operation with two polygonal objects. \~
  \ingroup Polygonal_Objects
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
class MATH_CLASS MbPolymeshBooleanParams : public MbPrecision
{
public:
  //------------------------------------------------------------------------------
  /** \brief \ru Перечисление типов булевой операции.
             \en Enumeration for boolean operation type. \~
  */
  // ---
  enum class OpType
  {
    opUnion,        ///< \ru Объединение. \en Union. \~
    opSubtraction,  ///< \ru Вычитание. \en Subtraction. \~
    opIntersection, ///< \ru Пересечение. \en Intersection. \~
    opNonManifold,  ///< \ru Неманифолдная сетка. \en Non-manifold mesh. \~
    opImprint       ///< \ru Врезать линию пересечения с другим объектом. \en Insert intersection line with the other object. \~
  };

private:
  OpType _type; ///< \ru Тип булевой операции. \en Boolean operation type. \~

public:
  /// \ru Конструктор. \en Constructor.
  MbPolymeshBooleanParams( OpType type, double tolerance );
  /// \ru Деструктор. \en Destructor.
  ~MbPolymeshBooleanParams() {}

  /// \ru Получить тип булевой операции. \en Get boolean operation type. \~
  OpType GetOperationType() const { return _type; }

  OBVIOUS_PRIVATE_COPY( MbPolymeshBooleanParams );
};


//------------------------------------------------------------------------------
/** \brief \ru Результаты булевой операции для двух полигональных объектов.
           \en Results of a boolean operation with two polygonal objects. \~
  \details \ru Результаты булевой операции для двух полигональных объектов. \n
           \en Results of a boolean operation with two polygonal objects. \~
  \ingroup Polygonal_Objects
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
class MATH_CLASS MbPolymeshBooleanResults
{
private:
  c3d::IndicesVector                   _changedSubregions; ///< \ru Индексы измененных подрегионов. \en Changed subregions indicies. \~
  c3d::IndicesVector                   _addedSubregions;   ///< \ru Индексы добавленных подрегионов. \en Added subregions indicies. \~
  c3d::IndicesVector                   _deletedSubregions; ///< \ru Индексы удаленных подрегионов. \en Deleted subregions indicies. \~
  std::vector<c3d::IndicesPairsVector> _insertedEdges;     ///< \ru Индексы ребер вставленных вдоль линии пересечения, заполняется только для операции opImprint.
                                                           ///< \en Indices for edges inserted along intersection, used for opImprint operation only. \~

public:
  /// \ru Конструктор. \en Constructor.
  MbPolymeshBooleanResults() {}
  /// \ru Деструктор. \en Destructor.
  ~MbPolymeshBooleanResults() {}

  // \ru Инициализировать данные. \en Initialize data.
  void Init( const c3d::IndicesVector & changed, const c3d::IndicesVector & added, const c3d::IndicesVector & deleted );
  void Init( std::vector<c3d::IndicesPairsVector>&& insertedEdges );
  // \ru Получить индексы измененных подрегионов. \en Get changed subregions indicies.
  const c3d::IndicesVector & GetChangedSubregions() const { return _changedSubregions; }
  // \ru Получить индексы добавленных подрегионов. \en Get added subregions indicies.
  const c3d::IndicesVector & GetAddedSubregions() const { return _addedSubregions; }
  // \ru Получить индексы удаленных подрегионов. \en Get deleted subregions indicies.
  const c3d::IndicesVector & GetDeletedSubregions() const { return _deletedSubregions; }
  // \ru Получить ребра вдоль линии разреза. \en Get cutting line edges indices.
  const std::vector<c3d::IndicesPairsVector> & GetInsetedEdges() const { return _insertedEdges; }

  OBVIOUS_PRIVATE_COPY( MbPolymeshBooleanResults );
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры создания оттиска на полигональном объекте.
           \en Parameters of imprinting a 2D line or a plane on a polygonal object. \~
  \details \ru Параметры создания оттиска на полигональном объекте.
               Можно создать оттиск прямой, плоскости или набора точек.
               Прямая задаётся 2D точкой и направлением в домене параметров.
           \en Parameters of imprinting a 2D line or a plane on a polygonal object.
               A line, a plane or a point array can be imprinted on a polygonal object.
               A line is given by a 2D point and a 2D direction in the parameters domain. \~
  \ingroup Polygonal_Objects
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
class MATH_CLASS MbPolymeshImprintParams : public MbPrecision
{
private:
  //------------------------------------------------------------------------------
  /** \brief \ru Объект для оттиска.
             \en Imprinting object. \~
  */
  // ---
  enum class ImprintingObject
  {
    plane,  ///< \ru Плоскость. \en Plane. \~
    line2d, ///< \ru Прямая в домене параметров. \en Line in the parameters domain. \~
    points  ///< \ru Неупорядоченный набор точек. \en Unordered point array. \~
  };

private:
  ImprintingObject       _object;       ///< \ru Объект для оттиска. \en Imprinting object. \~
  MbPlacement3D          _plane;        ///< \ru Плоскость. \en Plane. \~
  c3d::SpacePointsVector _points;       ///< \ru Набор точек. \en Point array. \~
  c3d::IndicesVector     _idsOnBound;   ///< \ru Индексы точек, которые нужно спроецировать на границу. \en Indices of points that should be imprinted on boundaries.
  MbCartPoint            _lineOrg;      ///< \ru Точка на прямой. \en Line point. \~
  MbVector               _lineDir;      ///< \ru Направление прямой. \en Line direction. \~
  bool                   _bTriangulate; ///< \ru Триангулировать ли разрезанные фасеты. \en Should cut facets be triangulated.
  VERSION                _version;      ///< \ru Версия исполнения. \en The version of execution. 

public:
  /** \brief \ru Конструктор для оттиска плоскости.
             \en Constructor for imprinting a plane. \~
      \param[in] plane     - \ru Плоскость.
                             \en Plane. \~
      \param[in] precision - \ru Точность.
                             \en Precision. \~
      \param[in] ver       - \ru Версия исполнения.
                             \en The version of execution. \~
  */
  MbPolymeshImprintParams( const MbPlacement3D & plane, double precision, VERSION ver = Math::DefaultMathVersion() );
  /** \brief \ru Конструктор для оттиска прямой.
             \en Constructor for imprinting a line. \~
      \param[in] lineOrg   - \ru Точка на прямой.
                             \en Line point. \~
      \param[in] lineDir   - \ru Направление прямой.
                             \en Line direction. \~
      \param[in] precision - \ru Точность.
                             \en Precision. \~
      \param[in] ver       - \ru Версия исполнения.
                             \en The version of execution. \~
  */
  MbPolymeshImprintParams( const MbCartPoint & lineOrg, const MbVector & lineDir, double precision, VERSION ver = Math::DefaultMathVersion() );
  /** \brief \ru Конструктор для оттиска набора точек.
             \en Constructor for imprinting a point array. \~
      \param[in] points     - \ru Неупорядоченный набор точек.
                              \en Unordered point array. \~
      \param[in] idsOnBound - \ru Индексы точек, которые нужно спроецировать на границу.
                              \en Indices of points, that should be imprinted on boundaries. \~
      \param[in] precision  - \ru Точность.
                              \en Precision. \~
      \param[in] ver        - \ru Версия исполнения.
                              \en The version of execution. \~
  */
  MbPolymeshImprintParams( const c3d::SpacePointsVector & points, const c3d::IndicesVector & idsOnBound, double precision, VERSION ver = Math::DefaultMathVersion() );
  /// \ru Деструктор. \en Destructor.
  ~MbPolymeshImprintParams() {}

public:
  /// \ru Получить объект для оттиска. \en Get the imprinting object. \~
  ImprintingObject GetImprintingObject() const { return _object; }
  ///< \ru Делать ли оттиск плоскости. \en Is imprinting a plane. \~
  bool IsImprintingPlane() const { return _object == ImprintingObject::plane; }
  ///< \ru Делать ли оттиск прямой. \en Is imprinting a line. \~
  bool IsImprintingLine2d() const { return _object == ImprintingObject::line2d; }
  ///< \ru Делать ли оттиск набора точек. \en Is imprinting a point array. \~
  bool IsImprintingPointArray() const { return _object == ImprintingObject::points; }
  ///< \ru Получить плоскость для оттиска. \en Get the imprinting plane. \~
  const MbPlacement3D & GetImprintingPlane() const { return _plane; }
  ///< \ru Получить точку на прямой для оттиска. \en Get the point on the imprinting line. \~
  const MbCartPoint & GetImprintingLineOrg() const { return _lineOrg; }
  ///< \ru Получить направление прямой для оттиска. \en Get the direction of the imprinting line. \~
  const MbVector & GetImprintingLineDir() const { return _lineDir; }
  ///< \ru Получить набор точек для оттиска. \en Get the point array for imprinting. \~
  const c3d::SpacePointsVector & GetImprintingPointArray() const { return _points; }
  ///< \ru Получить индексы точек, которые нужно спроецировать на границу. \en Get the indices of points, that should be imprinted on boundaries. \~
  const c3d::IndicesVector & GetPointsToImprintOnBoundaries() const { return _idsOnBound; }
  ///< \ru Триангулировать ли разрезанные фасеты. \en Should cut facets be triangulated. \~
  bool IsTriangulationNeeded() const { return _bTriangulate; }
  ///< \ru Установить флаг триангуляции разрезанных фасетов. \en Set the flag of triangulating cut facets. \~
  void SetTriangulationFlag( bool bTriangulate ) { _bTriangulate = bTriangulate; }
  /// \ru Получить версию исполнения. \en Get the version of execution. 
  VERSION GetMathVersion() const { return _version; }

  OBVIOUS_PRIVATE_COPY( MbPolymeshImprintParams );
};


//------------------------------------------------------------------------------
/** \brief \ru Результаты создания оттиска на полигональном объекте.
           \en Results of imprinting on a polygonal object. \~
  \details \ru Результаты создания оттиска на полигональном объекте. \n
               Можно создать оттиск прямой, плоскости или набора точек.
               Результаты содержат массив цепочек пар рёбер на рассекающей геометрии (в случае оттиска прямой или плоскости) 
               или набор внедренных вершин (в случае оттиска набора точек).
           \en Results of imprinting on a polygonal object. \n
               A line, a plane or a point array can be imprinted on a polygonal object.
               Results contain paths of edge pairs on the intersection (in case of imprinting a line or a plane)
               or a vertex array (in case of imprinting a point array). \~
  \ingroup Polygonal_Objects
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
class MATH_CLASS MbPolymeshImprintResults
{
private:
  std::vector<c3d::IndicesPairsVector> _paths;    ///< \ru Цепочки пар рёбер. \en Paths of edge pairs. \~
  c3d::IndicesVector                   _vertices; ///< \ru Вершины. \en Vertices. \~

public:
  /// \ru Конструктор. \en Constructor.
  MbPolymeshImprintResults() : _paths() {}
  /// \ru Деструктор. \en Destructor.
  ~MbPolymeshImprintResults() {}

public:
  // \ru Получить набор цепочек рёбер. \en Get the paths of edge pairs. \~
  const std::vector<c3d::IndicesPairsVector> & GetEdgePairPaths() const { return _paths; }
  // \ru Получить внедренные вершины. \en Get the inserted vertices. \~
  const c3d::IndicesVector & GetInsertedVertices() const { return _vertices; }
  // \ru Инициализировать набором цепочек. \en Initialize by the paths of edge pairs. \~
  void InitializeByEdgePairPaths( const std::vector<c3d::IndicesPairsVector> & paths ) { _paths = paths; }
  // \ru Инициализировать набором внедренных вершин. \en Initialize by the inserted vertices. \~
  void InitializeByInsertedVertices( const c3d::IndicesVector & vertices ) { _vertices = vertices; }

  OBVIOUS_PRIVATE_COPY( MbPolymeshImprintResults );
};

#endif // __OP_POLYMESH_PARAMETERS_H
