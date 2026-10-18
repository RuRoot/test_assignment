//////////////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Типы данных утилиты обнаружения столкновений.
         \en Data types of collision detection. \~
*/
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CDET_DATA_H
#define __CDET_DATA_H

#include <templ_sptr.h>
#include <topology.h>
#include <set>
#include <map>

class CdSceneItem;

/**
  \addtogroup Collision_Detection
  \{
*/

//----------------------------------------------------------------------------------------
/** \brief \ru Код результата контроля столкновений или измерений. 
           \en Codes of collision detection or measuring query.    
*/
//---
enum CDM_result
{
  /*
    Attention: (!)  Dont reorder codes because they are sorted by priority.
  */
  CDM_RESULT_None,
  /// \ru Успешный результат измерительного запроса (например, запрос на минимальное расстояние).
  /// \en Successful result of a measurement request (for example, a distance query).
  CDM_RESULT_MeasuringSucceed,
  /// \ru Столкновний не выявлено. Пересечений нет или поиск был прерван.
  /// \en No collision was detected. There are no intersections or the search was aborted.
  CDM_RESULT_NoCollisionDetected,
  /// \ru Выявлено хотя бы одно объемное пересечение (коллизия).
  /// \en Volume intersection (collision) detected.
  CDM_RESULT_CollisionDetected,
  /// \ru Неизвестная ошибка или внутрисистемная ошибка.
  /// \en Unknown error or internal system error.
  CDM_RESULT_Error,
};

//----------------------------------------------------------------------------------------
/// \ru Объект из набора контроля столкновений. \en Object from the set of collision detection.
//---
typedef const CdSceneItem * cdet_item;
typedef CDM_result          cdet_result; ///< \ru Код результата контроля столкновений. \en Result code of collision queries.

//----------------------------------------------------------------------------------------
// Codes of collision detection. Deprecated values, use CDM_result instead.
//---
const CDM_result CDET_RESULT_Intersected    = CDM_RESULT_CollisionDetected;
const CDM_result CDET_RESULT_NoIntersection = CDM_RESULT_NoCollisionDetected;
const CDM_result CDET_RESULT_Ok             = CDM_RESULT_MeasuringSucceed;
const CDM_result CDET_RESULT_None           = CDM_RESULT_None;
const CDM_result CDET_RESULT_Error          = CDM_RESULT_Error;

//----------------------------------------------------------------------------------------
// \ru Геометрический объект пользователя. \en User geometric item.
//---
typedef const void * cdet_app_item;

//----------------------------------------------------------------------------------------
// Constants
//---
const cdet_item     CDET_NULL = nullptr;     ///< \ru Пустой объект набора для контроля столкновений. \en Empty object of the collision query set.
const cdet_app_item CDET_APP_NULL = nullptr; ///< \ru "Нулевой" объект модели приложения. \en "Null object" of the client app.

//----------------------------------------------------------------------------------------
/** \brief  \ru Структура данных и обратных вызовов для запроса на поиск соударений.
            \en Data structure and callbacks for the collision search request.
    \details
    \ru Базовый класс, предназначенный для реализации на стороне приложения структуры 
  запроса к алгоритму поиска соударений. С помощью обратных вызовов приложение
  управляет поиском соударений и получает сведения o найденных пересечениях.
    \en The base class intended to implement on the application side the structure of a query 
  to the collision search algorithm. The application uses a callback to control the collision 
  search and obtain details of detected interferencies.
            
*/
//---
struct MATH_CLASS cdet_query
{
  enum cback_res  ///< Result code of the callback function
  {
      CBACK_VOID    
    , CBACK_SUFFICIENT  ///< This code means that an app stops collision query for the given pair of geometric objects.
    , CBACK_SKIP        ///< Skip testing the given pair.
    , CBACK_NEED        ///< Enable testing the given pair.
    , CBACK_BREAK       ///< Break search of all collisions of the set.
    , CBACK_SEARCH_MORE = CBACK_VOID  ///< This code notifies a collision detector to continue working at cases CDET_INTERSECTED, CDET_TOUCHED.
  };

  enum message  ///< Code of notification
  {
      CDET_NONE             ///< No messages.
    , CDET_QUERY_STARTED    ///< The collision query is started for the all objects of the scene set.
    , CDET_STARTED          ///< The collision query is started for the given pair of geometric objects.
    , CDET_FINISHED         ///< Collision detector complete searching collisions for the given pair of geometric objects.
      // The codes below indicates the intersection state for a pair.
    , CDET_NO_INTERSECTION  ///< Definitely the pair has no intersection (this enum value for internal use only).
    , CDET_TOUCHED          ///< Touched faces has been founded with no penetration of the solids.    
    , CDET_INTERSECTED      ///< The collided pair of objects founded.    
    , CDET_INCLUDED         ///< The included pair of objects founded.
  };

  struct geom_element ///< Structure representing a collision detection geometry.
  {
    cdet_app_item       appItem;  ///< Application pointer to a geometric object.
    const MbRefItem *   refItem;  ///< Pointer to the collided face or grid.
    const MbMatrix3D *  wMatrix;  ///< Matrix mapping the collided item to The world CS of the scene.
    geom_element()
      : appItem( nullptr )
      , refItem( nullptr )
      , wMatrix( &MbMatrix3D::identity ) {}
  };

  struct cback_data ///< Data structure that notifies an app about collision detection event.
  {    
    geom_element first, second; ///< Pair of geometric objects.
    cback_data(): first(), second() {}
  };

  // If message is CDET_INCLUDED, then cback_data::first is in cback_data::second.
  cback_res operator() ( message code, cback_data & cData ) { return func( this, code, cData ); }  

protected:
  typedef cback_res (*cback_func)( cdet_query *, message, cback_data & );

  cdet_query( cback_func _func ) : func(_func) {}
  cdet_query( const cdet_query & cQuery ) : func(cQuery.func) {}
 ~cdet_query() {}

  cdet_query & operator = ( const cdet_query & cQuery ) { func = cQuery.func; return *this; }

private:
  cback_func func;
};

//----------------------------------------------------------------------------------------
// Simple collision query data to search for the first detected interference.
//---
struct MATH_CLASS cdet_query_result: public cdet_query
{  
  cdet_result result;

  cdet_query_result() 
    : cdet_query( QueryFunc )
    , result( CDM_RESULT_NoCollisionDetected )
  {}

  cdet_query_result( const cdet_query_result & cQuery ) 
    : cdet_query( cQuery )
    , result( cQuery.result )
  {}


private:  
  static cback_res QueryFunc( cdet_query * query, message code, cback_data & )
  { 
    C3D_ASSERT( nullptr != query );
    cdet_query_result * q = static_cast<cdet_query_result*>( query );
    switch( code )
    {      
    case CDET_QUERY_STARTED: // The collision query is started for all solids of the scene.
      {
        q->result = CDM_RESULT_NoCollisionDetected;
        return CBACK_VOID;
      }
    case CDET_INTERSECTED: // First intersection is founded.
      {
        q->result = CDM_RESULT_CollisionDetected;
        return CBACK_SUFFICIENT;
      }            
    case CDET_FINISHED: // A pair of solids is finished.
      return (q->result == CDM_RESULT_CollisionDetected) ? CBACK_BREAK : CBACK_VOID;

    default:
      return CBACK_VOID;
    }
  }
};

//----------------------------------------------------------------------------------------
// The structure queries first founded collision faces.
//---
struct MATH_CLASS cdet_first_collided: public cdet_query
{ 
  SPtr<const MbRefItem> first, second;  // collided faces

  cdet_first_collided() 
    : cdet_query( QueryFunc )
    , first()
    , second()
  {}

private:
  static cback_res QueryFunc( cdet_query * query, message code, cback_data & cData )
  {
    if ( cdet_first_collided * q = static_cast<cdet_first_collided*>(query) )
    {
      switch( code )
      {      
      case CDET_QUERY_STARTED: // The collision query is started for all solids of the set
        {
          q->first = q->second = nullptr;
          return CBACK_VOID;
        }
      case CDET_FINISHED: // A pair of solids is finished.
        return (q->first && q->second) ? CBACK_BREAK : CBACK_SEARCH_MORE;

      case CDET_INTERSECTED: // First intersection is founded.
        {
          q->first = cData.first.refItem;
          q->second = cData.second.refItem;
          return (q->first && q->second) ? CBACK_SUFFICIENT : CBACK_SEARCH_MORE;
        }            
      default:
        return CBACK_VOID;
      }
    }
    return CBACK_VOID;
  }
  OBVIOUS_PRIVATE_COPY( cdet_first_collided );
};

//----------------------------------------------------------------------------------------
/** \brief  \ru Структура запроса для поиска граней столкновения.
            \en The structure of the query to find collision faces.
*/
//---
struct MATH_CLASS cdet_collided_faces: public cdet_query
{ 
  typedef std::pair<cdet_app_item,const MbRefItem*> item_face;  // represents a face of app item
  typedef std::set<item_face> collided_faces;
  collided_faces                        faces;
  std::map<cdet_app_item,cdet_app_item> groups;
  cdet_app_item                         excluded; // a member of excluded group

public:
  cdet_collided_faces()
    : cdet_query( _QueryFunc )
    , faces()
    , groups()
    , excluded( CDET_APP_NULL )
  {}

  /** \brief  \ru Объединить пару геометрических объектов в группу.
              \en Unite a pair of geometric items to the group.
    \details  \ru Функция объединяет в группу два отдельных объекта или присоединяет 
                  первый объект к группе, которой принадлежит второй. Если оба объекта уже 
                  принадлежат каждый своей группе, то обе группы сливаются в одну общую.
              \en The function unites to group two separate objects or the first object 
                  attaches to the group, which owns the second. If both objects already 
                  belong to each of their group, the two groups merged into a single.
  */
  void Group( cdet_app_item fst, cdet_app_item snd )
  {
    fst = PivoteItem_( fst );
    snd = PivoteItem_( snd );
    if ( fst < snd )
    {
      std::swap( fst, snd );      
    }
    groups[fst] = snd;
  }

  /** \brief  \ru Исключить из контроля на столкновения тела группы.
              \en Exclude from the collision control solids of the group.
      \param[in] member - \ru Любой участник группы, элементы которой исключаются.
                          \en Any member of the group whose elements are excluded. \~ 
  */
  void ExludeGroup( cdet_app_item member )
  {
    if ( excluded == CDET_APP_NULL )
      excluded = PivoteItem_( member );
    else
      Group( excluded, member );
  }
  /** \brief  \ru Отменить результаты работы функций Group() и ExludeGroup().
              \en Cancel the results of the functions Group() and ExludeGroup().
  */
  void Reset()
  {
    faces.clear();
    groups.clear();
    excluded = CDET_APP_NULL;
  }

private:
  static cback_res _QueryFunc( cdet_query * query, message code, cback_data & cData )
  {
    if ( cdet_collided_faces * q = static_cast<cdet_collided_faces*>(query) )
    {
      switch( code )
      {      
      case CDET_QUERY_STARTED:
        {
          q->faces.clear();
          return CBACK_VOID;
        }
      case CDET_STARTED:
        {
          if ( q->_SameGroups(cData.first.appItem,cData.second.appItem) )
          {
            return CBACK_SKIP;
          }
          return CBACK_VOID;
        }

      case CDET_FINISHED: // a pair of solids was finished.
        return CBACK_SEARCH_MORE;

      case CDET_INTERSECTED:
        {
          if ( cData.first.refItem )
          {
            q->faces.insert( item_face(cData.first.appItem,cData.first.refItem) );
          }
          if ( cData.second.refItem )
          {
            q->faces.insert( item_face(cData.second.appItem,cData.second.refItem) );
          }
          return CBACK_SEARCH_MORE;
        }            
      default:
        return CBACK_VOID;
      }
    }
    return CBACK_VOID;
  }

  cdet_app_item PivoteItem_( cdet_app_item appItem ) const
  {
    std::map<cdet_app_item,cdet_app_item>::const_iterator iter = groups.find( appItem );
    if ( iter == groups.end() || (iter->second == iter->first) )
    {
      return appItem;
    }
    C3D_ASSERT( iter->second < iter->first );

    return PivoteItem_( iter->second );
  }

  bool  _SameGroups( cdet_app_item fst, cdet_app_item snd ) const
  {    
    return PivoteItem_( fst ) == PivoteItem_( snd );
  }

  OBVIOUS_PRIVATE_COPY( cdet_collided_faces );
};

//----------------------------------------------------------------------------------------
//
//---
typedef enum
{
    CDET_EXAM_EnableComponentsOnly  // Test collisions between components only.
  , CDET_EXAM_Enabled   // Enable the pair to test collisions.
  , CDET_EXAM_Disabled  // Reject the collision test of the pair.  
} CDM_exam_status;

//----------------------------------------------------------------------------------------
//
//---
struct MATH_CLASS CDM_item_data
{
  cdet_item comp = CDET_NULL; // Descriptor of the component owning the inctance.
  cdet_item inst = CDET_NULL; // Descriptor of the instance.
  cdet_app_item appItem = CDET_APP_NULL; // Application pointer for the instance or the component.
  CDM_item_data()
  {
    comp = inst = CDET_NULL;
    appItem = CDET_APP_NULL;
  }
};

//----------------------------------------------------------------------------------------
//
//---
typedef CDM_exam_status (*CDM_exam_func)( cdet_query *, const CDM_item_data &, const CDM_item_data & );
typedef double (*CDM_tolerance_func)( cdet_query *, const CDM_item_data & );

/** \} */ // Collision_Detection

//----------------------------------------------------------------------------------------
/** \brief \ru Грань столкновения.  \en Face of collision. \~
*/
// ---
class MATH_CLASS CdCollisionFace
{
  cdet_item           item;      // The instance to witch the face belongs.
  SPtr<const MbFace>  mathFace;  // The topological face identified in the collision detection or proximity query.
  TapeBase*           partFace;  // The face of the application representation.

public:
  CdCollisionFace();
  CdCollisionFace(const MbFace& face) 
    : item( CDET_NULL )
    , mathFace( &face )
    , partFace( nullptr ) 
  {}  
  CdCollisionFace(const CdCollisionFace&) = default;
  CdCollisionFace(CdCollisionFace&& other);
public:
  const MbFace& Face() const { return *mathFace; }
  cdet_item     Item() const { return item;  }
  const MbFace& GetMathFace() const { return Face(); }
  // \ru Установка объекта модели. \en Setting an object of model.
  void SetCollisionFaceObject(TapeBase* _partFace) { partFace = _partFace; }
  
  // \ru Выдача объекта модели. \en Getting an object of model.
  TapeBase * GetCollisionFaceObject() const { return partFace; }

  // \ru Задать грань и компонент-вставку, которой принадлежит. \en Set a face and its component-instance.
  CdCollisionFace & SetFace( const MbFace * f, cdet_item inst )
  {
    mathFace = f;
    item = inst;
    return *this;
  }

  CdCollisionFace & operator = ( const CdCollisionFace & other )
  { 
    item     = other.item;
    mathFace = other.mathFace;
    partFace = other.partFace;
    return *this;
  }
  bool operator >   ( const CdCollisionFace & other ) const { return mathFace >  other.mathFace; }
  bool operator <   ( const CdCollisionFace & other ) const { return mathFace <  other.mathFace; }
  bool operator ==  ( const CdCollisionFace & other ) const { return mathFace == other.mathFace; }
  bool operator !=  ( const CdCollisionFace & other ) const { return !(*this == other); }

private:
  
};

//----------------------------------------------------------------------------------------
/** \brief  \ru Параметры запроса и результат измерения расстояния между двух объектов.
            \en Parameters and the result of the query to measure distance between geometric objects. \~
*/
// ---
class MATH_CLASS CdProximityParameters
{
  CdCollisionFace m_face1;
  CdCollisionFace m_face2;
public:
  MbCartPoint3D fstPnt, sndPnt;   // \ru Пара точек близости, принадлежащие триангуляционным сеткам. \en The points of the proximity belonging to the triangulation grids.
  MbCartPoint thePar1, thePar2;   // \ru Пара точек близости, заданная в поверхностных координатах граней. \en The points of the proximity specified in the surface coordinates of the faces.
  double      theDistance;        // \ru Расстояние. \en Distance. 
  double      upperDist;          // \ru Верхняя оценка для поиска минимальной дистанции. \en The upper bound of the minimal distance estimation.
public:
  CdProximityParameters();
  CdProximityParameters(const CdProximityParameters&) = default;
  CdProximityParameters(CdProximityParameters&&) = default;
  CdProximityParameters& operator = (const CdProximityParameters&) = default;
 ~CdProximityParameters();
public:
  const CdCollisionFace & FaceOne() const { return m_face1; }
  const CdCollisionFace & FaceTwo() const { return m_face2; }
  void  SetFacePair( const MbFace &, const MbFace & );
  void  SetFacePair( const MbFace *, cdet_item, const MbFace *, cdet_item );
  /// \ru Сбросить результат измерения в неопределенное состояние. \en Reset the measured result  to the undefined state.
  CdProximityParameters & Reset(double _upperDistance = MB_MAXDOUBLE);
protected:
  CdProximityParameters(const MbFace&, const MbFace&, const MbCartPoint& uv1, const MbCartPoint& uv2, double dist);
};

//----------------------------------------------------------------------------------------
// \ru Сбросить результат измерения в неопределенное состояние. \en Reset the measured result  to the undefined state.
//---
inline CdProximityParameters& CdProximityParameters::Reset(double _upperDistance)
{
  m_face1.SetFace(nullptr, CDET_NULL);
  m_face2.SetFace(nullptr, CDET_NULL);
  fstPnt = sndPnt = MbCartPoint3D::origin;
  thePar1 = thePar2 = MbCartPoint::origin;
  theDistance = -1.0;
  upperDist = _upperDistance;
  return *this;
}


// deprecated typenames.
using MbHRepSolid = CdSceneItem;  
using MbCollisionFace = CdCollisionFace;
using MbProximityParameters = CdProximityParameters;

#endif // __CDET_DATA_H

// eof