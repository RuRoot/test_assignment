////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Построение оболочки из листового материала, согнутого вдоль отрезка.
         \en Construction of a shell from sheet material bent along a segment. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_SHEET_BEND_OVER_SEG_SOLID_H
#define __CR_SHEET_BEND_OVER_SEG_SOLID_H


#include <templ_ss_array.h>
#include <creator.h>
#include <sheet_metal_param.h>


//------------------------------------------------------------------------------
/** \brief \ru Строитель оболочки из листового материала, согнутой вдоль отрезка.
           \en Constructor of a shell from sheet material bent along a segment. \~
  \details \ru Строитель оболочки из листового материала, согнутой слева или справа от отрезка,
    либо указанных граней, либо, в случае отсутствия таковых, всех подходящих для сгиба граней. \n
           \en Constructor of a shell from sheet material bent to the left or to the right from a segment
    or from the specified faces or, if they are absent, from all the faces appropriate for bending. \n \~ 
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbBendOverSegSolid : public MbCreator {
  SArray<MbItemIndex> bendingFacesIndices; ///< \ru Идентификаторы указанных для сгиба граней (сортированы и не повторяются). \en Identifiers of faces given for the bend (sorted and not duplicated). 
  MbCurve3D *         curve;               ///< \ru Линия по которой гнуть. \en Line along which to bend. 
  bool                unbended;            ///< \ru Флаг построения сгиба в разогнутом состоянии. \en Flag of construction of a bend in unbent form. 
  MbBendOverSegValues parameters;          ///< \ru Параметры операции. \en The operation parameters. 

public :
  MbBendOverSegSolid( const SArray<MbItemIndex> & bendingFacesIndices,
                            MbCurve3D &           curve,
                      const bool                  unbended,
                      const MbBendOverSegValues & pars,
                      const MbSNameMaker &        names );
private:
  MbBendOverSegSolid( const MbBendOverSegSolid &, MbRegDuplicate *ireg );
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbBendOverSegSolid( const MbBendOverSegSolid & );

public:
  virtual ~MbBendOverSegSolid();

  // \ru Общие функции математического объекта \en Common functions of the mathematical object 

  MbeCreatorType  IsA()  const override; // \ru Тип элемента \en Type of element 
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию \en Create a copy

  bool        IsSame   ( const MbCreator & other, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        IsSimilar( const MbCreator & item ) const override; // \ru Являются ли объекты подобными? \en Determine whether an object is similar?
  bool        SetEqual ( const MbCreator & ) override; // \ru Сделать равным \en Make equal 

  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override; // \ru Сдвиг \en Translation
  void        Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate about an axis

  MbePrompt   GetPropertyName() override; // \ru Выдать заголовок свойства объекта \en Get a name of object property 
  void        GetProperties( MbProperties & properties ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & properties ) override; // \ru Записать свойства объекта \en Set properties of the object 
  void        GetBasisItems ( RPArray<MbSpaceItem> & ) override; // \ru Дать базовые объекты \en Get the basis objects 
  void        GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.
  
  // \ru Общие функции твердого тела \en Common functions of solid solid 

  bool        CreateShell( MbFaceShell *& shell, MbeCopyMode sameShell,
                           RPArray<MbSpaceItem> * items = nullptr ) override; // \ru Построение \en Construction

  // \ru Дать параметры. \en Get the parameters. 
  void        GetParameters(       MbBendOverSegValues & params ) const { params = parameters; }
  // \ru Установить параметры. \en Set the parameters. 
  void        SetParameters( const MbBendOverSegValues & params )       { parameters = params; }

private:
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
  MbBendOverSegSolid  & operator = ( const MbBendOverSegSolid & );

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbBendOverSegSolid )
}; // MbBendOverSegSolid

IMPL_PERSISTENT_OPS( MbBendOverSegSolid )


//------------------------------------------------------------------------------
/** \brief \ru Построить оболочку из листового материала, согнутую вдоль отрезка.
           \en Create a shell from sheet material bent along a segment. \~
  \details \ru Построить оболочку из листового материала, согнутую слева или справа от отрезка,
    либо указанных граней, либо, в случае отсутствия таковых, всех подходящих для сгиба граней. \n 
    Одновременно с построением оболочки функция создаёт её строитель.\n
           \en Construct a shell from sheet material bent to the left and to the right from a segment
    or from the specified faces or, if they are absent, from all the faces appropriate for bending. \n 
    The function simultaneously creates the shell and its constructor.\n \~ 
  \param[in]  initialShell - \ru Исходная оболочка.
                             \en The initial shell. \~
  \param[in]  sameShell  - \ru Режим копирования исходной оболочки.
                           \en Mode of copying the initial shell. \~
  \param[in]  bendingFaces - \ru Грани, которые гнуть.
                             \en Faces to bend. \~
  \param[in]  curve - \ru Кривая, по которой сгибать.
                      \en A curve along which to bend. \~
  \param[in]  unbended - \ru Флаг построения сгиба в разогнутом виде.
                         \en Flag of construction of a bend in unbent form. \~
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
MATH_FUNC (MbCreator *) CreateBendOverSegment(       SPtr<MbFaceShell> &   initialShell,
                                                     MbeCopyMode           sameShell,
                                               const RPArray<MbFace> &     bendingFaces,
                                                     MbCurve3D &           curve,
                                               const bool                  unbended,
                                               const MbBendOverSegValues & parameters,
                                               const MbSNameMaker &        names,
                                                     MbResultType &        res,
                                                     MbFaceShell *&        shell );


#endif // __CR_SHEET_BEND_OVER_SEG_SOLID_H
