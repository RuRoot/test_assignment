////////////////////////////////////////////////////////////////////////////////
/**
\file
\brief \ru Дерево геометрической модели.
       \en Tree of geometric model. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __IO_TREE_H
#define __IO_TREE_H

#include <set>
#include <vector>
#include <math_define.h>
#include <tool_cstring.h>

class reader;
class writer;
struct ClusterReference;
class TapeBase;

namespace c3d // namespace C3D
{

//----------------------------------------------------------------------------------------
// \ru Предварительное объявление структуры для данных узла дерева.
// \en The forward declaration of a structure for the tree node data. \~
// ---
struct MbItemData;

//----------------------------------------------------------------------------------------
/** \brief   \ru Узел дерева модели.
             \en Model tree node. \~
    \details \ru Узел дерева модели (может иметь несколько потомков). 
                 Умеет записывать в поток и читаться из потока. \n
             \en Model tree node (can have several children). 
                 Can be written to a stream and read from a stream. \n \~
    \ingroup Base_Tools_IO
*/
// ---
class MATH_CLASS IModelTreeNode
{
protected:
  // \ru Непосредственные потомки узла. \en The immediate children of the node. 
  std::set<const IModelTreeNode*> m_children;
  // \ru Непосредственные предки узла. \en The immediate parents of the node. 
  std::set<const IModelTreeNode*> m_parents;
public:
  IModelTreeNode() {}
  virtual ~IModelTreeNode() {}

  // \ru Доступ к непосредственным предкам узла. \en Access to the immediate node parents. 
  std::set<const IModelTreeNode*>& GetParents() { return m_parents; }
  const std::set<const IModelTreeNode*>& GetParents() const { return m_parents; }

  // \ru Доступ к непосредственным потомкам узла. \en Access to the immediate node children. 
  std::set<const IModelTreeNode*>& GetChildren() { return m_children; }
  const std::set<const IModelTreeNode*>& GetChildren() const { return m_children; }

  // \ru Добавить предка. \en Add a parent. 
  void AddParent( IModelTreeNode* parent ) { if (parent) m_parents.insert(parent); }

  // \ru Добавить потомка. \en Add a child. 
  void AddChild( IModelTreeNode* child ) { if (child) { m_children.insert(child); child->AddParent(this); } }

  // \ru Доступ к данным узла. \en Access to the node data. 
  virtual MbItemData& GetData() = 0;
  virtual const MbItemData& GetData() const = 0;

  // \ru Доступ к позиции чтения/записи узла. \en Access to the node read/write position. 
  virtual ClusterReference& GetPosition() = 0;
  virtual const ClusterReference& GetPosition() const = 0;

  // \ru Признак частичного или полного чтения узла.
  //     При чтении объекта может возникнуть необходимость чтения некоторых данных его родителя.
  //     В этом случае объект родителя читается частично и имеет соответствующий флаг.
  // \en Indicator of partial reading of the current node.
  //     While reading an object there can be a need to read some data from its parent.
  //     In this case the parent object is read partially and has a corresponding flag.
  // \ru Узнать, читать ли только часть узла.
  // \en Check whether to read the node partially.
  virtual bool PartialRead() const = 0;
  // \ru Установить признак частичного или полного чтения узла.
  // \en Set indication of full or partial node reading.
  virtual void SetPartialRead ( bool partial ) const = 0;

  // \ru Записать узел. \en Write the node. 
  virtual writer &   operator >> ( writer & ) = 0;
  // \ru Прочитать узел. \en Read the node. 
  virtual reader &   operator << ( reader & ) = 0;

  // \ru Операторы для записи узла дерева поток в xml формате. \en Operators to output tree node to a stream in xml format.
  friend MATH_FUNC( c3d::t_ofstream & ) operator << ( c3d::t_ofstream& file, IModelTreeNode& node );
  friend MATH_FUNC( c3d::t_ofstream & ) operator << ( c3d::t_ofstream& file, const IModelTreeNode& node );

OBVIOUS_PRIVATE_COPY(IModelTreeNode)
};

//----------------------------------------------------------------------------------------
// \ru Предварительное объявление Дерева Исполнений.
// \en The forward declaration of Embodiments Tree. \~
// ---
class IEmbodimentTree;

//----------------------------------------------------------------------------------------
/** \brief   \ru Обобщенное дерево модели.
             \en Generic model tree. \~
    \details \ru Обобщенное дерево модели (может иметь несколько корней). 
             Умеет записываться в поток и читаться из потока. \n
             Дерево Модели отражает иерархию стандартной модели (объекта MbModel).
             \en Generic model tree (can have several roots). 
             Can be written to a stream and read from a stream. \n \~
             Model Tree presents a hierarchy of a standard model (MbModel object).
    \ingroup Base_Tools_IO
*/
// ---
class MATH_CLASS IModelTree
{
public:
  enum TreeType
  {
    mtt_Model,     // \ru Дерево содержит модель. \en Tree contains model.
    mtt_Embodiment // \ru Дерево содержит исполнения. \en Tree contains embodiments.
  };

  // \ru Тип, представляющий листовой узел с ветвью дерева, ведущей к нему, начиная с корневого узла дерева.
  // \en A type which represents a leaf node with the tree branch, leading to it, starting from the root of the tree.
  typedef std::pair<const IModelTreeNode*,std::vector<const IModelTreeNode*> > NodeBranch;

  // \ru Тип функции для выбора узлов дерева по фильтрам.
  // \en The type of a function for selecting tree nodes by filters.
  typedef bool ( CALL_DECLARATION * FilterNodesFunc ) ( std::vector<NodeBranch>&, const std::vector<MbItemData>&, const IModelTree* );

  // \ru Тип функции для определения, нужно ли добавлять объект в дерево модели, и заполнения данных узла.
  // \en The type of a function for determining, whether to add the object to the model tree, and filling the node data.
  typedef bool ( CALL_DECLARATION *NodeToAddFunc ) ( const TapeBase* mem, MbItemData& data );

protected:
  // \ru Тип дерева. \en Tree type.
  TreeType        m_type;

  // \ru Функция для определения, нужно ли добавлять объект в дерево модели, и заполнения данных узла.
  // \en A function for determining, whether to add the object to the model tree, and filling the node data.
  NodeToAddFunc   m_nodeToAddFunc;

  // \ru Функция для выбора объектов по фильтрам.
  // \en A function for for selecting objects by filters.
  FilterNodesFunc m_filterFunc;

  // \ru Корни дерева. \en The tree roots.
  std::vector<const IModelTreeNode*> m_roots;

public:

  IModelTree() : m_type ( mtt_Model ), m_nodeToAddFunc( nullptr ), m_filterFunc( nullptr ) {}
  virtual ~IModelTree() {}

  // \ru Выдать тип дерева. \en Get the tree type.
  TreeType GetType() const { return m_type; }
  // \ru Установить тип дерева. \en Set the tree type.
  void SetType( TreeType type ) { m_type = type; }

  // \ru Построить дерево из узлов, выбранных по фильтрам. В случае дерева исполнений, функция работает с первым исполнением.
  //  \en Build a tree with nodes, selected by filters. In case of embodiment tree, the function works with the first embodiment.
  virtual std::unique_ptr<const IModelTree> GetFilteredTree ( const std::vector<MbItemData>& filters ) const = 0;

  // \ru Построить дерево по заданным узлам. Не применимо к дереву исполнений (в этом случае возвращает nullptr).
  //  \en Build a tree for given nodes. Not applicable to embodiment tree (in this case, returns nullptr).
  virtual std::unique_ptr<const IModelTree> GetFilteredTree ( std::vector<const IModelTreeNode*>& nodes ) const = 0;

  // \ru Выдать указатель на дерево исполнений. Выдает nullptr, если не применимо (нет исполнений).
  // \en Get pointer to embodiments tree. Return nullptr if not applicable (no embodiments).
  virtual const IEmbodimentTree* GetEmbodimentsTree() const = 0;

  // \ru Добавить узел. \en Add a node. 
  virtual void AddNode ( const TapeBase* mem, const ClusterReference& ref ) = 0;

  // \ru Нотификация об окончании чтения/записи текущего узла.
  // \en Notification about the end of current node writing/reading. 
  virtual void CloseNode( const TapeBase* mem ) = 0;

  // \ru Установить функцию для выбора геометрического объекта для добавления в дерево модели, и заполнения данных узла.
  // \en Define a function for selecting a geometric object for adding to the model tree, and filling the node data.
  virtual void SetNodeToAddFunction( NodeToAddFunc callback ) { if ( callback ) m_nodeToAddFunc = callback; }

  // \ru Установить функцию для выбора узлов из дерева модели.
  // \en Define a function for selecting nodes from the model tree.
  virtual void SetFilterFunction( FilterNodesFunc callback ) { if ( callback ) m_filterFunc = callback; }

  // \ru Записать дерево. \en Write the tree. 
  virtual writer &   operator >> ( writer & ) = 0;
  // \ru Прочитать дерево. \en Read the tree. 
  virtual reader &   operator << ( reader & ) = 0;


  // \ru Доступ к корням дерева.
  // Узел дерева может быть рекурсивно вложен
  // (например, Instance может содержать сборку, которая содержит другой Instance, ссылающийся на эту же сборку).
  // \en Access to the tree roots.
  // Tree node could be nested recursively
  // (e.g. Instance can contain an Assembly which contains another Instance which includes this Assembly).
  const std::vector<const IModelTreeNode*>& GetRoots() const { return m_roots; }
  std::vector<const IModelTreeNode*>& GetRoots() { return m_roots; }

  // \ru Версия дерева. \en Tree version.
  virtual VERSION GetVersion() = 0;
  virtual void SetVersion( VERSION ) = 0;

  // \ru Построить поддерево из потомков заданного узла.
  // \en Build a tree from children of a given node.
  static std::unique_ptr<const IModelTree> GetSubtree ( const IModelTreeNode* node );

  // \ru Получить значимые узлы поддерева с указанным корнем (исключив узлы, которые добавлены для восстановления иерархии дерева).
  //     Возвращает количество значимых узлов.
  // \en Get significant nodes in a subtree with the given root (excluding nodes that were added to reconstruct the tree hierarchy).
  //     Return a number of significant nodes.
  static size_t GetSubtreeSignificantNodes ( const c3d::IModelTreeNode* node, std::set<const c3d::IModelTreeNode*>& nodes );

  // \ru Создать экземпляр дерева. \en Create a tree instance. 
  static IModelTree* CreateModelTree();

  // \ru Операторы для записи дерева в поток в xml формате. \en Operators to output a tree to a stream in xml format.
  friend MATH_FUNC( c3d::t_ofstream & ) operator << ( c3d::t_ofstream& file, IModelTree& tree );
  friend MATH_FUNC( c3d::t_ofstream & ) CALL_DECLARATION operator << ( c3d::t_ofstream& file, const IModelTree& tree );

OBVIOUS_PRIVATE_COPY(IModelTree)
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Узел дерева исполнений.
             \en Embodiments tree node. \~
    \details \ru Узел дерева исполнений (может иметь несколько потомков).
             \en Embodiments tree node (can have several children).
    \ingroup Base_Tools_IO
*/
// ---
class MATH_CLASS IEmbodimentNode
{
protected:
  // \ru Непосредственные потомки узла. \en The immediate children of the node. 
  std::set<const IEmbodimentNode*> m_children;
public:
  IEmbodimentNode() {}
  virtual ~IEmbodimentNode() {
    for ( std::set<const IEmbodimentNode*>::iterator i = m_children.begin(); i != m_children.end(); ++i )
      if ( *i != nullptr ) delete *i;
  }

  // \ru Построить поддерево модели, содержащееся в данном исполнении.
  // \en Build a subtree of a model tree which is contained in a given embodiment.
  virtual std::unique_ptr<const IModelTree> GetEmbodiment() const = 0;

  // \ru Выдать узел дерева модели, соответствующий данному исполнению.
  // \en Get a model tree node which corresponds to a given embodiment.
  virtual const IModelTreeNode * GetModelTreeNode() const = 0;

  // \ru Доступ к информации об исполнении. \en Access to the embodiment info. 
  virtual const MbItemData& GetEmbodimentData() const = 0;

  // \ru Доступ к непосредственным потомкам узла. \en Access to the immediate node children. 
  std::set<const IEmbodimentNode*>& GetChildren() { return m_children; }
  const std::set<const IEmbodimentNode*>& GetChildren() const { return m_children; }

  // \ru Добавить потомка. \en Add a child. 
  void AddChild( IEmbodimentNode* child ) { if ( child ) m_children.insert( child ); }

  OBVIOUS_PRIVATE_COPY( IEmbodimentNode )
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Дерево Исполнений.
             \en Embodiment Tree. \~
    \details \ru Дерево Исполнений отражает иерархию вариантов реализации модели (исполнений).
             Каждый узел дерева представляет одно исполнение.
             Подразумевается, что в геометрической модели исполнение хранится,
             как объект MbAssembly с выставленным атрибутом типа at_Embodiment.\n
             \en Embodiment Tree presents a hierarchy of variants of model implementation (embodiments).
             Each node of the tree presents an embodiment.
             It is assumed that in a geometric model an embodiment is stored as
             MbAssembly object with an attribute of type at_Embodiment. \n \~
    \ingroup Base_Tools_IO
*/
class MATH_CLASS IEmbodimentTree
{
protected:
  std::vector<const IEmbodimentNode*> m_roots;

public:
  IEmbodimentTree() {}
  virtual ~IEmbodimentTree() {}

  // \ru Доступ к корням дерева. \en Access to the tree roots.
  const std::vector<const IEmbodimentNode*>& GetRoots() const { return m_roots; }
  std::vector<const IEmbodimentNode*>& GetRoots() { return m_roots; }

};

} //namespace c3d

#endif // __IO_TREE_H
