////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Расширенная матрица преобразования в двумерном пространстве.
         \en The extended matrix of transformation in a two-dimensional space. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MB_MATRIX_H
#define __MB_MATRIX_H


#include <mb_cart_point.h>
#include <io_base.h>
#include <mb_homogeneous.h>


#define MATRIX_DIM_2D  3


class   MbPlacement;
class   MbHomogeneous;


template<class Transform>
void CheckOrigin  ( const Transform & trans, uint8 & flag, bool resetFlag );
template<class Transform>
void CheckRotation( const Transform & trans, uint8 & flag, bool resetFlag );


//------------------------------------------------------------------------------
/** \brief \ru Расширенная матрица преобразования в двумерном пространстве.
           \en The extended matrix of transformation in a two-dimensional space. \~
  \details \ru Расширенная матрица преобразования в двумерном пространстве. \n
    Расширенная матрица имеет размерность (3, 3) и представляет собой обычную матрицу, 
    окаймлённую снизу вектором сдвига а справа нулевым столбцом. 
    Трансформация точки p по матрице M имеет вид: r = p * M (строка координат умножается на матрицу слева).\n 
    Матрица преобразования из локальной системы координат может быть получена следующим образом:\n
    первая строка матрицы должна быть заполнена соответствующими компонентами первого вектора локальной системы,
    вторая строка матрицы должна быть заполнена соответствующими компонентами второго вектора локальной системы,
    последняя строка матрицы должна быть заполнена соответствующими координатами положения начала локальной системы.
    Матрица трансформации состоит из четырёх блоков:\n 
    | R, P |
    | V, S |
    где R - матрица вращения, тензор размерностью (2, 2),
    V - вектор сдвига, тензор (0, 2),
    P - вектор перспективы, тензор (2, 0) (всегда ноль),
    S - скаляр масштабного преобразования (единица).\n
           \en The extended matrix of transformation in a two-dimensional space. \n
    The extended matrix has dimension (3, 3) and is a regular matrix 
    bordered below by a translation vector and by the null column to the right. 
    The transformation of point "p" by the matrix "M" has the form: r = p * M (the row of coordinates is multiplied by the matrix to the left).\n 
    The transformation matrix from a local coordinate system can be obtained as follows:\n
    the first row must be filled with the corresponding components of the first vector of a local system,
    the second row must be filled with the corresponding components of the second vector of a local system,
    the last row of the matrix must be filled with the corresponding coordinates of a local system origin.
    The matrix of transformation consists of four blocks:\n 
    | R, P |
    | V, S |
    where R - is a rotation matrix, the tensor with dimension (2, 2),
    V - translation vector, tensor (0, 2),
    P - perspective vector, the tensor (2, 0) (always null),
    S - scalar of scale transformation (the unit). \ n \~ 
  \ingroup Mathematic_Base_2D
*/
// ---
class MATH_CLASS MbMatrix {
  friend MATH_FUNC (void) MulMatrix( const MbMatrix &, const MbMatrix &, MbMatrix & );
protected:
  double el[MATRIX_DIM_2D][MATRIX_DIM_2D]; ///< \ru Элементы матрицы. \en Elements of matrix. 
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
  static const MbMatrix identity; ///< \ru Единичная матрица, I = diag(1,1,1); \en Identity matrix, I = diag(1,1,1); 

public :
  /// \ru Конструктор по умолчанию. \en Default constructor. 
  MbMatrix() : flag( MB_UNSET ) { Init(); }
  /// \ru Конструктор копирования. \en Copy constructor. 
  MbMatrix( const MbMatrix & init ) : flag( init.flag ) { ::memcpy( &el, init.el, sizeof(el) ); }
  /// \ru Конструктор по локальной системе координат. \en The constructor by placement. 
  explicit MbMatrix( const MbPlacement & place ) : flag( MB_UNSET ) { Set( place ); }
public:
  /**
    \brief \ru Конструктор по точке и нормализованному вектору.
           \en The constructor by a point and a normalized vector \~
    \details \ru Конструктор по точке и нормализованному вектору.
             \en The constructor by a point and a normalized vector \~
    \param[in] pnt - \ru Точка. Задает сдвиг относительно нуля.
                     \en A point. Sets translation relative to zero. \~
    \param[in] dir - \ru Единичный вектор. Задает поворот.
                     \en Unit vector. Sets a rotation. \~
  */
  MbMatrix( const MbCartPoint & pnt, const MbDirection & dir ) : flag( MB_UNSET ) { Init( pnt, dir ); }
  /**
    \brief \ru Конструктор матрицы масштабирования по x и y.
           \en The constructor of scaling matrix by x and y. \~
    \details \ru Конструктор матрицы масштабирования по x и y.
             \en The constructor of scaling matrix by x and y. \~
    \param[in] pnt - \ru Точка. Задает сдвиг относительно нуля.
                     \en A point. Sets translation relative to zero. \~
    \param[in] sx, sy - \ru Коэффициенты масштабирования по x и y соответственно.
                        \en Scaling coefficients by x and y, respectively. \~
  */
  MbMatrix( const MbCartPoint & pc, double sx, double sy ) : flag( MB_UNSET ) { Init( pc, sx, sy ); }
  /**
    \brief \ru Конструктор матрицы поворота.
           \en The constructor of rotation matrix. \~
    \details \ru Конструктор матрицы поворота вокруг точки pc на угол angle.
             \en The constructor of rotation matrix around the point "pc" on the angle "angle". \~
    \param[in] pc - \ru Точка.
                    \en A point. \~
    \param[in] angle - \ru Угол поворота.
                       \en A rotation angle. \~
  */
  MbMatrix( const MbCartPoint & pc, double angle ) : flag( MB_UNSET ) { Init( pc, angle ); } 
public:
  /// \ru Деструктор. \en Destructor. 
 ~MbMatrix();

public :
  /** 
    \ru \name Функции инициализации.
    \en \name Initialization functions.
    \{ */
  /// \ru Инициализировать матрицу как единичную. \en Initialize a matrix as unit one. 
  MbMatrix &  Init();
  /**
    \brief \ru Инициализировать матрицу по точке и нормализованному вектору.
           \en Initialize a matrix by a point and normalized vector. \~
    \details \ru Инициализировать матрицу по точке и нормализованному вектору.
             \en Initialize a matrix by a point and normalized vector. \~
    \param[in] pnt - \ru Точка. Задает сдвиг относительно нуля.
                     \en A point. Sets translation relative to zero. \~
    \param[in] dir - \ru Единичный вектор. Задает поворот.
                     \en Unit vector. Sets a rotation. \~
  */
  MbMatrix &  Init( const MbCartPoint & pnt, const MbDirection & dir ); 
  /**
    \brief \ru Инициализировать матрицу масштабирования по x и y.
           \en Initialize scaling matrix by x and y. \~
    \details \ru Инициализировать матрицу масштабирования по x и y.
             \en Initialize scaling matrix by x and y. \~
    \param[in] pnt - \ru Точка. Задает сдвиг относительно нуля.
                     \en A point. Sets translation relative to zero. \~
    \param[in] sx, sy - \ru Коэффициенты масштабирования по x и y соответственно.
                        \en Scaling coefficients by the x and y, respectively. \~
  */
  MbMatrix &  Init( const MbCartPoint & pc, double sx, double sy );
  /**
    \brief \ru Инициализировать матрицу поворота.
           \en Initialize a rotation matrix. \~
    \details \ru Инициализировать матрицу поворота вокруг точки pc на угол angle.
             \en Initialize a rotation matrix around the point "pc" on the angle "angle". \~
    \param[in] pc - \ru Точка.
                    \en A point. \~
    \param[in] angle - \ru Угол поворота.
                       \en A rotation angle. \~
  */
  MbMatrix &  Init( const MbCartPoint & pc, double angle );
  /**
    \brief \ru Инициализировать матрицу согласно плейсменту.
           \en Initialize a matrix according to a placement. \~
    \details \ru Инициализировать матрицу согласно плейсменту.
             \en Initialize a matrix according to a placement. \~
    \param[in] place - \ru Исходный плейсмент.
                       \en The initial placement. \~
  */
  MbMatrix &  Set ( const MbPlacement & place );
  /// \ru Задать нулевую матрицу. \en Set a zero matrix.
  MbMatrix &  SetZero();

  /** \} */
public:
  /** 
    \ru \name Функции проверки свойств матриц.
    \en \name Functions for check of matrices properties.
    \{ */

  
  /// \ru Выдать признак отрицательности детерминанта матрицы вращения. \en Get an attribute of negativity of the determinant of a rotation matrix. 
  bool        IsInvert() const { return IsLeft(); }
  /// \ru Выдать признак отрицательности детерминанта матрицы вращения. \en Get an attribute of negativity of the determinant of a rotation matrix. 
  bool        IsInvertEps( double eps = EXTENT_EPSILON ) const { return (el[0][0] * el[1][1] - el[0][1] * el[1][0]) < -eps;  }
  /// \ru Выдать признак единичности матрицы. \en Get an attribute of unit matrix. 
  bool        IsSingle() const { return (MB_IDENTITY == CheckFlag()); }
  /// \ru Выдать признак единичности матрицы с заданной точностью. \en Get an attribute of the identity matrix with a given tolerance. 
  bool        IsSingleEps( double eps = LENGTH_EPSILON ) const;

  /// \ru Выдать признак не равенства нулю вектора трансляции. \en Get an attribute of inequality to zero of translation vector. 
  bool        IsTranslation() const { return !!( CheckFlag() & MB_TRANSLATION ); }
  /// \ru Выдать признак не единичности матрицы вращения. \en Get an attribute of non-identity of a rotation matrix. 
  bool        IsRotation   () const { return !!( CheckFlag() & MB_ROTATION    ); }
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
  bool        IsSame( const MbMatrix &, double accuracy ) const;

  /// \ru Выдать признак, что есть только перенос. \en Get an attribute that there is only translation. 
  bool        IsTranslationOnly() const;
  /// \ru Совпадают ли оси описываемой матрицей ЛСК с глобальными осями с точностью до поворотов на 90 градусов? \en Are the axis described by the matrix local system coincide with global axes up to a 90-degree rotation?
  bool        IsSubstitutionOnly( double epsilon = METRIC_EPSILON ) const;
  /**
    \brief \ru Выдать признак изотропности.
           \en Get an attribute of isotropism. \~
    \details \ru Выполняется проверка, является ли матрица ортогональной с равными по длине 
      осями X, Y (круг остается кругом).
             \en Checks whether the matrix is orthogonal with equal lengths of  
      X and Y axes (circle remains a circle). \~
    \param[out] l - \ru Длина ортов осей X, Y.
                    \en The length of the X and Y axes orts. \~
    \return \ru true, если матрица изотропна.
            \en Returns true if the matrix is isotropic. \~
  */
  bool        IsIsotropic( double & l ) const;

  /// \ru Можно ли трансформировать габарит без искажений? \en Is it possible to transform a bounding box without distortion? 
  bool        CanTransformGabarit() const;

  /** \} */
public:
  /** 
    \ru \name Функции доступа к элементам матрицы.
    \en \name Functions of access to matrix elements.
    \{ */

  /// \ru Выдать элемент матрицы. \en Get an element of the matrix. 
        double    El( size_t i, size_t j ) const { return el[i][j]; }
  /// \ru Выдать указатель на первый элемент матрицы. \en Get a pointer to the first matrix element. 
  const double *  GetEl() const { return (const double *)el; }

  /// \ru Выдать первую строку (ось X). \en Get the first row (the X-axis). 
  const MbVector &    GetAxisX() const { return (const MbVector &)*el[0]; }
  /// \ru Выдать вторую строку (ось Y). \en Get the second row (the Y-axis). 
  const MbVector &    GetAxisY() const { return (const MbVector &)*el[1]; }
  /// \ru Выдать третью строку (начало системы координат). \en Get the third row (the origin of coordinates) 
  const MbCartPoint & GetOrigin() const { return (const MbCartPoint &)*el[2]; }

  /// \ru Выдать i-ый вектор-столбец матрицы. \en Get the i-th column vector of the matrix. 
  MbVector      GetColumn    ( size_t i ) const { return MbVector( el[0][i], el[1][i] ); }
  /// \ru Выдать i-ую вектор-строку матрицы. \en Get the i-th row-vector of the matrix. 
  MbVector      GetRow       ( size_t i ) const { return MbVector( el[i][0], el[i][1] ); }
  /// \ru Выдать i-ый вектор-столбец матрицы. \en Get the i-th column vector of the matrix. 
  MbHomogeneous GetFullColumn( size_t i ) const { return MbHomogeneous( el[0][i], el[1][i], el[2][i] ); }
  /// \ru Выдать i-ую вектор-строку матрицы. \en Get the i-th row-vector of the matrix. 
  MbHomogeneous GetFullRow   ( size_t i ) const { return MbHomogeneous( el[i][0], el[i][1], el[i][2] ); }

  /** \} */
public:
  /** 
  \ru \name Функции модификации элементов матрицы.
  \en \name Functions for matrix elements modification.
  \{ */

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
  void        El( size_t i, size_t j, double e ) { flag = MB_UNSET; el[i][j] = e; }
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
  /**
    \brief \ru Присвоить элементам столбца значения координат точки.
           \en Assign point coordinates values to column elements. \~
    \details \ru Присвоить элементам столбца значения координат точки.
             \en Assign point coordinates values to column elements. \~
    \param[in] icol - \ru Индекс столбца.
                      \en A column index. \~
    \param[in] column - \ru Исходная точка.
                        \en The initial point. \~
  */
  void        SetColumn( size_t icol, const MbCartPoint & column );
  /**
    \brief \ru Присвоить элементам столбца значения компонент вектора.
           \en Assign vector components values to column elements. \~
    \details \ru Присвоить элементам столбца значения компонент вектора.
             \en Assign vector components values to column elements. \~
    \param[in] icol - \ru Индекс столбца.
                      \en A column index. \~
    \param[in] column - \ru Исходный вектор.
                        \en The initial vector. \~
  */
  void        SetColumn( size_t icol, const MbVector & column );
  /**
    \brief \ru Присвоить элементам столбца значения координат однородной точки.
           \en Assign coordinates of the uniform point to column elements. \~
    \details \ru Присвоить элементам столбца значения координат однородной точки.
             \en Assign coordinates of the uniform point to column elements. \~
    \param[in] icol - \ru Индекс столбца.
                      \en A column index. \~
    \param[in] column - \ru Исходная точка.
                        \en The initial point. \~
  */
  void        SetColumn( size_t icol, const MbHomogeneous & column );
  /**
    \brief \ru Присвоить элементам строки значения компонент вектора.
           \en Assign vector components values to row elements. \~
    \details \ru Присвоить элементам строки значения компонент вектора.
             \en Assign vector components values to row elements. \~
    \param[in] irow - \ru Индекс столбца.
                      \en A column index. \~
    \param[in] row - \ru Исходный вектор.
                     \en The initial vector. \~
  */
  void        SetRow( size_t irow, const MbVector & row );
  /**
    \brief \ru Присвоить элементам строки значения координат точки.
           \en Assign point coordinates values to row elements. \~
    \details \ru Присвоить элементам строки значения координат точки.
             \en Assign point coordinates values to row elements. \~
    \param[in] irow - \ru Индекс столбца.
                      \en A column index. \~
    \param[in] row - \ru Исходная точка.
                     \en The initial point. \~
  */
  void        SetRow( size_t irow, const MbCartPoint & row );
  /**
    \brief \ru Присвоить элементам строки значения координат однородной точки.
           \en Assign coordinates of the uniform point to row elements. \~
    \details \ru Присвоить элементам строки значения координат однородной точки.
             \en Assign coordinates of the uniform point to row elements. \~
    \param[in] irow - \ru Индекс столбца.
                      \en A column index. \~
    \param[in] row - \ru Исходная точка.
                     \en The initial point. \~
  */
  void        SetRow( size_t irow, const MbHomogeneous & row );
  /// \ru Установить компоненты сдвига матрицы. \en Set components of matrix translation.  
  void        SetOrigin( const MbCartPoint & p );
  /** \} */

public:
  /** 
    \ru \name Функции умножения матриц.
    \en \name Matrices multiplication.
    \{ */

  /// \ru Домножить матрицу слева \en Multiply a matrix on the left 
  MbMatrix &  PreMultiply ( const MbMatrix & );
  /// \ru Домножить матрицу справа \en Multiply a matrix on the right. 
  MbMatrix &  PostMultiply ( const MbMatrix & );

  /** \} */
public:
  /** 
    \ru \name Функции масштабирования.
    \en \name Scaling functions.
    \{ */
  /// \ru Масштабировать по X и Y. \en Scale by X and Y. 
  MbMatrix &  Scale ( double s );
  /// \ru Масштабировать по X. \en Scale by X. 
  void        ScaleX( double s );
  /// \ru Масштабировать по Y. \en Scale by Y. 
  void        ScaleY( double s );
  /// \ru Выдать коэффициент масштабирования по X. \en Get scaling coefficient by X. 
  double      GetScaleX() const;
  /// \ru Выдать коэффициент масштабирования по Y. \en Get scaling coefficient by Y. 
  double      GetScaleY() const;
  /// \ru Проверить, различаются ли коэффициенты масштабирования по X и Y. \en Check differences of scaling coefficients (X and Y). 
  bool        IsDifferentScale() const;

  /** \} */
public:
  /** 
    \ru \name Функции преобразований матрицы: сдвиг, поворот и т.д.
    \en \name Functions of matrix transformations: translation, rotation, etc.
    \{ */

  /// \ru Сместить по X и Y. \en Shift by X and Y. 
  void        Shift ( double shift );
  /// \ru Сместить по X. \en Shift by X. 
  void        ShiftX( double shift );
  /// \ru Сместить по Y. \en Shift by Y. 
  void        ShiftY( double shift );

  /// \ru Сдвинуть на вектор (домножение справа на матрицу сдвига). \en Translate by the vector (multiply by translation matrix on the right). 
  MbMatrix &  Move( const MbVector & v ) { return Move( v.x, v.y ); }
  /// \ru Сдвинуть на заданные приращения. \en Translate by given increments. 
  MbMatrix &  Move  ( double dx, double dy );

  /**
    \brief \ru Повернуть на угол.
           \en Rotate by an angle. \~
    \details \ru Поворот совершается вокруг оси Z.
             \en Rotation is around the axis Z. \~
    \param[in] angle - \ru Угол вращения.
                       \en A rotation angle. \~
  */
  MbMatrix &  Rotate( double angle );
  /**
    \brief \ru Повернуть согласно вектору направления.
           \en Rotate according to the direction vector. \~
    \details \ru Поворот совершается вокруг оси Z, т.к. матрица двумерная. Вектор направления 
      определяет угол поворота.
             \en Rotation is around the Z axis, because matrix is two-dimensional. A direction vector 
      defines a rotation angle. \~
    \param[in] - \ru Вектор направления.
                 \en A direction vector. \~
  */
  MbMatrix &  Rotate( const MbDirection & );
  /**
    \brief \ru Повернуть вокруг точки на угол.
           \en Rotate at angle around a point. \~
    \details \ru Повернуть вокруг точки на угол.
             \en Rotate at angle around a point. \~
    \param[in] - \ru Точка.
                 \en A point. \~
    \param[in] angle - \ru Вектор направления, задающий угол вращения.
                       \en A direction vector which defines a rotation angle \~
  */
  MbMatrix &  Rotate( const MbCartPoint &, const MbDirection & angle );

  /**
    \brief \ru Преобразовать координаты.
           \en Transform coordinates. \~
    \details \ru Координаты преобразуются согласно матрице.
             \en Coordinates are transformed according to a matrix. \~
    \param[in] x, y - \ru Старые координаты по x и y.
                      \en Old coordinates by x and y. \~
    \param[in] xn, yn - \ru Преобразованные координаты.
                        \en Transformed coordinates. \~
  */
  void        TransformCoord ( double x, double y, double & xn, double & yn ) const;
  /**
    \brief \ru Преобразовать длину по направлению 0X.
           \en Transform the length in the direction of 0X. \~
    \details \ru Длина преобразуются согласно матрице. Если матрица имеет разные масштабы по X и Y,
      то масштаб по Y проигнорируется.
             \en The length is transformed according to a matrix. If the matrix has a different scales for the X and Y,
      then the scale by Y is ignored. \~
    \param[in, out] len - \ru Длина по направлению 0X.
                          \en The length in the direction of 0X. \~
  */
  void        TransformScalarX( double & len ) const;

  /**
    \brief \ru Задать матрицу преобразования симметрии (отражение).
           \en Set the matrix to symmetry transformation (reflection). \~
    \details \ru Находится матрица для преобразования симметрии относительно прямой, заданной точкой и нормалью к прямой.
             \en The matrix of symmetry transformation is found relative to a line which is defined by a point and the normal. \~
    \param[in] origin - \ru Точка прямой.
                        \en A point of a line. \~
    \param[in] normal - \ru Нормаль прямой.
                        \en The normal of a line. \~
  */
  MbMatrix &  Symmetry( const MbCartPoint & origin, const MbVector & normal );
  /// \ru Найти матрицу для преобразования симметрии относительно прямой, заданной точкой и направлением. \en Find a matrix for transformation of symmetry relative to a line given by a point and direction. 
  MbMatrix &  Symmetry( const MbCartPoint & origin, const MbDirection & direction );  

  /** \} */
public:
  /** 
    \ru \name Расчет алгебраических свойств матрицы.
    \en \name Calculation of the algebraic properties of a matrix.
    \{ */

  /// \ru Транспонировать матрицу. \en Transpose a matrix. 
  void        Adj();
  /// \ru Вычислить алгебраическое дополнение. \en Calculate the algebraic adjunct. 
  double      Delta( size_t line, size_t column, size_t dim ) const;
  /// \ru Вычислить определитель матрицы. \en Calculate the determinant of a matrix. 
  double      Det  ( size_t dim ) const;
  /// \ru Вычислить обратную матрицу. \en Calculate inverse matrix.
  void        Div  ( MbMatrix &, double eps = DOUBLE_EPSILON ) const;
  /// \ru Наибольший элемент матрицы по абсолютному значению. \en Maximal element of the matrix in absolute value.
  double      NormMax() const;

  /** \} */
public:
  /** 
    \ru \name Перегрузка алгебраических и логических операций.
    \en \name Overload of arithmetical and logical operations.
    \{ */
  /// \ru Умножить на матрицу справа. \en Multiply by the matrix on the right. 
  MbMatrix    operator *  ( const MbMatrix & ) const;
  /// \ru Домножить на матрицу справа. \en Multiply by the matrix on the right. 
  MbMatrix &  operator *= ( const MbMatrix & m ) { return PostMultiply( m ); }
  /// \ru Присвоить значение. \en Assign a value. 
  MbMatrix &  operator =  ( const MbMatrix & m ) { flag = m.flag; ::memcpy( &el, m.el, sizeof(el) ); return *this; }
  /// \ru Сравнить с матрицей (точность - LENGTH_EPSILON). \en Compare with a matrix(tolerance- LENGTH_EPSILON). 
  bool        operator == ( const MbMatrix & ) const;

  /// \ru Доступ по ссылке к элементу матрицы. \en Access to a matrix element by a reference. 
        double &  operator() ( size_t i, size_t j )       { C3D_ASSERT( std_max( i, j ) < MATRIX_DIM_2D ); flag = MB_UNSET; return el[i][j]; }
  /// \ru Значение элемента матрицы. \en The value of a matrix element. 
  const double &  operator() ( size_t i, size_t j ) const { C3D_ASSERT( std_max( i, j ) < MATRIX_DIM_2D );                  return el[i][j]; }

  /** \} */

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

public:
  KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( MbMatrix, MATH_FUNC_EX ) // \ru Для работы со ссылками и объектами класса \en For working with references and objects of the class. 
  DECLARE_NEW_DELETE_CLASS( MbMatrix )
  DECLARE_NEW_DELETE_CLASS_EX( MbMatrix )
};


//-------------------------------------------------------------------------------
// \ru Установка флага смещения для матрицы и системы координат. \en Check translation 
/**  
\attention \ru Только для внутреннего использования.
\en For internal use only. \~
*/
// ---
template<class Transform>
void CheckOrigin( const Transform & trans, uint8 & flag, bool resetFlag )
{
  if ( !(flag & MB_UNSET) ) {
    const MbCartPoint & pOrigin = trans.GetOrigin();
    const double lengthEpsilon = LENGTH_EPSILON; 

    if ( (::fabs(pOrigin.x) > lengthEpsilon) || 
         (::fabs(pOrigin.y) > lengthEpsilon) )
      flag |= MB_TRANSLATION;
    else if ( resetFlag ) {
      flag &= ~MB_TRANSLATION;
    }
  }
}


//-------------------------------------------------------------------------------
// \ru Установка флага вращения для матрицы и системы координат. \en Check rotation.
// ( Использование корректно только для ортонормированных СК)
/**  
  \attention \ru Только для внутреннего использования.
             \en For internal use only. \~
*/
// ---
template<class Transform>
void CheckRotation( const Transform & trans, uint8 & flag, bool resetFlag )
{
  if ( !(flag & MB_UNSET) ) {
    const MbVector & axisX = trans.GetAxisX();
    const MbVector & axisY = trans.GetAxisY();

    // Барьер нулевых элементов матрицы
    const double eps = EXTENT_EPSILON;
    if ( ::fabs(axisX.y) > eps ||
         ::fabs(axisY.x) > eps ||
         ::fabs(axisX.x - 1.0) > eps ||
         ::fabs(axisY.y - 1.0) > eps )
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


//------------------------------------------------------------------------------
// \ru Инициализация матрицы. \en Initialize matrix. 
// ---
inline MbMatrix & MbMatrix::Init() 
{
             el[0][1] = el[0][2] = 0.0;            //-V525
  el[1][0] =            el[1][2] = 0.0;
  el[2][0] = el[2][1] =            0.0;
  el[0][0] = el[1][1] = el[2][2] = 1.0;
  flag = MB_IDENTITY;
  return *this;
}

//------------------------------------------------------------------------------
// \ru Инициализация матрицы по точке и нормализованному вектору направления \en Initialization of a matrix by a point and normalized direction vector 
// ---
inline MbMatrix & MbMatrix::Init( const MbCartPoint & pnt, const MbDirection & dir ) 
{
  Init(); // \ru Инициализация матрицы как единичной \en Initialization of unit matrix 
  Rotate( dir ); // \ru Поворот на угол dir \en Rotation by the "dir" angle. 
  Move( pnt.x, pnt.y ); // \ru Сдвиг в точку pnt \en Translation to "pnt" point 
  flag = MB_UNSET;
  return *this;
}


//-------------------------------------------------------------------------------
// \ru Выдать признак, что есть только перенос. \en Get an attribute that there is only translation. 
// ---
inline bool MbMatrix::IsTranslationOnly() const
{
  CheckFlag();
  if ( !!(flag & MB_TRANSLATION) && !(flag & MB_ROTATION) && !(flag & MB_LEFT) && !(flag & MB_SCALING) && !(flag & MB_PERSPECTIVE) )
    return true;
  return false;
}


//------------------------------------------------------------------------------
// \ru Проверка на равенство матриц. \en Check for equality of matrices.
// ---
inline bool MbMatrix::operator == ( const MbMatrix & m ) const
{
  bool bRes = true;
  for ( size_t  i = 0; (i < MATRIX_DIM_2D) && bRes; i++ ) {   
    bRes = ( (::fabs(el[i][0] - m.el[i][0]) <= LENGTH_EPSILON) &&
             (::fabs(el[i][1] - m.el[i][1]) <= LENGTH_EPSILON) &&
             (::fabs(el[i][2] - m.el[i][2]) <= LENGTH_EPSILON) );
  }
  return bRes;
}


//------------------------------------------------------------------------------
// \ru Преобразовать длину по направлению 0X                     \en Transform the length in the direction of 0X 
// \ru Если вдруг матрица начинена разными масштабами по X и Y,  \en If the matrix has a different scales by X and Y, 
// \ru То масштаб по Y игнорируется                              \en Then the scale by Y is ignored 
// ---
inline void MbMatrix::TransformScalarX( double & len ) const {
  len *= MbVector( el[0][0], el[0][1] ).Length();
}


//------------------------------------------------------------------------------
// \ru Двумерная матрица - преобразовать координаты \en Two-dimensional matrix - transform coordinates 
// ---
inline void MbMatrix::TransformCoord( double x, double y, double & xn, double & yn ) const {
  xn = x * el[0][0] + y * el[1][0] + el[2][0];
  yn = x * el[0][1] + y * el[1][1] + el[2][1];
}

//------------------------------------------------------------------------------
// \ru Являются ли объекты равными? \en Determine whether an object is equal?
// ---
inline bool MbMatrix::IsSame( const MbMatrix & m2, double accuracy ) const
{
  const MbMatrix & m1 = *this;

  bool isSame = (
    (::fabs(m1.el[0][0] - m2.el[0][0]) <= accuracy) &&
    (::fabs(m1.el[0][1] - m2.el[0][1]) <= accuracy) &&
    (::fabs(m1.el[0][2] - m2.el[0][2]) <= accuracy) &&

    (::fabs(m1.el[1][0] - m2.el[1][0]) <= accuracy) &&
    (::fabs(m1.el[1][1] - m2.el[1][1]) <= accuracy) &&
    (::fabs(m1.el[1][2] - m2.el[1][2]) <= accuracy) &&

    (::fabs(m1.el[2][0] - m2.el[2][0]) <= accuracy) &&
    (::fabs(m1.el[2][1] - m2.el[2][1]) <= accuracy) &&
    (::fabs(m1.el[2][2] - m2.el[2][2]) <= accuracy) );

  return isSame;
}


//------------------------------------------------------------------------------
/** 
  \brief \ru Перемножить матрицы.
         \en Multiply matrices. \~
  \details \ru Умножение матрицы m1 на матрицу m2 (вместо res = m1 * m2).
           \en Multiply m1 matrix by m2 matrix (instead of res = m1 * m2). \~
  \param[in] m1, m2 - \ru Исходные матрицы.
                      \en Initial matrices. \~
  \param[out] res - \ru Результирующая матрица.
                    \en The required matrix. \~
  \ingroup Mathematic_Base_2D
*/
MATH_FUNC (void) MulMatrix( const MbMatrix & m1, const MbMatrix & m2, MbMatrix & res );


#endif // __MB_MATRIX_H
