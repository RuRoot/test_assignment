////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Строитель булевой операции.
         \en Boolean operation constructor.
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_BOOLEAN_SOLID_H
#define __CR_BOOLEAN_SOLID_H


#include <creator.h>


class   MbSolid;
class   MbBooleanOperationParams;
struct  MbBooleanFlags;


//------------------------------------------------------------------------------
/** \brief \ru Строитель булевой операции.
           \en Boolean operation constructor. \~
  \details \ru Строитель булевой операции выполняет операции объединения, пересечения и вычитания множеств точек двух тел. \n.
           \en The Boolean operation constructor performs union, intersection and subtraction operations for sets of points of two solids. \n. \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbBooleanSolid : public MbCreator {
protected :
  c3d::CreatorsSPtrVector creators;              ///< \ru Журнал построения: 0<=i<countOne -  оболочки первого тела-операнда; countOne<=i<creators.Count() - оболочки второго тела-операнда. \en History tree: 0<=i<countOne -  shells of the first operand-solid; countOne<=i<creators.Count() - shells of the second operand-solid. 
  size_t                  sharedCount;           ///< \ru Количество общих строителей обоих тел. \en The number of the common creators of both solids..
  size_t                  firstCount;            ///< \ru Количество строителей первого тела. \en The number of first-solid creators.
  OperationType           operation;             ///< \ru Тип булевой операции над оболочками. \en Type of Boolean operation on shells. 
  bool                    mergeFaces;            ///< \ru Сливать подобные грани (true). \en Whether to merge similar faces (true). 
  bool                    mergeEdges;            ///< \ru Сливать подобные ребра (true). \en Whether to merge similar edges (true). 
  bool                    closed;                ///< \ru Замкнутость оболочек операндов. \en Closedness of operands' shells. 
  bool                    allowNonIntersecting;  ///< \ru Выдавать конечную оболочку, если нет пересечений. \en Allow a final result if there is no intersection.
  double                  buildSag;              ///< \ru Угловое отклонение при движении по кривым и поверхностям. \en Angular deviation while moving along curves and surfaces.  
  MbPrecision             precision;             ///< \ru  Точность построения объекта. \en The precision of object construction.

public:
  MbBooleanSolid( const c3d::CreatorsSPtrVector &  solid2, 
                        bool                       sameCreators2, 
                  const MbBooleanOperationParams & params );

  MbBooleanSolid( const c3d::CreatorsSPtrVector &  solids12,
                        size_t                     firstCount, 
                        bool                       sameCreators1, 
                        bool                       sameCreators2,
                  const MbBooleanOperationParams & params );
private :
  MbBooleanSolid( const MbBooleanSolid & init, MbRegDuplicate * ireg );
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbBooleanSolid( const MbBooleanSolid & init ); 
public :
  virtual ~MbBooleanSolid();

  // \ru Общие функции математического объекта \en Common functions of the mathematical object 

  MbeCreatorType  IsA() const override; // \ru Тип элемента. \en A type of element. 
  void        Transform( const MbMatrix3D &, MbRegTransform * ireg = nullptr ) override;              // \ru Преобразовать элемент согласно матрице. \en Transform element according to the matrix.
  void        Move     ( const MbVector3D &, MbRegTransform * ireg = nullptr ) override;              // \ru Сдвиг. \en Translation.
  void        Rotate   ( const MbAxis3D &, double angle, MbRegTransform * ireg = nullptr ) override;  // \ru Повернуть вокруг оси. \en Rotate around an axis.
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию \en Create a copy

  MbePrompt   GetPropertyName() override; // \ru Выдать заголовок свойства объекта \en Get a name of object property. 
  void        GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  void        SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта. \en Set properties of the object. 
  void        GetBasisItems ( RPArray<MbSpaceItem> & ) override; // \ru Дать базовые объекты. \en Get the base objects. 
  void        GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  size_t      GetCreatorsCount   ( MbeCreatorType ) const override; // \ru Посчитать внутренние построители по типу. \en Count internal creators by type.
  bool        GetInternalCreators( MbeCreatorType, c3d::ConstCreatorsSPtrVector & ) const override; // \ru Получить внутренние построители по типу. \en Get internal creators by type.
  bool        SetInternalCreators( MbeCreatorType, c3d::CreatorsSPtrVector & ) override;            // \ru Получить внутренние построители по типу. \en Get internal creators by type.

  bool        IsSame( const MbCreator &, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        IsSimilar( const MbCreator & ) const override;   // \ru Являются ли объекты подобными. \en Whether the objects are similar 
  bool        SetEqual ( const MbCreator & ) override;         // \ru Сделать равным \en Make equal. 

  // \ru Общие функции твердого тела. \en Common functions of solid. 

  bool        CreateShell( MbFaceShell *& shell, MbeCopyMode sameShell, 
                           RPArray<MbSpaceItem> * items = nullptr ) override; // \ru Построение \en Construction

  void        SetYourVersion( VERSION version, bool forAll ) override;
  const MbPrecision & GetPrecision() const override { return precision; } // \ru Выдать точность построения. \en Get the precision of object construction.
public:
  /// \ru Тип булевой операции над телами. \en Type of Boolean operation on solids. 
  OperationType GetOperationType() const { return operation; } 
  /// \ru Угловое отклонение при движении по кривым и поверхностям. \en Angular deviation while moving along curves and surfaces. 
  double        GetBuildSag()        const { return buildSag; }

  /// \ru Количество общих строителей тел. \en The number of common creators. 
  size_t        GetSharedCount()     const { return sharedCount; } 
  /// \ru Количество строителей первого тела. \en The number of first-solid creators. 
  size_t        GetFirstCount()      const { return firstCount; } 
  /// \ru Общее количество строителей. \en Total count of creators. 
  size_t        GetCreatorsCount()   const { return creators.size(); } 
  /// \ru Дать строитель. \en Get the creator. 
  const   MbCreator *   GetCreator( size_t k ) const { return ( (k < creators.size()) ? creators[k] : nullptr ); }
  /// \ru Удалить из журнала строители первого тела. \en Delete first-solid creators from the history tree. 
  bool          DeleteFirstCreators(); 
private :
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
  void          operator = ( const MbBooleanSolid & ); 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbBooleanSolid )
}; // MbBooleanSolid

IMPL_PERSISTENT_OPS( MbBooleanSolid )


//------------------------------------------------------------------------------
/** \brief \ru Создать оболочку булевой операции.
           \en Create the shell of Boolean operation. \~
  \details \ru Для указанных оболочек построить оболочку как результат булевой операции над оболочками тел.
    Одновременно с построением оболочки функция создаёт её строитель. \n
           \en Create a shell as a result of Boolean operation on the given shells of solids.
    The function simultaneously constructs the shell and creates its constructor. \n \~
  \param[in]  shell1         - \ru Набор граней первого тела.
                               \en The set of faces of the first solid. \~
  \param[in]  sameShell1     - \ru Способ копирования граней первого тела.
                               \en Method of copying the faces of the first solid. \~
  \param[in]  shell2         - \ru Набор граней второго тела.
                               \en The second solid face set. \~
  \param[in]  sameShell2     - \ru Способ копирования граней второго тела.
                               \en Method of copying the faces of the second solid. \~
  \param[in]  creators       - \ru Набор строителей первого и второго набора граней.
                               \en The set of creators of the first and the second face sets. \~
  \param[in]  sharedCount    - \ru Количество общих строителей обоих наборов граней.
                               \en The number of shared creators of the both face sets. \~
  \param[in]  firstCount     - \ru Количество строителей первого набора граней.
                               \en The number of creators of the first face set. \~
  \param[in]  params         - \ru Параметры операции.
                               \en A Boolean operation parameters. \~
  \param[out] res            - \ru Код результата операции.
                               \en Operation result code. \~
  \param[out] shell          - \ru Построенный набор граней.
                               \en Constructed set of faces. \~  
  \result \ru Возвращает строитель, если операция была выполнена успешно.
          \en Returns the constructor if the operation has been successfully performed. \~
  \ingroup Model_Creators
*/
// ---
MATH_FUNC (MbCreator *) CreateBoolean(       c3d::ShellSPtr &           shell1, 
                                             MbeCopyMode                sameShell1, 
                                             c3d::ShellSPtr &           shell2,
                                             MbeCopyMode                sameShell2, 
                                       const c3d::CreatorsSPtrVector &  creators, 
                                             size_t &                   sharedCount,
                                             size_t &                   firstCount,
                                       const MbBooleanOperationParams & params,
                                             MbResultType &             res, 
                                             c3d::ShellSPtr &           shell );


#endif // __CR_BOOLEAN_SOLID_H
