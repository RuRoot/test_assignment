////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Строитель оболочки из листового материала на основе произвольного тела.
         \en Constructor of the sheet metal shell based on an arbitrary solid.
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_SHEET_BUILDER_SOLID_H
#define __CR_SHEET_BUILDER_SOLID_H


#include <creator.h>
#include <sheet_metal_param.h>


//------------------------------------------------------------------------------
/** \brief \ru Строитель оболочки из листового материала на основе произвольного тела.
           \en Constructor of the sheet metal shell based on an arbitrary solid. \~
  \details \ru Строитель оболочки из листового материала на основе граней и ребер произвольного тела.\n
               Оболочка строится на базе исходной плоской грани и указанных ребер сгиба и разреза.
           \en Constructor of the sheet metal shell based on faces and edges of an arbitrary solid. \n 
               Shell builds based on initial planar face and given edges of bend and corner enclosure. \~ 
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbBuildSheetMetalSolid : public MbCreator {
private:
  MbItemIndex               faceIndex;  ///< \ru Индекс исходной грани для построения листового тела. \en Index of initial face for sheet metal solid creation. 
  bool                      sense;      ///< \ru Признак совпадения придания толщины с нормалью исходной грани. \en Attribute of coincidence of extrusion direction to the normal of the initial face. 
  MbSolidToSheetMetalValues parameters; ///< \ru Параметры построения листового тела по произвольному телу. \en The parameters of sheet metal solid building based on an arbitrary solid. 

public :
  MbBuildSheetMetalSolid( const MbItemIndex &               faceIndex,
                          const bool                        sense,
                          const MbSolidToSheetMetalValues & params,
                          const MbSNameMaker &              names );
private:
  MbBuildSheetMetalSolid( const MbBuildSheetMetalSolid &, MbRegDuplicate * iReg );

public:
  virtual ~MbBuildSheetMetalSolid();

  // \ru Общие функции математического объекта. \en Common functions of the mathematical object.

  MbeCreatorType  IsA()  const override; // \ru Тип элемента. \en A type of element.
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию. \en Create a copy.

  bool        IsSame   ( const MbCreator & other, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        IsSimilar( const MbCreator & item ) const override; // \ru Являются ли объекты подобными? \en Determine whether an object is similar?
  bool        SetEqual ( const MbCreator & ) override; // \ru Сделать равным. \en Make equal.

  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать элемент согласно матрице. \en Transform element according to the matrix.
  void        Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override; // \ru Сдвиг. \en Translation.
  void        Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси. \en Rotate around an axis.

  void        GetProperties  ( MbProperties & properties ) override; // \ru Выдать свойства объекта. \en Get properties of the object.
  void        SetProperties  ( const MbProperties & properties ) override; // \ru Записать свойства объекта. \en Set properties of the object.
  MbePrompt   GetPropertyName() override; // \ru Выдать заголовок свойства объекта. \en Get a name of object property.

  // \ru Общие функции твердого тела. \en Common functions of solid.
          
  // \ru Построение оболочки листового тела. \en Construction of a sheet metal shell.
  bool        CreateShell( MbFaceShell *& shell, MbeCopyMode sameShell, RPArray <MbSpaceItem> *items = nullptr ) override;
  // \ru Получить параметры. \en Get the parameters. 
  void        GetParameters( MbSolidToSheetMetalValues & params ) const { params = parameters; }
  // \ru Установить параметры. \en Set the parameters. 
  void        SetParameters( const MbSolidToSheetMetalValues & params ) { parameters = params; }

private:
  OBVIOUS_PRIVATE_COPY( MbBuildSheetMetalSolid )
  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbBuildSheetMetalSolid )
}; // MbBuildSheetMetalSolid

IMPL_PERSISTENT_OPS( MbBuildSheetMetalSolid )


//------------------------------------------------------------------------------
/** \brief \ru Строитель оболочки из листового материала на основе произвольного тела.
           \en Constructor of the sheet metal shell based on an arbitrary solid. \~
  \details \ru На базе исходной произвольной оболочки построить оболочку из листового материала. \n
               Одновременно с построением оболочки функция создаёт её строитель.\n
           \en A shell is to be constructed on the basis of the source arbitrary shell. \n
               The function simultaneously creates the shell and its constructor.\n \~ 
  \param[in]  initialShell  - \ru Исходная оболочка.
                              \en The source shell. \~
  \param[in]  sameShell     - \ru Режим копирования исходной оболочки.
                              \en Mode of copying the source shell. \~
  \param[in,out] parameters - \ru Параметр операции.
                              \en The operation parameters. \~
  \param[out] res           - \ru Код результата операции.
                              \en Operation result code. \~
  \param[out] resultShell   - \ru Результирующее тело.
                              \en The resultant solid. \~
  \result \ru Возвращает строитель оболочки.
          \en Returns the shell constructor. \~
  \ingroup Model_Creators
*/
// ---
MATH_FUNC (c3d::CreatorSPtr) ConvertShellToSheetMetall( const c3d::ShellSPtr &            initialShell,
                                                        const MbeCopyMode                 sameShell,
                                                              MbSolidToSheetMetalParams & parameters,
                                                              MbResultType &              res,
                                                              c3d::ShellSPtr &            resultShell );


#endif // __CR_SHEET_BUILDER_SOLID_H
