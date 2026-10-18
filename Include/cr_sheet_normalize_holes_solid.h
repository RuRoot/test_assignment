////////////////////////////////////////////////////////////////////////////////
/** 
\file
\brief \ru Нормализация отверстий листовых тел.
\en Normalization of the holes of sheet solid. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_SHEET_NORMALIZE_HOLES_SOLID_H
#define __CR_SHEET_NORMALIZE_HOLES_SOLID_H


#include <creator.h>


class MbNormalizeCutSidesParams;


//------------------------------------------------------------------------------
/** \brief \ru Строитель нормализации отверстий листовых тел.
\en Constructor of the normalization of the holes of sheet solid. \~
\details \ru Строитель нормализации отверстий листовых тел. \n
\en Constructor of the normalization of the holes of sheet solid.\n \~ 
\ingroup Model_Creators
*/
// ---
class MATH_CLASS MbNormalizeHolesSolid : public MbCreator {
  bool _cutOnly; // \ru Нормализовавать только вырез \en Normalize only hole.
public :
  MbNormalizeHolesSolid( const MbSNameMaker & names,
                               bool           isCut );
private:
  MbNormalizeHolesSolid( const MbNormalizeHolesSolid &, MbRegDuplicate * iReg );
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbNormalizeHolesSolid( const MbNormalizeHolesSolid & );

public:
  virtual ~MbNormalizeHolesSolid();

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
  MbNormalizeHolesSolid  & operator = ( const MbNormalizeHolesSolid & ); // \ru Не реализовано \en Not implemented 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbNormalizeHolesSolid )
}; // MbNormalizeHolesSolid

IMPL_PERSISTENT_OPS( MbNormalizeHolesSolid )


//------------------------------------------------------------------------------
/** \brief \ru Нормализовать вырезы листового тела.
           \en Normalize of the holes of sheet solid. \~
  \details \ru Нормализовать вырезы листового тела. \n
            \en Normalize of the holes of sheet solid. \n \~
  \param[in] initialShell - \ru Исходная оболочка.
                            \en The source shell. \~
  \param[in] sameShell - \ru Флаг удаления оболочки исходного тела.
                          \en Whether to delete the shell of the source solid. \~
  \param[in] normParam - \ru Набор параметров.
                         \en Parameters list. \~
  \param[out] shell - \ru Построенная оболочка.
                      \en The resultant shell. \~
  \result \ru Возвращает строитель оболочки.
          \en Returns the shell constructor. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (MbCreator *) NormalizeHolesSides (       c3d::ShellSPtr &              initialShell,
                                              const MbeCopyMode                   sameShell,
                                              const MbNormalizeCutSidesParams &   normParam,
                                                    MbResultType &                res,
                                                    c3d::ShellSPtr &              shell );


#endif // __CR_SHEET_NORMALIZE_HOLES_SOLID_H
