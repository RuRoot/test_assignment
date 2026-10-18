////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Строитель оболочки из листового материала с буртиком.
         \en Constructor of a shell from sheet material with a bead.
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_STAMP_BEAD_SOLID_H
#define __CR_STAMP_BEAD_SOLID_H


#include <creator.h>
#include <sheet_metal_param.h>


class    MbPlacement3D;
class    MbCurveBoundedSurface;


//------------------------------------------------------------------------------
/** \brief \ru Строитель оболочки из листового материала с буртиком.
           \en Constructor of a shell from sheet material with a bead. \~
  \details \ru Строитель оболочки из листового материала с буртиком. \n 
    В зависимости от параметров может быть построен буртик:
    круглый - с образующей в виде дуги окружности, 
    V-образный - с образующей в виде дуги с касательными отрезками с каждой стороны или
    U-образный - с образующей в виде трёх отрезков со скруглениями или без них.
           \en Constructor of a shell from sheet material with a bead. \n 
    Subject to the parameters a bead can be constructed of the following type:
    circular - with generating curve in the form of circular arc, 
    V-shaped - with generating curve in the form of an arc with the tangent segments in each sides or
    U-shaped - with generating curve in the form of three segments with fillets or without them. \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbBeadSolid : public MbCreator {
  MbItemIndex             faceIndex;     ///< \ru Индекс грани, на которой строится буртик \en Index of face on which the bead is constructed.
  MbItemIndex             pairFaceIndex; ///< \ru Индекс грани парной к грани буртика. \en Index of the face which is pair to the bead face.
  MbCurveBoundedSurface * boundSurface;  ///< \ru Поверхность, границами которой надо подрезать буртик. \en Surface, by which bounds the bead is cutted.
  MbPlacement3D           placement;     ///< \ru Локальная система координат, в плоскости XY которй расположены направляющие буртика. \en The local coordinate system in XY plane of which the spine curves of the bead is located. 
  RPArray<MbContour>      contours;      ///< \ru Направляющие буртика. \en Spine curves of the bead. 
  SArray<MbCartPoint>     centers;       ///< \ru Центры сферических штамповок. \en The spherical stamps' centers.
  MbBeadValues            parameters;    ///< \ru Параметры операции. \en The operation parameters.
  double                  thickness;     ///< \ru Толщина листа. \en The thickness of the sheet metal.
  bool                    add;           ///< \ru Создавать добавляемую или вычитаемую часть буртика. \en Create additional or subtructional part of a bead.

public :
  MbBeadSolid( const MbItemIndex &           faceIndex,
               const MbItemIndex &           pairFaceIndex,
               const MbCurveBoundedSurface * boundSurface,
               const MbPlacement3D &         placement,
               const RPArray<MbContour> &    contours,
               const SArray<MbCartPoint> &   centers,
               const MbBeadValues &          params,
               const double                  thickness,
               const bool                    add,
               const MbSNameMaker &          names );
private:
  MbBeadSolid( const MbBeadSolid &, MbRegDuplicate * iReg ); 
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbBeadSolid( const MbBeadSolid & ); 

public:
  virtual ~MbBeadSolid();

  // \ru Общие функции математического объекта \en Common functions of the mathematical object 

  MbeCreatorType  IsA  ()  const override; // \ru Тип элемента \en A type of element
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию \en Create a copy

  bool        IsSame   ( const MbCreator & other, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        IsSimilar( const MbCreator & item ) const override; // \ru Являются ли объекты подобными? \en Determine whether an object is similar?
  bool        SetEqual ( const MbCreator & ) override; // \ru Сделать равным \en Make equal 

  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override; // \ru Сдвиг \en Translation
  void        Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate around an axis

  MbePrompt   GetPropertyName() override; // \ru Выдать заголовок свойства объекта \en Get a name of object property 
  void        GetProperties  ( MbProperties &properties ) override; // \ru Выдать свойства объекта \en Get properties of the object
  void        SetProperties  ( const MbProperties &properties ) override; // \ru Записать свойства объекта \en Set properties of the object
  void        GetBasisItems ( RPArray<MbSpaceItem>   & ) override; // \ru Дать базовые объекты \en Get the base objects
  void        GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  // \ru Общие функции твердого тела \en Common functions of solid  

  bool        CreateShell( MbFaceShell *& shell, MbeCopyMode sameShell, 
                           RPArray<MbSpaceItem> * items = nullptr ) override; // \ru Построение \en Construction

  // \ru Дать параметры. \en Get the parameters. 
  void        GetParameters(       MbBeadValues & params ) const { params = parameters; }
  // \ru Установить параметры. \en Set the parameters. 
  void        SetParameters( const MbBeadValues & params )       { parameters = params; }

private:
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
  MbBeadSolid  &      operator = ( const MbBeadSolid & );

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbBeadSolid )
}; // MbBeadSolid

IMPL_PERSISTENT_OPS( MbBeadSolid )


//------------------------------------------------------------------------------
/** \brief \ru Построить оболочку из листового материала с буртиком.
           \en Construct a shell from sheet material with a bead. \~
  \details \ru Построить оболочку из листового материала с буртиком,
    который может быть круглым - с образующей в виде дуги окружности, 
    V-образным - с образующей в виде дуги с касательными отрезками с каждой стороны или 
    U-образный - с образующей в виде трёх отрезков со скруглениями или без них.
    Одновременно с построением оболочки функция создаёт её строитель.\n
           \en Create a shell from sheet material with a bead
    which can be circular - with generating curve in the form of circular arc, 
    V-shaped - with generating curve in the form of an arc with the tangent segments in each sides or 
    U-shaped - with generating curve in the form of three segments with fillets or without them.
    The function simultaneously creates the shell and its constructor.\n \~ 
  \param[in]  initialShell      - \ru Исходная оболочка.
                                  \en The source shell. \~
  \param[in]  sameShell  - \ru Режим копирования исходной оболочки.
                           \en Mode of copying the source shell. \~
  \param[in]  face       - \ru Грань буртика.
                           \en The bead face. \~
  \param[in]  placement  - \ru Локальная система координат, в плоскости XY которй расположены контуры буртика.
                           \en The local coordinate system in XY plane of which the contours of the bead is located. \~
  \param[in]  contours   - \ru Контуры буртика.
                           \en The bead contours. \~
  \param[in]  parameters - \ru Параметры буртика.
                           \en The bead parameters. \~
  \param[in]  thickness  - \ru Толщина пластины.
                           \en The thickness of the plate. \~
  \param[in]  add        - \ru Создавать добавляемую компоненту буртика.
                           \en To create the added part of the bead. \~
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
MATH_FUNC (MbCreator *) CreateBead(       SPtr<MbFaceShell> &   initialShell,
                                    const MbeCopyMode           sameShell,
                                    const MbFace *              face,
                                    const MbPlacement3D &       placement,
                                    const RPArray<MbContour> &  contours,
                                    const SArray<MbCartPoint> & centers,
                                    const MbBeadValues &        parameters,
                                    const double                thickness,
                                    const bool                  add,
                                    const MbSNameMaker &        nameMaker,
                                          MbResultType &        res,
                                          SPtr<MbFaceShell> &   resultShell );


#endif // __CR_STAMP_BEAD_SOLID_H
