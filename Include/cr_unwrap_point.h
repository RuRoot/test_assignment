////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Строитель развернутой точки.
         \en Unwrapped point creator.
*/
////////////////////////////////////////////////////////////////////////////////
  
#ifndef __CR_UNWRAP_POINT_H
#define __CR_UNWRAP_POINT_H

#include <creator.h>
#include <op_curve_parameter.h>

//------------------------------------------------------------------------------
/** \brief \ru Строитель развернутой точки.
           \en Unwrapped point creator. \~
  \details \ru Строитель развернутой точки.
           \en Unwrapped point creator. \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbUnWrapPointCreator : public MbCreator {
private:
  MbCurvesWrappingParams _params; ///< \ru Параметры разворачивания. \en Parameters of the unwrapping.

protected:
  /// \ru Конструктор копирования. \en Copy-constructor.
  MbUnWrapPointCreator( const MbUnWrapPointCreator & other, MbRegDuplicate * iReg );

public:
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  MbUnWrapPointCreator( const MbCurvesWrappingParams & params );

public:
  virtual ~MbUnWrapPointCreator() {};

  // \ru Общие функции строителя. \en The common functions of the creator.
  MbeCreatorType  IsA()                                       const override; // \ru Тип элемента \en A type of element 
  MbCreator & Duplicate( MbRegDuplicate * iReg = nullptr )    const override; // \ru Сделать копию \en Create a copy

  bool        IsSame   ( const MbCreator &, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        IsSimilar( const MbCreator & )                  const override; // \ru Являются ли объекты подобными \en Whether the objects are similar 
  bool        SetEqual ( const MbCreator & )                        override; // \ru Сделать равным \en Make equal

  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override; // \ru Сдвиг \en Translation
  void        Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate about an axis

  MbePrompt   GetPropertyName()                          override; // \ru Дать имя свойства объекта \en Get the object property name  
  void        GetProperties  ( MbProperties & )          override; // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties  ( const MbProperties & )    override; // \ru Записать свойства объекта \en Set properties of the object 

  // \ru Построить каркас о журналу построения \en Create a wireframe from the history tree.
  bool        CreateWireFrame( MbWireFrame *&, MbeCopyMode, RPArray<MbSpaceItem> * items = nullptr ) override;
  
  const MbPrecision & GetPrecision() const override { return _params; } // \ru Выдать точность построения. \en Get the precision of object construction.

private:
  MbUnWrapPointCreator(); // \ru Не реализовано \en Not implemented
  
OBVIOUS_PRIVATE_COPY( MbUnWrapPointCreator )
DECLARE_PERSISTENT_CLASS_NEW_DEL( MbUnWrapPointCreator )
}; // MbUnWrapPointCreator

#endif // __CR_UNWRAP_POINT_H
