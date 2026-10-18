////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Строитель листовой линейчатой оболочки.
         \en Constructor of a sheet ruled shell.
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_STAMP_RULED_SOLID_H
#define __CR_STAMP_RULED_SOLID_H


#include <cur_contour.h>
#include <creator.h>
#include <sheet_metal_param.h>


//------------------------------------------------------------------------------
/** \brief \ru Строитель листовой линейчатой оболочки.
           \en Constructor of a sheet ruled shell. \~
  \details \ru Строитель оболочки по заданным контурам, соединением их линейчатой поверхностью и приданием толщины. \n 
    Второй контур и его локальная система координат могут отсутствовать, 
    в этом случае они создаются по параметрам операции - высоте и углу уклона. 
    Углы контуров скругляются радиусом, заданным в параметрах.
           \en Constructor of a shell from the given contours by connecting them with a ruled surface and supplying with thickness. \n 
    The second contour and the local coordinate system can be absent, 
    in this case they are created from the parameters of the operation - the height and the slope angle. 
    Corners of the contours are rounded with the radius given as a parameter. \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbRuledSolid : public MbCreator {
private:
  MbRuledSolidValues    parameters; ///< \ru Параметры операции. \en The operation parameters. 
  PArray<MbSMBendNames> bendParams; ///< \ru Параметры формируемых сгибов. \en Parameters of bends being formed. 

public :
  MbRuledSolid( const MbRuledSolidValues &    parameters,
                const MbSNameMaker &          nameMaker,
                const PArray<MbSMBendNames> & bendParams );

private:
  MbRuledSolid( const MbRuledSolid &, MbRegDuplicate * iReg );
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbRuledSolid( const MbRuledSolid & );

public:
  virtual ~MbRuledSolid();

  // \ru Общие функции математического объекта \en Common functions of the mathematical object 
  MbeCreatorType  IsA()  const override; // \ru Тип элемента \en A type of element 
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию \en Create a copy

  bool        IsSame   ( const MbCreator & other, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        IsSimilar( const MbCreator & item ) const override; // \ru Являются ли объекты подобными? \en Determine whether an object is similar?
  bool        SetEqual ( const MbCreator & ) override; // \ru Сделать равным \en Make equal 

  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override; // \ru Сдвиг \en Translation
  void        Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate around an axis

  void        GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of the object 
  MbePrompt   GetPropertyName() override; // \ru Выдать заголовок свойства объекта \en Get a name of object property 
  void        GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.
  
  // \ru Общие функции твердого тела. \en Common functions of solid. 

  bool        CreateShell( MbFaceShell *&         shell,
                           MbeCopyMode            sameShell, 
                           RPArray<MbSpaceItem> * items = nullptr ) override; // \ru Построение \en Construction
  // \ru Дать базовые объекты. \en Get the base objects. 
  void        GetBasisItems( RPArray<MbSpaceItem> & s ) override;
  
  // \ru Дать параметры. \en Get the parameters. 
  void        GetParameters(       MbRuledSolidValues & params ) const { params = parameters; }
  // \ru Установить параметры. \en Set the parameters. 
  void        SetParameters( const MbRuledSolidValues & params )       { parameters = params; }

private:
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
  MbRuledSolid & operator = ( const MbRuledSolid & ); 
  
  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbRuledSolid )
}; // MbRuledSolid

IMPL_PERSISTENT_OPS( MbRuledSolid )


//------------------------------------------------------------------------------
/** \brief \ru Построить линейчатую оболочку по контуру.
           \en Create a ruled shell from the contour. \~
  \details \ru Построить листовую оболочку выдавливанием плоского контура с уклоном и приданием ему толщины.\n
    Одновременно с построением оболочки функция создаёт её строитель.\n
           \en Create a sheet shell by extruding of a planar contour with a slope and supplying it with thickness.\n
    The function simultaneously creates the shell and its constructor.\n \~ 
  \param[in]  parameters  - \ru Параметры операции.
                            \en The operation parameters. \~
  \param[in]  operNames   - \ru Именователь операции.
                            \en An object defining names generation in the operation. \~
  \param[in]  resultBends - \ru Параметры и имена элементов сгиба.
                            \en Parameters and names of bend's elements. \~
  \param[out] res         - \ru Код результата операции выдавливания.
                            \en The extrusion operation result code. \~
  \param[out] shell       - \ru Построенная оболочка.
                            \en The resultant shell. \~
  \result \ru Возвращает строитель оболочки.
          \en Returns the shell constructor. \~
  \ingroup Model_Creators
*/
// ---
MATH_FUNC (MbCreator *) CreateRuledSolid(       MbRuledSolidValues &     parameters,
                                          const MbSNameMaker &           operNames,
                                                RPArray<MbSMBendNames> & resultBends,
                                                MbContour *&             resultContour,
                                                MbResultType &           res,
                                                MbFaceShell *&           shell );


#endif // __CR_STAMP_RULED_SOLID_H
