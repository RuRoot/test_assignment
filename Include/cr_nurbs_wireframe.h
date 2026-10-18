////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Строитель NURBS копии каркаса.
         \en Creator NURBS copies of the wire frame.
*/
////////////////////////////////////////////////////////////////////////////////
  
#ifndef __CR_NURBS_WIREFRAME_H
#define __CR_NURBS_WIREFRAME_H

#include <creator.h>
#include <wire_frame.h>
#include <op_wireframe_parameter.h>


//------------------------------------------------------------------------------
/** \brief \ru Строитель NURBS копии каркаса.
           \en Creator NURBS copies of the wire frame. \~
  \details \ru Строитель NURBS копии каркаса.
           \en Creator NURBS copies of the wire frame.\n \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbNurbsWireFrameCreator : public MbCreator {
private:
  MbNurbsWireFrameInfo _frameIntoNurbsInfo; ///< \ru Параметры построения NURBS копии каркаса. \en Parameters of the NURBS copies of the wire frame..

protected:
  /// \ru Конструктор копирования. \en Copy-constructor.
  MbNurbsWireFrameCreator( const MbNurbsWireFrameCreator & other, MbRegDuplicate * iReg );

public:
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  MbNurbsWireFrameCreator( const MbNurbsWireFrameParams & params );

public:
  virtual ~MbNurbsWireFrameCreator() {};

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

  // \ru Построить кривую по журналу построения \en Create a curve from the history tree 
  bool        CreateWireFrame( MbWireFrame *&, MbeCopyMode, RPArray<MbSpaceItem> * items = nullptr ) override;

private:
  MbNurbsWireFrameCreator(); // \ru Не реализовано \en Not implemented
  
OBVIOUS_PRIVATE_COPY( MbNurbsWireFrameCreator )
DECLARE_PERSISTENT_CLASS_NEW_DEL( MbNurbsWireFrameCreator )
}; // MbNurbsFrameCreator

#endif // __CR_NURBS_WIREFRAME_H
