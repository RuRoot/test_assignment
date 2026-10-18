////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Кинематическая поверхность с адаптацией.
         \en Sweep with guide curve surface with rotating ends. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __SURF_EXACTION_SURFACE_H
#define __SURF_EXACTION_SURFACE_H


#include <mb_axis3d.h>
#include <surf_evolution_surface.h>


class    MbContourOnPlane;
class    MbContourOnSurface;


//------------------------------------------------------------------------------
/** \brief \ru Кинематическая поверхность с адаптацией.
           \en Sweep with guide curve surface with rotating ends. \~
  \details \ru Кинематическая поверхность образуется путем движения образующей кривой curve по направляющей кривой spine->curve.
    Поверхность является аналогом поверхности MbEvolutionSurface, но отличающаяся от нее плоскопараллельным доворотом точек поверхности. 
    В каждом сечении поверхности v=const точки сдвигаются по линейному закону вдоль образующей так, 
    чтобы на концах направляющей v=vmin и v=vmax края поверхности повернулись бы за заданные углы angle0 и angle1.
    Первый параметр поверхности совпадает с параметром образующей кривой. 
    Второй параметр поверхности совпадает с параметром направляющей кривой. 
    Поверхность используется при построении кинематической оболочки с направляющей кривой в виде контура, имеющего негладкую стыковку сегментов.
           \en Sweep with guide curve surface is formed by moving the 'curve' generating curve along spine->curve guide curve.
    Surface is analog of MbEvolutionSurface, but differs from it by plane-parallel additional turn of surface points. 
    Points of each 'v=const' surface section are moved by linear law along generating curve so 
    that on v=vmin and v=vmax ends of guide curve the surface boundaries would rotate by specified angle0 and angle1 angles.
    First parameter of surface coincides with parameter of generating curve. 
    Second parameter of surface coincides with parameter of guide curve. 
    Surface is used in construction of sweep with guide curve shell as contour with non-smooth connection of segments. \~
  \ingroup Surfaces
*/
// ---
class MATH_CLASS MbExactionSurface : public MbEvolutionSurface {
private:
  MbVector3D normal0; ///< \ru Вектор нормали к плоскости стыковки в начальной точке. \en A vector of normal to the plane of connection at the start point. 
  MbVector3D normal1; ///< \ru Вектор нормали к плоскости стыковки в конечной  точке. \en A vector of normal to the plane of connection at the end point. 
  double     angle0;  ///< \ru Угол излома в начальной точке направляющей. \en Angle of break at start point of guide curve. 
  double     angle1;  ///< \ru Угол излома в конечной  точке направляющей. \en Angle of break at end point of guide curve. 
  MbVector3D move0;   ///< \ru Касательный вектор сдвига начальных точек. \en Tangent vector of translation of start points. 
  MbVector3D move1;   ///< \ru Касательный вектор сдвига конечных  точек. \en Tangent vector of translation of end points. 
  bool       mode0;   ///< \ru true, если вектор move0 не равен нулю. \en True if 'move0' vector isn't equal to zero. 
  bool       mode1;   ///< \ru true, если вектор move1 не равен нулю. \en True if 'move1' vector isn't equal to zero. 
  MbVector3D factorX; ///< \ru Сомножитель векторного произведения для касательной к образующей curve (нормаль плоскости эскиза). \en The multiplier of the vector product for the tangent of the generating 'curve' (the scetch normal). 
  double     rangeX;  ///< \ru Эквидистантное смещение точек образующей кривой в конце траектории. \en The offset range of generating curve on the end of spine curve. 
  bool       modeX;   ///< \ru true, если вектор 'factorX' не равен нулю. \en True if 'factorX' vector isn't equal to zero. 

public:  

  /** \brief \ru Конструктор по образующей и направляющей.
             \en Constructor by generating curve and guide curve. \~
    \details \ru Конструктор по образующей и направляющей.
             \en Constructor by generating curve and guide curve. \~ 
    \param[in] cr - \ru Образующая.
                    \en Generating curve. \~
    \param[in] sameCurve - \ru Признак использования оригинала образующей, а не копии.
                           \en Attribute of usage of original of generating curve, not a copy. \~      
    \param[in] sp - \ru Направляющая.
                    \en Guide curve. \~
    \param[in] n0 - \ru Вектор нормали к плоскости стыковки в начальной точке.
                    \en Vector of normal to the plane of connection at the start point. \~
    \param[in] ang0 - \ru Угол излома в начальной точке направляющей.
                      \en Angle of break at start point of guide curve. \~
    \param[in] n1 - \ru Вектор нормали к плоскости стыковки в конечной точке.
                    \en Vector of normal to the plane of connection at the end point. \~
    \param[in] ang1 - \ru Угол излома в конечной точке направляющей.
                      \en Angle of break at end point of guide curve. \~
    \param[in] range - \ru Эквидистантное смещение точек образующей кривой в конце траектории. 
                       \en The offset range of generating curve on the end of spine curve. \~
  */
  MbExactionSurface( const MbCurve3D & cr, bool sameCurve, 
                     const MbSpine & sp, 
                     const MbVector3D & n0, double ang0, 
                     const MbVector3D & n1, double ang1, 
                     double range );

protected:
  MbExactionSurface( const MbExactionSurface &, MbRegDuplicate * );
private:
  MbExactionSurface( const MbExactionSurface & ); // \ru Не реализовано. \en Not implemented. 
public:
  virtual ~MbExactionSurface();

public:
  VISITING_CLASS( MbExactionSurface );

  /** \ru \name Общие функции геометрического объекта
      \en \name Common functions of a geometric object 
      \{ */
  MbeSpaceType  IsA() const override; // \ru Тип элемента. \en A type of element. 
  MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента. \en Create a copy of the element.
  bool    IsSame   ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const override; // \ru Равны ли объекты. \en Whether the objects are equal. 
  bool    SetEqual ( const MbSpaceItem & ) override; // \ru Сделать равными. \en Make equal. 
  bool    IsSimilar( const MbSpaceItem & ) const override; // \ru Являются ли объекты подобными. \en Determine whether the objects are similar. 
  void    Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать элемент согласно матрице. \en Transform element according to the matrix.
  void    Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override; // \ru Сдвиг. \en Move.
  void    Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси. \en Rotate around an axis.
  
  void    GetProperties( MbProperties & properties ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  void    SetProperties( const MbProperties & properties ) override; // \ru Записать свойства объекта. \en Set properties of the object. 
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
  /** \ru \name Общие функции поверхности
      \en \name Common functions of surface
      \{ */
  bool    IsPlanar( double accuracy = METRIC_EPSILON ) const override; // \ru Является ли поверхность плоской. \en Whether the surface is planar. 

  MbSplineSurface * NurbsSurface( double, double, double, double, bool bmatch = false ) const override; // \ru NURBS копия поверхности. \en NURBS copy of a surface. 
  MbSurface *       Offset( double d, bool same ) const override; // \ru Создание эквидистантной поверхности. \en Create an offset surface. 

  MbCurve3D * CurveU( double v, MbRect1D * pRgn, bool bApprox = true ) const override; // \ru Пространственная копия линии v = const. \en Spatial copy of 'v = const'-line.
  MbCurve3D * CurveV( double u, MbRect1D * pRgn, bool bApprox = true ) const override; // \ru Пространственная копия линии u = const. \en Spatial copy of 'u = const'-line.

  // \ru Подобные ли поверхности для объединения (слива). \en Whether the surfaces to union (joining) are similar. 
  bool    IsSimilarToSurface( const MbSurface & surf, VERSION version, double precision = METRIC_PRECISION ) const override;
  // \ru Дать двумерную матрицу преобразования из своей параметрической области в параметрическую область surf. \en Get two-dimensional transformation matrix from own parametric region to parametric region of 'surf'. 
  bool    GetMatrixToSurface( const MbSurface & surf, MbMatrix & matr, VERSION version, double precision = METRIC_PRECISION ) const override;

  /** \} */
  /** \ru \name Функции кинематической поверхности с адаптацией
      \en \name Functions of sweep with guide curve surface with rotating ends.
      \{ */
  /// \ru Признак наличия ненулевого касательного вектора сдвига начальных точек. \en Attribute of existence of non-zero tangent vector of translation of start points. 
  bool    GetMode0()    const { return mode0;  } 
  /// \ru Признак наличия ненулевого касательного вектора сдвига конечных точек. \en Attribute of existence of non-zero tangent vector of translation of end points. 
  bool    GetMode1()    const { return mode1;  } 
  /// \ru true, если вектор 'factorX' не равен нулю. \en True if 'factorX' vector isn't equal to zero.
  bool    GetModeX()    const { return modeX; }
  /// \ru Угол излома в начальной точке направляющей. \en Angle of break at start point of guide curve. 
  double  GetBegAngle() const { return angle0; } 
  /// \ru Угол излома в конечной точке направляющей. \en Angle of break at end point of guide curve. 
  double  GetEndAngle() const { return angle1; } 
  /// \ru Эквидистантное смещение точек образующей кривой в конце траектории. \en The offset range of generating curve on the end of spine curve.
  double  GetRangeX() const { return rangeX; }
  /// \ru Сомножитель векторного произведения для касательной к образующей curve (нормаль плоскости эскиза). \en The multiplier of the vector product for the tangent of the generating 'curve' (the scetch normal).
  const MbVector3D & GetFactorX() const { return factorX; }
  /// \ru Вектор нормали к плоскости стыковки в начальной точке. \en A vector of normal to the plane of connection at the start point.
  const MbVector3D & GetNormal0() const { return normal0; }
  /// \ru Вектор нормали к плоскости стыковки в конечной  точке. \en A vector of normal to the plane of connection at the end point.
  const MbVector3D & GetNormal1() const { return normal1; }
  /** \} */

private:
  void    Init();
  void    InitEnd();
  void    PrepareTangent();
  void    AddTangent0( double & v, MbVector3D & ) const;
  void    AddTangentV( MbVector3D & ) const;
  void    AddOffsetX0( double & v, const MbVector3D & first, MbVector3D & r ) const;
  void    AddOffsetXU( double & v, MbVector3D & first, const MbVector3D & second ) const;
  void    AddOffsetXUU( double & v, const MbVector3D & first, MbVector3D & second, const MbVector3D & third ) const;
  void    AddOffsetXV( const MbVector3D & derive, MbVector3D & r ) const;
  void    AddOffsetXUV( const MbVector3D & first, const MbVector3D & second, MbVector3D & r ) const;

  void    operator = ( const MbExactionSurface & ); // \ru Не реализовано. \en Not implemented. 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbExactionSurface )
};

IMPL_PERSISTENT_OPS( MbExactionSurface )


//------------------------------------------------------------------------------
// \ru Проверка на самопересечение кинематической поверхности \en Check for self-intersection of sweep with guide curve surface 
// \ru Если изменится расчет точек кинематической поверхности, то надо переделывать \en If calculation of points of a sweep with guide curve surface changes, then it is necessary to remake 
// ---
bool FindSelfIntersections( const MbCurve3D           & curve3d,
                            const MbSpine             & baseSpine,
                            const SArray<SpineData>   & childSpines,
                                  //bool                  natur,
                                  VERSION               version );


//------------------------------------------------------------------------------
// \ru Проверить корректность кинематики по движении по замкнутому контуру \en Check correctness of kinematics while moving along closed contour 
// \ru (путем трансформации копии образующего контура по направляющем сегментам \en (by transformation of copy of generating contour along guide segments 
// \ru И сравнения его с исходным образующим контуром) \en And comparing it with source generating contour) 
// ---
bool CheckClosingContour( const MbContourOnSurface & contourOnSurface,  // \ru Образующий контур \en Generating contour 
                          const MbSpine            & baseSpine,         // \ru Направляющий контур \en Guide contour 
                          const SArray<SpineData>  & childSpines,       // \ru Сегменты направляющей \en Segments of guide curve 
                                //bool                 natur,             // \ru Тип привязки тела \en Type of binding of solid 
                        bool                 closedShell );     // \ru Замкнутость результирующей оболочки \en Closedness of resultant shell 


//------------------------------------------------------------------------------
// \ru РЕАЛИЗОВАНА НЕ ПОЛНОСТЬЮ \en NOT FULLY IMPLEMENTED 
// \ru Проверка на самопересечение кинематической поверхности \en Check for self-intersection of sweep with guide curve surface. 
// ---
bool IsSelfIntersect( const MbEvolutionSurface & );


#endif // __SURF_EXACTION_SURFACE_H
