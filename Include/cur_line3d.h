////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Прямая в трехмерном пространстве.
         \en Line in three-dimensional space. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CUR_LINE3D_H
#define __CUR_LINE3D_H


#include <curve3d.h>


class     MbLine;


//------------------------------------------------------------------------------
/** \brief \ru Прямая в трехмерном пространстве.
           \en Line in three-dimensional space. \~
  \details \ru Прямая линия ведёт себя как бесконечный объект, хотя в своих данных имеет граничные значения параметра tmin и tmax. 
    В отличие от других кривых в методах вычисления радиуса-вектора и его производных прямая не корректирует параметр t при его выходе за предельные значения tmin и tmax. \n
    Радиус-вектор кривой в методе PointOn(double&t,MbCartPoint3D&r) описывается векторной функцией\n
    r(t) = origin + (t direction).
           \en Straight line behaves as an infinite object although its data has boundary parameter values tmin and tmax. 
    In contrast to curves in the calculation methods of radius-vector and its derivatives the line doesn't correct parameter "t" if it is outside the values tmin and tmax. \n
    Radius-vector of the curve in the method PointOn(double&t,MbCartPoint3D&r) is described by the vector function\n
    r(t) = origin + (t direction). \~
  \ingroup Curves_3D
*/
// ---
class MATH_CLASS MbLine3D : public MbCurve3D {
private :
  MbCartPoint3D origin;     ///< \ru Начальная точка. \en Start point. 
  MbVector3D    direction;  ///< \ru Вектор направления. \en A direction vector. 

public :
  MbLine3D();
  MbLine3D( const MbCartPoint3D & initP, const MbVector3D & initV );
  MbLine3D( const MbCartPoint3D & p1,    const MbCartPoint3D & p2 );
protected:
  MbLine3D( const MbLine3D & );
public :
  virtual ~MbLine3D();

public :
  VISITING_CLASS( MbLine3D ); 

  void    Init( const MbLine3D & init );
  void    Init( const MbCartPoint3D & p0,  const MbVector3D & dir );
  void    Init( const MbCartPoint3D & p1,  const MbCartPoint3D & p2 );
  void    Init( const MbPlacement3D & pos, const MbLine & line );

  // \ru Общие функции математического объекта \en Common functions of the mathematical object 

  MbeSpaceType  IsA() const override; // \ru Тип элемента \en Type of element 
  MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента \en Create a copy of the element
  bool    IsSame   ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const override;
  bool    SetEqual ( const MbSpaceItem & ) override;          // \ru Сделать равным \en Make equal 
  void    Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override;             // \ru Преобразовать \en Transform
  void    Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override; // \ru Сдвиг \en Translation
  void    Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate around an axis
  double  DistanceToPoint( const MbCartPoint3D & ) const override; // \ru Расстояние до точки \en Distance to a point 
  void    AddYourGabaritTo( MbCube & ) const override; // \ru Добавить габарит кривой в куб. \en Add a bounding box of a curve to a cube. 

  void    GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void    SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of the object 
  void    GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void    SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  // \ru Общие функции кривой \en Common functions of curve 

  double  GetTMax()  const override;  // \ru Вернуть максимальное значение параметра \en Get the maximum value of parameter 
  double  GetTMin()  const override;  // \ru Вернуть минимальное значение параметра \en Get the minimum value of parameter 
  bool    IsClosed() const override; // \ru Проверка замкнутости кривой \en Check for curve closedness 
  // \ru Функции кривой для работы в области определения параметрической кривой \en Functions of curve for working at parametric curve domain 
  void    PointOn  ( double & t, MbCartPoint3D & ) const override; // \ru Точка на кривой \en Point on the curve 
  void    FirstDer ( double & t, MbVector3D & )    const override; // \ru Первая производная \en First derivative
  void    SecondDer( double & t, MbVector3D & )    const override; // \ru Вторая производная \en Second derivative
  void    ThirdDer ( double & t, MbVector3D & )    const override; // \ru Третья производная по t \en Third derivative with respect to t
  // \ru Функции кривой для работы вне области определения параметрической кривой \en Functions of curve for working outside the domain of parametric curve 
  void   _PointOn  ( double t, MbCartPoint3D & )  const override; // \ru Точка на расширенной кривой \en Point on the extended curve
  void   _FirstDer ( double t, MbVector3D & )     const override; // \ru Первая производная \en First derivative
  void   _SecondDer( double t, MbVector3D & )     const override; // \ru Вторая производная \en Second derivative
  void   _ThirdDer ( double t, MbVector3D & )     const override; // \ru Третья производная по t \en Third derivative with respect to t
  // \ru Вычислить значения точки и производных для заданного параметра. \en Calculate point and derivatives of object for given parameter. \~
  void    Explore( double & t, bool ext,
                   MbCartPoint3D & pnt, MbVector3D & fir, MbVector3D * sec, MbVector3D * thir ) const override;

  // \ru Построить NURBS копию кривой \en Create a NURBS copy of the curve 
  MbNurbs3D * NurbsCurve( const MbCurveIntoNurbsInfo & ) const override;
  MbCurve3D * NurbsCurve( const MbNurbsParameters & ) const override; // \ru Построить Nurbs-копию кривой \en Construct NURBS copy of the curve 

  // \ru Построить усеченную кривую c учетом заданной трехмерной точности. \en Construct a trimmed curve with the given three-dimensional accuracy.
  MbCurve3D * Trimmed( double t1, double t2, int sense, const MbDimAccuracy & xyzEps, bool saveParamLenAndLaw ) const override;
  using MbCurve3D::Trimmed; // \ru Метод базового класса MbCurve3D без учета трехмерной точности. Точность по умолчанию - MbDimAccuracy::threeDimAcc.  \en Method of MbCurve3D base class without three-dimensional accuracy. The default accuracy is MbDimAccuracy::threeDimAcc.

  // \ru Сдвинуть параметр t на расстояние len по направлению \en Shift the parameter t by the distance 'len' in the direction 
  bool    DistanceAlong( double & t, double len, int curveDir, double eps = Math::metricPrecision,
                                 VERSION version = Math::DefaultMathVersion() ) const override;

  void    Inverse( MbRegTransform * iReg = nullptr ) override; // \ru Изменить направление \en Change direction
  double  GetMetricLength() const override;  // \ru Метрическая длина кривой \en Metric length of a curve
  double  Step         ( double t, double sag ) const override; // \ru Вычисление шага по стрелке прогиба. \en Calculation of parameter step by the sag.
  double  DeviationStep( double t, double angle ) const override; // \ru Вычисление шага по углу отклонения нормали. \en Calculation of parameter step by the deviation angle.   
  double  MetricStep   ( double t, double length ) const override; // \ru Вычисление шага параметра по длине. \en Calculation of parameter step by the given length. 

  /// \ru Вычислить габарит кривой. \en Calculate bounding box of a curve. 
  void    CalculateGabarit( MbCube &  ) const override;
  // \ru Вычислить габарит в локальной системе координат. \en Calculate bounding box in the local coordinate system. 
  void    CalculateLocalGabarit( const MbMatrix3D & into, MbCube &  ) const override;

  // \ru Ближайшая проекция точки на кривую \en The nearest point projection to the curve 
  bool    NearPointProjection( const MbCartPoint3D &, double & t, bool ext, MbRect1D * tRange = nullptr ) const override;

  bool    operator == ( const MbLine3D & with ) const; // \ru Проверка на равенство \en Check for equality 
  bool    operator != ( const MbLine3D & with ) const; // \ru Проверка на неравенство \en Check for inequality 

  void    GetCentre      ( MbCartPoint3D &  c ) const override; // \ru Выдать центр кривой \en Get the center of curve
  void    GetWeightCentre( MbCartPoint3D & wc ) const override; // \ru Выдать центр тяжести кривой \en Get the center of gravity of the curve
  bool    IsStraight( bool ignoreParams = false ) const override; // \ru Является ли линия прямолинейной \en Whether the line is straight
  bool    IsPlanar  ( double accuracy = METRIC_EPSILON ) const override; // \ru Является ли кривая плоской \en Whether a curve is planar

  double  GetParamToUnit() const override; // \ru Дать приращение параметра, осреднённо соответствующее единичной длине в пространстве \en Get increment of parameter, corresponding to the unit length in space
  double  GetParamToUnit( double t ) const override; // \ru Дать приращение параметра, соответствующее единичной длине в пространстве \en Get increment of parameter, corresponding to the unit length in space

  // \ru Дать плоскую проекцию кривой \en Get a planar projection of curve 
  MbCurve * GetMap( const MbMatrix3D &, MbRect1D * pRgn = nullptr,
                    VERSION version = Math::DefaultMathVersion(), bool * coincParams = nullptr ) const override;
  MbCurve * GetMapPsp( const MbMatrix3D &, double zNear,
                       MbRect1D * pRgn = nullptr ) const override;
  void    CalculatePolygon( const MbStepData & stepData, MbPolygon3D & ) const override; // \ru рассчитать полигон \en Calculate a polygon
  bool    IsSimilarToCurve( const MbCurve3D & curve, double precision = METRIC_PRECISION ) const override; // \ru Подобные ли кривые для объединения (слива) \en Whether the curves for union (joining) are similar 

  const MbCartPoint3D & GetOrigin()    const { return origin;   }
  const MbVector3D    & GetDirection() const { return direction;}
        MbCartPoint3D & SetOrigin()    { return origin;   }
        MbVector3D    & SetDirection() { return direction;}
  void    SetOrigin( const MbCartPoint3D & p ) { origin    = p; }
  void    SetDirection( const MbVector3D & v ) { direction = v; }
  bool    RoundColinear ( const MbLine3D & with, double eps = Math::angleRegion ) const; // \ru Грубая коллинеарность \en Rough collinearity 
  bool    Colinear ( const MbLine3D & with, double eps = Math::angleRegion ) const; // \ru Коллинеарность \en Collinearity 
  bool    Orthogonal( const MbLine3D & with, double eps = Math::angleRegion ) const; // \ru Ортогональность \en Orthogonality 

private:
  void    operator = ( const MbLine3D & ); // \ru Не реализовано. \en Not implemented. 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbLine3D )
};

IMPL_PERSISTENT_OPS( MbLine3D )


//------------------------------------------------------------------------------
// \ru Проверка на равенство \en Check for equality 
// ---
inline bool MbLine3D::operator == ( const MbLine3D &with ) const {
  return (origin == with.origin) && ( direction.Colinear( with.direction ) );
}


//------------------------------------------------------------------------------
// \ru Проверка на неравенство \en Check for inequality 
// ---
inline bool MbLine3D::operator != ( const MbLine3D &with ) const {
  return !(*this == with);
}


//------------------------------------------------------------------------------
// \ru Грубая коллинеарность по скалярному произведению \en Rough colinearity by dot product 
// ---
inline bool MbLine3D::RoundColinear( const MbLine3D &with, double eps ) const {
  return direction.RoundColinear( with.direction, eps );
}


//------------------------------------------------------------------------------
// \ru Коллинеарность \en Collinearity 
// ---
inline bool MbLine3D::Colinear( const MbLine3D &with, double eps ) const {
  return direction.Colinear( with.direction, eps );
}


//------------------------------------------------------------------------------
// \ru Ортогональность \en Orthogonality 
// ---
inline bool MbLine3D::Orthogonal( const MbLine3D &with, double eps ) const {
  return direction.Orthogonal( with.direction, eps );
}

                                  
#endif // __CUR_LINE3D_H
