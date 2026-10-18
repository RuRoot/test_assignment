////////////////////////////////////////////////////////////////////////////////
/** 
  \file \brief  \ru Квадратная матрица чисел N x N.
                \en Square matrix of numbers N x N. \~
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MB_MATRIXNN_H
#define __MB_MATRIXNN_H


#include <math_define.h>
#include <alg_indicator.h>
#include <mb_property_title.h>
#include <mb_enum.h>


//------------------------------------------------------------------------------
/** \brief \ru Квадратная матрица чисел N x N.
           \en Square matrix of numbers N x N. \~
  \ingroup Base_Items
*/
// ---
class MATH_CLASS MatrixNN 
{
private :
  double ** parr; ///< \ru Указатель на первый элемент матрицы. \en A pointer to the first element of the matrix. 
  size_t    n;    ///< \ru Размерность матрицы. \en A dimension of the matrix. 

protected:
  /// \ru Конструктор. \en Constructor. 
  MatrixNN() : parr( nullptr ), n( 0 ) {}
  /// \ru Конструктор по заданной размерности. \en The constructor by a given dimension. 
  MatrixNN( size_t dim ) : parr( nullptr ), n( 0 ) { SetSize( dim ); }
public:
  /// \ru Конструктор ограниченной размерности. \en The constructor of restricted dimension. 
  MatrixNN ( const uint16 & dim ) : parr( nullptr ), n( 0 ) { SetSize( dim ); }
  /// \ru Конструктор копирования. \en The copy constructor. 
  explicit MatrixNN ( const MatrixNN & ); 
  /// \ru Деструктор. \en Destructor. 
  ~MatrixNN() { SetSize( 0 ); }

public:
  /// \ru Конструктор по заданной размерности. \en The constructor by a given dimension. 
  static MatrixNN * Create( size_t m );

public: // Общие методы матриц (двумерных массивов)
        size_t      Lines  () const { return n; } ///< \ru Дать количество строк матрицы. \en Give the number of matrix rows. 
        size_t      Columns() const { return n; } ///< \ru Дать количество столбцов матрицы. \en Give the number of matrix columns.
        size_t      Count  () const { return (n*n); } ///< \ru Количество элементов матрицы. \en Give the number of matrix elements. 
   c3d::IndicesPair GetSize() const { return c3d::IndicesPair( n, n ); } ///< \ru Дать размер матрицы. \en Give the size of the matrix. 
        bool        SetSize( c3d::IndicesPair sz ) { size_t m = std_min( sz.first, sz.second ); return SetSize( m ); } ///< \ru Установить размер. \en Set size. 
        bool        SetSize( size_t lsz, size_t csz, bool save_vals = false ) { size_t m = std_min( lsz, csz ); return SetSize( m, save_vals ); } ///< \ru Установить размер. \en Set size. 
        bool        SetSize( size_t dim, bool save_vals = false ); ///< \ru Установить размер. \en Set size. 

        /// \ru Получить элемент матрицы (i,j). \en Get an element of the matrix (i,j). 
  const double &    GetElem( size_t i, size_t j ) const     { C3D_ASSERT( !!parr && i < n && j < n ); return parr[i][j]; }
        /// \ru Установить элемент матрицы (i,j). \en Set an element of the matrix (i,j). 
        void        SetElem( size_t i, size_t j, double v ) { C3D_ASSERT( !!parr && i < n && j < n ); parr[i][j] = v; }
        /// \ru Добавить элемент матрицы (i,j). \en Add an element of the matrix (i,j). 
        void        AddElem( size_t i, size_t j, double v ) { C3D_ASSERT( !!parr && i < n && j < n ); parr[i][j] += v; }
        /// \ru Получить элемент матрицы (i,j). \en Get an element of the matrix (i,j). 
  const double &    operator() ( size_t i, size_t j ) const { C3D_ASSERT( !!parr && i < n && j < n ); return parr[i][j]; }
        /// \ru Обнулить матрицу. \en Set the matrix to null. 
        MatrixNN &  SetZero();
        /// \ru Инициализировать элементами другой матрицы. \en Initialize by elements of another matrix. 
        bool        Init( const MatrixNN & );
        /// \ru Оператор присваивания. \en The assignment operator. 
        MatrixNN &  operator = ( const MatrixNN & mtr ) { Init( mtr ); return *this; }
        /// \ru Поменять местами строки. \en Swap lines. 
        bool        SwapLines( size_t ln1, size_t ln2 );

public:
        /// \ru Установить элемент матрицы (i,j). \en Set an element of the matrix (i,j). 
        double &    operator() ( size_t i, size_t j )       { C3D_ASSERT( !!parr && i < n && j < n ); return parr[i][j]; }
        /// \ru Выдать адрес начала строки матрицы. \en Get an address of the matrix row start . 
  const double *    GetLine( size_t i ) const { C3D_ASSERT( !!parr && i < n ); return parr[i]; }
        /// \ru Выдать адрес начала строки матрицы. \en Get an address of the matrix row start . 
        double *    SetLine( size_t i )       { C3D_ASSERT( !!parr && i < n ); return parr[i]; } // SKIP_SA
        /// \ru Инициировать элемент. \en Initiate an element. 
        void        Init( size_t i, size_t j, double v ) { C3D_ASSERT( !!parr && i < n && j < n ); parr[i][j] = v; }
        /// \ru Установить строку. \en Set a row. 
        void        SetLine( size_t i, double * p ) { C3D_ASSERT( !!parr && i < n ); parr[i] = p; }
        /// \ru Выдать адрес матрицы. \en Get an address of the matrix. 
        double **   SetParr() { return parr; }

        /// \ru Сделать матрицу единичной. \en Set the matrix to unit. 
        void        SetSingle();  
        /// \ru Увеличить размерность, добавив строку и столбец в конец. \en Increase the dimension by adding a row and coloumn to the end. 
        bool        Add();
        /// \ru Удалить строку и столбец. \en Remove a row and column. 
        void        Delete( size_t i );

private:
        // \ru Инициализировать матрицу и установить размерность. \en Initialize a matrix and set dimension. 
        void        SetParrAndDimension( double ** p, size_t i ) { parr = p; n = i; }
}; // MatrixNN


//------------------------------------------------------------------------------
/** \brief \ru Решение системы линейных уравнений методом исключения Гаусса.
           \en System of linear equations is solved by the Gauss method. \~
  \details \ru Решение системы линейных уравнений методом исключения Гаусса. \n
           \en System of linear equations is solved by the Gauss method. \n \~
  \param[in] a - \ru Матрица коэффициентов при неизвестных
                 \en Coefficient matrix \~
  \param[in] b - \ru Массив правых частей, в него же помещается результат решения
                 \en Array of right parts, on output it contains the result of the solution \~
  \param[in] epsilon - \ru Погрешность решения
                       \en Tolerance of solution \~
  \param[in] baseProgBar - \ru Индикатор процесса решения
                           \en Progress indicator of solution \~ 
  \return \ru Код ошибки: если nr_Success (+1), то система решена, если nr_Special, то нет решений или система вырождена.
          \en Error code: if nr_Success (+1), then the system is solved, if nr_Special, there is no solution or the system is degenerate. \~
  \ingroup Base_Items
*/
// ---
template <class Type, class MatrixNN>
MbeNewtonResult TypedGaussEquation ( MatrixNN & a, Type * b, double epsilon, ProgressBarWrapper * baseProgBar = nullptr )
{
  ProgressBarWrapper * progBar = nullptr;
  if ( baseProgBar != nullptr ) {
    StrData<MbeProgBarId_PointsSurface> strData( pbarId_Solve_LinearEquationsSystem );
    progBar = &baseProgBar->CreateChildAddRef( strData );
  }

  ptrdiff_t count = (ptrdiff_t)std_min( a.Lines(), a.Columns() );
  C3D_ASSERT( a.Lines() == a.Columns() );

  if ( count < 1 ) {
    ::FinishProgressBar( progBar );
    ::ReleaseItem( progBar );
    return nr_Special;
  }

  ptrdiff_t i, j, k;
  double m, tmp;
  Type parr;

  ptrdiff_t halfCount = count / 2;

  for ( k = 0; k < count - 1; k++ ) {
    // \ru Переставить уравнения так, чтобы a[k][k] != 0 \en Swap equations so that a[k][k] != 0 
    ptrdiff_t l = k;
    tmp = ::fabs( a(k, k) );
    for ( i = k + 1; i < count; i++ ) {
      if ( ::fabs( a(i, k) ) > tmp ) {
        l = i;
        tmp = ::fabs( a(i, k) );
      }
    }

    if ( k == halfCount )
      ::SetProgressBarValue( progBar, 25 );
    if ( ::StopProgressBar( progBar ) ) { // \ru Остановка по запросу \en Stop by request 
      ::ReleaseItem( progBar );
      return nr_Special;
    }

    if ( l != k ) {
      // (!) Рекомендуется использовать MatrixNN::SwapLines(k,l)
      for ( j = k; j < count; j++ ) 
      {
        tmp     = a(k, j);
        a.SetElem( k, j, a(l, j) );
        a.SetElem( l, j, tmp );
      }
      parr = b[k];
      b[k] = b[l];
      b[l] = parr;
    }

    if ( ::StopProgressBar( progBar ) ) { // \ru Остановка по запросу \en Stop by request 
      ::ReleaseItem( progBar );
      return nr_Special;
    }

    if ( ::fabs( a(k, k) ) < epsilon ) {
      ::FinishProgressBar( progBar );
      ::ReleaseItem( progBar );
      return nr_Special; // \ru Система не имеет решений \en System doesn't have solutions 
    }
    
    tmp = 1 / a(k, k);

    for ( i = k + 1; i < count; i++ ) 
    {
      if ( a(i, k) != 0.0 ) {
        m = a(i, k) * tmp;
        a.SetElem( i, k, 0.0 );
        for ( j = k + 1; j < count; j++ )
          a.SetElem( i, j, a(i, j) - a(k, j) * m );
        b[i] -= b[k] * m; // SKIP_SA
      }
    }

    if ( ::StopProgressBar( progBar ) ) { // \ru Остановка по запросу \en Stop by request 
      ::ReleaseItem( progBar );
      return nr_Special;
    }
  }

  ::SetProgressBarValue( progBar, 50 );

  if ( ::fabs( a(k, k) ) < epsilon ) {
    ::FinishProgressBar( progBar );
    ::ReleaseItem( progBar );
    return nr_Special; // \ru Система не имеет решений \en System doesn't have solutions 
  }
  
  // \ru Обратная подстановка \en Back-substitution 
  i = count - 1;
  b[i] *= 1 / a(i, i);

  for ( i = count - 2; i >= 0; i-- ) {
    j = i + 1;
    parr = b[j] * a(i, j);
    for ( j = i + 2; j < count; j++ )
      parr += b[j] * a(i, j);
    m = 1.0 / a(i, i);
    b[i] = ( b[i] - parr ) * m;

    if ( i == halfCount )
      ::SetProgressBarValue( progBar, 75 );
    if ( ::StopProgressBar( progBar ) ) { // \ru Остановка по запросу \en Stop by request 
      ::ReleaseItem( progBar );
      return nr_Special;
    }
  }

  ::SetProgressBarValue( progBar, 100 );
  ::FinishProgressBar( progBar );
  ::ReleaseItem( progBar );
  return nr_Success;
} // GaussEquation


//------------------------------------------------------------------------------
/** \brief \ru Решение системы линейных уравнений методом исключения Гаусса.
           \en System of linear equations is solved by the Gauss method. \~
  \details \ru Решение системы линейных уравнений методом исключения Гаусса. \n
           \en System of linear equations is solved by the Gauss method. \n \~
  \param[in] a - \ru Матрица коэффициентов при неизвестных
                 \en Coefficient matrix \~
  \param[in] b - \ru Массив правых частей, в него же помещается результат решения
                 \en Array of right parts, on output it contains the result of the solution \~
  \param[in] epsilon - \ru Погрешность решения
                       \en Tolerance of solution \~
  \param[in] baseProgBar - \ru Индикатор процесса решения
                           \en Progress indicator of solution \~ 
  \return \ru Код ошибки: если nr_Success (+1), то система решена, если nr_Special, то нет решений или система вырождена.
          \en Error code: if nr_Success (+1), then the system is solved, if nr_Special, there is no solution or the system is degenerate. \~
  \ingroup Base_Items
*/
// ---
template <class Type, class SparseMatrix>
MbeNewtonResult TypedGaussEquationWithBandMatrix( SparseMatrix & a, Type * b, double epsilon, ProgressBarWrapper * baseProgBar = nullptr )
{
  ProgressBarWrapper * progBar = nullptr;
  if ( baseProgBar != nullptr ) {
    StrData<MbeProgBarId_PointsSurface> strData( pbarId_Solve_LinearEquationsSystem );
    progBar = &baseProgBar->CreateChildAddRef( strData );
  }

  ptrdiff_t count = (ptrdiff_t)std_min( a.Lines(), a.Columns() );
  C3D_ASSERT( a.Lines() == a.Columns() );

  if ( count < 1 ) {
    ::FinishProgressBar( progBar );
    ::ReleaseItem( progBar );
    return nr_Special;
  }

  ptrdiff_t i, j, k;

  ptrdiff_t halfCount = count / 2;

  std::vector<size_t> nzIndices;
  c3d::IndicesPair searchRange;

  a.NzUpdate();

  for ( k = 0; k < count - 1; k++ ) {
    // \ru Переставить уравнения так, чтобы a[k][k] != 0 \en Swap equations so that a[k][k] != 0 
    ptrdiff_t l = k;
    double tmp = ::fabs( a.GetElem( k, k ) );
    for ( i = k + 1; i < count; i++ ) {
      double faik = ::fabs( a.GetElem( i, k ) );
      if ( faik > tmp ) {
        l = i;
        tmp = faik;
      }
    }

    if ( k == halfCount )
      ::SetProgressBarValue( progBar, 25 );
    if ( ::StopProgressBar( progBar ) ) { // \ru Остановка по запросу \en Stop by request 
      ::ReleaseItem( progBar );
      return nr_Special;
    }

    if ( l != k ) {
      a.SwapLines( k, l );
      std::swap( b[k], b[l] );
    }

    if ( ::StopProgressBar( progBar ) ) { // \ru Остановка по запросу \en Stop by request 
      ::ReleaseItem( progBar );
      return nr_Special;
    }

    tmp = a.GetElem(k, k);

    if ( ::fabs( tmp ) < epsilon ) {
      ::FinishProgressBar( progBar );
      ::ReleaseItem( progBar );
      return nr_Special; // \ru Система не имеет решений \en System doesn't have solutions 
    }
    
    tmp = 1.0 / tmp;

    for ( i = k + 1; i < count; i++ ) {
      double aik = a.GetElem( i, k );
      if ( ::fabs(aik) > NULL_EPSILON ) {
        double m = aik * tmp;
        a.SetElem( i, k, 0.0 );
        aik = 0.0;

        nzIndices.clear();
        if ( k + 1 < count ) {
          // [k+1,count)
          searchRange.first  = (k + 1);
          searchRange.second = (count - 1);
          a.NzIndices( k, searchRange, nzIndices );
        }
        size_t nzCnt = nzIndices.size();

        for ( size_t nzInd = 0; nzInd < nzCnt; ++nzInd ) {
          j = nzIndices[nzInd];
          double ae = a.GetElem( k, j ) * m;
          if ( ::fabs(ae) > NULL_EPSILON ) {
            ae = a.GetElem( i, j ) - ae;
            a.SetElem( i, j, ae );
          }
        }
        b[i] -= b[k] * m;
      }
    }

    if ( ::StopProgressBar( progBar ) ) { // \ru Остановка по запросу \en Stop by request 
      ::ReleaseItem( progBar );
      return nr_Special;
    }
  }

  a.NzUpdate();

  ::SetProgressBarValue( progBar, 50 );

  if ( ::fabs( a.GetElem(k, k) ) < epsilon ) {
    ::FinishProgressBar( progBar );
    ::ReleaseItem( progBar );
    return nr_Special; // \ru Система не имеет решений \en System doesn't have solutions 
  }
  
  // \ru Обратная подстановка \en Back-substitution 
  i = count - 1;
  b[i] *= 1.0 / a.GetElem(i, i);

  Type be;
  for ( i = count - 2; i >= 0; i-- ) {
    j = i + 1;
    be = b[j] * a.GetElem( i, j );

    nzIndices.clear();
    if ( i + 2 < count ) {
      // [i+2,count)
      searchRange.first  = (i + 2);
      searchRange.second = (count - 1);
      a.NzIndices( i, searchRange, nzIndices );
    }
    size_t nzCnt = nzIndices.size();

    for ( size_t nzInd = 0; nzInd < nzCnt; ++nzInd ) {
      j = nzIndices[nzInd];
      be += b[j] * a.GetElem( i, j );
    }
    double m = 1.0 / a.GetElem( i, i );
    b[i] = ( b[i] - be ) * m;

    if ( i == halfCount )
      ::SetProgressBarValue( progBar, 75 );
    if ( ::StopProgressBar( progBar ) ) { // \ru Остановка по запросу \en Stop by request 
      ::ReleaseItem( progBar );
      return nr_Special;
    }
  }

  ::SetProgressBarValue( progBar, 100 );
  ::FinishProgressBar( progBar );
  ::ReleaseItem( progBar );
  return nr_Success;
} // GaussEquation


//------------------------------------------------------------------------------
/** 
  \brief \ru Решение системы линейных уравнений с трехдиагональной матрицей методом прогонки.
         \en System of linear equations with a tridiagonal matrix is solved by the sweep method. \~
  \details \ru Решение системы линейных уравнений с трехдиагональной матрицей методом прогонки.
    Используется для построения NURBS-копии незамкнутого кубического сплайна.
           \en System of linear equations with a tridiagonal matrix is solved by the sweep method.
    Used to build a NURBS-copy of a non-closed cubic spline. \~ 
  \param[in] n - \ru число неизвестных
                 \en The number of unknown variables \~
  \param[in] a - \ru Главная диагональ трехдиагональной матрицы, массив double размерности n
                 \en Main diagonal of a tridiagonal matrix is an array of doubles of size "n" \~ 
  \param[in] b - \ru Верхняя диагональ, размерность n-1
                 \en Upper diagonal, dimension is n-1 \~
  \param[in] c - \ru <b>c</b> - нижняя  диагональ, размерность n-1
                 \en <b>c</b> - lower diagonal, dimension is n-1 \~
  \param[in] r - \ru Вектор правой части, массив точек или векторов размерности n; должна быть определена операция умножения на double справа
                 \en Vector of the right part is an array of points or vectors of dimension n; the multiplication operation by the double value on the right must be defined \~
  \param[in] solution - \ru Массив решений (точек, векторов), размерности n
                        \en Array of solutions (points, vectors) of dimension n \~
  \param[in] epsZero - \ru Погрешность нуля
                       \en Tolerance of zero \~
  \return \ru Код ошибки: если nr_Success (+1), то система решена, если nr_Special, то нет решений или система вырождена.
          \en Error code: if nr_Success (+1), then the system is solved, if nr_Special, there is no solution or the system is degenerate. \~
  \ingroup Base_Items
*/
// ---
template <class ArrayDouble, class ArrayType>
MbeNewtonResult TypedTridiagonalSolve ( const size_t n, ArrayDouble & a, ArrayDouble & b, ArrayDouble & c, 
                                        ArrayType & r, ArrayType & solution, double epsZero )
{
  if ( ::fabs ( a[0] ) < epsZero ) 
    return nr_Special; // \ru Прогонка не работает \en Sweep does not work 
  // \ru Прямой ход прогонки \en Forward step of sweep 
  b[0] /= - a[0];  
  r[0] /=   a[0];  
  for ( size_t i = 1; i < n; ++i ) {
    double dom = a[i] + b[i - 1] * c[i - 1];
    if ( ::fabs ( dom ) < epsZero )
      return nr_Special; // \ru Прогонка не работает \en Sweep does not work 
    if ( i < n - 1 ) // \ru Размерность b равна n-1, поэтому не можем вычислять b[n-1], да и не нужно оно. \en B dimension is equal to n-1, therefore we can not calculate b [n-1], and it isn't necessary. 
      b[i] /= - dom;                         
    r[i] = ( r[i] - r[i - 1] * c[i - 1] ) / dom;
  }
  // \ru Обратный ход \en Backward substitution 
  solution[n - 1] = r[n - 1];
  for ( ptrdiff_t i = n - 2; i >= 0; --i )
    solution[i] = r[i] + solution[i + 1] * b[i];
  return nr_Success;
} 


#endif // __MB_MATRIXNN_H
