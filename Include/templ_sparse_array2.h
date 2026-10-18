////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Двумерный массив объектов.
         \en Two-dimensional array of objects. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEMPL_SPARSE_ARRAY2_H
#define __TEMPL_SPARSE_ARRAY2_H


#include <math_define.h>
#include <functional>
#include <map>
#include <utility>
#include <vector>


//------------------------------------------------------------------------------
/** \brief \ru Разреженный вектор объектов.
           \en Sparse vector of objects. \~
  \details \ru Разреженный вектор объектов. \n
           \en Sparse vector of objects. \n \~
  \ingroup Base_Tools_Containers
*/
// ---
template <class Type>
class SparseRow {
public:
  typedef std::pair<ptrdiff_t, ptrdiff_t>   NumberRange;
  typedef std::pair<size_t,Type>            NumberType;
  typedef std::vector<Type>                 RowVector;
  typedef std::map<size_t, Type>            RowMap;
  typedef typename RowMap::const_iterator   RowMapConstIt;
  typedef typename RowMap::iterator         RowMapIt;
  typedef typename std::pair<RowMapIt,bool> RowMapRet;

protected:
  RowMap      rowMap;    ///< \ru Таблица ненулевых элементов. \en Map of nonzero elements.
  RowVector   rowVector; ///< \ru Вектор ненулевых элементов. \en Vector of nonzero elements.
  size_t      rowSize;   ///< \ru Размер вектора. \en Vector size.
  NumberRange nzRange;   ///< \ru Диапазон ненулевых элементов [nz1,nz2]. \en Range of nonzero elements [nz1,nz2].

public:
  static const Type defaultElem; ///< \ru Нулевой элемент (элемент по умолчанию). \en Zero element (default element).

public:
  /// \ru Конструктор. \en Constructor. 
  SparseRow() : rowMap(), rowVector(), rowSize(0), nzRange( SYS_MAX_ST, SYS_MIN_ST ) {}
  /// \ru Конструктор копирования. \en Copy-constructor. 
  SparseRow( const SparseRow<Type> & );
  /// \ru Деструктор. \en Destructor.  
  virtual ~SparseRow() {}

public:
  /// \ru Оператор присваивания. \en The assignment operator. 
  const SparseRow<Type> & operator = ( const SparseRow<Type> & );

public:
        void              Clear() { rowMap.clear(); rowVector.clear(); ResetNzRange(); }
        void              SetSize( size_t rsz ) { Clear(); rowSize = rsz; }

        bool              IsNzRange() const { return (nzRange.first <= nzRange.second); }
  const NumberRange &     NzRange() const { return nzRange; }
        void              ResetNzRange() { nzRange.first = SYS_MAX_ST; nzRange.second = SYS_MIN_ST; }
        void              EnlargeNzRange( ptrdiff_t n ) { nzRange.first = std_min( nzRange.first, n ); nzRange.second = std_max( nzRange.second, n ); }
        
  const Type &            GetElem( size_t k ) const;
        bool              SetElem( size_t k, const Type & );
        bool              SetLine( std::vector<NumberType> & );
        ptrdiff_t         NzBegin() const { return nzRange.first; }
        ptrdiff_t         NzEnd  () const { return IsNzRange() ? nzRange.second + 1 : nzRange.first; }
        bool              NzIndices( const NumberRange & searchRange, std::vector<size_t> & ) const;
        void              NzUpdate();
        void              Swap( SparseRow<Type> & );

protected:
        bool              CheckNzRange() const;
        bool              CheckBand( std::vector<NumberType> & ) const;
};


//------------------------------------------------------------------------------
//
// ---
template<class Type>
const Type SparseRow<Type>::defaultElem = Type();


//------------------------------------------------------------------------------
//
// ---
template<class Type>
SparseRow<Type>::SparseRow( const SparseRow<Type> & src )
  : rowMap   ( src.rowMap    )
  , rowVector( src.rowVector )
  , rowSize  ( src.rowSize   )
  , nzRange  ( src.nzRange   )
{
}


//------------------------------------------------------------------------------
//
// ---
template<class Type>
const SparseRow<Type> & SparseRow<Type>::operator = ( const SparseRow<Type> & src )
{
  rowMap = src.rowMap;
  rowVector = src.rowVector;
  rowSize  = src.rowSize;
  nzRange = src.nzRange;
  return *this;
}


//------------------------------------------------------------------------------
//
// ---
template <class Type>
inline bool SparseRow<Type>::CheckNzRange() const
{
  bool res = false;

  if ( 0 <= nzRange.first && nzRange.second <= (ptrdiff_t)rowSize ) {
    res = true;

    if ( rowVector.empty() ) {
      RowMapConstIt it = rowMap.begin();
      while ( res && it != rowMap.end() ) {
        res = false;
        if ( nzRange.first <= (ptrdiff_t)it->first && (ptrdiff_t)it->first <= nzRange.second )
          res = true;
        ++it;
      }
    }
    else {
      size_t nzSize = nzRange.second - nzRange.first + 1;
      if ( nzSize <= rowSize && nzSize == rowVector.size() )
        res = true;
    }
  }

  return res;
}


//------------------------------------------------------------------------------
//
// ---
template <class Type>
inline const Type & SparseRow<Type>::GetElem( size_t k ) const
{
  //C3D_ASSERT( CheckNzRange() );

  if ( nzRange.first <= (ptrdiff_t)k && (ptrdiff_t)k <= nzRange.second ) {
    if ( rowVector.empty() ) {
      RowMapConstIt it = rowMap.find( k );
      if ( it != rowMap.end() )
        return it->second;
    }
    else {
      ptrdiff_t index = (ptrdiff_t)k - nzRange.first;
      PRECONDITION( index >= 0 && index < (ptrdiff_t)rowVector.size() );
      if ( index >= 0 && index < (ptrdiff_t)rowVector.size() )
        return rowVector[index];
    }
  }
  return defaultElem;
}


//------------------------------------------------------------------------------
//
// ---
template <class Type>
inline bool SparseRow<Type>::SetElem( size_t k, const Type & item )
{
  //C3D_ASSERT( CheckNzRange() );

  PRECONDITION( k < rowSize );
  if ( k < rowSize ) {
    if ( rowVector.empty() ) {
      RowMapRet ret = rowMap.insert( std::make_pair( k, item ) );
      if ( !ret.second )
        ret.first->second = item;
      EnlargeNzRange( k );
      return true;
    }
    else {
      if ( nzRange.first <= (ptrdiff_t)k && (ptrdiff_t)k <= nzRange.second ) {
        ptrdiff_t index = k - nzRange.first;
        PRECONDITION( index >= 0 && index <= (ptrdiff_t)rowVector.size() );
        if ( index >= 0 && index < (ptrdiff_t)rowVector.size() )
          rowVector[index] = item;
      }
      else if ( (ptrdiff_t)k < nzRange.first ) {
        size_t addCnt = nzRange.first - k;
        std::reverse( rowVector.begin(), rowVector.end() );
        rowVector.resize( rowVector.size() + addCnt );
        rowVector.back() = item;
        std::reverse( rowVector.begin(), rowVector.end() );
        EnlargeNzRange( k );
      }
      else if ( (ptrdiff_t)k > nzRange.second ) {
        size_t addCnt = k - nzRange.second;
        rowVector.resize( rowVector.size() + addCnt );
        rowVector.back() = item;
        EnlargeNzRange( k );
      }
      return true;
    }
  }
  return false;
}


//------------------------------------------------------------------------------
//
// ---
template <class Type>
inline bool SparseRow<Type>::CheckBand( std::vector<NumberType> & row ) const
{
  bool res = true;

  size_t nzSize = row.size();

  if ( nzSize > 0 ) {
    if ( nzSize > 1 ) {
      std::sort( row.begin(), row.end() );

      size_t k;
      for ( k = nzSize; k--; ) {
        PRECONDITION( row[k].first < rowSize );
        if ( row[k].second == defaultElem ) {
          row.erase( row.begin() + k );
          nzSize--;
        }
        else if ( row[k].first >= rowSize ) {
          row.erase( row.begin() + k );
          nzSize--;
        }
      }
      if ( nzSize > 1 ) {
        size_t nzBegInd = row.front().first;
        size_t nzEndInd = row.back().first;
        size_t nzCnt = nzEndInd - nzBegInd + 1;

        if ( nzCnt > 2*nzSize ) {
          res = false;
        }
      }
    }
  }

  return res;
}


//------------------------------------------------------------------------------
//
// ---
template <class Type>
inline bool SparseRow<Type>::SetLine( std::vector<NumberType> & row )
{
  PRECONDITION( row.size() <= rowSize );

  if ( row.size() > 0 && row.size() <= rowSize ) {
    Clear();

    if ( !CheckBand( row ) ) {
      size_t nzSize = row.size();
      for ( size_t k = 0; k < nzSize; ++k ) {
        size_t nzCurInd = row[k].first;
        rowMap.insert( std::make_pair( nzCurInd, row[k].second ) );
        EnlargeNzRange( nzCurInd );
      }
    }
    else {
      size_t nzSize = row.size();

      size_t nzBegInd = row.front().first;
      size_t nzEndInd = row.back().first;
      size_t nzCnt = nzEndInd - nzBegInd + 1;

      rowVector.resize( nzCnt );
      for ( size_t k = 0; k < nzSize; ++k ) {
        size_t nzCurInd = row[k].first;
        rowVector[nzCurInd - nzBegInd] = row[k].second;
        EnlargeNzRange( nzCurInd );
      }
    }

    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
//
// ---
template <class Type>
inline bool SparseRow<Type>::NzIndices( const NumberRange & searchRange, std::vector<size_t> & indices ) const
{
  PRECONDITION( indices.empty() && searchRange.first <= searchRange.second );
  indices.clear();

  if ( searchRange.first <= searchRange.second ) {
    if ( rowVector.empty() ) {
      RowMapConstIt it = rowMap.begin();
      while ( it != rowMap.end() ) {
        if ( searchRange.first <= (ptrdiff_t)it->first && (ptrdiff_t)it->first <= searchRange.second )
          indices.push_back( it->first );
        ++it;
      }
    }
    else {
      size_t nzCnt = rowVector.size();
      indices.reserve( nzCnt );
      for ( size_t i = 0; i < nzCnt; ++i ) {
        size_t index = nzRange.first + i;
        if ( searchRange.first <= (ptrdiff_t)index && (ptrdiff_t)index <= searchRange.second ) {
          if ( rowVector[i] != defaultElem )
            indices.push_back( index );
        }
      }
    }
    if ( indices.size() > 1 )
      std::sort( indices.begin(), indices.end() );
  }

  return (indices.size() > 0);
}


//------------------------------------------------------------------------------
//
// ---
template <class Type>
inline void SparseRow<Type>::NzUpdate()
{
  bool doUpdate = false;
  std::vector<NumberType> nzElems;
  
  if ( rowVector.empty() ) {
    if ( !rowMap.empty() ) {
      ResetNzRange();

      nzElems.reserve( rowMap.size() );

      RowMapIt it = rowMap.begin();

      while ( it != rowMap.end() )
      {
        if ( it->second == defaultElem ) {
          it = c3d::older_stl_support::erase<RowMap>(rowMap, it );
          doUpdate = true;
        }
        else {
          EnlargeNzRange( it->first );
          nzElems.push_back( std::make_pair( it->first, it->second ) );
          ++it;
        }
      }
    }
  }
  else {
    PRECONDITION( rowMap.empty() );
    size_t nzCnt = rowVector.size();
    nzElems.reserve( nzCnt );

    for ( size_t k = 0; k < nzCnt; k++ ) {
      if ( rowVector[k] != defaultElem )
        nzElems.push_back( std::make_pair( nzRange.first + k, rowVector[k] ) );
      else
        doUpdate = true;
    }
  }

  if ( doUpdate ) {
    SetLine( nzElems );
  }

  PRECONDITION( CheckNzRange() );
}


//------------------------------------------------------------------------------
//
// ---
template <class Type>
inline void SparseRow<Type>::Swap( SparseRow<Type> & r2 )
{
  SparseRow<Type> & r1 = *this;
  std::swap( r1.rowMap, r2.rowMap );
  std::swap( r1.rowVector, r2.rowVector );
  PRECONDITION( r1.rowSize == r2.rowSize );
  std::swap( r1.rowSize, r2.rowSize );
  std::swap( r1.nzRange, r2.nzRange );
}


//------------------------------------------------------------------------------
/** \brief \ru Двумерный разреженный массив объектов.
           \en Two-dimensional sparse array of objects. \~
  \details \ru Двумерный разреженный массив объектов. \n
           \en Two-dimensional sparse array of objects. \n \~
  \ingroup Base_Tools_Containers
*/
// ---
template <class Type>
class SparseArray2 {
public:
  typedef std::pair<ptrdiff_t, ptrdiff_t> NumberRange;
  typedef std::pair<size_t,Type>          NumberType;
  typedef std::vector< SparseRow<Type> >  SparseData;
  typedef SparseArray2<Type>              SparseMatrix;
  
protected :
  SparseData  data;     ///< \ru Данные. \en Data.
  size_t      nColumns; ///< \ru Количество столбцов массива. \en Count of columns of array. 

protected:
  /// \ru Конструктор по заданной размерности. \en The constructor by a given dimension. 
  SparseArray2( size_t lsz, size_t csz ) : data(), nColumns(0) { SetSize( lsz, csz ); }
public:
  /// \ru Конструктор. \en Constructor. 
  SparseArray2() : data(), nColumns(0) {}
  /// \ru Конструктор ограниченной размерности. \en The constructor of restricted dimension. 
  SparseArray2( const uint16 & lsz, const uint16 & csz ) : data(), nColumns(0) { SetSize( lsz, csz ); }
  /// \ru Конструктор копирования. \en Copy-constructor. 
  explicit SparseArray2( const SparseArray2<Type> & src ) : data(), nColumns(0) { Init( src ); }
  /// \ru Деструктор. \en Destructor. 
  virtual ~SparseArray2() { SetSize( 0, 0 ); }

public:
  /// \ru Конструктор по заданной размерности. \en The constructor by a given dimension. 
  static SparseMatrix * Create( size_t lsz, size_t csz );

public: // Общие методы матриц (двумерных массивов)
        size_t          Lines  () const { return data.size(); } ///< \ru Количество строк. \en Count of rows. 
        size_t          Columns() const { return nColumns;    } ///< \ru Количество столбцов. \en Count of columns. 
        size_t          Count  () const { return (data.size()*nColumns); } ///< \ru Количество элементов. \en Count of elements.  
   c3d::IndicesPair     GetSize() const { return c3d::IndicesPair( data.size(), nColumns ); } ///< \ru Дать размер массива. \en Give the size of the array. 
        bool            SetSize( c3d::IndicesPair sz ) { return SetSize( sz.first, sz.second ); } ///< \ru Установить размер. \en Set size. 
        bool            SetSize( size_t  lsz, size_t csz ); ///< \ru Установить размер. \en Set size. 
        bool            SetSize( size_t sz ) { return SetSize( sz, sz ); } ///< \ru Установить размер. \en Set size. 

        /// \ru Получить элемент массива. \en Get an element of the array. 
        const Type &    GetElem( size_t ln, size_t cn ) const;
        /// \ru Установить элемент массива. \en Set an element of the array. 
        bool            SetElem( size_t ln, size_t cn, const Type & );
        /// \ru Оператор доступа по индексам. \en Access by indices operator. 
        const Type &    operator () ( size_t ln, size_t cn ) const { C3D_ASSERT( ln < data.size() && cn < nColumns ); return GetElem( ln, cn ); }
        /// \ru Расписать массив нулями. \en Assign zeros to array. 
        SparseMatrix &  SetZero();
        /// \ru Функция присваивания. \en An assignment function. 
        bool            Init( const SparseMatrix & );
        /// \ru Оператор присваивания. \en The assignment operator. 
        SparseMatrix &  operator = ( const SparseMatrix & src ) { Init( src ); return *this; }
        /// \ru Поменять местами строки. \en Swap lines. 
        bool            SwapLines( size_t ln1, size_t ln2 );

public:
        /// \ru Установить ненулевые элементы строки. \en Set nonzero elements of the row. 
        bool            SetLine( size_t ln, std::vector<NumberType> & );
        /// \ru Индекс первого ненулевого элемента. \en Index of the first nonzero element. 
        size_t          NzBegin( size_t ln ) const { C3D_ASSERT(ln < data.size()); if ( ln < data.size() ) return data[ln].NzBegin(); return SYS_MAX_ST; }
        /// \ru Завершающий индекс последовательности ненулевых элементов. \en The final index of the sequence of non-zero elements. 
        size_t          NzEnd  ( size_t ln ) const { C3D_ASSERT(ln < data.size()); if ( ln < data.size() ) return data[ln].NzEnd(); return SYS_MIN_ST;  }
        /// \ru Получить индексы ненулевых элементов строки. \en Get indices of nonzero elements of the row. 
        bool            NzIndices( size_t ln, const NumberRange & searchRange, std::vector<size_t> & ) const;
        /// \ru Обновить строку (удалить нулевые элементы по возможности). \en Update the row (delete zero elements as far as possible). 
        void            NzUpdate();
};


//------------------------------------------------------------------------------
//
// ---
template <class Type>
inline bool SparseArray2<Type>::SetSize( size_t lsz, size_t csz )
{
  if ( lsz*csz <= c3d::MATRIX_MAX_COUNT )
  {
    data.clear();
    data.resize( lsz );
    for ( size_t k = data.size(); k--; )
      data[k].SetSize( csz );
    nColumns = csz;
    return true;
  }
  return false;
}


//------------------------------------------------------------------------------
//
// ---
template <class Type>
inline SparseArray2<Type> * SparseArray2<Type>::Create( size_t lsz, size_t csz )
{
  if ( lsz*csz <= c3d::MATRIX_MAX_COUNT )
    return new SparseMatrix( lsz, csz );
  return nullptr;
}


//------------------------------------------------------------------------------
//
// ---
template <class Type>
bool SparseArray2<Type>::Init( const SparseArray2<Type> & src )
{
  data.assign( src.data.begin(), src.data.end() );
  nColumns = src.nColumns;
  return true;
}


//------------------------------------------------------------------------------
//
// ---
template <class Type>
inline const Type & SparseArray2<Type>::GetElem( size_t ln, size_t cn ) const
{
  PRECONDITION( ln < data.size() && cn < nColumns );
  if ( ln < data.size() && cn < nColumns )
    return data[ln].GetElem( cn );
  return SparseRow<Type>::defaultElem;
}


//------------------------------------------------------------------------------
//
// ---
template <class Type>
inline bool SparseArray2<Type>::SetElem( size_t ln, size_t cn, const Type & item )
{
  PRECONDITION( ln < data.size() && cn < nColumns );
  if ( ln < data.size() && cn < nColumns )
    return data[ln].SetElem( cn, item );
  return false;
}


//------------------------------------------------------------------------------
//
// ---
template <class Type>
inline SparseArray2<Type> & SparseArray2<Type>::SetZero()
{
  std::for_each( data.begin(), data.end(), std::mem_fn( &SparseRow<Type>::Clear ) );
  return *this;
}


//------------------------------------------------------------------------------
//
// ---
template <class Type>
inline bool SparseArray2<Type>::SetLine( size_t ln, std::vector<NumberType> & newRow )
{
  PRECONDITION( ln < data.size() && newRow.size() <= nColumns );
  if ( ln < data.size() && newRow.size() <= nColumns )
    return data[ln].SetLine( newRow );
  return false;
}


//------------------------------------------------------------------------------
//
// ---
template <class Type>
inline bool SparseArray2<Type>::SwapLines( size_t ln1, size_t ln2 )
{
  size_t nLines = data.size();
  PRECONDITION( ln1 < nLines && ln2 < nLines );
  if ( ln1 < nLines && ln2 < nLines ) {
    data[ln1].Swap( data[ln2] );
    return true;
  }
  return false;
}


//------------------------------------------------------------------------------
//
// ---
template <class Type>
inline bool SparseArray2<Type>::NzIndices( size_t ln, const NumberRange & searchRange, std::vector<size_t> & indices ) const
{
  PRECONDITION( ln < data.size() );
  if ( ln < data.size() )
    return data[ln].NzIndices( searchRange, indices );
  return false;
}


//------------------------------------------------------------------------------
//
// ---
template <class Type>
inline void SparseArray2<Type>::NzUpdate()
{
  std::for_each( data.begin(), data.end(), std::mem_fn( &SparseRow<Type>::NzUpdate ) );
}


#endif // __TEMPL_SPARSE_ARRAY2_H
