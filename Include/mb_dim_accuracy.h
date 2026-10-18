////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru N-мерная точность.
         \en N-dimensional accuracy. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MB_DIM_ACCURACY_H
#define __MB_DIM_ACCURACY_H

#include <math_define.h>
#include <mb_variables.h>


//------------------------------------------------------------------------------
/** \brief \ru N-мерная точность.
           \en N-dimensional accuracy. \~
  \details \ru N-мерная точность задается N положительными числами.
    Наиболее востребованы двумерная и трехмерная точности. 
    Компоненты точности в этих случаях соответствуют точностям вдоль осей x, y и z. 
    Двумерная точность может использоваться, например, при выполнении операций над
    двумерными кривыми MbCurve. В этом случае компоненты точности будут рассчитываться
    с использованием методов GetUEpsilon() и GetVEpsilon(), вызванных для 
    поверхности (MbSurface) на которой лежит данная кривая. 
           \en N-dimensional accuracy is specified by N positive numbers. 
    Two-dimensional and three-dimensional accuracies are the most demanded.  
    Components of accuracy in these cases correspond to accuracies along x, y and z axes. 
    Two-dimensional precision can be used, for example, when performing operations on two-dimensional MbCurve curves. 
    In this case, the accuracy components will be calculated using 
    the GetUEpsilon() and GetVEpsilon() methods called 
    for the surface (MbSurface) on which the curve lies. \~ 
  \ingroup Legend
  \warning \ru В разработке. \en Under development.
*/
// ---

class MATH_CLASS MbDimAccuracy
{
private :
  c3d::DoubleVector  _epsilon; ///< \ru Вектор N-мерной точности. \en N-dimensional accuracy vector.

public :
  /// \ru Двумерная точность по умолчанию - ( PARAM_EPSILON, PARAM_EPSILON ) \en Default two-dimensional accuracy - ( PARAM_EPSILON, PARAM_EPSILON )
  static const MbDimAccuracy twoDimAcc;
  /// \ru Двумерная точность по умолчанию - ( METRIC_REGION, METRIC_REGION ) \en Default two-dimensional accuracy - ( METRIC_REGION, METRIC_REGION )
  static const MbDimAccuracy twoDimRgn;
  /// \ru Трехмерная точность по умолчанию - ( METRIC_EPSILON, METRIC_EPSILON, METRIC_EPSILON ) \en Default three-dimensional accuracy - ( METRIC_EPSILON, METRIC_EPSILON, METRIC_EPSILON )
  static const MbDimAccuracy threeDimAcc;


public :
  /** \brief \ru Конструктор для двумерной точности по двум значениям.
             \en Constructor for two-dimensional accuracy on the three values. \~
    \details \ru Конструктор для двумерной точности по двум значениям.
      Если передаваемые компоненты точности меньше, чем 'DOUBLE_EPSILON',
      то этим значениям присваивается 'DOUBLE_EPSILON'.
             \en Constructor for two-dimensional accuracy on two values.
      If the passed accuracy components are smaller than the 'DOUBLE_EPSILON', 
      then the value of 'DOUBLE_EPSILON' is assigned to these components.
    \param[in] xEps - \ru Точность по оси x.
                      \en x-axis accuracy. \~
    \param[in] yEps - \ru Точность по оси y.
                      \en y-axis accuracy. \~
  */
  MbDimAccuracy( double xEps, double yEps );

  /** \brief \ru Конструктор для трехмерной точности по трем значениям.
             \en Constructor for two-dimensional accuracy on the three values. \~
    \details \ru Конструктор для трехмерной точности по трем значениям.
      Если передаваемые компоненты точности меньше, чем 'DOUBLE_EPSILON',
      то этим значениям присваивается 'DOUBLE_EPSILON'.
             \en Constructor for three-dimensional accuracy on three values.
      If the passed accuracy components are smaller than the 'DOUBLE_EPSILON', 
      then the value of 'DOUBLE_EPSILON' is assigned to these components.
    \param[in] xEps - \ru Точность по оси x.
                      \en x-axis accuracy. \~
    \param[in] yEps - \ru Точность по оси y.
                      \en y-axis accuracy. \~
    \param[in] zEps - \ru Точность по оси z.
                      \en z-axis accuracy. \~
  */
  MbDimAccuracy( double xEps, double yEps, double zEps );

  /** \brief \ru Конструктор N-мерной точности по вектору значений. 
             \en Constructor of N-dimensional accuracy on a vector of values. \~
    \details \ru Конструктор N-мерной точности по вектору значений.
      Если вектор пустой, то создаётся одномерная точность c компонентой равной 'DOUBLE_EPSILON'.
             \en Constructor of N-dimensional accuracy on a vector of values.
      If the vector is empty, a one-dimensional accuracy with component equal to 'DOUBLE_EPSILON' is returned. \~
    \param[in] vector - \ru Вектор с компонентами N-мерной точности
                        \en Vector with components of N-dimensional accuracy. \~
  */
  template <class Vector>
  MbDimAccuracy( const Vector & vector );

  /// \ru Конструктор копирования. \en The copy constructor. 
  explicit MbDimAccuracy( const MbDimAccuracy & acc ) : _epsilon( acc.GetEpsilon() ) {}

  /// \ru Конструктор по умолчанию удалён. \en Default constructor explicitly deleted.
  MbDimAccuracy() = delete;

  /// \ru Деструктор. \en Destructor.
  ~MbDimAccuracy() {}

public: // Общие методы
  size_t GetDimension() const { return _epsilon.size(); } ///< \ru Дать размерность точности. \en Get the accuracy dimension. 

  /// \ru Дать вектор с компонентами точности. \en Get a vector with accuracy components.
  const c3d::DoubleVector & GetEpsilon() const { return _epsilon; }

  /// \ru Вернуть значение минимальной компоненты точности. \en  Return the value of the minimum accuracy component.
  double GetMin() const;

  /// \ru Длина вектора, построенного на компонентах точности. \en Length of the vector built on accuracy components.
  double GetLength() const;

  /// \ru Доступ к компоненте точности по индексу. \en Access to accuracy component by index. 
  double & operator [] ( size_t i );
  /// \ru Значение компоненты точности по индексу. \en The value of accuracy component by index. 
  double   operator [] ( size_t i ) const;

  /// \ru Присвоить точности значение другой точности. \en Assign the value of another accuracy to the accuracy. 
  MbDimAccuracy & operator = ( const MbDimAccuracy & );
};

#endif // __MB_DIM_ACCURACY_H
