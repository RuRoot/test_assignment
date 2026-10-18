////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Строитель оболочки с уклонёнными гранями.
         \en Constructor of a shell with drafted faces. 
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_DRAFT_SOLID_H
#define __CR_DRAFT_SOLID_H


#include <creator.h>

class  MbDraftSolidParams;

//------------------------------------------------------------------------------
/** \brief \ru Строитель оболочки с уклонёнными гранями.
           \en Constructor of a shell with drafted faces. \~
  \details \ru Строитель оболочки с уклонёнными гранями для создания литейных уклонов.\n
           \en Constructor of a shell with drafted faces for pattern drafts creation. \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbDraftSolid: public MbCreator {
protected:
  double                     angle;          ///< \ru Угол уклона. \en Draft angle. 
  c3d::ItemIndices           faceIndices;    ///< \ru Индексы множества уклоняемых граней. \en Indices of faces to draft. 
  SArray<MbEdgeFacesIndexes> edgeIndices;    ///< \ru Индексы множества нейтральных ребер. \en Indices of edges to draft.
  MbeFacePropagation         fp;             ///< \ru Признак захвата граней ( face propagation ). \en Flag of face propagation. 
  // \ru Атрибуты, определяющие направление тяги (pull direction) и нейтральную изолинию уклона. \en Attributes determining the pull direction and the neutral isoline of the draft. 
  MbPlacement3D *            np;             ///< \ru Нейтральная плоскость ( neutral plane )         ( не обязателен ). \en Neutral plane (optional). 
  ptrdiff_t                  edgeNb;         ///< \ru Номер прямолинейного ребра, направляющего уклон ( не обязателен ). \en The index of straight edge specifying the draft (optional). 
  SArray<ptrdiff_t> *        pl;             ///< \ru Линии разъема (ребра) ( parting line  )         ( не обязателен ). \en Parting lines (of edge) (optional). 
  bool                       reverse;        ///< \ru Обратное направление тяги. \en Reverse pull direction. 
  bool                       step;           ///< \ru Ступенчатый способ уклона. \en Stepwise method of draft.  
  bool                       rebuildFillets; ///< \ru Перестраивать ли скругления. \en Whether to rebuild the fillets.


public:
  /// \ru Конструктор уклона по известной нейтральной плоскости. \en Constructor of drafting by the given neutral plane. 
  MbDraftSolid( const MbPlacement3D &            nPlace,   // нейтральная плоскость ( neutral plane )
                      double                     ang,      // угол уклона
                const std::vector<MbItemIndex> & faceInds, // номера множества уклоняемых граней
                      MbeFacePropagation         faceProp, // признак захвата граней
                      bool                       rev,      // обратное направление тяги
                const MbSNameMaker &             n,
                      bool                       _rebuildFillets = false )
    : MbCreator     ( n        )
    , angle         ( ang      ) 
    , faceIndices   ( faceInds )
    , edgeIndices   (          )
    , fp            ( faceProp ) 
    , np            ( new MbPlacement3D( nPlace ) )
    , edgeNb        ( -1       )
    , pl            ( nullptr     )
    , reverse       ( rev      )
    , step          ( false    )
    , rebuildFillets( _rebuildFillets )
  {
  }
  /// \ru Конструктор уклона по линии разъема \en Constructor of drafting by the parting line 
  MbDraftSolid(       double              ang,       // угол уклона
                const MbPlacement3D *     nPlace,    // нейтральная плоскость ( neutral plane )
                      ptrdiff_t           edgeInd,   // номер прямолинейного ребра - направляющего уклон  ( не обязателен )
                      MbeFacePropagation  faceProp,  // признак захвата граней
                const SArray<ptrdiff_t> & partLines, // линии разъема (ребра) (parting line) (не обязателен)
                      bool                rev,       // обратное направление тяги
                      bool                st,        // ступенчатый способ уклона
                const MbSNameMaker &      n,
                      bool                _rebuildFillets = false )
    : MbCreator     ( n        )
    , angle         ( ang      ) 
    , faceIndices   (          )
    , edgeIndices   (          )
    , fp            ( faceProp ) 
    , np            ( nPlace ? new MbPlacement3D( *nPlace ) : nullptr )
    , edgeNb        ( edgeInd  )
    , pl            ( new SArray<ptrdiff_t>( partLines ) )
    , reverse       ( rev      )
    , step          ( st       )
    , rebuildFillets( _rebuildFillets )

  {
  }
  /// \ru Конструктор уклона по линии разъема и уклоняемым граням \en Constructor of drafting by the parting line and drafting faces 
  MbDraftSolid(       double              ang,             // угол уклона
                const MbPlacement3D *     nPlace,          // нейтральная плоскость ( neutral plane )
                      ptrdiff_t           edgeInd,         // номер прямолинейного ребра - направляющего уклон  ( не обязателен )
                      MbeFacePropagation  faceProp,        // признак захвата граней
                const std::vector<MbItemIndex> & faceInds, // индексы множества уклоняемых граней
                const SArray<ptrdiff_t> & partLines,       // линии разъема (ребра) (parting line) (не обязателен)
                      bool                rev,             // обратное направление тяги
                      bool                st,              // ступенчатый способ уклона
                const MbSNameMaker &      n,
                      bool                _rebuildFillets = false )
    : MbCreator     ( n        )
    , angle         ( ang      ) 
    , faceIndices   ( faceInds )
    , edgeIndices   (          )
    , fp            ( faceProp ) 
    , np            ( nPlace ? new MbPlacement3D( *nPlace ) : nullptr )
    , edgeNb        ( edgeInd  )
    , pl            ( new SArray<ptrdiff_t>( partLines ) )
    , reverse       ( rev      )
    , step          ( st       )
    , rebuildFillets( _rebuildFillets )
  {
  }

  /// \ru Конструктор уклона по нейтральным ребрам и уклоняемым граням \en Constructor of drafting by the parting line and drafting faces 
  MbDraftSolid(       double                     ang,        // угол уклона
                const MbPlacement3D *            nPlace,     // нейтральная плоскость ( neutral plane )
                      ptrdiff_t                  edgeInd,    // номер прямолинейного ребра - направляющего уклон  ( не обязателен )
                      MbeFacePropagation         faceProp,   // признак захвата граней
                const std::vector<MbItemIndex> & faceInds,   // индексы множества уклоняемых граней
                const SArray<MbEdgeFacesIndexes> & edgeInds, // индексы множества нейтральных ребер
                      bool                       rev,        // обратное направление тяги
                      bool                       st,         // ступенчатый способ уклона
                const MbSNameMaker &             n,
                      bool                       _rebuildFillets = false )
    : MbCreator     ( n        )
    , angle         ( ang      ) 
    , faceIndices   ( faceInds )
    , edgeIndices   ( edgeInds )
    , fp            ( faceProp ) 
    , np            ( nPlace ? new MbPlacement3D( *nPlace ) : nullptr )
    , edgeNb        ( edgeInd  )
    , pl            (  nullptr )
    , reverse       ( rev      )
    , step          ( st       )
    , rebuildFillets( _rebuildFillets )
  {
  }

private :
  // \ru Конструктор копирования \en Copy-constructor 
  MbDraftSolid( const MbDraftSolid &, MbRegDuplicate * );  
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbDraftSolid( const MbDraftSolid & ); 
public :
  virtual ~MbDraftSolid();

  /// \ru Общие функции математического объекта \en Common functions of the mathematical object 

  MbeCreatorType  IsA() const override; // \ru Тип элемента \en A type of element 
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override;
  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override;               // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move     ( const MbVector3D &, MbRegTransform * = nullptr   ) override;               // \ru Сдвиг \en Translation
  void        Rotate   ( const MbAxis3D   &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate around an axis

  void        GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of the object 
  MbePrompt   GetPropertyName() override; // \ru Выдать заголовок свойства объекта \en Get a name of object property 

  bool        IsSame   ( const MbCreator &, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal? 
  bool        SetEqual ( const MbCreator & ) override;       // \ru Сделать равным \en Make equal 
  bool        IsSimilar( const MbCreator & ) const override; // \ru Являются ли объекты подобными \en Whether the objects are similar 

  /// \ru Общие функции твердого тела \en Common functions of solid 

  bool        CreateShell( MbFaceShell *&, MbeCopyMode sameShell,
                           RPArray<MbSpaceItem> * = nullptr ) override; // \ru Построение \en Construction

private :
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
  void        operator = ( const MbDraftSolid & ); 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbDraftSolid )
}; // MbDraftSolid

IMPL_PERSISTENT_OPS( MbDraftSolid )


//------------------------------------------------------------------------------
/** \brief \ru Построить оболочку с уклоном граней.
           \en Create a shell with drafted faces. \~
  \details \ru Для исходной оболочки построить оболочку с уклоном граней от нейтральной изоплоскости или нейтральных ребер для создания литейных уклонов. \n
    Одновременно с построением оболочки функция создаёт её строитель.\n
           \en For the source shell create a shell with faces drafted from the neutral isoplane or neutral edges for pattern tapers creation. \n
    The function simultaneously creates the shell and its constructor.\n \~ 
  \param[in]  solid      - \ru Исходная оболочка.
                           \en The source shell. \~
  \param[in]  sameShell  - \ru Режим копирования исходной оболочки.
                           \en Mode of copying the source shell. \~
  \param[in]  draftParams - \ru Параметры уклона.
                            \en Draft parameters. \~
  \param[out] res        - \ru Код результата операции выдавливания.
                           \en The extrusion operation result code. \~
  \param[out] shell      - \ru Построенная оболочка.
                           \en The resultant shell. \~
  \result \ru Возвращает строитель оболочки.
          \en Returns the shell constructor. \~
  \ingroup Model_Creators
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
MATH_FUNC (c3d::CreatorSPtr) CreateDraft(       c3d::ShellSPtr &     solid,
                                                MbeCopyMode          sameShell,
                                          const MbDraftSolidParams & draftParams,
                                                MbResultType &       res,
                                                c3d::ShellSPtr &     resShell );


#endif // __CR_DRAFT_SOLID_H
