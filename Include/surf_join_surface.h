////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Поверхность соединения.
         \en The surface of the joint. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __SURF_JOIN_SURFACE_H
#define __SURF_JOIN_SURFACE_H


#include <surface.h>
#include <mb_operation_result.h>
#include <tool_multithreading.h>


class   MbSurfaceCurve;
class   MbProperties;


//------------------------------------------------------------------------------
/** \brief \ru Поверхность соединения.
           \en The surface of the joint. \~
  \details \ru Поверхность представляет собой сплайновую поверхность,
    натянутую на набор заданных однонаправленных кривых. 
    Кривые не должны пересекаться или касаться друг друга. 
    Касание или пересечение кривых допустимо только в конечных точках. 
    Координата u изменяется вдоль заданных кривых в соответствии с параметризацией каждой кривой. 
    Значение координаты v вдоль каждой кривой постоянно. 
    В направлении координаты v поверхность строится аналогично NURBS кривой с заданной степенью, узловым вектором и 
    использующей в качестве узлов точки заданных кривых, вычисленных с одинаковыми параметрами u.
    Поверхность используется для гладкого соединения краёв двух поверхностей.
           \en The surface is spline surface
    tensed on a set of the given unidirectional curves. 
    Curves shouldn't be intersected or concerned each other. 
    Tangency or intersection of curves is acceptable only at the end points. 
    Coordinate u is changed along given curves according to parameterization of each curve. 
    Value of coordinate v along each curve is constant. 
    In the direction of v coordinate the surface is constructed similar to NURBS curve with the given degree, a nodal vector and 
    points of the given curves calculated with identical parameters u are used as knots.
    Surface is used for smooth connection of boundaries of two surfaces. \~
  \ingroup Surfaces
*/
// ---
class MATH_CLASS MbJoinSurface : public MbSurface {
protected:
  RPArray<MbCurve3D> curves;     ///< \ru Набор кривых для построения поверхности. \en Set of curves to construct surface. 
  SArray<double>     knots;      ///< \ru Значения узлов для сплайна по v. \en Knot values for spline by v.  
  ptrdiff_t          degree;     ///< \ru Степень сплайна по v. \en Order of spline by v. 
  double             umin;       ///< \ru Минимальное значение параметра u. \en Minimal value of parameter u. 
  double             umax;       ///< \ru Максимальное значение параметра u. \en Maximal value of parameter u. 
  bool               closedU;    ///< \ru Замкнутость по u. \en Closedness by u. 
  bool               closedV;    ///< \ru Замкнутость по V. \en Closedness by V. 
  bool               isPoleUmin; ///< \ru Полюс при u == umin. \en Pole at u == umin. 
  bool               isPoleUmax; ///< \ru Полюс при u == umax. \en Pole at u == umax. 
  bool               isPoleVmin; ///< \ru Полюс при u == vmin. \en Pole at u == vmin. 
  bool               isPoleVmax; ///< \ru Полюс при u == vmax. \en Pole at u == vmax. 

private:
  //------------------------------------------------------------------------------
  /** \brief \ru Вспомогательные данные. 
             \en Auxiliary data. \~
    \details \ru Вспомогательные данные служат для ускорения работы объекта.
             \en Auxiliary data are used for fast calculations. \n \~
  */
  // ---
  class MbJoinSurfaceAuxiliaryData : public AuxiliaryData {
  public:
    double    calcU;     ///< \ru Последнее обработанное значение u. \en Last processed value of u.  
    double    calcV;     ///< \ru Последнее обработанное значение v. \en Last processed value of v. 
    ptrdiff_t lastIndex; ///< \ru Левый индекс узлового вектора из последних вычислений. \en Left index of knot vector from last calculations. 
    double ** points;    ///< \ru 2-х мерный массив для хранения данных по точкам для текущих вычислений. \en Two-dimensional array to store points data for current calculations. 
    double ** nMatrix;   ///< \ru Матрица коэффициентов для NURBS. \en Matrix of coefficients for NURBS. 
    SArray<MbCartPoint3D> tempPoints;  ///< \ru Множество рабочих точек. \en Set of working points. 
    SArray<MbVector3D>    tempVectors; ///< \ru Множество рабочих векторов. \en Set of working vectors. 
    SArray<MbVector3D>    readyData;   ///< \ru Множество для хранения вычисленных значений точки и производных. \en Set to store calculated values of point and derivatives. 

    // \ru Рабочие указатели для создания базисных сплайнов \en Working pointers for creation of basis splines 
    double * m_left;  ///< \ru Рабочие указатели для создания базисных сплайнов. \en Working pointers for creation of basis splines. 
    double * m_right; ///< \ru Рабочие указатели для создания базисных сплайнов. \en Working pointers for creation of basis splines. 
    ptrdiff_t *degree; ///< \ru Степень сплайна по v. \en Order of spline by v. 
    MbJoinSurfaceAuxiliaryData();
    MbJoinSurfaceAuxiliaryData( const MbJoinSurfaceAuxiliaryData & init );
    virtual ~MbJoinSurfaceAuxiliaryData();
    void    CreateVars();
    void    InitVars  ();
    void    FreeVars  ();
  private:
    void    operator = ( const MbJoinSurfaceAuxiliaryData & );
  };
  mutable CacheManager<MbJoinSurfaceAuxiliaryData> cache;

protected:
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbJoinSurface( const MbJoinSurface &, MbRegDuplicate * ); 
private:
  MbJoinSurface( const MbJoinSurface & ); // \ru Не реализовано. \en Not implemented. 
public:
  /** \brief \ru Конструктор поверхности соединения.
             \en Constructor of surface of the joint. \~
      \details \ru Конструктор поверхности соединения по набору кривых. Кривые должны быть непересекающиеся.\n 
      В конструкторе этот факт не проверяется.\n
               \en Constructor of surface of the joint by set of curves. Curves shouldn't be intersected.\n 
      In constructor this fact doesn't checked.\n \~
      \param[in] initCurves - \ru Список кривых, на которых натягивается поверхность.
                              \en List of of curves which the surface is tensed on. \~
      \param[in] sameCurves - \ru Определяет, надо ли делать копии кривых:\n
                 true - использовать в объекте пришедшие в конструктор кривые не дублируя,\n
                 false - использовать копии кривых.
                              \en Determines whether to copy curves:\n
                 true - use curves given in the constructor in object without copying,\n
                 false - use copies of curves. \~
  */
  MbJoinSurface( const RPArray<MbCurve3D> & initCurves, bool sameCurves );
  /** \brief \ru Конструктор поверхности соединения.
             \en Constructor of surface of the joint. \~
      \details \ru Конструктор поверхности соединения по набору кривых и порядку поверхности. Кривые должны быть непересекающиеся.\n 
      В конструкторе этот факт не проверяется.
               \en Constructor of surface of the joint by set of curves and order of surface. Curves shouldn't be intersected.\n 
      In constructor this fact doesn't checked. \~
      \param[in] initCurves - \ru Список кривых, на которых натягивается поверхность.
                              \en List of of curves which the surface is tensed on. \~
      \param[in] initDegree - \ru Порядок поверхности по v.
                              \en Surface order by v. \~ 
      \param[in] sameCurves - \ru Определяет, надо ли делать копии кривых:\n 
                 true - использовать в объекте пришедшие в конструктор кривые не дублируя,\n
                 false - использовать копии кривых.
                              \en Determines whether to copy curves:\n 
                 true - use curves given in the constructor in object without copying,\n
                 false - use copies of curves. \~
  */
  MbJoinSurface( const RPArray<MbCurve3D> & initCurves, ptrdiff_t initDegree, bool sameCurves );
  /** \brief \ru Конструктор поверхности соединения.
             \en Constructor of surface of the joint. \~
      \details \ru Конструктор поверхности соединения по набору кривых, порядку поверхности и узловому вектору.\n
      Кривые должны быть непересекающиеся. В конструкторе этот факт не проверяется.
               \en Constructor of surface of the joint by set of curves, order of surface and knot vector.\n
      Curves shouldn't be intersected. In constructor this fact doesn't checked. \~
      \param[in] initCurves - \ru Список кривых, на которых натягивается поверхность.
                              \en List of of curves which the surface is tensed on. \~
      \param[in] initDegree - \ru Порядок поверхности по v.
                              \en Surface order by v. \~ 
      \param[in] initKnots - \ru Узловой вектор по v.
                             \en A knot vector by v. \~
      \param[in] sameCurves - \ru Определяет, надо ли делать копии кривых:\n 
                 true - использовать в объекте пришедшие в конструктор кривые не дублируя,\n
                 false - использовать копии кривых.
                              \en Determines whether to copy curves:\n 
                 true - use curves given in the constructor in object without copying,\n
                 false - use copies of curves. \~
  */ 
  MbJoinSurface( const RPArray<MbCurve3D> & initCurves, ptrdiff_t initDegree, const SArray<double> & initKnots, bool sameCurves );

public:
  virtual ~MbJoinSurface();

public:
  VISITING_CLASS( MbJoinSurface );

public:

  /** \brief \ru Инициализация поверхности соединения.
              \en Initialization of surface of the joint. \~
      \details \ru Инициализация поверхности соединения по набору кривых. Кривые должны быть непересекающиеся.\n 
      Этот факт в функции не проверяется. Порядок поверхности не изменяется.
                \en Initialization of surface of the joint by set of curves. Curves shouldn't be intersected.\n 
      This fact isn't checked in the function. Surface order doesn't changed. \~
      \param[in] initCurves - \ru Список кривых, на которых натягивается поверхность.
                              \en List of of curves which the surface is tensed on. \~
      \param[in] sameCurves - \ru Определяет, надо ли делать копии кривых:\n
                  true - использовать в объекте пришедшие в конструктор кривые не дублируя,\n
                  false - использовать копии кривых.
                              \en Determines whether to copy curves:\n
                  true - use curves given in the constructor in object without copying,\n
                  false - use copies of curves. \~
  */

  void    Init( const RPArray<MbCurve3D> & initCurves, bool sameCurves ); 
  /** \brief \ru Инициализация поверхности соединения.
              \en Initialization of surface of the joint. \~
      \details \ru Инициализация поверхности соединения по набору кривых и порядку поверхности. Кривые должны быть непересекающиеся.\n 
      Этот факт в функции не проверяется. Порядок поверхности не изменяется.
                \en Initialization of surface of the joint by set of curves and order of surface. Curves shouldn't be intersected.\n 
      This fact isn't checked in the function. Surface order doesn't changed. \~
      \param[in] initCurves - \ru Список кривых, на которых натягивается поверхность.
                              \en List of of curves which the surface is tensed on. \~
      \param[in] initDegree - \ru Порядок поверхности по v.\n
                              \en Surface order by v.\n \~ 
      \param[in] sameCurves - \ru Определяет, надо ли делать копии кривых:\n
                  true - использовать в объекте пришедшие в конструктор кривые не дублируя,\n
                  false - использовать копии кривых.
                              \en Determines whether to copy curves:\n
                  true - use curves given in the constructor in object without copying,\n
                  false - use copies of curves. \~
  */
  bool    Init( const RPArray<MbCurve3D> & initCurves, ptrdiff_t initDegree, bool sameCurves ); 

  /** \brief \ru Инициализация поверхности соединения.
              \en Initialization of surface of the joint. \~
      \details \ru Инициализация поверхности соединения по набору кривых, порядку поверхности и узловому вектору.\n 
      Кривые должны быть непересекающиеся. Этот факт в функции не проверяется. Порядок поверхности не изменяется.
                \en Initialization of surface of the joint by set of curves, order of surface and knot vector.\n 
      Curves shouldn't be intersected. This fact isn't checked in the function. Surface order doesn't changed. \~
      \param[in] initCurves - \ru Список кривых, на которых натягивается поверхность.
                              \en List of of curves which the surface is tensed on. \~
      \param[in] initDegree - \ru Порядок поверхности по v.
                              \en Surface order by v. \~ 
      \param[in] initKnots - \ru Узловой вектор по v.
                              \en A knot vector by v. \~
      \param[in] sameCurves - \ru Определяет, надо ли делать копии кривых:\n 
                  true - использовать в объекте пришедшие в конструктор кривые не дублируя,\n
                  false - использовать копии кривых.
                              \en Determines whether to copy curves:\n 
                  true - use curves given in the constructor in object without copying,\n
                  false - use copies of curves. \~
  */
  bool    Init( const RPArray<MbCurve3D> & initCurves, ptrdiff_t initDegree, const SArray<double> & initKnots, bool sameCurves ); 

  /** \ru \name Общие функции геометрического объекта
      \en \name Common functions of a geometric object
      \{ */
  MbeSpaceType  IsA()  const override;                                                   // \ru Тип элемента \en A type of element
  MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const override;                     // \ru Сделать копию элемента \en Create a copy of the element
  bool    IsSame   ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const override; // \ru Являются ли объекты равными \en Determine whether objects are equal 
  bool    SetEqual ( const MbSpaceItem & ) override;                                     // \ru Сделать равным \en Make equal 
  void    Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override;             // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void    Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override;             // \ru Сдвиг \en Translation
  void    Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate around an axis

  void    GetProperties( MbProperties & properties ) override; // \ru Выдать свойства объекта \en Get properties of the object
  void    SetProperties( const MbProperties & properties ) override; // \ru Записать свойства объекта \en Set properties of the object 
  void    GetBasisItems ( RPArray<MbSpaceItem>   & ) override;  // \ru Дать базовые объекты \en Get the base objects 
  void    GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void    SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.
  /** \} */

  /** \ru \name Функции описания области определения поверхности
      \en \name Functions for surface domain description
      \{ */
  double  GetUMin()   const override; // \ru Минимальное значение параметра u \en Minimal value of parameter u
  double  GetVMin()   const override; // \ru Минимальное значение параметра v \en Minimal value of parameter v
  double  GetUMax()   const override; // \ru Максимальное значение параметра u \en Maximal value of parameter u
  double  GetVMax()   const override; // \ru Максимальное значение параметра v \en Maximal value of parameter v 
  bool    IsUClosed() const override; // \ru Замкнута ли поверхность по параметру u. \en Whether the surface is closed by parameter u. 
  bool    IsVClosed() const override; // \ru Замкнута ли поверхность по параметру v. \en Whether the surface is closed by parameter v. 
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
  void    PointOn  ( double & u, double & v, MbCartPoint3D & )  const override; // \ru Точка на поверхности \en Point on the surface
  void    DeriveU  ( double & u, double & v, MbVector3D & )     const override; // \ru Первая производная по u \en First derivative with respect to u
  void    DeriveV  ( double & u, double & v, MbVector3D & )     const override; // \ru Первая производная по v \en First derivative with respect to v
  void    DeriveUU ( double & u, double & v, MbVector3D & )     const override; // \ru Вторая производная по u \en Second derivative with respect to u
  void    DeriveVV ( double & u, double & v, MbVector3D & )     const override; // \ru Вторая производная по v \en Second derivative with respect to v
  void    DeriveUV ( double & u, double & v, MbVector3D & )     const override; // \ru Вторая производная \en The second derivative
  void    DeriveUUU( double & u, double & v, MbVector3D & )     const override; // \ru Третья производная \en Third derivative
  void    DeriveUUV( double & u, double & v, MbVector3D & )     const override; // \ru Третья производная \en Third derivative
  void    DeriveUVV( double & u, double & v, MbVector3D & )     const override; // \ru Третья производная \en Third derivative
  void    DeriveVVV( double & u, double & v, MbVector3D & )     const override; // \ru Третья производная \en Third derivative
  void    Normal   ( double & u, double & v, MbVector3D & )     const override; // \ru Нормаль \en Normal
  /** \} */

  /** \ru \name Функции для работы внутри и вне области определения поверхности
            функции _PointOn, _Derive... поверхностей не корректируют
            параметры при выходе их за пределы прямоугольной области определения параметров.
      \en \name Functions for working inside and outside the surface's domain
            functions _PointOn, _Derive... of surfaces don't correct
            parameters when they are out of bounds of rectangular domain of parameters. 
      \{ */
  void   _PointOn  ( double u, double v, MbCartPoint3D & p ) const override; // \ru Точка на поверхности \en Point on the surface
  void   _DeriveU  ( double u, double v, MbVector3D & p ) const override; // \ru Первая производная по u \en First derivative with respect to u
  void   _DeriveV  ( double u, double v, MbVector3D & p ) const override; // \ru Первая производная по v \en First derivative with respect to v
  void   _DeriveUU ( double u, double v, MbVector3D & p ) const override; // \ru Вторая производная по u \en Second derivative with respect to u
  void   _DeriveVV ( double u, double v, MbVector3D & p ) const override; // \ru Вторая производная по v \en Second derivative with respect to v
  void   _DeriveUV ( double u, double v, MbVector3D & p ) const override; // \ru Вторая производная по uv \en Second derivative with respect to uv
  void   _DeriveUUU( double u, double v, MbVector3D & p ) const override; // \ru Третья производная \en Third derivative
  void   _DeriveUUV( double u, double v, MbVector3D & p ) const override; // \ru Третья производная \en Third derivative
  void   _DeriveUVV( double u, double v, MbVector3D & p ) const override; // \ru Третья производная \en Third derivative
  void   _DeriveVVV( double u, double v, MbVector3D & p ) const override; // \ru Третья производная \en Third derivative
  void   _Normal   ( double u, double v, MbVector3D & )   const override; // \ru Нормаль \en Normal
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
  double  StepU         ( double u, double v, double sag ) const override; // \ru Вычисление шага параметра u по по величине прогиба \en Calculation of parameter u step by the value of sag  
  double  StepV         ( double u, double v, double sag ) const override; // \ru Вычисление шага параметра v по по величине прогиба \en Calculation of parameter v step by the value of sag  
  double  DeviationStepU( double u, double v, double angle ) const override; // \ru Вычисление шага параметра u по углу отклонения нормали \en Calculation of parameter u step by the angle of deviation of normal 
  double  DeviationStepV( double u, double v, double angle ) const override; // \ru Вычисление шага параметра v по углу отклонения нормали \en Calculation of parameter v step by the angle of deviation of normal 
  double  MetricStepU   ( double u, double v, double length ) const override; // \ru Вычисление шага параметра u по заданной метрической длине \en Calculation of parameter u step by the given metric length  
  size_t  GetUCount() const override;                                      // \ru Количество разбиений по параметру u для проверки событий \en Count of splittings by parameter u to check for events 
  size_t  GetVCount() const override;                                      // \ru Количество разбиений по параметру v для проверки событий \en Count of splittings by parameter v to check for events 
  /** \} */

  /** \ru \name Общие функции поверхности
      \en \name Common functions of surface
      \{ */
  void    Refresh  () override; ///< \ru Сбросить все временные данные. \en Reset all temporary data. 

  MbSplineSurface * NurbsSurface( double u1, double u2, double v1, double v2, bool bmatch = false ) const override;
  MbSurface *       NurbsSurface( const MbNurbsParameters & uParam, const MbNurbsParameters & vParam ) const override;
  MbSurface *       Offset( double d, bool same ) const override; // \ru Построить смещенную поверхность \en Create a shifted surface 

  /// \ru Изменение степени NURBS кривой по v. \en Change degree of NURBS curve by v. 
  void        ChangeDegree  ( ptrdiff_t newDegree );  
  /// \ru Получить количество базовых кривых. \en Get count of base curves. 
  size_t      GetCurvesCount () const; 
  /// \ru Получить кривую с индексом  k. \en Get curve with 'k' index.
   const  MbCurve3D * GetCurve( size_t k ) const; 
   const  SArray<double> & GetKnots() const { return knots; } ///< \ru Получить значения узлов для сплайна по v. \en  Get knot values for spline by v.

  /** \brief \ru Получить список начальных или конечных базовых точек кривых.
              \en Get list of start or end base points of curves. \~
      \details \ru Получить список начальных или конечных базовых точек кривых.\n
                \en Get list of start or end base points of curves.\n \~
      \param[in] isFirstPoints - \ru Определяет конечные или начальные точки запрошены: true - начальные, false - конечные.\n
                                  \en Determines start or end points were requested: true - start, false - end.\n \~
      \param[in] points - \ru Список, в который помещаются найденные точки. \n 
                  Порядок точек соответствует порядку кривых в списке curves.
                          \en List to store found points. \n 
                  Order of points corresponds to order of curves in 'curves' list. \~
      \return \ru false и список points остается пустым,\n
      если хотя бы одна кривая не имеет базовых точек (не отрезок и не кривая, заданная точками).
              \en False then 'points' list remains empty,\n
      if at least one curve has no base points (not segment and not curve given by points). \~
  */

   bool    GetCurvesBasePoints( bool isFirstPoints, SArray<MbCartPoint3D> & points ) const; 
  /** \brief \ru Изменить крайние базовые точки кривых.
              \en Change end base points of curves. \~
      \details \ru Базовые точки можно изменить в том случае, если все кривые, на которые натянута поверхность,\n
      являются отрезками или кривыми, заданными точками.
                \en Base points can be changed in case of all curves which the surface is tensed on\n
      are segments or curves given by points. \~
      \param[in] isFirstPoints - \ru Определяет конечные или начальные точки запрошены: true - начальные, false - конечные.\n
                                  \en Determines start or end points were requested: true - start, false - end.\n \~
      \param[in] points - \ru Список, в который помещаются новые значения базовых точек.\n 
                  Порядок точек соответствует порядку кривых в списке curves.
                          \en List to store new values of base points.\n 
                  Order of points corresponds to order of curves in 'curves' list. \~
      \return \ru false и список points остается пустым,\n
      если хотя бы одна кривая не имеет базовых точек (не отрезок и не кривая, заданная точками).
              \en False then 'points' list remains empty,\n
      if at least one curve has no base points (not segment and not curve given by points). \~ 
  */
  bool    SetCurvesBasePoints( bool isFirstPoints, SArray<MbCartPoint3D> & points ); 
  /** \} */
 
 
  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbJoinSurface )

private:
  void    operator = ( const MbJoinSurface & ); // \ru Не реализовано. \en Not implemented. 

  void    ResetTCalc();
  bool    CheckData     ( const SArray<double> & newKnots, ptrdiff_t newDegree ); // \ru Проверить корректность данных для NURBS \en Check correctness of data for NURBS 
  void    ChangeKnots   ( const ptrdiff_t newDegree, const bool closed, SArray<double> & newKnots ); // \ru Изменить массив узлов, если изменилась степень сплайна \en Change array of knots if degree of spline was changed 
  void    CreateTempVars( MbJoinSurfaceAuxiliaryData * ucache ) const;
  void    InitTempVars  ( MbJoinSurfaceAuxiliaryData * ucache ) const;
  void    FreeTempVars  ( MbJoinSurfaceAuxiliaryData * ucache ) const;
  void    PreparePointsData( ptrdiff_t lIndex, ptrdiff_t derNum, MbJoinSurfaceAuxiliaryData * ucache ) const;
  void    PreparePointList ( const double u, ptrdiff_t derNumberU, MbJoinSurfaceAuxiliaryData * ucache  ) const; 
  void    CheckPointData   ( const MbeSurfaceDerivativeType derUVNumber, double & u, double & v, MbVector3D & vect, MbJoinSurfaceAuxiliaryData * ucache  ) const;
  ptrdiff_t GetUDerNumber( const MbeSurfaceDerivativeType derUVNumber ) const;
  ptrdiff_t GetVDerNumber( const MbeSurfaceDerivativeType derUVNumber ) const;
  void    CheckPole();
  void    CheckParams  ( double & u, double & v ) const;
  void    PoleDerive   ( double u, double v, MbVector3D & vDerU, MbVector3D & vDerV ) const;
  double  DeviationStep( double u, double v, double angle ) const;
  double  StepD        ( double u, double v, double sag, bool checkAngle, double angle ) const;
  // \ru Вычисление точки и производных поверхности. \en Calculation of the point and derivatives of the surface. \~
  void    ExploreVector( SArray<MbCartPoint3D> & points, SArray<MbVector3D> & vectors,
                         ptrdiff_t lIndex, ptrdiff_t derNum, MbVector3D & vect, MbJoinSurfaceAuxiliaryData * ucache  ) const;

};

IMPL_PERSISTENT_OPS( MbJoinSurface )


//------------------------------------------------------------------------------
// \ru Проверить параметры и в случае захода за полюс загнать в полюсную область \en Check parameters and if it is out of pole, then drive it to pole region 
// ---
inline void MbJoinSurface::CheckParams( double & u, double & v ) const
{
  if ( isPoleUmin ) {
    if ( u < umin )
      u = umin;
  }
  if ( isPoleUmax ) {
    if ( u > umax )
      u = umax;
  }
  if ( isPoleVmin ) {
    const double & vmin = knots[degree - 1];
    if ( v < vmin )
      v = vmin;
  }
  if ( isPoleVmax ) {
    const double & vmax = knots[knots.MaxIndex() - degree + 1];
    if ( v > vmax )
      v = vmax;
  }
}


#endif // __SURF_JOIN_SURFACE_H
