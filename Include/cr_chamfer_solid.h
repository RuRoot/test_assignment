////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Строитель тела с фасками рёбер.
         \en Constructor of solid with edges' chamfers.
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_CHAMFER_SOLID_H
#define __CR_CHAMFER_SOLID_H


#include <cr_smooth_solid.h>


//------------------------------------------------------------------------------
/** \brief \ru Строитель тела с фасками рёбер.
           \en Constructor of solid with edges' chamfers. \~
  \details \ru Строитель тела с фасками рёбер, выполняющий замену указанных рёбер линейчатыми гранями,
    стыкующимися со смежными гранями обрабатываемых ребер.
           \en Constructor of solid with edges' chamfers performing the replacement of the specified edges by ruled faces
    connected with the adjacent faces of the edges being processed. \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbChamferSolid : public MbSmoothSolid {
public :

public :
  MbChamferSolid( SArray<MbEdgeFacesIndexes> & _indexes,
                  const SmoothValues & params, const MbSNameMaker & n );
private :
  MbChamferSolid( const MbChamferSolid & init, MbRegDuplicate * iReg ); // \ru Конструктор копирования \en Copy-constructor 
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbChamferSolid( const MbChamferSolid & init ); 
public :
  virtual ~MbChamferSolid();

  // \ru Общие функции математического объекта \en Common functions of the mathematical object 

  MbeCreatorType  IsA() const override; // \ru Тип элемента \en A type of element 
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию \en Create a copy

  void        GetProperties( MbProperties &properties ) override; // \ru Выдать свойства объекта \en Get properties of the object
  void        SetProperties( const MbProperties &properties ) override; // \ru Записать свойства объекта \en Set properties of the object
  MbePrompt   GetPropertyName() override; // \ru Выдать заголовок свойства объекта \en Get a name of object property 

  bool        IsSame( const MbCreator & other, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal? 
  bool        SetEqual( const MbCreator &init ) override;    // \ru Сделать равным \en Make equal

  // \ru Общие функции твердого тела \en Common functions of solid  

  bool        CreateShell( MbFaceShell *&shell, MbeCopyMode sameShell,
                           RPArray<MbSpaceItem> * items = nullptr ) override; // \ru Построение \en Construction

private :
  void        ReadDistances ( reader &in ) override;
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
  void        operator = ( const MbChamferSolid & );

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbChamferSolid )
}; // MbChamferSolid

IMPL_PERSISTENT_OPS( MbChamferSolid )


//------------------------------------------------------------------------------
/** \brief \ru Создать оболочку с фасками ребер.
           \en Create a shell with edges' chamfers. \~
  \details \ru Для указанной оболочки построить оболочку, в которой выполнены фаски указанных рёбер.\n
           \en For the given shell create a shell with chamfers of the specified edges.\n \~ 
  \param[in]  solid         - \ru Исходная оболочка.
                              \en The source shell. \~
  \param[in]  sameShell     - \ru Способ копирования граней исходной оболочки.
                              \en Method of copying the source shell faces. \~
  \param[in] chamferParams  - \ru Параметры операции.
                              \en The operation parameters. \~
                              \en An object defining names generation in the operation. \~
  \param[out] res           - \ru Код результата операции.
                              \en Operation result code. \~
  \param[out] shell         - \ru Построенный набор граней.
                              \en Constructed set of faces. \~  
  \result \ru Возвращает строитель, если операция была выполнена успешно.
          \en Returns the constructor if the operation has been successfully performed. \~
  \ingroup Model_Creators
*/
// ---
MATH_FUNC (MbCreator *) CreateChamfer(        MbFaceShell *           solid, 
                                              MbeCopyMode             sameShell, 
                                       const  MbShellChamferParams &  chamferParams,
                                              MbResultType &          res, 
                                              MbFaceShell *&          shell );


#endif // __CR_CHAMFER_SOLID_H
