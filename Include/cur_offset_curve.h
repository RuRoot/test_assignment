////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Эквидистантная продолженная кривая.
         \en Offset extended curve. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CUR_OFFSET_CURVE_H
#define __CUR_OFFSET_CURVE_H


#include <curve.h>
#include <mb_rect.h>
#include <mb_matrix.h>


class MbRegDuplicate;
class MbRegTransform;


//------------------------------------------------------------------------------
/** \brief \ru Эквидистантная продолженная кривая.
           \en Offset extended curve. \~
  \details \ru Эквидистантная продолженная кривая строится смещением точек базовой кривой вдоль нормали к ней. \n
    Параметры "offsetTmin, offsetTmax" задают смещение точек базовой кривой в точках tmin, tmax.
    Радиус-вектор кривой в методе PointOn(double&t,MbCartPoint3D&r) описывается векторной функцией \n
    r(t) = basisCurve(t) + (Offset0(t) * normal(t)), где normal(t) - нормаль базовой кривой. \n
    Базовой кривой для эквидистантной кривой не может служить другая эквидистантная кривая.
    В подобной ситуации выполняется переход к первичной базовой кривой.
    В качестве базовой кривой для эквидистантной кривой не могут быть заданы контур или ломаная.
           \en Offset extended curve is constructed by shifting points of the base curve along a normal to it. \n
    The "offsetTmin, offsetTmax" parameters set shift of base curve on begin and end points.
    Radius-vector of the curve in the method PointOn(double&t,MbCartPoint3D&r) is described by the function \n
    r(t) = basisCurve(t) + (Offset0(t) * normal(t)), where normal(t) - normal of base curve. \n
    Base curve for offset curve can not be other offset curve.
    In such situation it changes to the initial base curve. 
    A contour or polyline cannot be specified as the base curve for an equidistant curve. \~
  \ingroup Curves_2D
*/
// ---
class MATH_CLASS MbOffsetCurve : public MbCurve {
protected :
  MbCurve *      basisCurve;   ///< \ru Базовая кривая (всегда не nullptr) \en Base curve (always not nullptr).
  double         tmin;         ///< \ru Начальный параметр basisCurve. \en Start parameter of basisCurve. 
  double         tmax;         ///< \ru Конечный  параметр basisCurve. \en End parameter of basisCurve. 
  bool           closed;       ///< \ru Замкнутость basisCurve. \en Closedness of basisCurve. 
  double         offsetTmin;   ///< \ru Смещение от базовой кривой по нормали в точке tmin. \en The offset from the base curve along normal in point tmin. 
  double         offsetTmax;   ///< \ru Смещение от базовой кривой по нормали в точке tmax. \en The offset from the base curve along normal in point tmax. 
  MbeOffsetType  type;         ///< \ru Тип смещения точек: константный, линейный или кубический. \en The type of points offset: constant, or linear, or cubic.
  double         deltaTmin;    ///< \ru Увеличение tmin параметра базовой кривой. \en Increase of tmin of base curve parameter. 
  double         deltaTmax;    ///< \ru Увеличение tmax параметра базовой кривой. \en Increase of tmax of base curve parameter. 
  MbMatrix       transform;    ///< \ru Матрица преобразования (используется при разных масштабных коэффициентах трансформации). \en A transformation matrix (is used for different scale transformation). 

  mutable MbRect        rect;         ///< \ru Габаритный прямоугольник \en Bounding box 
  mutable atomic_double metricLength; ///< \ru Метрическая длина \en The metric length 

public :
  MbOffsetCurve( const MbCurve & bc, double dist, double t1, double t2, bool same );      
  MbOffsetCurve( const MbCurve & bc, double dist, double t1, double t2, const MbMatrix & matr, bool same );
  MbOffsetCurve( const MbCurve & bc, double dist, bool same );
  MbOffsetCurve( const MbCurve & bc, double dist, const MbMatrix & matr, bool same );

  /** \brief \ru Конструктор по базовой кривой и смещению c приращениями параметров.
             \en Constructor by base curve and offset with increments of parameters. \~  
    \details \ru Смещение задано на краях параметрической области базовой кривой и может изменяться по константному, линейному и кубическому законам.\n  
      Приращение параметров нужно использовать для изменения области определения кривой относительно базовой кривой.
             \en The offset displacement is defined in the begin and the end of the parametric region of the base curve and can be changed by constant, linear and cubic laws.\n  
      Increment of parameters needs to be used for change of curve domain relative to base curve. \~
    \param[in] bc -   \ru Базовая кривая.
                      \en Base curve. \~
    \param[in] d1 -   \ru Смещение в точке Tmin базовой кривой.
                      \en Offset distance on point Tmin of base curve. \~
    \param[in] d2 -   \ru Смещение в точке Tmax базовой кривой.
                      \en Offset distance on point Tmax of base curve. \~
    \param[in] t  -   \ru Тип смещения точек: константный, линейный или кубический.
                      \en The offset type: constant, or linear, or cubic. \~
    \param[in] t1 -   \ru Минимальный параметр кривой.
                      \en The maximum parameter of offset curve. \~
    \param[in] t2 -   \ru Максимальный параметр кривой.
                      \en The minimum parameter of offset curve. \~
    \param[in] matr - \ru Матрица преобразования (единичная или анизотропная).
                      \en The matrix (single or anisotropic). \~               
    \param[in] same - \ru Признак использования оригинала базовой кривой, а не ее копии.
                      \en Attribute of usage of original of base curve, not copy. \~
  */
  MbOffsetCurve( const MbCurve & bc, double d1, double d2, MbeOffsetType t, double t1, double t2, const MbMatrix & matr, bool same );

protected :
  MbOffsetCurve( const MbOffsetCurve &, MbRegDuplicate * ireg );
private:
  MbOffsetCurve( const MbOffsetCurve & ); // \ru Не реализовано. \en Not implemented. 
public :
  virtual ~MbOffsetCurve ();

public :
  VISITING_CLASS( MbOffsetCurve ); 

  /** \ru \name Общие функции геометрического объекта.
      \en \name Common functions of geometric object.
      \{ */

  MbePlaneType  IsA() const override;                        // \ru Тип элемента \en Type of element 
  bool        IsSimilar ( const MbPlaneItem & ) const override;   // \ru Являются ли элементы подобными \en Whether the elements are similar 
  bool        SetEqual( const MbPlaneItem & ) override;         // \ru Сделать элементы равными \en Make equal elements 
  MbPlaneItem & Duplicate( MbRegDuplicate * = nullptr ) const override;   // \ru Сделать копию элемента \en Create a copy of the element
  void        Transform( const MbMatrix & matr, MbRegTransform * ireg = nullptr, const MbSurface * newSurface = nullptr ) override; // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move( const MbVector & to, MbRegTransform * = nullptr, const MbSurface * newSurface = nullptr ) override; // \ru Сдвиг \en Translation
  void        Rotate( const MbCartPoint & pnt, const MbDirection & angle, MbRegTransform * = nullptr, const MbSurface * newSurface = nullptr ) override; // \ru Поворот \en Rotation
  bool        IsSame( const MbPlaneItem & other, double accuracy = LENGTH_EPSILON ) const override; // \ru Является ли кривая curve копией данной кривой ? \en Whether the 'curve' curve is duplicate of current curve. 
  void        AddYourGabaritTo( MbRect & ) const override; // \ru Добавь свой габарит в прямой прям-к \en Add bounding box into a straight box 

  /** \} */
  /** \ru \name Функции инициализации.
      \en \name Initialization functions.
      \{ */  
  /** \brief \ru Инициализация по смещению и приращениям параметров.
              \en Initialization by offset and increments of parameters. \~  
    \details \ru Смещение задано на краях параметрической области базовой кривой и может изменяться по константному, линейному и кубическому законам.\n  
      Приращение параметров нужно использовать для изменения области определения кривой относительно базовой кривой.
              \en The offset displacement is defined in the begin and the end of the parametric region of the base curve and can be changed by constant, linear and cubic laws.\n  
      Increment of parameters needs to be used for change of curve domain relative to base curve. \~          
    \param[in] d1 - \ru Смещение в точке Tmin базовой кривой.
                    \en Offset distance on point Tmin of base curve. \~
    \param[in] d2 - \ru Смещение в точке Tmax базовой кривой.
                    \en Offset distance on point Tmax of base curve. \~
    \param[in] t  - \ru Тип смещения точек: константный, линейный или кубический.
                    \en The offset type: constant, or linear, or cubic. \~
    \param[in] t1 - \ru Увеличение tmin параметра
                    \en Increment of tmin parameter \~
    \param[in] t2 - \ru Увеличение tmax параметра
                    \en Increment of tmax parameter \~
  */
  void        Init( double d1, double d2, MbeOffsetType t, double t1, double t2 );  
  void        Init( double d, double t1, double t2 );
  void        Init( double t1, double t2 );
  /** \} */

  /** \ru \name Функции описания области определения кривой.
      \en \name Functions for curve domain description.
      \{ */
  double      GetTMax()   const override; // \ru Вернуть максимальное значение параметра \en Get the maximum value of parameter 
  double      GetTMin()   const override; // \ru Вернуть минимальное значение параметра \en Get the minimum value of parameter 
  bool        IsClosed()  const override; // \ru Проверка замкнутости кривой \en Check for curve closedness 
  double      GetPeriod() const override; // \ru Вернуть период \en Get period  
  /** \} */

  /** \ru \name Функции для работы в области определения кривой.
            Функции PointOn, FirstDer, SecondDer, ThirdDer,... корректируют параметр
            при выходе его за пределы области определения.
      \en \name Functions for working in the curve's domain. 
            Functions PointOn, FirstDer, SecondDer, ThirdDer,... correct parameter
            when it is outside domain.
      \{ */
  void        PointOn  ( double & t, MbCartPoint & pnt ) const override; // \ru Точка на кривой \en Point on curve
  void        FirstDer ( double & t, MbVector    & fd  ) const override; // \ru Первая производная \en First derivative
  void        SecondDer( double & t, MbVector    & sd  ) const override; // \ru Вторая производная \en Second derivative
  void        ThirdDer ( double & t, MbVector    & td  ) const override; // \ru Третья производная \en Third derivative
  /** \} */

  /** \ru \name Функции для работы внутри и вне области определения кривой. 
            Функции _PointOn, _FirstDer, _SecondDer, _ThirdDer,... не корректируют параметр
            при выходе за пределы области определения. При выходе за область определения незамкнутая
            кривая, в общем случае, продолжается по касательной, которую она имеет на соответствующем конце.
      \en \name Functions for working inside and outside the curve's domain. 
            Functions _PointOn, _FirstDer, _SecondDer, _ThirdDer,... do not correct parameter
            when it is out of domain bounds. When parameter is out of domain bounds, an unclosed
            curve is extended by tangent vector at corresponding end point in general case.
      \{ */
  void       _PointOn  ( double t, MbCartPoint & p ) const override;
  void       _FirstDer ( double t, MbVector    & v ) const override;
  void       _SecondDer( double t, MbVector    & v ) const override;
  void       _ThirdDer ( double t, MbVector    & v ) const override;
  /** \} */

  /** \ru \name Функции доступа к группе данных для работы внутри и вне области определения параметра кривой.
      \en \name Functions for get of the group of data inside and outside the curve's domain of parameter.
      \{ */
  // \ru Вычислить значения точки и производных для заданного параметра. \en Calculate point and derivatives of object for given parameter. \~
  void        Explore( double & t, bool ext,
                       MbCartPoint & pnt, MbVector & fir, MbVector * sec, MbVector * thir ) const override;
  /** \} */

  /** \ru \name Функции движения по кривой
      \en \name Function of moving by curve
    \{ */
  double      Step( double t, double sag ) const override; // \ru Вычисление шага аппроксимации \en Calculation of approximation step 
  double      DeviationStep( double t, double angle ) const override; // \ru Вычисление шага аппроксимации с учетом угла отклонения \en Calculation of approximation step with consideration of deviation angle 

  /** \} */
  /** \ru \name Общие функции кривой
      \en \name Common function of curve.
      \{ */
  bool        IsStraight( bool ignoreParams = false ) const override; // \ru Является ли линия прямолинейной \en Whether the line is straight 
  MbCurve  *   Offset( double rad ) const override;  // \ru Смещение смещенной кривой \en Offset of the offset curve
  void        Inverse( MbRegTransform * iReg = nullptr ) override; // \ru Изменение направления кривой на противоположное \en Change to the opposite direction of a curve
  void        Refresh() override; // \ru Сбросить все временные данные \en Reset all temporary data 
  void        PrepareIntegralData( const bool forced ) const override; // \ru Рассчитать временные (mutable) данные объекта. \en Calculate temporary (mutable) data of an object.
  
  // \ru Проекция точки на кривую \en Point projection on the curve 
  // \ru Функция не работает для самопересекающейся кривой \en This function does not work for self-intersecting curve // BUG_54628
  // virtual double      PointProjection( const MbCartPoint & pnt ) const override;

  MbeState    Deformation( const MbRect &, const MbMatrix & ) override;  // \ru Деформация \en Deformation 
  // \ru Удалить часть смещенной кривой между параметрами t1 и t2 \en Delete a part of a offset curve between parameters t1 and t2 
  MbeState    DeletePart( double t1, double t2, MbCurve *& part2 ) override;
  // \ru Оставить часть смещенной кривой между параметрами t1 и t2 \en Save a part of a offset curve between t1 and t2 parameters 
  MbeState    TrimmPart ( double t1, double t2, MbCurve *& part2 ) override;
  // \ru Построить усеченную кривую c учетом заданной двумерной точности. \en Construct a trimmed curve with the given two-dimensional accuracy.
  MbCurve *   Trimmed   ( double t1, double t2, int sense, const MbDimAccuracy & xyEps, bool saveParamLenAndLaw ) const override;
  using MbCurve::Trimmed; // \ru Метод базового класса MbCurve без учета двумерной точности. Точность по умолчанию - MbDimAccuracy::twoDimAcc.  \en Method of MbCurve base class without two-dimensional accuracy. The default accuracy is MbDimAccuracy::twoDimAcc.

  MbNurbs *   NurbsCurve( const MbCurveIntoNurbsInfo & ) const override;

  bool        Break( MbNurbs &nurbs, double t1, double t2, ptrdiff_t degree );

  bool        IsBounded()    const override; // \ru Признак ограниченной кривой \en Attribute of a bounded curve
  bool        IsDegenerate( double eps = Math::LengthEps ) const override; // \ru Проверка вырожденности кривой \en Check for curve degeneracy 

  bool        HasLength( double & length ) const override;
  double      GetMetricLength() const override; // \ru Метрическая длина кривой \en Metric length of a curve 
  double      GetLengthEvaluation() const override; // \ru Оценка метрической длины кривой \en Evaluation of the metric length of the curve  

  const MbCurve & GetBasisCurve() const override;
        MbCurve & SetBasisCurve() override;

  bool        GetAxisPoint( MbCartPoint & p ) const override; // \ru Точка для построения оси \en A point to the axis construction 
  size_t      GetCount() const override; // \ru Количество разбиений для прохода в операциях \en The number of partitions for passage in the operations  

  void        OffsetCuspPoint( SArray<double> & tCusps, double dist ) const override; // \ru Определение особых точек офсетной кривой \en Determination of singular points of the offset curve 
  double      Curvature( double t ) const override; // \ru Кривизна кривой \en Curvature of the curve 
  // \ru Сдвинуть параметр t на расстояние len \en Move parameter t on the distance len 
  bool        DistanceAlong( double & t1, double ln, int curveDir, double eps = Math::LengthEps,
                             VERSION version = Math::DefaultMathVersion() ) const override;
  bool        IsSimilarToCurve( const MbCurve & curve, double precision = PARAM_PRECISION ) const override; // \ru Подобные ли кривые для объединения (слива) \en Whether the curves are similar for merge (joining) 

  void        GetProperties( MbProperties & properties ) override;  // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & properties ) override;  // \ru Записать свойства объекта \en Set properties of the object 
  void        GetBasisPoints( MbControlData & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  bool        IsContinuousDerivative( bool & contLength, bool & contDirect, c3d::DoubleVector * params = nullptr, double epsilon = EPSILON ) const override; // \ru Непрерывна ли первая производная? \en Have the first derivative the continuous?
  // \ru Устранить разрывы первых производных по длине. \en Eliminate the discontinuities of the first derivative at length.
  bool        SetContinuousDerivativeLength( VERSION version, double epsilon = EPSILON ) override; 

  void        SetBasisCurve( MbCurve & ); // \ru Установить базовую кривую \en Set the base curve 
  // \ru Тип смещения точек. \en The type of points offset.
  MbeOffsetType GetOffsetType() const { return type; }
  // \ru Постоянное ли смещение точек? \en Is const the offset type?
  bool        IsConstOffset() const { return ( (type == off_Empty) || (type == off_Const) ); }
  // \ru Величина смещения. \en The offset distance. 
  double      GetDistance( size_t i ) const { 
    i = i % 2;
    if ( i == 0 )
      return offsetTmin;
    return offsetTmax;
  }
  // \ru Средняя величина смещения. \en The average offset distance. 
  double      GetDistance() const { return ( offsetTmin + offsetTmax ) / 2; }
          
  /** \brief \ru Установить величины смещения.
              \en Set offset distances. \~  
    \param[in] d - \ru Новая величина смещения
                    \en New offset distance. \~
  */
  void        SetDistance( double d, size_t i );
  // \ru Установить постоянную величину смещения. Set new constant offset distance.
  void        SetDistance( double d );

  void        CalculateGabarit( MbRect & ) const override; // \ru Определить габаритный прямоугольник кривой. \en Detect the bounding box of a curve.
  double      CalculateMetricLength() const override; // \ru Вычислить метрическую длину кривой. \en Calculate the metric length of a curve. 

  const MbRect & GetGabarit() const;
  void        SetDirtyGabarit() const;
  const   double &    GetDmin() const { return deltaTmin; } // \ru Дать расширение начала \en Get extension of start 
  const   double &    GetDmax() const { return deltaTmax; } // \ru Дать расширение конца \en Get extension of end 
  void        SetDmin( double d ) { deltaTmin = d; } // \ru Установить расширение начала \en Set extension of start 
  void        SetDmax( double d ) { deltaTmax = d; } // \ru Установить расширение конца \en Set extension of end 

  double      GetBegExtend() const { return deltaTmin; } // \ru Дать расширение начала \en Get extension of start 
  double      GetEndExtend() const { return deltaTmax; } // \ru Дать расширение конца \en Get extension of end 
  int         ExtendedParam( double &t ) const; // \ru Проверка, лежит ли параметр в пределах \en Check if parameter is in range 
  void        GetCurves( RPArray<MbCurve> & curves ); // \ru Дать составляющие кривые \en Get curves 

  bool        operator == ( const MbOffsetCurve & ) const; // \ru Проверка на равенство \en Check for equality 
  bool        operator != ( const MbOffsetCurve & ) const; // \ru Проверка на неравенство \en Check for inequality 

  bool        SubstrateParamOn( double &t, double &delta ) const; // \ru Находится ли параметр в пределах подложки \en Check if parameter is in the range of substrate 
  bool        IsMatrixSingle() const { return transform.IsSingle(); } ///< \ru Является ли матрица преобразования единичной. \en Whether the transformation matrix is unit. 
  const MbMatrix & GetMatrix() const { return transform; } ///< \ru Матрица преобразования. \en A transformation matrix. 

  /** \} */

private:
  // \ru Вычисление эквидистанты и её производных. \en The offset calculation and it derivatives calculation.
  double      Offset0  ( double t ) const;
  double      OffsetT  ( double t ) const;
  double      OffsetTT ( double t ) const;
  double      OffsetTTT( double t ) const;

  void        operator = ( const MbOffsetCurve & ); // \ru Не реализовано. \en Not implemented. 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbOffsetCurve )
}; // MbOffsetCurve

IMPL_PERSISTENT_OPS( MbOffsetCurve )


#endif // __CUR_OFFSET_CURVE_H
