////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Построение боковых рёбер сгибов.
         \en Construction of side edges of bends. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_SHEET_RESTORED_EDGES_SOLID_H
#define __CR_SHEET_RESTORED_EDGES_SOLID_H


#include <creator.h>
#include <op_shell_parameter.h>


struct  MbSheetMetalBend;


//------------------------------------------------------------------------------
/** \brief \ru Строитель боковых рёбер сгибов.
           \en Constructor of side edges of bends. \~
  \details \ru Строитель оболочки c восстановлением боковых рёбер сгибов. \n
           \en Constructor of a shell with restored side edges of bends. \n \~ 
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbRestoredEdgesSolid : public MbCreator {
  SArray<MbItemIndex> outerFacesIndices; ///< \ru Идентификаторы внешних граней сгибов. \en Identifiers of external faces of bends. 
  bool                strict;            ///< \ru При false - восстанавить рёбра, где это возможно. \en If it equals false then restore edges where it is possible. 

public :
  MbRestoredEdgesSolid( const SArray<MbItemIndex> & outerFacesIndices,
                        const bool                  strict,
                        const MbSNameMaker &        nameMaker );
private:
  MbRestoredEdgesSolid( const MbRestoredEdgesSolid &, MbRegDuplicate * iReg );
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbRestoredEdgesSolid( const MbRestoredEdgesSolid & );

public:
  virtual ~MbRestoredEdgesSolid();

  // \ru Общие функции математического объекта \en Common functions of the mathematical object 

  MbeCreatorType  IsA()  const override; // \ru Тип элемента \en Type of element 
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию \en Create a copy

  bool        IsSame   ( const MbCreator & other, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        IsSimilar( const MbCreator & item ) const override; // \ru Являются ли объекты подобными? \en Determine whether an object is similar?
  bool        SetEqual ( const MbCreator & ) override; // \ru Сделать равным \en Make equal 

  void        Transform( const MbMatrix3D & matr, MbRegTransform * = nullptr ) override; // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move     ( const MbVector3D & to, MbRegTransform * = nullptr ) override; // \ru Сдвиг \en Translation
  void        Rotate   ( const MbAxis3D & axis, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate about an axis

  void        GetProperties( MbProperties & properties ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & properties ) override; // \ru Записать свойства объекта \en Set properties of the object 
  MbePrompt   GetPropertyName() override; // \ru Выдать заголовок свойства объекта \en Get a name of object property 

  // \ru Общие функции твердого тела \en Common functions of solid solid 
  
  bool        CreateShell( MbFaceShell *&         shell,
                           MbeCopyMode            sameShell, 
                           RPArray<MbSpaceItem> * items = nullptr ) override; // \ru Построение \en Construction
  
private:
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
  MbRestoredEdgesSolid  & operator = ( const MbRestoredEdgesSolid & ); 
  
  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbRestoredEdgesSolid )
}; // MbRestoredEdgesSolid

IMPL_PERSISTENT_OPS( MbRestoredEdgesSolid )


//------------------------------------------------------------------------------
/** \brief \ru Построить боковых рёбер сгибов.
           \en Construct side edges of bends. \~
  \details \ru Построить оболочку c восстановлением боковых рёбер сгибов. \n 
    Одновременно с построением оболочки функция создаёт её строитель.\n
           \en Construct a shell with restored side edges of bends. \n 
    The function simultaneously creates the shell and its constructor.\n \~ 
  \param[in]  initialShell - \ru Исходная оболочка.
                             \en The initial shell. \~
  \param[in]  sameShell  - \ru Режим копирования исходной оболочки.
                           \en Mode of copying the initial shell. \~
  \param[in]  outerFaces - \ru Грани сгибов.
                           \en Faces of bends. \~
  \param[in]  strict     - \ru Восстановить все рёбра.
                           \en Restore all edges. \~
  \param[in]  resultBends - \ru Имена построенных сгибов.
                            \en Constructed bends names. \~
  \param[in]  nameMaker  - \ru Именователи граней.
                           \en An object for naming faces. \~
  \param[out] res - \ru Код результата операции.
                    \en Operation result code. \~
  \param[out] shell - \ru Построенная оболочка.
                      \en The resultant shell. \~
  \result \ru Возвращает строитель оболочки.
          \en Returns the shell constructor. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (MbCreator *) RestoreSideEdges(       MbFaceShell &               initialShell,
                                          const MbeCopyMode                 sameShell,
                                          const RPArray<MbFace> &           outerFaces,
                                          const bool                        strict,
                                                RPArray<MbSheetMetalBend> & resultBends,
                                          const MbSNameMaker &              nameMaker,
                                                MbResultType &              res,
                                                MbFaceShell *&              shell );


#endif // __CR_SHEET_RESTORED_EDGES_SOLID_H
