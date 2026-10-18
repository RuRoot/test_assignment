////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Поверхность сопряжения.
         \en Smooth surface. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __SURF_SMOOTH_SURFACE_H
#define __SURF_SMOOTH_SURFACE_H


#include <surface.h>
#include <cur_surface_curve.h>
#include <templ_sptr.h>


constexpr bool _EVEN_ = false; // \ru Неравномерная параметризация по дуге при u = const \en Uneven parameterization along an arc where u = const

class  MbSurfaceCurve;
class  MbSurfaceIntersectionCurve;


//------------------------------------------------------------------------------
/** \brief \ru Поверхность сопряжения.
           \en Smooth surface. \~
  \details \ru Поверхность сопряжения соединяет две кривые curve1 и curve2 на сопрягаемых поверхностях. 
    Поверхность сопряжения является родительским классом поверхности скругления MbFilletSurface и поверхности фаски MbCamferSurface.
    В отличие от других поверхностей Функции PointOn и Derive... поверхностей сопряжения не корректируют
    первый параметр (u) при его выходе за пределы определения параметров (umin umax).
           \en A smooth surface connects two curves ('curve1' and 'curve2') on interfacing surfaces. 
    Smooth surface is the parent class of fillet surface (MbFilletSurface) and chamfer surface (MbChamferSurface).
    In contrast to other surfaces functions PointOn and Derive.. of smooth surface don't correct
    the first parameter (u) when getting out of domain bounds (umin and umax). \~
  \ingroup Surfaces
*/ 
// ---
class MATH_CLASS MbSmoothSurface : public MbSurface {
protected:
  MbSurfaceCurve * curve1;    ///< \ru Опорная кривая на первой поверхности (всегда не nullptr). \en Support curve on the first surface (it never equals nullptr).
  MbSurfaceCurve * curve2;    ///< \ru Опорная кривая на второй поверхности (всегда не nullptr). \en Support curve on the second surface (it never equals nullptr).
  MbeSmoothForm    form;      ///< \ru Тип сопряжения. \en Conjugation type. 
  double           distance1; ///< \ru Радиус скругления или "катет" фаски со знаком для поверхности кривой curve1. \en Fillet radius or chamfer "cathetus" with sign for surface of curve1 curve. 
  double           distance2; ///< \ru Радиус скругления или "катет" фаски со знаком для поверхности кривой curve2. \en Fillet radius or chamfer "cathetus" with sign for surface of curve2 curve. 
  double           umin;      ///< \ru Минимальное  значение параметра u. \en Minimal value of parameter u. 
  double           umax;      ///< \ru Максимальное значение параметра u. \en Maximal value of parameter u. 
  double           vmin;      ///< \ru Минимальное  значение параметра v. \en Minimal value of parameter v. 
  double           vmax;      ///< \ru Максимальное значение параметра v. \en Maximal value of parameter v. 
  bool             uclosed;   ///< \ru Признак замкнутости по параметру u. \en An attribute of closedness in u-parameter direction. 
  bool             poleUMin;  ///< \ru Наличие полюса при umin. \en Existence of a pole at umin. 
  bool             poleUMax;  ///< \ru Наличие полюса при umax. \en Existence of a pole at umax. 
  bool             poleVMin;  ///< \ru Наличие полюса при vmin. \en Existence of a pole at vmin. 
  bool             poleVMax;  ///< \ru Наличие полюса при vmax. \en Existence of a pole at vmax. 

protected:

  /** \brief \ru Конструктор по двум кривым и типу сопряжения.
             \en Constructor by two curves and conjugation type. \~ 
    \details \ru Конструктор по двум кривым и типу сопряжения.
             \en Constructor by two curves and conjugation type. \~ 
    \param[in] crv1 - \ru Опорная кривая на первой поверхности
                      \en Support curve on the first surface. \~ 
    \param[in] crv2 - \ru Опорная кривая на второй поверхности
                      \en Support curve on the second surface. \~ 
    \param[in] fm - \ru Тип сопряжения (0 - скругление, 1 - фаска)
                    \en Conjugation type (0 - fillet, 1 - chamfer) \~
  */
  MbSmoothSurface( MbSurfaceCurve & crv1, MbSurfaceCurve & crv2, MbeSmoothForm fm, double d1, double d2 );
  
  /** \brief \ru Конструктор по двум кривым и типу сопряжения.
             \en Constructor by two curves and conjugation type. \~ 
    \details \ru Конструктор по двум кривым и типу сопряжения.
             \en Constructor by two curves and conjugation type. \~ 
    \param[in] surf1 - \ru Первая поверхность
                       \en First surface. \~ 
    \param[in] curv1 - \ru Кривая в области определения первой поверхности
                       \en A curve in domain of the first surface \~
    \param[in] surf2 - \ru Вторая поверхность
                       \en Second surface. \~
    \param[in] curv2 - \ru Кривая в области определения второй поверхности
                       \en A curve in domain of the second surface \~
    \param[in] fm - \ru Тип сопряжения (0 - скругление, 1 - фаска)
                    \en Conjugation type (0 - fillet, 1 - chamfer) \~
  */
  MbSmoothSurface( MbSurface &surf1, MbCurve &curv1, MbSurface &surf2, MbCurve &curv2, MbeSmoothForm fm, double d1, double d2 );
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbSmoothSurface( const MbSmoothSurface &, MbRegDuplicate * ); 
  /// \ru Конструктор копирования с теми же опорными поверхностями. \en Copy constructor with the same support surfaces. 
  MbSmoothSurface( const MbSmoothSurface * ); // \ru Для CurvesDuplicate() \en For CurvesDuplicate() 
private:
  MbSmoothSurface( const MbSmoothSurface & ); // \ru Не реализовано. \en Not implemented. 
public:
  virtual ~MbSmoothSurface();

public:
  VISITING_CLASS( MbSmoothSurface );

  /** \ru \name Функции инициализации
      \en \name Initialization functions
      \{ */

  /** \brief \ru Коррекция средней линии поверхности скругления.
             \en Correction of the middle line of fillet surface. \~ 
    \details \ru Коррекция средней линии поверхности скругления.
             \en Correction of the middle line of fillet surface. \~ 
    \param[in] tmin - \ru Новый минимальный параметр средней линии
                      \en New minimal parameter of the middle line. \~
    \param[in] tmax - \ru Новый максимальный параметр средней линии
                      \en New maximal parameter of the middle line. \~
    \param[in] insertPoints - \ru Увеличить число контрольных точек средней кривоф.
                              \en Insert control points for middle cerve0. \~
  */
  virtual void    Init0( double tmin, double tmax, bool insertPoints = true ) = 0;    

  /** \} */
  /** \ru \name Общие функции геометрического объекта
      \en \name Common functions of a geometric object
      \{ */
  //virtual MbeSpaceType  IsA()  const = 0; // \ru Тип элемента. \en A type of element.
  MbeSpaceType  Type() const override;     // \ru Тип элемента. \en A type of element. 
  //virtual MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const = 0; // \ru Сделать копию элемента. \en Create a copy of the element.
  //virtual bool    IsSame   ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const = 0;
  //virtual bool    SetEqual ( const MbSpaceItem & ) = 0; // \ru Сделать равным. \en Make equal.
  //virtual void    Transform( const MbMatrix3D    &, MbRegTransform * = nullptr ) = 0; // \ru Преобразовать элемент согласно матрице. \en Transform element according to the matrix.
  //virtual void    Move     ( const MbVector3D    &, MbRegTransform * = nullptr ) = 0; // \ru Сдвиг. \en Translation.
  //virtual void    Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) = 0; // \ru Повернуть вокруг оси. \en Rotate around an axis.

  //virtual void    GetProperties( MbProperties &properties ) = 0; // \ru Выдать свойства объекта. \en Get properties of the object.
  //virtual void    SetProperties( const MbProperties &properties ) = 0; // \ru Записать свойства объекта. \en Set properties of the object.
  /** \} */
  /** \ru \name Функции описания области определения поверхности
      \en \name Functions for surface domain description
      \{ */
  double  GetUMin() const override; // \ru Вернуть минимальное значение параметра u. \en Get the minimum value of u. 
  double  GetUMax() const override; // \ru Вернуть максимальное значение параметра u. \en Get the maximum value of u. 
  double  GetVMin() const override; // \ru Вернуть минимальное значение параметра v. \en Get the minimum value of v. 
  double  GetVMax() const override; // \ru Вернуть максимальное значение параметра v. \en Get the maximum value of v. 
  bool    IsUClosed() const override; // \ru Проверка замкнутости по параметру u. \en Check of surface closedness in u direction. 
  bool    IsVClosed() const override; // \ru Проверка замкнутости по параметру v. \en Check of surface closedness in v direction. 
  double  GetUPeriod() const override; // \ru Вернуть период. \en Return period. 

  // \ru Существует ли полюс на границе параметрической области. \en Whether a pole exists on parametric region boundary. 
  bool    GetPoleUMin() const override;
  bool    GetPoleUMax() const override;
  bool    GetPoleVMin() const override;
  bool    GetPoleVMax() const override;
  bool    IsPole( double u, double v, double paramPrecision = PARAM_PRECISION ) const override; // \ru Является ли точка особенной. \en Whether the point is singular. 
  /** \} */
  /** \ru \name Функции для работы в области определения поверхности
            Функции PointOn и Derive... поверхностей сопряжения не корректируют
            первый параметр при его выходе за пределы определения параметров.
      \en \name Functions for working at surface domain
            Functions PointOn and Derive... of smooth surfaces don't correct
            the first parameter when getting out of domain bounds.
      \{ */
  //virtual void    PointOn  ( double &u, double &v, MbCartPoint3D & ) const = 0; // \ru Точка на поверхности. \en A point on surface.
  //virtual void    DeriveU  ( double &u, double &v, MbVector3D & ) const = 0; // \ru Первая производная по u. \en First derivative with respect to u.
  //virtual void    DeriveV  ( double &u, double &v, MbVector3D & ) const = 0; // \ru Первая производная по v. \en First derivative with respect to v.
  //virtual void    DeriveUU ( double &u, double &v, MbVector3D & ) const = 0; // \ru Вторая производная по u. \en Second derivative with respect to u.
  //virtual void    DeriveVV ( double &u, double &v, MbVector3D & ) const = 0; // \ru Вторая производная по v. \en Second derivative with respect to v.
  //virtual void    DeriveUV ( double &u, double &v, MbVector3D & ) const = 0; // \ru Вторая производная по uv. \en Second derivative with respect to u and v.
  //virtual void    DeriveUUU( double &u, double &v, MbVector3D & ) const = 0;
  //virtual void    DeriveUUV( double &u, double &v, MbVector3D & ) const = 0;
  //virtual void    DeriveUVV( double &u, double &v, MbVector3D & ) const = 0;
  //virtual void    DeriveVVV( double &u, double &v, MbVector3D & ) const = 0;
  /** \} */
  /** \ru \name Функции доступа к группе данных для работы внутри и вне области определения параметров поверхности.
      \en \name Functions for get of the group of data inside and outside the surface's domain of parameters.
      \{ */
  //virtual void    Explore( double & u, double & v, bool ext,
  //                         MbCartPoint3D & pnt, MbVector3D & uDer, MbVector3D & vDer,
  //                         MbVector3D * uuDer, MbVector3D * vvDer, MbVector3D * uvDer, MbVector3D * nor ) const = 0;
  /** \} */
  /** \ru \name Функции движения по поверхности
      \en \name Functions of moving over the surface
      \{ */
  size_t  GetUCount() const override;
  size_t  GetVCount() const override;       
  /** \} */
  /** \ru \name Общие функции поверхности
      \en \name Common functions of surface.
      \{ */     
  // \ru Определение точки пересечения поверхности и кривой. \en Determination of a point of intersection between a surface and a curve. 
  MbeNewtonResult CurveIntersectNewton( const MbCurve3D &, double funcEpsilon, size_t iterLimit,
                                        double &u0, double &v0, double &t1, bool ext0, bool ext1 ) const override;
  // \ru Дать максимальное приращение параметра. \en Get the maximum increment of parameter. 
  double  GetParamDelta() const override;
  // \ru Дать мимнимально различимую величину параметра. \en Get the minimum distinguishable value of parameter.    
  double  GetParamPrice() const override; 
  // \ru Построить NURBS-копию поверхности. \en Construct a NURBS copy of a surface. 
  MbSurface * NurbsSurface( const MbNurbsParameters & uParam, const MbNurbsParameters & vParam ) const override;

  /** \} */
  /** \ru \name Функции поверхности сопряжения
      \en \name Functions of smooth surface
      \{ */
  /// \ru Копия с теми же опорными поверхностям. \en A copy with the same support surfaces.  
  virtual MbSmoothSurface & CurvesDuplicate() const = 0; 
  /// \ru Сделать полное копирование поверхности. \en Perform a full copying of a surface. 
  MbSurface & TotalDuplicate() const; 
  /// \ru Дать радиус. \en Get radius. 
  virtual double  GetSmoothRadius() const = 0; 
  /// \ru Дать радиусы со знаком. \en Get radii with a sign. 
  virtual void    GetDistances( double u, double &d1, double &d2 ) const = 0; 
  /// \ru Дать радиус со знаком. \en Get radius with a sign. 
  virtual double  GetDistance( bool s ) const = 0; 
 
  /** \brief \ru Объединить поверхности путём включения поверхности init в данную поверхность.
             \en Combine surfaces by inclusion of the surface 'init' into the given surface. \~
    \details \ru Объединить поверхности путём включения поверхности init в данную поверхность.
             \en Combine surfaces by inclusion of the surface 'init' into the given surface. \~
    \param[in] edge - \ru Кривая разделяющего ребра
                      \en A curve of the splitting edge. \~
    \param[in] init - \ru Поверхность, которую нужно добавить в данную
                      \en A surface which should be added into the given one \~ 
    \param[in] add  - \ru Добавить в конец (true), добавить в начало (false)
                      \en Add to end (true) or add to start (false) \~
    \param[in] matr - \ru Матрица преобразования объектов с init в данную поверхность,
                      \en A matrix of transformation of objects from 'init' to the given surface, \~
    \param[in] seam - \ru Кривая другого разделяющего ребра (может быть nullptr)
                      \en A curve of another splitting edge (possibly it is nullptr) \~
  */
  virtual bool    SurfacesCombine( const MbSurfaceIntersectionCurve & edge,
                                   const MbSurface & init, bool add, bool inv, MbMatrix & matr,
                                   const MbSurfaceIntersectionCurve * seam, VERSION vers );

  /// \ru Дать коэффициент для радиуса. \en Get coefficient for radius. 
  virtual double  DistanceRatio( bool firstCurve, MbCartPoint3D & p, double distance ) const; 

  /// \ru Опорная кривая на первой поверхности. Для внутреннего использования. Всегда устанавливает не null. 
  /// \en Support curve on the first surface. For internal use. Always sets not null.
  virtual void GetCurve1( SPtr<const MbSurfaceCurve> & c ) const { c = curve1; } 
  /// \ru Опорная кривая на второй поверхности. Для внутреннего использования. Всегда устанавливает не null. 
  /// \en Support curve on the second surface. For internal use. Always sets not null.
  virtual void GetCurve2( SPtr<const MbSurfaceCurve> & c ) const { c = curve2; } 
  /// \ru Дать опорную кривую на первой поверхности для изменения. Для внутреннего использования. Всегда устанавливает не null. 
  /// \en Get the support curve on the first surface for changing. For internal use. Always sets not null.
  virtual void SetCurve1( SPtr<MbSurfaceCurve> & c ) { c = curve1; } 
  /// \ru Дать опорную кривую на второй поверхности для изменения. Для внутреннего использования. Всегда устанавливает не null. 
  /// \en Get the support curve on the second surface for changing. For internal use. Always sets not null.
  virtual void SetCurve2( SPtr<MbSurfaceCurve> & c ) { c = curve2; } 
  /// \ru Дать первую опорную поверхность. \en Get the first surface. 
  const MbSurface & GetSurface1() const;
  /// \ru Дать вторую опорную поверхность. \en Get the second surface. 
  const MbSurface & GetSurface2() const;

  /** \brief \ru Построить граничную кривую вдоль поверхности (V = const).
              \en Construct boundary curve along a surface (V = const). \~
    \details \ru Построить граничную кривую вдоль поверхности (V = const).
              \en Construct boundary curve along a surface (V = const). \~
    \param[in] s - \ru Если true, то вдоль минимального значения V,\n 
                    если false, то вдоль максимального значения V
                    \en If it equals true then construct along the minimal value of V,\n 
                    otherwise construct along the maximal value of V \~
  */
  MbCurve   * CreateBound( bool s ) const; 

  /** \brief \ru Вид опорных кривых.
              \en Type of support curves. \~
    \details \ru Вид опорных кривых.
              \en Type of support curves. \~
    \return \ru cbt_Specific если кривые построены по отдельным точкам\n
            cbt_Ordinary если кривые аналитические
            \en Cbt_Specific if the curves have been constructed by the separate points\n
            cbt_Ordinary if the curves are analytical \~
  */
  MbeCurveBuildType GetBuildType() const;  
          
  /** \brief \ru Форма поверхности.
              \en Form of a surface. \~
    \details \ru Форма поверхности.
              \en Form of a surface. \~
    \return \ru 0 в случае поверхности скругления\n
            1 в случае поверхности фаски
            \en 0 in a case of fillet\n
            1 in a case of chamfer \~
  */
  MbeSmoothForm Form() const { return form; }  
          
  /** \brief \ru Добавить точку в опорные кривые границы.
              \en Add a point to the support curves of the boundary. \~ 
    \details \ru Добавить точку в опорные кривые границы.\n
      Точка будет добавлена в кривую, если она имеет тип pt_LineSegment, pt_CubicSpline или pt_Hermit.
              \en Add a point to the support curves of the boundary.\n
      A point will be added into a curve if it has a type pt_LineSegment, pt_CubicSpline or pt_Hermit. \~
    \param[out] t1 - \ru Параметр точки на первой кривой (если add1 = true)
                      \en Parameter of a point on the first curve (if add1 equals true) \~
    \param[in] p1 - \ru Точка на первой кривой
                    \en Point on the first curve \~
    \param[in] add1 - \ru Нужно ли добавлять точку в первую кривую
                      \en Whether to add a point to the first curve \~
    \param[out] t2 - \ru Параметр точки на второй кривой (если add2 = true)
                      \en Parameter of a point on the second curve (if add2 equals true) \~
    \param[in] p2 - \ru Точка на второй кривой
                    \en Point on the second curve \~
    \param[in] add2 - \ru Нужно ли добавлять точку во вторую кривую
                      \en Whether to add a point to the second curve \~
    \param[in] version - \ru Версия операции.
                         \en Version of operation. \~
    \result \ru true - если точка была добавлена.
            \en true - if point was added. \~
  */
  virtual bool    InsertPoints( double & t1, const MbCartPoint & p1, bool add1,
                                double & t2, const MbCartPoint & p2, bool add2, VERSION vers );            

  /** \brief \ru Продлить поверхность.
              \en Prolong surface. \~ 
    \details \ru Построить и добавить точки в опорные кривые до или после границы, удлиннив поверхность.\n
      Точки будут построены и добавлены в кривые, если они имеют тип pt_Hermit.
              \en Build and add points to the support curves of the boundary.\n
      A points will be builded and added into curves if they have a type pt_Hermit. \~
    \param[in] t - \ru Первый параметр поверхности 
                    \en First parameter of surface \~
    \param[in] p1 - \ru Точка на первой кривой
                    \en Point on the first curve \~
    \param[in] p2 - \ru Точка на второй кривой
                    \en Point on the second curve \~
    \param[in] anyCase - \ru Штатная работа со значением false (true исключение).
                          \en Regular work with the value false (true exception). \~
    \param[in] version - \ru Версия операции.
                         \en Version of operation. \~
    \result \ru true - в случае успеха.
            \en true - if the operation was successful. \~
  */
  bool    ProlongSurface( double u, const MbCartPoint & p1, const MbCartPoint & p2, bool anyCase, VERSION vers ); 

  /** \brief \ru Скорректировать кривые.
              \en Correct curves. \~ 
    \details \ru Скорректировать опорные кривые после вставки точек.\n
      Кривая будет скорректирована, если поверхность имеет полюс на краю, опорная кривая имеет тип pt_Hermit и содержит опорную точку с заданным параметром.
      Корректируется опорная точка кривой, ближайшая к точке с заданным параметром со стороны полюса поверхности.
              \en Correct the support curves after inserting of the points.\n
      A curve will be corrected if the surface has a pole on the boundary, a support curve has the type pt_Hermit and contains the support point with the given parameter.
      The curve support point which is the nearest to the point with the given parameter from the side of the surface pole is corrected. \~
    \param[in] t1 - \ru Параметр точки на первой опорной кривой.
                    \en Parameter of a point on the first support curve. \~
    \param[in] t2 - \ru Параметр точки на второй опорной кривой.
                    \en Parameter of a point on the second support curve. \~
  */
  bool    CurveStraighten( double t1, double t2 ); 
  /// \ru Дать свойства объекта. \en Get the object properties. 
  void    AddProperties( MbProperties &properties ); 
  /// \ru Проверить полюса. \en Check poles. 
  void    SetPole(); 
  /** \} */
protected:
  /// \ru Корректировка параметров. \en Correction of parameters. 
  inline  void    CheckParam  ( double &u, double &v ) const; 

  void    InitSmoothSurface ( const MbSmoothSurface & );
  void    Init              ( const MbSmoothSurface & );          

private:  
          // \ru Определение точки пересечения края поверхности и кривой на смежной поверхности. \en Determination of intersection point between the surface boundary and the adjacent surface. 
          MbeNewtonResult TangentIntersection( const MbSurfaceIntersectionCurve &,
                                               size_t iterLimit, double &u, double &v, double &t, bool ext0, bool ext1 ) const;
          MbeNewtonResult CurveTangentIntersection( const MbCurve3D &, double funcEpsilon, size_t iterLimit,
                                                    double &u, double &v, double &t, bool ext0, bool ext1 ) const;
  bool    IsSameSurface( const MbCurve3D &, double &u, double &v, double &t ) const; // \ru Идентификация кривой. \en Identification of a curve. 

  void    operator = ( const MbSmoothSurface & ); // \ru Не реализовано. \en Not implemented. 

  DECLARE_PERSISTENT_CLASS( MbSmoothSurface )
};

IMPL_PERSISTENT_OPS( MbSmoothSurface )


//------------------------------------------------------------------------------
// \ru Корректировка параметров \en Correction of parameters 
// ---
inline void MbSmoothSurface::CheckParam( double &u, double &v ) const {
  if ( v < vmin )
    v = vmin;
  else
    if ( v > vmax )
      v = vmax;

  if ( uclosed ) {
    if ( (u < umin) || (u > umax ) ) {
      double tmp = umax - umin;
      u -= ::floor((u - umin) / tmp) * tmp; 
    }
  }
  else {
    if ( poleUMin && u < umin )
      u = umin;
    if ( poleUMax && u > umax )
      u = umax;
  }
}


//------------------------------------------------------------------------------
// Скорректировать крайние точки контейнеров, если они лежат в полюсах поверхности.
// ---
void CorrectPolePoins(const MbSurface & surface, SArray<MbCartPoint> & points );


//------------------------------------------------------------------------------
// \ru Наполнить массив параметров для кривых на поверхностях \en Fill an array of parameters for the curves on surfaces 
// ---
void CreateParams( const MbSurface & surface1, SArray<MbCartPoint> & points1,
                   const MbSurface & surface2, SArray<MbCartPoint> & points2,
                   double radius, SArray<double> * values, SArray<double> * valuesDerive,
                   bool & degenerate1, bool & degenerate2, ptrdiff_t & begN, ptrdiff_t & endN, 
                   SArray<double> & params ); 


//------------------------------------------------------------------------------
// \ru Создать кривые на поверхности \en Create curves on a surface. 
// ---
void CreateSurfaceCurves( const MbSurface & surface1, SArray<MbCartPoint> & points1,
                          const MbSurface & surface2, SArray<MbCartPoint> & points2,
                          MbeSmoothForm form, bool firstFree, double distance1, double distance2, 
                          bool insert, ptrdiff_t begN, ptrdiff_t endN, VERSION version, 
                          MbSurfaceCurve *& curve1, MbSurfaceCurve *& curve2 );


//------------------------------------------------------------------------------
// \ru Создать поверхность \en Create a surface 
// ---
MbSmoothSurface * CreateSmoothSurface( const MbSurface & surface1, SArray<MbCartPoint> & points1,
                                       const MbSurface & surface2, SArray<MbCartPoint> & points2,
                                       MbeSmoothForm form, bool firstFree, double distance1, double distance2, 
                                       double conic, bool even, ptrdiff_t begN, ptrdiff_t endN, VERSION version );

#endif // __SURF_SMOOTH_SURFACE_H
