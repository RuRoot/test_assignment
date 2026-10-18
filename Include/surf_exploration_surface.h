////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Поверхность заметания с масштабированием и поворотом образующей кривой.
         \en The swept surface with scaling and winding of generation curve. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __SURF_EXTENSION_SURFACE_H
#define __SURF_EXTENSION_SURFACE_H


#include <surf_evolution_surface.h>
#include <mb_axis3d.h>


class  MbFunction;


//------------------------------------------------------------------------------
/** \brief \ru Поверхность заметания с масштабированием и поворотом образующей кривой.
           \en The swept surface with scaling and winding of generation curve. \~
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
class MATH_CLASS MbExplorationSurface : public MbEvolutionSurface {
protected:
  MbFunction *  scaling; ///< \ru Функция второго параметра (v) масштабирования образующей кривой. \en The function of curve scale by second parameter (v). 
  MbFunction *  winding; ///< \ru Функция второго параметра (v) вращения образующей кривой. \en The function of curve rotation by second parameter (v). 
  MbAxis3D      axis;    ///< \ru Не пишется. \en The axis are not writing.

protected:
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
  MbExplorationSurface( const MbCurve3D & c, const MbSpine & s, bool sameCurve, bool sameSpine,
                        MbFunction & _scaling, MbFunction & _winding ); 

protected:
  MbExplorationSurface( const MbExplorationSurface &, MbRegDuplicate * );
private:
  MbExplorationSurface( const MbExplorationSurface & ); // \ru Не реализовано. \en Not implemented. 
public:
  virtual ~MbExplorationSurface();

public:
  VISITING_CLASS( MbExplorationSurface );

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
  /** \} */
  /** \ru \name Общие функции поверхности
      \en \name Common functions of surface
      \{ */
  MbSplineSurface * NurbsSurface( double, double, double, double, bool bmatch = false ) const override; // \ru NURBS копия поверхности. \en NURBS copy of a surface. 
  MbSurface *       Offset( double d, bool same ) const override; // \ru Создание эквидистантной поверхности. \en Create an offset surface. 

  MbCurve3D * CurveU( double v, MbRect1D *pRgn, bool bApprox = true ) const override; // \ru Пространственная копия линии v = const. \en Spatial copy of 'v = const'-line.
  MbCurve3D * CurveV( double u, MbRect1D *pRgn, bool bApprox = true ) const override; // \ru Пространственная копия линии u = const. \en Spatial copy of 'u = const'-line. 

  // \ru Определение разбивки параметрической области поверхности вертикалями и горизонталями. \en Determine splitting of parametric region of surface by vertical and horizontal lines. 
  void    GetTesselation( const MbStepData & stepData,
                          double u1, double u2, double v1, double v2,
                          SArray<double> & uu, SArray<double> & vv ) const override;
  
  size_t  GetUCount() const override; // \ru Количество разбиений по параметру u для проверки событий. \en The number of splittings by u-parameter for a check of events. 
  size_t  GetVCount() const override; // \ru Количество разбиений по параметру v для проверки событий. \en The number of splittings by v-parameter for a check of events. 

  size_t  GetUMeshCount() const override; // \ru Выдать количество полигонов по u. \en Get the count of polygons by u. 
  size_t  GetVMeshCount() const override; // \ru Выдать количество полигонов по v. \en Get the count of polygons by v. 

  //------------------------------------------------------------------------------
  /** \brief \ru Создать кинематическую поверхность.
             \en Create an evolution surface. \~
    \details \ru Создать кинематическую поверхность.
             \en Create an evolution surface. \~ 
    \param[in] curve - \ru Образующая кривая.
                       \en Generating curve. \~
    \param[in] spine - \ru Направляющая кривая.
                       \en Guide curve. \~
    \param[in] samec - \ru Признак использования оригинала образующей кривой, а не копии.
                       \en Attribute of usage of original of generating curve, not a copy. \~
    \param[in] _scaling - \ru Функция масштабирования образующей кривой.
                          \en The function of curve scaling. \~
    \param[in] _winding - \ru Функция вращения образующей кривой.
                          \en The function of curve rotation. \~
    \return \ru Возвращает созданную поверхность.
            \en Return the created surface. \~ 
    \ingroup Surface_Modeling
  */
  // ---
  static MbSurface * Create( const MbCurve3D & curve, const MbSpine & spine, bool samec, bool sames,
                             MbFunction & _scaling, MbFunction & _winding );

protected :
  void    Init();

private:
  void    operator = ( const MbExplorationSurface & ); // \ru Не реализовано. \en Not implemented. 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbExplorationSurface )
};


#endif // __SURF_EXTENSION_SURFACE_H
