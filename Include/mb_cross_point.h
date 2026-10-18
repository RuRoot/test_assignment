////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Точка на кривой. Точка пересечения двух кривых.
         \en Point on a curve. Intersection point of two curves. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MB_CROSS_POINT_H
#define __MB_CROSS_POINT_H


#include <mb_cart_point.h>


class   MbCurve;


//------------------------------------------------------------------------------
/** \brief \ru Точка на кривой.
           \en Point on a curve. \~
  \details \ru Точка на кривой, представленная в виде указателя на кривую и параметра точки на кривой.\n
           \en Point on a curve is represented as a pointer to the curve and a parameter of a point on the curve.\n \~
  \ingroup Point_Modeling
*/
// ---
template <class Curve>
class MbPointOnCurve {
public :
  double        t;     ///< \ru Параметрическая координата точки на кривой. \en Parametric coordinate of a point on a curve. 
  const Curve * curve; ///< \ru Указатель на кривую. \en Pointer to the curve. 

public:
  /// \ru Конструктор по умолчанию. \en Default constructor. 
  MbPointOnCurve(); 
  /// \ru Конструктор по параметру и кривой. \en Constructor by a parameter and a curve. 
  MbPointOnCurve( double initT, const Curve * initC );
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbPointOnCurve( const MbPointOnCurve<Curve> & other );
  /// \ru Деструктор. \en Destructor. 
  ~MbPointOnCurve();
public:
  /// \ru Инициализировать точку по параметру и кривой. \en Initialize a point by a parameter and a curve. 
  void   Init( double initT, const Curve * initC );
  /// \ru Оператор присваивания. \en Assignment operator. 
  void   operator = ( const MbPointOnCurve<Curve> & other );
};


//------------------------------------------------------------------------------
// \ru Конструктор по умолчанию. \en Default constructor. 
// ---
template <class Curve>
MbPointOnCurve<Curve>::MbPointOnCurve() 
  : t    ( 0.0  )
  , curve( nullptr )
{}


//------------------------------------------------------------------------------
// \ru Конструктор по параметру и кривой. \en Constructor by a parameter and a curve. 
// ---
template <class Curve>
MbPointOnCurve<Curve>::MbPointOnCurve( double initT, const Curve * initC )
  : t    ( initT )
  , curve( initC )
{}


//------------------------------------------------------------------------------
// \ru Конструктор по копирования. \en Copy-constructor. 
// ---
template <class Curve>
MbPointOnCurve<Curve>::MbPointOnCurve( const MbPointOnCurve<Curve> & other )
  : t    ( other.t     )
  , curve( other.curve )
{}


//------------------------------------------------------------------------------
// \ru Деструктор \en Destructor 
// ---
template <class Curve>
MbPointOnCurve<Curve>::~MbPointOnCurve()
{}


//------------------------------------------------------------------------------
// \ru Инициализировать точку по параметру и кривой. \en Initialize a point by a parameter and a curve. 
// ---
template <class Curve>
void MbPointOnCurve<Curve>::Init( double initT, const Curve * initC )
{ 
  t = initT;
  curve = initC;
}


//------------------------------------------------------------------------------
// \ru Оператор присваивания. \en Assignment operator. 
// ---
template <class Curve>
void MbPointOnCurve<Curve>::operator = ( const MbPointOnCurve<Curve> & other )
{ 
  t = other.t;
  curve = other.curve;
}


//------------------------------------------------------------------------------
/** \brief \ru Точка пересечения двух кривых.
           \en Intersection point of two curves. \~
  \details \ru Точка пересечения двух кривых, состоящая из двумерной точки пересечения, 
    ее параметрических координат на пересекающихся кривых и типа пересечения (простое пересечение или касание). \n
           \en Intersection point of two curves consisting of a two-dimensional intersection point, 
    its parametric coordinates on intersecting curves and type of intersection (simple intersection or tangent intersection). \n \~
  \ingroup Point_Modeling
*/
// ---
class MATH_CLASS MbCrossPoint {
public :
  MbCartPoint             p;    ///< \ru Двумерные координаты точки пересечения двух кривых. \en Two-dimensional coordinates of intersection points of two curves. 
  MbPointOnCurve<MbCurve> on1;  ///< \ru Параметрическая координата точки на первой кривой. \en Parametric coordinate of point on the first curve. 
  MbPointOnCurve<MbCurve> on2;  ///< \ru Параметрическая координата точки на второй кривой. \en Parametric coordinate of a point on the second curve. 
  MbeIntersectionType     form; ///< \ru Тип точки пересечения (ipt_Simple - нормальное пересечение, ipt_Tangent - касание). \en Type of intersection point (ipt_Simple - simple intersection, ipt_Tangent - tangent). 
public:
  /// \ru Конструктор по умолчанию. \en Default constructor. 
  MbCrossPoint()
    : p()
    , on1()
    , on2()
    , form( ipt_Simple )  
  {}
  /// \ru Конструктор по точке пересечения и ее параметрическим координатам на каждой кривой. \en Constructor by intersection point and its parametric coordinates on each curve. 
  MbCrossPoint( const MbCartPoint & pnt, const MbPointOnCurve<MbCurve> & pOn1, const MbPointOnCurve<MbCurve> & pOn2 )
    : p( pnt )
    , on1( pOn1 )
    , on2( pOn2 )
    , form( ipt_Simple )  
  {}
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbCrossPoint( const MbCrossPoint & other )
    : p( other.p )
    , on1( other.on1 )
    , on2( other.on2 )
    , form( other.form )  
  {}
  /// \ru Деструктор. \en Destructor. 
  ~MbCrossPoint()
  {}
public:
  /// \ru Поменять местами кривые с параметрами. \en Swap curves with parameters. 
  void  Swap();
  /// \ru Иницализировать точку по двумерной точке пересечения и параметрам пересечения. \en Initialize a point by a two-dimensional intersection point and parameters of intersection. 
  void  Init( const MbCartPoint & pnt, const MbPointOnCurve<MbCurve> & pOn1, const MbPointOnCurve<MbCurve> & pOn2 );

  /// \ru Установить тип пересечения. \en Set type of intersection. 
  void                SetFormType( MbeIntersectionType iType );
  /// \ru Получить тип пересечения. \en Get type of intersection. 
  MbeIntersectionType GetFormType() const;

  /// \ru Является точка пересечения касанием. \en Whether the intersection point is a tangent intersection. 
  bool  IsTangent() const;
  /// \ru Оператор сравнения. \en Comparison operator. 
  bool  operator == ( const MbCrossPoint & ) const;
  /// \ru Оператор присваивания. \en Assignment operator. 
  void  operator =  ( const MbCrossPoint & other );

}; // MbCrossPoint


//------------------------------------------------------------------------------
// \ru Поменять параметры точки пересечения \en Swap parameters of the intersection point. 
// ---
inline void MbCrossPoint::Swap()
{
  double tTmp = on1.t;
  on1.t       = on2.t;
  on2.t       = tTmp;

  const MbCurve * swap = on1.curve;
  on1.curve              = on2.curve;
  on2.curve              = swap;
}


//------------------------------------------------------------------------------
// \ru Иницализировать точку по двумерной точке пересечения и параметрам пересечения. \en Initialize a point by a two-dimensional intersection point and parameters of intersection. 
// ---
inline void MbCrossPoint::Init( const MbCartPoint & pnt, const MbPointOnCurve<MbCurve> & pOn1, const MbPointOnCurve<MbCurve> & pOn2 )
{
  p = pnt;
  on1 = pOn1;
  on2 = pOn2;
  form = ipt_Simple;
}


//------------------------------------------------------------------------------
// \ru Установить тип пересечения. \en Set type of intersection. 
// ---
inline void MbCrossPoint::SetFormType( MbeIntersectionType iType ) {
  form = iType;
}


//------------------------------------------------------------------------------
// \ru Получить тип пересечения. \en Get type of intersection. 
// ---
inline MbeIntersectionType MbCrossPoint::GetFormType() const { 
  return form;
}


//------------------------------------------------------------------------------
// \ru Является точка пересечения касанием. \en Whether the intersection point is a tangent intersection. 
// ---
inline bool MbCrossPoint::IsTangent() const {
  return (form == ipt_Tangent);
}


//------------------------------------------------------------------------------
// \ru Оператор сравнения. \en Comparison operator. 
// ---
inline bool MbCrossPoint::operator == ( const MbCrossPoint & point ) const
{
  return ( ::fabs(p.x - point.p.x)     < Math::LengthEps &&
           ::fabs(p.y - point.p.y)     < Math::LengthEps &&
           ::fabs(on1.t - point.on1.t) < Math::paramEpsilon &&
           ::fabs(on2.t - point.on2.t) < Math::paramEpsilon);
}


//------------------------------------------------------------------------------
// \ru Оператор присваивания. \en Assignment operator. 
// ---
inline void MbCrossPoint::operator = ( const MbCrossPoint & other )
{
  p = other.p;
  on1 = other.on1;
  on2 = other.on2;
  form = other.form;
}


#endif // __MB_CROSS_POINT_H
