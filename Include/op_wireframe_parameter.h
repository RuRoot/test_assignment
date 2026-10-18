////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Параметры операций над каркасом.
         \en Parameters of operations on the wire frame. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __OP_WIREFRAME_PARAMETERS_H
#define __OP_WIREFRAME_PARAMETERS_H


#include <op_curve_parameter.h>
#include <solid.h>
#include <wireframe_point.h>


class MbOffsetWireFrameInfo;
class MbTrimmedWireFrameInfo;
class MbProjWireFrameParamsData;


//------------------------------------------------------------------------------
/** \brief \ru Параметры скругления каркаса.
           \en Parameters of wire frame fillets. \~
  \details \ru Параметры скругления каркаса: \n
               type - тип скругления( обычное или на поверхности ). \n
               radiuses - радиусы скругления, i-й радиус соответствует стыку i-го и i+1-го ребра. \n
           \en Parameters of wire frame fillets:  \n
               'type' is a fillet type( ordinary or on a surface ). \n
               'radiuses' are the fillet radii, the i-th radius corresponds to the joint of the i-th and the i+1-th edges. \n \~
  \warning \ru В разработке.
           \en Under development. \~
  \ingroup WireFrame_Parameters
*/ // ---
class MATH_CLASS MbWireFrameFilletsParams : public MbPrecision {
private:
  MbeConnectingType   _type;     ///< \ru Тип выполняемых скруглений. \en Fillet type( ordinary or on a surface ).
  c3d::DoubleVector   _radiuses; ///< \ru Множество радиусов скругления. \en An array of fillet radii.
  c3d::SNameMakerSPtr _snMaker;  ///< \ru Именователь с версией операции. \en Names maker with operation version.

public:
  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters.\~
    \details \ru Конструктор по параметрам.
             \en Constructor by parameters.\~
    \param[in] type      - \ru Тип выполняемых скруглений. \en Fillet type.\~
    \param[in] radiuses  - \ru Множество радиусов скругления.  \en An array of fillet radii. \~
    \param[in] nameMaker - \ru Именователь с версией операции. \en Names maker with operation version.\~
  */
  MbWireFrameFilletsParams( MbeConnectingType type, const c3d::DoubleVector & radiuses, const MbSNameMaker & nameMaker );
  /// \ru Конструктор для чтения. \en Constructor for reading.
  MbWireFrameFilletsParams( TapeInit tapeInit );
  /// \ru Деструктор. \ en Destructor.
  ~MbWireFrameFilletsParams() {}

public:
  /// \ru Получить множество радиусов скругления. \en Get an array of fillet radii. 
  const c3d::DoubleVector & GetRadii()          const { return _radiuses; }
  /// \ru Получить тип выполняемых скруглений. \en Get the type of fillets. 
  MbeConnectingType         GetConnectingType() const { return _type; }
  /// \ru Получить именователь. \en Get names maker. 
  const MbSNameMaker &      GetNameMaker()      const { return *_snMaker; }

OBVIOUS_PRIVATE_COPY( MbWireFrameFilletsParams )
};


//-------------------------------------------------------------------------------
/** \brief \ru Информация для преобразования каркаса в Nurbs.
           \en Information for transformation of a wire frame to NURBS. \~
  \details \ru Информация для преобразования каркаса в Nurbs. \n
           \en Information for transformation of a wire frame to NURBS. \n \~
  \warning \ru В разработке.
           \en Under development. \~
  \ingroup WireFrame_Parameters
*/
// ---
class MATH_CLASS MbNurbsWireFrameInfo {
private:
  MbWireFramePoint _pbeg;         ///< \ru Начало участка каркаса. \en Wire frame piece start. 
  MbWireFramePoint _pend;         ///< \ru Конец участка каркаса. \en Wire frame piece end. 
  bool             _sense;        ///< \ru Направление сплайн-кривой. \en Direction of spline-curve. 
  bool             _matchParams;  ///< \ru Сохранять ли при преобразовании однозначное соответствие параметрических областей. \en Whether to save correspondence of parametric regions while transforming or not. 
  bool             _extendRange;  ///< \ru Строится ли преобразование на продолжении для незамкнутой подложки. \en Whether transformation is constructed on the extension for a non-closed substrate. 
  VERSION          _version;      ///< \ru Версия исполнения. \en The version of execution. 

private:
  MbNurbsWireFrameInfo();
public:

  /// \ru Конструктор. \en Constructor. 
  MbNurbsWireFrameInfo( const MbWireFramePoint & p1, const MbWireFramePoint & p2, bool sense, bool match, bool ext, VERSION version ) 
    : _pbeg       ( p1      )
    , _pend       ( p2      )
    , _sense      ( sense   )
    , _matchParams( match   )
    , _extendRange( ext     )
    , _version    ( version )
  {}
  /// \ru Конструктор. \en Constructor. 
  MbNurbsWireFrameInfo( const MbNurbsWireFrameInfo & other, const MbWireFramePoint & p1, const MbWireFramePoint & p2, bool sense ) 
    : _pbeg       ( p1    )
    , _pend       ( p2    )
    , _sense      ( sense )
    , _matchParams( other._matchParams )
    , _extendRange( other._extendRange )
    , _version    ( other._version     )
  {}
  /// \ru Конструктор копирования. \en Copy constructor. 
  MbNurbsWireFrameInfo( const MbNurbsWireFrameInfo & other ) 
    : _pbeg       ( other._pbeg  )
    , _pend       ( other._pend  )
    , _sense      ( other._sense )
    , _matchParams( other._matchParams )
    , _extendRange( other._extendRange )
    , _version    ( other._version     )
  {}
  /// \ru Конструктор для чтения. \en Constructor for reading. 
  MbNurbsWireFrameInfo( TapeInit tapeInit );


  /// \ru Функция инициализации. \en The initialization function. 
  void Init( const MbWireFramePoint & p1, const MbWireFramePoint & p2, bool sense )
  {
    _pbeg = p1;
    _pend = p2;
    _sense = sense;
  }
  /// \ru Функция инициализации. \en The initialization function. 
  void Init( const MbWireFramePoint & p1, const MbWireFramePoint & p2, bool sense, bool match, bool ext, VERSION version ) 
  {
    _matchParams = match;
    _extendRange = ext;
    _version = version;
    Init( p1, p2, sense );
  }

public:
  /// \ru Получить начало участка кривой. \en Get the start curve region.
  const MbWireFramePoint & GetPBeg()  const { return _pbeg;  }
  /// \ru Получить конец участка кривой. \en Get the end curve region.
  const MbWireFramePoint & GetPEnd()  const { return _pend;  }
  /// \ru Получить направление сплайн-кривой. \en Get the direction of spline-curve. 
  bool      GetSense() const { return _sense; }
  /// \ru Сохранять ли при преобразовании однозначное соответствие параметрических областей. \en Whether to save correspondence of parametric regions by transformation or not. 
  bool      MatchParams() const { return _matchParams; }
  /// \ru Строится ли преобразование на продолжении для незамкнутой подложки. \en Whether transformation is constructed on the extension for a non-closed substrate or not. 
  bool      ExtendRange() const { return _extendRange; }
  /// \ru Получить версию исполнения. \en Get the version of execution. 
  VERSION   GetMathVersion() const { return _version; }
  /// \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool      IsSame( const MbNurbsWireFrameInfo & other, double accuracy ) const;

  /// \ru Оператор присваивания. \en An assignment operator. 
  const MbNurbsWireFrameInfo & operator = ( const MbNurbsWireFrameInfo & other ) {
    _pbeg        = other._pbeg;
    _pend        = other._pend;
    _sense       = other._sense;
    _matchParams = other._matchParams;
    _extendRange = other._extendRange;
    _version     = other._version;
    return *this;
  }

KNOWN_OBJECTS_RW_REF_OPERATORS( MbNurbsWireFrameInfo ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class. \~
};


//-------------------------------------------------------------------------------
/** \brief \ru Параметры преобразования каркаса в Nurbs.
           \en Parameters for transformation of a wire frame to NURBS. \~
  \details \ru Параметры преобразования каркаса в Nurbs. \n
           \en Parameters for transformation of a wire frame to NURBS. \n \~
  \ingroup WireFrame_Parameters
*/
// ---
class MATH_CLASS MbNurbsWireFrameParams : public MbPrecision {
private:
  MbNurbsWireFrameInfo _frameIntoNurbsInfo;   ///< \ru Информация для преобразования каркаса в Nurbs. \en Information for transformation of a wire frame to NURBS. \~
  c3d::SNameMakerSPtr  _snMaker;              ///< \ru Именователь с версией операции. \en Names maker with operation version.

private:
  MbNurbsWireFrameParams();

public:
  /// \ru Конструктор. \en Constructor. 
  MbNurbsWireFrameParams( const MbNurbsWireFrameInfo & info, 
                          const MbSNameMaker &         nameMaker )
    : _frameIntoNurbsInfo( info )
    , _snMaker( &nameMaker.Duplicate() )
  {}

  /// \ru Конструктор. \en Constructor. 
  explicit MbNurbsWireFrameParams( const MbWireFramePoint & p1, 
                                   const MbWireFramePoint & p2, 
                                         bool               sense,
                                         bool               match, 
                                         bool               ext, 
                                   const MbSNameMaker &     nameMaker,
                                         VERSION            ver );

  /// \ru Конструктор. \en Constructor. 
  explicit MbNurbsWireFrameParams( const MbNurbsWireFrameParams & other, 
                                   const MbWireFramePoint &       p1, 
                                   const MbWireFramePoint &       p2, 
                                         bool                     sense,
                                   const MbSNameMaker &           nameMaker );

  /// \ru Конструктор копирования. \en Copy constructor. 
  MbNurbsWireFrameParams( const MbNurbsWireFrameParams & other );

  /// \ru Конструктор для чтения. \en Constructor for reading. 
  MbNurbsWireFrameParams( TapeInit tapeInit );

  /// \ru Функция инициализации. \en The initialization function. 
  void Init( const MbWireFramePoint & p1, const MbWireFramePoint & p2, bool sense )
  {
    _frameIntoNurbsInfo.Init( p1, p2, sense );
  }
  /// \ru Функция инициализации. \en The initialization function. 
  void Init( const MbWireFramePoint & p1, const MbWireFramePoint & p2, bool sense, bool match, bool ext ) {
    _frameIntoNurbsInfo.Init( p1, p2, sense, match, ext, Math::DefaultMathVersion() );
  }

public:
  /// \ru Получить параметр начала участка кривой. \en Get the parameter of the start curve region. 
  const MbWireFramePoint & GetPBeg()  const { return _frameIntoNurbsInfo.GetPBeg();  }
  /// \ru Получить параметр конца участка кривой. \en Get the parameter of the end curve region. 
  const MbWireFramePoint & GetPEnd()  const { return _frameIntoNurbsInfo.GetPEnd();  }
  /// \ru Получить направление сплайн-кривой. \en Get the direction of spline-curve. 
  bool      GetSense() const { return _frameIntoNurbsInfo.GetSense(); }
  /// \ru Сохранять ли при преобразовании однозначное соответствие параметрических областей. \en Whether to save correspondence of parametric regions by transformation or not. 
  bool      MatchParams() const { return _frameIntoNurbsInfo.MatchParams(); }
  /// \ru Строится ли преобразование на продолжении для незамкнутой подложки. \en Whether transformation is constructed on the extension for a non-closed substrate or not. 
  bool      ExtendRange() const { return _frameIntoNurbsInfo.ExtendRange(); }
  /// \ru Получить версию исполнения. \en Get the version of execution. 
  VERSION   GetMathVersion() const { return _frameIntoNurbsInfo.GetMathVersion(); }

  /// \ru Получить информацию для преобразования каркаса в Nurbs. \en Get information for transformation of a wire frame to NURBS.
  const MbNurbsWireFrameInfo & GetNurbsWireFrameInfo() const { return _frameIntoNurbsInfo; }

  /// \ru Получить именователь. \en Get names maker. 
  const MbSNameMaker & GetNameMaker() const { return *_snMaker; }

  /// \ru Оператор присваивания. \en An assignment operator. 
  const MbNurbsWireFrameParams & operator = ( const MbNurbsWireFrameParams & other ) {
    MbPrecision::operator=( other );
   _frameIntoNurbsInfo = other._frameIntoNurbsInfo;
    return *this;
  }

};


//-------------------------------------------------------------------------------
/** \brief \ru Параметры проецирования точки на проволочный каркас.
           \en Parameters for projecting points onto a wire frame. \~
  \details \ru Параметры проецирования точки на проволочный каркас. \n
           \en Parameters for projecting points onto a wire frame. \n \~
  \ingroup WireFrame_Parameters
*/
// ---
class MATH_CLASS MbWireFrameProjParams : public MbPrecision {
private:
  MbCartPoint3D            _point;    ///< \ru Точка для проецирования. \en Projection point.
  c3d::WireFramePointsPair _range;    ///< \ru Диапазон поиска проекции. Может быть не задан. \en Projection search range. May not be specified.
  bool                     _ext;      ///< \ru Признак выхода за пределы диапазона поиска или за пределы областей связных контуров каркаса. \en Sign of going beyond the search range or beyond the areas of connected frame contours.

public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  MbWireFrameProjParams()
    : MbPrecision()
    , _point()
    , _range( MbWireFramePoint(), MbWireFramePoint() )
    , _ext( false )
  {}

  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \details \ru Конструктор по точке и признаку выхода за пределы диапазона.
             \en Constructor by point and a sign of out of range. \~
    \param[in] point - \ru Проецируемая точка.
                       \en Projected point. \~
    \param[in] ext - \ru Признак выхода за пределы областей связных контуров каркаса.
                     \en Sign of going beyond the beyond the areas of connected wire frame contours. \~
  */
  MbWireFrameProjParams( const MbCartPoint3D & point, bool ext )
    : MbPrecision()
    , _point( point )
    , _range( MbWireFramePoint(), MbWireFramePoint() )
    , _ext( ext )
  {}

  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \details \ru Конструктор с указанием диапазона поиска.
             \en Constructor specifying the search range. \~
    \param[in] point - \ru Проецируемая точка.
                       \en Projected point. \~
    \param[in] range - \ru Диапазон поиска проекции.
                       \en Projection search range. \~
    \param[in] ext - \ru Признак выхода за пределы диапазона поиска или за пределы областей связных контуров каркаса.
                     \en Sign of going beyond the search range or beyond the areas of connected frame contours. \~
  */
  MbWireFrameProjParams( const MbCartPoint3D & point, const c3d::WireFramePointsPair & range, bool ext )
    : MbPrecision()
    , _point( point )
    , _range( range )
    , _ext( ext )
  {}

  /// \ru Конструктор копирования. \en Copy constructor.
  MbWireFrameProjParams( const MbWireFrameProjParams & other )
    : MbPrecision( other )
    , _point( other._point )
    , _range( other._range )
    , _ext( other._ext )
  {}

public:
  /// \ru Задан ли диапазон. \en Is the range set?
        bool                      IsRangeSet()  const { return _range.first.IsValid() && _range.second.IsValid(); }
  /// \ru Выдать диапазон поиска. \en Give search range.
  const c3d::WireFramePointsPair & GetRange()   const { return _range; }
  /// \ru Выдать признак выхода за пределы диапазона поиска. \en Get sign of going beyond the search range.
        bool                      GetExt()      const { return _ext; }
  /// \ru Выдать точку для проецирования. \en Get projection point.
  const MbCartPoint3D &           GetPoint()    const { return _point; }

  /// \ru Оператор присваивания. \en An assignment operator. 
  const MbWireFrameProjParams & operator = ( const MbWireFrameProjParams & other ) {
    MbPrecision::operator=( other );
   _point = other._point;
   _range = other._range;
   _ext = other._ext;
   return *this;
  }
};


//-------------------------------------------------------------------------------
/** \brief \ru Результаты проецирования точки на проволочный каркас.
           \en Results for projecting points onto a wire frame. \~
  \details \ru Результаты проецирования точки на проволочный каркас. \n
           \en Results for projecting points onto a wire frame. \n \~
  \ingroup WireFrame_Parameters
*/
// ---
class MATH_CLASS MbWireFrameProjResult {
private:
  MbWireFramePoint _projection;  ///< \ru Точка на проволочном каркасе. \en Point on the wire frame.
  bool             _result;      ///< \ru true, если найденный параметр находится в допустимом диапазоне, или false - в противном случае. \en true if the found parameter is in a valid range or false - otherwise.

public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  MbWireFrameProjResult()
    : _projection()
    , _result( false )
  {}
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  MbWireFrameProjResult( const MbWireFramePoint & point, bool result )
    : _projection( point )
    , _result( result )
  {}
  /// \ru Конструктор копирования. \en Copy constructor.
  MbWireFrameProjResult( const MbWireFrameProjResult & other )
    : _projection( other._projection )
    , _result( other._result )
  {}

  /// \ru Инициализация по параметрам. \en Initialization by parameters.
  void Init( const MbWireFramePoint & point, bool result ) {
    _projection = point;
    _result = result;
  }

  /// \ru Выдать точку на проволочном каркасе. \en Get point on the wire frame.
  const MbWireFramePoint & GetWireFramePoint() const { return _projection; }

  /// \ru
  bool GetResult() const { return _result; }

  /// \ru Оператор присваивания. \en An assignment operator. 
  const MbWireFrameProjResult & operator = ( const MbWireFrameProjResult & other ) {
    _projection = other._projection;
    return *this;
  }
};


//-------------------------------------------------------------------------------
/** \brief \ru Параметры для усечения каркаса.
           \en Parameters for trimmed of a wire frame. \~
  \details \ru Параметры для усечения каркаса. \n
           \en Parameters for trimmed of a wire frame. \n \~
  \ingroup WireFrame_Parameters
*/
// ---
class MATH_CLASS MbTrimmedWireFrameParams : public MbPrecision {
private:
  DPtr<MbTrimmedWireFrameInfo> _trimmedWireFrameInfo; ///< \ru Информация для усечения каркаса. \en Information for trimmed wire frame. \~
  c3d::SNameMakerSPtr          _snMaker;              ///< \ru Именователь с версией операции. \en Names maker with operation version.
private:
  MbTrimmedWireFrameParams();
public:
  /// \ru Конструктор. \en Constructor. 
  explicit MbTrimmedWireFrameParams( const MbWireFramePoint & p1,
                                     const MbWireFramePoint & p2, 
                                           bool               sense,
                                           bool               saveParLenAndLaw,
                                     const MbSNameMaker &     nameMaker );
  /// \ru Конструктор копирования. \en Copy constructor. 
           MbTrimmedWireFrameParams( const MbTrimmedWireFrameParams & other );
  /// \ru Деструктор \en Destructor
  virtual  ~MbTrimmedWireFrameParams();
  /// \ru Функция инициализации. \en The initialization function. 
  void     Init( const MbWireFramePoint & p1, 
                 const MbWireFramePoint & p2, 
                       bool               sense,
                       bool               saveParLenAndLaw );
  /// \ru Получить параметр начала участка кривой. \en Get the parameter of the start curve region. 
  const MbWireFramePoint &         GetPBeg()  const;
  /// \ru Получить параметр конца участка кривой. \en Get the parameter of the end curve region. 
  const MbWireFramePoint &         GetPEnd()  const;
  /// \ru Получить направление усеченной кривой относительно исходной. \en Get the direction of a trimmed curve in relation to an initial curve.  
  bool                             GetSense() const;
  /// \ru Получить информацию о сохранении параметрической длины и закона. \en Get information about saving parametric length and law. 
  bool                             GetSaveLengthAndLow() const;
  /// \ru Получить именователь. \en Get names maker. 
  const MbSNameMaker &             GetNameMaker() const { return *_snMaker; }
  /// \ru Оператор присваивания. \en An assignment operator. 
  const MbTrimmedWireFrameParams & operator = ( const MbTrimmedWireFrameParams & other );
};


//-------------------------------------------------------------------------------
/** \brief \ru Параметры эквидистантного каркаса.
           \en Parameters of an offset wire frame. \~
  \details \ru Параметры эквидистантного каркаса.
           \en Parameters of an offset wire frame. \~
  \warning \ru В разработке.
           \en Under development. \~
  \ingroup WireFrame_Parameters
*/
// ---
class MATH_CLASS MbOffsetWireFrameParams : public MbPrecision {
public:
  /** \brief \ru Перечисление способов создания эквидистантного каркаса.
             \en Enumeration of ways to offset the wire frame. \~
  */
  enum class OffsetMethod : uint8
  {
    chordal = 0x01, ///< \ru Смещение каркаса по хорде. \en Chordal offset method of a wire frame.
  };

private:
  std::unique_ptr<MbOffsetWireFrameInfo> _offsetWireFrameInfo; ///< \ru Информация для смещения каркаса. \en Information for offsetting a wire frame. \~
  c3d::ConstSolidSPtr _solid;        ///< \ru Оболочка, на которой строится эквидистанта. \en The shell on which to build the offset wire frame.
  c3d::IndicesVector  _facesIndices; ///< \ru Индексы граней, на которых строится эквидистанта. \en The faces indices on which to build the offset wire frame.
  c3d::SNameMakerSPtr _snMaker;      ///< \ru Именователь с версией операции. \en Names maker with operation version.

public:
  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \details \ru Конструктор для смещения каркаса.
             \en Constructor for offsetting a wire frame. \~
    \param[in] offsetVect - \ru Вектор, задающий смещение в точке каркаса.
                            \en The displacement vector at a point of the wire frame. \~
    \param[in] useFillet - \ru Если true, то разрывы заполнять скруглением, иначе продолженными кривыми.
                           \en If 'true', the gaps are to be filled with fillet, otherwise with the extended curves. \~
    \param[in] keepRadius - \ru Если true, то в существующих скруглениях сохранять радиусы.
                            \en If 'true', the existent fillet radii are to be kept. \~
    \param[in] bluntAngle - \ru Если true, то в притуплять острые углы.
                            \en If 'true', sharp corners are to be blunt. \~
    \param[in] fromBeg - \ru Вектор смещения привязан к началу (если true).
                         \en The translation vector is associated with the beginning (if true). \~
    \param[in] nameMaker - \ru Именователь ребер каркаса.
                           \en An object defining the frame edge names. \~
    \param[in] useSurfaceNormal - \ru Эквидистанта согласована с нормалью к поверхности.
                                  \en Offset point is moved according to surface normal. \~
    \param[in] surface - \ru Поверхность каркаса или подобная ей.
                         \en Frame's surface or similar to such surface. \~
  */
  explicit MbOffsetWireFrameParams( const MbVector3D &          offsetVect,
                                          bool                  useFillet,
                                          bool                  keepRadius,
                                          bool                  bluntAngle,
                                          bool                  fromBeg,
                                    const MbSNameMaker &        nameMaker,
                                          bool                  useSurfaceNormal,
                                          c3d::ConstSurfaceSPtr surface = nullptr );
  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \details \ru Конструктор для смещения каркаса.
             \en Constructor for offsetting a wire frame. \~
    \param[in] offsetVect - \ru Вектор, задающий смещение в точке каркаса.
                            \en The displacement vector at a point of the wire frame. \~
    \param[in] nameMaker - \ru Именователь ребер каркаса.
                           \en An object defining the frame edge names. \~
  */
  explicit MbOffsetWireFrameParams( const MbVector3D &   offsetVect,
                                    const MbSNameMaker & nameMaker );
  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
\deprecated \ru Конструктор устарел и будет удален в версии 2026.
            \en The constructor is deprecated and will be removed in version 2026. \~
    \details \ru Конструктор для смещения каркаса вдоль поверхности.
             \en Constructor for offsetting a wire frame along a surface. \~
    \param[in] face - \ru Грань, на которой строится эквидистанта.
                      \en The face on which to build the offset wire frame. \~
    \param[in] axis - \ru Направление смещения с точкой приложения.
                      \en The offset direction with a reference point. \~
    \param[in] dist - \ru Величина смещения.
                      \en The offset distance. \~
    \param[in] nameMaker - \ru Именователь ребер каркаса.
                           \en An object defining the frame edge names. \~
  */
  DEPRECATE_DECLARE
  explicit MbOffsetWireFrameParams(       c3d::ConstFaceSPtr & face,
                                    const MbAxis3D &           axis,
                                          double               dist,
                                    const MbSNameMaker &       nameMaker );
  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \details \ru Конструктор для смещения каркаса вдоль набора граней тела.
             \en Constructor for offsetting a wire frame along a set of faces of the solid. \~
    \param[in] solid - \ru Тело, на котором строится эквидистанта.
                       \en The solid on which to build the offset wire frame. \~
    \param[in] facesIndices - \ru Индексы граней тела, на которых строится эквидистанта.
                              \en Indices of the solid faces on which to build the offset wire frame. \~
    \param[in] axis - \ru Направление смещения с точкой приложения.
                      \en The offset direction with a reference point. \~
    \param[in] dist - \ru Величина смещения.
                      \en The offset distance. \~
    \param[in] method - \ru Метод смещения.
                        \en The offset method. \~
    \param[in] extendWire - \ru Флаг, определяющий, продлевать ли эквидистанту до границ граней.
                            \en A flag that determines whether to extend the equidistant line to the edges of the faces. \~
    \param[in] extendShell - \ru Флаг, определяющий, строить ли эквидистанту на продолжении граней (true) или усекать границами граней (false).
                             \en A flag that determines whether to construct an equidistant line on the continuation of the faces (true) or truncate it by the boundaries of the faces (false) \~
    \param[in] nameMaker - \ru Именователь ребер каркаса.
                           \en An object defining the frame edge names. \~
  */
  MbOffsetWireFrameParams(       c3d::ConstSolidSPtr  solid,
                           const c3d::IndicesVector & facesIndices,
                           const MbAxis3D &           axis,
                                 double               dist,
                                 OffsetMethod         method,
                                 bool                 extendWire,
                                 bool                 extendShell,
                           const MbSNameMaker &       nameMaker );
  /// \ru Деструктор. \en Destructor.
  virtual ~MbOffsetWireFrameParams();

public:
  /// \ru Получить тело. \en Get solid.
  c3d::ConstSolidSPtr GetSolid() const;
  /// \ru Получить массив индексов. \en Get indices.
  const c3d::IndicesVector & GetIndices() const { return _facesIndices; }
  /// \ru Получить именователь операции. \en Get names maker.
  const MbSNameMaker & GetNameMaker() const { return *_snMaker; }

public:
  /// \ru Получить параметры. Для внутреннего пользования. \en Get parameters. For internal use.
  const MbOffsetWireFrameInfo * GetOffsetInfo() const;

  private:
  MbOffsetWireFrameParams( const MbOffsetWireFrameParams & other );

//OBVIOUS_PRIVATE_COPY( MbOffsetWireFrameParams )
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры операции построения проекции каркаса на тело, набор
               граней или поверхность.
           \en The parameters for creating a projection of a wireframe to a
               solid, set of faces or a surface. \~
  \details \ru Содержит параметры, необходимые для построения проекции набора кривых по нормали или по направлению
               на тело, набор граней или поверхность. \n
           \en Contains parameters required for normal or vector projection of a set of curves to a solid, set of faces
               or a surface. \n \~
  \ingroup WireFrame_Parameters
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
class MATH_CLASS MbProjectionWireFrameParams : public MbPrecision {
private:
  const MbVector3D * _direction;        ///< \ru Вектор направления проецирования (если не задан, то проекция по нормали). \en The direction of the vector projection (if not provided, normal projection is performed).
  bool               _projOnParallel;   ///< \ru Создавать ли проекции на поверхностях параллельных заданному вектору проецирования. Такие проекции всегда будут усекаться границами вне зависимости от значения флага _truncateByBounds. \en Should the projection be performed onto surfaces parallel to a provided direction of the vector projection. These projections will be truncated by bounds regardless of _truncateByBounds value.
  bool               _createExact;      ///< \ru Создавать проекционную кривую при необходимости. \en Create a projection curve if necessary.
  bool               _truncateByBounds; ///< \ru Усечь границами. \en Truncate by bounds.
  MbSNameMaker       _nameMaker;        ///< \ru Именователь с версией. \en An object defining the names with the version.
  std::unique_ptr<MbProjWireFrameParamsData> _paramsData; ///< \ru Для внутреннего использования. \en For internal use.

public:
  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \details \ru В этом конструкторе задаются данные для проецирования кривой на поверхность.
             \en In this constructor, data is specified for projecting a curve onto a surface. \~
    \param[in] surface          - \ru Поверхность для проецирования заданной кривой.
                                  \en A surface to project the specified curve onto. \~
    \param[in] curve            - \ru Кривая для проецирования на заданную поверхность.
                                  \en A curve to project onto the specified surface. \~
    \param[in] direction        - \ru Вектор направления проецирования (если не задан, то проекция по нормали).
                                  \en The direction of the vector projection (if not provided, normal projection is performed). \~
    \param[in] createExact      - \ru Создавать проекционную кривую при необходимости.
                                  \en Create a projection curve if necessary. \~
    \param[in] truncateByBounds - \ru Усекать границами поверхности.
                                  \en Truncate by the surface bounds. \~
    \param[in] version          - \ru Версия исполнения.
                                  \en The version. \~
  */
  MbProjectionWireFrameParams(       c3d::ConstRefWrap<MbSurface> surface,
                                     c3d::ConstRefWrap<MbCurve3D> curve,
                               const MbVector3D *                 direction,
                               const bool                         createExact,
                               const bool                         truncateByBounds,
                               const VERSION                      version = Math::DefaultMathVersion() );

  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \details \ru В этом конструкторе задаются данные для проецирования кривой на набор граней.
             \en In this constructor, data is specified for projecting a curve to a set of faces. \~
    \param[in] curve            - \ru Кривая для проецирования на заданный набор граней.
                                  \en A curve to project to a specified set of faces. \~
    \param[in] faces            - \ru Связный набор граней для проецирования заданной кривой.
                                  \en A connected set of faces to project the specified curve to. \~
    \param[in] direction        - \ru Вектор направления проецирования (если не задан, то проекция по нормали).
                                  \en The direction of the vector projection (if not provided, normal projection is performed). \~
    \param[in] projOnParallel   - \ru Создавать ли проекции на поверхностях параллельных заданному вектору проецирования.
                                  \en Should the projection be performed onto surfaces parallel to a provided direction of the vector projection. \~
    \param[in] createExact      - \ru Создавать проекционную кривую при необходимости.
                                  \en Create a projection curve if necessary. \~
    \param[in] truncateByBounds - \ru Усекать границами.
                                  \en Truncate by bounds. \~
    \param[in] snMaker          - \ru Именователь с версией.
                                  \en An object defining the names with the version. \~
  */
  MbProjectionWireFrameParams(       c3d::ConstRefWrap<MbCurve3D>             curve,
                                     c3d::ConstRefWrap<c3d::ConstFacesVector> faces,
                               const MbVector3D *                             direction,
                               const bool                                     projOnParallel,
                               const bool                                     createExact,
                               const bool                                     truncateByBounds,
                               const MbSNameMaker &                           snMaker );

  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \details \ru В этом конструкторе задаются данные для проецирования проволочного каркаса на тело.
             \en In this constructor, data is specified for projecting a wireframe to a solid. \~
    \param[in] wireFrame        - \ru Проволочный каркас для проецирования на заданное тело.
                                  \en A wireframe to project to a specified solid. \~
    \param[in] sameWireFrame    - \ru Использовать тот же экземпляр проволочного каркаса или создать копию.
                                  \en Use the same wireframe or copy it. \~
    \param[in] solid            - \ru Тело для проецирования заданного проволочного каркаса.
                                  \en A solid to project the specified wireframe to. \~
    \param[in] sameCreators     - \ru Использовать ли тот же экземпляр журнала тела или создать копию.
                                  \en Use the same creators of the solid or make a copy. \~
    \param[in] faceIndices      - \ru Индексы граней оболочки тела.
                                  \en Indices of faces in the solid's shell. \~
    \param[in] direction        - \ru Вектор направления проецирования (если не задан, то проекция по нормали).
                                  \en The direction of the vector projection (if not provided, normal projection is performed). \~
    \param[in] projOnParallel   - \ru Создавать ли проекции на поверхностях параллельных заданному вектору проецирования.
                                  \en Should the projection be performed onto surfaces parallel to a provided direction of the vector projection. \~
    \param[in] createExact      - \ru Создавать проекционную кривую при необходимости.
                                  \en Create a projection curve if necessary. \~
    \param[in] truncateByBounds - \ru Усекать границами.
                                  \en Truncate by bounds. \~
    \param[in] snMaker          - \ru Именователь с версией.
                                  \en An object defining the names with the version. \~
  */
  MbProjectionWireFrameParams(       c3d::ConstRefWrap<MbWireFrame> wireFrame,
                               const bool                           sameWireFrame,
                                     c3d::ConstRefWrap<MbSolid>     solid,
                               const bool                           sameCreators,
                               const c3d::IndicesVector &           faceIndices,
                               const MbVector3D *                   direction,
                               const bool                           projOnParallel,
                               const bool                           createExact,
                               const bool                           truncateByBounds,
                               const MbSNameMaker &                 snMaker );

  /// \ru Деструктор. \en Destructor.
  ~MbProjectionWireFrameParams();

public:
  /// \ru Выдать вектор направления проецирования (nullptr если не задан). \en Get the direction of the vector projection (nullptr if not provided).
  const MbVector3D * GetDirection() const { return _direction; }

  /// \ru Создавать ли проекции на поверхностях параллельных заданному вектору проецирования. \en Whether the projection should be performed onto surfaces parallel to a provided direction of the vector projection.
  bool GetProjectOnParallel() const { return _projOnParallel; }

  /// \ru Создавать ли проекционную кривую при необходимости. \en Whether to create a projection curve if necessary.
  bool GetCreateExact() const { return _createExact; }

  /// \ru Усекать ли границами. \en Whether to truncate by bounds.
  bool GetTruncateByBounds() const { return _truncateByBounds; }

  /// \ru Получить именователь. \en Get an object defining the names.
  const MbSNameMaker & GetNameMaker() const { return _nameMaker; }

  /// \ru Для внутреннего использования. \en For internal use.
  const MbProjWireFrameParamsData * GetParametersData() const { return _paramsData.get(); }

OBVIOUS_PRIVATE_COPY( MbProjectionWireFrameParams )
};


//------------------------------------------------------------------------------
/** \brief \ru Результаты операции построения проекционного проволочного каркаса.
           \en The results of creating a projection wireframe. \~
  \details \ru Результаты, полученные при построении проекции набора кривых по нормали или по направлению
               на тело, набор граней или поверхность. Результаты содержат полученный проекционный проволочный каркас.
               При проецировании на тело в атрибутах ребер лежат имена соответствующих граней этого тела.
               При проецировании на набор граней в результатах операции будет заполнен массив индексы которого являются
               индексами ребер каркаса, а значения являются индексами граней в исходном массиве. \n
           \en Results of normal or vector projection of a set of curves to a solid, set of faces
               or a surface. Results contain obtained projection wireframe.
               When projecting a wireframe to a solid, the attributes of the edges contain the names of corresponding faces.
               When projecting to a set of faces results of the operation contain an array where the indices are the indices
               of the edges in a wireframe, and the values are the indices of the faces in the input array. \n \~
  \ingroup WireFrame_Parameters
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
class MATH_CLASS MbProjectionWireFrameResults : public MbOperationResults {
private:
  c3d::IndicesVector _resIndices;   ///< \ru Индексы граней в исходном массиве, соответствующие ребрам полученного каркаса. \en The indices of faces in the input array corresponding to the edges of the obtained wireframe.
  c3d::WireFrameSPtr _resWireFrame; ///< \ru Проволочный каркас, построенный по полученным проекционным кривым. \en The resulting wireframe constructed using obtained projection curves.

public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  MbProjectionWireFrameResults()
    : MbOperationResults(         )
    , _resIndices       (         )
    , _resWireFrame     ( nullptr )
  {}

public:
  /// \ru Инициализировать результаты по проволочному каркасу и индексам граней в исходном массиве, соответствующим полученным проекционным кривым. \en Initialize the results by the projection wireframe and the indices of faces in the input array corresponding to the obtained projection curves.
  template<class IndicesVector>
  void Init( MbWireFrame * resWireFrame, const IndicesVector & copyFrom ) {
    _resWireFrame = resWireFrame;
    _resIndices.clear();
    _resIndices.reserve( copyFrom.size() );

    for ( size_t id : copyFrom ) {
      _resIndices.push_back( id );
    }
  }

  /// \ru Выдать индексы граней в исходном массиве, соответствующие полученным проекционным кривым. \en Get the indices of faces in the input array corresponding to the obtained projection curves.
  template<class IndicesVector>
  void GetResIndices( IndicesVector & copyTo ) {
    copyTo.reserve( copyTo.size() + _resIndices.size() );

    for ( size_t id : _resIndices ) {
      copyTo.push_back( id );
    }
  }

  /// \ru Отцепить полученные проекционные кривые в заданный массив. \en Detach obtained projection curves into the specified array.
  template <class SpaceCurvesVector>
  void DetachCurves( SpaceCurvesVector & curves ) {
    MbWireFrame * wf = _resWireFrame.detach();

    ::ExtractCurvesDeleteFrame( wf, curves );
  };

  /// \ru Выдать проекционный проволочный каркас. \en Get the projection wireframe.
  const c3d::WireFrameSPtr & GetResWireFrame() { return _resWireFrame; }

  /// \ru Отцепить проекционный проволочный каркас. \en Detach the projection wireframe.
  MbWireFrame * DetachWireFrame() { return _resWireFrame.detach(); }

  /// \ru Очистить результаты. \en Clear the results.
  void Clear() { _resIndices.clear(); _resWireFrame.reset(); }

OBVIOUS_PRIVATE_COPY( MbProjectionWireFrameResults )
};


#endif // __OP_WIREFRAME_PARAMETERS_H
