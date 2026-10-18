////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Строитель кривой сопряжения двух кривых.
         \en Constructor of curve connecting two curves.
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_CONNECTING_CURVE_H
#define __CR_CONNECTING_CURVE_H


#include <creator.h>


class   MbCartPoint;
class   MbCurve3D;
class   MbSurface;
class   MbElementarySurface;
class   MbEdge;


//------------------------------------------------------------------------------
/** \brief \ru Строитель кривой сопряжения двух кривых.
           \en Constructor of curve connecting two curves. \~
  \details \ru Строитель кривой сопряжения двух кривых.\n
           \en Constructor of curve connecting two curves.\n \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbConnectingCurveCreator : public MbCreator {
private:
  MbCurve3D *   curve1;   ///< \ru Первая скругляемая кривая \en The first curve to connect  
  MbCurve3D *   curve2;   ///< \ru Вторая скругляемая кривая \en The second curve to connect  
  double        init1;    ///< \ru Исходное приближение параметра первой скругляемой кривой (для ft_Fillet и ft_OnSurface) \en The initial approximation of parameter of the first curve to be connected (for ft_Fillet and ft_OnSurface) 
  double        init2;    ///< \ru Исходное приближение параметра второй скругляемой кривой (для ft_Fillet и ft_OnSurface) \en The initial approximation of parameter of the second curve to be connected (for ft_Fillet and ft_OnSurface) 
  double        param1;   ///< \ru Параметр точки стыковки первой скругляемой кривой (кроме ft_Double) \en Connection point parameter of the first curve (except ft_Double) 
  double        param2;   ///< \ru Параметр точки стыковки второй скругляемой кривой (кроме ft_Double) \en Connection point parameter of the second curve (except ft_Double) 
  double        radius1;  ///< \ru Исходное приближение радиуса (кроме ft_Bridge, для ft_Double - радиус скругления первого участка, для ft_Spline - tension) \en The initial approximation of radius (except ft_Bridge, for ft_Double - the first segment fillet radius, for ft_Spline - tension) 
  double        radius2;  ///< \ru Результат расчета радиуса    (кроме ft_Bridge, для ft_Double - радиус скругления второго участка, для ft_Spline - tension) \en The radius calculation result (except ft_Bridge, for ft_Double - the second segment fillet radius, for ft_Spline - tension) 
  bool          sense1;   ///< \ru Совпадение направления кривой скругления и первой кривой (кроме ft_Spline, для ft_Double - начало/конец кривой) \en Coincidence of the connecting curve direction and the first curve (except ft_Spline, for ft_Double - start/end point of the curve) 
  bool          sense2;   ///< \ru Совпадение направления кривой скругления и второй кривой (кроме ft_Spline, для ft_Double - начало/конец кривой) \en Coincidence of the connecting curve direction and the second curve (except ft_Spline, for ft_Double - start/end point of the curve) 
  MbeMatingType mating1;  ///< \ru Тип сопряжения с первой кривой  (для ft_Spline) \en Type of mating with the first curve (for ft_Spline) 
  MbeMatingType mating2;  ///< \ru Тип сопряжения со второй кривой (для ft_Spline) \en Type of mating with the second curve (for ft_Spline) 
  MbeConnectingType type; ///< \ru Тип скругления (обычное или на поверхности) \en Connection type (ordinary or on a surface) 

protected:
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbConnectingCurveCreator( const MbConnectingCurveCreator & , MbRegDuplicate * iReg );
private:
  MbConnectingCurveCreator( const MbConnectingCurveCreator & ); // \ru Не реализовано \en Not implemented 
  MbConnectingCurveCreator(); // \ru Не реализовано \en Not implemented 

public:
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  MbConnectingCurveCreator( const MbSNameMaker & n, 
                            const MbCurve3D & c1, double t1, double p1, double r1, bool s1, MbeMatingType m1,
                            const MbCurve3D & c2, double t2, double p2, double r2, bool s2, MbeMatingType m2, MbeConnectingType t ); 

public :
  virtual ~MbConnectingCurveCreator();

  // \ru Общие функции строителя \en The common functions of the creator 
  MbeCreatorType IsA() const override; // \ru Тип элемента \en A type of element
  MbCreator & Duplicate( MbRegDuplicate * iReg = nullptr ) const override; // \ru Сделать копию \en Create a copy
 
  bool        IsSame   ( const MbCreator &, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        IsSimilar( const MbCreator & ) const override;            // \ru Являются ли объекты подобными \en Whether the objects are similar 
  bool        SetEqual ( const MbCreator & ) override;                  // \ru Сделать равным \en Make equal 

  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override; // \ru Сдвиг \en Translation
  void        Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate about an axis

  MbePrompt   GetPropertyName() override; // \ru Дать имя свойства объекта \en Get the object property name  
  void        GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of the object 
  void        GetBasisItems ( RPArray<MbSpaceItem> & ) override;  // \ru Дать базовые объекты \en Get the basis objects 
  void        GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  // \ru Построить кривую по журналу построения \en Create a curve from the history tree 
  bool        CreateWireFrame( MbWireFrame *&, MbeCopyMode, RPArray<MbSpaceItem> * items = nullptr ) override;

  /** \} */

private:
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation, to prevent an assignment by default. 
  void        operator = ( const MbConnectingCurveCreator & ); 

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbConnectingCurveCreator )
};

IMPL_PERSISTENT_OPS( MbConnectingCurveCreator )


//------------------------------------------------------------------------------
/** \brief \ru Создание строителя скругления двух кривых.
           \en Create two curves fillet constructor. \~
  \details \ru Создание строителя скругления двух кривых.\n
           \en Create two curves fillet constructor.\n \~
  \param[in] curve1 - \ru Кривая 1.
                      \en Curve 1. \~
  \param[in,out] t1 - \ru Параметр точки на кривой 1 соединения с кривой соединения.
                      \en A point parameter on curve 1 of connection with fillet curve. \~
  \param[in] curve2 - \ru Кривая 2.
                      \en Curve 2. \~
  \param[in,out] t2 - \ru Параметр точки на кривой 2 соединения с кривой соединения.
                      \en A point parameter on curve 2 of connection with fillet curve. \~
  \param[in,out] radius - \ru Радиус дуги или цилиндра.
                          \en The radius of an arc or a cylinder. \~
  \param[in] type   - \ru Тип скругления.
                      \en The fillet type. \~
  \param[in] names  - \ru Именователь построенного ребра.
                      \en An object defining the edges names. \~
  \param[out] res -   \ru Код результата операции.
                      \en Operation result code. \~
  \param[out] surface - \ru Поверхность, которая будет создана и на которой базируется соединительная кривая, (может быть возращён nullptr).
                        \en A surface on which the fillet curve is based on, it will be created by the method (can be nullptr). \~
  \param[out] edge  - \ru Построенная кривая сопряжения.
                      \en The constructed conjugation edge. \~
  \result \ru Возвращает строитель.
          \en Returns the constructor. \~
  \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC (MbCreator *) CreateFilletEdge( const MbCurve3D & curve1, double & t1, 
                                          const MbCurve3D & curve2, double & t2, 
                                          double & radius, bool sense, 
                                          MbeConnectingType type, 
                                          const MbSNameMaker & names,  
                                          MbResultType & res, 
                                          bool & unchanged,               // \ru Для ft_Fillet и ft_OnSurface \en For ft_Fillet and ft_OnSurface 
                                          MbElementarySurface *& surface, // \ru Для ft_Fillet и ft_OnSurface \en For ft_Fillet and ft_OnSurface 
                                          MbEdge *& edge ); 


//------------------------------------------------------------------------------
/** \brief \ru Создание строителя сопряжения двух кривых сплайном.
           \en Create constructor of two curves connection by a spline. \~
  \details \ru Создание строителя сопряжения двух кривых сплайном.\n
           \en Create constructor of two curves connection by a spline.\n \~
  \param[in] curve1 - \ru Кривая 1.
                      \en Curve 1. \~
  \param[in] curve2 - \ru Кривая 2.
                      \en Curve 2. \~
  \param[out] edge  - \ru Построенная кривая сопряжения.
                      \en The constructed conjugation edge. \~
  \result \ru Возвращает строитель.
          \en Returns the constructor. \~
  \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC (MbCreator *) CreateSplineEdge( const MbCurve3D & curve1, double t1, MbeMatingType mating1,
                                          const MbCurve3D & curve2, double t2, MbeMatingType mating2,
                                          double tension1, double tension2,
                                          const MbSNameMaker & names,  
                                          MbResultType & res, 
                                          MbEdge *& edge ); 


//------------------------------------------------------------------------------
/** \brief \ru Создание строителя сопряжения концов двух кривых составной кривой плавного соединения.
           \en Create a constructor of conjugation of two curves end points by a composite curve of smooth connection. \~
  \details \ru Создание строителя сопряжения концов двух кривых составной кривой плавного соединения.\n
           \en Create a constructor of conjugation of two curves end points by a composite curve of smooth connection.\n \~
  \param[in] curve1 - \ru Кривая 1.
                      \en Curve 1. \~
  \param[in] curve2 - \ru Кривая 2.
                      \en Curve 2. \~
  \param[out] edge  - \ru Построенная кривая сопряжения.
                      \en The constructed conjugation edge. \~
  \result \ru Возвращает строитель.
          \en Returns the constructor. \~
  \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC (MbCreator *) CreateConnectingEdge( const MbCurve3D & curve1, bool isBegin1, double radius1, 
                                              const MbCurve3D & curve2, bool isBegin2, double radius2, 
                                              const MbSNameMaker & names,  
                                              MbResultType & res, 
                                              MbEdge *& edge ); 


//------------------------------------------------------------------------------
/** \brief \ru Создание строителя сопряжения двух кривых кубическим сплайном Эрмита (кривой-мостиком).
           \en Create a constructor of two curves conjugation by a cubic Hermite spline (transition curve). \~
  \details \ru Создание строителя сопряжения двух кривых кубическим сплайном Эрмита (кривой-мостиком).\n
           \en Create a constructor of two curves conjugation by a cubic Hermite spline (transition curve).\n \~
  \param[in] curve1 - \ru Кривая 1.
                      \en Curve 1. \~
  \param[in] curve2 - \ru Кривая 2.
                      \en Curve 2. \~
  \param[out] edge  - \ru Построенная кривая сопряжения.
                      \en The constructed conjugation edge. \~
  \result \ru Возвращает строитель.
          \en Returns the constructor. \~
  \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC (MbCreator *) CreateBridgeEdge( const MbCurve3D & curve1, double t1, bool sense1, 
                                          const MbCurve3D & curve2, double t2, bool sense2, 
                                          const MbSNameMaker & names,  
                                          MbResultType & res, 
                                          MbEdge *& edge );


#endif  // __CR_CONNECTING_CURVE_H
