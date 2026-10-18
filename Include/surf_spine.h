////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Криволинейная направляющая для кинематической поверхности (поверхности заметания).
         \en Curvilinear spine for sweep surface. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __SURF_SPINE_H
#define __SURF_SPINE_H


#include <mb_matrix3d.h>
#include <curve3d.h>
#include <reference_item.h>
#include <tool_multithreading.h>


const VERSION SPINE_ALG_VERSION1 = 0x0A000000L; // \ru Добавлена optionalCurve и новый алгоритм вычисления direction \en Added curve "optionalCurve" and the new algorithm to calculate "direction" 
const VERSION SPINE_ALG_VERSION2 = 0x0D000023L; // \ru Добавлено создание optionalCurve в общем случае \en Added creation of "optionalCurve" in general case 


class   MbSpine;
class   MbWireFrame;

namespace c3d // namespace C3D
{
typedef SPtr<MbSpine>       SpineSPtr;
typedef SPtr<const MbSpine> ConstSpineSPtr;
}


//------------------------------------------------------------------------------
/** \brief \ru Криволинейная направляющая для кинематической поверхности.
           \en Curvilinear spine for sweep surface. \~
  \details \ru Криволинейная направляющая для кинематической поверхности (поверхности заметания) служит для расчёта в каждой точке направляющей кривой локальной системы координат. \n
    Локальная ось 0 ориентирована по касательной кривой "curve". \n
    Локальная ось 1 ориентирована в сторону вектора "direction" или в сторону кривой "optionalCurve". \n
    Локальная ось 2 дополняет локальную систему до правой системы координат.
           \en Curvilinear spine for sweep surface serves for calculation of a spine curve in the local coordinate system at each point. \n
    The local axis 0 is oriented in direction of the curve "curve" tangent. \n
    The local axis 1 is oriented in direction of the vector "direction" or of the curve "optionalCurve". \n
    The local axis 2 complements the local system to the right coordinate system. \~
  \ingroup Surface_Modeling
*/ // ---
class MATH_CLASS MbSpine : public MbRefItem, public TapeBase {
public:
  /// \ru Способы движения локальной системы координат вдоль направляющей кривой "curve". \en Methods of movement of the local coordinate system along the guide curve "curve".
  enum LocalAxes {
    la_planeParallel = 0, ///< \ru Плоскопараллельный, сохраняющий исходную ориентацию осей. \en Plane-parallel, preserving the original orientation of the axes.
    la_calcDirection = 1, ///< \ru Вектор "direction" рассчитан объектом. Ось 0 ориентирована по касательной кривой "curve", ось 1 - в сторону вектора "direction". \en Vector "direction" was calculated by object. Axis 0 is oriented along the tangent of the curve "curve", the axis 1 in the direction of the vector "direction".
    la_userDirection = 2, ///< \ru Вектор "direction" задан конструктору. Ось 0 ориентирована по касательной кривой "curve", ось 1 - в сторону вектора "direction". \en Vector "direction" was sent to conctructor.  Axis 0 is oriented along the tangent of the curve "curve", the axis 1 in the direction of the vector "direction".
    la_surfaceNormal = 3, ///< \ru Ось 0 ориентирована по касательной кривой на поверхности "curve", ось 1 - по нормали поверхности кривой "curve". \en The 0 axis is oriented along the tangent curve on the curve surface, the 1 axis is oriented along the normal of the curve surface.
    la_optionalCurve = 4, ///< \ru Ось 0 ориентирована по касательной кривой "curve", ось 1 - в сторону кривой "optionalCurve". \en Axis 0 is oriented along the tangent of the curve "curve", the axis 1 in the direction of the curve "optionalCurve".
  };

private:
  SPtr<MbCurve3D> curve;         ///< \ru Направляющая кривая - всегда не nullptr. \en Spine curve - it is always not nullptr.
  MbVector3D      direction;     ///< \ru Вектор ориентации матрицы преобразования. \en Vector of transformation matrix orientation. 
  SPtr<MbCurve3D> optionalCurve; ///< \ru Кривая вектора ориентации матрицы преобразования (может быть nullptr для простой траектории). \en A curve of the transformation matrix orientation (it may be nullptr for a simple trajectory).
  SPtr<MbSurface> spineSurface;  ///< \ru Поверхность направляющей кривой, если "curve" - кривая на поверхности, или nullptr. \en The surface of the "curve", if it is curve on surface, or nullptr.
  SPtr<MbCurve>   featureCurve;  ///< \ru Двумерная кривая, если "curve" - кривая на поверхности, или nullptr. \en Two-dimensional curve of the "curve", if it is curve on surface, or nullptr.
  LocalAxes       localAxes;     ///< \ru Способы ориентации локальной системы координат вдоль направляющей кривой "curve". \en Methods of orientation of the local coordinate system along the guide curve "curve".
  double          crossSize;     ///< \ru Поперечный масштаб при построении optionalCurve. \en Transverse scale in construction of "optionalCurve". 
  double          ortParam;      ///< \ru Параметр кривой, для которой рассчитаны ort0, ort1, ort2. \en Parameter of a curve with evaluated ort0, ort1 and ort2. 
  MbVector3D      ort0;          ///< \ru Вектор базиса в точке ortParam направляющей. \en A basis vector in the point ortParam of the spine. 
  MbVector3D      ort1;          ///< \ru Вектор базиса в точке ortParam направляющей. \en A basis vector in the point ortParam of the spine. 
  MbVector3D      ort2;          ///< \ru Вектор базиса в точке ortParam направляющей. \en A basis vector in the point ortParam of the spine. 
  VERSION         version;       ///< \ru Версия расчета вектора direction. \en Version of vector "direction" calculation. 
  bool            quadrExt;      ///< \ru Флаг параболического продления. \en Flag of parabolic extension. 

  //------------------------------------------------------------------------------
  /** \brief \ru Вспомогательные данные. 
             \en Auxiliary data. \~
    \details \ru Вспомогательные данные служат для ускорения работы объекта.
             \en Auxiliary data are used for fast calculations. \n \~
  */
  // ---
  class MbSpineAuxiliaryData : public AuxiliaryData {
  public:
    double      t0;   ///< \ru Исходный параметр. \en Initial parameter.
    double      t;    ///< \ru Модифицированный параметр. \en Modified parameter.
    bool        ext;  ///< \ru Флаг расчета на продолжении. \en Extension flag.
    MbMatrix3D  matrix0; ///< \ru Матрица преобразования из местной системы координат \en Transformation matrix from the local coordinate system. 
    MbMatrix3D  matrix1; ///< \ru Первая производная матрицы преобразования \en First derivative of transformation matrix. 
    MbMatrix3D  matrix2; ///< \ru Вторая производная матрицы преобразования \en Second derivative of transformation matrix.

  public:
    MbSpineAuxiliaryData();
    MbSpineAuxiliaryData( const MbSpineAuxiliaryData & );
    virtual ~MbSpineAuxiliaryData();

    bool IsChanged( double pmin, double pmax, double p, bool pext ) const
    {
      bool changed = false;
      if ( p != t0 )
        changed = true;
      else if ( ext != pext ) {
        changed = true;
        if ( pmin <= p && p <= pmax )
          changed = false;
      }
      return changed;
    }
    void Init();
    void Init( const MbSpineAuxiliaryData & );
    void Move( const MbVector3D & );
  };
  
  mutable CacheManager<MbSpineAuxiliaryData> cache;

protected:

  /** \brief \ru Конструктор по направляющей кривой.
             \en Constructor by spine curve. \~  
    \details \ru Конструктор по направляющей кривой.\n
             \en Constructor by spine curve.\n \~
    \param[in] cur  - \ru Направляющая кривая.
                      \en A spine curve. \~ 
    \param[in] par  - \ru Признак параллельного переноса.
                      \en Attribute of parallel translation. \~
    \param[in] same - \ru Признак использования оригинала направляющей кривой, а не ее копии.
                      \en Attribute of using the original of spine curve instead of its copy. \~
    \param[in] vers - \ru Версия операции.
                     \en Version of operation. \~
  */
  MbSpine( const MbCurve3D & cur, bool par, bool same, VERSION vers = Math::DefaultMathVersion() );

  /** \brief \ru Конструктор по направляющей кривой и вектору ориентации матрицы преобразования.
             \en Constructor by spine curve and a vector of transformation matrix orientation. \~  
    \details \ru Конструктор по направляющей кривой и вектору ориентации матрицы преобразования.\n
             \en Constructor by spine curve and a vector of transformation matrix orientation.\n \~
    \param[in] cur  - \ru Направляющая кривая.
                      \en A spine curve. \~ 
    \param[in] same - \ru Признак использования оригинала направляющей кривой, а не ее копии.
                      \en Attribute of using the original of spine curve instead of its copy. \~
    \param[in] dir  - \ru Вектор ориентации матрицы преобразования.
                      \en Vector of transformation matrix orientation. \~
    \param[in] par  - \ru Признак параллельного переноса.
                      \en Attribute of parallel translation. \~
    \param[in] vers - \ru Версия вычисления осей локальной системы координат.
                      \en Version of calculation the local coordinate system. \~
  */
  MbSpine( const MbCurve3D & cur, bool same, const MbVector3D & dir, bool par, VERSION vers = Math::DefaultMathVersion() );
  
  /** \brief \ru Конструктор по направляющей кривой и кривой вектора ориентации матрицы преобразования.
             \en Constructor by spine curve and a curve of the vector of transformation matrix orientation. \~  
    \details \ru Конструктор по направляющей кривой и кривой вектора ориентации матрицы преобразования.\n
             \en Constructor by spine curve and a curve of the vector of transformation matrix orientation.\n \~
    \param[in] cur   - \ru Направляющая кривая
                       \en A spine curve \~ 
    \param[in] same  - \ru Признак использования оригинала направляющей кривой, а не ее копии
                       \en Attribute of using the original of spine curve instead of its copy \~
    \param[in] opt   - \ru Кривая вектора ориентации матрицы преобразования
                       \en A curve of the vector of transformation matrix orientation \~
    \param[in] sameO - \ru Признак использования оригинала кривой вектора ориентации матрицы преобразования, а не ее копии
                       \en Attribute of using the original of a curve of the vector of transformation matrix orientation instead of its copy \~
    \param[in] par   - \ru Признак параллельного переноса.
                       \en Attribute of parallel translation. \~
    \param[in] vers  - \ru Версия вычисления осей локальной системы координат.
                       \en Version of calculation the local coordinate system. \~
  */
  MbSpine( const MbCurve3D & cur, bool same, const MbCurve3D & opt, bool sameO, bool par, VERSION vers = Math::DefaultMathVersion() );
  // \ru Конструктор по направляющей кривой и поверхности, на которой лежит направляющая кривая. \en Constructor by spine curve and the surface on which the spine curve lies. \~  
  MbSpine( const MbCurve3D & spineCurve, bool sameSpine, const MbSurface & surface, bool sameSurface, const MbCurve & pCurve, bool sameCurve, VERSION ver );

protected:
  MbSpine( const MbSpine &, MbRegDuplicate * );
public:
  virtual ~MbSpine();

public:
  virtual void Accept( Visitor & visitor )
  {
    VisitorImpl<MbSpine> * impl = dynamic_cast<VisitorImpl<MbSpine> *>(&visitor);
    if( impl )
      impl->Visit( *this );
  }

  /** \brief \ru Конструктор по направляющей кривой.
             \en Constructor by spine curve. \~  
    \details \ru Конструктор по направляющей кривой.\n
             \en Constructor by spine curve.\n \~
    \param[in] c - \ru Направляющая кривая
                   \en A spine curve \~ 
    \param[in] parallel - \ru Признак параллельного переноса
                          \en Attribute of parallel translation. \~
    \param[in] same - \ru Признак использования оригинала направляющей кривой, а не ее копии
                      \en Attribute of using the original of spine curve instead of its copy \~
    \param[in] vers - \ru Версия вычисления осей локальной системы координат.
                      \en Version of calculation the local coordinate system. \~
  */
  static  MbSpine &  Create( const MbCurve3D & c, bool parallel, bool same, 
                             VERSION vers = Math::DefaultMathVersion() );
  /** \brief \ru Конструктор по направляющей кривой и вектору ориентации матрицы преобразования.
             \en Constructor by spine curve and a vector of transformation matrix orientation. \~  
    \details \ru Конструктор по направляющей кривой и вектору ориентации матрицы преобразования.\n
             \en Constructor by spine curve and a vector of transformation matrix orientation.\n \~
    \param[in] c - \ru Направляющая кривая
                   \en A spine curve \~ 
    \param[in] same - \ru Признак использования оригинала направляющей кривой, а не ее копии
                      \en Attribute of using the original of spine curve instead of its copy \~
    \param[in] direction - \ru Вектор ориентации матрицы преобразования.
                           \en Vector of transformation matrix orientation. \~
    \param[in] parallel - \ru Признак параллельного переноса
                          \en Attribute of parallel translation. \~
    \param[in] vers - \ru Версия вычисления осей локальной системы координат.
                      \en Version of calculation the local coordinate system. \~
    \return \ru Возвращает указатель на созданный объект или нулевой указатель в случае неудачи.
            \en Returns pointer to the created object or null pointer in case of failure. \~
  */
  static  MbSpine *  Create( const MbCurve3D & c, bool same, const MbVector3D & dir, bool parallel, 
                             VERSION vers = Math::DefaultMathVersion() );
  /** \brief \ru Конструктор по направляющей кривой и кривой вектора ориентации матрицы преобразования.
             \en Constructor by spine curve and a curve of the vector of transformation matrix orientation. \~  
    \details \ru Конструктор по направляющей кривой и кривой вектора ориентации матрицы преобразования.\n
             \en Constructor by spine curve and a curve of the vector of transformation matrix orientation.\n \~  
    \param[in] sp -    \ru Направляющая кривая
                       \en A spine curve \~ 
    \param[in] sameS - \ru Признак использования оригинала направляющей кривой, а не ее копии
                       \en Attribute of using the original of spine curve instead of its copy \~
    \param[in] dc -    \ru Кривая вектора ориентации матрицы преобразования
                       \en A curve of the vector of transformation matrix orientation \~
    \param[in] sameD - \ru Признак использования оригинала кривой вектора ориентации матрицы преобразования, а не ее копии
                       \en Attribute of using the original of a curve of the vector of transformation matrix orientation instead of its copy \~
    \param[in] parallel - \ru Признак параллельного переноса
                          \en Attribute of parallel translation. \~
    \param[in] vers -  \ru Версия вычисления осей локальной системы координат.
                       \en Version of calculation the local coordinate system. \~
    \return \ru Возвращает указатель на созданный объект или нулевой указатель в случае неудачи.
            \en Returns pointer to the created object or null pointer in case of failure. \~
  */
  static  MbSpine *  Create( const MbCurve3D & sp, bool sameS, const MbCurve3D & dc, bool sameD, bool parallel, 
                             VERSION vers = Math::DefaultMathVersion() );
  /** \brief \ru Конструктор по направляющей кривой на кривой, связанной с поверхностью.
             \en Constructor by spine curve on a curve associated with a surface. \~  
    \details \ru Конструктор по направляющей кривой и поверхности, на которой лежит направляющая кривая.\n
             \en Constructor by spine curve and the surface on which the spine curve lies.\n \~  
    \param[in] sp -    \ru Направляющая кривая
                       \en A spine curve \~ 
    \param[in] sameS - \ru Признак использования оригинала направляющей кривой, а не ее копии
                       \en Attribute of using the original of spine curve instead of its copy \~
    \param[in] surf -  \ru Поверхность направляющей кривой. 
                       \en The surface of the sp, it should be curve on surface. \~
    \param[in] pCrv -  \ru Двумерная кривая на поверхности направляющей кривой.
                       \en The two-dimensional curve on the surface of the guide curve. \~
    \param[in] vers -  \ru Версия вычисления осей локальной системы координат.
                       \en Version of calculation the local coordinate system. \~
    \return \ru Возвращает указатель на созданный объект или нулевой указатель в случае неудачи.
            \en Returns pointer to the created object or null pointer in case of failure. \~
  */
  static  MbSpine *  Create( const MbCurve3D & sp, bool sameS, const MbSurface & surf, const MbCurve * pCrv,
                             VERSION vers );

  /** \brief \ru Конструктор каркасу.
             \en Constructor by wireFrame. \~  
    \details \ru Конструктор каркасу.\n
             \en Constructor by wireFrame. \n \~
    \param[in] wireFrame - \ru Каркас с направляющей кривой.
                           \en Wire frame with a spine curve. \~ 
    \param[in] same -      \ru Признак использования оригинала направляющей кривой, а не ее копии
                           \en Attribute of using the original of spine curve instead of its copy \~
    \param[in] direction - \ru Вектор ориентации матрицы преобразования.
                           \en Vector of transformation matrix orientation. \~
    \param[in] parallel -  \ru Признак параллельного переноса
                           \en Attribute of parallel translation. \~
    \param[in] version -   \ru Версия вычисления осей локальной системы координат.
                           \en Version of calculation the local coordinate system. \~
    \return \ru Возвращает указатель на созданный объект или нулевой указатель в случае неудачи.
            \en Returns pointer to the created object or null pointer in case of failure. \~
  */
  static  MbSpine *  Create( const MbWireFrame & wireFrame, bool same, const MbVector3D * direction, bool parallel, VERSION version );
  /** \brief \ru Конструктор каркасу.
             \en Constructor by wireFrame. \~  
    \details \ru Конструктор каркасу.\n
             \en Constructor by wireFrame. \n \~
    \param[in] wireFrame - \ru Направляющая кривая
                           \en A spine curve \~ 
    \param[in] same -      \ru Признак использования оригинала направляющей кривой, а не ее копии
                           \en Attribute of using the original of spine curve instead of its copy \~
    \param[in] surface -   \ru Поверхность направляющей кривой. 
                           \en The surface of the sp, it should be curve on surface. \~
    \param[in] pCurve -    \ru Двумерная кривая на поверхности направляющей кривой.
                           \en The two-dimensional curve on the surface of the guide curve. \~
    \param[in] version -   \ru Версия вычисления осей локальной системы координат.
                           \en Version of calculation the local coordinate system. \~
    \return \ru Возвращает указатель на созданный объект или нулевой указатель в случае неудачи.
            \en Returns pointer to the created object or null pointer in case of failure. \~
  */
  static  MbSpine *  Create( const MbWireFrame & wireFrame, bool same, const MbSurface & surface, const MbCurve * pCurve, VERSION version );

  /** \} */
  /** \ru \name Базовые функции
      \en \name Base functions
      \{ */
  /// \ru Тип элемента. \en A type of element. 
  MbeSpaceType       IsA() const { return curve->IsA(); } 
  /// \ru Сделать копию элемента. \en Create a copy of the element. 
  MbSpine &          Duplicate( MbRegDuplicate * = nullptr ) const;
  /// \ru Сделать равным. \en Make equal. 
  bool               SetEqual( const MbSpine & ); 
  /// \ru Являются ли объекты подобными. \en Determine whether the objects are similar. 
  bool               IsSimilar( const MbSpine & ) const; 
  /// \ru Равны ли объекты. \en Whether the objects are equal. 
  bool               IsSame  ( const MbSpine & other, double accuracy = LENGTH_EPSILON ) const; 
  /// \ru Преобразовать элемент согласно матрице. \en Transform element according to the matrix. 
  void               Transform( const MbMatrix3D &, MbRegTransform * = nullptr );
  /// \ru Сдвиг. \en Translation. 
  void               Move     ( const MbVector3D &, MbRegTransform * = nullptr );
  /// \ru Повернуть вокруг оси. \en Rotate around an axis. 
  void               Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr );
  /// \ru Изменить направление. \en Change the direction. 
  void               Inverse( MbRegTransform * iReg = nullptr );
  /// \ru Сбросить временные данные объекта. \en Reset temporary data of an object. 
  void               Reset(); 
  /** \} */
  /** \ru \name Общие описания области определения направляющей кривой
      \en \name General descriptions of a spine curve domain
      \{ */
  /// \ru Вернуть максимальное значение параметра. \en Get the maximum value of parameter. 
  double             GetTMax()  const  { return curve->GetTMax(); }  
  /// \ru Вернуть минимальное значение параметра. \en Get the minimum value of parameter. 
  double             GetTMin()  const  { return curve->GetTMin(); }  
  /// \ru Проверка замкнутости кривой. \en Check for curve closeness 
  bool               IsClosed() const  { return curve->IsClosed(); } 
  /** \} */
  /** \ru \name Функции для работы в области определения направляющей кривой
            Функции PointOn, FirstDer... корректируют параметры
            при выходе их за пределы области определения параметров направляющей кривой.
      \en \name Functions for working in the spine curve's domain.
            Functions PointOn, FirstDer... correct parameters
            when getting out of the spine curve domain bounds.
      \{ */
  /// \ru Точка на кривой. \en Point on the curve. 
  void               PointOn  ( double & t, MbCartPoint3D & p ) const;
  /// \ru Первая производная. \en The first derivative. 
  void               FirstDer ( double & t, MbVector3D & p ) const;
  /// \ru Вторая производная. \en The second derivative. 
  void               SecondDer( double & t, MbVector3D & p ) const;
  /// \ru Третья производная. \en Third derivative. 
  void               ThirdDer ( double & t, MbVector3D & p ) const;
  /** \} */
  /** \ru \name Функции для работы внутри и вне области определения направляющей кривой
            функции _PointOn, _FirstDer... корректируют параметры
            при выходе их за пределы области определения параметров направляющей кривой.
      \en \name Functions for working inside and outside the spine curve's domain
            functions _PointOn, _FirstDer... correct parameters
            when getting out of the spine curve domain bounds.
      \{ */
  /// \ru Точка на кривой. \en Point on the curve. 
  void              _PointOn  ( double  t, MbCartPoint3D & p ) const;
  /// \ru Первая производная. \en The first derivative. 
  void              _FirstDer ( double  t, MbVector3D & p ) const;
  /// \ru Вторая производная. \en The second derivative. 
  void              _SecondDer( double  t, MbVector3D & p ) const;
  /// \ru Третья производная. \en Third derivative. 
  void              _ThirdDer ( double  t, MbVector3D & p ) const;
  /// \ru Вычислить значения точки и производных для заданного параметра. \en Calculate point and derivatives of object for given parameter. \~
  virtual void       Explore( double & t, bool ext,
                              MbCartPoint3D & pnt, MbVector3D & fir, MbVector3D * sec, MbVector3D * thir ) const; 
  /** \} */
  /** \ru \name Функции движения по направляющей кривой
      \en \name Function of moving by spine curve
      \{ */
  /// \ru Вычисление шага аппроксимации по величине прогиба. \en Calculation of the approximation step by the value of sag. 
  double             Step( double t, double sag ) const; 
  /// \ru Вычисление шага аппроксимации по углу отклонения нормали. \en Calculation of the approximation step by the deviation angle of the normal vector. 
  double             DeviationStep( double t, double angle ) const; 
  /** \} */
  /** \ru \name Функции изменения и доступа к данным
      \en \name Functions for changing data and access to data
      \{ */

  /** \brief \ru Определение матрицы переноса для образующей.
             \en Determination of a transfer matrix for generatrix. \~  
    \details \ru Определение матрицы переноса для образующей.\n
             \en Determination of a transfer matrix for generatrix.\n \~ 
    \param[in] v - \ru Параметр на направляющей кривой
                   \en Parameter on spine curve \~
    \param[out] matrix - \ru Результат - матрица
                         \en The result is a matrix \~ 
  */
  void               TransformMatrix( double v, MbMatrix3D & matrix ) const;              
  
  /** \brief \ru Определение вектора переноса для образующей.
             \en Determination of a transfer vector for generatrix. \~  
    \details \ru Определение вектора переноса для образующей.\n
             \en Determination of a transfer vector for generatrix.\n \~ 
    \param[in] v - \ru Параметр на направляющей кривой
                   \en Parameter on spine curve \~
    \param[out] vect - \ru Результат - вектор
                       \en The result is a vector \~ 
  */
  void               MoveVector( double & v, MbVector3D & vect ) const; // \ru Определение вектора переноса для образующей \en Determination of a transfer vector for generatrix 
  
  /** \brief \ru Первой вектор базиса в рассчитанной направляющей.
             \en The first vector of basis in the calculated spine. \~
    \details \ru Первой вектор базиса в рассчитанной направляющей.\n
             \en The first vector of basis in the calculated spine.\n \~
    \return \ru Вектор
            \en A vector \~
  */
  const MbVector3D & GetOrt0() const { return ort0; }

  /** \brief \ru Вычисление матриц преобразования.
             \en Calculation of transformation matrices. \~
    \details \ru Вычисление матриц преобразования радиуса-вектора (matrix0) и его первой и второй производных (matrix1, matrix2) для параметра на направляющей.
      При ext==true функция переносит параметр v в область определения направляющей кривой.
             \en Calculation of transformation matrices for radius vector (matrix0) and for first and second derivatives (matrix1, matrix2) for the parameter on a spine.
      The function moves the parameter v inside the spine curve domain when ext==true. \~
    \param[in, out] v - \ru Параметр на направляющей кривой
                        \en Parameter on spine curve \~
  */
  void               CalculateMatrix ( double & v, bool ext, MbMatrix3D & matrix0, MbMatrix3D & matrix1, MbMatrix3D & matrix2 ) const; 
  // \ru Вычисление матриц преобразования радиуса-вектора и его первой производной. \en Calculation of transformation matrices for radius vector and for first derivative. \~
  void               CalculateMatrix ( double & v, bool ext, MbMatrix3D & matrix0, MbMatrix3D & matrix1 ) const; 
  // \ru Вычисление матрицы преобразования радиуса-вектора. \en Calculation of transformation matrix for radius vector. \~
  void               CalculateMatrix0( double & v, bool ext, MbMatrix3D & matrix0 ) const; 
  // \ru Вычисление матрицы преобразования первой производной радиуса-вектора. \en Calculation of transformation matrix for first derivative of radius vector. \~
  void               CalculateMatrix1( double & v, bool ext, MbMatrix3D & matrix1 ) const; 
  // \ru Вычисление матрицы преобразования второй производной радиуса-вектора. \en Calculation of transformation matrix for second derivative of radius vector. \~
  void               CalculateMatrix2( double & v, bool ext, MbMatrix3D & matrix2 ) const; 
  
  /// \ru Направляющая кривая. \en The spine curve. 
  const MbCurve3D  & GetCurve() const { return *curve; } 
  /// \ru Дать направляющую кривую для изменения. \en Get spine curve for editing. 
        MbCurve3D  & SetCurve()       { cache.Reset( true ); return *curve; } 

  /// \ru Ориентирующая кривая. \en Direction curve. 
  const MbCurve3D  * GetDirectionCurve() const { return optionalCurve; } 

  /// \ru Версия расчета вектора ориентации матрицы преобразования. \en A version of calculation of the vector of transformation matrix orientation. 
  VERSION            GetVersion() const { return version; } 

  /// \ru Изменить направляющую кривую. \en Change the spine curve. 
  void               ChangeCurve( const MbCurve3D & );
  /// \ru Изменить ориентирующую кривую. \en Change the direction curve. 
  void               ChangeOptionalCurve( const MbCurve3D & ); 

  /// \ru Вектор ориентации матрицы преобразования. \en Vector of transformation matrix orientation. 
  const MbVector3D & GetDirection() const { return direction; }
  /// \ru Изменить вектор ориентации матрицы преобразования. \en Change the vector of transformation matrix orientation. 
  bool               SetDirection( const MbVector3D & d, bool checkBySpineCurve = false );

  /// \ru Дать способ ориентации локальной системы координат. \en Get the method of orientation of the local coordinate system. \~
  LocalAxes          GetLocalAxisMethod() const { return localAxes; }
  /// \ru Дать способ ориентации локальной системы координат. \en Get the method of orientation of the local coordinate system. \~
  void               SetLocalAxisMethod( LocalAxes la ) { localAxes = la; }
  /// \ru Признак параллельного переноса. \en Attribute of plane-parallel translation. 
  bool               IsParallel() const { return localAxes == la_planeParallel; } 
  /// \ru Признак пользовательского направления. \en Attribute of user direction. 
  bool               IsUserDirection() const { return ( localAxes == la_userDirection ); }
  /// \ru Признак направления по нормали к поверхности. \en Attribute of direction as surface normal. 
  bool               BySurfaceNormal() const { return ( localAxes == la_surfaceNormal ); }
  /// \ru Признак направления по вспомогательной кривой. \en Attribute of direction by optional curve. 
  bool               ByOptionalCurve() const { return ( localAxes == la_optionalCurve ); }
  /// \ru Параметр кривой, для которой рассчитан базис направляющей. \en Parameter of a curve with calculated basis of spine.  
  double             GetOrtParam() const { return ortParam; } 
  /// \ru Установить параметр кривой, для которой рассчитан базис направляющей. \en Set the parameter of a curve with calculated basis of spine. 
  void               SetOrtParam( double t );
  /// \ru Вычисление ортов для матриц преобразования. \ en The basis vectors calculation for matrix. 
  void               BaseVectors0( double & v, bool ext, MbCartPoint3D & origin,
                                   MbVector3D & vector0, MbVector3D & vector1, MbVector3D & vector2 ) const;
  /// \ru Вычисление ортов и их первых производных. \ en Calculation of the basis vectors and their first derivatives.
  void               BaseVectors1( double & v, bool ext, MbVector3D & origin1,
                                   MbVector3D & vector0, MbVector3D & vector1, MbVector3D & vector2,
                                   MbVector3D & derive0, MbVector3D & derive1, MbVector3D & derive2 ) const;
  /// \ru Вычисление ортов и их первых и вторых производных. \ en Calculation of the basis vectors and their first and second derivatives.
  void               BaseVectors2( double & v, bool ext, MbVector3D & origin1, MbVector3D & origin2,
                                   MbVector3D & vector0, MbVector3D & vector1, MbVector3D & vector2,
                                   MbVector3D & derive0, MbVector3D & derive1, MbVector3D & derive2,
                                   MbVector3D & second0, MbVector3D & second1, MbVector3D & second2 ) const;
  /** \brief \ru Построить плейсмент в заданной точке.
             \en Construct placement in the given point. \~
    \details \ru Построить плейсмент в заданной точке.\n
             \en Construct placement in the given point.\n \~
    \param[in] v - \ru Параметр на направляющей кривой
                   \en Parameter on spine curve \~
  */
  MbPlacement3D      GetPlacement( double v ) const; 
  /// \ru Инициализировать плейсмент для заданной точке. \en Initialize placement for the given point. \~
  void               GetPlacement( double v, MbPlacement3D & pl ) const;

  /// \ru Количество сегментов направляющей кривой. \en The number of segments of the spine curve. 
  size_t             GetSegmentsCount() const; 

  /** \brief \ru Функция регистрации по количеству ссылок для предотвращения многократной записи.
             \en Function of registration by the number of references to avoid multiple writing. \~
    \details \ru Функция регистрации по количеству ссылок для предотвращения многократной записи.\n
             \en Function of registration by the number of references to avoid multiple writing.\n \~    
  */   
  void               PrepareWrite() const { SetRegistrable( (GetUseCount() > 1) ? registrable : noRegistrable ); }

  const MbSurface *  GetSpineSurface() const { return spineSurface; } ///< \ru Поверхность направляющей кривой, если "curve" - кривая на поверхности. \en The surface of the "curve", if it is curve on surface. 
  const MbCurve *    GetFeatureCurve() const { return featureCurve; } ///< \ru Двумерная кривая, если "curve" - кривая на поверхности. \en Two-dimensional curve of the "curve", if it is curve on surface. 

  /** \} */

private:
  void               CheckParam( double & v ) const ; // \ru Функция переносит параметр v в область определения направляющей кривой. \en The function moves the parameter v inside the spine curve domain.
  void               InitDirection(); // \ru Вычисление direction; разбор частных случаев типа плоских кривых, дуг окружности и т.п. \en Calculation of "direction"; analysis of such special cases as planar curves, circle arcs etc. 
  bool               CalculateSpineSurface(); // \ru Для кривой на поверхности выделить поверхность и двумерную кривую. \en Get "spineSurface" and "featureCurve" if "curve" is a curve on surface.
  void               CalculateDirection( double v, MbVector3D & direct ) const; // \ru Вектор ориентации матрицы преобразования. \en Vector of transformation matrix orientation. 
  bool               InitOptionalCurve( bool exactOnly ); // \ru Вычисление optionalCurve; использовать, если нетривиальный случай. \en Calculation of "optionalCurve"; use if the case is nontrivial. 
  void               Multiplication( const MbVector3D & vector0, const MbVector3D & vector1, const MbVector3D & vector2,
                                     MbMatrix3D & matrix ) const; // \ru Определение матрицы преобразования. \en Determination of transformation matrix. 

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbSpine )
OBVIOUS_PRIVATE_COPY( MbSpine )
}; // MbSpine


IMPL_PERSISTENT_OPS( MbSpine )


//------------------------------------------------------------------------------
/** \brief \ru Направляющая кривая и её окружение.
           \en A spine curve and its neighborhood. \~
  \details \ru Направляющая кривая и её окружение.\n
           \en A spine curve and its neighborhood.\n \~
  \ingroup Surface_Modeling
*/ // ---
struct SpineData {
  MbSpine &     spine; ///< \ru Направляющая кривая. \en A spine curve. 
  MbCartPoint3D point; ///< \ru Положение характерной точки сечения в начале направляющей. \en Position of characteristic point of the section at the beginning of the spine. 
  MbVector3D    vect;  ///< \ru Вектор оси поворота сечения. \en Vector of the section rotation axis. 
  MbVector3D    norm;  ///< \ru Вектор нормали сечения в начале направляющей. \en Normal vector of section at the beginning of the spine. 
  double        angle; ///< \ru Угол поворота сечения в начале направляющей. \en Rotation angle of section at the beginning of the spine. 
  //double        range; ///< \ru Эквидистантное смещение точек образующей кривой в конце траектории. \en The offset range of generating curve on the end of spine curve. \~

  SpineData( MbSpine & sp, const MbCartPoint3D & org, const MbVector3D & v, const MbVector3D & n, double ang )//, double ran )
    : spine( sp  )
    , point( org )
    , vect ( v   )
    , norm ( n   )
    , angle( ang )
    //, range( ran )
  {
  }
  virtual ~SpineData() {}

OBVIOUS_PRIVATE_COPY( SpineData )
};


//------------------------------------------------------------------------------
/** \brief \ru Создание массива направляющих по контуру направляющих.
           \en Creation of array of spines by the contour of spines. \~
  \details \ru Создание массива направляющих по контуру направляющих с согласованными.
    векторами ориентации матрицы преобразования.\n
    Для внутреннего использования.
           \en Creation of array of spines by the contour of spines with the consistent
    vectors of transformation matrix orientation.\n
    For internal use only. \~
  \param[in] sp - \ru Контур направляющих
                  \en Contour of spines \~
  \param[out] items - \ru Массив направляющих.
                      \en Array of spines. \~
*/ // ---
MATH_FUNC (void) MakeSpines( const MbSpine & sp, SArray<SpineData> & items );
  

//------------------------------------------------------------------------------
/** \brief \ru Удалить неиспользованные направляющие.
           \en Delete unused spines. \~  
  \details \ru Удалить неиспользованные направляющие.\n 
    Для внутреннего использования.
           \en Delete unused spines.\n 
    For internal use only. \~     
  \param[in,out] spineDataItems - \ru Массив направляющих.
                                  \en An array of spines. \~   
*/ // ---
template <class SpineDataVector>
void DeleteNonUsedSpines( SpineDataVector & spineDataItems )
{
  if ( !spineDataItems.empty() ) {
    for ( size_t i = spineDataItems.size(); i--; ) {
      MbSpine * spine = &spineDataItems[i].spine;
      ::DeleteItem( spine );
    }
    spineDataItems.clear();
  }
}


//------------------------------------------------------------------------------
/** \brief \ru Поиск вектора ориентации матрицы преобразования.
           \en Search of vector for transformation matrix orientation. \~
  \details \ru Поиск вектора ориентации матрицы преобразования. \n 
    Для внутреннего использования.
           \en Search of vector for transformation matrix orientation. \n 
    For internal use only. \~     
  \param[in]  curve - \ru Кривая.
                      \en A curve. \~
  \param[out] direction - \ru Вектор, который не совпадает с касательной к кривой.
                          \en A vector which is not coincident with the curve tangent. \~
  \param[in]  version - \ru Версия.
                        \en Version. \~
  \return \ru ts_positive или ts_neutral, если  максимально подходящий вектор направления direction найден.
          \en It equals ts_positive or ts_neutral if the most suitable direction vector "direction" was found. \~ 
*/ // ---
MATH_FUNC (ThreeStates) InitSpineDirection( const MbCurve3D & curve, MbVector3D & direction, VERSION version );


//------------------------------------------------------------------------------
/** \brief \ru Проверить вектор ориентации матрицы преобразования.
           \en Check a vector for transformation matrix orientation. \~
  \details \ru Проверить вектор ориентации матрицы преобразования. \n 
    Для внутреннего использования.
           \en Check a vector for transformation matrix orientation. \n 
    For internal use only. \~     
  \param[in]  curve - \ru Кривая.
                      \en A curve. \~
  \param[in] direction - \ru Вектор, который не должен совпадать с касательной к кривой.
                         \en A vector which must not be coincident with the curve tangent. \~
  \param[in]  version - \ru Версия.
                        \en Version. \~
  \return \ru true, если вектор не совпадает с касательной к кривой.
          \en It equals true if the vector is not coincident with the curve tangent. \~ 
*/ // ---
MATH_FUNC (bool) CheckSpineDirection( const MbCurve3D & curve, const MbVector3D & direction, VERSION version = Math::DefaultMathVersion() );


#endif //__SURF_SPINE_H
