////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Строитель усечения каркаса.
         \en Trimmed wire frame  creator.
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_TRIMMED_WIREFRAME_H
#define __CR_TRIMMED_WIREFRAME_H


#include <creator.h>
#include <templ_dptr.h>


class  MbTrimmedWireFrameParams;
class MbTrimmedWireFrameInfo;


//------------------------------------------------------------------------------
/** \brief \ru Строитель усечения каркаса.
           \en Wire frame trimmed creator. \~
  \details \ru Строитель усечения каркаса.\n
           \en Wire frame trimmed creator.\n \~
  \warning \ru В разработке.
           \en Under development. \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbTrimmedWireFrameCreator : public MbCreator {
private:
  DPtr<MbTrimmedWireFrameInfo> _trimmedWireFrameInfo; ///< \ru Параметры усечения каркаса. \en Parameters of trimmed the wire frame.
  MbPrecision                  _precision;            ///< \ru Точность построения объекта. \en The precision of object construction.

public:
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  MbTrimmedWireFrameCreator( const MbTrimmedWireFrameParams & params );
  /// \ru Деструктор. \en Destructor.
  virtual ~MbTrimmedWireFrameCreator();

protected:
  /// \ru Конструктор копирования. \en Copy-constructor.
  MbTrimmedWireFrameCreator( const MbTrimmedWireFrameCreator & other, 
                                   MbRegDuplicate *            iReg );

public:
  // \ru Общие функции строителя. \en The common functions of the creator.
  MbeCreatorType IsA()                                       const override; // \ru Тип элемента \en A type of element 
  MbCreator &    Duplicate( MbRegDuplicate * iReg = nullptr) const override; // \ru Сделать копию \en Create a copy

  bool           IsSame   ( const MbCreator &, double accuracy) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool           IsSimilar( const MbCreator & )                 const override; // \ru Являются ли объекты подобными \en Whether the objects are similar 
  bool           SetEqual ( const MbCreator & )                       override; // \ru Сделать равным \en Make equal

  void           Transform( const MbMatrix3D &, MbRegTransform * = nullptr )             override; // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void           Move     ( const MbVector3D &, MbRegTransform * = nullptr )             override; // \ru Сдвиг \en Translation
  void           Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate about an axis

  MbePrompt      GetPropertyName()                          override; // \ru Дать имя свойства объекта \en Get the object property name  
  void           GetProperties  ( MbProperties & )          override; // \ru Выдать свойства объекта \en Get properties of the object 
  void           SetProperties  ( const MbProperties & )    override; // \ru Записать свойства объекта \en Set properties of the object

  // \ru Построить кривую по журналу построения \en Create a curve from the history tree 
  bool           CreateWireFrame( MbWireFrame *&, MbeCopyMode, RPArray<MbSpaceItem> * items = nullptr ) override;

  const MbPrecision & GetPrecision() const override { return _precision; } // \ru Выдать точность построения. \en Get the precision of object construction.
private:
  MbTrimmedWireFrameCreator(); // \ru Не реализовано \en Not implemented

OBVIOUS_PRIVATE_COPY( MbTrimmedWireFrameCreator )
DECLARE_PERSISTENT_CLASS_NEW_DEL( MbTrimmedWireFrameCreator )   
}; // MbTrimmedWireFrameCreator

IMPL_PERSISTENT_OPS( MbTrimmedWireFrameCreator )



#endif // __CR_TRIMMED_WIREFRAME_H
