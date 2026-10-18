////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Билинейная поверхность на четырех кривых.
         \en Bilinear surface on four curves. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __SURF_COVER_SURFACE_H
#define __SURF_COVER_SURFACE_H


#include <surface.h>


#define COVER_COUNT 4 ///< \ru Число кривых, используемых для построения билинейной поверхности. \en Count of curves used to construct bilinear surface. 


//------------------------------------------------------------------------------
/** \brief \ru Четырёхугольная поверхность на кривых.
           \en Quadrangular surface on curves. \~
  \details \ru Билинейная поверхность на четырех кривых. \n
    Кривые должны попарно пересекаться или иметь точки скрещения. 
    Если кривые попарно пересекаются, то поверхность проходит через определяющиее её кривые. \n
           \en Bilinear surface on four curves. \n
    Curves have to be intersected pairwise or have crossing points. 
    If curves are intersected pairwise then surface passes through its determining curves. \n \~
  \ingroup Surfaces
*/
// ---
class MATH_CLASS MbCoverSurface : public MbSurface {

//                                             t2min    curve2    t2max
//              R(u,v) =                     P3 ______________________ P2
//   (curve0(t0) - P0*(1-u)) *(1-v)+     t3max |                      | t1max
//   (curve1(t1) - P1*(1-v)) * u   +           |                      |
//   (curve2(t2) - P2* u   ) * v   +           |                      |
//   (curve3(t3) - P3* v   ) *(1-u)     curve3 |           R          | curve1
//     t0=t0min*(1-u)+t0max*u                  |                      |
//     t1=t1min*(1-v)+t1max*v                  |                      |
//     t2=t2min*(1-u)+t2max*u            t3min |______________________| t1min
//     t3=t3min*(1-v)+t3max*v                P0                        P1
//                                             t0min    curve0    t0max

private:
  MbCurve3D   * curve0;     ///< \ru Кривая  0. \en Curve 0. 
  MbCurve3D   * curve1;     ///< \ru Кривая  1. \en Curve 1. 
  MbCurve3D   * curve2;     ///< \ru Кривая  2. \en Curve 2. 
  MbCurve3D   * curve3;     ///< \ru Кривая  3. \en Curve 3. 

  MbCartPoint3D vertex[COVER_COUNT];  ///< \ru Вершины \en Vertices 
  double        t0min;      ///< \ru Минимальное значение параметра на кривой  0. \en Minimal value of parameter on curve 0. 
  double        t0max;      ///< \ru Максимальное значение параметра на кривой 0. \en Maximal value of parameter on curve 0. 
  double        t1min;      ///< \ru Минимальное значение параметра на кривой  1. \en Minimal value of parameter on curve 1. 
  double        t1max;      ///< \ru Максимальное значение параметра на кривой 1. \en Maximal value of parameter on curve 1. 
  double        t2min;      ///< \ru Минимальное значение параметра на кривой  2. \en Minimal value of parameter on curve 2. 
  double        t2max;      ///< \ru Максимальное значение параметра на кривой 2. \en Maximal value of parameter on curve 2. 
  double        t3min;      ///< \ru Минимальное значение параметра на кривой  3. \en Minimal value of parameter on curve 3. 
  double        t3max;      ///< \ru Максимальное значение параметра на кривой 3. \en Maximal value of parameter on curve 3. 
  bool          uclosed;    ///< \ru Замкнутость поверхности по u. \en Closedness of surface by u. 
  bool          vclosed;    ///< \ru Замкнутость поверхности по v. \en Closedness of surface by v. 
  bool          poleUMin;   ///< \ru Полюс в начале. \en Pole at the beginning. 
  bool          poleUMax;   ///< \ru Полюс в конце. \en Pole at the end. 
  bool          poleVMin;   ///< \ru Полюс в начале. \en Pole at the beginning. 
  bool          poleVMax;   ///< \ru Полюс в конце. \en Pole at the end. 

public:
  /** \brief \ru Конструктор билинейной поверхности.
             \en Constructor of bilinear surface. \~
      \details \ru Конструктор билинейной поверхности по набору кривых.
               \en Constructor of bilinear surface by set of curves. \~
      \param[in] initCurve0 - \ru Кривая 0.
                              \en Curve 0. \~
      \param[in] initCurve1 - \ru Кривая 1.
                              \en Curve 1. \~
      \param[in] initCurve2 - \ru Кривая 2.
                              \en Curve 2. \~
      \param[in] initCurve3 - \ru Кривая 3.
                              \en Curve 3. \~
  */
  MbCoverSurface ( const MbCurve3D & initCurve0, const MbCurve3D & initCurve1, 
                   const MbCurve3D & initCurve2, const MbCurve3D & initCurve3 );
private:
  MbCoverSurface( const MbCoverSurface & ); // \ru Не реализовано. \en Not implemented. 
  MbCoverSurface( const MbCoverSurface &, MbRegDuplicate * );
public:
  virtual ~MbCoverSurface( void );

public:
  VISITING_CLASS( MbCoverSurface );

  /// \ru Инициализация билинейной поверхности заданной билинейной поверхностью. \en Initialization of bilinear surface by given bilinear surface. 
  void    Init( const MbCoverSurface & );

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

  void    GetProperties( MbProperties & properties ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void    SetProperties( const MbProperties & properties ) override; // \ru Записать свойства объекта \en Set properties of the object 
  void    GetBasisItems ( RPArray<MbSpaceItem> & ) override;  // \ru Дать базовые объекты \en Get the base objects 
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
  // \ru Существует ли полюс на границе параметрической области \en Whether there is pole on boundary of parametric region 
  bool    GetPoleUMin() const override;
  bool    GetPoleUMax() const override;
  bool    GetPoleVMin() const override;
  bool    GetPoleVMax() const override;
  bool    IsPole( double u, double v, double paramPrecision = PARAM_PRECISION ) const override; // \ru Является ли точка особенной \en Whether the point is special 
  /** \} */

  /** \ru \name Функции для работы в области определения поверхности
            Функции PointOn, Derive... поверхностей корректируют параметры
            при выходе их за пределы прямоугольной области определения параметров.\n
      \en \name Functions for working at surface domain
            Functions PointOn, Derive... of surfaces correct parameters
            when they are out of bounds of rectangular domain of parameters.\n 
      \{ */
  void    PointOn  ( double & u, double & v, MbCartPoint3D & p ) const override; // \ru Точка на поверхности \en Point on the surface 
  void    DeriveU  ( double & u, double & v, MbVector3D & p ) const override; // \ru Первая производная по u \en First derivative with respect to u 
  void    DeriveV  ( double & u, double & v, MbVector3D & p ) const override; // \ru Первая производная по v \en First derivative with respect to v 
  void    DeriveUU ( double & u, double & v, MbVector3D & p ) const override; // \ru Вторая производная по u \en Second derivative with respect to u 
  void    DeriveVV ( double & u, double & v, MbVector3D & p ) const override; // \ru Вторая производная по v \en Second derivative with respect to v 
  void    DeriveUV ( double & u, double & v, MbVector3D & p ) const override; // \ru Вторая производная по uv \en Second derivative with respect to uv 
  void    DeriveUUU( double & u, double & v, MbVector3D & p ) const override; // \ru Третья производная \en Third derivative 
  void    DeriveUUV( double & u, double & v, MbVector3D & p ) const override; // \ru Третья производная \en Third derivative 
  void    DeriveUVV( double & u, double & v, MbVector3D & p ) const override; // \ru Третья производная \en Third derivative 
  void    DeriveVVV( double & u, double & v, MbVector3D & p ) const override; // \ru Третья производная \en Third derivative 
  void    Normal   ( double & u, double & v, MbVector3D & p ) const override; // \ru Нормаль \en Normal 
  /** \} */

  /** \ru \name Функции для работы внутри и вне области определения поверхности
            функции _PointOn, _Derive... поверхностей не корректируют
            параметры при выходе их за пределы прямоугольной области определения параметров.
      \en \name Functions for working inside and outside the surface's domain
            functions _PointOn, _Derive... of surfaces don't correct
            parameters when they are out of bounds of rectangular domain of parameters.
      \{ */
  void   _PointOn  ( double  u, double  v, MbCartPoint3D & p ) const override; // \ru Точка на расширенной поверхности \en Point on the extended surface 
  void   _DeriveU  ( double  u, double  v, MbVector3D & p ) const override; // \ru Первая производная по u \en First derivative with respect to u 
  void   _DeriveV  ( double  u, double  v, MbVector3D & p ) const override; // \ru Первая производная по v \en First derivative with respect to v 
  void   _DeriveUU ( double  u, double  v, MbVector3D & p ) const override; // \ru Вторая производная по u \en Second derivative with respect to u 
  void   _DeriveVV ( double  u, double  v, MbVector3D & p ) const override; // \ru Вторая производная по v \en Second derivative with respect to v 
  void   _DeriveUV ( double  u, double  v, MbVector3D & p ) const override; // \ru Вторая производная по uv \en Second derivative with respect to uv 
  void   _DeriveUUU( double  u, double  v, MbVector3D & p ) const override; // \ru Третья производная \en Third derivative 
  void   _DeriveUUV( double  u, double  v, MbVector3D & p ) const override; // \ru Третья производная \en Third derivative 
  void   _DeriveUVV( double  u, double  v, MbVector3D & p ) const override; // \ru Третья производная \en Third derivative 
  void   _DeriveVVV( double  u, double  v, MbVector3D & p ) const override; // \ru Третья производная \en Third derivative 
  void   _Normal   ( double  u, double  v, MbVector3D & p ) const override; // \ru Нормаль \en Normal 
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
  MbCurve3D * CurveU( double v, MbRect1D * pRgn, bool doApprox = true ) const override; // \ru Пространственная копия линии v = const \en Spatial copy of 'v = const'-line 
  MbCurve3D * CurveV( double u, MbRect1D * pRgn, bool doApprox = true ) const override; // \ru Пространственная копия линии u = const \en Spatial copy of 'u = const'-line 

  void    ChangeCarrier( const MbSpaceItem & item, MbSpaceItem & init ) override; // \ru Изменение носителя \en Changing of carrier 
  // \ru Подобные ли поверхности для объединения (слива). \en Whether the surfaces to union (joining) are similar. 
  bool    IsSimilarToSurface( const MbSurface & surf, VERSION version, double precision = METRIC_PRECISION ) const override;
  // \ru Дать двумерную матрицу преобразования из своей параметрической области в параметрическую область surf. \en Get two-dimensional transformation matrix from own parametric region to parametric region of 'surf'. 
  bool    GetMatrixToSurface( const MbSurface & surf, MbMatrix & matr, VERSION version, double precision = METRIC_PRECISION ) const override;
  // \ru Подобные ли поверхности для объединения (слива) \en Whether the surfaces to union (joining) are similar 
  bool    IsSpecialSimilarToSurface( const MbSurface & surf, VERSION version, double precision = METRIC_PRECISION ) const override; // \ru Специальный случай \en Special case 

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
  /// \ru Получить кривую по индексу. \en Get curve by an index. 
  const   MbCurve3D * GetCurve( size_t ind ) const; 
  /// \ru Получить кривую в параметрах поверхности по индексу (удалить после использования). \en Get curve in surface parameters by an index (delete after use). 
          MbCurve * GetCurve2D( size_t ind ) const;
          /// \ru Получить количество кривых. \en Get count of curves. 
  size_t  GetCurvesCount() const { return 4; } //-V112
  const   MbCartPoint3D * GetVertex() const { return vertex; } ///< \ru Выдать вершины P0, P1, P2. \en Get vertices P0, P1, P2. 
  double  GetT0Min() const { return t0min; } ///< \ru Минимальное значение параметра на кривой  0. \en Minimal value of parameter on curve 0. 
  double  GetT0Max() const { return t0max; } ///< \ru Максимальное значение параметра на кривой 0. \en Maximal value of parameter on curve 0. 
  double  GetT1Min() const { return t1min; } ///< \ru Минимальное значение параметра на кривой  1. \en Minimal value of parameter on curve 1. 
  double  GetT1Max() const { return t1max; } ///< \ru Максимальное значение параметра на кривой 1. \en Maximal value of parameter on curve 1. 
  double  GetT2Min() const { return t2min; } ///< \ru Минимальное значение параметра на кривой  2. \en Minimal value of parameter on curve 2. 
  double  GetT2Max() const { return t2max; } ///< \ru Максимальное значение параметра на кривой 2. \en Maximal value of parameter on curve 2. 
  double  GetT3Min() const { return t3min; } ///< \ru Минимальное значение параметра на кривой  3. \en Minimal value of parameter on curve 3. 
  double  GetT3Max() const { return t3max; } ///< \ru Максимальное значение параметра на кривой 3. \en Maximal value of parameter on curve 3. 
  double  GetTMin( size_t ind ) const; ///< \ru Минимальное  значение параметра на кривой с индексом ind. \en Get The minimal value of parameter on curve by index. 
  double  GetTMax( size_t ind ) const; ///< \ru Максимальное значение параметра на кривой с индексом ind. \en Get The maximal value of parameter on curve by index. 

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
  const   MbCurve3D * GetExactCurve( size_t k, bool &sense ) const;

  /** \brief \ru Проверка полюсов на кривых.
             \en Check poles on curves. \~
      \details \ru Определяет, есть ли полюс на границе области определения по длине кривой, определяющей границу.\n
      Результат вычислений можно получить с помощью функций GetPoleUMin, GetPoleUMax, GetPoleVMin, GetPoleVMax.
               \en Determines whether the pole at domain boundary by curve length determining boundary.\n
      Result of calculations can be obtained with help of GetPoleUMin, GetPoleUMax, GetPoleVMin, GetPoleVMax functions. \~ 
  */
  void    CheckPole(); 
  /** \brief \ru Корректировка параметров.
             \en Correct parameters. \~
      \details \ru Загоняет параметры, выходящие за область определения в область определения,\n 
      если поверхность замкнута по соответствующему параметру или параметр лежит за полюсом.
               \en Drives parameters leaving out of domain into domain\n 
      if the surface is closed by corresponding parameter or parameter lies behind a pole. \~
  */
  inline  void    CheckParam( double & u, double & v ) const; 

private:
  void    operator = ( const MbCoverSurface & ); // \ru Не реализовано. \en Not implemented. 
  bool    Init();
          // \ru Определение местных координат. \en Determination of local coordinates. 
  void    CalculateCoordinate( double & u, double & v, bool ext,
                                       double & t0, double & t1, double & t2, double & t3 ) const;
  void    CalculatePoint ( double & u, double & v, bool ext, MbCartPoint3D * point ) const; 
  void    CalculateFirst ( double & u, double & v, bool ext, MbCartPoint3D * point, MbVector3D * first ) const; 
  void    CalculateSecond( double & u, double & v, bool ext, MbVector3D * second ) const; 
  void    CalculateThird ( double & u, double & v, bool ext, MbVector3D * third ) const; 
  void    CalculateExplore( double & u, double & v, bool ext,
                            MbCartPoint3D * point, MbVector3D * first, MbVector3D * second ) const;

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbCoverSurface )
}; // MbCoverSurface

IMPL_PERSISTENT_OPS( MbCoverSurface )


//------------------------------------------------------------------------------
// \ru Получить кривую по индексу \en Get curve by an index 
// ---
inline const MbCurve3D * MbCoverSurface::GetCurve( size_t ind ) const
{
  if ( ind >= COVER_COUNT )
    ind = ind % COVER_COUNT;
  switch ( ind ) {
    case 0 : { return curve0; }
    case 1 : { return curve1; }
    case 2 : { return curve2; }
    case 3 : { return curve3; }
  }
  return nullptr;
}


//------------------------------------------------------------------------------
// \ru Минимальное  значение параметра на кривой с индексом ind. \en Get The minimal value of parameter on curve by index. 
// ---
inline double MbCoverSurface::GetTMin( size_t ind ) const
{
  if ( ind >= COVER_COUNT )
    ind = ind % COVER_COUNT;
  switch ( ind ) {
    case 0 : { return t0min; }
    case 1 : { return t1min; }
    case 2 : { return t2min; }
    case 3 : { return t3min; }
  }
  return UNDEFINED_DBL;
}


//------------------------------------------------------------------------------
// \ru Максимальное значение параметра на кривой с индексом ind. \en Get The maximal value of parameter on curve by index. 
// ---
inline double MbCoverSurface::GetTMax( size_t ind ) const
{
  if ( ind >= COVER_COUNT )
    ind = ind % COVER_COUNT;
  switch ( ind ) {
    case 0 : { return t0max; }
    case 1 : { return t1max; }
    case 2 : { return t2max; }
    case 3 : { return t3max; }
  }
  return UNDEFINED_DBL;
}


//------------------------------------------------------------------------------
// \ru Корректировка параметров \en Correct parameters 
// ---
inline void MbCoverSurface::CheckParam( double &u, double &v ) const 
{
  double umin = 0;
  double umax = 1;
  double vmin = 0;
  double vmax = 1;
  if ( uclosed ) {
    if ( (u < umin) || (u > umax ) ) {
      double tmp = umax - umin;
      u -= ::floor((u - umin) / tmp) * tmp; 
    }
  }
  else {
    if ( poleUMin && u<umin )
      u = umin;
    if ( poleUMax && u>umax )
      u = umax;
  }
  if ( vclosed ) {
    if ( (v < vmin) || (v > vmax ) ) {
      double tmp = vmax - vmin;
      v -= ::floor((v - vmin) / tmp) * tmp; 
    }
  }
  else {
    if ( poleVMin && v<vmin )
      v = vmin;
    if ( poleVMax && v>vmax )
      v = vmax;
  }
}


#endif // __SURF_COVER_SURFACE_H
