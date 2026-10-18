////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Параметры операций над телами.
         \en Parameters of operations on the solids. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __OP_SWEPT_PARAMETERS_H
#define __OP_SWEPT_PARAMETERS_H



#include <mb_enum.h>
#include <mb_vector3d.h>
#include <mb_axis3d.h>
#include <op_binding_data.h>
#include <cur_contour.h>
#include <cur_contour3d.h>
#include <surface.h>
#include <solid.h>
#include <wire_frame.h>
#include <op_boolean_flags.h>
#include <function.h>


class MbPlacement3D;
class MbMatrix3D;
class MbAxis3D;
class MbCurve3D;
class MbPolyCurve;
class MbSNameMaker;
class MbFilletBundle;
class MbFilletData;
class MbRegTransform;
class MbRegDuplicate;


//------------------------------------------------------------------------------
/** \brief \ru Данные об образующей.
           \en The generating data. \~
  \details \ru Данные об образующей операции движения. \n
    Образующая операции выдавливания, вращения или кинематической операции
    может включать в себя набор двумерных контуров, набор трехмерных контуров, проволочный каркас, тело. \n
    Для набора двумерных контуров на поверхности существуют следующие ограничения:\n
    – может быть один или несколько контуров;\n
    – если контуров несколько, они должны быть либо все замкнуты, либо все разомкнуты;\n
    - если контуры замкнуты, они могут быть вложенными друг в друга, уровень вложенности не ограничивается;\n
    – контуры не должны пересекаться между собой или самопересекаться.\n
    Для двумерных контуров на не плоской поверхности есть дополнительное ограничение:
      все контуры должны быть замкнуты.\n
    Построение операции по двумерным контурам на не плоской поверхности рассчитано на указание пользователем
      грани тела в качестве образующей. В этом случае данные для образующей можно получить
      с помощью метода грани MbFace::GetSurfaceCurvesData.\n
    Ограничения для трехмерных контуров:\n
    – контуры не должны пересекаться между собой или самопересекаться.\n
           \en Data about generating of movement operation. \n
    При указании тела и поверхности одновременно предполагается, что выполняется кинематическая операция над 
      телом вдоль кривой на этой поверхности, причем движение согласовано с нормалью. \n
    Generating of extrusion operation, rotation or sweeping operation
    can include a set of two-dimensional contours, a set of three-dimensional contours, wire frame, solid. \n
    For a set of two-dimensional contours on the surface, the following restrictions:\n
    - can be one or multiple contours;\n
    - If there are multiple contours, all of them must be either closed or open;\n
    - if contours are closed, then they can be nested into each other, the level of nesting is not limited;\n
    - contours can't overlap each other or self-intersect.\n
    For two-dimensional contour on the non-planar surface is additional constraint:
      all the contours must be closed.\n
    Constructing operation by two-dimensional contours on non-planar surface  it is necessary to specify the by the user
      face of solid as generating. In this case, the generating data can be obtained
      by the method of face MbFace::GetSurfaceCurvesData.\n
    Constraints for three-dimensional contour:\n
    - contours can't overlap each other or self-intersect.\n 
    When set a solid and a surface at the same time, we suppose that sweeping operation over solid along curve on surface
      is done, and moving is according to surface normal. \n \~
  \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbSweptData {

public:
  /// \ru Тип интерпретации проволочного каркаса. \en Wire frame interpretation type.
  enum class MbeWireFrameType {
    undefined = 0,          ///< \ru Не определен. \en Undefined. \~
    planeContours = 1,      ///< \ru Контуры на плоскости. \en Contours on plane. \~
    surfaceContours = 2,    ///< \ru Контуры на поверхности. \en Contours on surface. \~
    spaceContours = 3,      ///< \ru Трехмерные контуры. \en Space contours. \~
  };

private:
  // \ru Информация об образующей - проволочном каркасе. В разработке. \en Information about the generatrix - wire frame. In Development.
  struct WireFrameInfo {
    c3d::WireFrameSPtr _wireFrame;  ///< \ru Трехмерный проволочный каркас. \en Three-dimensional wire frame.
    MbeWireFrameType   _frameType;  ///< \ru Тип интерпретации проволочного каркаса. \en Wire frame interpretation type.
    c3d::SurfaceSPtr   _surface;    ///< \ru Поверхность. \en The surface.

    /// \ru Конструктор по умолчанию. \en Default constructor.
    WireFrameInfo();
    /// \ru Конструктор копирования. \en Copy-constructor.
    WireFrameInfo( const WireFrameInfo & );
    /// \ru Конструктор копирования. \en Copy-constructor.
    WireFrameInfo( const WireFrameInfo &, MbRegDuplicate * );
    /// \ru Конструктор. \en Constructor.
    WireFrameInfo( MbWireFrame &, MbeWireFrameType, MbSurface * );
    /// \ru Оператор присваивания. \en Assignment operator. 
    const WireFrameInfo & operator = ( const WireFrameInfo & );

    void Transform( const MbMatrix3D &, MbRegTransform * = nullptr );       ///< \ru Преобразовать объект. \en Transform the object.
    void Move     ( const MbVector3D &, MbRegTransform * = nullptr );       ///< \ru Сдвинуть объект. \en Move the object.
    void Rotate   ( const MbAxis3D &, double, MbRegTransform * = nullptr ); ///< \ru Повернуть объект. \en Rotate the object.
    bool IsSame( const WireFrameInfo &, double ) const;                     ///< \ru Определить, являются ли объекты равными. \en Determine whether the objects are equal.
    bool IsSimilar( const WireFrameInfo & ) const;                          ///< \ru Определить, являются ли объекты подобными. \en Determine whether the objects are similar.
    bool SetEqual ( const WireFrameInfo & );                                ///< \ru Сделать объекты равным. \en Make objects equal.
  };

private:
  // \ru Данные о двумерных контурах на поверхности. \en Data about two-dimensional contours on the surface.
  c3d::SurfaceSPtr              surface;        ///< \ru Поверхность. \en The surface.
  c3d::PlaneContoursSPtrVector  contours;       ///< \ru Множество двумерных контуров. \en Set of two-dimensional contours.
  // \ru Трехмерные контуры. \en Three-dimensional contours.
  c3d::SpaceContoursSPtrVector  contours3D;     ///< \ru Множество трёхмерных контуров. \en Set of three-dimensional contours.
  // \ru Тело. \en Solid.
  c3d::SolidSPtr                solid;          ///< \ru Тело. \en A solid.

  // \ru Проволочные каркасы. \en Wire frames.
  std::vector<WireFrameInfo>    wireFrames;     ///< \ru Информация о проволочных каркасах. \en Information about wire frames.

public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  MbSweptData();
  /// \ru Конструктор копирования. \en Copy-constructor.
  MbSweptData( const MbSweptData &, MbRegDuplicate * ireg = nullptr );

public:

  /** \brief \ru Конструктор плоской образующей.
             \en Constructor of planar swept. \~
    \details \ru Конструктор плоской образующей из одного контура.
             \en Constructor of planar swept from one contour. \~
    \param[in] place   - \ru Локальная система координат.
                         \en A local coordinate system. \~
    \param[in] contour - \ru Контур в параметрах заданной системы координат. Используется оригинал.
                         \en Contour in parameters of the given coordinate system. Used original. \~
  */
  MbSweptData( const MbPlacement3D & place, MbContour & contour );

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по набору контуров на поверхности.
             \en Constructor by a set of contours on a surface. \~
    \param[in] _surface  - \ru Поверхность. Используется оригинал.
                           \en The surface. Used original. \~
    \param[in] _contours - \ru Набор контуров. Используются оригиналы.
                           \en A set of contours. Used originals. \~
    \param[in] spatialThickness - \ru Тип тонкой стенки: true - построение по пространственному случаю, false - по двумерному.
                                  \en Type of the thin wall: true - spatial contours will be used, false - contours on a surface. \~
  */
  MbSweptData( MbSurface & _surface, RPArray<MbContour> & _contours, bool spatialThickness = false );

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по набору контуров на поверхности.
             \en Constructor by a set of contours on a surface. \~
    \param[in] _surface  - \ru Поверхность. Используется оригинал.
                           \en The surface. Used original. \~
    \param[in] _contours - \ru Набор контуров. Используются оригиналы.
                           \en A set of contours. Used originals. \~
    \param[in] spatialThickness - \ru Тип тонкой стенки: true - построение по пространственному случаю, false - по двумерному.
                                  \en Type of the thin wall: true - spatial contours will be used, false - contours on a surface. \~
  */
  MbSweptData( MbSurface & _surface, c3d::PlaneContoursSPtrVector & _contours, bool spatialThickness = false );

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по кривой.
             \en Constructor by a contour. \~
    \param[in] _curve3d - \ru Кривая. Используются оригиналы.
                          \en A curve. Used originals. \~
  */
  MbSweptData( MbCurve3D & _curve3d );

  /** \brief \ru Конструктор по трехмерным кривым.
             \en Constructor by 3d curves. \~
    \details \ru Конструктор по трехмерным кривым.
             \en Constructor by 3d curves. \~
    \param[in] _curves3d - \ru Кривые. Используются оригиналы.
                           \en A curves. Used originals. \~
  */
  MbSweptData( c3d::SpaceCurvesSPtrVector & _curves3d );

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по контуру.
             \en Constructor by a contour. \~
    \param[in] _contour3d - \ru Контур. Используются оригиналы.
                             \en A contour. Used originals. \~
  */
  MbSweptData( MbContour3D & _contour3d );

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по набору пространственных контуров.
             \en Constructor by a set of spatial contours. \~
    \param[in] _contours3d - \ru Набор контуров. Используются оригиналы.
                             \en A set of contours. Used originals. \~
  */
  MbSweptData( RPArray<MbContour3D> & _contours3d );

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по набору пространственных контуров.
             \en Constructor by a set of spatial contours. \~
    \param[in] _contours3d - \ru Набор контуров. Используются оригиналы.
                             \en A set of contours. Used originals. \~
  */
  MbSweptData( c3d::SpaceContoursSPtrVector & _contours3d );

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по телу.
             \en Constructor by a solid. \~
    \param[in] _solid - \ru Тело. Используется оригинал объекта.
                        \en A solid. Used original of object. \~
    \param[in] newNameMaker - \ru Новое главное имя для топологических элементов тела.
                              \en New main name for names of solid's topological elements. \~
  */
  MbSweptData( MbSolid & _solid, const MbSNameMaker * newNameMaker = nullptr );

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор смешанной образующей.
             \en Constructor of mixed swept. \~
    \param[in] _surface    - \ru Поверхность. Используется оригинал.
                             \en The surface. Used original. \~
    \param[in] _contours   - \ru Набор двумерных контуров в параметрах заданной поверхности. Используются оригиналы.
                             \en Set of two-dimensional contours in the parameters of the given surface. Used originals. \~
    \param[in] _contours3d - \ru Набор трехмерных контуров. Используются оригиналы.
                             \en A set of three-dimensional contours. Used originals. \~
    \param[in] _solid      - \ru Тело. Используется оригинал объекта.
                             \en A solid. Used original of object. \~
  */
  MbSweptData( MbSurface * _surface, RPArray<MbContour> & _contours,
               RPArray<MbContour3D> & _contours3d, MbSolid * _solid );

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по трехмерному проволочному каркасу. Используются оригинал.
             \en Constructor по three-dimensional wire frame. Used original. \~
    \param[in] _wireFrame    - \ru Трехмерный проволочный каркас.
                               \en Three-dimensional wire frame. \~
    \param[in] _wireFrameType - \ru Тип интерпретации проволочного каркаса. 
                                \en Wire frame interpretation type. \~
  */
  MbSweptData( MbWireFrame & _wireFrame, MbeWireFrameType _wireFrameType = MbeWireFrameType::undefined );

  /// \ru Деструктор. \en Destructor.
  ~MbSweptData();

public:
  /** \brief \ru Добавить данные.
             \en Add data. \~
    \details \ru Добавить данные о контурах на поверхности.
             \en Add data about contours to the surface. \~
    \param[in] _surface  - \ru Поверхность. Добавляется оригинал объекта.
                           \en The surface. Added original of the object. \~
    \param[in] _contours - \ru Набор контуров. Добавляются оригиналы.
                           \en A set of contours. Originals are added. \~
  */
  bool              AddData( MbSurface & _surface, const RPArray<MbContour> & _contours );

  /** \brief \ru Добавить данные.
             \en Add data. \~
    \details \ru Добавить данные о контурах на поверхности.
             \en Add data about contours to the surface. \~
    \param[in] _surface  - \ru Поверхность. Добавляется оригинал объекта.
                           \en The surface. Added original of the object. \~
    \param[in] _contours - \ru Набор контуров. Добавляются оригиналы.
                           \en A set of contours. Originals are added. \~
  */
  bool              AddData( MbSurface & _surface, c3d::PlaneContoursSPtrVector & _contours );

  /** \brief \ru Добавить данные.
             \en Add data. \~
    \details \ru Добавить данные о проволочном каркасе.
             \en Add data about wireframe. \~
    \param[in] _wireFrame    - \ru Трехмерный проволочный каркас.
                               \en Three-dimensional wire frame. \~
    \param[in] _wireFrameType - \ru Тип интерпретации проволочного каркаса. 
                                    Все проволочные каркасы, добавленные в образующую, должны иметь типы: \n
                                    1. 'MbeWireFrameType::surfaceContours' или 'MbeWireFrameType::planeContours', или 'MbeWireFrameType::undefined'; \n
                                    или \n
                                    2. 'MbeWireFrameType::spaceContours' или 'MbeWireFrameType::undefinedContours'. \n
                                \en Wire frame interpretation type.  \n
                                    All wire frames added to the generating must have the following types: \n
                                    1. 'MbeWireFrameType::surfaceContours' or 'MbeWireFrameType::planeContours', or 'MbeWireFrameType::undefined'; \n
                                    or \n
                                    2. 'MbeWireFrameType::spaceContours' or 'MbeWireFrameType::undefinedContours'. \~
    \param[in] _surface  - \ru Поверхность, общая для кривых каркаса, при типе интерепретации 'MbeWireFrameType::surfaceContours'. 
                               Добавляется оригинал объекта.
                           \en Surface shared by wireframe curves when interpretation type is 'MbeWireFrameType::surfaceContours'. 
                               Added original of the object. \~
  */
  void              AddData( MbWireFrame & _wireFrame, MbeWireFrameType _wireFrameType, MbSurface * _surface );

  /** \brief \ru Количество всех кривых.
             \en The count of all the curves. \~
    \details \ru Общее количество двумерных и трехмерных кривых.
             \en The total count of two and three-dimensional curves. \~
  */
  size_t            CurvesCount() const;

  /** \brief \ru Получить кривую по индексу.
             \en Get the curve by the index. \~
    \details \ru Получить кривую из множества кривых на поверхности и трехмерных кривых.
             \en Get the curve from set of curves on the surface and three-dimensional curves. \~
    \param[in] i - \ru Номер кривой в пределах от 0 до CurvesCount().
                   \en The index of curve from 0 to CurvesCount(). \~
    \return \ru Кривую на поверхности или трехмерную кривую.
            \en Curve on the surface or three-dimensional curve. \~
  */
  SPtr<MbCurve3D>   GetCurve3D( size_t i ) const;

  /// \ru Есть данные о двумерных кривых на поверхности? \en Is there data of two-dimensional curves on the surface?
  bool              IsSurfaceCurvesData() const { return ((surface != nullptr) && !contours.empty()); }
  /// \ru Есть данные о пространственных кривых? \en Is there data of spatial curves?
  bool              IsSpaceCurvesData() const { return !contours3D.empty(); }
  /// \ru Есть данные о проволочном каркасе? \en Is there data about the wire frame?
  bool              IsWireFrameData() const { return wireFrames.size() != 0; }
  /// \ru Есть данные о теле? \en Is there data about the solid?
  bool              IsSolidData() const { return (solid != nullptr); }

  /// \ru Выдать поверхность. \en Get the surface.
  const MbSurface * GetSurface() const { return surface; }
  /// \ru Выдать поверхность для изменения. \en Get the surface for editing.
        MbSurface * SetSurface()       { return surface; }

  /** \brief \ru Установить поверхность.
             \en Set a surface. \~
    \details \ru Установить новую поверхность как носитель двумерных контуров или как целевую поверхность для направляющей.
             \en Set surface carrier of two-dimensional contours or desired surface-carrier of guide curve. \~
    \param[in] surf - \ru Новая поверхность как носитель для двумерных контуров или целевая поверхность для направляющей.
                      \en Surface carrier of two-dimensional contours or desired surface-carrier of guide curve. \~
  */
        void        SetSurface( const MbSurface & surf ) { surface = const_cast<MbSurface *>( &surf ); }

  /// \ru Выдать набор двумерных контуров. \en Get the set of two-dimensional contours.
  const c3d::PlaneContoursSPtrVector &  GetContours()   const { return contours;   }
  /// \ru Выдать набор трехмерных контуров. \en Get the set of three-dimensional contours.
  const c3d::SpaceContoursSPtrVector &  GetContours3D() const { return contours3D; }

  /// \ru Выдать проволочный каркас. \en Get the wire frame.
  DEPRECATE_DECLARE
  const MbWireFrame * GetWireFrame() const { return ( wireFrames.size() == 1 ) ? wireFrames[0]._wireFrame : nullptr; }
  /// \ru Выдать проволочный каркас для изменения. \en Get the wire frame for editing.
  DEPRECATE_DECLARE
        MbWireFrame * SetWireFrame() const { return ( wireFrames.size() == 1 ) ? wireFrames[0]._wireFrame : nullptr; }
  /// \ru Выдать тип интерпретации проволочного каркаса. \en Get wire frame interpretation type.
  DEPRECATE_DECLARE
        MbeWireFrameType GetWireFrameType() const { return ( wireFrames.size() == 1 ) ? wireFrames[0]._frameType : MbSweptData::MbeWireFrameType::undefined; }

  /// \ru Выдать количество проволочных каркасов. \en Get the wire frames count.
        size_t        GetWireFramesCount()       const { return wireFrames.size(); }
  /// \ru Выдать проволочный каркас по индексу. \en Get the wire frame by index.
  const MbWireFrame * GetWireFrame( size_t ind ) const { return wireFrames[ind]._wireFrame; }
  /// \ru Выдать проволочный каркас по индексу для изменения. \en Get the wire frame by index for editing.
        MbWireFrame * SetWireFrame( size_t ind )       { return wireFrames[ind]._wireFrame; }

  /// \ru Выдать тело. \en Get the solid.
  const MbSolid *     GetSolid() const { return solid; }
  /// \ru Выдать тело для изменения. \en Get the solid for editing.
        MbSolid *     SetSolid() const { return solid; }

  /// \ru Перестроить объект по журналу построения. Для перестроения проволочного каркаса. Вернуть true в случае успешного перестроения. \en Rebuild object according to the history tree. For rebuilding the wire frame. Return true if rebuild has been finished successfully. \~
        bool          RebuildItem();

  /** \brief \ru Выдать двумерные контуры с поверхностью или трехмерные контуры, полученные с проволочного каркаса. 
             \en Get two-dimensional contours with a surface or three-dimensional contours obtained from a wireframe. \~
    \details \ru Выдать двумерные контуры с поверхностью или трехмерные контуры, полученные с проволочного каркаса.
                 Каркас трактуется как поверхностные контуры, если у контуров общая поверхность,
                 иначе как пространственные.
             \en Get two-dimensional contours with a surface or three-dimensional contours obtained from a wireframe.
                 A wireframe is treated as a surface contours if the contours have a common surface,
                 else than spatial. \~
    \param[out] wireContours - \ru Набор двумерных контуров.
                               \en Set of two-dimensional contours.. \~
    \param[out] wireSurface - \ru Поверхность.
                              \en Surface. \~
    \param[out] wireContours3D - \ru Набор трехмерных контуров.
                                 \en Set of three-dimensional contours. \~
    \param[in,out] contoursNames - \ru На входе - именователь ребер каркаса. На выходе - именователь полученных контуров.
                                   \en At the input - the frame edge namer. At the output - the namer of the obtained contours. \~
  */
   void             GetWireFrameContours( c3d::PlaneContoursSPtrVector & wireContours,
                                          c3d::SurfaceSPtr &             wireSurface,
                                          c3d::SpaceContoursSPtrVector & wireContours3D,
                                          RPArray<MbSNameMaker> *        contoursNames = nullptr ) const;

  /** \brief \ru Выдать двумерные контуры с поверхностью, полученные с проволочного каркаса. 
             \en Get two-dimensional contours with a surface obtained from a wireframe. \~
    \details \ru Выдать двумерные контуры с поверхностью, полученные с проволочного каркаса.
                 Контуры будут созданы, если у всех каркасов заданы типы frameType == 'MbeWireFrameType::wft_planeContours' или frameType == 'MbeWireFrameType::wft_surfaceContours', или 'MbeWireFrameType::undefinedContours'.
                 Каркас трактуется как плоские контуры или как поверхностные контуры, в зависимости от заданного типа frameType.
             \en Get two-dimensional contours with a surface obtained from a wireframe.
                 Contours will be created if all wire frames have types frameType == 'MbeWireFrameType::wft_planeContours' or frameType == 'MbeWireFrameType::wft_surfaceContours' or 'MbeWireFrameType::undefinedContours'.
                 The frame is treated as either plane contours or surface contours, depending on the specified frameType. \~
    \param[out] wireSurfaces - \ru Набор поверхностей.
                               \en A set of surfaces. \~
    \param[out] wireContours - \ru Наборы двумерных контуров для каждой поверхности.
                               \en Sets of two-dimensional contours for all surface. \~
    \param[in] version  - \ru Версия.
                          \en A version. \~
    \return \ru Количество поверхностей и наборов контуров.
            \en Number of surfaces and contour sets. \~
  */
  size_t            GetWireFrameSurfaceContours( c3d::SurfacesSPtrVector &                   wireSurfaces,
                                                 std::vector<c3d::PlaneContoursSPtrVector> & wireContours,
                                                 std::vector<c3d::SNamesMakerSPtrVector> &   contoursNames,
                                                 VERSION                                     version = Math::DefaultMathVersion() ) const;

  /** \brief \ru Выдать трехмерные контуры, полученные с проволочного каркаса. 
             \en Get space contours obtained from a wireframe. \~
    \details \ru Выдать трехмерные контуры, полученные с проволочного каркаса. [
                 Контуры будут созданы, если для всех проволочных каркасов заданы типы frameType == 'MbeWireFrameType::wrf_spaceContours' или 'MbeWireFrameType::undefinedContours'.
             \en Get space contours obtained from a wireframe.
                 Contours will be created if all wire frames have type == 'MbeWireFrameType::wrf_spaceContours' or 'MbeWireFrameType::undefinedContours'. \~
    \param[out] wireContours - \ru Наборы двумерных контуров для каждой поверхности.
                               \en Sets of two-dimensional contours for all surface. \~
    \param[in] version  - \ru Версия.
                          \en A version. \~
    \return \ru Количество контуров.
            \en Number of contours. \~
  */
  size_t            GetWireFrameSpaceContours( c3d::SpaceContoursSPtrVector & wireContours3D,
                                               c3d::SNamesMakerSPtrVector &   contoursNames,
                                               VERSION                        version = Math::DefaultMathVersion() ) const;

  /** \brief \ru Преобразовать объект.
             \en Transform the object. \~
    \details \ru Преобразовать исходный объект согласно матрице c использованием регистратора.
             \en Transform the initial object according to the matrix using the registrator. \~
    \param[in] matr - \ru Матрица преобразования.
                      \en A transformation matrix. \~
    \param[in] iReg - \ru Регистратор.
                      \en Registrator. \~
  */
  void              Transform( const MbMatrix3D & matr, MbRegTransform * iReg = nullptr );
  /** \brief \ru Сдвинуть объект.
             \en Move the object. \~
    \details \ru Сдвинуть геометрический объект вдоль вектора с использованием регистратора.
             \en Move a geometric object along the vector using the registrator. \~
    \param[in] to   - \ru Вектор сдвига.
                      \en Translation vector. \~
    \param[in] iReg - \ru Регистратор.
                      \en Registrator. \~
  */
  void              Move     ( const MbVector3D & to, MbRegTransform * iReg = nullptr );
  /** \brief \ru Повернуть объект.
             \en Rotate the object. \~
    \details \ru Повернуть объект вокруг оси на заданный угол с использованием регистратора.
             \en Rotate an object about the axis by the given angle using the registrator. \~
    \param[in] axis  - \ru Ось поворота.
                       \en The rotation axis. \~
    \param[in] angle - \ru Угол поворота.
                       \en The rotation angle. \~
    \param[in] iReg  - \ru Регистратор.
                       \en Registrator. \~
  */
  void              Rotate   ( const MbAxis3D & axis, double angle, MbRegTransform * iReg = nullptr );
  /** \brief \ru Определить, являются ли объекты равными.
             \en Determine whether the objects are equal. \~
    \details \ru Определить, являются ли объекты равными с заданной точностью.
             \en Determine whether the objects are equal with defined accuracy. \~
    \param[in] other - \ru Объект для сравнения.
                       \en Object for comparison. \~
    \return \ru Подобны ли объекты.
            \en Whether the objects are similar. \~
  */
  bool              IsSame( const MbSweptData & other, double accuracy ) const;
  /** \brief \ru Определить, являются ли объекты подобными.
             \en Determine whether the objects are similar. \~
    \details \ru Подобный объект можно инициализировать по данным подобного ему объекта.
             \en Similar object can be initialized by data of object which is similar to it. \~
    \param[in] other - \ru Объект для сравнения.
                       \en Object for comparison. \~
    \return \ru Подобны ли объекты.
            \en Whether the objects are similar. \~
  */
  bool              IsSimilar( const MbSweptData & other ) const;
  /** \brief \ru Сделать объекты равным.
             \en Make objects equal. \~
    \details \ru Равными можно сделать только подобные объекты.
             \en It is possible to make equal only similar objects. \~
    \param[in] init - \ru Объект для инициализации.
                      \en Object for initialization. \~
    \return \ru Сделан ли объект равным присланному.
            \en Whether the object is made equal to the given one. \~
  */
  bool              SetEqual ( const MbSweptData & other );

  /** \brief \ru Замкнуты ли все контуры.
             \en Whether all contours are closed. \~
    \details \ru Замкнуты ли все контуры. \n
             \en Whether all contours are closed. \n \~
    \return \ru Возвращает true, если все контуры замкнуты.
            \en Returns true if all contours are closed. \~
  */
  bool              IsContoursClosed() const;

  /// \ru Проверить, что нет разрывов между сегментами поверхностных контуров. \en Check that there are no gaps between the segments of the surface contours.
  bool              CheckSurfaceContourConnection( double eps ) const;
  /// \ru Проверить, что нет разрывов между сегментами пространственных контуров. \en Check that there are no gaps between the segments of the spatial contours.
  bool              CheckSpaceContourConnection( double eps ) const;

private:
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default.
  MbSweptData &     operator = ( const MbSweptData & );

KNOWN_OBJECTS_RW_REF_OPERATORS( MbSweptData ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class.
};


//------------------------------------------------------------------------------
/** \brief \ru Способ выдавливания/вращения.
           \en Method of extrusion/rotation. \~
  \details \ru Способ построения выдавливания/вращения. \n
           \en Method of extrusion/rotation constructing. \n \~
  \ingroup Shell_Building_Parameters
*/
// ---
enum MbSweptWay {
  sw_scalarValue = -2, ///< \ru Выдавить на заданную глубину / вращать на заданный угол. \en Extrude to a given depth / rotate by a given angle.
  sw_shell       = -1, ///< \ru До ближайшего объекта (тела). \en To the nearest object (solid).
  sw_surface     =  0, ///< \ru До поверхности. \en To the surface.
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры вращения и выдавливания.
           \en Parameters of rotation and extrusion. \~
  \details \ru Данные о построении операции вращения или выдавливания
    в одном из направлений: прямом или обратном.
           \en Data about construction of rotation and extrusion
    in one of directions: forward or backward. \~
  \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbSweptSide {
public:
  MbSweptWay        way;          ///< \ru Способ выдавливания/вращения. \en Method of extrusion/rotation.
  double            scalarValue;  ///< \ru Угол вращения/глубина выдавливания. \en Angle of rotation/depth of extrusion.

  /** \brief \ru Расстояние от поверхности.
             \en Distance from the surface. \~
    \details \ru Расстояние от поверхности, до которой строим операцию.
      Задавать при построении операции до поверхности (way = sw_surface).
      distance < 0.0 при построении операции за поверхность,
      distance > 0.0 при построении операции до поверхности.
             \en Distance from the surface to construct up to.
      Set when constructing operation to the surface (way = sw_surface).
      distance < 0.0 when constructing operation back of surface,
      distance > 0.0 when constructing operation front of surface. \~
  */
  double            distance;

  /** \brief \ru Угол уклона.
             \en Draft angle. \~
    \details \ru Угол уклона при выдавливании.\n
      Операцию выдавливания с уклоном можно построить только в случае плоской образующей.
             \en Draft angle when extruding.\n
      Extrusion operation with draft can be constructed in the case of planar swept. \~
  */
  double            rake;

protected:

  /** \brief \ru Строгое соблюдение заданного угла уклона.
             \en Strict adherence to the specified draft angle. \~
    \details \ru Строгое соблюдение заданного угла уклона.\n
      При значении true каждая боковая грань операции с уклоном должна быть построена строго под заданным углом уклона,
      иначе операция не будет выполнена.
             \en Strict adherence to the specified draft angle.\n
      If set to true, each side face of a taper operation must be built strictly at the specified taper angle,
      otherwise, the operation will not be performed. \~
  */
  bool              strictRake;

  /** \brief \ru Поверхность, до которой строим операцию.
             \en The surface to construct up to. \~
    \details \ru Поверхность, до которой строим операцию.\n
      Задавать при построении операции до поверхности (way = sw_surface).
             \en The surface to construct up to.\n
      Set when constructing operation to the surface (way = sw_surface). \~
  */
  c3d::SurfaceSPtr  surface;

  /** \brief \ru Признак совпадения нормали поверхности с нормалью грани.
             \en An attribute of coincidence between the surface normal and the face normal. \~
    \details \ru Признак совпадения нормали поверхности, до которой строим операцию, с нормалью грани.\n
      Задавать при построении операции до поверхности (way = sw_surface).\n
      Указывает положение оболочки-результата относительно поверхности.
      Используется при построении массива операций до поверхности.
      Если у всех элементов массива признак должен быть одинаковым,
      то при построении исходной операции нужно задать признак равным orient_BOTH (направление не определено).
      При построении признак будет определен, и его значение нужно использовать для построения остальных элементов массива.
             \en An attribute of coincidence between the face normal and the normal of surface to which to create operation.\n
      Set when constructing operation to the surface (way = sw_surface).\n
      Specifies the position of shell-result relative to the surface.
      Used when constructing the array of operations to the surface.
      If attributes of all the elements of array must be the same,
      then when constructing of the original operation need to set attribute which is equal to orient_BOTH (the direction is not determined).
      When constructing the attribute is determined and its value should be used for the construction of other elements of the array. \~
 */
  MbeSenseValue sameSense;


public:
  /** \brief \ru Конструктор по умолчанию.
             \en Default constructor. \~
    \details \ru Задает параметры операции со способом "на заданную глубину".
      Для построения операции параметры нужно изменить,
      например, указать глубину выдавливания (угол вращения).
             \en Sets parameters of the operation with the method "to a given depth".
      For construction of operation the parameters need to change,
      for example: specify the depth of extrusion (angle of rotation). \~
  */
  MbSweptSide()
    : way        ( sw_scalarValue )
    , scalarValue( 0.0 )
    , distance   ( 0.0 )
    , rake       ( 0.0 )
    , strictRake ( false )
    , surface    ( nullptr )
    , sameSense  ( orient_BOTH )
  {}

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор на угол вращения\глубину выдавливания.
             \en Constructor by angle of rotation\depth of extrusion. \~
    \param[in] sVal - \ru Угол вращения\глубина выдавливания.
                      \en Angle of rotation\depth of extrusion. \~
  */
  MbSweptSide( double sVal )
    : way        ( sw_scalarValue )
    , scalarValue( sVal )
    , distance   ( 0.0  )
    , rake       ( 0.0  )
    , strictRake ( false )
    , surface    ( nullptr )
    , sameSense  ( orient_BOTH )
  {}

  /** \brief \ru Конструктор до поверхности.
             \en Constructor to the surface. \~
    \details \ru Конструктор до поверхности. Расстояние от поверхности задается равным 0.0.
             \en Constructor to the surface. Distance from the surface is set to 0.0. \~
    \param[in] surf - \ru Поверхность, до которой строится операция.
                      \en The surface to construct up to. \~
  */
  MbSweptSide( const MbSurface * surf );

  /** \brief \ru Конструктор до поверхности.
             \en Constructor to the surface. \~
    \details \ru Конструктор до поверхности. Для элемента массива.
             \en Constructor to the surface. For array element. \~
    \param[in] surf - \ru Поверхность, до которой строится операция.
                      \en The surface to construct up to. \~
    \param[in] sense - \ru Признак совпадения нормали заданной поверхности с нормалью грани.
      Указывает, по какую сторону от поверхности должна находиться построенная оболочка.
                       \en An attribute of coincidence between the normal of given surface and the face normal.
      Indicates at which side of the surface the must be located constructed shell. \~
  */
  MbSweptSide( const MbSurface * surf, MbeSenseValue sense );

  /** \brief \ru Конструктор копирования.
             \en Copy-constructor. \~
    \details \ru Конструктор копирования данных с использованием той же поверхности.
             \en Copy-constructor of data with using of the same surface. \~
    \param[in] other - \ru Исходные параметры.
                       \en Initial parameters. \~
  */
  MbSweptSide( const MbSweptSide & other );

  /** \brief \ru Конструктор копирования с регистратором.
             \en Copy-constructor with the registrator. \~
    \details \ru Конструктор копирования с регистратором. Поверхность копируется.
             \en Copy-constructor with the registrator. Surface is copying. \~
    \param[in] other - \ru Исходные параметры.
                       \en Initial parameters. \~
  */
  MbSweptSide( const MbSweptSide & other, MbRegDuplicate * ireg );

  /// \ru Деструктор. \en Destructor.
  virtual ~MbSweptSide();

public:
        /// \ru Оператор присваивания данных с использованием той же поверхности. \en Assignment operator of data with using of the same surface.
        MbSweptSide &   operator = ( const MbSweptSide & other );

        /// \ru Получить поверхность. \en Get the surface.
  const MbSurface *     GetSurface() const { return surface; }
        /// \ru Получить поверхность. \en Get the surface.
        MbSurface *     SetSurface()      { return surface; }
        /// \ru Заменить поверхность. \en Replace surface.
        void            SetSurface( const MbSurface * s );

        /// \ru Получить признак совпадения нормали поверхности с нормалью грани. \en Get the attribute of coincidence between the surface normal and the face normal.
        MbeSenseValue   GetSameSense() const { return sameSense; }
        /// \ru Установить признак совпадения нормали поверхности с нормалью грани. \en Set the attribute of coincidence between the surface normal and the face normal.
        void            SetSameSense( MbeSenseValue sense ) { sameSense = sense; }
        /// \ru Доступ к признаку совпадения нормали поверхности с нормалью грани. \en Access to the attribute of coincidence between the surface normal and the face normal.
        MbeSenseValue & SetSameSense() { return sameSense; }

        /// \ru Задан признак строгого соблюдения угла уклона. \en A sign of strict observance of the draft angle is set.
        bool            IsStrictRake() const { return strictRake; }
        /// \ru Установить признак строгого соблюдения угла уклона. \en Set a sign of strict observance of the draft angle.
        void            SetStrictRake( bool strict ) { strictRake = strict; }

        /// \ru Являются ли объекты равными? \en Determine whether an object is equal?
        bool            IsSame( const MbSweptSide & other, double accuracy ) const
        {
          if ( (other.way == way) && (other.sameSense == sameSense) ) {
            if ( (::fabs(other.scalarValue - scalarValue) < accuracy) &&
                 (::fabs(other.distance - distance) < accuracy) &&
                 (::fabs(other.rake - rake) < accuracy) &&
                 ( strictRake == other.strictRake ) )
            {
              bool isSurf1 = (surface != nullptr);
              bool isSurf2 = (other.surface != nullptr);

              if ( isSurf1 == isSurf2 ) {
                if ( isSurf1 && isSurf2 ) {
                  if ( !other.surface->IsSame( *surface, accuracy ) )
                    return false;
                }
                return true;
              }
            }
          }

          return false;
        }
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры формообразующей операции.
           \en The parameters of form-generating operation. \~
  \details \ru Параметры построения формообразующей операции
    (например, выдавливания, вращения, кинематической, по сечениям). \n
           \en The construction parameters of form-generating operation.
    (for example: extrusion, rotation, sweeping, loft). \n \~
  \ingroup Shell_Building_Parameters
*/
// ---
struct MATH_CLASS SweptValues {
public:
  /** \brief \ru Толщина стенки (величина эквидистанты) в прямом направлении.
             \en Wall thickness (offset distance) along the forward direction. \~
    \details \ru Толщина стенки (величина эквидистанты) в положительном направлении нормали объекта
     (грани, поверхности, плоскости кривой).
             \en Wall thickness (offset distance) along the positive direction of the normal of an object
     (face, surface, plane of the curve). \~
  */
  double          thickness1;

  /** \brief \ru Толщина стенки (величина эквидистанты) в обратном направлении.
             \en Wall thickness (offset distance) along the backward direction. \~
    \details \ru Толщина стенки (величина эквидистанты) в отрицательном направлении нормали объекта
     (грани, поверхности, плоскости кривой).
             \en Wall thickness (offset distance) along the negative direction of the normal of an object
     (face, surface, plane of the curve). \~
  */
  double          thickness2;

  bool            shellClosed;   ///< \ru Замкнутость создаваемой оболочки. \en Closedness of created shell.

private:
  bool            checkSelfInt; ///< \ru Флаг проверки самопересечений (вычислительно "тяжелыми" методами). \en Flag for checking of self-intersection (computationally by "heavy" methods).
  MbMergingFlags  mergeFlags;   ///< \ru Управляющие флаги слияния элементов оболочки. \en Control flags of shell items merging. 

public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  SweptValues()
    : thickness1  ( 0.0  )
    , thickness2  ( 0.0  )
    , shellClosed ( true )
    , checkSelfInt( true )
    , mergeFlags  (      )
  {}
  /// \ru Конструктор по толщинам и замкнутости. \en Constructor by thicknesses and closedness.
  SweptValues( double t1, double t2, bool c = true )
    : thickness1  ( t1   )
    , thickness2  ( t2   )
    , shellClosed ( c    )
    , checkSelfInt( true )
    , mergeFlags   (      )
  {}
  /// \ru Конструктор копирования. \en Copy-constructor.
  SweptValues( const SweptValues & other )
    : thickness1  ( other.thickness1   )
    , thickness2  ( other.thickness2   )
    , shellClosed ( other.shellClosed  )
    , checkSelfInt( other.checkSelfInt )
    , mergeFlags  ( other.mergeFlags   )
  {}
  /// \ru Деструктор. \en Destructor.
  virtual ~SweptValues() {}

public:
  /// \ru Это параметры выдавливания? \en This is extrusion parameters?
  virtual bool IsExtrusionValues()  const { return false; }
  /// \ru Это параметры вращения? \en This is rotation parameters?
  virtual bool IsRevolutionValues() const { return false; }
  /// \ru Это параметры кинематики? \en This is "evolution" parameters?
  virtual bool IsEvolutionValues()  const { return false; }
  /// \ru Это параметры операции по сечениям? \en This is "lofted" parameters?
  virtual bool IsLoftedValues()     const { return false; }
  /// \ru Это параметры операции ребра жесткости? \en This is "rib" parameters?
  virtual bool IsRibValues()        const { return false; }

  /// \ru Определить, являются ли объекты равными? \en Determine whether an object is equal?
  virtual bool IsSame( const SweptValues & other, double accuracy ) const;
  /// \ru Определить, являются ли объекты подобными. \en Determine whether the objects are similar. \~
  virtual bool IsSimilar( const MbSweptData & other ) const;
  /// \ru Сделать объекты равным. \en Make objects equal. \~
  virtual bool SetEqual ( const MbSweptData & other );

public:
  /// \ru Функция копирования данных. \en Function of copying data.
  void  Init( const SweptValues & other ) {
    thickness1   = other.thickness1;
    thickness2   = other.thickness2;
    shellClosed  = other.shellClosed;
    checkSelfInt = other.checkSelfInt;
    mergeFlags   = other.mergeFlags;
  }

  /// \ru Получить состояние замкнутости. \en Get the closedness state.
  bool        IsShellClosed() const { return shellClosed; }
  /// \ru Установит состояние замкнутости. \en Set the closedness state.
  void        SetShellClosed( bool cl ) { shellClosed = cl; }

  /// \ru Получить состояние флага проверки самопересечений. \en Get the state of flag of checking self-intersection.
  bool        CheckSelfInt() const { return checkSelfInt; }
  /// \ru Установить состояние флага проверки самопересечений. \en Set the state of flag of checking self-intersection.
  void        SetCheckSelfInt( bool c ) { checkSelfInt = c; }

  /// \ru Сливать ли подобные грани. \en Whether to merge similar faces. 
  bool        MergeFaces() const { return mergeFlags.MergeFaces(); }
  /// \ru Сливать подобные грани. \en Whether to merge similar faces.
  void        SetMergingFaces( bool mf ) { mergeFlags.SetMergingFaces( mf ); }

  /// \ru Сливать ли подобные ребра. \en Whether to merge similar edges. 
  bool        MergeEdges() const { return mergeFlags.MergeEdges(); }
  /// \ru Сливать подобные ребра. \en Whether to merge similar edges. 
  void        SetMergingEdges( bool me ) { mergeFlags.SetMergingEdges( me ); }

  /// \ru Получить управляющие флаги слияния элементов оболочки. \en Get control flags of shell items merging. 
  const MbMergingFlags &  MergingFlags() const { return mergeFlags;  }
  /// \ru Установить управляющие флаги слияния элементов оболочки. \en Set control flags of shell items merging. 
  void                    SetMergingFlags( const MbMergingFlags & f ) { mergeFlags = f; }

  /// \ru Оператор присваивания. \en Assignment operator.
  void       operator = ( const SweptValues & other ) { Init( other ); }

  KNOWN_OBJECTS_RW_REF_OPERATORS( SweptValues ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class.
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры выдавливания или вращения.
           \en The parameters of extrusion or rotation. \~
  \details \ru Параметры выдавливания или вращения кривых с опциями по направлениям. \n
     В операции выдавливания прямым направлением считается направление, сонаправленное
     с вектором выдавливания, а обратным - противоположное направление.
     В операции вращения прямое направлением определяется по оси вращения с помощью правила правой руки.
           \en The parameters of extrusion or rotation of curves with options along the directions. \n
     In the extrusion operations the forward direction is the direction collinear
     with the vector of extrusion and back - the opposite direction.
     In the rotation operation the forward direction is determined by the axis of rotation using the right hand rule. \~
  \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS SweptValuesAndSides: public SweptValues {
public:
  MbSweptSide side1; ///< \ru Параметры выдавливания/вращения в прямом направлении. \en The parameters of extrusion/rotation along the forward direction.
  MbSweptSide side2; ///< \ru Параметры выдавливания/вращения в обратном направлении. \en The parameters of extrusion/rotation along the backward direction.

public:
  /** \brief \ru Конструктор по умолчанию.
             \en Default constructor. \~
    \details \ru Конструктор параметров для построения замкнутой оболочки без тонкой стенки.
      Способ построение в обоих направлениях - на заданную глубину, равную 0.0.
             \en Constructor of parameters for construction of closed shell without the thin wall.
      Method of construction in both directions - to a given depth equal to 0.0. \~
  */
  SweptValuesAndSides()
    : SweptValues()
    , side1      ()
    , side2      ()
  {}
  /** \brief \ru Конструктор по углам вращения или глубинам выдавливания.
             \en Constructor by rotation angles and extrusion depths. \~
    \details \ru Конструктор параметров для построения замкнутой оболочки без тонкой стенки.
      Способ построение в обоих направлениях - на заданную глубину.
             \en Constructor of parameters for construction of closed shell without the thin wall.
      Method of construction in both directions - to a given depth. \~
    \param[in] scalarValue1 - \ru Угол вращения\глубина выдавливания в прямом направлении.
                              \en Angle of rotation\depth of extrusion along the forward direction. \~
    \param[in] scalarValue2 - \ru Угол вращения\глубина выдавливания в обратном направлении.
                              \en Angle of rotation\depth of extrusion along the backward direction. \~
  */
  SweptValuesAndSides( double scalarValue1, double scalarValue2 )
    : SweptValues(              )
    , side1      ( scalarValue1 )
    , side2      ( scalarValue2 )
  {}
  /// \ru Конструктор копирования данных на тех же поверхностях. \en Copy-constructor of data on the same surfaces.
  SweptValuesAndSides( const SweptValuesAndSides & other )
    : SweptValues( other       )
    , side1      ( other.side1 )
    , side2      ( other.side2 )
  {}
  /// \ru Конструктор полного копирования данных. \en Constructor of complete copying of data.
  SweptValuesAndSides( const SweptValuesAndSides & other, MbRegDuplicate * ireg )
    : SweptValues( other             )
    , side1      ( other.side1, ireg )
    , side2      ( other.side2, ireg )
  {}
  /// \ru Деструктор. \en Destructor.
  virtual ~SweptValuesAndSides();

public:
  // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  virtual bool IsSame( const SweptValues & other, double accuracy ) const
  {
    const SweptValuesAndSides * obj = dynamic_cast<const SweptValuesAndSides *>( &other );
    if ( obj != nullptr ) {
      if ( side1.IsSame( obj->side1, accuracy ) && side2.IsSame( obj->side2, accuracy ) ) {
        if ( obj->SweptValues::IsSame( *this, accuracy ) ) {
          return true;
        }
      }
    }
    return false;
  }

public:
  /// \ru Оператор присваивания данных на тех же поверхностях. \en Assignment operator of data copying on the same surfaces.
  void        operator = ( const SweptValuesAndSides & other ) {
    SweptValues::Init( other );
    side1 = other.side1;
    side2 = other.side2;
  }

  /** \brief \ru Преобразовать согласно матрице.
             \en Transform according to the matrix. \~
    \details \ru Преобразовать согласно матрице поверхности в прямом и обратном направлении.
             \en Transform according to the matrix of surface in the forward and backward direction. \~
    \param[in] matr - \ru Матрица преобразования.
                      \en A transformation matrix. \~
    \param[in] iReg - \ru Регистратор.
                      \en Registrator. \~
  */
  void        Transform( const MbMatrix3D & matr, MbRegTransform * iReg = nullptr );
  /** \brief \ru Сдвинуть вдоль вектора.
             \en Move along a vector. \~
    \details \ru Сдвинуть вдоль вектора поверхности в прямом и обратном направлении.
             \en Move along the vector of the surface along the forward and backward direction. \~
    \param[in] to   - \ru Вектор сдвига.
                      \en Translation vector. \~
    \param[in] iReg - \ru Регистратор.
                      \en Registrator. \~
  */
  void        Move     ( const MbVector3D & to, MbRegTransform * iReg = nullptr );
  /** \brief \ru Повернуть вокруг оси.
             \en Rotate around an axis. \~
    \details \ru Повернуть вокруг оси поверхности в прямом и обратном направлении.
             \en Rotate around the axis of the surface along the forward and backward direction. \~
    \param[in] axis  - \ru Ось поворота.
                       \en The rotation axis. \~
    \param[in] angle - \ru Угол поворота.
                       \en The rotation angle. \~
    \param[in] iReg  - \ru Регистратор.
                       \en Registrator. \~
  */
  void        Rotate   ( const MbAxis3D & axis, double angle, MbRegTransform * iReg = nullptr );

  /** \brief \ru Сделать копии поверхностей.
             \en Make copies of surfaces. \~
    \details \ru Если в каком-либо направлении задана поверхность, заменить эту поверхность на ее копию.
             \en If the surface is given in any direction, then replace the surface with its copy. \~
    \param[in] ireg - \ru Регистратор копий.
                      \en Registrator of copies. \~
    \return \ru true, если хотя бы одна поверхность имелась и сдублирована.
            \en True if at least one surface is had and copied. \~
  */
  bool        DuplicateSurfaces( MbRegDuplicate * ireg = nullptr );

public:
        /// \ru Получить поверхность в положительном направлении. \en Get the surface along the positive direction.
  const MbSurface * GetSurface1() const { return side1.GetSurface(); }
        /// \ru Получить поверхность в отрицательном направлении. \en Get the surface along the negative direction.
  const MbSurface * GetSurface2() const { return side2.GetSurface(); }
        /// \ru Получить поверхность в положительном направлении. \en Get the surface along the positive direction.
        MbSurface * SetSurface1()       { return side1.SetSurface(); }
        /// \ru Получить поверхность в отрицательном направлении. \en Get the surface along the negative direction.
        MbSurface * SetSurface2()       { return side2.SetSurface(); }
        /// \ru Установить поверхность в положительном направлении. \en Set the surface along the positive direction.
        void        SetSurface1( const MbSurface * s ) { side1.SetSurface( s ); }
        /// \ru Установить поверхность в отрицательном направлении. \en Set the surface along the negative direction.
        void        SetSurface2( const MbSurface * s ) { side2.SetSurface( s ); }
        /// \ru Поменять поверхности местами. \en Swap surfaces.
        void        ExchangeSurfaces();
        /// \ru Уклон задан. \en Is draft set.
        bool        IsRakeSet() const;
        /// \ru Тонкая стенка задана. \en Is thickness set.
        bool        IsThicknessSet() const;
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры операции выдавливания.
           \en The parameters of extrusion operation. \~
  \details \ru Параметры операции выдавливания кривых с опциями по направлениям. \n
           \en The parameters of extrusion operation of curves with options along directions. \n \~
  \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS ExtrusionValues : public SweptValuesAndSides {
public:

  /** \brief \ru Конструктор по умолчанию.
             \en Default constructor. \~
    \details \ru Конструктор параметров выдавливания для построения замкнутой оболочки без тонкой стенки
      в прямом направлении на величину, равную 10.0.
             \en Constructor of extrusion parameters for construction of closed shell without the thin wall.
      along the forward direction by value 10.0. \~
  */
  ExtrusionValues()
    : SweptValuesAndSides( 10., 0. ) {}
  /** \brief \ru Конструктор по глубинам выдавливания.
             \en Constructor by extrusion depths. \~
    \details \ru Конструктор параметров выдавливания для построения замкнутой оболочки без тонкой стенки.
      Способ построение в обоих направлениях - на заданную глубину.
             \en Constructor of extrusion parameters for construction of closed shell without the thin wall.
      Method of construction in both directions - to a given depth. \~
    \param[in] scalarValue1 - \ru Глубина выдавливания в прямом направлении.
                              \en Depth of extrusion along the forward direction. \~
    \param[in] scalarValue2 - \ru Глубина выдавливания в обратном направлении.
                              \en Depth of extrusion along the backward direction. \~
  */
  ExtrusionValues( double scalarValue1, double scalarValue2 ) 
    : SweptValuesAndSides( scalarValue1, scalarValue2 ) {}
  /// \ru Конструктор копирования, на тех же поверхностях. \en Copy-constructor on the same surfaces.
  ExtrusionValues( const ExtrusionValues & other ) 
    : SweptValuesAndSides( other ) {}
  /// \ru Конструктор копирования. \en Copy-constructor.
  ExtrusionValues( const ExtrusionValues & other, MbRegDuplicate * ireg ) 
    : SweptValuesAndSides( other, ireg ) {}
  /// \ru Деструктор. \en Destructor.
  virtual ~ExtrusionValues();

public:
  // \ru Это параметры выдавливания? \en This is extrusion parameters?
  virtual bool IsExtrusionValues() const { return true; }

  // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  virtual bool IsSame( const SweptValues & other, double accuracy ) const
  {
    const ExtrusionValues * obj = dynamic_cast<const ExtrusionValues *>( &other );
    if ( obj != nullptr ) {
      if ( obj->SweptValuesAndSides::IsSame( *this, accuracy ) )
        return true;
    }
    return false;
  }

public:
  /// \ru Оператор присваивания, на тех же поверхностях. \en Assignment operator on the same surfaces.
  ExtrusionValues & operator = ( const ExtrusionValues & other ) {
    *static_cast<SweptValuesAndSides*>(this) = *static_cast<const SweptValuesAndSides*>(&other);
    return *this;
  }

  KNOWN_OBJECTS_RW_REF_OPERATORS( ExtrusionValues ) // \ru Для работы со ссылками и объектами класса \en For treatment of references and objects of the class
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры операции вращения.
           \en The parameters of revolution operation. \~
  \details \ru Параметры операции вращения кривых с опциями по направлениям. \n
               При построении операции вращения до конструктивной плоскости с вырожденной областью определения
               оболочка будет построена до ближайшего пересечения с указанной плоскостью.
               В случае, если плоскость имеет не вырожденные границы, часть плоскости, до которой будет построено вращение, 
               определяется по положению центра тяжести контура, ограничивающего плоскость, относительно проекции оси вращения на плоскость.
           \en The parameters of revolution operation of curves with options along directions. \n 
               When constructing a rotation operation to a constructive plane with a degenerate domain of definition
               the shell will be constructed to the nearest intersection with the specified plane.
               If the plane has non-degenerate boundaries, the part of the plane up to which the rotation will be constructed is
               is selected according to the position of the center of gravity of the contour limiting the plane relative to the 
               projection of the axis of rotation onto the plane. \~
  \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS RevolutionValues : public SweptValuesAndSides {
public:
  /** \brief \ru Форма топологии.
             \en Topology shape. \~
    \details \ru Форма топологии: 0 - тело типа сферы, 1 - тело типа тора.\n
      Если образующая - не замкнутая плоская кривая, и ось вращения лежит в плоскости кривой,
      то возможно построение тела вращения с топологией типа сферы. В этом случае образующая достраивается до оси вращения.
             \en Topology shape: 0 - sphere, 1 - torus.\n
      If swept is non-closed planar curve and axis of rotation lies on the curve plane,
      then is possible to construct revolution solids with the topology of sphere type. In this case the swept is being updated to the rotation axis.
I \~  */
  int  shape;

public:
   /** \brief \ru Конструктор по умолчанию.
              \en Default constructor. \~
    \details \ru Конструктор параметров вращения для построения замкнутой оболочки типа тора
      без тонкой стенки в прямом направлении на полный оборот.
             \en Constructor of revolution parameters for construction of closed shell of torus type
      without thin wall along the forward direction at full turn. \~
  */
  RevolutionValues()
    : SweptValuesAndSides( M_PI, 0. )
    , shape( 1 )
  {}
  /** \brief \ru Конструктор по углам вращения.
             \en Constructor by revolution angles. \~
    \details \ru Конструктор параметров вращения для построения замкнутой оболочки без тонкой стенки.
      Способ построение в обоих направлениях - на заданную глубину (заданный угол).
             \en Constructor of revolution parameters for construction of closed shell without the thin wall.
      Method of construction in both directions - to a given depth (given angle). \~
    \param[in] scalarValue1 - \ru Угол вращение в прямом направлении.
                              \en Revolution angle along the forward direction. \~
    \param[in] scalarValue2 - \ru Угол вращения в обратном направлении.
                              \en Revolution angle along the backward direction. \~
    \param[in] s            - \ru Форма топологии.
                              \en Topology shape. \~
  */
  RevolutionValues( double scalarValue1, double scalarValue2, int s )
    : SweptValuesAndSides( scalarValue1, scalarValue2 )
    , shape( s )
  {}
  /// \ru Конструктор копирования, на тех же поверхностях. \en Copy-constructor on the same surfaces.
  RevolutionValues( const RevolutionValues & other )
    : SweptValuesAndSides( other )
    , shape( other.shape )
  {}
  /// \ru Конструктор копирования. \en Copy-constructor.
  RevolutionValues( const RevolutionValues & other, MbRegDuplicate * ireg )
    : SweptValuesAndSides( other, ireg )
    , shape( other.shape )
  {}
  /// \ru Деструктор. \en Destructor.
  virtual ~RevolutionValues();

public:
  // \ru Это параметры вращения? \en This is rotation parameters?
  virtual bool IsRevolutionValues() const { return true; }

  // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  virtual bool IsSame( const SweptValues & other, double accuracy ) const
  {
    const RevolutionValues * obj = dynamic_cast<const RevolutionValues *>( &other );
    if ( obj != nullptr ) {
      if ( obj->shape == shape ) {
        if ( obj->SweptValuesAndSides::IsSame( *this, accuracy ) )
          return true;
      }
    }
    return false;
  }

public:
  /// \ru Оператор присваивания, на тех же поверхностях. \en Assignment operator on the same surfaces.
  RevolutionValues & operator = ( const RevolutionValues & other ) {
    *static_cast<SweptValuesAndSides*>(this) = *static_cast<const SweptValuesAndSides*>(&other);
    shape = other.shape;
    return *this;
  }

  KNOWN_OBJECTS_RW_REF_OPERATORS( RevolutionValues ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class.
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры кинематической операции.
           \en Parameters of the sweeping operation. \~
  \details \ru Параметры операции движения образующей по направляющей кривой. \n
           \en The operation parameters of moving the generating curve along the spine curve. \n \~
  \ingroup Shell_Building_Parameters
*/
// ---
struct MATH_CLASS EvolutionValues : public SweptValues {
public:
  /// \ru Способы переноса образующего объекта вдоль направляющей. \en Moving method of generating object along the spine curve.
  enum ModesList {
    eom_Parallel        = 0x00,  // 00000  ///< \ru Образующая переносится параллельно самой себе. \en Generating curve is moved parallel to itself. 
    eom_KeepingAngle    = 0x01,  // 00001  ///< \ru Образующая при переносе сохраняет исходный угол с направляющей. \en Generating curve when moving preserves initial angle with spine. 
    eom_Orthogonal      = 0x02,  // 00010  ///< \ru Плоскость образующей выставляется и сохраняется ортогональной направляющей. \en Plane of generating curve is set and saved as orthogonal to spine. 
    eom_BySurfaceNormal = 0x04,  // 00100  ///< \ru Образующая переносится согласованно с нормалью к поверхности. \en Generating object is moved according to surface normal. 
  };

protected:
  /** \brief \ru Способ переноса образующего контура вдоль направляющей.
             \en Moving method of generating contour along the spine curve. \~
    \details \ru Способ переноса образующего контура вдоль направляющей: \n
             mode <= 0  - Образующая переносится параллельно самой себе; \n
             mode == 1  - Образующая при переносе сохраняет исходный угол с направляющей; \n
             mode == 2  - Плоскость образующей выставляется и сохраняется ортогональной направляющей. \n
             mode >  3  - Образующая переносится согласованно с нормалью к поверхности. \n
             \en Moving method of generating contour along the spine curve: \n
             mode <= 0  - Generating curve is moved parallel to itself; \n
             mode == 1  - Generating curve when moving preserves initial angle with spine; \n
             mode == 2  - Plane of generating curve is set and saved as orthogonal to spine. \n 
             mode >  3  - Generating object is moved according to surface normal. \n \~
  */
  int                   mode; // "mode" or "parallel" on documentation pictures.
public:
  // \ru Данные о функциях изменения образующих кривых вдоль направляющей кривой (могут быть nullptr). \en Data about changes of generating curves along the guide curve (can be nullptr).
  double                range;   ///< \ru Эквидистантное смещение точек образующей кривой в конце траектории. \en The offset range of generating curve on the end of spine curve. 
  SPtr<MbFunction>      scaling; ///< \ru Функция масштабирования образующей кривой. \en The function of curve scale. 
  SPtr<MbFunction>      winding; ///< \ru Функция вращения образующей кривой. \en The function of curve rotation. 
  c3d::ConstSurfaceSPtr surface; ///< \ru Поверхность для управления направляющей кривой MbSpine. \en The surface for guide curve control (for MbSpine). 
private:
  bool                  fixShellSelfInt; ///< \ru Флаг обработки по устранению самопересечений оболочки. \en Flag for shell self-intersection fix.

public:

  /** \brief \ru Конструктор по умолчанию.
             \en Default constructor. \~
    \details \ru Конструктор параметров кинематической операции для построения замкнутой оболочки
      без тонкой стенки с сохранением угла наклона.
             \en Constructor of sweeping operation parameters for construction of closed shell
      without the thin wall with keeping the angle inclination. \~
  */
  EvolutionValues    ()
    : SweptValues    ()
    , mode           ( eom_KeepingAngle )
    , range          ( 0.0 )
    , scaling        ( nullptr )
    , winding        ( nullptr )
    , surface        ( nullptr )
    , fixShellSelfInt( false )
  {}
  /// \ru Конструктор копирования. \en Copy-constructor.
  EvolutionValues( const EvolutionValues & other );
  /// \ru Деструктор. \en Destructor.
  virtual ~EvolutionValues();

public:
  // \ru Это параметры кинематики? \en This is "evolution" parameters?
  virtual bool          IsEvolutionValues() const { return true; }

  // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  virtual bool          IsSame( const SweptValues & other, double accuracy ) const;
  // \ru Определить, являются ли объекты подобными. \en Determine whether the objects are similar. \~
  virtual bool          IsSimilar( const SweptValues & other ) const;
  // \ru Сделать объекты равным. \en Make objects equal. \~
  virtual bool          SetEqual ( const SweptValues & other );

  /// \ru Копировать значение режима операции. \en Copy operation mode.
          void          CopyMode( const EvolutionValues & ev ) { mode = ev.mode; }
  /// \ru Получить значение режима операции. \en Get operation mode. 
          int           GetMode() const { return mode; }
  /// \ru Переносится ли образующая параллельно самой себе. \en Whether generating curve is moved parallel to itself. 
          bool          IsParallel()      const { return (mode < 1);  }
  /// \ru Сохраняет ли образующая при переносе исходный угол с направляющей. \en Whether generating curve when moving preserves initial angle with spine.
          bool          IsKeepingAngle()  const { return !!(mode & eom_KeepingAngle); }
  /// \ru Выставляется ли плоскость образующей ортогонально направляющей. \en Whether plane of generating curve is set and saved as orthogonal to spine.
          bool          IsOrthogonal()    const { return !!(mode & eom_Orthogonal); }
  /// \ru Переносится ли образующая согласованно с нормалью к поверхности. \en Whether generating object is moved according to surface normal.
          bool          BySurfaceNormal() const { return !!(mode & eom_BySurfaceNormal); }

  /// \ru Переносить образующая параллельно самой себе. \en Move generating curve parallel to itself. 
          void          SetParallel()        { mode = eom_Parallel; }
  /// \ru Сохранять при переносе исходный угол между образующей и направляющей. \en Preserve initial angle between generatrix and spine when moving.
          void          SetKeepingAngle()    { mode = eom_KeepingAngle; }
  /// \ru Выставлять плоскость образующей ортогонально направляющей. \en Set and keep plane of generating curve as orthogonal to spine.
          void          SetOrthogonal()      { mode = eom_Orthogonal; }
  /// \ru Переносить образующую согласованно с нормалью к поверхности. \en Move generating object according to surface normal.
          bool          SetBySurfaceNormal( bool s )
          { 
            if ( !IsParallel() ) {
              if ( s ) mode |= eom_BySurfaceNormal;
              else     mode ^= eom_BySurfaceNormal;
              return true;
            }
            return false;
          }
  /// \ru Выдать функцию масштабирования образующей кривой. \en Get the function of curve scale. 
          double        GetRange() const { return range; }
          double &      SetRange() { return range; }
          void          SetRange( double r ) { range = r; }

  /** \brief \ru Добавить данные.
             \en Add data. \~
    \details \ru Добавить данные об изменении образующих контурах на поверхности вдоль образующей кривой.
             \en Add data about changes of generating contours on the surface along the guide curve. \~
    \param[in] _scaling - \ru Масштабирование.
                          \en The scaling. \~
    \param[in] _winding - \ru Поворот.
                          \en The winding. \~
  */
          bool          AddData( MbFunction & _scaling, MbFunction & _winding );

  /// \ru Выдать функцию масштабирования образующей кривой. \en Get the function of curve scale. 
  const   MbFunction *  GetScaling() const { return scaling; }
          MbFunction *  SetScaling()       { return scaling; }

  /// \ru Выдать функцию вращения образующей кривой. \en Get the function of curve rotation. 
  const   MbFunction *  GetWinding() const { return winding; }
          MbFunction *  SetWinding()       { return winding; }

  /// \ru Выдать поверхность для направляющей кривой MbSpine. \en Get the surface for guide curve MbSpine. 
  const   MbSurface *   GetSurface() const { return surface; }
          void          SetSurface( const MbSurface & surf );

          /// \ru Устранять ли самопересечения оболочки. \en Whether to fix shell self-intersections. 
          bool FixShellSelfIntersections() const { return fixShellSelfInt; }
          /// \ru Устранять ли самопересечения оболочки. \en Whether to fix shell self-intersections. 
          void SetFixShellSelfIntersections( bool b ) { fixShellSelfInt = b; }

public:
  /// \ru Оператор присваивания. \en Assignment operator.
  EvolutionValues & operator = ( const EvolutionValues & other );

  KNOWN_OBJECTS_RW_REF_OPERATORS( EvolutionValues ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class.
};


//------------------------------------------------------------------------------
/** \brief \ru Абстрактный базовый класс для получения сечений в кинематической операции с динамической параметризацией сечения.
           \en An Abstract Base Class for obtaining sections for sweeping operation with variable section. \~
  \details \ru Абстрактный базовый класс для получения сечений в кинематической операции с динамической параметризацией сечения. \n
               Реализация должна быть выполнена на стороне пользователя. \n
           \en An Abstract Base Class for obtaining sections for sweeping operation with variable section. \n
               Actual implementation must be provided by the user. \n
           \~
  \ingroup Shell_Building_Parameters
  \warning \ru В разработке. \en Under development.
*/
// ---
class MATH_CLASS IVariableSectionData {
public:
  /// \ru Деструктор. \en Destructor.
  virtual ~IVariableSectionData(){}

  /** \brief \ru Получить сечение.
             \en Obtain a section. \~
    \param[in]  t        - \ru Значение параметра вдоль траектории.
                           \en Value of parameter along the trajectory. \~
    \param[in]  place    - \ru Плейсмент сечения.
                           \en Placement of the section. \~
    \param[out] contours - \ru Множество двумерных контуров.
                           \en Array of plane contours. \~
    \return \ru Возвращает "true" в случае успеха.
            \en Returns "true" in case of success. \~
  */
  virtual bool GetSection(       double                         t,
                           const MbPlacement3D &                place,
                                 c3d::PlaneContoursSPtrVector & contours ) const = 0;
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры операции построения тела по плоским сечениям.
           \en The operation parameters of constructing solid by lofted. \~
  \details \ru Параметры операции построения тела по плоским сечениям, заданных контурами. \n
           \en The parameters of constructing operation by lofted which are given by contours. \n \~
  \ingroup Shell_Building_Parameters
*/
// ---
struct MATH_CLASS LoftedValues : public SweptValues {
public:
  bool       closed;      ///< \ru Замкнутость трубки сечений. \en Closedness of tube.
  MbVector3D vector1;     ///< \ru Производная в начале. \en The derivative at the start.
  MbVector3D vector2;     ///< \ru Производная в конце. \en The derivative at the end.
  bool       setNormal1;  ///< \ru Установлена нормаль в начале, если начальное сечение точечное. \en The normal is set at the start, if first section is point curve.
  bool       setNormal2;  ///< \ru Установлена нормаль в конце, если начальное сечение точечное. \en The normal is set at the end, if last section is point curve.
  double     derFactor1;  ///< \ru Множитель величины производной при установке нормали в начале. По умолчанию 1.0. \en The modifier of the derivative when setting the normal at the beginning. The default is 1.0.
  double     derFactor2;  ///< \ru Множитель величины производной при установке нормали в конце. По умолчанию 1.0. \en The modifier of the derivative when setting the normal at the end. The default is 1.0.
  MbVector3D directSurf1; ///< \ru Ось направления движения поверхности в начале при установке нормали. \en Direction axis of the surface progress near the starting curve when setting the normal.
  MbVector3D directSurf2; ///< \ru Ось направления движения поверхности в конце при установке нормали. \en Direction axis of the surface progress near the ending curve when setting the normal.

public:
  /** \brief \ru Конструктор по умолчанию.
             \en Default constructor. \~
    \details \ru Конструктор параметров операции по сечениям для построения замкнутой оболочки без тонкой стенки.
             \en Constructor of lofted operation parameters for construction of closed shell without the thin wall. \~
  */
  LoftedValues()
    : SweptValues (               )
    , closed      ( false         )
    , vector1     ( 0.0, 0.0, 0.0 )
    , vector2     ( 0.0, 0.0, 0.0 )
    , setNormal1  ( false         )
    , setNormal2  ( false         )
    , derFactor1  ( 1.0           )
    , derFactor2  ( 1.0           )
    , directSurf1 ( UNDEFINED_DBL, UNDEFINED_DBL, UNDEFINED_DBL )
    , directSurf2 ( UNDEFINED_DBL, UNDEFINED_DBL, UNDEFINED_DBL )
  {}
  /// \ru Конструктор копирования. \en Copy-constructor.
  LoftedValues( const LoftedValues & other )
    : SweptValues ( other            )
    , closed      ( other.closed     )
    , vector1     ( other.vector1    )
    , vector2     ( other.vector2    )
    , setNormal1  ( other.setNormal1 )
    , setNormal2  ( other.setNormal2 )
    , derFactor1  ( other.derFactor1 )
    , derFactor2  ( other.derFactor2 )
    , directSurf1 ( other.directSurf1 )
    , directSurf2 ( other.directSurf2 )
  {}
  /// \ru Оператор присваивания. \en Assignment operator.
  LoftedValues & operator = ( const LoftedValues & other )
  {
    SweptValues::Init( other );
    closed      = other.closed;
    vector1     = other.vector1;
    vector2     = other.vector2;
    setNormal1  = other.setNormal1;
    setNormal2  = other.setNormal2;
    directSurf1 = other.directSurf1;
    directSurf2 = other.directSurf2;
    return *this;
  }
  /// \ru Деструктор. \en Destructor.
  virtual ~LoftedValues();

public:
  // \ru Это параметры операции по сечениям? \en This is "lofted" parameters?
  virtual bool IsLoftedValues() const { return true; }

  // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  virtual bool IsSame( const SweptValues & other, double accuracy ) const
  {
    const LoftedValues * obj = dynamic_cast<const LoftedValues *>( &other );
    if ( obj != nullptr ) {
      if ( obj->closed == closed ) {
        if ( c3d::EqualVectors(vector1, obj->vector1, accuracy) && c3d::EqualVectors(vector2, obj->vector2, accuracy) ) {
          if ( obj->setNormal1 == setNormal1 && obj->setNormal2 == setNormal2 ) {
            if ( (setNormal1 == false || obj->derFactor1 == derFactor1 || c3d::EqualVectors(directSurf1, obj->directSurf1, accuracy)) &&  // Фактор может различаться, если нормаль не установлена.
                 (setNormal2 == false || obj->derFactor2 == derFactor2 || c3d::EqualVectors(directSurf2, obj->directSurf2, accuracy)) ) {
              if ( obj->SweptValues::IsSame(*this, accuracy) ) {
                return true;
              }
            }
          }
        }
      }
    }
    return false;
  }

public:
  /// \ru Преобразовать объект согласно матрице. \en Transform an object according to the matrix.
  void        Transform( const MbMatrix3D & matr );
  /// \ru Сдвинуть объект вдоль вектора. \en Move an object along a vector.
  void        Move     ( const MbVector3D & to );
  /// \ru Повернуть объект вокруг оси на заданный угол. \en Rotate an object at a given angle around an axis.
  void        Rotate   ( const MbAxis3D & axis, double ang );

  KNOWN_OBJECTS_RW_REF_OPERATORS( LoftedValues ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class.
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры ребра жёсткости.
           \en Parameters of a rib. \~
  \details \ru Параметры построения ребра жёсткости по кривой, задающей его форму. \n
           \en The construction parameters of rib by curve gives its shape. \n \~
  \ingroup Sheet_Metal_Building_Parameters
*/
// ---
struct MATH_CLASS RibValues : public SweptValues {
public:
  /** \brief \ru Сторона заполнения пространства телом ребра.
             \en The side to place the rib on. \~
    \details \ru С какой стороны от кривой располагается ребро. \n
             \en With which side of the curve is rib. \n \~
    \ingroup Sheet_Metal_Building_Parameters
  */
  enum ExtrudeSide {
    es_Left  = 0,     ///< \ru Ребро выдавливается в левую сторону от кривой вдоль плоскости. \en Rib is extruded to the left side of the curve along the plane.
    es_Right,         ///< \ru Ребро выдавливается в правую сторону от кривой вдоль плоскости. \en Rib is extruded to the right side of the curve along the plane.
    es_Up,            ///< \ru Ребро выдавливается в сторону нормали плоскости. \en Rib is extruded to the side of the surface normal.
    es_Down,          ///< \ru Ребро выдавливается в сторону против нормали плоскости. \en Rib is extruded to the side opposite to the surface normal.
  };

public:
  double      angle1; ///< \ru Угол уклона плоскости в прямом направлении. \en Draft angle of the plane along the forward direction.
  double      angle2; ///< \ru Угол уклона плоскости в обратном направлении. \en Draft angle of the plane along the backward direction.
  ExtrudeSide side;   ///< \ru Сторона заполнения пространства телом ребра. \en The side to place the rib on.

public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  RibValues()
    : SweptValues(          )
    , angle1     ( 0.0      )
    , angle2     ( 0.0      )
    , side       ( es_Right )
  {}
  /// \ru Конструктор по толщинам, углам и стороне заполнения пространства. \en Constructor by thickness, angles and filling space.
  RibValues( double t1, double t2, double a1, double a2, int s )
    : SweptValues( t1, t2          )
    , angle1     ( a1              )
    , angle2     ( a2              )
    , side       ( (ExtrudeSide)s  )
  {}
  /// \ru Конструктор копирования. \en Copy-constructor.
  RibValues( const RibValues & other )
    : SweptValues( other        )
    , angle1     ( other.angle1 )
    , angle2     ( other.angle2 )
    , side       ( other.side   )
  {}
  /// \ru Деструктор. \en Destructor.
  virtual ~RibValues();

public:
  // \ru Это параметры операции ребра жесткости? \en This is "rib" parameters?
  virtual bool IsRibValues() const { return true; }

  // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  virtual bool  IsSame( const SweptValues & other, double accuracy ) const
  {
    const RibValues * obj = dynamic_cast<const RibValues *>( &other );

    if ( obj != nullptr ) {
      if ( obj->side == side ) {
        if ( ::fabs(obj->angle1 - angle1) < accuracy && ::fabs(obj->angle2 - angle2) < accuracy )
          return SweptValues::IsSame( *obj, accuracy );
      }
    }
    return false;
  }

public:
  /// \ru Функция копирования. \en Copy function.
  void          Init( const RibValues & other )
  {
    SweptValues::Init( other );
    angle1      = other.angle1;
    angle2      = other.angle2;
    side        = other.side;
  }
  /// \ru Оператор присваивания. \en Assignment operator.
  RibValues &   operator = ( const RibValues & other )
  {
    Init( other );
    return *this;
  }

  KNOWN_OBJECTS_RW_REF_OPERATORS( RibValues ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class.
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры ребра жёсткости листового тела.
           \en Parameters of a sheet metal rib. \~
  \details \ru Параметры построения ребра жёсткости листового тела по кривой, задающей его форму. \n
           \en The construction parameters of a sheet metal rib by curve gives its shape. \n \~
\ingroup Sheet_Metal_Building_Parameters
*/
// ---
struct MATH_CLASS SheetRibValues: public RibValues {
public:
  double radRibConvex;   ///< \ru Радиус скругления выпуклой части ребра жесткости. \en Fillet radius of convex part of rib.
  double radSideConcave; ///< \ru Радиус скругления примыкания вогнутой части ребра жесткости к листовому телу. \en Fillet radius of connection of concave part of rib and metal sheet.

public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  SheetRibValues()
    : RibValues     (     )
    , radRibConvex  ( 0.0 )
    , radSideConcave( 0.0 )
  {}
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  SheetRibValues( double t1, double t2, double a1, double a2, int s, double rFilletRib, const double & rFilletSide )
    : RibValues     ( t1, t2, a1, a2, s   )
    , radRibConvex  ( ::fabs(rFilletRib)  )
    , radSideConcave( ::fabs(rFilletSide) )
  {}
  /// \ru Конструктор копирования. \en Copy-constructor.
  SheetRibValues( const SheetRibValues & other )
    : RibValues     ( other                )
    , radRibConvex  ( other.radRibConvex   )
    , radSideConcave( other.radSideConcave )
  {}
  /// \ru Деструктор. \en Destructor.
  virtual ~SheetRibValues();

public:
  // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  virtual bool  IsSame( const SweptValues & other, double accuracy ) const
  {
    const SheetRibValues * obj = dynamic_cast<const SheetRibValues *>( &other );

    if ( obj != nullptr ) {
      if ( (::fabs(radRibConvex - obj->radRibConvex) < accuracy) && (::fabs(radSideConcave - obj->radSideConcave) < accuracy) )
        return RibValues::IsSame( *obj, accuracy );
    }
    return false;
  }

public:
  /// \ru Функция копирования. \en Copy function.
  void        Init( const SheetRibValues & other )
  {
    RibValues::Init( other );
    radRibConvex = other.radRibConvex;
    radSideConcave = other.radSideConcave;
  }

  /// \ru Оператор присваивания. \en Assignment operator.
  SheetRibValues & operator = ( const SheetRibValues & other ) {
    Init( other );
    return *this;
  }

  /// \ru Преобразовать объект согласно матрице. \en Transform an object according to the matrix.
  void        Transform( const MbMatrix3D & matr );

  KNOWN_OBJECTS_RW_REF_OPERATORS( SheetRibValues ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class.
};

//------------------------------------------------------------------------------
/** \brief \ru Параметры булевой операции выдавливания или вращения до объекта.
           \en The parameters of Boolean operation of extrusion or revolution to object. \~
  \details \ru Параметры булевой операции выдавливания или вращения до объекта. \n
    Используется при булевой операции исходного тела
    и построенной операции выдавливания или вращения двумерных контуров на поверхности.
           \en The parameters of Boolean operation of extrusion or revolution to object. \n
    Used in Boolean operation of initial solid
    and constructed operation of extrusion or revolution of two-dimensional contours on the surface. \~
  \ingroup Shell_Building_Parameters
*/
// ---
struct MATH_CLASS MbSweptLayout {
public:
  /** \brief \ru Направление выдавливания (вращения).
             \en A direction of extrusion (revolution). \~
    \details \ru Направление выдавливания (вращения) по отношению к вектору выдавливания (оси вращения).
             \en The direction of extrusion relative to the extrusion vector. \~
  */
  enum Direction {
    ed_minus_minus = -2, ///< \ru В обратном направлении, для обеих строн. \en Along the backward direction, for both sides.
    ed_minus       = -1, ///< \ru В обратном направлении, для одной стороны. \en Along the backward direction, for one sides.
    ed_both        = 0,  ///< \ru В обоих направлениях. \en Along both directions.
    ed_plus        = 1,  ///< \ru В прямом направлении, для одной стороны. \en Along the forward direction, for one sides.
    ed_plus_plus   = 2,  ///< \ru В прямом направлении, для обеих сторон. \en Along the forward direction, for both sides.
  };
  Direction   direction; ///< \ru Направление выдавливания относительно вектора. \en The direction of extrusion relative to the vector.
  bool        skipUnion; ///< \ru Создавать новое тело (Не приклеивать к телу). \en Create a new solid.

protected:
  SPtr<const MbSurface> surface;  ///< \ru Поверхность, на которой размещена образующая. \en The surface, which contains the generating curve.

protected:
  /// \ru Конструктор. \en Constructor.
  MbSweptLayout( const MbSurface & surf, Direction dir ) : direction( dir ), skipUnion( false ), surface( &surf ) {}
  /// \ru Конструктор копирования. \en Copy-constructor.
  MbSweptLayout( const MbSweptLayout & other ) : direction( other.direction ), skipUnion( other.skipUnion ), surface( other.surface ) {}
  /// \ru Деструктор. \en Destructor.
  virtual ~MbSweptLayout();
public:
  /// \ru Получить поверхность. \en Get the surface.
  const MbSurface & GetSurface() const { return *surface; }

  /// \ru Создавать новое тело (Не приклеивать к телу). \en Create a new solid.
  bool SkipUnion() const { return skipUnion; }
  /// \ru Создавать новое тело (Не приклеивать к телу). \en Create a new solid.
  void SkipUnion( bool su ) { skipUnion = su; }
public:
  /// \ru Это параметры выдавливания? \en This is extrusion parameters?  
  virtual bool IsExtrusionLayout() const { return false; }
  /// \ru Это параметры вращения? \en This is rotation parameters?
  virtual bool IsRevolutionLayout() const { return false; }
public:
  /// \ru Классификация точки относительно несущей поверхности. \en Classification point relative to the surface.
  MbeItemLocation PointRelative( const MbCartPoint3D & p ) const;
private:
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default.
  MbSweptLayout &        operator = ( const MbSweptLayout & );
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры булевой операции выдавливания до объекта.
           \en The parameters of Boolean operation of extrusion to object. \~
  \details \ru Параметры булевой операции выдавливания до объекта. \n
    Используется при булевой операции исходного тела
    и построенной операции выдавливания двумерных контуров на поверхности.
           \en The parameters of Boolean operation of extrusion to object. \n
    Used in Boolean operation of initial solid
    and constructed operation of extrusion of two-dimensional contours on the surface. \~
  \ingroup Shell_Building_Parameters
*/
// ---
struct MATH_CLASS MbExtrusionLayout : public MbSweptLayout {
public:
  MbVector3D dirVector; ///< \ru Вектор выдавливания. \en An extrusion vector.
  double     rake1;     ///< \ru Уклон в прямом направлении. \en Slope in the forward direction.
  double     rake2;     ///< \ru Уклон в обратном направлении. \en Slope in the opposite direction.

public:
  /// \ru Конструктор. \en Constructor.
  MbExtrusionLayout( const MbSurface & surf, Direction dir, const MbVector3D & dirVec )
    : MbSweptLayout( surf, dir )
    , dirVector( dirVec ) 
    , rake1( 0.0 )
    , rake2( 0.0 )
  {}
  /// \ru Конструктор. \en Constructor.
  MbExtrusionLayout( const MbSurface & surf, Direction dir, double initRake1, double initRake2, const MbVector3D & dirVec )
    : MbSweptLayout( surf, dir )
    , dirVector( dirVec ) 
    , rake1( initRake1 )
    , rake2( initRake2 )
  {}
  /// \ru Конструктор копирования. \en Copy-constructor.
  MbExtrusionLayout( const MbExtrusionLayout & other ) 
    : MbSweptLayout( other )
    , dirVector( other.dirVector )
    , rake1( other.rake1 )
    , rake2( other.rake2 )
  {}
  /// \ru Деструктор. \en Destructor.
  virtual ~MbExtrusionLayout();
public:
  /// \ru Это параметры выдавливания? \en This is extrusion parameters?  
  virtual bool IsExtrusionLayout() const { return true; }
private:
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default.
  void        operator = ( const MbExtrusionLayout & );
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры булевой операции вращения до объекта.
           \en The parameters of Boolean operation of revolution to object. \~
  \details \ru Параметры булевой операции вращения до объекта. \n
    Используется при булевой операции исходного тела
    и построенной операции вращения двумерных контуров на поверхности.
           \en The parameters of Boolean operation of revolution to object. \n
    Used in Boolean operation of initial solid
    and constructed operation of revolution of two-dimensional contours on the surface. \~
  \ingroup Shell_Building_Parameters
*/
// ---
struct MATH_CLASS MbRevolutionLayout : public MbSweptLayout {
public:
  MbAxis3D revAxis; ///< \ru Ось вращения. \en An revolution axis.

public:
  /// \ru Конструктор. \en Constructor.
  MbRevolutionLayout( const MbSurface & surf, Direction dir, const MbAxis3D & rotAxis ) : MbSweptLayout( surf, dir ), revAxis( rotAxis ) {}
  /// \ru Конструктор копирования. \en Copy-constructor.
  MbRevolutionLayout( const MbRevolutionLayout & other ) : MbSweptLayout( other ), revAxis( other.revAxis ) {}
  /// \ru Деструктор. \en Destructor.
  virtual ~MbRevolutionLayout();
public:
  /// \ru Это параметры вращения? \en This is rotation parameters?
  virtual bool IsRevolutionLayout() const { return true; }
private:
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default.
  void        operator = ( const MbRevolutionLayout & );
};


//------------------------------------------------------------------------------
/** \brief \ru Данные края сечения поверхности.
           \en The surface section control function. \~
  \details \ru Точку края сечения определяют: или рёбра, или кривые. Направление сечения на краю определяют: или поверхности смежных граней рёбер, или поверхности граней, или функция угла наклона. 
           \en The end of the section is determined as point either edges, or curves. The direction on the end of the section is determined as either the surfaces of edges, or the surfaces of faces, or a function of the angle.
  \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbSectionRail {

private:
  std::vector<c3d::SolidSPtr>      solids;    ///< \ru Тела опорных граней или направляющих рёбер (могут отсутствовать). \en The solids of reference faces or guide edges (may be empty). \~
  std::vector<c3d::FaceSPtr>       faces;     ///< \ru Опорные грани (могут отсутствовать). \en The reference faces (may be empty). \~
  std::vector<bool>                faceSide;  ///< \ru С каких сторон касаться поверхностей при form==cs_Linea (синхронно с faces). \en On which sides to touch surfaces when form==cs_Linea (synchronously with faces). \~
  std::vector<MbItemIndex>         faceIndex; ///< \ru Номера опорных граней. \en The reference face numbers (may be empty). \~
  std::vector<c3d::EdgeSPtr>       edges;     ///< \ru Направляющие рёбра (могут отсутствовать). \en The guide edges (may be empty). \~
  std::vector<bool>                edgeSide;  ///< \ru С какой гранью ребра гладко стыковать поверхность (синхронно с edges). \en What face of edge should the surface join smoothly to (synchronously with edges). \~
  std::vector<MbItemIndex>         edgeIndex; ///< \ru Номера направляющих рёбер (могут отсутствовать). \en The guide edge numbers (may be empty). \~
  std::vector<c3d::SpaceCurveSPtr> curves;    ///< \ru Направляющие кривые (могут отсутствовать). \en The guide curves (may be empty). \~
  c3d::SpaceCurveSPtr              track;     ///< \ru Кривая, через которую должно пройти сечение (может отсутствовать). \en The curve that the section should pass through (may be nullptr).  \~
  c3d::FunctionSPtr                function;  ///< \ru Функция угла наклона, или длины, или радиуса (может отсутствовать). \en The function of the angle of inclination, or of the length, or of the fillet radius (may be nullptr). \~
  ThreeStates                      state;     ///< \ru Как использовать function: угол к хорде или направлению (ts_neutral), отклонение от касательной поверхности (ts_positive), отклонение от нормали к поверхности (ts_negative). \~ 
                                              ///< \en How to use the function: the angle to chord or to direction (ts_neutral), deviation from tangent surface (ts_positive), deviation from normal to surface (ts_negative). \~ 
public:

  /// \ru Конструктор по умолчанию. \en Empty constructor.
  MbSectionRail()
  : solids   ()
  , faces    ()
  , faceSide ()
  , faceIndex()
  , edges    ()
  , edgeSide ()
  , edgeIndex()
  , curves   ()
  , track    ( nullptr )
  , function ( nullptr )
  , state    ( ts_neutral )
  {}

  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \param[in] eds   - \ru Направляющие рёбра.
                       \en The guide edges. \~
    \param[in] eSide - \ru Какую сторону кривой гладко стыковать с поверхностью (синхронно с edges).
                       \en Which side should the surface join smoothly to (synchronously with edges). \~
    \param[in] fcs   - \ru Направляющие грани (могут отсутствовать).
                       \en The guide faces (may be empty). \~
    \param[in] fSide - \ru С каких сторон касаться поверхностей (синхронно с faces).
                       \en On which sides to touch surfaces (synchronously with faces). \~
    \param[in] cs    - \ru Направляющие кривые (могут отсутствовать). 
                       \en The guide curves (may be empty). \~
    \param[in] trk   - \ru Кривая, через которую должно пройти сечение (может отсутствовать). 
                       \en The curve that the section should pass through (may be nullptr). \~
    \param[in] ang   - \ru Функция угла наклона (может быть nullptr).
                       \en The function of the angle of inclination (may be nullptr). \~
    \param[in] st    - \ru Как использовать ang.
                       \en How to use ang. \~
  */
  MbSectionRail( std::vector<MbCurveEdge *> & edges_, std::vector<bool> & eSides, 
                 std::vector<MbFace *> & faces_, std::vector<bool> & fSides, 
                 std::vector<MbCurve3D *> & cs, MbCurve3D * trk,
                 MbFunction * ang, ThreeStates st );
  /// \ru Конструктор копирования. \en Copy-constructor.
  MbSectionRail( const MbSectionRail & other );
  /// \ru Конструктор копирования. \en Copy-constructor.
  MbSectionRail( const MbSectionRail & other, MbRegDuplicate * ireg );
  /// \ru Конструктор копирования без копирования указателей. \en Copy-constructor without pointer copy.
  MbSectionRail( MbFilletBundle & other );
  /// \ru Деструктор. \en Destructor.
  ~MbSectionRail();

public:

  /// \ru Выдать тела опорных граней или направляющих рёбер. \en Get solids of reference faces or guide edges.
  void            GetSolids( std::vector<MbSolid *> & sols ) const;
  void            GetSolids( RPArray<MbSolid> & sols ) const;
  /// \ru Выдать количество тел опорных граней или направляющих рёбер. \en Get solids of reference faces or guide edges count.
  size_t          GetSolidsCount() const { return solids.size(); }
  /// \ru Выдать тело по индексу. \en Get solid by index.
  const MbSolid * GetSolid( size_t i ) const { return ( i < solids.size() ) ? solids[i].get() : nullptr; }
  MbSolid *       SetSolid( size_t i )       { return ( i < solids.size() ) ? solids[i].get() : nullptr; }
  /// \ru Очистить контейр тел. \en Solid conteiner clear.
  void            SolidsClear() { solids.clear(); }
  /// \ru Выдать уникальные тела. \en Give out unique solids.
  void            GetUniqueSolids( std::vector<MbSolid *> & sols ) const;

  /// \ru Добавить в данные поверхность. \en Add surface to data. \~
  void            AddFace( MbFace & _face, bool side, MbSolid * solid = nullptr );
  /// \ru Выдать грани. \en Get faces.
  void            GetFaces( std::vector<MbFace *> & fas ) const;
  void            GetFaces( RPArray<MbFace> & fas ) const;
  /// \ru С каких сторон касаться поверхностей? \en On which sides to touch surfaces?
  void            GetFaceSide( std::vector<bool> & fSide ) const;
  const std::vector<bool> & GetFaceSide() const { return faceSide; }
  /// \ru Выдать номера опорных граней. \en Get reference face numbers.
  void            GetFaceIndex( std::vector<MbItemIndex> & fas ) const;
  const std::vector<MbItemIndex> & GetFaceIndex() const { return faceIndex; }
  std::vector<MbItemIndex> & SetFaceIndex() { return faceIndex; }
  /// \ru Выдать количество опорных граней. \en Get guide faces count.
  size_t          GetFacesCount() const { return faces.size(); }
  size_t          GetFaceSideCount() const { return faceSide.size(); }
  /// \ru Выдать опорную грань. \en Get guide face.
  const MbFace *  GetFace( size_t i ) const { return ( i < faces.size() ) ? faces[i].get() : nullptr; }
  MbFace *        SetFace( size_t i )       { return ( i < faces.size() ) ? faces[i].get() : nullptr; }
  /// \ru Установить сторону касания грани. \en Set face side.
  void            SetFaceSide( size_t i, bool s ) { if ( i < faceSide.size() ) faceSide[i] = s; }
  /// \ru Очистить контейр тел. \en Solid conteiner clear.
  void            FacesClear() { faces.clear(); }

  /// \ru Добавить в данные направляющую кривую. \en Add guiding to data. \~
  void            AddEdge( MbCurveEdge & _edge, bool side, MbSolid * solid = nullptr );
  /// \ru Выдать направляющие рёбра. \en Get guide edges.
  void            GetEdges( std::vector<MbCurveEdge *> & eds ) const;
  void            GetEdges( RPArray<MbCurveEdge> & eds ) const;
  /// \ru Какую сторону кривой гладко стыковать с поверхностью? \en Which side should the surface join smoothly to?
  void            GetEdgeSide( std::vector<bool> & eSide ) const;
  /// \ru Выдать номера направляющих рёбер. \en Get guide edge numbers.
  void            GetEdgeIndex( std::vector<MbItemIndex> & ads ) const;
  std::vector<MbItemIndex> & SetEdgeIndex() { return edgeIndex; } 
  /// \ru Выдать количество направляющих ребер. \en Get guide edges count.
  size_t          GetEdgesCount() const { return edges.size(); }
  size_t          GetEdgeSideCount() const { return edgeSide.size(); }
  /// \ru Выдать направляющее ребро. \en Get guide edge.
  const MbCurveEdge * GetEdge( size_t i ) const { return ( i < edges.size() ) ? edges[i].get() : nullptr; }
  MbCurveEdge *   SetEdge( size_t i ) { return ( i < edges.size() ) ? edges[i].get() : nullptr; }
  /// \ru Установить сторону ребра. \en Set edge side.
  void            SetEdgeSide( size_t i, bool s ) { if ( i < edgeSide.size() ) edgeSide[i] = s; }
  /// \ru Очистить контейр тел. \en Solid conteiner clear.
  void            EdgesClear() { edges.clear(); }

  /// \ru Добавить в данные кривую. \en Add curve to data. \~
  void            AddCurve( MbCurve3D & _curve );
  /// \ru Добавить в данные кривые. \en Add curves to data. \~
  void            AddCurves( std::vector<MbCurve3D *> & _curves );
  /// \ru Выдать дополнительные направляющие кривые. \en Get additional guide curves.
  void            GetCurves( std::vector<MbCurve3D *> & crs ) const;
  /// \ru Выдать количество дополнительных направляющих кривых. \en Get additional guide curves count.
  size_t          GetCurvesCount() const { return curves.size(); }
  /// \ru Выдать дополнительную направляющую кривую. \en Get additional guide curve.
  const MbCurve3D * GetCurve( size_t i ) const { return ( i < curves.size() ) ? curves[i].get() : nullptr; }
  MbCurve3D *     SetCurve( size_t i ) { return ( i < curves.size() ) ? curves[i].get() : nullptr; }
  /// \ru Очистить контейр тел. \en Solid conteiner clear.
  void            CurvesClear() { curves.clear(); }

  /// \ru Добавить в данные кривую. \en Add curve to data. \~
  void            SetTrack( MbCurve3D & trk );
  /// \ru Выдать кривую, через которую должно пройти сечение. \en Get the curve that the section should pass through.
  const MbCurve3D * GetTrack() const { return track.get(); }
        MbCurve3D * SetTrack() { return track.get(); }

  /// \ru Установить функцию управления сечением. \en Set section control function.
  void            SetAngle( MbFunction & an, ThreeStates ts );
  /// \ru Выдать функцию управления сечением (радиус или дискриминант). \en Get section control function (radius or discriminant).
  const MbFunction * GetAngle() const { return function.get(); }
        MbFunction * SetAngle()       { return function.get(); }
  /// \ru Получить функцию радиуса скругления для набора граней. \en Get the function of fillet radius for the face set.
  const c3d::FunctionSPtr & GetFunction() const { return function; }
  /// \ru К чему задан угол функции: к хорде, к поверхности, к нормали поверхности. \en What is the angle of the function set to: to the chord, to the surface, to the surface normal.
  ThreeStates     GetState() const { return state; }
  void            SetState( ThreeStates st )  { state = st; }

  /// \ru Преобразовать объект. \en Transform the object. \~
  void            Transform( const MbMatrix3D & matr, MbRegTransform * iReg = nullptr );
  /// \ru Сдвинуть объект. \en Move the object. \~
  void            Move     ( const MbVector3D & to, MbRegTransform * iReg = nullptr );
  /// \ru Повернуть объект. \en Rotate the object. \~
  void            Rotate   ( const MbAxis3D & axis, double angle, MbRegTransform * iReg = nullptr );
  /// \ru Определить, являются ли объекты равными? \en Determine whether an object is equal?
  bool            IsSame( const MbSectionRail & other, double accuracy ) const;
  /// \ru Определить, являются ли объекты подобными. \en Determine whether the objects are similar. \~
  bool            IsSimilar( const MbSectionRail & other ) const;
  /// \ru Сделать объекты равным. \en Make objects equal. \~
  bool            SetEqual ( const MbSectionRail & other );
  
  // \ru Дать любую точку и ориентировочную длину. \en Give any point and approximate length. \~
  double          GetAnyPopint( MbCartPoint3D & p0 );
  /// \ru Найти номера рёбер и граней в оболочке. \en Find the numbers of edges and faces in the shell. \~
  bool            FindIndex( const MbFaceShell & shell );

  /// \ru Оператор присваивания без копирования данных. \en Assignment operator without copying.
  void            operator = ( const MbSectionRail & other );
  /// \ru Инициализация данных по другим данным без копирования указателей. \en Data copy without pointer copy.
  void            Init( MbFilletBundle & other );

  KNOWN_OBJECTS_RW_REF_OPERATORS( MbSectionRail ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class.

}; // MbSectionRail


//------------------------------------------------------------------------------
/** \brief \ru Функция управления сечением поверхности.
           \en The surface section control function. \~
  \details \ru Форму сечения поверхности заметания определяет функция управления сечением (радиус или дискриминант). 
      Если функция управления сечением не определена, то она рассчитывается по кривой, через которую должно пройти сечение, или поверхности, которой должно касаться сечение. \n
           \en The surface section form is determined by the section control function (radius or discriminant). 
      If the section control function is not determined, it is calculated with curve that the section should pass through or surface that the section should touch. \n
  \ingroup Surface_Building_Parameters
*/
// ---
struct MATH_CLASS MbSectionRule {

private:
  c3d::FunctionSPtr        discr;     ///< \ru Функция управления сечением (дискриминант или радиус, может быть nullptr). \en Section control function (discriminant or radius). \~
  c3d::SpaceCurveSPtr      track;     ///< \ru Кривая, через которую должно пройти сечение. \en The curve that the section should pass through. \~ 
  c3d::SurfaceSPtr         touch;     ///< \ru Поверхность, которой должно касаться сечение. \en The surface that the section should touch. \~ 
  c3d::ShellSPtr           shell;     ///< \ru Оболочка, которой должно касаться сечение. \en The shell that the section should touch. \~ 
  bool                     faceSide;  ///< \ru Строить со стороны нормали поверхности или граней при true. \en Build from the side of the surface or faces normal if true. \~
  std::vector<MbItemIndex> faceIndex; ///< \ru Номера используемых граней оболочки (все при отсутствии). \en The numbers of the shell faces used (all if empty). \~
  MbeSmoothForm            sform;     ///< \ru Форма поверхности сопряжения (скругления или фаски). \en The blend surface cross-section form (fillet or chamfer). \~
  double                   ratio;     ///< \ru Или натяжение, или скос, или соотношение. \en Or tension, or bevel, or ratio. \~ 

public:
  /// \ru Конструктор по умолчанию. \en Empty constructor.
  MbSectionRule();
  /// \ru Конструктор по функции. \en The constructor by function.
  MbSectionRule( MbFunction * fun );
  /// \ru Конструктор по кривой. \en The constructor by function.
  MbSectionRule( MbCurve3D * cur );
  /// \ru Конструктор по поверхности. \en The constructor by surface.
  MbSectionRule( MbSurface * sur, bool side = true );
  /// \ru Конструктор по оболочке. \en The constructor by shell. \~
  MbSectionRule( MbFaceShell * she, bool side = true );
  /// \ru Конструктор копирования. \en Copy-constructor. \~
  MbSectionRule( const MbSectionRule & other );
  /// \ru Конструктор копирования. \en Copy-constructor. \~
  MbSectionRule( const MbSectionRule & other, MbRegDuplicate * ireg );
  /// \ru Конструктор копирования без копирования указателей. \en Copy-constructor without pointer copy.
  MbSectionRule( MbFunction * func, MbeSmoothForm sf );
  /// \ru Деструктор. \en Destructor.
  ~MbSectionRule();

public:

  /// \ru Выдать функцию управления сечением. \en Get section control function. \~
  const MbFunction * GetFunction() const { return discr.get(); }
  MbFunction *    SetFunction() { return discr.get(); }
  /// \ru Установить функцию управления сечением. \en Set section control function. \~
  void            SetFunction( MbFunction & f );
  void            SetFunction( double f );
  /// \ru Выдать кривую управления сечением. \en Get section control curve. \~ 
  const MbCurve3D * GetCurve() const { return track.get(); }
  /// \ru Установить кривую управления сечением. \en Set section control curve. \~ 
  void            SetCurve( MbCurve3D & c );
  /// \ru Выдать поверхность управления сечением. \en Get section control surface. \~ 
  const MbSurface * GetSurface() const { return touch.get(); }
  /// \ru Установить поверхность управления сечением. \en Set section control surface. \~ 
  void            SetSurface( MbSurface & s );
  /// \ru Выдать оболочку управления сечением. \en Get section control shell. \~ 
  const MbFaceShell * GetShell() const { return shell.get(); }
  /// \ru Установить оболочку управления сечением. \en Set section control shell. \~ 
  void            SetShell( MbFaceShell & s );
  /// \ru С какой стороны касаться поверхностей? \en On which side to touch surfaces?
  bool            GetFaceSide() const { return faceSide; }
  void            SetFaceSide( bool fs ) { faceSide = fs; }
  /// \ru Выдать номера используемых граней. \en Get used face numbers.
  void            GetFaceIndex( std::vector<MbItemIndex> & fas ) const;
  const std::vector<MbItemIndex> & GetFaceIndex() const { return faceIndex; }
  std::vector<MbItemIndex> & SetFaceIndex() { return faceIndex; }
  /// \ru Выдать количество используемых граней. \en Get used faces count.
  size_t          GetFaceIndexCount() const { return faceIndex.size(); }
  /// \ru Выдать номер грани. \en Get face number.
  MbItemIndex     GetFaceIndex( size_t i ) const {
                    MbItemIndex ind; 
                    if ( i < faceIndex.size() )
                      ind = faceIndex[i]; 
                    return ind; 
  }
  /// \ru Добавить номер грани. \en Set face number.
  void            AddFaceIndex( size_t faceInd );
  void            AddFaceIndex( const MbFace * face );

  /// \ru Выдать форму поверхности сопряжения (скругления или фаски). \en Get a blend surface cross-section form (fillet or chamfer). \~
  MbeSmoothForm   GetSubForm() const { return sform; }
  /// \ru Установить форму поверхности сопряжения (скругления или фаски). \en Set a blend surface cross-section form (fillet or chamfer). \~
  void            SetSubForm( MbeSmoothForm f ) { sform = f; }
  /// \ru Выдать соотношение. \en Get the ratio. \~ 
  double          GetRatio() const { return ratio; }
  /// \ru Установить соотношение. \en Set the ratio. \~ 
  void            SetRatio( double r ) { ratio = r; }
        
  /// \ru Преобразовать объект. \en Transform the object. \~
  void            Transform( const MbMatrix3D & matr, MbRegTransform * iReg = nullptr );
  /// \ru Сдвинуть объект. \en Move the object. \~
  void            Move     ( const MbVector3D & to, MbRegTransform * iReg = nullptr );
  /// \ru Повернуть объект. \en Rotate the object. \~
  void            Rotate   ( const MbAxis3D & axis, double angle, MbRegTransform * iReg = nullptr );
  /// \ru Определить, являются ли объекты равными? \en Determine whether an object is equal?
  bool            IsSame( const MbSectionRule & other, double accuracy ) const;
  /// \ru Определить, являются ли объекты подобными. \en Determine whether the objects are similar. \~
  bool            IsSimilar( const MbSectionRule & other ) const;
  /// \ru Сделать объекты равным. \en Make objects equal. \~
  bool            SetEqual ( const MbSectionRule & other );

  /// \ru Оператор присваивания без копирования данных. \en Assignment operator without copying. \~
  void            operator = ( const MbSectionRule & other );
  /// \ru Инициализация данных по другим данным без копирования указателей. \en Data copy without pointer copy.
  void            Init( MbFunction * func, MbeSmoothForm sf );

  KNOWN_OBJECTS_RW_REF_OPERATORS( MbSectionRule ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class.

}; // MbSectionRule


//------------------------------------------------------------------------------
/** \brief \ru Параметры операции построения поверхности заметания переменного сечения.
           \en The parameters for building the swept mutable section surface. \~
  \details \ru Поверхность заметания строится путем движения плоского сечения вдоль опорной кривой. 
      Плоское сечение может начинаться на направляющей кривой и заканчиваться на другой направляющей кривой.
      Направляющих кривых может быть две, одна или ни одной. Кроме того, в построении могут использоваться управляющие кривые. \n
      Сечение поверхности плоскостью, перпендикулярной опорной кривой, может иметь одну из пяти форм и некоторые из них могут меняться по заданному закону.
      Сечение может иметь форму окружности (или её дуги), отрезка прямой, кривой второго порядка, кривой третьего порядка или заданной сплайновой кривой. \n
      В качестве примера рассмотрим сечение в форме участка кривой второго порядка. 
      В этом случае должны быть заданы две направляющие в виде рёбер MbSectionRail::edges или в виде кривых MbSectionRail::curves.
      Кривая второго порядка начинается в точке пересечения первой направляющей с плоскостью сечения и
      оканчивается в точке пересечения второй направляющей с плоскостью сечения.
      Направление кривой второго порядка на краю может быть задано одним из способов:
      - поверхностью грани ребра (рёбер) MbSectionRail::edges, где MbSectionRail::edgeSide указывает на грань слева или справа;
      - поверхностью кривой (кривых) MbSectionRail::curves, если кривая имеют поверхность в своих данных;
      - функцией угла наклона касательной MbSectionRail::angle, параметр MbSectionRail::state указывает, от чего отсчитывается угол;
      - дополнительной контрольной кривой MbSectionRail::track, через точку пересечения которой с плоскостью сечения должна проходить кривая второго порядка;
      - вершинной кривой MbSectionData::apexCurve, точка пересечения которой с плоскостью сечения определяет точку пересечения касательных кривой второго порядка на краях;
      Дискриминант кривой второго порядка может быть задан одним из способов:
      - функцией изменения дискриминанта MbSectionRule::discr;
      - дополнительной контрольной кривой MbSectionRule::track, через точку пересечения которой с плоскостью сечения должна проходить кривая второго порядка;
      - дополнительной поверхностью MbSectionRule::touch, которой должна касаться кривая второго порядка сечения;
      При наличие направляющих кривых и их несущих поверхностей построенная поверхность заметания гладко стыкуется с несущими поверхностями. \n
           \en The swept mutable section surface is form-generating by moving the flat section along the reference curve. \n
      The flat section can start on a guide curve and end on another guide curve.
      There can be two guide curves, one or none. In addition, control curves can be used in the construction. \n 
      The cross section of a surface with a plane perpendicular to the reference curve can have one of five shapes, and some of them can change according to a given law.
      The cross section can take the form of a circle (or its arc), a straight line segment, a second-order curve, a third-order curve, or a given spline curve. \n 
      As an example, consider a section in the form of a second-order curve.
      In this case, you must specify two edges as MbSectionRail::edges or as MbSectionRail::curves.
      The second-order curve starts at the intersection of the first guide with the cross-section plane and
      ends at the intersection of the second guide with the cross-section plane.
      The direction of the second-order curve on the beginning or on the end can be set in one of the following ways:
      - the face surface of the edge (s) MbSectionRail::edges, where MbSectionRail::edgeSide specifies the face on the left or on the right;
      - surface of the curve (s) MbSectionRail::curves, if the curve has a surface in its data;
      - function of the tangent angle MbSectionRail:: angle, the parameter MbSectionRail::state specifies from what the angle is calculated;
      - an additional control curve MbSectionRail::track, the second-order curve should pass through the point of intersection of the control curve with the cross-section plane;
      - vertex curve MbSectionData::apexCurve, whose intersection point with the section plane determines the intersection point of the tangents of second-order curve at the begining and at the end;
      The second-order curve discriminant can be set in one of the following ways:
      - the function for changing the discriminant MbSectionRule:: discr;
      - an additional control curve MbSectionRule::track, the second-order curve should pass through the point of intersection of the control curve with the cross-section plane;
      - an additional surface MbSectionRule::touch that the second-order curve of the section should touch;
      If there are guide curves and their bearing surfaces, the constructed sweep surface is smoothly joined to the bearing surfaces. \n \~
  \ingroup Surface_Building_Parameters
*/
// ---
class MATH_CLASS MbSectionData : public MbPrecision {

private:
  c3d::SpaceCurveSPtr  spine;     ///< \ru Опорная кривая (может отсутствовать для скругления). \en The reference curve (may be nullptr for fillet). \~ 
  MbeSectionShape      form;      ///< \ru Форма сечения поверхности. \en The surface cross-section shape. \~
  MbSectionRail        rail1;     ///< \ru Данные начального края сечения. \en The data of the beginning of section. \~
  MbSectionRail        rail2;     ///< \ru Данные конечного края сечения. \en The data of the end of section. \~
  c3d::SpaceCurveSPtr  apexCurve; ///< \ru Кривая вершин (может отсутствовать). \en The apex curve (may be nullptr). \~
  MbeCopyMode          copyMode;  ///< \ru Способы передачи данных при копировании оболочек. \en Methods of transferring data while copying shells. \~
  MbSectionRule        descript;  ///< \ru Функция управления сечением поверхности (радиус или дискриминант, может быть nullptr). \en The section control function (radius or discriminant). \~
  SPtr<MbPolyCurve>    pattern;   ///< \ru Образующая кривая при form==cs_Shape (для других форм nullptr). \en Forming curve for form==cs_Shape (nullptr on other case). \~
  MbVector3D           direction; ///< \ru Направление, от которого отсчитывается угол при form==cs_Linea. \en The direction from which the angle is calculated when form==cs_Linea. \~
  MbeSideShape         sideShape; ///< \ru Форма обрезки боков поверхности. \en The form of cropping the sides of the surface. \~
  MbeFaceHandling      handling;  ///< \ru Обработка исходных опорных граней. \en The processing of initial reference faces. \~
  bool                 faceSplit; ///< \ru Разделять оболочку на грани по сегментам направляющих кривых. \en Divide the shell into faces by segments of guides. \~
  bool                 elongated; ///< \ru Обрезать опорную оболочку на границе по касательной (true)/по нормали (false) к граничному ребру. \en The support shell will cutted along the tangent (true)/along the normal (false) to the boundary edge. \~
  bool                 prolong;   ///< \ru Продолжить по касательной. \en Prolong along the tangent. \~
  ThreeStates          keepCant;  ///< \ru Автоопределение сохранения кромки (ts_neutral), сохранение поверхности (ts_negative), сохранение кромки (ts_positive). \en Auto detection of boundary saving (ts_neutral), surface saving (ts_negative), boundary saving (ts_positive). \~
  double               uMin;      ///< \ru Минимальное значение первого параметра. \en Minimal value of the first parameter. \~ 
  double               uMax;      ///< \ru Максимальное значение первого параметра. \en Maximal value of the first parameter. \~ 
  double               buildSag;  ///< \ru Угловое отклонение при движении по кривым и поверхностям. \en Angular deviation while moving along curves and surfaces. \~
  uint32               count;     ///< \ru Минимальное количество шагов по опорной кривой. \en Minimum number of steps along the reference curve. \~
  bool                 check;     ///< \ru Проверять самопересечение построенной поверхности. \en Check the self-intersection of the constructed surface (default false). \~
  MbSNameMaker         nameMaker; ///< \ru Именователь новых граней операции. \en An object defining names generation in the operation. \~

public:
  /// \ru Конструктор по умолчанию. \en Empty constructor.
  MbSectionData();

  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \param[in] sp    - \ru Опорная кривая.
                       \en The reference curve. \~
    \param[in] f     - \ru Форма сечения поверхности.
                       \en The surface cross-section shape. \~
    \param[in] r1    - \ru Данные начального края сечения. 
                       \en The data of the beginning of section. \~
    \param[in] r2    - \ru Данные конечного края сечения.
                       \en The data of the end of section. \~
    \param[in] ap    - \ru Кривая вершин (может быть nullptr).
                       \en The apex curve (may be empty). \~
    \param[in] desc  - \ru Функция управления сечением (может быть nullptr).
                       \en Section control function (may be nullptr). \~
    \param[in] patt  - \ru Образующая кривая (может быть nullptr).
                       \en Forming curve (may be nullptr). \~
    \param[in] names - \ru Именователь новых граней операции.
                       \en An object defining names generation in the operation. \~
  */
  MbSectionData( MbCurve3D & sp,
                 MbeSectionShape f, 
                 MbSectionRail & r1,
                 MbSectionRail & r2,
                 MbCurve3D * ap,
                 MbSectionRule & desc,
                 MbPolyCurve * patt,
                 const MbSNameMaker & names ); 
  /// \ru Конструктор по параметрам для скругления. \en Constructor by parameters for fillet.
  MbSectionData( MbSectionRail & r1, MbSectionRail & r2,
                 bool split, bool elong, MbeSideShape shape,
                 bool prol, ThreeStates kCant, const MbSNameMaker & n );
  /// \ru Конструктор копирования. \en Copy-constructor.
  MbSectionData( const MbSectionData & other );
  /// \ru Конструктор копирования. \en Copy-constructor.
  MbSectionData( const MbSectionData & other, MbRegDuplicate * ireg );
  /// \ru Конструктор копирования без копирования указателей. \en Copy-constructor without pointer copy.
  MbSectionData( MbFilletData & other );
  /// \ru Деструктор. \en Destructor.
  ~MbSectionData();

public:

  /// \ru Установить опорную кривую. \en Set reference curve. 
  void            SetSpine( MbCurve3D & s );
  /// \ru Установить вектор направления опорной кривой (если spine==nullptr). \en Set the direction vector of the reference curve (if spine= = nullptr).
  void            SetSpine( const MbVector3D & a );
  /// \ru Выдать опорную кривую. \en Get reference curve. 
  const MbCurve3D * GetSpine() const { return spine.get(); }
  /// \ru Выдать опорную кривую. \en Get reference curve. 
        MbCurve3D * SetSpine()       { return spine.get(); }

  /// \ru Выдать форму сечения поверхности. \en Get cross-section shape.
  MbeSectionShape GetSectionForm() const { return form;  }
  /// \ru Установить форму сечения поверхности. \en Set cross-section shape.
  void            SetSectionForm( MbeSectionShape f ) { form = f; }

  ///< \ru Данные начального края сечения. \en The data of the beginning of section.
  const MbSectionRail & GetRail1() const { return rail1; }
  MbSectionRail & SetRail1() { return rail1; }
  ///< \ru Данные конечного края сечения. \en The data of the end of section.
  const MbSectionRail & GetRail2() const { return rail2; }
  MbSectionRail & SetRail2() { return rail2; }

  /// \ru Добавить направляющую грань. \en Add guide face.
  void            AddFace1( MbFace & _face, bool side, MbSolid * solid = nullptr ) { rail1.AddFace( _face, side, solid ); }
  void            AddFace2( MbFace & _face, bool side, MbSolid * solid = nullptr ) { rail2.AddFace( _face, side, solid ); }
  /// \ru Выдать поверхности. \en Get surfaces.
  void            GetFaces1( std::vector<MbFace *> & fas ) const { rail1.GetFaces( fas ); }
  void            GetFaces1( RPArray<MbFace> & fas ) const { rail1.GetFaces( fas ); }
  /// \ru Выдать поверхности. \en Get surfaces.
  void            GetFaces2( std::vector<MbFace *> & fas ) const { rail2.GetFaces( fas ); }
  void            GetFaces2( RPArray<MbFace> & fas ) const { rail2.GetFaces( fas ); }
  /// \ru С каких сторон касаться поверхностей? \en On which sides to touch surfaces?
  void            GetFaceSide1( std::vector<bool> & fSide ) const { rail1.GetFaceSide( fSide ); }
  /// \ru С каких сторон касаться поверхностей? \en On which sides to touch surfaces?
  void            GetFaceSide2( std::vector<bool> & fSide ) const { rail2.GetFaceSide( fSide ); }
  /// \ru Выдать номера опорных граней. \en Get reference face numbers.
  void            GetFaceIndex1( std::vector<MbItemIndex> & fas ) const { rail1.GetFaceIndex( fas ); }
  void            GetFaceIndex2( std::vector<MbItemIndex> & fas ) const { rail2.GetFaceIndex( fas ); }
  std::vector<MbItemIndex> & SetFaceIndex( size_t i ) { return ( (i == 1) ? rail1.SetFaceIndex() : rail2.SetFaceIndex() ); }
  /// \ru Выдать количество направляющих граней. \en Get guide faces count.
  size_t          GetFacesCount1() const { return rail1.GetFacesCount(); }
  size_t          GetFacesCount2() const { return rail2.GetFacesCount(); }
  size_t          GetFaceSideCount1() const { return rail1.GetFaceSideCount(); }
  size_t          GetFaceSideCount2() const { return rail2.GetFaceSideCount(); }
  /// \ru Выдать направляющую грань. \en Get guide face.
  const MbFace *  GetFace1( size_t i ) const { return rail1.GetFace( i ); }
  MbFace *        SetFace1( size_t i )       { return rail1.SetFace( i ); }
  /// \ru Выдать направляющую грань. \en Get guide face.
  const MbFace *  GetFace2( size_t i ) const { return rail2.GetFace( i ); }
  MbFace *        SetFace2( size_t i )       { return rail2.SetFace( i ); }

  /// \ru Добавить в данные направляющее ребро. \en Add guiding to data. \~
  void            AddEdge1( MbCurveEdge & _edge, bool side, MbSolid * solid = nullptr ) { rail1.AddEdge( _edge, side, solid ); }
  void            AddEdge2( MbCurveEdge & _edge, bool side, MbSolid * solid = nullptr ) { rail2.AddEdge( _edge, side, solid ); }
  /// \ru Выдать направляющие рёбра. \en Get guide edges.
  void            GetEdges1( std::vector<MbCurveEdge *> & eds ) const { rail1.GetEdges( eds ); }
  void            GetEdges1( RPArray<MbCurveEdge> & eds ) const { rail1.GetEdges( eds ); }
  /// \ru Выдать направляющие рёбра. \en Get guide edges.
  void            GetEdges2( std::vector<MbCurveEdge *> & eds ) const { rail2.GetEdges( eds ); }
  void            GetEdges2( RPArray<MbCurveEdge> & eds ) const { rail2.GetEdges( eds ); }
  /// \ru Какую сторону кривой гладко стыковать с поверхностью? \en Which side should the surface join smoothly to?
  void            GetEdgeSide1( std::vector<bool> & eSide ) const { rail1.GetEdgeSide( eSide ); }
  /// \ru Какую сторону кривой гладко стыковать с поверхностью? \en Which side should the surface join smoothly to?
  void            GetEdgeSide2( std::vector<bool> & eSide ) const { rail2.GetEdgeSide( eSide ); }
  /// \ru Выдать номера направляющих рёбер. \en Get guide edge numbers.
  void            GetEdgeIndex1( std::vector<MbItemIndex> & eds ) const { rail1.GetEdgeIndex( eds ); }
  void            GetEdgeIndex2( std::vector<MbItemIndex> & eds ) const { rail2.GetEdgeIndex( eds ); }
  std::vector<MbItemIndex> & SetEdgeIndex( size_t i ) { return ( (i == 1) ? rail1.SetEdgeIndex() : rail2.SetEdgeIndex() ); } 
  /// \ru Выдать количество направляющих ребер. \en Get guide edges count.
  size_t          GetEdgesCount1() const { return rail1.GetEdgesCount(); }
  size_t          GetEdgesCount2() const { return rail2.GetEdgesCount(); }
  size_t          GetEdgeSideCount1() const { return rail1.GetEdgeSideCount(); }
  size_t          GetEdgeSideCount2() const { return rail2.GetEdgeSideCount(); }
  /// \ru Выдать направляющее ребро. \en Get guide edge.
  const MbCurveEdge * GetEdge1( size_t i ) const { return rail1.GetEdge( i ); }
  MbCurveEdge *   SetEdge1( size_t i )           { return rail1.SetEdge( i ); }
  /// \ru Выдать направляющее ребро. \en Get guide edge.
  const MbCurveEdge * GetEdge2( size_t i ) const { return rail2.GetEdge( i ); }
  MbCurveEdge *   SetEdge2( size_t i )           { return rail2.SetEdge( i ); }

  /// \ru Выдать тела опорных граней или направляющих рёбер. \en Get solids of reference faces or guide edges.
  void            GetSolids1( std::vector<MbSolid *> & sols ) const { rail1.GetSolids( sols ); }
  void            GetSolids1( RPArray<MbSolid> & sols ) const { rail1.GetSolids( sols ); }
  void            GetSolids2( std::vector<MbSolid *> & sols ) const { rail2.GetSolids( sols ); }
  void            GetSolids2( RPArray<MbSolid> & sols ) const { rail2.GetSolids( sols ); }
  /// \ru Выдать количество тел опорных граней или направляющих рёбер. \en Get solids of reference faces or guide edges count.
  size_t          GetSolidsCount1() const { return rail1.GetSolidsCount(); }
  size_t          GetSolidsCount2() const { return rail2.GetSolidsCount(); }
  /// \ru Выдать тело по индексу. \en Get solid by index.
  const MbSolid * GetSolid1( size_t i ) const { return rail1.GetSolid( i ); }
  MbSolid *       SetSolid1( size_t i )       { return rail1.SetSolid( i ); }
  const MbSolid * GetSolid2( size_t i ) const { return rail2.GetSolid( i ); }
  MbSolid *       SetSolid2( size_t i )       { return rail2.SetSolid( i ); }
  /// \ru Выдать уникальные тела. \en Give out unique solids.
  void            GetUniqueSolids1( std::vector<MbSolid *> & sols ) const { rail1.GetUniqueSolids( sols ); }
  void            GetUniqueSolids2( std::vector<MbSolid *> & sols ) const { rail2.GetUniqueSolids( sols ); }
  void            GetUniqueSolids ( std::vector<MbSolid *> & sols ) const { rail1.GetUniqueSolids( sols ); 
                                                                            rail2.GetUniqueSolids( sols ); }
  /// \ru Добавить в данные направляющую кривую. \en Add guiding to data. \~
  void            AddCurve1( MbCurve3D & crv ) { rail1.AddCurve( crv ); }
  void            AddCurve2( MbCurve3D & crv ) { rail2.AddCurve( crv ); }
  /// \ru Выдать дополнительные направляющие кривые. \en Get additional guide curves.
  void            GetCurves1( std::vector<MbCurve3D *> & crs ) const { rail1.GetCurves( crs ); }
  /// \ru Выдать дополнительные направляющие кривые. \en Get additional guide curves.
  void            GetCurves2( std::vector<MbCurve3D *> & crs ) const { rail2.GetCurves( crs ); }
  /// \ru Выдать количество дополнительных направляющих кривых. \en Get additional guide curves count.
  size_t          GetCurvesCount1() const { return rail1.GetCurvesCount(); }
  size_t          GetCurvesCount2() const { return rail2.GetCurvesCount(); }
  /// \ru Выдать направляющую кривую. \en Get guide curve.
  const MbCurve3D * GetCurve1( size_t i ) const { return rail1.GetCurve( i ); }
  MbCurve3D *     SetCurve1( size_t i )         { return rail1.SetCurve( i ); }
  /// \ru Выдать направляющую кривую. \en Get guide curve.
  const MbCurve3D * GetCurve2( size_t i ) const { return rail2.GetCurve( i ); }
  MbCurve3D *     SetCurve2( size_t i )         { return rail2.SetCurve( i ); }

  /// \ru Добавить в данные кривую, через которую должно пройти сечение. \en Add curve that the section should pass through. \~
  void            SetTrack1( MbCurve3D & tr ) { rail1.SetTrack( tr ); }
  /// \ru Добавить в данные кривую, через которую должно пройти сечение. \en Add curve that the section should pass through. \~
  void            SetTrack2( MbCurve3D & tr ) { rail2.SetTrack( tr ); }
  /// \ru Выдать кривую, через которую должно пройти сечение. \en Get the curve that the section should pass through. \~
  const MbCurve3D * GetTrack1() const { return rail1.GetTrack(); }
  /// \ru Выдать кривую, через которую должно пройти сечение. \en Get the curve that the section should pass through. \~
  const MbCurve3D * GetTrack2() const { return rail2.GetTrack(); }
  /// \ru Выдать кривую, через которую должно пройти сечение. \en Get the curve that the section should pass through. \~
  MbCurve3D *     SetTrack1() { return rail1.SetTrack(); }
  /// \ru Выдать кривую, через которую должно пройти сечение. \en Get the curve that the section should pass through. \~
  MbCurve3D *     SetTrack2() { return rail2.SetTrack(); }

  /// \ru Добавить в данные функцию. \en Add functions to data. \~
  void            SetAngle1( MbFunction & ang, ThreeStates ts ) { rail1.SetAngle( ang, ts ); }
  /// \ru Добавить в данные функцию. \en Add functions to data. \~
  void            SetAngle2( MbFunction & ang, ThreeStates ts ) { rail2.SetAngle( ang, ts ); }
  /// \ru Выдать функции углов наклона. \en Get angle functions.
  const MbFunction * GetAngle1() const { return rail1.GetAngle(); }
  /// \ru Выдать функции углов наклона. \en Get angle functions.
  const MbFunction * GetAngle2() const { return rail2.GetAngle(); }
  /// \ru Выдать функцию угла наклона. \en Get angle function.
  MbFunction *    SetAngle1() { return rail1.SetAngle(); }
  /// \ru Выдать функцию угла наклона. \en Get angle function.
  MbFunction *    SetAngle2() { return rail2.SetAngle(); }
  /// \ru Получить функцию радиуса скругления для первого набора граней. \en Get the function of fillet radius for the first face set.
  const c3d::FunctionSPtr & GetFunction1() const { return rail1.GetFunction(); }
  /// \ru Получить функцию радиуса скругления для второго набора граней. \en Get the function of fillet radius for the second face set.
  const c3d::FunctionSPtr & GetFunction2() const { return rail2.GetFunction(); }

  /// \ru К чему задан угол функции: к хорде, к поверхности, к нормали поверхности. \en What is the angle of the function set to: to the chord, to the surface, to the surface normal.
  ThreeStates     GetState1() const { return rail1.GetState(); }
  ThreeStates     GetState2() const { return rail2.GetState(); }
  /// \ru Как отсчитывать угол функции: от хорде, от поверхности, от нормали поверхности. \en How to count the angle of the function: from the chord, from the surface, from the surface normal.
  void            SetState1( ThreeStates st ) { rail1.SetState( st ); }
  void            SetState2( ThreeStates st ) { rail2.SetState( st ); }

  /// \ru Добавить в данные кривую вершин. \en Set apex curve. \~
  void            SetApexCurve( MbCurve3D & curv );
  /// \ru Выдать кривую вершин. \en Get apex curve.
  MbCurve3D *     SetApexCurve() { return apexCurve.get(); }
  /// \ru Выдать кривую вершин. \en Get apex curve.
  const MbCurve3D * GetApexCurve() const { return apexCurve.get(); }
  /// \ru Получить способы передачи данных при копировании оболочек. \en Get methods of transferring data while copying shells. \~
  MbeCopyMode     GetCopyMode() const { return copyMode; }
  /// \ru Установить способы передачи данных при копировании оболочек. \en Set methods of transferring data while copying shells. \~
  void            SetCopyMode( MbeCopyMode m ) { copyMode = m; }

  /// \ru Выдать данные управления сечением. \en Get section control data.
  const MbSectionRule & GetSectionRule() const { return descript; }
        MbSectionRule & SetSectionRule()       { return descript; }
  /// \ru Выдать функцию управления сечением (радиус или дискриминант). \en Get section control function (radius or discriminant).
  const MbFunction * GetFunction() const { return descript.GetFunction(); }
        MbFunction * SetFunction()       { return descript.SetFunction(); }
  /// \ru Установить функцию управления сечением. \en Set section control function.
  void            SetFunction( MbFunction & f ) { descript.SetFunction( f ); }
  void            SetFunction( double f )       { descript.SetFunction( f ); }
  /// \ru Выдать кривую управления сечением. \en Get section control curve. 
  const MbCurve3D * GetDescriptCurve() const { return descript.GetCurve(); }
  /// \ru Выдать поверхность управления сечением. \en Get section control surface. 
  const MbSurface * GetDescriptSurface() const { return descript.GetSurface(); }
  /// \ru Выдать оболочку управления сечением. \en Get section control shell. 
  const MbFaceShell * GetDescriptShell() const { return descript.GetShell(); }
  /// \ru С какой стороны касаться поверхностей? \en On which side to touch surfaces?
  bool            GetFaceSide3() const { return descript.GetFaceSide(); }
  void            SetFaceSide3( bool fs ) { descript.SetFaceSide(fs); }
  /// \ru Выдать количество используемых граней. \en Get used faces count.
  size_t          GetFaceIndex3Count() const { return descript.GetFaceIndexCount(); }
  /// \ru Выдать номера используемых граней. \en Get used face numbers.
  MbItemIndex     GetFaceIndex3( size_t i ) const { return descript.GetFaceIndex(i); }
  /// \ru Выдать форму поверхности сопряжения (скругления или фаски). \en Get a blend surface cross-section form (fillet or chamfer). \~
  MbeSmoothForm   GetSubForm() const { return descript.GetSubForm(); }
  /// \ru Установить форму поверхности сопряжения (скругления или фаски). \en Set a blend surface cross-section form (fillet or chamfer). \~
  void            SetSubForm( MbeSmoothForm f ) { descript.SetSubForm( f ); }
  /// \ru Выдать соотношение. \en Get the ratio. \~ 
  double          GetRatio() const { return descript.GetRatio(); }
  /// \ru Установить соотношение. \en Set the ratio. \~ 
  void            SetRatio( double r ) { descript.SetRatio( r ); }

  /// \ru Выдать образующую кривую. \en Get forming curve.
  const MbPolyCurve * GetPattern() const { return pattern.get(); }
        MbPolyCurve * SetPattern()       { return pattern.get(); }
  /// \ru Установить образующую кривую. \en Set forming curve.
  void            SetPattern( MbPolyCurve & p );
  /// \ru Выдать направление, от которого отсчитывается угол. \en Get the direction from which the angle is calculated.
  const MbVector3D & GetDirection() const { return direction; }
  MbVector3D &    SetDirection() { return direction; }
  /// \ru Установить направление, от которого отсчитывается угол. \en Set the direction from which the angle is calculated.
  void            SetDirection( const MbVector3D & dir ) { direction = dir; }
  /// \ru Выдать форму обрезки боков поверхности. \en Get the shape of cropping the sides of the surface. \~
  MbeSideShape    GetSideShape() const { return sideShape; }
  /// \ru Установить форму обрезки боков поверхности. \en Set the shape of cropping the sides of the surface. \~
  void            SetSideShape( MbeSideShape s ) { sideShape = s; }
  /// \ru Выдать обработку исходных опорных граней. \en Get the processing of initial reference faces. \~
  MbeFaceHandling GetFaceHandling() const { return handling; }
  /// \ru Установить обработку исходных опорных граней. \en Set the processing of initial reference faces. \~
  void            SetFaceHandling( MbeFaceHandling h ) { handling = h; }
  /// \ru Разделять оболочку на грани по сегментам направляющих кривых? \en Is divide the shell into faces by segments of guides? \~
  bool            GetFaceSplit() const { return faceSplit; }
  /// \ru Установить деление оболочки на грани по сегментам направляющих кривых. \en Set division the shell into faces by segments of guides.
  void            SetFaceSplit( bool s ) { faceSplit = s; }
  /// \ru Обрезать опорную оболочку на границе по касательной или по нормали к граничному ребру? \en How to cut the support shell on the boundary edge? \~
  bool            GetElongated() const { return elongated; }
  /// \ru Установить обрезку опорной оболочки на границе к граничному ребру. \en Set the support shell cutting on the boundary edge. \~
  void            SetElongated( bool e ) { elongated = e; }
  /// \ru Продолжить по касательной. \en Prolong along the tangent. \~
  bool            GetProlong() const { return prolong; }
  /// \ru Установить продолжить по касательной. \en Set prolong along the tangent. \~
  void            SetProlong( bool p ) { prolong = p; }
  /// \ru Автоопределение сохранения кромки (ts_neutral), сохранение поверхности (ts_negative), сохранение кромки (ts_positive). \en Auto detection of boundary saving (ts_neutral), surface saving (ts_negative), boundary saving (ts_positive). \~
  ThreeStates     GetKeepCant() const { return keepCant; }
  /// \ru Установить автоопределение сохранения кромки (ts_neutral), сохранение поверхности (ts_negative), сохранение кромки (ts_positive). \en Set auto detection of boundary saving (ts_neutral), surface saving (ts_negative), boundary saving (ts_positive). \~
  void            SetKeepCant( ThreeStates kc ) { keepCant = kc; }

  /// \ru Минимальное значение первого параметра. \en Minimal value of the first parameter. 
  double          GetUMin() const { return uMin; }
  /// \ru Максимальное значение первого параметра. \en Maximal value of the first parameter. 
  double          GetUMax() const { return uMax; }
  /// \ru Установить область определения первого параметра поверхностей. \en Set the first parameter region of the surface. 
  void            SetUParams( double u1, double u2 );
  /// \ru Угловое отклонение при движении по кривым и поверхностям. \en Angular deviation while moving along curves and surfaces. 
  double          GetBuildSag() const { return buildSag; }
  /// \ru Минимальное количество шагов по опорной кривой. \en Minimum number of steps along the reference curve.
  uint32          GetCount() const { return count; }
  void            SetCount( uint32 c );
  /// \ru Проверять самопересечение построенной поверхности. \en Check the self-intersection of the constructed surface.
  bool            GetCheck() const { return check; }
  void            SetCheck( bool c );
  /// \ru Выдать именователь операции. \en Get an object defining names generation in the operation.
  const MbSNameMaker & GetNameMaker() const { return nameMaker; }
  /// \ru Выдать именователь операции. \en Get an object defining names generation in the operation.
  MbSNameMaker & SetNameMaker() { return nameMaker; }
  /// \ru Задать именователь операции. \en Set an object defining names generation in the operation.
  void           SetNameMaker( const MbSNameMaker & nm ) { nameMaker.SetNameMaker( nm, true ); }

  /// \ru Преобразовать объект. \en Transform the object. \~
  void            Transform( const MbMatrix3D & matr, MbRegTransform * iReg = nullptr );
  /// \ru Сдвинуть объект. \en Move the object. \~
  void            Move     ( const MbVector3D & to, MbRegTransform * iReg = nullptr );
  /// \ru Повернуть объект. \en Rotate the object. \~
  void            Rotate   ( const MbAxis3D & axis, double angle, MbRegTransform * iReg = nullptr );
  /// \ru Определить, являются ли объекты равными? \en Determine whether an object is equal?
  bool            IsSame( const MbSectionData & other, double acc ) const;
  /// \ru Определить, являются ли объекты подобными. \en Determine whether the objects are similar. \~
  bool            IsSimilar( const MbSectionData & other ) const;
  /// \ru Сделать объекты равным. \en Make objects equal. \~
  bool            SetEqual ( const MbSectionData & other );
  // \ru Найти номера рёбер и граней в оболочке. \en Find the numbers of edges and faces in the shell. \~
  bool            FindIndex( const MbFaceShell & shell );
  /// \ru Выдать базовые объекты. \en Get basis objects.
  void            GetBasisItems( RPArray<MbSpaceItem> & s ) const;
  // \ru Выдать версию построения. \en Get the build version. \~
  VERSION         GetVersion() const { return nameMaker.GetMathVersion(); }
  /// \ru Оператор присваивания без копирования данных. \en Assignment operator without copying.
  void            operator = ( const MbSectionData & other );
  /// \ru Инициализация данных по другим данным без копирования указателей. \en Data copy without pointer copy.
  void            Init( MbFilletData & other );

  KNOWN_OBJECTS_RW_REF_OPERATORS( MbSectionData ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class.

}; // MbSectionData


//------------------------------------------------------------------------------
/** \brief \ru Данные фантома для торца поверхности переменного сечения.
           \en The parameters of form-generating operation. \~
  \details \ru Данные фантома содержат точки и направления сторон охватывающего треугольника. \n
           \en The phantom data contains the points and directions of the flank of the enclosing triangle. \~
  \ingroup Surface_Building_Parameters
*/
// ---
struct MATH_CLASS MbSectionLayout {
public :
  MbPlacement3D place;   ///< \ru Плоскость охватывающего треугольника (XY) на краю поверхности. \en The plane of the enclosing triangle (XY) at the flank of the surface. \~ 
  MbCartPoint3D point1;  ///< \ru Точка пересечения первой направляющей кривой и плоскости XY. \en The intersection point of the first guide curve ans XY plane. \~
  MbCartPoint3D point2;  ///< \ru Точка пересечения второй направляющей кривой и плоскости XY. \en The intersection point of the second guide curve ans XY plane. \~
  MbVector3D    vector1; ///< \ru Вектор направления от первой направляющей к вершине охватывающего треугольника. \en The direction vector from the first guide point to the vertex of the enclosing triangle. \~
  MbVector3D    vector2; ///< \ru Вектор направления от второй направляющей к вершине охватывающего треугольника. \en The direction vector from the second guide point to the vertex of the enclosing triangle. \~
  MbCartPoint3D apex;    ///< \ru Точка пересечения вершинной кривой и плоскости XY - является вершиной охватывающего треугольника (может быть в бесконечности). 
                         ///< \en The intersection point of the apex curve ans XY plane is the vertex of the enclosing triangle (may be on infinity). \~
public :
  /// \ru Конструктор по умолчанию. \en Default constructor.
  MbSectionLayout()
    : place  ()
    , point1 ()
    , point2 ()
    , vector1()
    , vector2()
    , apex   ()
  {}
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  MbSectionLayout( const MbPlacement3D & pl, const MbCartPoint3D & pnt1, const MbCartPoint3D & pnt2, 
                   const MbVector3D & vec1, const MbVector3D & vec2, const MbCartPoint3D & apx )
    : place  ( pl   )
    , point1 ( pnt1 )
    , point2 ( pnt2 )
    , vector1( vec1 )
    , vector2( vec2 )
    , apex   ( apx  )
  {}
  /// \ru Конструктор копирования. \en Copy-constructor.
  MbSectionLayout( const MbSectionLayout & other )
    : place  ( other.place   )
    , point1 ( other.point1  )
    , point2 ( other.point2  )
    , vector1( other.vector1 )
    , vector2( other.vector2 )
    , apex   ( other.apex    )
  {}
  /// \ru Деструктор. \en Destructor.
  ~MbSectionLayout() {}

  /// \ru Выдать значения объектов. \en Get an object values. \~
  void    GetLayout( MbPlacement3D & pl, MbCartPoint3D & pnt1, MbCartPoint3D & pnt2, 
                     MbVector3D & vec1, MbVector3D & vec2, MbCartPoint3D & apx ) const
  {
    pl   = place;
    pnt1 = point1;
    pnt2 = point2;
    vec1 = vector1;
    vec2 = vector2;
    apx  = apex;
  }
  /// \ru Выдать значения объектов и полложение центра скеругления. \en Get an object values and the fillet center position. \~
  void    CentreLayout( MbPlacement3D & pl, MbCartPoint3D & pnt1, MbCartPoint3D & pnt2, 
                        MbVector3D & vec1, MbVector3D & vec2, MbCartPoint3D & apx, MbCartPoint3D & centre ) const;
  /// \ru Установить значения объектов. \en Set an object values. \~
  void    SetLayout( const MbPlacement3D & pl, const MbCartPoint3D & pnt1, const MbCartPoint3D & pnt2, 
                     const MbVector3D & vec1, const MbVector3D & vec2, const MbCartPoint3D & apx )
  {
    place   = pl;
    point1  = pnt1;
    point2  = pnt2;
    vector1 = vec1;
    vector2 = vec2;
    apex    = apx;
  }
  /// \ru Оператор присваивания. \en Assignment operator.
  void    operator = ( const MbSectionLayout & other ) { 
    place   = other.place;
    point1  = other.point1;
    point2  = other.point2;
    vector1 = other.vector1;
    vector2 = other.vector2;
    apex    = other.apex;
  }

}; // MbSectionLayout


//------------------------------------------------------------------------------
/** \brief \ru Данные фантома для торцев поверхности переменного сечения.
           \en The parameters of form-generating operation. \~
  \details \ru Данные фантома содержат точки и направления сторон охватывающих треугольников на торцах поверхности переменного сечения. \n
           \en The phantom data contains the points and directions of the enclosing triangles at the ends of the mutable section surface. \~
  \ingroup Surface_Building_Parameters
*/
// ---
class MATH_CLASS MbSectionPhantom {
public : 
  MbSectionLayout begLayout; ///< \ru Данные фантома для начального торца поверхности переменного сечения. \en Phantom data for the initial cross-section of the surface.
  MbSectionLayout endLayout; ///< \ru Данные фантома для конечного  торца поверхности переменного сечения. \en Phantom data for the end cross-section of the surface.
  MbSectionLayout midLayout; ///< \ru Данные фантома для середины поверхности переменного сечения. \en Phantom data for the middle cross-section of the surface.

public : 
  /// \ru Конструктор по умолчанию. \en Default constructor.
  MbSectionPhantom()
    : begLayout()
    , endLayout() 
    , midLayout()
  {}
  /// \ru Конструктор по данным. \en Constructor by data.
  //MbSectionPhantom( const MbPlacement3D & pl_1, const MbCartPoint3D & pnt1_1, const MbCartPoint3D & pnt2_1, 
  //                  const MbVector3D & vec1_1, const MbVector3D & vec2_1, const MbCartPoint3D & apx_1,
  //                  const MbPlacement3D & pl_2, const MbCartPoint3D & pnt1_2, const MbCartPoint3D & pnt2_2, 
  //                  const MbVector3D & vec1_2, const MbVector3D & vec2_2, const MbCartPoint3D & apx_2 )
  //  : begLayout( pl_1, pnt1_1, pnt2_1, vec1_1, vec2_1, apx_1 )
  //  , endLayout( pl_2, pnt1_2, pnt2_2, vec1_2, vec2_2, apx_2 )
  //  , midLayout()
  //{}
  /// \ru Конструктор копирования. \en Copy-constructor.
  MbSectionPhantom( const MbSectionPhantom & other )
    : begLayout( other.begLayout )
    , endLayout( other.endLayout )
    , midLayout( other.midLayout )
  {}
  /// \ru Деструктор. \en Destructor.
  ~MbSectionPhantom() {}

  /// \ru Выдать значения объектов для края по его индексу i. \en Get an object values for the side by its index i. \~
  void    GetLayout( size_t i, MbPlacement3D & pl, MbCartPoint3D & pnt1, MbCartPoint3D & pnt2, 
                               MbVector3D & vec1, MbVector3D & vec2, MbCartPoint3D & apx ) const {
    if ( i == 1 )
      begLayout.GetLayout( pl, pnt1, pnt2, vec1, vec2, apx );
    else if ( i == 2 ) 
      endLayout.GetLayout( pl, pnt1, pnt2, vec1, vec2, apx );
    else
      midLayout.GetLayout( pl, pnt1, pnt2, vec1, vec2, apx );
  }
  /// \ru Выдать значения объектов для края по его индексу i. \en Get an object values for the side by its index i. \~
  void    CentreLayout( size_t i, MbPlacement3D & pl, MbCartPoint3D & pnt1, MbCartPoint3D & pnt2, 
                                  MbVector3D & vec1, MbVector3D & vec2, MbCartPoint3D & apx, MbCartPoint3D & centre ) const {
    if ( i == 1 )
      begLayout.CentreLayout( pl, pnt1, pnt2, vec1, vec2, apx, centre );
    else if ( i == 2 ) 
      endLayout.CentreLayout( pl, pnt1, pnt2, vec1, vec2, apx, centre );
    else
      midLayout.CentreLayout( pl, pnt1, pnt2, vec1, vec2, apx, centre );
  }
  /// \ru Установить значения объектов для края по его индексу i. \en Set an object values for the side by its index i. \~
  void    SetLayout( size_t i, const MbPlacement3D & pl, const MbCartPoint3D & pnt1, const MbCartPoint3D & pnt2, 
                               const MbVector3D & vec1, const MbVector3D & vec2, const MbCartPoint3D & apx ) {
    if ( i == 1 )
      begLayout.SetLayout( pl, pnt1, pnt2, vec1, vec2, apx );
    else if ( i == 2 ) 
      endLayout.SetLayout( pl, pnt1, pnt2, vec1, vec2, apx );
    else
      midLayout.SetLayout( pl, pnt1, pnt2, vec1, vec2, apx );
  }

  /// \ru Оператор присваивания. \en Assignment operator.
  void    operator = ( const MbSectionPhantom & other ) { 
    begLayout = other.begLayout;
    endLayout = other.endLayout;
    midLayout = other.midLayout;
  }

}; // MbSectionPhantom


//------------------------------------------------------------------------------
/** \brief \ru Параметры операции поиска ближайшего тела GetNearestSolid.
           \en Parameters of the GetNearestSolid operation to find the nearest body. \~
    \details \ru Параметры операции поиска ближайшего тела GetNearestSolid.
             \en Parameters of the GetNearestSolid operation to find the nearest body. \n \~
    \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbNearestSolidParams : public MbPrecision
{
private:
  c3d::SolidsSPtrVector    _solids;               ///< \ru Контейнер тел для поиска ближайшего. \en Container of bodies to find the nearest one.
  MbSweptData              _sweptData;            ///< \ru Данные об образующей операции выдавливания. \en Information about the generatrix of the extrusion operation.

  MbVector3D               _direction;            ///< \ru Направление выдавливания. \en Extrusion direction.
  MbSweptLayout::Direction _operationDirection;   ///< \ru Направление поиска тел (прямое, обратное или оба). \en Body search direction (forward, reverse, or both).

  SweptValuesAndSides      _valuesAndSides;       ///< \ru Для учета угла уклона и тонкой стенки. В разработке. \en To account for slope angle and thin wall. In developing.

  VERSION                  _version;          ///< \ru Версия операции. \en Version of the operation. \~

public:

  /** \brief \ru Конструктор.
             \en Constructor. \~
      \details \ru Конструктор для образующей общего вида.
               \en Constructor for a generatrix of a general form. \~
      \param[in] sweptData          - \ru Образующая.
                                      \en Generatrix. \~
      \param[in] direction          - \ru Направление выдавливания.
                                      \en Direction of extrusion. \~
      \param[in] operationDirection - \ru Направление выдавливания (прямое, обратное или оба).
                                      \en Direction of extrusion (forward, reverse, or both). \~
      \param[in] solids             - \ru Набор тел для поиска ближайшего 
                                      \en Container of bodies to find the nearest one. \~
      \param[in] version            - \ru Версия исполнения. 
                                      \en The version. \~
  */
  MbNearestSolidParams( const MbSweptData &            sweptData,
                        const MbVector3D &             direction,
                              MbSweptLayout::Direction operationDirection,
                        const c3d::SolidsSPtrVector  & solids,
                              VERSION                  version = Math::DefaultMathVersion() );

  /** \brief \ru Конструктор.
             \en Constructor. \~
      \details \ru Конструктор для образующей общего вида.
               \en Constructor for a generatrix of a general form. \~
      \param[in] sweptData          - \ru Образующая.
                                      \en Generatrix. \~
      \param[in] direction          - \ru Направление выдавливания.
                                      \en Direction of extrusion. \~
      \param[in] operationDirection - \ru Направление выдавливания (прямое, обратное или оба).
                                      \en Direction of extrusion (forward, reverse, or both). \~
      \param[in] valuesAndSides     - \ru Для учета угла уклона и тонкой стенки. В разработке.
                                      \en To account for slope angle and thin wall. In developing. \~
      \param[in] solids             - \ru Набор тел для поиска ближайшего 
                                      \en Container of bodies to find the nearest one. \~
      \param[in] version            - \ru Версия исполнения. 
                                      \en The version. \~
  */
  MbNearestSolidParams( const MbSweptData &            sweptData,
                        const MbVector3D &             direction,
                              MbSweptLayout::Direction operationDirection,
                        const SweptValuesAndSides &    valuesAndSides,
                        const c3d::SolidsSPtrVector  & solids,
                              VERSION                  version = Math::DefaultMathVersion() );


  const MbSweptData &             GetSweptData()        const { return _sweptData; }            ///< \ru Выдать информацию об образующей. \en Get information about the generatrix.
  const MbVector3D &              GetDirectionVector()  const { return _direction; }            ///< \ru Выдать вектор направления выдавливания. \en Get the direction vector of the extrusion.
        MbSweptLayout::Direction  GetDirection()        const { return _operationDirection; }   ///< \ru Выдать направление выдавливания (прямое, обратное или оба). \en Get the direction of the extrusion (forward, reverse, or both).
  const c3d::SolidsSPtrVector &   GetSolids()           const { return _solids; }               ///< \ru Выдать набор тел. \en Get a set of solids.

  const SweptValuesAndSides &     GetValuesAndSides()   const { return _valuesAndSides;     }   ///< \ru Выдать параметры для учета угла уклона о тонкой стенки. \en Get parameters to take into account the slope angle of the thin wall.

  /** \brief \ru Изменение тела по индексу.
             \en Changing the solid by index. \~
      \details \ru Изменить тело по индексу.
               \en Changing the solid by index. \~
      \param[in] ind - \ru Индекс тела в контейнере тел _solids. Соответствует индексу тела в контейнере в аргументах конструктора.
                       \en Solid index in solids container _solids. Corresponds to the index of the solid in the container in the constructor arguments. \~
      \param[in] ind - \ru Новое тело.
                       \en New solid. \~
      \return \ru true, если индекс корректный.
              \en true if the index is valid. \~
  */
        bool                      ChangeSolid( size_t ind, MbSolid * newSolid );

  /// \ru Версия операции. \en Version of the operation. \~
  VERSION GetVersion() const { return _version; }

private:
  // \ru Добавление набора тел. \en Adding a set of solids.
  void Add( const c3d::SolidsSPtrVector & solids );

OBVIOUS_PRIVATE_COPY( MbNearestSolidParams )
};


//------------------------------------------------------------------------------
/** \brief \ru Результат операции поиска ближайшего тела в заданном направлении.
           \en The result of the search operation for the nearest body in the specified direction. \~
    \details \ru Результат операции поиска ближайшего тела в заданном направлении..
             \en The result of the search operation for the nearest body in the specified direction. \n \~
    \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbNearestSolidResult
{
private:
  size_t _nPlus;    ///< \ru Индекс ближайшего тела в прямом направлении.   \en The index of the nearest solid in the forward direction.
  size_t _nMinus;   ///< \ru Индекс ближайшего тела в обратном направлении. \en The index of the nearest solid in the reverse direction.

public:
  /// \ru Конструктор. \en Constructor.
  MbNearestSolidResult()
    : _nPlus ( SYS_MAX_T )
    , _nMinus( SYS_MAX_T )
  {}

  /// \ru Конструктор. \en Constructor.
  MbNearestSolidResult( size_t plus, size_t minus )
    : _nPlus ( plus )
    , _nMinus( minus )
  {}

  /// \ru Конструктор копирования. \en Copy constructor.
  MbNearestSolidResult( const MbNearestSolidResult & other )
    : _nPlus ( other._nPlus )
    , _nMinus( other._nMinus )
  {}

  /// \ru Оператор присваивания. \en assignment operator.
  const MbNearestSolidResult & operator = ( const MbNearestSolidResult & other ) {
    _nPlus  = other._nPlus;
    _nMinus = other._nMinus;
    return *this;
  }

  ///< \ru Индекс ближайшего тела в прямом направлении. \en The index of the nearest solid in the forward direction.
  size_t GetNearestPlus()  const { return _nPlus;  }
  ///< \ru Индекс ближайшего тела в обратном направлении. \en The index of the nearest solid in the opposite direction.
  size_t GetNearestMinus() const { return _nMinus; }
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры создания скалярной функции.
           \en Parameters of creating a scalar function. \~
  \details \ru Параметры создания скалярной функции.
           \en Parameters of creating a scalar function. \~
  \ingroup Function_Building_Parameters
*/ // ---
class MATH_CLASS MbFunctionParameters {
private:
  bool  _bNormalizeDir; ///< \ru Флаг согласования направления изменения функции с возрастанием X-координаты кривой.\en Flag for matching the function change direction with increasing X-coordinate of the curve.\~

public:
  /// \ru Конструктор. \en Constructor.
  MbFunctionParameters( VERSION version = Math::DefaultMathVersion() )
    : _bNormalizeDir( version > C3D_2023_VERSION )
  {}
  /// \ru Деструктор. \en Destructor.
  virtual ~MbFunctionParameters() {}

  /// \ru Выдать флаг согласования направления изменения функции с направлением возрастания X-координаты кривой. \en Get a matching flag of the function change direction with the increase direction of the X-coordinate of the curve. \~
  bool  NormalizeDir() const { return _bNormalizeDir; }

  OBVIOUS_PRIVATE_COPY( MbFunctionParameters );
};


#endif // __OP_SWEPT_PARAMETERS_H
