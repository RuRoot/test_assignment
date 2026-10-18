////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Построение проволочного каркаса без истории.
         \en Construction of wireframe without history. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_WIREFRAME_CREATOR_H
#define __CR_WIREFRAME_CREATOR_H


#include <creator.h>


//------------------------------------------------------------------------------
/** \brief \ru Строитель проволочного каркаса без истории.
           \en Constructor of a wireframe without history. \~
  \details \ru Строитель проволочного каркаса без истории. \n
           \en Constructor of a wireframe without history. \n \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbSimpleWireFrameCreator : public MbCreator {
public:
  c3d::WireEdgesSPtrVector _edges; ///< \ru Множество ребер. \en A set of edges. 

public:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по данным проволочного каркаса.
             \en Constructor by wireframe data. \~
    \param[in] edges  - \ru Множество указателей на ребра.
                        \en A set of pointers to edges.
    \param[in] names  - \ru Именователь операции.
                        \en An object defining names generation in the operation. \~
    \param[in] same   - \ru Признак использования оригиналов данных.
                        \en Flag of using the original data. \~
  */
  MbSimpleWireFrameCreator( const c3d::ConstWireEdgesSPtrVector & edges,
                            const MbSNameMaker &                  names,
                                  bool                            same );

private:
  MbSimpleWireFrameCreator ( const MbSimpleWireFrameCreator &, 
                                   MbRegDuplicate *  iReg ); // \ru Конструктор копирования с регистратором \en Copy-constructor with the registrator 
public:
  /// \ru Деструктор. \en Destructor.
  virtual ~MbSimpleWireFrameCreator();
  /** \ru \name Общие функции строителя проволочного каркаса.
      \en \name Common functions of the wireframe creator.
      \{ */
  MbeCreatorType IsA() const override;                                   // \ru Тип элемента \en Type of element 
  MbCreator &    Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию \en Create a copy
  void           Transform( const MbMatrix3D &, 
                                  MbRegTransform * = nullptr ) override; // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void           Move( const MbVector3D &,
                             MbRegTransform * = nullptr ) override;      // \ru Сдвиг \en Translation
  void           Rotate( const MbAxis3D &, 
                               double           angle, 
                               MbRegTransform * = nullptr ) override;    // \ru Повернуть вокруг оси \en Rotate about an axis

  MbePrompt      GetPropertyName() override;                             // \ru Выдать заголовок свойства объекта \en Get a name of object property 
  void           GetProperties( MbProperties & ) override;               // \ru Выдать свойства объекта \en Get properties of the object 
  void           SetProperties( const MbProperties & ) override;         // \ru Записать свойства объекта \en Set properties of the object 
  void           GetBasisItems( RPArray<MbSpaceItem> & ) override;  // \ru Дать базовые объекты \en Get the basis objects 

  bool           IsSame( const MbCreator &, 
                               double accuracy ) const override;         // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool           IsSimilar( const MbCreator & ) const override;          // \ru Являются ли объекты подобными \en Whether the objects are similar 
  bool           SetEqual( const MbCreator & ) override;                 // \ru Сделать равным \en Make equal 

  /// \ru Построить проволочный каркас по исходным данным. \en Create a wire-frame from the source data.
  bool           CreateWireFrame( MbWireFrame *&, 
                                  MbeCopyMode, 
                                  RPArray<MbSpaceItem> * ) override;

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbSimpleWireFrameCreator )
  OBVIOUS_PRIVATE_COPY( MbSimpleWireFrameCreator )
};// MbSimpleWireFrameCreator

IMPL_PERSISTENT_OPS( MbSimpleWireFrameCreator )

#endif // __CR_WIREFRAME_CREATOR_H