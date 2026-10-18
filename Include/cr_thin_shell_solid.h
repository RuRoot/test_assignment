////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Строитель эквидистантной оболочки.
         \en Constructor of an offset shell.
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_THIN_SHELL_SOLID_H
#define __CR_THIN_SHELL_SOLID_H


#include <creator.h>
#include <math_define.h>
#include <op_shell_parameter.h>
#include <op_swept_parameter.h>


//------------------------------------------------------------------------------
/** \brief \ru Строитель эквидистантной оболочки.
           \en Constructor of an offset shell. \~
  \details \ru Строитель эквидистантной оболочки предназначен для выполнения следующих операций:
    эквидистантная оболочка, тонкостенное тело, придание толщины. \n
    Условия построения эквидистантной оболочки: исходная оболочка не замкнута, конечная оболочка замкнута.
    Условия построения тонкостенного тела: исходная оболочка замкнута, конечная оболочка замкнута.
    Условия построения придания толщины: исходная оболочка незамкнута, конечная оболочка замкнута. \n
    Построение тонкостенного тела и придания толщины упрощенно можно описать следующим образом: \n
    у заданного тела удалим указанные грани, а оставшимся граням придадим конечную толщину. \n
    Придание конечной толщины граням достигнем путем построения к оставшейся после удаления указанных граней 
    незамкнутой оболочке эквидистантной оболочки и соединения этих незамкнутых оболочек частями удаленных граней
    (тонкостенное тело) или новыми гранями (придание толщины). \n
           \en Constructor of an offset shell is intended for performing the following operations:
    offset shell, thin-walled solid, thickening. \n
    Conditions of the offset shell construction: the source shell is not closed, the resultant shell is closed.
    Conditions of a thin-walled solid construction: the source shell is closed, the resultant shell is closed.
    Conditions of thickening construction: the source shell is not closed, the resultant shell is closed. \n
    Construction of a thin-walled solid and thickening can be simply described in the following way: \n
    delete the specified faces from the given solid and supply the remained faces with a finite thickness. \n
    Supplying the faces with a thickness is performed by construction the offset shell to the open shell 
    remained after deletion of the specified faces and by connection of these open shells with parts of deleted faces
    (thin-walled solid) or new faces (thickening). \n \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbShellSolid : public MbCreator {
protected :
  SweptValues              parameters;     ///< \ru Общее эквидистантное смещение от исходной оболочки и замкнутость результирующей оболочки. \en The common offset distance from the source shell and closedness of the resultant shell. 
  SArray<MbItemIndex>      outIndices;     ///< \ru Номера вскрываемых граней. \en Indices of faces to open. 
  SArray<MbItemThinValues> offIndParams;   ///< \ru Номера граней и их индивидуальные эквидистантные смещения. \en Indices of faces and their unique offset distances. 
  bool                     copyAttributes; ///< \ru Копировать атрибуты из исходных граней в эквидистантные. \en Copy attributes of initial faces to offset faces. \~

public :
  /// \ru Конструктор с общим эквидистантным смещением граней. \en Constructor with the common offset distance of the faces. 
  MbShellSolid( const SweptValues & p, SArray<MbItemIndex> & outInds,
                const MbSNameMaker & n, bool copyFaceAttrs );
  /// \ru Конструктор с индивидуальными эквидистантными смещениям граней. \en Constructor with the unique offset distance of the faces. 
  MbShellSolid( const SweptValues & p, SArray<MbItemIndex> & outInds, SArray<MbItemThinValues> & offIndPars,
                const MbSNameMaker & n, bool copyFaceAttrs );
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbShellSolid();
private :
  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  MbShellSolid( const MbShellSolid &, MbRegDuplicate * );
public :  
  // \ru Общие функции математического объекта \en Common functions of the mathematical object 
  MbeCreatorType  IsA() const override; // \ru Тип элемента \en A type of element
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию \en Create a copy
  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override;   // \ru Сдвиг \en Translation
  void        Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate around an axis

  void        GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of the object 
  MbePrompt   GetPropertyName() override; // \ru Выдать заголовок свойства объекта \en Get a name of object property

  bool        IsSame( const MbCreator &, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        SetEqual ( const MbCreator & ) override;       // \ru Сделать равным \en Make equal
  bool        IsSimilar( const MbCreator & ) const override; // \ru Являются ли объекты подобными \en Whether the objects are similar

  // \ru Общие функции твердого тела \en Common functions of solid  
  bool        CreateShell( MbFaceShell *& shell, MbeCopyMode sameShell,
                           RPArray<MbSpaceItem> * items = nullptr ) override; // \ru Построение \en Construction

  /// \ru Дать параметры. \en Get the parameters. 
  void        GetParameters(       SweptValues & params ) const { params = parameters; }
  /// \ru Установить параметры. \en Set the parameters. 
  void        SetParameters( const SweptValues & params )       { parameters = params; }

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbShellSolid )
OBVIOUS_PRIVATE_COPY( MbShellSolid )
}; // MbShellSolid

IMPL_PERSISTENT_OPS( MbShellSolid )


//------------------------------------------------------------------------------
/** \brief \ru Создать эквидистантную оболочку с общим эквидистантным смещением.
           \en Create an offset shell with the common offset distance. \~
  \details \ru Для указанной оболочки построить эквидистантную оболочку (тонкостенное тело, придание толщины),
    удалив указанные грани, построив эквидистантные грани для оставшихся граней, 
    и соединив две полученные незамкнутые оболочки частями удалённых граней или новыми гранями. \n
    Одновременно с построением оболочки функция создаёт её строитель.\n
           \en For the given shell construct an offset shell (thin-walled solid, thickening)
    by deletion the specified faces and construction the offset faces for the remained faces 
    and connecting two obtained open shells with parts of deleted faces or with new faces. \n
    The function simultaneously creates the shell and its constructor.\n \~ 
  \param[in]  solid         - \ru Исходная оболочка.
                              \en The source shell. \~
  \param[in]  sameShell     - \ru Режим копирования исходной оболочки.
                              \en Mode of copying the source shell. \~
  \param[in]  parameters    - \ru Общее эквидистантное смещение от исходной оболочки и замкнутость результирующей оболочки.
                              \en The common offset distance from the source shell and closedness of the resultant shell. \~
  \param[in]  outFaces      - \ru Вскрываемые грани.
                              \en Faces to open. \~
  \param[in]  names         - \ru Именователь операции.
                              \en An object defining names generation in the operation. \~
  \param[in]  copyFaceAttrs - \ru Копировать атрибуты из исходных граней в эквидистантные.
                              \en Copy attributes of initial faces to offset faces. \~
  \param[out] res           - \ru Код результата операции.
                              \en Operation result code. \~
  \param[out] shell         - \ru Результирующая оболочка.
                              \en The required shell. \~
  \result \ru Возвращает строитель, если операция была выполнена успешно.
          \en Returns the constructor if the operation has been successfully performed. \~
  \ingroup Model_Creators
*/
// ---
MATH_FUNC (MbCreator *) CreateThinShelling(       MbFaceShell *     solid, 
                                                  MbeCopyMode       sameShell, 
                                                  SweptValues &     parameters, 
                                                  RPArray<MbFace> & outFaces, 
                                            const MbSNameMaker &    names, // \ru Используется только для главного имени \en Used for the main name only. 
                                                  bool              copyFaceAttrs,
                                                  MbResultType &    res, 
                                                  MbFaceShell *&    shell );


//------------------------------------------------------------------------------
/** \brief \ru Создать эквидистантную оболочку с индивидуальными эквидистантными смещениями.
           \en Create an offset shell with unique offset distance. \~
  \details \ru Для указанной оболочки построить эквидистантную оболочку (тонкостенное тело, придание толщины),
    удалив указанные грани, построив эквидистантные грани для оставшихся граней, 
    и соединив две полученные незамкнутые оболочки частями удалённых граней или новыми гранями. \n
    Одновременно с построением оболочки функция создаёт её строитель.\n
           \en For the given shell construct an offset shell (thin-walled solid, thickening)
    by deletion the specified faces and construction the offset faces for the remained faces 
    and connecting two obtained open shells with parts of deleted faces or with new faces. \n
    The function simultaneously creates the shell and its constructor.\n \~ 
  \param[in]  solid         - \ru Исходная оболочка.
                              \en The source shell. \~
  \param[in]  sameShell     - \ru Режим копирования исходной оболочки.
                              \en Mode of copying the source shell. \~
  \param[in]  parameters    - \ru Общее эквидистантное смещение от исходной оболочки и замкнутость результирующей оболочки.
                              \en The common offset distance from the source shell and closedness of the resultant shell. \~
  \param[in]  outFaces      - \ru Вскрываемые грани.
                              \en Faces to open. \~
  \param[in]  offFaces      - \ru Грани с индивидуальными эквидистантными смещениям.
                              \en Faces with unique offset distance. \~
  \param[in]  offDists      - \ru Индивидуальные эквидистантные смещения.
                              \en Unique offset distances. \~
  \param[in]  names         - \ru Именователь операции.
                              \en An object defining names generation in the operation. \~
  \param[in]  copyFaceAttrs - \ru Копировать атрибуты из исходных граней в эквидистантные.
                              \en Copy attributes of initial faces to offset faces. \~
  \param[out] res           - \ru Код результата операции.
                              \en Operation result code. \~
  \param[out] shell         - \ru Результирующая оболочка.
                              \en The required shell. \~
  \result \ru Возвращает строитель, если операция была выполнена успешно.
          \en Returns the constructor if the operation has been successfully performed. \~
  \ingroup Model_Creators
*/
// ---
MATH_FUNC (MbCreator *) CreateThinShelling(       MbFaceShell *     solid, 
                                                  MbeCopyMode       sameShell, 
                                                  SweptValues &     parameters,
                                                  RPArray<MbFace> & outFaces, 
                                                  RPArray<MbFace> & offFaces, 
                                                  SArray<double> &  offDists,
                                            const MbSNameMaker &    names, // \ru Используется только для главного имени \en Used for the main name only. 
                                                  bool              copyFaceAttrs,
                                                  MbResultType &    res, 
                                                  MbFaceShell *&    shell );


//------------------------------------------------------------------------------
/** \brief \ru Построить эквидистантную оболочку.
           \en Construct an offset shell. \~
  \details \ru Для указанной оболочки построить эквидистантную оболочку (тонкостенное тело, придание толщины),
    удалив указанные грани, построив эквидистантные грани для оставшихся граней, 
    и соединив две полученные незамкнутые оболочки частями удалённых граней или новыми гранями. \n
           \en For the given shell construct an offset shell (thin-walled solid, thickening)
    by deletion the specified faces and construction the offset faces for the remained faces 
    and connecting two obtained open shells with parts of deleted faces or with new faces. \n \~
  \param[in]  solid         - \ru Исходная оболочка.
                              \en The source shell. \~
  \param[in]  sameShell     - \ru Режим копирования исходной оболочки.
                              \en Mode of copying the source shell. \~
  \param[in]  parameters    - \ru Общее эквидистантное смещение от исходной оболочки и замкнутость результирующей оболочки.
                              \en The common offset distance from the source shell and closedness of the resultant shell. \~
  \param[in]  outInds       - \ru Номера вскрываемых граней.
                              \en Indices of faces to open. \~
  \param[in]  offIndPars    - \ru Номера граней и их индивидуальные эквидистантные смещения.
                              \en Indices of faces and their unique offset distances. \~
  \param[in]  names         - \ru Именователь операции.
                              \en An object defining names generation in the operation. \~
  \param[in]  copyFaceAttrs - \ru Копировать атрибуты из исходных граней в эквидистантные.
                              \en Copy attributes of initial faces to offset faces. \~
  \param[out] res           - \ru Код результата операции.
                              \en Operation result code. \~
  \result \ru Возвращает построенную оболочку, если операция была выполнена успешно.
          \en Returns the constructed shell if the operation has been successfully performed. \~
  \ingroup Model_Creators
*/
// ---
MATH_FUNC (MbFaceShell *) MakeThinShell(       MbFaceShell &              solid,
                                               MbeCopyMode                sameShell,
                                         const SweptValues &              parameters,
                                               SArray<MbItemIndex>      & outInds,
                                               SArray<MbItemThinValues> & offIndPars,
                                         const MbSNameMaker &             names,
                                               bool                       copyFaceAttrs,
                                               MbResultType &             res );


#endif // __CR_THIN_SHELL_SOLID_H
