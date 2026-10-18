////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Строитель оболочки из листового материала с жалюзи.
         \en Constructor of a shell form sheet material with jalousie.
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_STAMP_JALOUSIE_SOLID_H
#define __CR_STAMP_JALOUSIE_SOLID_H

#include <cur_line_segment.h>
#include <creator.h>
#include <sheet_metal_param.h>


class MbCurveBoundedSurface;


//------------------------------------------------------------------------------
/** \brief \ru Строитель оболочки из листового материала с жалюзи.
           \en Constructor of a shell form sheet material with jalousie. \~
  \details \ru Строитель оболочки из листового материала с жалюзи. \n 
    В зависимости от параметров строятся вытянутые или подрезанные жалюзи.
    В случае подрезанных жалюзи материал плоского участка листового тела
    подрезается с трёх сторон образующего прямоугольника и отгибается относительно четвёртой стороны.
    В случае вытянутых жалюзи отгибаемый материал принимает цилиндрическую форму вдоль
    отрезка построения и сферическую форму на его концах.
           \en Constructor of a shell form sheet material with jalousie. \n 
    Subject to the parameters stretched or trimmed jalousie are constructed.
    In the case of trimmed jalousie the material of the planar part of the sheet solid
    is trimmed from three sides of the generating rectangle and deflected relative to the fourth side.
    In the case of stretched jalousie the deflected material take the cylindric shape along
    the construction segment and the spherical shape at its ends. \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbJalousieSolid : public MbCreator {
  MbItemIndex             faceIndex;     ///< \ru Индекс грани, на которой строятся жалюзи. \en Index of the face on which jalousie are constructed. 
  MbItemIndex             pairFaceIndex; ///< \ru Индекс грани парной к грани жалюзи. \en Index of the face which is pair to the jalousie face.
  MbCurveBoundedSurface * boundSurface;  ///< \ru Поверхность, границами которой надо подрезать жалюзи. \en Surface, by which bounds the jalousie is cutted.
  MbPlacement3D           placement;     ///< \ru Локальная система координат, в плоскости XY которй расположены отрезки, по которым строятся жалюзи. \en The local coordinate system in the XY plane of which the segments are located by which jalousie are constructed. 
  RPArray<MbLineSegment>  lineSegments;  ///< \ru Отрезки, по которым строятся жалюзи. \en The segments by which jalousie are constructed. 
  MbJalousieValues        parameters;    ///< \ru Параметры жалюзи. \en Jalousie parameters. 
  double                  thickness;     ///< \ru Толщина листа. \en The thickness of the sheet metal.
  bool                    add;           ///< \ru Создавать добавляемую или вычитаемую часть жалюзи. \en Create additional or subtructional part of a jalousie.

public :
  MbJalousieSolid( const MbItemIndex &            faceIndex,
                   const MbItemIndex &            pairFaceIndex,
                   const MbCurveBoundedSurface *  boundSurface,
                   const MbPlacement3D &          placement,
                   const RPArray<MbLineSegment> & lineSegments,
                   const MbJalousieValues &       params,
                   const double                   thickness,
                   const bool                     add,
                   const MbSNameMaker &           names );
private:
  MbJalousieSolid( const MbJalousieSolid &, MbRegDuplicate * iReg );
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbJalousieSolid( const MbJalousieSolid & );

public:
  virtual ~MbJalousieSolid();

  // \ru Общие функции математического объекта \en Common functions of the mathematical object 

  MbeCreatorType  IsA      ()  const override; // \ru Тип элемента \en A type of element
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию \en Create a copy
  bool        IsSame   ( const MbCreator & other, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        IsSimilar( const MbCreator & item ) const override; // \ru Являются ли объекты подобными? \en Determine whether an object is similar?
  bool        SetEqual ( const MbCreator & ) override; // \ru Сделать равным \en Make equal 
  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override; // \ru Сдвиг \en Translation
  void        Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate around an axis

  void        GetProperties  ( MbProperties &properties ) override; // \ru Выдать свойства объекта \en Get properties of the object
  void        SetProperties  ( const MbProperties &properties ) override; // \ru Записать свойства объекта \en Set properties of the object
  MbePrompt   GetPropertyName() override; // \ru Выдать заголовок свойства объекта \en Get a name of object property 
  void        GetBasisItems ( RPArray<MbSpaceItem>   & ) override; // \ru Дать базовые объекты \en Get the base objects
  void        GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  // \ru Общие функции твердого тела \en Common functions of solid 

  bool        CreateShell( MbFaceShell *& shell, MbeCopyMode sameShell, 
                           RPArray<MbSpaceItem> * items = nullptr ) override; // \ru Построение \en Construction

  // \ru Дать параметры. \en Get the parameters. 
  void        GetParameters(       MbJalousieValues & params ) const { params = parameters; }
  // \ru Установить параметры. \en Set the parameters. 
  void        SetParameters( const MbJalousieValues & params )       { parameters = params; }

private:
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
  MbJalousieSolid  &  operator = ( const MbJalousieSolid & ); // \ru Не реализовано \en Not implemented 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbJalousieSolid )
}; // MbJalousieSolid

IMPL_PERSISTENT_OPS( MbJalousieSolid )


//------------------------------------------------------------------------------
/** \brief \ru Построить оболочку из листового материала с жалюзи.
           \en Construct a shell from a sheet material with jalousie. \~
  \details \ru Построить оболочку из листового материала с вытянутыми или подрезанными жалюзи. \n
               Одновременно с построением оболочки функция создаёт её строитель.\n
           \en Construct a shell from a sheet material with stretched or trimmed jalousie. \n
               The function simultaneously creates the shell and its constructor.\n \~ 
  \param[in]  initialShell      - \ru Исходная оболочка.
                                  \en The source shell. \~
  \param[in]  sameShell  - \ru Режим копирования исходной оболочки.
                           \en Mode of copying the source shell. \~
  \param[in]  face       - \ru Грань жалюзи.
                           \en A face of jalousie. \~
  \param[in]  placement  - \ru Локальная система координат, в плоскости XY которй расположены отрезки жалюзи.
                           \en The local coordinate system in the XY plane of which the segments of  are located. \~
  \param[in]  lineSegments - \ru Отрезки жалюзи.
                             \en The segments of jalousie. \~
  \param[in]  parameters - \ru Параметры жалюзи.
                           \en The parameters of jalousie. \~
  \param[in]  thickness  - \ru Толщина пластины.
                           \en The thickness of the plate. \~
  \param[in]  add        - \ru Создавать добавляемую компоненту жалюзи.
                           \en To create the added part of the jalousie. \~
  \param[in]  nameMaker  - \ru Именователь операции.
                           \en An object defining names generation in the operation. \~
  \param[out] res        - \ru Код результата операции выдавливания.
                           \en The extrusion operation result code. \~
  \param[out] resultShell - \ru Построенная оболочка.
                            \en The resultant shell. \~
  \result \ru Возвращает строитель оболочки.
          \en Returns the shell constructor. \~
  \ingroup Model_Creators
*/
// ---
MATH_FUNC (MbCreator *) CreateJalousie(       SPtr<MbFaceShell> &      initialShell,
                                        const MbeCopyMode              sameShell,
                                        const MbFace *                 face,
                                        const MbPlacement3D &          placement,
                                        const RPArray<MbLineSegment> & lineSegments,
                                        const MbJalousieValues &       parameters,
                                        const double                   thickness,
                                        const bool                     add,
                                        const MbSNameMaker &           nameMaker,
                                              MbResultType &           res,
                                              SPtr<MbFaceShell> &      resultShell );


#endif // __CR_STAMP_JALOUSIE_SOLID_H
