////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Косинусоида в двумерном пространстве.
         \en Cosinusoid in two-dimensional space. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CUR_COSINUSOID_H
#define __CUR_COSINUSOID_H


#include <curve.h>
#include <mb_placement.h>
#include <mb_rect.h>
#include <mb_rect1d.h>
#include <alg_diskrete_length_data.h>


class            MbRegDuplicate;
class            MbRegTransform;


//------------------------------------------------------------------------------
/** \brief \ru Косинусоида в двумерном пространстве.
           \en Cosinusoid in two-dimensional space. \~
  \details \ru Косинусоида расположена вдоль оси X локальной системы координат. \n
    Радиус-вектор кривой в методе PointOn(double&t,MbCartPoint3D&r) описывается векторной функцией\n
    r(t) = position.origin + (position.axisX ((tmin + t) - phase) / frequency) + (amplitude cos(tmin + t) position.axisY).\n
    Косинусоида приведена на рисунке ниже. 
                t = 0                                              
      amplitude | /\            /\                                 
                |/  \          /  \                                
     t = -phase |    \ tmin  t/    \ tmax                          
         ______/|_____\|_____/______\|_____________________________
              / |      \    /        \                             
             /  |       \  /                                       
            /   |        \/                                        
     y = amplitude cos(frequency x + phase)
           \en Cosinusoid located along the X-axis of the local coordinate system. \n
    Radius-vector of the curve in the method PointOn(double&t,MbCartPoint3D&r) is described by the vector function\n
    r(t) = position.origin + (position.axisX ((tmin + t) - phase) / frequency) + (amplitude cos(tmin + t) position.axisY).\n
    Cosinusoid is shown in the figure below. 
                t = 0                                              
      amplitude | /\            /\                                 
                |/  \          /  \                                
     t = -phase |    \ tmin  t/    \ tmax                          
         ______/|_____\|_____/______\|_____________________________
              / |      \    /        \                             
             /  |       \  /                                       
            /   |        \/                                        
     y = amplitude cos(frequency x + phase) \~
  \ingroup Curves_2D
*/
// ---
class MATH_CLASS MbCosinusoid: public MbCurve {
private :
  MbPlacement    position;     ///< \ru Локальная система координат. \en Local coordinate system. 
  double         frequency;    ///< \ru Циклическая частота (angular frequency). \en Angular frequency. 
  double         phase;        ///< \ru Начальная фаза в радианах. \en Initial phase in radians. 
  double         amplitude;    ///< \ru Амплитуда. \en Amplitude. 
  double         tmin, tmax;   ///< \ru Область определения (по умолчанию - один период). \en Domain (one period by default). 
  
  mutable MbRect        rect;         ///< \ru Габаритный прямоугольник. \en Bounding box. 
  mutable atomic_double metricLength; ///< \ru Метрическая длина. \en The metric length. 

public :
  // \ru Конструктор по амплитуде, начальной фазе и круговой частоте \en Constructor by amplitude, initial phase and angular frequency 
  MbCosinusoid( const double & am , const double & phase, const double & anf );
  // \ru Конструктор по амплитуде, начальной фазе и признаку начала параметра \en Constructor by amplitude, initial phase and attribute of the beginning of the parameter 
  // \ru ( от максимума косинуса ( x(0) = -фаза ) или от начала координат( x(0) = 0 ) ) \en ( from the maximum cosine ( x(0) = -phase ) or from the origin( x(0) = 0 ) ) 
  MbCosinusoid( const double & am , const double & phase = 0.0, bool maxBegin = true );
  // \ru Конструктор по расположению, амплитуде, начальной фазе и круговой частоте \en Constructor by location, amplitude, initial phase and angular frequency 
  MbCosinusoid( const MbPlacement & pos,
                const double      & am    = 0.0,
                const double      & phase = 0.0,
                const double      & anf   = 0.0 );
  // \ru Конструктор по расположению, амплитуде, начальной фазе и предельным параметрам \en Constructor by location, amplitude, initial phase and limit parameters 
  MbCosinusoid( const MbPlacement & pos, double am, double ph, double af, double t1, double t2 );
protected :
  MbCosinusoid( const MbCosinusoid & ); // \ru Конструктор копирования \en Copy-constructor 
public :
  virtual ~MbCosinusoid();

public:
  VISITING_CLASS( MbCosinusoid ); 

  // \ru \name Общие функции геометрического объекта. \en Common functions of a geometric object.

  MbePlaneType  IsA() const override; // \ru Тип элемента \en Type of element 
  bool        SetEqual  ( const MbPlaneItem & ) override;       // \ru Сделать элементы равными \en Make the elements equal
  bool        IsBounded () const override { return true; } // \ru Ограниченность кривой \en Bounded curve
  void        Transform ( const MbMatrix & matr, MbRegTransform * ireg = nullptr, const MbSurface * newSurface = nullptr ) override;  // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move      ( const MbVector & to, MbRegTransform * = nullptr, const MbSurface * newSurface = nullptr ) override; // \ru Сдвиг \en Translation
  void        Rotate    ( const MbCartPoint & pnt, const MbDirection & angle, MbRegTransform * = nullptr, const MbSurface * newSurface = nullptr ) override; // \ru Поворот \en Rotation
  bool        IsSame    ( const MbPlaneItem & other, double accuracy = LENGTH_EPSILON ) const override;
  MbPlaneItem & Duplicate ( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента \en Create a copy of the element
  void        Refresh() override; // \ru Сбросить все временные данные \en Reset all temporary data 
  void        PrepareIntegralData( const bool forced ) const override; // \ru Рассчитать временные (mutable) данные объекта. \en Calculate temporary (mutable) data of an object.

  void        AddYourGabaritTo( MbRect & r ) const override;  // \ru Добавь свой габарит в прямой прям-к \en Add bounding box into a straight box 
  void        CalculateGabarit( MbRect & r ) const override;  // \ru Определить габариты кривой \en Determine the bounding box of the curve 
  bool        IsVisibleInRect( const MbRect & r, bool exact = false ) const override; // \ru Виден ли объект в заданном прямоугольнике \en Whether the object is visible in the given rectangle 
  using      MbCurve::IsVisibleInRect;        

  // \ru \name Общие функции кривой. \en Common functions of curve.

  double      GetTMin () const override; // \ru Вернуть минимальное значение параметра \en Get the minimum value of parameter 
  double      GetTMax () const override; // \ru Вернуть максимальное значение параметра \en Get the maximum value of parameter 
  bool        IsClosed() const override; // \ru Проверка замкнутости \en Check for closedness 
  // \ru Функции для работы внутри области определения кривой. \en Functions for working inside of the curve domain. \~
  void        PointOn  ( double & t, MbCartPoint & p ) const override; // \ru Точка на кривой \en Point on the curve 
  void        FirstDer ( double & t, MbVector    & v ) const override; // \ru Первая производная \en First derivative 
  void        SecondDer( double & t, MbVector    & v ) const override; // \ru Вторая производная \en Second derivative 
  void        ThirdDer ( double & t, MbVector    & v ) const override; // \ru Третья производная \en Third derivative 
  // \ru Функции для работы внутри и вне области определения кривой. \en Functions for working inside and outside of the curve domain. \~
  void       _PointOn  ( double t,  MbCartPoint & p ) const override;
  void       _FirstDer ( double t,  MbVector    & v ) const override;
  void       _SecondDer( double t,  MbVector    & v ) const override;
  void       _ThirdDer ( double t,  MbVector    & v ) const override;
  // \ru Вычислить значения точки и производных для заданного параметра. \en Calculate point and derivatives of object for given parameter. \~
  void        Explore( double & t, bool ext,
                       MbCartPoint & pnt, MbVector & fir, MbVector * sec, MbVector * thir ) const override;

  bool        HasLength ( double & length ) const override;
  double      GetMetricLength() const override; // \ru Метрическая длина \en The metric length 
  void        Inverse( MbRegTransform * iReg = nullptr ) override;  // \ru Изменение направления кривой на противоположное \en Change to the opposite direction of a curve

  MbNurbs *   NurbsCurve( const MbCurveIntoNurbsInfo & ) const override;

  MbeState    DeletePart( double t1, double t2, MbCurve *& part2 ) override; // \ru Удалить часть кривой между параметрами t1 и t2 \en Delete a part of a curve between parameters t1 and t2 
  MbeState    TrimmPart ( double t1, double t2, MbCurve *& part2 ) override; // \ru Оставить часть кривой между параметрами t1 и t2 \en Save a curve part between t1 and t2 parameters 

  // \ru Построить усеченную кривую c учетом заданной двумерной точности. \en Construct a trimmed curve with the given two-dimensional accuracy.
  MbCurve *   Trimmed( double t1, double t2, int sense, const MbDimAccuracy & xyEps, bool saveParamLenAndLaw ) const override; // \ru Создание усеченной кривой \en Creation of a trimmed curve 
   using MbCurve::Trimmed; // \ru Метод базового класса MbCurve без учета двумерной точности. Точность по умолчанию - MbDimAccuracy::twoDimAcc.  \en Method of MbCurve base class without two-dimensional accuracy. The default accuracy is MbDimAccuracy::twoDimAcc.

  void        IntersectHorizontal( double y, SArray<double> & cross ) const override;  // \ru Пересечение кривой с горизонтальной прямой \en Intersection of curve with the horizontal line 
  void        IntersectVertical  ( double x, SArray<double> & cross ) const override;  // \ru Пересечение с вертикальной прямой \en Intersection with the vertical line 
  double      Step( double t, double sag ) const override; // \ru Вычисление шага аппроксимации \en Calculation of approximation step  
  double      DeviationStep( double t, double angle ) const override; // \ru Вычисление шага аппроксимации с учетом угла отклонения \en Calculation of approximation step with consideration of deviation angle 

  double      PointProjection( const MbCartPoint & pnt ) const override; // \ru Проекция точки на кривую \en Point projection on the curve 
  bool        NearPointProjection( const MbCartPoint & pnt, double xEpsilon, double yEpsilon, 
                                   double & t, bool ext, MbRect1D * tRange = nullptr ) const override; // \ru Проекция точки на кривую или её продолжение в области поиска проекции \en Point projection on the curve or its extension in the projection search area

  void        GetProperties( MbProperties & properties ) override;  // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & properties ) override;  // \ru Записать свойства объекта \en Set properties of the object 
  void        GetBasisPoints( MbControlData & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.
  // \ru Подобные ли кривые для объединения? \en Are the curves similar to merge? 
  bool        IsSimilarToCurve( const MbCurve & curve, double precision = PARAM_PRECISION ) const override; 

  const   MbPlacement & GetPlacement() const { return position; }
          MbPlacement & SetPlacement() { return position; }
  double      GetFrequency() const { return frequency; }
  double      GetPhase() const     { return phase; }
  double      GetAmplitude() const { return amplitude; }
  double      GetOwnTMin() const   { return tmin; }
  double      GetOwnTMax() const   { return tmax; }
  void        SetPlacement( const MbPlacement &pos );
  void        SetFrequency( double f ); 
  void        SetPhase    ( double p );
  void        SetAmplitude( double a );
  void        SetOwnTMin ( double t );
  void        SetOwnTMax ( double t );
  inline  void        CheckParam( double & t ) const;

  bool        IsHorizontal( double eps = Math::AngleEps ) const;  // \ru Проверка горизонтальности \en Check for horizontality 
  bool        IsVertical  ( double eps = Math::AngleEps ) const;  // \ru Проверка вертикальности \en Check for verticality 

  void        Init ( const MbCosinusoid & );
  void        Init ( double t1, double t2 ); 
  void        Init ( const MbPlacement & pos, double am, double ph, double af );
  void        Init1( CosinusoidPar & par, MbCartPoint & p1, MbCartPoint & p2, double & len, double & angle );
  void        Init2( CosinusoidPar & par, const MbCartPoint & p1, MbCartPoint & p2, const double & len, double & angle );
  void        Init3( CosinusoidPar & par, const MbCartPoint & p1, MbCartPoint & p2, double & len, const double & angle,
                     const DiskreteLengthData * = nullptr );
  void        Init4( CosinusoidPar & par, MbCartPoint & p1, const MbCartPoint & p2, const double & len, double & angle );
  void        Init5( CosinusoidPar & par, MbCartPoint & p1, const MbCartPoint & p2, double & len, const double & angle,
                     const DiskreteLengthData * = nullptr );
  void        Init6( CosinusoidPar & par, const MbCartPoint & p1, MbCartPoint & p2, const double & len, const double & angle );
  void        Init7( CosinusoidPar & par, MbCartPoint & p1, const MbCartPoint & p2, const double & len, const double & angle );
  void        Init8( CosinusoidPar & par, MbCartPoint & p1, MbCartPoint & p2, double & len, double & angle,
                     const DiskreteLengthData & diskrData, bool correctP1 );
  void        SpecInit( const CosinusoidPar &, const MbCartPoint & p1, double angle, double len );

private:
  void        operator = ( const MbCosinusoid & ); // \ru Не реализовано. \en Not implemented. 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbCosinusoid )
}; // MbCosinusoid

IMPL_PERSISTENT_OPS( MbCosinusoid )


//-------------------------------------------------------------------------------
// \ru Проверка параметра \en Check parameter 
// ---
inline void MbCosinusoid::CheckParam( double & t ) const {
  if ( t < 0 )
    t = 0;
  else if ( t > ( tmax - tmin ) )
    t = ( tmax - tmin );
}


#endif // __CUR_COSINUSOID_H
