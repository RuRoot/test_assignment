////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Построение комбинированного сгиба.
         \en A composite bend construction. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_SHEET_JOINT_BEND_SOLID_H
#define __CR_SHEET_JOINT_BEND_SOLID_H


#include <cur_contour.h>
#include <creator.h>
#include <sheet_metal_param.h>


//------------------------------------------------------------------------------
/** \brief \ru Строитель комбинированного сгиба.
           \en Construction of a composite bend. \~
  \details \ru Строитель сгибов, заданных эскизом, по рёбрам оболочки тела из листового материала. \n 
    По заданному контуру, состоящему из отрезков и дуг, строит листовое тело, формируя сгибы на месте
    дуг и между отрезками по параметрам, заданным в bendsParams, и присоединяет его к каждому ребру,
    указанному в edgesIndices.
           \en Construction of bends specified by a sketch, along edges of solid's shell from sheet material. \n 
    From the given contour consisting of segments and arcs it constructs a sheet solid with forming bends at
    the arcs and between segments using parameters specified in bendsParams, and attaches it to each edge
    specified in edgesIndices. \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbJointBendSolid : public MbCreator {
  MbPlacement3D                     placement;    ///< \ru Локальная система координат образующего контура. \en The local coordinate system of the generating contour. 
  MbContour                         contour;      ///< \ru Образующий контур. \en Generating contour. 
  SArray<MbEdgeFacesIndexes>        edgesIndices; ///< \ru Идентификаторы направляющих рёбер. \en Identifiers of guide edges. 
  bool                              unbended;     ///< \ru Флаг построения сгибов в разогнутом состоянии. \en Flag of construction of bends in unbent form. 
  MbJointBendValues                 parameters;   ///< \ru Параметры операции. \en The operation parameters. 
  RPArray< RPArray<MbSMBendNames> > bendsParams;  ///< \ru Множество параметров для каждого формируемого сгиба. \en Set of parameters for each bend. 

public :
  MbJointBendSolid( const MbPlacement3D &                     placement,
                    const MbContour &                         contour,
                    const SArray<MbEdgeFacesIndexes> &        edgesIndices,
                    const bool                                unbended,
                    const MbJointBendValues &                 parameters,
                    const RPArray< RPArray<MbSMBendNames> > & bendsParams,
                    const MbSNameMaker &                      nameMaker );

private:
  MbJointBendSolid( const MbJointBendSolid &, MbRegDuplicate * iReg );
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbJointBendSolid( const MbJointBendSolid & );

public:
  virtual ~MbJointBendSolid();
  
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
  void        GetParameters(       MbJointBendValues & params ) const { params = parameters; }
  // \ru Установить параметры. \en Set the parameters. 
  void        SetParameters( const MbJointBendValues & params )       { parameters = params; }
  
private:
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
  MbJointBendSolid  & operator = ( const MbJointBendSolid & ); // \ru Не реализовано \en Not implemented 
  
  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbJointBendSolid )
}; // MbJointBendSolid

IMPL_PERSISTENT_OPS( MbJointBendSolid )


//------------------------------------------------------------------------------
/** \brief \ru Построить комбинированные сгибы.
           \en Construct composite bends. \~
  \details \ru Построить сгибы, заданные эскизом, по рёбрам оболочки тела из листового материала. 
    По заданному контуру, состоящему из отрезков и дуг, строит листовое тело, формируя сгибы на месте
    дуг и между отрезками по параметрам, заданным в bendsParams, и присоединяет его к каждому ребру,
    указанному в edgesIndices. 
    Одновременно с построением оболочки функция создаёт её строитель.\n
           \en Construct bends specified by a sketch, along edges of solid's shell from sheet material. 
    From the given contour consisting of segments and arcs it constructs a sheet solid with forming bends at
    the arcs and between segments using parameters specified in bendsParams, and attaches it to each edge
    specified in edgesIndices. 
    The function simultaneously creates the shell and its constructor.\n \~ 
  \param[in]  initialShell - \ru Исходная оболочка.
                             \en The initial shell. \~
  \param[in]  sameShell  - \ru Режим копирования исходной оболочки.
                           \en Mode of copying the initial shell. \~
  \param[in]  placement  - \ru Локальная система координат, в плоскости XY которй расположен контур сгиба.
                           \en A local coordinate system the bend contour is located in XY plane of. \~
  \param[in]  contours   - \ru Контур сгиба.
                           \en The bend contour. \~
  \param[in]  edges - \ru Рёбра, по которым строятся сгибы.
                      \en Edges the bends are built along. \~
  \param[in]  unbended - \ru Флаг построения сгиба в разогнутом виде.
                         \en Flag of construction of a bend in unbent form. \~
  \param[in]  parameters - \ru Параметры построения.
                           \en Parameters of shell creation. \~
  \param[in]  nameMaker - \ru Именователь граней.
                          \en An object for naming faces. \~
  \param[in]  resultBends - \ru Имена построенных сгибов.
                            \en Constructed bends names. \~
  \param[out] res - \ru Код результата операции.
                    \en Operation result code. \~
  \param[out] shell - \ru Построенная оболочка.
                      \en The resultant shell. \~
  \result \ru Возвращает строитель оболочки.
          \en Returns the shell constructor. \~
  \ingroup Model_Creators
*/
// ---
MATH_FUNC (MbCreator *) CreateJointBend(       c3d::ShellSPtr &                    initialShell,
                                         const MbeCopyMode                         sameShell,
                                         const MbPlacement3D &                     placement,
                                         const MbContour &                         contour,
                                         const RPArray<MbCurveEdge> &              edges,
                                         const bool                                unbended,
                                         const MbJointBendValues &                 parameters,
                                         const MbSNameMaker &                      nameMaker,
                                               RPArray< RPArray<MbSMBendNames> > & resultBends,
                                               MbResultType &                      res,
                                               MbFaceShell *&                      shell );


#endif // __CR_SHEET_JOINT_BEND_SOLID_H
