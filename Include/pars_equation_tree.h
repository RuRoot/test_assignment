////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Узел бинарного дерева.
         \en Node of a binary tree. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __PARS_EQUATION_TREE_H
#define __PARS_EQUATION_TREE_H


#include <math_x.h> // \ru СМВ для компиляции ICC \en СМВ for compilation by ICC 
#include <io_tape_define.h>
#include <templ_ifc_array.h>
#include <templ_sptr.h>
#include <pars_tree_variable.h>
#include <mb_enum.h>
#include <set>
#include <map>
#include <functional>
#include <utility>


//------------------------------------------------------------------------------
/** \brief \ru Количество параметров.
           \en Parameters count. \~
  \details \ru Выдать количество параметров для типа операции.\n
           \en Get the count of parameters for type of operation.\n \~
  \param[in] operationType - \ru Тип операции.
                             \en An operation type. \~
  \return \ru Количество параметров операции.
          \en Operation parameters count. \~
  \ingroup Parser
*/   
// ---   
inline
uint GetCountOfParams( PceOperationType operationType )
{
  uint res = 1;
  if ( operationType < oprt_BinaryOperation )
    res = 3;
  else if ( operationType < oprt_UnaryOperation )
    res = 2;

  return res;
}


//-----------------------------------------------------------------------------
/** \brief \ru Информация о характерных точках дерева.
           \en Information about characteristic points of a tree. \~
  \details \ru Информация о характерных точках дерева. \n
           \en Information about characteristic points of a tree. \n \~
  \ingroup Parser
*/
// ---
class BTreeNode;
struct CharacterPointInfo
{
  /** \brief \ru Тип характерной точки.
             \en Type of a characteristic point. \~
    \details \ru Тип характерной точки.\n
             \en Type of a characteristic point.\n \~      
  */
  enum EquCharacterPointType
  {
    equPoint_DefRangeRight, ///< \ru Граница области определения. \en Boundary of the definition domain. 
    equPoint_DefRangeLeft,  ///< \ru Граница области определения. \en Boundary of the definition domain. 
    equPoint_Extr,          ///< \ru Экстремум. \en Extremum. 
    equPoint_Break1,        ///< \ru Разрыв первого рода. \en Discontinuity of the first kind. 
    equPoint_Break2,        ///< \ru Разрыв второго рода. \en Discontinuity of the second kind. 
    equPoint_DerBreak1      ///< \ru Разрыв производной. \en Derivative discontinuity. 
  };

  const BTreeNode *     m_tree;   ///< \ru Узел дерева. Не равен nullptr. \en Node of a tree. Not equal to nullptr.
  EquCharacterPointType m_type;   ///< \ru Тип характерной точки. \en Type of a characteristic point. 
  double                m_ph;     ///< \ru Значение параметра функции. \en The value of the function parameter. 
  double                m_period; ///< \ru Период функции. \en A period of a function. 

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор.\n
             \en Constructor.\n \~     
    \param[in] tree   - \ru Узел дерева.
                        \en Node of a tree. \~
    \param[in] type   - \ru Тип характерной точки.
                        \en Type of a characteristic point. \~
    \param[in] ph     - \ru Значение параметра функции.
                        \en Value of a function parameter. \~
    \param[in] period - \ru Период функции.
                        \en Period of a function. \~
  */
  CharacterPointInfo( const BTreeNode & tree, EquCharacterPointType type, double ph, double period )
    : m_tree  ( &tree  )
    , m_type  ( type   )
    , m_ph    ( ph     )
    , m_period( period )
  {}     

};
     

//-----------------------------------------------------------------------------
/** \brief \ru Элемент области определения функции.
           \en Element of the function definition domain. \~
  \details \ru Элемент области определения функции. \n
           \en Element of the function definition domain. \n \~
  \ingroup Parser
*/
// ---   
struct DefRangeItem 
{
  /** \brief \ru Тип элемента.
             \en A type of the element. \~
    \details \ru Тип элемента области определения функции. \n
             \en Type of an element of the function definition domain. \n \~        
  */
  enum RangeItemType
  {
    range_def,   ///< \ru Область определения. \en Definition domain. 
    range_break, ///< \ru Разрыв. \en Discontinuity. 
    range_extr,  ///< \ru Экстремум. \en Extremum. 
  };

  double        lbound; ///< \ru Левая граница элемента области определения. \en Left bound of an element of the definition domain. 
  double        rbound; ///< \ru Правая граница элемента области определения. \en Right bound of an element of the definition domain. 
  RangeItemType type;   ///< \ru Тип элемента. \en A type of an element. 

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор. \n
             \en Constructor. \n \~    
    \param[in] l - \ru Левая граница элемента области определения.
                   \en The left bound of an element of the definition domain. \~
    \param[in] r - \ru Правая граница элемента области определения.
                   \en The right bound of an element of the definition domain. \~
    \param[in] t - \ru Тип элемента.
                   \en A type of the element. \~     
  */
  DefRangeItem( double l, double r, RangeItemType t = range_def )
    : lbound( l )
    , rbound( r )
    , type( t ) 
  {
    if ( lbound > rbound )
      std::swap( lbound, rbound );
  }
};


namespace std {
//-----------------------------------------------------------------------------
/** \brief \ru Сравнение элементов области определения.
           \en Comparison of elements of the definition domain. \~
  \details \ru Сравнение элементов области определения функции. \n
           \en Comparison of elements of the function definition domain. \n \~
  \ingroup Parser
*/
// --- 
template<>
struct less<DefRangeItem>
{
  /** \brief \ru Сравнение элементов области определения.
             \en Comparison of elements of the definition domain. \~
    \details \ru Сравнение элементов области определения функции. \n
             \en Comparison of elements of the function definition domain. \n \~  
    \param[in] _Left  - \ru Первый элемент.
                        \en The first element. \~
    \param[in] _Right - \ru Второй элемент
                        \en The second element \~
    \return \ru true, если первый элемент меньше второго.
            \en true if the first element is less than second one. \~
  */
  bool operator()(const DefRangeItem & _Left, const DefRangeItem & _Right) const
  { return ( _Right.lbound - _Left.rbound > -METRIC_EPSILON ); }
};

} // namespace std


//-----------------------------------------------------------------------------
/** \brief \ru Область определения функции.
           \en The function domain. \~
  \details \ru Область определения функции. \n
           \en The function domain. \n \~
  \ingroup Parser
*/
// ---  
class DefRange
{
  std::set<DefRangeItem>  arr;
  bool                    m_hasbreak;
public:
  static const double     eps; ///< \ru Погрешность. \en Tolerance. 

public:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор области определения без разрывов. \n
             \en Constructor of the definition domain without discontinuities. \n \~    
    \param[in] l - \ru Левая граница области определения.
                   \en Left bound of the definition domain. \~ 
    \param[in] r - \ru Правая граница области определения.
                   \en Right bound of the definition domain. \~
  */
  DefRange( double l, double r )
    : m_hasbreak( false )
  {
    if ( l > r )
      std::swap( l, r );
    arr.insert( DefRangeItem(l - METRIC_EPSILON, r + METRIC_EPSILON) );
  }

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор области определения без разрывов. \n
             \en Constructor of the definition domain without discontinuities. \n \~   
  */
  DefRange() : m_hasbreak( false ) {}; 

  /// \ru Дать набор элементов области определения. \en Get a set of elements of the definition domain. 
  const std::set<DefRangeItem> & GetSet() const { return arr; }
  
  /// \ru Есть ли разрывы в области определения. \en Whether the discontinuities is in the definition domain. 
  bool HasBreaks() const { return m_hasbreak; }

  /// \ru Разрезать область определения. \en Cut the domain. 
  bool Cut( DefRangeItem & i );
  /// \ru Добавить элемент области определения. \en Add element of the definition domain.    
  void Add( DefRangeItem & i );
  /// \ru Оператор сравнения. \en Comparison operator.   
  bool operator == ( const DefRange & other );
};
     

class BTreeConst;
class BTreeIdent;
class BTreeFunction;
class BTreeOperation;
class BTreeOperation1Arg;
class BTreeOperation3Args;
class BTreeUserFunc;

typedef SPtr<BTreeNode> BTreeNodeSPtr;

//-----------------------------------------------------------------------------
/** \brief \ru Значение функции и производных.
           \en Value of the function and derivatives. \~
  \details \ru Значение функции и её первой, второй и третьей производных. \n
           \en Values of function, its first, second and third derivatives. \n \~
  \ingroup Parser
*/
// ---
struct DerivesValues
{
  double value;     ///< \ru Значение функции. \en The value of function. 
  double firstDer;  ///< \ru Первая производная. \en The first derivative. 
  double secondDer; ///< \ru Вторая производная. \en The second derivative. 
  double thirdDer;  ///< \ru Третья производная. \en The third derivative. 
};


//-----------------------------------------------------------------------------
/** \brief \ru Базовый класс для узлов дерева выражения.
           \en Base class for nodes of the expression tree. \~
  \details \ru Базовый класс для узлов дерева выражения. \n
           \en Base class for nodes of the expression tree. \n \~
  \ingroup Parser
*/
// ---
class MATH_CLASS BTreeNode : public TapeBase, public MbRefItem
{
public:
  typedef std::map<ItTreeVariable *, ItTreeVariable *>  EqualVarsMap;  ///< \ru Набор пар переменных. \en A set of pairs of variables. 
  typedef std::map<const ItCoord *, DerivesValues>      VarsDerives;   ///< \ru Набор пар: координата - значение и производные. \en A set of pairs: coordinate - value and derivatives. 

public:
  BteNodeType type; ///< \ru Тип узла. \en A type of node. \~ \internal \ru Для отладки. \en For debugging. \~ \endinternal
protected:
  mutable VarsDerives varDers; ///< \ru Рабочие переменные. \en Working variables. \~

protected:
  /// \ru Конструктор по умолчанию. \en Default constructor. 
  BTreeNode() {}

public:
  virtual ~BTreeNode() {}
  /// \ru Выдать тип узла дерева. \en Get type of a tree node. 
  virtual BteNodeType     IsA      () const = 0;
  /// \ru Создать копию объекта. \en Create a copy of the object.
  virtual BTreeNode *     Duplicate() const = 0;

  /// \ru Функция линейная. \en Function is linear.
  virtual bool            IsLine()    const = 0;

  /**\ru \name Функции для вычисления значения и производной.
     \en \name Functions for calculation of the value and the derivative.
    \{ */  
  
  /** \brief \ru Вычислить значение.
             \en Calculate value. \~
    \details \ru Вычислить значение узла.\n
             \en Calculate value of node.\n \~
    \param[out] fValue - \ru Значение.
                         \en Value. \~
    \return \ru Код результата разбора строки.
            \en String parsing result code. \~
  */
  virtual  EquTreeResCode  GetValue( double & fValue ) const = 0;
  
  /** \brief \ru Установить значение.
             \en Set value. \~
    \details \ru Устанавливает значение v узлу дерева.
             \en Set 'v' value to a tree node. \~
    \param[in] v - \ru Желаемое значение.
                   \en Desirable value. \~
    \param[in] unfixedDVars - \ru Переменные, значение которых можно менять.
                              \en Variables which values can be changed. \~
  */
  virtual bool            SetValue( double v, const std::set<ItTreeVariable *> & unfixedDVars ) = 0;

  /** \brief \ru Вычислить строковое значение.
             \en Calculate string value. \~
    \details \ru Вычислить строковое значение узла.\n
             \en Calculate string value of node.\n \~
    \param[out] varName - \ru Значение.
                          \en Value. \~
    \return \ru Код результата разбора строки.
            \en String parsing result code. \~
  */
  virtual  EquTreeResCode  GetStringValue( c3d::string_t & varName ) const { varName = _T(""); return equTreeResCode_InvalidVariableType; }

  /** \brief \ru Вычислить значение и производные.
             \en Calculate a value and derivatives. \~
    \details \ru Вычислить значение и производные. \n
             \en Calculate a value and derivatives. \n \~       
    \param[out] fValue  - \ru Значение.
                          \en Value. \~
    \param[out] derive1 - \ru Первая производная.
                          \en The first derivative. \~
    \param[out] derive2 - \ru Вторая производная.
                          \en The second derivative. \~
    \param[out] derive3 - \ru Третья производная.
                          \en The third derivative. \~
    \param[in]  ders    - \ru Набор значений и производных.
                          \en Set of values and derivatives. \~
    \return \ru Код результата разбора строки.
            \en String parsing result code. \~
  */
  virtual EquTreeResCode  CalculateDerives( double & fValue, double & derive1, 
                                            double & derive2, double & derive3, const VarsDerives & ders ) const = 0;

  /** \brief \ru Вычислить значение и производные.
             \en Calculate a value and derivatives. \~
    \details \ru Вычислить значение и производные. \n
             \en Calculate a value and derivatives. \n \~  
    \param[in]  coord - \ru Координата.
                        \en Coordinate. \~
    \param[out] v     - \ru Значение.
                        \en Value. \~
    \param[out] fd    - \ru Первая производная.
                        \en The first derivative. \~
    \param[out] sd    - \ru Вторая производная.
                        \en The second derivative. \~
    \param[out] td    - \ru Третья производная.
                        \en The third derivative. \~
    \return \ru Код результата разбора строки.
            \en String parsing result code. \~
  */
          EquTreeResCode  CalculateDerives( const ItCoord * coord, double & v, double & fd, double & sd, double & td ) const;

  /** \brief \ru Выдать использованные переменные.
             \en Get the used variables. \~
    \details \ru Выдать использованные переменные. \n
             \en Get the used variables. \n \~     
    \param[out] arr   - \ru Переменные.
                        \en Variables. \~
    \param[out] funcs - \ru Пользовательские функции.
                        \en User functions. \~        
  */
  virtual void            GetUsedVariables( SSArray<ItTreeVariable *> & arr, SSArray<ItUserFunc *> & funcs ) const = 0;
  
  /** \} */  
  /**\ru \name Функции замены переменных по именам.
     \en \name Functions for replacing variables by names.
      \{ */
  
  /** \brief \ru Заменить переменные.
             \en Replace variables. \~
    \details \ru Заменить все переменные с указанными именем на новую переменную.\n
             \en Replace all variables with the specified name by a new variable.\n \~
    \param[out] varName - \ru Имя.
                          \en Name. \~
    \param[out] newVar  - \ru Новая переменная.
                          \en New variable. \~
  */
  virtual void            ReplaceParVariable( const c3d::string_t & varName, ItTreeVariable & newVar ) = 0;
  
  /** \brief \ru Заменить узел.
             \en Replace a node. \~
    \details \ru Заменить узел на копию нового, если заданная переменная использована.\n
             \en Replace a node with a copy of a new one if the given variable is used.\n \~
    \param[out] var     - \ru Переменная.
                          \en Variable. \~
    \param[out] subTree - \ru Новый узел.
                          \en New node. \~
  */
  virtual void            ReplaceParVariable( const ItTreeVariable & var, const BTreeNode & subTree )        = 0;
  
  /** \brief \ru Заменить переменные.
             \en Replace variables. \~
    \details \ru Заменить все переменные с указанными именем на новую переменную.\n
             \en Replace all variables with the specified name by a new variable.\n \~
  */
  virtual void            ReplaceIntVariable( const c3d::string_t &, ItIntervalTreeVariable & ) {}

  /** \} */  
  /**\ru \name Функции доступа к данным.
     \en \name Functions for access to data.
      \{ */       

  /** \brief \ru Выдать значение параметра экстремума.
             \en Get value of parameter of extremum. \~
    \details \ru Выдать значение параметра экстремума, если это возможно.\n
             \en Get value of parameter of extremum if it is possible.\n \~     
    \param[in] interval - \ru Интервал для поиска.
                          \en Interval to search. \~
    \param[in] var      - \ru Переменная.
                          \en Variable. \~
    \param[out] points  - \ru Точки экстремума.
                          \en Points of extremum. \~
  */
          bool            GetExtremumPoints( const c3d::DoublePair & interval, 
                                                   ItTreeVariable & var, 
                                                   c3d::DoubleVector & points );  
  /** \brief \ru Область определения.
             \en Domain. \~
    \details \ru Область определения.\n
             \en Domain.\n \~     
    \param[in,out] defRange    - \ru Область определения.
                                 \en Domain. \~
    \param[out]    var         - \ru Переменная.
                                 \en Variable. \~
    \param[in]     stopOnBreak - \ru Не искать разрывы области определения.
                                 \en Not to search the discontinuities in the definition domain. \~
  */
  virtual bool            GetDefRange( DefRange & defRange, ItTreeVariable & var, bool stopOnBreak )   const = 0;
  
  /** \brief \ru Только для внутреннего использования! Порядок переменной.
             \en For internal use only! Order of variable. \~
    \details \ru Порядок переменной.\n
             \en Order of variable.\n \~
    \param[in] var - \ru Переменная.
                     \en Variable. \~
    \return \ru Порядок.
            \en Order. \~
  */
  virtual size_t          GetPseudoOrderByVar     ( ItTreeVariable & var )                                   const = 0;

  /** \brief \ru Фиксированные переменные.
             \en Fixed variables. \~
    \details \ru Фиксированные переменные.\n
             \en Fixed variables.\n \~           
    \param[in]  unfixedVars - \ru Набор нефиксированных переменных. Если переменная нашлась в наборе, фиксировать копию.
                              \en A set of unfixed variables. If variable was found in set, then fix the copy. \~
    \param[in]  newFuncs    - \ru Пользовательские функции.
                              \en User functions. \~
    \param[out] code        - \ru Коды результата разбора строки.
                              \en Result codes of string parsing. \~
    \return \ru Переменную для фиксирования.
            \en Variable for fixation. \~
  */
  virtual BTreeNodeSPtr  FixVars( const RPArray<ItTreeVariable> & unfixedVars,
                                        IFC_Array<ItUserFunc> & newFuncs, EquTreeResCode & code )     const = 0;
  /// \ru Дать эквивалентный узел. \en Get equivalent node. 
  virtual BTreeNodeSPtr  GetCalcEquivalent()                                                          const = 0;
  
  /** \brief \ru Дать строку.
             \en Get string. \~
    \details \ru Дать строку выражения.\n
             \en Get expression string.\n \~
    \param[out] - \ru Строка.
                  \en String. \~
  */
  virtual void            GetString( c3d::string_t & )                                                   const = 0;
  
  /// \ru Вычислить размер в байтах. \en Get size in bytes. 
  virtual size_t          SizeOf() const = 0;

  /** \} */  
  /**\ru \name Функции сравнения.
     \en \name Comparison function.
      \{ */

  /** \brief \ru Равен ли узел заданному узлу.
             \en Whether the node is equal to the given node. \~
    \details \ru Равен ли узел заданному узлу.\n
             \en Whether the node is equal to the given node.\n \~
    \param[in] other   - \ru Узел для сравнения.
                         \en Node for comparison. \~
    \param[in] varsMap - \ru Набор пар равных переменных.
                         \en A set of pairs of equal variables. \~
    \return \ru true, если узлы равны.
            \en true if nodes are equal. \~
  */
  virtual bool            IsEqual( const BTreeNode           & other, const EqualVarsMap & varsMap ) const = 0;
  
  /** \brief \ru Равен ли узел заданному узлу.
             \en Whether the node is equal to the given node. \~
    \details \ru Равен ли узел заданному узлу.\n
             \en Whether the node is equal to the given node.\n \~      
    \return false.
  */
  virtual bool            IsEqual( const BTreeConst          &      , const EqualVarsMap &         ) const { return false; }
  
  /** \brief \ru Равен ли узел заданному узлу.
             \en Whether the node is equal to the given node. \~
    \details \ru Равен ли узел заданному узлу.\n
             \en Whether the node is equal to the given node.\n \~      
    \return false.
  */
  virtual bool            IsEqual( const BTreeIdent          &      , const EqualVarsMap &         ) const { return false; }
  
  /** \brief \ru Равен ли узел заданному узлу.
             \en Whether the node is equal to the given node. \~
    \details \ru Равен ли узел заданному узлу.\n
             \en Whether the node is equal to the given node.\n \~      
    \return false.
  */
  virtual bool            IsEqual( const BTreeFunction       &      , const EqualVarsMap &         ) const { return false; }
  
  /** \brief \ru Равен ли узел заданному узлу.
             \en Whether the node is equal to the given node. \~
    \details \ru Равен ли узел заданному узлу.\n
             \en Whether the node is equal to the given node.\n \~      
    \return false.
  */
  virtual bool            IsEqual( const BTreeOperation      &      , const EqualVarsMap &         ) const { return false; }
  
  /** \brief \ru Равен ли узел заданному узлу.
             \en Whether the node is equal to the given node. \~
    \details \ru Равен ли узел заданному узлу.\n
             \en Whether the node is equal to the given node.\n \~      
    \return false.
  */
  virtual bool            IsEqual( const BTreeOperation1Arg  &      , const EqualVarsMap &         ) const { return false; }
  
  /** \brief \ru Равен ли узел заданному узлу.
             \en Whether the node is equal to the given node. \~
    \details \ru Равен ли узел заданному узлу.\n
             \en Whether the node is equal to the given node.\n \~   
    \return false.
  */
  virtual bool            IsEqual( const BTreeOperation3Args &      , const EqualVarsMap &         ) const { return false; }
  
  /** \brief \ru Равен ли узел заданному узлу.
             \en Whether the node is equal to the given node. \~
    \details \ru Равен ли узел заданному узлу.\n
             \en Whether the node is equal to the given node.\n \~      
    \return false.
  */
  virtual bool            IsEqual( const BTreeUserFunc       &      , const EqualVarsMap &         ) const { return false; }
  /** \} */  

private:
  // \ru не реализовано \en not implemented 
  BTreeNode( const BTreeNode & );       
  void operator =( const BTreeNode & );

  DECLARE_PERSISTENT_CLASS( BTreeNode )
};

IMPL_PERSISTENT_OPS( BTreeNode )


//-----------------------------------------------------------------------------
/** \brief \ru Типы узлов бинарного дерева.
           \en Types of nodes of the binary tree. \~
  \details \ru Типы узлов бинарного дерева. \n
           \en Types of nodes of the binary tree. \n \~
  \ingroup Parser
*/
// ---
enum TeIntervalNodeType
{
  tei_Const, ///< \ru Константа. \en Constant. 
  tei_Ident  ///< \ru Идентификатор. \en Identifier. 
};


//-----------------------------------------------------------------------------
/** \brief \ru Узел дерева интервального выражения.
           \en Node of interval expression tree. \~
  \details \ru Узел дерева интервального выражения. \n
           \en Node of interval expression tree. \n \~
  \ingroup Parser
*/
// ---
class MATH_CLASS TreeIntervalNode : public TapeBase
{
protected:
  TreeIntervalNode() {}
public:
  virtual ~TreeIntervalNode() {}

public:
  /// \ru Выдать тип узла дерева. \en Get type of a tree node. 
  virtual TeIntervalNodeType  IsA      ()                  const = 0;
  /// \ru Выдать копию объекта. \en Get a copy of the object. 
  virtual TreeIntervalNode *  Duplicate()                  const = 0;
       
  /// \ru Дать первую переменную. \en Get the first variable.    
  virtual  EquTreeResCode      GetFirstValue ( double & )   const = 0;
  /// \ru Дать вторую переменную. \en Get the second variable. 
  virtual EquTreeResCode      GetSecondValue( double & )   const = 0;
  /// \ru Дать строку. \en Get a string. 
  virtual void                GetString( c3d::string_t & ) const = 0;
  /** \brief \ru Установить значение.
             \en Set value. \~
    \details \ru Попытаться установить значение [f;s] узлу дерева.
             \en Try to set value [f;s] to a tree node. \~
    \param[in] f - \ru Нижняя граница интервала.
                   \en Lower bound of the interval. \~
    \param[in] s - \ru Верхняя граница интервала.
                   \en Upper bound of the interval. \~
    \param[in] unfixedIVars - \ru Множество интервальных переменных, которые можно менять.
                              \en Set of interval variables, which can be changed. \~
    \param[in] unfixedDVars - \ru Множество вещественных переменных, которые можно менять.
                              \en Set of real variables, which can be changed. \~
    \access    public 
    \return \ru Истину, если удалось установить значение.
            \en True if it was succeeded to set value. \~
  */
  virtual c3d::BoolPair       SetValue( double f, double s
                                      , const std::set<ItIntervalTreeVariable *> & unfixedIVars
                                      , const std::set<ItTreeVariable *> & unfixedDVars ) = 0;
  /** \brief \ru Заменить переменные.
             \en Replace variables. \~
    \details \ru Заменить все переменные с указанными именем на новую переменную.\n
             \en Replace all variables with the specified name by a new variable.\n \~     
    \param[out] varName - \ru Имя.
                          \en Name. \~
    \param[out] newVar  - \ru Новая переменная.
                          \en New variable. \~        
  */
  virtual void                ReplaceParVariable( const c3d::string_t & varName, ItTreeVariable & newVar ) = 0;

DECLARE_PERSISTENT_CLASS( TreeIntervalNode )
OBVIOUS_PRIVATE_COPY( TreeIntervalNode )
};

IMPL_PERSISTENT_OPS( TreeIntervalNode )


//-----------------------------------------------------------------------------
/** \brief \ru Интервал простых выражений.
           \en Interval of simple expressions. \~
  \details \ru Операция [] - получение интервала из простых выражений.\n
           \en operation [] - obtaining of interval of simple expressions.\n \~
  \ingroup Parser
*/
// ---
class MATH_CLASS IntervalConstNode : public TreeIntervalNode
{
  BTreeNode * m_firstValue;  ///< \ru Первое значение (всегда не nullptr). \en First value (always not nullptr).
  BTreeNode * m_secondValue; ///< \ru Второе  значение (всегда не nullptr). \en Second value (always not nullptr).

public:

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор.\n
             \en Constructor.\n \~    
    \param[in] firstValue  - \ru Первый узел.
                             \en The first node. \~
    \param[in] secondValue - \ru Второй узел.
                             \en The second node. \~
  */
  IntervalConstNode( BTreeNode & firstValue, BTreeNode & secondValue )
    : m_firstValue ( &firstValue  )
    , m_secondValue( &secondValue )
  {}

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор копирования.\n
             \en Copy-constructor.\n \~    
    \param[in] other - \ru Копируемый объект.
                       \en Object to copy. \~
  */
  IntervalConstNode( const IntervalConstNode & other )
    : TreeIntervalNode {}
    , m_firstValue ( other.m_firstValue->Duplicate()  ) // ( other.m_firstValue  )
    , m_secondValue( other.m_secondValue->Duplicate() ) // ( other.m_secondValue )
  {}
  /** \brief \ru Деструктор.
             \en Destructor. \~
    \details \ru Деструктор.\n
             \en Destructor.\n \~    
  */
  virtual ~IntervalConstNode();

public:   
  /**\ru \name Функции узла дерева интервального выражения.
     \en \name Functions of interval expression tree node.
      \{ */

  TeIntervalNodeType    IsA      () const override { return tei_Const; } // \ru выдать тип узла дерева \en get type of a tree node
  IntervalConstNode *   Duplicate() const override { return new IntervalConstNode( *this ); }

  // \ru вычисление значения функции и производной по переменной V \en calculate the value of a function and the derivative with respect to V 
  EquTreeResCode        GetFirstValue ( double & v ) const override { return m_firstValue->GetValue( v ); }
  EquTreeResCode        GetSecondValue( double & v ) const override { return m_secondValue->GetValue( v ); }
  void                  GetString( c3d::string_t & ) const override {}
  // \ru Установить значение. \en Set value. 
  c3d::BoolPair         SetValue( double f, double s
                                          , const std::set<ItIntervalTreeVariable *> & unfixedIVars
                                          , const std::set<ItTreeVariable *> & unfixedDVars ) override;
  void                  ReplaceParVariable( const c3d::string_t & varName, ItTreeVariable & newVar ) override;
  /** \brief \ru Заменить переменные.
                     \en Replace variables. \~
            \details \ru Заменить все переменные с указанными именем на новую интервальную переменную.\n
                     \en Replace all variables with the specified names by a new interval variable.\n \~
          */
  virtual void                  ReplaceIntVariable( const c3d::string_t & /*varName*/, ItIntervalTreeVariable & /*newVar*/ ) {}

  /** \} */

  /**\ru \name Функции операции [].
     \en \name Functions of operation [].
      \{ */

          /// \ru Дать первый узел. \en Get the first node. 
          const BTreeNode &    GetFirstTree () const { return *m_firstValue;  }
          /// \ru Дать второй узел. \en Get the second node. 
          const BTreeNode &    GetSecondTree() const { return *m_secondValue; }
          /// \ru Дать первый узел. \en Get the first node. 
                BTreeNode &    SetFirstTree () { return *m_firstValue;  }
          /// \ru Дать второй узел. \en Get the second node. 
                BTreeNode &    SetSecondTree() { return *m_secondValue; }

          
  /** \} */
  //virtual size_t          SizeOf() const = 0;

private:
  // \ru не реализовано \en not implemented 
  void operator =( const TreeIntervalNode & );

DECLARE_PERSISTENT_CLASS_NEW_DEL( IntervalConstNode )
};

IMPL_PERSISTENT_OPS( IntervalConstNode )

//-----------------------------------------------------------------------------
/** \brief \ru Интервальная переменная как узел бинарного дерева.
           \en Interval variable as a node of a binary tree. \~
  \details \ru Узел дерева - интервальная переменная.\n
           \en Tree node is an interval variable.\n \~
  \ingroup Parser
*/
// ---
class MATH_CLASS IntervalIdentNode : public TreeIntervalNode
{
  ItIntervalTreeVariable * m_ident; ///< \ru Всегда не nullptr. \en Always not nullptr.

public:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по интервальной переменной.\n
             \en Constructor by an interval variable.\n \~
    \param[in] ident - \ru Интервальная переменная.
                       \en Interval variable. \~
  */
  IntervalIdentNode( ItIntervalTreeVariable & ident )
    : m_ident( &ident )    
  {}

  /// \ru Конструктор копирования. \en Copy-constructor. 
  IntervalIdentNode( const IntervalIdentNode & other );

public:
  /**\ru \name Функции узла дерева интервального выражения.
     \en \name Functions of the interval expression tree node.
      \{ */
  TeIntervalNodeType    IsA      () const override { return tei_Ident; } // \ru выдать тип узла дерева \en get type of a tree node
  IntervalIdentNode *   Duplicate() const override { return new IntervalIdentNode( *this ); }

  // \ru вычисление значения функции и производной по переменной V \en calculate the value of a function and the derivative with respect to V 
  EquTreeResCode        GetFirstValue ( double & ) const override;
  EquTreeResCode        GetSecondValue( double & ) const override;
  void                  GetString     ( c3d::string_t & ) const override {}
  // \ru Установить значение. \en Set value. 
  std::pair<bool, bool>  SetValue( double f, double s
                                           , const std::set<ItIntervalTreeVariable *> & unfixedIVars
                                           , const std::set<ItTreeVariable *> & unfixedDVars ) override;

  void                  ReplaceParVariable( const c3d::string_t & /*varName*/, ItTreeVariable & /*newVar*/ ) override {}
  //virtual void           ReplaceIntVariable( const string & varName, ItIntervalTreeVariable & newVar ){}
 
  /** \} */  

private:
  void operator = ( const IntervalIdentNode & );

DECLARE_PERSISTENT_CLASS_NEW_DEL( IntervalIdentNode )
};
   
IMPL_PERSISTENT_OPS( IntervalIdentNode )


//------------------------------------------------------------------------------
/** \brief \ru Константа как узел бинарного дерева.
           \en Constant as a node of a binary tree. \~
  \details \ru Узел бинарного дерева, обозначающий константу. \n
           \en Node of a binary tree denoting a constant. \n \~
  \ingroup Parser
*/
// ---
class MATH_CLASS BTreeConst : public BTreeNode
{
private:
  double        value;  ///< \ru Значение константы. \en Value of a constant. 
  c3d::string_t m_name; ///< \ru Имя константы, если есть, в файл не пишется. \en A name of a constant if it exists, not written to file. 

public:

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по значению.\n
             \en Constructor by value.\n \~  
    \param[in] val  - \ru Значение константы.
                      \en Value of a constant. \~
    \param[in] name - \ru Имя константы.
                      \en Name of a constant. \~
  */
  BTreeConst( double val, const c3d::string_t & name = _T("") );

  /// \ru Конструктор копирования. \en Copy-constructor. 
  BTreeConst( const BTreeConst & );

  virtual ~BTreeConst();
         
public:
  // \ru выдать тип узла дерева \en get type of a tree node
  BteNodeType IsA()       const override;
  BTreeNode * Duplicate() const override;

  // \ru Функция линейная. \en Function is linear.
  bool        IsLine()    const override { return false; }

  /**\ru \name Функции для вычисления значения и производной.
     \en \name Functions for calculation of the value and the derivative.
      \{ */
  // \ru вычисление значения функции и производной по переменной V \en calculate the value of a function and the derivative with respect to V 
  EquTreeResCode  GetValue        ( double & fvalue ) const override;
  EquTreeResCode  CalculateDerives( double &, double &, double &, double &, const VarsDerives & ) const override;

  bool            SetValue( double, const std::set<ItTreeVariable*> & ) override { return false; }

  /** \} */  
  /**\ru \name Функции замены переменных по именам.
     \en \name Functions for replacing variables by names.
      \{ */

  // \ru Заменить все переменные с именем varName на переменную newVar \en Replace all variables with name 'varName' by variable 'newVar'  
  void            ReplaceParVariable( const c3d::string_t & /*varName*/, ItTreeVariable & /*newVar*/ ) override {}
  void            ReplaceParVariable( const ItTreeVariable & /*var*/, const BTreeNode & /*subTree*/ ) override {}

  /** \} */  
  /**\ru \name Функции доступа к данным.
     \en \name Functions for access to data.
      \{ */

  /** \brief \ru Получить вложенный узел.
             \en Get a child node. \~
    \details \ru Получить вложенный узел по индексу.\n
             \en Get a child node by an index.\n \~
  */
  virtual BTreeNodeSPtr   GetSubNode( size_t /*i*/ ) { return nullptr; }

  bool            GetDefRange(DefRange &, ItTreeVariable &, bool /*stopOnBreak*/ ) const override { return true; }

  void            GetUsedVariables( SSArray<ItTreeVariable*> &, SSArray<ItUserFunc*> & )const override{}

  BTreeNodeSPtr   FixVars( const RPArray<ItTreeVariable> & /*unfixedVars*/,
                                 IFC_Array<ItUserFunc> & /*newFuncs*/, EquTreeResCode & ) const override
                                          { return BTreeNodeSPtr(Duplicate()); }

  BTreeNodeSPtr   GetCalcEquivalent() const override
                                          { return BTreeNodeSPtr(Duplicate()); }

  void            GetString( c3d::string_t & str ) const override;
  size_t          SizeOf() const override;

  // \ru доступ к данным \en access to data 
  double                  GetValue() const       { return value; } ///< \ru Дать переменную. \en Get variable. 
  void                    SetValue( double val ) { value = val;  } ///< \ru Установить значение переменную. \en Set value of a variable. 

  /** \} */  
  /**\ru \name Функции доступа к данным.
     \en \name Functions for access to data.
      \{ */

  /** \brief \ru Равен ли узел заданному узлу.
             \en Whether the node is equal to the given node. \~
    \details \ru Равен ли узел заданному узлу.\n
             \en Whether a node is equal to the given node.\n \~
    \param[in] other   - \ru Узел для сравнения.
                         \en Node for comparison. \~
    \param[in] equVars - \ru Набор пар равных переменных.
                         \en A set of pairs of equal variables. \~
    \return \ru true, если узлы равны.
            \en true if nodes are equal. \~
  */
  bool            IsEqual         ( const BTreeNode & other, const EqualVarsMap & equVars ) const override { return other.IsEqual( *this, equVars ); }
  
  /** \brief \ru Равен ли узел заданному узлу.
             \en Whether a node is equal to the given node. \~
    \details \ru Равен ли узел заданному узлу.\n
             \en Whether a node is equal to the given node.\n \~
    \param[in] other   - \ru Узел для сравнения.
                         \en Node for comparison. \~      
    \return \ru true, если узлы равны.
            \en true if nodes are equal. \~
  */
  bool            IsEqual         ( const BTreeConst & other, const EqualVarsMap & )        const override { return other.value == value; }
  /** \} */  
private:
  void operator =( const BTreeConst & ); // \ru не реализовано \en not implemented 
  size_t GetPseudoOrderByVar( ItTreeVariable & ) const override { return 0; } // \ru Только для внутреннего использования! \en For internal use only!

  DECLARE_PERSISTENT_CLASS_NEW_DEL( BTreeConst )
};

IMPL_PERSISTENT_OPS( BTreeConst )

//------------------------------------------------------------------------------
/** \brief \ru Переменная как узел бинарного дерева.
           \en Variable as a node of a binary tree. \~
  \details \ru Узел бинарного дерева, обозначающий переменную. \n
           \en Node of a binary tree denoting variable. \n \~
  \ingroup Parser
*/
// ---
class MATH_CLASS BTreeIdent : public BTreeNode
{
private:
  ItTreeVariable * id;  

public :
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по переменной. \n
             \en Constructor by a variable \n \~    
  */
  BTreeIdent( ItTreeVariable & );

  /// \ru Копирующий конструктор. \en Copy-constructor. 
  BTreeIdent( const BTreeIdent & );

  // \ru выдать тип узла дерева \en get type of a tree node 
  BteNodeType   IsA      () const override;
  BTreeNode   * Duplicate() const override;

  // \ru Функция линейная. \en Function is linear.
  bool          IsLine()    const override;

  /**\ru \name Функции для вычисления значения и производной.
     \en \name Functions for calculation of the value and the derivative.
      \{ */
  
  EquTreeResCode GetValue        ( double & fvalue ) const override;
  EquTreeResCode GetStringValue  ( c3d::string_t & svalue ) const override;
  EquTreeResCode CalculateDerives( double &, double &, double &, double &, const VarsDerives & ) const override;
  void           GetUsedVariables  ( SSArray<ItTreeVariable*> &, SSArray<ItUserFunc*> & ) const override;

  bool           SetValue( double, const std::set<ItTreeVariable*> & ) override;
  
  /** \} */  
  /**\ru \name Функции замены переменных по именам.
     \en \name Functions for replacing variables by names.
      \{ */

  // \ru Заменить все переменные с именем varName на переменную newVar \en Replace all variables with name 'varName' by variable 'newVar'  
  void          ReplaceParVariable( const c3d::string_t & varName, ItTreeVariable & newVar ) override;
  void          ReplaceParVariable( const ItTreeVariable &, const BTreeNode & /*subTree*/ ) override {}
  
  /** \} */  
  /**\ru \name Функции замены переменных по именам.
     \en \name Functions for replacing variables by names.
      \{ */

  /// \ru Дать вложенный узел по индексу. \en Get a child node by an index. 
  virtual BTreeNodeSPtr GetSubNode( size_t /*i*/ ) { return nullptr; }

  bool          GetDefRange( DefRange &, ItTreeVariable &, bool /*stopOnBreak*/ ) const override{ return true; }

  BTreeNodeSPtr  FixVars( const RPArray<ItTreeVariable> & unfixed,
                                IFC_Array<ItUserFunc> & newFuncs, EquTreeResCode & ) const override;

  BTreeNodeSPtr  GetCalcEquivalent() const override { return BTreeNodeSPtr(Duplicate()); }

  void           GetString( c3d::string_t & str ) const override { str += id->GetName(); }
  size_t         SizeOf() const override;

  // \ru доступ к данным \en access to data 
          ItTreeVariable & GetVariable() const { C3D_ASSERT( id ); return *id; } ///< \ru Дать параметрический идентификатор (переменную). \en Get parametric identifier (variable). 
          void             SetVariable( ItTreeVariable & var ) { id = &var; }      ///< \ru Установить параметрический идентификатор (переменную) \en Set a parametric identifier (variable). 
  
  /** \} */  
  /**\ru \name Функции сравнения.
     \en \name Comparison function.
      \{ */

  /** \brief \ru Равен ли узел заданному узлу.
             \en Whether a node is equal to the given node. \~
    \details \ru Равен ли узел заданному узлу.\n
             \en Whether a node is equal to the given node.\n \~
    \param[in] other   - \ru Узел для сравнения.
                         \en Node for comparison. \~
    \param[in] equVars - \ru Набор пар равных переменных.
                         \en A set of pairs of equal variables. \~
    \return \ru true, если узлы равны.
            \en true if nodes are equal. \~
  */
  bool            IsEqual( const BTreeNode & other, const EqualVarsMap & equVars ) const override { return other.IsEqual( *this, equVars ); }
  
  /** \brief \ru Равен ли узел заданному узлу.
             \en Whether a node is equal to the given node. \~
    \details \ru Равен ли узел заданному узлу.\n
             \en Whether a node is equal to the given node.\n \~
    \param[in] other   - \ru Узел для сравнения.
                         \en Node for comparison. \~
    \param[in] equVars - \ru Набор пар равных переменных.
                         \en A set of pairs of equal variables. \~
    \return \ru true, если узлы равны.
            \en true if nodes are equal. \~
  */
  bool            IsEqual( const BTreeIdent & other, const EqualVarsMap & equVars ) const override;

private:
  void operator = ( const BTreeIdent & ); // \ru не реализовано \en not implemented 
  size_t GetPseudoOrderByVar( ItTreeVariable & var ) const override { return ( &var == id ) ? 1 : 0; } // \ru Только для внутреннего использования! \en For internal use only!

  DECLARE_PERSISTENT_CLASS_NEW_DEL( BTreeIdent )
};

IMPL_PERSISTENT_OPS( BTreeIdent )

//------------------------------------------------------------------------------
/** \brief \ru Функция как узел бинарного дерева.
           \en Function as node of a binary tree. \~
  \details \ru Узел бинарного дерева, обозначающий функцию. \n
           \en Node of a binary tree denoting a function. \n \~
  \ingroup Parser
*/
// ---    
class MATH_CLASS BTreeFunction : public BTreeNode
{
public: 

  /** \brief \ru Типы функций.
             \en Types of functions. \~
    \details \ru Типы функций. \n
             \en Types of functions. \n \~         
  */
  // \ru ССА K13 не менять! Пишутся в файл! \en ССА K13 not to change! Are written to file!  
  enum EquFnCode
  {
    eFnCode_unknown = -1,            ///< \ru Неизвестный тип. \en An unknown type. 
    eFnCode_first = 0,               ///< \ru Начало диапазона известных функций. \en Start of the range of known functions. 
    eFnCode_sin = eFnCode_first,     ///< \ru Синус. \en Sine. 
    eFnCode_cos,                     ///< \ru Косинус. \en Cosine. 
    eFnCode_tan,                     ///< \ru Тангенс. \en Tangent. 
    eFnCode_sqrt,                    ///< \ru Квадратный корень. \en Square root. 
    eFnCode_atan,                    ///< \ru Арктангенс. \en Arctangent. 
    eFnCode_exp,                     ///< \ru Экспонента. \en Exponent. 
    eFnCode_ln,                      ///< \ru Натуральный логарифм. \en Natural logarithm. 
    eFnCode_abs,                     ///< \ru Модуль. \en Absolute value. 
    eFnCode_DegBegin,                ///< \ru Начала диапазона функций с аргументом в градусах. \en Start of the range of functions with argument in degrees. 
    eFnCode_sind = eFnCode_DegBegin, ///< \ru Синус угла в градусах. \en Sine of the angle in degrees. 
    eFnCode_cosd,                    ///< \ru Косинус угла в градусах. \en Cosine of the angle in degrees. 
    eFnCode_tand,                    ///< \ru Тангенс угла в градусах. \en Tangent of the angle in degrees. 
    eFnCode_DegEnd = eFnCode_tand,   ///< \ru Конец диапазона функций с аргументом в градусах. \en End of the range of functions with argument in degrees. 
    eFnCode_atand,                   ///< \ru Арктангенс с результатом в градусах. \en Arctangent with the result in degrees. 
    eFnCode_lg,                      ///< \ru Десятичный логарифм. \en Decimal logarithm. 
    eFnCode_ceil,                    ///< \ru Ближайшее большее целое число. \en Nearest largest integer number. 
    eFnCode_floor,                   ///< \ru Ближайшее меньшее целое число. \en Nearest smallest integer number. 
    eFnCode_round,                   ///< \ru Ближайшее целое число. \en Nearest integer number. 
    eFnCode_acos,                    ///< \ru Арккосинус. \en Arccosine. 
    eFnCode_acosd,                   ///< \ru Арккосинус с результатом в градусах. \en Arccosine with the result in degrees. 
    eFnCode_asin,                    ///< \ru Арксинус. \en Arcsine. 
    eFnCode_asind,                   ///< \ru Арксинус с результатом в градусах. \en Arcsine with the result in degrees. 
    eFnCode_rad,                     ///< \ru Перевод из градусов в радианы. \en Conversion from degrees to radians. 
    eFnCode_deg,                     ///< \ru Перевод из радиан в градусы. \en Conversion from radians to degrees. 
    eFnCode_last = eFnCode_deg       ///< \ru Конец диапазона известных функций. \en End of the range of known functions. 
  };

private :
  EquFnCode     fnCode;  // \ru код функции \en code of function 
  BTreeNodeSPtr par;     // \ru параметр функции \en parameter of the function 

public :
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор. \n
             \en Constructor. \n \~ 
    \param[in] code - \ru Тип функции.
                      \en Type of a function. \~      
    \param[in] p    - \ru Параметр функции.
                      \en Parameter of a function. \~
  */
  BTreeFunction( EquFnCode code, BTreeNode & p );

  /// \ru Копирующий конструктор. \en Copy-constructor. 
  BTreeFunction( const BTreeFunction & );

  /// \ru Деструктор. \en Destructor. 
  virtual  ~BTreeFunction();
          
public :
  // \ru выдать тип узла дерева \en get type of a tree node 
  BteNodeType   IsA      () const override;
  BTreeNode   * Duplicate() const override;

  // \ru Функция линейная. \en Function is linear.
  bool          IsLine()    const override;
 
  /**\ru \name Функции для вычисления значения и производной.
     \en \name Functions for calculation of the value and the derivative.
      \{ */

  EquTreeResCode  GetValue          ( double & value ) const override;
  // \ru вычисление значения функции и производной по переменной V \en calculate the value of a function and the derivative with respect to V 
  EquTreeResCode  CalculateDerives  ( double &, double &, double &, double &, const VarsDerives & ) const override;
  void            GetUsedVariables  ( SSArray<ItTreeVariable*> &, SSArray<ItUserFunc*> & ) const override;
  
  bool            SetValue          ( double, const std::set<ItTreeVariable*> & ) override { return false; } // \ru не реализовано \en not implemented
  /** \} */  
  /**\ru \name Функции замены переменных по именам.
     \en \name Functions for replacing variables by names.
      \{ */
                    
  void            ReplaceParVariable( const c3d::string_t & varName, ItTreeVariable & newVar ) override;
  void            ReplaceParVariable( const ItTreeVariable & var, const BTreeNode & subTree ) override;

  /** \} */  
  /**\ru \name Функции доступа к данным.
     \en \name Functions for access to data.
      \{ */

  /// \ru Дать вложенный узел по индексу. \en Get a child node by an index. 
  virtual BTreeNodeSPtr   GetSubNode( size_t i );
  bool            GetDefRange( DefRange &, ItTreeVariable &, bool stopOnBreak ) const override;
  BTreeNodeSPtr   FixVars( const RPArray<ItTreeVariable> & unfixed,
                                 IFC_Array<ItUserFunc> & newFuncs, EquTreeResCode & ) const override;

  BTreeNodeSPtr   GetCalcEquivalent() const override;

  void            GetString( c3d::string_t & str ) const override;
  size_t          SizeOf() const override;

  /** \} */  
  /**\ru \name Функции сравнения.
     \en \name Comparison function.
      \{ */

  /** \brief \ru Равен ли узел заданному узлу.
             \en Whether a node is equal to the given node. \~
    \details \ru Равен ли узел заданному узлу.\n
             \en Whether a node is equal to the given node.\n \~
    \param[in] other   - \ru Узел для сравнения.
                         \en Node for comparison. \~
    \param[in] equVars - \ru Набор пар равных переменных.
                         \en A set of pairs of equal variables. \~
    \return \ru true, если узлы равны.
            \en true if nodes are equal. \~
  */
  bool            IsEqual         ( const BTreeNode & other, const EqualVarsMap & equVars  ) const override { return other.IsEqual( *this, equVars ); }
  
  /** \brief \ru Равен ли узел заданному узлу.
             \en Whether a node is equal to the given node. \~
    \details \ru Равен ли узел заданному узлу.\n
             \en Whether a node is equal to the given node.\n \~           
    \param[in] other   - \ru Узел для сравнения.
                         \en Node for comparison. \~
    \param[in] equVars - \ru Набор пар равных переменных.
                         \en A set of pairs of equal variables. \~
    \return \ru true, если узлы равны.
            \en true if nodes are equal. \~
  */
  bool            IsEqual         ( const BTreeFunction & other, const EqualVarsMap & equVars )const override;
  /** \} */  

          bool            IsCos() const; ///< \ru имеет ли вид a * cos() + b \en looks like a * cos() + b



private:
  // \ru выдать значение параметра для экстремума( если это возможно ). \en get value of parameter of extremum( if it is possible ).   
  virtual bool            GetCharacterPoints( std::vector<CharacterPointInfo> & ) const;
  EquTreeResCode GetValue( double arg, double & value ) const;
  // \ru Только для внутреннего использования! \en For internal use only!
  size_t          GetPseudoOrderByVar( ItTreeVariable & var ) const override;
  void operator = ( const BTreeFunction & ); // \ru не реализовано \en not implemented 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( BTreeFunction ) 
};

IMPL_PERSISTENT_OPS( BTreeFunction )

//------------------------------------------------------------------------------
/** \brief \ru Дать тип функции.
           \en Get type of function. \~
  \details \ru Дать тип функции по имени.\n
           \en Get type of function by name.\n \~
  \param[in] name - \ru Имя функции.
                    \en Name of function. \~
  \return \ru Тип функции.
          \en Type of a function. \~
  \ingroup Parser
*/
// ---
BTreeFunction::EquFnCode GetFunCodeByName( const c3d::string_t & name );


//------------------------------------------------------------------------------
/** \brief \ru Дать имя функции.
           \en Get name of function. \~
  \details \ru Дать имя функции по типу.\n
           \en Get name of function by type.\n \~
  \param[in]  code - \ru Тип функции.
                     \en Type of a function. \~
  \param[out] name - \ru Имя функции.
                     \en Name of function. \~
  \ingroup Parser
*/
// ---
void GetFunNameByCode( BTreeFunction::EquFnCode code, c3d::string_t & name );


//------------------------------------------------------------------------------
/** \brief \ru Операция с двумя аргументами как узел бинарного дерева.
           \en Operation with two arguments as node of binary tree. \~
  \details \ru Узел бинарного дерева, обозначающий операцию с двумя аргументами. \n
           \en Node of binary tree denoting operation with two arguments. \n \~
  \ingroup Parser
*/
// ---
class MATH_CLASS BTreeOperation : public BTreeNode
{
private:
  PceOperationType  opCode; ///< \ru Код операции. \en Code of operation. 
  BTreeNodeSPtr     op1;    ///< \ru Первый операнд. \en The first operand. 
  BTreeNodeSPtr     op2;    ///< \ru Второй операнд. \en The second operand. 

public:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор.\n
             \en Constructor.\n \~
    \param[in] code - \ru Код операции.
                      \en Code of operation. \~
    \param[in] op1  - \ru Первый операнд.
                      \en The first operand. \~
    \param[in] op2  - \ru Второй операнд.
                      \en The second operand. \~
    \ingroup Parser
  */
  BTreeOperation( PceOperationType code, BTreeNode & op1, BTreeNode & op2 );

  /// \ru Конструктор копирования. \en Copy-constructor. 
  BTreeOperation( const BTreeOperation & );

  /// \ru Деструктор. \en Destructor. 
  virtual  ~BTreeOperation();

public:
  // \ru выдать тип узла дерева \en get type of a tree node 
  BteNodeType     IsA      () const override;
  BTreeNode *     Duplicate() const override;

  // \ru Функция линейная. \en Function is linear.
  bool            IsLine()    const override;
  
  /**\ru \name Функции для вычисления значения и производной.
     \en \name Functions for calculation of the value and the derivative.
      \{ */

  // \ru вычисление значения функции и производной по переменной V \en calculate the value of a function and the derivative with respect to V 
  EquTreeResCode  GetValue        ( double & fvalue ) const override;
  EquTreeResCode  GetStringValue  ( c3d::string_t & sVal ) const override;
  EquTreeResCode  CalculateDerives( double &, double &, double &, double &, const VarsDerives & ) const override;
  void            GetUsedVariables( SSArray<ItTreeVariable*> &, SSArray<ItUserFunc*> & ) const override;
  bool            SetValue        ( double, const std::set<ItTreeVariable *> & ) override { return false; }// \ru не реализовано \en not implemented

  /** \} */  
  /**\ru \name Функции замены переменных по именам.
     \en \name Functions for replacing variables by names.
      \{ */

  // \ru Заменить все переменные с именем varName на переменную newVar \en Replace all variables with name 'varName' by variable 'newVar'  
  void            ReplaceParVariable( const c3d::string_t & varName, ItTreeVariable & newVar ) override;
  void            ReplaceParVariable( const ItTreeVariable & var, const BTreeNode & subTree ) override;

  /// \ru Заменить операнд. \en Replace the operand.
  BTreeNodeSPtr ReplaceSubNode( BTreeNode & newNode, size_t opNum );

  /** \} */  
  /**\ru \name Функции доступа к данным.
     \en \name Functions for access to data.
      \{ */

  /// \ru Дать вложенный узел по индексу. \en Get a child node by an index. 
  virtual BTreeNodeSPtr   GetSubNode( size_t i );
  bool            GetDefRange( DefRange &, ItTreeVariable &, bool stopOnBreak ) const override;

  BTreeNodeSPtr   FixVars( const RPArray<ItTreeVariable> & unfixed,
                                 IFC_Array<ItUserFunc> & newFuncs, EquTreeResCode & ) const override;
  BTreeNodeSPtr   GetCalcEquivalent() const override;

  void            GetString( c3d::string_t & str ) const override;
  size_t          SizeOf() const override;

  /** \} */  
  /**\ru \name Функции сравнения.
     \en \name Comparison function.
      \{ */

  /** \brief \ru Равен ли узел заданному узлу.
             \en Whether a node is equal to the given node. \~
    \details \ru Равен ли узел заданному узлу.\n
             \en Whether a node is equal to the given node.\n \~
    \param[in] other   - \ru Узел для сравнения.
                         \en Node for comparison. \~
    \param[in] equVars - \ru Набор пар равных переменных.
                         \en A set of pairs of equal variables. \~
    \return \ru true, если узлы равны.
            \en true if nodes are equal. \~
  */
  bool IsEqual( const BTreeNode & other, const EqualVarsMap & equVars ) const override { return other.IsEqual( *this, equVars ); }

  /** \brief \ru Равен ли узел заданному узлу.
             \en Whether a node is equal to the given node. \~
    \details \ru Равен ли узел заданному узлу.\n
             \en Whether a node is equal to the given node.\n \~
    \param[in] other   - \ru Узел для сравнения.
                         \en Node for comparison. \~
    \param[in] equVars - \ru Набор пар равных переменных.
                         \en A set of pairs of equal variables. \~
    \return \ru true, если узлы равны.
            \en true if nodes are equal. \~
  */
  bool IsEqual( const BTreeOperation & other, const EqualVarsMap & equVars ) const override;
    
  /** \} */  
  /**\ru \name Функции доступа к данным.
     \en \name Functions for access to data.
      \{ */

  /// \ru Код операции. \en Code of operation. 
  int16       GetOperationCode() const { return (int16)opCode;  }
  /// \ru Первый операнд. \en The first operand. 
  BTreeNode & GetFirstOperand () const { C3D_ASSERT( op1 ); return *op1; }
  /// \ru Второй операнд. \en The second operand. 
  BTreeNode & GetSecondOperand() const { C3D_ASSERT( op2 ); return *op2; }
  /** \} */  

  ///< \ru имеет ли вид a * cos() + b. \en look like a * cos() + b.
  virtual bool IsCos ( double &a, double& b ) const;

private:
  void operator = ( const BTreeOperation & ); // \ru не реализовано \en not implemented 
  EquTreeResCode      GetValue( double par1, double par2, double & value ) const;
  // \ru Вычисление значения функции строковых переменных. \en Get value of a function for string variables.
  EquTreeResCode      GetValue( const c3d::string_t & par1, const c3d::string_t &  par2, double & value ) const;
  EquTreeResCode      GetValue( const c3d::string_t & par1, const c3d::string_t &  par2, c3d::string_t & sValue ) const;
  virtual bool        GetCharacterPoints( std::vector<CharacterPointInfo> &, const ItTreeVariable & ) const; 
  // \ru Только для внутреннего использования! \en For internal use only!
  size_t      GetPseudoOrderByVar( ItTreeVariable & var )          const override;

  DECLARE_PERSISTENT_CLASS_NEW_DEL( BTreeOperation ) 
};

IMPL_PERSISTENT_OPS( BTreeOperation )

//------------------------------------------------------------------------------
/** \brief \ru Операция с одним аргументом как узел бинарного дерева.
           \en Operation with one argument as a node of a binary tree. \~
  \details \ru Узел бинарного дерева, обозначающий операцию с одним аргументом. \n
           \en Node of a binary tree denoting an operation with one argument. \n \~
  \ingroup Parser
*/
// ---
class MATH_CLASS BTreeOperation1Arg : public BTreeNode
{
private :
  PceOperationType opCode;  ///< \ru Код операции. \en Code of operation. 
  BTreeNodeSPtr    op;      ///< \ru Операнд. \en Operand. 

public :
  BTreeOperation1Arg( PceOperationType code, BTreeNode & );
  BTreeOperation1Arg( const BTreeOperation1Arg & );
  virtual  ~BTreeOperation1Arg();

  // \ru выдать тип узла дерева \en get type of a tree node
  BteNodeType     IsA      () const override;
          PceOperationType  GetOperationType() const { return opCode; }
   BTreeNode *     Duplicate() const override;

  // \ru Функция линейная. \en Function is linear.
  bool            IsLine()    const override;

  // \ru вычисление значения функции и производной по переменной V \en calculate the value of a function and the derivative with respect to V 
  EquTreeResCode  GetValue        ( double &fvalue ) const override;
  EquTreeResCode  GetStringValue  ( c3d::string_t & sVal ) const override;
  EquTreeResCode  CalculateDerives( double &, double &, double &, double &, const VarsDerives & ) const override;
  void            GetUsedVariables( SSArray<ItTreeVariable*> &, SSArray<ItUserFunc*> & ) const override;

  bool            SetValue         ( double, const std::set<ItTreeVariable *> & ) override;

  /// \ru Заменить все переменные с именем varName на переменную newVar \en Replace all variables with name 'varName' by variable 'newVar'  
  void            ReplaceParVariable( const c3d::string_t & varName, ItTreeVariable & newVar ) override;
  void            ReplaceParVariable( const ItTreeVariable &, const BTreeNode & ) override;
  virtual BTreeNodeSPtr   GetSubNode( size_t i );
  // \ru выдать значение параметра для экстремума( если это возможно ). \en get value of parameter of extremum( if it is possible ).   
  virtual bool            GetCharacterPoints( std::vector<CharacterPointInfo> & ) const;
  bool            GetDefRange( DefRange & range, ItTreeVariable & var, bool stopOnBreak ) const override { C3D_ASSERT( op );
                                                                                                          return op->GetDefRange( range, var, stopOnBreak ); }
  BTreeNodeSPtr   FixVars( const RPArray<ItTreeVariable> & unfixed,
                                 IFC_Array<ItUserFunc> & newFuncs, EquTreeResCode & ) const override;
  BTreeNodeSPtr   GetCalcEquivalent() const override;

  void            GetString( c3d::string_t & str ) const override;
  bool            IsEqual         ( const BTreeNode & other, const EqualVarsMap & equVars ) const override { return other.IsEqual( *this, equVars ); }
  bool            IsEqual         ( const BTreeOperation1Arg & other, const EqualVarsMap & equVars ) const override;
  size_t          SizeOf() const override;

private:
  void operator =( const BTreeOperation1Arg & ); // \ru не реализовано \en not implemented 
  EquTreeResCode GetValue( double arg, double & value ) const;
  // \ru Только для внутреннего использования! \en For internal use only!
  size_t GetPseudoOrderByVar( ItTreeVariable & var ) const override { return opCode == oprt_NOT ? SYS_MAX_T : op->GetPseudoOrderByVar(var); }

  DECLARE_PERSISTENT_CLASS_NEW_DEL( BTreeOperation1Arg ) 
};

IMPL_PERSISTENT_OPS( BTreeOperation1Arg )

//------------------------------------------------------------------------------
/** \brief \ru Операция с тремя аргументами как узел синтаксического дерева.
           \en Operation with three arguments as node of syntax tree. \~
  \details \ru Узел бинарного дерева, обозначающий операцию с тремя аргументами. \n
           \en Node of binary tree denoting operation with three arguments. \n \~
  \ingroup Parser
*/
// ---
class MATH_CLASS BTreeOperation3Args : public BTreeNode
{
private :
  BTreeNodeSPtr op1; ///< \ru Первый операнд. \en The first operand. 
  BTreeNodeSPtr op2; ///< \ru Второй операнд. \en The second operand. 
  BTreeNodeSPtr op3; ///< \ru Третий операнд. \en The third operand. 

public :
  BTreeOperation3Args( BTreeNode &, BTreeNode &, BTreeNode & );
  BTreeOperation3Args( const BTreeOperation3Args & );
  virtual  ~BTreeOperation3Args();

  // \ru выдать тип узла дерева \en get type of a tree node
  BteNodeType     IsA() const override;
  BTreeNode *     Duplicate() const override;

  // \ru Функция линейная. \en Function is linear.
  bool            IsLine()    const override;

  // \ru вычисление значения функции и производной по переменной V \en calculate the value of a function and the derivative with respect to V 
  EquTreeResCode  GetValue          ( double & fVal ) const override;
  EquTreeResCode  GetStringValue    ( c3d::string_t & sVal ) const override;
  EquTreeResCode  CalculateDerives  ( double &, double &, double &, double &, const VarsDerives & ) const override;
  void            GetUsedVariables  ( SSArray<ItTreeVariable *> &, SSArray<ItUserFunc *> & ) const override;

  bool            SetValue         ( double, const std::set<ItTreeVariable*> & ) override{ return false;} // \ru не реализовано \en not implemented

  /// \ru Заменить все переменные с именем varName на переменную newVar \en Replace all variables with name 'varName' by variable 'newVar'  
  void            ReplaceParVariable( const c3d::string_t & varName, ItTreeVariable & newVar ) override;
  void            ReplaceParVariable( const ItTreeVariable &, const BTreeNode & ) override;
  // \ru Заменить операнд. \en Replace the operand.
  BTreeNodeSPtr   ReplaceSubNode( BTreeNode & newNode, size_t opNum );
  virtual BTreeNodeSPtr   GetSubNode( size_t i );
  bool            GetDefRange( DefRange &, ItTreeVariable &, bool stopOnBreak ) const override;

  BTreeNodeSPtr   FixVars( const RPArray<ItTreeVariable> & unfixed,
                                 IFC_Array<ItUserFunc> & newFuncs, EquTreeResCode & ) const override;
  BTreeNodeSPtr   GetCalcEquivalent() const override;

  void            GetString( c3d::string_t & str ) const override;
  bool            IsEqual         ( const BTreeNode & other, const EqualVarsMap & equVars ) const override { return other.IsEqual( *this, equVars ); }
  bool            IsEqual         ( const BTreeOperation3Args & other, const EqualVarsMap & equVars ) const override;
  size_t          SizeOf() const override;

private:
  // \ru выдать значение параметра для экстремума( если это возможно ). \en get value of parameter of extremum( if it is possible ).   
  virtual bool            GetCharacterPoints( std::vector<CharacterPointInfo> &, ItTreeVariable & ) const; 
  // \ru Только для внутреннего использования! \en For internal use only!
  size_t          GetPseudoOrderByVar( ItTreeVariable & ) const override { return SYS_MAX_T; }
  void operator =( const BTreeOperation3Args & ); // \ru не реализовано \en not implemented 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( BTreeOperation3Args ) 
};

IMPL_PERSISTENT_OPS( BTreeOperation3Args )

//------------------------------------------------------------------------------
/** \brief \ru Пользовательская функция как узел бинарного дерева.
           \en User-defined function as a node of a binary tree. \~
  \details \ru Узел бинарного дерева, обозначающий пользовательскую функцию. \n
           \en Node of a binary tree denoting a user-defined function. \n \~
  \ingroup Parser
*/
// ---
class MATH_CLASS BTreeUserFunc : public BTreeNode
{
  SPtr<ItUserFunc>     func; ///< \ru Пользовательская функция. \en A user-defined function. 
  IFC_Array<BTreeNode> pars;

public:
  // Конструктор копирования
  BTreeUserFunc( const BTreeUserFunc & other )
    : BTreeNode {}
    , func( &other.func->Duplicate() )
  {
    for ( size_t i = 0, c = other.pars.size(); i < c; ++i ) {
      pars.Add( other.pars[i]->Duplicate() );
    }
  }
  // Конструктор
  template <class BTreeNodesVector>
  BTreeUserFunc( ItUserFunc & _func, const BTreeNodesVector & _pars )
    : func( &_func )
  {
    C3D_ASSERT( _pars.size() == func->GetParsCount() );
    pars.reserve( _pars.size() );
    for ( size_t i = 0, c = _pars.size(); i < c; ++i ) {
      pars.Add( _pars[i] );
    }
  }

  // \ru выдать тип узла дерева \en get type of a tree node
  BteNodeType     IsA      () const override { return bt_Function; }
  BTreeNode *     Duplicate() const override { return new BTreeUserFunc( *this ); }

  // \ru Функция линейная. \en Function is linear.
  bool            IsLine()    const override { return false; }

  bool            SetValue( double, const std::set<ItTreeVariable *> & ) override { return false; } // \ru не реализовано \en not implemented

  // \ru вычисление значения функции и производной по переменной V \en calculate the value of a function and the derivative with respect to V 
  EquTreeResCode  GetValue        ( double & ) const override;
  EquTreeResCode  CalculateDerives( double &, double &, double &, double &, const VarsDerives & ) const override;
  void            GetUsedVariables( SSArray<ItTreeVariable *> &, SSArray<ItUserFunc *> & ) const override;
  /// \ru Заменить все переменные с именем varName на переменную newVar \en Replace all variables with name 'varName' by variable 'newVar'  
  void            ReplaceParVariable( const c3d::string_t & varName, ItTreeVariable & newVar ) override;
  void            ReplaceParVariable( const ItTreeVariable &, const BTreeNode & ) override;
  virtual BTreeNodeSPtr   GetSubNode        ( size_t i );
  // \ru выдать значение параметра для экстремума( если это возможно ). \en get value of parameter of extremum( if it is possible ).   
// ССА K13   virtual bool           GetCharacterPoints( std::vector<CharacterPointInfo> & ) const { return false; }
  bool            GetDefRange( DefRange &, ItTreeVariable &, bool stopOnBreak ) const override;

  BTreeNodeSPtr   FixVars( const RPArray<ItTreeVariable> & unfixed,
                                 IFC_Array<ItUserFunc> & newFuncs, EquTreeResCode & ) const override;
  BTreeNodeSPtr   GetCalcEquivalent() const override;

  void            GetString( c3d::string_t & str ) const override;
  bool            IsEqual  ( const BTreeNode &     other, const EqualVarsMap & equVars ) const override { return other.IsEqual( *this, equVars ); }
  bool            IsEqual  ( const BTreeUserFunc & other, const EqualVarsMap & equVars ) const override;
  size_t          SizeOf() const override { return 0; };

private:
  void operator =( const BTreeOperation3Args & ); // \ru не реализовано \en not implemented 
  EquTreeResCode GetValue( SArray<double> &, double & value ) const;
  // \ru Только для внутреннего использования! \en For internal use only!
  size_t GetPseudoOrderByVar( ItTreeVariable & var ) const override;

DECLARE_PERSISTENT_CLASS_NEW_DEL( BTreeUserFunc )
};

IMPL_PERSISTENT_OPS( BTreeUserFunc )

#endif // __PARS_EQUATION_TREE_H
