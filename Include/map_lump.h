////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Объекты для проецирования.
         \en Objects for the projection. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MAP_LUMP_H
#define __MAP_LUMP_H


#include <set>
#include <lump.h>
#include <io_define.h>
#include <templ_sptr.h>
#include <curve3d.h>
#include <cur_contour_on_plane.h>
#include <mesh_polygon.h>
#include <mesh_primitive.h>
#include <mb_thread.h>
#include <map_vestige.h>
#include <solid.h>
#include <mb_symbol.h>


//------------------------------------------------------------------------------
/** \brief \ru Настройки видимости следов проецируемых объектов.
           \en Visibility mode of mapping. \~
  \details \ru Настройки видимости следов проецируемых объектов.\n
           \en Visibility mode of mapping.\n \~
  \ingroup Mapping
*/
// ---
struct MATH_CLASS MbMapVisibilityMode {

  /** \brief \ru Настройки проецирования объектов по глубине видимимости.
             \en Mapping mode by visibility depth. \~
    \details \ru При наличии ограничений тип опредяляет видимость следов в зависимости от положения объекта относительно границы (глубины) видимости.
             \en If there are constraints, the type determines the vestige visibility depending on object position relative to the edge (depth) of visibility.. \~
    \warning \ru В разработке.
             \en Under development. \~
  */
  enum class LimitDepthMode : uint8 {
    off,        ///< \ru Ограничения по глубине видимости отключены. \en Visibility constraints are disabled. \~
    on,         ///< \ru Ограничения включены, объекты целиком находящиеся за границей видимости не проецируются. \en Visibility constraints are enabled. Objects located entirely outside the visibility edge are not mapped. \~
    on_exclude, ///< \ru Ограничения включены, объекты целиком находящиеся за границей видимости и на границе видимости не проецируются. \en Visibility constraints are enabled. Objects located entirely outside the visibility edge and on the visibility edge are not mapped. \~
    cut,        ///< \ru Ограничения включены, объекты целиком находящиеся за границей видимости не проецируются, находящиеся на границе видимости будут проецироваться в рассеченном виде. \en Visibility constraints are enabled. Objects located entirely outside the visibility edge are not mapped, while those located within the visibility edge will be mapped in a cutted form. \~
  };

protected:
  bool           _invisible;            ///< \ru Добавлять проекции невидимых линий и точек. \en Add vestiges of invisible curves and points.
  bool           _skipVertices;         ///< \ru Не добавлять проекции вершин тела. \en Skip vestiges of vertices.  
  bool           _shadeCurves;          ///< \ru Флаг затенения пространственных кривых телами (false по умолчанию). \en Shade spatial curves by bodies(false by default). 
  bool           _useBodyTransparency;  ///< \ru Учитывать прозрачность тел при проецировании (false по умолчанию). \en Use bodies transparency while mapping(false by default).
  bool           _addCenterLines;       ///< \ru Добавлять проекции осевых линий поверхностей (false по умолчанию). \en Add vestiges of center lines of surfaces(false by default). 
  LimitDepthMode _limitDepthMode;       ///< \ru Настройки проецирования объектов по глубине видимимости (off по умолчанию). \en Mapping mode by visibility depth (off by default).
  double         _visibilityDepth;      ///< \ru Глубина видимости (0 по умолчанию). \en Visibility depth (0 by default).
  
public:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор видимости следов проецируемых объектов.\n
             \en Constructor of visibility mode of mapping.\n \~
  \param[in] invis            - \ru Добавлять проекции невидимых линий и точек.
                                \en Add vestiges of invisible curves and points. \~
  \param[in] skipVerts        - \ru Не добавлять проекции вершин тела.
                                \en Skip vestiges of vertices. \~
  \param[in] shadeSpaceCurves - \ru Флаг затенения пространственных кривых телами (false по умолчанию).
                                \en Shade spatial curves by bodies(false by default). \~
  \param[in] useTransparency  - \ru Учитывать прозрачность тел при проецировании (false по умолчанию).
                                \en Use bodies transparency while mapping(false by default). \~
  \param[in] addCentLines     - \ru Добавлять проекции осевых линий поверхностей (false по умолчанию).
                                \en Add vestiges of center lines of surfaces(false by default). \~
  */
  MbMapVisibilityMode( bool invis, 
                       bool skipVerts, 
                       bool shadeSpaceCurves = false, 
                       bool useTransparency  = false, 
                       bool addCentLines     = false )
    : _invisible           ( invis               )
    , _skipVertices        ( skipVerts           )
    , _shadeCurves         ( shadeSpaceCurves    )
    , _useBodyTransparency ( useTransparency     )
    , _addCenterLines      ( addCentLines        )
    , _limitDepthMode      ( LimitDepthMode::off )
    , _visibilityDepth     ( 0.0                 )
  {}
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор видимости следов проецируемых объектов.\n
             \en Constructor of visibility mode of mapping.\n \~
  \param[in] invis            - \ru Добавлять проекции невидимых линий и точек.
                                \en Add vestiges of invisible curves and points. \~
  \param[in] skipVerts        - \ru Не добавлять проекции вершин тела.
                                \en Skip vestiges of vertices. \~
  \param[in] shadeSpaceCurves - \ru Флаг затенения пространственных кривых телами.
                                \en Shade spatial curves by bodies. \~
  \param[in] useTransparency  - \ru Учитывать прозрачность тел при проецировании.
                                \en Use bodies transparency while mapping. \~
  \param[in] limitDepthMode   - \ru Настройки проецирования объектов по глубине видимимости.
                                \en Mapping mode by visibility depth. \~
  \param[in] visibleDepth     - \ru Глубина видимости.
                                \en Visibility depth. \~
  \warning \ru В разработке.
           \en Under development. \~
  */
  MbMapVisibilityMode( bool           invis,
                       bool           skipVerts,
                       bool           shadeSpaceCurves, 
                       bool           useTransparency,
                       bool           addCentLines,
                       LimitDepthMode limitDepthMode,
                       double         visibleDepth     )
    : _invisible            ( invis            )
    , _skipVertices         ( skipVerts        )
    , _shadeCurves          ( shadeSpaceCurves )
    , _useBodyTransparency  ( useTransparency  )
    , _addCenterLines       ( addCentLines     )
    , _limitDepthMode       ( limitDepthMode   )
    , _visibilityDepth      ( visibleDepth     )
  {}
  MbMapVisibilityMode( const MbMapVisibilityMode & m ) = default;

public:
  /** \brief   \ru Инициализация.
               \en Initialization. \~
      \details \ru Инициализация по параметрам.\n
               \en Initialization by parameters.\n \~
  \param[in] invis            - \ru Добавлять проекции невидимых линий и точек.
                                \en Add vestiges of invisible curves and points. \~
  \param[in] skipVerts        - \ru Не добавлять проекции вершин тела.
                                \en Skip vestiges of vertices. \~
  \param[in] shadeSpaceCurves - \ru Флаг затенения пространственных кривых телами (false по умолчанию).
                                \en Shade spatial curves by bodies(false by default). \~
  \param[in] useTransparency  - \ru Учитывать прозрачность тел при проецировании (false по умолчанию).
                                \en Use bodies transparency while mapping(false by default). \~
  \param[in] addCentLines     - \ru Добавлять проекции осевых линий поверхностей (false по умолчанию).
                                \en Add vestiges of center lines of surfaces(false by default). \~
  */
  void Init( bool invis,
             bool skipVerts, 
             bool shadeSpaceCurves = false,
             bool useTransparency  = false,
             bool addCentLines     = false ) 
  {
    _invisible = invis;
    _skipVertices = skipVerts;
    _shadeCurves = shadeSpaceCurves;
    _useBodyTransparency = useTransparency;
    _addCenterLines = addCentLines;
    _limitDepthMode = LimitDepthMode::off;
    _visibilityDepth = 0.0;
  }
  /** \brief  \ru Инициализация.
              \en Initialization. \~
     \details \ru Инициализация по параметрам.\n
              \en Initialization by parameters.\n \~
 \param[in] invis            - \ru Добавлять проекции невидимых линий и точек.
                               \en Add vestiges of invisible curves and points. \~
 \param[in] skipVerts        - \ru Не добавлять проекции вершин тела.
                               \en Skip vestiges of vertices. \~
 \param[in] shadeSpaceCurves - \ru Флаг затенения пространственных кривых телами.
                               \en Shade spatial curves by bodies. \~
 \param[in] useTransparency  - \ru Учитывать прозрачность тел при проецировании.
                               \en Use bodies transparency while mapping. \~
 \param[in] addCentLines     - \ru Добавлять проекции осевых линий поверхностей.
                               \en Add vestiges of center lines of surfaces. \~
 \param[in] limitDepthMode   - \ru Настройки проецирования объектов по глубине видимимости.
                               \en Mapping mode by visibility depth. \~
 \param[in] visibleDepth     - \ru Глубина видимости.
                               \en Visibility depth. \~
 \warning \ru В разработке.
          \en Under development. \~
  */
  void Init( bool           invis,
             bool           skipVerts,
             bool           shadeSpaceCurves,
             bool           useTransparency,
             bool           addCentLines,
             LimitDepthMode limitDepthMode,
             double         visibleDepth    )
  {
    _invisible = invis;
    _skipVertices = skipVerts;
    _shadeCurves = shadeSpaceCurves;
    _useBodyTransparency = useTransparency;
    _addCenterLines = addCentLines;
    _limitDepthMode = limitDepthMode;
    _visibilityDepth = visibleDepth;
  }
  /// \ru Добавить проекции невидимых линий и точек? \en Add vestiges of invisible curves and points?
  bool AddInvisible() const { return _invisible; }
  /// \ru Пропустить проекции вершин тела? \en Skip vestiges of vertices?
  bool SkipVertices() const { return _skipVertices; }
  /// \ru Затенять пространственные кривые телами? \en Shade spatial curves by bodies?
  bool ShadeSpaceCurves() const { return _shadeCurves; }
  /// \ru Учитывать прозрачность тел при проецировании? \en Use bodies transparency while mapping?
  bool UseBodyTransparency() const { return _useBodyTransparency; }
  /// \ru Добавить проекции осевых линий поверхностей? \en Add vestiges of center lines of surfaces?
  bool AddCenterLines() const { return _addCenterLines; }
  /// \ru Получить тип ограничения. В разработке. \en Get constraint mode. Under development.
  LimitDepthMode GetLimitDepthMode() const { return _limitDepthMode; }
  /// \ru Установить тип ограничения. В разработке. \en Set constraint mode. Under development.
  void SetLimitDepthMode( const LimitDepthMode & limitDepthMode ) { _limitDepthMode = limitDepthMode; }
  /// \ru Получить глубину видимости. В разработке. \en Get visibility depth. Under development.
  double GetVisibilityDepth() const { return _visibilityDepth; }

public:
  /// \ru Оператор присваивания. \en An assignment operator.
  MbMapVisibilityMode & operator = ( const MbMapVisibilityMode & m )
  {
    _invisible = m._invisible;
    _skipVertices = m._skipVertices;
    _shadeCurves = m._shadeCurves;
    _useBodyTransparency = m._useBodyTransparency;
    _addCenterLines = m._addCenterLines;
    _limitDepthMode = m._limitDepthMode;
    _visibilityDepth = m._visibilityDepth;
    return *this;
  }

private:
  MbMapVisibilityMode(); // \ru Не реализован. \en Not implemented. 
};


//------------------------------------------------------------------------------
/** \brief \ru Кривая с типом и погрешностью.
           \en The curve with the type and tolerance. \~
  \details \ru Кривая с типом и погрешностью.\n
           \en The curve with the type and tolerance.\n \~
  \ingroup Mapping
*/
// ---
class MATH_CLASS CurveWType : public TapeBase {
private:
  c3d::ConstSpaceCurveSPtr  curve;      ///< \ru Кривая. \en A curve. 
  MbBaseVestige::Type       type;       ///< \ru Тип. \en A type. 
  MbBaseVestige::SubType    subType;    ///< \ru Подтип. \en A subtype. 
  double                    tolerance;  ///< \ru Толерантность. \en A tolerance. 
  mutable bool              hidden;     ///< \ru Флаг невидимости. \en An invisibility flag. 

private:
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию \en The declaration without implementation of the copy-constructor to prevent a copying by default 
  CurveWType( const CurveWType & other ); 
  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  CurveWType( const CurveWType & other, MbRegDuplicate * iReg );

public:

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор видимой кривой.\n
             \en Constructor of a visible curve.\n \~    
    \param[in] _curve     - \ru Кривая.
                            \en A curve. \~
    \param[in] _type      - \ru Тип плоского отображения.
                            \en A type of planar mapping. \~
    \param[in] _subType   - \ru Подтип.
                            \en A subtype. \~
    \param[in] _tolerance - \ru Толерантность.
                            \en A tolerance. \~
  */
  CurveWType( const MbCurve3D & _curve, MbBaseVestige::Type _type, MbBaseVestige::SubType _subType, double _tolerance ) 
    : curve    ( &_curve        )
    , type     ( _type          )
    , subType  ( _subType       )
    , tolerance( _tolerance     )
    , hidden   ( false          )
  { 
  }

  /// \ru Деструктор. \en Destructor. 
  virtual ~CurveWType() {}
  /// \ru Сделать копию объекта. \en Create a copy of the object. 
  virtual CurveWType & Duplicate( MbRegDuplicate * iReg = nullptr ) const;

public:  

        /// \ru Получить кривую. \en Get a curve. 
  const MbCurve3D             & GetCurve()     const { return *curve;    }
        /// \ru Получить тип. \en Get a type. 
        MbBaseVestige::Type     GetType()      const { return type;      }
        /// \ru Получить подтип. \en Get a subtype. 
        MbBaseVestige::SubType  GetSubType()   const { return subType;   }
        /// \ru Получить толерантность. \en Get a tolerance. 
        double                  GetTolerance() const { return tolerance; }
        /// \ru Является ли невидимой. \en Whether or not invisible. 
        bool                    IsHidden() const { return hidden; }
        /// \ru Установить состояние невидимости. \en Set the invisibility state. 
        void                    SetHidden( bool h ) const { hidden = h; }

protected:
  DECLARE_PERSISTENT_CLASS_NEW_DEL ( CurveWType )
private:
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию \en The declaration without implementation of the assignment operator to prevent an assignment by default 
  void    operator = ( const CurveWType & ); 
};

IMPL_PERSISTENT_OPS( CurveWType )

//------------------------------------------------------------------------------
/** \brief \ru Множество аннотационных кривых.
           \en The array of annotative curves. \~
  \details \ru Множество аннотационных кривых.\n
           \en The array of annotative curves.\n \~
  \ingroup Mapping
*/
// ---
class MATH_CLASS MbAnnCurves : public MbRefItem, public TapeBase {
private:
  uint                      component;  ///< \ru Компонент, в котором определен набор кривых. \en A component in which a set of curves is defined. 
  size_t                    identifier; ///< \ru Идентификатор нити. \en A thread identifier.  
  c3d::ConstSolidSPtr       solid;      ///< \ru Тело, на котором нарезана резьба. \en A threaded solid.
  MbMatrix3D                from;       ///< \ru Матрица пересчета в мир. \en A matrix of transformation to the world coordinate system. 
  TOwnPointer<const MbName> name;       ///< \ru Имя набора кривых. \en A name of a set of curves. \~ \internal \ru Всегда есть!!! \en Always exists!!! \~ \endinternal
  PArray<CurveWType>        wtCurves;   ///< \ru Набор кривых с типом и погрешностью. \en A set of curves with type and tolerance. 

private:
  MbAnnCurves(); // \ru не реализовано \en not implemented 
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию \en The declaration without implementation of the copy-constructor to prevent a copying by default 
  MbAnnCurves( const MbAnnCurves & other ); 
public:
  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  MbAnnCurves( const MbAnnCurves & other, MbRegDuplicate * iReg );
  
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор объекта с пустым набором кривых.\n
             \en Constructor of an object with an empty set of curves.\n \~   
    \param[in] _name  - \ru Имя набора кривых.
                        \en A name of a set of curves. \~
    \param[in] _comp  - \ru Компонент, в котором определен набор кривых.
                        \en A component in which a set of curves is defined. \~
    \param[in] _ident - \ru Идентификатор нити.
                        \en A thread identifier. \~
    \param[in] _solid - \ru Тело, в котором нарезана резьба.
                        \en A threaded solid. \~
    \param[in] _from  - \ru Матрица пересчета в мир.
                        \en A matrix of transformation to the world coordinate system. \~
  */
  explicit MbAnnCurves( const MbName & _name, uint _comp, size_t _ident, const MbSolid * _solid, const MbMatrix3D & _from )
    : component ( _comp      )
    , identifier( _ident     )
    , solid     ( _solid     )
    , from      ( _from      )
    , name      ( &_name     )
    , wtCurves  ( 0, 1, true )
  {
    name.SetOwn(false);
  }

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор объекта с пустым набором кривых, нулевым телом и единичной матрицей.\n
             \en Constructor of an object with the empty set of curves, the null solid and the identity matrix.\n \~   
    \param[in] _name  - \ru Имя набора кривых.
                        \en A name of a set of curves. \~
    \param[in] _comp  - \ru Компонент, в котором определен набор кривых.
                        \en A component in which a set of curves is defined. \~
    \param[in] _ident - \ru Идентификатор нити.
                        \en A thread identifier. \~       
  */
  explicit MbAnnCurves( const MbName & _name, uint _comp, size_t _ident )
    : component ( _comp      )
    , identifier( _ident     )
    , solid     ( nullptr       )
    , from      (            )
    , name      ( &_name     )    
    , wtCurves ( 0, 1, true )
  {
    name.SetOwn(false);
  }

  /// \ru Деструктор. \en Destructor. 
  virtual ~MbAnnCurves()
  {    
  }

public:

  /// \ru Дать копию объекта. \en Get a copy of the object. 
  virtual       MbAnnCurves & Duplicate( MbRegDuplicate * iReg = nullptr ) const;

                /// \ru Получить имя компонента. \en Get the component name. 
                uint          GetComponent()  const { return component; }
                /// \ru Получить идентификатор нити. \en Get the thread identifier. 
                size_t        GetIdentifier() const { return identifier; }
                /// \ru Получить указатель на тело. \en Get the pointer to the solid. 
          const MbSolid     * GetSolid()      const { return solid; }
                /// \ru Получить матрицу преобразования в мир. \en Get the matrix of transformation to the world coordinate system. 
          const MbMatrix3D  & GetMatrixFrom() const { return from;  }
                /// \ru Получить имя набора кривых. \en Get the name of a set of curves. 
          const MbName      & GetName()       const { return *name; }

                /** \brief \ru Забрать кривую к себе.
                           \en Take the curve. \~
                  \details \ru Добавить кривую в набор кривых и обнулить указатель.\n
                           \en Add a curve to the set of curves and reset the pointer.\n \~   
                  \param[in, out] wtCurve - \ru Кривая.
                                            \en A curve. \~                
                */
                void          AbsorbCurve( CurveWType *& wtCurve ) { wtCurves.Add( wtCurve ); wtCurve = nullptr; }

                /// \ru Количество кривых в наборе. \en The number of curves in the set. 
                size_t        GetCurvesCount() const { return wtCurves.Count(); } 
                /// \ru Получить указатель на кривую. \en Get the pointer to the curve. 
          const CurveWType  * GetCurve( size_t k ) const { return ((k < wtCurves.Count()) ? wtCurves[k] : nullptr); }
        
private:
  DECLARE_PERSISTENT_CLASS_NEW_DEL ( MbAnnCurves )

private:
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию \en The declaration without implementation of the assignment operator to prevent an assignment by default 
  void    operator = ( const MbAnnCurves & ); 
};

IMPL_PERSISTENT_OPS( MbAnnCurves )

//------------------------------------------------------------------------------
/** \brief \ru Реализация интерфейса аннотационного вида.
           \en The implementation of the annotation view interface. \~
  \details \ru Реализация интерфейса аннотационного вида.\n
           \en The implementation of the annotation view interface.\n \~
  \ingroup Mapping
*/
// ---
class MATH_CLASS MbSimbolthThreadView : public MbRefItem, public TapeBase {
protected :
  MbAnnCurves               m_annCurves; ///< \ru Аннотационные кривы. \en Annotative curves. 
  uint                      m_compHash;  ///< \ru Имя компонента. \en A component name. 
  TOwnPointer<const MbName> m_name;      ///< \ru Имя. \en A name. \~ \internal \ru Всегда есть. \en Always exists. \~ \endinternal

public :
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор.\n
             \en Constructor.\n \~   
    \param[in] _component - \ru Компонент.
                            \en A component. \~
    \param[in] threadId   - \ru Идентификатор нити.
                            \en A thread identifier. \~
    \param[in] _name      - \ru Имя.
                            \en A name. \~
  */
  MbSimbolthThreadView( uint _component, uint threadId, const MbName & _name );
  
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbSimbolthThreadView();         
  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  MbSimbolthThreadView( const MbSimbolthThreadView & other, MbRegDuplicate * iReg );

public:
  /// \ru Дать копию объекта. \en Get a copy of the object. 
  virtual MbSimbolthThreadView & Duplicate( MbRegDuplicate * iReg = nullptr ) const;
public :  

  /// \ru Получить аннотационные кривые для редактирования. \en Get annotative curves for editing. 
  virtual       MbAnnCurves & SetAnnCurves    ();
  /// \ru Получить аннотационные кривые. \en Get annotative curves. 
  virtual const MbAnnCurves & GetAnnCurves    () const;
  /// \ru Получить компонент. \en Get the component. 
  virtual uint                GetComponent    () const;
  /// \ru Получить имя. \en Get the name. 
  virtual const MbName      & GetName         () const;
  /// \ru Получить номер нитки. \en Get a thread number. 
  virtual size_t              GetShellThreadId() const;

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbSimbolthThreadView )
};

IMPL_PERSISTENT_OPS( MbSimbolthThreadView )

//-------------------------------------------------------------------------------
/** \brief \ru Сортировать объекты.
           \en Sort the objects. \~
  \details \ru Сортировать аннотационные объекты. по имени компонента. \n
           \en Sort the annotative objects by the component name. \n \~
  \param[in] pf - \ru Первый объект.
                  \en The first object. \~
  \param[in] ps - \ru Второй объект.
                  \en The second object. \~
  \return \ru 0, если имена компонентов равны,\n
         -1, если имя первого объекта меньше,\n
          1, если имя первого объекта больше.
          \en 0, if the names of components are equal,\n
         -1, if the first object name is less than the second,\n
          1, if the first object name is greater than the second. \~
  \ingroup Mapping
*/   
// ---
inline int AnnotationSort( const MbSimbolthThreadView ** pf, const MbSimbolthThreadView ** ps )
{
  uint vf = (*pf)->GetComponent();
  uint vs = (*ps)->GetComponent();
  return ( (vf == vs) ? 0 : ((vf < vs) ? -1 : 1) );
}


//-------------------------------------------------------------------------------
/** \brief \ru Тип аннотированного объекта.
           \en A type of an annotated object. \~
  \details \ru Тип аннотированного объекта.\n
           \en The type of annotated object.\n \~
  \ingroup Mapping
*/
// ---
enum AnnotatedObjectType {
  aot_SymbolicThread = 0,         ///< \ru Проекционный (полный) вид. \en The projective (full) view. 
  aot_SymbolicThread_CuttedView,  ///< \ru Вид-разрез. \en The cutaway-view. 
  aot_SymbolicThread_SectionView  ///< \ru Вид-сечение. \en The section-view. 
};


//-------------------------------------------------------------------------------
/** \brief \ru Интерфейс хранилища аннотированных объектов.
           \en The interface of annotated objects storage. \~
  \details \ru Интерфейс хранилища аннотированных объектов.\n
           \en The interface of annotated objects storage.\n \~       
  \ingroup Mapping
*/
// ---
struct MATH_CLASS ItAnnObjectStore : public MbRefItem {
public:
  /** \brief \ru Получить массив резьб.
             \en Get the array of threads. \~
    \details \ru Получить массив объектов типа резьба по массиву разрезанных тел.\n
             \en Get the array of objects of thread type by array of cut solids. \n \~  
    \param[in] bodies - \ru Тип аннотированного объекта.
                        \en A type of an annotated object. \~
    \param[in] threads    - \ru Имя грани, с которой ассоциирован объект.
                            \en A face name the object is associated with. \~       
  */
  virtual bool GetMathThreads( const c3d::ConstLumpsMultiSet & bodies, RPArray<MbThread> & threads, bool draw ) = 0;
  /** \brief \ru Получить массив резьб.
             \en Get the array of threads. \~
    \details \ru Получить массив объектов типа резьба по массиву разрезанных тел.\n
             \en Get the array of objects of thread type by array of cut solids. \n \~  
    \param[in] bodies - \ru Тип аннотированного объекта.
                        \en A type of an annotated object. \~
    \param[in] threads    - \ru Имя грани, с которой ассоциирован объект.
                            \en A face name the object is associated with. \~       
  */
  virtual bool GetMathThreads( const c3d::ConstLumpsMultiSet & bodies, std::vector< SPtr<MbThread> > & threads, bool draw ) = 0;
};


//-------------------------------------------------------------------------------
/** \brief \ru Интерфейс хранилища условных обозначений.
           \en The interface of storage of conventional notations. \~
  \details \ru Интерфейс хранилища условных обозначений.\n
           \en The interface of storage of conventional notations.\n \~       
  \ingroup Mapping
*/
// ---
struct MATH_CLASS ItSymbolObjectStore : public MbRefItem
{       
  /** \brief \ru Получить массив объектов.
             \en Get the array of objects. \~
    \details \ru получить массив условных обозначений.\n
             \en get the array of conventional notations.\n \~  
    \param[in] place - \ru Видовая система координат.
                       \en A view coordinate system. \~
  */
  virtual RPArray<MbSymbol> & GetMbSymbols( const MbPlacement3D & place ) = 0;
};


//------------------------------------------------------------------------------
/** \brief \ru Множество пространственных точек для проецирования.
           \en The array of spatial points for projection. \~
  \details \ru Множество пространственных точек для проецирования.\n
           \en The array of spatial points for projection.\n \~
  \ingroup Mapping
*/
// ---
class MATH_CLASS MbSpacePoints : public MbRefItem, public TapeBase {
private:
  uint                  component; ///< \ru Компонент в котором определен набор точек. \en A component in which a set of points is defined. 
  uint16                style;     ///< \ru Стиль. \en A style. 

  MbMatrix3D            from;      ///< \ru Матрица пересчета в мир. \en A matrix of transformation to the world coordinate system. 
  MbName                name;      ///< \ru Имя набора, если есть. \en A set name, if it exists.
  SArray<MbCartPoint3D> points;    ///< \ru Точки. \en Points. 
  PArray<MbName>        names;     ///< \ru Имена точек, не копии. \en Names of points, not copies.  
  mutable bool          hidden;    ///< \ru Видимость точек для проецирования. \en Visibility of points for projection. 

private:
  MbSpacePoints(); // \ru не реализовано \en not implemented 
  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  MbSpacePoints( const MbSpacePoints & other, MbRegDuplicate * iReg );
public:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор пустого видимого массива пространственных точек.
             \en Constructor of an empty array of visible spatial points. \~  
    \param[in] _comp  - \ru Компонент в котором определен набор точек.
                        \en A component in which a set of points is defined. \~
    \param[in] _style - \ru Стиль.
                        \en A style. \~
    \param[in] _from  - \ru Матрица пересчета в мир.
                        \en A matrix of transformation to the world coordinate system. \~
    \param[in] _name  - \ru Имя набора.
                        \en A set name. \~  
  */
  MbSpacePoints( uint _comp, uint16 _style, const MbMatrix3D & _from, const MbName & _name )
    : MbRefItem(             )
    , component( _comp       )
    , style    ( _style      )
    , from     ( _from       )
    , name     ( _name       )
    , points   ( 0, 1        )
    , names    ( 0, 1, false )
    , hidden   ( false       )
  {
  }
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор пустого видимого массива пространственных точек
      без указания имени.
             \en Constructor of an empty array of visible spatial points
      without name. \~  
    \param[in] _comp  - \ru Компонент в котором определен набор точек.
                        \en A component in which a set of points is defined. \~
    \param[in] _style - \ru Стиль.
                        \en A style. \~
    \param[in] _from  - \ru Матрица пересчета в мир.
                        \en A matrix of transformation to the world coordinate system. \~     
  */
  MbSpacePoints( uint _comp, uint16 _style, const MbMatrix3D & _from )
    : MbRefItem(             )
    , component( _comp       )
    , style    ( _style      )
    , from     ( _from       )
    , name     (             )
    , points   ( 0, 1        )
    , names    ( 0, 1, false )
    , hidden   ( false       )
  {
  }
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbSpacePoints() {}

public:
  /// \ru Дать копию объекта. \en Get a copy of the object. 
  virtual MbSpacePoints & Duplicate( MbRegDuplicate * iReg = nullptr ) const;

public:
  
  /**\ru \name Функции доступа к данным.
     \en \name Functions for access to data.
      \{ */

        /// \ru Получить имя компонента. \en Get the name of a component. 
        uint          GetComponent()  const { return component; } 
        /// \ru Получить стиль. \en Get the style. 
        uint16        GetStyle()      const { return style; } 
        /// \ru Получить матрицу преобразования в мир. \en Get the matrix of transformation to the world coordinate system. 
  const MbMatrix3D  & GetMatrixFrom() const { return from; } 
        /// \ru Получить имя. \en Get the name. 
  const MbName      & GetName()       const { return name; } 

        /// \ru Получить флаг невидимости кривых для проецирования. \en Get the flag of invisibility of curves for projection. 
        bool          IsHidden() const { return hidden; }
        /// \ru Установить флаг невидимости кривых для проецирования. \en Set the flag of invisibility of curves for projection. 
        void          SetHidden( bool h ) const { hidden = h; }

  /** \} */  
  /**\ru \name Функции изменения данных.
     \en \name Functions for changing data.
      \{ */

        /** \brief \ru Добавить точку с именем.
                   \en Add a point with a name. \~
          \details \ru Добавить точку с именем.\n
            Выдает предупреждение, если у массива имен точек
            стоит флаг удаления элементов.
                   \en Add a point with a name.\n
            Generates a warning if the array of names of points
            has a flag of elements removing. \~
          \param[in] pnt - \ru Точка.
                           \en A point. \~
          \param[in] nm  - \ru Имя точки.
                           \en A point name. \~             
        */
        void          AddNamedPoint( const MbCartPoint3D & pnt, MbName * nm ); 

        /** \brief \ru Добавить точки с именами.
                   \en Add points with names. \~
          \details \ru Добавить точки с именами.\n 
            Количество точек и имен в массивах должно совпадать.
            Иначе выдает предупреждение.\n
            Последовательно добавляет точки и имена в объект
            с помощью вызова AddNamedPoint.
                   \en Add points with names.\n 
            The number of points in the array must be equal to the number of names.
            Otherwise generates a warning.\n
            Points and names are added to the object sequentially
            by calling the AddNamedPoint. \~
          \param[in] pnts - \ru Набор точек.
                            \en A point set. \~
          \param[in] nms  - \ru Набор имен.
                            \en A set of names. \~          
        */
        template <class Points>
        void          AddNamedPoints( const Points & pnts, const RPArray<MbName> & nms ); 

        /** \brief \ru Удалить точки с именами.
                   \en Remove points with names. \~
          \details \ru Удалить точки с именами.\n 
            Чистит массивы точек и имен.
                   \en Remove points with names.\n 
            Clean arrays of points and names. \~
        */
        void          RemoveNamedPoints() { points.clear(); names.clear();  } 

        /** \brief \ru Освободить лишнюю память.
                   \en Free the unnecessary memory. \~
          \details \ru Освободить лишнюю память.\n 
            Освобождает лишнюю память в массивах точек и имен.
                   \en Free the unnecessary memory.\n 
            Free the unnecessary memory in arrays of points and names. \~
        */
        void          AdjustMemory() { 
                                      points.shrink_to_fit(); names.shrink_to_fit(); 
                                      }

  /** \} */  
  /**\ru \name Доступ к точкам.
     \en \name Access to points.
      \{ */

        /// \ru Количество точек. \en The number of points. 
        size_t        GetPointsCount() const { return points.size(); } 

        /** \brief \ru Получить точки.
                   \en Get points. \~
          \details \ru Получить точки.\n 
            Добавляет точки в присланный массив.
                   \en Get points.\n 
            Add points into a given array. \~  
          \param[out] pnts - \ru Множество для добавления точек.
                             \en An array for adding of points. \~
        */
        void          GetPoints( SArray<MbCartPoint3D> & pnts ) const { pnts += points; } 

        /** \brief \ru Получить точку.
                   \en Get a point. \~
          \details \ru Получить точку по индексу.\n                
            Если индекс некорректный, то есть не меньше числа точек,
            выдается предупреждение.
                   \en Get a point by an index.\n                
            If the index is incorrect i.e. it isn't less than the number of points,
            a warning is generated. \~
          \param[in]  k   - \ru Индекс точки.
                            \en A point index. \~
          \param[out] pnt - \ru Нужная точка.
                            \en Required point. \~
          \return \ru true в случае, если индекс меньше числа точек в наборе.
                  \en returns true if the index is less than the number of points in the set. \~
        */
        bool          GetPoint( size_t k, MbCartPoint3D & pnt ) const
        {
          if ( k < points.size() ) {
            pnt = points[k];
            return true;
          }
          C3D_ASSERT_UNCONDITIONAL( false );
          return false;
        } 

  /** \} */  
  /**\ru \name Доступ к именам.
     \en \name Access to names.
      \{ */ 

        /// \ru Количество имен. \en The number of names. 
        size_t        GetNamesCount() const { return names.size(); } 

        /** \brief \ru Получить имена.
                   \en Get the names. \~
          \details \ru Получить имена.\n 
            Добавляет имена в присланный массив.
                   \en Get the names.\n 
            Add the names into a given array. \~  
          \param[out] ns - \ru Множество для добавления имен.
                           \en An array for adding of names. \~
        */
        void          GetNames( RPArray<MbName> & ns ) const { ns.AddArray( names ); } 

        /** \brief \ru Получить имя.
                   \en Get the name. \~
          \details \ru Получить имя по индексу.\n           
            Если индекс некорректный, то есть не меньше числа точек,
            вернет nullptr.
                   \en Get the name by an index.\n           
            If the index is incorrect i.e. it isn't less than the number of points,
            nullptr is returned. \~
          \param[in]  k   - \ru Индекс имени.
                            \en A name index. \~             
          \return \ru Имя по индексу из набора имен.
                  \en A name by an index from the set of names. \~
        */
  const MbName      * GetName( size_t k ) const { return ((k < names.size()) ? names[k] : nullptr); }
  /** \} */  
         
DECLARE_PERSISTENT_CLASS_NEW_DEL ( MbSpacePoints )
OBVIOUS_PRIVATE_COPY( MbSpacePoints )
};
IMPL_PERSISTENT_OPS( MbSpacePoints )

//------------------------------------------------------------------------------
// \ru добавить точку \en add a point 
// ---
inline void MbSpacePoints::AddNamedPoint( const MbCartPoint3D & pnt, MbName * nm )
{
  points.push_back( pnt );
  names.push_back( nm );

  if ( names.OwnsElem() )
  {
    C3D_ASSERT_UNCONDITIONAL( false );
    names.OwnsElem( false );
  }
}

//------------------------------------------------------------------------------
// \ru Добавить точки \en Add points 
// ---
template <class Points>
inline void MbSpacePoints::AddNamedPoints( const Points & pnts, const RPArray<MbName> & nms )
{
  size_t cnt = pnts.size();
  C3D_ASSERT( cnt == nms.size() );

  if ( cnt > 0 && cnt == nms.size() ) {
    for ( size_t k = 0; k < cnt; k++ )
      AddNamedPoint( pnts[k], nms[k] );
  }
}


//------------------------------------------------------------------------------
/** \brief \ru Множество пространственных кривых для проецирования.
           \en An array of spatial curves for projection. \~
  \details \ru Множество пространственных кривых для проецирования.\n
           \en An array of spatial curves for projection.\n \~
  \ingroup Mapping
*/
// ---
class MATH_CLASS MbSpaceCurves : public MbRefItem, public TapeBase {
private:
  uint                  component; ///< \ru Компонент в котором определен набор кривых. \en A component in which a set of curves is defined. 
  uint16                style;     ///< \ru Стиль. \en A style. 
  MbAttributeContainer  attrData;  ///< \ru Атрибуты. \en Attributes.  \~ \internal \ru По просьбе группы Приложений (Компас) \en Apps (Kompas) at request. \~ \endinternal

  MbMatrix3D            from;      ///< \ru Матрица пересчета в мир. \en A matrix of transformation to the world coordinate system. 
  MbName                name;      ///< \ru Имя набора, если есть. \en A name of set, if it exists.  
  RPArray<MbCurve3D>    curves;    ///< \ru Кривые (оригиналы, владеет по счетчику ссылок). \en Curves (originals, owns by the reference counter). 
  PArray<MbName>        names;     ///< \ru Имена кривых, не копии. \en Names of curves, not copies. 
  mutable bool          hidden;    ///< \ru Видимость кривых для проецирования. \en The visibility of curves for projection. 

private:
  MbSpaceCurves(); // \ru не реализовано \en not implemented 
public:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор пустого видимого массива пространственных кривых.
             \en Constructor of empty array of visible spatial curves. \~  
    \param[in] _comp  - \ru Компонент в котором определен набор точек.
                        \en A component in which a set of points is defined. \~
    \param[in] _style - \ru Стиль.
                        \en A style. \~
    \param[in] _from  - \ru Матрица пересчета в мир.
                        \en A matrix of transformation to the world coordinate system. \~
    \param[in] _name  - \ru Имя набора.
                        \en A name of set. \~  
  */
  MbSpaceCurves( uint _comp, uint16 _style, const MbMatrix3D & _from, const MbName & _name )
    : MbRefItem(             )
    , component( _comp       )
    , style    ( _style      )
    , attrData (             )
    , from     ( _from       )
    , name     ( _name       )
    , curves   ( 0, 1        )
    , names    ( 0, 1, false )
    , hidden   ( false       )
  {}
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор пустого видимого массива пространственных кривых без указания имени.
             \en Constructor of empty array of visible spatial curves without names. \~  
    \param[in] _comp  - \ru Компонент в котором определен набор точек.
                        \en A component in which a set of points is defined. \~
    \param[in] _style - \ru Стиль.
                        \en A style. \~
    \param[in] _from  - \ru Матрица пересчета в мир.
                        \en A matrix of transformation to the world coordinate system. \~       
  */
  MbSpaceCurves( uint _comp, uint16 _style, const MbMatrix3D & _from )
    : MbRefItem(             )
    , component( _comp       )
    , style    ( _style      )
    , attrData (             )
    , from     ( _from       )
    , name     (             )
    , curves   ( 0, 1        )
    , names    ( 0, 1, false )
    , hidden   ( false       )
  {}
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbSpaceCurves() {
    RemoveNamedCurves();
  }
private:
  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  MbSpaceCurves( const MbSpaceCurves & other, MbRegDuplicate * iReg );
public:

  /// \ru Дать копию объекта. \en Get a copy of the object. 
  virtual MbSpaceCurves & Duplicate( MbRegDuplicate * iReg = nullptr ) const;

public:
  /** \} */  
  /**\ru \name Функции доступа к данным.
     \en \name Functions for access to data.
      \{ */
        /// \ru Получить имя компонента. \en Get the name of a component. 
        uint          GetComponent()  const { return component; } 
        /// \ru Получить стиль. \en Get the style. 
        uint16        GetStyle()      const { return style; } 

        /// \ru Получить контейнер атрибутов для чтения. \en Get the attribute container for reading. 
  const MbAttributeContainer & GetAttributes() const { return attrData; }
        /// \ru Получить контейнер атрибутов для записи. \en Get the attribute container for writing. 
        MbAttributeContainer & SetAttributes()       { return attrData; }

        /// \ru Получить матрицу преобразования в мир. \en Get the matrix of transformation to the world coordinate system. 
  const MbMatrix3D  & GetMatrixFrom() const { return from; } 
        /// \ru Получить имя. \en Get the name. 
  const MbName      & GetName()       const { return name; } 

        /// \ru Получить флаг невидимости кривых для проецирования. \en Get the flag of invisibility of curves for projection. 
        bool          IsHidden() const { return hidden; }
        /// \ru Установить флаг невидимости кривых для проецирования. \en Set the flag of invisibility of curves for projection. 
        void          SetHidden( bool h ) const { hidden = h; }

  /** \} */  
  /**\ru \name Функции изменения данных.
     \en \name Functions for changing data.
      \{ */

        /** \brief \ru Добавить кривую с именем.
                   \en Add a curve with a name. \~
          \details \ru Добавить кривую с именем.\n  
            Добавляет ненулевую кривую в набор кривых.
            В случае noSameCheck = false кривая добавится в массив
            после проверки на отсутствие в нем.
            Выдает предупреждение, если у массива имен точек
            стоит флаг удаления элементов.
                   \en Add a curve with a name.\n  
            Add a non-zero curve into the set of curves.
            If noSameCheck = false a curve is added into the array
            after checking of its absence in it.
            Generates a warning if the array of names of points
            has a flag of elements removing. \~
          \param[in] curve       - \ru Кривая.
                                   \en A curve. \~
          \param[in] name        - \ru Имя точки.
                                   \en A name of point. \~    
          \param[in] noSameCheck - \ru Файл отсутствия проверки наличия кривой в массиве.
                                   \en A flag to disable checking of curve existence in the array. \~ 
        */
        void          AddNamedCurve( MbCurve3D * curve, MbName * name, bool noSameCheck = false ); 
        
        /** \brief \ru Добавить кривые с именами.
                   \en Add curves with names. \~
          \details \ru Добавить кривые с именами.\n 
            Количество кривых и имен должно совпадать.
            Иначе выдает предупреждение.\n
            Последовательно добавляет кривую с именем методом AddNamedCurve.
                   \en Add curves with names.\n 
            The count of curves must be equal to the count of names.
            Otherwise generates a warning.\n
            Adds sequentially a curve with a name by the "AddNamedCurve" method. \~
          \param[in] curves      - curves.
          \param[in] names       - \ru Имя точки.
                                   \en A name of point. \~    
          \param[in] noSameCheck - \ru Файл отсутствия проверки наличия кривой в массиве.
                                   \en A flag to disable checking of curve existence in the array. \~ 
        */
        template <class Curves>
        void          AddNamedCurves( const Curves & curves, const RPArray<MbName> & names, bool noSameCheck = false );

        /** \brief \ru Удалить кривые с именами.
                   \en Remove curves with names. \~
          \details \ru Удалить кривые с именами.\n 
            Чистит массивы кривых и имен.
                   \en Remove curves with names.\n 
            Cleans arrays of curves and names. \~ 
        */
        void          RemoveNamedCurves(); 

        /** \brief \ru Освободить лишнюю память.
                   \en Free the unnecessary memory. \~
          \details \ru Освободить лишнюю память.\n 
            Освобождает лишнюю память в массивах с кривыми и именами.
                   \en Free the unnecessary memory.\n 
            Frees the unnecessary memory in arrays of points and names. \~
        */
        void          AdjustMemory() {
                                        curves.shrink_to_fit(); names.shrink_to_fit(); 
                                      }

  /** \} */  
  /**\ru \name Доступ к кривым.
     \en \name Access to curves.
      \{ */

        /// \ru Количество кривых. \en The number of curves. 
        size_t        GetCurvesCount() const { return curves.size(); } 

        /** \brief \ru Получить кривые.
                   \en Get the curves. \~
          \details \ru Получить кривые.\n 
            Добавляет кривые в присланный массив.
                   \en Get the curves.\n 
            Adds curves into a given array. \~
          \param[out] crvs - \ru Множество для добавления кривых.
                             \en An array for curves adding. \~
        */
        void          GetCurves( RPArray<MbCurve3D> & crvs ) const { crvs.AddArray( curves ); } 

        /** \brief \ru Получить кривую.
                   \en Get a curve. \~
          \details \ru Получить кривую по индексу.\n
                   \en Get a curve by an index.\n \~             
          \param[in] k - \ru Индекс кривой.
                         \en A curve index. \~
          \return \ru Указатель на кривую, если индекс меньше количества кривых,\n
                  иначе nullptr.
                  \en A pointer to a curve, if the index is less than the number of curves,\n
                  otherwise nullptr is returned. \~
        */
  const MbCurve3D   * GetCurve( size_t k ) const { return ((k < curves.size()) ? curves[k] : nullptr); }

  /** \} */  
  /**\ru \name Доступ к именам.
     \en \name Access to names.
      \{ */

        /// \ru Количество имен. \en The number of names. 
        size_t        GetNamesCount() const { return names.size(); } 
        
        /** \brief \ru Получить имена.
                   \en Get the names. \~
          \details \ru Получить имена.\n 
            Добавляет имена в присланный массив.
                   \en Get the names.\n 
            Add the names into a given array. \~
          \param[out] ns - \ru Множество для добавления имен.
                           \en An array for adding of names. \~
        */
        void          GetNames( RPArray<MbName> & ns ) const { ns.AddArray( names ); }

        /** \brief \ru Получить имя.
                   \en Get the name. \~
          \details \ru Получить имя по индексу.\n
                   \en Get the name by an index.\n \~             
          \param[in] k - \ru Индекс имени.
                         \en A name index. \~
          \return \ru Указатель на имя, если индекс меньше количества имен,\n
                  иначе nullptr.
                  \en A pointer to a name, if the index is less than the number of curves,
                  otherwise nullptr is returned. \~
        */
  const MbName      * GetName( size_t k ) const { return ((k < names.size()) ? names[k] : nullptr); } ///< \ru Получить имя. \en Get the name.
  /** \} */  
 
  DECLARE_PERSISTENT_CLASS_NEW_DEL ( MbSpaceCurves )
  OBVIOUS_PRIVATE_COPY( MbSpaceCurves )
};
IMPL_PERSISTENT_OPS( MbSpaceCurves )

//------------------------------------------------------------------------------
// \ru Добавить кривую \en Add a curve 
// ---
inline
void MbSpaceCurves::AddNamedCurve( MbCurve3D * crv, MbName * nm, bool noSameCheck )
{
  if ( crv != nullptr && (noSameCheck || curves.FindIt( crv ) == SYS_MAX_T ) ) {
    curves.push_back( crv );
    crv->AddRef();
    names.push_back( nm );

    if ( names.OwnsElem() )
    {
      C3D_ASSERT_UNCONDITIONAL( false );
      names.OwnsElem( false );
    }
  }
}

//------------------------------------------------------------------------------
// \ru Добавить кривые \en Add curves 
// ---
template <class Curves>
void MbSpaceCurves::AddNamedCurves( const Curves & crvs, const RPArray<MbName> & nms, bool noSameCheck )
{
  size_t cnt = crvs.size();
  C3D_ASSERT( cnt == nms.size() );

  if ( cnt > 0 && cnt == nms.size() ) {
    for ( size_t k = 0; k < cnt; k++ )
      AddNamedCurve( crvs[k], nms[k], noSameCheck );
  }
}


//------------------------------------------------------------------------------
/** \brief \ru Тело с признаком разрезки в производном виде.
           \en Solid with a sing cutting in derive view. \~
    \ingroup Mapping
*/ 
// --
struct MATH_CLASS MbCutLump: public MbLump {
public:
  /** brief \ru Нужно ли разрезать тело при построении производного вида 
                (выносного элемента, местного разреза/сечения)?
            \en Whether it is necessary to cut solid in a derive view 
                (a local view, a detail view, the local cutaway/section) or not?
  */
  bool willCutOnDeriveView;

public:
  /// \ru Пустой конструктор. \en Empty constructor. 
  MbCutLump() 
    : MbLump()
    , willCutOnDeriveView( true )
  {}
  /// \ru Конструктор по данным. \en Constructor by data. 
  MbCutLump( const MbSolid & _solid, const MbMatrix3D & _from, uint _comp = 0, size_t _ident = SYS_MAX_T )
    : MbLump( _solid, _from, _comp, _ident )
    , willCutOnDeriveView( true )
  {}

  /// \ru Деструктор. \en Destructor. 
  virtual ~MbCutLump() {};

public:
  /// \ru Тело с признаком резки на базовом виде? \en Solid with cutting type on base view?
  virtual bool      IsCutLump() const { return true; } 
  // \ru Разрезать тело в производном виде. \en Cut solid on derive view.
  virtual bool      WillCutOnDeriveView() const { return willCutOnDeriveView; }

          /** \brief \ru Установить признак разрезки на производном виде.
                     \en Set type of cutting the solid on derive view. \~
            \details \ru Установить признак разрезки на производном виде.\n
                     \en Set type of cutting the solid on derive view. \n \~
            \param[in] cut      - \ru Разрезать тело.
                                  \en Cut solid.
          */
          void SetCuttingTypeOnDeriveView ( bool cut ) { willCutOnDeriveView = cut; }
};


//------------------------------------------------------------------------------
/** \brief \ru Тело или набор тел, определенных в системе координат, с признаком разрезания в сечениях и разрезах.
           \en A solid or a set of solids which are defined in a coordinate system with an attribute of cutting in sections and cutaways. \~
  \details \ru Тело или набор тел, определенных в системе координат, с признаком разрезания в сечениях и разрезах.\n   
    - При построении разрезов:\n
      1) если willCut = true - строится разрез тел;\n
      2) если willCut = false - строится полный вид тел.\n
    - При построении сечений:\n
      1) если willCut = true - строится сечение тел;\n
      2) если willCut = false - тела не учитываются в результате построения.\n
           \en A solid or a set of solids which are defined in a coordinate system with an attribute of cutting in sections and cutaways.\n   
    - When constructing the cutaways:\n
      1) if willCut = true the cutaway of solids is built;\n
      2) if willCut = false the full view of solids is built;\n
    - When constructing the sections:\n
      1) if willCut = true the cutaway of solids is built;\n
      2) if willCut = false the solids are not considered in the result of the construction.\n \~

  \internal  <TABLE border="1">
            \ru <TR><TD><b>willCut</b></TD><TD><b>разрез</b></TD><TD><b>строим вид-разрез (не вид-сечение)</b></TD></TR> 
                <TR><TD>   true       </TD><TD>   true      </TD><TD>   разрез (режем, а потом проецируем)</TD></TR>
                <TR><TD>   false      </TD><TD>   true      </TD><TD>   полный вид (остальные режем, а это остается целым)</TD></TR>
                <TR><TD>   true       </TD><TD>   false     </TD><TD>   сечение (проецируем только сечение)</TD></TR>
                <TR><TD>   false      </TD><TD>   false     </TD><TD>   не рисовать вообще (при сечении тело, которое не должно быть разрезано, не проецируется вообще)</TD></TR>
  </TABLE>
            \en <TR><TD><b>willCut</b></TD><TD><b>cutaway</b></TD><TD><b>build cutaway-view (no section-view)</b></TD></TR> 
                <TR><TD>   true       </TD><TD>   true       </TD><TD>   cutaway (cut and then project)</TD></TR>
                <TR><TD>   false      </TD><TD>   true       </TD><TD>   full view (cut the others, but it remains intact)</TD></TR>
                <TR><TD>   true       </TD><TD>   false      </TD><TD>   section (project only the section)</TD></TR>
                <TR><TD>   false      </TD><TD>   false      </TD><TD>   do not draw at all (a solid which must not be cut, isn't projected at all while cutting)</TD></TR>
  </TABLE> \~
  \endinternal
\ru Является наследником от объекта MbLump и содержит указатель на контейнер объектов MbLump.\n   
    Если в объекте одно тело с матрицей, то массив lumps пустой.\n
    Если в разрезе или сечении на подсборке стоит флаг "не разрезать", то все тела подсборки входят в один
    объект MbMappingLumps с общим флагом разрезки. В этом случае первое тело с матрицей лежит в базовом MbLump,
    а остальные в контейнере объектов MbLump.
\en The MbMappingLumps is an inheritor of the MbLump object and contains the pointer to a container of MbLump objects.\n   
    If the object contains one solid with a matrix the lumps array is empty.\n
    If the "do not cut" flag is in cutaway or section on subassembly all solids of subassembly are in the same
    MbMappingLumps object with common flag of cutaway. In this case the first solid with a matrix is in the basic MbLump
    and the others are in the container of MbLump objects. \~
  \ingroup Mapping
*/     
// \ru Наследование от MbLump нужно в 2D \en The inheritance from the MbLump is needed in 2D. 
// --
struct MATH_CLASS MbMappingLumps : public MbCutLump {

private:
  /// \ru Тела с признаками резки в производном виде. \en Solids with signs of cutting in derive view.
  c3d::LumpsSPtrVector * lumps; // \ru может быть нулем \en can be zero. 

  /** brief \ru Нужно ли разрезать тело при построении базового вида (разреза, сечения)?
                Для всех тел в наборе.
            \en Whether it is necessary to cut solid in a base view (cutaway, section) or not?
                For all solids.
  */
  bool willCut;

public:       
  /** \brief \ru Конструктор по данным.
             \en Constructor by data. \~
    \details \ru Конструктор по одному телу.
             \en Constructor by one solid. \~
    \param[in] _solid   - \ru Тело.
                          \en A solid. \~
    \param[in] _from    - \ru Матрица перевода в глобальную систему координат.
                          \en A matrix of translation to the global coordinate system. \~
    \param[in] _willCut - \ru Признак разрезки тела в базовом виде.
                          \en The attribute of a solid cutaway in base view. \~
    \param[in] _comp    - \ru Компонент.
                          \en A component. \~
    \param[in] _ident   - \ru Идентификатор.
                          \en An identifier. \~
  */
  MbMappingLumps( const MbSolid & _solid, const MbMatrix3D & _from, bool _willCut, uint _comp = 0, size_t _ident = SYS_MAX_T )
    : MbCutLump ( _solid, _from, _comp, _ident )
    , lumps     ( nullptr                         )
    , willCut   ( _willCut                     )
  {
  }

  /** \brief \ru Конструктор по данным.
             \en Constructor by data. \~
    \details \ru Конструктор по набору тел.\n
      Захватывает тело MbSolid из первого элемента _lumps
      и остальные элементы _lumps методом AddRef().\n
      Если в _lumps один элемент, массив lumps остается nullptr.\n
      Если в _lumps нет элементов, тело MbSolid в базовом объекте = nullptr. Таких объектов быть не должно.
             \en Constructor by a set of solids.\n
      Captures MbSolid solid from the first element of the _lumps
      and the other elements of the _lumps by AddRef() method.\n
      If the _lumps contains one element the lumps array remains nullptr.\n
      If the _lumps doesn't contain any elements the MbSolid solid in the base object = nullptr. These objects should not be. \~
    \param[in] _lumps - \ru Контейнер тел с матрицами преобразования в глобальную систему координат,.
      не должен быть пустым контейнером.
                        \en A container of solids with the matrices of transformation to the global coordinate system
      should not be an empty container. \~
  */
  template <class LumpsVector>
  MbMappingLumps( const LumpsVector & _lumps )
    : MbCutLump()
    , lumps( nullptr )
    , willCut( false ) // конструктор по нескольким телам только в случае "не рассекать"
  {
    size_t count = _lumps.size();
    C3D_ASSERT( count > 0 && _lumps[0] != nullptr );

    if ( count > 0 && _lumps[0] != nullptr ) {
      _from = _lumps[0]->GetMatrixFrom();
      _component = _lumps[0]->GetComponent();
      _identifier = _lumps[0]->GetIdentifier();
      _item = &_lumps[0]->GetItem();

      willCutOnDeriveView = _lumps[0]->WillCutOnDeriveView();

      if ( count > 1 ) {
        lumps = new c3d::LumpsSPtrVector();
        for ( size_t i = 1; i < count; ++i ) {
          MbLump * lump = _lumps[i];
          if ( lump != nullptr )
            lumps->push_back( c3d::LumpSPtr(lump) );
        }
      }
    }
  }

  /// \ru Деструктор. \en Destructor. 
  virtual ~MbMappingLumps();
  
  /** \brief \ru Число тел.
             \en The number of solids. \~
    \details \ru Число тел.\n
      Минимальное количество - 1 тело. В этом случае массив lumps = nullptr.
      В случае, если массив lumps != nullptr, количество тел равно количеству
      элементов в массиве плюс один.
             \en The number of solids.\n
      Minimal number = 1 solid. In this case the lumps array is nullptr.
      In a case when the lumps array isn't nullptr the number of solids is equal to
      the number of elements in the array plus one. \~
    \return \ru Число тел.
            \en The number of solids. \~
  */
  size_t Count() const {
    size_t res = 1;   
    if ( lumps != nullptr )
      res += lumps->size();
    return res;
  }

  /** \brief \ru Тело по индексу.
             \en A solid by an index. \~
    \details \ru Тело по индексу.\n
      По индексу 0 выдается базовый объект.\n
      По индексу i выдается объект из массива lumps с индексом i-1.\n
      Индекс проверяется на корректность. 
      В случае некорректного индекса возвращает nullptr.
             \en A solid by an index.\n
      The basic object is given by the "0" index.\n
      An object with the index i - 1 from the lumps array is issued by the index i.\n
      An index is validated for correctness. 
      In a case of an incorrect index the method returns nullptr. \~
    \return \ru Указатель на тело с матрицей.
            \en A pointer to a solid with a matrix. \~
  */
  MbLump * operator []( size_t ind ) {
    if ( ind == 0 )
      return static_cast<MbLump *>( this ); 
    else if ( lumps != nullptr && ind - 1 < lumps->size() )
      return lumps->operator []( ind - 1 );
    return nullptr;
  } 

  /** \brief \ru Тело по индексу.
             \en A solid by an index. \~
    \details \ru Тело по индексу.\n
      По индексу 0 выдается базовый объект.\n
      По индексу i выдается объект из массива lumps с индексом i-1.\n
      Индекс проверяется на корректность. 
      В случае некорректного индекса возвращает nullptr.
             \en A solid by an index.\n
      The basic object is given by the "0" index.\n
      An object with the index i - 1 from the lumps array is issued by the index i.\n
      An index is validated for correctness. 
      In a case of an incorrect index the method returns nullptr. \~
    \return \ru Константный указатель на тело с матрицей.
            \en A constant pointer to a solid with a matrix. \~
  */
  const MbLump * operator []( size_t ind ) const
  {
    if ( ind == 0 )
      return static_cast<const MbLump *>( this ); 
    else if ( lumps != nullptr && ind - 1 < lumps->size() )
      return lumps->operator []( ind - 1 );
    return nullptr;
  } 

  void ChangeLump( size_t ind, MbLump * newLump )
  {
    if ( ind == 0 ) {
      _item       = &newLump->GetItem();
      _from       =  newLump->GetMatrixFrom();
      _component  =  newLump->GetComponent();
      _identifier =  newLump->GetIdentifier();
    }
    else if ( lumps != nullptr && ind - 1 < lumps->size() ) {
      (*lumps)[ind - 1] = newLump;
    }
  }

  /** \brief \ru Базовый ли объект.
             \en Whether it is the basic object or not. \~
    \details \ru Базовый ли объект.\n  
      Возвращает false. Возвращает true у объекта MbLump.
             \en Whether it is the basic object or not.\n  
      Returns false. Returns true at the MbLump object. \~
    \return false
  */
  virtual bool IsBaseLump() const { return false; }

  // \ru Тело с признаком резки? \en Solid with cutting type?
  virtual bool IsMappingLump() const { return true; } 

          /** \brief \ru Установить признак разрезки.
                     \en Set type of cutting the solid. \~
            \details \ru Установить признак разрезки.\n
                     \en Set type of cutting the solid. \n \~
            \param[in] baseView - \ru Базовый вид или производный.
                                  \en Base or derive view. \~
            \param[in] cut      - \ru Разрезать тело.
                                  \en Cut solid.
          */
          void SetCuttingType( bool cut ) { willCut = cut; }

          /** \brief \ru Признак разрезки.
                     \en Type of cutting the solid. \~
            \details \ru Признак разрезки.\n
                     \en Type of cutting the solid. \n \~
          */
          bool WillCut() const { return willCut; }

OBVIOUS_PRIVATE_COPY( MbMappingLumps ) // \ru Объявление конструктора копирования и оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration without implementation of the copy-constructor and assignment operator to prevent an assignment by default. 
};


//------------------------------------------------------------------------------
/** \brief \ru Установить значение флагов рассеченности.
           \en Set cutting flags. \~
  \details \ru Установить значение флагов рассеченности.\n
           \en Set cutting flags.\n \~
  \ingroup Mapping
*/
// ---
MATH_FUNC(void) SetCuttingFlags(       RPArray<MbLump> & lumps, 
                                 const SArray<uint>    * baseNotSected, 
                                 const SArray<uint>    * deriveNotSected );


//------------------------------------------------------------------------------
/** \brief \ru Полигон с телом.
           \en A polygon with a solid. \~
  \details \ru Полигон с телом.\n
           \en A polygon with a solid.\n \~
  \ingroup Mapping
*/
// ---
class MATH_CLASS MbPolygon3DSolid {
private:
  uint          component; ///< \ru Имя компонента, в котором определено тело. \en A name of a component in which a solid is defined. 
  MbPolygon3D * polygon;   ///< \ru Полигон. \en A polygon. 

public:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по имени компонента и полигону.\n
             \en Constructor by a component name and a polygon.\n \~  
    \param[in] _comp  - \ru Имя компонента.
                        \en A component name. \~
    \param[in] _polyg - \ru Полигон.
                        \en A polygon. \~
    \return false
  */
  MbPolygon3DSolid( uint _comp, MbPolygon3D &_polyg )
    : component( _comp   )
    , polygon  ( &_polyg )
  {
  }
  /// \ru Деструктор. \en Destructor. 
  ~MbPolygon3DSolid() {
    if ( polygon != nullptr )
      delete polygon;
  }

  /// \ru Получить полигон. \en Get the polygon. 
  MbPolygon3D * GetPolygon  () const { return polygon; } 
  /// \ru Получить имя компонента. \en Get the name of a component. 
  uint          GetComponent() const { return component;  } 
  /// \ru Занулить полигон без удаления. \en Reset polygon without removal. 
  void          DoNotDeletePolyg() { polygon = nullptr; }

OBVIOUS_PRIVATE_COPY( MbPolygon3DSolid )
};


//------------------------------------------------------------------------------
/** \brief \ru Тип ассоциативного вида.
           \en A type of an associative view. \~
  \details \ru Тип ассоциативного вида. \n
           \en A type of an associative view. \n \~
  \ingroup Mapping
*/
// ---
enum MbeMapViewType { 
  mvt_View,    ///< \ru Вид. \en A view. 
  mvt_Cut,     ///< \ru Разрез. \en A cutaway. 
  mvt_Section  ///< \ru Сечение. \en A section. 
};


//------------------------------------------------------------------------------
/** \brief \ru Информация о виде.
           \en The information about a view. \~
  \details \ru Информация об ассоциативном виде. Применяется для передачи
    информации о виде при построении местного вида\разреза или
    выносного элемента.
           \en The information about an associative view. Used for transfer
    The information about a view in constructing the local view(cutaway) or
    detail view. \~
  \ingroup Mapping
*/
// ---
class MATH_CLASS MbMapViewInfo {
using LimitDepthMode = MbMapVisibilityMode::LimitDepthMode;
private:
  MbeMapViewType         viewType;
  LimitDepthMode         limitDepthMode;  ///< \ru Настройки проецирования объектов по глубине видимимости. \en Mapping mode by visibility depth.
  double                 visibilityDepth; ///< \ru Глубина видимости. \en Visibility depth.
  MbPlacement3D          viewPlace;       ///< \ru Плоскость разреза для viewType = mvt_Cut, viewType = mvt_Section \en A cutaway plane for viewType = mvt_Cut, viewType = mvt_Section
  SPtr<MbContourOnPlane> secContour;      ///< \ru Для сложного разреза. \en For a complex section.

public:
  /// \ru Конструктор. \en Constructor.
  MbMapViewInfo( MbeMapViewType type, const MbPlacement3D & place )
    : viewType        ( type                )
    , limitDepthMode  ( LimitDepthMode::off )
    , visibilityDepth ( 0.0                 )
    , viewPlace       ( place               )
    , secContour      (                     )
  {}

  /// \ru Конструктор. \en Constructor.
  explicit MbMapViewInfo( MbeMapViewType type )
    : viewType        ( type                )
    , limitDepthMode  ( LimitDepthMode::off )
    , visibilityDepth ( 0.0                 )
    , viewPlace       (                     )
    , secContour      (                     )
  {}

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по параметрам.\n
             \en Constructor by parameters.\n \~
    \param[in] type  - \ru Тип ассоциативного вида.
                       \en Type of an associative view. \~
    \param[in] place - \ru Плоскость разреза для viewType.
                       \en Cutaway plane for viewType. \~
    \param[in] mode -  \ru Настройки проецирования объектов по глубине видимимости.
                       \en Mapping mode by visibility depth. \~
    \param[in] depth - \ru Глубина видимости.
                       \en Visibility depth. \~
*/
  MbMapViewInfo( MbeMapViewType type, const MbPlacement3D & place, LimitDepthMode mode, double depth )
    : viewType        ( type  )
    , limitDepthMode  ( mode  )
    , visibilityDepth ( depth )
    , viewPlace       ( place )
    , secContour      (       )
  {}

  ~MbMapViewInfo()
  {}

  /// \ru Тип ассоциативного вида. \en A type of an associative view. 
  MbeMapViewType           GetViewType()  const { return viewType;  }

  /// \ru Плоскость вида, разреза или сечения. \en The plane of a view, a cutaway or a section. 
  const MbPlacement3D &    GetPlacement() const { return viewPlace; }
  /// \ru Плоскость вида, разреза или сечения. \en The plane of a view, a cutaway or a section. 
  void                     SetPlacement( const MbPlacement3D & newPlace ) { viewPlace = newPlace; }

  /// \ru Выдать секущий контур. \en Get section contour.
  const MbContourOnPlane * GetSectionContour() const { return secContour; }
  /// \ru Установить секущий контур. \en Set section contour. 
  bool                     SetSectionContour( const MbContour & contour, const MbPlacement3D & place );
  /// \ru Получить тип ограничения. \en Get constraint mode.
  LimitDepthMode GetLimitDepthMode() const { return limitDepthMode; }
  /// \ru Получить глубину видимости. \en Get visibility depth.
  double GetVisibilityDepth() const { return visibilityDepth; }

OBVIOUS_PRIVATE_COPY( MbMapViewInfo )
};


#endif // __MAP_LUMP_H
