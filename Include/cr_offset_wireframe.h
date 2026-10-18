////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Строитель эквидистантного каркаса.
         \en Creator of an offset wire frame.
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_OFFSET_WIREFRAME_H
#define __CR_OFFSET_WIREFRAME_H


#include <creator.h>
#include <op_wireframe_parameter.h>


//------------------------------------------------------------------------------
/** \brief \ru Строитель эквидистантного каркаса.
           \en Creator of an offset wire frame. \~
  \details \ru Строитель эквидистантного каркаса.
           \en Creator of an offset wire frame. \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbOffsetWireFrameCreator : public MbCreator {
private:
  std::unique_ptr<MbOffsetWireFrameInfo> _offsetWireFrameInfo; ///< \ru Параметры для смещения пространственной кривой. \en Parameters for offsetting a spatial curve. \~
  c3d::CreatorsSPtrVector _creators;     ///< \ru Строители исходной оболочки. \en Shell initional creators. 
  c3d::ItemIndices        _facesIndices; ///< \ru Индексы граней, на которых строится эквидистанта. \en The faces indices on which to build the offset curve.
  MbPrecision             _precision;    ///< \ru Точность построения объекта. \en The precision of object construction.

public:
  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \details \ru Конструктор эквидистантного каркаса в пространстве.
             \en Constructor of an offset wire frame in the space. \~
    \param[in] params  - \ru Параметры.
                         \en Parameters. \~
  */
  MbOffsetWireFrameCreator( const MbOffsetWireFrameParams & params );

  /// \ru Деструктор. \en Destructor.
  virtual ~MbOffsetWireFrameCreator();

protected:
  /// \ru Конструктор копирования. \en Copy-constructor.
  MbOffsetWireFrameCreator( const MbOffsetWireFrameCreator & other,
                                  MbRegDuplicate *           iReg );
private:
  MbOffsetWireFrameCreator(); ///< \ru Не реализовано. \en Not implemented.

public:
  // \ru Общие функции строителя. \en The common functions of the creator.
  MbeCreatorType IsA()                                       const override; ///< \ru Тип элемента. \en A type of element.
  MbCreator &    Duplicate( MbRegDuplicate * iReg = nullptr) const override; ///< \ru Сделать копию. \en Create a copy.

  bool           IsSame   ( const MbCreator &, double accuracy) const override; ///< \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool           IsSimilar( const MbCreator & )                 const override; ///< \ru Являются ли объекты подобными. \en Whether the objects are similar.
  bool           SetEqual ( const MbCreator & )                       override; ///< \ru Сделать равным. \en Make equal.

  void           Transform( const MbMatrix3D &, MbRegTransform * = nullptr )             override; ///< \ru Преобразовать элемент согласно матрице. \en Transform element according to the matrix.
  void           Move     ( const MbVector3D &, MbRegTransform * = nullptr )             override; ///< \ru Сдвиг. \en Translation.
  void           Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; ///< \ru Повернуть вокруг оси. \en Rotate about an axis.

  MbePrompt      GetPropertyName()                                override; ///< \ru Дать имя свойства объекта. \en Get the object property name.
  void           GetProperties( MbProperties & )                  override; ///< \ru Выдать свойства объекта. \en Get properties of the object.
  void           SetProperties( const MbProperties & properties ) override; ///< \ru Записать свойства объекта. \en Set properties of the object. 

  bool           CreateWireFrame( MbWireFrame *&, MbeCopyMode, RPArray<MbSpaceItem> * items = nullptr ) override; ///< \ru Построить каркас по журналу построения \en Create an offset wire frame from the history tree.
  
  const MbPrecision & GetPrecision() const override { return _precision; } // \ru Выдать точность построения. \en Get the precision of object construction.

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbOffsetWireFrameCreator )
}; // MbOffsetWireFrameCreator


IMPL_PERSISTENT_OPS( MbOffsetWireFrameCreator )


#endif // __CR_OFFSET_WIREFRAME_H
