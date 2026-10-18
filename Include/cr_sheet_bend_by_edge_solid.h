////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Построение сгибов по рёбрам оболочки тела из листового материала.
         \en Construction of bends by edges of a shell of a solid from sheet material. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_SHEET_BEND_BY_EDGE_SOLID_H
#define __CR_SHEET_BEND_BY_EDGE_SOLID_H


#include <creator.h>
#include <sheet_metal_param.h>


//------------------------------------------------------------------------------
/** \brief \ru Строитель сгибов по рёбрам оболочки тела из листового материала.
           \en Constructor of bends by edges of a shell of a solid from sheet material. \~
  \details \ru Строитель сгибов по рёбрам оболочки тела из листового материала. \n 
    От заданных рёбер строятся сгибы с продолжением. 
    В зависимости от параметров операции они могут быть смещены от рёбер внутрь или наружу тела, 
    строиться от всей длины ребра или от его части, иметь уклон на сгибе и/или его продолжении, 
    расширение продолжения с каждой стороны. 
    Сгиб может быть построен с освобождением, а также с подрезкой сгибов,
    с которыми он стыкуется своими боковыми сторонами.
           \en Constructor of bends by edges of a shell of a solid from sheet material. \n 
    Bends with extensions are built from the given edges. 
    Depending on parameters of the operation they can be shifted from the edges inside or outside the solid, 
    they can be built from the whole length of the edge or from its part, they can have a slope at the bend and/or its extension, 
    an expansion of the extension from each side. 
    A bend can be constructed with release and also with trimming of the bends
    it meets with by its side boundaries. \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbBendsByEdgesSolid : public MbCreator {
  SArray<MbEdgeFacesIndexes> edgesIndices; ///< \ru Идентификаторы рёбер, по которым строятся сгибы. \en Identifiers of edges the bends are built by. 
  bool                       unbended;     ///< \ru Флаг построения сгиба в разогнутом виде. \en Flag of construction of a bend in unbent form. 
  MbBendByEdgeValues         parameters;   ///< \ru Параметры построения. \en Construction parameters. 
  RPArray<MbSMBendNames>     bendsParams;  ///< \ru Множество параметров для каждого формируемого сгиба. \en Set of parameters for each bend. 
  
public :
  MbBendsByEdgesSolid( const SArray<MbEdgeFacesIndexes> & edgesIndices,
                       const bool                         unbended,
                       const MbBendByEdgeValues &         params,
                       const RPArray<MbSMBendNames> &     bendsParams,
                       const MbSNameMaker &               nameMaker );
private:
  MbBendsByEdgesSolid( const MbBendsByEdgesSolid &, MbRegDuplicate * iReg );
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbBendsByEdgesSolid( const MbBendsByEdgesSolid & );
  
public:
  virtual ~MbBendsByEdgesSolid();
  
  // \ru Общие функции математического объекта \en Common functions of the mathematical object 
  
  MbeCreatorType  IsA()  const override; // \ru Тип элемента \en Type of element 
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию \en Create a copy

  bool        IsSame   ( const MbCreator & other, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        IsSimilar( const MbCreator & item ) const override; // \ru Являются ли объекты подобными? \en Determine whether an object is similar?
  bool        SetEqual ( const MbCreator & ) override; // \ru Сделать равным \en Make equal 
  void        Transform( const MbMatrix3D & matr, MbRegTransform * = nullptr ) override; // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move     ( const MbVector3D & to, MbRegTransform * = nullptr ) override; // \ru Сдвиг \en Translation
  void        Rotate   ( const MbAxis3D & axis, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate about an axis

  void        GetProperties( MbProperties & properties ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & properties ) override; // \ru Записать свойства объекта \en Set properties of the object 
  MbePrompt   GetPropertyName() override; // \ru Выдать заголовок свойства объекта \en Get a name of object property 

  
  // \ru Общие функции твердого тела \en Common functions of solid solid 
  
  bool        CreateShell( MbFaceShell *&         shell,
                           MbeCopyMode            sameShell, 
                           RPArray<MbSpaceItem> * items = nullptr ) override; // \ru Построение \en Construction

  // \ru Дать параметры. \en Get the parameters. 
  void        GetParameters(       MbBendByEdgeValues & params ) const { params = parameters; }
  // \ru Установить параметры. \en Set the parameters. 
  void        SetParameters( const MbBendByEdgeValues & params )       { parameters = params; }

private:
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
  MbBendsByEdgesSolid  & operator = ( const MbBendsByEdgesSolid & ); // \ru Не реализовано \en Not implemented 
  
  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbBendsByEdgesSolid )
}; // MbBendsByEdgesSolid

IMPL_PERSISTENT_OPS( MbBendsByEdgesSolid )


//------------------------------------------------------------------------------
/** \brief \ru Построить сгибы вдоль рёбер оболочки.
           \en Construct bends along edges of a shell. \~
  \details \ru Построить сгибы по рёбрам оболочки тела из листового материала. 
    От заданных рёбер строятся сгибы с продолжением. 
    В зависимости от параметров операции они могут быть смещены от рёбер внутрь или наружу тела, 
    строиться от всей длины ребра или от его части, иметь уклон на сгибе и/или его продолжении, 
    расширение продолжения с каждой стороны. 
    Сгиб может быть построен с освобождением, а также с подрезкой сгибов,
    с которыми он стыкуется своими боковыми сторонами.
    Одновременно с построением оболочки функция создаёт её строитель.\n
           \en Construct bends by edges of a shell of a solid from sheet material. 
    Bends with extensions are built from the given edges. 
    Depending on parameters of the operation they can be shifted from the edges inside or outside the solid, 
    they can be built from the whole length of the edge or from its part, they can have a slope at the bend and/or its extension, 
    an expansion of the extension from each side. 
    A bend can be constructed with release and also with trimming of the bends
    it meets with by its side boundaries.
    The function simultaneously creates the shell and its constructor.\n \~ 
  \deprecated \ru Метод устарел. \en The method is deprecated. \~
  \param[in]  initialShell - \ru Исходная оболочка.
                             \en The initial shell. \~
  \param[in]  sameShell  - \ru Режим копирования исходной оболочки.
                           \en Mode of copying the initial shell. \~
  \param[in]  edges - \ru Рёбра, по которым строятся сгибы.
                      \en Edges the bends are built along. \~
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
MATH_FUNC (MbCreator *) CreateBendsByEdges(       SPtr<MbFaceShell> &      initialShell,
                                            const MbeCopyMode              sameShell, 
                                            const RPArray<MbCurveEdge> &   edges,
                                            const bool                     unbended,
                                            const MbBendByEdgeValues &     parameters,
                                            const MbSNameMaker &           names,
                                                  RPArray<MbSMBendNames> & resultBends,
                                                  MbResultType &           res,
                                                  MbFaceShell *&           shell );


#endif // __CR_SHEET_BEND_BY_EDGE_SOLID_H
