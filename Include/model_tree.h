//////////////////////////////////////////////////////////////////////////////////////////
/** \file 
    \brief  \ru Реализация дерева модели 
            \en Implementation of Model Tree classes \~
*/
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __MODEL_TREE_H
#define __MODEL_TREE_H


#include <io_tree.h>
#include <model_tree_data.h>
#include <templ_s_array.h>
#include <stack>
#include <vector>
#include <set>
#include <map>
#include <mb_variables.h>

//----------------------------------------------------------------------------------------
// \ru Реализация интерфейсов дерева модели. \en Implementation of Model Tree interfaces.
//----------------------------------------------------------------------------------------

namespace c3d // namespace C3D
{
//----------------------------------------------------------------------------------------
/** \brief   \ru Узел дерева.
             \en Tree node. \~
    \details \ru Узел дерева (может иметь несколько потомков). \n
             \en Tree node (can have several children). \n \~
    \ingroup Base_Tools_IO
*/
// ---
class MATH_CLASS MbTreeNode : public IModelTreeNode
{
  // \ru Временное хранилище для индексов потомков узла (используется при чтении узла).
  // \en Temporary storage for indices of child nodes (used while reading the node).
  std::vector<size_t> m_childrenIndices;
  // \ru Данные узла \en The node data.
  MbItemData m_data;
  // \ru Флаг, указывающий, открыт ли узел при проходе вглубь по дереву.
  // \en Flag which indicates whether the node is entered during traversing into depth over the tree.
  mutable bool m_open;
  // \ru Флаг, указывающий, читать ли только часть узла.
  // \en Flag which indicates whether to read only a part of the node.
  mutable bool m_partial;
public:
  MbTreeNode() : m_open(false), m_partial(false) {}
  MbTreeNode ( const MbItemData& data ) : m_data(data), m_open(false), m_partial(false) {}
  MbTreeNode ( const MbTreeNode& node ) : m_data(node.m_data), m_childrenIndices(node.m_childrenIndices), 
    m_open(node.m_open), m_partial(node.m_partial) { GetChildren() = node.GetChildren(); GetParents() = node.GetParents(); }

  ///---------
  /// \ru Методы IModelTreeNode. \en IModelTreeNode methods. 

  /// \ru Доступ к данным узла. \en Access to the node data. 
  virtual MbItemData& GetData() { return m_data; }
  virtual const MbItemData& GetData() const { return m_data; }

  /// \ru Доступ к позиции чтения/записм узла. \en Access to the node read/write position. 
  virtual ClusterReference& GetPosition() { return m_data.position; }
  virtual const ClusterReference& GetPosition() const { return m_data.position; }

  /// \ru Узнать, читать ли только часть узла.
  /// \en Check whether to read the node partially.
  virtual bool PartialRead() const { return m_partial; };

  /// \ru Установить признак частичного или полного чтения узла.
  /// \en Set indication of full or partial node reading.
  virtual void SetPartialRead ( bool partial ) const { m_partial = partial; };

  /// \ru Записать узел. \en Write the node. 
  virtual writer &   operator >> ( writer & );

  /// \ru Прочитать узел. \en Read the node. 
  virtual reader &   operator << ( reader & );

  /// \ru Доступ ко все потомкам узла. \en Access to the all descendants of the node. 
  void GetAllDescendants ( std::set<const IModelTreeNode*>& nodes ) const;

  /// \ru Создать узел с данными текущего узла и добавить его в дерево.
  ///     Рекурсивно скопировать в дерево всех предков текущего узла с сохранением иерархии.
  /// \en Create a node with data from the current node. 
  ///     Copy recursively all parents of the node to the tree preserving the hierarchy.
  MbTreeNode* CopyToTreeWithParents ( c3d::IModelTree* tree, bool partial ) const;

  /// \ru Создать узел с данными текущего узла и добавить его в дерево.
  ///     Рекурсивно скопировать в дерево всех потомков текущего узла с сохранением иерархии.
  /// \en Create a node with data from the current node. 
  ///     Copy recursively all children of the node to the tree preserving the hierarchy.
  MbTreeNode* CopyToTreeWithChildren ( c3d::IModelTree* tree, bool partial ) const;

  /// \ru Доступ к флагу, который указывет, открыт ли узел при проходе вглубь по дереву
  ///     (false означает, что узел и его потомки уже пройдены или еще не обнаружены).
  /// \en Access to the flag which indicates whether the node is entered during traversing into depth over the tree
  ///     (false - means that the node and its children are already leaved or are not met yet).
  bool IsOpen() const { return m_open; }
  void SetOpen ( bool open ) { m_open = open; }
  void SetOpen ( bool open ) const { m_open = open; }

  // \ru Равенство определяется по id объекта MbItemData.
  // \en Equality is defined by id field of MbItemData object.
  bool operator == ( const MbTreeNode& node2 ) const
  {
    if ( !( GetData() == node2.GetData() ) ||
      GetData().id != node2.GetData().id )
      return false;
    return true;
  }

  // \ru Сравнение по полям объектов MbItemData.
  // \en Comparison of filtering fields of MbItemData objects.
  bool operator < ( const MbTreeNode& node2 ) const
  {
    return m_data < node2.m_data;
  }

  friend class MbModelTree;

};

//----------------------------------------------------------------------------------------
/** \brief \ru Узел дерева исполнений.
           \en Embodiments tree node. \~
  \details \ru Узел дерева исполнений (может иметь несколько потомков).
           \en Embodiments tree node (can have several children).
  \ingroup Base_Tools_IO
*/
// ---
class MATH_CLASS MbEmbodimentNode : public IEmbodimentNode
{
  const MbTreeNode * m_subtree;
public:
  MbEmbodimentNode( const MbTreeNode* node ) : IEmbodimentNode(), m_subtree( node ) {}
  ~MbEmbodimentNode() {}

  // \ru Выдать узел дерева модели, соответствующий данному исполнению.
  // \en Get a model tree node which corresponds to a given embodiment.
  virtual const IModelTreeNode * GetModelTreeNode() const { return m_subtree; }

  // \ru Доступ к информации об исполнении. \en Access to the embodiment info. 
  virtual const MbItemData& GetEmbodimentData() const { C3D_ASSERT( m_subtree != nullptr ); return m_subtree->GetData(); }

  // \ru Построить дерево модели, которое содержится в данном исполнении.
  // \en Build a tree of a model which is contained in a given embodiment.
  virtual std::unique_ptr<const IModelTree> GetEmbodiment() const;

private:
  MbEmbodimentNode();
  MbEmbodimentNode( const MbEmbodimentNode & emb );
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Дерево геометрической модели.
             \en Tree of geometric model. \~
    \details \ru Дерево геометрической модели. (может иметь несколько корней).
             \en Tree of geometric model (can have several roots). \n \~
    \ingroup Base_Tools_IO
*/
// ---
class MATH_CLASS MbModelTree : public IModelTree
{
private:
  // \ru Временное хранилище для индексов корней дерева (используется при чтении дерева).
  // \en Temporary storage for indices of the tree roots (used while reading the tree).
  std::vector<size_t> m_rootsIndices;
  // \ru Все узлы дерева, упорядоченные по данным.
  // \en All nodes of the tree, ordered by data.
  std::set<MbTreeNode> m_filteredNodes;
  // \ru Стек узлов, открытых при чтении/записи дерева.
  // \en Stack of nodes opened during reading/writing the tree.
  std::stack<MbTreeNode*> m_nestedNodesStack;
  // \ru Все узлы дерева, упорядоченные по ID. 
  // \en All nodes of the tree, ordered by ID.
  std::map<size_t, MbTreeNode*> m_indexToNode; // \ru Вспомогательный массив. \en Auxiliary map.
  VERSION                       m_currentVersion;
  IEmbodimentTree               m_embTree;    // \ru Дерево исполнений. \en Embodiment tree.
public:

  // \ru Конструктор. \en Constructor.
  MbModelTree();

  // \ru Деструктор. \en Destructor.
  virtual ~MbModelTree();

  ///---------
  /// \ru Методы IModelTreeNode. \en IModelTreeNode methods. 

  /// \ru Создать узел по данными и добавить в дерево. \en Create a node by data and add to the tree. 
  virtual void AddNode ( const TapeBase* mem, const ClusterReference& ref );

  /// \ru Закрыть узел (удалить узел из стека, так что родительский узел станет текущим).
  /// \en Close the node (remove it from the stack so that its parent becomes the current node). 
  virtual void CloseNode ( const TapeBase* mem );

  // \ru Построить дерево из узлов, выбранных по фильтрам. В случае дерева исполнений, функция работает с первым исполнением.
  //  \en Build a tree with nodes, selected by filters. In case of embodiment tree, the function works with the first embodiment.
  virtual std::unique_ptr<const IModelTree> GetFilteredTree ( const std::vector<MbItemData>& filters ) const;

  // \ru Построить дерево по заданным узлам. Не применимо для дерева исполнений (в этом случае возвращает nullptr).
  //  \en Build a tree for given nodes. Not applicable to embodiment tree (in this case, returns nullptr).
  virtual std::unique_ptr<const IModelTree> GetFilteredTree ( std::vector<const IModelTreeNode*>& nodes ) const;

  // \ru Выдать указатель на дерево исполнений. Выдает nullptr, если не применимо (нет исполнений).
  // \en Get pointer to embodiments tree. Return nullptr if not applicable (no embodiments).
  virtual const IEmbodimentTree* GetEmbodimentsTree() const { return GetType() == mtt_Embodiment ? &m_embTree : nullptr; }

  /// \ru Версия дерева. \en Tree version.
  virtual VERSION GetVersion() { return m_currentVersion; }
  virtual void SetVersion( VERSION version ) { m_currentVersion = version; }

  /// \ru Записать дерево. \en Write the tree. 
  virtual writer &   operator >> ( writer & );

  /// \ru Прочитать дерево. \en Read the tree. 
  virtual reader &   operator << ( reader & );

  ///---------

  /// \ru Добавить узел с данными из указанного узла, если узел с такими данными не существует.
  ///     \param node  - узел с данными.
  ///     \param added - заполняется, если ненулевой (true - узел добавлен, false - узел уже существует).
  ///     \return      - возвращает указатель на узел дерева.
  /// \en Add a node with the data from the given node if a node with such data does not exist.
  ///     \param node  - a node with data.
  ///     \param added - filled if non-null (true - if a node added, false - a node already exists).
  ///     \return      - a pointer to the tree node.
  MbTreeNode* AddNode ( const MbTreeNode& node, bool* added = nullptr );

   /// \ru Добавить узел с указанными данными, если узел с такими данными не существует.
  ///     \param node  - данные.
  ///     \param added - заполняется, если ненулевой (true - узел добавлен, false - узел уже существует).
  ///     \return      - возвращает указатель на узел дерева.
  /// \en Add a node with the given if a node with such data does not exist.
  ///     \param node  - a data.
  ///     \param added - filled if non-null (true - if a node added, false - a node already exists).
  ///     \return      - a pointer to the tree node.
  MbTreeNode* AddNode ( const MbItemData& data, bool* added = nullptr );

 /// \ru Доступ к узлам дерева, упорядоченным по данным.
  /// \en Access to nodes of the tree, ordered by data.
        std::set<MbTreeNode>& GetFilteredNodes()       { return m_filteredNodes; }
  const std::set<MbTreeNode>& GetFilteredNodes() const { return m_filteredNodes; }

  // \ru Добавить в корень текущего дерева указанное поддерево. \en Add a given subtree to the current tree root.
  const void AddSubtree( IModelTree* tree, const IModelTreeNode* node ) const;

  /// \ru Заполнить массив корней дерева. 
  /// \en Fill the tree roots. 
  void FillRoots();

protected:
  /// \ru Добавить ветвь в дерево:
  ///     \param branch - листовой узел с ветвью дерева, ведущей к нему, начиная с корневого узла дерева;
  ///     \param partial - определяет тип чтения листового узла (частичное или полное).
  /// \en Add a branch to the tree:
  ///     \param branch - a leaf node with the tree branch, leading to it;
  ///     \param partial - defines partial or full read of the leaf node.
  void AddBranch( const NodeBranch& branch, bool partial );

  /// \ru Получить уникальные узлы для данного набора узлов. Проходятся все заданные узлы и исключаются те,
  ///     которые являются потомками заданных узлов (и будут прочитаны, как их часть).
  ///     Таким образом, результат будет содержать узлы поддеревьев, содержащих все заданные узлы.
  /// \en Get unique nodes for given set of nodes. Walk through the given nodes and exclude nodes,
  ///     which are children of other given nodes (and will be read as a part of them).
  ///     Thus, the result set of nodes will represent the roots of subtrees containing all given nodes.
  std::vector<const IModelTreeNode*> GetUniqueNodes ( std::vector<const IModelTreeNode*>& nodes ) const;


  // \ru Построить дерево по заданным узлам без проверки типа.
  //  \en Build a tree for given nodes without type check.
  const IModelTree* GetFilteredTreeEx( std::vector<const IModelTreeNode*>& nodes ) const;

  /// \ru Построить дерево по индексам (используется при чтении дерева). 
  /// \en Build the tree using indices (used during reading the tree). 
  void BuildTree();

private:
  OBVIOUS_PRIVATE_COPY(MbModelTree)
};

//----------------------------------------------------------------------------------------
/// \ru Операторы для записи дерева в xml формате.
/// \en Operators for outputing a tree to xml.
// ---

//----------------------------------------------------------------------------------------
/// \ru Запись узла дерева в xml формате. \en Tree node writing to xml.
// ---
MATH_FUNC( c3d::t_ofstream& ) operator << ( c3d::t_ofstream& file, const IModelTreeNode& node );

//----------------------------------------------------------------------------------------
/// \ru Запись узла дерева в xml формате. \en Tree node writing to xml.
// ---
MATH_FUNC( c3d::t_ofstream& ) operator << ( c3d::t_ofstream& file, IModelTreeNode& node );

//----------------------------------------------------------------------------------------
/// \ru Запись дерева в xml формате. \en Tree writing to xml.
// ---
MATH_FUNC( c3d::t_ofstream& ) operator << ( c3d::t_ofstream& file, const IModelTree& tree );

//----------------------------------------------------------------------------------------
/// \ru Запись дерева в xml формате. \en Tree writing to xml.
// ---
MATH_FUNC( c3d::t_ofstream& ) operator << ( c3d::t_ofstream& file, IModelTree& tree );

} //namespace c3d

#endif // __MODEL_TREE_H
