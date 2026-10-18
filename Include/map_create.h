////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Модуль проецирования. Главные функции и интерфейсы.
         \en The projection module. The general functions and interfaces. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MAP_CREATE_H
#define __MAP_CREATE_H


#include <map_lump.h>
#include <map_vestige.h>
#include <templ_ss_array.h>
#include <templ_rp_array.h>
#include <mesh_primitive.h>
#include <curve3d.h>
#include <templ_pointer.h>


class   MbPlacement3D;
class   MbGrid;
class   MbFloatGrid;


////////////////////////////////////////////////////////////////////////////////
//
// \ru Построение плоских проекций по модельным объектам \en The planar projections construction on model objects 
//
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
/** \brief \ru Дополнительные проецируемые объекты.
           \en The additional projected objects. \~

  \details \ru Дополнительные проецируемые объекты содержат:\n
    - аннотационные кривые,\n
    - аннотационные объекты,\n
    - условные обозначения,\n
    - пространственные точки,\n
    - пространственные кривые.\n
    Внимание: \n
    1. Элементы в массивах должны лежать с захватом по счетчику ссылок. \n
    2. Объект безусловно владеет массивами. Если массивы созданы локально, то их нужно отпустить через вызов Relinquish.
           \en The class of additional projected objects. \n
    Contains sets of projected objects:\n
    - annotation curves,\n
    - annotation objects,\n
    - conventional notations,\n
    - spatial points,\n
    - spatial curves.\n
    Attention: \n
    1. The elements in the arrays must lie with the capture by the reference count. \n
    2. The object owns arrays. If the arrays were created locally, then you have to  release them using the object function Relinquish. \~
  \ingroup Mapping
*/
// ---
class MATH_CLASS MbProjectionsObjects {

public:  
  TPointer< RPArray<MbAnnCurves> >          annCurves;     ///< \ru Аннотационные кривые (может быть нулем). \en Annotation curves (can be nullptr).
  TPointer< RPArray<MbSimbolthThreadView> > annotations;   ///< \ru Аннотационные объекты (может быть нулем). \en Annotation objects (can be nullptr).
  TPointer< RPArray<MbSymbol> >             symbolObjects; ///< \ru Условные обозначения (может быть нулем). \en Conventional notations (can be nullptr).
  TPointer< RPArray<MbSpacePoints> >        pointsData;    ///< \ru Пространственные точки (может быть нулем). \en Spatial points (can be nullptr).
  TPointer< RPArray<MbSpaceCurves> >        curvesData;    ///< \ru Пространственные кривые (может быть нулем). \en Spatial curves (can be nullptr).

public:
  /** \brief \ru Конструктор.
             \en Constructor. \~

    \details \ru Конструктор пустых наборов проецируемых объектов.\n
             \en Constructor of empty sets of projected objects.\n \~      
  */
  MbProjectionsObjects()
    : annCurves    ( nullptr )
    , annotations  ( nullptr )
    , symbolObjects( nullptr )
    , pointsData   ( nullptr )
    , curvesData   ( nullptr )
  {}
  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  MbProjectionsObjects( const MbProjectionsObjects & other, MbRegDuplicate * iReg );

public:
  /// \ru Деструктор. Отпускает захваченные объекты. \en Destructor. Detaches captured objects. 
  ~MbProjectionsObjects()
  {
    if ( annCurves )
      ::ReleaseItems( *annCurves );
    if ( annotations )
      ::ReleaseItems( *annotations );
    if ( symbolObjects )
      ::ReleaseItems( *symbolObjects );
    if ( pointsData )
      ::ReleaseItems( *pointsData );
    if ( curvesData )
      ::ReleaseItems( *curvesData );
  }

  /// \ru Дать копию объекта. \en Get a copy of the object. 
  virtual MbProjectionsObjects & Duplicate( MbRegDuplicate * iReg = nullptr ) const;

  /// \ru Отпустить все указатели. \en Detach all pointers. 
  void Relinquish()
  {
    annCurves.Relinquish();   
    annotations.Relinquish();
    symbolObjects.Relinquish();
    pointsData.Relinquish();
    curvesData.Relinquish();
  }


KNOWN_OBJECTS_RW_REF_OPERATORS_EX( MbProjectionsObjects, MATH_FUNC_EX )
OBVIOUS_PRIVATE_COPY( MbProjectionsObjects )
}; // MbProjectionsObjects


//------------------------------------------------------------------------------
/** \brief \ru Структура для обмена данными между потоками.
           \en A structure for data transmission between threads. \~

  \details \ru Структура для обмена данными между потоками. \n
           \en A structure for data transmission between threads. \n \~
  \ingroup Mapping
*/
// ---
class MATH_CLASS GetVestigesTransData {
public:
  typedef RPArray<MbLump> Lumps;
private:
  Lumps                 lumps;     ///< \ru Проецируемые объекты (владеет). \en Projected objects (owns) 
public:
  MbPlacement3D         formPlace; ///< \ru Проекционная плоскость. \en A projection plane. 
  double                znear;     ///< \ru Параметр перспективного изображения. \en A parameter of perspective image. 
  MbProjectionsObjects  prObjects; ///< \ru Дополнительные проецируемые объекты. \en Additional projected objects 
  MbVEFVestiges         vestiges;  ///< \ru Результат. \en The result. 
  MbMapVisibilityMode   visMode;   ///< \ru Показывать невидимые линии. \en Show invisible lines. 
  VERSION               version;   ///< \ru Под какую версию проецируем. \en Version of projecting. 

public:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор с версией по умолчанию.\n
             \en Constructor with default version.\n \~      
  */
  GetVestigesTransData() 
    : lumps    ( 0, 1                       )
    , formPlace(                            )
    , znear    ( UNDEFINED_DBL              )
    , prObjects(                            )
    , vestiges (                            )
    , visMode  ( false, false               )
    , version  ( Math::DefaultMathVersion() ) 
  {}

  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  GetVestigesTransData( const GetVestigesTransData & other, MbRegDuplicate * iReg );

  /// \ru Деструктор. \en Destructor. 
  virtual ~GetVestigesTransData() {
    DeleteLumps();
  }

public:
        Lumps &   SetLumps() { return lumps; }

        size_t    GetLumpsCount() const { return lumps.size(); }
  const MbLump * _GetLump( size_t k ) const { return lumps[k]; }
  const MbLump *  GetLump( size_t k ) const { return ((k < lumps.size()) ? lumps[k] : nullptr); }
        MbLump * _SetLump( size_t k )       { return lumps[k]; }
        MbLump *  SetLump( size_t k )       { return ((k < lumps.size()) ? lumps[k] : nullptr); }

        template <class Lumps>
        void      GetLumps( Lumps & _lumps ) const
        {
          size_t getCnt = lumps.size();
          if ( getCnt > 0 ) {
            _lumps.reserve( getCnt );
            for ( size_t k = 0; k < getCnt; ++k ) {
              SPtr<MbLump> lump( const_cast<MbLump *>( lumps[k] ) );
              _lumps.push_back( lump );
              ::DetachItem( lump );
            }
          }
        }

        void      AddLump( const MbLump & );

        template <class Lumps>
        void      AddLumps( Lumps & _lumps )
        {
          size_t addCnt = _lumps.size();
          if ( addCnt > 0 ) {
            lumps.reserve( addCnt );
            for ( size_t k = 0; k < addCnt; ++k ) {
              const MbLump * lump = _lumps[k];
              ::AddRefItem( lump );
              lumps.push_back( lump );
            }
          }
        }

        MbLump *  DetachLump( size_t k );

        template <class Lumps>
        void      DetachLumps( Lumps & _lumps )
        {
          size_t detCnt = lumps.size();
          if ( detCnt > 0 ) {
            _lumps.reserve( detCnt );
            for ( size_t k = 0; k < detCnt; ++k ) {
              ::DecRefItem( lumps[k] );
              SPtr<MbLump> lump( lumps[k] );
              _lumps.push_back( lump );
              ::DetachItem( lump );
            }
            lumps.clear();
          }
        }

        void      DeleteLumps() {
          ::ReleaseItems( lumps );
        }


KNOWN_OBJECTS_RW_REF_OPERATORS_EX( GetVestigesTransData, MATH_FUNC_EX )
OBVIOUS_PRIVATE_COPY( GetVestigesTransData )
}; // GetVestigesTransData


//------------------------------------------------------------------------------
/** \brief \ru Построение проекций вида.
           \en Construction of view projections. \~

  \details \ru Построения проекций вида на указанную плоскость.\n
    Создает набор следов объектов - тел с матрицами и дополнительных проецируемых объектов.
           \en Construction of view projections to the given plane.\n
    Creates a set of vestiges of objects - solids with matrices and additional projected objects. \~
  \note    \ru В многопоточном режиме выполняется параллельно. 
           \en In multithreaded mode runs in parallel. \~

  \param[in]  place     - \ru Проекционная плоскость.
                          \en A projection plane. \~
  \param[in]  znear     - \ru Параметр перспективного изображения. Задавать равным 0.0.
                          \en A parameter of perspective image. Should be set to 0.0. \~
  \param[in]  lumps     - \ru Проецируемые объекты.
                          \en Projected objects. \~  
  \param[in]  objects   - \ru Дополнительные проецируемые объекты.
                          \en Additional projected objects. \~
  \param[out] result    - \ru Результат.
                          \en The result. \~
  \param[in]  visMode   - \ru Настройки видимости следов проецируемых объектов.
                              Относится к проекциям тел, пространственным точкам, пространственным кривым.
                              Не используется для условных обозначений.
                          \en Visibility mode of mapping.
                              Applicable to projections of solids, spatial points, spatial curves.
                              Not applicable to conventional notations. \~
  \param[in]  version   - \ru Версия построения. Последняя версия Math::DefaultMathVersion().
                          \en The version of construction. The last version Math::DefaultMathVersion().
  \param[in]  merge     - \ru Флаг слияния подобных кривых (по умолчанию true).
                          \en Merge same curves (default true). \~
  \param[in]  prevCubes - \ru Габариты тел до изменений.
                          \en Bounding boxes of solids before changes. \~
  \result                 \ru Возвращает результат операции.
                          \en Returns the result of the operation. \~

  \ingroup Mapping
*/
// ---
MATH_FUNC (MbResultType) GetVestiges ( const MbPlacement3D &           place,
                                             double                    znear,
                                       const RPArray<MbLump> &         lumps,
                                       const MbProjectionsObjects &    objects,
                                             MbVEFVestiges &           result,
                                       const MbMapVisibilityMode &     visMode,
                                             VERSION                   version = Math::DefaultMathVersion(),
                                             bool                      merge = true,
                                       const std::vector<MbLumpCube> * prevCubes = nullptr );


//------------------------------------------------------------------------------     
/** \brief 
    \ru Параметры для построения одной проекции вида.
    \en Parameters for the construction of one view projection.\n \~
\ingroup Mapping
*/ 
struct MATH_CLASS MbMapSettings {
  typedef std::vector<MbLumpCube> LumpCubes;
public:
  /// \ru Проекционная плоскость. \en A projection plane.
  MbPlacement3D       m_place;
  /// \ru Параметр перспективного изображения. Задавать равным 0.0. \en A parameter of perspective image. Should be set to 0.0.
  double              m_zNear;

  /** \brief \ru Настройки видимости следов проецируемых объектов.
             \en Visibility mode of mapping. \~
    \details \ru Настройки видимости следов проецируемых объектов.
                 Относится к проекциям тел, пространственным точкам, пространственным кривым.
                 Не используется для условных обозначений.
             \en Visibility mode of mapping.
                 Applicable to projections of solids, spatial points, spatial curves.
                 Not applicable to conventional notations. \~
  */
  MbMapVisibilityMode m_visMode;
  /// \ru Флаг слияния подобных кривых (по умолчанию true). \en Merge same curves (default true).
  bool                m_merge;
  /// \ru Габариты тел до изменений. \en Bounding boxes of solids before changes.
  const LumpCubes *   m_prevCubes;

public:
  MbMapSettings( MbMapVisibilityMode mode, MbPlacement3D place = MbPlacement3D::global, 
                 double znear = 0, bool merge = true, const LumpCubes * prevCubes = nullptr )
    : m_place    ( place )
    , m_zNear    ( znear )
    , m_visMode  ( mode  )
    , m_merge    ( merge )
    , m_prevCubes( prevCubes )
  {}

  MbMapSettings & operator = ( const MbMapSettings & rt )
  {
    m_place = rt.m_place;
    m_zNear = rt.m_zNear;
    m_visMode = rt.m_visMode;
    m_merge = rt.m_merge;
    m_prevCubes = rt.m_prevCubes;
    return *this;
  }

private:
  MbMapSettings(); // \ru Не реализован. \en Not implemented.
}; // MbMapSettings


//------------------------------------------------------------------------------
/** \brief \ru Построение проекций для нескольких видов.
           \en Construction of several view projections. \~

  \details \ru Построение проекций на указанные плоскости.\n
   Создает наборы следов объектов (тел с матрицами и дополнительных проецируемых объектов).
           \en Construction of projections to the given planes.\n
   Creates sets of vestiges of objects (solids with matrices and additional projected objects). \~
   \note   \ru В многопоточном режиме выполняется параллельно. \en In multithreaded mode runs in parallel. \~

  \param[in] settings  - \ru Массив наборов параметров для построения проекций.
                          \en An array of parameter sets for the construction of view projections. \~
  \param[in]  lumps     - \ru Проецируемые объекты.
                          \en Projected objects. \~  
  \param[in]  objects   - \ru Дополнительные проецируемые объекты.
                          \en Additional projected objects. \~
  \param[out] results   - \ru Массив указателей на результаты (перед началом вычислений очищается с удалением объектов).
                          Освобождать созданные результаты должна вызывающая функция.
                          \en The results array (is erased prior to the calculations with objects deletion). 
                          Destruction of the results is a responsibility of the function caller. \~
  \param[in]  version   - \ru Версия построения. Последняя версия Math::DefaultMathVersion().
                          \en The version of construction. The last version Math::DefaultMathVersion().
  \result                 \ru Возвращает результат операции.
                          \en Returns the result of the operation. \~

  \ingroup Mapping
*/
// ---
MATH_FUNC (MbResultType) GetVestiges ( const SArray<MbMapSettings> & settings,
                                       const RPArray<MbLump> &       lumps,
                                       const MbProjectionsObjects &  objects,
                                             PArray<MbVEFVestiges> & results,
                                             VERSION                 version = Math::DefaultMathVersion() );

//------------------------------------------------------------------------------
/** \brief \ru Определение участков граничной кривой.
           \en The definition of boundary curve regions. \~

  \details \ru Определение участков граничной кривой местного вида или выносного вида.
           \en Definition of regions of boundary curve of local view or remote view. \~
  \note \ru В многопоточном режиме выполняется параллельно. \en In multithreaded mode runs in parallel. \~

  \param[in]  lumps        - \ru Проецируемые объекты.
                             \en Projected objects. \~
  \param[in]  contour      - \ru Граничная кривая. В системе координат вида viewInfo.
                             \en A boundary curve. In the coordinate system of the view "viewInfo". \~
  \param[in]  cross        - \ru Точки пересечения граничной кривой с линиями чертежа -
                                 точки разбивки кривой на области разной видимости.
                             \en Points of intersection between boundary curve and lines of drawing - 
                                 points of intersection of curve on regions of different visibility. \~

  \param[in]  baseViewInfo - \ru Информация о базовом виде, на котором построен местный вид или разрез:\n
                                       тип вида:\n
                                         mvt_View - Вид,\n
                                         mvt_Cut - Разрез,\n
                                         mvt_Section - Сечечние;\n
                                       настройки проецирования объектов по глубине видимимости,\n
                                       глубина видимости,\n
                                       плоскость вида, разреза или сечения.\n
                             \en The information about base view, on which a local or sectional view:\n
                                       a view type:\n
                                         mvt_View - View,\n
                                         mvt_Cut - Cutaway,\n
                                         mvt_Section - Section;\n
                                       a mapping mode by visibility depth,\n
                                       a visibility depth,\n
                                       a plane of view, cutaway or section.\n \~


  \param[in]  viewInfo     - \ru Информация о виде:\n
                                       тип вида:\n
                                         mvt_View - Местный вид, Выносной элемент,\n
                                         mvt_Cut - Местный разрез,\n
                                         mvt_Section - Местное сечение;\n
                                       настройки проецирования объектов по глубине видимимости,\n
                                       глубина видимости,\n
                                       плоскость вида, разреза или сечения.\n
                                       Если тип производного и базового вида != mvt_View,
                                       то тип производного вида должен совпадать с видом базового.\n
                                       У базового и производного вида должны быть одинаковые настройки видимости,
                                       и согласованные глубины видимости.\n
                             \en The information about a view:\n
                                       a view type:\n
                                         mvt_View - A local view. A detail view,\n
                                         mvt_Cut - A local cutaway,\n
                                         mvt_Section - A local section;\n
                                       a mapping mode by visibility depth,\n
                                       a visibility depth,\n
                                       a plane of view, cutaway or section.\n
                                       If the type of derived and basic view is not equal to the mvt_View
                                       then the type of the derived view must coincide with the type of the basic view.\n
                                       The basic and derived views must have the same visibility modes
                                       and consistent visibility depths.\n \~


  \internal \ru См. справку "Команда Местный разрез". 
    Если вид, на котором базируется местный разрез (сечение), является разрезом (сечением),
    то тип изображения (производного вида) совпадает с типом опорного вида (базового).
            \en See in documentation about the command "Local cutaway". 
    If a view which the local cutaway (section) is based on is a cutaway (section)
    then the type of image (of derivative view) matches the type of the reference (the base) type. \~
  \endinternal

  \param[out] curves       - \ru Результат - набор видимых участков граничной кривой.
                             \en The result - a set of visible pieces of a boundary curve. \~
  \result                    \ru Возвращает результат операции.
                             \en Returns the result of the operation. \~
  \ingroup Mapping
*/
// ---
MATH_FUNC (MbResultType) JustCutLimitCurve( const RPArray<MbLump> &      lumps,
                                            const MbCurve &              contour,
                                            const SArray<MbCrossPoint> & cross,
                                            const MbMapViewInfo &        baseViewInfo,
                                            const MbMapViewInfo &        viewInfo,
                                                  RPArray<MbCurve> &     curves );

//------------------------------------------------------------------------------
/** \brief \ru Рассчитать полигоны линий.
           \en Calculate polygons of lines. \~
  \details \ru Рассчитать полигоны видимых и невидимых линий для сборки.
           \en Calculate polygons of visible and invisible lines for assembly. \~
  \param[in]  lumps        - \ru Набор тел с матрицами.
                             \en A set of solids with matrices. \~
  \param[in]  place        - \ru Проекционная плоскость.
                             \en A projection plane. \~
  \param[in]  znear        - \ru Параметр перспективного отображения. Задавать равным 0.0.
                             \en A parameter of perspective mapping. Set to be equal to 0.0. \~
  \param[in]  sag          - \ru Угловая толерантность.
                             \en An angular tolerance. \~
  \param[out] visibleEdges - \ru Полигоны видимых линий ребер.
                             \en Polygons of visible lines of edges. \~
  \param[out] hiddenEdges  - \ru Полигоны невидимых линий ребер.
                             \en Polygons of invisible lines of edges. \~
  \param[out] visibleTangs - \ru Полигоны видимых линий гладких ребер.
                             \en Polygons of visible lines of smooth edges. \~
  \param[out] hiddenTangs  - \ru Полигоны невидимых линий гладких ребер.
                             \en Polygons of invisible lines of smooth edges. \~
  \ingroup Mapping
*/
// ---
MATH_FUNC (void) HiddenLinesMapping( const RPArray<MbLump> &          lumps, 
                                     const MbPlacement3D &            place, 
                                           double                     znear, 
                                           double                     sag,
                                           PArray<MbPolygon3DSolid> & visibleEdges, 
                                           PArray<MbPolygon3DSolid> & hiddenEdges,
                                           PArray<MbPolygon3DSolid> & visibleTangs, 
                                           PArray<MbPolygon3DSolid> & hiddenTangs );

//------------------------------------------------------------------------------
/** \brief \ru Рассчитать полигоны видимых линий.
           \en Calculate polygons of visible lines. \~
  \details \ru Рассчитать полигоны видимых линий и линий очерка для сборки.
           \en Calculate polygons of visible lines and isocline curves for assembly. \~
  \param[in]  lumps        - \ru Набор тел с матрицами.
                             \en A set of solids with matrices. \~
  \param[in]  place        - \ru Проекционная плоскость.
                             \en A projection plane. \~
  \param[in]  znear        - \ru Параметр перспективного отображения. Задавать равным 0.0.
                             \en A parameter of perspective mapping. Set to be equal to 0.0. \~
  \param[in]  sag          - \ru Угловая толерантность.
                             \en An angular tolerance. \~
  \param[out] visibleEdges - \ru Полигоны видимых линий.
                             \en Polygons of visible lines. \~
  \param[out] visibleTangs - \ru Полигоны видимых линий гладких ребер.
                             \en Polygons of visible lines of smooth edges. \~
  \param[in] version        - \ru Версия построения.
                              \en The version. \~
  \ingroup Mapping
*/
// ---
MATH_FUNC (void) VisualLinesMapping( const RPArray<MbLump> &          lumps, 
                                     const MbPlacement3D &            place, 
                                           double                     znear, 
                                           double                     sag,
                                           PArray<MbPolygon3DSolid> & visibleEdges, 
                                           PArray<MbPolygon3DSolid> & visibleTangs,
                                           VERSION                    version = Math::DefaultMathVersion() );

//------------------------------------------------------------------------------
/** \brief \ru Построить силуэтные линий по триангуляционной сетке.
           \en Construct silhouette lines by a triangular mesh. \~
  \details \ru Выдать силуэтные линии в виде пар указателей на существующие точки в массиве триангуляции.
           \en Get silhouette lines as pairs of pointers to the existed points in the array of triangulation. \~
  \param[in]  grid        - \ru Триангуляционная сетка.
                            \en A triangular mesh. \~
  \param[in]  matrix      - \ru Матрица для задания вектора взгляда.
                            \en A matrix for setting of view vector. \~
  \param[in]  perspective - \ru Признак перспективного отображения.
                            \en An attribute of perspective mapping. \~
  \param[out] points      - \ru Результат - набор точек.
                            \en The result - the set of points. \~
  \ingroup Triangulation
*/
// ---
MATH_FUNC (void) CalculateBoundsSltFast( const MbFloatGrid &             grid, 
                                         const MbMatrix3D &              matrix, 
                                               bool                      perspective,  
                                               RPArray<MbFloatPoint3D> & points );

//------------------------------------------------------------------------------
/** \brief \ru Построить силуэтные линий по триангуляционной сетке.
           \en Construct silhouette lines by a triangular mesh. \~
  \details \ru Выдать силуэтные линии в виде пар точек.
           \en Get silhouette lines as pairs of points. \~
  \param[in]  grid        - \ru Триангуляционная сетка.
                            \en A triangular mesh. \~
  \param[in]  matrix      - \ru Матрица для задания вектора взгляда.
                            \en A matrix for setting of view vector. \~
  \param[in]  perspective - \ru Признак перспективного отображения.
                            \en An attribute of perspective mapping. \~
  \param[out] points      - \ru Результат - набор точек.
                            \en The result - the set of points. \~
  \ingroup Triangulation
*/
// ---
MATH_FUNC (void) CalculateBoundsSlt( const MbGrid &                 grid, 
                                     const MbMatrix3D &             matrix, 
                                           bool                     perspective,  
                                           SArray<MbFloatPoint3D> & points );

//------------------------------------------------------------------------------
/** \brief \ru Построить линии сечения по триангуляционной сетке.
           \en Construct section lines by a triangular mesh. \~
  \details \ru Выдать линии сечения в виде пар точек.
           \en Get section lines as pairs of points. \~
  \param[in]  grid        - \ru Триангуляционная сетка.
                            \en A triangular mesh. \~
  \param[in]  matrix      - \ru Матрица для задания плоскости сечения.
                            \en A matrix for setting of section plane. \~
  \param[out] points      - \ru Результат - набор точек.
                            \en The result - the set of points. \~
  \ingroup Triangulation
*/
// ---
MATH_FUNC (void) CalculateSections( const MbGrid &                 grid, 
                                    const MbMatrix3D &             matrix, 
                                          SArray<MbFloatPoint3D> & points );


#endif // __MAP_CREATE_H
