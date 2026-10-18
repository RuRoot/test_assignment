////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Мультилиния.
         \en Multiline. \~

*/
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// \ru Заголовочный файл мультилинии содержит следующие разделы. \en Header file of multiline contains the following sections. 
// \ru _1_  EnMLVertexTracingType - тип обхода углов в вершине мультилинии \en _1_  EnMLVertexTracingType - type of traverse of corners at a vertex of multiline 
// \ru _2_  EnMLInnerTipType - тип внутренней законцовки мультилинии \en _2_  EnMLInnerTipType - type of inner tip of multiline 
// \ru _3_  EnMLTipType - тип законцовки мультилинии \en _3_  EnMLTipType - type of tip of multiline 
// \ru _4_  StMLTipParams - структура параметров законцовки мультилинии \en _4_  StMLTipParams - tip of multiline parameters structure 
// \ru _5_  StVertexOfMultilineInfo - информация о вершине мультилинии \en _5_  StVertexOfMultilineInfo - information about vertex of multiline 
// \ru _6_  MbMultiline - класс мультилиния \en _6_  MbMultiline - multiline class 
// \ru _7_  Функция построения скругления базовой кривой мультилинии \en _7_  Function for multiline base curve fillet construction 
// \ru _8_  Функция построения фаски базовой кривой мультилинии \en _8_  Function for multiline base curve chamfer construction 
// \ru _9_  Внеклассные функции расчета/учета радиусов кривизны \en _9_  Out-of-class functions for curvature radii calculation/consideration 
// \ru _10_ Внеклассная функция гладкого стыка двух последовательных кривых \en _10_ Out-of-class function for smooth joint of two consecutive curves 
// \ru _11_ Разбить мультилинию на две части \en _11_ Split multiline into two pieces 
// \ru _12_ Разбить мультилинию на N равных частей \en _12_ Split multiline into N equal pieces 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __MULTILINE_H
#define __MULTILINE_H


#include <templ_css_array.h>
#include <mb_matrix.h>
#include <cur_nurbs.h>
#include <cur_contour.h>
#include <cur_contour_with_breaks.h>


//------------------------------------------------------------------------------
// _1_ 
/** \brief \ru Тип обхода углов.
           \en Type of traverse of corners. \~
  \details \ru Тип обхода углов в вершине мультилинии.\n
           \en Type of traverse of corners at a vertex of multiline.\n \~
  \ingroup Algorithms_2D
*/ // ---
enum EnMLVertexTracingType {
  mvt_ShearType,          ///< \ru Обход срезом. \en Traverse by shear. 
  mvt_FilletType,         ///< \ru Обход со скруглением. \en Traverse by fillet. 
  mvt_SpecFilletType,     ///< \ru Обход со скруглением заданным радиусом. \en Traverse by fillet with a given radius. 
  // \ru ДОБАВЛЕНИЕ ТОЛЬКО В КОНЕЦ!!! \en ADDITION ONLY TO THE END!!! 
};  


//------------------------------------------------------------------------------
// _2_ 
/** \brief \ru Тип внутренней законцовки.
           \en Type of inner tip. \~
  \details \ru Тип внутренней законцовки мультилинии.\n
           \en Type of inner tip of multiline.\n \~
  \ingroup Algorithms_2D
*/ // ---
enum EnMLInnerTipType {
  mit_UndefTip,      ///< \ru Законцовки нет. \en No tip. 
  mit_VerticesTip,   ///< \ru Законцовка между соответствующими вершинами. \en Tip between corresponding vertices. 
  mit_LinearTip,     ///< \ru Линейная законцовка. \en Linear tip. 
  mit_ArcTip,        ///< \ru Дуговая законцовка. \en Arc tip. 
  // \ru ДОБАВЛЕНИЕ ТОЛЬКО В КОНЕЦ!!! \en ADDITION ONLY TO THE END!!! 
};  


//------------------------------------------------------------------------------
// _3_ 
/** \brief \ru Тип законцовки.
           \en Type of tip. \~
  \details \ru Тип законцовки мультилинии.\n
           \en Type of tip of multiline.\n \~
  \ingroup Algorithms_2D
*/ // ---
enum EnMLTipType {
  mtt_UndefTip,    ///< \ru Законцовки нет. \en No tip. 
  mtt_LinearTip,   ///< \ru Линейная законцовка. \en Linear tip. 
  mtt_ArcTip,      ///< \ru Дуговая законцовка. \en Arc tip. 
  mtt_PolylineTip, ///< \ru Ломаная законцовка. \en Polyline tip. 
  mtt_ObliqueTip,  ///< \ru Наклонная законцовка. \en Inclined tip. 
  // \ru ДОБАВЛЕНИЕ ТОЛЬКО В КОНЕЦ!!! \en ADDITION ONLY TO THE END!!! 
};     


//------------------------------------------------------------------------------
// _4_ 
/** \brief \ru Структура параметров законцовки.
           \en Tip parameters structure. \~
  \details \ru Структура параметров законцовки мультилинии.\n
    Изменять данные объекта можно только из MbMultiline.
           \en Multiline tip parameters structure.\n
    Object data can be changed only from MbMultiline. \~
  \ingroup Algorithms_2D
*/
struct MATH_CLASS StMLTipParams {
friend class MbMultiline;
private:
  EnMLTipType tipType;  ///< \ru Тип законцовки. \en Type of tip. 

  /** \brief \ru Параметр законцовки.
             \en Parameter of tip. \~
      \details \ru Для mtt_UndefTip неопределен,\n
                   для mtt_LinearTip - расстояние от конца ЛМ до законцовки,\n
                   для mtt_ArcTip - расстояние от конца ЛМ до вершины дуги законцовки,\n
                   для mtt_PolylineTip - расстояние от конца ЛМ до вершины угла законцовки,\n
                   для mtt_ObliqueTip - угол поворота нормали от конца мультилинии (в радианах).
               \en Undefined for mtt_UndefTip,\n 
                   for mtt_LinearTip - distance from the end of multiline to the tip,\n 
                   for mtt_ArcTip - distance from the end of multiline to the vertex of arc of tip,\n 
                   for mtt_PolylineTip - distance from the end of multiline to the vertex of corner of tip,\n 
                   for mtt_ObliqueTip - angle of rotation of normal from the end of multiline (in radians).\~
  */
  double      tipParam;

public:
  StMLTipParams();      ///< \ru Умолчательный конструктор. \en Default constructor. 
  StMLTipParams( const StMLTipParams & other             ); ///< \ru Копирующий конструктор. \en Copy-constructor. 
  
  /** \brief \ru Конструктор по типу законцовки и параметру законцовки.
             \en Constructor by type of tip and parameter of tip. \~
    \details \ru Конструктор по типу законцовки и параметру законцовки.\n
             \en Constructor by type of tip and parameter of tip.\n \~  
    \param[in] _tipType  - \ru Тип законцовки.
                           \en Type of tip. \~
    \param[in] _tipParam - \ru Параметр законцовки, зависит от типа законцовки:\n
                           для mtt_UndefTip неопределен,\n
                           для mtt_LinearTip - расстояние от конца ЛМ до законцовки,\n
                           для mtt_ArcTip - расстояние от конца ЛМ до вершины дуги законцовки,\n
                           для mtt_PolylineTip - расстояние от конца ЛМ до вершины угла законцовки,\n
                           для mtt_ObliqueTip - угол поворота нормали от конца мультилинии (в радианах).
                           \en Parameter of tip, depends on type of tip:\n
                           for mtt_UndefTip is undefined,\n
                           for mtt_LinearTip - distance from the end of multiline to the tip,\n
                           for mtt_ArcTip - distance from the end of multiline to the vertex of arc of tip,\n
                           for mtt_PolylineTip - distance from the end of multiline to the vertex of corner of tip,\n
                           for mtt_ObliqueTip - angle of rotation of normal from the end of multiline (in radians). \~               
  */
  StMLTipParams( EnMLTipType _tipType, double _tipParam );

public:
 
  /**\ru \name Функции доступа к данным
     \en \name Functions for access to data
      \{ */ 
  /// \ru Тип законцовки. \en Type of tip. 
  EnMLTipType  GetTipType () const { return tipType;     } 
  /// \ru Параметр законцовки. \en Parameter of tip. 
  double       GetTipParam() const { return tipParam;    } 
  /** \} */   
  /**\ru \name Операторы сравнения
     \en \name Comparison operators 
      \{ */ 
  /// \ru Оператор сравнения. \en Comparison operator. 
  bool         operator ==( const StMLTipParams & ) const; 
  /// \ru Оператор сравнения. \en Comparison operator. 
  bool         operator !=( const StMLTipParams & ) const; 
  /** \} */   

protected:  
  
  /**\ru \name Функции инициализации
     \en \name Initialization functions
  \{ */   

  /// \ru Инициализация по структуре параметров законцовки. \en Initialization by tip parameters structure. 
  void Init          ( const StMLTipParams & other            );

  /** \brief \ru Инициализация по типу законцовки и параметру законцовки.
             \en Initialization by type of tip and parameter of tip. \~
    \details \ru Инициализация по типу законцовки и параметру законцовки.\n
             \en Initialization by type of tip and parameter of tip.\n \~  
    \param[in] _tipType  - \ru Тип законцовки.
                           \en Type of tip. \~
    \param[in] _tipParam - \ru Параметр законцовки, зависит от типа законцовки:\n
                           для mtt_UndefTip неопределен,\n
                           для mtt_LinearTip - расстояние от конца ЛМ до законцовки,\n
                           для mtt_ArcTip - расстояние от конца ЛМ до вершины дуги законцовки,\n
                           для mtt_PolylineTip - расстояние от конца ЛМ до вершины угла законцовки,\n
                           для mtt_ObliqueTip - угол поворота нормали от конца мультилинии (в радианах).
                           \en Parameter of tip, depends on type of tip:\n
                           for mtt_UndefTip is undefined,\n
                           for mtt_LinearTip - distance from the end of multiline to the tip,\n
                           for mtt_ArcTip - distance from the end of multiline to the vertex of arc of tip,\n
                           for mtt_PolylineTip - distance from the end of multiline to the vertex of corner of tip,\n
                           for mtt_ObliqueTip - angle of rotation of normal from the end of multiline (in radians). \~               
  */
  void Init          ( EnMLTipType _tipType, double _tipParam );

  /** \} */   
  /**\ru \name Функции изменения данных
     \en \name Functions for changing data
      \{ */   

  /** \brief \ru Изменить тип законцовки.
             \en Change type of tip. \~
    \details \ru Изменить тип законцовки мультилинии.\ n
             \en Change type of tip of multiline.\n \~      
    \param[in] othTipType - \ru Новый тип законцовки.
                            \en New type of tip. \~
    \return \ru false, если старое значение типа совпадает со значением othTipType.
            \en False if the old value of type coincides with the value of othTipType. \~
  */
  bool ChangeTipType ( EnMLTipType othTipType                 ); 

  /** \brief \ru Изменить параметр законцовки.
             \en Change parameter of tip. \~
    \details \ru Изменить параметр законцовки мультилинии.\n
             \en Change parameter of tip of multiline.\n \~      
    \param[in] othTipParam - \ru Новый параметр законцовки.
                             \en New parameter of tip. \~
    \return \ru false, если старое значение параметра совпадает со значением othTipParam.
            \en False if the old value of parameter coincides with the value of othTipParam. \~
  */
  bool ChangeTipParam( double      othTipParam                ); 
  
  /** \brief \ru Трансформация.
             \en Transformation. \~
    \details \ru Преобразование объекта согласно матрице.\n
             \en Transform object according to the matrix.\n \~      
    \param[in] matr - \ru Матрица трансформации.
                      \en Transformation matrix. \~
  */
  void Transform     ( const MbMatrix & matr                  );
  /** \} */ 

private:
  /// \ru Оператор присваивания. \en Assignment operator. 
  void operator =    ( const StMLTipParams &                  ); 

  KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( StMLTipParams, MATH_FUNC_EX )
}; // StMLTipParams


//------------------------------------------------------------------------------
// _5_ 
/** \brief \ru Информация о вершине.
           \en Information about a vertex. \~
  \details \ru Информация о вершине мультилинии.\n
    Изменять данные объекта можно только из MbMultiline.
           \en Information about a vertex of multiline.\n
    Object data can be changed only from MbMultiline. \~
  \ingroup Algorithms_2D
*/
struct MATH_CLASS StVertexOfMultilineInfo {
friend class MbMultiline;
private:
  bool                   smoothJoint;   ///< \ru Флаг гладкого стыка в вершине сегментов базовой линии мультилинии \en Flag of multiline base line segments smooth joint at a vertex 
                                        ///< \ru (только для сплайнов). \en (only for splines). 
  EnMLVertexTracingType  tracingType;   ///< \ru Тип обхода углов в вершине мультилинии. \en Type of traverse of corners at a vertex of multiline. 
  double                 specFilletRad; ///< \ru Радиус особого скругления на линии мультилинии (если tracingType == mvt_SpecFilletType). \en Radius of a special fillet on a line of multiline (if tracingType == mvt_SpecFilletType). 
  // \ru Параметры законцовки в вершине (внутренней) \en Parameters of (inner) tip at a vertex 
  EnMLInnerTipType       tipType;       ///< \ru Тип внутренней законцовки. \en Type of inner tip. 
  bool                   firstSegTip;   ///< \ru Законцовка для первого сегмента вершины. \en Tip for the first segment of a vertex. 

public:
  StVertexOfMultilineInfo();            ///< \ru Умолчательный конструктор. \en Default constructor. 
  StVertexOfMultilineInfo( const StVertexOfMultilineInfo & other ); ///< \ru Копирующий конструктор. \en Copy-constructor. 

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор.\n
             \en Constructor.\n \~      
    \param[in] _smoothJoint   - \ru Флаг гладкого стыка в вершине сегментов базовой линии мультилинии,\n
                                используется только для сплайнов.
                                \en Flag of multiline base line segments smooth joint at a vertex,\n
                                used only for splines. \~
    \param[in] _tracingType   - \ru Тип обхода углов в вершине мультилинии.
                                \en Type of traverse of corners at a vertex of multiline. \~
    \param[in] _specFilletRad - \ru Радиус особого скругления на линии мультилинии,\n
                                если tracingType == mvt_SpecFilletType.
                                \en Radius of a special fillet on line of multiline,\n
                                if tracingType == mvt_SpecFilletType. \~
    \param[in] _tipType       - \ru Тип внутренней законцовки.
                                \en Type of inner tip. \~
    \param[in] _firstSegTip   - \ru Законцовка для первого сегмента вершины.
                                \en Tip for the first segment of a vertex. \~
  */
  StVertexOfMultilineInfo( bool _smoothJoint, EnMLVertexTracingType _tracingType,
                           double _specFilletRad, EnMLInnerTipType _tipType, bool _firstSegTip );

public:  
  
  /**\ru \name Функции доступа к данным
     \en \name Functions for access to data
      \{ */  
  /// \ru Тип обхода углов в вершине мультилинии. \en Type of traverse of corners at a vertex of multiline. 
  EnMLVertexTracingType  GetTracingType  () const { return tracingType;   } 
  /// \ru Флаг гладкого стыка в вершине сегментов базовой линии мультилинии. \en Flag of multiline base line segments smooth joint at a vertex. 
  bool                   IsSmoothJoint   () const { return smoothJoint;   } 
  /// \ru Радиус особого скругления на линии мультилинии. \en Radius of special fillet on base line of multiline. 
  double                 GetSpecFilletRad() const { return specFilletRad; }
  /// \ru Тип внутренней законцовки. \en Type of inner tip. 
  EnMLInnerTipType       GetTipType      () const { return tipType;       } 
  /// \ru Законцовка для первого сегмента вершины. \en Tip for the first segment of a vertex. 
  bool                   IsFirstSegTip   () const { return firstSegTip;   } 
  /// \ru Обход скруглением. \en Traverse by fillet. 
  bool                   IsFilletTracing () const;      

public:  

  /** \} */   
  /**\ru \name Операторы сравнения и присваивания
     \en \name Comparison operators and assignment
      \{ */ 
  /// \ru Оператор сравнения. \en Comparison operator. 
  bool                      operator ==     ( const StVertexOfMultilineInfo & ) const; 
  /// \ru Оператор сравнения. \en Comparison operator. 
  bool                      operator !=     ( const StVertexOfMultilineInfo & ) const; 
  /// \ru Оператор присваивания. \en Assignment operator.
  StVertexOfMultilineInfo & operator = ( const StVertexOfMultilineInfo & other )
  {
    Init( other );
    return *this;
  }

  /** \} */ 


protected:    

  /**\ru \name Функции инициализации
     \en \name Initialization functions
      \{ */

  /// \ru Инициализация по информации о вершине мультилинии. \en Initialization by an information about a vertex of multiline. 
  void                   Init( const StVertexOfMultilineInfo & other  );

  /** \brief \ru Инициализация.
             \en Initialization. \~
    \details \ru Инициализация.\n
             \en Initialization.\n \~      
    \param[in] _smoothJoint   - \ru Флаг гладкого стыка в вершине сегментов базовой линии мультилинии,\n
                                используется только для сплайнов.
                                \en Flag of multiline base line segments smooth joint at a vertex,\n
                                used only for splines. \~
    \param[in] _tracingType   - \ru Тип обхода углов в вершине мультилинии.
                                \en Type of traverse of corners at a vertex of multiline. \~
    \param[in] _specFilletRad - \ru Радиус особого скругления на линии мультилинии,\n
                                если tracingType == mvt_SpecFilletType.
                                \en Radius of a special fillet on line of multiline,\n
                                if tracingType == mvt_SpecFilletType. \~
    \param[in] _tipType       - \ru Тип внутренней законцовки.
                                \en Type of inner tip. \~
    \param[in] _firstSegTip   - \ru Законцовка для первого сегмента вершины.
                                \en Tip for the first segment of a vertex. \~
  */
  void                   Init               ( bool _smoothJoint, EnMLVertexTracingType _tracingType,
                                              double _specFilletRad, EnMLInnerTipType _tipType,
                                              bool _firstSegTip                      ); 
  /** \} */   
  /**\ru \name Функции изменения данных
     \en \name Functions for changing data
      \{ */ 
  /// \ru Изменить флаг гладкого стыка в вершине сегментов базовой линии мультилинии. \en Change flag of multiline base line segments smooth joint at a vertex. 
  bool                   ChangeSmoothJoint  ( bool                  othSmoothJoint   ); 
  /// \ru Изменить тип обхода углов в вершине мультилинии. \en Change type of traverse of corners at a vertex of multiline. 
  bool                   ChangeTracingType  ( EnMLVertexTracingType othTracingType   ); 
  /// \ru Изменить радиус особого скругления на линии мультилинии. \en Change radius of special fillet on base line of multiline. 
  bool                   ChangeSpecFilletRad( double                othSpecFilletRad ); 
  /// \ru Изменить тип внутренней законцовки. \en Change type of inner tip. 
  bool                   ChangeTipType      ( EnMLInnerTipType      othTipType       ); 
  /// \ru Изменить флаг законцовки для первого сегмента вершины. \en Change flag of tip for the first segment of a vertex. 
  bool                   ChangeFirstSegTip  ( bool                  othFirstSegTip   ); 
  
  /** \brief \ru Трансформация.
             \en Transformation. \~
    \details \ru Преобразование объекта согласно матрице.\n
             \en Transform object according to the matrix.\n \~      
    \param[in] matr - \ru Матрица трансформации.
                      \en Transformation matrix. \~
  */
  void                   Transform          ( const MbMatrix & matr                  );
  /** \} */

private:
  KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( StVertexOfMultilineInfo, MATH_FUNC_EX )
}; // StVertexOfMultilineInfo


//------------------------------------------------------------------------------
/** \brief \ru Класс для перестроения разрывов.
           \en Class for breaks rebuilding. \~
  \details \ru Класс для перестроения разрывов. Содержит номера сегментов базовой кривой.\n
           \en Class for breaks rebuilding. Contains indices of the base curve segments.\n \~
  \ingroup Algorithms_2D
*/
// ---
class MATH_CLASS MbBreaksRebuild {

public:
  SArray<size_t> baseNumbers; ///< \ru Номера сегментов базовой кривой для сегментов линии мультилинии. \en Indices of the base curve segments for line segments of multiline.  

public:
  /// \ru Конструктор по массиву номеров. \en Constructor by array of indices.  
  MbBreaksRebuild( const SArray<size_t> & bNumbers ) 
    :baseNumbers( bNumbers )
  {
  }
  ~MbBreaksRebuild() {}
};


//------------------------------------------------------------------------------
// _6_ 
/** \brief \ru Мультилиния.
           \en Multiline. \~
  \details \ru Мультилиния - составной геометрический объект, состоящий из\n
    1) кривых curves, построенных эквидистантно к базовой кривой мультилинии basisCurve
    с радиусами equidRadii. При этом способ обхода углов для каждой вершины
    basisCurve определяется отдельно (с помощью vertices.tracingType и vertices.specFilletRad).\n
    2) массива законцовок tipCurves в вершинах мультилинии, тип которых определяется
    соответствующим vertices[i].tipParams.tipType.\n
    3) законцовок begTipCurve и endTipCurve на концах мультилинии, тип которых определяется
    с помощью begTipParams и endTipParams.\n
    При создании линии мультилинии (добавлении радиуса эквидистанты) создается контур,
    который будет жить (меняться только внутри) до того момента, пока в массиве equidRadii
    есть соответствующий ему в начале элемент.\n
    Контур законцовки живет до тех пор, пока соответствующий тип законцовки не равен m_t_UndefTip.\n
    За существованием и удалением законцовок должны следить функции SetTipType.\n
    При построении кривых мультилинии вырожденные участки исключаются только в вершинах (перехлесты).
           \en Multiline is a composite geometric object consisting of\n
    1) 'curves' curves constructed to be equidistant to base line 'basisCurve' of the multiline
    with 'equidRadii' radii. The method of traverse of corners for each vertex of
    'basisCurve' is defined separately (with the help of vertices.tracingType and vertices.specFilletRad).\n
    2) 'tipCurves' array of tips at vertices of multiline, which type is defined
    by corresponding vertices[i].tipParams.tipType.\n
    3) 'begTipCurve' and 'endTipCurve' tips at the ends of multiline, which type is defined
    with the help of 'begTipParams' and 'endTipParams'.\n
    While creating line of multiline (adding equidistance radius) the contour is created
    which will be alive (can be changed only internally) while the 'equidRadii' array
    contains element at the beginning corresponding to it.\n
    The contour of a tip will be alive while the corresponding type of the tip isn't equal to m_t_UndefTip.\n
    SetTipType functions care about the existence and removal of tips.\n
    Degenerated regions are excluded only at vertices (overlaps) while constructing the curves of a multiline. \~
  \ingroup Region_2D
*/
// ---
class MATH_CLASS MbMultiline : public MbPlaneItem {
private:
  MbContour *                     basisCurve;    ///< \ru Базовая кривая (БК) (всегда не nullptr). \en Base curve (BC) (always not nullptr).
  SArray<StVertexOfMultilineInfo> vertices;      ///< \ru Массив вершин мультилинии (согласован с вершинами БК). \en Array of vertices of a multiline (agreed with the vertices of the base curve). 
  CSSArray<double>                equidRadii;    ///< \ru Сортированный массив радиусов эквидистантных кривых. \en Sorted array of radii of equidistant curves. 
  StMLTipParams                   begTipParams;  ///< \ru Параметры законцовки в начале мультилинии (начале БК). \en Parameters of a tip at the beginning of a multiline ( the beginning of base curve). 
  StMLTipParams                   endTipParams;  ///< \ru Параметры законцовки в конце мультилинии (конце БК). \en Parameters of a tip at the end of a multiline (end of the base curve). 
  bool                            processClosed; ///< \ru Обрабатывать ли замкнутость БК (доп. вершина). \en Whether to process the closedness of the base curve (additional vertex). 
  bool                            isTransparent; ///< \ru "Прозрачная" ли мультилиния. \en Whether the multiline is "transparent". 
  // \ru Объекты, которые составляют мультилинию (рекомендовали не делать их mutable, а писать и читать) \en Objects which constitute a multiline (recommended to read and write and not to make them mutable) 
  // \ru ЭТИ ОБЪЕКТЫ НЕЛЬЗЯ МЕНЯТЬ СНАРУЖИ!!! \en THESE OBJECTS CAN'T BE CHANGED OUTSIDE!!! 
  PArray<MbContourWithBreaks>     curves;        ///< \ru Кривые мультилинии (согласован с equidRadii) (всегда не nullptr). \en Curves of a multiline (agreed with the 'equidRadii') (always not nullptr).
  PArray<MbContour>               tipCurves;     ///< \ru Законцовки в вершинах мультилинии (согласован с vertices). \en Tips at vertices of a multiline (agreed with 'vertices'). 
  MbContour *                     begTipCurve;   ///< \ru Законцовка в начале мультилинии (начале БК). \en Tip at the beginning of a multiline (beginning of the base curve). 
  MbContour *                     endTipCurve;   ///< \ru Законцовка в конце мультилинии (конце БК). \en Tip at the end of a multiline (end of the base curve). 

  mutable double                  maxPosRadius;  ///< \ru Максимально возможный для невывернутого построения положительный радиус. \en Maximum possible positive radius for non-everted construction. 
  mutable double                  minNegRadius;  ///< \ru Минимально возможный для невывернутого построения отрицательный радиус. \en Minimum possible negative radius for non-everted construction. 
  mutable size_t                  minNotDegInd;  ///< \ru Индекс невырожденного элемента curves с минимальным радиусом. \en Index of non-everted element of 'curves' with minimal radius. 
  mutable size_t                  maxNotDegInd;  ///< \ru Индекс невырожденного элемента curves с максимальным радиусом. \en Index of non-everted element of 'curves' with maximal radius. 

public:
  MbMultiline(); ///< \ru Конструктор пустой мультилинии. \en Constructor of an empty multiline.  

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор.\n
             \en Constructor.\n \~      
    \param[in] _basisCurve    - \ru Базовая кривая.
                                \en Base curve. \~
    \param[in] vertInfo       - \ru Информация о вершине мультилинии (применяется ко всем вершинам).
                                \en Information about a vertex of the multiline (applied to all vertices). \~
    \param[in] _equidRadii    - \ru Сортированный массив радиусов эквидистантных кривых.
                                \en Sorted array of radii of equidistant curves. \~
    \param[in] _begTipParams  - \ru Параметры законцовки в начале мультилинии.
                                \en Parameters of tip at the beginning of multiline. \~
    \param[in] _endTipParams  - \ru Параметры законцовки в конце мультилинии.
                                \en Parameters of a tip at the end of a multiline. \~
    \param[in] _processClosed - \ru Обрабатывать ли замкнутость базовой кривой.
                                \en Whether to process the closedness of the base curve. \~
    \param[in] _isTransparent - \ru "Прозрачная" ли мультилиния\n
                                параметр не используется, мультилиния считается прозрачной.
                                \en Whether the multiline is "transparent"\n
                                parameter isn't used, multiline is considered to be transparent. \~
  */
  MbMultiline( const MbContour & _basisCurve, const StVertexOfMultilineInfo & vertInfo,
               const SArray<double> & _equidRadii,
               const StMLTipParams & _begTipParams, const StMLTipParams & _endTipParams,
               bool _processClosed, bool _isTransparent );

protected:
  /// \ru Копирующий конструктор. \en Copy-constructor. 
  MbMultiline( const MbMultiline &                      );

private:
  MbMultiline( const MbContour & _basisCurve, const SArray<StVertexOfMultilineInfo> & _vertices,
               const SArray<double> & _equidRadii,
               const StMLTipParams & _begTipParams, const StMLTipParams & _endTipParams,
               bool _processClosed, bool _isTransparent, bool & error );

public:
  virtual ~MbMultiline();

public:

  /**\ru \name Общие функции геометрического объекта
     \en \name Common functions of a geometric object
      \{ */
  MbePlaneType IsA         () const override;                                          // \ru Тип объекта. \en A type of an object.
  MbePlaneType Type        () const override;                                          // \ru Групповой тип объекта. \en Group type of an object.
  MbePlaneType Family      () const override;                                          // \ru Семейство объекта. \en Family of an object.
  bool        IsSame       ( const MbPlaneItem & other, double accuracy = LENGTH_EPSILON ) const override;                // \ru Являются ли объекты равными. \en Determine whether objects are equal.
  bool        IsSimilar    ( const MbPlaneItem & item ) const override;                // \ru Являются ли объекты подобными. \en Determine whether the objects are similar.
  bool        SetEqual     ( const MbPlaneItem & item ) override;                      // \ru Сделать объекты равными. \en Make the objects equal.
  void        Transform    ( const MbMatrix & matr, MbRegTransform * = nullptr, const MbSurface * newSurface = nullptr ) override;// \ru Преобразовать объект согласно матрице. \en Transform an object according to the matrix.
  void        Move         ( const MbVector & to,   MbRegTransform * = nullptr, const MbSurface * newSurface = nullptr ) override;// \ru Сдвинуть объект вдоль вектора. \en Move an object along a vector.
  void        Rotate       ( const MbCartPoint & pnt, const MbDirection & angle, MbRegTransform * = nullptr, const MbSurface * newSurface = nullptr ) override; // \ru Повернуть вокруг точки на угол. \en Rotate at angle around a point.
  MbPlaneItem & Duplicate  ( MbRegDuplicate * = nullptr ) const override;                 // \ru Сделать копию объекта. \en Create a copy of the object.
  void     AddYourGabaritTo( MbRect & r                              ) const override; // \ru Добавить свой габарит в присланный габарит. \en Add your own bounding box into the given bounding box.

  bool     IsVisibleInRect      ( const MbRect & r, bool exact = false    ) const override; // \ru Виден ли объект в заданном прям-ке. \en Whether the object is visible in the given rectangle.
  double   DistanceToPoint      ( const MbCartPoint & toP                 ) const override; // \ru Вычислить расстояние до точки. \en Calculate the distance to a point.
  bool     DistanceToPointIfLess( const MbCartPoint & to, double &distance) const override; // \ru Вычислить расстояние до точки, если оно меньше d. \en Calculate the distance to the point if it is less than d.

  MbProperty& CreateProperty( MbePrompt name ) const override;  // \ru Создать собственное свойство. \en Create a custom property. 
  void     GetProperties( MbProperties & properties ) override;  // \ru Выдать свойства объекта. \en Get properties of the object.
  void     SetProperties( const MbProperties & properties ) override;  // \ru Записать свойства объекта. \en Set properties of the object.
  void     GetBasisPoints( MbControlData & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void     SetBasisPoints( const MbControlData & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  /** \} */   
  /**\ru \name Функции доступа к данным
     \en \name Functions for access to data
      \{ */

  /// \ru Базовая кривая. \en Base curve. 
  const MbContour &               GetBasisCurve     ()            const { return *basisCurve;        } 
  /// \ru Дать базовую кривую для изменения. \en Get base curve for editing. 
        MbContour &               SetBasisCurve     ()                  { return *basisCurve;        } 
  /// \ru Количество вершин. \en Count of vertices. 
        size_t                    GetVerticesCount  ()            const { return vertices.Count();   } 
  /// \ru Вершина по номеру. Номер должен быть меньше количества вершин. \en Vertex by an index. Index must be less than count of vertices. 
  const StVertexOfMultilineInfo & GetVertex         ( size_t i )  const { return vertices[i];        } 
  /// \ru Количество радиусов эквидистант. \en Count of radii of equidistant curves. 
        size_t                    GetEquidRadiiCount()            const { return equidRadii.Count(); } 
  /// \ru Радиус эквидистанты по номеру. Номер должен быть меньше количества радиусов. \en Radius of equidistant curve by an index. Index must be less than count of radii. 
        double                    GetEquidRadius    ( size_t i )  const { return equidRadii[i];      } 
  /// \ru Параметры законцовки в начале мультилинии. \en Parameters of tip at the beginning of multiline. 
  const StMLTipParams &           GetBegTipParams   ()            const { return begTipParams;       } 
  /// \ru Параметры законцовки в конце мультилинии. \en Parameters of a tip at the end of a multiline. 
  const StMLTipParams &           GetEndTipParams   ()            const { return endTipParams;       } 
  /// \ru Признак обрабатывания замкнутости базовой кривой. \en An attribute of the base curve closedness processing. 
        bool                      IsProcessClosed   ()            const { return processClosed;      } 
  /// \ru Прозрачность мультилинии. \en Transparency of multiline. 
        bool                      IsTransparent     ()            const { return isTransparent;      } 
  // \ru Функции доступа к объектам мультилинии \en Functions for access to objects of multiline 
  // \ru ЭТИ ОБЪЕКТЫ НЕЛЬЗЯ МЕНЯТЬ СНАРУЖИ!!! \en THESE OBJECTS CAN'T BE CHANGED OUTSIDE!!! 

  /// \ru Количество кривых мультилинии. \en Count of curves of a multiline. 
        size_t                    GetCurvesCount    ()            const { return curves.Count();     } 
  /// \ru Кривая мультилинии по номеру. Номер должен быть меньше количества кривых. \en Curve of multiline by an index. Index must be less than count of curves. 
  const MbContourWithBreaks*      GetCurve          ( size_t i )  const { return curves[i];          } 

  /// \ru Количество кривых - законцовок в вершинах мультилинии. \en Count of tip-curves at vertices of multiline. 
        size_t                    GetTipCurvesCount ()            const { return tipCurves.Count();  } 
  /// \ru Кривая - законцовка по номеру. Номер должен быть меньше количества законцовок. \en Tip-curve by an index. Index must be less than the count of tips. 
  const MbContour *               GetTipCurve       ( size_t i )  const { return tipCurves[i];       } 

  /// \ru Законцовка в начале мультилинии. \en Tip at the beginning of a multiline. 
  const MbContour *               GetBegTipCurve    ()            const { return begTipCurve;        } 
  /// \ru Законцовка в конце мультилинии. \en Tip at the end of a multiline. 
  const MbContour *               GetEndTipCurve    ()            const { return endTipCurve;        } 

  /// \ru Максимально возможный для невывернутого построения положительный радиус. \en Maximum possible positive radius for non-everted construction. 
        double                    GetMaxPosRadius   ()            const { return maxPosRadius;       } 
  /// \ru Минимально возможный для невывернутого построения отрицательный радиус. \en Minimum possible negative radius for non-everted construction. 
        double                    GetMinNegRadius   ()            const { return minNegRadius;       } 

  /// \ru Индекс невырожденной кривой мультилинии с минимальным радиусом \en Index of non-degenerated curve of a multiline with minimal radius 
        size_t                    GetMinNotDegInd   ()            const { return minNotDegInd;       } 
  /// \ru Индекс невырожденной кривой мультилинии с максимальным радиусом \en Index of non-degenerated curve of a multiline with maximal radius 
        size_t                    GetMaxNotDegInd   ()            const { return maxNotDegInd;       } 

  /** \} */ 
  /**\ru \name Функции изменения данных: изменение базовой кривой мультилинии
     \en \name Functions for changing data: changing the base curve of a multiline
      \{ */     
  
  /// \ru Очистить базовую кривую. \en Clear the base curve. 
  void ClearBasisCurve          ();                                           

  /** \brief \ru Заменить базовую кривую.
             \en Replace the base curve. \~
    \details \ru Заменить базовую кривую.\n   
      При замене базовой кривой мультилинии в ней удаляются все разрывы.
             \en Replace the base curve.\n   
      When replacing the base curve of multiline all of its breaks are removed. \~
    \param[in] _basisCurve - \ru Новая базовая кривая.
                             \en New base curve. \~
    \param[in] vertInfo    - \ru Новая информация о вершинах мультилинии.
                             \en New information about vertices of multiline. \~
  */
  void ReplaceBasisCurve        ( const MbContour & _basisCurve,
                                  const StVertexOfMultilineInfo & vertInfo ); 

  /** \brief \ru Добавить сегмент в базовую кривую.
             \en Add a segment to a base curve. \~
    \details \ru Добавить сегмент в базовую кривую.\n
             \en Add a segment to a base curve.\n \~       
    \param[in] segment  - \ru Добавляемый сегмент,\n
                          если является ломаной или контуром, то в мультилинию добавляются только составляющие сегменты.
                          \en Segment to add,\n
                          if it is polyline or contour, then only component segments are added to the multiline. \~
    \param[in] vertInfo - \ru Информация присваивается всем добавленным или измененным вершинам.
                          \en Information is assigned to all added or changed vertices. \~
    \return \ru true, если сегмент был добавлен.
            \en True if a segment has been added. \~
  */
  bool AddBasisSegment          ( MbCurve * segment,
                                  const StVertexOfMultilineInfo & vertInfo );

  /// \ru Удалить последний сегмент базовой кривой. \en Delete the last segment of the base curve. 
  void DeleteLastBasisSegment   ();                                           

  /** \brief \ru Вставить вершину.
             \en Insert a vertex. \~
    \details \ru Вставить вершину. Сегмент разбивается на два.\n
             \en Insert a vertex. Segment is split into two segments.\n \~       
    \param[in] t        - \ru Параметр на базовой кривой.
                          \en Parameter on the base curve. \~
    \param[in] vertInfo - \ru Информация о новой вершине мультилинии.
                          \en Information about new vertex of a multiline. \~
    \return \ru Индекс разбитого сегмента.
            \en Index of split segment. \~
  */
  size_t InsertVertex           ( double t,  
                                  const StVertexOfMultilineInfo & vertInfo );

  /** \brief \ru Удалить вершину мультилинии.
             \en Delete a vertex of multiline. \~
    \details \ru Удалить вершину мультилинии.\n  
      Пара сегментов, примыкающих к вершине, заменяется отрезком.\n
      Разрывы на прилегающих сегментах исчезают.
      Разрывы, лежащие на прямолинейных прилегающих сегментах, 
      остаются, привязываясь к вершинам нового отрезка.
             \en Delete a vertex of multiline.\n  
      Pair of segments adjoining the vertex is replaced by a segment.\n
      Breaks on adjacent segments are disappeared.
      Breaks on straight adjacent segments 
      are remained by binding to the vertices of a new segment. \~
    \param[in] i - \ru Индекс вершины.
                   \en An index of a vertex. \~
    \return \ru true, если вершина была удалена.
            \en True if a vertex is deleted. \~
  */
  bool RemoveVertex             ( size_t i                                 );      
 
  /** \brief \ru Сместить геометрическую hot-точку.
             \en Shift a geometric hot-point. \~
    \details \ru Сместить геометрическую hot-точку базовой кривой.\n     
      При перемещении точки разрывы контуров, соответствующих прилегающим прямолинейным 
      сегментам привязываются к соседним неподвижным вершинам базовой кривой.
             \en Shift a geometric hot-point of the base curve.\n     
      While shifting point, the contour breaks corresponding to adjacent straight 
      segments are bound to the neighboring fixed vertices of the base curve. \~
    \param[in] segInd   - \ru Номер сегмента.
                          \en An index of a segment. \~
    \param[in] subInd   - \ru Номер точки на сегменте:\n
                          если subInd = 0 - общая точка между двумя сегментами (может быть задана для любого сегмента),\n
                          если subInd = 1 - средняя точкой отрезка, средняя точкой дуги, базовая точка сплайнов pt_Nurbs и pt_Bezier,\n
                          если subInd > 1 - базовая точка сплайнов pt_Nurbs и pt_Bezier.
                          \en An index of a point on the segment:\n
                          if subInd = 0 - common point between two segments (can be specified for any segment),\n
                          if subInd = 1 - middle point of a segment, middle point of an arc, the base point of pt_Nurbs and pt_Bezier splines,\n
                          if subInd > 1 - base point of pt_Nurbs and pt_Bezier splines. \~
    \param[in] newPoint - \ru Новое положение точки.
                          \en New position of the point. \~
    \return \ru Точка была смещена.
            \en Point has been shifted. \~
  */
  bool SetBasisCurvesGeoHotPoint( size_t segInd, size_t subInd,
                                  const MbCartPoint & newPoint             );    

  /** \brief \ru Удалить геометрическую hot-точку.
             \en Delete a geometric hot-point. \~
    \details \ru Удалить геометрическую hot-точку базовой кривой.\n
             \en Delete a geometric hot-point of the base curve.\n \~  
    \param[in] segInd - \ru Номер сегмента.
                        \en An index of a segment. \~
    \param[in] subInd - \ru Номер точки на сегменте:\n 
                        если subInd = 0 - результат аналогичен удалению вершины мультилинии RemoveVertex,\n
                        если subInd = 1 - удаление средней точки дуги (дуга заменяется отрезком), 
                                          удаление базовой точки сплайнов pt_Nurbs и pt_Bezier (изменение формы),\n
                        если subInd > 1 - удаление базовой точки сплайнов pt_Nurbs и pt_Bezier (изменение формы).
                        \en An index of a point on the segment:\n 
                        if subInd = 0 - result is similar to deletion of the 'RemoveVertex' vertex of a multiline,\n
                        if subInd = 1 - deletion of the middle point of an arc (the arc is replaced by segment), 
                                          deletion of the base point of pt_Nurbs and pt_Bezier splines (changing of shape),\n
                        if subInd > 1 - deletion of the base point of pt_Nurbs and pt_Bezier splines (changing of shape). \~     
    \return \ru true, если hot-точка была удалена.
            \en True if hot-point has been deleted. \~
  */
  bool DelBasisCurvesGeoHotPoint( size_t segInd, size_t subInd             );
  
  /** \} */ 
  /**\ru \name Функции изменения данных: скругления и фаски базовой кривой
     \en \name Functions for changing data: fillet and chamfer of the base curve
      \{ */     
  
  /** \brief \ru Скруглить два соседних сегмента.
             \en Fillet two neighboring segments. \~
    \details \ru Скруглить два соседних сегмента базовой кривой.\n
             \en Fillet two neighboring segments of base curve.\n \~       
    \param[in] index    - \ru Индекс первого скругляемого сегмента.
                          \en Index of the first segment to fillet. \~
    \param[in] rad      - \ru Радиус скругления.
                          \en The radius of fillet. \~
    \param[in] vertInfo - \ru Информация для новых вершин мультилинии.
                          \en Information for new vertices of a multiline. \~
    \return \ru true в случае успеха операции.
            \en Returns true if the operation succeeded. \~
  */
  bool FilletTwoBasisSegments   ( ptrdiff_t & index, double rad,
                                  const StVertexOfMultilineInfo & vertInfo );

  /** \brief \ru Скруглить базовую кривую.
             \en Fillet the base curve. \~
    \details \ru Скруглить все углы базовой кривой.\n
             \en Fillet all corners of the base curve.\n \~  
    \param[in] rad      - \ru Радиус скругления.
                          \en The radius of fillet. \~
    \param[in] vertInfo - \ru Информация для новых вершин мультилинии.
                          \en Information for new vertices of a multiline. \~
    \return \ru true, если добавилось хотя бы одно скругление.
            \en True if at least one fillet is added. \~
  */
  bool FilletBasisCurve         ( double rad,
                                  const StVertexOfMultilineInfo & vertInfo );    
  
  /** \brief \ru Вставить фаску между двумя соседними сегментами базовой кривой мультилинии.
             \en Insert a chamfer between two neighboring segments of the base curve of a multiline. \~
    \details \ru Вставить фаску между двумя соседними сегментами базовой кривой мультилинии.\n
             \en Insert a chamfer between two neighboring segments of the base curve of a multiline.\n \~       
    \param[in] index    - \ru Индекс первого скругляемого сегмента.
                          \en Index of the first segment to fillet. \~
    \param[in] len      - \ru Длина фаски.
                          \en Length of chamfer. \~
    \param[in] par      - \ru Параметр в зависимости от типа type:\n
                          если type = true, par - угол\n
                          если type = false, par - размер.
                          \en Parameter depending on 'type' type:\n
                          if type = true, par is a corner\n
                          if type = false, par is a size. \~
    \param[in] type     - \ru Тип задания фаски:\n
                          true  - фаска задана как размер + угол,\n
                          false - фаска задана как размер + размер.
                          \en The type of a chamfer specification:\n
                          true - chamfer specified as size + angle,\n
                          false - chamfer specified as size + size. \~
    \param[in] firstSeg - \ru true, если параметр par относится к первому сегменту.
                          \en True if 'par' parameter is related to the first segment. \~
    \param[in] vertInfo - \ru Информация для новых вершин мультилинии.
                          \en Information for new vertices of a multiline. \~
    \return \ru true в случае успеха операции.
            \en Returns true if the operation succeeded. \~
  */
  bool ChamferTwoBasisSegments  ( ptrdiff_t & index, double len, double par,
                                  bool type, bool firstSeg,
                                  const StVertexOfMultilineInfo & vertInfo ); 

  /** \brief \ru Вставить фаску между каждыми двумя соседними сегментами базовой кривой мультилинии.
             \en Insert a chamfer between each two neighboring segments of the base curve of a multiline. \~
    \details \ru Вставить фаску между каждыми двумя соседними сегментами базовой кривой мультилинии.\n   
      Параметр par относится к первому сегменту из каждой пары.
             \en Insert a chamfer between each two neighboring segments of the base curve of a multiline.\n   
      'par' parameter is related to the first segment of each pair. \~
    \param[in] len      - \ru Длина фаски.
                          \en Length of chamfer. \~
    \param[in] par      - \ru Параметр в зависимости от типа type:\n
                          если type = true, par - угол,\n
                          если type = false, par - размер.
                          \en Parameter depending on 'type' type:\n
                          if type = true, par is a corner,\n
                          if type = false, par is a size. \~
    \param[in] type     - \ru Тип задания фаски:\n
                          true  - фаска задана как размер + угол,\n
                          false - фаска задана как размер + размер.
                          \en The type of a chamfer specification:\n
                          true - chamfer specified as size + angle,\n
                          false - chamfer specified as size + size. \~          
    \param[in] vertInfo - \ru Информация для новых вершин мультилинии.
                          \en Information for new vertices of a multiline. \~
    \return \ru true, если добавилась хотя бы одна фаска.
            \en True if at least one chamfer is added. \~
  */
  bool ChamferBasisCurve        ( double len, double par, bool type,
                                  const StVertexOfMultilineInfo & vertInfo ); ///< \ru Вставить фаску БК \en Insert chamfer of the base curve 

  /** \} */ 
  /**\ru \name Функции изменения данных: изменение параметров вершины мультилинии
     \en \name Functions for changing data: changing the parameters of a vertex of a multiline
      \{ */
            
  /** \brief \ru Установить флаг гладкого стыка.
             \en Set the flag of smooth joint. \~
    \details \ru Установить флаг гладкого стыка в вершине.\n
             \en Set the flag of smooth joint at a vertex.\n \~   
    \param[in] i              - \ru Индекс вершины мультилинии.
                                \en An index of a vertex of a multiline. \~
    \param[in] othSmoothJoint - \ru Флаг гладкого стыка.
                                \en Flag of smooth joint. \~
  */
  void SetSmoothJoint           ( size_t i, bool                  othSmoothJoint   );

  /** \brief \ru Установить тип обхода.
             \en Set the type of traverse. \~
    \details \ru Установить тип обхода вершины.\n
             \en Set the type of traverse of the vertex.\n \~   
    \param[in] i              - \ru Индекс вершины мультилинии.
                                \en An index of a vertex of a multiline. \~
    \param[in] othTracingType - \ru Тип обхода.
                                \en Type of traverse. \~
  */
  void SetTracingType           ( size_t i, EnMLVertexTracingType othTracingType   );

  /** \brief \ru Установить радиус специального скругления.
             \en Set the radius of a special fillet. \~
    \details \ru Установить радиус специального скругления вершины мультилинии.\n
             \en Set the radius of special fillet of a vertex of the multiline.\n \~   
    \param[in] i                - \ru Индекс вершины мультилинии.
                                  \en An index of a vertex of a multiline. \~
    \param[in] othSpecFilletRad - \ru Радиус.
                                  \en Radius. \~
  */
  void SetSpecFilletRad         ( size_t i, double                othSpecFilletRad );

  /** \brief \ru Установить тип законцовки.
             \en Set the type of a tip. \~
    \details \ru Установить тип законцовки (разделителя) в вершине.\n
             \en Set the type of tip (splitter) at the vertex.\n \~   
    \param[in] i          - \ru Индекс вершины мультилинии.
                            \en An index of a vertex of a multiline. \~
    \param[in] othTipType - \ru Тип законцовки.
                            \en Type of tip. \~
  */
  void SetTipType               ( size_t i, EnMLInnerTipType      othTipType       );

  /** \brief \ru Установить направление законцовки.
             \en Set the direction of a tip. \~
    \details \ru Установить направление законцовки в вершине мультилинии.\n
             \en Set the direction of a tip at a vertex of multiline.\n \~   
    \param[in] i              - \ru Индекс вершины мультилинии.
                                \en An index of a vertex of a multiline. \~
    \param[in] othFirstSegTip - \ru Направление законцовки, если true - от первого сегмента.
                                \en Direction of tip, if true - from the first segment. \~
  */
  void SetTipDirection          ( size_t i, bool                  othFirstSegTip   );

  /** \brief \ru Установить информацию о вершине.
             \en Set the information about a vertex. \~
    \details \ru Установить информацию о вершине мультилинии.\n
             \en Set the information about a vertex of multiline.\n \~   
    \param[in] i        - \ru Индекс вершины мультилинии.
                          \en An index of a vertex of a multiline. \~
    \param[in] vertInfo - \ru Информация о вершине.
                          \en Information about a vertex. \~
  */
  void SetVertexOfMultilineInfo ( size_t i,
                                  const StVertexOfMultilineInfo & vertInfo         );

  /** \} */ 
  /**\ru \name Функции изменения данных: изменение радиусов эквидистант
     \en \name Functions for changing data: changing the radii of equidistant curves
      \{ */    
  
  /** \brief \ru Установить значение радиуса кривой.
             \en Set the value of radius of curve. \~
    \details \ru Установить значение радиуса кривой.\n
             \en Set the value of radius of curve.\n \~   
    \param[in]  i        - \ru Индекс кривой мультилинии.
                           \en An index of curve of multiline. \~
    \param[in]  radius   - \ru Новое значение радиуса.
                           \en New value of the radius. \~
    \param[out] newIndex - \ru Новый индекс кривой.
                           \en New index of curve. \~
    \return \ru true, если радиус был изменен.
            \en True if a radius has been changed. \~
  */
  bool SetRadius                ( size_t i, double radius, size_t & newIndex );   
  
  /** \brief \ru Изменение всех радиусов кривых.
             \en Changing all radii of curves. \~
    \details \ru Изменение одновременно всех радиусов кривых.\n
      Значения радиусов будут изменены, если их количество в newRadii совпадает с количеством кривых.
             \en Simultaneous changing all radii of curves.\n
      Values of radii will be changed if their count in 'newRadii' is coincident to the count of curves. \~
    \param[in] newRadii - \ru Новые значения радиусов кривых.
                          \en New values of radii of curves. \~
    \return \ru true, если радиусы были изменены.
            \en True if radii has been changed. \~
  */
  bool SetRadii                 ( const CSSArray<double> & newRadii );

  /** \brief \ru Изменение радиуса.
             \en Change the radius. \~
    \details \ru Изменение радиуса кривой мультилинии.\n
             \en Change the radius of a curve of multiline.\n \~         
    \param[in] oldRadius - \ru Старое значение радиуса.
                           \en Old value of the radius. \~
    \param[in] radius    - \ru Новое значение радиуса.
                           \en New value of the radius. \~
  */
  void ChangeRadius             ( double oldRadius, double radius );

  /** \brief \ru Добавление радиуса кривой мультилинии.
             \en Add the radius of a curve of multiline. \~
    \details \ru Добавление радиуса кривой мультилинии.\n 
      Фактически, добавление кривой мультилинии.
             \en Add the radius of a curve of multiline.\n 
      Actually, addition of a curve of multiline. \~    
    \param[in] radius - \ru Значение радиуса.
                        \en Value of radius. \~
    \return \ru Значение индекса новой кривой\n
            если кривая не была добавлена, индекс равен SYS_MAX_T.
            \en Value of an index of the new curve.\n
            if curve was not added, then the index is equal to SYS_MAX_T. \~
  */
  size_t AddRadius              ( double radius                   );

  /** \brief \ru Удаление кривой.
             \en Delete a curve. \~
    \details \ru Удаление кривой мультилинии.\n
             \en Delete a curve of multiline.\n \~        
    \param[in] i - \ru Индекс кривой.
                   \en A curve index. \~
    \return \ru true, если кривая была удалена.
            \en True if curve has been deleted. \~
  */
  bool RemoveRadius             ( size_t i                        );
  
  /** \brief \ru Удаление кривой.
             \en Delete a curve. \~
    \details \ru Удаление кривой мультилинии.\n
             \en Delete a curve of multiline.\n \~        
    \param[in] oldRadius - \ru Радиус кривой.
                           \en Radius of a curve. \~
    \return \ru true, если кривая была удалена.
            \en True if curve has been deleted. \~
  */
  bool RemoveRadius             ( double oldRadius                );

  /** \} */ 
  /**\ru \name Функции изменения данных: изменение параметров законцовок
     \en \name Functions for changing data: change the parameters of tips
      \{ */
             
  /** \brief \ru Изменение типа законцовки в начале.
             \en Change the type of a tip at the beginning. \~
    \details \ru Изменение типа законцовки мультилинии в начале.\n
             \en Change the type of a tip of multiline at the beginning.\n \~        
    \param[in] othTipType - \ru Новый тип законцовки.
                            \en New type of tip. \~
  */
  void SetBegTipType            ( EnMLTipType othTipType          );
             
  /** \brief \ru Изменение параметра законцовки в начале.
             \en Change the parameter of a tip at the beginning. \~
    \details \ru Изменение параметра законцовки мультилинии в начале.\n
             \en Change the parameter of a tip of multiline at the beginning.\n \~        
    \param[in] othTipParam - \ru Новый параметр законцовки.
                             \en New parameter of tip. \~
  */
  void SetBegTipParam           ( double      othTipParam         );

  /** \brief \ru Изменение типа законцовки в конце.
             \en Change the type of a tip at the end. \~
    \details \ru Изменение типа законцовки мультилинии в конце.\n
             \en Change the type of a tip of multiline at the end.\n \~        
    \param[in] othTipType - \ru Новый тип законцовки.
                            \en New type of tip. \~
  */
  void SetEndTipType            ( EnMLTipType othTipType          );

  /** \brief \ru Изменение параметра законцовки в конце.
             \en Change the parameter of a tip at the end. \~
    \details \ru Изменение параметра законцовки мультилинии в конце.\n
             \en Change the parameter of a tip of multiline at the end.\n \~        
    \param[in] othTipParam - \ru Новый параметр законцовки.
                             \en New parameter of tip. \~
  */
  void SetEndTipParam           ( double      othTipParam         );

  /** \} */ 
  /**\ru \name Функции изменения данных
     \en \name Functions for changing data
      \{ */    
 
  /** \brief \ru Изменение обработки замкнутости.
             \en Change the closedness processing. \~
    \details \ru Изменение флага обработки замкнутости.\n
             \en Change the flag of the closedness processing.\n \~        
    \param[in] othProcessClosed - \ru Флаг обработки замкнутости.
                                  \en Flag of the closedness processing. \~
  */
  void SetProcessClosed         ( bool othProcessClosed           );  
  
  /** \brief \ru Изменение прозрачности мультилинии.
             \en Change the transparency of multiline. \~
    \details \ru Изменение флага прозрачности мультилинии.\n
             \en Change the flag of transparency of multiline.\n \~        
    \param[in] othTransparent - \ru Флаг прозрачности.
                                \en Transparency flag. \~
  */
  void SetTransparent           ( bool othTransparent             ) { isTransparent = othTransparent; }

  /** \} */ 
  /**\ru \name Работа с разрывами: добавление разрывов
     \en \name Working with breaks: addition of breaks
      \{ */    
  
  /** \brief \ru Усечение части кривой мультилинии между точками.
             \en Trimming of a piece of a curve of multiline between points. \~
    \details \ru Усечение части кривой мультилинии между точками.\n
      Добавление разрыва.
             \en Trimming of a piece of a curve of multiline between points.\n
      Add a break. \~  
    \param[in] contour     - \ru Кривая мультилинии для добавления разрыва.
                             \en Curve of multiline for addition of a break. \~
    \param[in] point1      - \ru Первая граница разрыва.
                             \en The first boundary of the break. \~
    \param[in] point2      - \ru Вторая граница разрыва.
                             \en The second boundary of the break. \~
    \param[in] point3      - \ru Точка, которая показывает удаляемую часть замкнутого контура,\n
                             в случае разомкнутого контура она игнорируется.
                             \en The point indicating the piece of a closed contour to be deleted,\n
                             ignored in case of the opened contour. \~ 
    \param[in] invertBreak - \ru Если true, то разрыв накладывается на противоположную часть контура.
                             \en If 'true', then the break is applied to the opposite piece of the contour. \~
    \return \ru true, если разрыв был добавлен.
            \en True if a break has been added. \~
  */
  bool DeletePartP1P2           ( MbContourWithBreaks * contour,
                                  const MbCartPoint & point1, 
                                  const MbCartPoint & point2,
                                  const MbCartPoint & point3,
                                  bool invertBreak = false        );

  /** \brief \ru Усечение части кривой мультилинии между точками.
             \en Trimming of a piece of a curve of multiline between points. \~
    \details \ru Усечение части кривой мультилинии между точками.\n
      Добавление разрыва.
             \en Trimming of a piece of a curve of multiline between points.\n
      Add a break. \~  
    \param[in] cNumber     - \ru Номер кривой мультилинии для добавления разрыва.
                             \en Index of curve of multiline for addition of a break. \~
    \param[in] point1      - \ru Первая граница разрыва.
                             \en The first boundary of the break. \~
    \param[in] point2      - \ru Вторая граница разрыва.
                             \en The second boundary of the break. \~
    \param[in] point3      - \ru Точка, которая показывает удаляемую часть замкнутого контура,\n
                             в случае разомкнутого контура она игнорируется.
                             \en The point indicating the piece of a closed contour to be deleted,\n
                             ignored in case of the opened contour. \~ 
    \param[in] invertBreak - \ru Если true, то разрыв накладывается на противоположную часть контура.
                             \en If 'true', then the break is applied to the opposite piece of the contour. \~
    \return \ru true, если разрыв был добавлен.
            \en True if a break has been added. \~
  */
  bool DeletePartP1P2           ( size_t cNumber,                      
                                  const MbCartPoint & point1, 
                                  const MbCartPoint & point2,
                                  const MbCartPoint & point3,
                                  bool invertBreak = false        );

  /** \brief \ru Усечение части кривой мультилинии между параметрами контура.
             \en Trimming of a piece of a curve of multiline between parameters of contour. \~
    \details \ru Усечение части кривой мультилинии между параметрами контура.\n
      Добавление разрыва.
             \en Trimming of a piece of a curve of multiline between parameters of contour.\n
      Add a break. \~  
    \param[in] cNumber     - \ru Номер кривой мультилинии для добавления разрыва.
                             \en Index of curve of multiline for addition of a break. \~
    \param[in] t1          - \ru Первая граница разрыва.
                             \en The first boundary of the break. \~
    \param[in] t2          - \ru Вторая граница разрыва.
                             \en The second boundary of the break. \~
    \param[in] t3          - \ru Параметр, который показывает удаляемую часть замкнутого контура,\n
                             в случае разомкнутого контура он игнорируется.
                             \en A parameter which indicates a removable part of the closed contour, \n
                             ignored in case of opened contour. \~ 
    \param[in] invertBreak - \ru Если true, то разрыв накладывается на противоположную часть контура.
                             \en If 'true', then the break is applied to the opposite piece of the contour. \~
    \return \ru true, если разрыв был добавлен.
            \en True if a break has been added. \~
  */
  bool DeletePartP1P2           ( size_t cNumber, 
                                  double t1, double t2, double t3,
                                  bool invertBreak = false        );

  /** \} */ 
  /**\ru \name Работа с разрывами: удаление разрывов
     \en \name Working with breaks: deletion of breaks
      \{ */  

  /** \brief \ru Удалить разрывы.
             \en Remove breaks. \~
    \details \ru Удалить все разрывы мультилинии.\n
             \en Delete all breaks of multiline.\n \~        
    \return \ru true, если хотя бы один разрыв был удален.
            \en true, if at least one break has been deleted. \~
  */
  bool DeleteBreaks             (                                 );
     
  /** \brief \ru Удалить разрывы кривой.
             \en Remove breaks of curve. \~
    \details \ru Удалить все разрывы кривой мультилинии.\n
             \en Delete all breaks of curve of multiline.\n \~     
    \param[in] cNumber - \ru Номер кривой.
                         \en Index of curve. \~
    \return \ru true, если хотя бы один разрыв был удален.
            \en true, if at least one break has been deleted. \~
  */
  bool DeleteBreaks             ( size_t cNumber                  );
    
  /** \brief \ru Удалить разрыв кривой.
             \en Delete a break of curve. \~
    \details \ru Удалить разрыв по параметру на кривой.\n
             \en Delete a break by a parameter on the curve.\n \~     
    \param[in] cNumber - \ru Номер кривой.
                         \en Index of curve. \~
    \param[in] t       - \ru Параметр на кривой.
                         \en A parameter on the curve. \~
    \return \ru true, если разрыв был удален.
            \en True if a break has been deleted. \~
  */
  bool DeleteBreak              ( size_t cNumber, double t        );      
 
  /** \brief \ru Удалить разрыв.
             \en Delete a break. \~
    \details \ru Удалить разрыв по номеру.\n
             \en Delete a break by an index.\n \~     
    \param[in] cNumber  - \ru Номер кривой.
                          \en Index of curve. \~
    \param[in] brNumber - \ru Номер разрыва на кривой.
                          \en Index of a break on the curve. \~
    \return \ru true, если разрыв был удален.
            \en True if a break has been deleted. \~
  */
  bool DeleteBreakAtNumber      ( size_t cNumber, size_t brNumber );  

  /** \brief \ru Удалить разрывы малой длины.
             \en Delete breaks of small length. \~
    \details \ru Удалить разрывы малой метрической длины у кривой мультилинии.\n
      В случае успеха линия мультилинии перестраивается соответственно разрывам.
             \en Delete breaks of small length of a curve of multiline.\n
      In case of success the line of multiline is rebuilt according to breaks. \~
    \param[in] cNumber - \ru Номер кривой мультилинии.
                         \en Index of a curve of multiline. \~
    \param[in] length  - \ru Минимальная длина невидимой части.
                         \en Minimal length of invisible piece. \~
    \return \ru true, если хотя бы один разрыв кривой был удален.
            \en True if at least one break of the curve has been deleted. \~ 
  */
  bool DeleteSmallBreaks        ( size_t cNumber, double length   );

  /** \brief \ru Удалить малые видимые части.
             \en Delete small visible pieces. \~
    \details \ru Удалить видимые части малой метрической длины у кривой мультилинии.\n  
      Соответствует объединению близких разрывов в один.
      В случае успеха видимые контуры кривой перестраиваются соответственно разрывам.
             \en Delete small visible pieces of a curve of multiline with small metric length.\n  
      Corresponds to union of close breaks into one.
      In case of success the visible contours of the line is rebuilt according to breaks. \~
    \param[in] cNumber - \ru Номер кривой мультилинии.
                         \en Index of a curve of multiline. \~
    \param[in] length  - \ru Минимальная длина видимой части.
                         \en Minimal length of visible piece. \~
    \return \ru true, если разрывы кривой были изменены.
            \en True if breaks has been changed. \~
  */
  bool DeleteSmallVisContours   ( size_t cNumber, double length   );

  /** \} */ 
  /**\ru \name Работа с разрывами
     \en \name Working with breaks
      \{ */ 

  /** \brief \ru Находится ли интервал на разрыве.
             \en Whether the interval is on a break. \~
    \details \ru Находится ли интервал параметров на разрыве кривой.\n
             \en Whether the interval of parameters is on a break of a curve.\n \~    
    \param[in] cNumber - \ru Номер кривой мультилинии.
                         \en Index of a curve of multiline. \~
    \param[in] rect    - \ru Интервал для проверки.
                         \en Interval to check. \~
    \return \ru true, если интервал полностью находится на разрыве или совпадает с ним.
            \en True if interval entirely is on the break or coincides with it. \~
  */
  bool IsRectInBreak             ( size_t cNumber, const MbRect1D & rect );   

  /** \brief \ru Запомнить разрывы.
             \en Memorize the breaks. \~
    \details \ru Запомнить разрывы невидимыми контурами.\n   
      Для использования в паре с AddBreaksByInvisContours.\n
      Удаляет все разрывы мультилинии.\n
      Все контуры invisContours имеют счетчик ссылок = 1 (вызван AddRef()).
             \en Memorize the breaks as invisible contours.\n   
      For using together with AddBreaksByInvisContours.\n
      Deletes all breaks of multiline.\n
      All 'invisContours' contours has a reference counter = 1 (called AddRef()). \~ 
    \param[out] invisContours - \ru Набор невидимых контуров всех кривых мультилинии.
                                \en Set of invisible contours of all curves of multiline. \~
  */
  void GetBreaksInInvisContours( RPArray<MbContour> & invisContours ); 

  /** \brief \ru Добавить разрывы.
             \en Add breaks. \~
    \details \ru Добавить разрывы невидимыми контурами.\n   
      Для использования в паре с GetBreaksInInvisContours.\n      
      Контуры invisContours удаляются (вызывается Release()).\n 
      Данной функцией можно наложить разрывы на мультилинию,
      если они были получены методом GetBreaksInInvisContours у этой мультилинии
      и форма мультилинии не была изменена.
             \en Add the breaks as invisible contours.\n   
      For using together with GetBreaksInInvisContours.\n      
      'invisContours' contours are deleted (called Release()).\n 
      It is possible to apply breaks to multiline by this function,
      if they were obtained by GetBreaksInInvisContours method from this multiline
      and shape of multiline wasn't changed. \~
    \param[in] invisContours - \ru Набор невидимых контуров всех кривых мультилинии.
                               \en Set of invisible contours of all curves of multiline. \~
  */
  void AddBreaksByInvisContours( RPArray<MbContour> & invisContours ); 
   
  /** \brief \ru Номера контуров, пересекаемых отрезком.
             \en Indices of contours intersected with a segment. \~
    \details \ru Номера контуров, которые пересекаются с отрезком по двум точкам.\n
             \en Indices of contours which are intersected with segment by two points.\n \~         
    \param[in]  p1       - \ru Первая точка отрезка.
                           \en The first point of a segment. \~
    \param[in]  p2       - \ru Вторая точка отрезка.
                           \en The second point of a segment. \~
    \param[out] cNumbers - \ru Номера контуров.
                           \en Indices of contours. \~
  */
  void CurvesIntersectNumbers   ( const MbCartPoint & p1, const MbCartPoint & p2, SArray<size_t> & cNumbers ) const; 

  /** \} */   
  /**\ru \name Информация о мультилинии
     \en \name Information about multiline.
      \{ */   
 
  /// \ru Вырождена ли мультилиния. \en Whether the multiline is degenerate. 
  bool   IsDegenerate( double lenEps = Math::LengthEps ) const; 
  /// \ru Замкнутая ли мультилиния. \en Whether the multiline is closed. 
  bool   IsClosed    ()                            const; 
  /// \ru Получить ширину мультилинии. \en Get width of multiline.        
  double GetWidth    ()                            const; 
  /// \ru Лежит ли данная точка на мультилинии. \en Whether the given point is on multiline. 
  bool   IsPointOn   ( const MbCartPoint & point ) const; 

  /** \brief \ru Найти индекс кривой мультилинии.
             \en Find an index of a curve of multiline. \~
    \details \ru Найти индекс кривой мультилинии.\n
             \en Find an index of a curve of multiline.\n \~         
    \param[in] radius - \ru Радиус нужной кривой.
                        \en Radius of the required curve. \~
    \return \ru Индекс кривой.
            \en A curve index. \~
  */
  size_t FindRadius  ( double radius );  

  /** \} */       
  /**\ru \name Операции с мультилинией
     \en \name Operations with multiline
      \{ */

  /** \brief \ru Усечь мультилинию.
             \en Truncate multiline. \~
    \details \ru Усечь мультилинию.\n 
      Вернуть мультилинию, базовая кривая которой - 
      копия участка между параметрами t1 и t2 базовой данной мультилинии.
             \en Truncate multiline.\n 
      Return multiline which base curve is 
      a copy of piece between parameters t1 and t2 of the base curve of a given multiline. \~
    \param[in] t1    - \ru Начальный параметр усечения.
                       \en Start parameter of trimming. \~
    \param[in] t2    - \ru Конечный параметр усечения.
                       \en End parameter of trimming. \~
    \param[in] sense - \ru Направление усеченной базовой кривой.
                       \en Direction of the trimmed base curve. \~
    \return \ru Новую мультилинию.
            \en New multiline. \~
  */
  MbMultiline * Trimmed( double t1, double t2, int sense ) const;  

  /** \} */        
private:
  /**\ru \name Внутренние функции мультилинии
     \en \name Internal functions of multiline
      \{ */

  // \ru Все внутренние функции реализованы в MltLine_.cpp (кроме тех, для которых указан другой файл) \en All internal functions implemented in MltLine_.cpp (except ones for which the other file is specified) 
  // \ru Насчет объектов \en Calculation of objects 
  /// \ru Насчитать кривую мультилинии c радиусом эквидистанты rad. \en Calculate curve of multiline with 'rad' equidistant radius. 
  void CalculateCurve             ( double rad, MbContourWithBreaks & contour );
  /// \ru Насчитать кривую мультилинии c радиусом эквидистанты rad c заполнением информации. \en Calculate curve of multiline with equidistant radius 'rad' and filling the information. 
  void CalculateCurveWithInfo     ( double rad, MbContourWithBreaks & contour, 
                                    SArray<size_t> & baseIndexes              );
  /// \ru Насчитать все кривые (с определением minNotDegInd и maxNotDegInd). \en Calculate all the curves (with determination of minNotDegInd and maxNotDegInd). 
  void CalculateCurves            ();
  /// \ru Насчитать все кривые с информацией для граничных невырожденных кривых. \en Calculate all the curves with information for non-degenerate boundary curves. 
  bool CalculateCurvesWithInfo    ( SArray<size_t> & minInfo, SArray<size_t> & maxInfo );
  /// \ru Насчитать все законцовки в вершинах. \en Calculate all tips at vertices. 
  void CalculateTipCurves         ( const SArray<size_t> & minInfo, const SArray<size_t> & maxInfo );
  /// \ru Насчитать все законцовки в вершинах. \en Calculate all tips at vertices. 
  void CalculateTipCurves         ();
  /// \ru Насчитать все кривые и все законцовки в вершинах. \en Calculate all curves and all tips at vertices. 
  void CalculateCurvesAndTipCurves();
  /// \ru Насчитать законцовку в начале. \en Calculate tip at the beginning. 
  void CalculateBegTipCurve       ( SArray<size_t> * changeCurvesNumbers = nullptr );
  /// \ru Насчитать законцовку в конце. \en Calculate tip at the end. 
  void CalculateEndTipCurve       ();

  // \ru Отцепление объектов \en Detach the objects 

  /// \ru Отцепить все кривые. \en Detach all curves. 
  void DeleteCurves               ();           
  /// \ru Отцепить кривую с индексом i. \en Detach i-th curve. 
  void DeleteCurve                ( size_t i ); 
  /// \ru Отцепить все законцовки в вершинах. \en Detach all tips at vertices. 
  void DeleteTipCurves            ();           
  /// \ru Отцепить законцовку в вершине с индексом i. \en Detach i-th tip at vertex. 
  void DeleteTipCurve             ( size_t i ); 
  /// \ru Отцепить законцовку в начале. \en Detach tip at the beginning. 
  void DeleteBegTipCurve          ();           
  /// \ru Отцепить законцовку в конце. \en Detach tip at the end. 
  void DeleteEndTipCurve          ();           
  /// \ru Отцепить все объекты. \en Detach all the objects. 
  void DeleteAllObjects           ();           

  // \ru Пересчет объектов \en Recalculation of objects 

  /// \ru Перестроить все объекты. \en Rebuild all objects. 
  void RebuildAllObjects          ();           

  // \ru Обработки изменения vertices (без перестроения) \en Processing of 'vertices' changing (without rebuilding) 
  void AddVert                    (           const StVertexOfMultilineInfo & vertInfo );
  void InsertVert                 ( size_t i, const StVertexOfMultilineInfo & vertInfo );
  void ChangeVert                 ( size_t i, const StVertexOfMultilineInfo & vertInfo );
  void RemoveVert                 ( size_t i                                           );

  // \ru Вспомогательные функции \en Auxiliary functions 

  /// \ru Обработать изменение i-ого сегмента БК. \en Process change of i-th segment of base curve. 
  void CalculateChangeOfSegment   ( size_t i ); 
  /// \ru Продлить кривую до законцовок. \en Extend curve to the tips. 
  void ProlongCurveToTips         ( size_t i ); 
  /// \ru Рассчитать удаление i-ой кривой (обработка). \en Calculate removal of i-th curve (processing). 
  void CalculateRemovalOfCurve    ( size_t i ); 

  // \ru Скругление/фаска двух соседних сегментов БК (реализация в MLOper.cpp) \en Fillet/chamfer of two neighboring segments of the base curve (implementation in MLOper.cpp) 
  bool FilletOrChamferTwoBasisSegs( ptrdiff_t & index, const StVertexOfMultilineInfo & vertInfo,
                                    bool fillet, bool recalcCrvRadii,
                                    double param,
                                    double par = 0.0, bool type = false, bool firstSeg = true );
  
  // \ru Работа с разрывами \en Working with breaks 
  // \ru Работа с разрывами при редактировании (сохранение фиксированной точки и длины) \en Working with breaks while editing (preserving of fixed point and length) 
  void SetBreaksFixedVars         ( size_t segInd, size_t subInd, const MbCartPoint & newPoint ); 
  void TransformBreaks            ( const MbMatrix & matr );

  // \ru Удалить разрывы на сегментах контура, с соотв. базовым номером \en Delete breaks on segments of contour with corresponding base index 
  void DeleteBreaksAtBaseNumber   ( size_t baseNumber, bool delTracingBreaks, bool delEquidBreaks, 
                                    bool delInLineSeg = true/*\ru Удалять ли разрывы с прямолинейных сегментов
                                                              \en Whether to remove breaks from straight segments \~*/ ); 
  
  // \ru Функции восстановления разрывов по невидимым контурам \en Functions for recovering breaks by invisible contours   
  void GetBreaksInInvisContours( bool allBreaks,                                // \ru Удалять все разырвы \en Delete all breaks 
                                 size_t i,                                      // \ru Номер вершины, специально для изменения типа обхода \en Index of vertex, specially for changing the type of traverse \~
                                 bool addContWbr,                               // \ru Набирать контуры с разрывами \en Collect contours with breaks 
                                 RPArray<MbContourWithBreaks> & breaksContours,
                                 RPArray<MbContour> & invisContours ); 
  void AddBreaksByInvisContours( RPArray<MbContourWithBreaks> & breaksContours,
                                 RPArray<MbContour> & invisContours ); 
  // \ru Для перестроения разрывов \en For breaks rebuilding 
  void GetBreaks               ( RPArray<MbBreaksRebuild> & baseNumbers );
  void RebuildBreaks           ( RPArray<MbBreaksRebuild> & baseNumbers );

  /** \} */  
private:
  void operator =( const MbMultiline & );     ///< \ru Не реализован \en Not implemented 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbMultiline )
}; // MbMultiline

IMPL_PERSISTENT_OPS( MbMultiline )

//------------------------------------------------------------------------------
// _7_ 
/** \brief \ru Построить скругления мультилинии.
           \en Construct a fillet of multiline. \~
  \details \ru Построить скругления базовой кривой мультилинии.\n
           \en Construct a fillet of the base curve of multiline.\n \~
  \param[in] multiline - \ru Изменяемая мультилиния.
                         \en A multiline to be modified. \~
  \param[in] rad       - \ru Радиус скругления.
                         \en The radius of fillet. \~
  \param[in] nodeFlag  - \ru Флаг выбора скругляемых вершин:\n
                         true - скругление всех вершин мультилинии,\n
                         false - скругление ближайшей вершины к точке pnt.
                         \en Flag of selection of vertices to fillet:\n
                         true - fillet of all vertices of multiline,\n
                         false - fillet of the vertex nearest to 'pnt' point. \~
  \param[in] pnt       - \ru Точка для указания нужной вершины.
                         \en Point for indication the required vertex. \~
  \param[in] vertInfo  - \ru Информация для новых вершин мультилинии.
                         \en Information for new vertices of a multiline. \~
  \ingroup Algorithms_2D
*/ // ---
MATH_FUNC (bool) FilletMultiline ( MbMultiline & multiline, double rad,
                                   bool nodeFlag, MbCartPoint & pnt,
                                   const StVertexOfMultilineInfo & vertInfo );


//------------------------------------------------------------------------------
// _8_ 
/** \brief \ru Построить фаски мультилинии.
           \en Construct a chamfer of multiline. \~
  \details \ru Построить фаски базовой кривой мультилинии.\n
           \en Construct a chamfer of the base curve of multiline.\n \~
  \param[in] multiline - \ru Изменяема мультилиния.
                         \en A multiline to be modified. \~
  \param[in] len       - \ru Длина фаски.
                         \en Length of chamfer. \~
  \param[in] par       - \ru Параметр в зависимости от типа type:\n
                         если type = true, par - угол,\n
                         если type = false, par - размер.
                         \en Parameter depending on 'type' type:\n
                         if type = true, par is a corner,\n
                         if type = false, par is a size. \~
  \param[in] type      - \ru Тип задания фаски:\n
                         true - фаска задана как размер + угол,\n
                         false - фаска задана как размер + размер.
                         \en The type of a chamfer specification:\n
                         true - chamfer specified as size + angle,\n
                         false - chamfer specified as size + size. \~  
  \param[in] nodeFlag  - \ru Флаг выбора обрабатываемых вершин:\n
                         true - фаска между каждыми двумя соседними сегментами мультилинии,\n
                         false - фаска между двумя соседними сегментами мультилинии, примыкающими к ближайшей к точке pnt вершине.
                         \en Flag of selection of vertices to process:\n
                         true - chamfer between each two neighboring segments of multiline,\n
                         false - chamfer between two neighboring multiline segments joining at the vertex nearest to 'pnt' point. \~
  \param[in] pnt       - \ru Точка для указания нужной пары сегментов.
                         \en Point for indication the required pair of segments. \~
  \param[in] vertInfo  - \ru Информация для новых вершин мультилинии.
                         \en Information for new vertices of a multiline. \~
  \ingroup Algorithms_2D
*/ // ---
MATH_FUNC (bool) ChamferMultiline( MbMultiline & multiline, double len, double par, bool type,
                                   bool nodeFlag, MbCartPoint & pnt,
                                   const StVertexOfMultilineInfo & vertInfo );


////////////////////////////////////////////////////////////////////////////////
//
// _9_ 
/// \ru Внеклассные функции расчета/учета радиусов кривизны (реализация в MltLine.cpp) \en Out-of-class functions for curvature radii calculation/consideration (implementation in MltLine.cpp) 
//
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
/** \brief \ru Учесть радиусы кривизны кривой.
           \en Consider the curve curvature radii. \~ 
  \details \ru Учесть минимальный положительный и максимальный отрицательный радиусы кривизны кривой.\n
    Для внутреннего использования.
           \en Consider the curve curvature minimum positive and maximum negative radii.\n
    For internal use only. \~
  \param[in]      curve  - \ru Кривая мультилинии.
                           \en A curve of multiline. \~
  \param[in]      angle  - \ru Угловая толерантность.
                           \en An angular tolerance. \~
  \param[in, out] minPos - \ru Радиус кривой, если он меньше текущего значения переменной minPos.
                           \en Curve radius if it is less than current value of 'minPos' variable. \~
  \param[in, out] maxNeg - \ru Радиус кривой, если он больше текущего значения переменной minPos.
                           \en Curve radius if it is greater than current value of 'minPos' variable. \~
  \ingroup Algorithms_2D
*/ // ---
void ToTakeIntoCurvesCrvRadii( MbCurve & curve, double angle, double & minPos, double & maxNeg );


//------------------------------------------------------------------------------
/** \brief \ru Получить радиусы кривизны контура.
           \en Get the contour curvatures radii. \~ 
  \details \ru Получить минимальный положительный и максимальный отрицательный радиусы кривизны контура.\n
    Для внутреннего использования.
           \en Get the contour curvature minimum positive and maximum negative radii.\n
    For internal use only. \~
  \param[in]  contour - \ru Контур.
                        \en A contour. \~
  \param[in]  angle   - \ru Угловая толерантность.
                        \en An angular tolerance. \~
  \param[out] minPos  - \ru Минимальный радиус сегмента контура.
                        \en Minimal contour segment radius. \~
  \param[out] maxNeg  - \ru Максимальный радиус сегмента контура.
                        \en Maximal contour segment radius. \~
  \ingroup Algorithms_2D
*/ // ---
void GetContoursCrvRadii( MbContour & contour, double angle, double & minPos, double & maxNeg );
  

//------------------------------------------------------------------------------
// _10_     
/** \brief \ru Состыковать две кривые.
           \en Join two curves. \~ 
  \details \ru Гладко состыковать две последовательные кривые.\n
    Для внутреннего использования.
           \en Smoothly join two consecutive curves.\n
    For internal use only. \~
  \param[in] curve1 - \ru Первая кривая.
                      \en The first curve. \~
  \param[in] curve2 - \ru Вторая кривая.
                      \en The second curve. \~
  \return \ru true, если хотя бы одна кривая была изменена.
          \en True if at least one curve has been changed. \~
  \ingroup Algorithms_2D
*/ // ---
bool SmoothJointSuccessiveCurves( MbCurve & curve1, MbCurve & curve2 );


//------------------------------------------------------------------------------
// _11_ 
/** \brief \ru Разбить мультилинию.
           \en Split multiline. \~ 
  \details \ru Разбить мультилинию на две части.
           \en Split multiline into two pieces. \~
  \param[in]  multiline - \ru Разбиваемая мультилиния.
                          \en A multiline to be split. \~
  \param[in]  p1        - \ru Точка разбиения или, если мультилиния замкнута, начальная точка для новой мультилинии.
                          \en Splitting point or start point of a new multiline if the multiline is closed. \~
  \param[in]  p2        - \ru Если мультилиния замкнута, то конечная точка для новой мультилинии.
                          \en End point of a new multiline if the multiline is closed. \~
  \param[out] parts     - \ru Массив полученных участков (2 элемента).
                          \en The array of obtained pieces (two elements). \~
  \ingroup Algorithms_2D
*/ // ---
MATH_FUNC (bool) BreakMultiline( const MbMultiline & multiline,
                                 const MbCartPoint & p1, const MbCartPoint & p2,
                                 PArray<MbMultiline> & parts );


//------------------------------------------------------------------------------
// _12_ 
/** \brief \ru Разбить мультилинию.
           \en Split multiline. \~ 
  \details \ru Разбить мультилинию на N равных частей.
           \en Split multiline into N equal pieces. \~
  \param[in]  multiline  - \ru Разбиваемая мультилиния.
                           \en A multiline to be split. \~
  \param[in]  partsCount - \ru Количество частей.
                           \en The count of pieces. \~
  \param[in]  point      - \ru Ограничивающая точка для замкнутой мультилинии.
                           \en Bounding point for closed multiline. \~
  \param[out] parts      - \ru Массив полученных участков (partsCount элементов).
                           \en The array of obtained pieces (partsCount elements). \~
  \ingroup Algorithms_2D
*/ // ---
MATH_FUNC (bool) BreakMultilineNParts( const MbMultiline & multiline, size_t partsCount,
                                       const MbCartPoint & point, PArray<MbMultiline> & parts );

// \ru LF-Linux: inline-функции следует помещать в заголовочный файл, а не cpp! \en LF-Linux: inline-functions should be placed in the header file instead of cpp! 
//-------------------------------------------------------------------------------
/// \ru Вырождена ли мультилиния \en Whether the multiline is degenerate 
// ---
inline bool MbMultiline::IsDegenerate( double lenEps ) const {
  return ( (minNotDegInd == SYS_MAX_T) || // \ru Значит, и maxNotDegInd == SYS_MAX_T \en So maxNotDegInd == SYS_MAX_T 
           (basisCurve != nullptr && basisCurve->IsDegenerate(lenEps)) );
}


//-------------------------------------------------------------------------------
/// \ru Замкнутая ли мультилиния \en Whether the multiline is closed 
// ---
inline bool MbMultiline::IsClosed() const {
  return ( basisCurve->IsClosed() && processClosed && basisCurve->GetSegmentsCount() > 1 );
}


//-------------------------------------------------------------------------------
/// \ru Получить ширину мультилинии \en Get width of multiline 
// ---
inline double MbMultiline::GetWidth() const
{
  if ( minNotDegInd == maxNotDegInd )
    return 0.0;
  else
    return ( equidRadii[maxNotDegInd] - equidRadii[minNotDegInd] );
}

//-------------------------------------------------------------------------------
/// \ru Обход скруглением (одним из) \en Traverse by fillet (one of) 
// ---
inline bool StVertexOfMultilineInfo::IsFilletTracing() const
{
  return ( tracingType == mvt_FilletType || tracingType == mvt_SpecFilletType );
}


//-------------------------------------------------------------------------------
///
// ---
inline bool StVertexOfMultilineInfo::operator ==( const StVertexOfMultilineInfo & with ) const
{
  return smoothJoint == with.smoothJoint && 
         tracingType == with.tracingType &&
         ::fabs(specFilletRad - with.specFilletRad) < EXTENT_REGION && 
         tipType     == with.tipType     &&
         firstSegTip == with.firstSegTip;
}


//-------------------------------------------------------------------------------
///
// ---
inline bool StVertexOfMultilineInfo::operator !=( const StVertexOfMultilineInfo & with ) const
{
  return !(*this == with);
}


//-------------------------------------------------------------------------------
/// \ru Инициализация по объекту \en Initialization by object 
// ---
inline void StVertexOfMultilineInfo::Init( const StVertexOfMultilineInfo & other )
{
  smoothJoint   = other.smoothJoint;
  tracingType   = other.tracingType;
  specFilletRad = other.specFilletRad;
  tipType       = other.tipType;
  firstSegTip   = other.firstSegTip;
}


//-------------------------------------------------------------------------------
/// \ru Инициализация по параметрам \en Initialize by parameters 
// ---
inline void StVertexOfMultilineInfo::Init( bool _smoothJoint, EnMLVertexTracingType _tracingType,
                                           double _specFilletRad, EnMLInnerTipType _tipType,
                                           bool _firstSegTip )
{
  smoothJoint   = _smoothJoint;
  tracingType   = _tracingType;
  specFilletRad = _specFilletRad;
  tipType       = _tipType;
  firstSegTip   = _firstSegTip;
}


//-------------------------------------------------------------------------------
/// \ru Изменить флаг "гладкий стык" (smoothJoint) \en Change flag "smooth joint" (smoothJoint) 
// ---
inline bool StVertexOfMultilineInfo::ChangeSmoothJoint( bool othSmoothJoint )
{
  if ( smoothJoint == othSmoothJoint )
    return false;
  else {
    smoothJoint = othSmoothJoint;
    return true;
  }
}


//-------------------------------------------------------------------------------
/// \ru Изменить тип обхода углов в вершине (tracingType) \en Change type of traverse of corners at a vertex (tracingType) 
// ---
inline bool StVertexOfMultilineInfo::ChangeTracingType( EnMLVertexTracingType othTracingType )
{
  if ( tracingType == othTracingType )
    return false;
  else {
    tracingType = othTracingType;
    return true;
  }
}


//-------------------------------------------------------------------------------
/// \ru Изменить радиус особого скругления (specFilletRad) \en Change radius of special fillet (specFilletRad) 
// ---
inline bool StVertexOfMultilineInfo::ChangeSpecFilletRad( double othSpecFilletRad )
{
  if ( ::fabs(specFilletRad - othSpecFilletRad) < Math::LengthEps )
    return false;
  else {
    specFilletRad = othSpecFilletRad;
    return true;
  }
}


//-------------------------------------------------------------------------------
/// \ru Изменить тип законцовки (tipType) \en Change type of tip (tipType) 
// ---
inline bool StVertexOfMultilineInfo::ChangeTipType( EnMLInnerTipType othTipType )
{
  if ( tipType == othTipType )
    return false;
  else {
    tipType = othTipType;
    return true;
  }
}


//-------------------------------------------------------------------------------
/// \ru Изменить флаг сегмента законцовки (isFirstSegTip) \en Change flag of segment of tip (isFirstSegTip) 
// ---
inline bool StVertexOfMultilineInfo::ChangeFirstSegTip( bool othFirstSegTip )
{
  if ( firstSegTip == othFirstSegTip )
    return false;
  else {
    firstSegTip = othFirstSegTip;
    return true;
  }
}


//-------------------------------------------------------------------------------
/// \ru Преобразовать объект согласно матрице \en Transform an object according to the matrix 
// ---
inline void StVertexOfMultilineInfo::Transform( const MbMatrix & matr )
{
  matr.TransformScalarX( specFilletRad ); // \ru Преобразовать радиус \en Transform radius 
}


//-------------------------------------------------------------------------------
///
// ---
inline bool StMLTipParams::operator ==( const StMLTipParams & with ) const
{
  return (tipType == with.tipType ) &&
         ::fabs(tipParam - with.tipParam) < EXTENT_REGION;
}


//-------------------------------------------------------------------------------
///
// ---
inline bool StMLTipParams::operator !=( const StMLTipParams & with ) const
{
  return !(*this == with);
}


//-------------------------------------------------------------------------------
/// \ru Инициализация \en Initialization 
// ---
inline void StMLTipParams::Init( const StMLTipParams & other )
{
  tipType  = other.tipType;
  tipParam = other.tipParam;
}


//-------------------------------------------------------------------------------
/// \ru Инициализация \en Initialization 
// ---
inline void StMLTipParams::Init( EnMLTipType _tipType, double _tipParam )
{
  tipType  = _tipType;
  tipParam = _tipParam;
}


//-------------------------------------------------------------------------------
/// \ru Изменить тип законцовки (tipType) \en Change type of tip (tipType) 
// ---
inline bool StMLTipParams::ChangeTipType( EnMLTipType othTipType )
{
  if ( tipType == othTipType )
    return false;
  else {
    tipType = othTipType;
    return true;
  }
}


//-------------------------------------------------------------------------------
/// \ru Изменить параметр законцовки (tipParam) \en Change parameter of tip (tipParam) 
// ---
inline bool StMLTipParams::ChangeTipParam( double othTipParam )
{
  if ( ::fabs(tipParam - othTipParam) < Math::LengthEps )
    return false;
  else {
    tipParam = othTipParam;
    return true;
  }
}


//-------------------------------------------------------------------------------
/// \ru Преобразовать объект согласно матрице \en Transform an object according to the matrix 
// ---
inline void StMLTipParams::Transform( const MbMatrix & matr )
{
  matr.TransformScalarX( tipParam ); // \ru Преобразовать расстояние \en Transform distance 
}


#endif // __MULTILINE_H
