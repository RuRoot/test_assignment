////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Линия очерка или силуэтная кривая поверхности.
         \en Isocline curve or silhouette curve of surface. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CUR_SILHOUETTE_CURVE_H
#define __CUR_SILHOUETTE_CURVE_H


#include <cur_surface_curve.h>



//------------------------------------------------------------------------------
/** \brief \ru Линия очерка или силуэтная кривая поверхности.
           \en Isocline curve or silhouette curve of surface. \~
  \details \ru Линии, отделяющие видимую часть поверхности от невидимой её части называются силуэтными линиями.
    Силуэтные линии могут проходить как по краю поверхности, так и внутри неё. 
    В последнем случае силуэтные линии называют линиями очерка.
    При переходе через линию очерка нормаль поверхности меняет направление по отношению к линии визирования. \n
    Линией визирования называется линия, проходящая через точку наблюдения и точку поверхности. 
    В точках линии очерка нормаль поверхности ортогональна линии визирования. 
    В общем случае у поверхности линий очерка может быть несколько. \n 
    Если параметр species==cbt_Ordinary, то кривая является точной. \n
    Если параметр species==cbt_Specific, то кривая построена по упорядоченной совокупности 
    двумерных точек на параметрической плоскости поверхности.
    Эта совокупность точек представлена в виде сплайна на поверхности. 
    Сплайн при значениях параметра, соответствующих опорным точкам, совпадает с силуэтной линией, 
    при других значениях параметра он проходит вблизи силуэтной линии. 
    Для любого значения параметра сплайна точки силуэтной линии вычисляются с достаточной точностью
    из решения системы уравнений для силуэтной линии поверхности. \n
    Каждая линия очерка или замкнута, или оканчивается на краях поверхности. 
    Для разных направлений взгляда существует своя совокупность линий очерка, 
    поэтому при повороте поверхности линии очерка необходимо строить заново. \n
    Линия очерка используется для построения проекции поверхности. \n
    Линию очерка могут иметь поверхности, обладающие кривизной хотя бы вдоль одного параметра.
           \en Lines separating the visible part of a surface from its invisible part are called silhouette lines.
    Silhouette lines can pass both by the surface boundary and inside it. 
    In the last case silhouette lines are called isocline curves.
    The normal of a surface changes the direction relative to the line of sight while moving through isocline curve. \n
    The line passing through a point of a surface and the observation point is called the line of sight. 
    The normal of surface is orthogonal to the line of sight at points of isocline curve. 
    Generally there can be several isocline curves of surface. \n 
    If parameter 'species' is equal to cbt_Ordinary, then the curve is exact. \n
    If parameter 'species' is equal to cbt_Specific, then the curve is constructed by an ordered set of 
    two-dimensional points in parametric space of surface.
    This set of points is represented as spline on surface. 
    Spline coincides with the silhouette line at values of parameter corresponding to support points, 
    it passes near the silhouette line at other values of parameter. 
    For any parameter of a spline the points of silhouette line are calculated with enough tolerance 
    from solution of system of equations for silhouette line of surface. \n
    Each isocline curve is either closed or terminates at surface boundaries. 
    For the different directions of view there is a set of isocline curves, 
    therefore the isocline curves need to be rebuilt at surface rotation. \n
    Isocline curve is used for construction of surface projection. \n
    The surfaces having curvature along at least one parameter can have the isocline curve. \~
  \ingroup Curves_3D
*/
// ---
class MATH_CLASS MbSilhouetteCurve : public MbSurfaceCurve {

protected:
  MbeCurveBuildType species;     ///< \ru Вид кривой (точная полностью или только в отдельных точках). \en Curve type (exact completely or in separate points only). 
  bool              perspective; ///< \ru Перспективная или параллельная проекция. \en Perspective or parallel projection. 
  MbVector3D        eye;         ///< \ru Вектор взгляда (для параллельной проекции) или радиус-вектор точки наблюдения (для перспективной проекции). \en Vector of view (for parallel projection) or radius-vector of point of view (for perspective projection). 
  MbAxis3D *        lathe;       ///< \ru Ось кругового проецирования (проекции токарного сечения). \en The axis for rotate projection. 
  MbCurve3D *       approxCurve; ///< \ru Пространственное представление линии очерка. \en The spatial representation of isocline curve. 
  bool              approxExact; ///< \ru Точная ли кривая approxCurve. \en Is exact approxCurve. 
  
public :
  /// \ru Конструктор по поверхности, двумерной кривой, типу кривой, матрице и флагу перспективы. \en Constructor by surface, two-dimensional curve, type of curve, matrix and flag of perspective. 
  MbSilhouetteCurve( const MbSurface & surf, const MbCurve & crv, MbeCurveBuildType _species,
                     const MbMatrix3D & m, bool p );
  /// \ru Конструктор по поверхности, двумерной кривой, типу кривой, вектору взгляда и флагу перспективы. \en Constructor by surface, two-dimensional curve, type of curve, vector of view and flag of perspective. 
  MbSilhouetteCurve( const MbSurface & surf, const MbCurve & crv, MbeCurveBuildType _species,
                     const MbVector3D & e, bool p, const MbAxis3D * axis = nullptr );
protected:
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbSilhouetteCurve( const MbSilhouetteCurve &, MbRegDuplicate * );
private:
  MbSilhouetteCurve( const MbSilhouetteCurve & ); // \ru Не реализовано!!! \en Not implemented!!! 
public:
  virtual ~MbSilhouetteCurve();

public:  
  VISITING_CLASS( MbSilhouetteCurve );

  // \ru Общие функции геометрического объекта. \en Common functions of a geometric object.

  MbeSpaceType  IsA()  const override; // \ru Дать тип элемента. \en Get element type. 
  MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента. \en Create a copy of the element.
  bool        IsSame   ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const override; // \ru Определить, являются ли объекты одинаковыми. \en Determine whether objects are equal. 
  bool        SetEqual ( const MbSpaceItem & ) override; // \ru Сделать равным. \en Make equal. 
  bool        IsSimilar( const MbSpaceItem & ) const override; // \ru Определить, являются ли объекты подобными. \en Determine whether the objects are similar. 
  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать элемент согласно матрице. \en Transform element according to the matrix.
  void        Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override; // \ru Сдвинуть вдоль вектора. \en Move along a vector.
  void        Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override;  // \ru Повернуть вокруг оси. \en Rotate about an axis.
  void        Refresh() override; // \ru Сбросить все временные данные. \en Reset all temporary data. 
  void        GetProperties( MbProperties & ) override;  // \ru Выдать свойства объекта. \en Get properties of the object. 
  void        SetProperties( const MbProperties & ) override;  // \ru Записать свойства объекта. \en Set properties of the object. 

  // \ru Общие функции кривой. \en Common functions of curve.
  // \ru Функции для работы в области определения. \en Functions for working in the definition domain. 
  void        PointOn  ( double & t, MbCartPoint3D & ) const override; // \ru Вычислить точку на кривой. \en Calculate a point on the curve. 
  void        FirstDer ( double & t, MbVector3D & )    const override; // \ru Вычислить первую производную. \en Calculate the first derivative. 
  void        SecondDer( double & t, MbVector3D & )    const override; // \ru Вычислить вторую производную. \en Calculate the second derivative. 
  void        ThirdDer ( double & t, MbVector3D & )    const override; // \ru Вычислить третью производную по t. \en Calculate the third derivative by t. 
  // \ru Функции для работы вне области определения. \en Functions for working outside of definition domain. 
  void       _PointOn  ( double t, MbCartPoint3D & )  const override; // \ru Вычислить точку на расширенной кривой. \en Calculate a point on the extended curve. 
  void       _FirstDer ( double t, MbVector3D & )     const override; // \ru Вычислить первую производную. \en Calculate the first derivative. 
  void       _SecondDer( double t, MbVector3D & )     const override; // \ru Вычислить вторую производную. \en Calculate the second derivative. 
  void       _ThirdDer ( double t, MbVector3D & )     const override; // \ru Вычислить третью производную по t. \en Calculate the third derivative by t. 
  // \ru Вычислить значения точки и производных для заданного параметра. \en Calculate point and derivatives of object for given parameter. \~
  void        Explore( double & t, bool ext,
                               MbCartPoint3D & pnt, MbVector3D & fir, MbVector3D * sec, MbVector3D * thir ) const override;
  // \ru Функции приближённого быстрого вычисления точки и производных на кривой. \en Functions of approximate fast calculation of point and derivatives on the curve. 
  void        FastApproxExplore( double & t, MbCartPoint3D & pnt, MbVector3D & fir, MbVector3D * sec ) const override;

  // \ru Вычислить шаг параметра по заданному углу отклонения касательной. \en Calculate step of parameter by a given angle of deviation of tangent. 
  double      DeviationStep( double t, double angle ) const override;
  // \ru Вычислить габарит кривой. \en Calculate bounding box of a curve. 
  void        CalculateGabarit( MbCube & ) const override;
  // \ru Определить, является ли кривая плоской. \en Determine whether the curve is planar. 
  bool        IsPlanar( double accuracy = METRIC_EPSILON ) const override;
  // \ru Заполнить плейсмент, если кривая плоская. \en Fill the placement if the curve is planar. 
  bool        GetPlacement( MbPlacement3D & place, PlanarCheckParams params = PlanarCheckParams() ) const override;
  // \ru Дать плоскую кривую и плейсмент, если пространственная кривая плоская (после использования вызывать DeleteItem на двумерную кривую). \en Get the planar curve and placement if the spatial curve is planar (call DeleteItem for two-dimensional curve after using ). 
  bool        GetPlaneCurve( MbCurve *& curve2d, MbPlacement3D & place3d, bool saveParams, PlanarCheckParams params = PlanarCheckParams() ) const override;
  // \ru Дать поверхностную кривую, если пространственная кривая поверхностная (после использования вызывать DeleteItem на аргументы). \en Get a surface curve if a spatial curve is on a surface (call DeleteItem for arguments after use). 
  bool        GetSurfaceCurve( MbCurve *& curve2d, MbSurface *& surface, VERSION version = Math::DefaultMathVersion() ) const override;
  // \ru Получить вектор взгляда  \en Get the eye vector.
  const MbVector3D &  GetEye() const { return eye; }
  // \ru Тип проецирования. \en Projection type.
  bool        IsPerspective() const { return perspective; }

  // \ru Построить усеченную кривую c учетом заданной трехмерной точности. \en Construct a trimmed curve with the given three-dimensional accuracy.
  MbCurve3D * Trimmed( double t1, double t2, int sense, const MbDimAccuracy & xyzEps, bool saveParamLenAndLaw ) const override; 
  using MbCurve3D::Trimmed; // \ru Метод базового класса MbCurve3D без учета трехмерной точности. Точность по умолчанию - MbDimAccuracy::threeDimAcc.  \en Method of MbCurve3D base class without three-dimensional accuracy. The default accuracy is MbDimAccuracy::threeDimAcc.

  // \ru Дать плоскую проекцию кривой(локальная система координат, шаг, параметрическая область). \en Get the planar projection of a curve (local coordinate system, step, parametric region). 
  MbCurve *   GetMap( const MbMatrix3D &, MbRect1D * pRgn = nullptr,
                              VERSION version = Math::DefaultMathVersion(), bool * coincParams = nullptr ) const override;

  double      GetMetricLength() const override; // \ru Метрическая длина кривой \en Metric length of a curve 
  double      GetLengthEvaluation() const override; // \ru Оценить метрическую длину кривой. \en Estimate the metric length of a curve. 

  double      GetParamToUnit() const override; // \ru Дать приращение параметра, осреднённо соответствующее единичной длине в пространстве. \en Get parameter increment which averagingly corresponds to the unit length in space.
  double      GetParamToUnit( double t ) const override; // \ru Дать приращение параметра, соответствующее единичной длине в пространстве. \en Get parameter increment which corresponds to the unit length in space.

  void        Inverse( MbRegTransform * iReg = nullptr ) override; // \ru Изменить направление. \en Change the direction.

  MbeCurveBuildType GetBuildType() const override; // \ru Дать тип кривой. \en Get type of curve.
  bool        InsertPoint( double & t ) override; // \ru Вставить точку и выдать её параметр. \en Insert point and get its parameter.
  bool        IsSimilarToCurve( const MbCurve3D & curve, double precision = METRIC_PRECISION ) const override; // \ru Определить, подобные ли кривые для объединения (слива). \en Determine whether the curves for union (joining) are similar. 

          /// \ru Определить, существует ли точное пространственное представление линии очерка. \en Determine whether the exact spatial representation of isocline curve exists. 
          bool        IsExactSpaceCurve() const { return (approxExact && approxCurve != nullptr); }
          /// \ru Получить указатель на кривую точного пространственное представление линии очерка. (Может и не быть). \en Get a pointer to the curve of exact spatial representation of isocline curve. (Can be absent). 
    const MbCurve3D * GetExactSpaceCurve() const { return approxExact ? approxCurve : nullptr; }

  /// \ru Дать пространственную копию линии очерка. \en Construct a new spatial copy of isocline curve. 
  const   MbCurve3D * GetApproxCurve() const { return approxCurve; } 
          bool        InsertPointToApproxCurve( double & t ); // \ru Вставить точку аппроксимационную кривую и выдать её параметр. \en Insert point to approximation curve and get its parameter. 
  /// \ru Установить тип линии очерка. \en Set type of isocline curve. 
          void        SetBuildType( MbeCurveBuildType spec ) { species = spec; }
  /// \ru Вычислить точку. \en Calculate a point. 
          void        GetPointOn ( double & t, MbCartPoint3D & ) const;
  /// \ru Вычислить первую производную. \en Calculate the first derivative. 
          void        GetFirstDer( double & t, MbVector3D & ) const;

  // \ru Получить текущую точку на кривой по параметру. \en Get current point on a curve by a parameter. 
  bool        GetCurvePoint( double & t, MbCartPoint & cPoint ) const override;

private:
          // \ru Проверить параметр и вычислить параметрическую точки. \en Check parameter and calculate parametric points. 
          bool        CorrectPoint( double & t, bool ext, MbCartPoint & cPoint, MbVector & cFirst, MbVector * cSecond, MbVector * cThird ) const;
          void        CalculatePoint ( double & t, bool ext, MbCartPoint3D & ) const; // \ru Вычислить точку на расширенной кривой. \en Calculate a point on the extended curve. 
          void        CalculateFirst ( double & t, bool ext, MbVector3D & )    const; // \ru Вычислить первую производную. \en Calculate the first derivative. 
          void        CalculateSecond( double & t, bool ext, MbVector3D & )    const; // \ru Вычислить вторую производную. \en Calculate the second derivative. 
          void        CalculateThird ( double & t, bool ext, MbVector3D & )    const; // \ru Вычислить третью производную по t. \en Calculate the third derivative by t. 
          // \ru Решить уравнения для определения точек очерка. \en Solve equations for determination of outline points. 
      MbeNewtonResult SilhouetteNewton( const MbCartPoint3D & point, const MbVector3D & vector,
                                        const MbRect & wrkRect, // \ru Рабочая область поиска \en Working region of search 
                                        double funcEpsilon, size_t iterLimit, 
                                        double & u, double & v, 
                                        MbCartPoint3D * pnt = nullptr ) const;

private:
          /// \ru Построить точную пространственную копию кривой. \en Construct the exact spatial curve copy. 
          bool        CreateExactCurve();
          /// \ru Построить новую пространственную копию линии очерка. \en Construct a new spatial copy of isocline curve. 
          bool        CreateApproxCurve();

          void        operator = ( const MbSilhouetteCurve & ); // \ru Не реализовано!!! \en Not implemented!!! 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbSilhouetteCurve )
};

IMPL_PERSISTENT_OPS( MbSilhouetteCurve )


#endif // __CUR_SILHOUETTE_CURVE_H
