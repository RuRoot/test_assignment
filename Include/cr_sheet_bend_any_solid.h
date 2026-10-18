////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Построение оболочки тела с выполнеными сгибами.
         \en Construction of a shell from any solid with bends. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_SHEET_BEND_ANY_SOLID_H
#define __CR_SHEET_BEND_ANY_SOLID_H


#include <creator.h>
#include <sheet_metal_param.h>
#include <surf_plane.h>


//------------------------------------------------------------------------------
/** \brief \ru Строитель оболочки из листового материала с выполненым сгибом/разгибом.
           \en Constructor of a shell from sheet material with bend/unbend. \~
  \details \ru Строитель оболочки из листового материала с выполненым сгибом/разгибом.
    Построение сгиба/разгиба на касательную плоскость к указанной грани в указанной
    точке с индивидуальными для каждого сгиба параметрами. \n
           \en Constructor of a shell from sheet material with bend/unbend.
    Construction of a bend/unbend to the tangent plane to the specified face at
    the given point with parameters individual for each bend. \n \~ 
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbBendAnySolid : public MbCreator {
  MbPlane           cutPlane;
  SArray<MbAnyBend> bends;

public :
  MbBendAnySolid( const MbPlane &           cutPlane,
                  const SArray<MbAnyBend> & bends,
                  const MbSNameMaker &      names );
private:
  MbBendAnySolid( const MbBendAnySolid &, MbRegDuplicate * iReg );
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbBendAnySolid( const MbBendAnySolid & );

public:
  virtual ~MbBendAnySolid();

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
  MbBendAnySolid  & operator = ( const MbBendAnySolid & ); // \ru Не реализовано \en Not implemented 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbBendAnySolid )
}; // MbBendAnySolid

IMPL_PERSISTENT_OPS( MbBendAnySolid )


//------------------------------------------------------------------------------
/** \brief \ru Построить оболочку с выполнеными сгибами.
           \en Construct a shell with bends. \~
  \details \ru Построить оболочку любого тела с выполнеными сгибами.
    Построение сгиба/разгиба на касательную плоскость к указанной грани в указанной
    точке с индивидуальными для каждого сгиба параметрами. \n 
    Одновременно с построением оболочки функция создаёт её строитель.\n
           \en Construct a shell from sheet material with bend/unbend.
    Construction of a bend/unbend to the tangent plane to the specified face at
    the given point with parameters individual for each bend. \n 
    The function simultaneously creates the shell and its constructor.\n \~ 
  \param[in]  initialShell - \ru Исходная оболочка.
                             \en The initial shell. \~
  \param[in]  sameShell  - \ru Режим копирования исходной оболочки.
                           \en Mode of copying the initial shell. \~
  \param[in]  bends - \ru Сгибы оболочки.
                      \en Bends of a shell. \~
  \param[in]  fixedFace - \ru Неподвижная грань.
                          \en Fixed face. \~
  \param[in]  fixedPoint - \ru Неподвижная точка.
                           \en Fixed point. \~
  \param[in]  names - \ru Именователь граней.
                      \en An object for naming faces. \~
  \param[out] res - \ru Код результата операции.
                    \en Operation result code. \~
  \param[out] shell - \ru Построенная оболочка.
                      \en The resultant shell. \~
  \result \ru Возвращает строитель оболочки.
          \en Returns the shell constructor. \~
  \ingroup Model_Creators
*/
// ---
MATH_FUNC (MbCreator *) CreateAnyBend(       MbFaceShell &       initialShell,
                                       const MbeCopyMode         sameShell,
                                       const MbPlane &           cutPlane,
                                       const SArray<MbAnyBend> & bends,
                                       const MbSNameMaker &      names,
                                             MbResultType &      res,
                                             MbFaceShell *&      shell );



#endif // __CR_SHEET_BEND_ANY_SOLID_H

