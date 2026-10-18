////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Строитель скругления каркаса.
         \en Wire frame fillets creator.
*/
////////////////////////////////////////////////////////////////////////////////
  
#ifndef __CR_FILLET_WIREFRAME_H
#define __CR_FILLET_WIREFRAME_H

#include <creator.h>

class  MbWireFrameFilletsParams;

//------------------------------------------------------------------------------
/** \brief \ru Строитель скругления каркаса.
           \en Wire frame fillets creator. \~
  \details \ru Строитель скругления каркаса.\n
           \en Wire frame fillets creator.\n \~
  \warning \ru В разработке.
           \en Under development. \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbFilletWireFrameCreator : public MbCreator {
private:
  MbeConnectingType _type;      ///< \ru Тип выполняемых скруглений. \en Fillet type( ordinary or on a surface ).
  c3d::DoubleVector _radiuses;  ///< \ru Множество радиусов скругления. \en An array of fillet radii.
  MbPrecision       _precision; ///< \ru  Точность построения объекта. \en The precision of object construction.
public:
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  MbFilletWireFrameCreator( const MbWireFrameFilletsParams & params );
  /// \ru Деструктор. \en Destructor.
  virtual ~MbFilletWireFrameCreator() {};

protected:
  /// \ru Конструктор копирования. \en Copy-constructor.
  MbFilletWireFrameCreator( const MbFilletWireFrameCreator & other, MbRegDuplicate * iReg );

private:
  MbFilletWireFrameCreator(); // \ru Не реализовано \en Not implemented 

public:
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

  // \ru Построить кривую по журналу построения. \en Create a curve from the history tree 
  bool        CreateWireFrame( MbWireFrame *&, MbeCopyMode, RPArray<MbSpaceItem> * items = nullptr ) override;
  
  const MbPrecision & GetPrecision() const override { return _precision; } // \ru Выдать точность построения. \en Get the precision of object construction.

OBVIOUS_PRIVATE_COPY( MbFilletWireFrameCreator )
DECLARE_PERSISTENT_CLASS_NEW_DEL( MbFilletWireFrameCreator )
}; // MbFilletWireFrameCreator

IMPL_PERSISTENT_OPS( MbFilletWireFrameCreator )



#endif // __CR_FILLET_WIREFRAME_H
