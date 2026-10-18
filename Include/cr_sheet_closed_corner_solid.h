////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Построение оболочки из листового материала с замыканием угла.
         \en Construction of a shell from sheet material with corner enclosure. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_SHEET_CLOSED_CORNER_SOLID_H
#define __CR_SHEET_CLOSED_CORNER_SOLID_H


#include <creator.h>
#include <sheet_metal_param.h>


//------------------------------------------------------------------------------
/** \brief \ru Строитель оболочки из листового материала с замыканием угла.
           \en Constructor of a shell from sheet material with corner enclosure. \~
  \details \ru Строитель оболочки из листового материала с замыканием угла. 
    В зависимости от параметров замыкание продолжений сгибов может быть с перекрытием, встык и плотное, 
    а сами сгибы могут остаться без замыкания или замкнуться по хорде или по кромке. 
    Возможно также построение замыкания с зазором. \n
           \en Constructor of a shell from sheet material with corner enclosure. 
    Subject to the parameters closure of bends extensions can be overlapping, butted or tight, 
    the bends themselves can remain unclosed or can be closed by a chord or a boundary. 
    Construction of corner closure with a gap is also possible. \n \~ 
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbClosedCornerSolid : public MbCreator {
  MbEdgeFacesIndexes   edgeIndexPlus;  ///< \ru Идентификатор ребра сгиба, условно принятого за положительное. \en Identifier of an edge of the bend considered to be positive. 
  MbEdgeFacesIndexes   edgeIndexMinus; ///< \ru Идентификатор ребра сгиба, условно принятого за отрицательное. \en Identifier of an edge of the bend considered to be negative. 
  MbClosedCornerValues parameters;     ///< \ru Параметры замыкания угла. \en Parameters of a corner closure. 

public :
  MbClosedCornerSolid( const MbEdgeFacesIndexes     edgeIndexPlus,
                       const MbEdgeFacesIndexes     edgeIndexMinus,
                       const MbClosedCornerValues & params,
                       const MbSNameMaker &         nameMaker );
private:
  MbClosedCornerSolid( const MbClosedCornerSolid &, MbRegDuplicate * iReg );
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbClosedCornerSolid( const MbClosedCornerSolid & );

public:
  virtual ~MbClosedCornerSolid();

  // \ru Общие функции математического объекта \en Common functions of the mathematical object 

  MbeCreatorType  IsA()  const override; // \ru Тип элемента \en Type of element 
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию \en Create a copy

  bool        IsSame   ( const MbCreator & other, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        IsSimilar( const MbCreator & item ) const override; // \ru Являются ли объекты подобными? \en Determine whether an object is similar?
  bool        SetEqual ( const MbCreator & ) override; // \ru Сделать равным \en Make equal

  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override; // \ru Сдвиг \en Translation
  void        Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate about an axis

  void        GetProperties( MbProperties & properties ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & properties ) override; // \ru Записать свойства объекта \en Set properties of the object 
  MbePrompt   GetPropertyName() override; // \ru Выдать заголовок свойства объекта \en Get a name of object property 

  // \ru Общие функции твердого тела \en Common functions of solid solid 
  
  bool        CreateShell( MbFaceShell *&         shell,
                           MbeCopyMode            sameShell, 
                           RPArray<MbSpaceItem> * items = nullptr ) override; // \ru Построение \en Construction

  // \ru Дать параметры. \en Get the parameters. 
  void        GetParameters(       MbClosedCornerValues & params ) const { params = parameters; }
  // \ru Установить параметры. \en Set the parameters. 
  void        SetParameters( const MbClosedCornerValues & params )       { parameters = params; }

private:
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
  MbClosedCornerSolid  & operator = ( const MbClosedCornerSolid & );
  
  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbClosedCornerSolid )
}; // MbClosedCornerSolid

IMPL_PERSISTENT_OPS( MbClosedCornerSolid )


//------------------------------------------------------------------------------
/** \brief \ru Построить оболочку из листового материала с замыканием угла.
           \en Construct a shell form sheet material with corner closure. \~
  \details \ru Построить оболочку из листового материала с замыканием угла. 
    В зависимости от параметров замыкание продолжений сгибов может быть с перекрытием, встык и плотное, 
    а сами сгибы могут остаться без замыкания или замкнуться по хорде или по кромке. 
    Возможно также построение замыкания с зазором. \n 
    Одновременно с построением оболочки функция создаёт её строитель.\n
           \en Construct a shell form sheet material with corner closure. 
    Subject to the parameters closure of bends extensions can be overlapping, butted or tight, 
    the bends themselves can remain unclosed or can be closed by a chord or a boundary. 
    Construction of corner closure with a gap is also possible. \n 
    The function simultaneously creates the shell and its constructor.\n \~ 
  \param[in]  initialShell - \ru Исходная оболочка.
                             \en The source shell. \~
  \param[in]  sameShell  - \ru Режим копирования исходной оболочки.
                           \en Mode of copying the source shell. \~
  \param[in]  curveEdgePlus - \ru Ребро сгиба, условно принятого за положительное.
                              \en Edge of the bend considered as positive. \~
  \param[in]  curveEdgeMinus - \ru Ребро сгиба, условно принятого за отрицательное.
                               \en Edge of the bend considered as negative. \~
  \param[in]  parameters - \ru Параметры построения.
                           \en Parameters of shell creation. \~
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
MATH_FUNC (MbCreator *) CreateClosedCorner(       SPtr<MbFaceShell> &    initialShell,
                                                  MbeCopyMode            sameShell,
                                                  MbCurveEdge *          curveEdgePlus,
                                                  MbCurveEdge *          curveEdgeMinus,
                                            const MbClosedCornerValues & parameters,
                                            const MbSNameMaker &         names,
                                                  MbResultType &         res,
                                                  MbFaceShell *&         shell );


#endif // __CR_SHEET_CLOSED_CORNER_SOLID_H
