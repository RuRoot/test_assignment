////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Создание скалярных функций.
         \en Creation of scalar functions. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __FUNCTION_FACTORY_H
#define __FUNCTION_FACTORY_H


#include <function.h>
#include <tool_cstring.h>
#include <mb_operation_result.h>
#include <tool_mutex.h>


class MbListVars;
class MbMathematicalNode;
class MbTransitionMarker;

namespace c3d // namespace C3D
{
typedef std::vector<MbTransitionMarker> TransitionMarkerVector;
}


//------------------------------------------------------------------------------
/** \brief \ru Маркер перехода.
           \en A transition marker. \~
  \details \ru Поле m_position определяет положение маркера.
    Тип перехода m_transition устанавливает поведение функции на следующим за
    маркером (справа) интервалом:
    Linear - линейная функция,
    Blend  - гладкая сплайновая функция с непрерывной первой производной,
    Minmax - сплайновая функция с фиксированной нулевой первой производной справа от точки
    маркера (df(t)/dt = 0, при t = m_position+0).
    Тип перехода Minmax в последнем маркере установит нулевое значение производной слева от последней
    точки, если предыдущий маркер имеет типы Blend или Minmax.
           \en A marker's position is specified by m_position.
    The transition type m_transition sets the function's behavior in the interval immediately
    following the marker to the right:
    Linear - a linear function,
    Blend - a smooth spline function with a continuous first derivative,
    Minmax - a spline function with a fixed zero first derivative to the right of the marker point (df(t)/dt = 0, at t = m_position+0).
    For the final marker, the Minmax transition type will set the derivative value to zero to the left of the last point, provided
    the preceding marker has a Blend or Minmax type. \~
*/
// ---
class MATH_CLASS MbTransitionMarker
{
public:
  /** \brief \ru Тип перехода.
             \en The transition type.
  */
  enum class TransitionType
  {
    Linear = 0, ///< \ru Линейный. \en Linear.
    Blend = 1,  ///< \ru Сплайновый. \en Spline.
    Minmax = 2  ///< \ru Сплайновый с фиксированной нулевой производной. \en Spline with zero derivative.
  };

private:
  double         m_position;   ///< \ru Положение маркера. \en A marker's position.
  double         m_value;      ///< \ru Значение. \en Value.
  TransitionType m_transition; ///< \ru Тип перехода. \en Transition type.

public:
   /** \brief \ru Конструктор.
              \en Constructor. \~
     \details \ru Конструктор по параметрам.
              \en Constructor by parameters. \~
     \param[in] t -          \ru Положение маркера.
                             \en A marker's position. \~
     \param[in] value -      \ru Значение.
                             \en Value. \~
     \param[in] transition - \ru Тип перехода.
                             \en Transition type. \~
  */
  explicit MbTransitionMarker( const double t, const double value, const TransitionType transition )
    : m_position   ( t          )
    , m_value      ( value      )
    , m_transition ( transition ) {}

public:
  ///< \ru Вернуть положение маркера. \en Return a marker's position. \~
  double GetPosition() const { return m_position; };
  ///< \ru Вернуть значение. \en Return value. \~
  double GetValue() const { return m_value; };
  ///< \ru Вернуть тип перехода. \en Return transition type. \~
  TransitionType GetTransition() const { return m_transition; };
};


//------------------------------------------------------------------------------
/** \brief \ru Фабрика функций.
           \en Factory of functions. \~
  \details \ru Фабрика функций. \n
           \en Factory of functions. \n \~
  \ingroup Functions
*/
// ---
class MATH_CLASS MbFunctionFactory : public MbSyncItem {
private:
  mutable MbResultType status; ///< \ru Результат создания. \en The result of creation. 

public :
  /// \ru Конструктор. \en Constructor. 
  MbFunctionFactory(); 
  virtual ~MbFunctionFactory();

public:
  MbResultType Status() const; ///< \ru Получить статус создания. \en Get the state of creation. 

public:
  /** \brief \ru Создать символьную функцию.
             \en Create a symbolic function. \~
    \details \ru Создать символьную функцию из ее строкового представления.\n
             \en Create symbolic function from its string representation.\n \~
    \param[in] data        - \ru Строковое представление функции.
                             \en String representation of a function. \~
    \param[in] argument    - \ru Признак замкнутости.
                             \en String representation of a function argument. \~
    \param[in] tmin        - \ru Минимальное значение функции.
                             \en Minimum function value. \~
    \param[in] tmax        - \ru Максимальное значение функции.
                             \en Maximum function value. \~
    \param[in] useFirstVar - \ru Флаг выбора используемой аналитической функции: при useFirstVar = true используется MdAnalyticalFunction; при useFirstVar = false используется MdCharacterFunction.
                             \en Flag for selecting the analytical function to use: if useFirstVar = true, MdAnalyticalFunction is used; when useFirstVar = false, MdCharacterFunction is used. \~
  */
  MbFunction * CreateAnalyticalFunction ( const c3d::string_t & data,
                                          const c3d::string_t & argument,
                                          double tmin, double tmax, bool useFirstVar = false )  const;
  /// \ru Создать постоянную функцию. \en Create a constant function. 
  MbFunction * CreateConstFunction      ( double value )                                        const;
  /// \ru Создать линейную функцию. \en Create a linear function. 
  MbFunction * CreateLineFunction       ( double v1, double v2, double t1, double t2 )          const;
  /// \ru Создать кубическую функцию. \en Create a cubic function. 
  MbFunction * CreateCubicFunction      ( const SArray<double> & values, bool closed )          const;
  /// \ru Создать кубическую функцию. \en Create a cubic function. 
  MbFunction * CreateCubicFunction      ( const SArray<double> & values, 
                                          const SArray<double> & params, bool closed )          const;
  /// \ru Создать кубическую функцию. \en Create a cubic function. 
  MbFunction * CreateCubicFunction      ( const SArray<double> & values,
                                          const SArray<double> & firsts,
                                          const SArray<double> & params, bool closed )          const;

  /** \brief \ru Создать незамкнутую функцию по маркерам перехода.
             \en Create a non-closed function based on transition markers. \~
    \param[in] markers - \ru Список маркеров перехода.
    \return \ru Незамкнутая функция, определенная от первого до последнего маркера.
            \en A non-closed function defined from the first to the last marker. \~
  */
  c3d::FunctionSPtr CreateFunctionByTransitions ( const c3d::TransitionMarkerVector & markers ) const;

  /** \brief \ru Создать замкнутую функцию по маркерам перехода на заданном периоде.
             \en Create a closed function based on transition markers at a given period. \~
    \param[in] markers   - \ru Список маркеров перехода.
                           \en List of transition markers. \~
    \param[in] periodBeg - \ru Начало периода.
                           \en Start point of the period. \~
    \param[in] periodEnd - \ru Конец периода.
                           \en End point of the period. \~
    \return \ru Замкнутая функция, определенная на периоде.
            \en A closed function defined on a period. \~
  */
  c3d::FunctionSPtr CreateFunctionByTransitions ( const c3d::TransitionMarkerVector & markers,
                                                  const double                        periodBeg,
                                                  const double                        periodEnd ) const;

OBVIOUS_PRIVATE_COPY( MbFunctionFactory )
};


//------------------------------------------------------------------------------
// \ru Найти в строке переменные \en Find variables in string 
// ---
const MbMathematicalNode * GetVarListForAnalyticalFunc( const c3d::string_t & data, MbListVars & vars );


#endif // __FUNCTION_FACTORY_H
