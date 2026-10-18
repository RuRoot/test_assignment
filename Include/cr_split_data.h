////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Усекающие элементы оболочки.
         \en Truncating elements of a shell. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_SPLIT_DATA_H
#define __CR_SPLIT_DATA_H

#include <mb_placement3d.h>
#include <reference_item.h>
#include <cur_contour.h>
#include <curve3d.h>
#include <surface.h>
#include <creator.h>

class  reader;
class  writer;
class  MbProperties;
class  MbCartPoint3D;
class  MbAxis3D;
class  MbSpaceItem;
class  MbCurve;
class  MbSurfaceIntersectionCurve;
class  MbSNameMaker;
class  MbSolid;
class  MbWireFrame;
struct MbControlData3D;
class  MbRegDuplicate;
class  MbRegTransform;
enum   MbeSenseValue;
enum   MbeCopyMode;


//------------------------------------------------------------------------------
/** \brief \ru Усекающие элементы.
           \en Truncating elements. \~
  \details \ru Усекающие элементы используются для разделения граней на части и усечения оболочек. 
    Усечение может выполняться двумерными кривыми, расположенными в плоскости XY локальной системы координат,
    трёхмерными кривыми, поверхностями, оболочками и каркасом. 
    Усекающие элементы используются в строителе усеченной оболочки MbTruncatedShell и 
    строителе оболочки с разбиением граней MbSplitShell. \n
           \en Truncating elements are used for splitting faces into parts and truncation of shells. 
    Truncating can be performed by two-dimensional curves located in the XY plane of the local coordinate system,
    by three-dimensional curves, surfaces, shells and wire frame. 
    Truncating elements are used in the creator of truncated shell MbTruncatedShell and 
    in the creator of shell with face splitting MbSplitShell. \n \~
  \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbSplitData : public MbRefItem {
  /// \ru Типы усекающих объектов. \en Truncating objects types. 
  enum MbeSplitItemsType {
    sit_NoItems    = 0, ///< \ru Нет объектов. \en No objects. 
    sit_Curves2d   = 1, ///< \ru Двумерные кривые в локальной системе координат. \en Two-dimensional curves in the local coordinate system.  
    sit_Curves3d   = 2, ///< \ru Трехмерные кривые. \en Three-dimensional curves. 
    sit_Surfaces   = 3, ///< \ru Поверхности. \en Surfaces. 
    sit_Creators   = 4, ///< \ru Строители тела. \en Solid creators.
    sit_WfCreators = 5, ///< \ru Строители каркаса. \en Wire frame.
  };

private:
  // Sketch contours
  c3d::PlaneContoursSPtrVector  sketchContours; ///< \ru Двумерные кривые. \en Two-dimensional curves. 
  MbPlacement3D                 place;          ///< \ru Локальная система координат двумерных кривых. \en Local coordinate system of two-dimensional curves. 
  MbVector3D                    direction;      ///< \ru Вектор выдавливания двумерных кривых. \en Extrusion direction vector of two-dimensional curves. 
  MbeSenseValue                 sense;          ///< \ru Направление выдавливания двумерных кривых относительно вектора. \en Extrusion direction of two-dimensional curves relative to direction vector. 
  // Space Curves
  c3d::SpaceCurvesSPtrVector    spaceCurves;    ///< \ru Пространственные кривые. \en Spatial curves. 
  // Surfaces
  c3d::SurfacesSPtrVector       surfaces;       ///< \ru Поверхности. \en Surfaces. 
  // Shell
  c3d::CreatorsSPtrVector       creators;       ///< \ru Строители оболочки. \en Shell creators. 
  c3d::ShellSPtr                solidShell;     ///< \ru Оболочка. \en A shell. 
  // Wire frame
  c3d::CreatorsSPtrVector       wfCreators;     ///< \ru Строители каркаса. \en Wire frame creators.

public:
  /// \ru Конструктор. \en Constructor. 
  MbSplitData()
    : sketchContours(             )
    , place         (             )
    , direction     (             )
    , sense         ( orient_BOTH )
    , spaceCurves   (             )
    , surfaces      (             )
    , creators      (             )
    , solidShell    ( nullptr     )
    , wfCreators    (             )
  {
  }
  /// \ru Конструктор по двумерному контуру в локальной системе координат. \en Constructor by two-dimensional contour in the local coordinate system. 
  MbSplitData( const MbPlacement3D & pl, MbeSenseValue dirSense, const MbContour & item, bool same )
    : sketchContours(           )
    , place         ( pl        )
    , direction     (           )
    , sense         ( dirSense  )
    , spaceCurves   (           )
    , surfaces      (           )
    , creators      (           )
    , solidShell    ( nullptr   )
    , wfCreators    (           )
  {
    SPtr<MbContour> sketchContour;
    sketchContour = same ? const_cast<MbContour *>(&item) : static_cast<MbContour *>(&item.Duplicate());
    sketchContours.push_back( sketchContour );
  }
  /// \ru Конструктор по двумерному контуру в локальной системе координат. \en Constructor by two-dimensional contour in the local coordinate system. 
  MbSplitData( const MbPlacement3D & pl, const MbVector3D & dir, const MbContour & item, bool same )
    : sketchContours(             )
    , place         ( pl          )
    , direction     ( dir         )
    , sense         ( orient_BOTH )
    , spaceCurves   (             )
    , surfaces      (             )
    , creators      (             )
    , solidShell    ( nullptr     )
    , wfCreators    (             )
  {
    C3D_ASSERT( (direction.MaxFactor() < LENGTH_EPSILON) || !direction.Orthogonal( place.GetAxisZ(), ANGLE_EPSILON ) );

    SPtr<MbContour> sketchContour;
    sketchContour = same ? const_cast<MbContour *>(&item) : static_cast<MbContour *>(&item.Duplicate());
    sketchContours.push_back( sketchContour );
  }
  /// \ru Конструктор по двумерным контурам в локальной системе координат. \en Constructor by two-dimensional contours in the local coordinate system. 
  template <class PlaneContoursVector>
  MbSplitData( const MbPlacement3D & pl, MbeSenseValue dirSense, const PlaneContoursVector & items, bool same )
    : sketchContours(           )
    , place         ( pl        )
    , direction     (           )
    , sense         ( dirSense  )
    , spaceCurves   (           )
    , surfaces      (           )
    , creators      (           )
    , solidShell    ( nullptr   )
    , wfCreators    (           )
  {
    ::AddRefItems( items, same, sketchContours );
  }
  /// \ru Конструктор по двумерным контурам в локальной системе координат. \en Constructor by two-dimensional contours in the local coordinate system. 
  template <class PlaneContoursVector>
  MbSplitData( const MbPlacement3D & pl, const MbVector3D & dir, const PlaneContoursVector & items, bool same )
    : sketchContours(             )
    , place         ( pl          )
    , direction     ( dir         )
    , sense         ( orient_BOTH )
    , spaceCurves   (             )
    , surfaces      (             )
    , creators      (             )
    , solidShell    ( nullptr     )
    , wfCreators    (             )
  {
    C3D_ASSERT( (direction.MaxFactor() < LENGTH_EPSILON) || !direction.Orthogonal( place.GetAxisZ(), ANGLE_EPSILON ) );

    ::AddRefItems( items, same, sketchContours );
  }
  /// \ru Конструктор по пространственным кривым. \en Constructor by spatial curves. 
  MbSplitData( const c3d::ConstSpaceCurvesSPtrVector & items, bool same )
    : sketchContours(             )
    , place         (             )
    , direction     (             )
    , sense         ( orient_BOTH )
    , spaceCurves   (             )
    , surfaces      (             )
    , creators      (             )
    , solidShell    ( nullptr     )
    , wfCreators    (             )
  {
    ::AddRefItems( items, same, spaceCurves );
  }
  /// \ru Конструктор по пространственным кривым. \en Constructor by spatial curves. 
  MbSplitData( const c3d::ConstSpaceCurvesVector & items, bool same )
    : sketchContours(             )
    , place         (             )
    , direction     (             )
    , sense         ( orient_BOTH )
    , spaceCurves   (             )
    , surfaces      (             )
    , creators      (             )
    , solidShell    ( nullptr     )
    , wfCreators    (             )
  {
    ::AddRefItems( items, same, spaceCurves );
  }
  /// \ru Конструктор по поверхности. \en Constructor by a surface. 
  MbSplitData( const MbSurface & item, bool same )
    : sketchContours(             )
    , place         (             )
    , direction     (             )
    , sense         ( orient_BOTH )
    , spaceCurves   (             )
    , surfaces      (             )
    , creators      (             )
    , solidShell    ( nullptr     )
    , wfCreators    (             )
  {
    SPtr<MbSurface> surface;
    surface = same ? const_cast<MbSurface *>(&item) : static_cast<MbSurface *>(&item.Duplicate());
    surfaces.push_back( surface );
  }
  /// \ru Конструктор по поверхностям. \en Constructor by surfaces. 
  MbSplitData( const c3d::ConstSurfacesSPtrVector & items, bool same )
    : sketchContours(             )
    , place         (             )
    , direction     (             )
    , sense         ( orient_BOTH )
    , spaceCurves   (             )
    , surfaces      (             )
    , creators      (             )
    , solidShell    ( nullptr     )
    , wfCreators    (             )
  {
    ::AddRefItems( items, same, surfaces );
  }
  /// \ru Конструктор по поверхностям. \en Constructor by surfaces. 
  MbSplitData( const c3d::ConstSurfacesVector & items, bool same )
    : sketchContours(             )
    , place         (             )
    , direction     (             )
    , sense         ( orient_BOTH )
    , spaceCurves   (             )
    , surfaces      (             )
    , creators      (             )
    , solidShell    ( nullptr     )
    , wfCreators    (             )
  {
    ::AddRefItems( items, same, surfaces );
  }
  /// \ru Конструктор по телу. \en Constructor by a solid. 
  MbSplitData( const MbSolid & solid, bool same, bool keepShell );
  /// \ru Конструктор по каркасу. \en
  MbSplitData( const MbWireFrame & wireFrame, bool same );
  /// \ru Конструктор копирования с регистратором копирования. \en Copy constructor with registrator of copying. 
  explicit MbSplitData( const MbSplitData &, bool same, MbRegDuplicate * iReg );
  /// \ru Деструктор. \en Destructor. 
 ~MbSplitData();

public:
        /// \ru Инициализировать по двумерному контуру в локальной системе координат. \en Initialize by two-dimensional contour in the local coordinate system. 
        bool          InitPlaneContour( const MbPlacement3D & pl, MbeSenseValue dirSense, const MbContour & item, bool same )
        {
          DeleteItems();
          place.Init( pl );
          direction.SetZero();
          sense = dirSense;

          SPtr<MbContour> sketchContour;
          sketchContour = same ? const_cast<MbContour *>( &item ) : static_cast<MbContour *>(&item.Duplicate());
          sketchContours.push_back( sketchContour );
          return true;
        }
        /// \ru Инициализировать по двумерному контуру в локальной системе координат. \en Initialize by two-dimensional contour in the local coordinate system. 
        bool          InitPlaneContour( const MbPlacement3D & pl, const MbVector3D & dir, const MbContour & item, bool same )
        {
          DeleteItems();
          place.Init( pl );
          direction.Init( dir );
          C3D_ASSERT( (direction.MaxFactor() < LENGTH_EPSILON) || !direction.Orthogonal( place.GetAxisZ(), ANGLE_EPSILON ) );
          sense = orient_BOTH;

          SPtr<MbContour> sketchContour;
          sketchContour = same ? const_cast<MbContour *>( &item ) : static_cast<MbContour *>(&item.Duplicate());
          sketchContours.push_back( sketchContour );
          return true;
        }
        /// \ru Инициализировать по двумерным контурам в локальной системе координат. \en Initialize by two-dimensional contours in the local coordinate system. 
        template <class PlaneContoursVector>
        bool          InitPlaneContours( const MbPlacement3D & pl, MbeSenseValue dirSense, const PlaneContoursVector & items, bool same )
        {
          if ( items.size() > 0 ) {
            DeleteItems();
            place.Init( pl );
            direction.SetZero();
            sense = dirSense;

            ::AddRefItems( items, same, sketchContours );
            return true;
          }
          return false;
        }
        /// \ru Инициализировать по двумерным контурам в локальной системе координат. \en Initialize by two-dimensional contours in the local coordinate system. 
        template <class PlaneContoursVector>
        bool          InitPlaneContours( const MbPlacement3D & pl, const MbVector3D & dir, const PlaneContoursVector & items, bool same )
        {
          if ( items.size() > 0 ) {
            DeleteItems();
            place.Init( pl );
            direction.Init( dir );
            C3D_ASSERT( (direction.MaxFactor() < LENGTH_EPSILON) || !direction.Orthogonal( place.GetAxisZ(), ANGLE_EPSILON ) );
            sense = orient_BOTH;

            ::AddRefItems( items, same, sketchContours );
            return true;
          }
          return false;
        }
        /// \ru Инициализировать по пространственным кривым. \en Initialize by spatial curves. 
        template <class SpaceCurvesVector>
        bool          InitSpaceCurves( const SpaceCurvesVector & items, bool same )
        {
          if ( items.size() > 0 ) {
            DeleteItems();

            ::AddRefItems( items, same, spaceCurves );
            return true;
          }
          return false;
        }
        /// \ru Инициализировать по поверхностям. \en Initialize by surfaces. 
        template <class SurfacesVector>
        bool          InitSurfaces( const SurfacesVector & items, bool same )
        {
          if ( items.size() > 0 ) {
            DeleteItems();

            ::AddRefItems( items, same, surfaces );
            return true;
          }
          return false;
        }
        /// \ru Инициализировать по телу. \en Initialize by a solid. 
        bool          InitSolid( const MbSolid & solid, bool same, bool keepShell );
        /// \ru Инициализировать по построителям тела. \en Initialize by solid creators. 
        template <class CreatorsVector>
        bool          InitSolid( const CreatorsVector & solidCreators, bool sameCreators )
        {
          DeleteItems();
          size_t creatorsCnt = solidCreators.size();
          if ( creatorsCnt > 0 ) {
            MbRegDuplicate * iReg = nullptr;
            MbAutoRegDuplicate autoReg( iReg );
            SPtr<MbCreator> creator;
            creators.reserve( creatorsCnt );
            for ( size_t k = 0; k < creatorsCnt; ++k ) {
              if ( solidCreators[k] != nullptr ) {
                creator = sameCreators ? &const_cast<MbCreator &>( *solidCreators[k] ) : static_cast<MbCreator *>( &solidCreators[k]->Duplicate( iReg ) );
                creators.push_back( creator );
                ::DetachItem( creator );
              }
            }
            if ( creators.size() > 0 )
              return true;
          }
          return false;
        }
        /// \ru Сделать равным. \en Make equal. 
        bool          SetEqual ( const MbSplitData & );
        /// \ru Являются ли объекты подобными. \en Determine whether the objects are similar. 
        bool          IsSimilar( const MbSplitData & ) const;
        /// \ru Преобразовать элемент согласно матрице. \en Transform element according to the matrix. 
        void          Transform( const MbMatrix3D &, MbRegTransform * = nullptr );
        /// \ru Сдвинуть по вектору. \en Shift by  a vector. 
        void          Move     ( const MbVector3D &, MbRegTransform * = nullptr );
        /// \ru Повернуть вокруг оси. \en Rotate about an axis. 
        void          Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr );
        /// \ru Отсутствуют ли объекты? \en Are the objects absent? 
        bool          IsEmpty() const { 
                        return ( sketchContours.empty() && 
                                 spaceCurves.empty() && 
                                 surfaces.empty() && 
                                 (creators.empty() && (solidShell == nullptr)) &&
                                 wfCreators.empty() ); }
        /// \ru Являются ли объекты равными? \en Determine whether an object is equal? 
        bool          IsSame( const MbSplitData &, double accuracy ) const; 

  /** \ru \name Доступ к эскизу.
      \en \name Access to a sketch.
      \{ */
        /// \ru Выдать количество двумерных кривых. \en Get number of two-dimensional curves. 
        size_t          GetSketchCurvesCount() const { return sketchContours.size(); }
        /// \ru Получить локальную систему координат двумерных кривых. \en Get the local coordinate system of two-dimensional curves. 
  const MbPlacement3D & GetSketchPlace() const { return place; }
        /// \ru Получить локальную систему координат двумерных кривых. \en Get the local coordinate system of two-dimensional curves. 
        MbPlacement3D & SetSketchPlace()       { return place; }
        /// \ru Получить вектор направления выдавливания двумерных кривых. \en Get the extrusion direction vector of two-dimensional curves. 
  const MbVector3D &    GetSketchDirection() const { return direction; }
        /// \ru Получить вектор направления выдавливания двумерных кривых. \en Get the extrusion direction vector of two-dimensional curves. 
        MbVector3D &    SetSketchDirection()       { return direction; }
        /// \ru Выдать направление выдавливания двумерных кривых. \en Get extrusion direction of two-dimensional curves. 
  const MbeSenseValue   GetSketchSense() const { return sense; }
        /// \ru Выдать направление выдавливания двумерных кривых. \en Get extrusion direction of two-dimensional curves. 
        MbeSenseValue & SetSketchSense()       { return sense; }
        /// \ru Установить направление выдавливания двумерных кривых. \en Set extrusion direction of two-dimensional curves. 
        void            SetSketchSense( MbeSenseValue zdir ) { sense = zdir; }
        /// \ru Получить двумерную кривую по индексу. \en Get two-dimensional curve by index. 
  const MbContour   *   GetSketchCurve( size_t k ) const { return ((k < sketchContours.size()) ? sketchContours[k].get() : nullptr ); }
        /// \ru Получить двумерную кривую по индексу. \en Get two-dimensional curve by index. 
        MbContour   *   SetSketchCurve( size_t k )       { return ((k < sketchContours.size()) ? sketchContours[k].get() : nullptr ); }
        /// \ru Получить все двумерные кривые. \en Get all two-dimensional curves. 
        template <class PlaneContoursVector>
        void            GetSketchCurves( PlaneContoursVector & curvs ) const
        {
          curvs.reserve( curvs.size() + sketchContours.size() );
          c3d::PlaneContourSPtr sketchContour;
          for ( size_t k = 0, addCnt = sketchContours.size(); k < addCnt; ++k ) {
            sketchContour = const_cast<MbContour *>( sketchContours[k].get() );
            curvs.push_back( sketchContour );
          }
        }
        /// \ru Удалить двумерную кривую по индексу. \en Delete two-dimensional curve by index. 
        bool            DeleteSketchCurve( size_t k );

  /** \} */
  /** \ru \name Доступ к пространственным кривым.
      \en \name Access to spatial curves.
      \{ */
        /// \ru Выдать количество пространственных кривых. \en Get number of spatial curves. 
        size_t        GetSpaceCurvesCount() const { return spaceCurves.size(); }
        /// \ru Получить пространственную кривую по индексу. \en Get a spatial curve by index. 
  const MbCurve3D   * GetSpaceCurve( size_t k ) const { return ((k < spaceCurves.size()) ? spaceCurves[k].get() : nullptr ); }
        /// \ru Получить пространственную кривую по индексу. \en Get a spatial curve by index. 
        MbCurve3D   * SetSpaceCurve( size_t k )       { return ((k < spaceCurves.size()) ? spaceCurves[k].get() : nullptr ); }
        /// \ru Получить все пространственные кривые. \en Get all spatial curves. 
        template <class SpaceCurvesVector>
        void          GetSpaceCurves( SpaceCurvesVector & curvs ) const
        {
          curvs.reserve( curvs.size() + spaceCurves.size() );
          c3d::SpaceCurveSPtr spaceCurve;
          for ( size_t k = 0, addCnt = spaceCurves.size(); k < addCnt; ++k ) {
            spaceCurve = const_cast<MbCurve3D *>( spaceCurves[k].get() );
            curvs.push_back( spaceCurve );
          }
        }
        /// \ru Установить пространственную кривую по индексу. \en Set spatial curve by index. 
        bool          SetSpaceCurve( const MbCurve3D & curve, size_t k );

  /** \} */
  /** \ru \name Доступ к поверхностям.
      \en \name Access to surfaces.
      \{ */
        /// \ru Выдать количество поверхностей. \en Get number of surfaces. 
        size_t        GetSurfacesCount() const { return surfaces.size(); }
        /// \ru Получить поверхность по индексу. \en Get a surface by index. 
  const MbSurface   * GetSurface( size_t k ) const { return ((k < surfaces.size()) ? surfaces[k].get() : nullptr); }
        /// \ru Получить поверхность по индексу. \en Get a surface by index. 
        MbSurface   * SetSurface( size_t k )       { return ((k < surfaces.size()) ? surfaces[k].get() : nullptr); }
        /// \ru Получить все поверхности. \en Get all surfaces. 
        template <class SurfacesVector>
        void          GetSurfaces( SurfacesVector & surfs ) const
        {
          surfs.reserve( surfs.size() + surfaces.size() );
          c3d::SurfaceSPtr surface;
          for ( size_t k = 0, addCnt = surfaces.size(); k < addCnt; ++k ) {
            surface = const_cast<MbSurface *>( surfaces[k].get() );
            surfs.push_back( surface );
          }
        }
        /// \ru Установить поверхность по индексу. \en Set a surface by index. 
        bool          SetSurface( const MbSurface & surface, size_t k );

  /** \} */
  /** \ru \name Доступ к строителям.
      \en \name Access to creators.
      \{ */
        /// \ru Выдать количество строителей тела. \en Get number of solid creators. 
        size_t        GetCreatorsCount() const { return creators.size(); }
        /// \ru Получить строитель по индексу. \en Get constructor by index. 
  const MbCreator   * GetCreator( size_t k ) const { return ((k < creators.size()) ? creators[k].get() : nullptr ); }
        /// \ru Получить строитель по индексу. \en Get constructor by index. 
        MbCreator   * SetCreator( size_t k )       { return ((k < creators.size()) ? creators[k].get() : nullptr ); }
        /// \ru Получить все строители. \en Get all creators. 
        template <class CreatorsVector>
        void          GetCreators( CreatorsVector & crs ) const
        {
          crs.reserve( crs.size() + creators.size() );
          c3d::ConstCreatorSPtr creator;
          for ( size_t k = 0, addCnt = creators.size(); k < addCnt; ++k ) {
            creator = creators[k];
            crs.push_back( creator );
            ::DetachItem( creator );
          }
        }
        /// \ru Получить все строители. \en Get all creators. 
        template <class CreatorsVector>
        void          GetCreatorsCopies( CreatorsVector & crs ) const
        {
          MbRegDuplicate * iReg = nullptr;
          MbAutoRegDuplicate autoReg( iReg );

          crs.reserve( crs.size() + creators.size() );
          c3d::CreatorSPtr creator;
          for ( size_t k = 0, addCnt = creators.size(); k < addCnt; ++k ) {
            if ( creators[k] != nullptr )
              creator = static_cast<MbCreator *>( &creators[k]->Duplicate( iReg ) );
            crs.push_back( creator );
            ::DetachItem( creator );
            creator = nullptr;
          }
        }
        /// \ru Получить все строители. \en Get all creators. 
        template <class CreatorsVector>
        void          SetCreators( CreatorsVector & crs )
        {
          crs.reserve( crs.size() + creators.size() );
          c3d::CreatorSPtr creator;
          for ( size_t k = 0, addCnt = creators.size(); k < addCnt; ++k ) {
            creator = creators[k];
            crs.push_back( creator );
            ::DetachItem( creator );
          }
        }
        /// \ru Получить хранимую оболочку. \en Get stored shell.
  const MbFaceShell * GetSolidShell() const { return solidShell; }
        /// \ru Создать оболочку по строителям (solidShell остается нетронутой). \en Create a shell by creators (solidShell remains unchanged). 
        MbFaceShell * CreateShell( MbeCopyMode copyMode );
        /// \ru Создать оболочку по строителям. \en Create a shell by creators (solidShell remains unchanged). 
        bool          UpdateShell( MbeCopyMode copyMode );
        /// \ru Выдать количество строителей тела. \en Get number of wire frame creators. 
        size_t        GetWFrameCreatorsCount() const { return wfCreators.size(); }
        /// \ru Получить строитель каркаса по индексу. \en Get wire frame constructor by index. 
  const MbCreator   * GetWireFrameCreator( size_t k ) const { return ((k < wfCreators.size()) ? wfCreators[k].get() : nullptr ); }
        /// \ru Получить строитель каркаса по индексу. \en Get wire frame constructor by index. 
        MbCreator   * SetWireFrameCreator( size_t k )       { return ((k < wfCreators.size()) ? wfCreators[k].get() : nullptr ); }
        /// \ru Создать каркас по строителям. \en Create a wire frame by creators.
        MbWireFrame * CreateWireFrame( MbeCopyMode copyMode );
        /// \ru Удалить данные. \en Delete data. 
        void          DeleteItems();
        /// \ru Прочитать данные. \en Read data. 
        void          ReadItems ( reader & );
        /// \ru Записать данные. \en Write data. 
        void          WriteItems( writer & ) const;
        /// \ru Выдать свойства объекта. \en Get properties of the object. 
        void          GetProperties( MbProperties & );
        /// \ru Записать свойства объекта. \en Set properties of the object. 
        void          SetProperties( const MbProperties & );
        /// \ru Дать базовые объекты. \en Get the base objects. 
        void          GetBasisItems ( RPArray<MbSpaceItem> & );
        /// \ru Выдать контрольные точки объекта. \en Get control points of object.
        void          GetBasisPoints( MbControlData3D & ) const;
        /// \ru Изменить объект по контрольным точкам. \en Change the object by control points.
        void          SetBasisPoints( const MbControlData3D & );
  /** \} */
OBVIOUS_PRIVATE_COPY( MbSplitData )
};


#endif // __CR_SPLIT_DATA_H
