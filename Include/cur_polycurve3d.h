////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Кривая в трехмерном пространстве, заданная контрольными точками.
         \en Curve in three-dimensional space, defined by control points. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CUR_POLYCURVE3D_H
#define __CUR_POLYCURVE3D_H


#include <curve3d.h>
#include <cur_line_segment3d.h>
#include <mb_cube.h>


//------------------------------------------------------------------------------
/** \brief \ru Кривая в трехмерном пространстве, заданная точками.
           \en Curve in three-dimensional space, defined by points. \~
  \details \ru Родительский класс кривых в трехмерном пространстве, заданных контрольными точками: 
    MbBezier3D, MbCubicSpline3D, MbHermit3D, MbNurbs3D, MbPolyline3D. \n
           \en Parent class of curves in three-dimensional space, defined by control points: 
    MbBezier3D, MbCubicSpline3D, MbHermit3D, MbNurbs3D, MbPolyline3D. \n \~
  \ingroup Curves_3D
*/
// ---
class MATH_CLASS MbPolyCurve3D : public MbCurve3D {
protected :
  ptrdiff_t             uppIndex;         ///< \ru Количество участков кривой (равно количество контрольных точек минус единица). \en Count of curve pieces (is equal to count of control points minus one). 
  SArray<MbCartPoint3D> pointList;        ///< \ru Множество контрольных точек. \en Set of control points. 
  bool                  closed;           ///< \ru Признак замкнутости кривой. \en An attribute of curve closedness. 
  
  mutable atomic_double metricLength;     ///< \ru Метрическая длина кривой. \en Metric length of curve. 
  mutable atomic_double lengthEvaluation; ///< \ru Оценочная длина кривой. \en Estimated length of a curve. 
  mutable MbCube        cube;             ///< \ru Габаритный куб кривой. \en Bounding box of curve. 

protected:
  MbPolyCurve3D();
  MbPolyCurve3D( const MbPolyCurve3D & );
public :
  virtual ~MbPolyCurve3D();

public :
  VISITING_CLASS( MbPolyCurve3D ); 

  // \ru Общие функции математического объекта \en Common functions of the mathematical object 

  //virtual MbeSpaceType  IsA()  const = 0; // \ru Тип элемента \en Type of element
  MbeSpaceType  Type() const override;     // \ru Тип элемента \en Type of element 
  //virtual MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const = 0;          // \ru Сделать копию элемента \en Create a copy of the element
  //virtual bool    IsSame   ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const = 0;
  //virtual bool    SetEqual ( const MbSpaceItem & ) = 0; // \ru Сделать равным \en Make equal
  //virtual void    Transform( const MbMatrix3D &, MbRegTransform * = nullptr )    = 0; // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  //virtual void    Move     ( const MbVector3D &, MbRegTransform * = nullptr )    = 0; // \ru Сдвиг \en Translation
  //virtual void    Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) = 0; // \ru Повернуть вокруг оси \en Rotate about an axis
  void    AddYourGabaritTo( MbCube & ) const override; // \ru Добавь свой габарит в куб \en Add your own bounding box into the cube 
  void    PrepareIntegralData( const bool forced ) const override; // \ru Рассчитать временные (mutable) данные объекта. \en Calculate temporary (mutable) data of an object.

  //virtual void    GetProperties( MbProperties & ) = 0; // \ru Выдать свойства объекта \en Get properties of the object
  //virtual void    SetProperties( const MbProperties & ) = 0; // \ru Записать свойства объекта \en Set properties of the object
  void    GetBasisItems ( RPArray<MbSpaceItem> & ) override;   // \ru Дать базовые поверхности \en Get basis surfaces 
  void    GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void    SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  void    Refresh() override; // \ru Сбросить все временные данные \en Reset all temporary data 

  // \ru Общие функции кривой \en Common functions of curve 

  // \ru Функции кривой для работы в области определения параметрической кривой \en Functions of curve for working at parametric curve domain 
  //virtual void    PointOn  ( double & t, MbCartPoint3D & ) const = 0; // \ru Точка на кривой \en Point on the curve
  //virtual void    FirstDer ( double & t, MbVector3D & ) const = 0; // \ru Первая производная \en The first derivative
  //virtual void    SecondDer( double & t, MbVector3D & ) const = 0; // \ru Вторая производная \en The second derivative
  //virtual void    ThirdDer ( double & t, MbVector3D & ) const = 0; // \ru Третья производная по t \en The third derivative with respect to t
  // \ru Вычислить значения точки и производных для заданного параметра. \en Calculate point and derivatives of object for given parameter. \~
  //virtual void    Explore( double & t, bool ext,
  //                         MbCartPoint3D & pnt, MbVector3D & fir, MbVector3D * sec, MbVector3D * thir ) const = 0;

  //virtual MbNurbs3D * NurbsCurve( const MbCurveIntoNurbsInfo & ) const = 0;

  //virtual double  GetTMax()  const = 0;   // \ru Вернуть максимальное значение параметра \en Get the maximum value of parameter
  //virtual double  GetTMin()  const = 0;   // \ru Вернуть минимальное значение параметра \en Get the minimum value of parameter
  bool    IsClosed() const override;       // \ru Замкнутость кривой \en A curve closedness 
  //virtual void    Inverse( MbRegTransform * iReg = nullptr ) = 0; // \ru Изменить направление \en Change direction

  double  GetMetricLength() const override;      // \ru Выдать метрическую длину ограниченной кривой \en Get metric length of bounded curve 
  double  GetLengthEvaluation() const override;  // \ru Оценка метрической длины кривой \en Estimation of metric length of the curve 

  bool    IsPlanar  ( double accuracy = METRIC_EPSILON ) const override; // \ru Является ли кривая плоской \en Whether the curve is planar 
  bool    GetPlacement( MbPlacement3D & place, PlanarCheckParams params = PlanarCheckParams() ) const override; // \ru Заполнить плейсемент, ести кривая плоская \en Fill the placement if the curve is planar 
  // \ru Дать плоскую кривую и плейсмент, если пространственная кривая плоская (после использования вызывать DeleteItem на двумерную кривую) \en Get the planar curve and placement if the spatial curve is planar (call DeleteItem for two-dimensional curve after using) 
  //virtual bool    GetPlaneCurve( MbCurve *& curve2d, MbPlacement3D & place, bool saveParams, PlanarCheckParams params = PlanarCheckParams() ) const = 0;

  // \ru Общие функции полигональной кривой \en Common functions of polygonal curve 

  virtual void    Rebuild() = 0; // \ru Перестроить кривую \en Rebuild curve 
  virtual void    SetClosed  ( bool cls );                            // \ru Установить признак замкнутости \en Set attribute of closedness 
  virtual ptrdiff_t GetNearPointIndex( const MbCartPoint3D & ) const; // \ru Выдать индекс точки, ближайшей к заданной \en Get index of the point nearest to the given one 
  virtual void    AddPoint         ( const MbCartPoint3D & );         // \ru Добавить точку в конец массива \en Add a point to the end of the array 
  virtual void    InsertPoint( ptrdiff_t index, const MbCartPoint3D & ) = 0;   // \ru Добавить точку \en Add a point 
  virtual void    InsertPoint( double t,  const MbCartPoint3D &, double ) = 0; // \ru Добавить точку \en Add a point
  virtual void    RemovePoint( ptrdiff_t index );                        // \ru Удалить точку \en Remove the point 
  virtual void    RemovePoints();                                        // \ru Удалить все точки \en Delete all points 
  virtual bool    ChangePoint( ptrdiff_t index, const MbCartPoint3D & ); // \ru Заменить точку \en Replace a point 
  virtual void    GetPoint   ( ptrdiff_t index, MbCartPoint3D & ) const; // \ru Выдать точку \en Get point 
  virtual size_t  GetPointsCount() const;                                // \ru Выдать количество точек \en Get count of points 
  virtual void    GetRuleInterval( ptrdiff_t index, double & t1, double & t2 ) const = 0; // \ru Выдать интервал влияния точки кривой \en Get the interval of point influence 

  virtual bool    CheckParam ( double & t, ptrdiff_t & i0, ptrdiff_t & i1, double & t0, double & t1 ) const = 0; // \ru Загнать параметр получить локальный индексы и параметры \en Drive parameter into domain, get local indices and parameters 
  virtual double  GetParam( ptrdiff_t i ) const = 0;
  size_t  GetCount() const override;
  // \ru Периодичность \en Periodicity  
  virtual bool    IsPointsPeriodic( ptrdiff_t & begPointNumber,          // \ru Номер первой точки \en Index of the first point 
                                    ptrdiff_t & endPointNumber,          // \ru Номер последней точки \en Index of the last point 
                                    ptrdiff_t & period )         const;  // \ru Количество точек в периоде \en Count of points in period 

  const   MbCube & GetGabarit() const; // \ru Выдать габарит кривой \en Get bounding box of curve 

  size_t    GetPointListCount() const { return pointList.Count(); }
  ptrdiff_t GetPointListMaxIndex() const { return pointList.MaxIndex(); }

  template <class PointsVector>
  void    GetPoints( PointsVector & pnts ) const { std::copy( pointList.begin(), pointList.end(), std::back_inserter( pnts ) ); } ///< \ru Вернуть массив контрольных точек. \en Get array of control points.  
  void    GetPointList( SArray<MbCartPoint3D>  & pnts ) const { pnts.assign( pointList.begin(), pointList.end() ); } // \ru Получить характерные точки \en Get control points 
  void    GetPointList( c3d::SpacePointsVector & pnts ) const { pnts.assign( pointList.begin(), pointList.end() ); } // \ru Получить характерные точки \en Get control points 

  const   MbCartPoint3D & GetPointList( size_t i ) const { return pointList[i]; } // \ru Характерные точки \en Control points 
          MbCartPoint3D & SetPointList( size_t i )       { return pointList[i]; } // \ru Характерные точки \en Control points 

  ptrdiff_t GetUppIndex() const { return uppIndex; }
  size_t    GetSegmentsCount() const { return (uppIndex > 0) ? (uppIndex + (!!closed)) : 0; }

  template <class SegmentsVector>
  void    GetLineSegments( SegmentsVector & segments, double eps = PARAM_REGION ) const; ///< \ru Выдать массив отрезков. \en Get the array of segments. 

  // \ru Дать информацию для функции NurbsCurve \en Get information for NurbsCurve function 
  bool    NurbsParam( double epsilon, double & pmin, double & pmax, 
                      ptrdiff_t & i1, double & t1, ptrdiff_t & i2, double & t2 ) const;  

protected:
  // \ru Сдвинуть параметр t на расстояние len. \en Move parameter t on the metric distance len. 
  bool    CompositeDistanceAlong( double & t, double len, int curveDir, double eps, const SArray<double> & tList ) const;
  // \ru Рассчитать метрическую длину сегмента кривой. \en Calculate metric length of curve segment.
  double  SegmentCalculateLength( double w1, double w2, size_t n, double * x, double * w ) const;
  // \ru Сдвинуть параметр t на расстояние len. \en Move parameter t on the metric distance len. 
  bool    SegmentDistanceAlong( double & t1, double ln, int curveDir, double eps, double stepMax, size_t n, double * x, double * w ) const;

private:
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
  void    operator = ( const MbPolyCurve3D & ); 

  DECLARE_PERSISTENT_CLASS( MbPolyCurve3D )
};

IMPL_PERSISTENT_OPS( MbPolyCurve3D )


//------------------------------------------------------------------------------
// \ru Выдать массив отрезков. \en Get the array of segments.
// ---
template <class SegmentsVector>
void MbPolyCurve3D::GetLineSegments( SegmentsVector & segments, double eps ) const
{
  size_t count = pointList.size();

  ptrdiff_t cnt = count;
  if ( !IsClosed() )
    cnt--;

  if ( cnt > 0 )
    segments.reserve( segments.size() + (size_t)cnt );

  c3d::SpaceCurveSPtr segment;

  for ( ptrdiff_t i = 0; i < cnt; i++ ) {
    ptrdiff_t inext = (i + 1) % count;

    if ( !pointList[i].IsSame( pointList[inext], eps ) ) {
      segment = new MbLineSegment3D( pointList[i], pointList[inext] );
      segments.push_back( static_cast<MbLineSegment3D *>( segment.get() ) );
      ::DetachItem( segment );
    }
  }
}


#endif // __CUR_POLYCURVE3D_H
