////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Параметры прямого редактирования тела.
         \en Direct modeling parameters. \~
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __OP_DIRECT_MOD_PARAMETERS_H
#define __OP_DIRECT_MOD_PARAMETERS_H


#include <solid.h>


struct DirectModValues;


//------------------------------------------------------------------------------
/** \brief \ru Старые типы модификации.
           \en Old type of modification. \~
  \details \ru Тип определяет действия при прямом моделировании.
           \en Type determines direct modeling actions. \~
  \ingroup Direct_Building_Parameters
*/
enum MbeModifyingType {
  dmt_Remove = 0,   ///< \ru Удаление из тела выбранных граней с окружением. \en Removal of the specified faces with the neighborhood from a solid.
  dmt_Create,       ///< \ru Создание тела из выбранных граней с окружением. \en Creation of a solid from the specified faces with the neighborhood.
  dmt_Action,       ///< \ru Перемещение выбранных граней с окружением относительно оставшихся граней тела. \en Translation of the specified faces with neighborhood relative to the other faces of the solid.
  dmt_Offset,       ///< \ru Замена выбранных граней тела эквидистантными гранями (перемещение по нормали, изменение радиуса). \en Replacement of the specified faces of a solid with the offset faces (translation along the normal, change of the radius).
  dmt_Fillet,       ///< \ru Изменение радиусов выбранных граней скругления на заданную величину. \en Change of radii of the specified fillet faces for the set value.
  dmt_Supple,       ///< \ru Замена выбранных граней тела деформируемыми гранями (превращение в NURBS для редактирования). \en Replacement of the specified faces of a solid with a deformable faces (conversion to NURBS for editing).
  dmt_Purify,       ///< \ru Удаление из тела выбранных скруглений. \en Removal of the specified fillets from a solid.
  dmt_Merger,       ///< \ru Слияние вершин ребёр и удаление рёбер. \en Merging vertices of edges and edges removal.
  dmt_United,       ///< \ru Замена гладко стыкующихся граней одной гранью. \en Replacing smoothly joined faces with one face.
  dmt_Rotate,       ///< \ru Поворот выбранных граней тела. \en Rotate the chosen faces.
  dmt_Undefined,    ///< \ru Служебный. \en Inner type.
};


//------------------------------------------------------------------------------
/** \brief \ru Типы модификации.
           \en Type of modification. \~
  \details \ru Тип определяет действия при прямом моделировании.
           \en Type determines direct modeling actions. \~
  \ingroup Direct_Building_Parameters
*/
enum MbeModifyingMode {
  dmm_Undefined                = 0,   ///< \ru Служебный. \en Inner type.

  dmm_RemoveFaces              = 10,             ///< \ru Удаление из тела выбранных граней. \en Removal of the specified faces.
  dmm_RemoveFacesWithAdjacentFillets = 11,       ///< \ru Удаление из тела выбранных граней с прилегающими скруглениями. \en Removal of the specified faces with the adjacent fillets from a solid.
  dmm_RemoveBelowRadius        = 12,             ///< \ru Удаление замкнутых цилиндрических, сферических граней, граней вращения и выдавливания радиуса меньше заданного. \en Remove the close cylindric, spherical faces, extrusion and rotation faces below specified radius.
  dmm_RemoveBelowRadiusWithAdjacentFillets = 13, ///< \ru Удаление замкнутых цилиндрических, сферических граней, граней вращения и выдавливания радиуса меньше заданного вместе с прилегающими скруглениями. \en Remove the close cylindric, spherical faces, extrusion and rotation faces below specified radius with the adjacent fillets.

  dmm_Create                   = 30,  ///< \ru Создание тела из выбранных граней с окружением. \en Creation of a solid from the specified faces with the neighborhood.

  dmm_MoveFacesByVector        = 40,  ///< \ru Перемещение выбранных граней с окружением относительно оставшихся граней тела. \en Translation of the specified faces with neighborhood relative to the other faces of the solid.
  dmm_MoveFacesByVectorNoAdaptation = 41,  ///< \ru Перемещение выбранных граней без адаптации прилегающих скруглений. \en Translation of the specified faces without the adjacent fillets adaptation.
  
  dmm_OffsetFaces                   = 50,  ///< \ru Замена выбранных граней тела эквидистантными гранями (перемещение по нормали, изменение радиуса). \en Replacement of the specified faces of a solid with the offset faces (translation along the normal, change of the radius).
  dmm_CylinderRadiusSet             = 51,  ///< \ru В разработке. Выставление нового размера для цилиндрических граней. \en Under development. Set new size for cylindrical faces.
  dmm_CylinderRadiusSetNoAdaptation = 52,  ///< \ru В разработке. Выставление нового размера для цилиндрических граней без адаптации прилегающих скруглений. \en Under development. Set new size for cylindrical faces without the adjacent fillets adaptation.
  dmm_SphereRadiusSet               = 53,  ///< \ru В разработке. Выставление нового размера для сферических граней. \en Under development. Set new size for spherical faces.
  dmm_SphereRadiusSetNoAdaptation   = 54,  ///< \ru В разработке. Выставление нового размера для сферических граней без адаптации прилегающих скруглений. \en Under development. Set new size for spherical faces without the adjacent fillets adaptation.

  dmm_ConvertFacesToNurbs      = 60,  ///< \ru Замена выбранных граней тела деформируемыми гранями (превращение в NURBS для редактирования). \en Replacement of the specified faces of a solid with a deformable faces (conversion to NURBS for editing).
  
  dmm_PurifyFillets            = 70,  ///< \ru Удаление из тела выбранных скруглений. \en Removal of the specified fillets from a solid.
  dmm_PurifyFilletsChains      = 71,  ///< \ru Удаление из тела цепочек скруглений гладко стыкующихся с выбранными скруглениями. \en Removal of the links of fillets smoothly connected to the chosen fillets.
  dmm_PurifyFilletsBelowRadius = 72,  ///< \ru Удаление из тела скруглений, меньших заданного радиуса. \en Remove the fillets with radius less then specified radius from a solid.
  
  //dmm_PurifyFilletsInRange    = 80,  ///< \ru Пока не реализовано. Удаление из тела скруглений определённого радиуса. \en Not implemented. Remove the fillets with specific radius from a solid.
  
  dmm_DeleteEdges              = 90,  ///< \ru Слияние вершин ребёр и удаление рёбер. \en Merging vertices of edges and edges removal.
  
  dmm_UniteFaces               = 100, ///< \ru Замена гладко стыкующихся граней одной гранью. \en Replacing smoothly joined faces with one face.
  
  dmm_RotateFaces              = 110, ///< \ru Поворот выбранных граней тела с адаптацией прилегающих скруглений. \en Rotate the chosen faces with the adjacent fillets adaptation.
  dmm_RotateFacesNoFilletAdaptation = 111, ///< \ru Поворот выбранных граней тела без адаптации прилегающих скруглений. \en Rotate the chosen faces without the adjacent fillets adaptation.

  dmm_FilletsRadiusChange      = 120, ///< \ru Изменение радиусов выбранных граней скругления на заданную величину. \en Change of radii of the specified fillet faces for the set value.
  
  dmm_FilletsRadiusSet         = 130, ///< \ru Установка радиусов выбранных граней скругления для цепочки гладко состыкованных скруглений. \en Set the radii for the specified smoothly joined fillet faces chain.
  dmm_FilletsRadiusSetSingle   = 131, ///< \ru Установка радиусов выбранных граней скругления. \en Set the radii for the specified fillet faces.

  dmm_ReplaceFaces             = 140, ///< \ru В разработке. Замена граней тела. \en Under development. Replacement faces of a solid.
  dmm_ReplaceGroupFaces        = 141, ///< \ru В разработке. Замена группы граней тела. \en Under development. Replacement group of faces of a solid.
};


//------------------------------------------------------------------------------
/** \brief \ru Старые параметры прямого редактирования тела.
           \en Old parameters for a solid direct modification. \~
  \details
  \ru Параметры прямого редактирования тела содержат тип операции (#MbeModifyingType) и её параметры.
      В зависимости от параметра way возможны следующие действия: \n
      1.  Удаление из тела выбранных граней с окружением (way==dmt_Remove).
          По-умолчанию грани удаляются вместе с прилегающими скруглениями.
          Для удаления без прилегающих скруглений выставить флаг removeWithFillets = false.\n
      2.  Создание тела из выбранных граней с окружением (way==dmt_Create). \n
      3.  Перемещение выбранных граней с окружением относительно оставшихся граней тела (way==dmt_Action).
          Направление и величину перемещения определяет вектор direction. \n
      4.  Замена выбранных граней тела эквидистантными гранями (перемещение по нормали, изменение радиуса)
          (way==dmt_Offset). Расстояние смещения определяется параметром value или длиной вектора direction.\n
      5.  Изменение радиуса выбранных граней скругления (way==dmt_Fillet).
          Величина изменения радиуса определяется параметром value или длиной вектора direction.\n
      6.  Замена выбранных граней тела деформируемыми гранями (превращение в NURBS) для редактирования (way==dmt_Supple). \n
      7.  Удаление выбранных граней скругления тела (way==dmt_Purify). \n
      8.  Слияние вершин ребёр и удаление рёбер (way==dmt_Merger). \n
      9.  Замена гладко стыкующихся граней одной гранью (way==dmt_United). \n
      10. Поворот выбранных граней (way==dmt_Rotate). Угол поворота определяется параметром value,
          ось поворота определяется точкой origin и направлением direction.\n

  \en Parameters for a solid direct modification include the type of operation (#MbeModifyingType) and its parameters.
      The possible operations depend on the 'way' parameter and are listed below: \n
      1.  Removal of the specified faces with the neighborhood from a solid (way==dmt_Remove).
          By default the faces are removed with the connected fillets.
          To remove without connected fillets set the flag removeWithFillets = false.\n
      2.  Creation of a solid from the specified faces with the neighborhood (way==dmt_Create). \n
      3.  Translation of the specified faces with neighborhood relative to the other faces of the solid (way==dmt_Action).
          The translation direction is defined by the 'direction' vector.\n
      4.  Replacement of the specified faces by offset faces (way==dmt_Offset).
          The distance of the offset (or radius change) is defined by 'value' or by the length of 'direction'. \n
      5.  Radius change of the specified fillet faces (way==dmt_Fillet).
          The value of radius change is defined by 'value' or by the length of 'direction'.\n
      6.  Replacement of the specified faces of a solid with a deformable NURBS faces for editing (way==dmt_Supple). \n 
      7.  Removal of the specified fillet faces from a solid (way==dmt_Purify). \n
      8.  Edge vertices merging and edges removal (way==dmt_Merger). \n
      9.  Replacement of the smoothly joined faces with a single face (way==dmt_United). \n
      10. Rotation of the chosen faces (way==dmt_Rotate). The rotation angle is defined by the 'value',
          the rotation axis is defined by the point 'origin' and 'direction' vector.\n \~
  \deprecated \ru Класс устарел и будет удален в версии 2024. Взамен использовать MbModifiedSolidParams.
              \en The class is deprecated and will be removed in version 2024. Use MbModifiedSolidParams intead.\~
  \ingroup Direct_Building_Parameters
*/
// ---
struct MATH_CLASS ModifyValues {
public:
  MbeModifyingType way;       ///< \ru Тип модификации. \en Type of modification.
  MbVector3D       direction; ///< \ru Перемещение при модификации. \en Moving when modifying.
  MbCartPoint3D    origin;    ///< \ru Точка опоры при модификации. \en Fulcrum when modifying.
  double           value;     ///< \ru Величина смещения/изменение радиуса. \en Offset value/change of radius.
  double           tolerance; ///< \ru Точность построения. \en Operation tolerance.
  bool             removeWithFillets; ///< \ru Удалять ребро вместе с прилегающими скруглениями. \en Whether to delete a face toghether with adjacent fillets.

public:
  /** \brief \ru Конструктор по умолчанию.
             \en Default constructor. \~
    \details \ru Конструктор параметров операции удаления из тела выбранных граней.
             \en Constructor of operation parameters of removing the specified faces from the solid. \~
  */
  ModifyValues()
    : way      ( dmt_Remove )
    , direction( 0.0, 0.0, 0.0 )
    , origin   ( 0.0, 0.0, 0.0 )
    , value    ( 0.0 )
    , tolerance( Math::metricAccuracy )
    , removeWithFillets ( true )
  {}
  /// \ru Конструктор по способу модификации и вектору перемещения. \en Constructor by way of modification and movement vector.
  ModifyValues( MbeModifyingType w, const MbVector3D & p )
    : way      ( w )
    , direction( p )
    , origin   ( 0.0, 0.0, 0.0 )
    , value    ( 0.0 )
    , tolerance( Math::metricAccuracy )
    , removeWithFillets ( true )
  {}
  /// \ru Конструктор по способу модификации и скалярному параметру. \en Constructor by way of modification and the scalar value.
  ModifyValues( MbeModifyingType w, double val, double eps = Math::metricAccuracy )
    : way      ( w )
    , direction( 0.0, 0.0, 0.0 )
    , origin   ( 0.0, 0.0, 0.0 )
    , value    ( val )
    , tolerance( eps )
    , removeWithFillets ( true )
  {}
  /// \ru Конструктор по способу модификации и скалярному параметру. \en Constructor by way of modification and the scalar value.
  ModifyValues( MbeModifyingType w, double val, bool removeAdjacentFillets )
    : way      ( w )
    , direction( 0.0, 0.0, 0.0 )
    , origin   ( 0.0, 0.0, 0.0 )
    , value    ( val )
    , tolerance( Math::metricAccuracy )
    , removeWithFillets ( removeAdjacentFillets )
  {}
  /// \ru Конструктор копирования. \en Copy-constructor.
  ModifyValues( const ModifyValues & other )
    : way      ( other.way )
    , direction( other.direction )
    , origin   ( other.origin )
    , value    ( other.value )
    , tolerance( other.tolerance )
    , removeWithFillets ( other.removeWithFillets)
  {}
  /// \ru Деструктор. \en Destructor.
  ~ModifyValues() {}
public:
  /// \ru Функция копирования. \en Copy function.
  void        Init( const ModifyValues & other ) {
    way       = other.way;
    direction = other.direction;
    origin    = other.origin;
    value     = other.value;
    tolerance = other.tolerance;
    removeWithFillets = other.removeWithFillets;
  }
  /// \ru Оператор присваивания. \en Assignment operator.
  ModifyValues & operator = ( const ModifyValues & other ) {
    way       = other.way;
    direction = other.direction;
    origin    = other.origin;
    value     = other.value;
    tolerance = other.tolerance;
    removeWithFillets = other.removeWithFillets;
    return *this;
  }
  /// \ru Преобразовать объект согласно матрице. \en Transform an object according to the matrix.
  void        Transform( const MbMatrix3D & matr );
  /// \ru Сдвинуть объект вдоль вектора. \en Move an object along a vector.
  void        Move     ( const MbVector3D & to );
  /// \ru Повернуть объект вокруг оси на заданный угол. \en Rotate an object at a given angle around an axis.
  void        Rotate   ( const MbAxis3D & axis, double ang );
  /// \ru Являются ли объекты равными? \en Determine whether an object is equal? 
  bool        IsSame( const ModifyValues & other, double accuracy ) const; 

  /// \ru Перемещение при модификации. \en Moving when modifying.
  const MbVector3D & GetDirection() const { return direction; }
  void        SetDirection( const MbVector3D & d ) { direction = d; }
  /// \ru Точка опоры при модификации. \en Fulcrum when modifying.
  const MbCartPoint3D & GetOrigin() const { return origin; }
  void        SetOrigin( const MbCartPoint3D & p ) { origin = p; }
  /// \ru Величина смещения/изменение радиуса. \en Offset value/change of radius.
  double      GetValue() const { return value; }
  void        SetValue( double v ) { value = v; }
  /// \ru Точность построения. \en Operation tolerance.
  double      GetTolerance() const { return tolerance; }
  void        SetTolerance( double t ) { tolerance = ::fabs( t ); }
  /// \ru Установить флаг удаления сопряжённых скруглений. \en Set the flag to remove adjacent fillets.
  void        SetRemoveFillets( bool remove ) { removeWithFillets = remove; }

  KNOWN_OBJECTS_RW_REF_OPERATORS( ModifyValues ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class.
};


// Forward declaration for the deprecated friend function
MATH_FUNC (MbCreator *) CreateFaceModifiedSolid(       MbFaceShell *          outer, 
                                                       MbeCopyMode            sameShell, 
                                                 const ModifyValues &         oldParams, 
                                                 const RPArray<MbFace> &      faces, 
                                                 const RPArray<MbCurveEdge> & edges, 
                                                 const MbSNameMaker &         names, 
                                                       MbResultType &         res,
                                                       MbFaceShell *&         shell );
// Forward declaration for the deprecated friend function
MATH_FUNC (MbResultType) EdgeModifiedSolid(       MbSolid &         solid, 
                                                  MbeCopyMode       sameShell,
                                            const ModifyValues &    oldParams,
                                            const RPArray<MbCurveEdge> & edges,
                                            const MbSNameMaker &    names,
                                                  MbSolid *&        result );
// Forward declaration for the deprecated friend function
MATH_FUNC (MbResultType) FaceModifiedSolid(       MbSolid &         solid, 
                                                  MbeCopyMode       sameShell,
                                            const ModifyValues &    params,
                                            const RPArray<MbFace> & faces, 
                                            const MbSNameMaker &    names,
                                                  MbSolid *&        result );


//------------------------------------------------------------------------------
/** \brief \ru Параметры для подсветки граней для прямого редактирования тела.
           \en Parameters for faces highlighting for solid direct modification. \~
  \details
    \ru Параметры содержат тип операции (#MbeModifyingMode) и параметры, необходимые для подсветки.
    \en Parameters include the type of operation (#MbeModifyingMode) and parameters necessary for faces highligting. \~
  \ingroup Direct_Building_Parameters
*/
// ---
class MATH_CLASS MbCollectFacesParams : public MbPrecision
{
protected:
  MbeModifyingMode _modificationType;          ///< \ru Тип модификации. \en Type of modification.
  std::shared_ptr<DirectModValues> _modValues; ///< \ru Уникальные параметры в зависимости от типа операции. \en The unique values, depending on the operation.
  VERSION _version;                            ///< \ru Версия операции. \en The operation version.

protected:
  /** \brief   \ru Внутренний конструктор.
               \en Inner constructor. \~
      \details \ru Внутренний конструктор по параметрам.
               \en Inner constructor by parameters. \~
      \param[in] type  - \ru Тип модификации.
                         \en Modification type.\~
      \param[in] version - \ru Версия операции.
                           \en The operation version. \~
  */
  MbCollectFacesParams( MbeModifyingMode type, VERSION version = Math::DefaultMathVersion() );

public:
  /** \brief   \ru Конструктор.
               \en Constructor. \~
      \details \ru Конструктор по параметрам.
               \en Constructor by parameters. \~
      \param[in] modType - \ru Тип модификации.
                           \en Modification type.\~
      \param[in] faces -   \ru Список граней для модификации.
                           \en Faces for the modification. \~
      \param[in] version - \ru Версия операции.
                           \en The operation version. \~
  */
  MbCollectFacesParams(        MbeModifyingMode   modType,
                         const c3d::ItemIndices & faces,
                               VERSION            version = Math::DefaultMathVersion() );

  /** \brief   \ru Конструктор.
               \en Constructor. \~
      \details \ru Конструктор по параметрам.
               \en Constructor by parameters. \~
      \param[in] type  - \ru Тип модификации.
                         \en Modification type.\~
      \param[in] faces - \ru Список граней для модификации.
                         \en Faces for the modification. \~
      \param[in] value - \ru Величина максимального радиуса для dmm_Remove* и dmm_Purify*.
                         \en Max value of a radius for dmm_Remove* and dmm_Purify*. \~
      \param[in] version - \ru Версия операции.
                           \en The operation version. \~
  */
  MbCollectFacesParams(        MbeModifyingMode   type,
                         const c3d::ItemIndices & faces,
                               double             value,
                               VERSION            version = Math::DefaultMathVersion() );

  ~MbCollectFacesParams(){} ///< \ru Деструктор. \en Destructor.

protected:
  /// \ru Выдать значения запрошенного типа без проверок. \en Get the values of the requested type.
  template<class DMValues>
  std::shared_ptr<DMValues> _GetValues() const
  {
    return std::static_pointer_cast<DMValues>( _modValues );
  };

public:
  /** \brief   \ru Добавить грань в параметры.
               \en Add face to values. \~
      \details \ru Добавить грань в параметры с проверкой на повторы.
               \en Add face to values wihout repeats. \~
      \param[in] face - \ru Индекс грани.
                        \en Face pointer.\~
  */
  void AddFace( const MbItemIndex & face );
  /** \brief   \ru Добавить грани в параметры.
               \en Add faces to values. \~
      \details \ru Добавить грани в параметры с проверкой на повторы.
               \en Add faces to values wihout repeats. \~
      \param[in] faces - \ru Массив указателей на грани тела.
                         \en Array of faces pointers.\~
  */
  void AddFaces( const c3d::ItemIndices & faces )
  {
    for ( const auto & face : faces )
      AddFace ( face );
  }

  /// \ru Выдать значения запрошенного типа. \en Get the values of the requested type.
  template<class DMValues>
  MATH_FUNC( std::shared_ptr<DMValues> ) GetValues() const;

  /** \brief   \ru Валидны ли созданные параметры.
               \en Whether the created parameters are valid.\~
      \details \ru Валидны ли созданные параметры для заданной оболочки. Проверяется массив присланных объектов (граней, рёбер, поверхностей),
                   установленные значения операции, соответствие типа и параметров.
               \en Whether the created parameters are valid for the specified shell. Checks the input objects arrays (faces, edges, sufaces),
                   input operation values, consistency between type and parameters.\~
      \param[in] shell - \ru Указатель на оболочку, на которой будет производиться операция.
                         \en Pointer to the to-be-modified shell.\~
      \return \ru Возвращает "true", если параметры валидны.
              \en Returns "true" if parameters are valid.\~
  */
  bool IsValid( const MbFaceShell * shell = nullptr ) const;

  /// \ru Выдать тип модификации. \en Get the modification type.
  MbeModifyingMode GetType() const { return _modificationType; }

  /// \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool IsSame( const MbCollectFacesParams & other, double accuracy ) const;

  OBVIOUS_PRIVATE_COPY( MbCollectFacesParams );
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры прямого редактирования тела.
           \en Parameters for a solid direct modification. \~
  \details
  \ru Параметры прямого редактирования тела содержат тип операции (#MbeModifyingMode) и её параметры.
      В зависимости от параметра _modificationType возможны следующие действия: \n
      1.  Удаление из тела выбранных граней без прилегающих скруглений dmm_RemoveFaces.\n
          Для удаления грани вместе с прилегающими скруглениями dmm_RemoveFacesWithAdjacentFillets.\n
          Удаление замкнутых цилиндрических, сферических граней, граней вращения и выдавливания радиуса меньше заданного
          без прилегающих скруглений dmm_RemoveBelowRadius.\n
          Удаление замкнутых цилиндрических, сферических граней, граней вращения и выдавливания радиуса меньше заданного
          вместе с прилегающими скруглениями dmm_RemoveBelowRadiusWithAdjacentFillets. \n
      2.  Создание тела из выбранных граней с окружением dmm_Create. \n
      3.  Перемещение выбранных граней с окружением относительно оставшихся граней тела dmm_MoveFacesByVector 
          или dmm_MoveFacesByVectorNoAdaptation без адаптации скруглений.
          Направление и величину перемещения определяет вектор direction. \n
      4.  Замена выбранных граней тела эквидистантными гранями (перемещение по нормали, изменение радиуса)
          dmm_OffsetFaces. Расстояние смещения определяется параметром value.\n
      5.  Изменение радиуса выбранных граней скругления на заданную величину dmm_FilletsRadiusChange.
          Величина изменения радиуса определяется параметром value.\n
      6.  Замена выбранных граней тела деформируемыми гранями (превращение в NURBS) для редактирования dmm_ConvertFacesToNurbs. \n
      7.  Удаление выбранных граней скругления тела dmm_PurifyFillets.
          Для удаления также всех гладко прилегающих скруглений _modificationType==dmm_PurifyFilletsChains.
          Для удаления всех скруглений тела, меньших определённого радиуса _modificationType==dmm_PurifyFilletsBelowRadius.\n
      8.  Слияние вершин ребёр и удаление рёбер dmm_DeleteEdges. \n
      9.  Замена гладко стыкующихся граней одной гранью dmm_UniteFaces. \n
      10. Поворот выбранных граней dmm_RotateFaces с адаптацией прилегающих скруглений
          или dmm_RotateFacesNoFilletAdaptation без адаптации. Угол поворота определяется параметром angle,
          ось поворота определяется осью axis.\n
      11. Замена выбранных граней тела dmm_ReplaceFaces.\n
      12. Установить радиус цилиндрических граней dmm_CylinderRadiusSet с адаптацией прилегающих скруглений
          или dmm_CylinderRadiusSetNoAdaptation без адаптации.
          Новый радиус граней определяется параметром value. \n
      13. Установить радиус сферических граней dmm_SphereRadiusSet с адаптацией прилегающих скруглений
          или dmm_dmm_SphereRadiusSetNoAdaptation без адаптации.
          Новый радиус граней определяется параметром value. \n

  \en Parameters for a solid direct modification include the type of operation (#MbeModifyingMode) and its parameters.
      The possible operations depend on the '_modificationType' parameter and are listed below: \n
      1.  Removal of the specified faces without the adjacent fillets dmm_RemoveFaces.\n
          Removal of the specified faces with the adjacent fillets dmm_RemoveFacesWithAdjacentFillets.
          Remove the close cylindric, spherical faces, extrusion and rotation faces below specified radius
          with the adjacent fillets dmm_RemoveBelowRadiusWithAdjacentFillets.\n
          Remove the close cylindric, spherical faces, extrusion and rotation faces below specified radius
          without the adjacent fillets dmm_RemoveBelowRadius.\n
      2.  Creation of a solid from the specified faces with the neighborhood dmm_Create. \n
      3.  Translation of the specified faces with neighborhood relative to the other faces of the solid dmm_MoveFacesByVector or
          dmm_MoveFacesByVectorNoAdaptation without fillet adaptation.
          The translation direction is defined by the 'direction' vector.\n
      4.  Replacement of the specified faces by offset faces dmm_OffsetFaces.
          The distance of the offset (or radius change) is defined by 'value'. \n
      5.  Radius change of the specified fillet faces dmm_FilletChange.
          The value of radius change is defined by 'value'.\n
      6.  Replacement of the specified faces of a solid with a deformable NURBS faces for editing dmm_ConvertFacesToNurbs. \n 
      7.  Remove of the specified fillet faces from a solid dmm_PurifyFillets.
          Remove of the specified fillet faces with all smoothly connected fillets _modificationType==dmm_PurifyFilletsChains.
          Remove all fillets below specified radius from a solid _modificationType==dmm_PurifyFilletsBelowRadius.\n
      8.  Edge vertices merging and edges removal dmm_DeleteEdges. \n
      9.  Replacement of the smoothly joined faces with a single face dmm_UniteFaces. \n
      10. Rotation of the chosen faces dmm_RotateFaces with the adjacent fillets adaptation
          or dmm_RotateFacesNoFilletAdaptation without the adaptation. The parameters are the rotation angle
          and the rotation axis.\n
      11. Replacement of the specified faces of a solid dmm_ReplaceFaces.\n
      12. Set the radius of cylindrical faces dmm_CylinderRadiusSet with the adjacent fillet adaptation
          or dmm_CylinderRadiusSetNoAdaptation without the adaptation.
          New radius for the faces is defined by the "value" parameter. \n
      13. Set the radius of spherical faces dmm_SphereRadiusSet with the adjacent fillet adaptation
          or dmm_SphereRadiusSetNoAdaptation without the adaptation.
          New radius for the faces is defined by the "value" parameter. \n
          \~
  \ingroup Direct_Building_Parameters
*/
// ---
class MATH_CLASS MbModifiedSolidParams : public MbCollectFacesParams
{
protected:
  mutable SPtr<MbSNameMaker> _names; ///< \ru Именователь. \en Name maker.
  bool _hotPointsOn;                 ///< \ru Флаг расчета хот точек. \en Hot points calculation flag.

private:
  MbModifiedSolidParams();
  MbModifiedSolidParams( MbeModifyingMode type );
  MbModifiedSolidParams( MbeModifyingMode type, const MbSNameMaker & names );

/** \ru \name Конструкторы без набора граней.
    \en \name Constructors without faces array.
    \{ */
  /** \brief   \ru Конструктор.
               \en Constructor. \~
      \details \ru Конструктор по параметрам.
               \en Constructor by parameters. \~
      \param[in] type  - \ru Тип модификации. Для данного конструктора доступны: dmm_OffsetFaces, dmm_FilletChange.
                         \en Modification type. Available types for this constructor are: dmm_OffsetFaces, dmm_FilletChange. \~
      \param[in] value - \ru Величина сдвига для dmm_OffsetFaces или величина изменения радиуса для dmm_FilletChange.
                         \en Offset value for dmm_OffsetFaces or calue of fillet radius change for dmm_FilletChange. \~
      \param[in] names - \ru Именователь операции.
                         \en An object defining names generation in the operation. \~
  */
  MbModifiedSolidParams(       MbeModifyingMode type,
                         const MbSNameMaker &   names,
                               double           value );

  /** \brief   \ru Конструктор.
               \en Constructor. \~
      \details \ru Конструктор по параметрам.
               \en Constructor by parameters. \~
      \param[in] type -  \ru Тип модификации. Для данного конструктора доступны: dmm_RotateFaces, dmm_RotateFacesNoFilletAdaptation.
                         \en Modification type. Available types for this constructor are: dmm_RotateFaces, dmm_RotateFacesNoFilletAdaptation. \~
      \param[in] axis -  \ru Ось вращения.
                         \en Rotation axis. \~
      \param[in] angle - \ru Угол вращения.
                         \en Rotation angle. \~
      \param[in] names - \ru Именователь операции.
                         \en An object defining names generation in the operation. \~
  */
  MbModifiedSolidParams(       MbeModifyingMode type,
                         const MbSNameMaker &   names,
                         const MbAxis3D &       axis,
                               double           angle );

  /** \brief   \ru Конструктор.
               \en Constructor. \~
      \details \ru Конструктор по параметрам.
               \en Constructor by parameters. \~
      \param[in] type -  \ru Тип модификации. Для данного конструктора доступны: dmm_MoveFacesByVector, dmm_MoveFacesByVectorNoAdaptation.
                         \en Modification type. Available types for this constructor are: dmm_MoveFacesByVector, dmm_MoveFacesByVectorNoAdaptation. \~
      \param[in] direction - \ru Направление перемещения.
                             \en Direction for the move. \~
      \param[in] names - \ru Именователь операции.
                         \en An object defining names generation in the operation. \~
  */
  MbModifiedSolidParams(       MbeModifyingMode type,
                         const MbSNameMaker &   names,
                         const MbVector3D &     direction );
/** \} */

public:
  MbModifiedSolidParams( const MbModifiedSolidParams & other, MbRegDuplicate * iReg = nullptr ); ///< \ru Конструктор копирования. \en Copy contructor.
  MbModifiedSolidParams( TapeInit );  ///< \ru Конструктор для чтения. \en The read constructor.
/** \ru \name Конструкторы по набору граней.
    \en \name Constructors by faces array.
    \{ */

  /** \brief   \ru Конструктор.
               \en Constructor. \~
      \details \ru Конструктор по параметрам.
               \en Constructor by parameters. \~
      \param[in] modType - \ru Тип модификации. Для данного конструктора доступны: dmm_UniteFaces, dmm_Create, dmm_ConvertFacesToNurbs, dmm_PurifyFillets*.
                           \en Modification type. Available types for this constructor are:  dmm_UniteFaces, dmm_Create, dmm_ConvertFacesToNurbs, dmm_PurifyFillets*. \~
      \param[in] names   - \ru Именователь операции.
                           \en An object defining names generation in the operation. \~
      \param[in] faces -   \ru Список граней для модификации.
                           \en Faces for the modification. \~
  */
  MbModifiedSolidParams(       MbeModifyingMode     modType,
                         const MbSNameMaker &       names,
                         const c3d::ItemIndices &   faces );

  /** \brief   \ru Конструктор.
               \en Constructor. \~
      \details \ru Конструктор по параметрам.
               \en Constructor by parameters. \~
      \param[in] type  - \ru Тип модификации. Для данного конструктора доступны: dmm_OffsetFaces, dmm_FilletChange,
                              dmm_RemoveBelowRadius, dmm_RemoveBelowRadiusWithAdjacentFillets, dmm_PurifyFilletsBelowRadius,
                              dmm_CylinderRadiusSet, dmm_CylinderRadiusSetNoAdaptation, dmm_SphereRadiusSet, dmm_SphereRadiusSetNoAdaptation.
                         \en Modification type. Available types for this constructor are: dmm_OffsetFaces, dmm_FilletChange,
                              dmm_RemoveBelowRadius, dmm_RemoveBelowRadiusWithAdjacentFillets, dmm_PurifyFilletsBelowRadius.
                              dmm_CylinderRadiusSet, dmm_CylinderRadiusSetNoAdaptation, dmm_SphereRadiusSet, dmm_SphereRadiusSetNoAdaptation.\~
      \param[in] names - \ru Именователь операции.
                         \en An object defining names generation in the operation. \~
      \param[in] faces - \ru Список граней для модификации.
                         \en Faces for the modification. \~
      \param[in] value - \ru Величина сдвига для dmm_OffsetFaces,
                             величина изменения радиуса для dmm_FilletChange,
                             величина максимального радиуса для dmm_Remove* и dmm_Purify*.
                             величина нового радиуса граней для dmm_CylinderRadiusSet и dmm_SphereRadiusSet.
                         \en Offset value for dmm_OffsetFaces,
                             value of fillet radius change for dmm_FilletChange,
                             max value of a radius for dmm_Remove* and dmm_Purify*. 
                             new radius value for dmm_CylinderRadiusSet or dmm_SphereRadiusSet.\~
  */
  MbModifiedSolidParams(       MbeModifyingMode   type,
                         const MbSNameMaker &     names,
                         const c3d::ItemIndices & faces,
                               double             value )
    : MbModifiedSolidParams( type, names, value )
  {
    AddFaces( faces );
  }

  /** \brief   \ru Конструктор.
               \en Constructor. \~
      \details \ru Конструктор по параметрам.
               \en Constructor by parameters. \~
      \param[in] type -  \ru Тип модификации. Для данного конструктора доступны: dmm_RotateFaces, dmm_RotateFacesNoFilletAdaptation.
                         \en Modification type. Available types for this constructor are: dmm_RotateFaces, dmm_RotateFacesNoFilletAdaptation. \~
      \param[in] names - \ru Именователь операции.
                         \en An object defining names generation in the operation. \~
      \param[in] faces - \ru Список граней для модификации.
                         \en Faces for the modification. \~
      \param[in] axis -  \ru Ось вращения.
                         \en Rotation axis. \~
      \param[in] angle - \ru Угол вращения.
                         \en Rotation angle. \~
  */
  MbModifiedSolidParams(       MbeModifyingMode   type,
                         const MbSNameMaker &     names,
                         const c3d::ItemIndices & faces,
                         const MbAxis3D &         axis,
                               double             angle )
    : MbModifiedSolidParams( type, names, axis, angle )
  {
    AddFaces( faces );
  }

  /** \brief   \ru Конструктор.
               \en Constructor. \~
      \details \ru Конструктор по параметрам.
               \en Constructor by parameters. \~
      \param[in] type -  \ru Тип модификации. Для данного конструктора доступны: dmm_MoveFacesByVector.
                         \en Modification type. Available types for this constructor are: dmm_MoveFacesByVector. \~
      \param[in] names - \ru Именователь операции.
                         \en An object defining names generation in the operation. \~
      \param[in] faces - \ru Список граней для модификации.
                         \en Faces for the modification. \~
      \param[in] direction - \ru Направление сдвига.
                             \en Shiftdirection. \~
  */
  MbModifiedSolidParams(       MbeModifyingMode   type,
                         const MbSNameMaker &     names,
                         const c3d::ItemIndices & faces,
                         const MbVector3D &       direction )
    : MbModifiedSolidParams( type, names, direction )
  {
    AddFaces( faces );
  }

  /** \brief   \ru Конструктор.
               \en Constructor. \~
      \details \ru Конструктор по параметрам.
               \en Constructor by parameters. \~
      \param[in] type -  \ru Тип модификации. Для данного конструктора доступны: dmm_ConvertFacesToNurbs.
                         \en Modification type. Available types for this constructor are: dmm_ConvertFacesToNurbs. \~
      \param[in] names - \ru Именователь операции.
                         \en An object defining names generation in the operation. \~
      \param[in] faces - \ru Список граней для модификации.
                         \en Faces for the modification. \~
      \param[in] surfaces - \ru Поверхности замены (может быть пустым массивом).
                            \en Surfaces for the change (may be empty). \~
  */
  template<class SurfArray>
  MbModifiedSolidParams(       MbeModifyingMode   type,
                         const MbSNameMaker &     names,
                         const c3d::ItemIndices & faces,
                         const SurfArray &        surfaces )
    : MbModifiedSolidParams( type, names )
  {
    AddFaces( faces );
    AddSurfaces( surfaces );
  }

  /** \brief   \ru Конструктор.
               \en Constructor. \~
      \details \ru Конструктор по параметрам.
               \en Constructor by parameters. \~
      \param[in] type -                \ru Тип модификации. Для данного конструктора доступны: dmm_ReplaceFaces.
                                       \en Modification type. Available types for this constructor are: dmm_ReplaceFaces. \~
      \param[in] names -               \ru Именователь операции.
                                       \en An object defining names generation in the operation. \~
      \param[in] replacedFaces -       \ru Список заменяемых граней.
                                       \en Faces to be replaced. \~
      \param[in] replacementSurfaces - \ru Список заменяющих поверхностей.
                                       \en Replacement surfaces. \~
      \param[in] surfaceParts -        \ru Список номеров частей поверхностей, которые будут использоваться при нескольких возможных вариантах замены.
                                       \en An array of part numbers of surfaces to be used in several possible replacement options. \~
      \param[in] sameSurfaces -        \ru Использовать исходные поверхности (true) или их копии (false).
                                       \en Use the source surfaces (true) or its copies (false). \~
      \param[in] offsets -             \ru Список сдвигов от заменяющих поверхностей.
                                       \en Offsets from replacement surfaces. \~
  */
  template<class SurfArray>
  MATH_FUNC_EX MbModifiedSolidParams(       MbeModifyingMode     type,
                                      const MbSNameMaker &       names,
                                      const c3d::ItemIndices &   replacedFaces,
                                      const SurfArray &          replacementSurfaces,
                                            bool                 sameSurfaces,
                                      const c3d::NumbersVector & surfaceParts,
                                      const c3d::DoubleVector &  offsets );
  //
/** \} */

  /** \brief   \ru Конструктор.
               \en Constructor. \~
      \details \ru Конструктор по параметрам.
               \en Constructor by parameters. \~
      \param[in] modType - \ru Тип модификации. Для данного конструктора доступны: dmm_DeleteEdges.
                           \en Modification type. Available types for this constructor are: dmm_DeleteEdges. \~
      \param[in] names - \ru Именователь операции.
                         \en An object defining names generation in the operation. \~
      \param[in] edges -   \ru Список рёбер для модификации.
                           \en Edges for the modification. \~
      \param[in] beginOrEnd - \ru Удалять начало или конец соответствующего ребра.
                              \en Delete the beginning or the end of the corresponding edge. \~
  */
  MbModifiedSolidParams(       MbeModifyingMode        modType,
                         const MbSNameMaker &          names,
                         const c3d::EdgeFacesIndices & edges,
                         const c3d::BoolVector &       beginOrEnd )
    : MbModifiedSolidParams( modType, names )
  {
    AddEdges( edges, beginOrEnd );
  }

   /** \brief   \ru Конструктор.
                \en Constructor. \~
      \details \ru Конструктор по параметрам.
               \en Constructor by parameters. \~
      \param[in] type -                \ru Тип модификации. Для данного конструктора доступны: dmm_ReplaceGroupFaces.
                                       \en Modification type. Available types for this constructor are: dmm_ReplaceGroupFaces. \~
      \param[in] names -               \ru Именователь операции.
                                       \en An object defining names generation in the operation. \~
      \param[in] solid -               \ru Тело с заменяющими гранями.
                                       \en Solid with replacement faces. \~
      \param[in] replacedFacesInd -    \ru Список индексов заменяемых граней.
                                       \en Replaced faces indicies . \~
      \param[in] replacementFacesInd - \ru Список индексов заменяющих граней из тела с заменяющими гранями.
                                       \en Replacement faces indices from a solid with replacement faces. \~
      \param[in] offset -              \ru Сдвиг заменяющих граней.
                                       \en Replacement faces offset. \~
  */
  MbModifiedSolidParams(       MbeModifyingMode          type, 
                         const MbSNameMaker &            names,
                         const MbSolid &                 solid,
                         const c3d::ItemIndices &        replacedFacesInd,
                         const c3d::ItemIndices &        replacementFacesInd,
                               double                    offset );
  
  ~MbModifiedSolidParams(){} ///< \ru Деструктор. \en Destructor.

private:
/** \ru \name Функции инициализации.
    \en \name Initialization functions.
    \{ */
  bool CreateValues();                               ///< \ru Инициализация при чтении. \en Read Initialization.
  void Init ( double value );                        ///< \ru Инициализация. \en Initialization.
  void Init ( const MbAxis3D & axis, double angle ); ///< \ru Инициализация. \en Initialization.
  void Init ( const MbVector3D & direction );        ///< \ru Инициализация. \en Initialization.
  void Init ( const ModifyValues &               oldParams,
                    MbFaceShell *                inputShell,
              const SArray<MbItemIndex> *        faces,
              const SArray<MbEdgeFacesIndexes> * edges,
                    RPArray<MbSurface> *         surfaces ); ///< \ru Обратная совместимость. \en Backward compatability
/** \} */
  MbModifiedSolidParams & operator = ( const MbModifiedSolidParams & ); ///< \ru Оператор копирования. \en Copy operator.

public:
  void Init ( const MbModifiedSolidParams & other, MbRegDuplicate * iReg = nullptr ); ///< \ru Инициализация. \en Initialization.

  /** \brief   \ru Получить грани из параметров.
               \en Get faces from values. \~
      \details \ru Получить грани из параметров.
               \en Get faces from values. \~
      \param[out] faces - \ru Массив индексов граней в оболочке, имеющиеся добавляются в конец.
                          \en Array of faces indices in shell, existing values are added at the end.\~
  */
  void GetFaces( c3d::ItemIndices & faces ) const;
  /** \brief   \ru Получить грани из параметров.
               \en Get faces from values. \~
      \details \ru Получить грани из параметров.
               \en Get faces from values. \~
      \param[out] faces - \ru Массив индексов граней в оболочке, имеющиеся добавляются в конец.
                          \en Array of faces indices in shell, existing values are added at the end.\~
  */
  void GetFaces( SArray<MbItemIndex> & faces ) const;

  /** \brief   \ru Получить грани из параметров для указанной оболочки.
               \en Get faces from values for the given shell. \~
      \details \ru Получить грани из параметров для указанной оболочки.
               \en Get faces from values for the given shell. \~
      \param[in]  shell - \ru Оболочка, в которой ищем грани.
                          \en Shell, where the faces are searched for.\~
      \param[out] faces - \ru Массив указателей на грани тела, имеющиеся добавляются в конец.
                          \en Array of faces pointers, existing values are added at the end.\~
  */
  template<class FacesArray>
  MATH_FUNC( void ) FindFaces( const MbFaceShell & shell, FacesArray & faces ) const;
  /** \brief   \ru Получить грани из параметров для указанной оболочки.
               \en Get faces from values for the given shell. \~
      \details \ru Получить грани из параметров для указанной оболочки, отсортированные в порядке возрастания индексов.
               \en Get faces from values for the given shell, sorted in ascending index order. \~
      \param[in]  shell - \ru Оболочка, в которой ищем грани.
                          \en Shell, where the faces are searched for.\~
      \param[out] faces - \ru Массив указателей на грани тела, имеющиеся добавляются в конец.
                          \en Array of faces pointers, existing values are added at the end.\~
  */
  template<class FacesArray>
  MATH_FUNC( void ) FindSortedFaces( const MbFaceShell & shell, FacesArray & faces ) const;

/** \ru \name Функции добавления рёбер.
    \en \name Functions to add edges.
    \{ */
  /** \brief   \ru Добавить ребро в параметры.
               \en Add edge to values. \~
      \details \ru Добавить ребро в параметры с проверкой на повторы.
               \en Add edge to values wihout repeats. \~
      \param[in] edge - \ru Индекс ребра.
                        \en Edge index.\~
      \param[in] beginOrEnd - \ru Удалить начало или конец ребра.
                              \en Delete the beginning or the end of the edge.\~
  */
  bool AddEdge( const MbEdgeFacesIndexes & edge, bool beginOrEnd );
  /** \brief   \ru Добавить рёбра в параметры.
               \en Add edges to values. \~
      \details \ru Добавить рёбра в параметры с проверкой на повторы.
               \en Add edges to values wihout repeats. \~
      \param[in] edges - \ru Массив индексов рёбер тела.
                         \en Array of edge indices.\~
      \param[in] beginOrEnds - \ru Массив флагов, удалять начало или конец ребра.
                               \en Array of flags showing whether to delete the beginning or the end of the edge.\~
  */
  bool AddEdges( const c3d::EdgeFacesIndices & edges, const c3d::BoolVector & beginOrEnds );
  /** \brief   \ru Получить рёбра из параметров.
               \en Get edges from values. \~
      \details \ru Получить рёбра из параметров.
               \en Get edges from values. \~
      \param[out] edges - \ru Массив индексов рёбер тела, имеющиеся добавляются в конец..
                          \en Array of edges indices, existing values are added at the end.\~
  */
  void GetEdges( c3d::EdgeFacesIndices & edges, c3d::BoolVector & beginOrEnds ) const;
  /** \brief   \ru Получить рёбра из параметров.
               \en Get edges from values. \~
      \details \ru Получить рёбра из параметров.
               \en Get edges from values. \~
      \param[out] edges - \ru Массив индексов рёбер тела, имеющиеся добавляются в конец..
                          \en Array of edges indices, existing values are added at the end.\~
  */
  void GetEdges( SArray<MbEdgeFacesIndexes> & edges, c3d::BoolVector & beginOrEnds ) const;
/** \} */

/** \ru \name Функции добавления поверхностей.
    \en \name Functions to add surfaces.
    \{ */
  /** \brief   \ru Добавить поверхность для замены граней.
               \en Add surface to change faces. \~
      \details \ru Добавить поверхность для замены граней.
               \en Add surface to change faces. \~
      \param[in] surf - \ru Указатель на поверхность.
                        \en Surface pointer.\~
  */
  bool AddSurface( MbSurface * surf );
  /** \brief   \ru Добавить поверхности для замены граней.
               \en Add surfaces to change faces. \~
      \details \ru Добавить поверхности для замены граней.
               \en Add surfaces to change faces. \~
      \param[in] surfs - \ru Массив указателей на поверхности.
                         \en Surface pointers array.\~
  */
  template <class SurfArray>
  MATH_FUNC( bool ) AddSurfaces( const SurfArray & surfs );
  /** \brief   \ru Получить поверхности для замены граней.
               \en Get surfaces to change faces. \~
      \details \ru Получить поверхности для замены граней.
               \en Get surfaces to change faces. \~
      \param[in] surfs - \ru Массив указателей на поверхности.
                         \en Surface pointers array.\~
  */
  void GetSurfaces( c3d::SurfacesVector & surfs );
/** \} */

  /// \ru Получить именователь операции. \en Get the object defining names generation in the operation.
  const MbSNameMaker & GetNameMaker() const;
  /// \ru Получить именователь операции. \en Get the object defining names generation in the operation.
  MbSNameMaker & SetNameMaker();
  /// \ru Установить именователь операции. \en Set the object defining names generation in the operation.
  void SetNameMaker( const MbSNameMaker & other, bool setVersion = true );

  void Transform( const MbMatrix3D & matr, MbRegTransform * ireg = nullptr ); ///< \ru Преобразовать по матрице. \en Transform by matrix.
  void Move( const MbVector3D &to, MbRegTransform *ireg = nullptr ); ///< \ru Сдвиг. \en Move.
  void Rotate( const MbAxis3D &axis, double angle, MbRegTransform *ireg = nullptr ); ///< \ru Поворот. \en Rotate.

  void GetProperties( MbProperties & properties );       ///< \ru Выдать свойства объекта \en Get properties of the object 
  void SetProperties( const MbProperties & properties ); ///< \ru Записать свойства объекта \en Set properties of the object 

  /// \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool IsSame( const MbModifiedSolidParams & other, double accuracy ) const;

  /// \ru Включен ли расчет хот-точек? \en Is hot points calculation enabled?
  bool AreHotPointsOn() const { return _hotPointsOn; };

  /// \ru Включить или выключить расчет хот-точек. \en Enable or disable hot points calculation.
  void SetHotPointsOn( bool isOn ) { _hotPointsOn = isOn; };

  KNOWN_OBJECTS_RW_REF_OPERATORS( MbModifiedSolidParams ); // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class.
  friend class MATH_CLASS MbFaceModifiedSolid;
  friend MATH_FUNC (MbCreator *) CreateFaceModifiedSolid(       MbFaceShell *          outer, 
                                                                MbeCopyMode            sameShell, 
                                                          const ModifyValues &         oldParams, 
                                                          const RPArray<MbFace> &      faces, 
                                                          const RPArray<MbCurveEdge> & edges, 
                                                          const MbSNameMaker &         names, 
                                                                MbResultType &         res,
                                                                MbFaceShell *&         shell ); 
  friend MATH_FUNC (MbResultType) EdgeModifiedSolid(       MbSolid &         solid, 
                                                           MbeCopyMode       sameShell,
                                                     const ModifyValues &    oldParams,
                                                     const RPArray<MbCurveEdge> & edges,
                                                     const MbSNameMaker &    names,
                                                           MbSolid *&        result );
  friend MATH_FUNC (MbResultType) FaceModifiedSolid(       MbSolid &         solid, 
                                                           MbeCopyMode       sameShell,
                                                     const ModifyValues &    params,
                                                     const RPArray<MbFace> & faces, 
                                                     const MbSNameMaker &    names,
                                                           MbSolid *&        result );
};


//------------------------------------------------------------------------------
/** \brief \ru Результаты прямого редактирования тела.
           \en Results for a solid direct modification. \~
  \details \ru Результаты прямого редактирования тела.
           \en Results for a solid direct modification. \~
  \ingroup Direct_Building_Parameters
*/
// ---
class MATH_CLASS MbModifiedSolidResults : public MbOperationResults
{
public:
  SPtr<MbSolid>            _resSolid;                  ///< \ru Результирующее тело. \en The resulting body.
  double                   _maxDifUnited;              ///< \ru Максимальное отклонение объединённой поверхности от присланных граней (оценочная величина). \en The maximum deviation of the united surfaces from the input surfaces (approximate value).
  c3d::SurfacesSPtrVector  _surfaces;                  ///< \ru Список поверхностей (только для dmm_ConvertFacesToNurbs). \en The list of surfaces (only for dmm_ConvertFacesToNurbs).

private:
  c3d::IndicesVector       _replacementOptionsNumbers; ///< \ru Количество возможных вариантов замены граней. \en Estimated numbers of options for replacing faces.

public:
  // \ru Конструктор. \en Constructor.
  MbModifiedSolidResults()
    : MbOperationResults()
    , _resSolid()
    , _maxDifUnited( -1. )
    , _surfaces()
    , _replacementOptionsNumbers()
  {}
  ~MbModifiedSolidResults() {}
  /** \brief \ru Получить нормаль и хот-точку на первой грани для операции по установке радиусов граням скругления.
             \en Get normal and hot point on first face for operation of setting radius of fillet faces . \~
    \param[in]  pnt - \ru Хот-точка.
                      \en Hot point. \~
    \param[in]  dir - \ru Направление.
                      \en Direction. \~
    \deprecated \ru Метод устарел. \en The method is deprecated. \~
    \return \ru Возвращает "true" в случае успеха.
            \en Returns "true" in case of success. \~
  */
  //DEPRECATE_DECLARE_REPLACE(GetFilletRadiusSetHotPoint with MbAxis3D)
  bool GetFilletRadiusSetHotPoint( MbCartPoint3D & pnt,
                                   MbVector3D &    dir ) const;

  /** \brief \ru Получить нормаль и хот-точку на первой грани для операции по установке радиусов граням скругления.
             \en Get normal and hot point on first face for operation of setting radius of fillet faces . \~
      \details \ru В центре грани скругления берётся точка pointOnFilletFace. Через неё строится
                   плоскость, перпендикулярная поверхности скругления или содержащая линию u(или v) = const.
                   Эта линию является дугой окружности в этой плоскости. Ось circleCenter начинается
                   из центра этой дуги и направлена перпендикулярно этой плоскости. Таким образом, расстояние
                   между началом оси и точкой на грани равно радиусу сругления.
               \en The point 'pointOnFilletFace' is located in the center of the fillet face.
                   There us a plane that contains this point and the u(or v)=const line of the fillet face.
                   This line is an arc on this plane. Axis circleCenter starts from the center of this arc and
                   the direction is perpendicular to the plane. So, the distance between the axis start point and
                   the point on the face equals fillet radius.\~
    \param[out]  pointOnFilletFace - \ru Точка, лежащая на грани скругления нового радиуса.
                                     \en Point on the new radius fillet face. \~
    \param[out]  circleCenter - \ru Ось вдоль оси грани скругления.
                                \en Axis along the fillet axis.\~
    \return \ru Возвращает "true", если хот-точка была рассчитана.
            \en Returns "true" if the hot-point is calculated. \~
  */
  //DEPRECATE_DECLARE_REPLACE(GetFilletRadiusSetHotPoint with a third argument of type double)
  bool GetFilletRadiusSetHotPoint( MbCartPoint3D & pointOnFilletFace,
                                   MbAxis3D &      circleCenter ) const;
  /** \brief \ru Получить нормаль, хот-точку и угол раскрыва скругления на первой грани для операции по установке радиусов граням скругления.
             \en Get normal, hot point and opening angle of the fillet on first face for operation of setting radius of fillet faces . \~
      \details \ru В центре грани скругления берётся точка pointOnFilletFace. Через неё строится
                   плоскость, перпендикулярная поверхности скругления или содержащая линию u(или v) = const.
                   Эта линию является дугой окружности в этой плоскости. Ось circleCenter начинается
                   из центра этой дуги и направлена перпендикулярно этой плоскости. Таким образом, расстояние
                   между началом оси и точкой на грани равно радиусу скругления. Угол раскрыва alpha - угол между
                   радиусами, проведёнными в рассматриваемой плоскости от центра дуги окружности до её граничных точек.
               \en The point 'pointOnFilletFace' is located in the center of the fillet face.
                   There us a plane that contains this point and the u(or v)=const line of the fillet face.
                   This line is an arc on this plane. Axis circleCenter starts from the center of this arc and
                   the direction is perpendicular to the plane. So, the distance between the axis start point and
                   the point on the face equals fillet radius. The opening angle alpha is the angle between the radii
                   drawn in the considered plane from the center of the circular arc to its boundary points. \~
    \param[out]  pointOnFilletFace - \ru Точка, лежащая на грани скругления нового радиуса.
                                     \en Point on the new radius fillet face. \~
    \param[out]  circleCenter - \ru Ось вдоль оси грани скругления.
                                \en Axis along the fillet axis.\~
    \param[out]  alpha - \ru Угол раскрыва скругления.
                         \en Opening angle of the fillet.\~
    \return \ru Возвращает "true", если хот-точка была рассчитана.
            \en Returns "true" if the hot-point is calculated. \~
  */
  bool GetFilletRadiusSetHotPoint( MbCartPoint3D & pointOnFilletFace,
                                   MbAxis3D &      circleCenter,
                                   double   &      alpha ) const;
  
  /** \brief \ru Получить хот-точку и её вектор направления смещения для операции замены граней.
             \en Get hot point and its displacement direction vector for the face replacement operation. \~
      \details \ru В случае успешного выполнения операции хот-точка размещается в центре первой замещающей грани.
                   В случае неудачного выполнения операции хот-точка располагается на линии, соединяющей центр первой заменяемой
                   грани и его проекцию на заменяющую поверхность, при этом смещение учитывается относительно проекции.
               \en If the operation is successful, the hot point is placed in the center of the first replacement face.
                   If the operation fails, the hot point is located on the line connecting the center of the first replaced face
                   and its projection onto the replacement surface, and the offset is taken relative to the projection.\~
    \param[out]  pnt - \ru Хот-точка.
                       \en Hot point. \~
    \param[out]  dir - \ru Вектор направления смещения хот-точки.
                       \en Hot point displacement direction vector.\~
    \return \ru Возвращает "true", если хот-точка была рассчитана.
            \en Returns "true" if the hot-point is calculated. \~
  */
  bool GetReplaceFacesHotPoint( MbCartPoint3D & pnt,
                                MbVector3D &    dir ) const;

  /** \brief \ru Получить хот-точку и её вектор направления смещения для операции замены граней на эквидистантные.
             \en Get hot point and its displacement direction vector for the operation of replacement faces with the offset ones. \~
      \details \ru В случае успешного выполнения операции хот-точка размещается в центре первой эквидистантной грани.
                   В случае неудачного выполнения операции хот-точка располагается на нормали к центру первой заменяемой грани,
                   при этом смещение учитывается относительно этого центра.
               \en If the operation is successful, the hot point is placed in the center of the first equidistant face.
                   If the operation fails, the hot point is located on the normal to the center of the first replaced face,
                   and the offset is taken relative to this center.\~
    \param[out]  pnt - \ru Хот-точка.
                       \en Hot point. \~
    \param[out]  dir - \ru Вектор направления смещения хот-точки.
                       \en Hot point displacement direction vector.\~
    \return \ru Возвращает "true", если хот-точка была рассчитана.
            \en Returns "true" if the hot-point is calculated. \~
  */
  bool GetOffsetFacesHotPoint( MbCartPoint3D & pnt,
                               MbVector3D &    dir ) const;

  /** \brief \ru Получить хот-точку и её вектор направления смещения для операции замены группы граней.
             \en Get hot point and its displacement direction vector for the replacement faces group operation. \~
      \details \ru В случае успешного выполнения операции хот-точка размещается в центре грани, ближайшей к центру заменяющей группы граней.
                   В случае неудачного выполнения операции хот-точка располагается в центре заменяющей группы граней плюс смещение.
               \en If the operation is successful, the hot point is placed in the center of the face, closest to the replacement group faces center.
                   If the operation fails, the hot point is located in the center of a replacement group faces, plus the offset is taken relative to this center.\~
    \param[out]  pnt - \ru Хот-точка.
                       \en Hot point. \~
    \param[out]  dir - \ru Вектор направления смещения хот-точки.
                       \en Hot point displacement direction vector.\~
    \return \ru Возвращает "true", если хот-точка была рассчитана.
            \en Returns "true" if the hot-point is calculated. \~
  */
  bool GetReplaceFacesGroupHotPoint( MbCartPoint3D & pnt,
                                     MbVector3D &    dir ) const;

  /** \brief \ru Получить константную ссылку на массив вариантов замены граней.
             \en Get a constant reference to an array of face replacement options. \~
      \details \ru Получить константную ссылку на массив вариантов замены граней.
               \en Get a constant reference to an array of face replacement options.\~
    \return \ru Возвращает константную сылку.
            \en Returns a constant reference. \~
  */
  const c3d::IndicesVector & GetReplacementOptionsNumbers() const { return _replacementOptionsNumbers; }

  /** \brief \ru Получить ссылку на массив вариантов замены граней.
             \en Get a reference to an array of face replacement options. \~
      \details \ru Получить ссылку на массив вариантов замены граней.
               \en Get a reference to an array of face replacement options.\~
    \return \ru Возвращает сылку.
            \en Returns a reference. \~
  */
        c3d::IndicesVector & SetReplacementOptionsNumbers() { return _replacementOptionsNumbers; }

  // \ru Очистка. \en Reset.
  void Reset()
  {
    _resSolid.reset();
    _surfaces.clear();
    _maxDifUnited = -1.;
    _hotPoint.reset();
    _replacementOptionsNumbers.clear();
  }

  OBVIOUS_PRIVATE_COPY( MbModifiedSolidResults );
};

#endif // __OP_DIRECT_MOD_PARAMETERS_H