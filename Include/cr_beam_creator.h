////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Строитель балочной кривой.
         \en Constructor of beam curve.
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_BEAM_CREATOR_H
#define __CR_BEAM_CREATOR_H


#include <creator.h>
#include <op_shell_parameter.h>

//------------------------------------------------------------------------------
/** \brief \ru Строитель балочной кривой.
           \en Constructor of beam curve. \~
  \details \ru Строитель балочной кривой.
           \en Constructor of beam curve. \~
  \ingroup Model_Creators
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
class MATH_CLASS MbBeamCreator : public MbCreator
{
private:
  MbBeamElementParams     _params;  ///< \ru Параметры операции. \en Operation parameters.
  c3d::CreatorsSPtrVector _creators;  ///< \ru Журнал построения исходного тела  \en History tree of the source solid.
protected:
  /// \ru Конструктор копирования. \en Copy-constructor.
  MbBeamCreator( const MbBeamCreator &, MbRegDuplicate * iReg );
private:
  MbBeamCreator(); // \ru Конструктор по умолчанию. Не реализован. \en Default constructor. Not implemented.

public:
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  MbBeamCreator( const c3d::CreatorsSPtrVector & solidCreators, const MbBeamElementParams & beamParams );

  /// \ru Деструктор. \en Destructor.
  ~MbBeamCreator() override;

  // \ru Общие функции строителя. \en The common functions of the creator.
  MbeCreatorType IsA() const override { return ct_BeamCurveCreator; };   // \ru Тип элемента. \en A type of element.
  MbCreator &    Duplicate( MbRegDuplicate * iReg = nullptr ) const override; // \ru Сделать копию. \en Create a copy.

  bool           IsSame( const MbCreator &, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool           IsSimilar( const MbCreator & ) const override; // \ru Являются ли объекты подобными? \en Whether the objects are similar?
  bool           SetEqual( const MbCreator & ) override; // \ru Сделать равным. \en Make equal. 

  void           Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать элемент согласно матрице. \en Transform element according to the matrix.
  void           Move( const MbVector3D &, MbRegTransform * = nullptr ) override; // \ru Сдвиг. \en Translation.
  void           Rotate( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси. \en Rotate about the axis.

  MbePrompt      GetPropertyName() override; // \ru Дать имя свойства объекта. \en Get the object property name.
  void           GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта. \en Get properties of the object.
  void           SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта. \en Set properties of the object. 

  /** \} */

  bool           CreateWireFrame( MbWireFrame *&, MbeCopyMode, RPArray<MbSpaceItem> * ) override;  // \ru Построить кривую по журналу построения. \en Create a curve from the history tree. \~
  
  const MbPrecision & GetPrecision() const override { return _params; } // \ru Выдать точность построения. \en Get the precision of object construction.

OBVIOUS_PRIVATE_COPY( MbBeamCreator )
DECLARE_PERSISTENT_CLASS_NEW_DEL( MbBeamCreator )
};

IMPL_PERSISTENT_OPS( MbBeamCreator )

//------------------------------------------------------------------------------
/** \brief \ru Создание строителя балочной кривой.
           \en Create a constructor of beam curve. \~
  \details \ru Создание строителя балочной кривой.\n
           \en Create a constructor of beam curve.\n \~
  \param[in] sourceCurve  - \ru Исходное тело.
                            \en Source solid. \~
  \param[in] parameters   - \ru Входные параметры.
                            \en Input parameters. \~
  \param[in] names        - \ru Именователь построенного каркаса.
                            \en An object defining the wireframe names. \~
  \param[out] res         - \ru Код результата операции.
                            \en Operation result code. \~
  \param[out] result      - \ru Выходные параметры.
                            \en Output parameters. \~
  \result \ru Возвращает строитель.
          \en Returns the constructor. \~
  \ingroup Curve3D_Modeling
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
MATH_FUNC( c3d::CreatorSPtr ) CreateBeamCurves( const MbSolid &              sourceSolid,
                                                const MbBeamElementParams &  parameters,
                                                      MbResultType &         res,
                                                      MbBeamElementResults & result );

#endif // __CR_EXTENDING_CURVE_H