////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Поверхность заметания.
         \en The swept surface. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __SURF_EVOLUTION_SURFACE_H
#define __SURF_EVOLUTION_SURFACE_H


#include <surf_swept_surface.h>
#include <surf_spine.h>
#include <tool_multithreading.h>
#include <templ_dptr.h>


class  MbSurfaceWorkingData;


//------------------------------------------------------------------------------
/** \brief \ru Поверхность заметания.
           \en The swept surface. \~
  \details \ru Кинематическая поверхность образуется путем движения образующей кривой curve по направляющей кривой spine->curve.
    В процессе движения вдоль направляющей кривой образующая кривая сохраняет своё положение в движущейся локальной системе координат,
    начало которой совпадает с текущей точкой базовой кривой.
    Одна из осей движущейся локальной системы координат всегда совпадает с касательной направляющей кривой, 
    а две другие оси ортогональны ей.
    Первый параметр поверхности совпадает с параметром образующей кривой. 
    Второй параметр поверхности совпадает с параметром направляющей кривой.
           \en Sweep with guide curve surface is formed by moving the 'curve' generating curve along spine->curve guide curve.
    While moving along a guide curve the generating curve keeps its position in the moving local coordinate system,
    which origin coincides with the current point of the base curve.
    One of the axes of the moving local coordinate system is always coincident to the tangent of the guide curve, 
    and the other two axes are orthogonal to it.
    First parameter of surface coincides with parameter of generating curve. 
    Second parameter of surface coincides with parameter of guide curve. \~ 
  \ingroup Surfaces
*/
// ---
class MATH_CLASS MbEvolutionSurface : public MbSweptSurface {
protected:
  MbSpine *     spine;  ///< \ru Направляющая кривая. \en Spine (guide) curve. 
  MbCartPoint3D origin; ///< \ru Начало направляющей кривой. \en Beginning of generating curve. 

  //------------------------------------------------------------------------------
  /** \brief \ru Вспомогательные данные. 
             \en Auxiliary data. \~
    \details \ru Вспомогательные данные служат для ускорения работы объекта.
             \en Auxiliary data are used for fast calculations. \n \~
  */
  // ---
  class MbEvolutionSurfaceAuxiliaryData : public AuxiliaryData {
  public:
    DPtr<MbSurfaceWorkingData> wData; ///< \ru Рабочие данные для расчета поверхности. \en Working data for the calculation of a surface. 
    MbCartPoint3D              wPnt;  ///< \ru Рабочая точка. \en Working point.
    MbVector3D                 wVect; ///< \ru Рабочий вектор. \en Working vector.
    MbMatrix3D                 wMatr; ///< \ru Рабочая матрица. \en Working matrix.

    MbEvolutionSurfaceAuxiliaryData();
    MbEvolutionSurfaceAuxiliaryData( const MbEvolutionSurfaceAuxiliaryData & );
    virtual ~MbEvolutionSurfaceAuxiliaryData();

    void Init();
    void Init( const MbEvolutionSurfaceAuxiliaryData & );
    void Move( const MbVector3D & );
  };
  
  mutable CacheManager<MbEvolutionSurfaceAuxiliaryData> cache;

public:

  /** \brief \ru Конструктор по образующей и направляющей.
             \en Constructor by generating curve and guide curve. \~
    \details \ru Конструктор по образующей и направляющей.
             \en Constructor by generating curve and guide curve. \~ 
    \param[in] c - \ru Образующая
                   \en Generating curve \~
    \param[in] s - \ru Направляющая
                   \en Guide curve \~
    \param[in] sameCurve - \ru Признак использования оригинала образующей, а не копии
                           \en Attribute of usage of original of generating curve, not a copy \~
    \param[in] sameSpine - \ru Признак использования оригинала направляющей, а не копии
                           \en Attribute of usage of original of guide curve, not a copy \~
  */
  MbEvolutionSurface( const MbCurve3D & c, const MbSpine & s, bool sameCurve, bool sameSpine = false ); 
  
  /** \brief \ru Конструктор по образующей и направляющей.
             \en Constructor by generating curve and guide curve. \~
    \details \ru Конструктор по образующей, направляющей и её кривой вектора ориентации матрицы преобразования.
             \en Constructor by generating curve, guide curve and its curve of transformation matrix orientation vector. \~ 
    \param[in] c - \ru Образующая
                   \en Generating curve \~
    \param[in] s - \ru Направляющая
                   \en Guide curve \~
    \param[in] d - \ru Кривая вектора ориентации матрицы преобразования направляющей
                   \en Curve of orientation vector of transformation matrix of guide curve \~
    \param[in] sameCurve - \ru Признак использования оригинала образующей, а не копии
                           \en Attribute of usage of original of generating curve, not a copy \~
    \param[in] sameSpine - \ru Признак использования оригинала направляющей, а не копии
                           \en Attribute of usage of original of guide curve, not a copy \~
    \param[in] sameD - \ru Признак использования оригинала кривой d, а не копии
                       \en Attribute of usage of original of 'd' curve, not a copy \~
  */
  MbEvolutionSurface( const MbCurve3D & c, const MbCurve3D & s, const MbCurve3D & d, 
                      bool sameCurve, bool sameSpine, bool sameD );
  
  /** \brief \ru Конструктор по радиусу, направляющей и её кривой вектора ориентации матрицы преобразования.
             \en Constructor by radius, guide curve and its curve of transformation matrix orientation vector. \~
    \details \ru Конструктор кинематической поверхности с образующей кривой - дугой окружности.
      Используется только в конвертерах.
             \en Constructor of evolution surface with circular arc as generating curve.
      Used only in converters. \~
    \param[in] r - \ru Радиус образующей
                   \en Radius of generating curve \~
    \param[in] s - \ru Направляющая
                   \en Guide curve \~
    \param[in] d - \ru Кривая вектора ориентации матрицы преобразования направляющей
                   \en Curve of orientation vector of transformation matrix of guide curve \~
    \param[in] pURgn - \ru Область параметра U
                       \en Region of U parameter \~
    \param[in] pVRgn - \ru Область параметра V
                       \en Region of V parameter \~
  */       
  MbEvolutionSurface( double r, const MbCurve3D & s, const MbCurve3D & d, MbRect1D * pURgn, MbRect1D * pVRgn, VERSION version = Math::DefaultMathVersion() );

protected:
  MbEvolutionSurface( const MbEvolutionSurface &, MbRegDuplicate * );
private:
  MbEvolutionSurface( const MbEvolutionSurface & ); // \ru Не реализовано. \en Not implemented. 
public:
  virtual ~MbEvolutionSurface();

public:
  VISITING_CLASS( MbEvolutionSurface );

  /** \ru \name Общие функции геометрического объекта
      \en \name Common functions of a geometric object
      \{ */
  MbeSpaceType  IsA() const override; // \ru Тип элемента \en A type of element 
  MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента. \en Create a copy of the element.
  bool    IsSame   ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const override; // \ru Равны ли объекты. \en Whether the objects are equal. 
  bool    SetEqual ( const MbSpaceItem & ) override;       // \ru Сделать равным. \en Make equal. 
  bool    IsSimilar  ( const MbSpaceItem & ) const override; // \ru Являются ли объекты подобными. \en Determine whether the objects are similar.
  void    Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override;          // \ru Преобразовать элемент согласно матрице. \en Transform element according to the matrix.
  void    Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override;          // \ru Сдвиг. \en Move.
  void    Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси. \en Rotate around an axis.
  
  void    GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  void    SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта. \en Set properties of the object. 
  void    GetBasisItems  ( RPArray <MbSpaceItem> & ) override; // \ru Дать базовые объекты. \en Get the base objects.
  void    GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void    SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  /** \} */
  /** \ru \name Функции для работы в области определения поверхности
            Функции PointOn, Derive... поверхностей корректируют параметры
            при выходе их за пределы прямоугольной области определения параметров.\n  
      \en \name Functions for working at surface domain
            Functions PointOn, Derive... of surfaces correct parameters
            when they are out of bounds of rectangular domain of parameters.\n
      \{ */
  void    PointOn  ( double & u, double & v, MbCartPoint3D & ) const override; // \ru Точка на поверхности. \en The point on the surface. 
  void    DeriveU  ( double & u, double & v, MbVector3D & ) const override; // \ru Первая производная по u. \en The first derivative with respect to u. 
  void    DeriveV  ( double & u, double & v, MbVector3D & ) const override; // \ru Первая производная по v. \en The first derivative with respect to v. 
  void    DeriveUU ( double & u, double & v, MbVector3D & ) const override; // \ru Вторая производная по u. \en The second derivative with respect to u. 
  void    DeriveVV ( double & u, double & v, MbVector3D & ) const override; // \ru Вторая производная по v. \en The second derivative with respect to v. 
  void    DeriveUV ( double & u, double & v, MbVector3D & ) const override; // \ru Вторая производная по uv. \en The second derivative with respect to uv. 
  void    DeriveUUU( double & u, double & v, MbVector3D & ) const override;
  void    DeriveUUV( double & u, double & v, MbVector3D & ) const override;
  void    DeriveUVV( double & u, double & v, MbVector3D & ) const override;
  void    DeriveVVV( double & u, double & v, MbVector3D & ) const override;
  void    Normal( double & u, double & v, MbVector3D & ) const override;
  /** \} */
  /** \ru \name Функции для работы внутри и вне области определения поверхности
            функции _PointOn, _Derive... поверхностей не корректируют
            параметры при выходе их за пределы прямоугольной области определения параметров.
      \en \name Functions for working inside and outside the surface's domain
            functions _PointOn, _Derive... of surfaces don't correct
            parameters when they are out of bounds of rectangular domain of parameters.
      \{ */
  void   _PointOn  ( double u, double v, MbCartPoint3D & ) const override; // \ru Точка на расширенной поверхности. \en The point on the extended surface. 
  void   _DeriveU  ( double u, double v, MbVector3D & ) const override; // \ru Первая производная по u. \en The first derivative with respect to u. 
  void   _DeriveV  ( double u, double v, MbVector3D & ) const override; // \ru Первая производная по v. \en The first derivative with respect to v. 
  void   _DeriveUU ( double u, double v, MbVector3D & ) const override; // \ru Вторая производная по u. \en The second derivative with respect to u. 
  void   _DeriveVV ( double u, double v, MbVector3D & ) const override; // \ru Вторая производная по v. \en The second derivative with respect to v. 
  void   _DeriveUV ( double u, double v, MbVector3D & ) const override; // \ru Вторая производная по uv. \en The second derivative with respect to uv. 
  void   _DeriveUUU( double u, double v, MbVector3D & ) const override;
  void   _DeriveUUV( double u, double v, MbVector3D & ) const override;
  void   _DeriveUVV( double u, double v, MbVector3D & ) const override;
  void   _DeriveVVV( double u, double v, MbVector3D & ) const override;
  void   _Normal( double u, double v, MbVector3D & ) const override;
  /** \} */
  /** \ru \name Функции доступа к группе данных для работы внутри и вне области определения параметров поверхности.
      \en \name Functions for get of the group of data inside and outside the surface's domain of parameters.
      \{ */
  void    Explore( double & u, double & v, bool ext,
                   MbCartPoint3D & pnt, MbVector3D & uDer, MbVector3D & vDer,
                   MbVector3D * uuDer, MbVector3D * vvDer, MbVector3D * uvDer, MbVector3D * nor ) const override;
  /** \} */
  /** \ru \name Функции движения по поверхности
      \en \name Functions of moving along the surface
      \{ */
  double  StepU         ( double u, double v, double sag ) const override; // \ru Вычисление шага по u по заданной стрелке прогиба. \en Calculation of the parameter step in u direction by the sag. 
  double  StepV         ( double u, double v, double sag ) const override; // \ru Вычисление шага по v по заданной стрелке прогиба. \en Calculation of the parameter step in v direction by the sag. 
  double  DeviationStepU( double u, double v, double angle ) const override; // \ru Вычисление шага по u по заданному углу отклонения. \en Calculation of the parameter step in u direction by the deviation angle. 
  double  DeviationStepV( double u, double v, double angle ) const override; // \ru Вычисление шага по v по заданному углу отклонения. \en Calculation of the parameter step in v direction by the deviation angle. 
  double  MetricStepU   ( double u, double v, double length ) const override; // \ru Вычисление шага по u по заданной длине. \en Calculation of the parameter step in u direction by the given length. 
  double  MetricStepV   ( double u, double v, double length ) const override; // \ru Вычисление шага по v по заданной длине. \en Calculation of the parameter step in v direction by the given length. 
  size_t  GetUCount() const override;
  size_t  GetVCount() const override;
  /** \} */
  /** \ru \name Общие функции поверхности
      \en \name Common functions of surface
      \{ */
  double  CurvatureU    ( double u, double v ) const override; // \ru Кривизна линии по u. \en Curvature of line by u. 
  bool    IsPlanar( double accuracy = METRIC_EPSILON ) const override; // \ru Является ли поверхность плоской. \en Whether the surface is planar. 
    
  void    ChangeCarrier( const MbSpaceItem & item, MbSpaceItem & init ) override; // \ru Изменение носителя. \en Changing of carrier. 

  void    CalculateGabarit( MbCube & ) const override; // \ru Выдать габарит. \en Get the bounding box. 
  void    CalculateLocalGabarit( const MbMatrix3D &, MbCube & ) const override; // \ru Рассчитать габарит относительно л.с.к. \en Calculate bounding box relative to the local coordinate system. 

  MbSplineSurface * NurbsSurface( double, double, double, double, bool bmatch = false ) const override; // \ru NURBS копия поверхности. \en NURBS copy of a surface. 
  MbSurface *       NurbsSurface( const MbNurbsParameters & uParam, const MbNurbsParameters & vParam ) const override; // \ru NURBS копия поверхности. \en NURBS copy of a surface. 
  MbSurface *       Offset( double d, bool same ) const override; // \ru Создание эквидистантной поверхности. \en Create an offset surface. 

  // \ru Подобные ли поверхности для объединения (слива) \en Whether the surfaces to union (joining) are similar 
  bool    IsSpecialSimilarToSurface( const MbSurface & surf, VERSION version, double precision = METRIC_PRECISION ) const override; // \ru Специальный случай \en Special case 

  MbCurve3D * CurveU( double v, MbRect1D *pRgn, bool bApprox = true ) const override; // \ru Пространственная копия линии v = const. \en Spatial copy of 'v = const'-line.
  MbCurve3D * CurveV( double u, MbRect1D *pRgn, bool bApprox = true ) const override; // \ru Пространственная копия линии u = const. \en Spatial copy of 'u = const'-line.

  // \ru Построить касательные и нормальные плейсменты конструктивных плоскостей. \en Construct tangent and normal placements of constructive planes. 
  bool    CreateNormalPlacements ( const MbVector3D & axisZ, double angle, SArray<MbPlacement3D> & places, VERSION version = Math::DefaultMathVersion() ) const override;
  bool    CreateTangentPlacements( const MbVector3D & axisZ, SArray<MbPlacement3D> & places ) const override;
  // \ru Подобные ли поверхности для объединения (слива). \en Whether the surfaces to union (joining) are similar. 
  bool    IsSimilarToSurface( const MbSurface & surf, VERSION version, double precision = METRIC_PRECISION ) const override;
  // \ru Дать двумерную матрицу преобразования из своей параметрической области в параметрическую область surf. \en Get two-dimensional transformation matrix from own parametric region to parametric region of 'surf'. 
  bool    GetMatrixToSurface( const MbSurface & surf, MbMatrix & matr, VERSION version, double precision = METRIC_PRECISION ) const override;
  bool    GetCenterLines( std::vector<MbCurve3D *> & clCurves ) const override; // \ru Дать осевые (центральные) линии для поверхности. \en Get center lines of a surface.
  // \ru Определение разбивки параметрической области поверхности вертикалями и горизонталями. \en Determine splitting of parametric region of surface by vertical and horizontal lines. 
  void    GetTesselation( const MbStepData & stepData,
                          double u1, double u2, double v1, double v2,
                          SArray<double> & uu, SArray<double> & vv ) const override;
  bool    IsSpinePeriodic() const override; // \ru Периодичность направляющей. \en Periodicity of a guide curve. 

  size_t  GetUMeshCount() const override; // \ru Выдать количество полигонов по u. \en Get the count of polygons by u. 
  size_t  GetVMeshCount() const override; // \ru Выдать количество полигонов по v. \en Get the count of polygons by v. 

  // \ru Включить точку в область определения. \en Include a point into domain. 
  void    IncludePoint( double u, double v ) override;
  /** \} */
  /** \ru \name Функции кинематической поверхности
      \en \name Functions of the evolution surface
      \{ */

  /** \brief \ru Определение матрицы переноса для образующей.
              \en Determination of translation matrix for generating curve. \~
    \details \ru Определение матрицы переноса для образующей по параметру направляющей.
              \en Determination of translation matrix for generating curve by parameter of guide curve. \~
    \param[in] v - \ru Параметр на направляющей
                    \en Parameter on the guide curve \~
    \param[in] matr - \ru Матрица-результат
                      \en Matrix-result \~
  */
  void    TransformMatrix( double v, MbMatrix3D & matr ) const; 

  /// \ru Направляющая. \en Guide curve. 
  const MbSpine       & GetSpine() const { return *spine; } 

  /// \ru Направляющая кривая. \en The spine (guide) curve. 
  const MbCurve3D     & GetSpineCurve() const { return spine->GetCurve(); } 
  /// \ru Центр тяжести образующей. \en Center of gravity of generating curve. 
  const MbCartPoint3D & GetOrigin() const { return origin; } 

  /// \ru Дать направляющую кривую для изменения. \en Get guide curve for editing. 
  MbCurve3D     & SetSpineCurve()   { return spine->SetCurve(); } 
  /// \ru Задать центр тяжести образующей. \en Set center of gravity of generating curve. 
  void            SetOrigin( const MbCartPoint3D & p ) { origin = p; SetDirtyGabarit(); } 
  /** \} */

protected :
  void    Init();
private:
  void    operator = ( const MbEvolutionSurface & ); // \ru Не реализовано. \en Not implemented. 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbEvolutionSurface )
};

IMPL_PERSISTENT_OPS( MbEvolutionSurface )


//------------------------------------------------------------------------------
/** \brief \ru Создать кинематическую поверхность.
           \en Create an evolution surface. \~
  \details \ru Создать кинематическую поверхность.
           \en Create an evolution surface. \~ 
  \param[in] curve - \ru Образующая кривая
                     \en Generating curve \~
  \param[in] spine - \ru Направляющая кривая
                     \en Guide curve \~
  \param[in] samec - \ru Признак использования оригинала образующей кривой, а не копии.
                     \en Attribute of usage of original of generating curve, not a copy. \~
  \param[in] sames - \ru Признак использования оригинала направляющей кривой spine, а не копии.
                     \en Attribute of usage of original of guide curve (spine), not a copy. \~
  \return \ru Возвращает созданную поверхность.
          \en Return the created surface. \~ 
  \ingroup Surface_Modeling
*/
// ---
MATH_FUNC (MbSurface &) CreateEvolutionSurface( const MbCurve3D & curve, const MbSpine & spine, bool samec, bool sames = false );


//------------------------------------------------------------------------------
/** \brief \ru Создать кинематическую поверхность.
           \en Create an evolution surface. \~
  \details \ru Создать кинематическую поверхность.
           \en Create an evolution surface. \~ 
  \param[in] curve - \ru Образующая кривая
                     \en Generating curve \~
  \param[in] spine - \ru Направляющая кривая
                     \en Guide curve \~
  \param[in] samec - \ru Признак использования оригинала образующей кривой, а не копии
                     \en Attribute of usage of original of generating curve, not a copy \~
  \param[in] sames - \ru Признак использования оригинала направляющей кривой, а не копии
                     \en Attribute of usage of original of guide curve, not a copy \~
  \return \ru Возвращает созданную поверхность.
          \en Return the created surface. \~ 
  \ingroup Surface_Modeling
*/
// ---
MATH_FUNC (MbSurface &) CreateEvolutionSurface( const MbCurve3D & curve, const MbCurve3D & spine,
                                                bool samec, bool sames );


//------------------------------------------------------------------------------
/** \brief \ru Создать кинематическую поверхность.
           \en Create an evolution surface. \~
  \details \ru Создать кинематическую поверхность.
           \en Create an evolution surface. \~ 
  \param[in] curve - \ru Образующая кривая
                     \en Generating curve \~
  \param[in] spine - \ru Направляющая кривая
                     \en Guide curve \~
  \param[in] spineDirection - \ru Направляющая кривая для направляющей кривой
                              \en Guide curve for guide curve \~
  \param[in] samec - \ru Признак использования оригинала образующей кривой, а не копии
                     \en Attribute of usage of original of generating curve, not a copy \~
  \param[in] sames - \ru Признак использования оригинала направляющей кривой, а не копии
                     \en Attribute of usage of original of guide curve, not a copy \~
  \param[in] samed - \ru Признак использования оригинала кривой spineDirection, а не копии
                     \en Attribute of usage of original of spineDirection curve, not a copy \~
  \return \ru Возвращает созданную поверхность.
          \en Return the created surface. \~ 
  \ingroup Surface_Modeling
*/
// ---
MATH_FUNC (MbSurface &) CreatePipeSurface( const MbCurve3D & curve, const MbCurve3D & spine, 
                                           const MbCurve3D & spineDirection, 
                                           bool samec, bool sames, bool samed );


#endif // __SURF_EVOLUTION_SURFACE_H
