////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Определение резьбы.
         \en Thread definition. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MB_THREAD_H
#define __MB_THREAD_H


#include <legend.h>
#include <mb_placement3d.h>
#include <mb_cube.h>
#include <mb_matrix.h>
#include <surface.h>
#include <topology.h>
#include <lump.h>
#include <solid.h>
#include <name_item.h>
#include <iterator>
#include <set>
#include <vector>


class    MbMatrix3D;
class    MbSurface;
class    MbCurveEdge;
class    MbFace;


//------------------------------------------------------------------------------
/** \brief \ru Тип отображения.
           \en Mapping type. \~
  \details \ru Тип отображения резьбы.
           \en A type of thread mapping. \~
  \ingroup Mapping
*/
// ---  
enum MbeThrMapType { 
  tmt_CompleteView = 0, ///< \ru Полный вид. Система координат в строителе отображения резьбы ThreadMapperStruct определяет плоскость вида. Отображаем все. \en Full view. Coordinate system in creator of thread mapping ThreadMapperStruct determines plane of view. Map all. 
  tmt_CuttedView,       ///< \ru Вид-разрез. Система координат в строителе отображения резьбы ThreadMapperStruct определяет плоскость разреза. Отображаем только то, что за плоскостью. \en The cutaway-view. Coordinate system in creator of thread mapping ThreadMapperStruct determines plane of cutaway. Map only that behind plane. 
  tmt_SectionView,      ///< \ru Сечение. Система координат в строителе отображения резьбы ThreadMapperStruct определяет плоскость сечения. Отображаем только то что на плоскости. \en Section. Coordinate system in creator of thread mapping ThreadMapperStruct determines section plane. Map only that on plane. 
};


//------------------------------------------------------------------------------
/** \brief \ru Состояние резьбы.
           \en Thread state. \~
  \details \ru Состояние резьбы.
           \en Thread state. \~
  \ingroup Mapping
*/
// ---  
enum MbeThrState {
  ts_NotChanged = 0, ///< \ru Не изменилась. \en Not changed. 
  ts_Changed,        ///< \ru Изменилась. \en Changed. 
  ts_Degenerated,    ///< \ru Выродилась. \en Degenerated. 
};


//------------------------------------------------------------------------------
/** \brief \ru Метод подгонки резьбы.
           \en Method of thread fitting (adapting). \~
  \details \ru Метод подгонки начала и конца резьбы.
           \en Method of fitting start and end of thread. \~
  \ingroup Mapping
*/
// ---
enum MbeThrAdapt {
  ta_UsingNothing = 0,  ///< \ru Не подгонять, только проверить. \en No fit, check only. 
  ta_UsingGabarit,      ///< \ru По габариту тела. \en By solid bounding box. 
  ta_UsingSurfaces,     ///< \ru По поверхностям тела под резьбой. \en By solid surfaces below thread. 
  ta_UsingSolid,        ///< \ru По телу. \en By solid.
  ta_TypesCount         ///< \ru Количество типов адаптации. \en Count of thread fitting methods.
};


//------------------------------------------------------------------------------
/** \brief \ru Ошибки при проверке резьбового соединения.
           \en Threaded joint check errors. \~
  \details \ru Ошибки при проверке резьбового соединения.
           \en Threaded joint check errors. \~
  \ingroup Legend
*/
// ---
enum MbeThreadedJointCheckErrors {
  tjc_NoErrors              = 0,  ///< \ru Нет ошибок. \en No errors.
  tjc_SameThread            = 1,  ///< \ru Ошибка - одна и та же резьба. \en Error - the same thread.
  tjc_WrongThreadParameters = 2,  ///< \ru Ошибка - некорректные параметры резьбы. \en Error - wrong thread parameters.
  tjc_NormalsMismatch       = 3,  ///< \ru Ошибка - несоответствие по нормалям (обе внешние или обе внутренние). \en Error - normals mismatch (both threads are outer or inner).
  tjc_RotationsMismatch     = 5,  ///< \ru Ошибка - несоответствие вращательных направлений нарезки (одна левая, другая правая). \en Error - the mismatch of rotational directions of the cut (one is left rotation, the other is right rotation).
  tjc_AngularMismatch       = 6,  ///< \ru Ошибка - угловое несоответствие конических резьб. \en Error - angular mismatch of tapered threads.
  tjc_AxesNotCollinear      = 7,  ///< \ru Ошибка - оси резьб не коллинеарны. \en Error - threads axes are not collinear.
  tjc_DepthOverlapMismatch  = 8,  ///< \ru Ошибка - недостаточное перекрытие по глубине поперек оси. \en Error - insufficient depth overlap transverse to both axes.
  tjc_LengthOverlapMismatch = 9,  ///< \ru Ошибка - недостаточное перекрытие по длине вдоль оси. \en Error - insufficient length overlap along the axis.
  tjc_PositionsCount        = 10, ///< \ru Количество позиций списка. \en List's positions count.
};


//------------------------------------------------------------------------------
/** \brief \ru Параметры проверки резьбовых соединений.
           \en Threaded joint check parameters. \~
  \details \ru Параметры проверки резьбовых соединений.\n
           \en Threaded joint check parameters.\n \~
  \ingroup Legend
*/
// --- 
struct MATH_CLASS MbThreadedJointCheckParameters {
protected:
  double  metricAccuracy;     ///< \ru Метрическая точность. \en Metric accuracy. 
  double  angleAccuracy;      ///< \ru Угловая точность. \en Angular accuracy. 
  double  minLengthOverlap;   ///< \ru Минимальная длина перекрытия резьб вдоль оси. \en Minimal thread overlap along axis. 
  double  minDepthOverlap;    ///< \ru Минимальная глубина перекрытия резьб. \en Minimal thread depth overlap. 

  bool    checkFullDepth;     ///< \ru Требование полного перекрытия резьб по глубине (иначе - minDepthOverlap). \en Full thread depth overlap required (otherwise - minDepthOverlap). 
  bool    checkRotCoherence;  ///< \ru Проверить согласованность вращений резьб. \en Check threaded rotations matching. 
  bool    collectErros;       ///< \ru Собрать все ошибки сопряжения. \en Collect all threaded joint errors. 

public:
  MbThreadedJointCheckParameters()
    : metricAccuracy   ( METRIC_PRECISION )
    , angleAccuracy    ( ANGLE_REGION     )
    , minLengthOverlap ( METRIC_PRECISION )
    , minDepthOverlap  ( c3d::MIN_RADIUS  )
    , checkFullDepth   ( false            )
    , checkRotCoherence( false            )
    , collectErros     ( false            )
  {}
  MbThreadedJointCheckParameters( const MbThreadedJointCheckParameters & par )
    : metricAccuracy   ( par.metricAccuracy    )
    , angleAccuracy    ( par.angleAccuracy     )
    , minLengthOverlap ( par.minLengthOverlap  )
    , minDepthOverlap  ( par.minDepthOverlap   )
    , checkFullDepth   ( par.checkFullDepth    )
    , checkRotCoherence( par.checkRotCoherence )
    , collectErros     ( par.collectErros      )
  {}
  MbThreadedJointCheckParameters & operator = ( const MbThreadedJointCheckParameters & par )
  {
    metricAccuracy    = par.metricAccuracy;
    angleAccuracy     = par.angleAccuracy;
    minLengthOverlap  = par.minLengthOverlap;
    minDepthOverlap   = par.minDepthOverlap;
    checkFullDepth    = par.checkFullDepth;
    checkRotCoherence = par.checkRotCoherence;
    collectErros      = par.collectErros;
    return *this;
  }
public:
  double GetMetricAccuracy()      const { return metricAccuracy;     } ///< \ru Угловая точность. \en Angular accuracy. 
  double GetAngleAccuracy()       const { return angleAccuracy;      } ///< \ru Метрическая точность. \en Metric accuracy. 
  double GetMinLengthOverlap()    const { return minLengthOverlap;   } ///< \ru Минимальная длина перекрытия резьб вдоль оси. \en Minimal thread overlap along axis. 
  double GetMinDepthOverlap()     const { return minDepthOverlap;    } ///< \ru Минимальная глубина перекрытия резьб. \en Minimal thread depth overlap. 
  bool   CheckFullDepthOverlap()  const { return checkFullDepth;     } ///< \ru Проверять полное перекрытия резьб по глубине (иначе - только на глубину minDepthOverlap). \en Check full thread depth overlap (otherwise - minDepthOverlap only). 
  bool   CheckRotationsMatching() const { return checkRotCoherence;  } ///< \ru Проверить согласованность вращений резьб. \en Check threaded rotations matching. 
  bool   CollectErrors()          const { return collectErros;       } ///< \ru Собрать все ошибки сопряжения. \en Collect all threaded joint errors. 

  void   SetFullDepthCheck         ( bool s ) { checkFullDepth    = s; } ///< \ru Установить требование проверки полного перекрытия резьб по глубине. \en Set full thread depth overlap check (otherwise - minDepthOverlap). 
  void   SetRotationsMatchingCheck ( bool s ) { checkRotCoherence = s; } ///< \ru Установить состояние проверки согласованности вращений резьб. \en Set threaded rotation matching check. 
  void   SetCollectErrors          ( bool s ) { collectErros      = s; } ///< \ru Установить состояние сбора всех ошибок сопряжения. \en Set gathering state of all errors of threaded joint. 

  /// \ru Установить метрическую точность. \en Set metric accuracy. 
  bool   SetMetricAccuracy( double mAcc )
  {
    bool res = true;
    mAcc = ::fabs( mAcc );
    metricAccuracy = mAcc;
    if ( metricAccuracy < EXTENT_EQUAL ) {
      metricAccuracy = EXTENT_EQUAL;
      res = false;
    }
    else if ( metricAccuracy > METRIC_NEAR ) {
      metricAccuracy = METRIC_NEAR;
      res = false;
    }
    return res;
  }
  /// \ru Установить угловую точность. \en Set angular accuracy.
  bool   SetAngularAccuracy( double aAcc )
  {
    bool res = true;
    aAcc = ::fabs( aAcc );
    angleAccuracy = aAcc;
    if ( angleAccuracy < EXTENT_EQUAL ) {
      angleAccuracy = EXTENT_EQUAL;
      res = false;
    }
    else if ( angleAccuracy > PARAM_NEAR ) {
      angleAccuracy = PARAM_NEAR;
      res = false;
    }
    return res;
  }
  /// \ru Установить минимальную глубина перекрытия резьб. \en Set minimal thread depth overlap. 
  void SetLengthOverlap( double v ) { minLengthOverlap = v; }
  /// \ru Установить минимальную длина перекрытия резьб вдоль оси. \en Set minimal thread overlap along axis. 
  void SetDepthOverlap ( double v ) { minDepthOverlap  = v; }
};


//------------------------------------------------------------------------------
/** \brief \ru Резьба.
           \en Thread. \~
  \details \ru Резьба отверстий и валов.\n
           \en Thread of holes and shafts.\n \~
  \ingroup Legend
*/
// --- 
class MATH_CLASS MbThread : public MbLegend {
public:
  typedef std::pair<const MbSpaceItem *,MbMatrix3D> ThreadLimiter;
  typedef std::pair<ThreadLimiter,ThreadLimiter> ThreadLimiters;
  typedef std::vector<MbeThreadedJointCheckErrors> ThreadedJointErrors;
protected:
  MbPlacement3D       place;   ///< \ru Система координат резьбы (в мировой системе координат). \en A thread coordinate system (in the world coordinate system). 
  double              radObj;  ///< \ru Начальный радиус резьбы на поверхности. \en Initial thread radius on surface. 
  double              radThr;  ///< \ru Начальный радиус резьбы в теле. \en Initial thread radius in solid. 
  double              length;  ///< \ru Длина резьбы. \en Thread length. 
  double              angle;   ///< \ru Угол конусности поверхности резьбы. \en Conicity angle of thread surface. 

private: // \ru Временные данные для проецирования резьб \en Temporary data for projection of threads 
  MbName            * name;    // \ru Имя резьбы (именно указатель, т.к. имя из 3D потом идет ссылкой в аннотационные кривые). \en Thread name (just pointer, because name from 3D later goes as reference to annotation curves).
  c3d::ConstLumpsSet  bodies;  // \ru Рабочий массив тел, на которых нарезана резьба, может быть пустой. \en Working array of threaded solids can be empty. 
                               // \ru Наполнять с помощью функции AddBodies. \en Fill using AddBodies function. 
private: // \ru Данные не влияющие на неизменность. \en Data not influencing on invariableness. 
  mutable MbFace    * faceObj;
  mutable MbFace    * faceThr;
  mutable MbCube      cube;    // \ru Габаритный куб резьбы. \en Bounding box of thread. 
  mutable MbeThrState state;   // \ru Состояние резьбы. \en Thread state. 

public:
  
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор.\n
             \en Constructor.\n \~  
    \param[in] place - \ru Система координат.
                       \en Coordinate system. \~
    \param[in] rObj  - \ru Начальный радиус резьбы на поверхности.
                       \en Initial thread radius on surface. \~
    \param[in] rThr  - \ru Начальный радиус резьбы в теле.
                       \en Initial thread radius in solid. \~
    \param[in] len   - \ru Длина резьбы.
                       \en Thread length. \~
    \param[in] ang   - \ru Угол конусности поверхности резьбы.
                       \en Conicity angle of thread surface. \~
  */
  MbThread( const MbPlacement3D & place, double rObj, double rThr, double len, double ang );

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор.\n
             \en Constructor.\n \~  
    \param[in] place - \ru Система координат.
                       \en Coordinate system. \~
    \param[in] rObj  - \ru Начальный радиус резьбы на поверхности.
                       \en Initial thread radius on surface. \~
    \param[in] rThr  - \ru Начальный радиус резьбы в теле.
                       \en Initial thread radius in solid. \~
    \param[in] begPos - \ru Положение начала резьбы вдоль оси Z системы координат.
                        \en Thread start position along the Z axis of the coordinate system. \~
    \param[in] begPos - \ru Положение конца резьбы вдоль оси Z системы координат.
                        \en Thread end position along the Z axis of the coordinate system. \~
    \param[in] ang   - \ru Угол конусности поверхности резьбы.
                       \en Conicity angle of thread surface. \~
  */
  MbThread( const MbPlacement3D & place, double rObj, double rThr, double begPos, double endPos, double ang );

  /// \ru Деструктор. \en Destructor. 
  virtual ~MbThread();

private:     
  MbThread();  ///< \ru Конструктор. \en Constructor. 
  MbThread( const MbThread & );  ///< \ru Конструктор копирования. \en Copy-constructor. 
  const MbThread & operator = ( const MbThread & thr ) { Init( thr ); return *this; }

public:
  
  /**\ru \name Общие функции геометрического объекта.
     \en \name Common functions of a geometric object.
      \{ */   
  MbeSpaceType  IsA()  const override;
  MbeSpaceType  Type() const override;
  MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const override;
  bool        IsSame  ( const MbSpaceItem &, double accuracy = LENGTH_EPSILON ) const override;
  bool        IsSimilar ( const MbSpaceItem & ) const override;
  bool        SetEqual( const MbSpaceItem & ) override;
  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override;
  void        Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override;
  void        Rotate   ( const MbAxis3D   &, double, MbRegTransform * = nullptr ) override;
  double      DistanceToPoint  ( const MbCartPoint3D & ) const override;
  void        AddYourGabaritTo( MbCube & ) const override;
  void        Refresh() override;
  void        CalculateMesh( const MbStepData & stepData, const MbFormNote & note, MbMesh & mesh ) const override; // \ru Построить полигональную копию mesh. \en Build polygonal copy mesh.

  // \ru Тестовые функции геометрического объекта \en Test functions of a geometric object 
  MbProperty & CreateProperty( MbePrompt ) const override;  // \ru Создать собственное свойство \en Create own property 
  void        GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of object 
  void        GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

public : 
  /** \} */   
  /**\ru \name Функции инициализации.
     \en \name Initialization functions.
      \{ */  
        
        /** \brief \ru Инициализация.
                   \en Initialization. \~
          \details \ru Инициализация.\n
                   \en Initialization.\n \~  
          \param[in] place  - \ru Система координат.
                              \en Coordinate system. \~
          \param[in] radObj - \ru Начальный радиус резьбы (на поверхности).
                              \en Initial thread radius (on surface). \~
          \param[in] radThr - \ru Конечный радиус резьбы.
                              \en Final thread radius. \~
          \param[in] len    - \ru Длина резьбы.
                              \en Thread length. \~
          \param[in] ang    - \ru Угол.
                              \en Angle. \~
        */
        bool          SetThreadParams( const MbPlacement3D & place, double radObj, double radThr, double len, double ang );
                                 
        /** \brief \ru Инициализация.
                   \en Initialization. \~
          \details \ru Инициализация.\n
                   \en Initialization.\n \~  
          \param[in] place   - \ru Система координат.
                               \en Coordinate system. \~
          \param[in] diamObj - \ru Начальный диаметр.
                               \en Initial diameter. \~
          \param[in] depth   - \ru Глубина нарезки
                               \en Depth of threading \~
          \param[in] bOutThr - \ru Признак внешней резьбы.\n
                               если true - резьба внешняя,\n
                               если false - резьба внутренняя.
                               \en External thread attribute.\n
                               if true then thread is external,\n
                               If false then thread is internal. \~
          \param[in] len     - \ru Длина резьбы.
                               \en Thread length. \~
          \param[in] ang     - \ru Угол.
                               \en Angle. \~
        */
        bool          SetThreadParams( const MbPlacement3D & place, double diamObj, double depth, bool bOutThr, double len, double ang );

  /** \} */   
  /**\ru \name Функции работы с именами.
     \en \name Functions for working with names.
      \{ */

        /** \brief \ru Установить имя.
                   \en Set name. \~
          \details \ru Установить имя резьбы.\n
                   \en Set thread name.\n \~ 
          \param[in] n - \ru Новое имя.
                         \en New name. \~
        */
        void          SetName( const MbName & n ) { name = const_cast<MbName *>(&n); }  

        /** \brief \ru Получить имя.
                   \en Get name. \~
          \details \ru Получить имя резьбы.\n
                   \en Get thread name.\n \~ 
          \return \ru Указатель на имя.
                  \en Pointer to name. \~
        */
        MbName      * GetName() const { return name; }     
        
        /** \brief \ru Есть ли имя.
                   \en Is there a name. \~
          \details \ru Есть ли имя у резьбы.\n
                   \en Is there a name of a thread.\n \~ 
          \return \ru true, если имя есть и оно не пустое.
                  \en True if there is name and it is not empty. \~
        */
        bool          IsName() const { return ((name != nullptr) ? name->IsEmpty() : false); }

  /** \} */   
  /**\ru \name Функции работы с телами, на которых нарезана резьба.
     \en \name Functions for operating with threaded solids.
      \{ */

        /** \brief \ru Добавить тела.
                   \en Add solids. \~
          \details \ru Добавить тела, на которых нарезана резьба.\n 
            Не добавляются нулевые указатели из присланного массива,
            не добавляются повторяющиеся объекты.
                   \en Add threaded solids.\n 
            Null pointers from specified array are not added,
            duplicate objects also not added. \~
          \param[in] lumps - \ru Набор тел.
                             \en Set of solids. \~
        */
        template <class ConstLumps>
        void          AddBodies( const ConstLumps & initBodies )
        {
          for ( size_t k = 0, cnt = initBodies.size(); k < cnt; ++k ) {
            bodies.insert( initBodies[k] );
          }
        }

        /** \brief \ru Найти тело.
                   \en Find solid. \~
          \details \ru Найти тело в массиве.\n
                   \en Find solid in array.\n \~                
          \return \ru Индекс тела.\n
                  SYS_MAX_T, если тело не найдено.
                  \en Solid index.\n
                  SYS_MAX_T, if solid isn't found. \~
        */
        bool          IsBody( const MbLump * ) const;   

        size_t        GetBodiesCount() const { return bodies.size(); }    ///< \ru Количество тел. \en Count of solids. 

        /** \brief \ru Получить тела.
                   \en Get solids. \~
          \details \ru Получить массив тел.\n
                   \en Get array of solids.\n \~                
          \param[out] lumps - \ru Результат - набор тел.
                              \en Set of solids as result. \~          
        */
        template <class ConstLumps>
        void          GetBodies( ConstLumps & lumps ) const { lumps.reserve( lumps.size() + bodies.size() ); std::copy( bodies.begin(), bodies.end(), std::back_inserter(lumps) ); }

        /** \brief \ru Отцепить тела, не принадлежащие этой резьбе.
                   \en Detach solids unsuitable for this thread. \~
          \details \ru Отцепить тела, не принадлежащие этой резьбе.\n
                   \en Detach solids unsuitable for this thread.\n \~                
        */
        void          DetachWrongBodies();

        /// \ru Отцепить тела, на которых нарезана резьба. \en Detach threaded solids. 
        void          DetachBodies();          

  /** \} */   
  /**\ru \name Функции доступа к данным. Информация о резьбе.
     \en \name Functions for access to data. Information about thread.
      \{ */

        /** \brief \ru Корректны ли параметры.
                   \en Check if parameters are correct. \~
          \details \ru Корректны ли параметры резьбы:\n 
            - резьба не должна быть вырожденной,\n
            - система координат резьбы должна быть ортонормированной,\n
            - значения радиусов и длины должны быть 
              не меньше минимального радиуса объекта MIN_RADIUS,
              не больше максимального радиуса объекта MAX_RADIUS,\n
            - радиусы должны быть различны,\n
            - угол не должен превосходить полный оборот,\n
            - резьба не должна иметь самопересечения.
                   \en Check if thread parameters are correct:\n 
            - thread should not be degenerate,\n
            - thread coordinate system should be orthonormalized,\n
            - values of radius and length should be 
              not less than minimal radius of object MIN_RADIUS,
              not greater than maximal radius of object MAX_RADIUS,\n
            - radii should be different,\n
            - angle should not be greater than full turn,\n
            - thread should not have self-intersections. \~
          \return 
        */
        bool          IsValid()         const;

        /** \brief \ru Является ли резьба конической.
                   \en Check if thread is conic. \~
          \details \ru Является ли резьба конической.\n
                   \en Check if thread is conic.\n \~          
          \return \ru true, если значение угла не нулевое.
                  \en True if value of angle is non-zero. \~
        */
        bool          IsConical()       const;

        /** \brief \ru Является ли резьба внешней.
                   \en Check if thread is external. \~
          \details \ru Является ли резьба внешней.\n   
            Резьба внешняя, если начальный радиус резьбы больше конечного.
                   \en Check if thread is external.\n   
            Thread is external if initial radius of thread is greater than final one. \~
          \return \ru Признак внешней резьбы.
                  \en External thread attribute. \~
        */
        bool          IsOutside()       const;

        /** \brief \ru Является ли резьба левой.
                   \en Check if thread is left. \~
          \details \ru Является ли резьба левой.\n
                   \en Check if thread is left.\n \~             
          \return \ru true, если система координат резьбы левая.
                  \en True if thread coordinate system is left. \~
        */
        bool          IsLeft()          const { return place.IsLeft(); } 

        /// \ru Получить СК резьбы в мировой СК. \en Get thread coordinate system in world coordinate system. 
  const MbPlacement3D & GetPlacement()  const { return place;  } 

        /// \ru Получить начальный радиус резьбы на поверхности. \en Get initial thread radius on surface. 
        double        GetObjBegRadius() const { return radObj; }
        /// \ru Получить конечный радиус резьбы на поверхности. \en Get final thread radius on surface. 
        double        GetObjEndRadius() const;
        /// \ru Получить начальный радиус резьбы в теле. \en Get initial thread radius in solid. 
        double        GetThrBegRadius() const { return radThr; }
        /// \ru Получить конечный радиус резьбы в теле. \en Get final thread radius in solid. 
        double        GetThrEndRadius() const;

        /// \ru Получить длину резьбы. \en Get thread length. 
        double        GetLength() const { return length; }
        /// \ru Получить угол конусности поверхности резьбы. \en Get conicity angle of thread surface. 
        double        GetAngle()  const { return angle;  }
        /// \ru Узнать состояние резьбы. \en Get thread state. 
        MbeThrState   GetState()  const { return state;  }

        /// \ru Глубина нарезки. \en Depth of threading. 
        double        GetDepth()        const { return ::fabs(radThr - radObj); }
        /// \ru Получить начальную точку резьбы на оси. \en Get start point of thread on axis. 
        void          GetBegAxisPoint( MbCartPoint3D & ) const;
        /// \ru Получить конечную  точку резьбы на оси. \en Get start point of thread on axis. 
        void          GetEndAxisPoint( MbCartPoint3D & ) const;
        
        /** \brief \ru Сопрягаются ли резьбы.
                   \en Check if threads are mating. \~
          \details \ru Сопрягаются ли резьбы (внутренняя с внешней.\n 
            1) Резьбы должны иметь одинаковый угол.\n
            2) Одна резьба должна быть внутренней, другая внешней.\n
            3) У обоих объектов должны быть корректные параметры.\n
            4) Оси резьб должны быть параллельны.\n
                   \en Check if threads are mated (internal with external).\n 
            1) Threads must have same angle.\n
            2) One of threads must be internal, other - external.\n
            3) Each object must have correct parameters.\n
            4) Axes of threads must be parallel.\n \~  
          \param[in] otherThread - \ru Вторая резьба.
                                   \en Second thread. \~  
          \return \ru true, если резьбы сопрягаемые.
                  \en True if threads are mating. \~
        */             
        bool          IsMatedTo( const MbThread & otherThread ) const;

        /** \brief \ru Сопрягаются ли резьбы.
                   \en Check if threads are mating. \~
          \details \ru Сопрягаются ли резьбы (внутренняя с внешней.\n 
            1) Резьбы должны иметь одинаковый угол.\n
            2) Одна резьба должна быть внутренней, другая внешней.\n
            3) У обоих объектов должны быть корректные параметры.\n
            4) Оси резьб должны быть параллельны.\n
                   \en Check if threads are mated (internal with external).\n 
            1) Threads must have same angle.\n
            2) One of threads must be internal, other - external.\n
            3) Each object must have correct parameters.\n
            4) Axes of threads must be parallel.\n \~  
          \param[in] otherThread - \ru Вторая резьба.
                                   \en Second thread. \~  
          \param[in] checkParams - \ru Параметры проверки резьбовых соединений.
                                   \en Threaded joint check parameters. \~
          \return \ru true, если резьбы сопрягаемые.
                  \en True if threads are mating. \~
        */             
        bool          IsMatedTo( const MbThread &                       otherThread, 
                                 const MbThreadedJointCheckParameters & checkParams,
                                       ThreadedJointErrors *            thrJointErrors = nullptr ) const;

        /// \ru Принадлежит ли резьба грани. \en Check if thread belongs to face. 
        bool          IsFaceThread( const MbFace *, const MbMatrix3D & ) const;    
        
        /** \brief \ru Принадлежит ли резьба телу.
                   \en Check if thread belongs to solid. \~                       
          \details \ru Принадлежит ли резьба телу.\n
                   \en Check if thread belongs to solid.\n \~   
          \param[in]  solid         - \ru Тело.
                                      \en A solid. \~
          \param[in]  matrix        - \ru Матрица преобразования в мировую систему координат.
                                      \en A matrix of transformation to the world coordinate system. \~
          \param[out] simObjNumbers - \ru Индексы граней, которым может принадлежать резьба.
                                      \en Indices of faces thread belongs to. \~
          \param[out] intObjNumbers - \ru Индексы граней, c которым может пересекаться резьба.
                                      \en Indices of faces intersecting with thread. \~
          \return \ru true, если резьба принадлежит одной из граней тела.
                  \en True if thread belongs to one of solid faces. \~
        */ 
        bool          IsBodyThread( const MbSolid & solid, const MbMatrix3D & matrix, 
                                    c3d::IndicesVector * simObjNumbers = nullptr,
                                    c3d::IndicesVector * intObjNumbers = nullptr ) const;

        /** \brief \ru Принадлежит ли резьба телу.
                   \en Check if thread belongs to solid. \~                       
          \details \ru Принадлежит ли резьба телу.\n
                   \en Check if thread belongs to solid.\n \~   
          \param[in]  solid - \ru Тело с матрицей преобразования в мировую систему координат.
                              \en A solid with a matrix of transformation to the world coordinate system. \~
          \return \ru true, если резьба принадлежит одной из граней тела.
                  \en True if thread belongs to one of solid faces. \~
        */ 
        bool          IsBodyThread( const MbLump & lump ) const;
       
        /** \brief \ru Адаптировать начало и конец резьбы к телу.
                   \en Fit start and end of thread to solid. \~                       
          \details \ru Адаптировать начало и конец резьбы к телу, если резьба может принадлежать телу.\n
                   \en Fit start and end of thread to solid if thread can belong to solid.\n \~              
          \param[in] solid    - \ru Тело.
                                \en A solid. \~
          \param[in] matrix   - \ru Матрица преобразования в мировую систему координат.
                                \en A matrix of transformation to the world coordinate system. \~
          \param[in] thrAdapt - \ru Метод подгонки резьбы.
                                \en Method of thread fitting. \~
          \param[in] limiters - \ru Начальный и конечный ограничители резьбы.
                                \en Beginning and ending thread limiters. \~
          \return \ru true в случае успеха операции.
                  \en True if the operation is successful. \~
        */ 
        bool          AdaptToBody( const MbSolid & solid, const MbMatrix3D & matrix, MbeThrAdapt thrAdapt, const ThreadLimiters * limiters = nullptr );

        /** \brief \ru Выдать начало и конец изменённой резьбы относительно исходной.
                   \en Get limit positions of the modified thread in regard to an initial thread. \~                       
          \details \ru Выдать начало и конец изменённой резьбы относительно исходной вдоль оси Z системы координат исходной резьбы.\n
                   \en Get limit positions of the modified thread in regard to an initial thread.
                       Positions are defined along the Z axis of the initial coordinate system. \n \~              
          \param[in] thread  - \ru Исходная резьба.
                               \en Initial thread. \~
          \param[out] begPos - \ru Положение начала резьбы.
                               \en Thread start position. \~
          \param[out] endPos - \ru Положение конца резьбы.
                               \en Thread end position. \~
          \return \ru true Если резьбы подобны.
                  \en True if these threads are similar to each other. \~
        */ 
        bool          GetLimitPositions( const MbThread & thread, double & begPos, double & endPos ) const;

        /** \brief \ru Найти тела, которым может принадлежать резьба.
                   \en Find solids thread belongs to. \~                       
          \details \ru Найти тела, которым может принадлежать резьба.\n
                   \en Find solids thread belongs to.\n \~              
          \param[in]  solids - \ru Набор тел.
                               \en Set of solids. \~
          \param[in]  matrices - \ru Набор матриц преобразования тел в мировую систему координат.
                                     Количество должно соответствовать количеству тел.
                                 \en Set of matrices of solid transformation to the world coordinate system.
                                     Count must be equal to count of solids. \~
          \param[out] solidsNumbers - \ru Номера тел, которым может принадлежать резьба.
                                      \en Indices of solids thread belongs to. \~
          \return \ru true, если резьба принадлежит хотя бы одному телу.
                  \en True if thread belongs at least to one of solids. \~
        */
        template <class SolidsVector, class MatricesVector, class IndicesVector>
        bool          FindThreadBodies( const SolidsVector & solids, const MatricesVector & matrices, 
                                        IndicesVector & solidsNumbers ) const;

public:
        /// \ru Рассчитать габаритный куб резьбы. \en Calculate bounding box of thread. 
  const MbCube &      CalculateGabarit() const;   
        
        /** \brief \ru Рассчитать габаритный куб резьбы.
                   \en Calculate bounding box of thread. \~
          \details \ru Рассчитать габаритный куб резьбы в локальной системе координат.\n
                   \en Calculate bounding box of thread in local coordinate system.\n \~             
          \param[in]  mIntoLocal - \ru Локальная система координат.
                                   \en A local coordinate system. \~
          \param[out] cubeLocal  - \ru Результат - посчитанный габарит.
                                   \en Calculated bounding box as result. \~ 
        */
        void          CalculateLocalGabarit( const MbMatrix3D & mIntoLocal, MbCube & cubeLocal ) const override; 

        void          Init( const MbThread & );               // \ru Инициализировать копию \en Initialize copy (may be public if required) 
  /** \} */   

  const MbFace *     _GetObjectFace() const { return faceObj; }
  const MbFace *     _GetThreadFace() const { return faceThr; }
    
private: // \ru Закрытые собственные функции резьбы \en Own private functions of thread 
        MbeThrState   SetState( MbeThrState ) const;          // \ru Установить состояние резьбы. \en Set thread state. 
        MbeThrState   CheckState();                           // \ru Проверить и скорректировать состояние резьбы. \en Check and correct thread state. 
        bool          SetDepth ( double );                    // \ru Изменить глубину резьбы. \en Change thread depth.
        bool          SetLength( double );                    // \ru Изменить длину резьбы. \en Change thread length.
        bool          SetLimits( double zMin, double zMax );  // \ru Изменить резьбу по новым положениям начала и конца. \en Change begin and end of the thread.

        void          CreateThreadFaces() const;                   // \ru Создать резьбовые грани в СК мира. \en Create threaded faces in world coordinate system. 
        void          UpdateThreadFaces() const;                   // \ru Обновить резьбовые грани. \en Update threaded faces. 
        void          MoveThreadFaces( const MbVector3D & ) const; // \ru Сдвинуть резьбовые грани. \en Move threaded faces. 
        void          DeleteThreadFaces() const;                   // \ru Удалить резьбовые грани. \en Delete threaded faces. 

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbThread )
};

IMPL_PERSISTENT_OPS( MbThread )


//------------------------------------------------------------------------------
// \ru Начальная точка резьбы на оси \en Start point of thread on axis 
// ---
inline void MbThread::GetBegAxisPoint( MbCartPoint3D & p ) const {
  p.Init( place.GetOrigin() );
}

//------------------------------------------------------------------------------
// \ru Конечная точка резьбы на оси \en End point of thread on axis 
// ---
inline void MbThread::GetEndAxisPoint( MbCartPoint3D & p ) const {
  p.Set( place.GetOrigin(), place.GetAxisZ(), length );
}


//------------------------------------------------------------------------------
// \ru Получить конечный радиус поверхности резьбы \en Get final radius of thread surface 
// ---
inline double MbThread::GetObjEndRadius() const {
  return (radObj + length * ::tan( angle ));
}

//------------------------------------------------------------------------------
// \ru Получить конечный радиус резьбы \en Get final radius of thread 
// ---
inline double MbThread::GetThrEndRadius() const {
  return (radThr + length * ::tan( angle ));
}   


//------------------------------------------------------------------------------
// \ru Найти тела, которым может принадлежать резьба. \en Find solids thread belongs to.
// ---
template <class SolidsVector, class MatricesVector, class IndicesVector>
bool MbThread::FindThreadBodies( const SolidsVector & solids, const MatricesVector & matrices, IndicesVector & indices ) const
{
  const size_t solidsCnt = solids.size();

  if ( solidsCnt > 0 && solidsCnt == matrices.size() ) {
    const size_t indicesCnt0 = indices.size();

    if ( cube.IsEmpty() )
      CalculateGabarit();

    indices.reserve( indices.size() + 2 );

    MbCube solidCube;
    for ( size_t i = 0; i < solidsCnt; ++i ) {
      const MbSolid * solid = solids[i];
      if ( solid != nullptr && solid->GetShell() != nullptr ) {
        solidCube.SetEmpty();
        solid->AddYourGabaritTo( solidCube );
        if ( cube.Intersect( solidCube ) && IsBodyThread( *solid, matrices[i] ) )
          indices.push_back( i );
      }
    }
    return (indices.size() > indicesCnt0);
  }

  return false;
}


//------------------------------------------------------------------------------
/** \brief \ru Удалить некорректные резьбы.
           \en Delete incorrect threads. \~
  \details \ru Удалить вырожденные и некорректно расположенные относительно плоскости вида резьбы.\n
    Из набора резьб удаляются:\n
    1) нулевые указатели,\n 
    2) резьбы с некорректными параметрами,\n
    3) резьбы без имен,\n
    4) повторяющиеся резьбы (в случае повторов появляется предупреждение - таких ситуаций быть не должно),\n
    5) некорректно расположенные относительно плоскости вида: 
       ось резьбы должна быть параллельна или перпендикулярна оси Z плоскости.
           \en Delete degenerate threads and ones incorrectly located relative to plane of view.\
    From set of threads will be deleted:\n
    1) null-pointers,\n 
    2) threads with incorrect parameters,\n
    3) threads without names,\n
    4) duplicate threads (warning raised in case of duplication - shouldn't be such situations),\n
    5) incorrectly located relative to plane of view: 
       thread axis must be parallel or perpendicular to Z-axis of plane. \~
  \param[in,out] threads  - \ru Набор резьб.
                            \en Set of threads. \~
  \param[in]     placeSec - \ru Плоскость вида.
                            \en Plane of view. \~
  \param[in]     checkThreadNames - \ru Проверять наличие имени у резьбы.
                                    \en Whether a name of thread exists. \~
  \ingroup Mapping
*/
// --- 
template <class ThreadsVector>
bool CheckThreads( ThreadsVector & threads, const MbPlacement3D * placeSec, bool checkThreadNames, double angleEps = Math::angleRegion )
{
  size_t cntThreads = threads.size();

  if ( cntThreads > 0 ) {
    for ( size_t i = threads.size(); i--; ) {
      MbThread * thr = threads[i];

      if ( (thr == nullptr) || !thr->IsValid() ) {
        threads.erase( threads.begin() + i );
        thr = nullptr;
      }
      else if ( checkThreadNames && (thr->GetName() == nullptr) ) { // C3D-695 : KOMPAS-25125
        threads.erase( threads.begin() + i );
        thr = nullptr;
      }
      else {
        if ( threads.size() > 1 ) {
          for ( ptrdiff_t j = i - 1; j >= 0; j-- ) {
            if ( thr == threads[j] ) {
              threads[i] = nullptr;
              threads.erase( threads.begin() + i );
              C3D_ASSERT_UNCONDITIONAL( false ); // Error case!
              thr = nullptr;
              break;
            }
          }
        }
      }
      if ( thr != nullptr ) // KOMPAS-37171
        thr->DetachWrongBodies();
    }

    if ( !threads.empty() && (placeSec != nullptr) ) {
      const MbVector3D & axisZsec = placeSec->GetAxisZ();
      for ( size_t i = threads.size(); i--; ) {
        const MbThread * thr = threads[i];
        const MbVector3D & axisZthr = thr->GetPlacement().GetAxisZ();
        bool isColinear = axisZthr.Colinear( axisZsec, angleEps );
        bool isOrthogonal = axisZthr.Orthogonal( axisZsec, angleEps );
        if ( !isColinear && !isOrthogonal )
          threads.erase( threads.begin() + i );
      }
    }
  }

  return (threads.size() > 0);
}


//------------------------------------------------------------------------------
/** \brief \ru Получить центральную точку.
           \en Get a central point. \~
  \details \ru Получить центральную точку замкнутого ребра или замкнутой цепочки ребер.
           \en Get a central point of closed edge or closed chain of edges. \~
  \param[in]  edge - \ru Ребро.
                     \en Edge. \~
  \param[out] pnt  - \ru Результат - центральная точка.
                     \en Central point as result. \~
  \ingroup Mapping
*/
// --- 
MATH_FUNC (bool) GetThreadEdgeCentre( const MbCurveEdge & edge, MbCartPoint3D & pnt );


//------------------------------------------------------------------------------
/** \brief \ru Найти граничные точки резьбы.
           \en Find the boundary point of the thread.\~
  \details \ru Найти граничные точки резьбы по локальному габариту ребер, 
               соединяющих грань резьбы и смежную с ней грань.
               Точка на оси резьбы упорядочена вдоль направления оси резьбы.
           \en Find the boundary point of the thread on the edges size.
               The point on the thread axis streamlined along the direction of the thread axis.\~
  \param[in] threadFace - \ru Грань резьбы.
                          \en The face of thread. \~
  \param[in] threadAxis - \ru Ось резьбы.
                          \en The axis of thread. \~
  \param[in] face - \ru Грань, смежная с гранью резьбы.
                    \en Face adjacent to the edge of the thread. \~
  \param[out] threadDir - \ru Направление резьбы.
                          \en The direction of the thread. \~
  \param[out] facePnt1 - \ru Начальная точка на смежной грани.
                         \en Starting point on the adjacent face. \~
  \param[out] facePnt2 - \ru Конечная точка на смежной грани.
                         \en The final point on the adjacent face. \~
  \param[out] axisPnt1 - \ru Начальная точка на оси резьбы.
                         \en Starting point for the thread axis. \~
  \param[out] axisPnt2 - \ru Конечная точка на оси резьбы.
                         \en Endpoint to the thread axis. \~
  \param[in] dr - \ru Глубина резьбы.
                  \en Thread depth. \~
  \param[in] findCommonEdges - \ru Искать общие ребра.
                               \en Find common edges. \~
  \result \ru Возвращает true, если получилось определить.
          \en Returns true, if successful. \~
  \ingroup Mapping
*/
// --- 
MATH_FUNC (bool) CalculateThreadLimits( const MbFace &         threadFace,
                                        const MbAxis3D &       threadAxis,
                                        const MbFace &         face, 
                                              MbVector3D &     threadDir,
                                              MbCartPoint3D &  facePnt1,
                                              MbCartPoint3D &  facePnt2,
                                              MbCartPoint3D &  axisPnt1,
                                              MbCartPoint3D &  axisPnt2,
                                              double           dr = 0.0,
                                              bool             findCommonEdges = true );


#endif // __MB_THREAD_H
