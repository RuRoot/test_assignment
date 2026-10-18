////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Строитель оболочки кинематического тела.
         \en Constructor of shell of evolution solid. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_EVOLUTION_SOLID_H
#define __CR_EVOLUTION_SOLID_H


#include <surf_spine.h>
#include <cr_swept_solid.h>
#include <cur_contour_on_surface.h>

class   MbEvolutionShellParams;
struct  MeshSurfaceValues;

//------------------------------------------------------------------------------
/** \brief \ru Направляющая кинематической операции.
           \en Spine for sweep operation. \~
  \details \ru Направляющая кинематической операции. Кривая или проволочный каркас. \n
           \en Spine for sweep operation. Curve or wireFrame \n \~
  \ingroup Surface_Modeling
*/
// ---
class MATH_CLASS MbSpineData {
private:
  SPtr<MbCurve3D>     _spineCurve;     ///< \ru Направляющая кривая. \en Spine curve. 
  SPtr<MbWireFrame>   _spineFrame;     ///< \ru Направляющая кривая в виде проволочного каркаса. \en Spine curve as a wire frame. \~

public:
  /// \ru Конструктор по кривой. \en Constructor by curve.
  MbSpineData( const MbCurve3D & spineCurve )
    :_spineCurve( static_cast<MbCurve3D *>(&spineCurve.Duplicate())  )
    ,_spineFrame( nullptr     ) {}

  /// \ru Конструктор по каркасу. \en Constructor by wire frame.
  MbSpineData( const MbWireFrame & spineFrame )
    :_spineCurve( nullptr     )
    ,_spineFrame( static_cast<MbWireFrame *>(&spineFrame.Duplicate()) ) {}

  /// \ru Конструктор по умолчанию. \en Default constructor.
  MbSpineData();
  /// \ru конструктор-копия. \en Copy constructor.
  MbSpineData( const MbSpineData & init, MbRegDuplicate * ireg );
  /// \ru Деструктор. \en Destructor.
  ~MbSpineData() {}

  void Transform( const MbMatrix3D &, MbRegTransform * = nullptr );             ///< \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void Move     ( const MbVector3D &, MbRegTransform * = nullptr );             ///< \ru Сдвиг \en Translation
  void Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ); ///< \ru Повернуть вокруг оси \en Rotate around an axis
  void GetProperties( MbProperties & );                                         ///< \ru Выдать свойства объекта \en Get properties of the object 
  void SetProperties( const MbProperties & );                                   ///< \ru Записать свойства объекта \en Set properties of the object 
  void GetBasisItems ( RPArray<MbSpaceItem>   & s );                            ///< \ru Дать базовые объекты \en Get the base objects 
  bool IsSame( const MbSpineData &, double accuracy ) const;                    ///< \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool IsSimilar( const MbSpineData & ) const;                                  ///< \ru Являются ли объекты подобными \en Whether the objects are similar   
  bool SetEqual ( const MbSpineData & );                                        ///< \ru Сделать равным \en Make equal 

  /// \ru Перестроить объект по журналу построения. Для перестроения проволочного каркаса. Вернуть true в случае успешного перестроения. \en Rebuild object according to the history tree. For rebuilding the wire frame. Return true if rebuild has been finished successfully. \~
  bool RebuildItem();

  /// \ru Получить направляющую кривую. \en Get the spine curve. \~
  const MbCurve3D * GetSpineCurve() const { return _spineCurve; }

  /// \ru Получить направляющую кривую в виде проволочного каркаса. \en Get the spine curve as a wire frame. \~
  const MbWireFrame * GetSpineFrame() const { return _spineFrame; }

OBVIOUS_PRIVATE_COPY( MbSpineData )
KNOWN_OBJECTS_RW_REF_OPERATORS( MbSpineData )
};


//------------------------------------------------------------------------------
/** \brief \ru Строитель оболочки кинематического тела.
           \en Constructor of shell of evolution solid. \~
  \details \ru Строитель оболочки тела путём движения образующей кривой по направляющей кривой. \n
           \en Constructor of solid shell by moving generating curve along a spine curve.  \n \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbCurveEvolutionSolid : public MbCurveSweptSolid {
protected :
  MbSweptData         sweptData;      ///< \ru Данные об образующей. \en Generating curve data. 
  MbSpineData         spineData;      ///< \ru Данные о направляющей кривой. \en Spine curve data. 
  SPtr<MbCurve3D>     directionCurve; ///< \ru Кривая вектора ориентации матрицы преобразования (может быть nullptr для простой траектории). \en A curve of the transformation matrix orientation (it may be nullptr for a simple trajectory).
  MbVector3D          direction;      ///< \ru Вектор ориентации матрицы преобразования (может быть нулевой, в случае автоопределения). \en Vector of transformation matrix orientation (it's equal zero in the mode of automatic direction calculation). 
  SPtr<MbSNameMaker>  spineNames;     ///< \ru Именователь направляющей. \en An object defining the name of the spine curve. 
  EvolutionValues     parameters;     ///< \ru Параметры. \en Parameters. 

  std::vector<std::shared_ptr<MeshSurfaceValues>> meshSurfVals; ///< \ru Данные для операции с динамической параметризацией сечения. \en Data for sweeping operation with variable section.

public :
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по одному контуру на поверхности.
             \en Constructor by one contour on a surface. \~
    \param[in] surface_     - \ru Поверхность образующей.
                              \en Surface of a generating curve. \~
    \param[in] contour_     - \ru Контур в параметрах поверхности.
                              \en Contour in surface parameters domain. \~
    \param[in] spine_       - \ru Направляющая кривая.
                              \en The spine curve. \~
    \param[in] params       - \ru Параметры кинематической операции.
                              \en Parameters of the sweeping operation. \~
    \param[in] oType        - \ru Тип булевой операции с предыдущим результатом.
                              \en Type of Boolean operation with the previous result. \~
    \param[in] operNames    - \ru Именователь операции.
                              \en An object defining names generation in the operation. \~
    \param[in] contourNames - \ru Имена контуров образующей для именования граней.
                              \en Generatix contours' names for naming faces. \~
    \param[in] spineNames   - \ru Имена направляющей.
                              \en Generating curve names. \~
  */
  MbCurveEvolutionSolid( const MbSurface &       surface_,
                         const MbContour &       contour_,
                         const MbCurve3D &       spine_,
                         const EvolutionValues & params,
                               OperationType     oType,
                         const MbSNameMaker &    operNames,
                         const MbSNameMaker &    contourNames,
                         const MbSNameMaker &    spineNames_ );

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по смешанной образующей.
             \en Constructor by combined generating curve. \~
    \param[in] sweptData_   - \ru Образующая.
                              \en Generating curve. \~
    \param[in] spine_       - \ru Направляющая кривая.
                              \en The spine curve. \~
    \param[in] params       - \ru Параметры кинематической операции.
                              \en Parameters of the sweeping operation. \~
    \param[in] oType        - \ru Тип булевой операции с предыдущим результатом.
                              \en Type of Boolean operation with the previous result. \~
    \param[in] operNames    - \ru Именователь операции.
                              \en An object defining names generation in the operation. \~
    \param[in] contourNames - \ru Имена контуров образующей для именования граней.
                              \en Generatix contours' names for naming faces. \~
    \param[in] spineNames   - \ru Имена направляющей.
                              \en Generating curve names. \~
  */
  MbCurveEvolutionSolid( const MbSweptData &           sweptData_,
                         const MbCurve3D &             spine_,
                         const EvolutionValues &       params,
                               OperationType           oType,
                         const MbSNameMaker &          operNames,
                         const RPArray<MbSNameMaker> & contoursNames,
                         const MbSNameMaker &          spineNames_ );

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по смешанной образующей.
             \en Constructor by combined generating curve. \~
    \param[in] sweptData_   - \ru Образующая.
                              \en Generating curve. \~
    \param[in] spineFrame_  - \ru Направляющая кривая в виде проволочного каркаса.
                              \en The spine curve as a wire frame. \~
    \param[in] params       - \ru Параметры кинематической операции.
                              \en Parameters of the sweeping operation. \~
    \param[in] oType        - \ru Тип булевой операции с предыдущим результатом.
                              \en Type of Boolean operation with the previous result. \~
    \param[in] operNames    - \ru Именователь операции.
                              \en An object defining names generation in the operation. \~
    \param[in] contourNames - \ru Имена контуров образующей для именования граней.
                              \en Generatrix contours' names for naming faces. \~
    \param[in] spineNames   - \ru Имена направляющей.
                              \en Generating curve names. \~
  */
  MbCurveEvolutionSolid( const MbSweptData &           sweptData_,
                         const MbWireFrame &           spineFrame_,
                         const EvolutionValues &       params,
                               OperationType           oType,
                         const MbSNameMaker &          operNames,
                         const RPArray<MbSNameMaker> & contoursNames,
                         const MbSNameMaker &          spineNames_ );

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по смешанной образующей.
             \en Constructor by combined generating curve. \~
    \param[in] sweptData_   - \ru Образующая.
                              \en Generating curve. \~
    \param[in] spine_       - \ru Направляющая кривая.
                              \en The spine curve. \~
    \param[in] params       - \ru Параметры кинематической операции.
                              \en Parameters of the sweeping operation. \~
    \param[in] oType        - \ru Тип булевой операции с предыдущим результатом.
                              \en Type of Boolean operation with the previous result. \~
    \param[in] operNames    - \ru Именователь операции.
                              \en An object defining names generation in the operation. \~
    \param[in] contourNames - \ru Имена контуров образующей для именования граней.
                              \en Generatix contours' names for naming faces. \~
    \param[in] spineNames   - \ru Имена направляющей.
                              \en Generating curve names. \~
  */
  MbCurveEvolutionSolid( const MbSweptData &           sweptData_,
                         const MbSpine &               spine_,
                         const EvolutionValues &       params,
                               OperationType           oType,
                         const MbSNameMaker &          operNames,
                         const RPArray<MbSNameMaker> & contoursNames,
                         const MbSNameMaker &          spineNames_ );

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по данным для операции с динамической параметризацией сечения.
             \en Constructor using data for sweeping operation with variable section. \~
    \param[in] sweptData_    - \ru Образующая.
                               \en Generating curve. \~
    \param[in] spine_        - \ru Направляющая кривая с дополнительной информацией.
                               \en The spine curve with additional information. \~
    \param[in] params        - \ru Параметры кинематической операции.
                               \en Parameters of the sweeping operation. \~
    \param[in] meshSurfVals_ - \ru Данные для операции с динамической параметризацией сечения.
                               \en Data for sweeping operation with variable section. \~
    \param[in] oType         - \ru Тип булевой операции с предыдущим результатом.
                               \en Type of Boolean operation with the previous result. \~
    \param[in] operNames     - \ru Именователь операции.
                               \en An object defining names generation in the operation. \~
    \param[in] contourNames  - \ru Имена контуров образующей для именования граней.
                               \en Generatix contours' names for naming faces. \~
    \param[in] spineNames    - \ru Имена направляющей.
                               \en Generating curve names. \~
  */
  MbCurveEvolutionSolid( const MbSweptData &                                     sweptData_,
                         const MbSpine &                                         spine_,
                         const EvolutionValues &                                 params,
                         const std::vector<std::shared_ptr<MeshSurfaceValues>> & meshSurfVals_,
                               OperationType                                     oType,
                         const MbSNameMaker &                                    operNames,
                         const RPArray<MbSNameMaker> &                           contoursNames,
                         const MbSNameMaker &                                    spineNames_ );

private :
  MbCurveEvolutionSolid( const MbCurveEvolutionSolid & init, MbRegDuplicate * ireg );
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbCurveEvolutionSolid( const MbCurveEvolutionSolid & ); 
public :
  virtual ~MbCurveEvolutionSolid();

  /** \ru \name Общие функции математического объекта.
      \en \name Common functions of the mathematical object.
      \{ */
  MbeCreatorType  IsA() const override; // \ru Тип элемента \en A type of element 
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию \en Create a copy
  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override;             // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override;             // \ru Сдвиг \en Translation
  void        Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate around an axis

  void        GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of the object 
  MbePrompt   GetPropertyName() override; // \ru Выдать заголовок свойства объекта \en Get a name of object property 
  void        GetBasisItems ( RPArray<MbSpaceItem>   & s ) override; // \ru Дать базовые объекты \en Get the base objects 
  void        GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  bool        IsSame( const MbCreator &, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        IsSimilar( const MbCreator & ) const override; // \ru Являются ли объекты подобными \en Whether the objects are similar 
  bool        SetEqual ( const MbCreator & ) override;       // \ru Сделать равным \en Make equal 
  
  /** \} */
  /** \ru \name Общие функции твердого тела (формообразующей операции).
      \en \name Common functions of the rigid solid (forming operations).
      \{ */
  MbFaceShell * InitShell( bool in ) override; 
  void        InitBasis( RPArray<MbSpaceItem> & items ) override; 
  bool        GetPlacement( MbPlacement3D & p ) const override;
  void        SetYourVersion( VERSION version, bool forAll ) override;
  /** \} */
  /** \ru \name Функции строителя оболочки кинематического тела.
      \en \name Functions of creator of evolution solid shell.
      \{ */
  /// \ru Дать параметры. \en Get the parameters. 
  void        GetParameters(       EvolutionValues & params ) const { params = parameters; }
  /// \ru Установить параметры. \en Set the parameters. 
  void        SetParameters( const EvolutionValues & params )       { parameters = params; }
  /** \} */

private :
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
  void        operator = ( const MbCurveEvolutionSolid & ); 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbCurveEvolutionSolid )
}; // MbCurveEvolutionSolid

IMPL_PERSISTENT_OPS( MbCurveEvolutionSolid )


//------------------------------------------------------------------------------
/** \brief \ru Создать оболочку кинематического тела.
           \en Create a shell of evolution solid. \~
  \details \ru Построить оболочку путём движения образующей кривой по направляющей кривой
    и выполнить булуву операцию с оболочкой, если последняя задана. \n
    Одновременно с построением оболочки функция создаёт её строитель.\n
           \en Create a shell by moving the generating curve along the spine curve
    and perform the Boolean operation with the shell if it is specified. \n
    The function simultaneously creates the shell and its constructor.\n \~
  \param[in] srcSolid - \ru Набор граней, к которым дополняется построение.
                        \en Face set the construction is complemented with respect to. \~
  \param[in] copyMode - \ru Способ копирования граней.
                        \en The method of copying faces. \~
  \param[in] oType    - \ru Тип операции дополнения построения.
                        \en Type of operation of construction complement. \~
  \param[in] evShellParams - \ru Исходные данные для создания оболочки кинематического тела.
                             \en The source data for making a shell of evolution solid. \~
  \param[out] res - \ru Код результата операции.
                    \en Operation result code. \~
  \param[out] resShell - \ru Построенный набор граней.
                         \en Constructed set of faces. \~
  \result \ru Возвращает строитель.
          \en Returns the constructor. \~
  \ingroup Model_Creators
*/
// ---
MATH_FUNC (c3d::CreatorSPtr) CreateCurveEvolution(       c3d::ShellSPtr &         srcSolid,
                                                         MbeCopyMode              copyMode,
                                                         OperationType            oType,
                                                   const MbEvolutionShellParams & evShellParams,
                                                         MbResultType &           res,
                                                         c3d::ShellSPtr &         resShell );


#endif // __CR_EVOLUTION_SOLID_H
