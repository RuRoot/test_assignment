////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Строитель оболочки из листового материала сферической штамповкой.
         \en Constructor of a shell from the sheet material with spherical stamping.
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_STAMP_SPHERICAL_SOLID_H
#define __CR_STAMP_SPHERICAL_SOLID_H


#include <mb_placement3d.h>
#include <creator.h>
#include <sheet_metal_param.h>


class MbCurveBoundedSurface;


//------------------------------------------------------------------------------
/** \brief \ru Строитель оболочки из листового материала сферической штамповкой.
           \en Constructor of a shell from the sheet material with spherical stamping. \~
  \details \ru Строитель оболочки из листового материала сферической штамповкой. \n 
    Строится только закрытая штамповка. \n 
           \en Constructor of a shell from the sheet material by spherical stamping. \n 
    Stamping of closed type only are constructed. \n 
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbSphericalStampSolid : public MbCreator {
private:
  MbItemIndex             faceIndex;     ///< \ru Индекс грани, на которой строится штамповка. \en Index of the face the stamping is constructed on. 
  MbItemIndex             pairFaceIndex; ///< \ru Индекс парной грани. \en Index of the pair face. 
  MbCurveBoundedSurface * boundSurface;  ///< \ru Поверхность, границами которой надо подрезать штамповку. \en Surface, by which bounds the stamp is cutted.
  MbPlacement3D           placement;     ///< \ru Локальная система координат контура штамповки. \en The local coordinate system of contour of stamping. 
  MbStampingValues        parameters;    ///< \ru Параметры штамповки. \en Stamping parameters.
  double                  thickness;     ///< \ru Толщина листа. \en Thickness of the plate.
  bool                    add;           ///< \ru Создавать добавляемую или вычитаемую часть штамповки. \en Create additional or subtructional part of a stamp.
  MbCartPoint             center;        ///< \ru Центр донышка штамповки. \en Center of the bottom of the stamping. 

public :
  MbSphericalStampSolid( const MbItemIndex &           faceIndex,
                         const MbItemIndex &           pairFaceIndex, 
                         const MbCurveBoundedSurface * boundSurface,
                         const MbPlacement3D &         placement,
                         const MbStampingValues &      params,
                         const double                  thickness,
                         const bool                    add,
                         const MbCartPoint &           center,
                         const MbSNameMaker &          names );
private:
  MbSphericalStampSolid( const MbSphericalStampSolid &, MbRegDuplicate * iReg );
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbSphericalStampSolid( const MbSphericalStampSolid & );

public:
  virtual ~MbSphericalStampSolid();

  // \ru Общие функции математического объекта \en Common functions of the mathematical object 

  MbeCreatorType  IsA()  const override; // \ru Тип элемента \en A type of element 
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

  bool        CreateShell( MbFaceShell *&shell, MbeCopyMode sameShell,
                           RPArray <MbSpaceItem> *items = nullptr ) override; // \ru Построение \en Construction

  // \ru Дать параметры. \en Get the parameters. 
  void        GetParameters(       MbStampingValues & params ) const { params = parameters; }
  // \ru Установить параметры. \en Set the parameters. 
  void        SetParameters( const MbStampingValues & params )       { parameters = params; }

private:
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
  MbSphericalStampSolid & operator = ( const MbSphericalStampSolid & ); // \ru Не реализовано \en Not implemented 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbSphericalStampSolid )
}; // MbSphericalStampSolid

IMPL_PERSISTENT_OPS( MbSphericalStampSolid )


//------------------------------------------------------------------------------
/** \brief \ru Построить оболочку из листового материала со сферической штамповкой.
           \en Construct a shell form sheet material by spherical stamping. \~
  \details \ru На базе исходной оболочки из листового материала построить оболочку методом сферической штамповки или части сферической штамповки без исходного листового тела. \n
    Одновременно с построением оболочки функция создаёт её строитель.\n
           \en A shell is to be constructed on the basis of the source shell by the method of spherical stamping or parts of spherical stamp without the basis sheet solid. \n
    The function simultaneously creates the shell and its constructor.\n \~ 
  \param[in]  solid      - \ru Исходная оболочка.
                           \en The source shell. \~
  \param[in]  sameShell  - \ru Режим копирования исходной оболочки.
                           \en Mode of copying the source shell. \~
  \param[in]  params     - \ru Параметры штамповки.
                           \en The parameters of stamping. \~
  \param[in]  add        - \ru Какую часть сферической штамповки создавать.
                           \en Which part of the spherical stamp to create.
  \param[out] res        - \ru Код результата операции выдавливания.
                           \en The extrusion operation result code. \~
  \param[out] shell      - \ru Построенная оболочка.
                           \en The resultant shell. \~
  \result \ru Возвращает строитель оболочки.
          \en Returns the shell constructor. \~
  \ingroup Model_Creators
*/
// ---
MATH_FUNC ( c3d::CreatorSPtr ) CreateSphericalStamp(       SPtr<MbFaceShell> &           solid,  
                                                     const MbeCopyMode                   sameShell,
                                                     const MbSphericalStampPartsParams & params,
                                                     const bool                          add,
                                                           MbResultType &                res,
                                                           SPtr<MbFaceShell> &           shell );


#endif // __CR_STAMP_SPHERICAL_SOLID_H

