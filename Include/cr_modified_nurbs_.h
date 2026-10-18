////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Строитель оболочки c деформируемыми гранями.
         \en Constructor of a shell with deformable faces.
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_MODIFIED_NURBS_H
#define __CR_MODIFIED_NURBS_H


#include <creator.h>
#include <op_shell_parameter.h>


//------------------------------------------------------------------------------
/** \brief \ru Строитель оболочки c деформируемыми гранями.
           \en Constructor of a shell with deformable faces. \~
  \details \ru Строитель оболочки, выполняющий замену поверхностей указанных граней деформируемыми поверхностями. \n
           \en Constructor of a shell performing replacement of the surfaces of the specified faces with deformable surfaces. \n \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbModifiedNurbsItem : public MbCreator {
protected: 
  NurbsValues         parameters;  ///< \ru Параметры модифицированных поверхностей. \en Parameters of modified surfaces. 
  SArray<MbItemIndex> itemIndices; ///< \ru Идентификаторы модифицируемых граней. \en Identifiers of faces being modified. 
  RPArray<MbSurface>  surfaces;    ///< \ru Множество поверхностей модифицированных граней. \en A set of surfaces of the modified faces. 

public: // \ru конструктор по параметрам \en constructor by parameters 
  MbModifiedNurbsItem( const NurbsValues & p, const SArray<MbItemIndex> & faces, 
                       RPArray<MbSurface> & surfs, const MbSNameMaker & names );
private: // \ru конструктор дублирующий \en duplication constructor 
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbModifiedNurbsItem( const MbModifiedNurbsItem & init ); 
  MbModifiedNurbsItem( const MbModifiedNurbsItem & init, MbRegDuplicate * ireg );

public: // \ru деструктор \en destructor 
  virtual ~MbModifiedNurbsItem();

public: // \ru Общие функции математического объекта \en Common functions of the mathematical object 
  MbeCreatorType  IsA() const override; // \ru Тип элемента \en A type of element 
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru сделать копию \en create a copy
  bool        IsSame( const MbCreator & other, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal? 
  bool        SetEqual ( const MbCreator & ) override; // \ru сделать равным \en make equal 
  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать по матрице \en Transform according to the matrix
  void        Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override; // \ru сдвиг по вектору \en translation by a vector
  void        Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate around an axis

  MbePrompt   GetPropertyName() override; // \ru выдать заголовок свойства объекта \en get a name of object property 
  void        GetProperties( MbProperties & properties ) override; // \ru выдать свойства объекта \en get properties of the object 
  void        SetProperties( const MbProperties & properties ) override; // \ru записать свойства объекта \en set properties of the object 
  void        GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  /// \ru Построение оболочки. \en creation of a shell 
  bool        CreateShell( MbFaceShell *& shell, MbeCopyMode sameShell, 
                                   RPArray<MbSpaceItem> * items = nullptr ) override;
  void        Refresh( MbFaceShell & outer ) override; ///< \ru обновить форму оболочки \en update shape of the shell
  // \ru Выдать базовые объекты. \en Get basis objects.
  void        GetBasisItems( RPArray<MbSpaceItem> & s ) override;

          // \ru Дать параметры. \en Get the parameters. 
          void        GetParameters(       NurbsValues & params ) const { params = parameters; }
          // \ru Установить параметры. \en Set the parameters. 
          void        SetParameters( const NurbsValues & params )       { parameters = params; }

private:
          // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
          void        operator = ( const MbModifiedNurbsItem & ); 
          void        SurfacesFree(); // \ru Удалить поверхности \en Delete the surfaces 
          void        SurfacesAddRef(); // \ru Учесть поверхности \en Consider the surfaces 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbModifiedNurbsItem )
};

IMPL_PERSISTENT_OPS( MbModifiedNurbsItem )

//------------------------------------------------------------------------------
/** \brief \ru Модификатор оболочки c деформируемой гранью.
           \en Modifier of a shell with a deformable face. \~
  \details \ru Модификатор оболочки выполняет деформацию поверхности указанной грани.
    Указанная грань должна быть дефолрмируемой. \n
           \en Modifier of a shell performs deformation of a surface of the specified face.
    The specified face should be deformable. \n \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbNurbsModification : public MbCreator {
protected:
  MbItemIndex  faceIndex;   ///< \ru Идентификатор деформируемой грани. \en Identifier of the deformable face. 
  MbSurface *  faceSurface; ///< \ru Поверхность деформируемой грани. \en Surface of the deformable face. 
  Array2<bool> fixedPoints; ///< \ru Матрица положений неизменяемых контрольных точек модифицируемой поверхности. \en Matrix of positions of the invariant control points of the modifiable surface. 

public: // \ru конструктор по параметрам \en constructor by parameters 
  MbNurbsModification( const MbItemIndex & index, MbSurface & fSurface, Array2<bool> & fPoints,
                       const MbSNameMaker & names );
private: // \ru конструктор дублирующий \en duplication constructor 
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbNurbsModification( const MbNurbsModification & init ); 
  MbNurbsModification( const MbNurbsModification & init, MbRegDuplicate * ireg );

public: // \ru деструктор \en destructor 
  virtual ~MbNurbsModification();

public: // \ru Общие функции математического объекта \en Common functions of the mathematical object 
  MbeCreatorType  IsA() const override; // \ru Тип элемента \en A type of element 
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru сделать копию \en create a copy
  bool        IsSame( const MbCreator & other, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal? 
  bool        SetEqual ( const MbCreator & ) override; // \ru сделать равным \en make equal 
  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать по матрице \en Transform according to the matrix
  void        Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override; // \ru сдвиг по вектору \en translation by a vector
  void        Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate around an axis

  MbePrompt   GetPropertyName() override; // \ru выдать заголовок свойства объекта \en get a name of object property 
  void        GetProperties( MbProperties & properties ) override; // \ru выдать свойства объекта \en get properties of the object 
  void        SetProperties( const MbProperties & properties ) override; // \ru записать свойства объекта \en set properties of the object 
  void        GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  /// \ru построение оболочки \en creation of a shell 
  bool        CreateShell( MbFaceShell *& shell, MbeCopyMode sameShell, 
                                   RPArray<MbSpaceItem> * items = nullptr ) override;
  void        Refresh( MbFaceShell & outer ) override; ///< \ru обновить форму оболочки \en update shape of the shell
  // \ru Выдать базовые объекты. \en Get basis objects.
  void        GetBasisItems( RPArray<MbSpaceItem> & s ) override; 

private:
          // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
          void        operator = ( const MbNurbsModification & ); // \ru не реализован!!! \en not implemented!!! 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbNurbsModification )
};

IMPL_PERSISTENT_OPS( MbNurbsModification )

//------------------------------------------------------------------------------
/** \brief \ru Построить оболочку c деформируемыми гранями.
           \en Construct a shell with deformable faces. \~
  \details \ru Построить оболочку c заменой указанных граней исходной оболочки деформируемыми гранями. 
    Поверхности выбранных граней аппроксимируются NURBS поверхностями или 
    деформируемыми поверхностями для последующего редактирования.
    Одновременно с построением оболочки функция создаёт её строитель.\n
           \en Construct a shell with replacement of the specified faces of the source shell with deformable faces. 
    Surfaces of the selected faces are approximated with NURBS surfaces or 
    deformable surfaces for the further editing.
    The function simultaneously creates the shell and its constructor.\n \~ 
  \param[in]  outer - \ru Исходная оболочка.
                      \en The source shell. \~
  \param[in]  sameShell - \ru Режим копирования исходной оболочки.
                          \en Mode of copying the source shell. \~
  \param[in]  parameters - \ru Параметры модификации.
                           \en Parameters of the modification. \~
  \param[in]  faces - \ru Изменяемые грани тела.
                      \en Faces to be modified. \~
  \param[in]  names - \ru Именователь операции.
                      \en An object defining names generation in the operation. \~
  \param[out] res   - \ru Код результата операции выдавливания.
                      \en The extrusion operation result code. \~
  \param[out] shell - \ru Построенная оболочка.
                      \en The resultant shell. \~
  \result \ru Возвращает строитель.
          \en Returns the constructor. \~
  \ingroup Model_Creators
*/
// ---
MATH_FUNC (MbCreator *) CreateModifiedNurbsItem( MbFaceShell * outer, 
                                                 MbeCopyMode sameShell, 
                                                 const NurbsValues & parameters, 
                                                 const RPArray<MbFace> & faces, 
                                                 const MbSNameMaker & names, 
                                                 MbResultType & res,
                                                 MbFaceShell *& shell );


//------------------------------------------------------------------------------
/** \brief \ru Построить оболочку, в которой деформирована указанная грань.
           \en Construct a shell in which the specified face is deformed. \~
  \details \ru Построить оболочку, в которой деформирована указанная грань путём 
    подстановки контрольных точек присланной NURBS-поверхности с фиксацией указанных точек.\n
    Одновременно с построением оболочки функция создаёт её строитель.\n
           \en Construct a shell in which the specified face is deformed by 
    replacement of the control points of the given NURBS surface with fixing the specified points.\n
    The function simultaneously creates the shell and its constructor.\n \~ 
  \param[in]  outer       - \ru Исходная оболочка.
                            \en The source shell. \~
  \param[in]  sameShell   - \ru Режим копирования исходной оболочки.
                            \en Mode of copying the source shell. \~
  \param[in]  face        - \ru Деформируемая грань оболочки.
                            \en Deformable face of the shell. \~
  \param[in]  faceSurface - \ru Новая деформируемая поверхность для грани.
                            \en The new deformable surface of the face. \~
  \param[in]  fixedPoints - \ru Матрица положений неизменяемых контрольных точек деформируемой поверхности (false).
                            \en Matrix of positions of invariant control points of the deformable surface (false). \~
  \param[in]  names       - \ru Именователь операции.
                            \en An object defining names generation in the operation. \~
  \param[out] res         - \ru Код результата операции выдавливания.
                            \en The extrusion operation result code. \~
  \param[out] shell       - \ru Построенная оболочка.
                            \en The resultant shell. \~
  \result \ru Возвращает строитель оболочки.
          \en Returns the shell constructor. \~
  \ingroup Model_Creators
*/
// ---
MATH_FUNC (MbCreator *) CreateNurbsModification( MbFaceShell * outer, 
                                                 MbeCopyMode sameShell, 
                                                 MbFace * face,
                                                 MbSurface & faceSurface, 
                                                 Array2<bool> & fixedPoints,
                                                 const MbSNameMaker & names, 
                                                 MbResultType & res,
                                                 MbFaceShell *& shell );


#endif // __CR_MODIFIED_NURBS_H
