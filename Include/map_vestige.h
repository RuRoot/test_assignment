////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Следы трехмерных объектов.
         \en Vestiges of three-dimensional objects. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MAP_VESTIGE_H
#define __MAP_VESTIGE_H


#include <templ_p_array.h>
#include <templ_pointer.h>
#include <templ_sptr.h>
#include <mb_cart_point.h>
#include <mb_matrix.h>
#include <curve.h>
#include <topology_item.h>
#include <mesh_primitive.h>
#include <algorithm>
#include <iterator>
#include <vector>

class MbEdgeVestige;
class MbVertexVestige;


//------------------------------------------------------------------------------     
/** \brief Тип пространственной геометрии кривой.
  \details Тип пространственной геометрии кривой. \n  
  \ingroup Mapping
*/ 
enum MbMapSpaceCurveType {
  mst_Unset = 0,    ///< тип геометрии кривой неопределен
  mst_Degenerate,   ///< кривая в проекции вырождается в точку
  mst_Line,         ///< кривая в проекции вырождается в линию
  mst_Circle,       ///< кривая в проекции вырождается в окружность
  mst_Ellipse,      ///< кривая в проекции вырождается в эллипс
  mst_Arbitrary,    ///< произвольный тип кривой
};
// ---

//------------------------------------------------------------------------------     
/** \brief \ru След трехмерного объекта.
           \en The vestige of three-dimensional object. \~
  \details \ru Базовый класс для классов следа трехмерного объекта.\n
           \en The base class for classes of three-dimensional object vestige.\n \~
  \ingroup Mapping
*/ 
// ---
class MATH_CLASS MbBaseVestige : public TapeBase {     
protected:
  uint                      comp;       ///< \ru Компонент. \en A component. 
  size_t                    ident;      ///< \ru Идентификатор нити. \en A thread identifier. 
  uint16                    style;      ///< \ru Базовый стиль. \en A basis style.  \~ \internal \ru По просьбе группы 3D (Компас) \en 3D (Kompas) at request. \~ \endinternal
  MbAttributeContainer      attrData;   ///< \ru Атрибуты. \en Attributes.  \~ \internal \ru По просьбе группы Приложений (Компас) \en Apps (Kompas) at request. \~ \endinternal
protected:
  const MbTopologyItem *    item;       ///< \ru Топологический объект(используется только как временный внутри проецирования). \en Topological object (is used only as a temporary object in projection). 
  TOwnPointer<const MbName> name;       ///< \ru Имя. \en A name. 

public:
  
  /** \brief \ru Классификация плоского отображения.
             \en The classification of a planar mapping. \~
    \details \ru Классификация плоского отображения. \n
             \en The classification of a planar mapping. \n \~  
  */
  enum Type {        
    vt_None,            ///< \ru Тип не определён. \en A type is undefined. 
    vt_SmoothEdge,      ///< \ru Линия перехода (гладкое ребро). \en A transition line (smooth edge). 
    vt_Edge,            ///< \ru Отображение ребра или линия очерка поверхности. \en Mapping of edge or isocline curve of surface. 
    vt_SectionLine,     ///< \ru Линия разреза (ребра тела, полученные сечением). \en A cutaway line (section edges). 
    vt_AnnThreadThin,   ///< \ru Аннотационный объект резьба тонкая. \en An annotative object - a thread is thin. 
    vt_AnnThreadThick,  ///< \ru Аннотационный объект резьба толстая. \en An annotative object - a thread is thick. 
    vt_AnnThreadDashed, ///< \ru Аннотационный объект резьба штриховая. \en An annotative object - a thread is dashed. 
    vt_BoundLeft,       ///< \ru Левая граница вида. \en The left boundary of a view. 
    vt_BoundRight,      ///< \ru Правая граница вида. \en The right boundary of a view. 
    vt_Vertex,          ///< \ru Вершина. \en A vertex. 
    vt_SpacePoint,      ///< \ru Пространственная точка. \en A spatial point. 
    vt_SpaceCurve,      ///< \ru Пространственная кривая. \en A spatial curve. 
    vt_CenterLine,      ///< \ru Осевая (центральная) линия. \en A center line. 
    vt_MeshEdge,        ///< \ru Отображение ребра или линия очерка полигонального объекта. \en Mapping of edge or isocline curve of polygonal object. 
  };

  /** \brief \ru Классификатор подтипов аннотационных ребер.
             \en The classifier of annotative edges subtypes. \~
    \details \ru Классификатор подтипов аннотационных ребер. \n
             \en The classifier of annotative edges subtypes. \n \~  
  */
  enum SubType { 
    vst_None = 0, ///< \ru Подтип не определен. \en A subtype is undefined. 
    vst_BaseBeg,  ///< \ru Основной начальный. \en A base initial type. 
    vst_BaseEnd,  ///< \ru Основной конечный. \en A base final type. 
    vst_ButtBeg,  ///< \ru Торцевой начальный. \en A butt initial type. 
    vst_ButtEnd,  ///< \ru Торцевой конечный. \en A butt final type. 
    vst_CLAxis,   ///< \ru Прямолинейная ось. \en A straight axis type. 
    vst_CLPath,   ///< \ru Криволинейная траектория. \en A curved path type. 
  };

protected:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор для топологического объекта.\n
             \en Constructor for a topological object.\n \~    
    \param[in] otherComp  - \ru Компонент.
                            \en A component. \~ 
    \param[in] otherIdent - \ru Идентификатор нити.
                            \en A thread identifier. \~
    \param[in] otherItem  - \ru Топологический объект.
                            \en A topological object. \~
  */
  MbBaseVestige( uint otherComp, size_t otherIdent, const MbTopologyItem & otherItem ) 
    : comp     ( otherComp            )
    , ident    ( otherIdent           )
    , style    ( SYS_MAX_UINT16       )
    , attrData (                      )
    , item     ( &otherItem           )
    , name     ( &otherItem.GetName() )
  { name.SetOwn( false ); }
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор для аннотационного объекта.\n
             \en Constructor for an annotative object.\n \~    
    \param[in] otherComp  - \ru Компонент.
                            \en A component. \~ 
    \param[in] otherIdent - \ru Идентификатор нити.
                            \en A thread identifier. \~
    \param[in] otherName  - \ru Имя топологического объекта.
                            \en A name of a topological object. \~
    \param[in] otherItem  - \ru Топологический объект.
                            \en A topological object. \~
  */
  MbBaseVestige( uint otherComp, size_t otherIdent, const MbName & otherName, const MbTopologyItem * otherItem ) 
    : comp     ( otherComp      )
    , ident    ( otherIdent     )
    , style    ( SYS_MAX_UINT16 )
    , attrData (                )
    , item     ( otherItem      )
    , name     ( &otherName     )
  { name.SetOwn( false ); }
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор для пространственной точки или кривой.\n
             \en Constructor for a spatial point or curve.\n \~    
    \param[in] otherComp  - \ru Компонент.
                            \en A component. \~ 
    \param[in] otherIdent - \ru Идентификатор нити.
                            \en A thread identifier. \~
    \param[in] otherName  - \ru Имя топологического объекта.
                            \en A name of a topological object. \~
  */
  MbBaseVestige( uint otherComp, size_t otherIdent, const MbName & otherName ) 
    : comp     ( otherComp      )
    , ident    ( otherIdent     )
    , style    ( SYS_MAX_UINT16 )
    , attrData (                )
    , item     ( nullptr           )
    , name     ( &otherName     )
  { name.SetOwn( false ); }
  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  MbBaseVestige( const MbBaseVestige & other, MbRegDuplicate * iReg );
  /// \ru Конструктор. \en Constructor. 
  MbBaseVestige()
    : comp    ( 0              )
    , ident   ( SYS_MAX_T      )
    , style   ( SYS_MAX_UINT16 )
    , item    ( nullptr           )
    , name    ( nullptr           )
  { name.SetOwn(false); }
  virtual ~MbBaseVestige() {}

public:
  virtual MbBaseVestige & Duplicate( MbRegDuplicate * iReg = nullptr ) const; ///< \ru Создать копию объекта. \en Create a copy of the object.
public:
        uint                    GetComponent() const { return comp; }
        size_t                  GetIdentifier() const { return ident; }
        uint16                  GetStyle() const { return style; }
  const MbAttributeContainer &  GetAttributes() const { return attrData; }

        void                    CopyIdData( const MbBaseVestige & obj ) { comp = obj.comp; ident = obj.ident; }

        void                    SetProperties ( uint16 st                                  ) { style = st; }
        void                    SetProperties ( uint16 st, const MbAttributeContainer & ac ) { style = st; attrData.AttributesAssign( ac ); }
        void                    CopyProperties( const MbBaseVestige & obj )                  { style = obj.style; attrData.AttributesAssign( obj.attrData ); }
        
public:
  const MbTopologyItem *        GetItem() const { return item; }                 ///< \ru Топологический объект. \en A topological object. 
  const MbName *                GetVestigeName() const { return name; }          ///< \ru Имя. \en A name.   

private:
  bool operator == ( const MbBaseVestige & ); // \ru Не реализован. \en Not implemented. 
  bool operator != ( const MbBaseVestige & ); // \ru Не реализован. \en Not implemented. 

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbBaseVestige )
OBVIOUS_PRIVATE_COPY( MbBaseVestige )
};
  
IMPL_PERSISTENT_OPS( MbBaseVestige )

//------------------------------------------------------------------------------     
/** \brief \ru След вершины.
           \en The vestige of a vertex. \~
  \details \ru След вершины.\n
           \en The vestige of a vertex.\n \~
  \ingroup Mapping
*/ 
// --- 
class MATH_CLASS MbVertexVestige : public MbBaseVestige {
  friend struct MbVEFVestiges;
protected:
  MbCartPoint point;    ///< \ru Проекция вершины. \en A vertex projection. 
  bool        bvisible; ///< \ru Флаг видимости. \en A visibility flag. 
private:
  uint8       vesType;  ///< \ru Тип вершины. \en A vertex type. 

protected:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор вершины.\n
             \en Constructor of a vertex.\n \~    
    \param[in] otherComp  - \ru Компонент.
                            \en A component. \~ 
    \param[in] otherIdent - \ru Идентификатор нити.
                            \en A thread identifier. \~
    \param[in] otherItem  - \ru Топологический объект.
                            \en A topological object. \~
    \param[in] vis        - \ru Флаг видимости.
                            \en A visibility flag. \~
  */
  MbVertexVestige( uint otherComp, size_t otherIdent, const MbTopologyItem & otherItem, bool vis )
    : MbBaseVestige( otherComp, otherIdent, otherItem )
    , point   (           )
    , bvisible( vis       )
    , vesType ( vt_Vertex )
  {}
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор пространственной точки.\n
             \en Constructor of a spatial point.\n \~    
    \param[in] otherComp  - \ru Компонент.
                            \en A component. \~ 
    \param[in] otherIdent - \ru Идентификатор нити.
                            \en A thread identifier. \~
    \param[in] otherName  - \ru Имя топологического объекта.
                            \en A name of a topological object. \~
    \param[in] vis        - \ru Флаг видимости.
                            \en A visibility flag. \~
  */
  MbVertexVestige( uint otherComp, size_t otherIdent, const MbName & otherName, bool vis, bool isDegenerateCurve )
    : MbBaseVestige( otherComp, otherIdent, otherName )
    , point   (                                                                 )
    , bvisible( vis                                                             )
    , vesType ( isDegenerateCurve ? (uint8)vt_SpaceCurve : (uint8)vt_SpacePoint )
  {}
  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  MbVertexVestige( const MbVertexVestige & other, MbRegDuplicate * iReg )
    : MbBaseVestige( other, iReg    )
    , point        ( other.point    ) 
    , bvisible     ( other.bvisible ) 
    , vesType      ( other.vesType  ) 
  {}
  /// \ru Конструктор. \en Constructor. 
  MbVertexVestige()
    : MbBaseVestige()
    , point   (         )
    , bvisible( true    )
    , vesType ( vt_None )
  {}
public:
  /// \ru Создать копию объекта. \en Create a copy of the object. 
  MbBaseVestige & Duplicate( MbRegDuplicate * iReg = nullptr ) const override;
public:
  /// \ru Тип отображения. \en Mapping type. 
        Type              GetType() const { return (Type)vesType; }
  /// \ru Это видимая точка? \en Is point visible? 
        bool              IsVisible() const { return bvisible; }
  /// \ru Получить точку. \en Get the point. 
  const MbCartPoint &     GetPoint() const { return point; }
  /// \ru Преобразовать точку по матрице. \en Transform the point. 
        void              TransformPoint( const MbMatrix & mtr ) { point.Transform( mtr ); }

private:
  bool operator == ( const MbVertexVestige & ); // \ru Не реализован. \en Not implemented. 
  bool operator != ( const MbVertexVestige & ); // \ru Не реализован. \en Not implemented. 

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbVertexVestige )
OBVIOUS_PRIVATE_COPY( MbVertexVestige )
};

IMPL_PERSISTENT_OPS( MbVertexVestige )

struct MbCurveVestige;
void ReplaceCurveVestigeDuplicates( MbCurveVestige & );

//------------------------------------------------------------------------------     
/** \brief \ru Информация о следе кривой.
           \en The information about a curve vestige. \~
  \details \ru Информация о следе кривой.\n 
    Для вложения в след ребра (MbVestigeEdge) и в след грани (MbVestigeEdge).\n
           \en The information about a curve vestige.\n 
    For including to the edge vestige (MbVestigeEdge) and face vestige (MbVestigeEdge).\n \~
  \ingroup Mapping
*/ 
// --- 
struct MATH_CLASS MbCurveVestige : public TapeBase {
protected:
  SPtr<MbCurve>               totalPrj;  ///< \ru Полная проекция (может быть nullptr). \en A full projection (can be nullptr). \~  \internal \ru Владеет. \en Owns. \~    \endinternal
  std::vector<MbCurve *>      arTotal;   ///< \ru Все проекции в упорядоченной форме. \en All projections in an ordered form. \~ \internal \ru Не владеет. \en Doesn't own. \~ \endinternal                                       

  // \ru Двумерные кривые лежат копиями, поэтому массивы владеющие. \en Two-dimensional uv-curves are copies therefore arrays are owners 
  // \ru Если кривых нет то указатель останется нулевым. \en If no curves then the pointer remains nullptr.
  TPointer< PArray<MbCurve> > arVisPrj; ///< \ru Видимые проекции. \en Visible projections. 
  TPointer< PArray<MbCurve> > arHidPrj; ///< \ru Не видимые проекции. \en Invisible projections. 

public:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по умолчанию.\n        
      Создает объект с нулевой проекцией.
             \en Default constructor.\n        
      Creates an object with the null projection. \~ 
  */
  MbCurveVestige()
    : totalPrj( nullptr )
    , arTotal (      )
    , arVisPrj( nullptr )
    , arHidPrj( nullptr )
  {}
  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  MbCurveVestige( const MbCurveVestige & other, MbRegDuplicate * iReg ); 
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbCurveVestige() { ClearAll(); }
public:
  /// \ru Создать копию объекта. \en Create a copy of the object. 
  virtual MbCurveVestige & Duplicate( MbRegDuplicate * iReg = nullptr ) const;

public:
          /** \brief \ru Очистить проекции.
                     \en Clear projections. \~
            \details \ru Очистить проекции.\n        
                         Очищает список проекций в arTotal, 
                         обнуляет указатели totalPrj, arVisPrj, arHidPrj.
                     \en Clear projections.\n        
                         Clears the list of projections in the arToral, 
                         resets the totalPrj, arVisPrj, arHidPrj. \~ 
          */
          void        ClearAll()
          {
            arTotal.clear();
            totalPrj = nullptr;
            arVisPrj = nullptr;
            arHidPrj = nullptr;
          }
          /// \ru Пустое ли отображение кривой? \en Is an empty curve vestige? 
          bool        IsEmpty() const
          {
            return ( totalPrj == nullptr )               &&
                   ( arTotal.size() < 1 )             && 
                   ( !arVisPrj || arVisPrj->empty() ) && 
                   ( !arHidPrj || arHidPrj->empty() );
          }
          /// \ru Количество видимых частей проекции. \en The number of visible parts of the projection. 
          size_t      GetVisiblePartsCount() const { return ( ( !!arVisPrj ) ? arVisPrj->size() : 0 ); }
          /// \ru Количество невидимых частей проекции. \en The number of hidden parts of the projection. 
          size_t      GetHiddenPartsCount () const { return ( ( !!arHidPrj ) ? arHidPrj->size() : 0 ); }
          /// \ru Количество всех частей проекции. \en The number of all parts of the projection. 
          size_t      GetAllPartsCount    () const { return arTotal.size(); }

          /// \ru Создан ли массив для видимых частей проекции. \en Is the array for visible parts of the projection created? 
          bool        IsVisibleCurvesArray() const { return !!arVisPrj; }
          /// \ru Создан ли массив для невидимых частей проекции. \en Is the array for hidden parts of the projection created?
          bool        IsHiddenCurvesArray () const { return !!arHidPrj; }

          /// \ru Получить видимую часть проекции. \en Get visible part of projection. 
  const   MbCurve *  _GetVisibleCurve( size_t k ) const { return (( !!arVisPrj ) ? (*arVisPrj)[k] : nullptr); }
          /// \ru Получить невидимую часть проекции. \en Get hidden part of projection. 
  const   MbCurve *  _GetHiddenCurve ( size_t k ) const { return (( !!arHidPrj ) ? (*arHidPrj)[k] : nullptr); }

          /// \ru Положить в массив указатели видимых частей проекции. \en Put pointers of visible parts of projection into the array. 
          template <class Curves>
          void        GetVisibleCurves( Curves & dst ) const;
          /// \ru Положить в массив указатели невидимых частей проекции. \en Put pointers of hidden parts of projection into the array. 
          template <class Curves>
          void        GetHiddenCurves ( Curves & dst ) const;

          /// \ru Добавить свой габарит в присланный габарит. \en Add your own bounding box into the sent bounding box. 
          void        AddYourGabaritTo( MbRect & ) const;
          /// \ru Преобразование согласно матрице. \en The transformation according to a matrix. 
          void        Transform( const MbMatrix & );

          /** \brief \ru Добавить часть проекции.
                     \en Add a part of projection. \~
            \details \ru Добавить новую часть общей проекции, имеющий признак видимости.\n
                     \en Add a new part of the general projection which has a visibility attribute.\n \~        
            \param[in] segment - \ru Часть проекции.
                                 \en A projection part. \~
            \param[in] visible - \ru Признак видимости.
                                 \en A visibility attribute. \~
          */
          void        AddSegment( MbCurve & segment, bool visible );
          /** \brief \ru Добавить копию часть проекции.
                     \en Add copy of a part of projection. \~
            \details \ru Добавить копию новой части общей проекции, имеющей признак видимости.\n
                     \en Add copy of a new part of the general projection which has a visibility attribute.\n \~        
            \param[in] segment - \ru Часть проекции.
                                 \en A projection part. \~
            \param[in] visible - \ru Признак видимости.
                                 \en A visibility attribute. \~
          */
          void        AddSegmentCopy( const MbCurve & segment, bool visible );

          /** \brief \ru Забрать все проекционные кривые из структуры.
                     \en Pick up all curves. \~
            \details \ru Забрать все проекционные кривые из структуры и очистить ее.
                     \en Pick up all curves of this structure and clear it. \~       
          */
          bool        PickUpMapCurves( RPArray<MbCurve> & crvArr, c3d::BoolVector & visArr );
          /// \ru Забрать видимую часть проекции (не обнуляет в массиве всех проекций). \en Pick up visible part of projection (it doesn't set zero in all projections array). 
          MbCurve *  _PickupVisibleCurve( size_t );
          /// \ru Забрать невидимую часть проекции (не обнуляет в массиве всех проекций). \en Pick up hidden part of projection (it doesn't set zero in all projections array). 
          MbCurve *  _PickupHiddenCurve ( size_t );
          /// \ru Поглотить данные структуры и очистить ее. \en Absorb data of this structure and clear it. 
          bool        EatupOther( MbCurveVestige & );
          /// \ru Отцепить все кривые из структуры и очистить ее. \en Detach all curves of this structure and clear it. 
          void        DetachAllCurves( PArray<MbCurve> *& visCurves, PArray<MbCurve> *& hidCurves, SPtr<MbCurve> & totalPrj );

          /// \ru Получение полной проекции. \en Merge total projection. 
  const   MbCurve *   MergeTotalMap ( MbMapSpaceCurveType spaceCurveGeomType );
          /// \ru Обновление полной проекции. \en Update total projection. 
  const   MbCurve *   UpdateTotalMap( MbMapSpaceCurveType spaceCurveGeomType );

          /// \ru Починить одиночное соответствие полной проекции и ее частями. \en Repair correspondence between total projection and parts of the projection.
          bool        RepairSpecificCorrespondence( bool uncertainIsVisible );

          /// \ru Есть ли указатель на полную проекцию? \en Is there a pointer to the full projection? 
          bool        IsTotalProjection() const { return (totalPrj != nullptr); }
          /// \ru Указатель на полную проекцию. \en The pointer to a full projection. 
          MbCurve *   DetachTotalProjection() { return ::DetachItem( totalPrj ); }
          /// \ru Установить полную проекцию. \en Set a full projection. 
          void        SetTotalProjection( MbCurve & ); 

          /// \ru Указатель на полную проекцию. \en The pointer to a full projection. 
  const   MbCurve *   GetFullProjection() const;
          /// \ru Обнулить указатель на полную проекцию. \en Set to null the pointer to a full projection. 
          MbCurve *   DetachFullProjection();                            

  friend  void        ReplaceCurveVestigeDuplicates( MbCurveVestige & );

private:
          bool        operator == ( const MbCurveVestige & ); // \ru Не реализован. \en Not implemented. 
          bool        operator != ( const MbCurveVestige & ); // \ru Не реализован. \en Not implemented. 

DECLARE_PERSISTENT_CLASS_NEW_DEL ( MbCurveVestige )
OBVIOUS_PRIVATE_COPY( MbCurveVestige )
};

IMPL_PERSISTENT_OPS( MbCurveVestige )

//------------------------------------------------------------------------------
// указатель на полную проекцию
// ---
inline
const MbCurve * MbCurveVestige::GetFullProjection() const
{
  C3D_ASSERT( arTotal.size() == GetVisiblePartsCount() + GetHiddenPartsCount() );

  MbCurve * curve = totalPrj;

  if ( curve == nullptr ) {
    if ( !!arVisPrj && (arVisPrj->size() == 1) )
      curve = arVisPrj->operator[]( 0 );
    else if ( !!arHidPrj && (arHidPrj->size() == 1) )
      curve = arHidPrj->operator[]( 0 );
  }
  return curve;
}

//------------------------------------------------------------------------------
// \ru Забрать видимую часть проекции. \en Pick up visible part of projection. 
//---
inline MbCurve * MbCurveVestige::_PickupVisibleCurve( size_t k )
{ 
  if ( arVisPrj != nullptr ) {
    PArray<MbCurve> & crvs = *arVisPrj;
    MbCurve * crv = crvs[k];
    ::AddRefItem( crv ); // захват и отпускание на случай перехода на владение по счетчику ссылок
    crvs[k] = nullptr;
    ::DecRefItem( crv );
    return crv;
  }
  return nullptr;
}

//------------------------------------------------------------------------------
// \ru Забрать видимую часть проекции. \en Pick up visible part of projection. 
//---
inline MbCurve * MbCurveVestige::_PickupHiddenCurve( size_t k )
{ 
  if ( arHidPrj != nullptr ) {
    PArray<MbCurve> & crvs = *arHidPrj;
    MbCurve * crv = crvs[k];
    ::AddRefItem( crv ); // захват и отпускание на случай перехода на владение по счетчику ссылок
    crvs[k] = nullptr;
    ::DecRefItem( crv );
    return crv;
  }
  return nullptr;
}

//------------------------------------------------------------------------------
// \ru Отцепить все кривые из структуры и очистить ее. \en Detach all curves of this structure and clear it. 
//---
inline void MbCurveVestige::DetachAllCurves( PArray<MbCurve> *& visCurves, PArray<MbCurve> *& hidCurves, SPtr<MbCurve> & wholePrj )
{
  wholePrj = totalPrj;
  totalPrj = nullptr;

  arTotal.clear();
  visCurves = !!arVisPrj ? arVisPrj.Relinquish() : nullptr;
  hidCurves = !!arHidPrj ? arHidPrj.Relinquish() : nullptr;
}

//------------------------------------------------------------------------------
// \ru Положить в массив указатели видимых частей проекции. \en Put pointers of visible parts of projection into the array. 
//---
template <class Curves>
void MbCurveVestige::GetVisibleCurves( Curves & dst ) const
{ 
  if ( !!arVisPrj ) { 
    PArray<MbCurve> & src = *arVisPrj;
    dst.reserve( dst.size() + src.size() );
    std::copy( src.begin(), src.end(), std::back_inserter( dst ) );
  }
}

//------------------------------------------------------------------------------
// \ru Положить в массив указатели невидимых частей проекции. \en Put pointers of hidden parts of projection into the array. 
//---
template <class Curves>
void MbCurveVestige::GetHiddenCurves( Curves & dst ) const
{
  if ( !!arHidPrj ) {
    PArray<MbCurve> & src = *arHidPrj;
    dst.reserve( dst.size() + src.size() );
    std::copy( src.begin(), src.end(), std::back_inserter( dst ) );
  }
}


//------------------------------------------------------------------------------     
/** \brief \ru След ребра или кривой.
           \en The vestige of an edge or a curve. \~
  \details \ru След ребра или кривой.
    Несет в себе информацию о следе одной кривой.\n
           \en The vestige of an edge or a curve.
    Carries the information about the vestige of one curve.\n \~
    \internal \ru МА -  На самом деле след не ребра, 
               а единичное отображение топологического объекта\n
               (как правило ребра)
              \en МА -  it is not an edge vestige, 
               it is unit mapping of a topological object\n
               (of edge usually) \~ \endinternal
  \ingroup Mapping
*/     
// ---      
class MATH_CLASS MbEdgeVestige : public MbBaseVestige {
  friend struct MbVEFVestiges;
public:
  /** \brief
      \deprecated \ru Поле будет объявлено приватным. Используйте для доступа методы GetCurveInfo() и SetCurveInfo().
                  \en The field will be declared private. Use methods GetCurveInfo() and SetCurveInfo() to access it.
  */
  MbCurveVestige curveInfo; ///< \ru Информация о следе кривой. \en The information about a curve vestige. 
private:
  uint8          vesType;    ///< \ru Тип отображения. \en A mapping type. 
  uint8          vesSubType; ///< \ru Подтип (для ветвления именования). \en A subtype for naming branching. 

protected:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор для ребра.\n
             \en Constructor for an edge.\n \~        
    \param[in] otherComp  - \ru Компонент.
                            \en A component. \~
    \param[in] otherIdent - \ru Идентификатор нити.
                            \en A thread identifier. \~
    \param[in] otherItem  - \ru Топологический объект.
                            \en A topological object. \~
    \param[in] isSmooth   - \ru Является ли ребро гладким или нет.
                            \en Is edge is smooth or not. \~
    \param[in] isSection  - \ru Является ли ребро линией разреза или нет.
                            \en Is edge is cutaway line or not. \~
  */
  explicit
  MbEdgeVestige( uint otherComp, size_t otherIdent, const MbTopologyItem & otherItem, bool isSmooth, bool isSection )
    : MbBaseVestige( otherComp, otherIdent, otherItem            )
    , curveInfo    (                                             )
    , vesType      ( (uint8)(isSmooth ? vt_SmoothEdge : vt_Edge) )
    , vesSubType   ( vst_None                                    )
  {
    if ( isSection )
      vesType = (uint8)vt_SectionLine;
  }
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор для пространственной точки или кривой.\n
             \en Constructor for a spatial point or curve.\n \~        
    \param[in] otherComp    - \ru Компонент.
                              \en A component. \~
    \param[in] otherIdent   - \ru Идентификатор нити.
                              \en A thread identifier. \~
    \param[in] otherName    - \ru Имя.
                              \en A name. \~
    \param[in] isCenterLine - \ru Является ли кривая осевой линией или нет.
                              \en Is curve is center line or not. \~
    \param[in] ownName -      \ru Является ли otherName собственным именем объекта.
                              \en Is otherName own object name or not. \~
  */
  explicit
  MbEdgeVestige( uint otherComp, size_t otherIdent, const MbName & otherName, bool isCenterLine, bool ownName = false )
    : MbBaseVestige( otherComp, otherIdent, otherName                      )
    , curveInfo    (                                                       )
    , vesType      ( (uint8)(isCenterLine ? vt_CenterLine : vt_SpaceCurve) )
    , vesSubType   ( vst_None                                              )
  {  if ( ownName ) name.SetOwn( ownName ); }
  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  MbEdgeVestige( const MbEdgeVestige & other, MbRegDuplicate * iReg )
    : MbBaseVestige( other, iReg           )
    , curveInfo    ( other.curveInfo, iReg ) 
    , vesType      ( other.vesType         )
    , vesSubType   ( other.vesSubType      )
  {}
  /// \ru Конструктор. \en Constructor. 
  MbEdgeVestige()
    : MbBaseVestige()
    , curveInfo()
    , vesType( vt_None )
    , vesSubType( vst_None )
  {}
public:
  MbBaseVestige & Duplicate( MbRegDuplicate * iReg = nullptr ) const override;
public:
  /// \ru Получить тип отображения. \en Get mapping type. 
        Type      GetType()    const { return (Type)vesType; }
  /// \ru Получить подтип отображения. \en Get mapping subtype. 
        SubType   GetSubType() const { return (SubType)vesSubType; }
  /// \ru Установить тип отображения. \en Set mapping type. 
        void      SetType( Type vt ) { vesType = (uint8)vt; }
  /// \ru Установить подтип отображения. \en Set mapping subtype. 
        void      SetSubType( SubType vt ) { vesSubType = (uint8)vt; }

  ///< \ru Получить информацию о следе кривой. \en Get the information about a curve vestige.
  const MbCurveVestige & GetCurveInfo() const { return curveInfo; }
  ///< \ru Получить информацию о следе кривой. \en Get the information about a curve vestige.
        MbCurveVestige & SetCurveInfo()       { return curveInfo; }

  /// \ru Добавить MbEdgeVestige в массив. \en Add the MbEdgeVestige to an array. \~
  friend MbEdgeVestige * AddVestigeCurve( uint otherComp, size_t otherIdent, const MbName & otherName, RPArray<MbEdgeVestige> & arr, bool isCenterLine, bool ownName );

private:
  bool operator == ( const MbEdgeVestige & ); // \ru Не реализован. \en Not implemented. 
  bool operator != ( const MbEdgeVestige & ); // \ru Не реализован. \en Not implemented. 

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbEdgeVestige )
OBVIOUS_PRIVATE_COPY( MbEdgeVestige )
};
IMPL_PERSISTENT_OPS( MbEdgeVestige )

//------------------------------------------------------------------------------     
/** \brief \ru Добавить MbEdgeVestige в массив.
           \en Add the MbEdgeVestige to an array. \~
  \details \ru Добавить MbEdgeVestige в массив через закрытые конструкторы. \n
           \en Add the MbEdgeVestige to an array by the private constructors. \n \~
  \param[in] otherComp    - \ru Компонент.
                            \en A component. \~
  \param[in] otherIdent   - \ru Идентификатор нити.
                            \en A thread identifier. \~
  \param[in] otherName    - \ru Имя топологического объекта.
                            \en A name of a topological object. \~
  \param[out] array       - \ru Массив ребер.
                            \en Array of edges. \~
  \param[in] isCenterLine - \ru Является ли ребро осевой линией или нет.
                            \en Is curve is center line or not. \~
  \param[in] ownName -      \ru Является ли otherName собственным именем объекта.
                            \en Is otherName own object name or not. \~
  \ingroup Mapping
*/
// ---          
inline MbEdgeVestige * AddVestigeCurve(       uint                     otherComp, 
                                              size_t                   otherIdent, 
                                        const MbName &                 otherName,
                                              RPArray<MbEdgeVestige> & arr, 
                                              bool                     isCenterLine,
                                              bool                     ownName )
{
  MbEdgeVestige * vestige = new MbEdgeVestige( otherComp, otherIdent, otherName, isCenterLine, ownName );
  if ( vestige ) {
    arr.Add( vestige );
  }
  return vestige;
}

          
//------------------------------------------------------------------------------     
/** \brief \ru След грани.
           \en The vestige of a face. \~
  \details \ru След грани.\n
    Несет в себе информацию о следах очерков этой грани.
           \en The vestige of a face.\n
    Carriers the information about the vestiges of this face outlines. \~
  \internal \ru МА -  На самом деле след не грани, 
                  а множественное отображение топологического объекта\n
                  (как правило грани или разбитого на части отображения ребра (при построении сечений) )
            \en МА -  it is not a vestige of a face, 
                  it is a multiple mapping of a topological object\n
                  (of a face or partitioned edge mapping usually) \~  \endinternal
  \ingroup Mapping
*/        
// ---          
class MATH_CLASS MbFaceVestige : public MbBaseVestige {
  friend struct MbVEFVestiges;
public:
  PArray<MbCurveVestige> curveInfos;  ///< \ru Информация о следах очерков этой грани. \en The information about vestiges of this face outlines. \~  \internal \ru Владеет. \en Owns. \~ \endinternal                                        
private:
  uint8                  vesType;     ///< \ru Тип отображения. \en A mapping type. 

protected:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор следа грани.\n
             \en Constructor of a face vestige.\n \~        
    \param[in] otherComp  - \ru Компонент.
                            \en A component. \~
    \param[in] otherIdent - \ru Идентификатор нити.
                            \en A thread identifier. \~
    \param[in] otherItem  - \ru Топологический объект.
                            \en A topological object. \~
  */
  MbFaceVestige( uint otherComp, size_t otherIdent, const MbTopologyItem & otherItem )
    : MbBaseVestige( otherComp, otherIdent, otherItem )
    , curveInfos   ( 0, 1, true )
    , vesType      ( vt_Edge    )
  {}
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор следа грани.\n
             \en Constructor of a face vestige.\n \~        
    \param[in] otherComp  - \ru Компонент.
                            \en A component. \~
    \param[in] otherIdent - \ru Идентификатор нити.
                            \en A thread identifier. \~
    \param[in] otherItem  - \ru Полигональный объект.
                            \en A polygonal object. \~
  */
  MbFaceVestige( uint otherComp, size_t otherIdent, const MbGrid & grid )
    : MbBaseVestige( otherComp, otherIdent, grid.GetName() )
    , curveInfos   ( 0, 1, true  )
    , vesType      ( vt_MeshEdge )
  {}
  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  MbFaceVestige( const MbFaceVestige & other, MbRegDuplicate * iReg );
public:
  MbBaseVestige & Duplicate( MbRegDuplicate * iReg = nullptr ) const override;
public:
  /// \ru Тип отображения. \en Mapping type. 
  Type      GetType() const { return (Type)vesType; }

private:
  bool operator == ( const MbFaceVestige & ); // \ru Не реализован. \en Not implemented. 
  bool operator != ( const MbFaceVestige & ); // \ru Не реализован. \en Not implemented. 

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbFaceVestige )
OBVIOUS_PRIVATE_COPY( MbFaceVestige )
};
IMPL_PERSISTENT_OPS( MbFaceVestige )


//------------------------------------------------------------------------------     
/** \brief \ru След аннотационного объекта.
           \en The vestige of an annotative object. \~
  \details \ru След аннотационного объекта.\n
           \en The vestige of an annotative object.\n \~
  \ingroup Mapping
*/ 
// ---  
class MATH_CLASS MbAnnotationEdgeVestige : public MbBaseVestige {
  friend struct MbVEFVestiges;
public:
  /** \brief
      \deprecated \ru Поле будет объявлено приватным. Используйте для доступа методы GetCurveInfo() и SetCurveInfo().
                  \en The field will be declared private. Use methods GetCurveInfo() and SetCurveInfo() to access it.
  */
  MbCurveVestige   curveInfo;  ///< \ru Информация о следе кривой. \en The information about a curve vestige. 
protected:
  uint8            vesType;    ///< \ru Тип отображения. \en A mapping type. 
  uint8            vesSubType; ///< \ru Подтип (для ветвления именования). \en A subtype for naming branching. 

protected:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор следа аннотационного объекта.\n
             \en Constructor of an annotative object vestige.\n \~        
    \param[in] otherComp   - \ru Компонент.
                             \en A component. \~
    \param[in] otherIdent  - \ru Идентификатор нити.
                             \en A thread identifier. \~
    \param[in] otherItem   - \ru Топологический объект.
                             \en A topological object. \~
    \param[in] otherName   - \ru Имя.
                             \en A name. \~
    \param[in] type        - \ru Тип аннотационных ребер.
                             \en A type of annotative edges. \~
    \param[in] subType     - \ru Подтип аннотационных ребер.
                             \en A subtype of annotative edges. \~
  */
  MbAnnotationEdgeVestige( uint otherComp, size_t otherIdent, const MbTopologyItem * otherItem, const MbName & otherName, 
                           Type type, SubType subType )
    : MbBaseVestige( otherComp, otherIdent, otherName, otherItem )
    , curveInfo    (                )
    , vesType      ( (uint8)type    )
    , vesSubType   ( (uint8)subType )
  {
  }
  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  MbAnnotationEdgeVestige( const MbAnnotationEdgeVestige & other, MbRegDuplicate * iReg );

public:
  MbBaseVestige & Duplicate( MbRegDuplicate * iReg = nullptr ) const override;

public:
        /// \ru Тип отображения. \en Mapping type. 
        Type        GetType()             const { return (Type)vesType;       }
        /// \ru Подтип аннотационных ребер. \en A subtype of annotative edges. 
        SubType     GetSubType()          const { return (SubType)vesSubType; }

  ///< \ru Получить информацию о следе кривой. \en Get the information about a curve vestige.
  const MbCurveVestige & GetCurveInfo() const { return curveInfo; }
  ///< \ru Получить информацию о следе кривой. \en Get the information about a curve vestige.
        MbCurveVestige & SetCurveInfo()       { return curveInfo; }

  /// \ru Добавить MbAnnotationEdgeVestige в массив. \en Add the MbAnnotationEdgeVestige to an array. \~
  friend MbAnnotationEdgeVestige *  AddVestigeAnnotationEdge( uint otherComp, size_t otherIdent, const MbTopologyItem * otherItem, 
                                                              const MbName & otherName, MbBaseVestige::Type type, MbBaseVestige::SubType subType,
                                                              RPArray<MbAnnotationEdgeVestige> & array );

private:
        bool        operator == ( const MbAnnotationEdgeVestige & ); // \ru Не реализован. \en Not implemented. 
        bool        operator != ( const MbAnnotationEdgeVestige & ); // \ru Не реализован. \en Not implemented. 

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbAnnotationEdgeVestige )
OBVIOUS_PRIVATE_COPY( MbAnnotationEdgeVestige )
};
IMPL_PERSISTENT_OPS( MbAnnotationEdgeVestige )

//------------------------------------------------------------------------------
/** \brief \ru Добавить MbAnnotationEdgeVestige в массив.
           \en Add the MbAnnotationEdgeVestige to an array. \~
  \details \ru Добавить MbAnnotationEdgeVestige в массив через закрытые конструкторы. \n
           \en Add the MbAnnotationEdgeVestige to an array by the private constructors. \n \~
  \param[in] otherComp   - \ru Компонент.
                           \en A component. \~
  \param[in] otherIdent  - \ru Идентификатор нити.
                           \en A thread identifier. \~
  \param[in] otherItem   - \ru Топологический объект.
                           \en A topological object. \~
  \param[in] otherName   - \ru Имя топологического объекта.
                           \en A name of a topological object. \~
  \param[in] type        - \ru Тип аннотационных ребер.
                           \en A subtype of annotative edges. \~
  \param[in] subType     - \ru Подтип аннотационных ребер.
                           \en A subtype of annotative edges. \~
  \param[out] array      - \ru Массив аннотационных ребер.
                           \en Array of annotative edges. \~
  \ingroup Mapping
*/
// --- 
inline
MbAnnotationEdgeVestige *  AddVestigeAnnotationEdge( uint otherComp, size_t otherIdent, const MbTopologyItem * otherItem, 
                                                     const MbName & otherName, MbBaseVestige::Type type, MbBaseVestige::SubType subType,
                                                     RPArray<MbAnnotationEdgeVestige> & array )
{
  MbAnnotationEdgeVestige * vestige = new MbAnnotationEdgeVestige( otherComp, otherIdent, otherItem, otherName, type, subType );
  if ( vestige ) {
    array.Add( vestige );
  }
  return vestige;
}


//------------------------------------------------------------------------------     
/** \brief \ru След условного обозначения.
           \en The vestige of a conventional notation. \~
  \details \ru След условного обозначения.\n
           \en The vestige of a conventional notation.\n \~
  \ingroup Mapping
*/ 
// ---   
class MATH_CLASS MbSymbolVestige : public MbBaseVestige { 
  friend struct MbVEFVestiges;
protected:
  bool                bvisible; ///< \ru Флаг видимости. \en A visibility flag. 
  TPointer<MbMatrix>  matrix;   ///< \ru Матрица трансформации плоскости сечения из внутреннего представления во внешнее. \en A matrix of transformation of a section plane from internal representation to external one. 

protected:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор следа условного обозначения с определенным признаком видимости.\n
             \en Constructor of a conventional notation vestige with a defined visibility attribute.\n \~        
    \param[in] otherComp  - \ru Компонент.
                            \en A component. \~
    \param[in] otherIdent - \ru Идентификатор нити.
                            \en A thread identifier. \~
    \param[in] otherItem  - \ru Топологический объект.
                            \en A topological object. \~
    \param[in] otherName  - \ru Имя.
                            \en A name. \~   
    \param[in] _bvisible  - \ru Признак видимости.
                            \en A visibility attribute. \~
  */
  MbSymbolVestige( uint otherComp, size_t otherIdent, const MbTopologyItem * otherItem, const MbName & otherName, bool _bvisible = true )
    : MbBaseVestige( otherComp, otherIdent, otherName, otherItem )
    , bvisible( _bvisible )
    , matrix  ( nullptr      )
  {}
  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  MbSymbolVestige( const MbSymbolVestige & other, MbRegDuplicate * iReg );
  /// \ru Конструктор. \en Constructor. 
  MbSymbolVestige()
    : MbBaseVestige()
    , bvisible( true )
    , matrix  ( nullptr )
  {}

public:         
  virtual ~MbSymbolVestige() {}
  MbBaseVestige & Duplicate( MbRegDuplicate * iReg = nullptr ) const override;

public:
  /// \ru Это видимая точка? \en Is point visible? 
  bool       IsVisible() const { return bvisible; }
  /// \ru Установить матрицу трансформации. \en Set a transformation matrix. 
  void       SetMatrix( const MbMatrix & initMatrix );
  /// \ru Матрица трансформации. \en Transformation matrix. 
  MbMatrix * GetMatrix() const { return matrix; }

private:
  bool operator == ( const MbSymbolVestige & ); // \ru Не реализован. \en Not implemented. 
  bool operator != ( const MbSymbolVestige & ); // \ru Не реализован. \en Not implemented. 

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbSymbolVestige )
OBVIOUS_PRIVATE_COPY( MbSymbolVestige )
};

IMPL_PERSISTENT_OPS( MbSymbolVestige )

//------------------------------------------------------------------------------
// 
// ---
inline void MbSymbolVestige::SetMatrix( const MbMatrix & initMatrix )
{ 
  C3D_ASSERT( matrix == nullptr );
  if ( matrix == nullptr )
    matrix = new MbMatrix( initMatrix );
  else
    *matrix = initMatrix;
}


//------------------------------------------------------------------------------     
/** \brief \ru Набор массивов, выдаваемых наружу при получении всех линий.
           \en The set of arrays which are given after getting of all lines. \~
  \details \ru Набор массивов, выдаваемых наружу при получении всех линий тел(а) 
    с селектированием на видимые и невидимые.\n
           \en The set of arrays which are given after getting off all solid(s) lines 
    with the separation to visible and invisible.\n \~
  \ingroup Mapping
*/ 
// ---  
struct MATH_CLASS MbVEFVestiges {
protected:
  PArray<MbVertexVestige>         vertexVestiges;   ///< \ru Следы вершин. \en Vestiges of vertices.
  PArray<MbEdgeVestige>           edgeVestiges;     ///< \ru Следы ребер. \en Vestiges of edges.
  PArray<MbFaceVestige>           faceVestiges;     ///< \ru Следы граней. \en Vestiges of faces.
  PArray<MbAnnotationEdgeVestige> annotateVestiges; ///< \ru Следы аннотационных объектов. \en Vestiges of annotative objects.
  PArray<MbSymbolVestige>         symbolVestiges;   ///< \ru Следы условного обозначения. \en Vestiges of a conventional notation. 
  PArray<MbVertexVestige>         pointVestiges;    ///< \ru Следы пространственных точек. \en Vestiges of spatial points. 
  PArray<MbEdgeVestige>           curveVestiges;    ///< \ru Следы пространственных кривых. \en Vestiges of spatial curves. 

public:
  /// \ru Конструктор. \en Constructor. 
  MbVEFVestiges();   
  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  MbVEFVestiges( const MbVEFVestiges & other, MbRegDuplicate * iReg );
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbVEFVestiges() {}

public:
  /// \ru Создать копию объекта. \en Create a copy of the object. 
  virtual MbVEFVestiges &   Duplicate( MbRegDuplicate * iReg = nullptr ) const;
  /// \ru Очистить массивы следов. \en Clear arrays of vestiges. 
  void                      SetEmpty();
  /// \ru Очистить массивы следов. \en Clear arrays of vestiges. 
  void                      SetEmptyHard();
  /// \ru Освободить неиспользуемую память. \en Adjust memory. 
  void                      Adjust();
  /// \ru Пусты ли массивы следов. \en Check if arrays of vestiges are empty. 
  bool                      IsEmpty() const;
  /// \ru Изменить флаг владения элементами. \en Change the flag of elements ownership 
  void                      DetachOwn();
  /// \ru Зарезервировать место под n элементов следов вершин. \en Reserve memory for n elements of vertices vestiges. 
  void                      VertexVestigesReserve( size_t count );
  /// \ru Зарезервировать место под n элементов следов вершин. \en Reserve memory for n elements of edges vestiges. 
  void                      EdgesVestigesReserve( size_t count );
  /// \ru Очистить массивы следов вершин. \en Clear arrays of vertices vestiges.
  void                      ClearVertexVestiges();
  /// \ru Очистить массивы следов ребер. \en Clear arrays of edges vestiges.
  void                      ClearEdgeVestiges();
  /// \ru Очистить массивы следов граней. \en Clear arrays of faces vestiges.
  void                      ClearFaceVestiges();
  /// \ru Очистить массивы следов аннотационных объектов. \en Clear arrays of annotative objects vestiges.
  void                      ClearAnnotationVestiges();
  /// \ru Очистить массивы следов условных обозначений. \en Clear arrays of conventional notations vestiges.
  void                      ClearSymbolVestiges();
  /// \ru Очистить массивы следов пространственных точек. \en Clear arrays of spatial points vestiges.
  void                      ClearPointVestiges();
  /// \ru Очистить массивы следов пространственных кривых. \en Clear arrays of spatial curves vestiges.
  void                      ClearCurveVestiges();

public:
  /// \ru Количество следов вершин. \en Count of vertices vestiges. 
  size_t                    VertexVestigesCount()              const { return vertexVestiges.size(); }
  /// \ru Количество следов ребер. \en Count of edges vestiges. 
  size_t                    EdgeVestigesCount()                const { return edgeVestiges.size(); }
  /// \ru Количество следов граней. \en Count of faces vestiges. 
  size_t                    FaceVestigesCount()                const { return faceVestiges.size(); }
  /// \ru Количество следов аннотационных объектов. \en Count of annotative objects vestiges.
  size_t                    AnnotateVestigesCount()            const { return annotateVestiges.size(); }
  /// \ru Количество следов условных обозначений. \en Count of conventional notations vestiges.
  size_t                    SymbolVestigesCount()              const { return symbolVestiges.size(); }
  /// \ru Количество следов пространственных точек. \en Count of spatial points vestiges.
  size_t                    PointVestigesCount()               const { return pointVestiges.size(); }
  /// \ru Количество следов пространственных кривых. \en Count of spatial curves vestiges.
  size_t                    CurveVestigesCount()               const { return curveVestiges.size(); }

  /// \ru Получить указатель на след вершины. \en Get pointer to vertex vestige.
  MbVertexVestige *         GetVertexVestige( size_t ind )     const { return vertexVestiges[ind]; }
  /// \ru Получить указатель на след ребра. \en Get pointer to edge vestige.
  MbEdgeVestige *           GetEdgeVestige( size_t ind )       const { return edgeVestiges[ind]; }
  /// \ru Получить указатель на след грани. \en Get pointer to face vestige.
  MbFaceVestige *           GetFaceVestige( size_t ind )       const { return faceVestiges[ind]; }
  /// \ru Получить указатель на след аннотационного объекта. \en Get pointer to annotative object vestige.
  MbAnnotationEdgeVestige * GetAnnotationVestige( size_t ind ) const { return annotateVestiges[ind]; }
  /// \ru Получить указатель на след условного обозначения. \en Get pointer to conventional notation vestige.
  MbSymbolVestige *         GetSymbolVestige( size_t ind )     const { return symbolVestiges[ind]; }
  /// \ru Получить указатель на след пространственной точки. \en Get pointer to spatial point vestige.
  MbVertexVestige *         GetPointVestige( size_t ind )      const { return pointVestiges[ind]; }
  /// \ru Получить указатель на след пространственной кривой. \en Get pointer to spatial curve vestige.
  MbEdgeVestige *           GetCurveVestige( size_t ind )      const { return curveVestiges[ind]; }

  /** \brief \ru Добавить MbVertexVestige в массив.
             \en Add the MbVertexVestige to an array. \~
    \details \ru Добавить MbVertexVestige в массив через закрытые конструкторы. \n
             \en Add the MbVertexVestige to an array by the private constructors. \n \~
    \param[in] otherComp  - \ru Компонент.
                            \en A component. \~
    \param[in] otherIdent - \ru Идентификатор нити.
                            \en A thread identifier. \~
    \param[in] otherItem  - \ru Топологический объект.
                            \en A topological object. \~
    \param[in] uv         - \ru Точка - след вершины.
                            \en Point - vertex vestige. \~
    \param[in] vis        - \ru Состояние видимости.
                            \en Visibility state. \~
  */
  MbVertexVestige * AddVestigeVertex( uint otherComp, size_t otherIdent, const MbTopologyItem & otherItem, const MbCartPoint & uv, bool vis ); 

  /** \brief \ru Добавить MbVertexVestige в массив.
             \en Add the MbVertexVestige to an array. \~
    \details \ru Добавить MbVertexVestige в массив через закрытые конструкторы. \n
             \en Add the MbVertexVestige to an array by the private constructors. \n \~
    \param[in] otherComp  - \ru Компонент.
                            \en A component. \~
    \param[in] otherIdent - \ru Идентификатор нити.
                            \en A thread identifier. \~
    \param[in] otherName  - \ru Имя топологического объекта.
                            \en A name of a topological object. \~
    \param[in] uv         - \ru Точка - след пространственной точки.
                            \en Point - vestige of a spatial point. \~
    \param[in] vis        - \ru Флаг видимости.
                            \en A visibility flag. \~
  */
  MbVertexVestige * AddVestigePoint( uint otherComp, size_t otherIdent, const MbName & otherName, const MbCartPoint & uv, bool vis, bool isDegeneratedCurve );

  /** \brief \ru Добавить MbEdgeVestige в массив.
              \en Add the MbEdgeVestige to an array. \~
    \details \ru Добавить MbEdgeVestige в массив через закрытые конструкторы. \n
             \en Add the MbEdgeVestige to an array by the private constructors. \n \~
    \param[in] otherComp  - \ru Компонент.
                            \en A component. \~
    \param[in] otherIdent - \ru Идентификатор нити.
                            \en A thread identifier. \~
    \param[in] otherItem  - \ru Топологический объект.
                            \en A topological object. \~
    \param[in] isSmooth   - \ru Тип ребра (гладкое или нет).
                            \en A type of edge (smooth or not). \~
    \param[in] isSection  - \ru Ребро от сечения или разреза.
                            \en Edge of section. \~
  */
  MbEdgeVestige * AddVestigeEdge( uint otherComp, size_t otherIdent, const MbTopologyItem & otherItem, bool isSmooth, bool isSection );

  /** \brief \ru Добавить MbEdgeVestige в массив.
              \en Add the MbEdgeVestige to an array. \~
    \details \ru Добавить MbEdgeVestige в массив через закрытые конструкторы. \n
             \en Add the MbEdgeVestige to an array by the private constructors. \n \~
    \param[in] otherComp     - \ru Компонент.
                               \en A component. \~
    \param[in] otherIdent    - \ru Идентификатор нити.
                               \en A thread identifier. \~
    \param[in] otherName     - \ru Имя топологического объекта.
                               \en A name of a topological object. \~
    \param[in] isCenterLine  - \ru Является ли след осевой линией объекта.
                               \en Is curve vestige a center line. \~
  */
  MbEdgeVestige * AddVestigeCurve( uint otherComp, size_t otherIdent, const MbName & otherName, bool isCenterLine = false );

  /** \brief \ru Добавить MbEdgeVestige в массив.
              \en Add the MbEdgeVestige to an array. \~
    \details \ru Добавить MbEdgeVestige в массив через закрытые конструкторы. \n
             \en Add the MbEdgeVestige to an array by the private constructors. \n \~
    \param[in] otherComp  - \ru Компонент.
                            \en A component. \~
    \param[in] otherIdent - \ru Идентификатор нити.
                            \en A thread identifier. \~
    \param[in] mapCurves  - \ru Следы кривой.
                            \en Spatial curve vestiges. \~
    \param[in] visible    - \ru Состояние видимости.
                            \en A visibility state. \~
    \param[in] otherName  - \ru Имя топологического объекта.
                            \en A name of a topological object. \~
  */
  MbEdgeVestige * AddVestigeCurve( uint otherComp, size_t otherIdent, const RPArray<MbCurve> & mapCurves, bool visible, const MbName & otherName );

  /** \brief \ru Добавить MbFaceVestige в массив.
             \en Add the MbFaceVestige to an array. \~
    \details \ru Добавить MbFaceVestige в массив через закрытые конструкторы. \n
             \en Add the MbFaceVestige to an array by the private constructors. \n \~
    \param[in] otherComp  - \ru Компонент.
                            \en A component. \~
    \param[in] otherIdent - \ru Идентификатор нити.
                            \en A thread identifier. \~
    \param[in] otherItem  - \ru Топологический объект.
                            \en A topological object. \~
  */
  MbFaceVestige * AddVestigeFace( uint otherComp, size_t otherIdent, const MbTopologyItem & otherItem );

  /** \brief \ru Добавить MbFaceVestige в массив.
             \en Add the MbFaceVestige to an array. \~
    \details \ru Добавить MbFaceVestige в массив через закрытые конструкторы. \n
             \en Add the MbFaceVestige to an array by the private constructors. \n \~
    \param[in] otherComp  - \ru Компонент.
                            \en A component. \~
    \param[in] otherIdent - \ru Идентификатор нити.
                            \en A thread identifier. \~
    \param[in] otherItem  - \ru Полигональный объект.
                            \en A polygonal object. \~
  */
  MbFaceVestige * AddVestigeFace( uint otherComp, size_t otherIdent, const MbGrid & otherItem );

  /** \brief \ru Добавить MbAnnotationEdgeVestige в массив.
             \en Add the MbAnnotationEdgeVestige to an array. \~
    \details \ru Добавить MbAnnotationEdgeVestige в массив через закрытые конструкторы. \n
             \en Add the MbAnnotationEdgeVestige to an array by the private constructors. \n \~
    \param[in] otherComp   - \ru Компонент.
                             \en A component. \~
    \param[in] otherIdent  - \ru Идентификатор нити.
                             \en A thread identifier. \~
    \param[in] otherItem   - \ru Топологический объект.
                             \en A topological object. \~
    \param[in] otherName   - \ru Имя топологического объекта.
                             \en A name of a topological object. \~
    \param[in] type        - \ru Подтип аннотационных ребер.
                             \en A subtype of annotative edges. \~
    \param[in] subType     - \ru Подтип аннотационных ребер.
                             \en A subtype of annotative edges. \~
  */
  MbAnnotationEdgeVestige *  AddVestigeAnnotationEdge( uint otherComp, size_t otherIdent, const MbTopologyItem * otherItem, 
                                                       const MbName & otherName, MbBaseVestige::Type type, MbBaseVestige::SubType subType );

  /** \brief \ru Добавить MbSymbolVestige в массив.
             \en Add the MbSymbolVestige to an array. \~
    \details \ru Добавить MbSymbolVestige в массив через закрытые конструкторы. \n
             \en Add the MbSymbolVestige to an array by the private constructors. \n \~
    \param[in] otherComp  - \ru Компонент.
                            \en A component. \~
    \param[in] otherIdent - \ru Идентификатор нити.
                            \en A thread identifier. \~
    \param[in] otherItem  - \ru Топологический объект.
                            \en A topological object. \~
    \param[in] otherName  - \ru Имя.
                            \en A name. \~   
    \param[in] _bvisible  - \ru Признак видимости.
                            \en A visibility attribute. \~
  */
  MbSymbolVestige * AddVestigeSymbol( uint otherComp, size_t otherIdent, const MbTopologyItem * otherItem, const MbName & otherName, bool _bvisible = true );

  /// \ru Трансформировать следы по матрице. \en Transform vestiges according to a matrix.
  void              Transform( const MbMatrix & matrix );
  /// \ru Удалить след вершины. \en Delete vertex vestige.
  void              RemoveVertexVestige( size_t ind );
  /// \ru Удалить след ребра. \en Delete edge vestige.
  void              RemoveEdgeVestige( size_t ind );
  /// \ru Удалить след грани. \en Delete face vestige.
  void              RemoveFaceVestige( size_t ind );
  /// \ru Удалить след аннотационного объекта. \en Delete annotative object vestige.
  void              RemoveAnnotationVestige( size_t ind );
  /// \ru Удалить след условного обозначения. \en Delete conventional notation vestige.
  void              RemoveSymbolVestige( size_t ind );
  /// \ru Удалить след пространственной точки. \en Delete spatial point vestige.
  void              RemovePointVestige( size_t ind );
  /// \ru Удалить след пространственной кривой. \en Delete spatial curve vestige.
  void              RemoveCurveVestige( size_t ind );

  /// \ru Добавить следы из другого набора. \en Add vestiges from other set.
  void              AddFrom( const MbVEFVestiges & other );
  /// \ru Инициализировать следы аннотационных объектов. \en Initialize annotative objects vestiges.
  template <class AnnotateVector>
  void InitAnnotateVestiges( const AnnotateVector & annotates ) {
    annotateVestiges.clear();
    annotateVestiges.AddArray( annotates );
  }
  /// \ru Инициализировать следы пространственных кривых. \en Initialize spatial curves vestiges.
  template <class CurveVector>
  void InitCurveVestiges( CurveVector & curves ) {
    curveVestiges.clear();
    curveVestiges.AddArray( curves );
  }
  /// \ru Выдать следы вершин. \en Get vertices vestiges.
  template <class VertexVector>
  void GetVertexVestiges( VertexVector & vertices ) const {
    vertices.reserve( vertices.size() + vertexVestiges.size() );
    std::copy( vertexVestiges.begin(), vertexVestiges.end(), std::back_inserter(vertices) );
  }
  /// \ru Выдать следы ребер. \en Get edges vestiges.
  template <class EdgeVector>
  void GetEdgeVestiges( EdgeVector & edges ) const {
    edges.reserve( edges.size() + edgeVestiges.size() );
    std::copy( edgeVestiges.begin(), edgeVestiges.end(), std::back_inserter(edges) );
  }
  /// \ru Выдать следы граней. \en Get faces vestiges.
  template <class FaceVector>
  void GetFaceVestiges( FaceVector & faces ) const {
    faces.reserve( faces.size() + faceVestiges.size() );
    std::copy( faceVestiges.begin(), faceVestiges.end(), std::back_inserter(faces) );
  }
  /// \ru Выдать следы аннотационных объектов. \en Get annotative objects vestiges.
  template <class AnnotateVector>
  void GetAnnotateVestiges( AnnotateVector & annotates ) const {
    annotates.reserve( annotates.size() + annotateVestiges.size() );
    std::copy( annotateVestiges.begin(), annotateVestiges.end(), std::back_inserter(annotates) );
  }
  /// \ru Выдать следы условных обозначений. \en Get conventional notations vestiges.
  template <class SymbolVector>
  void GetSymbolVestiges( SymbolVector & symbols ) const {
    symbols.reserve( symbols.size() + symbolVestiges.size() );
    std::copy( symbolVestiges.begin(), symbolVestiges.end(), std::back_inserter(symbols) );
  }
  /// \ru Выдать следы пространственных точек. \en Get spatial points vestiges.
  template <class PointVector>
  void GetPointVestiges( PointVector & points ) const {
    points.reserve( points.size() + pointVestiges.size() );
    std::copy( pointVestiges.begin(), pointVestiges.end(), std::back_inserter(points) );
  }
  /// \ru Выдать следы пространственных кривых. \en Get spatial curves vestiges.
  template <class CurveVector>
  void GetCurveVestiges( CurveVector & curves ) const {
    curves.reserve( curves.size() + curveVestiges.size() );
    std::copy( curveVestiges.begin(), curveVestiges.end(), std::back_inserter(curves) );
  }

private:
  bool operator == ( const MbVEFVestiges & ); // \ru Не реализован. \en Not implemented. 
  bool operator != ( const MbVEFVestiges & ); // \ru Не реализован. \en Not implemented. 

DECLARE_NEW_DELETE_CLASS( MbVEFVestiges )
DECLARE_NEW_DELETE_CLASS_EX( MbVEFVestiges )
KNOWN_OBJECTS_RW_REF_OPERATORS_EX ( MbVEFVestiges, MATH_FUNC_EX ) // \ru Для работы со ссылками и объектами класса \en For working with references and objects of the class 
OBVIOUS_PRIVATE_COPY( MbVEFVestiges )
};


//------------------------------------------------------------------------------
//
// ---
inline MbVertexVestige * MbVEFVestiges::AddVestigeVertex( uint otherComp, size_t otherIdent, const MbTopologyItem & otherItem, const MbCartPoint & uv, bool vis )
{
  MbVertexVestige * vestige = new MbVertexVestige( otherComp, otherIdent, otherItem, vis );
  if ( vestige ) {
    vestige->point = uv;
    vertexVestiges.Add( vestige );
  }
  return vestige;
}

//------------------------------------------------------------------------------
//
// ---
inline MbVertexVestige * MbVEFVestiges::AddVestigePoint( uint otherComp, size_t otherIdent, const MbName & otherName, const MbCartPoint & uv, bool vis, bool isDegeneratedCurve )
{
  MbVertexVestige * vestige = new MbVertexVestige( otherComp, otherIdent, otherName, vis, isDegeneratedCurve );
  if ( vestige ) {
    vestige->point = uv;
    pointVestiges.Add( vestige );
  }
  return vestige;
}

//------------------------------------------------------------------------------
//
// ---
inline MbEdgeVestige * MbVEFVestiges::AddVestigeEdge( uint otherComp, size_t otherIdent, const MbTopologyItem & otherItem, bool isSmooth, bool isSection ) 
{
  MbEdgeVestige * vestige = new MbEdgeVestige( otherComp, otherIdent, otherItem, isSmooth, isSection );
  if ( vestige ) {
    edgeVestiges.Add( vestige );
  }
  return vestige;
}

//------------------------------------------------------------------------------
//
// ---
inline MbEdgeVestige * MbVEFVestiges::AddVestigeCurve( uint otherComp, size_t otherIdent, const MbName & otherName, bool isCenterLine ) {
  return ::AddVestigeCurve( otherComp, otherIdent, otherName, curveVestiges, isCenterLine, false );
}

//------------------------------------------------------------------------------
//
// ---
inline MbEdgeVestige * MbVEFVestiges::AddVestigeCurve( uint otherComp, size_t otherIdent, const RPArray<MbCurve> & mapCurves, bool visible,
                                                       const MbName & otherName ) 
{
  MbEdgeVestige * ev = nullptr;

  ev = ::AddVestigeCurve( otherComp, otherIdent, otherName, curveVestiges, false, false );
  // BUG_93683 ev = ::AddVestigeEdge( otherComp, otherIdent, otherName, MbBaseVestige::vt_Edge, edgeVestiges );

  if ( ev != nullptr ) {
    MbCurveVestige & vc = ev->curveInfo;
    for ( size_t m = 0, mapCurvesCnt = mapCurves.Count(); m < mapCurvesCnt; m++ ) {
      MbCurve * mapCurve = mapCurves[m];
      if ( mapCurve != nullptr )
        vc.AddSegment( *mapCurve, visible );
    }
  }

  return ev;
}

//------------------------------------------------------------------------------
//
// ---
inline MbFaceVestige * MbVEFVestiges::AddVestigeFace( uint otherComp, size_t otherIdent, const MbTopologyItem & otherItem ) 
{
  MbFaceVestige * vestige = new MbFaceVestige( otherComp, otherIdent, otherItem );
  if ( vestige )
    faceVestiges.Add( vestige );
  return vestige;
}


//------------------------------------------------------------------------------
//
// ---
inline MbFaceVestige * MbVEFVestiges::AddVestigeFace( uint otherComp, size_t otherIdent, const MbGrid & otherItem )
{
  MbFaceVestige * vestige = new MbFaceVestige( otherComp, otherIdent, otherItem );
  if ( vestige )
    faceVestiges.Add( vestige );
  return vestige;
}


//------------------------------------------------------------------------------
//
// ---
inline
MbAnnotationEdgeVestige * MbVEFVestiges::AddVestigeAnnotationEdge( uint otherComp, size_t otherIdent, const MbTopologyItem * otherItem, 
                                                                   const MbName & otherName, MbBaseVestige::Type type, MbBaseVestige::SubType subType )
{
  return ::AddVestigeAnnotationEdge( otherComp, otherIdent, otherItem, otherName, type, subType, annotateVestiges );
}

//------------------------------------------------------------------------------
//
// ---
inline MbSymbolVestige * MbVEFVestiges::AddVestigeSymbol( uint otherComp, size_t otherIdent, const MbTopologyItem * otherItem, const MbName & otherName, bool _bvisible )
{
  MbSymbolVestige * vestige = new MbSymbolVestige( otherComp, otherIdent, otherItem, otherName, _bvisible );
  if ( vestige )
    symbolVestiges.Add( vestige );
  return vestige;
}


//------------------------------------------------------------------------------
/** \brief \ru Доступ к элементу массива.
           \en The access to an array element. \~
  \details \ru Доступ к элементу массива по индексу.\n
           \en The access to an array element by an index.\n \~
  \param[in] i     - \ru Индекс.
                     \en An index. \~
  \param[in] array - \ru Множество следов вершин.
                     \en An array of vertices vestiges. \~
  \return \ru След вершины по индексу
          \en A vestige of a vertex by an index. \~
  \ingroup Mapping
*/ 
// --- 
inline 
MbVertexVestige & GetVertexI( size_t i, const MbVEFVestiges & vestiges ) {
  return *vestiges.GetVertexVestige( i );
}


//------------------------------------------------------------------------------
/// \ru Получение полной проекции. \en Merge total projection. 
// ---
MbCurve * MergeTotalMap( const std::vector<MbCurve *> &, MbMapSpaceCurveType = mst_Unset );


//------------------------------------------------------------------------------
/// \ru Слияние наложений линий очерка. \en Merge impositions of silhouette lines of the face. 
// ---
MATH_FUNC (bool) MergeFaceVestiges( MbVEFVestiges &, const MbMatrix3D * );


#endif // __MAP_VESTIGE_H
