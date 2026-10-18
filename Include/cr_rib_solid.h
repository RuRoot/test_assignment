////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Строитель тела с ребром жёсткости.
         \en Constructor of a solid with a rib.
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_RIB_SOLID_H
#define __CR_RIB_SOLID_H


#include <cur_contour.h>
#include <creator.h>
#include <op_swept_parameter.h>


class MbRibSolidParameters;


//------------------------------------------------------------------------------
/** \brief \ru Строитель тела с ребром жёсткости.
           \en Constructor of a solid with a rib. \~
  \details \ru Строитель тела с ребром жёсткости, форма которого задана плоским контуром.
           \en Constructor of a solid with a rib whose shape is specified by a planar contour. \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbRibSolid : public MbCreator {
protected :
  MbPlacement3D place;      ///< \ru Подложка для формообразующей кривой, точки и вектора уклона. \en Placement of the forming curve, point and inclination vector. 
  MbContour *   spine;      ///< \ru Формообразующая кривая (хребет ребра жёсткости). \en Forming curve (rib's spine). 
  size_t        index;      ///< \ru Индекс сегмента в контуре, от которого будет установлено направление уклона. \en The segment index in the contour from which the inclination direction will be set. 
  RibValues     parameters; ///< \ru Параметры формообразования ребра жёсткости. \en Forming parameters of the rib. 

public :
  MbRibSolid( const MbPlacement3D & place, const MbContour & contour,
              size_t index, const RibValues & param, const MbSNameMaker & n );
private :
  MbRibSolid( const MbRibSolid & bres, MbRegDuplicate * ireg );
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbRibSolid( const MbRibSolid & bres );
public :
  virtual ~MbRibSolid();

  // \ru Общие функции математического объекта \en Common functions of the mathematical object 

  MbeCreatorType  IsA () const override; // \ru Тип элемента \en A type of element
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию \en Create a copy
  void        Transform ( const MbMatrix3D &, MbRegTransform * = nullptr ) override;  // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move      ( const MbVector3D &, MbRegTransform * = nullptr ) override;  // \ru Сдвиг \en Translation
  void        Rotate    ( const MbAxis3D   &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate around an axis

  void        GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of the object 
  MbePrompt   GetPropertyName() override; // \ru Выдать заголовок свойства объекта \en Get a name of object property 
  void        GetBasisItems ( RPArray<MbSpaceItem> & ) override; // \ru Дать базовые объекты \en Get the base objects 

  bool        IsSame( const MbCreator &, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        IsSimilar( const MbCreator & ) const override; // \ru Являются ли объекты подобными \en Whether the objects are similar 
  bool        SetEqual ( const MbCreator & ) override;       // \ru Сделать равным \en Make equal 

  // \ru Общие функции твердого тела \en Common functions of solid 

  bool        CreateShell( MbFaceShell *& shell, MbeCopyMode sameShell,
                           RPArray<MbSpaceItem> * = nullptr ) override; // \ru Построение \en Construction

  // \ru Дать параметры. \en Get the parameters. 
  void        GetParameters(       RibValues & params ) const { params = parameters; }
  // \ru Установить параметры. \en Set the parameters. 
  void        SetParameters( const RibValues & params )       { parameters = params; }

private :
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
  void        operator = ( const MbRibSolid & ); // \ru НЕЛЬЗЯ!!! \en NOT ALLOWED!!! 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbRibSolid )
}; // MbRibSolid

IMPL_PERSISTENT_OPS( MbRibSolid )


//------------------------------------------------------------------------------
/** \brief \ru Создать оболочку с ребром жёсткости.
           \en Create a shell with a rib. \~
  \details \ru Для указанной оболочки построить оболочку с ребром жёсткости, форма которого задана плоским контуром.\n 
    Одновременно с построением оболочки функция создаёт её строитель. \n
           \en For a specified shell create a shell with a rib which shape is given by the planar contour.\n 
    The function simultaneously constructs the shell and creates its constructor. \n \~
  \param[in]  solid          - \ru Исходная оболочка.
                               \en The source shell. \~
  \param[in]  sameShell      - \ru Способ копирования граней исходной оболочки.
                               \en Method of copying the source shell faces. \~
  \param[in]  parameters     - \ru Правметры операции.
                               \en The operation parameters. \~
  \param[out] res            - \ru Код результата операции.
                               \en Operation result code. \~
  \param[out] shell          - \ru Построенный набор граней.
                               \en Constructed set of faces. \~  
  \result \ru Возвращает строитель, если операция была выполнена успешно.
          \en Returns the constructor if the operation has been successfully performed. \~
  \ingroup Model_Creators
*/
// ---
MATH_FUNC (MbCreator *) CreateRib(       c3d::ShellSPtr & solid,
                                         MbeCopyMode      sameShell, 
                                   const MbRibSolidParameters & parameters, 
                                         MbResultType   & res,
                                         c3d::ShellSPtr & shell );


//------------------------------------------------------------------------------
/** \brief \ru Создать отдельное ребро жёсткости.
           \en Create a separate rib. \~
  \details \ru Для указанной оболочки построить оболочку в виде отдельного ребра жёсткости.
    Одновременно с построением оболочки функция создаёт её строитель. \n
           \en For the specified shell create a shell as a separate rib.
    The function simultaneously constructs the shell and creates its constructor. \n \~
  \param[in]  solid          - \ru Исходная оболочка.
                               \en The source shell. \~
  \param[in]  place          - \ru Локальная система координат, в плоскости XY которай расположен двумерный контур.
                               \en A local coordinate system the two-dimensional contour is located in XY plane of. \~
  \param[in]  contour        - \ru Двумерный контур ребра жесткости расположен в плоскости XY локальной системы координат.
                               \en Two-dimensional contour of a rib located in XY plane of the local coordinate system. \~
  \param[in]  index          - \ru Индекс сегмента в контуре, от которого будет установлено направление уклона.
                               \en Index of a segment in the contour at which the inclination direction will be set. \~
  \param[in]  parameters     - \ru Правметры операции.
                               \en The operation parameters. \~
  \param[in]  operNames      - \ru Именователь операции.
                               \en An object defining names generation in the operation. \~
  \param[out] res            - \ru Код результата операции.
                               \en Operation result code. \~
  \param[out] shell          - \ru Построенный набор граней.
                               \en Constructed set of faces. \~  
  \result \ru Возвращает строитель, если операция была выполнена успешно.
          \en Returns the constructor if the operation has been successfully performed. \~
  \ingroup Model_Creators
*/
// ---
MATH_FUNC (MbCreator *) CreateRibElement(       c3d::ShellSPtr       & solid,
                                          const MbRibSolidParameters & parameters,
                                                MbResultType         & res,
                                                c3d::ShellSPtr       & shell );


#endif // __CR_RIB_SOLID_H
