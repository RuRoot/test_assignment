////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Строитель плавной кривой.
         \en Constructor of fair curve.
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_FIRE_CURVE_H
#define __CR_FIRE_CURVE_H


#include <creator.h>
#include <curve3d.h>
#include <wire_frame.h>
#include <mb_data.h>
#include <op_curve_parameter.h>


//------------------------------------------------------------------------------
/** \brief \ru Результат построения строителя гладкой кривой.
           \en The result of creating or a fair curve creator. \~
  \details \ru Результат построения строителя гладкой кривой. \n
           \en The result of creating or a fair curve creator. \n \~
  \ingroup Data_Structures
*/
// ---
struct MATH_CLASS MbFairCreatorResult : public MbApproxError
{
  c3d::SpaceCurveSPtr _resCurve;
  MbResultType        _error;
  MbeFairWarning      _warning;
  MbFairCreatorResult() : MbApproxError(), _error( rt_Success ), _warning( fwarn_Success ) {}
};

//------------------------------------------------------------------------------
/** \brief \ru Базовый класс строителя плавной кривой.
           \en Base class of a constructor of fair curve. \~
  \details \ru Базовый класс строителя плавной кривой.
           \en Base class of a constructor of fair curve. \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbFireCreator : public MbCreator
{
protected:
  c3d::SpaceCurveSPtr _initCurve;   ///< \ru Исходная кривая. \en An initial curve.
  MbFairCurveMethod   _method;      ///< \ru Метод построения кривой. \en Method of a curve construction.

protected:
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbFireCreator( const MbFireCreator &, MbRegDuplicate * );
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  MbFireCreator( const MbCurve3D & curve, const MbFairCurveMethod & method,
                 const MbSNameMaker & nm );
private:
  MbFireCreator(); // \ru Не реализовано. \en Not implemented.

public:
  /// \ru Деструктор. \en Destructor.
  ~MbFireCreator() override;

  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать элемент согласно матрице. \en Transform element according to the matrix.
  void        Move( const MbVector3D &, MbRegTransform * = nullptr ) override; // \ru Сдвиг. \en Translation.
  void        Rotate( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси. \en Rotate about the axis.

  void        GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта. \en Get properties of the object.
  void        SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта. \en Set properties of the object. 

private:
  OBVIOUS_PRIVATE_COPY( MbFireCreator )

  DECLARE_PERSISTENT_CLASS( MbFireCreator )
};

IMPL_PERSISTENT_OPS( MbFireCreator )


//------------------------------------------------------------------------------
/** \brief \ru Строитель плавной кривой.
           \en Constructor of fair curve. \~
  \details \ru Строитель плавной кривой.
           \en Constructor of fair curve. \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbFireCurveCreator : public MbFireCreator
{
private:
  MbFairCreateData  _parameters;  ///< \ru Параметры построения кривой. \en The curve construction parameters. \~
protected:
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbFireCurveCreator( const MbFireCurveCreator &, MbRegDuplicate * iReg );
public:
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  MbFireCurveCreator( const MbCurve3D & curve, const MbFairCurveMethod & method,
                      const MbFairCreateData & params, const MbSNameMaker & nm );
private:
  MbFireCurveCreator(); // \ru Не реализовано. \en Not implemented.

  /// \ru Деструктор. \en Destructor.
  ~MbFireCurveCreator() override;

  // \ru Общие функции строителя. \en The common functions of the creator.
  MbeCreatorType IsA() const override { return ct_FairCurveCreator; };   // \ru Тип элемента. \en A type of element.
  MbCreator &    Duplicate( MbRegDuplicate * iReg = nullptr ) const override; // \ru Сделать копию. \en Create a copy.

  bool           IsSame( const MbCreator &, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool           IsSimilar( const MbCreator & ) const override; // \ru Являются ли объекты подобными? \en Whether the objects are similar?
  bool           SetEqual( const MbCreator & ) override; // \ru Сделать равным. \en Make equal. 

  MbePrompt      GetPropertyName() override; // \ru Дать имя свойства объекта. \en Get the object property name.
  void           GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта. \en Get properties of the object.
  void           SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта. \en Set properties of the object. 


  bool           CreateWireFrame( MbWireFrame *&, MbeCopyMode, RPArray<MbSpaceItem> * ) override;  // \ru Построить кривую по журналу построения. \en Create a curve from the history tree. \~
  bool           CreateWireFrame( c3d::WireFrameSPtr & result );  // \ru Построить кривую по журналу построения. \en Create a curve from the history tree. \~

private:
  OBVIOUS_PRIVATE_COPY( MbFireCurveCreator )

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbFireCurveCreator )
};

IMPL_PERSISTENT_OPS( MbFireCurveCreator )


//------------------------------------------------------------------------------
/** \brief \ru Строитель плавной кривой сопряжения. 
           \en Constructor of a fair fillet curve. \~
  \details \ru Строитель плавной кривой сопряжения. 
           \en Constructor of a fair fillet curve. \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbFireFilletCreator : public MbFireCreator
{
private:
  MbFairFilletData  _parameters;  ///< \ru Параметры построения кривой. \en The curve construction parameters. \~
protected:
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbFireFilletCreator( const MbFireFilletCreator &, MbRegDuplicate * iReg );
private:
  MbFireFilletCreator(); // \ru Не реализовано. \en Not implemented.
public:
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  MbFireFilletCreator( const MbCurve3D & curve, const MbFairCurveMethod & method,
                       const MbFairFilletData & params, const MbSNameMaker & nm );

  /// \ru Деструктор. \en Destructor.
  ~MbFireFilletCreator() override;

  // \ru Общие функции строителя. \en The common functions of the creator.
  MbeCreatorType IsA() const override { return ct_FairFilletCreator; };   // \ru Тип элемента. \en A type of element.
  MbCreator &    Duplicate( MbRegDuplicate * iReg = nullptr ) const override; // \ru Сделать копию. \en Create a copy.

  bool           IsSame( const MbCreator &, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool           IsSimilar( const MbCreator & ) const override; // \ru Являются ли объекты подобными? \en Whether the objects are similar?
  bool           SetEqual( const MbCreator & ) override; // \ru Сделать равным. \en Make equal. 

  MbePrompt      GetPropertyName() override; // \ru Дать имя свойства объекта. \en Get the object property name.
  void           GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта. \en Get properties of the object.
  void           SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта. \en Set properties of the object. 


  bool           CreateWireFrame( MbWireFrame *&, MbeCopyMode, RPArray<MbSpaceItem> * ) override;  // \ru Построить кривую по журналу построения. \en Create a curve from the history tree. \~
  bool           CreateWireFrame( c3d::WireFrameSPtr & result );  // \ru Построить кривую по журналу построения. \en Create a curve from the history tree. \~

private:
  OBVIOUS_PRIVATE_COPY( MbFireFilletCreator )

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbFireFilletCreator )
};

IMPL_PERSISTENT_OPS( MbFireFilletCreator )


//------------------------------------------------------------------------------
/** \brief \ru Строитель клотоиды. 
           \en Constructor of a clothoid. \~
  \details \ru Строитель клотоиды. 
           \en Constructor of a clothoid. \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbFireClothoidCreator : public MbCreator
{
private:
  MbClothoidParams  _parameters;  ///< \ru Параметры построения кривой. \en The curve construction parameters. \~
protected:
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbFireClothoidCreator( const MbFireClothoidCreator &, MbRegDuplicate * iReg );
private:
  MbFireClothoidCreator(); // \ru Не реализовано. \en Not implemented.
public:
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  MbFireClothoidCreator( const MbClothoidParams & params, const MbSNameMaker & nm );

  /// \ru Деструктор. \en Destructor.
  ~MbFireClothoidCreator() override;

  // \ru Общие функции строителя. \en The common functions of the creator.
  MbeCreatorType IsA() const override { return ct_FairClothoidCreator; };   // \ru Тип элемента. \en A type of element.
  MbCreator &    Duplicate( MbRegDuplicate * iReg = nullptr ) const override; // \ru Сделать копию. \en Create a copy.

  void           Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать элемент согласно матрице. \en Transform element according to the matrix.
  void           Move( const MbVector3D &, MbRegTransform * = nullptr ) override; // \ru Сдвиг. \en Translation.
  void           Rotate( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси. \en Rotate about the axis.
  
  bool           IsSame( const MbCreator &, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool           IsSimilar( const MbCreator & ) const override; // \ru Являются ли объекты подобными? \en Whether the objects are similar?
  bool           SetEqual( const MbCreator & ) override; // \ru Сделать равным. \en Make equal. 

  MbePrompt      GetPropertyName() override; // \ru Дать имя свойства объекта. \en Get the object property name.
  void           GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта. \en Get properties of the object.
  void           SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта. \en Set properties of the object. 


  bool           CreateWireFrame( MbWireFrame *&, MbeCopyMode, RPArray<MbSpaceItem> * ) override;  // \ru Построить кривую по журналу построения. \en Create a curve from the history tree. \~
  bool           CreateWireFrame( c3d::WireFrameSPtr & result );  // \ru Построить кривую по журналу построения. \en Create a curve from the history tree. \~

private:
  OBVIOUS_PRIVATE_COPY( MbFireClothoidCreator )

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbFireClothoidCreator )
};

IMPL_PERSISTENT_OPS( MbFireClothoidCreator )


//------------------------------------------------------------------------------
/** \brief \ru Строитель изменения плавной кривой.
           \en Constructor of changing a fair curve. \~
  \details \ru Строитель изменения плавной кривой.
           \en Constructor of fair curve. \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbFireChangeCreator : public MbFireCreator
{
private:
  MbFairChangeData     _parameters;  ///< \ru Параметры построения кривой. \en The curve construction parameters. \~
protected:
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbFireChangeCreator( const MbFireChangeCreator &, MbRegDuplicate * iReg );
private:
  MbFireChangeCreator(); // \ru Не реализовано. \en Not implemented.
public:
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  MbFireChangeCreator( const MbCurve3D & curve, const MbFairCurveMethod & method,
                       const MbFairChangeData & params, const MbSNameMaker & nm );

  /// \ru Деструктор. \en Destructor.
  ~MbFireChangeCreator() override;

  // \ru Общие функции строителя. \en The common functions of the creator.
  MbeCreatorType IsA() const override { return ct_FairChangeCreator; };   // \ru Тип элемента. \en A type of element.
  MbCreator &    Duplicate( MbRegDuplicate * iReg = nullptr ) const override; // \ru Сделать копию. \en Create a copy.

  bool           IsSame( const MbCreator &, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool           IsSimilar( const MbCreator & ) const override; // \ru Являются ли объекты подобными? \en Whether the objects are similar?
  bool           SetEqual( const MbCreator & ) override; // \ru Сделать равным. \en Make equal. 

  MbePrompt      GetPropertyName() override; // \ru Дать имя свойства объекта. \en Get the object property name.
  void           GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта. \en Get properties of the object.
  void           SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта. \en Set properties of the object. 

  bool           CreateWireFrame( MbWireFrame *&, MbeCopyMode, RPArray<MbSpaceItem> * ) override;  // \ru Построить кривую по журналу построения. \en Create a curve from the history tree. \~
  bool           CreateWireFrame( c3d::WireFrameSPtr & result );  // \ru Построить кривую по журналу построения. \en Create a curve from the history tree. \~

private:
  OBVIOUS_PRIVATE_COPY( MbFireChangeCreator )

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbFireChangeCreator )
};

IMPL_PERSISTENT_OPS( MbFireChangeCreator )


//------------------------------------------------------------------------------
/** \brief \ru Создание плавной кривой.
           \en Create a fair curve. \~
  \details \ru Создание плавной кривой.\n
           \en Create a fair curve.\n \~
  \param[in]  sourceCurve  - \ru Исходная кривая.
                            \en An initial curve. \~
  \param[in] method       - \ru Метод построения кривой.
                            \en Method of a curve construction. \~
  \param[in] parameters   - \ru Параметры построения.
                            \en Construction parameters. \~
  \param[in] snMaker      - \ru Именователь кривых.
                            \en An object defining curves names. \~
  \param[out] res         - \ru Код результата операции.
                            \en Operation result code. \~
  \param[out] errInfo     - \ru \ru Сведения об ошибке аппроксимации.
                            \en Approximation error information. \~
  \param[out] resCurve    - \ru Результат.
                            \en The result. \~
  \result \ru Возвращает строитель.
          \en Returns the constructor. \~
  \warning \ru В разработке.
           \en Under development. \~
 \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC( c3d::CreatorSPtr ) CreateFairCurve( const MbCurve3D & sourceCurve,
                                               const MbFairCurveMethod & method,
                                               const MbFairCreateData & parameters, 
                                               const MbSNameMaker & snMaker,
                                                     MbFairCreatorResult & result );


//------------------------------------------------------------------------------
/** \brief \ru Изменение плавной кривой.
           \en Changing a fair curve. \~
  \details \ru Изменение плавной кривой.\n
           \en Changing a fair curve.\n \~
  \param[in]  sourceCurve - \ru Исходная кривая.
                            \en An initial curve. \~
  \param[in] parameters   - \ru Параметры изменения.
                            \en Changing parameters. \~
  \param[in] method       - \ru Метод изменения кривой.
                            \en Method of a curve changing. \~
  \param[in] snMaker      - \ru Именователь кривых.
                            \en An object defining curves names. \~
  \param[out] res         - \ru Код результата операции.
                            \en Operation result code. \~
  \param[out] resCurve    - \ru Результат.
                            \en The result. \~
  \result \ru Возвращает строитель.
          \en Returns the constructor. \~
  \warning \ru В разработке.
           \en Under development. \~
  \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC( c3d::CreatorSPtr ) CreateFairCurve( const MbCurve3D & sourceCurve,
                                               const MbFairCurveMethod & method,
                                               const MbFairChangeData & parameters, 
                                               const MbSNameMaker & snMaker,
                                                     MbFairCreatorResult & result );


//------------------------------------------------------------------------------
/** \brief \ru Создание плавной кривой сопряжения.
           \en Creating a fair fillet curve. \~
  \details \ru Создание плавной кривой сопряжения.\n
           \en Creating a fair fillet curve.\n \~
  \param[in]  sourceCurve - \ru Исходная кривая.
                            \en An initial curve. \~
  \param[in] parameters   - \ru Параметры построения.
                            \en Construction parameters. \~
  \param[in] snMaker      - \ru Именователь кривых.
                            \en An object defining curves names. \~
  \param[out] res         - \ru Код результата операции.
                            \en Operation result code. \~
  \param[out] resCurve    - \ru Результат.
                            \en The result. \~
  \result \ru Возвращает строитель.
          \en Returns the constructor. \~
  \warning \ru В разработке.
           \en Under development. \~
  \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC( c3d::CreatorSPtr ) CreateFairCurve( const MbCurve3D & sourceCurve,
                                               const MbFairFilletData & parameters, 
                                               const MbSNameMaker & snMaker,
                                                     MbFairCreatorResult & result );


//------------------------------------------------------------------------------
/** \brief \ru Создание клотоиды.
           \en Creating a clothoid. \~
  \details \ru Создание клотоиды.\n
           \en Creating a clothoid.\n \~
  \param[in] parameters   - \ru Параметры построения.
                            \en Creation parameters. \~
  \param[in] snMaker      - \ru Именователь кривых.
                            \en An object defining curves names. \~
  \param[out] res         - \ru Код результата операции.
                            \en Operation result code. \~
  \param[out] resCurve    - \ru Результат.
                            \en The result. \~
  \result \ru Возвращает строитель.
          \en Returns the constructor. \~
  \warning \ru В разработке.
           \en Under development. \~
  \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC( c3d::CreatorSPtr ) CreateFairCurve( const MbClothoidParams & parameters,
                                               const MbSNameMaker & snMaker,
                                                     MbFairCreatorResult & result );


#endif // __CR_FIRE_CURVE_H