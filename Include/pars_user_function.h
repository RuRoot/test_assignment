////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Пользовательская функция.
         \en User-defined function. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __PARS_USER_FUNCTION_H
#define __PARS_USER_FUNCTION_H


#include <io_tape_define.h>
#include <templ_p_array.h>
#include <templ_ifc_array.h>
#include <pars_tree_variable.h>
#include <pars_variable.h>
#include <tool_mutex.h>
#include <math_define.h>


//------------------------------------------------------------------------------
/** \brief \ru Пользовательская функция.
           \en User-defined function. \~
  \details \ru Пользовательская функция. \n
           \en User-defined function. \n \~
  \ingroup Parser
*/
// ---  
class MATH_CLASS MbUserFunc : public TapeBase, public ItUserFunc, public MbSyncItem
{
protected:
  c3d::string_t             m_name;       ///< \ru Имя пользовательской функции. \en A name of user-defined function. 
  c3d::string_t             m_expression; ///< \ru Выражение. \en Expression. 
  SPtr<BTreeNode>           m_tree;       ///< \ru Дерево разбора выражения (обязательно должно быть). \en A tree of expression parsing (it should be for sure). 
  IFC_Array<MbTreeVariable> m_vars;       ///< \ru Формальные параметры функции. \en Formal parameters of function. 
  IFC_Array<ItUserFunc>     m_intFuncs;   ///< \ru Внутренние функции. \en Internal functions. 

public:
  MbUserFunc( const c3d::string_t & name, const std::vector<c3d::string_t> & pars ); ///< \ru Конструктор по имени функции и массиву аргументов. \en Constructor by function name and array of arguments. 
  MbUserFunc( const MbUserFunc & other );                                            ///< \ru Конструктор копирования. \en Copy-constructor. 
  MbUserFunc( const c3d::string_t & name );                                          ///< \ru Конструктор по имени функции. \en Constructor by function name. 
  virtual ~MbUserFunc();

public:
                /// \ru Сделать копию. \en Create a copy. 
        ItUserFunc &      Duplicate() const override { return *new MbUserFunc(*this); }
                /// \ru Получить имя функции. \en Get function name. 
        const c3d::string_t &  GetName()   const override { return m_name; }
                /// \ru Получить область определения функции. \en Get function domain. 
        bool              GetDefRange( DefRange &, ItTreeVariable & var,
                                               const std::vector<BTreeNode *> & ) const override;
  /** \brief \ru Получить область определения функции. \en Get function domain. \~
    \details \ru Получить область определения функции для параметра с индексом ind. Метод вырезает из заданной области определения участки, где функция не определена. \n
             \en Get the function domain for the parameter with index ind. The method cuts from the initial domain some parts where the function is not defined. \n \~
    \param[in, out] dr     - \ru Инициализируется некоторым начальным диапазоном. На выходе - пересечение этого диапазона с реальной областью определения функции.
                             \en It is initialized by some start range. The output value will be the intersection between this range and the true function domain. \~
    \param[in] ind         - \ru Индекс переменной, для которой ищется область определения.
                             \en An index of the variable, which the domain is searched for. \~
    \param[in] stopOnBreak - \ru Остановить поиск если найден разрыв.
                             \en Searching will be stopped if the first break is found. \~
    \return \ru Возвращает false если область определения найти не удалось.
            \en Returns false if domain cannot be found. \~
  */
        bool              GetDefRange( DefRange &, size_t ind, bool stopOnBreak ) const;

                /// \ru Количество аргументов (параметров). \en Parameters (arguments) count. 
        size_t            GetParsCount() const override { return m_vars.Count(); }
                /// \ru Внешние переменные. \en External variables. 
        void              GetExternalVars( SSArray<ItTreeVariable *> & vars
                                                 , SSArray<ItUserFunc *> & funcs ) const override;

                /// \ru Сравнить пользовательские функции. \en Compare symbolic (user-defined) functions. 
        bool              IsEqual( const ItUserFunc & other ) const override { return other.IsEqual( *this ); }
                /// \ru Сравнить пользовательские функции. \en Compare symbolic (user-defined) functions. 
        bool              IsEqual( const MbUserFunc & ) const override;
                /// \ru Вычислить значение функции в случае массива аргументов \en Calculate the value of a function in a case of arguments array 
        EquTreeResCode    GetValue( const SArray<double> & params, double & v ) const override;
                /// \ru Вычислить значение функции и производные в случае массива аргументов \en Calculate the value of a function and derivatives in a case of arguments array 
        EquTreeResCode    GetDerivates( const SArray<DerivesValues> & params,
                                                double & v, double & fd,
                                                double & sd, double & td,
                                                size_t dIndex = 0 ) const override;
                /// \ru Вычислить значение функции в случае одного аргумента \en Calculate the value of a function in a case of one argument 
                EquTreeResCode    GetValue( double t, double & v ) const;
                /// \ru Вычислить значение функции и производные в случае одного аргумента \en Calculate the value of a function and derivatives in a case of one argument 
                EquTreeResCode    GetDerivates( double t,
                                                double & v, double & fd,
                                                double & sd, double & td ) const;  
                /// \ru Функция константная. \en Function is const.
                bool              IsConst() const;
                /// \ru Функция линейная. \en Function is linear.
                bool              IsLine() const;

                /// \ru Установить выражение. \en Set expression. 
                EquTreeResCode    SetExpression( const c3d::string_t & expr, const BTreeNode & tree );
                EquTreeResCode    SetExpression( const TCHAR* expr, const BTreeNode & tree );
                /// \ru Получить выражение. \en Get expression. 
          const c3d::string_t &   GetExpression() const { return m_expression; }
                /// \ru Получить дерево разбора выражения \en Get expression parsing tree. 
          const BTreeNode &       GetTree()       const { return *m_tree; }    
                /// \ru Сравнение с пользовательской функцией. \en Comparison with user-defined function. 
                bool              IsEqualValue( const MbUserFunc & other ) const; 
                /// \ru Получить массив параметров особых точек для данного аргумента, заданного индексом на заданном интервале. \en Get an array of parameters of singular points for the argument which is given by the index on the given interval. 
                bool              GetExtremumPoints( size_t parIndex, const c3d::DoublePair & interval, c3d::DoubleVector & points );
                /// \ru Получить массив параметров. \en Get array of parameters. 
                void              GetPars ( RPArray<ItTreeVariable> & pars ) const; 
                /// \ru Получить аргумент по индексу. \en Get argument by index. 
                ItTreeVariable *  GetPar( size_t i ) const { return i < m_vars.Count() ? m_vars[i] : nullptr; }
                /// \ru Подготовить объект к записи. \en Prepare an object for writing. 
                void              WritingBeginEnd( bool begin ) { RegisterVars( begin ? registrable : noRegistrable ); }
                /// \ru Оператор присваивания. \en Assignment operator. 
          const MbUserFunc &      operator = ( const MbUserFunc & );
private:
  /// \ru Только для внутреннего использования! Порядок функции по номеру параметра. \en For internal use only! Degree of function by the number of parameter. 
        size_t            GetPseudoOrderByPar( size_t index ) const override;
                void              RegisterVars( RegistrableRec ) const;

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbUserFunc )
};

IMPL_PERSISTENT_OPS( MbUserFunc )

#endif // __PARS_USER_FUNCTION_H
