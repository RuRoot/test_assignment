////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Эквидистантная кривая в трехмерном пространстве.
         \en Offset curve in three-dimensional space. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CUR_OFFSET_CURVE3D_H
#define __CUR_OFFSET_CURVE3D_H


#include <curve3d.h>
#include <surface.h>
#include <mb_axis3d.h>
#include <mb_cube.h>


class   MbSpine;


//------------------------------------------------------------------------------
/** \brief \ru Эквидистантная кривая в трехмерном пространстве.
           \en Offset curve in three-dimensional space. \~
  \details \ru Эквидистантная кривая строится смещением точек базовой кривой вдоль некоторого вектора, 
    направление которого может меняться вдоль кривой. \n
    Вектор offset задаёт смещение начальной точки базовой криаой.
    В процессе движения вдоль кривой вектор offset сохраняет своё положение в движущейся локальной системе координат,
    начало которой совпадает с текущей точкой базовой кривой.
    Одна из осей движущейся локальной системы координат всегда совпадает с касательной базовой кривой, 
    а две другие оси ортогональны ей.
    Базовой кривой для эквидистантной кривой не может служить другая эквидистантная кривая.
    В подобной ситуации выполняется переход к первичной базовой кривой.
    В качестве базовой кривой для эквидистантной кривой не могут быть заданы контур или ломаная.
           \en Offset curve is constructed by shifting points of the base curve along some vector, 
    direction of which can be changed along the curve. \n
    Vector "offset" sets the offset of start point of the base curve.
    While moving along a curve the vector "offset" keeps the position in the moving local coordinate system,
    origin coincides with the current point of the base curve.
    One of the axes of the moving local coordinate system is always the same as the tangent of the base curve, 
    and the other two axes are orthogonal to it.
    Base curve for offset curve can not be other offset curve.
    In this situation it changes to the initial base curve.
    A contour or polyline cannot be specified as the base curve for an equidistant curve. \~
  \ingroup Curves_3D
*/
// ---
class MATH_CLASS MbOffsetCurve3D : public MbCurve3D {
protected :
  MbSpine     *  basisCurve; ///< \ru Базовая кривая. \en The base curve. 
  double         tmin;       ///< \ru Начальный параметр basisCurve. \en Start parameter of basisCurve. 
  double         tmax;       ///< \ru Конечный  параметр basisCurve. \en End parameter of basisCurve. 
  bool           closed;     ///< \ru Замкнутость basisCurve. \en Closedness of basisCurve. 
  MbVector3D     offset;     ///< \ru Смещение в начальной точке. \en Offset in start point. 
  double         factorTmin; ///< \ru Множитель смещения offset в точке tmin базовой кривой. \en The offset multiplier in point tmin of base curve. 
  double         factorTmax; ///< \ru Множитель смещения offset в точке tmax базовой кривой. \en The offset multiplier in point tmax of base curve. 
  MbeOffsetType  type;       ///< \ru Тип смещения: константный, линейный или кубический. \en The type of offset: constant, or linear, or cubic.
  double         deltaTmin;  ///< \ru Увеличение tmin параметра базовой кривой. \en Increase of tmin of base curve parameter. 
  double         deltaTmax;  ///< \ru Увеличение tmax параметра базовой кривой. \en Increase of tmax of base curve parameter. 
  mutable MbCube cube;       ///< \ru Габаритный куб. \en Bounding box. 

public :
  /** \brief \ru Конструктор.
             \en Constructor. \~    
    \details \ru Конструктор эквидистантной кривой по спайну и вектору.\n
             \en Constructor by a curve and offset vector in start point.\n \~
    \param[in] baseCurve    - \ru Базовая кривая. 
                              \en The base curve. \~
    \param[in] offsetVector - \ru Вектор смещения начальной точки кривой. 
                              \en Offset in start point. \~
    \param[in] sameCurve    - \ru Использовать присланную кривую (true) или ее копию (false).
                              \en Use same curve (true) or copy (false). \~
    \param[in] ort          - \ru Ортогонализовать вектор к касательной кривой в начальной точке.
                              \en Ortogonalize offset vector (true) or same vector (false). \~
    \param[in] version      - \ru Версия исполнения.
                              \en The version. \~
  */
  MbOffsetCurve3D( const MbCurve3D & baseCurve, const MbVector3D & offsetVector, bool sameCurve, bool ort, VERSION version = Math::DefaultMathVersion() );
  /** \brief \ru Конструктор.
             \en Constructor. \~    
    \details \ru Конструктор эквидистантной кривой по спайну и вектору, с заданным вектором ориентации матрицы преобразования.\n
             \en Constructor by a curve and offset vector in start point, with defined vector of transformation matrix orientation.\n \~
    \param[in] baseCurve    - \ru Базовая кривая. 
                              \en The base curve. \~
    \param[in] offsetVector - \ru Вектор смещения начальной точки кривой. 
                              \en Offset in start point. \~
    \param[in] dir          - \ru Вектор ориентации матрицы преобразования. \~
                              \en Vector of transformation matrix orientation. \~
    \param[in] sameCurve    - \ru Использовать присланную кривую (true) или ее копию (false).
                              \en Use same curve (true) or copy (false). \~
    \param[in] ort          - \ru Ортогонализовать вектор к касательной кривой в начальной точке.
                              \en Ortogonalize offset vector (true) or same vector (false). \~
    \param[in] version      - \ru Версия исполнения.
                              \en The version. \~
  */
  MbOffsetCurve3D( const MbCurve3D & baseCurve, const MbVector3D & offsetVector, const MbVector3D & dir, bool sameCurve, bool ort, VERSION version );
  /** \brief \ru Конструктор.
             \en Constructor. \~    
    \details \ru Конструктор эквидистантной кривой по спайну и вектору в режим по нормали к поверхности. \n
                 Если кривая не является поверхностной кривой, то будет выполнена инициализация без использования поверхности. \n
                 Поверхность в аргументах нужна для выбора нужной поверхности, если кривая является кривой пересечения двух разных поверхностей. \n
             \en Constructor by a surface curve and offset vector in start point.\n 
                 If the curve is not a curve on surface, it will be initialized without using the surface. \n
                 A surface in arguments is needed to select the desired surface if the curve is a curve of intersection of two different surfaces. \n \~
    \param[in] baseCurve       - \ru Базовая кривая. 
                                 \en The base curve. \~
    \param[in] sameCurve       - \ru Использовать присланную кривую (true) или ее копию (false).
                                 \en Use same curve (true) or copy (false). \~
    \param[in] surface         - \ru Поверхность кривой или поверхность, подобная поверхности кривой.
                                 \en Curve surface or surface similar to curve surface. \~
    \param[in] offsetVector    - \ru Вектор смещения начальной точки кривой. 
                                 \en Offset in start point. \~
    \param[in] ort             - \ru Ортогонализовать вектор к касательной кривой в начальной точке.
                                 \en Ortogonalize offset vector (true) or same vector (false). \~
    \param[in] version         - \ru Версия исполнения.
                                 \en The version. \~
  */
  MbOffsetCurve3D( const MbCurve3D & baseCurve, bool sameCurve, c3d::ConstSurfaceSPtr & surface, const MbVector3D & offsetVector, bool ort, VERSION version = Math::DefaultMathVersion() );
private :
  MbOffsetCurve3D( const MbOffsetCurve3D & ); // \ru Не реализовано. \en Not implemented. 
  /** \brief \ru Инициализация эквидистантной кривой по спайну и вектору.
             \en Initialization by a curve and offset vector in start point. \~
   \details \ru Инициализация эквидистантной кривой по спайну и вектору, с заданным вектором ориентации матрицы преобразования.\n
            \en Initialization by a curve and offset vector in start point, with defined vector of transformation matrix orientation.\n \~
   \param[in] baseCurve    - \ru Базовая кривая.
                             \en The base curve. \~
   \param[in] offsetVector - \ru Вектор смещения начальной точки кривой.
                             \en Offset in start point. \~
   \param[in] dir          - \ru Вектор ориентации матрицы преобразования. \~
                             \en Vector of transformation matrix orientation. \~
   \param[in] sameCurve    - \ru Использовать присланную кривую (true) или ее копию (false).
                             \en Use same curve (true) or copy (false). \~
   \param[in] ort          - \ru Ортогонализовать вектор к касательной кривой в начальной точке.
                             \en Ortogonalize offset vector (true) or same vector (false). \~
   \param[in] version      - \ru Версия исполнения.
                             \en The version. \~
 */
  void InitOffset( const MbCurve3D &  c, 
                   const MbVector3D & off, 
                   const MbVector3D & dir, 
                         bool         same, 
                         bool         ort, 
                         VERSION      version );
protected:
  MbOffsetCurve3D( const MbOffsetCurve3D & init, MbRegDuplicate * ireg );

public :
  virtual ~MbOffsetCurve3D();

public:
  VISITING_CLASS( MbOffsetCurve3D );

          /** \brief \ru Инициализация по смещению и приращениям параметров.
                     \en Initialization by offset and increments of parameters. \~  
            \details \ru Смещение задано на краях параметрической области базовой кривой и может изменяться по константному, линейному и кубическому законам.\n  
              Приращение параметров нужно использовать для изменения области определения кривой относительно базовой кривой.
                     \en The offset displacement is defined in the begin and the end of the parametric region of the base curve and can be changed by constant, linear and cubic laws.\n  
              Increment of parameters needs to be used for change of curve domain relative to base curve. \~          
            \param[in] d1 -  \ru Смещение в точке Tmin базовой кривой.
                             \en Offset distance on point Tmin of base curve. \~
            \param[in] d2 -  \ru Смещение в точке Tmax базовой кривой.
                             \en Offset distance on point Tmax of base curve. \~
            \param[in] t  -  \ru Тип смещения точек: константный, линейный или кубический.
                             \en The offset type: constant, or linear, or cubic. \~
            \param[in] dt1 - \ru Изменение tmin параметра
                             \en The change of tmin parameter \~
            \param[in] dt2 - \ru Изменение tmax параметра
                             \en The change of tmax parameter \~
          */
  void        Init( double d1, double d2, MbeOffsetType t, double dt1, double dt2 );  

  // \ru Общие функции математического объекта \en Common functions of the mathematical object 

  MbeSpaceType  IsA() const override; // \ru Тип элемента \en Type of element 
  MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента \en Create a copy of the element
  bool        IsSame   ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const override;
  bool        SetEqual ( const MbSpaceItem & ) override;       // \ru Сделать равным \en Make equal 
  bool        IsSimilar( const MbSpaceItem & ) const override; // \ru Являются ли объекты подобными \en Whether the objects are similar 
  void        Transform( const MbMatrix3D &, MbRegTransform * ireg ) override; // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move     ( const MbVector3D &, MbRegTransform * ireg ) override; // \ru Сдвиг \en Translation
  void        Rotate   ( const MbAxis3D &, double angle, MbRegTransform * ireg ) override; // \ru Повернуть вокруг оси \en Rotate about an axis
  void        Refresh  () override; // \ru Сбросить все временные данные \en Reset all temporary data
  void        PrepareIntegralData( const bool forced ) const override; // \ru Рассчитать временные (mutable) данные объекта. \en Calculate temporary (mutable) data of an object.
  void        AddYourGabaritTo( MbCube & ) const override; // \ru Добавь свой габарит в куб \en Add bounding box into a cube

  void        GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of the object 
  void        GetBasisItems  ( RPArray<MbSpaceItem> & ) override;  // \ru Дать базовые объекты \en Get the basis objects
  void        GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  // \ru Общие функции кривой \en Common functions of curve 

  double      GetTMin() const override;
  double      GetTMax() const override;
  bool        IsClosed() const override; // \ru Замкнутость кривой \en A curve closedness 
  double      GetPeriod() const override; // \ru Период кривой \en Curve period 
  // \ru Функции кривой для работы в области определения параметрической кривой \en Functions of curve for working at parametric curve domain 
  void        PointOn  ( double & t, MbCartPoint3D & p ) const override; // \ru Точка на кривой \en Point on curve
  void        FirstDer ( double & t, MbVector3D & fd ) const override;     // \ru Первая производная \en First derivative
  void        SecondDer( double & t, MbVector3D & sd ) const override;     // \ru Вторая производная \en Second derivative
  void        ThirdDer ( double & t, MbVector3D & td ) const override;     // \ru Третья производная по t \en Third derivative with respect to t

  // \ru ЗАКОМЕНТАРЕНО в связи с необходимостью использовать строгое продолжение по касательной \en COMMENTED because it is necessary to use a strong extension by the tangent  
  //virtual void       _PointOn  ( double t, MbCartPoint3D &p ) const;    // \ru Точка на расширенной кривой \en Point on the extended curve 
  //virtual void       _FirstDer ( double t, MbVector3D &fd ) const;      // \ru Первая производная \en The first derivative 
  //virtual void       _SecondDer( double t, MbVector3D &sd ) const;      // \ru Вторая производная \en The second derivative 
  //virtual void       _ThirdDer ( double t, MbVector3D &td ) const;      // \ru Третья производная по t \en The third derivative with respect to t 
  // \ru Вычислить значения точки и производных для заданного параметра. \en Calculate point and derivatives of object for given parameter. \~
  void        Explore( double & t, bool ext,
                       MbCartPoint3D & pnt, MbVector3D & fir, MbVector3D * sec, MbVector3D * thir ) const override;

  double      Step( double t, double sag ) const override; // \ru Вычисление шага аппроксимации \en Calculation of approximation step 
  double      DeviationStep( double t, double angle ) const override;

  const MbCurve3D & GetBasisCurve() const override;
         MbCurve3D & SetBasisCurve() override;
//virtual MbCurve3D * Trimmed( double t1, double t2, int sense ) const override; // \ru Создание усеченной кривой \en Creation of a trimmed curve 
  void        Inverse( MbRegTransform * iReg = nullptr ) override; // \ru Изменить направление \en Change direction

  size_t      GetCount() const override;
  void        ChangeCarrier( const MbSpaceItem & item, MbSpaceItem & init ) override; // \ru Изменение носителя \en Changing of carrier 
  bool        IsStraight( bool ignoreParams = false ) const override; // \ru Является ли линия прямолинейной \en Whether the line is straight 
  bool        IsPlanar  ( double accuracy = METRIC_EPSILON ) const override; // \ru Является ли кривая плоской \en Whether a curve is planar 

  bool        GetPlacement( MbPlacement3D & place, PlanarCheckParams params = PlanarCheckParams() ) const override; // \ru Заполнить плейсемент, ести кривая плоская \en Fill the placement if curve is planar 

  bool        IsContinuousDerivative( bool & contLength, bool & contDirect, c3d::DoubleVector * params = nullptr, double epsilon = EPSILON ) const override; // \ru Непрерывна ли первая производная? \en Have the first derivative the continuous?
  // \ru Устранить разрывы первых производных по длине. \en Eliminate the discontinuities of the first derivative at length.
  bool        SetContinuousDerivativeLength( VERSION version, double epsilon = EPSILON ) override; 

  /// \ru Смещение в начальной точке. \en Offset in the start point. 
  const MbVector3D &  GetOffsetVector() const { return offset; } 
  // \ru Тип смещения точек. \en The type of points offset.
  MbeOffsetType GetOffsetType() const { return type; }
  // \ru Постоянное ли смещение точек? \en Is const the offset type?
  bool        IsConstOffset() const { return ( (type == off_Empty) || (type == off_Const) ); }
  // \ru Множитель смещения. \en The offset multiplier. 
  double      GetFactor( size_t i ) const { 
    i = i % 2;
    if ( i == 0 )
      return factorTmin;
    return factorTmax;
  }
  // \ru Средний множитель смещения. \en The average offset multiplier. 
  double      GetFactor() const { return ( factorTmin + factorTmax ) / 2; }
          
  /** \brief \ru Установить множитель смещения. \en Set offset multiplier. \~  
    \param[in] d - \ru Новый множитель смещения. \en New offset multiplier. \~
  */
  void        SetFactor( double d, size_t i );
  // \ru Установить постоянный множитель смещения. Set new constant offset multiplier.
  void        SetFactor( double d );
  // \ru Проверить факторы и тип. \en Check factors and typr.
  void        CheckFactor();

  const   MbCube &    GetGabarit() const; // \ru Выдать габарит кривой \en Get the bounding box of curve 
  bool        IsSelfIntersect() const;
  /** \brief \ru Поиск точек излома оффсетной кривой.
              \en Search of break points of the offset curve. \~
  \details \ru Для нахождения точек точек излома используется характеристическая функция Ratio(),
            представляющая собой разность аналитически и численно посчитанной производной деленную 
            на модуль аналитической производной и величину шага, использованного для численного расчета производной. 
            Увеличение этой функции на порядок по сравнению с ее значением в гладкой области означает точку излома. \n
            \en To find the break points using the characteristic function Ratio(),
            which represents a difference between the analytical and numerical calculated derivative divided 
            by module of analytical derivative and step used for numerical calculation of the derivative. 
            Increase of this function on the order in comparison with its value in smooth region is a break point. \n \~
  \param[out] breakParams - \ru Массив параметров точек излома
                            \en Parameter array of break points \~
  */
  void        FindBreakParams( SArray<double> & breakParams ) const;
  int         ExtendedParam( double &t ) const; // \ru Проверка, лежит ли параметр в пределах \en Check if parameter is in range 

private:
          // \ru Вычисление множителя смещения и его производных. \en The offset multiplier and it derivatives.
  double      Factor0  ( double t ) const;
  double      FactorT  ( double t ) const;
  double      FactorTT ( double t ) const;
  double      FactorTTT( double t ) const;

  void        operator = ( const MbOffsetCurve3D & ); // \ru Не реализовано. \en Not implemented. 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbOffsetCurve3D )
};

IMPL_PERSISTENT_OPS( MbOffsetCurve3D )


#endif // __CUR_OFFSET_CURVE3D_H
