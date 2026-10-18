////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Составная функция.
         \en Composite Function. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __FUNC_COMPOSITE_FUNCTION_H
#define __FUNC_COMPOSITE_FUNCTION_H


#include <function.h>


//------------------------------------------------------------------------------
/** \brief \ru Составная функция.
           \en Composite function. \~
  \details \ru Скалярная составная функция скалярного параметра состоит из набора функций (сегментов). \n
      В составной функции начало каждого последующего сегмента стыкуется с концом предыдущего.
    Составная функция является замкнутой, если конец последнего сегмента стыкуется с началом первого сегмента.\n
      Начальное значение параметра составной функции равно нулю. 
    Параметрическая длина составной функции равна сумме параметрических длин составляющих её сегментов. \n
      В качестве сегментов составной функции не используются другие составные функции. 
    Если составную функцию нужно построить на основе других составных функций, 
    то последние должны рассматриваться как совокупность составляющих их функций, а не как единые функции.\n 
           \en Scalar composite function of scalar parameter consists of a set of functions (segments). \n \~
      The beginning of each subsequent segment of composite function is joined to the end of the previous one.
    Composite function is closed if the end of last segment is joined to the beginning of the first segment.\n
    If the segments of a composite function are not smoothly joined then the composite function will have breaks. 
      The initial value of the composite function is equal to zero. 
    The parametric length of a composite function is equal to the sum of the parametric lengths of components of its segments. \n
      Other composite curves are not used as segments of the composite function. 
    If it is required to create a composite function based on other composite functions, 
    then the latter must be regarded as a set of their functions, and not as single functions. \n 
  \ingroup Functions
*/
// ---
class MATH_CLASS MbCompositeFunction : public MbFunction {
protected :
  RPArray<MbFunction> segments; ///< \ru Множество сегментов составной функции. \en A set of composite function segments.
  bool   closed; ///< \ru Признак замкнутости функции. \en An attribute of function closedness.
  double tmin;   ///< \ru Начало области определения. \en Beginning of domain.
  double tmax;   ///< \ru Конец области определения. \en Ending of domain.

public :
  /// \ru Конструктор по функции. \en Constructor by function. 
  MbCompositeFunction( MbFunction & segment, bool same ); // \ru same - функции или их копии \en same - functions or their copies 
  /// \ru Конструктор по набору функции. \en Constructor by functions. 
  template <class FunctionVector>
  MbCompositeFunction( const FunctionVector & initSegments, bool same ); // \ru same - функции или их копии \en same - functions or their copies 
protected:
  MbCompositeFunction(); ///< \ru Пустой контур. \en Empty composite function. 
  MbCompositeFunction( const MbCompositeFunction & ); ///< \ru Конструктор копирования. \en Copy constructor.
public :
  virtual ~MbCompositeFunction();

public:

  // \ru Общие функции математического объекта. \en Common functions of mathematical object.
  MbeFunctionType  IsA  () const override; // \ru Тип элемента. \en A type of element.
  MbFunction & Duplicate() const override; // \ru Сделать копию элемента. \en Create a copy of the element. 
  bool    IsSame   ( const MbFunction & other, double accuracy = LENGTH_EPSILON ) const override; // \ru Являются ли объекты равными. \en Determine whether objects are equal 
  bool    IsSimilar( const MbFunction & ) const override; // \ru Являются ли объекты подобными. \en Determine whether objects are similar. 
  bool    SetEqual ( const MbFunction & ) override; // \ru Сделать равным \en Make equal 
  void    GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void    SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of the object 

  double  GetTMax  () const override; // \ru Вернуть максимальное значение параметра \en Get the maximum value of parameter 
  double  GetTMin  () const override; // \ru Вернуть минимальное значение параметра \en Get the minimum value of parameter 
  bool    IsClosed () const override; // \ru Замкнутость функции \en A function closedness 
  void    SetClosed( bool cl ) override; // \ru Замкнутость функции \en A function closedness 

  double  Value     ( double & t ) const override; // \ru Значение функции для t \en The value of function for a given t 
  double  FirstDer  ( double & t ) const override; // \ru Первая производная по t \en The first derivative with respect to t 
  double  SecondDer ( double & t ) const override; // \ru Вторая производная по t \en The second derivative with respect to t 
  double  ThirdDer  ( double & t ) const override; // \ru Третья производная по t \en The third derivative with respect to t 

  double _Value     ( double t ) const override; // \ru Значение функции для t \en The value of function for a given t 
  double _FirstDer  ( double t ) const override; // \ru Первая производная по t \en The first derivative with respect to t 
  double _SecondDer ( double t ) const override; // \ru Вторая производная по t \en The second derivative with respect to t 
  double _ThirdDer  ( double t ) const override; // \ru Третья производная по t \en The third derivative with respect to t 
  // \ru Вычислить значение и производные. \en Calculate value and derivatives of object for given parameter. \~
  void    Explore( double & t, bool ext,
                   double & val, double & fir, double * sec, double * thr ) const override;
  // \ru Вычислить аргумент t по значению функции. \en Calculate the argument t by the function value. 
  double  Argument( double & val ) const override;

  void    Inverse( MbRegTransform * iReg = nullptr ) override; // \ru Изменить направление \en Change direction
  double  Step( double t, double sag ) const override;
  double  DeviationStep( double t, double angle ) const override;
  /// \ru Создать функцию из части функции между параметрами t1 и t2 c выбором направления sense. \en Create a function in part of the function between the parameters t1 and t2 choosing the direction. 
  MbFunction * Trimmed( double t1, double t2, int sense ) const override;
  // \ru Разбить функцию точкой с параметром t и вернуть отрезанную часть. \en Function break by the parameter t, and cut off part of the function: begs == true - save the initial half, beg == false - save the final half.
  MbFunction * BreakFunction( double t, bool beg ) override;
  void    Break( double t1, double t2 ); ///< \ru Выделить часть функции. \en Select a part of a function.

  double  MinValue  ( double & t ) const override; // \ru Минимальное  значение функции \en The minimum value of function 
  double  MaxValue  ( double & t ) const override; // \ru Максимальное значение функции \en The maximum value of function 
  double  MidValue  () const override; // \ru Среднее значение функции \en The middle value of function 
  bool    IsGood    () const override; // \ru Корректность функции \en Correctness of function 

  bool    IsConst() const override;
  bool    IsLine () const override;
  /// \ru Сместить функцию. \en Shift a function. 
  void    SetOffsetFunc( double off, double scale ) override; 

  bool    SetLimitParam( double newTMin, double newTMax ) override; // \ru Установить область изменения параметра \en Set range of parameter 
  void    SetLimitValue( size_t n, double newValue ) override; // \ru Установить значение на конце ( 1 - в начале, 2 - в конце) \en Set the value at the end (1 - at start point, 2 - at end point) 
  double  GetLimitValue( size_t n ) const override; // \ru Дать значение на конце ( 1 - в начале, 2 - в конце) \en Get the value at the end (1 - at start point, 2 - at end point) 
  void    SetLimitDerive( size_t n, double newValue, double dt ) override; // \ru Установить значение на конце ( 1 - в начале, 2 - в конце) \en Set the value at the end (1 - at start point, 2 - at end point)
  double  GetLimitDerive( size_t n ) const override; // \ru Дать значение на конце ( 1 - в начале, 2 - в конце) \en Get the value at the end (1 - at start point, 2 - at end point)
  bool    InsertValue( double t, double newValue ) override; // \ru Установить значение для параметра t. \en Set the value for the parameter t. 

  /// \ ru Определение точек излома контура. \en The determination of contour smoothness break points.
  void    BreakPoints( std::vector<double> & vBreaks, double precision = ANGLE_REGION ) const override;

  /** \} */
  /** \ru \name Функции работы с сегментами контура
      \en \name Function for working with segments of contour
      \{ */

  /// \ru Инициализация по набору кривых (sameCurves - функции или их копии). \en Initialize by curves (sameCurves - curves or their copies). 
  template <class FunctionVector>
  bool    Init( const FunctionVector & initSegments, bool same, bool cls );

  /** \brief \ru Найти сегмент контура.
              \en Find a contour segment. \~
    \details \ru Найти сегмент контура по параметру контура. \n
              \en Find a contour segment by parameter on contour. \n \~
    \param[in,out] t - \ru Параметр контура.
                        \en Composite function parameter. \~
    \param[out] tSeg - \ru Параметр сегмента контура.
                        \en Composite function segment parameter. \~
    \return \ru Возвращает номер сегмента в случае успешного выполнения или -1.
            \en Returns the segment number in case of successful execution or -1. \~
  */
  ptrdiff_t FindSegment( double & t, double & tSeg ) const;

  /** \brief \ru Найти параметр контура.
              \en Find a contour segment. \~
    \details \ru Найти параметр контура по номеру сегмента и параметру сегмента. \n
              \en Find a contour parameter by segment number and segment parameter. \n \~
    \param[in] iSeg - \ru Номер сегмента (индекс).
                      \en Segment number (index). \~
    \param[in] tSeg - \ru Параметр сегмента контура.
                      \en Segment parameter. \~
    \return \ru Возвращает параметр контура или UNDEFINED_DBL в случае неудачи.
            \en Returns the contour parameter or UNDEFINED_DBL if failure. \~
  */
  double  FindParameter( size_t iSeg, double tSeg ) const;

  size_t  GetSegmentsCount() const { return segments.size(); } ///< \ru Выдать количество сегментов контура. \en Get the number of contour segments. 
  template <class CurvesVector>
  void    GetSegments( CurvesVector & curves ) const; ///< \ru Получить функции контура. \en Get contour segments. 

  void    DetachSegments(); ///< \ru Отцепить все сегменты контура. \en Detach all segments of contour. 
  void    DeleteSegments(); ///< \ru Отсоединить используемые сегменты и удалить остальные. \en Delete used segments and remove other segments. 

  void    DeleteSegment( size_t ind ); ///< \ru Удалить сегмент контура. \en Delete the segment of contour. 
  MbFunction * DetachSegment( size_t ind ); ///< \ru Отцепить сегмент контура. \en Detach the segment of contour. 

  const   MbFunction * GetSegment( size_t ind ) const { return segments[ind]; } ///< \ru Выдать сегмент контура по индексу. \en Get contour segment by the index.
          MbFunction * SetSegment( size_t ind )       { return segments[ind]; } ///< \ru Выдать сегмент контура по индексу. \en Get contour segment by the index. 

  void    SetSegment     ( MbFunction & newSegment, size_t ind, bool same ); ///< \ru Заменить сегмент в контуре. \en Replace a segment in the contour. 
  void    AddSegment     ( MbFunction & newSegment, bool same ); ///< \ru Добавить сегмент в контур. \en Add a segment to the contour. 
  void    AddAtSegment   ( MbFunction & newSegment, size_t ind, bool same ); ///< \ru Добавить сегмент в контур перед сегментом с индексом ind. \en Add a segment to the contour before the segment with index ind. 
  void    AddAfterSegment( MbFunction & newSegment, size_t ind, bool same ); ///< \ru Добавить сегмент в контур после сегмента с индексом ind. \en Add a segment to the contour after the segment with index ind. 

  /** \brief \ru Добавить (усеченную) копию сегмента в конец контура.
              \en Add a (truncated) segment copy to the end of the contour. \~
    \details \ru Добавить (усеченную) копию сегмента в конец контура. \n
              \en Add a (truncated) segment copy to the end of the contour. \n \~
    \param[in] pBasis- \ru Исходная функция.
                        \en Initial function. \~
    \param[in] t1 - \ru Начальный параметр усечения.
                    \en Truncation starting parameter. \~
    \param[in] t2 - \ru Конечный параметр усечения.
                    \en Truncation ending parameter. \~
    \param[in] sense - \ru Направление усеченной функции относительно исходной. \n
                sense =  1 - направление функции сохраняется.
                sense = -1 - направление функции меняется на обратное.
                \en Direction of a trimmed function in relation to an initial function.
                sense =  1 - direction does not change.
                sense = -1 - direction changes to the opposite value. \~
    \return \ru Возвращает в случае успешного выполнения ненулевой указатель на добавленную кривую.
            \en Returns, if successful, a non-zero pointer to the added function. \~
  */
  MbFunction * AddSegment( MbFunction & pBasis, double t1, double t2, int sense );

  void    SegmentsAdd( MbFunction & newSegment, bool calculateParamLength = true ); ///< \ru Добавить сегмент в контур без проверки. \en Add a segment to the contour without checking. 
  /// \ru Сбросить переменные кэширования. \en Reset variables caching.
  void    Clear() {
    CalculateParamLength();
    SetClosed();
  }
  /// \ru Управление распределением памяти в массиве segments. \en Control of memory allocation in the array "segments". 
  void    SegmentsReserve( size_t additionalSpace ) { segments.Reserve( additionalSpace ); } ///< \ru Зарезервировать место. \en Reserve space.  
  void    SegmentsAdjust ()                         { segments.Adjust();                   } ///< \ru Удалить лишнюю память. \en Free the unnecessary memory. 


  /// \ru Проверка непрерывности контура. \en Check for contour continuity. 
  bool    CheckConnection( double eps = METRIC_PRECISION ) const;
  void    CalculateParamLength(); ///< \ru Рассчитать параметрическую длину. \en Calculate parametric length. 
  void    CheckClosed( double eps ); ///< \ru Установить признак замкнутости контура. \en Set the closedness attribute of contour. 

private:
  void    SetClosed(); // \ru Проверить и установить признак замкнутости контура. \en Check and set closedness attribute of contour. 
  ptrdiff_t _FindSegment( double & t, double & tSeg ) const; // \ru Нахождение сегмента контура \en Finding of a contour segment 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbCompositeFunction )

  MbCompositeFunction & operator = ( const MbCompositeFunction & ); // Not implemented.

}; // MbCompositeFunction

IMPL_PERSISTENT_OPS( MbCompositeFunction )


//------------------------------------------------------------------------------
// \ru Конструктор по набору функций. \en Constructor by functions. 
// ---
template <class FunctionVector>
MbCompositeFunction::MbCompositeFunction( const FunctionVector & initSegments, bool same )
  : MbFunction  (                        )
  , segments   ( initSegments.size(), 1 )
  , closed     ( false                  )
  , tmin       ( 0.0                    )
  , tmax       ( 0.0                    )
{
  const size_t count = initSegments.size();

  if ( count > 0 ) {
    for ( size_t i = 0; i < count; ++i ) {
      const MbFunction * initSegment = initSegments[i];
      if ( initSegment != nullptr ) {
        if ( initSegment->IsA() == ft_CompositeFunction ) { // \ru Присланный контур удаляет владелец. \en contour should be deleted by owner.
          const MbCompositeFunction * cntr = static_cast<const MbCompositeFunction *>( initSegment );
          MbCompositeFunction * contour = const_cast<MbCompositeFunction *>( cntr );
          size_t cnt = contour->segments.size();
          for ( size_t j = 0; j < cnt; j++ ) {
            MbFunction * seg = contour->segments[j];
            if ( seg != nullptr ) {
              MbFunction * segment = same ? seg : static_cast<MbFunction *>( &seg->Duplicate() );
              segments.push_back( segment );
              segment->AddRef();
            }
          }
        }
        else {
          MbFunction * segment = same ? const_cast<MbFunction *>( initSegment ) : static_cast<MbFunction *>( &initSegment->Duplicate() );
          segments.push_back( segment );
          segment->AddRef();
        }
      }
    }
    Clear();
  }
}


//------------------------------------------------------------------------------
// \ru Инициализация по набору функций. \en Initialize by functions. 
// ---
template <class FunctionVector>
bool MbCompositeFunction::Init( const FunctionVector & initSegments, bool sames, bool cls )
{
  size_t count = initSegments.size();

  if ( count > 0 ) {
    ::AddRefItems( initSegments );
    DeleteSegments();
    segments.reserve( count );
    for ( size_t i = 0; i < count; ++i ) {
      const MbFunction * initSegment = initSegments[i];
      if ( initSegment != nullptr ) {
        if ( initSegment->IsA() == ft_CompositeFunction ) { // \ru Присланный контур удаляет владелец. \en contour should be deleted by owner.
          const MbCompositeFunction * cntr = static_cast<const MbCompositeFunction *>( initSegment );
          MbCompositeFunction * contour = const_cast<MbCompositeFunction *>( cntr );
          size_t cnt = contour->segments.size();
          for ( size_t j = 0; j < cnt; j++ ) {
            MbFunction * seg = contour->segments[j];
            if ( seg != nullptr ) {
              MbFunction * segment = sames ? seg : static_cast<MbFunction *>( &seg->Duplicate() );
              segments.push_back( segment );
              segment->AddRef();
            }
          }
        }
        else {
          MbFunction * segment = sames ? const_cast<MbFunction *>( initSegment ) : static_cast<MbFunction *>( &initSegment->Duplicate() );
          segments.push_back( segment );
          segment->AddRef();
        }
      }
    }
    ::DecRefItems( initSegments );
    CalculateParamLength();
    closed = cls;
    return true;
  }
  return false;
}


//------------------------------------------------------------------------------
// \ru Получить функции. \en Get segments. 
// ---
template <class CurvesVector>
void MbCompositeFunction::GetSegments( CurvesVector & funcs ) const
{
  size_t segmentsCnt = segments.size();
  funcs.reserve( funcs.size() + segmentsCnt );
  SPtr<MbFunction> function;
  for ( size_t k = 0; k < segmentsCnt; ++k ) {
    function = const_cast<MbFunction *>(segments[k]);
    if ( function != nullptr ) {
      funcs.push_back( function );
      ::DetachItem( function );
    }
  }
}


#endif // __FUNC_COMPOSITE_FUNCTION_H
