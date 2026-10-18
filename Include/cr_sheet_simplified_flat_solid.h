////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Построение упрощённой развёртки листового тела.
         \en Construction of the simplified flat pattern. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_SHEET_SIMPLIFIED_FLAT_SOLID_H
#define __CR_SHEET_SIMPLIFIED_FLAT_SOLID_H


#include <creator.h>
#include <sheet_metal_param.h>


//------------------------------------------------------------------------------
/** \brief \ru Строитель упрощения развёртки тела из листового материала.
           \en Constructor of the simplified flat pattern. \~
  \details \ru Строитель упрощения развёртки тела из листового материала.
    Возможно два вида упрощения: обработка углов и слияние подобных граней. \n
           \en Constructor of the simplified flat pattern.
    There are two types of simplification. The first one is the corners treatment. The second one is the similar faces unification. \n \~ 
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbSimplifyFlatSolid : public MbCreator {
  MbSimplifyFlatPatternValues parameters;

public :
  MbSimplifyFlatSolid( const MbSimplifyFlatPatternValues & params,
                       const MbSNameMaker &                names );
private:
  MbSimplifyFlatSolid( const MbSimplifyFlatSolid &, MbRegDuplicate * iReg );
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbSimplifyFlatSolid( const MbSimplifyFlatSolid & );

public:
  virtual ~MbSimplifyFlatSolid();

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
  MbSimplifyFlatSolid  & operator = ( const MbSimplifyFlatSolid & ); // \ru Не реализовано \en Not implemented 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbSimplifyFlatSolid )
}; // MbSimplifyFlatSolid

IMPL_PERSISTENT_OPS( MbSimplifyFlatSolid )


//------------------------------------------------------------------------------
/** \brief \ru Упростить развёртку листового тела.
           \en Simplify flattened sheet solid. \~
  \details \ru Упростить развёртку листового тела. \n
           \en Simplify flattened sheet solid. \n \~
  \param[in] solid - \ru Исходное тело.
                     \en The source solid. \~
  \param[in] sameShell - \ru Флаг удаления оболочки исходного тела.
                         \en Whether to delete the shell of the source solid. \~
  \param[in] params - \ru Параметры операции.
                      \en The operation parameters. \~
  \param[in] nameMaker - \ru Именователь.
                         \en An object for naming the new objects. \~
  \param[out] result - \ru Результирующее тело.
                       \en The resultant solid. \~
  \result \ru - Код результата операции.
          \en - The operation result code. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (MbCreator *) CreateSimplifiedFlatPattern(       MbFaceShell &                 initialShell,
                                                     const MbeCopyMode                   sameShell,
                                                     const MbSimplifyFlatPatternValues & params,
                                                     const MbSNameMaker &                nameMaker,
                                                           MbResultType &                res,
                                                           MbFaceShell *&                shell );


#endif // __CR_SHEET_SIMPLIFIED_FLAT_SOLID_H

