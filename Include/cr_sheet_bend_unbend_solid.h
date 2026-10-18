////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Построение оболочки из листового материала с выполненым сгибом/разгибом.
         \en Construction of a shell from sheet material with bend/unbend. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_SHEET_BEND_UNBEND_SOLID_H
#define __CR_SHEET_BEND_UNBEND_SOLID_H


#include <creator.h>
#include <sheet_metal_param.h>


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
class MATH_CLASS MbBendUnbendSolid : public MbCreator {
  PArray<MbBendIndices> bendIndices;    ///< \ru Идентификаторы сгибаемых/разгибаемых граней и параметры сгибов. \en Identifiers of faces to bend/unbend and parameters of bends. 
  MbItemIndex           fixedFaceIndex; ///< \ru Идентификатор грани, на касательную к которой разгибаем. \en Identifier of the face on a tangent to which to unbend. 
  MbCartPoint           fixedPoint;     ///< \ru Точка в параметрической области фиксированной грани, определяющая касательную плоскость, на которую будет выполняться разгиб. \en A point in parametric domain of a fixed face determining the tangent plane on which to perform the bend. 
  bool                  bend;           ///< \ru Флаг, определяющий тип операции: сгиб или разгиб. \en Flag determining the operation type: bend or unbend 

public :
  MbBendUnbendSolid( const RPArray<MbBendIndices> & bendInd,
                     const MbItemIndex              fixedFaceIndex,
                     const MbCartPoint &            fixedPoint,
                     const bool                     bend,
                     const MbSNameMaker &           names );
private:
  MbBendUnbendSolid( const MbBendUnbendSolid &, MbRegDuplicate * iReg );
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbBendUnbendSolid( const MbBendUnbendSolid & );

public:
  virtual ~MbBendUnbendSolid();

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
  MbBendUnbendSolid  & operator = ( const MbBendUnbendSolid & ); // \ru Не реализовано \en Not implemented 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbBendUnbendSolid )
}; // MbBendUnbendSolid

IMPL_PERSISTENT_OPS( MbBendUnbendSolid )


//------------------------------------------------------------------------------
/** \brief \ru Построить оболочку с выполненым сгибом/разгибом.
           \en Construct a shell with bend/unbend. \~
  \details \ru Построить оболочку из листового материала с выполненым сгибом/разгибом.
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
  \param[in]  fixedFace - \ru Неподвихная грань.
                          \en Fixed face. \~
  \param[in]  fixedPoint - \ru Неподвихная точка.
                           \en Fixed point. \~
  \param[in]  names - \ru Именователь граней.
                      \en An object for naming faces. \~
  \param[out] res - \ru Код результата операции.
                    \en Operation result code. \~
  \param[out] shell - \ru Построенная оболочка.
                      \en The resultant shell. \~
  \param[out] ribContours - \ru Набор контуров содержащих кривые границ ребер жесткости(при их наличии) в разогнутом виде.
                            \en The set of contours, which are containing edges of stamp rib in unfolded state. \~
  \result \ru Возвращает строитель оболочки.
          \en Returns the shell constructor. \~
  \ingroup Model_Creators
*/
// ---
MATH_FUNC (MbCreator *) CreateBendUnbend(       SPtr<MbFaceShell> &         initialShell,
                                                MbeCopyMode                 sameShell,
                                          const RPArray<MbSheetMetalBend> & bends,
                                          const MbFace &                    fixedFace,
                                          const MbCartPoint &               fixedPoint,
                                                bool                        bend,
                                          const MbSNameMaker &              names,
                                                MbResultType &              res,
                                                MbFaceShell *&              shell,
                                                RPArray<MbContour3D> *      ribContours = nullptr );


#endif // __CR_SHEET_BEND_UNBEND_SOLID_H
