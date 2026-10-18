////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Итераторы доступа к сегментации полигонального геометрического объекта.
         \en Access iterators to the polygonal geometric object's segmentation. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __HALFEDGE_SEGM_ITERATORS_H
#define __HALFEDGE_SEGM_ITERATORS_H

#include <math_define.h>

class  MbPolymeshTopo;


/** \brief \ru Итератор по ребрам цикла региона.
           \en Region's edge loop iterator.
  */
class MATH_CLASS EdgeLoopIterator
{
private:
  const MbPolymeshTopo & _segmentation;       //< \ru Ссылка на сегментацию. \en Segmentation referene.
  bool                   _bFirst;             //< \ru Признак того, что не нужно проверять окончание итерации. \en If true there is no need to check the end of the iteration.
  size_t                 _edge;               //< \ru Индекс текущего ребра. \en The current edge id.
  size_t                 _edge0;              //< \ru Индекс начального ребра для проверки окончания итерации. \en Start edge for the end of iteration check.
  bool                   _bExcludeInnerEdges; //< \ru Учитывать ли внутренние ребра при итерации. \en Exclude non-boundary edges during the iteration.

public:
  /** \brief \ru Конструктор. \en Constructor.
    \param[in] segm - \ru Сегментация. \en Segmentation.
    \param[in] iRegion - \ru Индекс региона. \en Region id.
    \param[in] iLoop - \ru Индекс цикла в регионе. \en Region's loop id.
  */
  EdgeLoopIterator( const MbPolymeshTopo & segm, size_t iLoop );
  /** \brief \ru Конструктор. \en Constructor.
    \param[in] segm - \ru Сегментация. \en Segmentation.
    \param[in] iRegion - \ru Индекс региона. \en Region id.
    \param[in] iLoop - \ru Индекс цикла в регионе. \en Region's loop id.
    \param[in] bExcludeInnerEdges - \ru Учитывать ли внутренние ребра при итерации. \en Exclude non-boundary edges during the iteration.
  */
  EdgeLoopIterator( const MbPolymeshTopo & segm, size_t iLoop, bool bExcludeInnerEdges );
  /// \ru Деструктор. \en Destructor.
  ~EdgeLoopIterator() {}

  /// \ru Сдвинуть итератор на следующее ребро региона. \en Increment operator moves iterator to the next edge in the loop.
  void  operator ++();
  /// \ru Получить индекс текущего ребра. \en Get the current edge.
  size_t Edge() const
  {
    return _edge;
  }
  /// \ru Закончен ли обход. \en Check if all edges in the loop were visited during iteration.
  bool Done() const
  {
    if ( _edge == SYS_MAX_T )
      return true;
    return _bFirst ? false
      : _edge == _edge0;
  }

  OBVIOUS_PRIVATE_COPY( EdgeLoopIterator );
};


/** \brief \ru Итератор по сеточным ребрам ребра сегментации.
           \en Segmentation mesh edge iterator. 
*/
class MATH_CLASS EdgeEdgesIterator
{
private:
  const MbPolymeshTopo & _segmentation;       //< \ru Ссылка на сегментацию. \en Segmentation reference.
  c3d::IndicesPair       _begEnd;             //< \ru Первое и последнее сеточные ребра ребра сегментации. \en The first and the last mesh edges in the segmentation edge.
  size_t                 _edge;               //< \ru Индекс текущего ребра. \en The current mesh edge id.
  bool                   _bReverse;           //< \ru Признак обхода ребер с конца ребра сегментации (true) или с начала (false). \en The iteration direction. If true the iterator is reverse iterator.

public:
  /** \brief \ru Конструктор. \en Constructor.
    \param[in] segm - \ru Сегментация. \en Segmentation.
    \param[in] iEd - \ru Индекс ребра сегментации. \en Segmentation edge id.
  */
  EdgeEdgesIterator( const MbPolymeshTopo & segm, size_t iEd, bool bReverse = false );
  /// \ru Деструктор. \en Destructor.
  ~EdgeEdgesIterator() {}

  /// \ru Сдвинуть итератор на следующее сеточное ребро ребра сегментации. \en Increment operator moves iterator to the next mesh edge in the segmentation edge.
  void  operator ++();
  /// \ru Получить индекс текущего ребра. \en Get current mesh edge.
  size_t Edge() const { return _edge; }
  /// \ru Закончен ли обход. \en Check if all mesh edges in the segmentation edge were visited during iteration.
  bool Done() const { return _edge == SYS_MAX_T; }

  OBVIOUS_PRIVATE_COPY( EdgeEdgesIterator );
};


/** \brief \ru Итератор по подрегионам региона.
           \en Segmentation subregion iterator for the region.
*/
class MATH_CLASS SubregionRegionIterator
{
private:
  const MbPolymeshTopo & _segmentation; //< \ru Ссылка на атрибут сегментации. \en Segmentation attribute reference.
  size_t                 _subreg;       //< \ru Текущий подрегион региона. \en The current subregion for the region.

public:
  /** \brief \ru Конструктор. \en Constructor.
    \param[in] segm - \ru Сегментация. \en Segmentation.
    \param[in] reg - \ru Регион. \en Region.
  */
  SubregionRegionIterator( const MbPolymeshTopo & segm, size_t reg );
  /// \ru Деструктор. \en Destructor.
  ~SubregionRegionIterator() {}

  /// \ru Сдвинуть итератор на следующий подрегион. \en Increment operator moves iterator to the next subregion.
  void  operator ++();
  /// \ru Получить индекс текущего подрегиона. \en Get the current subregion.
  size_t Subregion() const { return _subreg; }
  /// \ru Закончен ли обход. \en Check if all subregions are visited.
  bool Done() const { return _subreg == SYS_MAX_T; }

  OBVIOUS_PRIVATE_COPY( SubregionRegionIterator );
};


/** \brief \ru Итератор по циклам подрегиона.
           \en Subregion's loop iterator.
*/
class MATH_CLASS LoopSubregionIterator
{
private:
  const MbPolymeshTopo & _segmentation; //< \ru Ссылка на атрибут сегментации. \en Segmentation attribute reference.
  size_t                 _loop;         //< \ru Текущий цикл подрегиона. \en The current subregion's loop.

public:
  /** \brief \ru Конструктор. \en Constructor.
    \param[in] segm - \ru Сегментация. \en Segmentation.
    \param[in] subreg - \ru Подрегион. \en Subregion.
  */
  LoopSubregionIterator( const MbPolymeshTopo & segm, size_t subreg );
  /// \ru Деструктор. \en Destructor.
  ~LoopSubregionIterator() {}

  /// \ru Сдвинуть итератор на следующий цикл. \en Increment operator moves iterator to the next loop.
  void  operator ++();
  /// \ru Получить индекс текущего цикла. \en Get current segmentation loop.
  size_t Loop() const { return _loop; }
  /// \ru Закончен ли обход. \en Check if all subregion's loops are visited.
  bool Done() const { return _loop == SYS_MAX_T; }

  OBVIOUS_PRIVATE_COPY( LoopSubregionIterator );
};


#endif // __HALFEDGE_SEGM_ITERATORS_H
