////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Контур на плоскости.
         \en Contour on plane. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CUR_CONTOUR_ON_PLANE_H
#define __CUR_CONTOUR_ON_PLANE_H


#include <cur_contour_on_surface.h>
#include <surf_plane.h>


class   MbAxis3D;


//------------------------------------------------------------------------------
/** \brief \ru Контур на плоскости.
           \en Contour on plane. \~
  \details \ru Контур на плоскости представляет собой трёхмерную составную кривую,
    полученную движением вдоль двумерного контура MbContour,
    расположенного в пространстве параметров некоторой плоскости MbPlane. \n
    Контур на плоскости используется: 
    для описания области определения параметров поверхности,
    для описания плоского эскиза в операции.
           \en Contour on the plane a is three-dimensional composite curve,
    obtained by the motion along the two-dimensional contour MbContour,
    located in the parameters space of a plane MbPlane. \n
    Contour on the plane is used: 
    for description of surface domain,
    for description of planar sketch in the operation. \~
  \ingroup Curves_3D
*/
// ---
class MATH_CLASS MbContourOnPlane : public MbContourOnSurface {

public :
  /// \ru Конструктор по плоскости, контуру и флагу использования оригинала контура. \en Constructor by plane, contour and flag of using original contour. 
  MbContourOnPlane( const MbPlane &, const MbContour &, bool same );
  /// \ru Конструктор по плейсменту, контуру и флагу использования оригинала контура. \en Constructor by plane, contour and flag of using original contour. 
  MbContourOnPlane( const MbPlacement3D &, const MbContour &, bool same );
  /// \ru Конструктор по плоскости и направлению обхода поверхности. \en Constructor by plane and traverse direction of surface. 
  MbContourOnPlane( const MbPlane &, int sense );
  /// \ru Конструктор по плоскости. \en Constructor by plane. 
  MbContourOnPlane( const MbPlane & );

protected:
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbContourOnPlane( const MbContourOnPlane &, MbRegDuplicate * );
  /// \ru Конструктор копирования контура с той же поверхностью для CurvesDuplicate(). \en Constructor to copy contour with the same surface for CurvesDuplicate(). 
  explicit MbContourOnPlane( const MbContourOnPlane * );
private:
  MbContourOnPlane( const MbContourOnPlane & ); // \ru Не реализовано !!! \en Not implemented !!! 

public :
  virtual ~MbContourOnPlane();

public :
  VISITING_CLASS( MbContourOnPlane );

  // \ru Общие функции математического объекта. \en The common functions of the mathematical object. 

  MbeSpaceType  IsA() const override; // \ru Дать тип элемента. \en Get a type of the element. 
  MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента. \en Create a copy of the element.
  MbContourOnSurface & CurvesDuplicate() const override; // \ru Сделать копию со старой подложкой. \en Make a copy with old substrate.
  bool    IsSame( const MbSpaceItem &, double accuracy = LENGTH_EPSILON ) const override; // \ru Определить, является ли копией данного объекта? \en Determine whether the object is copy of a given object. 

  void    GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  void    SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта. \en Set properties of the object. 

  // \ru Общие функции кривой. \en Common functions of curve. 

  // \ru Функции для работы внутри области определения кривой. \en Functions for working inside of the curve domain. \~
  void    PointOn  ( double & t, MbCartPoint3D & ) const override; // \ru Вычислить точку на кривой. \en Calculate point on the curve. 
  void    FirstDer ( double & t, MbVector3D & )    const override; // \ru Вычислить первую производную. \en Calculate the first derivative. 
  void    SecondDer( double & t, MbVector3D & )    const override; // \ru Вычислить вторую производную. \en Calculate the second derivative. 
  void    ThirdDer ( double & t, MbVector3D & )    const override; // \ru Вычислить третью производную по t. \en Calculate the third derivative by t. 
  // \ru Функции для работы внутри и вне области определения кривой. \en Functions for working inside and outside of the curve domain. \~
  void   _PointOn  ( double t, MbCartPoint3D & )  const override; // \ru Вычислить точку на расширенной кривой. \en Calculate point on the extended curve. 
  void   _FirstDer ( double t, MbVector3D & )     const override; // \ru Вычислить первую производную. \en Calculate the first derivative. 
  void   _SecondDer( double t, MbVector3D & )     const override; // \ru Вычислить вторую производную. \en Calculate the second derivative. 
  void   _ThirdDer ( double t, MbVector3D & )     const override; // \ru Вычислить третью производную по t. \en Calculate the third derivative by t. 
    // \ru Вычислить значения точки и производных для заданного параметра. \en Calculate point and derivatives of object for given parameter. \~
  void    Explore( double & t, bool ext,
                           MbCartPoint3D & pnt, MbVector3D & fir, MbVector3D * sec, MbVector3D * thir ) const override;

  double  Curvature    ( double t )             const override; // \ru Вычислить кривизну кривой. \en Calculate the curve curvature.
  double  Step         ( double t, double sag ) const override; // \ru Вычислить шаг аппроксимации. \en Calculate the approximation step. 
  double  DeviationStep( double t, double sag ) const override; // \ru Вычислить шаг аппроксимации. \en Calculate the approximation step.

  // \ru Построить усеченную кривую c учетом заданной трехмерной точности. \en Construct a trimmed curve with the given three-dimensional accuracy.
  MbCurve3D * Trimmed( double t1, double t2, int sense, const MbDimAccuracy & xyzEps, bool saveParamLenAndLaw ) const override;
  using MbCurve3D::Trimmed; // \ru Метод базового класса MbCurve3D без учета трехмерной точности. Точность по умолчанию - MbDimAccuracy::threeDimAcc.  \en Method of MbCurve3D base class without three-dimensional accuracy. The default accuracy is MbDimAccuracy::threeDimAcc.

  double  GetMetricLength() const override; // \ru Вычислить метрическую длину кривой. \en Calculate the metric length of curve. 
  double  GetLengthEvaluation() const override; // \ru Оценить метрическую длину кривой. \en Evaluate the metric length of curve. 
  double  CalculateMetricLength() const override; // \ru Вычислить метрическую длину кривой. \en Calculate the metric length of curve. 
  
  bool    ChangeSurface( const MbSurface & newsurf ) override; // \ru Заменить поверхность контура. \en Replace the surface of contour.
  void    ChangeCarrier( const MbSpaceItem & item, MbSpaceItem & init ) override; // \ru Изменить носителя. \en Change the carrier. 
  bool    IsPlanar( double accuracy = METRIC_EPSILON ) const override; // \ru Является ли кривая плоской? \en Whether the curve is planar? 
  // \ru Дать плоскую кривую и плейсмент, если пространственная кривая плоская (после использования вызывать DeleteItem на двумерную кривую). \en Get planar curve and placement if the space curve is planar (after the using call DeleteItem for two-dimensional curves) 
  bool    GetPlaneCurve( MbCurve *& curve2d, MbPlacement3D & place3d, bool saveParams, PlanarCheckParams params = PlanarCheckParams() ) const override;
  // \ru Заполнить плейсмент, если кривая плоская. \en Fill the placement if curve is planar. 
  bool    GetPlacement( MbPlacement3D & place, PlanarCheckParams params = PlanarCheckParams() ) const override;

  double  GetRadius( double accuracy = METRIC_REGION ) const override; // \ru Дать физический радиус объекта или ноль, если это невозможно. \en Get the physical radius of the object or null if it impossible.
  bool    GetCircleAxis( MbAxis3D & ) const override; // \ru Дать ось кривой. \en Get the curve axis.
  void    GetCentre( MbCartPoint3D & ) const override; // \ru Дать центр тяжести. \en Get the center of gravity. 
  void    GetWeightCentre( MbCartPoint3D & ) const override; // \ru Дать центр тяжести. \en Get the center of gravity. 
  void    CalculateGabarit( MbCube & ) const override; // \ru Вычислить габарит кривой. \en Calculate the bounding box of curve. 

  bool    IsStraight( bool ignoreParams = false ) const override; // \ru Определить, является ли линия прямолинейной? \en Determine whether the line is straight. 

  MbCurve * GetMap( const MbMatrix3D &, MbRect1D *pRgn = nullptr,
                            VERSION version = Math::DefaultMathVersion(), bool * coincParams = nullptr ) const override; // \ru Дать плоскую проекцию кривой. \en Get a planar projection of curve.

  void    GetPointsByEvenLengthDelta( size_t n, std::vector<MbCartPoint3D> & pnts ) const override; // \ru Выдать n точек кривой с равными интервалами по длине дуги. \en Get n points of curve with equal intervals along the length of the arc.

          /// \ru Получить локальную систему координат плоскости. \en Get the local coordinate system of a plane. 
    const MbPlacement3D    & GetPlacement() const;
          /// \ru Получить плоскость. \en Get the plane. 
    const MbPlane &          GetPlane() const { return static_cast<const MbPlane &>(GetSurface()); }

          /// \ru Сделать правой локальную систему координат плоскости. \en Make the local coordinate system of plane right. 
          void    SetRightPlacement();
          /// \ru Адаптировать локальную систему координат плоскости. \en Adapt the local coordinate system of a plane. 
          void    AdaptToPlace( const MbPlacement3D & );
          /// \ru Заменить локальную систему координат плоскости. \en Replace the local coordinate system of a plane. 
          void    SetPlacement( const MbPlacement3D & );
          /// \ru Инвертировать нормаль плоскости. \en Invert the normal of plane. 
          void    InvertNormal( MbRegTransform * = nullptr );

          /// \ru Продлить кривую. \en Extend the curve. \~
          MbResultType Extend( const MbCurveExtensionParameters3D & parameters, c3d::SpaceCurveSPtr & resCurve ) const override;


private:
          void    operator = ( const MbContourOnPlane & ); // \ru Не реализовано !!! \en Not implemented !!! 

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbContourOnPlane )
};

IMPL_PERSISTENT_OPS( MbContourOnPlane )

#endif // __CUR_CONTOUR_ON_PLANE_H
