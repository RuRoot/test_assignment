////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Ломаная линия в трехмерном пространстве.
         \en Polyline in three-dimensional space. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CUR_POLYLINE3D_H
#define __CUR_POLYLINE3D_H


#include <templ_c_array.h>
#include <curve.h>
#include <cur_polycurve3d.h>
#include <vector>


class     MbPolyline;
class     MbItem;


//------------------------------------------------------------------------------
/** \brief \ru Ломаная линия в трехмерном пространстве.
           \en Polyline in three-dimensional space. \~
  \details \ru Ломаная линия в трехмерном пространстве определяется контрольными точками pointList.
    Параметр ломаной в контрольных точках принимают целочисленные значения, начиная с нуля. 
    Ломаная проходит через свои контрольные точки при целочисленных значениях параметра. 
    Параметр ломаной изменяется от нуля до k, 
    где k - количество контрольных точек минус один для не замкнутой ломаной и k - количество контрольных для замкнутой ломаной. 
    Производная ломаной на кождом участке постоянна и равна вектору, построенному между двумя соседними контрольными точками.
           \en Polyline in three-dimensional space is defined by 'pointList' control points.
    Parameters of polyline at control points take on integer values starting from zero. 
    Polyline passes through its control points at integer values of parameter. 
    Parameter of a polyline changes from zero to 'k', 
    where 'k' - count of control points minus one for an open polyline and 'k' - count of control points for a closed polyline. 
    Derivative of polyline is constant at each piece and is equal to vector constructed between two neighboring control points. \~ 
  \ingroup Curves_3D
*/
// ---
class MATH_CLASS MbPolyline3D : public MbPolyCurve3D {
private :
  ptrdiff_t segmentsCount; ///< \ru Число сегментов ломаной. \en Count of segments of polyline.  

public :
  /// \ru Конструктор отрезка. \en Constructor of a segment.
  MbPolyline3D( const MbCartPoint3D & p1, const MbCartPoint3D & p2 );
  /// \ru Конструктор по набору точек и признаку замкнутости. \en Constructor by points and closedness state.
  MbPolyline3D( const SArray<MbCartPoint3D> & initList, bool cls )
    : MbPolyCurve3D(   )
    , segmentsCount( 0 )
  {
    Init( initList, cls );
  }
  /// \ru Конструктор по набору точек и признаку замкнутости. \en Constructor by points and closedness state.
  MbPolyline3D( const std::vector<MbCartPoint3D> & initList, bool cls )
    : MbPolyCurve3D(   )
    , segmentsCount( 0 )
  {
    Init( initList, cls );
  }
  /// \ru Конструктор по плоской ломаной. \en Constructor by planar polyline.
  MbPolyline3D( const MbPolyline &, const MbPlacement3D & );
  MbPolyline3D( const CcArray<MbCartPoint3D> & initList, ptrdiff_t count, bool cls, double scale ); ///< \ru Используется в конвертере Parasolid. \en Used in converter of Parasolid.
protected :
  // \ru Конструктор копирования. \en Copy constructor.
  MbPolyline3D( const MbPolyline3D & );
public :
  virtual ~MbPolyline3D();

public :
  VISITING_CLASS( MbPolyline3D ); 

  /// \ru Инициализация по другой ломаной. \en Initialization by another polyline.
  void    Init( const MbPolyline3D & );
  /// \ru Инициализация по другой плоской ломаной. \en Initialization by another planar polyline.
  void    Init( const MbPolyline &, const MbPlacement3D & );
  /// \ru Инициализация по точкам и признаку замкнутости. \en Initialization by points and an attribute of closedness.
  template <class PointsVector>
  bool    Init( const PointsVector & initList, bool cls )
  {
    if ( initList.size() > 1 ) {
      pointList = initList;
      uppIndex = (ptrdiff_t)pointList.size() - 1;
      closed = cls;
      // if curve is closed then the start and the end points have to be different
      if ( uppIndex>1 && closed && c3d::EqualPoints( pointList.front(), pointList.back(), Math::metricRegion ) ) {
        pointList.erase( pointList.begin() + uppIndex );
        uppIndex--;
      }
      segmentsCount = ( uppIndex > 0 ) ? ( uppIndex + !!closed ) : 0;
      Refresh();
      return true;
    }
    return false;
  }
  /// \ru Построение прямоугольника. \en Construction of a rectangle. 
  void    Init( const MbCartPoint3D & p1, const MbCartPoint3D & p2 );

  // \ru Общие функции математического объекта \en Common functions of the mathematical object 

  MbeSpaceType  IsA() const override; // \ru Тип элемента \en Type of element 
  MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента. \en Create a copy of the element.
  bool    IsSame   ( const MbSpaceItem &, double accuracy = LENGTH_EPSILON ) const override;
  bool    SetEqual ( const MbSpaceItem & ) override;          // \ru Сделать равным. \en Make equal. 
  void    Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override;             // \ru Преобразовать. \en Transform.
  void    Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override;             // \ru Сдвиг. \en Translation.
  void    Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси. \en Rotate about an axis.
  double  DistanceToPoint( const MbCartPoint3D & ) const override;// \ru Расстояние до точки. \en Distance to a point. 

  void    GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  void    SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта. \en Set properties of the object. 

  // \ru Общие функции полилинии \en Common functions of polyline 

  // \ru Функции кривой для работы в области определения параметрической кривой \en Functions of curve for working at parametric curve domain 
  void    PointOn  ( double & t, MbCartPoint3D & ) const override;// \ru Точка на кривой. \en Point on the curve. 
  void    FirstDer ( double & t, MbVector3D & ) const override; // \ru Первая производная. \en The first derivative.
  void    SecondDer( double & t, MbVector3D & ) const override; // \ru Вторая производная. \en The second derivative.
  void    ThirdDer ( double & t, MbVector3D & ) const override; // \ru Третья производная. \en The third derivative.
  // \ru Вычислить значения точки и производных для заданного параметра. \en Calculate point and derivatives of object for given parameter. \~
  void    Explore( double & t, bool ext,
                   MbCartPoint3D & pnt, MbVector3D & fir, MbVector3D * sec, MbVector3D * thir ) const override;

  // \ru Построить NURBS копию кривой \en Create a NURBS copy of the curve 
  MbNurbs3D * NurbsCurve( const MbCurveIntoNurbsInfo & ) const override;
  MbCurve3D * NurbsCurve( const MbNurbsParameters & ) const override;

  // \ru Построить усеченную кривую c учетом заданной трехмерной точности. \en Construct a trimmed curve with the given three-dimensional accuracy.
  MbCurve3D * Trimmed( double t1, double t2, int sense, const MbDimAccuracy & xyzEps, bool saveParamLenAndLaw ) const override;
  using MbCurve3D::Trimmed; // \ru Метод базового класса MbCurve3D без учета трехмерной точности. Точность по умолчанию - MbDimAccuracy::threeDimAcc.  \en Method of MbCurve3D base class without three-dimensional accuracy. The default accuracy is MbDimAccuracy::threeDimAcc.

  void        Trimm( SArray<MbCartPoint3D> & points, double t1, double t2, double eps = METRIC_EPSILON ) const;

  double  GetTMin() const override; // \ru Вернуть минимальное значение параметра. \en Get the minimum value of the parameter. 
  double  GetTMax() const override; // \ru Вернуть максимальное значение параметра. \en Get the maximum value of parameter. 
  void    Inverse( MbRegTransform * iReg = nullptr ) override; // \ru Изменить направление \en Change direction

  double  Step         ( double t, double sag ) const override;    // \ru Шаг параметра с учетом радиуса кривизны \en Step of parameter with consideration of curvature 
  double  DeviationStep( double t, double angle ) const override;  // \ru Шаг параметра по заданному углу отклонения касательной \en Step of parameter by a given angle of deviation of tangent 

  bool    IsStraight( bool ignoreParams = false ) const override; // \ru Признак прямолинейности кривой \en An attribute of curve straightness. 

  void    CalculateGabarit( MbCube & ) const override; // \ru Определить габариты кривой. \en Determine the bounding box of a curve. 
  void    CalculateLocalGabarit( const MbMatrix3D &, MbCube & ) const override; // \ru Рассчитать габарит относительно л.с.к. \en Calculate bounding box relative to the local coordinate system. 

  double  CalculateMetricLength() const override;      // \ru Посчитать метрическую длину. \en Calculate the metric length. 
  void    GetCentre      ( MbCartPoint3D & wc ) const override; // \ru Посчитать центр кривой. \en Calculate the center of a curve. 
  void    GetWeightCentre( MbCartPoint3D & wc ) const override; // \ru Посчитать центр тяжести кривой. \en Calculate the gravity center of the curve. 
  // \ru Дать плоскую кривую и плейсмент, если пространственная кривая плоская (после использования вызывать DeleteItem на двумерную кривую). \en Get the planar curve and placement if the spatial curve is planar (call DeleteItem for two-dimensional curve after using). 
  bool    GetPlaneCurve( MbCurve *& curve2d, MbPlacement3D & place, bool saveParams, PlanarCheckParams params = PlanarCheckParams() ) const override;

  // \ru Общие функции полигональной кривой \en Common functions of polygonal curve 

  void    Rebuild() override; // \ru Перестроить кривую \en Rebuild curve 
  void    GetRuleInterval( ptrdiff_t index, double & t1, double & t2 ) const override; // \ru Выдать интервал влияния точки кривой. \en Get the interval of point influence. 

  // \ru Функции только 3D кривой \en Functions of 3D curve only 
  bool    NearPointProjection( const MbCartPoint3D &, double & t, bool ext, MbRect1D * tRange = nullptr ) const override; // \ru Ближайшая проекция точки на кривую. \en The nearest point projection to the curve.
  void    InsertPoint( ptrdiff_t index, const MbCartPoint3D &         ) override; // \ru Добавить точку \en Add a point
  void    InsertPoint( double t,  const MbCartPoint3D &, double ) override; // \ru Добавить точку \en Add a point
  bool    CheckParam ( double & t, ptrdiff_t & i0, ptrdiff_t & i1, double & t0, double & t1 ) const override; // \ru Установить параметр. \en Set parameter. 
  double  GetParam( ptrdiff_t i ) const override;            // \ru Выдать параметр для точки с номером. \en Get parameter for a point with index. 

  void    CheckParameter( double & ) const;  ///< \ru Проверка параметра. \en Check parameter. 

  //virtual bool        GoThroughPoint( double t, MbCartPoint3D & p ); // \ru Пройти через точку. \en Pass through point. 

  MbCurve * GetMap( const MbMatrix3D &, MbRect1D * pRgn = nullptr,
                    VERSION version = Math::DefaultMathVersion(), bool * coincParams = nullptr ) const override; // \ru Дать плоскую проекцию кривой. \en Get a planar projection of a curve.
  MbCurve * GetMapPsp( const MbMatrix3D &, double zNear, MbRect1D * pRgn = nullptr ) const override; // \ru Дать перспективную плоскую проекцию кривой. \en Get a planar geometric projection of a curve.

  size_t  GetCount() const override;
  bool    IsSmoothConnected( double angleEps, bool ignoreLimits = false ) const override; // \ru Являются ли стыки контура\кривой гладкими? \en Whether the joints of a contour\curve are smooth. 

  bool    IsContinuousDerivative( bool & contLength, bool & contDirect, c3d::DoubleVector * params = nullptr, double epsilon = EPSILON ) const override; // \ru Непрерывна ли первая производная? \en Have the first derivative the continuous?
  // \ru Устранить разрывы первых производных по длине. \en Eliminate the discontinuities of the first derivative at length.
  bool    SetContinuousDerivativeLength( VERSION version, double epsilon = EPSILON ) override; 

public:
  ptrdiff_t   GetSegmentsCount() const { return segmentsCount; }

  double  Step         ( double t, double sag,   ThreeStates dir ) const;  ///< \ru Шаг параметра с учетом радиуса кривизны. \en Step of parameter with consideration of curvature. 
  double  DeviationStep( double t, double angle, ThreeStates dir ) const;  ///< \ru Шаг параметра по заданному углу отклонения касательной. \en Step of parameter by a given angle of deviation of tangent. 

  bool    UnClamped( bool );
  void    DeleteEqPoints( double absEps ); // \ru Удалить одинаковые точки \en Remove equal points 
  void    AddAt( const MbCartPoint3D & spsP, ptrdiff_t i );
private:
  void    operator = ( const MbPolyline3D & ); // \ru Не реализовано. \en Not implemented. 

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbPolyline3D )
};

IMPL_PERSISTENT_OPS( MbPolyline3D )


#endif // __CUR_POLYLINE3D_H
