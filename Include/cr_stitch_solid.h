////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Строитель оболочки путём сшивки граней.
         \en Constructor of a shell by stitching the faces.
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_STITCH_SOLID_H
#define __CR_STITCH_SOLID_H


#include <creator.h>
#include <cr_simple_creator.h>
#include <mb_operation_result.h>
#include <op_shell_parameter.h>


//------------------------------------------------------------------------------
/** \brief \ru Строитель оболочки путём сшивки граней.
           \en Constructor of a shell by stitching the faces. \~
  \details \ru Строитель оболочки путём сшивки граней. \n
    Операция принимает в качестве исходных данных незамкнутые оболочки и отдельные грани,
    ищет среди их граничных рёбер совпадающие полностью или частично и "сшивает" их, т.е.
    устанавливает связи между гранями, рёбрами и вершинами. В ходе выполнения операции рёбра
    и вершины уточняются при необходимости путём пересечения стыкующихся граней. Алгоритм работы
    следующий:\n
    1. Для каждой вершины насчитывается габарит, в котором будет осуществляться поиск совпадающих вершин.
       Этот габарит зависит от длин стыкующихся в ней рёбер и от толщины тонкой стенки, если такая ситуация обнаружена.\n
    2. С помощью насчитанных габаритов все вершины разбиваются на группы, каждая из которых сформирует одну
       вершину результата.\n
    3. Все рёбра разбиваются на группы с одинаковыми конечными группами вершин.\n
    4. В каждой группе рёбер ищутся пары совпадающих с заданной точностью рёбер.\n
    5. Каждое непарное ребро разбивается лежащими на нём вершинами, и для них повторяются пункты с 1 по 4.\n
    6. С помощью пересечений стыкующихся граней парные рёбра уточняются и сшиваются, а каждая группа вершин
       заменяется одной вершиной.
           \en Constructor of a shell by stitching the faces. \n
    The operation takes open shells and separate faces as input data,
    and looks for the entirely or partially coincident edges among their boundary edges and "stitches" them, i.e.
    makes associations between faces, edges and vertices. During the operation edges
    and vertices can be defined more precisely by intersection of adjacent faces if necessary. The algorithm
    is the following:\n
    1. For each vertex the bounding box is calculated in which the search of the coincident vertices will be performed.
       This bounding box depends on the length of the edges adjacent to the vertex and on thickness of the thin wall if such a situation is detected.\n
    2. Using the calculated bounding boxes all the vertices are subdivided into the groups each of which will form a single
       resultant vertex.\n
    3. All the edges are divided into groups with similar resultant groups of vertices.\n
    4. In each group of edges pairs of edges coincident with the given tolerance are searched for.\n
    5. Each unpaired edge is split by vertices lying on it, and pt.1 to 4 are repeated for them.\n
    6. Using intersections of adjacent faces paired edges are refined and stitched, and each group of vertices
       is replaced with a single vertex. \~
  \ingroup Model_Creators
*/ 
// ---
class MATH_CLASS MbStitchedSolid : public MbCreator {
private:
  PArray< RPArray<MbCreator> > creatorsArray;   ///< \ru Множества строителей для каждой сшиваемой оболочки. \en Set of creators for each shell being stitched. 
  bool                         formSolidBody;   ///< \ru Флаг необходимости формирования замкнутой оболочки. \en Whether to construct a closed shell. 
  double                       stitchPrecision; ///< \ru Максимально допустимое расстояние между сшиваемыми рёбрами. \en Maximal acceptable distance between edges being stitched. 
  bool                         mergeEdges;      ///< \ru Сливать подобные ребра (true). \en Whether to merge similar edges (true). 

public :
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \param[in] creatorsData - \ru Множества строителей для каждой сшиваемой оболочки.
                              \en Set of creators for each shell being stitched. \~
    \param[in] tryClosed    - \ru Флаг необходимости формирования замкнутой оболочки.
                              \en Whether to construct a closed shell. \~
    \param[in] precision    - \ru Максимально допустимое расстояние между сшиваемыми рёбрами.
                              \en Maximal acceptable distance between edges being stitched. \~
    \param[in] names        - \ru Именователь с версией.
                              \en Names maker with version. \~
    \param[in] edgesMerging - \ru Сливать подобные ребра (true).
                              \en Whether to merge similar edges (true). \~
  */
  template <class CreatorsVectorPtr>
  MbStitchedSolid( const PArray<CreatorsVectorPtr> & creatorsData,
                         bool                        tryClosed,
                         double                      precision,
                   const MbSNameMaker &              names,
                         bool                        edgesMerging )
    : MbCreator      ( names        )
    , creatorsArray  ( creatorsData.size(), 1, true )
    , formSolidBody  ( tryClosed    )
    , stitchPrecision( precision    )
    , mergeEdges     ( edgesMerging )
  {
    size_t i, setsCnt = creatorsData.size();

    c3d::CreatorsSPtrVector simpleCreators;
    {
      size_t estSimpleCnt = 0;
      for ( i = 0; i < setsCnt; ++i ) {
        if ( creatorsData[i] != nullptr ) {
          size_t count = creatorsData[i]->size();
          for ( size_t j = 0; j < count; ++j ) {
            const MbCreator * creator = (*(creatorsData[i]))[j];
            if ( creator != nullptr )
              estSimpleCnt += creator->GetCreatorsCount( ct_SimpleCreator );
          }
        }
      }
      simpleCreators.reserve( estSimpleCnt );
    }

    c3d::CreatorSPtr creator;
    for ( i = 0; i < setsCnt; ++i ) {
      if ( creatorsData[i] != nullptr ) {
        size_t count = creatorsData[i]->size();
        RPArray<MbCreator> * creators = new RPArray<MbCreator>( count, 1 );
        creatorsArray.push_back( creators );
        for ( size_t j = 0; j < count; ++j ) { // важен порядок перебора
          creator = (*(creatorsData[i]))[j];
          if ( creator != nullptr ) {
            creators->push_back( creator );
            if ( creator->IsA() == ct_SimpleCreator )
              simpleCreators.push_back( creator );
            else {
              creator->SetInternalCreators( ct_SimpleCreator, simpleCreators );
            }
            creator->AddRef();
          }
        }
      }
    }
    if ( !simpleCreators.empty() ) {
      MbSimpleCreator::DeleteShellCopies( simpleCreators );
    }
  }
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \param[in] creatorsData - \ru Множества строителей для каждой сшиваемой оболочки.
                              \en Set of creators for each shell being stitched. \~
    \param[in] tryClosed    - \ru Флаг необходимости формирования замкнутой оболочки.
                              \en Whether to construct a closed shell. \~
    \param[in] precision    - \ru Максимально допустимое расстояние между сшиваемыми рёбрами.
                              \en Maximal acceptable distance between edges being stitched. \~
    \param[in] names        - \ru Именователь с версией.
                              \en Names maker with version. \~
    \param[in] edgesMerging - \ru Сливать подобные ребра (true).
                              \en Whether to merge similar edges (true). \~
  */
  MbStitchedSolid( const std::vector<c3d::CreatorsSPtrVector> & creatorsData,
                   const MbShellStitchParams &                  stitchParams )
    : MbCreator      ( stitchParams.GetNameMaker()        )
    , creatorsArray  ( creatorsData.size(), 1, true       )
    , formSolidBody  ( stitchParams.TryBodyForming()      )
    , stitchPrecision( stitchParams.GetStitchAccuracy()   )
    , mergeEdges     ( stitchParams.PerformEdgesMerging() )
  {
    size_t i, setsCnt = creatorsData.size();

    c3d::CreatorsSPtrVector simpleCreators;
    {
      size_t estSimpleCnt = 0;
      for ( i = 0; i < setsCnt; ++i ) {
        if ( !creatorsData[i].empty() ) {
          size_t count = creatorsData[i].size();
          for ( size_t j = 0; j < count; ++j ) {
            const MbCreator * creator = (creatorsData[i])[j];
            if ( creator != nullptr )
              estSimpleCnt += creator->GetCreatorsCount( ct_SimpleCreator );
          }
        }
      }
      simpleCreators.reserve( estSimpleCnt );
    }

    c3d::CreatorSPtr creator;
    for ( i = 0; i < setsCnt; ++i ) {
      if ( !creatorsData[i].empty() ) {
        size_t count = creatorsData[i].size();
        RPArray<MbCreator> * creators = new RPArray<MbCreator>( count, 1 );
        creatorsArray.push_back( creators );
        for ( size_t j = 0; j < count; ++j ) { // важен порядок перебора
          creator = (creatorsData[i])[j];
          if ( creator != nullptr ) {
            creators->push_back( creator );
            if ( creator->IsA() == ct_SimpleCreator )
              simpleCreators.push_back( creator );
            else {
              creator->SetInternalCreators( ct_SimpleCreator, simpleCreators );
            }
            creator->AddRef();
          }
        }
      }
    }
    if ( !simpleCreators.empty() ) {
      MbSimpleCreator::DeleteShellCopies( simpleCreators );
    }
  }
private:
  MbStitchedSolid( const MbStitchedSolid & init,
                         MbRegDuplicate *  ireg );
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbStitchedSolid( const MbStitchedSolid & );

public:
  virtual ~MbStitchedSolid();

  // \ru Общие функции математического объекта \en Common functions of the mathematical object 

  MbeCreatorType  IsA()  const override; // \ru Тип элемента \en A type of element
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию \en Create a copy
  bool        IsSame( const MbCreator & other, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        SetEqual ( const MbCreator & ) override; // \ru Сделать равным \en Make equal 
  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override; // \ru Сдвиг \en Translation
  void        Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate around an axis

  void        GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of the object 
  MbePrompt   GetPropertyName() override; // \ru Выдать заголовок свойства объекта \en Get a name of object property 
  void        GetBasisItems ( RPArray<MbSpaceItem> & ) override; // \ru Дать базовые объекты \en Get the base objects 
  void        GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.
  size_t      GetCreatorsCount( MbeCreatorType ct ) const override; // \ru Посчитать внутренние построители по типу. \en Count internal creators by type.
  bool        GetInternalCreators( MbeCreatorType, c3d::ConstCreatorsSPtrVector & ) const override; // \ru Получить внутренние построители по типу. \en Get internal creators by type.
  bool        SetInternalCreators( MbeCreatorType, c3d::CreatorsSPtrVector & ) override;            // \ru Получить внутренние построители по типу. \en Get internal creators by type.

  // \ru Общие функции твердого тела \en Common functions of solid  

  bool        CreateShell( MbFaceShell *&        shell,
                           MbeCopyMode           sameShell, 
                           RPArray<MbSpaceItem> * items = nullptr ) override; // \ru Построение \en Construction

  void        SetYourVersion( VERSION version, bool forAll ) override;

private:
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
  MbStitchedSolid  &  operator = ( const MbStitchedSolid & );

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbStitchedSolid )
}; // MbStitchedSolid

IMPL_PERSISTENT_OPS( MbStitchedSolid )


//------------------------------------------------------------------------------
/** \brief \ru Построение оболочки сшивки.
           \en Construction of a shell of stitching. \~
  \details \ru Построение оболочки сшивки. \n
           \en Construction of a shell of stitching. \n \~
  \param[in] initialShells - \ru Множество оболочек для сшивки.
                             \en A set of shells for stitching. \~
  \param[in] params        - \ru Параметры сшивки оболочек.
                             \en Shells stitch parameters. \~
  \param[out] res          - \ru Код результата операции выдавливания.
                             \en The extrusion operation result code. \~
  \result \ru Возвращает построенную оболочку, если операция была выполнена успешно.
          \en Returns the constructed shell if the operation has been successfully performed. \~
  \ingroup Model_Creators
*/
// ---
MATH_FUNC (MbFaceShell *) CreateStitchShell( const c3d::ShellsSPtrVector & initialShells,
                                             const MbShellStitchParams &   params,
                                                   MbeStitchResType &      res );


//------------------------------------------------------------------------------
/** \brief \ru Построить оболочку путём сшивки граней.
           \en Create a shell by faces stitching. \~
  \details \ru Создание строителя сшитого тела.\n
           \en Creation of a stitched solid creator.\n \~
  \details \ru Построить оболочку путём сшивки граней исходных оболочек и множеств граней. \n
    Одновременно с построением оболочки функция создаёт её строитель.\n
           \en Create a shell by stitching faces of the source shells and sets of faces. \n
    The function simultaneously creates the shell and its constructor.\n \~ 
  \param[in]  creatorsArray - \ru Множества строителей для сшиваемых оболочек.
                              \en Sets of creator for stitched shells. \~
  \param[in]  shells        - \ru Множества сшиваемых оболочек.
                              \en Sets of shells to stitch. \~
  \param[in] params         - \ru Параметры сшивки оболочек.
                              \en Shells stitch parameters. \~
  \param[out] res           - \ru Код результата операции выдавливания.
                              \en The extrusion operation result code. \~
  \param[out] resultShell   - \ru Построенная оболочка.
                              \en The resultant shell. \~
  \result \ru Возвращает строитель оболочки.
          \en Returns the shell constructor. \~
  \ingroup Model_Creators
*/
// ---
MATH_FUNC (MbCreator *) CreateStitchedSolid( const std::vector<c3d::CreatorsSPtrVector> & creatorsArray, 
                                             const c3d::ShellsSPtrVector &                shells,
                                             const MbShellStitchParams &                  params,
                                                   MbeStitchResType &                     res, 
                                                   c3d::ShellSPtr &                       resultShell );


#endif // __CR_STITCH_SOLID_H
