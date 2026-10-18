////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Локальная система координат.
         \en A local coordinate system. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MB_PLACEMENT3D_H
#define __MB_PLACEMENT3D_H


#include <mb_cart_point.h>
#include <mb_cart_point3d.h>
#include <mb_matrix3d.h>
#include <mb_enum.h>


class MbDirection;
class MbCurve3D;
class MbCube;
class MbLine3D;
class MbPlacement3D;

namespace c3d // namespace C3D
{
  typedef std::pair<MbPlacement3D, MbCartPoint3D> PlacePoint3DPair;       ///< \ru Пара ЛСК-точка. \en Pair local coordinate system-point.
  typedef std::vector<PlacePoint3DPair>           PlacePoint3DPairVector; ///< \ru Вектор пар ЛСК-точка. \en Local coordinate system-point pairs vector.
}


//------------------------------------------------------------------------------
/** \brief \ru Типы локальных систем координат в трёхмерном пространстве.
           \en Types of local coordinate systems in three dimensional space. \~
  \ingroup Mathematic_Base_3D
*/
// ---
enum MbeLocalSystemType3D 
{
  ls_CartesianSystem,   ///< \ru Декартова система координат. \en Cartesian coordinate system. 
  ls_CylindricalSystem, ///< \ru Цилиндрическая система координат. \en Cylindrical coordinate system. 
  ls_SphericalSystem,   ///< \ru Сферическая система координат. \en Spherical coordinate system. 
};
  

//------------------------------------------------------------------------------
/** \brief \ru Локальная система координат (ЛСК) в трёхмерном пространстве.
           \en Local coordinate system in three dimensional space. \~
  \details \ru Локальная система координат в трёхмерном пространстве. \n
    В большинстве случаев система координат является правой, а векторы системы ортонормированы. 
    С помощью преобразований система координат (СК) может стать левой и не ортонормированной. 
    Локальная система координат является декартовой. 
    Точка в декартовой системе координат определяется тремя координатами x, y, z.\n
    Локальная система может выступать в роли цилиндрической или сферической системы координат.\n 
    Точка в цилиндрической системе координат определяется тремя координатами r, f, z: \n
    r, f - полярные координаты проекции точки на основную плоскость;\n
    r - длина проекции радиус-вектора; \n
    f - полярный угол проекции радиус-вектора; \n
    z - аппликата (расстояние от точки до основной плоскости). \n
    При использовании локальной системы координат в роли цилиндрической системы считаем, что: \n 
    начало координат цилиндрической системы совпадает с началом координат декартовой системы; \n
    ось Oz цилиндрической системы координат совпадает с осью Oz декартовой системы; \n
    основная плоскость цилиндрической системы координат совпадает с плоскостью Oxy декартовой системы; \n
    полярная ось координат цилиндрической системы совпадает с осью Ox декартовой системы; \n
    полярный угол f цилиндрической системы отсчитываем от оси Ox к положительному направлению оси Oy. \n
    Точка в сферической системе координат определяется тремя координатами r, f, w: \n 
    r - расстояние от начала координат до точки (длина радиус-вектора);\n
    f - угол между проекцией радиус-вектора на плоскость и лучом принадлежащим плоскости (долгота);\n 
    w - угол между радиус-вектором и нормалью к плоскости сферической системы (полярное расстояние).\n
    При использовании локальной системы координат в роли сферической системы считаем, что: \n 
    начало сферической системы совпадает с началом декартовой системы координат; \n
    луч перпендикулярный плоскости сферической системы координат совпадает с осью Oz декартовой системы; \n
    плоскость сферической системы координат совпадает с плоскостью Oxy декартовой системы; \n
    угол f сферической системы определяется отсчитываем от оси Ox к положительному направлению оси Oy.\n
    Для ускорения преобразования координат локальная система имеет дополнительные данные - флаг состояния.\n 
    Для получения данных системы координат извне следует пользоваться методами Get...\n 
    Для модификации данных системы координат извне следует пользоваться методами Set..., которые автоматически сбрасывают флаг системы в неустановленное состояние.\n
           \en Local coordinate system (LCS) in three dimensional space. \n
    Local coordinate system is described by the initial point and three non-parallel vectors.
    In most cases the system of coordinates is right, and vectors of system are orthonormalized. 
    A coordinate system can become left and not orthonormalized via transformations. 
    Local coordinate system is Cartesian. 
    A point is defined by three coordinates x, y, z in the Cartesian coordinate system.\n
    A local coordinate system may act both as a cylindrical or a spherical coordinate system.\n 
    A point in a cylindrical coordinate system is defined by three coordinates r, f, z: \n
    r, f - polar coordinates of point projection to the main plane;\n
    r - length of radius-vector projection; \n
    f - polar angle of radius-vector projection; \n
    z - z-axis (the distance from point to the main plane). \n
    When a local coordinate system is used as a cylindrical coordinate system, it is considered that: \n 
    the origin of a cylindrical coordinate system is coincident to the origin of Cartesian coordinate system; \n
    Oz-axis of a cylindrical coordinate system is coincident to Oz-axis of the Cartesian coordinate system; \n
    the main plane of a cylindrical coordinate system is coincident to Oxy-plane of the Cartesian coordinate system; \n
    the polar axis of a cylindrical coordinate system is coincident to Ox-axis of the Cartesian coordinate system; \n
    the polar angle f of a cylindrical coordinate system is counted from Ox-axis to the positive direction of Oy-axis. \n
    A point in a spherical coordinate system is defined by three coordinates r, f, w: \n 
    r - distance from the origin to a point (length of radius-vector);\n
    f - the angle between radius-vector projection to the plane and a ray on the plane (longitude);\n 
    w - the angle between radius-vector and the normal of spherical coordinate system plane (polar distance).\n
    When a local coordinate system is used as a spherical coordinate system, it is considered that: \n 
    the origin of a spherical coordinate system is coincident to the origin of the Cartesian coordinate system; \n
    the ray perpendicular to the plane of a spherical coordinate system is coincident to Oz-axis of the Cartesian coordinate system; \n
    the plane of a spherical coordinate system is coincident to Oxy-plane of the Cartesian coordinate system; \n
    the angle f of a spherical coordinate system is counted from Ox-axis to the positive direction of Oy-axis.\n
    To speed up transformation of coordinates the local coordinate system has additional data - flag of state.\n 
    Use Get... methods to obtain data of a coordinate system from the outside. \n 
    To modify data of a coordinate system from the outside use Set... methods that automatically reset the flag of the system to the unspecified state. \n \~
  \ingroup Mathematic_Base_3D
*/
// ---
class MATH_CLASS MbPlacement3D {
private:
  MbCartPoint3D origin; ///< \ru Положение начала локальной системы координат. \en Position of a coordinate system origin. 
  MbVector3D    axisX;  ///< \ru Направление первой  оси. \en Direction of the first axis. 
  MbVector3D    axisY;  ///< \ru Направление второй  оси. \en Direction of the second axis. 
  MbVector3D    axisZ;  ///< \ru Направление третьей оси. \en Direction of the third axis. 
  /**
    \brief \ru Состояние локальной системы координат.
           \en State of a local coordinate system. \~
    \details \ru Состояние локальной системы координат определяется установкой битовых полей: \n 
      MB_TRANSLATION - начало координат не ноль \n 
      MB_ROTATION    - система координат не единичная \n 
      MB_LEFT        - признак левой системы координат \n 
      MB_ORTOGONAL   - ортогональная система координат, взводится только в случае аффинной системы координат \n 
      MB_AFFINE      - система координат произвольная аффинная \n 
      MB_UNSET       - битовые флаги не установлены \n 
      При изменении системы координат flag должен быть сброшен в неустановленное состояние MB_UNSET. \n 
      Если flag == MB_UNSET, то при использовании системы координат происходит определение её состояния.\n
             \en State of a local coordinate system is defined by setting-up of bit fields: \n 
      MB_TRANSLATION - the origin of a coordinate system is not zero \n 
      MB_ROTATION    - a coordinate system is not unit \n 
      MB_LEFT        - attribute of a left coordinate system \n 
      MB_ORTOGONAL   - orthogonal coordinate system,  it is set-up only if a coordinate system is affine \n 
      MB_AFFINE      - any affine coordinate system \n 
      MB_UNSET       - bit flags not set \n 
      'flag' has to be reset to unspecified MB_UNSET state while changing the coordinate system. \n 
      if 'flag' == MB_UNSET, then the state of coordinate system is specified while it is being used.\n \~ 
  */
  mutable uint8 flag;

public:
  /// \ru Константа глобальной системы координат. \en A constant of the global coordinate system.
  static const MbPlacement3D global;  

public: /** \ru \name Конструкторы.
            \en \name The constructors.
            \{ */

  /// \ru Конструктор по умолчанию. \en Default constructor. 
  MbPlacement3D();
  /// \ru Конструктор по точке. \en Constructor by point. 
  explicit MbPlacement3D( const MbCartPoint3D & org );
  /// \ru Конструктор по двум векторам и точке, ортогонализует вектор `Y` и нормализует все оси. \en Constructor for two vectors and a point. It orthogonalizes the `Y` vector and normalizes all axes.
  explicit MbPlacement3D( const MbVector3D & axisX, const MbVector3D & axisY, const MbCartPoint3D & org );
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbPlacement3D( const MbPlacement3D & place );
  /// \ru Конструктор по матрице. \en Constructor by matrix. 
  MbPlacement3D( const MbMatrix3D & matr );
  /// \ru Конструктор по точке и двум векторам. \en Constructor by point and two vectors. 
  explicit MbPlacement3D( const MbCartPoint3D & org, const MbVector3D & axisZ, const MbVector3D & axisX, bool l = false );
  /// \ru Конструктор по трем точкам. \en Constructor by three points. 
  explicit MbPlacement3D( const MbCartPoint3D & org, const MbCartPoint3D & px, const MbCartPoint3D & py, bool l = false );
  /// \ru Конструктор по точке и вектору (ось Z) с произвольной осью X. \en Constructor by point and vector (Z-axis) with arbitrary X-axis. 
  MbPlacement3D( const MbCartPoint3D & org, const MbVector3D & axisZ, bool l = false );

public: /** \}
            \ru \name Функции инициализации.
            \en \name Initialization functions.
            \{ */
 
  /// \ru Инициализировать единичную матрицу прямого преобразования (мировая СК). \en Initialize identity matrix of a direct transformation (the global coordinate system). 
  MbPlacement3D & Init();
  /// \ru Инициализировать по плейсменту. \en Initialize by a placement. 
  MbPlacement3D & Init( const MbPlacement3D & );
  /// \ru Инициализировать по матрице. \en Initialize by a matrix. 
  MbPlacement3D & Init( const MbMatrix3D & matr );
  /// \ru Инициализировать с произвольной осью 'X'. \en Initialize by point and vector (Z-axis) with arbitrary X-axis. 
  void            Init( const MbCartPoint3D & initOrigin, const MbVector3D & initAxisZ );
  /// \ru Инициализировать по началу и векторам осей X, Z. \en Initialize by the origin and vectors of X and Z axes. 
  MbPlacement3D & Init( const MbCartPoint3D & p, const MbVector3D & axisZ, const MbVector3D & axisX,
                        bool left = false );

  /// \ru Инициализировать по началу и точкам осей X, Y. \en Initialize by the origin and points of X and Y axes. 
  MbPlacement3D & Init( const MbCartPoint3D & p, const MbCartPoint3D & axisX, const MbCartPoint3D & axisY,
                        bool left = false );
  /// \ru Инициализировать осью Z с произвольной осью X. \en Initialize by Z-axis and arbitrary X-axis. 
  MbPlacement3D & Init  ( const MbCartPoint3D & p, const MbVector3D & axisZ, 
                        bool left );
  /// \ru Инициализировать по плейсменту со смещением. \en Initialize by placement with a shift. 
  MbPlacement3D & Init( const MbPlacement3D & pl, double distance );
  /// \ru Инициализировать по началу и ориентирующему плейсменту. \en Initialize by origin and orienting placement. 
  MbPlacement3D & Init( const MbCartPoint3D & p, const MbPlacement3D & pl );
  /// \ru Инициализировать по началу. \en Initialize by origin. 
  MbPlacement3D & Init( const MbCartPoint3D & org );
  // \ru Методы инициализации по началу и двум векторам осям \en Methods for initialization by origin and two vectors of axes 
  
  /** \brief \ru Инициализировать по точке и осям `X`,`Y`. 
             \en Initialize by a point and two vectors (`X` and `Y` axes).
    \param[in] p - \ru Задает начало ЛСК. \en Specifies the origin of LCS. \~
    \param[in] axisX - \ru Задает ось `X` ЛСК. \en Specifies axis `X` of LCS. \~
    \param[in] axisY - \ru Задает ось `Y` ЛСК. \en Specifies axis `Y` of LCS. \~
    \param[in] setOrtho - \ru Если setOrtho == true, то ось `Y` ортогонализуется к `X`, а ось ЛСК нормализуются.
                          \en If setOrtho == true, then the `Y` is orthogonalized to `X`, and the LCS axis is normalized. \~
   */
  MbPlacement3D & InitXY( const MbCartPoint3D & p, const MbVector3D & axisX, const MbVector3D & axisY, bool setOrtho );
  /// \ru Инициализировать по точке и двум векторам (оси X, Z). \en Initialize by a point and two vectors (X and Z axes). 
  MbPlacement3D & InitXZ( const MbCartPoint3D & p, const MbVector3D & axisX, const MbVector3D & axisZ );
  /// \ru Инициализировать по точке и двум векторам (оси Y, Z). \en Initialize by a point and two vectors (Y and Z axes). 
  MbPlacement3D & InitYZ( const MbCartPoint3D & p, const MbVector3D & axisY, const MbVector3D & axisZ );

  /// \ru Инициализировать по кривой и углу к плоскости. \en Initialize by curve and angle to plane. 
  bool        Init  ( const MbPlacement3D & pl, double ang, const MbCurve3D & c, double t );
  /// \ru Инициализировать по точке и кривой. \en Initialize by point and curve. 
  bool        Init  ( const MbCartPoint3D & p, const MbCurve3D & c, double t );
  /// \ru Инициализировать по точке перпендикулярно кривой. \en Initialize by point, perpendicularly to curve. 
  bool        Init  ( const MbCurve3D &, const MbCartPoint3D &, bool checkPlanar );
  /// \ru Инициализировать по прямой и точке. \en Initialize by a line and a point. 
  bool        Init  ( const MbLine3D  &, const MbCartPoint3D &  );
  /// \ru Инициализировать по прямой и вектору. \en Initialize by a line and a vector. 
  bool        Init  ( const MbLine3D  &, const MbVector3D    &  );
  /// \ru Инициализировать по двум прямым. \en Initialize by two lines. 
  bool        Init  ( const MbLine3D  & l1, const MbLine3D & l2 );
  /// \ru Инициализировать перпендикулярно кривой по параметру на кривой. \en Initialize perpendicularly to a curve by a parameter on the curve. 
  bool        Init  ( const MbCurve3D &, double t, bool checkPlanar );

  // \ru Методы инициализации для привязки \en Methods for initialization for binding 
  /// \ru Привязать плейсмент к другим координатам. \en Bind placement to another coordinates. 
  void        Update( const MbPlacement3D &, VERSION version, bool fuzzy_null = true );
  /// \ru Привязать плейсмент к другим координатам. \en Bind placement to another coordinates. 
  void        Update( const MbPlacement3D &, const MbPlacement3D & ); 
  
  /// \ru Привязать плейсмент к локальному нулю. \en Bind placement to the local zero. 
  void        UpdateFromNull( VERSION version, bool fuzzy_null = true ) { Update( MbPlacement3D(), version, fuzzy_null ); } 
      
public: /** \}
            \ru \name Булевские свойства.
            \en \name The boolean properties.
            \{ */
 
  /// \ru Проверить, свойство совпадения с мировой СК (т.е. прямая матрица СК - единичная). \en Check coincidence with the global coordinate system (i.e. the direct matrix of a coordinate system is unit). 
  bool        IsSingle() const { return (MB_IDENTITY == CheckFlag()); }
  /// \ru Проверить, присутствует ли сдвиг системы координат относительно глобальной. \en Check if a coordinate system is translated relative to the global coordinate system. 
  bool        IsTranslation() const { return !!(CheckFlag() & MB_TRANSLATION); }
  /// \ru Проверить, присутствует ли поворот системы координат относительно глобальной. \en Check if the coordinate system is rotated relative to the global coordinate system. 
  bool        IsRotation() const { return !!(CheckFlag() & MB_ROTATION); }
  /// \ru Проверить, является ли СК левой. \en Check if a coordinate system is left. 
  bool        IsLeft() const { return !!(CheckFlag() & MB_LEFT); }
  /// \ru Проверить, является ли СК правой. \en Check if a coordinate system is right. 
  bool        IsRight() const { return !(CheckFlag() & MB_LEFT ); }
  /// \ru Проверить, является ли СК ортогональной, но ненормированной. \en Check if a coordinate system is orthogonal, but not normalized. 
  bool        IsOrt() const { return !!(CheckFlag() & MB_ORTOGONAL); }
  /// \ru Выдать признак ортогональности СК. \en Get orthogonality property of coordinate system. 
  bool        IsOrthogonal() const { CheckFlag(); return ( !(flag & MB_AFFINE) || !!(flag & MB_ORTOGONAL) ); }
  /// \ru Проверить, является ли СК аффинной (если нет - то она ортонормированная). \en Check if a coordinate system is affine (otherwise it is orthonormalized). 
  bool        IsAffine() const { return !!(CheckFlag() & MB_AFFINE ); }
  /// \ru Проверить, что СК ортонормированная. \en Check if coordinate system is orthonormalized. 
  bool        IsNormal() const { return ( !IsAffine() ); }
  /// \ru Проверить, что битовые флаги не установлены. \en Check whether bit flags are not set. 
  bool        IsUnSet() const { return !!( flag & MB_UNSET ); }
  /// \ru Выдать признак единичного плейсмента. \en Get attribute of the unit placement. 
  bool        IsUnit( double eps = Math::lengthEpsilon ) const;
  /// \ru Выдать признак единичного плейсмента и квадраты ортов. \en Get attribute of unit placement and squares of orts. 
  bool        IsUnit( double & sqX, double & sqY, double & sqZ, double eps = Math::lengthEpsilon ) const;  
  /// \ru Проверить, являются ли оси СК совпадающими со стандартной (правой ортонормированной СК). \en Check if the axes of a coordinate system are coincident to the standard one (right orthonormalized coordinate system). 
  bool        IsTranslationStandard() const; 
  /// \ru Проверить, является ли СК ортогональной с равными по длине осями X,Y (круг остается кругом). \en Check if a coordinate system is orthogonal with axes X and Y of equal length (circle moves to circle). 
  bool        IsCircular () const;
  /// \ru Проверить, является ли СК ортогональной с равными по длине осями X,Y (круг остается кругом). \en Check if a coordinate system is orthogonal with axes X and Y of equal length (circle moves to circle). 
  bool        IsCircular ( double & lxy ) const;
  /// \ru Проверить, является ли СК ортогональной с равными по длине осями X,Y,Z. \en Check if a coordinate system is orthogonal with axes X and Y of equal length. 
  bool        IsIsotropic() const;
  /// \ru Проверить, является ли СК ортогональной с равными по длине осями X,Y,Z. \en Check if a coordinate system is orthogonal with axes X and Y of equal length. 
  bool        IsIsotropic( double & l ) const;

public: /** \}
            \ru \name Функции доступа к полям.
            \en \name Functions for access to fields.
            \{ */
 
  /// \ru Получить начало СК. \en Get the origin of a coordinate system. 
  const MbCartPoint3D & GetOrigin() const { return origin; }
  /// \ru Получить ось Z. \en Get the Z-axis. 
  const MbVector3D    & GetAxisZ () const { return axisZ;  }
  /// \ru Получить ось X. \en Get the X-axis. 
  const MbVector3D    & GetAxisX () const { return axisX;  }
  /// \ru Получить ось Y. \en Get the Y-axis. 
  const MbVector3D    & GetAxisY () const { return axisY;  }  
  /// \ru Дать начало ЛСК. \en Get the origin of a local coordinate system. 
  MbCartPoint3D & SetOrigin() { flag = MB_UNSET; return origin; }
  /// \ru Дать ось Z. \en Get the Z-axis. 
  MbVector3D    & SetAxisZ () { flag = MB_UNSET; return axisZ; }
  /// \ru Дать ось X. \en Get the X-axis. 
  MbVector3D    & SetAxisX () { flag = MB_UNSET; return axisX; }
  /// \ru Дать ось Y. \en Get the Y-axis. 
  MbVector3D    & SetAxisY () { flag = MB_UNSET; return axisY; }
  /// \ru Задать начало ЛСК. \en Set the origin of a local coordinate system. 
  void        SetOrigin( const MbCartPoint3D & o ) { origin = o; CheckOrigin();  }
  /// \ru Задать ось Z. \en Set the Z-axis. 
  void        SetAxisZ ( const MbVector3D & a    ) { flag = MB_UNSET; axisZ = a; }
  /// \ru Задать ось X. \en Set the X-axis. 
  void        SetAxisX ( const MbVector3D & a    ) { flag = MB_UNSET; axisX = a; }
  /// \ru Задать ось Y. \en Set the Y-axis. 
  void        SetAxisY ( const MbVector3D & a    ) { flag = MB_UNSET; axisY = a; }
  /// \ru Инвертировать ось Z. \en Invert the Z-axis. 
  void        AxisZInvert() { flag ^= MB_LEFT; axisZ.Invert(); CheckRotation(); }
  /// \ru Инвертировать ось X. \en Invert the X-axis. 
  void        AxisXInvert() { flag ^= MB_LEFT; axisX.Invert(); CheckRotation(); }
  /// \ru Инвертировать ось Y. \en Invert the Y-axis. 
  void        AxisYInvert() { flag ^= MB_LEFT; axisY.Invert(); CheckRotation(); }
  /// \ru Установить флаг состояния. \en Set the flag of state. 
  void        SetFlag( bool bLeft, bool bAffine = false, bool bOrt = true ) const;
  /// \ru Установить СК как правую. \en Set the coordinate system as right. 
  void        SetRight();

public: /** \}
            \ru \name Общие функции математического объекта.
            \en \name The common functions of the mathematical object.
            \{ */

  /// \ru Преобразовать согласно матрице. \en Transform according to the matrix. 
  MbPlacement3D & Transform( const MbMatrix3D & );
  /// \ru Сдвинуть вдоль вектора. \en Translate along a vector. 
  MbPlacement3D & Move( const MbVector3D & );
  /// \ru Повернуть вокруг оси на заданный угол. \en Rotate at a given angle around an axis. 
  MbPlacement3D & Rotate( const MbAxis3D & axis, double angle );
  /// \ru Масштабировать. \en Scale. 
  MbPlacement3D & Scale( double sx, double sy, double sz );
  /// \ru Масштабировать. \en Scale. 
  MbPlacement3D & Scale( double s );
  /// \ru Являются ли объекты равными? \en Determine whether an object is equal? 
  bool        IsSame( const MbPlacement3D & other, double accuracy ) const; 
                       
  /// \ru Дать матрицу преобразования в ЛСК: r=R*into (обратная матрица). \en Get the matrix of transformation to the local coordinate system: r=R*into (inverse matrix). 
  void        GetMatrixInto( MbMatrix3D & ) const; 
  /// \ru Дать матрицу преобразования в ЛСК: r=R*into (обратная матрица). \en Get the matrix of transformation to the local coordinate system: r=R*into (inverse matrix). 
  MbMatrix3D  GetMatrixInto()               const;
  /// \ru Дать матрицу преобразования из ЛСК: R=r*from (прямая матрица). \en Get the matrix of transformation from the local coordinate system: R=r*from (direct matrix). 
  void        GetMatrixFrom( MbMatrix3D & ) const;
  /// \ru Дать матрицу преобразования из ЛСК: R=r*from (прямая матрица). \en Get the matrix of transformation from the local coordinate system: R=r*from (direct matrix). 
  MbMatrix3D  GetMatrixFrom()               const;
  /// \ru Дать матрицу симметрии относительно плоскости XY. \en Get the matrix of symmetry with respect to the XY-plane. 
  void        Symmetry     ( MbMatrix3D & ) const;
                       
  /// \ru Дать матрицу преобразования в place. \en Get the matrix of transformation to place. 
  bool        GetMatrixToPlace( const MbPlacement3D & p, MbMatrix & matr, double eps = Math::angleRegion ) const;
  /// \ru Дать матрицу преобразования в place. \en Get the matrix of transformation to place. 
  void        GetMatrixToPlace( const MbPlacement3D & p, MbMatrix3D & matr ) const;

  /// \ru Вычислить расстояние до точки. \en Calculate the distance to a point. 
  double      DistanceToPoint ( const MbCartPoint3D & to ) const;
  /// \ru Найти проекцию точки на плоскость XY. \en Find point projection to XY-plane. 
  void        PointProjection ( const MbCartPoint3D & p, MbCartPoint3D & pOn ) const;
  /// \ru Найти вектор проекции на плоскость XY. \en Find the vector of projection to XY-plane. 
  void        VectorProjection( const MbVector3D    & v, double & x, double & y ) const;
  /// \ru Найти проекцию точки на плоскость XY. \en Find the point projection to XY-plane. 
  void        PointProjection ( const MbCartPoint3D & p, double & x, double & y ) const;
  /// \ru Найти проекцию точки на плоскость XY вдоль вектора в любом из двух направлений. \en Find the point projection to XY-plane along a vector in either of two directions. 
  bool        DirectPointProjection( const MbCartPoint3D & p, const MbVector3D & v, double & x, double & y ) const;
  /// \ru Найти проекцию точки на плоскость XY в направлении вектора. \en Find the point projection to XY-plane in direction of the vector. 
  bool        NearDirectPointProjection( const MbCartPoint3D & p, const MbVector3D & v, double & x, double & y, bool onlyPositiveDirection ) const;
  /// \ru Дать координаты точки на плоскости XY. \en Get the projection of a point on XY-plane. 
  void        PointOn ( double x, double y,    MbCartPoint3D & p ) const { p.Set( origin, axisX,   x, axisY,   y ); }
  /// \ru Дать пространственную точку по точке на плоскости XY. \en Get the space point by a point on XY-plane. 
  void        PointOn ( const MbCartPoint & r, MbCartPoint3D & p ) const { p.Set( origin, axisX, r.x, axisY, r.y ); }
  /// \ru Дать пространственный вектор по вектору на плоскости XY. \en Get the space vector by a vector on XY-plane. 
  void        VectorOn( const MbVector    & r, MbVector3D    & p ) const { p.Set( axisX,  r.x, axisY,  r.y ); }
  /// \ru Дать пространственный вектор по вектору на плоскости XY. \en Get the space vector by a vector on XY-plane. 
  void        VectorOn( const MbDirection & r, MbVector3D    & p ) const { p.Set( axisX, r.ax, axisY, r.ay ); }

  /// \ru Дать нормаль плоскости XY локальной системы координат, направление оси Z при этом не учитывается. \en Get the normal of the plane XY of a local coordinate system, the Z axis is not taken into account here. 
  void        Normal( MbVector3D & n ) const;
  /// \ru Дать нормаль плоскости XY локальной системы координат, направление оси Z при этом не учитывается. \en Get the normal of the plane XY of a local coordinate system, the Z axis is not taken into account here. 
  MbVector3D  Normal() const;

  // \ru C какой стороны от плоскости находится точка. \en Point location relative to the plane. 
  // \ru iloc_OnItem = 0 - на плоскости \en Iloc_OnItem = 0 - on plane 
  // \ru iloc_InItem = 1 - над плоскостью \en Iloc_InItem = 1 - above plane 
  // \ru iloc_OutOfItem = -1 - под плоскостью \en Iloc_OutOfItem = -1 - below plane 
  /// \ru Определить, с какой стороны от плоскости находится точка. \en Get point location relative to the plane. 
  MbeItemLocation PointRelative( const MbCartPoint3D & pnt, double eps = ANGLE_REGION ) const; 
  // \ru C какой стороны от плоскости находится габарит. \en Bounding box location relative to the plane. 
  // \ru iloc_OnItem = 0 - на плоскости \en Iloc_OnItem = 0 - on plane 
  // \ru iloc_InItem = 1 - над плоскостью \en Iloc_InItem = 1 - above plane 
  // \ru iloc_OutOfItem = -1 - под плоскостью \en Iloc_OutOfItem = -1 - below plane 
  /// \ru Определить, с какой стороны от плоскости находится габарит. \en Get bounding box location relative to the plane. 
  MbeItemLocation CubeRelative( const MbCube & cube, double eps = ANGLE_REGION ) const;

  // \ru Проверки расположения. \en Location checks. 
  /// \ru Проверить коллинеарность. \en Check collinearity 
  bool        Colinear      ( const MbPlacement3D & with, double eps = Math::angleRegion ) const;
  /// \ru Проверить коллинеарность нормалей. \en Check collinearity of normals. 
  bool        NormalColinear( const MbPlacement3D & with, double eps = Math::angleRegion ) const;
  /// \ru Проверить компланарность. \en Check complanarity. 
  bool        Complanar     ( const MbPlacement3D & with, double eps = Math::angleRegion ) const;
  /// \ru Проверить ортогональность. \en Check orthogonality. 
  bool        Orthogonal    ( const MbAxis3D &, double & x, double & y, double eps = Math::angleRegion ) const;

  /// \ru Совместить с place путем вращения до параллельности и перемещения вдоль нормали place. \en Match with the place by rotation till the parallelism and translation along place normal. 
  void        AdaptToPlace  ( const MbPlacement3D & );
  /// \ru Угол с осью Z. \en Angle to Z-axis. 
  double      Angle( const MbVector3D & v  ) const { return v.Angle(axisZ); }
  /// \ru Угол между осями Z плейсментов. \en Angle between Z-axes of the placements.
  double      Angle( const MbPlacement3D & ) const;
  /// \ru Дать синус угла вектора с плоскостью. \en Get the sine of angle between a vector and the plane. 
  double      GetNormalAngle( const MbVector3D & ) const;

  /// \ru Пересчитать СК по измененным внутренним данным. \en Recalculate the coordinate system for changed internal data. 
  void        Reset ();
  /// \ru Инвертировать. \en Invert. 
  void        Invert( MbMatrix * = nullptr );

  /** \brief \ru Найти ближайшую точку пересечения с линией.
             \en Find the nearest point of intersection with line. \~
    \details \ru Найти ближайшую точку пересечения с линией. Линия задана своими начальной точкой и направляющим вектором: p(d) = origin + d * direction.\n
             \en Find the nearest point of intersection with a line. The line is set by its origin and direction: p(d) = origin + d * direction.\n \~ 
    \param[in] origin    - \ru Начальная точка исходной линии.
                           \en The origin of the input line. \~
    \param[in] direction - \ru Направляющий вектор исходной линии.
                           \en The direction of the input line. \~
    \param[out] p        - \ru Найденная точка пересечения.
                           \en The found intersection point. \~
    \param[out] d        - \ru Параметр точки пересечения на линии.
                           \en A parameter of the intersection point along the line. \~
    \param[in] eps       - \ru Толерантность, при которой линия будет считаться параллельной плоскости.
                           \en A tolerance determining the situation when the line is parallel to the plane. \~
    \result \ru Возвращает true, если пересечение найдено.
            \en Returns true if the intersection point has been found. \~
    \ingroup Mathematic_Base_3D
  */
  bool        LineIntersectionPoint( const MbCartPoint3D & origin, const MbVector3D & direction, MbCartPoint3D & p, double & d, double eps = ANGLE_EPSILON ) const;
  /// \ru Получить точку и направление линии пересечения плоскостей. \en Get the point and direction of planes intersection line. 
  bool        PlanesIntersection   ( const MbPlacement3D & place, MbCartPoint3D & p, MbVector3D & axis, double eps = ANGLE_EPSILON ) const;
  /// \ru Нахождение точки пересечения плоскости XY и кривой. \en Intersection plane XY with a curve.
  MbeNewtonResult CurveIntersectNewton( const MbCurve3D & curve, bool ext, double funcEpsilon, size_t iterLimit,
                                        double & u, double & v, double & t ) const;
  /// \ru Вычислить параметры ближайших точек плоскости XY и кривой. \en Calculate parameters of the nearest points of surface and curve. \~
  MbeNewtonResult NearestPoints( const MbCurve3D & curve, bool ext, double & u0, double & v0, 
                                 double & t0, double & dmin, bool tCalc ) const;

  /// \ru Преобразовать 2D-точки в другую 2D-точку. \en Transform 2D-point to another 2D-point. 
  void        TransformPoint       ( const MbMatrix3D &, MbCartPoint & ) const;

  /// \ru Дать минимально различимую величину параметра U. \en Give the minimum distinguishable value of U parameter. 
  double      GetXEpsilon() const;
  /// \ru Дать минимально различимую величину параметра V. \en Get the minimum distinguishable value of V parameter. 
  double      GetYEpsilon() const;
  /// \ru Округлить. \en Round. 
  bool        SetRoundedValue( bool total, double eps );

  /// \ru Проверить на равенство. \en Check for equality. 
  bool        operator == ( const MbPlacement3D & ) const;
  /// \ru Проверить на неравенство. \en Check for inequality. 
  bool        operator != ( const MbPlacement3D & ) const;
  /// \ru Присвоить другую СК. \en Assign another coordinate system. 
  void        operator  = ( const MbPlacement3D & );
  /// \ru Умножить на локальную систему (как перемножение матриц): P = this * p. \en Multiply by a local coordinate system: P = this * p (as of matrix multiplication). 
  MbPlacement3D operator * ( const MbPlacement3D & p ) const;

  /// \ru Ортогонализовать и нормализовать оси ЛСК. \en Orthogonalize and normalize axes of a local coordinate system. 
  void        Normalize();
  /// \ru Проверить,является ли объект смещением. \en Check if the object is a translation. 
  bool        IsShift ( const MbPlacement3D &, MbVector3D &, bool & isSame, double accuracy = LENGTH_EPSILON ) const; 
  /// \ru Является ли объект смещением данного вдоль оси Z? \en Check if the object is a shift of the current one along axis Z?
  bool        IsShiftZ( const MbPlacement3D & other, MbVector3D & vect, double accuracy ) const;
  /// \ru Проверить на вырожденность. \en Check for degeneracy. 
  bool        IsDegenerate( double lenEps = Math::metricRegion, double angEps = Math::angleRegion ) const;

public: /** \}
            \ru \name Функции для перевода точек и векторов
            \en \name Functions for points and vectors transformation
            \{ */
  /// \ru Дать точку в глобальной СК по локальным координатам. \en Get the point in the global coordinate system by local coordinates. 
  void        GetPointFrom ( double x0, double y0, double z0, MbCartPoint3D & p,
                             MbeLocalSystemType3D type = ls_CartesianSystem                    ) const;
  /// \ru Дать вектор в глобальной СК по локальным координатам. \en Get the vector in the global coordinate system by local coordinates. 
  void        GetVectorFrom( double x1, double y1, double z1, MbVector3D & v,
                             MbeLocalSystemType3D type = ls_CartesianSystem                    ) const;
  /// \ru Перевести точку из локальной в глобальную СК. \en Transform a point from a local coordinate system to the global coordinate system. 
  void        GetPointFrom ( MbCartPoint3D & p, MbeLocalSystemType3D type = ls_CartesianSystem ) const;
  /// \ru Перевести вектор из локальной в глобальную СК. \en Transform a vector from a local coordinate system to the global coordinate system. 
  void        GetVectorFrom( MbVector3D & v,    MbeLocalSystemType3D type = ls_CartesianSystem ) const;
  /// \ru Перевести точку из глобальной в локальную СК. \en Transform a point from the global coordinate system to a local coordinate system. 
  void        GetPointInto ( MbCartPoint3D & p, MbeLocalSystemType3D type = ls_CartesianSystem ) const;
  /// \ru Перевести вектор из глобальной в локальную СК. \en Transform a vector from the global coordinate system to a local coordinate system. 
  void        GetVectorInto( MbVector3D & v,    MbeLocalSystemType3D type = ls_CartesianSystem ) const;
  /// \ru Перевести точку и первые три производные из локальной в глобальную СК. \en Transform a point and the first three derivatives from a local coordinate system to the global coordinate system. 
  void        GetPointAndDerivesFrom( MbCartPoint3D & point,     MbVector3D & firstDer,
                                      MbVector3D &    secondDer, MbVector3D & thirdDer,
                                      MbeLocalSystemType3D type = ls_CartesianSystem ) const;
  /// \ru Перевести точку и первые три производные из глобальной в локальную СК. \en Transform a point and the first three derivatives from the global coordinate system to a local coordinate system. 
  void        GetPointAndDerivesInto( MbCartPoint3D & point,     MbVector3D & firstDer,
                                      MbVector3D &    secondDer, MbVector3D & thirdDer,
                                      MbeLocalSystemType3D type = ls_CartesianSystem ) const;
  /** \} */

  /// \ru Выдать свойства объекта. \en Get properties of the object. 
  void        GetProperties( MbProperties & );
  /// \ru Записать свойства объекта. \en Set properties of the object. 
  void        SetProperties( const MbProperties & );
                              
private :
  // \ru Выставить флаги. \en Set flags. 
  uint8       ResetFlag() const;
  // Оценить флаги, если оценки не было
  uint8       CheckFlag() const { return IsUnSet() ? ResetFlag() : flag; }
  // Проверить флаг смещения.
  void        CheckOrigin() const { uint8 val = flag; ::CheckOrigin3D( *this, val, true ); flag = val; }
  // \ru Проверить флаг вращения. \en Check rotation flag. 
  void        CheckRotation() const { uint8 val = flag; ::CheckRotation3D( *this, val, true ); flag = val; }

  KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( MbPlacement3D, MATH_FUNC_EX )
  DECLARE_NEW_DELETE_CLASS( MbPlacement3D )
  DECLARE_NEW_DELETE_CLASS_EX( MbPlacement3D )
};


//------------------------------------------------------------------------------
// \ru Конструктор. \en Constructor. 
// ---
inline MbPlacement3D::MbPlacement3D() 
  : origin( 0.0, 0.0, 0.0 )
  , axisX ( 1.0, 0.0, 0.0 )
  , axisY ( 0.0, 1.0, 0.0 )
  , axisZ ( 0.0, 0.0, 1.0 )
  , flag  ( MB_IDENTITY   )
{
}


//------------------------------------------------------------------------------
// \ru Конструктор. \en Constructor. 
// ---
inline MbPlacement3D::MbPlacement3D( const MbCartPoint3D & org ) 
  : origin( org           )
  , axisX ( 1.0, 0.0, 0.0 )
  , axisY ( 0.0, 1.0, 0.0 )
  , axisZ ( 0.0, 0.0, 1.0 )
  , flag  ( MB_IDENTITY   )
{
  CheckOrigin();
}


//------------------------------------------------------------------------------
// \ru Конструктор. \en Constructor.
// ---
inline MbPlacement3D::MbPlacement3D( const MbVector3D & axisX, const MbVector3D & axisY, const MbCartPoint3D & org )
  : origin( 0.0, 0.0, 0.0 )
  , axisX ( 1.0, 0.0, 0.0 )
  , axisY ( 0.0, 1.0, 0.0 )
  , axisZ ( 0.0, 0.0, 1.0 )
  , flag( MB_UNSET )
{
  InitXY( org, axisX, axisY, true/*normalize*/);
}


//------------------------------------------------------------------------------
// \ru Конструктор. \en Constructor.
// ---
inline MbPlacement3D::MbPlacement3D( const MbPlacement3D & place )
  : origin( 0.0, 0.0, 0.0 )
  , axisX ( 1.0, 0.0, 0.0 )
  , axisY ( 0.0, 1.0, 0.0 )
  , axisZ ( 0.0, 0.0, 1.0 )
  , flag( MB_UNSET )
{
  Init( place );
}


//------------------------------------------------------------------------------
// \ru Конструктор \en Constructor 
// ---
inline MbPlacement3D::MbPlacement3D( const MbMatrix3D & matr )
  : origin( 0.0, 0.0, 0.0 )
  , axisX ( 1.0, 0.0, 0.0 )
  , axisY ( 0.0, 1.0, 0.0 )
  , axisZ ( 0.0, 0.0, 1.0 )
  , flag( MB_UNSET )
{
  Init( matr );
}


//------------------------------------------------------------------------------
// \ru Инициализация единичной матрицы (мировая СК). \en Initialize.
// ---
inline MbPlacement3D & MbPlacement3D::Init()
{
  origin.SetZero();
  axisX.Init( 1.0, 0.0, 0.0 );
  axisY.Init( 0.0, 1.0, 0.0 );
  axisZ.Init( 0.0, 0.0, 1.0 );
  flag = MB_IDENTITY;
  return *this;
}


//------------------------------------------------------------------------------
// \ru Инициализация по плейсменту. \en Initialize by placement.
// ---
inline MbPlacement3D & MbPlacement3D::Init( const MbPlacement3D & place )
{
  origin = place.origin;
  axisZ  = place.axisZ;
  axisX  = place.axisX;
  axisY  = place.axisY;
  flag   = place.flag;
  return *this;
}


//------------------------------------------------------------------------------
// \ru Инициализация по плейсменту и расстоянию. \en Initialize by placement and distance.
// ---
inline MbPlacement3D & MbPlacement3D::Init( const MbPlacement3D & init, double distance )
{
  Init( init );
  return Move( Normal() * distance );
}


//------------------------------------------------------------------------------
// \ru Инициализация по точке и плейсменту. \en  Initialize by point and placement.
// ---
inline MbPlacement3D & MbPlacement3D::Init( const MbCartPoint3D & p, const MbPlacement3D & init ) 
{
  return Init( p, init.axisZ, init.axisX );
}


//------------------------------------------------------------------------------
// \ru Инициализация по точке. \en Initialize by point.
// ---
inline MbPlacement3D &  MbPlacement3D::Init( const MbCartPoint3D & p ) 
{
  return Init( p, axisZ, axisX );
}


//------------------------------------------------------------------------------
// \ru Сдвиг по вектору. \en Move by vector.
// ---
inline MbPlacement3D & MbPlacement3D::Move( const MbVector3D & to )
{
  origin.Move( to );
  CheckOrigin();
  return *this;
}


//------------------------------------------------------------------------------
// \ru Трансформация по матрице. \en Transform by matrix.
// ---
inline MbPlacement3D & MbPlacement3D::Transform( const MbMatrix3D & matr )
{
  origin.Transform( matr );

  if ( matr.IsRotation() ) {
    axisZ.Transform( matr );
    axisX.Transform( matr );
    axisY.Transform( matr );
    flag = MB_UNSET;
  }
  ResetFlag();
  return *this;
}


//------------------------------------------------------------------------------
// \ru Расстояние до точки. \en Get distance to point.
// ---
inline double MbPlacement3D::DistanceToPoint( const MbCartPoint3D & to ) const
{
  MbCartPoint3D pOn;
  PointProjection( to, pOn );
  return pOn.DistanceToPoint( to );
}


//------------------------------------------------------------------------------
// \ru Спроецировать точку на плоскость XY. \en Get point projection onto XY-plane.
// ---
inline void MbPlacement3D::PointProjection( const MbCartPoint3D & p, MbCartPoint3D & pOn ) const
{
  double x, y;
  PointProjection( p, x, y );
  PointOn( x, y, pOn );
}


//------------------------------------------------------------------------------
// \ru Спроецировать точку на плоскость XY. \en Get point projection onto XY-plane.
// ---
inline void MbPlacement3D::PointProjection( const MbCartPoint3D & p, double & x, double & y ) const
{
  MbVector3D vect( origin, p );
  VectorProjection( vect, x, y );
}


//------------------------------------------------------------------------------
// \ru Дать матрицу для преобразования симметрии относительно плоскости. \en Get symmetry matrix relative to XY-plane.
// ---
inline void MbPlacement3D::Symmetry( MbMatrix3D & sym ) const {
  sym.Symmetry( origin, axisX, axisY );
}


//------------------------------------------------------------------------------
// \ru Дать матрицу преобразования в place. \en Get transformation matrix from this to place.
// ---
inline void MbPlacement3D::GetMatrixToPlace( const MbPlacement3D & place, MbMatrix3D & matr ) const {
  matr = ( GetMatrixFrom() * place.GetMatrixInto() );
}


//-------------------------------------------------------------------------------
// \ru Сделать с.к. правой \en Make the coordinate system right 
// ---
inline void MbPlacement3D::SetRight()
{
  if ( IsLeft() ) {
    axisZ.Invert();
    flag &= ~MB_LEFT;
  }
}


//------------------------------------------------------------------------------
// \ru Являются ли оси с.к. совпадающими со стандартной (правой ортонормированной с.к.) \en Check if the axes of a coordinate system are coincident to a standard one (right orthonormalized coordinate system) 
// ---
inline bool MbPlacement3D::IsTranslationStandard() const
{
  if ( !IsNormal() && IsRight() ) {
    const double eps = ANGLE_EPSILON;
    if ( axisX.Colinear( MbVector3D::xAxis, eps ) && axisY.Colinear( MbVector3D::yAxis, eps ) && axisZ.Colinear( MbVector3D::zAxis, eps ) )
      return true; 
  }
  return false;
}


//------------------------------------------------------------------------------
// \ru Является ли с.к. ортогональной с равными по длине осями X,Y (круг остается кругом) \en Whether a coordinate system is orthogonal with the axes X and Y of equal length (circle moves to circle) 
// ---
inline bool MbPlacement3D::IsCircular() const
{
  if ( IsNormal() )
    return true;
  else {
    bool isOrthogonal = IsOrthogonal();

    if ( !isOrthogonal ) { // BUG_55499
      if ( axisX.Orthogonal( axisY, EXTENT_EPSILON ) )
        isOrthogonal = true;
    }
    if ( isOrthogonal ) {
      if ( c3d::EqualLengths( axisX.Length(), axisY.Length() ) )
        return true;
    }
  }

  return false;
}


//------------------------------------------------------------------------------
// \ru Является ли с.к. ортогональной с равными по длине осями X,Y (круг остается кругом) \en Whether a coordinate system is orthogonal with the axes X and Y of equal length (circle moves to circle) 
// ---
inline bool MbPlacement3D::IsCircular( double & lxy ) const 
{
  lxy = 1.0;
  
  if ( IsNormal() )
    return true;
  else {
    bool isOrthogonal = IsOrthogonal();

    if ( !isOrthogonal ) { // BUG_55499
      if ( axisX.Orthogonal( axisY, EXTENT_EPSILON ) )
        isOrthogonal = true;
    }
    if ( isOrthogonal ) {
      double lx = axisX.Length();
      double ly = axisY.Length();
      if ( c3d::EqualLengths( lx, ly ) ) {
        lxy = 0.5 * (lx + ly);
        return true;
      }
    }
  }
  
  return false;
}


//------------------------------------------------------------------------------
// \ru Является ли с.к. ортогональной и изотропной по осям \en Whether a coordinate system is orthogonal and isotropic by axes 
// ---
inline bool MbPlacement3D::IsIsotropic() const
{
  if ( IsNormal() )
    return true;
  else if ( IsOrthogonal() ) {
    double lx = axisX.Length();
    double ly = axisY.Length();
    if ( c3d::EqualLengths( lx, ly ) ) {
      double lz = axisZ.Length();
      if ( c3d::EqualLengths( lz, lx ) && c3d::EqualLengths( lz, ly ) )
        return true;
    }
  }
  return false;
}


//------------------------------------------------------------------------------
// \ru Является ли с.к. ортогональной и изотропной по осям \en If coordinate system is orthogonal and isotropic by axes 
// ---
inline bool MbPlacement3D::IsIsotropic( double & l ) const
{
  l = 1.0;

  if ( IsNormal() )
    return true;
  else if ( IsOrthogonal() ) {
    double lx = axisX.Length();
    double ly = axisY.Length();
    if ( c3d::EqualLengths( lx, ly ) ) {
      double lz = axisZ.Length();
      if ( c3d::EqualLengths( lz, lx ) && c3d::EqualLengths( lz, ly ) ) {
        l = (lx + ly + lz) * c3d::ONE_THIRD;
        if ( ::fabs(l - 1.0) < EPSILON )
          l = 1.0;
        return true;
      }
    }
  }
  return false;
}


//------------------------------------------------------------------------------
// \ru Дать нормаль \en Give the normal 
// ---
inline void MbPlacement3D::Normal( MbVector3D & n ) const
{
  // This disabled code is faster, but it breaks some models that have not good fillets. Difference is in 1e-16.
  //if ( IsNormal() ) {
  //  n.Init( axisZ );
  //  if ( IsLeft() )
  //    n.Invert();
  //}
  //else
  if ( IsOrt() ) {
    n.Init( axisZ );
    if ( IsLeft() )
      n.Invert();
    if ( IsAffine() )
      n.Normalize();
  }
  else {  
    ::SetVecM( n, axisX, axisY );
    n.Normalize();
  }
}


//------------------------------------------------------------------------------
// \ru Дать нормаль \en Give the normal 
// ---
inline MbVector3D MbPlacement3D::Normal() const
{
  MbVector3D n;
  Normal( n );
  return n;
}


//------------------------------------------------------------------------------
// \ru Проверка на равенство \en Check for equality 
// ---
inline bool MbPlacement3D::operator == ( const MbPlacement3D & with ) const
{
  return ( ( origin == with.origin ) &&
           ( axisZ  == with.axisZ  ) &&
           ( axisY  == with.axisY  ) &&
           ( axisX  == with.axisX  ) );
}


//------------------------------------------------------------------------------
// \ru Проверка на неравенство \en Check for inequality 
// ---
inline bool MbPlacement3D::operator != ( const MbPlacement3D & with ) const {
  return !( *this == with );
}


//------------------------------------------------------------------------------
// \ru Оператор присвоения \en Assignment operator 
// ---
inline void MbPlacement3D::operator  = ( const MbPlacement3D & pl )
{
  origin = pl.origin;
  axisZ  = pl.axisZ;
  axisY  = pl.axisY;
  axisX  = pl.axisX;
  flag   = pl.flag;
}


//------------------------------------------------------------------------------
// \ru Преобразование 2D-точки в другую 2D-точку \en Transformation of 2D-point to another 2D-point 
// ---
inline void MbPlacement3D::TransformPoint( const MbMatrix3D & matr, MbCartPoint & p2d ) const
{
  MbCartPoint3D p3d( origin );
  p3d.Add( axisX, p2d.x, axisY, p2d.y );
  p3d.Transform( matr );
  p2d.Init( p3d.x, p3d.y );
}


//-------------------------------------------------------------------------------
// \ru Является ли объект смещением данного? \en Check if the object is a shift of the current one. 
// ---
inline bool MbPlacement3D::IsShift( const MbPlacement3D & other, MbVector3D & vect, bool & isSame, double accuracy ) const
{
  if ( axisZ.IsSame(other.axisZ, accuracy) &&
       axisY.IsSame(other.axisY, accuracy) &&
       axisX.IsSame(other.axisX, accuracy) ) {
    vect.Init( origin, other.origin );
    isSame = ( vect.MaxFactor() < accuracy );
    return true;
  }
  return false;
}


//-------------------------------------------------------------------------------
// \ru Является ли объект смещением данного вдоль оси Z? \en Check if the object is a shift of the current one along axis Z?
// ---
inline bool MbPlacement3D::IsShiftZ( const MbPlacement3D & other, MbVector3D & vect, double accuracy ) const
{
  if ( axisZ.IsSame(other.axisZ, accuracy) &&
       axisY.IsSame(other.axisY, accuracy) &&
       axisX.IsSame(other.axisX, accuracy) ) {
    vect.Init( origin, other.origin );
    if ( axisZ.RoundColinear(vect, accuracy) )
      return true;
  }
  return false;
}


//-------------------------------------------------------------------------------
/** \brief \ru Функция перевода координат из декартовой системы в цилиндрическую
           \en Function for transforming coordinates from the Cartesian coordinate system to a cylindrical coordinate system \~
  \details \ru Функция перевода координат из декартовой системы в цилиндрическую
           \en Function for transforming coordinates from Cartesian coordinate system to a cylindrical coordinate system \~
  \param[in, out] x, y, z - \ru Исходный координаты.
                            \en Source coordinates. \~
  \ingroup Mathematic_Base_3D
*/
//---
MATH_FUNC (void) CartesianToCylindrical( double & x, double & y, double & z );


//-------------------------------------------------------------------------------------
/** \brief \ru Функция перевода координат из цилиндрической системы в декартову
           \en Function for transforming coordinates from a cylindrical coordinate system to a Cartesian coordinate system \~
  \details \ru Функция перевода координат из цилиндрической системы в декартову
           \en Function for transforming coordinates from a cylindrical coordinate system to a Cartesian coordinate system \~
  \param[in, out] \ru x, y, z - Исходный координаты.
             out] \en x, y, z - Source coordinates. \~
  \ingroup Mathematic_Base_3D
*/
//---
MATH_FUNC (void) CylindricalToCartesian( double & x, double & y, double & z );


//-------------------------------------------------------------------------------------
/** \brief \ru Функция перевода координат из цилиндрической системы в сферическую
           \en Function for transforming coordinates from a cylindrical coordinate system to a spherical coordinate system \~
  \details \ru Функция перевода координат из цилиндрической системы в сферическую
           \en Function for transforming coordinates from a cylindrical coordinate system to a spherical coordinate system \~
  \param[in, out] \ru x, y, z - Исходный координаты.
             out] \en x, y, z - Source coordinates. \~
  \ingroup Mathematic_Base_3D
*/
//---
MATH_FUNC (void) CylindricalToSpherical( double & x, double & y, double & z );


//-------------------------------------------------------------------------------------
/** \brief \ru Функция перевода координат из декартовой системы в сферическую
           \en Function for transforming coordinates from a Cartesian coordinate system to a spherical coordinate system \~
  \details \ru Функция перевода координат из декартовой системы в сферическую
           \en Function for transforming coordinates from a Cartesian coordinate system to a spherical coordinate system \~
  \param[in, out] \ru x, y, z - Исходный координаты.
             out] \en x, y, z - Source coordinates. \~
  \ingroup Mathematic_Base_3D
*/
//---
MATH_FUNC (void) CartesianToSpherical  ( double & x, double & y, double & z );


//-------------------------------------------------------------------------------------
/** \brief \ru Функция перевода координат из сферической системы в декартову
           \en Function for transforming coordinates from a spherical coordinate system to a Cartesian coordinate system \~
  \details \ru Функция перевода координат из сферической системы в декартову
           \en Function for transforming coordinates from a spherical coordinate system to a Cartesian coordinate system \~
  \param[in, out] \ru x, y, z - Исходный координаты.
             out] \en x, y, z - Source coordinates. \~
  \ingroup Mathematic_Base_3D
*/
//---
MATH_FUNC (void) SphericalToCartesian  ( double & x, double & y, double & z );


//-------------------------------------------------------------------------------------
/** \brief \ru Функция перевода координат из сферической системы в цилиндрическую
           \en Function for transforming coordinates from a spherical coordinate system to a cylindrical coordinate system \~
  \details \ru Функция перевода координат из сферической системы в цилиндрическую
           \en Function for transforming coordinates from a spherical coordinate system to a cylindrical coordinate system \~
  \param[in, out] \ru x, y, z - Исходный координаты.
             out] \en x, y, z - Source coordinates. \~
  \ingroup Mathematic_Base_3D
*/
//---
MATH_FUNC (void) SphericalToCylindrical( double & x, double & y, double & z );


#endif // __MB_PLACEMENT3D_H
