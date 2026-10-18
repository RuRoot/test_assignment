////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Бикубическая поверхность Кунса на четырех кривых и их поперечных производных.
         \en Bicubic Coons surface on four curves and its transverse derivatives. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __SURF_COONS_SURFACE_H
#define __SURF_COONS_SURFACE_H


#include <templ_dptr.h>
#include <surface.h>
#include <surf_coons_surface_.h>
#include <cur_surface_curve.h>
#include <mb_operation_result.h>


class  MbCurve;


//------------------------------------------------------------------------------
/** \brief \ru Способ расчёта поверхности Кунса.
           \en Type of calculation of Coons surface. \~
  \details \ru Способ расчёта поверхности Кунса. \n
           \en Type of calculation of Coons surface. \n \~
  \ingroup Surfaces
*/ 
// ---
enum MbeCoonsSurfaceCalcType {
  cst_DefaultType = 0,    ///< \ru Способ по умолчанию. \en Default type.
  cst_SurfaceType,        ///< \ru Точный способ по кривым на поверхностях. \en Exact type by Curves on surfaces.
  cst_GregoryPatchType,   ///< \ru Патч Грегори. \en Gregory patch.
};


//------------------------------------------------------------------------------
/** \brief \ru Производные по uv в вершинах.
           \en Derivative by uv at vertices. \~
  \details \ru Производные по uv в вершинах. \n
           \en Derivative by uv at vertices. \n \~
*/
// ---
struct CoonsDerivesUV {
  MbVector3D firstUV0[COONS_COUNT]; ///< \ru Производные в началах кривых производных. \en Derivatives in the beginning curves of derivatives.
  MbVector3D firstUV1[COONS_COUNT]; ///< \ru Производные в концая кривых производных. \en Derivatives at the ends of derivative curves.

  /// \ru Конструктор. \en Constructor.
  CoonsDerivesUV()
  {
    SetZero();
  }

  /// \ru Обнулить координаты векторов. \en Set coordinates of vectors to zero.
  void SetZero() {
    for ( size_t i = 0; i < COONS_COUNT; ++i ) {
      firstUV0[i].SetZero();
      firstUV1[i].SetZero();
    }
  }

  OBVIOUS_PRIVATE_COPY( CoonsDerivesUV )
};


//------------------------------------------------------------------------------
/** \brief \ru Поверхность Кунса на четырех кривых.
           \en Coons surface on four curves. \~
  \details \ru Бикубическая поверхность Кунса определяется четырьмя кривыми и 
    производными поверхности на этих кривых в поперечном к кривым направлениях. 
    Поверхность проходит через определяющие её кривые и 
    имеет заданные производные на этих кривых в поперечном к кривым направлениях. \n
           \en Bicubic Coons surface is determined by four curves and 
    surface derivatives on these curves in transverse directions to curves. 
    Surface passes through its determining curves and 
    has specified derivatives on this curves in transverse directions to curves. \n \~
  \ingroup Surfaces
*/
// ---
class MATH_CLASS MbCoonsPatchSurface : public MbSurface {

//                                                      curve2V
//                                             t2min    curve2    t2max
//                                           P3 ______________________ P2
//                                       t3max |                      | t1max
//                                             |                      |
//                                             |                      |
//                                     curve3  |                      | curve1
//                                     curve3U |                      | curve1U
//     t0=t0min*(1-u)+t0max*u                  |                      |
//     t1=t1min*(1-v)+t1max*v                  |                      |
//     t2=t2min*(1-u)+t2max*u            t3min |______________________| t1min
//     t3=t3min*(1-v)+t3max*v                P0                        P1
//                                             t0min    curve0    t0max
//                                                      curve0V
// \ru Не переименовывать в MbCoonsSurface - хэш совпал с существующим объектом (BUG_60351). \en No renaming to MbCoonsSurface - hash was coincided with existing object (BUG_60351). 

private:
  MbCurve3D *   curve0;  ///< \ru Кривая  0. \en Curve 0. 
  MbCurve3D *   curve1;  ///< \ru Кривая  1. \en Curve 1. 
  MbCurve3D *   curve2;  ///< \ru Кривая  2. \en Curve 2. 
  MbCurve3D *   curve3;  ///< \ru Кривая  3. \en Curve 3. 
  MbCurve3D *   curve0V; ///< \ru Производная по v вдоль кривой  0. \en Derivative by v along curve 0. 
  MbCurve3D *   curve1U; ///< \ru Производная по u вдоль кривой  1. \en Derivative by u along curve 1. 
  MbCurve3D *   curve2V; ///< \ru Производная по v вдоль кривой  2. \en Derivative by v along curve 2. 
  MbCurve3D *   curve3U; ///< \ru Производная по u вдоль кривой  3. \en Derivative by u along curve 3. 
  MbCartPoint3D vertex[COONS_COUNT];   ///< \ru Вершины. \en Vertices. 
  MbCartPoint3D vertexU[COONS_COUNT];  ///< \ru Производная по u в вершинах. \en Derivative by u at vertices. 
  MbCartPoint3D vertexV[COONS_COUNT];  ///< \ru Производная по v в вершинах. \en Derivative by v at vertices. 
  MbCartPoint3D vertexUV[COONS_COUNT]; ///< \ru Производная по uv в вершинах. \en Derivative by uv at vertices. 
  double        t0min;   ///< \ru Минимальное значение параметра на кривой  0. \en Minimal value of parameter on curve 0. 
  double        t0max;   ///< \ru Максимальное значение параметра на кривой 0. \en Maximal value of parameter on curve 0. 
  double        t1min;   ///< \ru Минимальное значение параметра на кривой  1. \en Minimal value of parameter on curve 1. 
  double        t1max;   ///< \ru Максимальное значение параметра на кривой 1. \en Maximal value of parameter on curve 1. 
  double        t2min;   ///< \ru Минимальное значение параметра на кривой  2. \en Minimal value of parameter on curve 2. 
  double        t2max;   ///< \ru Максимальное значение параметра на кривой 2. \en Maximal value of parameter on curve 2. 
  double        t3min;   ///< \ru Минимальное значение параметра на кривой  3. \en Minimal value of parameter on curve 3. 
  double        t3max;   ///< \ru Максимальное значение параметра на кривой 3. \en Maximal value of parameter on curve 3. 
  bool          uclosed; ///< \ru Замкнутость по u. \en Closeness by u. 
  bool          vclosed; ///< \ru Замкнутость по v. \en Closeness by v. 
  bool          poleUMin;   ///< \ru Полюс в начале. \en Pole at the beginning. 
  bool          poleUMax;   ///< \ru Полюс в конце. \en Pole at the end. 
  bool          poleVMin;   ///< \ru Полюс в начале. \en Pole at the beginning. 
  bool          poleVMax;   ///< \ru Полюс в конце. \en Pole at the end. 
  MbeCoonsSurfaceCalcType calcType;  ///< \ru Версия реализации определяет способ расчёта поверхности. \en Version of implementation determines a type of calculation of surface.
  DPtr<CoonsDerivesUV> derivesUV; ///< \ru Производные в началах и концах кривых производных. \en Derivatives in the beginning and ends curves of derivatives.

  //------------------------------------------------------------------------------
  /** \brief \ru Вспомогательные данные.
             \en Auxiliary data. \~
    \details \ru Вспомогательные данные служат для ускорения работы объекта.
             \en Auxiliary data are used for fast calculations. \n \~
  */
  // ---
  class MbCoonsSurfaceAuxiliaryData: public AuxiliaryData {
  public:
     DPtr<MbCoonsWorkingData> mp;   ///< \ru Дополнительные временные данные для ускорения вычислений. \en Additional temporary data to speed up computations. 
     MbCoonsSurfaceAuxiliaryData();
     MbCoonsSurfaceAuxiliaryData( const MbCoonsSurfaceAuxiliaryData & init );
     virtual ~MbCoonsSurfaceAuxiliaryData();
     MbCoonsSurfaceAuxiliaryData & operator = ( const MbCoonsSurfaceAuxiliaryData & init );
  };
  mutable CacheManager<MbCoonsSurfaceAuxiliaryData> cache;

protected:
  /// \ru Конструктор поверхности Кунса по умолчанию. \en Default constructor of Coons surface.
  MbCoonsPatchSurface ();
private:
  MbCoonsPatchSurface( const MbCoonsPatchSurface & ); // \ru Не реализовано. \en Not implemented. 
  MbCoonsPatchSurface( const MbCoonsPatchSurface &, MbRegDuplicate * ); ///< \ru Конструктор копирования. \en Copy-constructor. 
public:
  virtual ~MbCoonsPatchSurface( void );

public:
  VISITING_CLASS( MbCoonsPatchSurface );

  /// \ru Создание поверхности Кунса по заданным кривым на поверхностях. \en Creation of Coons surface by curves on surfaces. 
  static  MbCoonsPatchSurface * Create( const MbCurve3D & curve0,
                                        const MbCurve3D & curve1,
                                        const MbCurve3D & curve2,
                                        const MbCurve3D & curve3,
                                              MbeCoonsSurfaceCalcType type,
                                              MbResultType & resType );

  /// \ru Инициализация поверхности Кунса заданной поверхностью Кунса. \en Initialization of Coons surface by specified Coons surface. 
  void    Init( const MbCoonsPatchSurface & );
  /// \ru Инициализация поверхности Кунса по заданным кривым на поверхностях. \en Initialization of Coons surface by curves on surfaces.
  bool    Init( const MbCurve3D &             crv0,
                        const MbCurve3D &             crv1,
                        const MbCurve3D &             crv2,
                        const MbCurve3D &             crv3,
                              MbeCoonsSurfaceCalcType type );

  /** \ru \name Общие функции геометрического объекта
      \en \name Common functions of a geometric object
      \{ */
  MbeSpaceType  IsA() const override; // \ru Тип элемента \en A type of element 
  MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr )  const override; // \ru Сделать копию элемента \en Make a copy of element
  bool    IsSame( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const override;   // \ru Является ли объект копией. \en Whether the object is a copy. 
  bool    SetEqual( const MbSpaceItem & init ) override;        // \ru Сделать равным \en Make equal 
  bool    IsSimilar( const MbSpaceItem & init ) const override; // \ru Являются ли объекты подобными \en Whether the objects are similar 
  void    Transform( const MbMatrix3D & matr, MbRegTransform * = nullptr ) override; // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void    Move( const MbVector3D & to, MbRegTransform * = nullptr ) override;                 // \ru Сдвиг \en Translation
  void    Rotate( const MbAxis3D & axis, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate around an axis

  void    GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void    SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of the object 
  void    GetBasisItems ( RPArray <MbSpaceItem> &s ) override; // \ru Дать базовые объекты \en Get the base objects 
  void    GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void    SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  /** \} */

  /** \ru \name Функции описания области определения поверхности
      \en \name Functions for surface domain description
      \{ */
  double  GetUMin() const override;
  double  GetVMin() const override;
  double  GetUMax() const override;
  double  GetVMax() const override;
  bool    IsUClosed() const override; // \ru Замкнута ли поверхность по параметру u. \en Whether the surface is closed by parameter u. 
  bool    IsVClosed() const override; // \ru Замкнута ли поверхность по параметру v. \en Whether the surface is closed by parameter v. 
  bool    GetPoleUMin() const override;
  bool    GetPoleUMax() const override;
  bool    GetPoleVMin() const override;
  bool    GetPoleVMax() const override;
  bool    IsPole( double u, double v, double paramPrecision = PARAM_PRECISION ) const override; // \ru Является ли точка особенной \en Whether the point is special 
  void    Refresh() override;                          // \ru Сбросить все временные данные \en Flush all the temporary data 
  /** \} */

  /** \ru \name Функции для работы в области определения поверхности
            Функции PointOn, Derive... поверхностей корректируют параметры
            при выходе их за пределы прямоугольной области определения параметров.\n
      \en \name Functions for working at surface domain
            Functions PointOn, Derive... of surfaces correct parameters
            when they are out of bounds of rectangular domain of parameters.\n
      \{ */
  void    PointOn  ( double & u, double & v, MbCartPoint3D & ) const override; // \ru Точка на поверхности \en Point on the surface 
  void    DeriveU  ( double & u, double & v, MbVector3D & ) const override; // \ru Первая производная по u \en First derivative with respect to u 
  void    DeriveV  ( double & u, double & v, MbVector3D & ) const override; // \ru Первая производная по v \en First derivative with respect to v 
  void    DeriveUU ( double & u, double & v, MbVector3D & ) const override; // \ru Вторая производная по u \en Second derivative with respect to u 
  void    DeriveVV ( double & u, double & v, MbVector3D & ) const override; // \ru Вторая производная по v \en Second derivative with respect to v 
  void    DeriveUV ( double & u, double & v, MbVector3D & ) const override; // \ru Вторая производная по uv \en Second derivative with respect to uv 
  void    DeriveUUU( double & u, double & v, MbVector3D & ) const override; // \ru Третья производная \en Third derivative 
  void    DeriveUUV( double & u, double & v, MbVector3D & ) const override; // \ru Третья производная \en Third derivative 
  void    DeriveUVV( double & u, double & v, MbVector3D & ) const override; // \ru Третья производная \en Third derivative 
  void    DeriveVVV( double & u, double & v, MbVector3D & ) const override; // \ru Третья производная \en Third derivative 
  void    Normal   ( double & u, double & v, MbVector3D & ) const override; // \ru Нормаль \en Normal 
  /** \} */

  /** \ru \name Функции для работы внутри и вне области определения поверхности
            функции _PointOn, _Derive... поверхностей не корректируют
            параметры при выходе их за пределы прямоугольной области определения параметров.
      \en \name Functions for working inside and outside the surface's domain
            functions _PointOn, _Derive... of surfaces don't correct
            parameters when they are out of bounds of rectangular domain of parameters.
      \{ */
  void   _PointOn  ( double  u, double  v, MbCartPoint3D & ) const override; // \ru Точка на расширенной поверхности \en Point on the extended surface
  void   _DeriveU  ( double  u, double  v, MbVector3D & ) const override; // \ru Первая производная по u \en First derivative with respect to u
  void   _DeriveV  ( double  u, double  v, MbVector3D & ) const override; // \ru Первая производная по v \en First derivative with respect to v
  void   _DeriveUU ( double  u, double  v, MbVector3D & ) const override; // \ru Вторая производная по u \en Second derivative with respect to u
  void   _DeriveVV ( double  u, double  v, MbVector3D & ) const override; // \ru Вторая производная по v \en Second derivative with respect to v
  void   _DeriveUV ( double  u, double  v, MbVector3D & ) const override; // \ru Вторая производная по uv \en Second derivative with respect to uv
  void   _DeriveUUU( double  u, double  v, MbVector3D & ) const override; // \ru Третья производная \en Third derivative
  void   _DeriveUUV( double  u, double  v, MbVector3D & ) const override; // \ru Третья производная \en Third derivative
  void   _DeriveUVV( double  u, double  v, MbVector3D & ) const override; // \ru Третья производная \en Third derivative
  void   _DeriveVVV( double  u, double  v, MbVector3D & ) const override; // \ru Третья производная \en Third derivative
  void   _Normal   ( double  u, double  v, MbVector3D & ) const override; // \ru Нормаль \en Normal

  /// \ru Вычислить производную нормали на расширенной поверхности. \en Calculate derivative of normal vector on extended surface. 
  void   _NormalU( double  u, double  v, MbVector3D & ) const override;
  /// \ru Вычислить производную нормали на расширенной поверхности. \en Calculate derivative of normal vector on extended surface. 
  void   _NormalV( double  u, double  v, MbVector3D & ) const override;
  /// \ru Вычислить вторую производную нормали на расширенной поверхности. \en Calculate second derivative of normal vector on extended surface. 
  void   _NormalUU( double  u, double  v, MbVector3D & ) const override;
  /// \ru Вычислить вторую производную нормали на расширенной поверхности. \en Calculate second derivative of normal vector on extended surface. 
  void   _NormalUV( double  u, double  v, MbVector3D & ) const override;
  /// \ru Вычислить вторую производную нормали на расширенной поверхности. \en Calculate second derivative of normal vector on extended surface. 
  void   _NormalVV( double  u, double  v, MbVector3D & ) const override;
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
  double  StepU( double u, double v, double sag ) const override; // \ru Вычисление шага параметра u по по величине прогиба \en Calculation of parameter u step by the value of sag
  double  StepV( double u, double v, double sag ) const override; // \ru Вычисление шага параметра v по по величине прогиба \en Calculation of parameter v step by the value of sag
  double  DeviationStepU( double u, double v, double ang ) const override; // \ru Вычисление шага параметра u по углу отклонения нормали \en Calculation of parameter u step by the angle of deviation of normal
  double  DeviationStepV( double u, double v, double ang ) const override; // \ru Вычисление шага параметра v по углу отклонения нормали \en Calculation of parameter v step by the angle of deviation of normal
  size_t  GetUCount() const override;
  size_t  GetVCount() const override;
  /** \} */

  /** \ru \name Общие функции поверхности
      \en \name Common functions of surface
      \{ */
  MbCurve3D * CurveU( double v, MbRect1D * pRgn, bool bApprox = true ) const override; // \ru Пространственная копия линии v = const \en Spatial copy of 'v = const'-line
  MbCurve3D * CurveV( double u, MbRect1D * pRgn, bool bApprox = true ) const override; // \ru Пространственная копия линии u = const \en Spatial copy of 'u = const'-line

  void    ChangeCarrier( const MbSpaceItem & item, MbSpaceItem & init ) override; // \ru Изменение носителя \en Changing of carrier 
  // \ru Существует ли полюс на границе параметрической области \en Whether there is pole on boundary of parametric region 
  double  GetFilletRadius( const MbCartPoint3D & p, double accuracy = METRIC_REGION ) const override; // \ru Является ли поверхность скруглением. \en Whether a surface is fillet. 

  size_t  GetUMeshCount() const override; // \ru Выдать количество полигонов по u \en Get the count of polygons by u 
  size_t  GetVMeshCount() const override; // \ru Выдать количество полигонов по v \en Get the count of polygons by v 

  /// \ru Получить кривую 0. \en Get curve 0. 
  const   MbCurve3D & GetCurve0() const { return *curve0; } 
  /// \ru Получить кривую 1. \en Get curve 1. 
  const   MbCurve3D & GetCurve1() const { return *curve1; } 
  /// \ru Получить кривую 2. \en Get curve 2. 
  const   MbCurve3D & GetCurve2() const { return *curve2; } 
  /// \ru Получить кривую 3. \en Get curve 3. 
  const   MbCurve3D & GetCurve3() const { return *curve3; } 
  /// \ru Получить кривую производной в трансверсальном направлении к кривой 0. \en Get derivative curve transversal to curve 0.
  const   MbCurve3D & GetDerCurve0() const { return *curve0V; } 
  /// \ru Получить кривую производной в трансверсальном направлении к кривой 1. \en Get derivative curve transversal to curve 1.
  const   MbCurve3D & GetDerCurve1() const { return *curve1U; } 
  /// \ru Получить кривую производной в трансверсальном направлении к кривой 2. \en Get derivative curve transversal to curve 2.
  const   MbCurve3D & GetDerCurve2() const { return *curve2V; } 
  /// \ru Получить кривую производной в трансверсальном направлении к кривой 3. \en Get derivative curve transversal to curve 3.
  const   MbCurve3D & GetDerCurve3() const { return *curve3U; } 
  /// \ru Получить кривую по индексу. \en Get curve by an index. 
  const   MbCurve3D * GetCurve( size_t ind ) const; 
  /// \ru Получить кривую в параметрах поверхности по индексу (удалить после использования). \en Get curve in surface parameters by an index (delete after use). 
          MbCurve *   GetCurve2D( size_t ind ) const;

  /// \ru Получить количество кривых. \en Get count of curves. 
  size_t  GetCurvesCount() const { return COONS_COUNT; } //-V112
  const   MbCartPoint3D * GetVertex() const { return vertex; } ///< \ru Выдать вершины P0, P1, P2. \en Get vertices P0, P1, P2. 
  MbeCoonsSurfaceCalcType GetCalcType() const { return calcType;  } ///< \ru Выдать способ расчёта поверхности. \en Get surface calculation type.
  /** \} */
  double  GetT0Min() const { return t0min; } ///< \ru Минимальное значение параметра на кривой  0. \en Minimal value of parameter on curve 0. 
  double  GetT0Max() const { return t0max; } ///< \ru Максимальное значение параметра на кривой 0. \en Maximal value of parameter on curve 0. 
  double  GetT1Min() const { return t1min; } ///< \ru Минимальное значение параметра на кривой  1. \en Minimal value of parameter on curve 1. 
  double  GetT1Max() const { return t1max; } ///< \ru Максимальное значение параметра на кривой 1. \en Maximal value of parameter on curve 1. 
  double  GetT2Min() const { return t2min; } ///< \ru Минимальное значение параметра на кривой  2. \en Minimal value of parameter on curve 2. 
  double  GetT2Max() const { return t2max; } ///< \ru Максимальное значение параметра на кривой 2. \en Maximal value of parameter on curve 2. 
  double  GetT3Min() const { return t3min; } ///< \ru Минимальное значение параметра на кривой  3. \en Minimal value of parameter on curve 3. 
  double  GetT3Max() const { return t3max; } ///< \ru Максимальное значение параметра на кривой 3. \en Maximal value of parameter on curve 3. 

 /** \brief \ru Получить образующую кривую по индексу, если она точно совпадает с соответствующим краем поверхности.
            \en Get exact curve by index, if it coincides with the corresponding border of the surface. \~
   \details \ru Совпадение кривой с краем поверхности определяется по крайним точкам кривой.
            \en Coincidence of the curve with the border of the surface is determined by the end points of the curve. \~
   \param[in] k - \ru Индекс кривой.
                  \en Index of the curve. \~
   \param[out] sense - \ru Флаг совпадения направленности кривой с рисунком, приведенным выше.
                       \en Flag that indicates the coincidence of the curve with the picture shown above.\~
   \return - \ru Указатель на кривую или nullptr.
             \en Pointer to the curve or nullptr. \~
 */
  const   MbCurve3D * GetExactCurve( size_t k, bool & sense ) const;

  /** \brief \ru Проверка полюсов на кривых.
             \en Check poles on curves. \~
      \details \ru Определяет, есть ли полюс на границе области определения по длине кривой, определяющей границу.\n
      Результат вычислений можно получить с помощью функций GetPoleUMin, GetPoleUMax, GetPoleVMin, GetPoleVMax.
               \en Determines whether the pole at domain boundary by curve length determining boundary.\n
      Result of calculations can be obtained with help of GetPoleUMin, GetPoleUMax, GetPoleVMin, GetPoleVMax functions. \~ 
  */
  void    CheckPole(); 

private:
  void    operator = ( const MbCoonsPatchSurface & ); // \ru Не реализовано. \en Not implemented. 
  void    Setup();
  void    SetupUVDerives();          
  void    CheckParams( double & u, double & v, bool ext = false ) const; // \ru Проверить и изменить при необходимости параметры. \en Check and correct parameters. 
  void    CalculateTwist( double u, double v ) const;
          // \ru Определение местных координат. \en Determination of local coordinates. 
  void    CalculateCoordinate( double & u, double & v,
                                       double & t0, double & t1, double & t2, double & t3 ) const;
  void    CalculatePoint ( double & u, double & v,
                                   MbCartPoint3D * point, MbCartPoint3D * pointUV ) const;
  void    CalculateFirst ( double & u, double & v,
                                   MbCartPoint3D * point, MbVector3D * first, 
                                   MbCartPoint3D * pointUV, MbVector3D * firstUV ) const;
  void    CalculateThird ( double & u, double & v,
                                   MbCartPoint3D * point, MbVector3D * third,
                                   MbCartPoint3D * pointUV, MbVector3D * thirdUV ) const;
  void    CalculateExplore( double & u, double & v,
                                    MbCartPoint3D * point, MbVector3D * first, MbVector3D * second, 
                                    MbCartPoint3D * pointUV, MbVector3D * firstUV, MbVector3D * secondUV ) const;
          // \ru Производные. \en Derivatives with respect to u and to v.
  void    Derivatives( double & u, double & v, MbVector3D & uDer, MbVector3D & vDer ) const; 
          // \ru Нормаль. \en Calculate surface normal with refinement on borders. 
  void    Normal( double u, double v, MbVector3D & derU, MbVector3D & derV, MbVector3D & norm ) const;
  inline  void    ParamPoint ( double w, double * t ) const;
  inline  void    ParamFirst ( double w, double * t ) const;
  inline  void    ParamSecond( double w, double * t ) const;
  inline  void    ParamThird ( double w, double * t ) const;
          // \ru Добавить матрицу поверхности. \en Add the matrix of the surface.
  inline  void    AddMatrix  ( double u, double v, double * uu, double * vv, MbVector3D & p ) const;

  void    AddDeriveU  ( double u, double v, MbVector3D & p ) const;
  void    AddDeriveV  ( double u, double v, MbVector3D & p ) const;
  void    AddDeriveUU ( double u, double v, MbVector3D & p ) const;
  void    AddDeriveVV ( double u, double v, MbVector3D & p ) const;
  void    AddDeriveUV ( double u, double v, MbVector3D & p ) const;
  void    AddDeriveUUU( double u, double v, MbVector3D & p ) const;
  void    AddDeriveUUV( double u, double v, MbVector3D & p ) const;
  void    AddDeriveUVV( double u, double v, MbVector3D & p ) const;
  void    AddDeriveVVV( double u, double v, MbVector3D & p ) const;

  void    SpecifyNormalOnPole( MbVector3D & norm ) const;

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbCoonsPatchSurface )
}; // MbCoonsSurface

IMPL_PERSISTENT_OPS( MbCoonsPatchSurface )


//------------------------------------------------------------------------------
// \ru Определение массива степеней параметра точки \en Determination of array of degrees of point parameter 
// ---
inline void MbCoonsPatchSurface::ParamPoint ( double w, double * t ) const {
  t[0] = 1.0 - 3.0 * w * w + 2.0 * w * w * w; //*/ 1.0 - 10.0 * w * w * w + 15.0 * w * w * w * w - 6.0 * w * w * w * w * w;
  t[1] =       3.0 * w * w - 2.0 * w * w * w; //*/       10.0 * w * w * w - 15.0 * w * w * w * w + 6.0 * w * w * w * w * w;
  t[2] = w   - 2.0 * w * w +       w * w * w; //*/ 0.0;
  t[3] =     -       w * w +       w * w * w; //*/ 0.0;
}


//------------------------------------------------------------------------------
// \ru Определение массива степеней параметра производной \en Determination of array of degrees of derivative parameter 
// ---
inline void MbCoonsPatchSurface::ParamFirst ( double w, double * t ) const {
  t[0] =     - 6.0 * w + 6.0 * w * w; //*/ -30.0 * w * w + 60.0 * w * w * w - 30.0 * w * w * w * w;
  t[1] =       6.0 * w - 6.0 * w * w; //*/  30.0 * w * w - 60.0 * w * w * w + 30.0 * w * w * w * w;
  t[2] = 1.0 - 4.0 * w + 3.0 * w * w; //*/ 0.0;
  t[3] =     - 2.0 * w + 3.0 * w * w; //*/ 0.0;
}


//------------------------------------------------------------------------------
// \ru Определение массива степеней параметра второй производной \en Determination of array of degrees of second derivative parameter 
// ---
inline void MbCoonsPatchSurface::ParamSecond( double w, double * t ) const {
  t[0] = - 6.0 + 12.0 * w; //*/ -60.0 * w + 180.0 * w * w - 120.0 * w * w * w;
  t[1] =   6.0 - 12.0 * w; //*/  60.0 * w - 180.0 * w * w + 120.0 * w * w * w;
  t[2] = - 4.0 + 6.0  * w; //*/  0.0;
  t[3] = - 2.0 + 6.0  * w; //*/  0.0;
}


//------------------------------------------------------------------------------
// \ru Определение массива степеней параметра третьей производной \en Determination of array of degrees of third derivative parameter 
// ---
inline void MbCoonsPatchSurface::ParamThird ( double /*w*/, double * t ) const {
  t[0] =  12.0; //*/ -60.0 + 360.0 * w - 360.0 * w * w;
  t[1] = -12.0; //*/  60.0 - 360.0 * w + 360.0 * w * w;
  t[2] =   6.0; //*/   0.0;
  t[3] =   6.0; //*/   0.0;
}


//------------------------------------------------------------------------------
// \ru Получить кривую по индексу \en Get curve by an index 
// ---
inline const MbCurve3D * MbCoonsPatchSurface::GetCurve( size_t ind ) const
{
  if ( ind >= COONS_COUNT )
    ind = ind % COONS_COUNT;
  switch ( ind ) {
    case 0 : { return curve0; }
    case 1 : { return curve1; }
    case 2 : { return curve2; }
    case 3 : { return curve3; }
  }
  return nullptr;
}


#endif // __SURF_COONS_SURFACE_H
