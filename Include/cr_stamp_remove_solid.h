////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Построение оболочки тела с без указанной операции.
         \en Construction of a shell without the specified operation. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_STAMP_REMOVE_SOLID_H
#define __CR_STAMP_REMOVE_SOLID_H


#include <creator.h>
#include <sheet_metal_param.h>
//#include <surf_plane.h>


class  MbRemoveOperationResultParams;


//------------------------------------------------------------------------------
/** \brief \ru Строитель оболочки из листового материала с удалёнными элементами указанной операции.
           \en The constructor of a shell from sheet material without elements of the specified operation. \~
  \details \ru Строитель оболочки из листового материала с удалёнными элементами указанной операции.
    Удаляет грани с указанным главным именем операции и затягивает образовавшуюся дыру расширением соседних граней. \n
           \en The constructor of a shell from sheet material without elements of the specified operation.
    It removes faces with specified main name and then mends the hole by stretching the neighbour faces. \n \~ 
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbRemoveOperationSolid : public MbCreator {
  SimpleName            removeName;
  MbeSheetOperationName operationType;
  
public :
  /// \ru Конструктор по параметрам операции. \en Constructor by operation parameters.
  MbRemoveOperationSolid( const MbRemoveOperationResultParams & params );

private:
  MbRemoveOperationSolid( const MbRemoveOperationSolid &, MbRegDuplicate * iReg );
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbRemoveOperationSolid( const MbRemoveOperationSolid & );

public:
  virtual ~MbRemoveOperationSolid();

  // \ru Общие функции математического объекта \en Common functions of the mathematical object 

  MbeCreatorType  IsA()  const override; // \ru Тип элемента \en Type of element 
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию \en Create a copy

  bool        IsSame   ( const MbCreator & other, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        IsSimilar( const MbCreator & item ) const override; // \ru Являются ли объекты подобными? \en Determine whether an object is similar?
  bool        SetEqual ( const MbCreator & ) override; // \ru Сделать равным \en Make equal

  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override; // \ru Сдвиг \en Translation
  void        Rotate   ( const MbAxis3D &, double, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate about an axis

  void        GetProperties( MbProperties & properties ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & properties ) override; // \ru Записать свойства объекта \en Set properties of the object 
  MbePrompt   GetPropertyName() override; // \ru Выдать заголовок свойства объекта \en Get a name of object property 

  // \ru Общие функции твердого тела \en Common functions of solid solid 

  bool        CreateShell( MbFaceShell *&         shell,
                           MbeCopyMode            sameShell, 
                           RPArray<MbSpaceItem> * items = nullptr ) override; // \ru Построение \en Construction

private:
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
  MbRemoveOperationSolid & operator = ( const MbRemoveOperationSolid & ); // \ru Не реализовано \en Not implemented 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbRemoveOperationSolid )
}; // MbRemoveOperationSolid

IMPL_PERSISTENT_OPS( MbRemoveOperationSolid )


//------------------------------------------------------------------------------
/** \brief \ru Построить оболочку без указанной операции.
           \en Constructs a shell without the specified operation. \~
  \details \ru Построить оболочку без указанной операции.
    Удаляет грани с указанным главным именем операции и затягивает образовавшуюся дыру расширением соседних граней. \n 
           \en Constructs a shell without the specified operation.
    It removes faces with specified main name and then mends the hole by stretching the neighbour faces. \n \~ 
  \param[in]  initialShell - \ru Исходная оболочка.
                             \en The initial shell. \~
  \param[in]  sameShell  - \ru Режим копирования исходной оболочки.
                           \en Mode of copying the initial shell. \~
  \param[in]  params - \ru Параметры операции.
                       \en Operation parameters. \~
  \param[out] res - \ru Код результата операции.
                    \en Operation result code. \~
  \param[out] shell - \ru Построенная оболочка.
                      \en The resultant shell. \~
  \result \ru Возвращает строитель оболочки.
          \en Returns the shell constructor. \~
  \ingroup Model_Creators
*/
// ---
MATH_FUNC (MbCreator *) CreateRemovedOperationResult (      MbFaceShell &                   initialShell,
                                                      const MbeCopyMode                     sameShell,
                                                      const MbRemoveOperationResultParams & params,
                                                            MbResultType &                  res,
                                                            SPtr<MbFaceShell> &             shell );


#endif // __CR_STAMP_REMOVE_SOLID_H


