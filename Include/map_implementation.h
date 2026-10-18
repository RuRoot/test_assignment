////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Множество проекций тел.
         \en The array of projections of solids. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MAP_IMPLEMENTATION_H
#define __MAP_IMPLEMENTATION_H


#include <map_lump.h>
#include <templ_psrt_array.h>


struct           MbLump;
struct           MbBody;
class            MbMapBody;
class  MbProjectionsObjects;
class  MbSpacePoints;
class  MbSpaceCurves;
class  MbSymbol;
class  MbMapViewInfo;



////////////////////////////////////////////////////////////////////////////////
//
// \ru Построение плоских проекций по модельным объектам \en The planar projections construction on model objects 
//
////////////////////////////////////////////////////////////////////////////////

  enum MbeMatrixCompareResult 
  {
    mcr_Same,
    mcr_Shift,
    mcr_ShiftZ,
    mcr_Other
  };

//------------------------------------------------------------------------------
/** \brief \ru Множество проекций тел.
           \en The set of solids projections. \~
  \details \ru Множество проекций тел.\n
    Содержит набор проецируемых тел.
           \en The set of solids projections.\n
    Contains a set of projected solids. \~  
  \ingroup Mapping
*/
// ---
class MATH_CLASS MbMapBodiesPArray {

private: 
  RPArray<MbMapBody> mapBodies;         ///< \ru Множество проекций тел \en A set of solids projections     
  SArray<size_t>     originIndices;     ///< \ru Индексы оригинальных тел в mapBodies. \en indices of original body in mapBodies.
                                        ///< \ru Все тела в mapBodies сгруппированы так OsssOssOOOssOsO, O - оригинальное тело, s - тело полученное сдвигом. 
                                        ///< \en All bodies in mapBodies was grouped like OsssOssOOOssOsO? O- original body, s - body was shifted. 
public:
  /// \ru Конструктор объекта с пустым набором тел. \en Constructor of an object with an empty set of solids. 
  MbMapBodiesPArray() : mapBodies( 0, 1 ), originIndices( 0, 1 ) {}
  /// \ru Деструктор. \en Destructor. 
  ~MbMapBodiesPArray();

public:  
  /// \ru Число проецируемых тел. \en The number of projected solids. 
        size_t        MapBodiesCount() const { return mapBodies.Count(); }

  /** \brief \ru Выдать тело.
             \en Get a solid. \~
    \details \ru Выдать тело по индексу. Без проверки индекса. \n
             \en Get a solid by an index. Without index checking. \n \~
    \param[in] index - \ru Индекс тела.
                       \en A solid index. \~
  */
        MbMapBody *  _SetMapBody( size_t index ) const { return mapBodies[index]; }

  /** \brief \ru Выдать тело.
             \en Get a solid. \~
    \details \ru Выдать тело по индексу. Без проверки индекса. \n
             \en Get a solid by an index. Without index checking. \n \~
    \param[in] index - \ru Индекс тела.
                       \en A solid index. \~
  */
  const MbMapBody *  _GetMapBody( size_t index ) const { return mapBodies[index]; }

  /** \brief \ru Построение ассоциативных проекций.
             \en The construction of associative projections. \~
    \details \ru Построение ассоциативных проекций. Первая часть.\n
             \en The construction of associative projections. The first part.\n \~
    \param[in] lumps       - \ru Набор тел для проецирования.
                             \en A set of solids for projection. \~
    \param[in] into        - \ru Матрица перехода в локальную систему координат.
                             \en A matrix of translation to the local coordinate system. \~
    \param[in] znear       - \ru Параметр перспективного отображения. Задавать равным 0.0.
                             \en A parameter of perspective mapping. Set to be equal to 0.0. \~
    \param[in] perspective - \ru Признак перспективного отображения. Задавать равным false.
                             \en An attribute of perspective mapping. Set to be equal to false. \~
    \param[in] visMode     - \ru Признак проецирования невидимых линий.
                             \en An attribute of invisible lines projection. \~
    \param[in] version     - \ru Версия построения чертежа. Задавать версию Math::DefaultMathVersion().
                             \en The version of construction of drawing. Set the Math::DefaultMathVersion() version. \~
    \param[in]  prevCubes  - \ru Габариты тел до изменений.
                             \en Bounding boxes of solids before changes. \~
    \warning \ru Для внутреннего использования.
             \en For internal use only. \~
  */
        MbResultType  CreateFirst( const RPArray<MbLump> & lumps, 
                                   const MbMatrix3D & into, double znear,
                                   bool perspective, const MbMapVisibilityMode & visMode, VERSION version,
                                   const std::vector<MbLumpCube> * prevCubes = nullptr );

  /** \brief \ru Построение ассоциативных проекций.
             \en The construction of associative projections. \~
    \details \ru Построение ассоциативных проекций. Вторая часть.\n    
      Построение проекций и заполнение следов объектов.
             \en The construction of associative projections. The second part.\n    
      The construction of projections and filling the vestiges. \~
    \param[out] vestiges      - \ru Результат - набор следов проецируемых объектов.
                                \en The result is a set of vestiges of projected objects. \~
    \param[in]  visMode       - \ru Признак проецирования невидимых линий.
                                \en An attribute of the invisible lines projection. \~
    \param[in]  into          - \ru Матрица перехода в локальную систему координат.
                                \en A matrix of translation to the local coordinate system. \~
    \param[in]  znear         - \ru Параметр перспективного отображения. Задавать равным 0.0.
                                \en A parameter of perspective mapping. Set to be equal to 0.0. \~
    \param[in]  perspective   - \ru Признак перспективного отображения. Задавать равным false.
                                \en An attribute of perspective mapping. Set to be equal to false. \~
    \param[in]  annotations   - \ru Набор аннотационных объектов.
                                \en A set of annotation objects. \~
    \param[in]  pointsData    - \ru Набор пространственных точек.
                                \en A set of spatial points. \~
    \param[in]  curvesData    - \ru Набор пространственных кривых.
                                \en A set of spatial curves. \~
    \param[in]  symbolObjects - \ru Набор условных обозначений.
                                \en A set of conventional notations. \~    
    \param[in]  merge         - \ru Флаг слияния подобных кривых (по умолчанию true).
                                \en Murge same curves (true default). \~    
    \warning \ru Для внутреннего использования.
             \en For internal use only. \~     
  */
      void            GetVestiges( MbVEFVestiges & vestiges, 
                                   const MbMapVisibilityMode & visMode, const MbMatrix3D & into, 
                                   double znear, bool perspective,
                                   PArraySort<MbSimbolthThreadView> * annotations,
                                   RPArray<MbSpacePoints> * pointsData,
                                   RPArray<MbSpaceCurves> * curvesData,
                                   RPArray<MbSymbol> *      symbolObjects, 
                                   bool merge = true );

  /** \brief \ru Рассчитать полигоны линий.
             \en Calculate polygons of lines. \~
    \details \ru Рассчитать полигоны видимых и невидимых линий для сборки.
             \en Calculate polygons of visible and invisible lines for assembly. \~       
    \param[out] arVL        - \ru Полигоны видимых линий ребер.
                              \en Polygons of visible lines of edges. \~
    \param[out] arHL        - \ru Полигоны невидимых линий ребер.
                              \en Polygons of invisible lines of edges. \~
    \param[out] arVT        - \ru Полигоны видимых линий гладких ребер.
                              \en Polygons of visible lines of smooth edges. \~
    \param[out] arHT        - \ru Полигоны невидимых линий гладких ребер.
                              \en Polygons of invisible lines of smooth edges. \~
    \param[in]  sag         - \ru Угловая толерантность.
                              \en An angular tolerance. \~
    \param[in]  annotations - \ru Набор аннотационных объектов.
                              \en A set of annotation objects. \~    
    \param[in]  place       - \ru Проекционная плоскость.
                              \en A projection plane. \~
    \param[in]  into        - \ru Матрица перехода в локальную систему координат.
                              \en A matrix of translation to the local coordinate system. \~   
    \warning \ru Для внутреннего использования.
             \en For internal use only. \~ 
  */
      void            GetVestiges( PArray<MbPolygon3DSolid> & arVL, 
                                   PArray<MbPolygon3DSolid> & arHL,
                                   PArray<MbPolygon3DSolid> & arVT, 
                                   PArray<MbPolygon3DSolid> & arHT, 
                                   double sag,
                                   PArraySort<MbSimbolthThreadView> * annotations,
                                   const MbPlacement3D & place,
                                   const MbMatrix3D & into );

private:
  // \ru заполнить массивы линий и поверхностей \en fill arrays of lines and surfaces 
      void            Fill              ( const RPArray<MbLump> & lumps, const MbMatrix3D & into, 
                                          double znear, bool perspective, const MbMapVisibilityMode & visMode, 
                                          VERSION version, const std::vector<MbLumpCube> * prevCubes ); 
  // \ru тест видимости пробной точки \en the test of a trial point visibility 
      int             IsShadeAll        ( const MbCartPoint3D &, bool section ) const;
  // \ru получение очерков и разрезка \en isocline getting and cutting 
  // \note \ru В многопоточном режиме m_Items выполняется параллельно. \en In multithreaded mode m_Items runs in parallel. \~
      void            BodiesCreateFirst();   
  // \ru Определение видимости тел. \en Define solids visibility. 
  // \note \ru В многопоточном режиме m_Items выполняется параллельно. \en In multithreaded mode m_Items runs in parallel. \~
      void            BodiesCreateSecondLocal( const PArraySort<MbSimbolthThreadView> * annotations );   
  // \ru добавление тела \en addition of solid 
      bool            AddBody( MbMapBody * mapBody, size_t & replaceInd, bool checkShadedSolids,
                               c3d::BoolVector & isSolids, c3d::IndicesVector & nFacesVec );

OBVIOUS_PRIVATE_COPY( MbMapBodiesPArray )
};


#endif // __MAP_IMPLEMENTATION_H
