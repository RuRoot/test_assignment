////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Поверхность движения.
         \en Swept surface. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __SURF_SWEPT_SURFACE_H
#define __SURF_SWEPT_SURFACE_H


#include <surface.h>


//------------------------------------------------------------------------------
/** \brief \ru Поверхность движения.
           \en Swept surface. \~
  \details \ru Родительский класс поверхностей: MbEvolutionSurface, MbExtrusionSurface, MbRevolutionSurface, MbRuledSurface, MbSectorSurface, MbSpiralSurface. 
    Поверхность движения описывается движением образующей кривой.
    Наследники поверхности отличаются разными траекториями движения образующей кривой.
    Первый параметр наследников поверхности движения совпадает с параметром образующей кривой.
           \en Parent class for surfaces: MbEvolutionSurface, MbExtrusionSurface, MbRevolutionSurface, MbRuledSurface, MbSectorSurface, MbSpiralSurface. 
    A swept surface is described by moving of the generating curve.
    Inheritors of swept surface differ by various trajectories of generating curve moving.
    The first parameter of swept surface inheritors coincides with the parameter of generatrix. \~
  \ingroup Surfaces
*/// ---
class MATH_CLASS MbSweptSurface : public MbSurface {
protected:
  MbCurve3D * curve;   ///< \ru Образующая кривая. \en Generating curve.  
  double      umin;    ///< \ru Минимальное значение параметра u. \en Minimal value of parameter u. 
  double      vmin;    ///< \ru Минимальное значение параметра v. \en Minimal value of parameter v. 
  double      umax;    ///< \ru Максимальное значение параметра u. \en Maximal value of parameter u. 
  double      vmax;    ///< \ru Максимальное значение параметра v. \en Maximal value of parameter v. 
  bool        uclosed; ///< \ru Признак замкнутости по параметру u. \en An attribute of closedness in u-parameter direction. 
  bool        vclosed; ///< \ru Признак замкнутости по параметру v. \en An attribute of closedness in v-parameter direction. 

protected:
  /** \brief \ru Конструктор по образующей.
             \en Constructor by generatrix. \~
    \details \ru Конструктор по образующей.
             \en Constructor by generatrix. \~
    \param[in] same - \ru true, если нужно использовать ту же кривую, false, если нужно использовать копию
                      \en it equals true if it is required to use the same curve, it equals false if it is required to use the copy \~
  */
  MbSweptSurface( const MbCurve3D &, bool same );
  MbSweptSurface( const MbSweptSurface &, MbRegDuplicate * );
  MbSweptSurface() // \ru Используется только в конвертерах. \en This is used only in converters. 
    : curve( nullptr ), umin( 0 ), vmin( 0 ), umax( 0 ), vmax( 0 ), uclosed( false ), vclosed( false ) {}

private:
  MbSweptSurface( const MbSweptSurface & ); // \ru Не реализовано. \en Not implemented. 
public:
  virtual ~MbSweptSurface();

public:
  VISITING_CLASS( MbSweptSurface );

  /** \ru \name Общие функции геометрического объекта
      \en \name Common functions of a geometric object
      \{ */
  //virtual MbeSpaceType  IsA()  const = 0; // \ru Тип элемента. \en A type of element.
  MbeSpaceType  Type() const override;     // \ru Тип элемента. \en A type of element. 
  //virtual MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const= 0; // \ru Сделать копию элемента. \en Create a copy of the element.
  //virtual bool    IsSame   ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const = 0; // \ru Являются ли объекты равными. \en Determine whether objects are equal.
  //virtual bool    SetEqual ( const MbSpaceItem & ) = 0; // \ru Сделать равным. \en Make equal.
  //virtual void    Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) = 0; // \ru Преобразовать элемент согласно матрице. \en Transform element according to the matrix.
  //virtual void    Move     ( const MbVector3D &, MbRegTransform * = nullptr ) = 0; // \ru Сдвиг. \en Translation.
  //virtual void    Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) = 0; // \ru Повернуть вокруг оси. \en Rotate around an axis.

  //virtual void    GetProperties( MbProperties & properties ) = 0; // \ru Выдать свойства объекта. \en Get properties of the object.
  //virtual void    SetProperties( const MbProperties & properties ) = 0; // \ru Записать свойства объекта. \en Set properties of the object.
  void    GetBasisItems ( RPArray <MbSpaceItem> & ) override;  // \ru Дать базовые объекты. \en Get the base objects. 
  void    GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void    SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  /** \} */
  /** \ru \name Функции описания области определения поверхности
      \en \name Functions for surface domain description 
      \{ */
  double  GetUMin() const override; // \ru Вернуть минимальное значение параметра u. \en Get the minimum value of u. 
  double  GetVMin() const override; // \ru Вернуть минимальное значение параметра v. \en Get the minimum value of v. 
  double  GetUMax() const override; // \ru Вернуть максимальное значение параметра u. \en Get the maximum value of u. 
  double  GetVMax() const override; // \ru Вернуть максимальное значение параметра v. \en Get the maximum value of v. 
  bool    IsUClosed() const override; // \ru Проверка замкнутости по параметру u. \en Check of surface closedness in u direction. 
  bool    IsVClosed() const override; // \ru Проверка замкнутости по параметру v. \en Check of surface closedness in v direction. 
  bool    IsUTouch() const override;   // \ru Замкнута ли фактически поверхность по параметру u независимо от гладкости. \en Whether the surface is actually closed by parameter u regardless of the smoothness.
  double  GetUPeriod() const override; // \ru Вернуть период . \en Return period.         
  /** \} */
  /** \ru \name Функции для работы в области определения поверхности
            Функции PointOn, Derive... поверхностей корректируют параметры
            при выходе их за пределы прямоугольной области определения параметров.\n        
      \en \name Functions for working at surface domain
            Functions PointOn, Derive... correct parameters
            when getting out of rectangular domain bounds. \n 
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
  /** \ru \name Функции поверхности движения
      \en \name Functions of swept surface
      \{ */
  /// \ru Дать образующую кривую. \en Get generating curve.   
  const   MbCurve3D & GetCurve() const { return *curve; } 
  /// \ru Дать образующую кривую для изменения. \en Get generating curve for editing. 
          MbCurve3D & SetCurve()       { return *curve; } 
  /** \} */ 

protected:
          /// \ru Инициализация по поверхности движения. \en Initialization by swept surface. 
  void    InitSwept( const MbSweptSurface & );
          /// \ru Проверить по граничным точкам, может ли поверхность оказаться плоской. \en Check by boundary points, whether a surface may be planar. 
  bool    CheckPlaneByLimitPoints() const; 

private:
  void    operator = ( const MbSweptSurface & ); // \ru Не реализовано. \en Not implemented. 

  DECLARE_PERSISTENT_CLASS( MbSweptSurface )
};

IMPL_PERSISTENT_OPS( MbSweptSurface )


//------------------------------------------------------------------------------
// \ru Получить исходную кривую, возвращает true для прямолинейной направляющей \en Get initial curve, it returns true for the rectilinear guide curve. 
// ---
bool GetSourceCurve( const MbCurve3D *& spineCurve, int & sense, 
                     MbPlacement3D & spinePlace, SPtr<const MbCurve> & planeSpine, VERSION version = Math::DefaultMathVersion() );


#endif // __SURF_SWEPT_SURFACE_H
