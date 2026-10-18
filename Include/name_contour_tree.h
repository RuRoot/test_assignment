////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Дерево именованых контуров.
         \en The tree of named contours. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __NAME_CONTOUR_TREE_H
#define __NAME_CONTOUR_TREE_H


#include <cur_contour.h>
#include <map>
#include <mb_operation_result.h>


//------------------------------------------------------------------------------   
/** \brief \ru Дерево именованных контуров.
           \en The tree of named contours. \~
  \details \ru Дерево именованных контуров.\n
           \en The tree of named contours.\n \~
  \ingroup Names
*/
// ---
class MATH_CLASS MbNamedContoursTree {
private:
  const MbContour *           contour;            // \ru Самый внешний контур \en The most external contour 
  bool                        own;                // \ru Владение контуром \en Ownership of contour 
  PArray<MbNamedContoursTree> children;           // \ru Внутренние контуры \en All inner contours 
  bool                        intersectChildren;  // \ru Пересекаются ли внутренние контуры \en Are inner contours intersect
public:
  /// \ru Конструктор. \en Constructor. 
  MbNamedContoursTree( const MbContour * con = nullptr, bool o = true );
  /// \ru Деструктор. \en Destructor. 
  ~MbNamedContoursTree();
public:
  /// \ru Сформировать дерево. \en Form the tree. 
  void          FillTree ( RPArray<MbContour> & comContours, double eps, VERSION version );
  /// \ru Получить количество деревьев. \en Get count of trees. 
  size_t        GetChildrenCount() const { return children.Count(); }
  /// \ru Получить дерево контуров по индексу. \en Get contour tree by an index. 
  const MbNamedContoursTree * GetTreeContour( size_t index ) const { return (GetChildrenCount() >= index) ? children[index] : nullptr; }
  /// \ru Проверить группы контуров на не пересечение. \en Check groups of contours for absence of intersection. 
  MbResultType  CheckProfiles( bool base ) const;
  /// \ru Получить указатель на внешний контур. \en Get the pointer to the external contour. 
  const MbContour * GetContour() const { return contour; }
  // \ru Пересекаются ли внутренние контуры \en Are inner contours intersect
  bool AreChildrenIntersect() const { return intersectChildren; }

private:
  void          FillItem( const RPArray<MbContour> & sortContours, double eps, bool contoursEqual, VERSION version );
  void          AddChild( const MbContour &, const RPArray<MbContour> &, size_t, double eps, bool contoursEqual, VERSION version ); // \ru добавить элемент в дерево \en add element to the tree 
  bool          IsExistContour( const MbContour & ) const; // \ru существует в дереве такой контур \en whether there is such a contour in the tree 
  void          GetCountInOneNode( const MbNamedContoursTree & tCont, size_t & countTmp ) const;
  void          SortTree( const std::map<const MbContour *, size_t> & contoursIndexes );
private:
  MbNamedContoursTree( const MbNamedContoursTree & ); // \ru не реализовано \en not implemented 
  void          operator = ( const MbNamedContoursTree & ); // \ru не реализовано \en not implemented 
};


//------------------------------------------------------------------------------
/** \brief \ru Выдать самый большой контур.
           \en Get the biggest contour. \~
  \details \ru Выдать самый большой контур по длине диагонали габарита.
           \en Get the biggest contour by bounding box diagonal length. \~
  \return \ru Возвращает указатель на найденный контур или nullptr.
          \en Returns pointer to the found contour or nullptr. \~
  \ingroup Names
*/
// ---
MATH_FUNC (MbContour *) FindBigContour( const RPArray<MbContour> & contours );


#endif // __NAME_CONTOUR_TREE_H
