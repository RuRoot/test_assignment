////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Расширенная матрица преобразования в трёхмерном пространстве.
         \en The extended matrix of transformation in a three-dimensional space. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MB_MATRIX3D_H
#define __MB_MATRIX3D_H


#include <mb_cart_point3d.h>
#include <mb_homogeneous3d.h>


constexpr size_t MATRIX_DIM_3D = 4; // \ru Размер матрицы \en A matrix size
constexpr size_t AXIS_0X       = 0; // \ru Ось 0X \en 0X-axis
constexpr size_t AXIS_0Y       = 1; // \ru Ось 0Y \en 0Y-axis
constexpr size_t AXIS_0Z       = 2; // \ru Ось 0Z \en 0Z-axis


class   MbMatrix;
class   MbAxis3D;
class   MbPlacement3D;
class   MbHomogeneous3D;
class   MbVector3D;
class   MbCartPoint3D;
class   MbProperties;


typedef double GLdouble;

template<class Transform>
void CheckOrigin3D  ( const Transform & trans, uint8 & flag, bool resetFlag );
template<class Transform>
void CheckRotation3D( const Transform & trans, uint8 & flag, bool resetFlag );
template<class Transform>
void CheckAffine3D  ( const Transform & trans, uint8 & flag );


//------------------------------------------------------------------------------
/** \brief \ru Расширенная матрица преобразования в трёхмерном пространстве.
           \en The extended matrix of transformation in a three-dimensional space. \~
  \details \ru Расширенная матрица преобразования в трёхмерном пространстве. \n
    Расширенная матрица имеет размерность (4, 4) и представляет собой обычную матрицу, 
    окаймлённую снизу вектором сдвига а справа нулевым столбцом. 
    Трансформация точки p по матрице M имеет вид: r = p * M (строка координат умножается на матрицу слева).\n
    Матрица преобразования из локальной системы координат может быть получена следующим образом:\n
    первая строка матрицы должна быть заполнена соответствующими компонентами первого вектора локальной системы,
    вторая строка матрицы должна быть заполнена соответствующими компонентами второго вектора локальной системы,
    третья строка матрицы должна быть заполнена соответствующими компонентами третьего вектора локальной системы,
    последняя строка матрицы должна быть заполнена соответствующими координатами положения начала локальной системы.
    Матрица трансформации состоит из четырёх блоков:\n  
    | R, P | \n
    | V, S | \n
    , где R - матрица вращения, тензор размерностью (3, 3),
    V - вектор сдвига, тензор (0, 3),
    P - вектор перспективы, тензор (3, 0) (всегда ноль),
    S - скаляр масштабного преобразования (единица).\n 
    Для ускорения вычислений матрица имеет дополнительные данные - флаг состояния.\n 
    Для получения данных матрицы извне следует пользоваться методами Get...\n 
    Для модификации данных матрицы извне следует пользоваться методами Set..., которые автоматически сбрасывают флаг системы в неустановленное состояние.\n
           \en The extended matrix of transformation in a three-dimensional space. \n
    The extended matrix has dimension (4, 4) and is a regular matrix 
    bordered below by a translation vector and by the null column to the right. 
    The transformation of point "p" by the matrix "M" has the form: r = p * M (the row of coordinates is multiplied by the matrix to the left).\n
    The transformation matrix from a local coordinate system can be obtained as follows:\n
    the first row must be filled with the corresponding components of the first vector of a locale system,
    the second row must be filled with the corresponding components of the second vector of a local system,
    the third row must be filled with the corresponding components of the third vector of a locale system,
    the last row of the matrix must be filled with the corresponding coordinates of a local system origin.
    The matrix of transformation consists of four blocks:\n  
    | R, P | \n
    | V, S | \n
    where R - is a rotation matrix, the tensor with dimension (3, 3),
    V - translation vector, tensor (0, 3),
    P - perspective vector, tensor (3, 0) (always is null)
    S - scalar of scale transformation (the unit). \ n 
    To speed up the calculation the matrix has additional data - flag of state.\n 
    Use "Get..." methods to get data of matrix from the outside. \n 
    To modify data of a matrix from the outside use "Set..." methods which automatically reset the system flag to unspecified state. \n \~
  \ingroup Mathematic_Base_3D
*/
// ---
class MATH_CLASS MbMatrix3D {
private:
  double el[MATRIX_DIM_3D][MATRIX_DIM_3D]; ///< \ru Элементы матрицы. \en Elements of matrix. 
private:
  /**
    \brief \ru Состояние матрицы определяется установкой битовых полей.
           \en State of the matrix is defined by setting of bit fields. \~
    \details \ru Состояние матрицы определяется установкой битовых полей: \n
      MB_TRANSLATION - вектор трансляции не ноль, \n
      MB_ROTATION    - матрица вращения не единичная, \n
      MB_SCALING     - масштабный компонент не 1.0, \n
      MB_REFLECTION  - детерминант матрицы вращения отрицателен, \n
      MB_ORTOGONAL   - матрица вращения ортогональная, взводится только в случае аффинной системы координат, \n
      MB_AFFINE      - матрица вращения произвольная аффинная, \n
      MB_PERSPECTIVE - присутствует перспективное преобразование (не нулевой вектор перспективы), \n
      MB_UNSET       - битовые флаги не установлены. \n
      При изменении элементов матрицы (el[..][..]) flag должен быть сброшен в неустановленное состояние MB_UNSET, 
    при котором происходит полный пересчет состояния матрицы по требованию, для оптимизации функционала матрицы 
    рекомендуется устанавливать его в ручную соответственно тому как изменилось содержание el[..][..]. \n
      При модификации извне пользоваться методами Set* которые автоматически сбрасыват флаг в неустановленное состояние
    при получении данных матрицы из вне пользоваться методами Get*, 
    НЕ ПОЛЬЗОВАТСЯ МЕТОДАМИ Get* ДЛЯ ИЗМЕННИЯ ЭЛЕМЕНТОВ МАТРИЦЫ ПЕРЕИМЕНОВЫВАЯ const В НЕ const, 
    НЕ ДОСТУПАТЬСЯ К ДАННЫМ МАТРИЦЫ НАПРЯМУЮ В ОБХОД Get* и Set*.
             \en State of the matrix is defined by setting of bit fields: \n
      MB_TRANSLATION - translation vector is not zero \n
      MB_ROTATION    - rotation matrix is not unit \n
      MB_SCALING     - scale component is not 1.0, \n
      MB_REFLECTION  - determinant of the rotation matrix is negative, \n
      MB_ORTOGONAL   - orthogonal matrix of rotation, it is used only if coordinate system is affine \n
      MB_AFFINE      - arbitrary affine rotation matrix, \n
      MB_PERSPECTIVE - is perspective transformation (non-zero perspective vector), \n
      MB_UNSET       - bit flags are not set. \n
      If matrix elements (el[..][..]) are changed, then 'flag' must be set to unspecified MB_UNSET state, 
    for which the full recalculation of the matrix state is performed (on request) to optimize the functional of the matrix. 
    It is recommended to set it manually according to how the content of el [..] [..] is changed. \n
      To modify data of a matrix from the outside use "Set..." methods which automatically reset the flag to unspecified state,
    use "Get..." methods to get data of matrix from the outside, 
    DO NOT USE "Get..." METHODS TO MODIFY MATRIX ELEMENTS BY RENAMING const TO non-const, 
    USE ONLY Get* AND Set* TO ACCESS AND MODIFY OF MATRIX DATA. \~
  */
  mutable uint8 flag;

public: 
  static const MbMatrix3D identity; ///< \ru Единичная матрица, I = diag(1,1,1,1); \en Identity matrix, I = diag(1,1,1,1); 
  
public:
  /// \ru Конструктор по умолчанию, определяет единичную матрицу. \en Default constructor. Defines the identity matrix.
  MbMatrix3D() : flag( MB_UNSET ) { Init(); }
  /// \ru Конструктор копирования. \en The copy constructor. 
  MbMatrix3D( const MbMatrix3D & init ) : flag( init.flag ) { ::memcpy( el, init.el, sizeof(el) ); }
  /// \ru Конструктор по локальной системе координат. \en The constructor by placement. 
  explicit MbMatrix3D( const MbPlacement3D & place ) : flag( MB_UNSET ) { Init( place ); }

public:
  /// \ru Конструктор по двумерной матрице. \en The constructor by a two-dimensional matrix. 
  explicit MbMatrix3D( const MbMatrix & m ) : flag( MB_UNSET ) { Init( m ); }
  /**
    \brief \ru Конструктор по двум матрицам.
           \en The constructor by two matrices. \~
    \details \ru Матрица определяется как произведение двух исходных: C = (B * A).
             \en Matrix is defined as the product of two initial matrices: C = (B * A). \~
    \param[in] A, B - \ru Исходные матрицы.
                      \en Initial matrices. \~
  */
  explicit MbMatrix3D( const MbMatrix3D & A, const MbMatrix3D & B ) : flag( MB_UNSET ) { Init( A, B );  } 
public:
  /// \ru Деструктор. \en Destructor. 
  ~MbMatrix3D();

public: 
  /** 
    \ru \name Функции инициализации.
    \en \name Initialization functions.
    \{ */
  /// \ru Инициализировать матрицу как единичную. \en Initialize a matrix as unit one. 
  MbMatrix3D & Init(); 
  /// \ru Инициализировать элементами другой матрицы. \en Initialize by elements of another matrix. 
  MbMatrix3D & Init( const MbMatrix3D & );
  /// \ru Инициализировать двумерной матрицей. \en Initialize by a two-dimensional matrix. 
  MbMatrix3D & Init( const MbMatrix & );
  /// \ru Инициализировать плейсментом. \en Initialize by a placement. 
  MbMatrix3D & Init( const MbPlacement3D & );
  /**
    \brief \ru Инициализировать произведением заданных матриц.
           \en Initialize by the product of given matrices. \~
    \details \ru Инициализировать произведением заданных матриц. this = b * a (!= a * b)!!!.
             \en Initialize by the product of given matrices. this = b * a (!= a * b)!!!. \~
    \param[in] a, b - \ru Исходные матрицы.
                      \en Initial matrices. \~
  */
  MbMatrix3D & Init( const MbMatrix3D & a, const MbMatrix3D & b );
  /** \} */

public:
  /** 
    \ru \name Функции проверки свойств матриц.
    \en \name Functions for check of matrices properties.
    \{ */
  /// \ru Выдать признак отрицательности детерминанта матрицы вращения. \en Get an attribute of negativity of the determinant of a rotation matrix. 
  bool        IsInvert     () const { return IsReflection(); }
  /// \ru Выдать признак единичности матрицы. \en Get an attribute of unit matrix. 
  bool        IsSingle     () const { return (MB_IDENTITY == CheckFlag()); }
  /// \ru Выдать признак единичности матрицы с заданной точностью. \en Get an attribute of the identity matrix with a given tolerance. 
  bool        IsSingleEps  ( double eps = PARAM_EPSILON ) const;
  
  /// \ru Выдать признак не равенства нулю вектора трансляции. \en Get an attribute of inequality to zero of translation vector. 
  bool        IsTranslation() const { return !!( CheckFlag() & MB_TRANSLATION ); }
  /// \ru Выдать признак не единичности матрицы вращения. \en Get an attribute of non-identity of a rotation matrix. 
  bool        IsRotation   () const { return !!( CheckFlag() & MB_ROTATION    ); }
  /// \ru Выдать признак отрицательности детерминанта матрицы вращения. \en Get an attribute of negativity of the determinant of a rotation matrix. 
  bool        IsReflection () const { return !!( CheckFlag() & MB_REFLECTION  ); }
  /// \ru Выдать признак лево-определенной матрицы. \en Get an attribute of the left-definite matrix. 
  bool        IsLeft       () const { return !!( CheckFlag() & MB_LEFT        ); }
  /// \ru Выдать признак ортогональности для случая аффинной матрицы. \en Get an attribute of orthogonality for the case if the matrix is affine. 
  bool        IsOrt        () const { return !!( CheckFlag() & MB_ORTOGONAL   ); }
  /// \ru Выдать признак ортогональности для матрицы вращения. \en Get an attribute of orthogonality for the case if the matrix is a rotation matrix. 
  bool        IsOrthogonal () const { CheckFlag(); return ( !(flag & MB_AFFINE) || !!(flag & MB_ORTOGONAL) ); }
  /// \ru Выдать признак того, что матрица вращения произвольная аффинная. \en Get an attribute that the rotational matrix is arbitrary and affine. 
  bool        IsAffine     () const { return !!( CheckFlag() & MB_AFFINE      ); }

  /// \ru Выдать признак не равенства 1.0 масштабного компонента. \en Get an attribute of inequality to 1.0 of a scale component. 
  bool        IsScaling    () const { return !!( CheckFlag() & MB_SCALING     ); }
  ///< \ru Выдать признак не равенства нулю вектора перспективы. \en Get an attribute of inequality to zero of perspective vector. 
  bool        IsPerspective() const { return !!( CheckFlag() & MB_PERSPECTIVE ); }
  /// \ru Проверить, что битовые флаги не установлены. \en Check whether bit flags are not set. 
  bool        IsUnSet      () const { return !!( flag & MB_UNSET ); }

  /// \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        IsSame( const MbMatrix3D &, double accuracy ) const;
  /// \ru Выдать признак идентичности матриц. \en Get an attribute of unit matrices. 
  bool        IsAbsIdentical( const MbMatrix3D & m ) const;

  /// \ru Выдать признак, что есть только перенос. \en Get an attribute that there is only translation. 
  bool        IsTranslationOnly() const;
  /// \ru Совпадают ли оси описываемой матрицей ЛСК с глобальными осями с точностью до поворотов на 90 градусов? \en Are the axis described by the matrix local system coincide with global axes up to a 90-degree rotation?
  bool        IsSubstitutionOnly( double epsilon = METRIC_EPSILON ) const;
  
  /**
    \brief \ru Выдать признак изотропности.
           \en Get an attribute of isotropism. \~
    \details \ru Выполняется проверка, является ли матрица ортогональной с равными по длине 
    осями X, Y и Z (круг остается кругом).
             \en Checks whether the matrix is ??orthogonal with equal lengths of  
    X, Y and Z axes (circle remains a circle) \~
    \param[out] l - \ru Длина ортов осей X, Y и Z.
                    \en The length of the X, Y and Z axes orts. \~
    \return \ru true, если матрица изотропна.
            \en Returns true if the matrix is isotropic. \~
  */
  bool        IsIsotropic( double & l ) const;

  /** \} */
public:
  /** 
    \ru \name Функции доступа к элементам матрицы.
    \en \name Functions of access to matrix elements.
    \{ */

  /// \ru Дать элемент матрицы. \en Get an element of the matrix. 
        double   El( size_t i, size_t j ) const { return el[i][j]; }
  /// \ru Дать указатель на первый элемент матрицы. \en Get a pointer to the first matrix element. 
  const double * GetEl() const { return (const double *)el;            }

  /// \ru Дать первую строку (ось X). \en Get the first row (the X-axis). 
  const MbVector3D &    GetAxisX()  const { return (const MbVector3D    &)*el[0]; }
  /// \ru Дать вторую строку (ось Y). \en Get the second row (the Y-axis). 
  const MbVector3D &    GetAxisY()  const { return (const MbVector3D    &)*el[1]; }
  /// \ru Дать третью строку (ось Z). \en Get the third row (the Z-axis). 
  const MbVector3D &    GetAxisZ()  const { return (const MbVector3D    &)*el[2]; }
  /// \ru Дать четвертую строку (начало системы координат). \en Get the fourth row (the origin of coordinates) 
  const MbCartPoint3D & GetOrigin() const { return (const MbCartPoint3D &)*el[3]; }

  /// \ru Выдать i-ый вектор-столбец матрицы. \en Get the i-th column vector of the matrix. 
  MbVector3D      GetColumn    ( size_t i ) const { return MbVector3D( el[0][i], el[1][i], el[2][i] ); }
  /// \ru Выдать i-ую вектор-строку матрицы. \en Get the i-th row-vector of the matrix. 
  MbVector3D      GetRow       ( size_t i ) const { return MbVector3D( el[i][0], el[i][1], el[i][2] ); }
  /// \ru Выдать i-ый вектор-столбец матрицы. \en Get the i-th column vector of the matrix. 
  MbHomogeneous3D GetFullColumn( size_t i ) const { return MbHomogeneous3D( el[0][i], el[1][i], el[2][i], el[3][i] ); }
  /// \ru Выдать i-ую вектор-строку матрицы. \en Get the i-th row-vector of the matrix. 
  MbHomogeneous3D GetFullRow   ( size_t i ) const { return MbHomogeneous3D( el[i][0], el[i][1], el[i][2], el[i][3] ); }
  /// \ru Выдать компоненты сдвига матрицы. \en Get components of matrix translation.  
  void            GetOffset    ( MbCartPoint3D & p ) const { p = GetOrigin(); }

  /** \} */
public: 
  /** 
    \ru \name Функции модификации элементов матрицы.
    \en \name Functions for matrix elements modification.
    \{ */

  /**
    \brief \ru Присвоить значение элементам матрицы.
           \en Assign a value to matrix elements. \~
    \details \ru Элементы матрицы инициализируются элементами исходного массива.
             \en Matrix elements are initialized by elements of initial array. \~
    \param[in] _el - \ru Исходный массив.
                     \en Initial array. \~
  */
  void        SetEl( const double * _el )           { flag = MB_UNSET; ::memcpy( el, _el, sizeof(el) ); }
  /**
    \brief \ru Дать массив элементов матрицы.
           \en Get an array of matrix elements. \~
    \details \ru ТОЛЬКО ДЛЯ OGL !!!
             \en ONLY FOR OGL !!! \~
    \return \ru Указатель на начало массива элементов матрицы.
            \en The pointer to the array of the matrix elements. \~
  */
  GLdouble  * SetEl(                    )           { flag = MB_UNSET; return (GLdouble *)el; }
  /**
    \brief \ru Присвоить значение элементу матрицы.
           \en Assign a value to the matrix element. \~
    \details \ru Присвоить значение элементу матрицы.
             \en Assign a value to the matrix element. \~
    \param[in] i - \ru Индекс строки.
                   \en A row index. \~
    \param[in] j - \ru Индекс столбца.
                   \en A column index. \~
    \param[in] e - \ru Исходное значение, которое надо присвоить элементу матрицы.
                   \en Initial value which to be assigned to an element of the matrix. \~
  */
  void        El   ( size_t i, size_t j, double e ) { flag = MB_UNSET; el[i][j]  = e; }
  /**
    \brief \ru Прибавить число к элементу матрицы.
           \en Add a number to the matrix element. \~
    \details \ru Прибавить число к элементу матрицы.
             \en Add a number to the matrix element. \~
    \param[in] i - \ru Индекс строки.
                   \en A row index. \~
    \param[in] j - \ru Индекс столбца.
                   \en A column index. \~
    \param[in] e - \ru Исходное число, которое надо прибавить к элементу матрицы.
                   \en Initial number which to be added to the matrix element. \~
  */
  void        AddEl( size_t i, size_t j, double e ) { flag = MB_UNSET; el[i][j] += e; }
  /**
    \brief \ru Умножить элемент матрицы на число.
           \en Multiply the matrix element by a number. \~
    \details \ru Умножить элемент матрицы на число.
             \en Multiply the matrix element by a number. \~
    \param[in] i - \ru Индекс строки.
                   \en A row index. \~
    \param[in] j - \ru Индекс столбца.
                   \en A column index. \~
    \param[in] e - \ru Исходное число, на которое надо умножить элемент матрицы.
                   \en Initial number by which to multiply the matrix element. \~
  */
  void        MulEl( size_t i, size_t j, double e ) { flag = MB_UNSET; el[i][j] *= e; }

  /// \ru Дать первую строку (ось X). \en Get the first row (the X-axis). 
  MbVector3D &    SetAxisX () { flag = MB_UNSET; return (MbVector3D    &)*el[0]; }
  /// \ru Дать вторую строку (ось Y). \en Get the second row (the Y-axis). 
  MbVector3D &    SetAxisY () { flag = MB_UNSET; return (MbVector3D    &)*el[1]; }
  /// \ru Дать третью строку (ось Z). \en Get the third row (the Z-axis). 
  MbVector3D &    SetAxisZ () { flag = MB_UNSET; return (MbVector3D    &)*el[2]; }
  /// \ru Дать четвертую строку (начало системы координат). \en Give the fourth row (the origin of coordinates) 
  MbCartPoint3D & SetOrigin() { flag = MB_UNSET; return (MbCartPoint3D &)*el[3]; }

  /// \ru Установить Z компоненту начала координат. \en Set Z component of the origin. 
  void        SetOriginZ( double or_z ) { el[3][2] = or_z; CheckOrigin(); }

  /// \ru Установить значения в i-ый столбец матрицы. \en Set values in the i-th column of the matrix. 
  void        SetColumn( size_t i, const MbCartPoint3D & );
  /// \ru Установить значения в i-ый столбец матрицы. \en Set values in the i-th column of the matrix. 
  void        SetColumn( size_t i, const MbVector3D    & );
  /// \ru Установить значения в i-ый столбец матрицы. \en Set values in the i-th column of the matrix. 
  void        SetColumn( size_t i, const MbHomogeneous3D & );
  /// \ru Установить значения в i-ую строку матрицы. \en Set values in the i-th row of the matrix. 
  void        SetRow   ( size_t i, const MbCartPoint3D & );
  /// \ru Установить значения в i-ую строку матрицы. \en Set values in the i-th row of the matrix. 
  void        SetRow   ( size_t i, const MbVector3D & );
  /// \ru Установить значения в i-ую строку матрицы. \en Set values in the i-th row of the matrix. 
  void        SetRow   ( size_t i, const MbHomogeneous3D & );
  /// \ru Установить компоненты сдвига матрицы. \en Set components of matrix translation.  
  void        SetOffset( const MbCartPoint3D & p );
  /**
    \brief \ru Установить флаг состояния.
           \en Set the flag of state. \~
    \details \ru Установить флаг состояния.
             \en Set the flag of state. \~
    \param[in] bReflection - \ru Флаг отрицательности детерминанта матрицы вращения.
                             \en Negativity flag of the rotation matrix determinant. \~
    \param[in] bAffine - \ru Флаг аффинности.
                         \en Affinity flag. \~
    \param[in] bOrt - \ru Флаг ортогональности.
                      \en Orthogonality flag. \~
  */
  void        SetFlag  ( bool bReflection, bool bAffine = false, bool bOrt = true ) const;

  /** \} */
public: 
  /** 
    \ru \name Функции умножения матриц.
    \en \name Matrices multiplication.
    \{ */

  /// \ru Умножить на матрицу: this = this * b; \en Multiply by a matrix: this = this * b; 
  void        Multiply( const MbMatrix3D & b );
  /// \ru Перемножить матрицы this = a * b. \en Multiply matrices this = a * b. 
  void        Multiply( const MbMatrix3D & a, const MbMatrix3D & b ) { Init( b, a ); }

  /** \} */
public:
  /** 
    \ru \name Функции масштабирования.
    \en \name Scaling functions.
    \{ */
 
  /**
    \brief \ru Масштабировать по X, Y, Z.
           \en Scale by X, Y and Z. \~
    \details \ru Масштабировать по X, Y, Z.
             \en Scale by X, Y and Z. \~
    \param[in] sx, sy, sz - \ru Коэффициенты масштабирования для каждой из осей.
                            \en Scaling coefficients for each axis. \~
  */
  void        Scale( double sx, double sy, double sz );
  /// \ru Масштабировать по X, Y, Z. \en Scale by X, Y and Z. 
  void        Scale ( double s );
  /// \ru Масштабировать по X. \en Scale by X. 
  void        ScaleX( double s );
  /// \ru Масштабировать по Y. \en Scale by Y. 
  void        ScaleY( double s );
  /// \ru Масштабировать по Z. \en Scale by Z. 
  void        ScaleZ( double s );
  /// \ru Масштабировать по X и Y и Z без сдвига. \en Scale by X, Y and Z without translation. 
  void        ScaleAxes( double s );

  /// \ru Выдать коэффициент масштабирования по X. \en Get scaling coefficient by X. 
  double      GetScaleX() const;
  /// \ru Выдать коэффициент масштабирования по Y \en Get scaling coefficient by Y 
  double      GetScaleY() const;
  /// \ru Выдать коэффициент масштабирования по Z \en Get scaling coefficient by Z 
  double      GetScaleZ() const;

  /** \} */
public:
  /** 
    \ru \name Функции преобразований матрицы: сдвиг, поворот и т.д.
    \en \name Functions of matrix transformations: translation, rotation, etc.
    \{ */

  /// \ru Сдвинуть на заданный вектор. \en Translate by a given vector. 
  void        Move( const MbVector3D & to ) { Move( to.x, to.y, to.z );  }
  /// \ru Сдвинуть на заданные приращения. \en Translate by given increments. 
  void        Move( double dx, double dy, double dz );
  /// \ru Сдвинуть в нуль. \en Translate to null. 
  void        MoveZero();

  /**
    \brief \ru Повернуть вокруг оси.
           \en Rotate around an axis. \~
    \details \ru Повернуть вокруг оси, проходящей через центр координат в направлении axisDir.
             \en Rotate around an axis which passes through the origin in the axisDir direction. \~
    \param[in] axisDir - \ru Вектор, задающий направление поворота.
                         \en A vector which defines the direction of rotation. \~
    \param[in] angle - \ru Угол поворота.
                       \en A rotation angle. \~
    \return \ru Матрица возвращает ссылку на себя.
    \en Returns the reference to this matrix. \~    

  */
  MbMatrix3D & RotateAbout( const MbVector3D & axisDir, double angle ); 
  /// \ru Повернуть вокруг заданной оси на заданный угол. \en Rotate around axis by angle. 
  MbMatrix3D & Rotate( const MbAxis3D & axis, double angle ); 
  /**
    \brief \ru Повернуть вокруг оси.
           \en Rotate around an axis. \~
    \details \ru Повернуть вокруг оси X, Y, Z.
             \en Rotate around the X, Y, or Z axis. \~
    \param[in] axis - \ru Номер оси, т.е. AXIS_0X, AXIS_0Y или AXIS_0Z.
                      \en The number of axis, i.e. AXIS_0X, AXIS_0Y or AXIS_0Z. \~
    \param[in] angle - \ru Угол поворота.
                       \en A rotation angle. \~
    \return \ru Матрица возвращает ссылку на себя.
    \en Returns the reference to this matrix. \~
  */
  MbMatrix3D & Rotate( int axis, double angle );

  /**
    \brief \ru Преобразовать длину.
           \en Transform the length. \~
    \details \ru Длина преобразуются согласно матрице (*this).
             \en The length is transformed according to a matrix (*this). \~
    \param[in, out] len - \ru Длина.
                          \en A length. \~
    \param[in] axis - \ru Ось, по которой отмеряется длина.
                      \en An axis on which the length is measured. \~
  */
  void        TransformLength( double & len, int axis = 1 ) const;
  /**
    \brief \ru Преобразовать координаты.
           \en Transform coordinates. \~
    \details \ru Координаты преобразуются согласно матрице. Преобразовываются две координаты (z = 0).
             \en Coordinates are transformed according to a matrix. Two coordinates are transformed (z=0). \~
    \param[in, out] x, y - \ru Координаты по x и y.
                           \en Coordinates by x and y. \~
  */
  void        TransformCoord2D( double & x, double & y ) const;

  /// \ru Преобразовать согласно заданной матрице. \en Transform according to the given matrix. 
  MbMatrix3D & Transform( const MbMatrix3D & matr ) { *this = ( *this ) * matr; return *this; } 
  /// \ru Инвертировать ось. \en Invert the axis. 
  void        Invert( size_t n );

  /**
    \brief \ru Задать матрицу преобразования симметрии (отражение).
           \en Set the matrix to symmetry transformation (reflection). \~
    \details \ru Находится матрица для преобразования симметрии относительно плоскости, заданной точкой и нормалью.
             \en The matrix of symmetry transformation is found relative to the plane which is defined by a point and normal. \~
    \param[in] origin - \ru Точка плоскости.
                        \en A point of plane. \~
    \param[in] normal - \ru Нормаль плоскости.
                        \en The normal of a plane. \~
    \return \ru Матрица возвращает ссылку на себя.
            \en Returns the reference to this matrix. \~    
  */
  MbMatrix3D &  Symmetry( const MbCartPoint3D & origin, const MbVector3D & normal );
  /**
    \brief \ru Задать матрицу преобразования симметрии (отражение).
           \en Set the matrix to symmetry transformation (reflection). \~
    \details \ru Находится матрица для преобразования симметрии относительно плоскости, 
      заданной точкой и двумя векторами.
             \en The matrix of symmetry transformation is found relative to the plane 
      which is defined by a point and two vectors. \~
    \param[in] origin - \ru Точка плоскости (начало системы координат).
                        \en A point of plane (an origin). \~
    \param[in] vx, vy - \ru Векторы, параллельные плоскости.
                        \en Vectors which parallel for a plane. \~
    \return \ru Матрица возвращает ссылку на себя.
            \en Returns the reference to this matrix. \~    
  */
  MbMatrix3D &  Symmetry( const MbCartPoint3D & origin, const MbVector3D & vx, const MbVector3D & vy );

  /** \} */
public:
  /** 
    \ru \name Расчет алгебраических свойств матрицы.
    \en \name Calculation of the algebraic properties of a matrix.
    \{ */

  /// \ru Транспонировать матрицу. \en Transpose a matrix. 
  void         Adj();
  /// \ru Вычислить алгебраическое дополнение. \en Calculate the algebraic adjunct. 
  double       Delta( size_t line, size_t column, size_t dim ) const;
  /// \ru Вычислить определитель матрицы. \en Calculate the determinant of a matrix. 
  double       Det( size_t dim )      const;
  /**
    \brief \ru Вычислить обратную матрицу. 
           \en Calculate inverse matrix.
    \return \ru Функция вернет аргумент, принятый по ссылке, с результатом вычисления обратной матрицы.
            \en The function returns the argument passed by reference with the result of calculating the inverse matrix. \~
  */
  MbMatrix3D & Div( MbMatrix3D & ) const;

  /** \} */

  /**
    \brief \ru Получить матрицу для масштабирования каверны литейной формы.
           \en Get the matrix for scale of a mold cavern. \~
    \details \ru Получить матрицу для масштабирования каверны литейной формы.
             \en Get the matrix for scale of a mold cavern. \~
    \param[in] fixedPoint - \ru Неподвижная точка.
                            \en Fixed point. \~
    \param[in] deltaX, deltaY, deltaZ - \ru Относительное приращение размера по направлению соответствующей координаты.
                                        \en Relative increment of size in the direction of corresponding coordinate. \~
  */
  void        MouldCavityScale( MbCartPoint3D & fixedPoint, double deltaX, double deltaY, double deltaZ );
  /**
    \brief \ru Округлить с точностью до eps.
           \en Round with eps tolerance. \~
    \details \ru Округлить с точностью до eps.
             \en Round with eps tolerance. \~
    \param[in] total - \ru Если true, то округлять в любом случае.
                       \en If true, round anyway. \~
    \param[in] eps - \ru Точность округления.
                     \en A round-off tolerance. \~
    \return \ru true, если округление было выполнено.
            \en Returns true if round-off has been done. \~
  */
  bool        SetRoundedValue( bool total, double eps );
  /// \ru Нормализовать. \en Normalize. 
  void        Normalize();
  /// \ru Вычислить неподвижную точку преобразования. \en Calculate a fixed point of transformation. 
  bool        CalculateFixedPoint( MbCartPoint3D & fixedPoint ) const;

  /** 
    \ru \name Перегрузка алгебраических и логических операций.
    \en \name Overload of arithmetical and logical operations.
    \{ */
  /// \ru Умножить на матрицу: M = this * m. \en Multiply by a matrix: M = this * m. 
  MbMatrix3D   operator *  ( const MbMatrix3D & m ) const { return MbMatrix3D( m, *this ); }
  /// \ru Умножить на матрицу: this = this * m. \en Multiply by a matrix: this = this * m. 
  MbMatrix3D & operator *= ( const MbMatrix3D & m ) { Multiply( m ); return *this; }
  /// \ru Присвоить значение. \en Assign a value. 
  MbMatrix3D & operator  = ( const MbMatrix3D & m ) { flag = m.flag; ::memcpy( el, m.el, sizeof(el) ); return *this; }
  /// \ru Сложить матрицы. \en Add matrices. 
  void         operator += ( const MbMatrix3D & );
  /// \ru Вычесть из матрицы матрицу. \en Subtract a matrix from the matrix. 
  void         operator -= ( const MbMatrix3D & );
  /// \ru Умножить матрицу на число. \en Multiply the matrix by a number. 
  void         operator *= ( double factor );
  /// \ru Разделить матрицу на число. \en Divide the matrix by a number. 
  void         operator /= ( double factor );
  /// \ru Сравнить с матрицей (точность - LENGTH_EPSILON). \en Compare with a matrix (tolerance - LENGTH_EPSILON). 
  bool         operator == ( const MbMatrix3D & ) const;

  /// \ru Доступ по ссылке к элементу матрицы. \en Access to a matrix element by a reference. 
        double & operator() ( size_t i, size_t j )       { C3D_ASSERT( std_max( i, j ) < MATRIX_DIM_3D ); flag = MB_UNSET; return el[i][j]; }
  /// \ru Значение элемента матрицы. \en The value of a matrix element. 
  const double & operator() ( size_t i, size_t j ) const { C3D_ASSERT( std_max( i, j ) < MATRIX_DIM_3D );                  return el[i][j]; }

  /** \} */

  /// \ru Выдать свойства объекта. \en Get properties of the object. 
  void        GetProperties( MbProperties & );
  /// \ru Записать свойства объекта. \en Set properties of the object. 
  void        SetProperties( const MbProperties & );

private:
  /// \ru Удалить мусор в данных \en Remove trash from data. 
  //void        RemoveInaccuracies();
  /// \ru Выставить флаги. \en Set flags. 
  uint8       ResetFlag() const;
  // Оценить флаги, если оценки не было
  uint8       CheckFlag() const { return IsUnSet() ? ResetFlag() : flag; }
  // Проверить флаг смещения.
  void        CheckOrigin()   const { uint8 val = flag; ::CheckOrigin3D( *this, val, true ); flag = val; }
  /// \ru Проверить флаг вращения. \en Check rotation flag. 
  void        CheckRotation() const { uint8 val = flag; ::CheckRotation3D( *this, val, true ); flag = val; }

  void        CheckScale() const;

public:
  KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( MbMatrix3D, MATH_FUNC_EX ) // \ru Для работы со ссылками и объектами класса \en For working with references and objects of the class. 
  DECLARE_NEW_DELETE_CLASS( MbMatrix3D )
  DECLARE_NEW_DELETE_CLASS_EX( MbMatrix3D )
};


//-------------------------------------------------------------------------------
// Установка флага смещения для матрицы и системы координат.
/**  
  \attention \ru Только для внутреннего использования.
             \en For internal use only. \~
*/
// ---
template<class Transform>
void CheckOrigin3D( const Transform & trans, uint8 & flag, bool resetFlag )
{
  if ( !(flag & MB_UNSET) ) {
    const MbCartPoint3D & pOrigin = trans.GetOrigin();
    const double eps = LENGTH_EPSILON;

    if ( (::fabs(pOrigin.x) > eps) ||
         (::fabs(pOrigin.y) > eps) ||
         (::fabs(pOrigin.z) > eps) )
    {
      flag |= MB_TRANSLATION;
    }
    else if ( resetFlag ) {
      flag &= ~MB_TRANSLATION;
    }
  }
}


//-------------------------------------------------------------------------------
// Установка флага вращения для матрицы и системы координат.
// ( Использование корректно только для ортонормированных СК)
/**  
  \attention \ru Только для внутреннего использования.
             \en For internal use only. \~
*/
// ---
template<class Transform>
void CheckRotation3D( const Transform & trans, uint8 & flag, bool resetFlag )
{
  if ( !(flag & MB_UNSET) ) {
    const MbVector3D & axisX = trans.GetAxisX();
    const MbVector3D & axisY = trans.GetAxisY();
    const MbVector3D & axisZ = trans.GetAxisZ();

    // Барьер нулевых элементов матрицы
    const double eps = EXTENT_EPSILON;
    if ( (::fabs(axisZ.x      ) > eps) ||
         (::fabs(axisZ.y      ) > eps) ||
         (::fabs(axisX.y      ) > eps) ||
         (::fabs(axisX.z      ) > eps) ||
         (::fabs(axisY.x      ) > eps) ||
         (::fabs(axisY.z      ) > eps) ||
         (::fabs(axisZ.z - 1.0) > eps) ||
         (::fabs(axisX.x - 1.0) > eps) ||
         (::fabs(axisY.y - 1.0) > eps) ) 
    {
      flag |= MB_ROTATION;
    }
    else if ( resetFlag ) {
      flag &= ~MB_ROTATION;
      flag &= ~MB_LEFT;
      flag &= ~MB_ORTOGONAL;
      flag &= ~MB_AFFINE;
    }
  }
}


//-------------------------------------------------------------------------------
// Установка флага аффинности для матрицы и системы координат.
// (Использование корректно только для ортонормированных СК)
/**  
  \attention \ru Только для внутреннего использования.
             \en For internal use only. \~
*/
// ---
template<class Transform>
void CheckAffine3D( const Transform & trans, uint8 & flag )
{
  if ( !(flag & MB_UNSET) ) {
    const MbVector3D & vAxisX = trans.GetAxisX();
    const MbVector3D & vAxisY = trans.GetAxisY();
    const MbVector3D & vAxisZ = trans.GetAxisZ();

    const double eps = EXTENT_EPSILON;

    double lnX = vAxisX.Length();
    double lnY = vAxisY.Length();
    double epsX = (eps * lnX);

    if ( ::fabs( vAxisX * vAxisY ) > (epsX * lnY) )
      flag |= MB_AFFINE;
    else {
      double lnZ = vAxisZ.Length();
      if ( ::fabs( vAxisX * vAxisZ ) > (epsX * lnZ) )
        flag |= MB_AFFINE;
      else {
        if ( ::fabs( vAxisY * vAxisZ ) > (eps * lnY * lnZ) )
          flag |= MB_AFFINE;
        else if ( (::fabs(lnX - 1.0) > eps) ||
                  (::fabs(lnY - 1.0) > eps) ||
                  (::fabs(lnZ - 1.0) > eps) )
        {
          flag |= MB_ORTOGONAL; // система ортогональна 
          flag |= MB_AFFINE;
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// \ru Инициализация матрицы. \en Initialization of a matrix. 
// ---
inline MbMatrix3D & MbMatrix3D::Init()
{
             el[0][1] = el[0][2] = el[0][3] = 0.0;
  el[1][0] =            el[1][2] = el[1][3] = 0.0;
  el[2][0] = el[2][1] =            el[2][3] = 0.0;
  el[3][0] = el[3][1] = el[3][2]            = 0.0;
  el[0][0] = el[1][1] = el[2][2] = el[3][3] = 1.0;
  flag = MB_IDENTITY;
  return *this;
}

//-----------------------------------------------------------------------------
// \ru Инициализировать элементами другой матрицы. \en Initialize by elements of another matrix. 
//
inline MbMatrix3D & MbMatrix3D::Init( const MbMatrix3D & init ) 
{ 
  flag = init.flag; ::memcpy( el, init.el, sizeof(el) ); 
  return *this;
}



//-------------------------------------------------------------------------------
// \ru Есть только перенос. \en Only translation.
// ---
inline bool MbMatrix3D::IsTranslationOnly() const
{
  CheckFlag();
  if ( !!(flag & MB_TRANSLATION) && !(flag & MB_ROTATION) && !(flag & MB_LEFT) && !(flag & MB_SCALING) && !(flag & MB_PERSPECTIVE) )
    return true;
  return false;
}


//------------------------------------------------------------------------------
// \ru Проверка на равенство матриц. \en Check for equality of matrices.
// ---
inline bool MbMatrix3D::operator == ( const MbMatrix3D & m ) const
{
  bool bRes = true;
  for ( size_t i = 0; (i < MATRIX_DIM_3D) && bRes; i++ ) {   
    bRes = ( (::fabs(el[i][0] - m.el[i][0]) <= LENGTH_EPSILON) &&
             (::fabs(el[i][1] - m.el[i][1]) <= LENGTH_EPSILON) &&
             (::fabs(el[i][2] - m.el[i][2]) <= LENGTH_EPSILON) &&
             (::fabs(el[i][3] - m.el[i][3]) <= LENGTH_EPSILON) );
  }
  return bRes;
}


//------------------------------------------------------------------------------
// \ru Умножение на матрицу this = this * b; \en Multiplication by a matrix this = this * b; 
// ---
inline void MbMatrix3D::Multiply( const MbMatrix3D & b )
{
  MbMatrix3D staticMatrix; // \ru Для промежуточных вычислений. \en For intermediate calculations 
  staticMatrix.Init( b, *this );
  flag = staticMatrix.flag;
  ::memcpy( el, staticMatrix.el, sizeof(el) );
}


//------------------------------------------------------------------------------
// \ru Преобразовать две координаты (z = 0) \en Transform two coordinates (z=0) 
// ---
inline void MbMatrix3D::TransformCoord2D( double & x, double & y ) const
{
  double xx = x * el[0][0] + y * el[1][0] + el[3][0];
  double yy = x * el[0][1] + y * el[1][1] + el[3][1];
  x = xx;
  y = yy;
}


//------------------------------------------------------------------------------
// \ru Являются ли объекты равными? \en Determine whether an object is equal?
// ---
inline bool MbMatrix3D::IsSame( const MbMatrix3D & m2, double accuracy ) const
{
  const MbMatrix3D & m1 = *this;

  bool isSame = (
    (::fabs(m1.el[0][0] - m2.el[0][0]) <= accuracy) &&
    (::fabs(m1.el[0][1] - m2.el[0][1]) <= accuracy) &&
    (::fabs(m1.el[0][2] - m2.el[0][2]) <= accuracy) &&
    (::fabs(m1.el[0][3] - m2.el[0][3]) <= accuracy) &&

    (::fabs(m1.el[1][0] - m2.el[1][0]) <= accuracy) &&
    (::fabs(m1.el[1][1] - m2.el[1][1]) <= accuracy) &&
    (::fabs(m1.el[1][2] - m2.el[1][2]) <= accuracy) &&
    (::fabs(m1.el[1][3] - m2.el[1][3]) <= accuracy) &&

    (::fabs(m1.el[2][0] - m2.el[2][0]) <= accuracy) &&
    (::fabs(m1.el[2][1] - m2.el[2][1]) <= accuracy) &&
    (::fabs(m1.el[2][2] - m2.el[2][2]) <= accuracy) &&
    (::fabs(m1.el[2][3] - m2.el[2][3]) <= accuracy) &&

    (::fabs(m1.el[3][0] - m2.el[3][0]) <= accuracy) &&
    (::fabs(m1.el[3][1] - m2.el[3][1]) <= accuracy) &&
    (::fabs(m1.el[3][2] - m2.el[3][2]) <= accuracy) &&
    (::fabs(m1.el[3][3] - m2.el[3][3]) <= accuracy) );

  return isSame;
}

//----------------------------------------------------------------------------------------
/** \brief \ru Извлечь углы Эйлера из ротационной подматрицы R = Rx*Ry*Rz.
           \en Extract the Euler angles from the rotational submatrix R = Rx*Ry*Rz.
  \param[in] trans - \ru Матрица преобразования, содержащая подматрицу вращения.
                     \en The transformation matrix containing the rotational sub-matrix. \~
  \param[out] alpha - \ru Угол поворота вокруг оси "X", извлеченный из матрицы вращения.
                      \en Angle of rotation around the "X" axis extracted from the rotation matrix. \~
  \param[out] betta - \ru Угол поворота вокруг оси "Y", извлеченный из матрицы вращения.
                      \en Angle of rotation around the "Y" axis extracted from the rotation matrix. \~
  \param[out] gamma - \ru Угол поворота вокруг оси "Z", извлеченный из матрицы вращения.
                      \en Angle of rotation around the "Z" axis extracted from the rotation matrix. \~

  \details \ru Функция разлагает подматрицу вращения на элементарные повороты вокруг осей R = Rx*Ry*Rz,
  заданные в виде угловых значений, а именно значения в радианах, определяющую присланную матрицу вращения R
  в виде комбинации (произведения) из трех элементарных поворотов: <b>R = Rx*Ry*Rz</b>, где\n
    Rx = Rx(alpha) - поворот вокруг оси "X",  \n
    Ry = Ry(betta) - поворот вокруг оси "Y",  \n
    Rz = Rz(gamma) - поворот вокруг оси "Z" и \n
    R - ротационная подматрица 3x3 из матрицы <b>trans</b>.
  Матрица trans может содержать любые преобразования, включая масштабирование и сдвиг. 
  Метод ExtractEulerAngles извлечет из данной матрицы вращающий компонент и разложит его 
  на три вращения: Rx(alpha), Ry(betta), Rz(gamma).

  \en The function factorizes the rotation submatrix into elementary rotations about the axes: R = Rx * Ry * Rz,
    given in the form of angular values, namely the values in radians, specifying the rotation sub-matrix R of the given <b>trans</b>
    in the form of a combination (product) of three elementary rotations: <b> R = Rx * Ry * Rz </b>, where \n
    Rx = Rx(alpha) - rotation around X-axis,    \n
    Ry = Ry(betta) - rotation around Y-axis,    \n
    Rz = Rz(gamma) - rotation around Z-axis and \n
    R is a rotational 3x3 sub-matrix from the matrix <b>trans</b>.  
  The matrix trans can contain any transformations including the scaling and the shear. 
  The ExtractEulerAngles method extracts from the given matrix a rotating component 
  and decomposes it into three rotations: Rx( alpha ), Ry( betta ), Rz( gamma ).
*/
//---
MATH_FUNC(void) ExtractEulerAngles( const MbMatrix3D & trans, double & alpha, double & betta, double & gamma );


#endif // __MB_MATRIX3D_H
