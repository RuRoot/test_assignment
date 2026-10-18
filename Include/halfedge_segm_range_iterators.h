////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Итераторы для использования range-based-for в Halfedge сегментации.
         \en Iterators with range-based-for support to use in a HalfEdge segmentation class \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __HALFEDGE_SEGM_RANGE_ITERATORS_H
#define __HALFEDGE_SEGM_RANGE_ITERATORS_H

#include <halfedge.h>

class MbHalfedgeSegmentation;


/** \brief \ru Итератор по ребрам сегментации для поддержки range-based for.
           \en Subregion iterator for haldedge range based for support
*/
struct MATH_CLASS ActiveEdgeSegmConstIterator
{
  const MbHalfedgeSegmentation & _segm;             //< \ru Ссылка на сегментацию. \en Segmentation reference.
  size_t                         _edge = SYS_MAX_T; //< \ru Индекс текущего segmentation cycle. \en An actual segmentation edge index.

  /** \brief \ru Конструктор. \en Constructor.
    \param[in] segm - \ru Сегментация. \en Halfedge segmentation.
    \param[in] begin - \ru Если true создает начальный итератор, иначе - конечный.
                       \en If true creates begin iterator, creates end iterator otherwise.
  */
  ActiveEdgeSegmConstIterator( const MbHalfedgeSegmentation & segm, bool begin = true ) noexcept;

  /// \ru Оператор инкремента. \en An increment operator.
  ActiveEdgeSegmConstIterator & operator ++() noexcept;
  /// \ru Оператор сравнения. \en A comparison operator.
  bool operator != ( const ActiveEdgeSegmConstIterator & it ) const noexcept;
  /// \ru Доступ к данным. \en Data access.
  const MbHalfedgeEdge & operator*() const noexcept;
};


/** \brief \ru Объект хранящий begin() и end() для итерации по циклам сегментации для поддержки range-based for.
           \en Storage for begin-end pair to support halfedge range-based for iteration
*/
struct MATH_CLASS ActiveEdgesSegmBegEndPair
{
  const MbHalfedgeSegmentation & _segm; //< \ru Ссылка на сегментацию. \en Segmentation reference.

  /// \ru Констуктор. \en Constructor.
  ActiveEdgesSegmBegEndPair( const MbHalfedgeSegmentation & segm ) noexcept
    : _segm( segm )
  {
  }

  /// \ru Начальный итератор. \en Begin iterator.
  ActiveEdgeSegmConstIterator begin() const noexcept
  {
    return ActiveEdgeSegmConstIterator( _segm );
  }

  /// \ru Конечный итератор. \en End Iterator.
  ActiveEdgeSegmConstIterator end() const noexcept
  {
    return ActiveEdgeSegmConstIterator( _segm, false );
  }
};


/** \brief \ru Итератор по вершинам сегментации для поддержки range-based for.
           \en Subregion iterator for haldedge range based for support
*/
struct MATH_CLASS ActiveVertexSegmConstIterator
{
  const MbHalfedgeSegmentation & _segm;                //< \ru Ссылка на сегментацию. \en Segmentation reference.
  size_t                         _vertex = SYS_MAX_T; //< \ru Индекс текущего segmentation cycle. \en An actual segmentation edge index.

  /** \brief \ru Конструктор. \en Constructor.
    \param[in] segm - \ru Сегментация. \en Halfedge segmentation.
    \param[in] begin - \ru Если true создает начальный итератор, иначе - конечный.
                       \en If true creates begin iterator, creates end iterator otherwise.
  */
  ActiveVertexSegmConstIterator( const MbHalfedgeSegmentation & segm, bool begin = true ) noexcept;

  /// \ru Оператор инкремента. \en An increment operator.
  ActiveVertexSegmConstIterator & operator ++() noexcept;
  /// \ru Оператор сравнения. \en A comparison operator.
  bool operator != ( const ActiveVertexSegmConstIterator & it ) const noexcept;
  /// \ru Доступ к данным. \en Data access.
  const MbHalfedgeVertex & operator*() const noexcept;
};


/** \brief \ru Объект хранящий begin() и end() для итерации по вершинам сегментации для поддержки range-based for.
           \en Storage for begin-end pair to support halfedge range-based for iteration
*/
struct MATH_CLASS ActiveVertexesSegmBegEndPair
{
  const MbHalfedgeSegmentation & _segm; //< \ru Ссылка на сегментацию. \en Segmentation reference.

  /// \ru Констуктор. \en Constructor.
  ActiveVertexesSegmBegEndPair( const MbHalfedgeSegmentation & segm ) noexcept
    : _segm( segm )
  {
  }

  /// \ru Начальный итератор. \en Begin iterator.
  ActiveVertexSegmConstIterator begin() const noexcept
  {
    return ActiveVertexSegmConstIterator( _segm );
  }

  /// \ru Конечный итератор. \en End Iterator.
  ActiveVertexSegmConstIterator end() const noexcept
  {
    return ActiveVertexSegmConstIterator( _segm, false );
  }
};


/** \brief \ru Итератор по циклам сегментации для поддержки range-based for.
           \en Subregion iterator for haldedge range based for support
*/
struct MATH_CLASS ActiveLoopSegmConstIterator
{
  const MbHalfedgeSegmentation & _segm;                 //< \ru Ссылка на сегментацию. \en Segmentation reference.
  size_t                         _iLoopSegm = SYS_MAX_T; //< \ru Индекс текущего segmentation cycle. \en An actual segmentation cycle index.

  /** \brief \ru Конструктор. \en Constructor.
    \param[in] segm - \ru Сегментация. \en Halfedge segmentation.
    \param[in] begin - \ru Если true создает начальный итератор, иначе - конечный.
                       \en If true creates begin iterator, creates end iterator otherwise.
  */
  ActiveLoopSegmConstIterator( const MbHalfedgeSegmentation & segm, bool begin = true ) noexcept;

  /// \ru Оператор инкремента. \en An increment operator.
  ActiveLoopSegmConstIterator & operator ++() noexcept;
  /// \ru Оператор сравнения. \en A comparison operator.
  bool operator != ( const ActiveLoopSegmConstIterator & it ) const noexcept;
  /// \ru Доступ к данным. \en Data access.
  const MbSegmentationLoop & operator*() const noexcept;
};


/** \brief \ru Объект хранящий begin() и end() для итерации по циклам сегментации для поддержки range-based for.
           \en Storage for begin-end pair to support halfedge range-based for iteration
*/
struct MATH_CLASS ActiveLoopsSegmBegEndPair
{
  const MbHalfedgeSegmentation & _segm; //< \ru Ссылка на сегментацию. \en Segmentation reference.

  /// \ru Констуктор. \en Constructor.
  ActiveLoopsSegmBegEndPair( const MbHalfedgeSegmentation & segm ) noexcept
    : _segm( segm )
  {
  }

  /// \ru Начальный итератор. \en Begin iterator.
  ActiveLoopSegmConstIterator begin() const noexcept
  {
    return ActiveLoopSegmConstIterator( _segm );
  }

  /// \ru Конечный итератор. \en End Iterator.
  ActiveLoopSegmConstIterator end() const noexcept
  {
    return ActiveLoopSegmConstIterator( _segm, false );
  }
};


/** \brief \ru Итератор по подрегионам сегментации для поддержки range-based for.
           \en Subregion iterator for haldedge range based for support
*/
struct MATH_CLASS ActiveSubRegionConstIterator
{
  const MbHalfedgeSegmentation & _segm;                   //< \ru Ссылка на сегментацию. \en Segmentation reference.
  size_t                         _iSubregion = SYS_MAX_T; //< \ru Индекс текущего подрегиона. \en An actual subregion index.

  /** \brief \ru Конструктор. \en Constructor.
    \param[in] segm - \ru Сегментация. \en Halfedge segmentation.
    \param[in] begin - \ru Если true создает начальный итератор, иначе - конечный.
                       \en If true creates begin iterator, creates end iterator otherwise.
  */
  ActiveSubRegionConstIterator( const MbHalfedgeSegmentation & segm, bool begin = true ) noexcept;

  /// \ru Оператор инкремента. \en An increment operator.
  ActiveSubRegionConstIterator & operator ++() noexcept;
  /// \ru Оператор сравнения. \en A comparison operator.
  bool operator != ( const ActiveSubRegionConstIterator & it ) const noexcept;
  /// \ru Доступ к данным. \en Data access.
  const MbSegmentationSubRegion & operator*() const noexcept;
};


/** \brief \ru Объект хранящий begin() и end() для итерации по подрегионам для поддержки range-based for.
           \en Storage for begin-end pair to support halfedge range-based for iteration
*/
struct MATH_CLASS ActiveSubRegionsBegEndPair
{
  const MbHalfedgeSegmentation & _segm; //< \ru Ссылка на сегментацию. \en Segmentation reference.

  /// \ru Констуктор. \en Constructor.
  ActiveSubRegionsBegEndPair( const MbHalfedgeSegmentation & segm ) noexcept
    : _segm( segm )
  {
  }

  /// \ru Начальный итератор. \en Begin iterator.
  ActiveSubRegionConstIterator begin() const noexcept
  {
    return ActiveSubRegionConstIterator( _segm );
  }

  /// \ru Конечный итератор. \en End Iterator.
  ActiveSubRegionConstIterator end() const noexcept
  {
    return ActiveSubRegionConstIterator( _segm, false );
  }
};


/** \brief \ru Итератор по дрегионам сегментации для поддержки range-based for.
           \en Subregion iterator for haldedge range based for support
*/
struct MATH_CLASS ActiveRegionConstIterator
{
  const MbHalfedgeSegmentation & _segm;                //< \ru Ссылка на сегментацию. \en Segmentation reference.
  size_t                         _iRegion = SYS_MAX_T; //< \ru Индекс текущего региона. \en An actual region index.

  /** \brief \ru Конструктор. \en Constructor.
    \param[in] segm - \ru Сегментация. \en Halfedge segmentation.
    \param[in] begin - \ru Если true создает начальный итератор, иначе - конечный.
                       \en If true creates begin iterator, creates end iterator otherwise.
  */
  ActiveRegionConstIterator( const MbHalfedgeSegmentation & segm, bool begin = true ) noexcept;

  /// \ru Оператор инкремента. \en An increment operator.
  ActiveRegionConstIterator & operator ++() noexcept;
  /// \ru Оператор сравнения. \en A comparison operator.
  bool operator != ( const ActiveRegionConstIterator & it ) const noexcept;
  /// \ru Доступ к данным. \en Data access.
  const MbHalfedgeFacet & operator*() const noexcept;
};


/** \brief \ru Объект хранящий begin() и end() для итерации по регионам для поддержки range-based for.
           \en Storage for begin-end pair to support halfedge range-based for iteration
*/
struct MATH_CLASS ActiveRegionsBegEndPair
{
  const MbHalfedgeSegmentation & _segm; //< \ru Ссылка на сегментацию. \en Segmentation reference.

  /// \ru Констуктор. \en Constructor.
  ActiveRegionsBegEndPair( const MbHalfedgeSegmentation & segm ) noexcept
    : _segm( segm )
  {
  }

  /// \ru Начальный итератор. \en Begin iterator.
  ActiveRegionConstIterator begin() const noexcept
  {
    return ActiveRegionConstIterator( _segm );
  }

  /// \ru Конечный итератор. \en End Iterator.
  ActiveRegionConstIterator end() const noexcept
  {
    return ActiveRegionConstIterator( _segm, false );
  }
};

#endif // __HALFEDGE_SEGM_RANGE_ITERATORS_H