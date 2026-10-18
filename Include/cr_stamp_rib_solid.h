////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Строитель листового тела с ребром жёсткости.
         \en Constructor of a sheet solid with a rib.
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_STAMP_RIB_SOLID_H
#define __CR_STAMP_RIB_SOLID_H


#include <cur_contour.h>
#include <creator.h>
#include <op_swept_parameter.h>


class MbSheetRibSolidParams;


//------------------------------------------------------------------------------
/** \brief \ru Строитель тела с ребром жёсткости.
           \en Constructor of a sheet solid with a rib. \~
  \details \ru Строитель тела с ребром жёсткости, форма которого задана плоским контуром.
           \en Constructor of a solid with a rib whose shape is specified by a planar contour. \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbStampRibSolid : public MbCreator {
protected :
  MbPlacement3D  place;      ///< \ru Подложка для формообразующей кривой, точки и вектора уклона. \en Placement of the forming curve, point and inclination vector. 
  MbContour *    spine;      ///< \ru Формообразующая кривая (хребет ребра жёсткости). \en Forming curve (rib's spine). 
  size_t         index;      ///< \ru Индекс сегмента в контуре, от которого будет установлено направление уклона. \en The segment index in the contour from which the inclination direction will be set. 
  SheetRibValues parameters; ///< \ru Параметры формообразования. \en Forming parameters. 

public :
  MbStampRibSolid( const MbPlacement3D &  place,
                   const MbContour &      contour,
                         size_t           index, 
                   const SheetRibValues & param,
                   const MbSNameMaker &   names );
private :
  MbStampRibSolid( const MbStampRibSolid & bres, MbRegDuplicate * ireg );
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbStampRibSolid( const MbStampRibSolid & bres );
public :
  virtual ~MbStampRibSolid();

  // \ru Общие функции математического объекта \en Common functions of the mathematical object 

  MbeCreatorType  IsA () const override; // \ru Тип элемента \en A type of element
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию \en Create a copy
  void        Transform ( const MbMatrix3D & matr, MbRegTransform * = nullptr ) override;  // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move      ( const MbVector3D & to, MbRegTransform * = nullptr ) override;  // \ru Сдвиг \en Translation
  void        Rotate    ( const MbAxis3D   & axis, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate around an axis

  void        GetProperties( MbProperties & properties ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & properties ) override; // \ru Записать свойства объекта \en Set properties of the object 
  MbePrompt   GetPropertyName() override; // \ru Выдать заголовок свойства объекта \en Get a name of object property 

  bool        IsSame   ( const MbCreator & other, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        IsSimilar( const MbCreator & item ) const override; // \ru Являются ли объекты подобными? \en Determine whether an object is similar?
  bool        SetEqual( const MbCreator & ) override;         // \ru Сделать равным \en Make equal

  // \ru Общие функции твердого тела \en Common functions of solid 

  bool        CreateShell( MbFaceShell *&shell, MbeCopyMode sameShell,
                           RPArray <MbSpaceItem> *items = nullptr ) override; // \ru Построение \en Construction

  // \ru Дать параметры. \en Get the parameters. 
  void        GetParameters( SheetRibValues & params ) const { params = parameters; }
  // \ru Установить параметры. \en Set the parameters. 
  void        SetParameters( const SheetRibValues & params )       { parameters = params; }

private :
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
  void        operator = ( const MbStampRibSolid & ); // \ru НЕЛЬЗЯ!!! \en NOT ALLOWED!!! 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbStampRibSolid )
}; // MbStampRibSolid

IMPL_PERSISTENT_OPS( MbStampRibSolid )


//------------------------------------------------------------------------------
/** \brief \ru Создать оболочку с ребром жёсткости.
           \en Create a shell with a rib. \~
  \details \ru Для указанной листовой оболочки построить оболочку с ребром жёсткости, форма которого задана плоским контуром.\n
    Одновременно с построением оболочки функция создаёт её строитель. \n
           \en For a specified sheet shell create a shell with a rib which shape is given by the planar contour.\n
    The function simultaneously constructs the shell and creates its constructor. \n \~
  \param[in]  solid     - \ru Исходная оболочка.
                          \en The source shell. \~
  \param[in]  sameShell - \ru Способ копирования граней исходной оболочки.
                          \en Method of copying the source shell faces. \~
  \param[in]  params    - \ru Параметры построения ребра жесткости для листового тела.
                          \en Parameters for creating a sheet solid with rib. \~
  \param[out] res       - \ru Код результата операции.
                          \en Operation result code. \~
  \param[out] shell     - \ru Построенный набор граней.
                          \en Constructed set of faces. \~
  \result \ru Возвращает строитель, если операция была выполнена успешно.
          \en Returns the constructor if the operation has been successfully performed. \~
  \ingroup Model_Creators
*/
// ---
MATH_FUNC (c3d::CreatorSPtr) CreateSheetRib( const c3d::ShellSPtr &        solid,
                                                   MbeCopyMode             sameShell,
                                             const MbSheetRibSolidParams & params,
                                                   MbResultType &          res,
                                                   c3d::ShellSPtr &        shell );


//------------------------------------------------------------------------------
/** \brief \ru Создание элементов оболочки с ребром жесткости.
           \en Create rib parts of a shell. \~
  \details \ru Создание элементов оболочки листового тела с ребром жесткости. \n
               По заданному контуру функция строит ребро жёсткости, а затем вычленяет составляющие его элементы, заделывая места разрыва. \n
           \en Create rib parts of a sheet shell. \n
               The function creates a rib from a given contour and then extructs its elements from the body, healing rip borders with patches. \n \~
  \param[in]  solid           - \ru Исходная оболочка.
                                \en The source shell. \~
  \param[in]  sameShell       - \ru Способ копирования граней исходной оболочки.
                                \en Method of copying the source shell faces. \~
  \param[in]  params          - \ru Параметры построения ребра жесткости для оболочки листового тела.
                                \en Parameters for creating a sheet shell with rib. \~
  \param[out] shellToAdd      - \ru Добавляемый элемент ребра.
                                \en The adding rib element. \~
  \param[out] shellToSubtract - \ru Вычитаемый элемент ребра.
                                \en The subtracting rib element. \~
  \return \ru Возвращает код результата операции.
          \en Returns operation result code. \~
  \ingroup Sheet_Metal_Modeling
*/
// ---
MATH_FUNC (MbResultType) CreateSheetRibParts( const c3d::ShellSPtr &        solid,
                                                    MbeCopyMode             sameShell,
                                              const MbSheetRibSolidParams & params,
                                                    c3d::ShellSPtr &        shellToAdd,
                                                    c3d::ShellSPtr &        shellToSubtract );


#endif // __CR_STAMP_RIB_SOLID_H
