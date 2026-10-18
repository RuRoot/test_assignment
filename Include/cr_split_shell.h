////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Строитель оболочки с разбиением граней.
         \en Construction of a shell with splitting of faces. 
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_SPLIT_SHELL_H
#define __CR_SPLIT_SHELL_H


#include <cur_contour.h>
#include <creator.h>
#include <op_boolean_flags.h>
#include <cr_split_data.h>


//------------------------------------------------------------------------------
/** \brief \ru Строитель оболочки с разбиением граней.
           \en Construction of a shell with splitting of faces. \~
  \details \ru Строитель оболочки с разбиением граней по указанным кривым на них. \n
           \en Constructor of a shell with splitting of faces by the specified curves on them. \n \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbSplitShell : public MbCreator {
protected:
  SArray<MbItemIndex> faceIndices; ///< \ru Идентификаторы разбиваемых граней. \en Identifiers of faces to split. 
  MbSplitData         splitItems;  ///< \ru Порождающие объекты линии разъема. \en Generating objects of parting lines. 
  bool                mergeFaces;  ///< \ru Сливать подобные грани (true). \en Whether to merge similar faces (true). 
  bool                mergeEdges;  ///< \ru Сливать подобные ребра (true). \en Whether to merge similar edges (true). 

public:
  MbSplitShell( const MbSplitData & spItems, bool sameItems, const SArray<MbItemIndex> & spFaceIndices, const MbMergingFlags & mf, const MbSNameMaker & n );
private :
  MbSplitShell( const MbSplitShell &, MbRegDuplicate * iReg );
public :
  virtual ~MbSplitShell();

  // \ru Общие функции математического объекта \en Common functions of the mathematical object 

  MbeCreatorType  IsA () const override; // \ru Тип элемента \en A type of element
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override;
  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override;               // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move     ( const MbVector3D &, MbRegTransform * = nullptr   ) override;               // \ru Сдвиг \en Translation
  void        Rotate   ( const MbAxis3D   &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate around an axis

  MbePrompt   GetPropertyName() override; // \ru Выдать заголовок свойства объекта \en Get a name of object property 
  void        GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of the object 
  void        GetBasisItems( RPArray<MbSpaceItem> & ) override; // \ru Дать базовые объекты \en Get the base objects 

  bool        IsSame( const MbCreator &, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal? 
  bool        SetEqual( const MbCreator & ) override;       // \ru Сделать равным \en Make equal
  bool        IsSimilar ( const MbCreator & ) const override; // \ru Являются ли объекты подобными \en Whether the objects are similar

  // \ru Общие функции твердого тела \en Common functions of solid 

  bool        CreateShell( MbFaceShell *& shell, MbeCopyMode sameShell, 
                           RPArray<MbSpaceItem> * items = nullptr ) override; // \ru Построение \en Construction

private: // \ru Не реализовано \en Not implemented 
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbSplitShell( const MbSplitShell & );
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
  void        operator = ( const MbSplitShell & );

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbSplitShell )
}; // MbSplitShell

IMPL_PERSISTENT_OPS( MbSplitShell )


//------------------------------------------------------------------------------
/** \brief \ru Построить оболочку с разбиением граней выдавливанием.
           \en Create a shell with faces splitting by extrusion. \~
  \details \ru Построить оболочку подразбиением граней поверхностями, 
    полученными выдавливанием контуров на плоскости XY локальной системы координат.\n
    Одновременно с построением оболочки функция создаёт её строитель.\n
           \en Create a shell by faces splitting by surfaces 
    obtained by extrusion of contours on XY plane of the local coordinate system.\n
    The function simultaneously creates the shell and its constructor.\n \~ 
  \param[in]  solid      - \ru Исходная оболочка.
                           \en The source shell. \~
  \param[in]  sameShell  - \ru Режим копирования исходной оболочки.
                           \en Mode of copying the source shell. \~
  \param[in]  splitPlace - \ru Локальная система координат.
                           \en A local coordinate system. \~
  \param[in]  splitType  - \ru Способ разбиения.
                           \en Method of splitting. \~
  \param[in]  splitContours - \ru Двумерные контуры на плоскости XY локальной системы координат.
                              \en Two-dimensional contours on XY plane of the local coordinate system. \~
  \param[in]  splitSame  - \ru Флаг копирования объектов.
                           \en Flag of objects' copying. \~
  \param[in]  selFaces   - \ru Разбиваемые грани.
                           \en Faces to split. \~
  \param[in]  operNames  - \ru Именователь операции.
                           \en An object defining names generation in the operation. \~
  \param[out] res        - \ru Код результата операции выдавливания.
                           \en The extrusion operation result code. \~
  \param[out] shell      - \ru Построенная оболочка.
                           \en The resultant shell. \~
  \result \ru Возвращает строитель оболочки.
          \en Returns the shell constructor. \~
  \ingroup Model_Creators
*/
// ---
MATH_FUNC (MbCreator *) CreateSplitSolid(       MbFaceShell        * solid, 
                                                MbeCopyMode          sameShell,
                                          const MbPlacement3D      & splitPlace, 
                                                MbeSenseValue        splitType, 
                                          const RPArray<MbContour> & splitContours, 
                                                bool                 splitSame,
                                                RPArray<MbFace>    & selFaces, 
                                          const MbMergingFlags     & mergingFlags,
                                          const MbSNameMaker       & operNames,
                                                MbResultType       & res, 
                                                MbFaceShell       *& shell );


//------------------------------------------------------------------------------
/** \brief \ru Построить оболочку с разбиением граней пространственными объектами.
           \en Create a shell with faces splitting by spatial objects. \~
  \details \ru Построить оболочку с разбиением граней пространственными кривыми, поверхностями и оболочками. \n
    Одновременно с построением оболочки функция создаёт её строитель.\n
           \en Create a shell with faces splitting by spatial curves, surfaces and shells. \n
    The function simultaneously creates the shell and its constructor.\n \~ 
  \param[in]  solid      - \ru Исходная оболочка.
                           \en The source shell. \~
  \param[in]  sameShell  - \ru Режим копирования исходной оболочки.
                           \en Mode of copying the source shell. \~
  \param[in]  splitItems - \ru Пространственные объекты.
                           \en Spatial objects. \~
  \param[in]  splitSame  - \ru Флаг копирования объектов.
                           \en Flag of objects' copying. \~
  \param[in]  selFaces   - \ru Разбиваемые грани.
                           \en Faces to split. \~
  \param[in]  operNames  - \ru Именователь операции.
                           \en An object defining names generation in the operation. \~
  \param[out] res        - \ru Код результата операции выдавливания.
                           \en The extrusion operation result code. \~
  \param[out] shell      - \ru Построенная оболочка.
                           \en The resultant shell. \~
  \result \ru Возвращает строитель оболочки.
          \en Returns the shell constructor. \~
  \ingroup Model_Creators
*/
// ---
MATH_FUNC (MbCreator *) CreateSplitSolid(       MbFaceShell          * solid, 
                                                MbeCopyMode            sameShell,
                                          const RPArray<MbSpaceItem> & splitItems, 
                                                bool                   splitSame,
                                                RPArray<MbFace>      & selFaces, 
                                          const MbMergingFlags       & mergingFlags,
                                          const MbSNameMaker         & operNames,
                                                MbResultType         & res, 
                                                MbFaceShell         *& shell );


#endif // __CR_SPLIT_SHELL_H
