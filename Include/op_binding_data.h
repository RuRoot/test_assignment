////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Классы привязки объектов.
         \en Items binding classes. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __OP_BINDING_DATA_H
#define __OP_BINDING_DATA_H

#include <math_define.h>
#include <mb_variables.h>
#include <mb_cart_point3d.h>
#include <attr_hotpoint_attribute.h>
#include <templ_sptr.h>

#include <hash32.h>


class    MbMatrix3D;
class    MbAxis3D;
class    MbCurveEdge;
class    MbFace;
class    MbFaceShell;

struct             MbItemIndex;
struct             MbEdgeFacesIndexes;


namespace c3d // namespace C3D
{
  typedef std::pair<MbItemIndex, MbItemIndex> ItemIndexPair;
  typedef std::vector<MbItemIndex>            ItemIndices;
  typedef std::vector<MbEdgeFacesIndexes>     EdgeFacesIndices;
}


//------------------------------------------------------------------------------
/** \brief \ru Точность построения объектов.
           \en The precision of object construction. \~
  \details \ru Класс содержит метрическую и угловую точность построения объектов. \n
           \en The class contains the metric and angular precision of object construction. \n \~
  \ingroup Build_Parameters
*/
// ---
class MATH_CLASS MbPrecision {
protected:
  double precision; ///< \ru Заданная метрическая точность построения объектов. \en The metric precision of the construction of objects.
  double deviation; ///< \ru Заданная угловая точность построения объектов. \en The angular precision of the construction of objects.

public:
  static const MbPrecision defaultPrecision; ///< \ru Точность по умолчанию (METRIC_PRECISION, ANGLE_REGION); \en Default precision (METRIC_PRECISION, ANGLE_REGION); 

public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  MbPrecision()
  : precision( METRIC_PRECISION )
  , deviation( ANGLE_REGION     )
  {}
  /// \ru Конструктор по заданной точности. \en Constructor by the specified precision.
  MbPrecision( double _precision, double _deviation )
  : precision( _precision )
  , deviation( _deviation )
  {}
  /// \ru Конструктор копирования. \en Copy-constructor.
  MbPrecision( const MbPrecision & other )
  : precision( other.precision )
  , deviation( other.deviation )
  {}
  /// \ru Деструктор. \en Destructor.
  ~MbPrecision() {}

public:
  /** \brief \ru Получить максимальную метрическую толерантность.
             \en Get the maximum metric tolerance. \~
    \details \ru Получить максимальную допустимую метрическую толерантность.\n
             \en Get the maximum allowable metric tolerance.\n \~
    \return \ru Возвращает величину максимальной метрической толерантности.
            \en Returns the value of maximum metric tolerance. \~
  */
  static double GetMaxMetricTolerance();

  /** \brief \ru Получить максимальную угловую толерантность.
             \en Get the maximum angular tolerance. \~
    \details \ru Получить максимальную допустимую угловую толерантность.\n
             \en Get the maximum allowable angular tolerance.\n \~
    \return \ru Возвращает величину максимальной угловой толерантности.
            \en Returns the value of maximum angular tolerance. \~
  */
  static double GetMaxAngleTolerance();

public:
  /// \ru Функция инициализации. \en Initialization function.
  void        Init( const MbPrecision & other ) {
    precision = other.precision;
    deviation = other.deviation;
  }
  /// \ru Функция инициализации. \en Initialization function.
  void        Init( double _precision ) {
    precision = _precision;
    deviation = ANGLE_REGION;
  }
  /// \ru Оператор присваивания. \en Assignment operator.
  MbPrecision & operator = ( const MbPrecision & other ) {
    precision = other.precision;
    deviation = other.deviation;
    return *this;
  }
  /// \ru Являются ли объекты равными? \en Determine whether an object is equal? 
  bool        IsSame( const MbPrecision & other, double accuracy ) const; 

  /// \ru Получить точность. \en Get precision.
  double      GetPrecision() const { return precision; }
  double      GetDeviation() const { return deviation; }

  /// \ru Установить точность. \en Set precision.
  void        SetPrecision( double a ) { precision = a; }
  void        SetDeviation( double a ) { deviation = a; }

  /// \ru Оператор чтения. \en Read operator. 
  friend MATH_FUNC( reader & )  operator >> ( reader &, MbPrecision & );
  /// \ru Оператор записи. \en Write operator. 
  friend MATH_FUNC( writer & )  operator << ( writer &, const MbPrecision & );

  DECLARE_NEW_DELETE_CLASS( MbPrecision )
  DECLARE_NEW_DELETE_CLASS_EX( MbPrecision )
};


//------------------------------------------------------------------------------
/** \brief \ru Обобщенные результаты операции.
           \en The common operation results. \~
  \details \ru Класс содержит достигнутую точность построения объектов, а также \n
               дополнительные данные, в виде хот-точек операции.
           \en The class contains the tolerance of the constructed objects and also \n
               additional data in the form of hot points. \n \~
  \ingroup Build_Parameters
*/
// ---
class MATH_CLASS MbOperationResults {
protected:
  double                    _tolerance; ///< \ru Толерантность построенных объектов. \en The tolerance of the constructed objects.
  c3d::BaseHotPointAttrSPtr _hotPoint;  ///< \ru Атрибут хот-точки операции. \en Hot-point's attribute.

public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  MbOperationResults()
    : _tolerance( METRIC_EPSILON )
    , _hotPoint (                )
  {}
  /// \ru Конструктор по заданной точности. \en Constructor by the specified precision.
  MbOperationResults( double tolerance )
    : _tolerance( tolerance )
    , _hotPoint (           )
  {}
  /// \ru Конструктор копирования. \en Copy-constructor.
  MbOperationResults( const MbOperationResults & other )
    : _tolerance( other._tolerance )
    , _hotPoint ( other._hotPoint  )
  {}
  /// \ru Деструктор. \en Destructor.
  virtual ~MbOperationResults() {}

public:
  /// \ru Функция инициализации. \en Initialization function.
  void Init( const c3d::BaseHotPointAttrSPtr & hotPoint,
                   double                      accuracy = METRIC_EPSILON) {
    _hotPoint = hotPoint;
    _tolerance = accuracy;
  }
  /// \ru Являются ли объекты равными? \en Determine whether an object is equal? 
  bool   IsSame( const MbOperationResults & other, double accuracy ) const;
  /// \ru Получить значение толерантности построенных объектов. \en Get tolerance of the constructed objects.
        double                      GetTolerance()         const { return _tolerance; }
  /// \ru Получить атрибут хот-точки операции. \en Get hot-point's attribute.
  const c3d::BaseHotPointAttrSPtr & GetHotPointAttribute() const { return _hotPoint; }
  /// \ru Установить атрибут хот-точки операции. \en Set hot-point's attribute.
        c3d::BaseHotPointAttrSPtr & SetHotPointAttribute()       { return _hotPoint; }
  /// \ru Оператор присваивания. \en Assignment operator.
  MbOperationResults & operator = ( const MbOperationResults & other );

  DECLARE_NEW_DELETE_CLASS( MbOperationResults )
  DECLARE_NEW_DELETE_CLASS_EX( MbOperationResults )
};


//------------------------------------------------------------------------------
/** \brief \ru Индекс идентификации объекта.
           \en Index of object identification. \~
  \details \ru Индекс содержит имя, номер в теле и контрольную точку и служит для поиска объекта (грани, ребра, вершины) в оболочке.
    Поиск объекта производится по имени, в случае неудачи - по номеру, и проверяется по контрольной точке. \n
           \en Index contains the name, the index in the solid and the control point, it is used to search for object (face, edge, vertex) in the shell.
    Object searching is performed by name. In failure case - by index, and checked by the control point. \n \~
  \ingroup Build_Parameters
*/
// ---
struct MATH_CLASS MbItemIndex {
protected:
  size_t        itemIndex; ///< \ru Номер объекта в оболочке. \en The index of object in the shell.
  MbCartPoint3D point;     ///< \ru Контрольная точка объекта. \en Control point of the object.
  SimpleName    itemName;  ///< \ru Имя объекта. \en A name of an object.

public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  MbItemIndex()
    : itemIndex( SYS_MAX_T )
    , point    ( -DETERMINANT_MAX, -DETERMINANT_MAX, -DETERMINANT_MAX )
    , itemName ( c3d::SIMPLENAME_MAX )
  {}

  /// \ru Конструктор по индексу без точки привязки. \en Constructor by the index without anchor point.
  explicit MbItemIndex( size_t i )
    : itemIndex( i )
    , point    ( -DETERMINANT_MAX, -DETERMINANT_MAX, -DETERMINANT_MAX )
    , itemName ( c3d::SIMPLENAME_MAX )
  {}

  /// \ru Конструктор по индексу с точкой привязки. \en Constructor by the index with anchor point.
  MbItemIndex( size_t i, const MbCartPoint3D & p, SimpleName n )
    : itemIndex( i )
    , point    ( p )
    , itemName ( n )
  {}

  /// \ru Конструктор по индексу с точкой привязки. \en Constructor by the index with anchor point.
  MbItemIndex( size_t i, const MbFace & face ) : itemIndex( i ), point(), itemName() { Init( face, i ); }
  /// \ru Конструктор по индексу с точкой привязки. \en Constructor by the index with anchor point.
  MbItemIndex( size_t i, const MbCurveEdge & edge ) : itemIndex( i ), point(), itemName() { Init( edge, i ); }

  /// \ru Конструктор копирования. \en Copy-constructor.
  MbItemIndex( const MbItemIndex & other )
    : itemIndex( other.itemIndex )
    , point    ( other.point     )
    , itemName ( other.itemName  )
  {}
  /// \ru Деструктор. \en Destructor.
  ~MbItemIndex();

public:
  /// \ru Функция инициализации. \en Initialization function.
  void        Init( const MbItemIndex & other )
  {
    itemIndex = other.itemIndex;
    point     = other.point;
    itemName  = other.itemName;
  }
  /// \ru Функция инициализации. \en Initialization function.
  void        Init( size_t ind, bool reset = true )
  {
    itemIndex = ind;
    if ( reset ) {
      point.Init( -DETERMINANT_MAX, -DETERMINANT_MAX, -DETERMINANT_MAX );
      itemName = c3d::SIMPLENAME_MAX;
    }
  }
  /// \ru Функция инициализации. \en Initialization function.
  void        Init( size_t i, const MbCartPoint3D & p, SimpleName n )
  {
    itemIndex = i;
    point     = p;
    itemName  = n;
  }
  /// \ru Функция инициализации. \en Initialization function.
  bool        Init( const MbFaceShell &, size_t faceIndex );
  /// \ru Функция инициализации. \en Initialization function.
  void        Init( const MbFace      &, size_t faceIndex );
  /// \ru Функция инициализации. \en Initialization function.
  void        Init( const MbCurveEdge &, size_t edgeIndex );

  /// \ru Оператор присваивания. \en Assignment operator.
  MbItemIndex & operator = ( const MbItemIndex & other )
  {
    Init( other );
    return *this;
  }

  /// \ru Получить индекс. \en Get index.
  size_t      GetIndex() const { return itemIndex; }
  /// \ru Получить имя. \en Get name.
  SimpleName  GetName()  const { return itemName; }
  /// \ru Получить точку привязки. \en Get anchoring point.
  const MbCartPoint3D &  GetPoint()  const { return point; }

  /// \ru Установить индекс. \en Set index.
  void        SetIndex( size_t index ) { itemIndex = index; }
  /// \ru Установить имя. \en Set name.
  void        SetName( SimpleName name ) { itemName = name; }
  /// \ru Установить точку привязки. \en Set anchoring point.
  void        SetPoint( const MbFace & );
  /// \ru Установить точку привязки. \en Set anchoring point.
  void        SetPoint( const MbCurveEdge & );

  /// \ru Изменить индекс и точку привязки. \en Change index and anchoring point.
  void        ChangeIndexPoint( size_t index, const MbCartPoint3D & pnt ) { itemIndex = index; point = pnt; }
  /// \ru Изменить индекс и точку привязки. \en Change index and anchoring point.
  void        ChangeIndexName( size_t index, const SimpleName & name ) { itemIndex = index; itemName = name; }

  /// \ru Преобразовать элемент согласно матрице. \en Transform element according to the matrix.
  void        Transform( const MbMatrix3D & matr );
  /// \ru Сдвинуть объект вдоль вектора. \en Move an object along a vector.
  void        Move     ( const MbVector3D & to );
  /// \ru Повернуть объект вокруг оси на заданный угол. \en Rotate an object at a given angle around an axis.
  void        Rotate   ( const MbAxis3D & axis, double ang );
  /// \ru Являются ли объекты равными? \en Determine whether an object is equal? 
  bool        IsSame( const MbItemIndex & other, double accuracy ) const; 

  /// \ru Статический оператор меньше. \en Static operator "less".
  static bool LessByItemIndex( const MbItemIndex & ind1, const MbItemIndex & ind2 )
  {
    if ( ind1.GetIndex() < ind2.GetIndex() )
      return true;
    return false;
  }
  /// \ru Статический оператор меньше. \en Static operator "less".
  static bool LessByItemName( const MbItemIndex & ind1, const MbItemIndex & ind2 )
  {
    if ( ind1.GetName() < ind2.GetName() )
      return true;
    return false;
  }
  /// \ru Статический оператор отсутствия индекса. \en Static operator "no item index".
  static bool NoItemIndex( const MbItemIndex & ind )
  {
    if ( ind.itemIndex == SYS_MAX_T )
      return true;
    return false;
  }

  KNOWN_OBJECTS_RW_REF_OPERATORS( MbItemIndex ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class.
  DECLARE_NEW_DELETE_CLASS( MbItemIndex )
  DECLARE_NEW_DELETE_CLASS_EX( MbItemIndex )
};


//------------------------------------------------------------------------------
/** \brief \ru Расширенный индекс идентификации объекта.
           \en Extended index of object identification. \~
  \details \ru Расширенный индекс содержит имя, номер в теле и контрольную точку и служит для поиска объекта
    (например, грани а теле для построения тонкой стенки).
    Поиск объекта производится по имени, в случае неудачи - по номеру, и проверяется по контрольной точке. \n
           \en Extended index contains the name, the index in the solid and the control point and is used to search object
    (or example: face in the solid, for construction of thin wall).
    Object searching is performed by name. In failure case - by index, and checked by the control point. \n \~
  \ingroup Build_Parameters
*/
// ---
struct MATH_CLASS MbItemThinValues : public MbItemIndex {
public:
  double  value1; ///< \ru Первое значение параметра объекта (толщина наружу). \en The first parameter value of the object (thickness of the outside).
  double  value2; ///< \ru Второе значение параметра объекта (толщина внутрь). \en The second parameter value of the object (thickness of the outside).

public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  MbItemThinValues()
    : MbItemIndex(     )
    , value1     ( 0.0 )
    , value2     ( 0.0 )
  {}

  /// \ru Конструктор по индексу и толщинам наружу и внутрь. \en Constructor by the index and thickness to outside and inside.
  MbItemThinValues( ptrdiff_t i, double d1, double d2 )
    : MbItemIndex( i  )
    , value1     ( ::fabs(d1) )
    , value2     ( ::fabs(d2) )
  {}

  /// \ru Конструктор по индексу, точке привязки и толщинам наружу и внутрь. \en Constructor by the index, anchor point and thickness to outside and inside.
  MbItemThinValues( ptrdiff_t i, const MbCartPoint3D & p, double d1, double d2, SimpleName n )
    : MbItemIndex( i, p, n    )
    , value1     ( ::fabs(d1) )
    , value2     ( ::fabs(d2) )
  {}

  /// \ru Конструктор по индексу, точке привязки и толщинам наружу и внутрь. \en Constructor by the index, anchor point and thickness to outside and inside.
  MbItemThinValues( const MbItemIndex & itemInd, double d1, double d2 )
    : MbItemIndex( itemInd    )
    , value1     ( ::fabs(d1) )
    , value2     ( ::fabs(d2) )
  {}

  /// \ru Конструктор копирования. \en Copy-constructor.
  MbItemThinValues( const MbItemThinValues & other )
    : MbItemIndex( other.itemIndex, other.point, other.itemName )
    , value1     ( other.value1                 )
    , value2     ( other.value2                 )
  {}

  /// \ru Оператор присваивания. \en Assignment operator.
  MbItemThinValues & operator = ( const MbItemThinValues & other ) {
    MbItemIndex::Init( (const MbItemIndex &)other );
    value1 = other.value1;
    value2 = other.value2;
    return *this;
  }

  /** \brief \ru Инициализировать по индексу идентификации.
             \en Initialize by identification index. \~
    \details \ru Инициализировать по индексу идентификации и толщине наружу и внутрь.
             \en Initialize by the identification index and thickness to outside and inside. \~
    \param[in] itemInd - \ru Индекс идентификации.
                         \en Identification index. \~
    \param[in] d1      - \ru Толщина наружу.
                         \en Thickness to outside. \~
    \param[in] d2      - \ru Толщина внутрь.
                         \en Thickness to inside. \~
  */
  void        Init( const MbItemIndex & itemInd, double d1, double d2 )
  {
    MbItemIndex::Init( itemInd );
    value1 = ::fabs(d1);
    value2 = ::fabs(d2);
  }
  /// \ru Являются ли объекты равными? \en Determine whether an object is equal? 
  bool        IsSame( const MbItemThinValues & other, double accuracy ) const; 

  KNOWN_OBJECTS_RW_REF_OPERATORS( MbItemThinValues ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class.
  DECLARE_NEW_DELETE_CLASS( MbItemThinValues )
  DECLARE_NEW_DELETE_CLASS_EX( MbItemThinValues )
};


//------------------------------------------------------------------------------
/** \brief \ru Индекс идентификации ребра.
           \en Index of edge identification. \~
  \details \ru Индекс содержит имя, номер ребра, номера соединяемых ребром граней в теле
    и контрольную точку и служит для поиска ребра для скругления или фаски.
    Поиск ребра производится по имени, в случае неудачи - по номерам, и проверяется по контрольной точке. \n
           \en Index contains the name, the index of the edge, indices of faces in the solid connected by edge
    and control point and is used to search the edge for fillet or chamfer.
    Edge searching is performed by name. In failure case - by indices, and checked by the control point. \n \~
  \ingroup Build_Parameters
*/
// ---
struct MATH_CLASS MbEdgeFacesIndexes {
public:
  size_t        edgeIndex;  ///< \ru Номер ребра в множестве рёбер тела. \en The index of the edge in the set of solid edges.
  size_t        facePIndex; ///< \ru Номер грани слева в множестве граней тела. \en The index of the face on the left in the set of solid faces.
  size_t        faceMIndex; ///< \ru Номер грани справа в множестве граней тела. \en The index of the face on the right in the set of solid faces.
  MbCartPoint3D point;      ///< \ru Контрольная точка ребра. \en Control point of the edge.
  SimpleName    itemName;   ///< \ru Имя объекта. \en A name of an object.

public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  MbEdgeFacesIndexes()
    : edgeIndex ( SYS_MAX_T )
    , facePIndex( SYS_MAX_T )
    , faceMIndex( SYS_MAX_T )
    , point     ( -DETERMINANT_MAX, -DETERMINANT_MAX, -DETERMINANT_MAX )
    , itemName  ( c3d::SIMPLENAME_MAX )
  {}

  /// \ru Конструктор копирования. \en Copy-constructor.
  MbEdgeFacesIndexes( const MbEdgeFacesIndexes & other )
    : edgeIndex ( other.edgeIndex  )
    , facePIndex( other.facePIndex )
    , faceMIndex( other.faceMIndex )
    , point     ( other.point      )
    , itemName  ( other.itemName   )
  {}

  /// \ru Оператор присваивания. \en Assignment operator.
  MbEdgeFacesIndexes & operator = ( const MbEdgeFacesIndexes & other ) {
    edgeIndex  = other.edgeIndex;
    facePIndex = other.facePIndex;
    faceMIndex = other.faceMIndex;
    point      = other.point;
    itemName   = other.itemName;
    return *this;
  }

  /// \ru Преобразовать элемент согласно матрице. \en Transform element according to the matrix.
  void        Transform( const MbMatrix3D & matr );
  /// \ru Сдвинуть объект вдоль вектора. \en Move an object along a vector.
  void        Move     ( const MbVector3D & to );
  /// \ru Повернуть объект вокруг оси на заданный угол. \en Rotate an object at a given angle around an axis.
  void        Rotate   ( const MbAxis3D & axis, double ang );
  /// \ru Являются ли объекты равными? \en Determine whether an object is equal? 
  bool        IsSame( const MbEdgeFacesIndexes & other, double accuracy ) const; 

  /// \ru Номер ребра в множестве рёбер тела. \en The index of the edge in the set of solid edges.
  size_t      GetEdgeIndex() const { return edgeIndex; }
  /// \ru Номер грани слева в множестве граней тела. \en The index of the face on the left in the set of solid faces.
  size_t      GetFacePIndex() const { return facePIndex; }
  /// \ru Номер грани справа в множестве граней тела. \en The index of the face on the right in the set of solid faces.
  size_t      GetFaceMIndex() const { return faceMIndex; }
  /// \ru Контрольная точка ребра. \en Control point of the edge.
  const MbCartPoint3D & GetPoint() const { return point; }
  MbCartPoint3D & SetPoint() { return point; }
  /// \ru Имя объекта. \en A name of an object.
  SimpleName  GetItemName() const { return itemName; }

  /// \ru Установить индекс. \en Set index.
  void        SetIndex( size_t index, size_t indexP, size_t indexM ) { edgeIndex = index; facePIndex = indexP; faceMIndex = indexM; }
  /// \ru Установить имя. \en Set name.
  void        SetName( SimpleName name ) { itemName = name; }
  /// \ru Установить имя. \en Set name.
  void        SetPoint( const MbCartPoint3D & p ) { point = p; }

  /// \ru Статический оператор меньше. \en Static operator "less".
  static bool LessByEdgeIndex( const MbEdgeFacesIndexes & ind1, const MbEdgeFacesIndexes & ind2 )
  {
    if ( ind1.edgeIndex < ind2.edgeIndex )
      return true;
    return false;
  }

  /// \ru Функция чтения. \en Read function.
  friend MATH_FUNC (reader &) operator >> ( reader & in,        MbEdgeFacesIndexes & ref );
  /// \ru Функция записи. \en Write function.
  friend MATH_FUNC (writer &) operator << ( writer & out, const MbEdgeFacesIndexes & ref );
  /// \ru Функция записи. \en Write function.
  friend MATH_FUNC (writer &) operator << ( writer & out,       MbEdgeFacesIndexes & ref ) {
    return operator << ( out,(const MbEdgeFacesIndexes &)ref );
  }

  DECLARE_NEW_DELETE_CLASS( MbEdgeFacesIndexes )
  DECLARE_NEW_DELETE_CLASS_EX( MbEdgeFacesIndexes )
};


//------------------------------------------------------------------------------
/** \brief \ru Индекс грани и её ребер.
           \en Index of face and it edges. \~
  \details \ru Индекс грани и её ребер, предназначенных для обработки. Индекс служит для поиска грани и ребра в оболочке.
               Поиск объекта производится по имени, в случае неудачи - по номеру, и проверяется по контрольной точке. \n
           \en Index of face and it edges for process. Index is used to search for face end edges in the shell.
               The face searching is performed by name. In failure case - by index, and checked by the control point. \n \~
  \ingroup Build_Parameters
*/
// ---
struct MATH_CLASS MbExtendedIndex {
public:
  MbItemIndex              faceIndex;   ///< \ru Идентификаторы граней в оболочке. \en Identifier of a shell face. 
  std::vector<MbItemIndex> edgeIndices; ///< \ru Идентификаторы обрабатываемых ребер грани. \en Identifiers of processed edges of the face. 

public:
  /// \ru Конструктор. \en Constructor.
  template <class ItemIndexVector>
  MbExtendedIndex( const MbItemIndex & fIndex, const ItemIndexVector & eIndices )
    : faceIndex  ( fIndex )
    , edgeIndices(        )
  {
    edgeIndices.assign( eIndices.begin(), eIndices.end() );
  }
  /// \ru Конструктор копирования. \en Copy-constructor.
  MbExtendedIndex( const MbExtendedIndex & other )
    : faceIndex( other.faceIndex )
    , edgeIndices( other.edgeIndices )
  {}
  /// \ru Деструктор. \en Destructor.
  ~MbExtendedIndex() {}

public:
  /// \ru Функция инициализации. \en Initialization function.
  void        Init( const MbExtendedIndex & other ) {
    faceIndex = other.faceIndex;
    edgeIndices.clear();
    edgeIndices = other.edgeIndices;
  }
  /// \ru Оператор присваивания. \en Assignment operator.
  MbExtendedIndex & operator = ( const MbExtendedIndex & other ) {
    Init( other );
    return *this;
  }

  /// \ru Получить индекс грани. \en Get face index.
  MbItemIndex & FaceIndex() { return faceIndex; }
  const MbItemIndex & GetFaceIndex() const { return faceIndex; }
  /// \ru Получить индексы рёбер грани. \en Get edges indices.
  std::vector<MbItemIndex> & EdgeIndices() { return edgeIndices; }
  const std::vector<MbItemIndex> & GetEdgeIndices() const { return edgeIndices; }

  /// \ru Преобразовать элемент согласно матрице. \en Transform element according to the matrix.
  void        Transform( const MbMatrix3D & matr );
  /// \ru Сдвинуть объект вдоль вектора. \en Move an object along a vector.
  void        Move     ( const MbVector3D & to );
  /// \ru Повернуть объект вокруг оси на заданный угол. \en Rotate an object at a given angle around an axis.
  void        Rotate   ( const MbAxis3D & axis, double ang );
  /// \ru Являются ли объекты равными? \en Determine whether an object is equal? 
  bool        IsSame( const MbExtendedIndex & other, double accuracy ) const; 

  /// \ru Функция чтения. \en Read function.
  friend MATH_FUNC (reader &) operator >> ( reader & in,        MbExtendedIndex & ref );
  /// \ru Функция записи. \en Write function.
  friend MATH_FUNC (writer &) operator << ( writer & out, const MbExtendedIndex & ref );
  /// \ru Функция записи. \en Write function.
  friend MATH_FUNC (writer &) operator << ( writer & out,       MbExtendedIndex & ref ) {
    return operator << ( out,(const MbExtendedIndex &)ref );
  }

  DECLARE_NEW_DELETE_CLASS( MbExtendedIndex )
  DECLARE_NEW_DELETE_CLASS_EX( MbExtendedIndex )
};


////////////////////////////////////////////////////////////////////////////////
//
// \ru Неклассные функции. \en Out-of-class functions.
//
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
/// \ru Сортировка по возрастанию номера. \en Sorting in ascending order of index.
// ---
template <class Indices> // Indices - MbItemIndex vector.
void SortItemIndices( Indices & indices )
{
  if ( indices.size() > 1 ) {
    std::sort( indices.begin(), indices.end(), MbItemIndex::LessByItemIndex );
  }
}


//------------------------------------------------------------------------------
/// \ru Сортировка по возрастанию номера. \en Sorting in ascending order of index.
// ---
template <class Indices> // Indices - MbEdgeFacesIndexes vector.
void SortEdgeFacesIndices( Indices & indices )
{
  if ( indices.size() > 1 ) {
    std::sort( indices.begin(), indices.end(), MbEdgeFacesIndexes::LessByEdgeIndex );
  }
}


#endif // __OP_BINDING_DATA_H
