////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Проекционная кривая.
         \en Projection curve. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CUR_PROJECTION_CURVE_H
#define __CUR_PROJECTION_CURVE_H


#include <templ_s_array.h>
#include <tool_multithreading.h>
#include <curve.h>
#include <mb_rect.h>
#include <mb_vector3d.h>
#include <mb_cart_point3d.h>


class    MbAxis3D;
class    MbMatrix3D;
class    MbCurve3D;
class    MbSurface;
class              MbRegTransform;
class              MbRegDuplicate;
class              MbCurveIntoNurbsInfo;


//------------------------------------------------------------------------------
/** \brief \ru Проекционная кривая.
           \en Projection curve. \~
  \details \ru Проекционная кривая строится в параметрическом пространстве поверхности
    как проекция пространственной кривой spaceCurve на поверхность surface.
    Если поверхность не плоская, то предполагается, что пространственная кривая лежит на поверхности.
    Двумерная кривая curve содержит начальные приближения для точного вычисления проекционной кривой.
           \en Projection curve is constructed in parametric space of surface
    as projection of spatial curve 'spaceCurve' onto surface 'surface'.
    If surface isn't planar, then it is considered that spatial curve lies on surface.
    Two-dimensional curve 'curve' contains initial approximations for precise calculation of the projection curve. \~
*/ 
// ---
class MATH_CLASS MbProjCurve : public MbCurve {
  enum MbeProjCurveProperties
  {
    pcp_None                 = 0x00,  ///< \ru Сброс всех полей.
                                      ///< \en Reset all fields.
    pcp_Version              = 0x07,  ///< \ru Битовые поля под версию.
                                      ///< \en Bitfields for version.
    pcp_SpaceCurveBelongSurf = 0x08, ///< \ru Пространственная кривая лежит на поверхности проецирования.
                                     ///< \en The spatial curve lies on the projection surface.
    pcp_ProjectionByCurveOne = 0x10, ///< \ru Проекция совпадает с первой поверхностной кривой кривой пересечения.
                                     ///< \en The projection coincides with the first surface curve of the intersection curve.
    pcp_ProjectionByCurveTwo = 0x20, ///< \ru Проекция совпадает со второй поверхностной кривой кривой пересечения.
                                     ///< \en The projection coincides with the second surface curve of the intersection curve.
    pcp_ProjectionWithMatrix = 0x40  ///< \ru Поверхностную кривую кривой пересечения нужно преобразовать по матрице.
                                     ///< \en The surface curve of the intersection curve needs to be matrix-transformed.
  };
private :
  MbCurve3D *    spaceCurve; ///< \ru Пространственная кривая (всегда не nullptr). \en Spatial curve (always not nullptr).
  MbSurface *    surface;    ///< \ru Поверхность (всегда не nullptr). \en Surface (always not nullptr).
  MbCurve *      curve;      ///< \ru Проекция пространственной кривой на поверхность (служит начальным приближением), всегда не nullptr. \en Projection of a spatial curve onto a surface (is used as initial approximation), always not nullptr.
  MbMatrix3D *   into;       ///< \ru Матрица пересчета в систему координат плоскости. Для случая плоской поверхности surface. Вычисляется заново при изменении поверхности. \en A matrix of transformation to the plane coordinate system. In case of planar surface 'surface'. Recalculated if the surface has been changed.
  MbMatrix *     surfMatrix; ///< \ru Матрица пересчета поверхностной кривой (от spaceCurve) в систему координат 'surface'.
                             ///< \en Matrix for transrorming the surface curve (from spaceCurve) to the 'surface' coordinate system.
  uint8          projProp;   ///< \ru Параметры кривой. \en Curves parameters.

  mutable MbRect rect;                ///< \ru Габарит проекционной кривой в параметрическом пространстве поверхности. \en Bounding box of projection curve in parametric space of surface. 
  mutable atomic_double metricLength; ///< \ru Метрическая длина проекционной кривой. \en Metric length of the projection curve. 
  mutable atomic_double tMiddle;      ///< \ru Параметр на кривой, соответствующий метрической середине кривой. \en Parameter on the curve corresponding to the metric middle of the curve. 

  //------------------------------------------------------------------------------
  /** \brief \ru Вспомогательные данные. 
             \en Auxiliary data. \~
    \details \ru Вспомогательные данные служат для ускорения работы объекта.
             \en Auxiliary data are used for fast calculations. \n \~
  */
  // ---
  class MbProjCurveAuxiliaryData : public AuxiliaryData {
  public:
    double      t0;   ///< \ru Исходный параметр. \en Initial parameter.
    double      t;    ///< \ru Модифицированный параметр. \en Modified parameter.
    bool        ext;  ///< \ru Флаг расчета на продолжении. \en Extension flag.

    MbVector    pcDers[cdt_CountDer]; ///< \ru Точка и производные двумерной кривой. \en Curve point and derivatives.
    MbVector3D  scDers[cdt_CountDer]; ///< \ru Точка и производные трехмерной кривой. \en Space curve point and derivatives.

    MbProjCurveAuxiliaryData();
    MbProjCurveAuxiliaryData( const MbProjCurveAuxiliaryData & );
    virtual ~MbProjCurveAuxiliaryData();

    bool IsChanged( double pmin, double pmax, double p, bool pext ) const
    {
      bool changed = false;
      if ( p != t0 )
        changed = true;
      else if ( ext != pext ) {
        changed = true;
        if ( pmin <= p && p <= pmax )
          changed = false;
      }
      return changed;
    }
    void Init();
    void Init( const MbProjCurveAuxiliaryData & );
    void Move( const MbVector & );
  };
  
  mutable CacheManager<MbProjCurveAuxiliaryData> cache;
  
public :
  /** \brief \ru Конструктор по пространственной кривой, поверхности и двумерной кривой.
             \en Constructor by spatial curve, surface and two-dimensional curve. \~
    \details \ru Конструктор по пространственной кривой, поверхности и двумерной кривой. \n
    Двумерная кривая используется как начальное приближение для расчета проекционной кривой, и если \n
    эта кривая сама является проекционной, то будет использоваться ее поле MbProjCurve::curve. \n
    В конструкторе используется копия подложки поверхности. \n
             \en Constructor by spatial curve, surface and two-dimensional curve. \n
    Two-dimensional curve is used as initial approximation for calculation of projection curve. \n
    If this curve is the projection one, then the MbProjCurve::curve will be used. \n
    Copy of surface substrate is used in the constructor. \n \~
    \deprecated \ru Устаревший конструктор. Взамен использовать новый конструктор.
                \en Deprecated constructor. Instead use a new constructor.
    \param[in] sCurve - \ru Проецируемая пространственная кривая.
                        \en A projected spatial curve. \~
    \param[in] sameSpaceCurve - \ru Использовать ли оригинал пространственной кривой.
                                \en Use the original of the spatial curve. \~
    \param[in] surface - \ru Поверхность для проецирования пространственной кривой.
                         \en The surface to project the spatial curve. \~
    \param[in] pCurve - \ru Параметрическая кривая - начальное приближение для проецирования пространственной кривой.
                        \en A parametric curve - initial approximation for projecting of a spatial curve. \~
    \param[in] samePlaneCurve - \ru Использовать ли оригинал параметрической кривой.
                                \en Use the original of the parametric curve. \~
    \param[in] iReg - \ru Регистратор дублирования.
                      \en Registrar of duplication. \~
  */ 
  MbProjCurve( const MbCurve3D & sCurve, bool sameSpaceCurve,
               const MbSurface & surface, 
               const MbCurve & pCurve, bool samePlaneCurve,
               MbRegDuplicate * iReg = nullptr );

  /** \brief \ru Конструктор по пространственной кривой, поверхности и двумерной кривой.
             \en Constructor by spatial curve, surface and two-dimensional curve. \~
    \details \ru Конструктор по пространственной кривой, поверхности и двумерной кривой. \n
    Двумерная кривая используется как начальное приближение для расчета проекционной кривой. \n
    В конструкторе используется копия подложки поверхности. \n
             \en Constructor by spatial curve, surface and two-dimensional curve. \n
    Two-dimensional curve is used as initial approximation for calculation of projection curve. \n
    Copy of surface substrate is used in the constructor. \n \~
    \param[in] sCurve - \ru Проецируемая пространственная кривая.
                        \en A projected spatial curve. \~
    \param[in] sameSpaceCurve - \ru Использовать ли оригинал пространственной кривой.
                                \en Use the original of the spatial curve. \~
    \param[in] pCurve - \ru Параметрическая кривая - начальное приближение для проецирования пространственной кривой.
                        \en A parametric curve - initial approximation for projecting of a spatial curve. \~
    \param[in] samePlaneCurve - \ru Использовать ли оригинал параметрической кривой.
                                \en Use the original of the parametric curve. \~
    \param[in] surface - \ru Поверхность для проецирования пространственной кривой.
                         \en The surface to project the spatial curve. \~
    \param[in] iReg - \ru Регистратор дублирования.
                      \en Registrar of duplication. \~
  */ 
  MbProjCurve( const MbCurve3D & sCurve, bool sameSpaceCurve, 
               const MbCurve & pCurve,   bool samePlaneCurve,
               const MbSurface & surface, MbRegDuplicate * iReg = nullptr );

private:
  MbProjCurve( const MbProjCurve &, MbRegDuplicate * ireg );

public :
  virtual ~MbProjCurve();

public :
  VISITING_CLASS( MbProjCurve );

  /** \ru \name Общие функции геометрического объекта.
      \en \name Common functions of a geometric object.
      \{ */

  MbePlaneType  IsA() const override;                   // \ru Тип элемента \en Type of element 
  bool        IsSimilar  ( const MbPlaneItem & ) const override;     // \ru Являются ли элементы подобными \en Whether the elements are similar 
  bool        SetEqual ( const MbPlaneItem & ) override;       // \ru Сделать элементы равными \en Make the elements equal 
  bool        IsSame( const MbPlaneItem & other, double accuracy = LENGTH_EPSILON ) const override; // \ru Является ли кривая curve копией данной кривой ? \en Whether curve 'curve' is a duplicate of the current curve.
  void        Transform( const MbMatrix &, MbRegTransform * ireg = nullptr, const MbSurface * newSurface = nullptr ) override; // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  virtual void        Transform( const MbMatrix3D &, MbRegTransform * ireg = nullptr );    // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  virtual void        Move     ( const MbVector3D & to, MbRegTransform * = nullptr );    // \ru Сдвиг \en Translation
  virtual void        Rotate   ( const MbAxis3D & axis, double angle, MbRegTransform * = nullptr );  // \ru Повернуть вокруг оси \en Rotate about an axis
  void        Move ( const MbVector &, MbRegTransform * = nullptr, const MbSurface * newSurface = nullptr ) override; // \ru Сдвиг \en Translation
  void        Rotate( const MbCartPoint & pnt, const MbDirection & angle, MbRegTransform * = nullptr, const MbSurface * newSurface = nullptr ) override; // \ru Поворот \en Rotation
  MbPlaneItem & Duplicate( MbRegDuplicate * = nullptr ) const override;  // \ru Сделать копию элемента \en Create a copy of the element

  void        Refresh() override; // \ru Сбросить все временные данные \en Reset all temporary data 
  void        PrepareIntegralData( const bool forced ) const override; // \ru Рассчитать временные (mutable) данные объекта. \en Calculate temporary (mutable) data of an object.

  void        AddYourGabaritTo ( MbRect & ) const override; // \ru Добавь в прям-к свой габарит \en Add your own bounding rectangle into the given rectangle 
  void        CalculateGabarit ( MbRect & ) const override; // \ru Определить габариты кривой. \en Determine bounding box of curve. 
  void        CalculateLocalGabarit( const MbMatrix &, MbRect & ) const override; // \ru Рассчитать габарит относительно л.с.к. \en Calculate bounding box relative to the local coordinate system
  /** \} */

  /** \ru \name Функции описания области определения кривой.
      \en \name Functions for description of a curve domain.
      \{ */
  double      GetTMax () const override; // \ru Вернуть максимальное значение параметра \en Get the maximum value of parameter 
  double      GetTMin () const override; // \ru Вернуть минимальное значение параметра \en Get the minimum value of the parameter 
  bool        IsClosed() const override; // \ru Замкнутость кривой \en A curve closedness 
  double      GetPeriod() const override; // \ru Вернуть период \en Get period  

  /** \} */
  /** \ru \name Функции для работы в области определения кривой. 
            Функции PointOn, FirstDer, SecondDer, ThirdDer,... корректируют параметр
            при выходе его за пределы области определения.
      \en \name Functions for working in the curve's domain. 
            Functions PointOn, FirstDer, SecondDer, ThirdDer,... correct parameter
            if it is out of domain bounds.
      \{ */
  void        PointOn  ( double & t, MbCartPoint & p ) const override; // \ru Точка на кривой \en Point on the curve 
  void        FirstDer ( double & t, MbVector    & v ) const override; // \ru Первая производная \en The first derivative 
  void        SecondDer( double & t, MbVector    & v ) const override; // \ru Вторая производная \en The second derivative 
  void        ThirdDer ( double & t, MbVector    & v ) const override; // \ru Третья производная \en The third derivative 
  /** \} */

  /** \ru \name Функции для работы внутри и вне области определения кривой. 
            Функции _PointOn, _FirstDer, _SecondDer, _ThirdDer,... не корректируют параметр
            при выходе за пределы области определения. При выходе за область определения незамкнутая
            кривая, в общем случае, продолжается по касательной, которую она имеет на соответствующем конце.
            Исключение составляют дуги эллипса и окружности - они продолжаются в соответствии со своими уравнениями.
      \en \name Functions for working inside and outside the curve's domain. 
            Functions _PointOn, _FirstDer, _SecondDer, _ThirdDer,... don't correct parameter
            if it is out of domain bounds. If the parameter is out of domain bounds, an unclosed
            curve is extended by tangent vector at corresponding end point in general case.
            Except arcs of an ellipse or a circle - they are extended according to their equations.
      \{ */
  void       _PointOn  ( double t, MbCartPoint & p ) const override;   // \ru Точка на кривой \en Point on the curve 
  void       _FirstDer ( double t, MbVector & v ) const override;      // \ru Первая производная \en The first derivative
  void       _SecondDer( double t, MbVector & v ) const override;      // \ru Вторая производная \en The second derivative
  void       _ThirdDer ( double t, MbVector & v ) const override;      // \ru Третья производная \en The third derivative
  /** \} */

  /** \ru \name Функции доступа к группе данных для работы внутри и вне области определения параметра кривой.
      \en \name Functions for get of the group of data inside and outside the curve's domain of parameter.
      \{ */
  // \ru Вычислить значения точки и производных для заданного параметра. \en Calculate point and derivatives of object for given parameter. \~
  void        Explore( double & t, bool ext,
                       MbCartPoint & pnt, MbVector & fir, MbVector * sec, MbVector * thir ) const override;
  /** \} */

  /** \ru \name Общие функции кривой
      \en \name Common functions of curve
      \{ */
  double      PointProjection( const MbCartPoint & pnt ) const override; // \ru Проекция точки на кривую. \en Point projection on the curve. 
  bool        NearPointProjection( const MbCartPoint & pnt, double xEpsilon, double yEpsilon, 
                                   double & t, bool ext, MbRect1D * tRange = nullptr ) const override; // \ru Проекция точки на кривую или её продолжение в области поиска проекции. \en Projection of a point onto the curve or its extension in the projection region.

  bool        HasLength( double & ) const override; // \ru Метрическая длина кривой. \en Metric length of a curve. 
  double      GetMetricLength()     const override; // \ru Метрическая длина кривой. \en Metric length of a curve.
  double      GetLengthEvaluation() const override; // \ru Оценка метрической длины кривой. \en Estimation of metric length of the curve. 
  // \ru Вычислить метрическую длину кривой от параметра t1 до t2. \en Calculate the metric length of unclosed curve from parameter t1 to parameter t2. 
  double      CalculateLength( double t1, double t2 ) const override; 
  /// \ru Вычислить метрическую длину кривой. \en Calculate the metric length of a curve. 
  double      CalculateMetricLength() const override;

  bool        GetMiddlePoint( MbCartPoint & ) const override; // \ru Вычислить среднюю точку кривой. \en Calculate mid-point of curve.

  void        Inverse( MbRegTransform * iReg = nullptr ) override; // \ru Изменение направления кривой на противоположное \en Change curve direction to the opposite one

  MbNurbs *   NurbsCurve( const MbCurveIntoNurbsInfo & ) const override;

  // \ru Удалить часть кривой между параметрами t1 и t2 \en Delete a part of a curve between parameters t1 and t2 
  MbeState    DeletePart( double t1, double t2, MbCurve *& part2 ) override;
  // \ru Оставить часть кривой между параметрами t1 и t2 \en Keep a piece of a curve between parameters t1 and t2  
  MbeState    TrimmPart( double t1, double t2, MbCurve *& part2 ) override;

  double      Step( double t, double sag ) const override; // \ru Вычисление шага аппроксимации с учетом радиуса кривизны \en Calculation of step of approximation with consideration of curvature radius 
  double      DeviationStep( double t, double angle ) const override; // \ru Вычисление шага аппроксимации с учетом угла отклонения \en Calculation of step of approximation with consideration of angle of deviation 
  // \ru Построить усеченную кривую c учетом заданной двумерной точности. \en Construct a trimmed curve with the given two-dimensional accuracy.
  MbCurve *   Trimmed( double t1, double t2, int sense, const MbDimAccuracy & xyEps, bool saveParamLenAndLaw ) const override;
  // \ru Построить усеченную кривую. \en Construct a trimmed curve.
  MbCurve *   Trimmed( double t1, double t2, int sense, bool saveParamLenAndLaw = false ) const override;

  const MbCurve3D & GetSpaceCurve() const { return *spaceCurve; }
  const MbSurface & GetSurface   () const { return *surface;    }
  const MbCurve &   GetParamCurve() const { return *curve;      }

  bool        GetCentre( MbCartPoint & c ) const override; // \ru Выдать центр кривой \en Get center of curve 
  double      GetRadius( double accuracy = PARAM_REGION ) const override; // \ru Дать физический радиус объекта или ноль, если это невозможно. \en Get the physical radius of the object or null if it impossible.
  bool        GetAxisPoint( MbCartPoint & ) const override; // \ru Точка для построения оси \en Point for the axis construction 

  bool        SetSameSurface( const MbSurface & s ); ///< \ru Заменить поверхность на такую же. \en Whether the projecting curve lies on the surface. 

  bool        IsBelong() const { return (projProp & pcp_SpaceCurveBelongSurf) > 0; } ///< \ru Лежит ли проецируемая кривая на поверхности. \en Whether the projecting curve lies on the surface. 

  bool        InvertNormal( MbRegTransform * = nullptr ); ///< \ru Инвертировать нормаль, если поверхность - плоскость. \en Invert normal if the surface is a plane.

  bool        ChangeCarrierBorne( const MbSurface & item, MbSurface & init, const MbMatrix & matr ); ///< \ru Изменение носителя. \en Change a carrier.  

  /// \ru Получить 2d сплайн с данной относительной точностью аппроксимирующий данную кривую. \en Get 2d spline which approximates given curve with a given relative tolerance. 
  MbCurve *   CreateSpline( double relEps, MbRect1D * pRgn = nullptr ) const;

  /// \ru Создать кривую путём сращивания части данной кривой с частью другой кривой. \en Create a curve by joining a part of this curve with a part of other curve.
  MbProjCurve * AddCurve( const MbProjCurve &, double accuracy, VERSION version = Math::DefaultMathVersion() ) const;
  /// \ru Сращивает исходную кривую с кривой other. \en Joining this curve with other curve.
  bool          AddCurveToCurve( const MbProjCurve &, double accuracy, VERSION version );

  void        GetProperties( MbProperties & ) override;  // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & ) override;  // \ru Записать свойства объекта \en Set properties of the object 
  void        GetBasisPoints( MbControlData & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.
  bool        IsSimilarToCurve( const MbCurve & curve, double precision = PARAM_PRECISION ) const override; // \ru Подобные ли кривые для объединения (слива). \en Whether the curves for union (joining) are similar. 
  bool        SetLimitParam( double newTMin, double newTMax ); // \ru Установить область изменения параметра \en Set range of parameter  
  /** \} */

private:
  // \ru Функция инициализации. \en Function of initialization.
  void        Init( const MbCurve3D & sCurve, bool sameSpaceCurve, const MbSurface & surface,
                    const MbCurve & pCurve, bool samePlaneCurve, size_t vers,
              MbRegDuplicate * iReg = nullptr );
  void        CheckPoint ( double & t, bool ext, MbCartPoint & cPoint, MbProjCurveAuxiliaryData * loccache ) const; // \ru Обнулить данные, вычислить точку \en Set data to zero, calculate point. 
  void        CheckFirst ( double & t, bool ext, MbVector & cFirst, MbProjCurveAuxiliaryData * loccache ) const;    // \ru Вычислить производную \en Calculate derivative 
  void        CheckSecond( double & t, bool ext, MbVector & cSecond, MbProjCurveAuxiliaryData * loccache ) const;   // \ru Вычислить производную \en Calculate derivative 
  void        CheckThird ( double & t, bool ext, MbVector & cThird, MbProjCurveAuxiliaryData * loccache ) const;    // \ru Вычислить производную \en Calculate derivative 
  bool        CalculatePoint( const MbCartPoint3D & sPoint, MbCartPoint & cPoint,
                              MbCartPoint3D & pnt, MbVector3D & uDer, MbVector3D & vDer, 
                              MbVector3D & uuDer, MbVector3D & vvDer, MbVector3D & uvDer, MbVector3D * nor ) const; // \ru Вычислить данные для точки. \en Calculate point.
  void        CalculateFirst( bool result, MbCartPoint3D & surfacePoint, MbVector3D & sDeriveU, MbVector3D & sDeriveV, 
                              MbVector3D & sDeriveUU, MbVector3D & sDeriveVV, MbVector3D & sDeriveUV,  MbVector3D & sNormal,
                              const MbCartPoint3D & sPoint, const MbCartPoint & cPoint, const MbVector3D & sFirst, MbVector & cFirst ) const; // \ru Вычислить производную \en Calculate derivative 
  void        CalculateSecond( const MbCartPoint3D & surfacePoint, const MbVector3D & sDeriveU, const MbVector3D & sDeriveV, 
                               const MbVector3D & sDeriveUU, const MbVector3D & sDeriveVV, const MbVector3D & sDeriveUV, const MbVector3D & sNormal, 
                               const MbCartPoint3D & sPoint, const MbCartPoint & cPoint, const MbVector3D & sFirst, const MbVector & cFirst, 
                               const MbVector3D & sSecond, MbVector & cSecond ) const; // \ru Вычислить производную \en Calculate derivative 
  void        SetBelong(); // \ru Вычисление параметра belong: лежит ли проецируемая кривая на поверхности \en Calculate 'belong' parameter: whether the projecting curve lies on the surface 
  // \ru Расчет в точке для общего случая.
  // \en Calculation of mathematics at a point for the general case.
  void        GeneralCaseExplore( double t, size_t ord ) const;
  // \ru Расчет в точке для случая, когда кривая лежит на поверхности.
  // \en Calculation of mathematics at a point for the case when the curve lies on the surface.
  void        BelongCaseExplore( double t, size_t ord ) const;
  // \ru Расчет в точке для случая проецирования на плоскость.
  // \en Calculation at a point for the case of projection onto a plane.
  void        PlaneCaseExplore( double t, size_t ord, bool single ) const;
  // \ru Расчет в точке.
  // \en Calculation at a point.
  void        Explore( double & t, bool ext, size_t ord, bool single, MbProjCurveAuxiliaryData * loccache ) const;
  // \ru Определение типа проецирования. \en Determining the type of projection
  void        SetProjType(); 
  // \ru Обновить связь кривой и поверхности. \en Refresh curve-to-surface relationship.
  void        UpdateProjSurface(); 
  void        SetInto(); // \ru Инициализировать матрицу пересчета в систему координат плоскости. \en Initialize matrix of transformation to the plane coordinate system.  
  void        PrepareCurveToTrimmed( MbCurve * curvett, double t1, double t2 ) const; // \ru Подготовить двумерную кривую к усечению \en Prepare a two-dimensional curve for trimming 
          
  /** \brief \ru Поменять базовую поверхность на подобную.
              \en Change base surface to the similar one. \~
    \details \ru Поменять базовую поверхность. Новая поверхность должна быть подобна старой.
              \en Change the base surface. The new surface has to be similar to the old one. \~
    \param[in] newSurface - \ru Новая поверхность. Захватывается кривой.
                            \en New surface. Is captured by the curve. \~
    \param[in] matrix     - \ru Матрица преобразования из старой поверхности в новую.
                            \en Transformation matrix from the old surface to a new one. \~                                           
  */
  void        ChangeSurfaceToSimilar( const MbSurface & newSurface, const MbMatrix & matrix, MbRegTransform * iReg );

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbProjCurve )
  OBVIOUS_PRIVATE_COPY( MbProjCurve )
}; // MbProjCurve

IMPL_PERSISTENT_OPS( MbProjCurve )


//------------------------------------------------------------------------------
// \ru Изменение носимых элементов \en Change a carrier elements 
// ---
bool ChangeCarrierBorne( const MbSurface & item, MbSurface & init, const MbMatrix & matr, MbCurve & curve );


//------------------------------------------------------------------------------
// \ru Пространственная трансформация проекционных кривых в двумерной кривой \en Spatial transformation of projection curves in two-dimensional curve 
// ---
bool TransformProjCurves( MbCurve & curve, const MbMatrix3D & matr, MbRegTransform * ireg );


//------------------------------------------------------------------------------
// \ru Пространственный сдвиг проекционных кривых в двумерной кривой \en Spatial translation of projection curves in two-dimensional curve 
// ---
bool MoveProjCurves( MbCurve & curve, const MbVector3D & to, MbRegTransform * ireg );


//------------------------------------------------------------------------------
// \ru Вращение проекционных кривых в двумерной кривой \en Rotation of projection curves in two-dimensional curve 
// ---
bool RotateProjCurves( MbCurve & curve, const MbAxis3D & axis3d, double angle, MbRegTransform * ireg );


#endif // __CUR_PROJECTION_CURVE_H
