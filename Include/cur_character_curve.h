////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Двумерная кривая, координатные функции которой заданы в символьном виде.
         \en Functionally defined two-dimensional curve. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CUR_CHARACTER_CURVE_H
#define __CUR_CHARACTER_CURVE_H

#include <curve.h>
#include <mb_placement.h>
#include <mb_matrix.h>
#include <mb_rect.h>
#include <vector>


class    MbFunction;


//------------------------------------------------------------------------------
/** \brief \ru Двумерная кривая, координатные функции которой заданы в символьном виде.
           \en Functionally defined two-dimensional curve. \~
  \details \ru Координатные функции кривой заданы в виде пользовательских функций общего параметра t. 
    Каждая координата кривой описана своей функцией в виде строкового выражения. 
    Параметр кривой, он же параметр координатных функций, изменяется на отрезке [tmin tmax]. \n
    Все пользовательские функции заданы в локальной системе координат position.
    Система координат может быть декартовой или полярной.
    Радиус-вектор кривой в методе PointOn(double&t,MbCartPoint3D&r) описывается векторной функцией: \n
    r(t) = position.origin + (position.axisX xFunction(t)) + (position.axisY yFunction(t)).
           \en Functions of coordinates of curve are given as custom functions of common parameter t. 
    Each coordinate of the curve is described by its own function in form of string expression. 
    Parameter of the curve, which is also the parameter of functions of coordinates, varies in range [tmin tmax]. \n
    All the custom functions are given in the local coordinate system position.
    The coordinate system can be Cartesian or polar.
    The radius-vector of the curve in method PointOn(double&t,MbCartPoint3D&r) is described by a vector function: \n
    r(t) = position.origin + (position.axisX xFunction(t)) + (position.axisY yFunction(t)). \~
  \ingroup Curves_2D
*/
// ---
class MATH_CLASS MbCharacterCurve : public MbCurve {
private:
  MbFunction *        xFunction; ///< \ru Функция координаты x. \en Function of x-coordinate. 
  MbFunction *        yFunction; ///< \ru Функция координаты y. \en Function of y-coordinate. 
  MbPlacement         position;  ///< \ru Локальная система координат, в которой заданы координатные функции. \en The local coordinate system the functions of coordinates are specified in. 
  MbMatrix            transform; ///< \ru Матрица трансформации кривой. \en Transformation matrix of the curve. 
  double              tmin;      ///< \ru Минимальное  значение параметра кривой. \en The minimal value of a curve parameter. 
  double              tmax;      ///< \ru Максимальное значение параметра кривой. \en The maximal value of the curve parameter. 
  bool                closed;    ///< \ru Признак замкнутости кривой. \en An attribute of curve closedness. 
  MbeLocalSystemType  coordinateType; ///< \ru Тип системы координат, в которой заданы координатные функции \en Type of coordinate system the functions of coordinates are specified in  
  c3d::DoubleVector   specialParams;  ///< \ru Перечень параметров особых точек кривой \en List of parameters of curve's singular points 

  // \ru Буферные данные для ускорения вычислений. \en Buffer data to speed up computations. 
  mutable atomic_double metricLength;
  mutable MbRect        rect;

public:
  MbCharacterCurve( MbFunction & x, MbFunction & y, 
                    MbeLocalSystemType cs,
                    const MbPlacement & place,
                    double tmin_, double tmax_ );
protected:
  MbCharacterCurve( const MbCharacterCurve & );
public:
  virtual ~MbCharacterCurve(); 

public:
  VISITING_CLASS( MbCharacterCurve ); 

  // \ru Общие функции геометрического объекта. \en Common functions of a geometric object.

  MbePlaneType  IsA()  const override; // \ru Тип элемента \en A type of element
  MbPlaneItem & Duplicate ( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента \en Create a copy of the element
  bool        IsSame   ( const MbPlaneItem & other, double accuracy = LENGTH_EPSILON ) const override;      // \ru Кривая есть копия этой кривой ? \en Is a curve a copy of this curve?
  bool        SetEqual ( const MbPlaneItem & ) override;            // \ru Сделать равным \en Make equal 
  void        Transform( const MbMatrix &, MbRegTransform * = nullptr, const MbSurface * newSurface = nullptr ) override; // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move     ( const MbVector &, MbRegTransform * = nullptr, const MbSurface * newSurface = nullptr ) override;               // \ru Сдвиг \en Translation
  void        Rotate   ( const MbCartPoint & pnt, const MbDirection & angle, MbRegTransform * = nullptr, const MbSurface * newSurface = nullptr ) override;   // \ru Повернуть вокруг оси \en Rotate around an axis
  void        AddYourGabaritTo(  MbRect & ) const override;          // \ru Добавь в прям-к свой габарит \en Add own bounding rectangle to the bounding rectangle
  void        PrepareIntegralData( const bool forced ) const override; // \ru Рассчитать временные (mutable) данные объекта. \en Calculate temporary (mutable) data of an object.
 
  // \ru Общие функции кривой. \en Common functions of the curve. 

  double      GetTMin () const override; // \ru Вернуть минимальное значение параметра \en Get the minimum value of parameter
  double      GetTMax () const override; // \ru Вернуть максимальное значение параметра \en Get the maximum value of parameter
  bool        IsClosed() const override; // \ru Проверка замкнутости кривой \en Check for curve closedness
  // \ru Функции для работы внутри области определения кривой. \en Functions for working inside of the curve domain. \~
  void        PointOn ( double & t, MbCartPoint & ) const override; // \ru Точка на кривой \en Point on the curve
  void        FirstDer( double & t, MbVector & ) const override;    // \ru Первая производная \en The first derivative
  void        SecondDer( double & t, MbVector & ) const override;   // \ru Вторая производная \en The second derivative
  void        ThirdDer( double & t, MbVector & ) const override;    // \ru Третья производная по t \en The third derivative with respect to t
  // \ru Вычислить значения точки и производных для заданного параметра. \en Calculate point and derivatives of object for given parameter. \~
  void        Explore( double & t, bool ext,
                       MbCartPoint & pnt, MbVector & fir, MbVector * sec, MbVector * thir ) const override;

  double      Step            ( double t, double sag ) const override; // \ru Вычисление шага параметра по величине прогиба кривой \en Calculation of parameter step by value of sag of the curve
  double      DeviationStep   ( double t, double ang ) const override; // \ru Вычисление шага параметра по углу отклонения касательной \en Calculation of parameter by the angle of tangent deviation

  bool        HasLength       ( double & length ) const override;    // \ru Метрическая длина кривой \en Metric length of a curve

  double      GetMetricLength () const override;                     // \ru Метрическая длина кривой \en Metric length of a curve
  double      GetLengthEvaluation() const override;                  // \ru Оценка метрической длины кривой \en Metric length evaluation of a curve 
  void        CalculateGabarit( MbRect & ) const override;           // \ru Определить габаритный прямоугольник кривой.

  void        Inverse     ( MbRegTransform * iReg = nullptr ) override; // \ru Изменить направление \en Change direction

  size_t      GetCount    () const override; // \ru Определить количество разбиений для прохода в операциях. \en Define the number of splittings for one passage in operations.
  MbNurbs *   NurbsCurve  ( const MbCurveIntoNurbsInfo & ) const override;

  // \ru Построить усеченную кривую c учетом заданной двумерной точности. \en Construct a trimmed curve with the given two-dimensional accuracy.
  MbCurve *   Trimmed     ( double t1, double t2, int sense, const MbDimAccuracy & xyEps, bool saveParamLenAndLaw ) const override;
  using MbCurve::Trimmed; // \ru Метод базового класса MbCurve без учета двумерной точности. Точность по умолчанию - MbDimAccuracy::twoDimAcc.  \en Method of MbCurve base class without two-dimensional accuracy. The default accuracy is MbDimAccuracy::twoDimAcc.


  MbeState    DeletePart  ( double t1, double t2, MbCurve *& part2 ) override; // \ru Удалить часть кривой между параметрами t1 и t2 \en Remove a piece of curve between t1 and t2 parameters
  MbeState    TrimmPart   ( double t1, double t2, MbCurve *& part2 ) override; // \ru Оставить часть кривой между параметрами t1 и t2 \en Keep a piece of curve between t1 and t2 parameters
          
  void        GetBasisPoints( MbControlData & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.
  void        GetProperties  ( MbProperties & ) override; // \ru Выдать свойства объекта \en Get properties of the object
  void        SetProperties  ( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of the object

  void        CheckParam    ( double & t ) const;
  void        CalculateParam( double t, MbCartPoint & point,
                              MbVector & firstDer, MbVector & secondDer, MbVector & thirdDer ) const;

  const   MbFunction * GetX() const { return xFunction; }
  const   MbFunction * GetY() const { return yFunction; }
  const   MbMatrix &  GetMatrix() const { return transform; }
  const   MbPlacement & GetPlacement() const { return position;  }
  MbeLocalSystemType  GetCoordinateType() const { return coordinateType; }
  void        GetSpecialParams( std::vector<double> & params ) const;

protected:
  double      ApproximationStep( double t, bool isAngle, double sag ) const; 
  void        ConvertParamsInd( size_t componentIndex,
                                const std::vector<double> & tComponent, 
                                std::vector<double> & tCrv ) const;
  void        ConvertParams( const double tCrv, 
                             double (&tComponents)      [2],
                             double (&proportionFactors)[2]) const;

private:

  // \ru Проверить и установить признак замкнутости. \en Check and set the flag of closedness. 
  void        CheckClosed();
  // \ru Скорректировать нулевую первую производную в крайней точке. \en Correct the zero first derivative at the limit point.
  void CorrectZeroDerivative( double t, MbVector & fd ) const;


private:
  void        operator =    ( const MbCharacterCurve & );         // \ru Не реализовано. \en Not implemented. 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbCharacterCurve )
};

IMPL_PERSISTENT_OPS( MbCharacterCurve )


#endif // __CUR_CHARACTER_CURVE_H
