//////////////////////////////////////////////////////////////////////////////////////////
/** \file 
    \brief  \ru Локальная система координат в двумерном пространстве.
            \en Local coordinate system in two dimensional space. \~
*/
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __MB_PLACEMENT_H
#define __MB_PLACEMENT_H

#include <mb_cart_point.h>
#include <mb_matrix.h>


//----------------------------------------------------------------------------------------
/** \brief \ru Типы локальных систем координат в двумерном пространстве.
           \en Types of local coordinate systems in two dimensional space. \~
  \details \ru Типы локальных систем координат в двумерном пространстве.
           \en Types of local coordinate systems in two dimensional space. \~
  \ingroup Mathematic_Base_2D
*/
// ---
enum MbeLocalSystemType {
  ls_CartesSystem,   ///< \ru Декартова система координат. \en Cartesian coordinate system. 
  ls_PolarSystem,    ///< \ru Полярная система координат. \en Polar coordinate system. 
};


//----------------------------------------------------------------------------------------
/** \brief \ru Локальная система координат в двумерном пространстве.
           \en Local coordinate system in two dimensional space. \~
  \details \ru Локальная система координат в двумерном пространстве. \n
    В большинстве случаев система координат (СК) является правой, а векторы системы ортонормированы. 
    С помощью преобразований система координат может стать левой и не ортонормированной. 
    Локальная система координат является декартовой, 
    Точка в декартовой системе координат определяется двумя координатами x, y.
           \en Local coordinate system in two dimensional space. \n
    Local coordinate system is described by the initial point and two non-parallel vectors.
    In most cases the system of coordinates is right, and vectors of system are orthonormalized. 
    A coordinate system can become left and not orthonormalized via transformations. 
    Local coordinate system is Cartesian, 
    Point in the Cartesian coordinate system is defined by two coordinates x, y. \~
  \ingroup Mathematic_Base_2D
*/
// ---
class MATH_CLASS MbPlacement {
private:
  MbCartPoint origin; ///< \ru Положение начала локальной системы координат. \en Origin of coordinate system. 
  MbVector    axisX;  ///< \ru Направление первой оси. \en Direction of first axis. 
  MbVector    axisY;  ///< \ru Направление второй оси. \en Direction of second axis. 
private:
  /**
    \brief \ru Состояние локальной системы координат.
           \en State of local coordinate system. \~
    \details \ru Состояние локальной системы координат определяется установкой битовых полей: \n 
      MB_TRANSLATION - начало координат не ноль \n 
      MB_ROTATION    - система координат не единичная \n 
      MB_LEFT        - признак левой системы координат \n 
      MB_ORTOGONAL   - ортогональная система координат, взводится только в случае аффинной системы координат \n 
      MB_AFFINE      - система координат произвольная аффинная \n 
      MB_UNSET       - битовые флаги не установлены \n 
      При изменении системы координат flag должен быть сброшен в неустановленное состояние MB_UNSET. \n 
      Если flag == MB_UNSET, то при использовании системы координат происходит определение её состояния.\n
             \en State of local coordinate system is defined by setting-up of bit fields: \n 
      MB_TRANSLATION - origin of coordinate system is not zero \n 
      MB_ROTATION    - coordinate system is not unit \n 
      MB_LEFT        - attribute of left coordinate system \n 
      MB_ORTOGONAL   - orthogonal coordinate system,  it is set-up only if coordinate system is affine \n 
      MB_AFFINE      - any affine coordinate system \n 
      MB_UNSET       - bit flags not set \n 
      'flag' has to be reset to unspecified MB_UNSET state while changing the coordinate system. \n 
      if 'flag' == MB_UNSET then the state of coordinate system is specified while it is being used.\n \~ 
  */
  mutable uint8 flag;

public :
  /// \ru Конструктор по умолчанию. \en Default constructor. 
  MbPlacement();
  /// \ru Конструктор по точке и нормализованному вектору направления. \en Constructor by point and normalized direction vector. 
  MbPlacement( const MbCartPoint &, const MbDirection &, bool l = false );
  /// \ru Конструктор по точке и двум векторам. \en Constructor by point and two vectors. 
  MbPlacement( const MbCartPoint &, const MbVector &, const MbVector & );
  /// \ru Конструктор по точке и знакам направления осей X и Y. \en Constructor by point and signs of X-axis and Y-axis directions. 
  MbPlacement( const MbCartPoint &, bool, bool );
  /// \ru Конструктор по точке и углу. \en Constructor by point and angle. 
  MbPlacement( const MbCartPoint &, double angle, bool l = false );
  /// \ru Конструктор по координатам начала ЛСК и углу. \en Constructor by angle and coordinates of local coordinate system origin. 
  MbPlacement( double x, double y, double angle );
  /// \ru Конструктор по двум точкам. \en Constructor by two points. 
  MbPlacement( const MbCartPoint & p1, const MbCartPoint & p2 );
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbPlacement( const MbPlacement & );
  /// \ru Конструктор по матрице. \en Constructor by matrix. 
  MbPlacement( const MbMatrix & matr );
  /// \ru Деструктор. \en The destructor. 
 ~MbPlacement();

public: // \ru Функции инициализации \en Initialization functions. 
 
  /// \ru Сделать совпадающей с мировой СК. \en Make coincident with global coordinate system. 
  void        Init();
  /// \ru Инициализировать по заданной ЛСК. \en Initialize by given local coordinate system. 
  void        Init( const MbPlacement & init );
  /// \ru Инициализировать по точке и вектору направления. \en Initialize by a point and direction vector. 
  void        Init( const MbCartPoint & initOrigin, const MbDirection & initDir, bool l = false );
  /// \ru Инициализировать по точке и углу. \en Initialize by a point and an angle. 
  void        Init( const MbCartPoint & initOrigin, double angle, bool l = false );
  /// \ru Инициализировать по точке и двум векторам. \en Initialize by a point and two vectors. 
  void        Init( const MbCartPoint &, const MbVector &, const MbVector & );

public: // \ru Функции доступа к данным. \en Getters and Setters. 

  /// \ru Дать начало ЛСК. \en Get the origin of local coordinate system. 
  const MbCartPoint & GetOrigin() const { return origin; }
  /// \ru Дать ось X. \en Get the X-axis. 
  const MbVector    & GetAxisX()  const { return axisX;  }
  /// \ru Дать ось Y. \en Get the Y-axis. 
  const MbVector    & GetAxisY()  const { return axisY;  }
  /// \ru Дать начало ЛСК. \en Get the origin of local coordinate system. 
  void        GetOrigin( MbCartPoint & pc ) const { pc = origin; }
  /// \ru Перевести точку и первые три производные из локальной в глобальную систему координат. \en Transform a point and the first three derivatives from a local coordinate system to the global coordinate system. 
  void        GetPointAndDerivesFrom( MbCartPoint & point,     MbVector & firstDer, 
                                      MbVector &    secondDer, MbVector & thirdDer,
                                      MbeLocalSystemType type = ls_CartesSystem ) const;

  // \ru Установить новое значение \en Set the new value 
  /// \ru Дать начало ЛСК. \en Get the origin of local coordinate system. 
  MbCartPoint & SetOrigin() { flag = MB_UNSET; return origin; }
  /// \ru Дать ось X. \en Get the X-axis. 
  MbVector    & SetAxisX()  { flag = MB_UNSET; return axisX;  }
  /// \ru Дать ось Y. \en Get the Y-axis. 
  MbVector    & SetAxisY()  { flag = MB_UNSET; return axisY;  }
  /// \ru Задать начало ЛСК. \en Set the origin of local coordinate system. 
  void        SetOrigin( const MbCartPoint & p );
  /// \ru Задать ось X. \en Set the X-axis. 
  void        SetAxisX ( const MbDirection & v );
  /// \ru Задать ось Y. \en Set the Y-axis. 
  void        SetAxisY ( const MbDirection & v );
  /// \ru Задать ось X. \en Set the X-axis. 
  void        SetAxisX ( const MbVector & v );
  /// \ru Задать ось Y. \en Set the Y-axis. 
  void        SetAxisY ( const MbVector & v );
  /// \ru Дать матрицу преобразования в  локальную СК: r=R*into (обратная матрица). \en Get the matrix of transformation to the local coordinate system: r=R*into (inverse matrix). 
  void        GetMatrixInto( MbMatrix & m ) const; 
  /// \ru Дать матрицу преобразования из локальной СК:  R=r*from (прямая матрица). \en Get the matrix of transformation from the local coordinate system:  R=r*from (direct matrix). 
  void        GetMatrixFrom( MbMatrix & m ) const;

public: // \ru Функции доступа к данным. \en Getters and Setters. 

  /// \ru Сдвинуть вдоль вектора. \en Translate along a vector. 
  void        Move( const MbVector & );
  /// \ru Сдвинуть на заданные приращения. \en Translate by given increments. 
  void        Move( double dx, double dy );
  /// \ru Повернуть вокруг точки на угол. \en Rotate at angle around a point. 
  void        Rotate( const MbCartPoint & pnt, double angle );
  /// \ru Повернуть вокруг точки на угол, заданный вектором направления. \en Rotate around a point at the angle given by direction vector. 
  void        Rotate( const MbCartPoint & pnt, const MbDirection & angle );
  /// \ru Преобразовать согласно матрице. \en Transform according to the matrix. 
  void        Transform( const MbMatrix & matr );
  /// \ru Рассчитать СК по начальной и конечной точкам. \en Calculate the coordinate system by the start point and the end point. 
  void        Calculate( const MbCartPoint & from, const MbCartPoint & to );
  /// \ru Пересчитать СК по измененным внутренним данным. \en Recalculate the coordinate system by changed internal data. 
  void        Reset();
  ///< \ru Масштабировать ЛСК. \en Scale the local coordinate system. 
  void        Scale( double sx, double sy, double &lx, double &ly );
  /// \ru Являются ли объекты равными? \en Determine whether an object is equal? 
  bool        IsSame( const MbPlacement & other, double accuracy ) const; 
  /// \ru Инвертировать ось 0X. \en Invert the 0X-axis. 
  void        InvertAxisX();
  /// \ru Инвертировать ось 0Y. \en Invert the 0Y-axis. 
  void        InvertAxisY();

  /// \ru Проверить на равенство. \en Check for equality. 
  bool        operator == ( const MbPlacement & ) const;
  /// \ru Проверить на неравенство. \en Check for inequality. 
  bool        operator != ( const MbPlacement & ) const;
  /// \ru Присвоить другую систему координат. \en Assign another coordinate system. 
  void        operator =  ( const MbPlacement & );
  /// \ru Умножить на локальную систему (как перемножение матриц): P = this * p. \en Multiply by a local coordinate system: P = this * p (as of matrix multiplication). 
  MbPlacement operator * ( const MbPlacement & p ) const;

  /// \ru Перевести точку из глобальной в локальную систему координат. \en Transform point from the global coordinate system to a local coordinate system. 
  void        TransformInto( MbCartPoint & ) const;
  /// \ru Перевести точку из локальной в глобальную систему координат. \en Transform point from a local coordinate system to the global coordinate system. 
  void        TransformFrom( MbCartPoint & ) const;
  /// \ru Перевести вектор из глобальной в локальную систему координат. \en Transform a vector from the global coordinate system to a local coordinate system. 
  void        TransformInto( MbVector & ) const;
  /// \ru Перевести вектор из локальной в глобальную систему координат. \en Transform a vector from a local coordinate system to the global coordinate system. 
  void        TransformFrom( MbVector & ) const;
  /// \ru Нормализовать. \en Normalize. 
  void        Normalize() { Reset(); }

public: // \ru Функции получения свойств. \en Properties. 

  /// \ru Свойство совпадения с мировой системой координат. \en Property of coincidence with global coordinate system. 
  bool        IsSingle     () const { return (MB_IDENTITY == CheckFlag()); }
  /// \ru Проверить, присутствует ли сдвиг системы координат относительно глобальной. \en Check if a coordinate system is translated relative to the global coordinate system. 
  bool        IsTranslation() const { return !!(CheckFlag() & MB_TRANSLATION); }
  /// \ru Проверить, присутствует ли поворот системы координат относительно глобальной. \en Check if the coordinate system is rotated relative to the global coordinate system. 
  bool        IsRotation   () const { return !!(CheckFlag() & MB_ROTATION); }
  /// \ru Выдать признак лево-ориентированного плейсмента. \en Get attribute of left placement. 
  bool        IsLeft       () const { return !!(CheckFlag() & MB_LEFT); }
  /// \ru Проверить, является ли СК ортогональной, но ненормированной. \en Check if a coordinate system is orthogonal, but not normalized. 
  bool        IsOrt        () const { return !!(CheckFlag() & MB_ORTOGONAL); }
  /// \ru Проверить признак ортогональности СК. \en Check orthogonality of a coordinate system. 
  bool        IsOrthogonal () const { CheckFlag(); return ( !(flag & MB_AFFINE) || !!(flag & MB_ORTOGONAL) ); }
  /// \ru Проверить, является ли СК аффинной (если нет - то она ортонормированная). \en Check if a coordinate system is affine (otherwise it is orthonormalized). 
  bool        IsAffine     () const { return !!(CheckFlag() & MB_AFFINE); }
  /// \ru Проверить признак ортонормированности СК. \en Check if a coordinate system is orthonormalized. 
  bool        IsNormal     () const { return !IsAffine(); }
  /// \ru Проверить, что битовые флаги не установлены. \en Check whether bit flags are not set. 
  bool        IsUnSet      () const { return !!( flag & MB_UNSET ); }

  /// \ru Вычислить и вернуть признак лево-ориентированного плейсмента. \en Calculate and return attribute of left placement. 
  bool        CheckLeft();
  
  /// \ru Проверить ортогональность. \en Check orthogonality. 
  bool        IsCardinalPoint( double eps = Math::AngleEps ) const;
  /// \ru Проверить параллелен оси 0X или 0Y. \en Check if parallel to 0X-axis or 0Y-axis 
  bool        IsCardinalStrict( double eps ) const;
  /// \ru Являются ли оси СК совпадающими со стандартной (правой ортонормированной СК). \en Check if axes of coordinate system are coincident to a standard one (right orthonormalized coordinate system). 
  bool        IsTranslationStandard() const;
  
  /// \ru Проверить, является ли СК ортогональной с равными по длине осями X,Y (круг остается кругом). \en Check if a coordinate system is orthogonal with axes X and Y of equal length (circle moves to circle). 
  bool        IsCircular() const;
  /// \ru Проверить, является ли СК ортогональной с равными по длине осями X,Y (круг остается кругом). \en Check if a coordinate system is orthogonal with axes X and Y of equal length (circle moves to circle). 
  bool        IsCircular( double & lxy ) const; 
  /// \ru Проверить, является ли СК ортогональной с равными по длине осями X,Y (круг остается кругом). \en Check if a coordinate system is orthogonal with axes X and Y of equal length (circle moves to circle). 
  bool        IsIsotropic() const { return IsCircular(); }
  /// \ru Проверить, является ли СК ортогональной с равными по длине осями X,Y (круг остается кругом). \en Check if a coordinate system is orthogonal with axes X and Y of equal length (circle moves to circle). 
  bool        IsIsotropic( double & lxy ) const { return IsCircular( lxy ); }

  /// \ru Выдать свойства объекта. \en Get properties of the object. 
  void        GetProperties( MbProperties & properties );
  /// \ru Записать свойства объекта. \en Set properties of the object. 
  void        SetProperties( const MbProperties & properties );

private:
  /// \ru Выставить флаги. \en Set flags. 
  uint8       ResetFlag() const;
  // Оценить флаги, если оценки не было
  uint8       CheckFlag() const { return IsUnSet() ? ResetFlag() : flag; }
  // Проверить флаг смещения.
  void        CheckOrigin() const { uint8 val = flag; ::CheckOrigin( *this, val, true ); flag = val; }
  // \ru Проверить флаг вращения. \en Check rotation flag. 
  void        CheckRotation() const { uint8 val = flag; ::CheckRotation( *this, val, true ); flag = val; }

  KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( MbPlacement, MATH_FUNC_EX )
  DECLARE_NEW_DELETE_CLASS( MbPlacement )
  DECLARE_NEW_DELETE_CLASS_EX( MbPlacement )
};


//----------------------------------------------------------------------------------------
// \ru Конструктор по умолчанию. \en Constructor. 
// ---
inline MbPlacement::MbPlacement() 
  : origin( 0.0, 0.0 )
  , axisX ( 1.0, 0.0 )
  , axisY ( 0.0, 1.0 )
  , flag  ( MB_IDENTITY )
{
}


//----------------------------------------------------------------------------------------
// \ru Конструктор копирования. \en Copy constructor. 
// ---
inline MbPlacement::MbPlacement( const MbPlacement & other )   
  : origin( other.origin ) // начало координат
  , axisX ( other.axisX  ) // направление оси 0X
  , axisY ( other.axisY  ) // направление оси 0Y
  , flag  ( other.flag   )
{ 
}


//------------------------------------------------------------------------------
// \ru Конструктор \en Constructor 
// ---
inline MbPlacement::MbPlacement( const MbMatrix & matr )
  : origin( matr.GetOrigin() )
  , axisX ( matr.GetAxisX() )
  , axisY ( matr.GetAxisY() )
  , flag( MB_UNSET )
{
  if ( !matr.IsAffine() ) {
    flag = MB_IDENTITY;
    if ( matr.IsTranslation() )
      flag |= MB_TRANSLATION;    
    if ( matr.IsRotation() )
      flag |= MB_ROTATION;
    if ( matr.IsLeft() )
      flag |= MB_LEFT;
    if ( matr.IsOrt() )
      flag |= MB_ORTOGONAL;
  }
}


//----------------------------------------------------------------------------------------
// \ru Инициализация единичной матрицы (мировая СК). \en Initialize.
// ---
inline void MbPlacement::Init()
{
  origin.SetZero();
  axisX.Init( 1.0, 0.0 );
  axisY.Init( 0.0, 1.0 );
  flag = MB_IDENTITY;
}


//----------------------------------------------------------------------------------------
// \ru Инициализация по плейсменту. \en Initialize by placement.
// ---
inline void MbPlacement::Init( const MbPlacement & init ) { 
  *this = init; 
}


//----------------------------------------------------------------------------------------
// \ru Сдвиг по вектору. \en Move by vector.
// ---
inline void MbPlacement::Move( const MbVector & to ) {
  origin.Move( to );
  CheckOrigin();
}


//----------------------------------------------------------------------------------------
// \ru Сдвиг по вектору. \en Move by vector.
// ---
inline void MbPlacement::Move( double dx, double dy ) {
  origin.Move( dx, dy );
  CheckOrigin();
}


//----------------------------------------------------------------------------------------
// \ru Трансформация по матрице. \en Transform by matrix.
// ---
inline void MbPlacement::Transform( const MbMatrix & matr )
{
  origin.Transform( matr );
  axisX.Transform( matr );
  axisY.Transform( matr );
  ResetFlag();
}


//----------------------------------------------------------------------------------------
// \ru Установить новое начало плейсмента. \en Set new origin.
// ---
inline void MbPlacement::SetOrigin( const MbCartPoint & pc )
{
  origin = pc;
  CheckOrigin();
}


//----------------------------------------------------------------------------------------
// \ru Проверка на равенство \en Check for equality 
// ---
inline bool MbPlacement::operator == ( const MbPlacement & with ) const
{
  return ( origin == with.origin ) &&
         ( axisX  == with.axisX )  &&
         ( axisY  == with.axisY );
}


//----------------------------------------------------------------------------------------
// \ru Проверка на неравенство \en Check for inequality 
// ---
inline bool MbPlacement::operator != ( const MbPlacement & with ) const {
  return !( *this == with );
}


//----------------------------------------------------------------------------------------
// \ru Присвоение другой системы координат \en Assignment of another coordinate system 
// ---
inline void MbPlacement::operator = ( const MbPlacement & other )
{
  origin = other.origin; // \ru Начало координат \en The origin 
  axisX  = other.axisX;  // \ru Направление оси 0X \en Direction of 0X-axis 
  axisY  = other.axisY;  // \ru Направление оси 0Y \en Direction of 0Y-axis 
  flag   = other.flag;
}


//----------------------------------------------------------------------------------------
// \ru Поворот \en Rotation 
// ---
inline void MbPlacement::Rotate( const MbCartPoint & pnt, double angle )
{
  MbDirection d( angle );
  Rotate( pnt, d );
}


//----------------------------------------------------------------------------------------
// \ru Вычислить и вернуть признак лево-ориентированного плейсмента \en Calculate and return attribute of left placement 
// ---
inline bool MbPlacement::CheckLeft()
{
  if ( !(flag & MB_UNSET) ) {
    if ( (axisX | axisY) < 0.0 )
      flag |= MB_LEFT;
    else
      flag &= ~MB_LEFT;
  }
  else {
    ResetFlag();
  }

  return !!( flag & MB_LEFT );
}


//----------------------------------------------------------------------------------------
// \ru Проверка параллелен оси 0X или 0Y \en Check if parallel to 0X-axis or 0Y-axis  
// ---
inline bool MbPlacement::IsCardinalPoint( double eps ) const
{
  return ( ((::fabs( axisX.y ) < eps) && (::fabs( axisY.x ) < eps)) ||
           ((::fabs( axisX.x ) < eps) && (::fabs( axisY.y ) < eps)) );
}


//----------------------------------------------------------------------------------------
// \ru Проверка параллелен оси 0X или 0Y \en Check if parallel to 0X-axis or 0Y-axis  
// ---
inline bool MbPlacement::IsCardinalStrict( double eps ) const {
  return ( (::fabs( axisX.y ) < eps) && (::fabs( axisY.x ) < eps) );
}


//----------------------------------------------------------------------------------------
// \ru Являются ли оси с.к. совпадающими со стандартной (правой ортонормированной с.к.) \en Check if the axes of a coordinate system are coincident to a standard one (right orthonormalized coordinate system) 
// ---
inline bool MbPlacement::IsTranslationStandard() const
{
  if ( !IsLeft() && IsNormal() ) {
    if ( axisX.Colinear( MbVector::xAxis, ANGLE_EPSILON ) && axisY.Colinear( MbVector::yAxis, ANGLE_EPSILON ) )
      return true; 
  }
  return false;
}


//----------------------------------------------------------------------------------------
// \ru Является ли с.к. ортогональной с равными по длине осями X,Y (круг остается кругом) \en Whether a coordinate system is orthogonal with the axes X and Y of equal length (circle moves to circle) 
// ---
inline bool MbPlacement::IsCircular() const
{
  if ( IsNormal() )
    return true;
  else if ( IsOrthogonal() && c3d::EqualLengths( axisX.Length(), axisY.Length() ) )
    return true;

  return false;
}


//----------------------------------------------------------------------------------------
// \ru Является ли с.к. ортогональной с равными по длине осями X,Y (круг остается кругом) \en Whether a coordinate system is orthogonal with the axes X and Y of equal length (circle moves to circle) 
// ---
inline bool MbPlacement::IsCircular( double & lxy ) const 
{
  lxy = 1.0;

  if ( IsNormal() )
    return true;  
  else if ( IsOrthogonal() ) {
    double lx = axisX.Length();
    double ly = axisY.Length();
    if ( c3d::EqualLengths( lx, ly ) ) {
      lxy = 0.5 * (lx + ly);
      return true;
    }
  }
  
  return false;
}


#endif // __MB_PLACEMENT_H
