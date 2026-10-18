////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Функции сравнения и тестирования тел.
         \en Functions for solids comparison and testing. \~
*/
////////////////////////////////////////////////////////////////////////////////
#ifndef __ATS_CHECK_H
#define __ATS_CHECK_H


#include <name_item.h>
#include <templ_p_array.h>
#include <topology_item.h>
#include <mb_cart_point3d.h>
#include <mb_vector3d.h>
#include <templ_visitor.h>


class  MbSolid;


//------------------------------------------------------------------------------
/** \brief \ru Различия примитивов.
           \en Differences of primitives. \~
  \details \ru Различия примитивов.\n
           \en Differences of primitives.\n \~
  \ingroup Algorithms_3D
*/
// ---
struct MATH_CLASS PrimitiveDifference {
public:    
  /// \ru Типы различий в именовании \en Types of naming differences   
  enum DifferenceType
  {    
    dt_Geometry = 0, ///< \ru Изменения в геометрии. \en Changes in geometry. 
    dt_NameChanged,  ///< \ru Изменилось наименование. \en Name has changed. 
    dt_NameNotFound, ///< \ru Не найдено соответствие имени. \en A correspondence for the name was not found. 
    dt_NameMultiple, ///< \ru Найдено более одного соответствия имени. \en There are more than one correspodences for the name. 
  };       
private:
  MbeTopologyType objType; ///< \ru Тип объекта с различиями. \en The type of an object with differences.     
public:       
  PrimitiveDifference( MbeTopologyType type ) : objType( type ) {}  ///< \ru Коннструктор по типу топологического объекта. \en Constructor by a type of topological object.  
  virtual ~PrimitiveDifference() {}
public:     
  MbeTopologyType    GetObjType() const { return objType; }         ///< \ru Тип топологического объекта. \en The type of topological object       
  virtual void  Accept( Visitor & visitor ) = 0;               ///< \ru Прием посетителя. \en Acceptance of a visitor. 
};


//------------------------------------------------------------------------------
/** \brief \ru Различие в именовании двух примитивов.
           \en Naming difference between two primitives. \~
  \details \ru Различие в именовании двух примитивов или ненайденный примитив.\n
           \en Naming difference between two primitives or not found primitive.\n \~
  \ingroup Algorithms_3D
*/
// ---
struct MATH_CLASS NameDifference : public PrimitiveDifference {
public:
  MbName         name1;    ///< \ru Имя некоего примитива в первом объекте. \en The name of a primitive in the first object. 
  MbName         name2;    ///< \ru Имя того же примитива во втором объекте. \en The name of a primitive in the first object. 
  DifferenceType diffType; ///< \ru Тип различия. \en A type of difference. 
public:
  /// \ru Различие в именовании двух примитивов. \en Naming difference between two primitives. 
  NameDifference( const MbName & n1, const MbName & n2, DifferenceType dType, MbeTopologyType oType ); 
  /// \ru Ненайденный примитив. \en The found primitive.   
  NameDifference( const MbName & n,                     DifferenceType dType, MbeTopologyType oTType );                  
public:
  /** \brief \ru Это различие в именовании?
             \en Is this a naming difference? \~
    \details \ru Это различие в именовании?\n
             \en Is this a naming difference?\n \~
    \return \ru true, если это различие в именовании,\n
      иначе это ненайденный примитив или геометрическое различие.\n
            \en true if this is a naming difference,\n
      otherwise this is a unfound primitive or geometric difference.\n \~
  */
  bool IsNamesDifference() const;  
                                  
VISITING_CLASS( NameDifference )
};


//------------------------------------------------------------------------------
/** \brief \ru Различие в количестве.
           \en Difference in count. \~
  \details \ru Различие в количестве.\n
           \en Difference in count.\n \~
  \ingroup Algorithms_3D
*/
// ---
class MATH_CLASS CountDifference : public PrimitiveDifference {
public:
  size_t cnt1;   ///< \ru Количество компонентов данного типа в первом объекте. \en The number of components of the given type in the first object. 
  size_t cnt2;   ///< \ru Количество компонентов данного типа во втором объекте. \en The number of components of the given type in the second object. 
  bool   valid;  ///< \ru Подсчитаны корректные объекты. \en Calculated objects are correct. 
public:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор.\n
             \en Constructor.\n \~
    \param[in] n1    - \ru Количество компонентов данного типа в первом объекте.
                       \en The number of components of the given type in the first object. \~
    \param[in] n2    - \ru Количество компонентов данного типа во втором объекте.
                       \en The number of components of the given type in the second object. \~
    \param[in] good  - \ru Подсчитаны корректные объекты.
                       \en Calculated objects are correct. \~
    \param[in] oType - \ru Тип объекта с различиями.
                       \en Type of an object with differences. \~  
  */
  CountDifference( size_t n1, size_t n2, bool good, MbeTopologyType oType )
    : PrimitiveDifference( oType )
    , cnt1( n1 )
    , cnt2( n2 )
    , valid( good )
  {}

VISITING_CLASS( CountDifference )
};


//------------------------------------------------------------------------------
/** \brief \ru Различие точек.
           \en Difference of points. \~
  \details \ru Различие точек.\n
           \en Difference of points.\n \~
  \ingroup Algorithms_3D
*/
// ---
class MATH_CLASS PointDifference : public PrimitiveDifference {
public:
  MbCartPoint3D pnt1; ///< \ru Первая точка. \en The first point. 
  MbCartPoint3D pnt2; ///< \ru Вторая точка. \en The second point. 
public:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор.\n
             \en Constructor.\n \~
    \param[in] p1      - \ru Первая точка.
                         \en The first point. \~
    \param[in] p2      - \ru Вторая точка.
                         \en The second point. \~
    \param[in] objType - \ru Тип объекта с различиями.
                         \en Type of an object with differences. \~  
  */
  PointDifference( const MbCartPoint3D & p1, const MbCartPoint3D & p2, MbeTopologyType objType );

VISITING_CLASS( PointDifference )
};


//------------------------------------------------------------------------------
/** \brief \ru Различие нормалей.
           \en Difference of normals. \~
  \details \ru Различие нормалей.\n
           \en Difference of normals.\n \~
  \ingroup Algorithms_3D
*/
// ---
class MATH_CLASS VectorDifference : public PrimitiveDifference {
public:
  MbVector3D vect1; ///< \ru Первый вектор. \en The first vector. 
  MbVector3D vect2; ///< \ru Второй вектор. \en The second vector. 
public:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор.\n
             \en Constructor.\n \~
    \param[in] v1      - \ru Первый вектор.
                         \en The first vector. \~
    \param[in] v2      - \ru Второй вектор.
                         \en The second vector. \~
    \param[in] objType - \ru Тип объекта с различиями.
                         \en Type of an object with differences. \~  
  */
  VectorDifference( const MbVector3D & v1, const MbVector3D & v2, MbeTopologyType objType );

VISITING_CLASS( VectorDifference )
};


//------------------------------------------------------------------------------
/** \brief \ru Результат сравнения двух объектов.
           \en The result of comparison between two objects. \~
  \details \ru Результат сравнения двух объектов.\n
           \en The result of comparison between two objects.\n \~
  \ingroup Algorithms_3D
*/
// ---
class MATH_CLASS CompareItemsResult {
protected:
  bool                        areItemsEqual; ///< \ru Признак отсутствия различий в моделях и в именованиях. \en Attribute of absence of differences in models and names.                                                
  PArray<PrimitiveDifference> differences;   ///< \ru Различия в именовании примитивов и ненайденные примитивы. \en Differences in primitives names and unfound primitives. 
public:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор результата сравнения одинаковых моделей.\n
             \en Constructor of result of comparison between equal models.\n \~        
  */
  CompareItemsResult();
  /// \ru Деструктор. \en Destructor. 
  virtual ~CompareItemsResult();
public:

  void    Reset();                           ///< \ru Сбросить различия. \en Reset differences. 
  void    Add( PrimitiveDifference & diff ); ///< \ru Добавить различие. \en Add a difference. 

  void    SetItemsEqual( bool set );         ///< \ru Установить флаг отсутствия различий. \en Set the flag when differences are absence. 

  /** \brief \ru Тела одинаковые?
             \en Are solids equal? \~
    \details \ru Тела одинаковые?\n
             \en Are solids equal?\n \~        
    \return \ru true, если нет ни различий в именовании, ни геометрических отличий.
            \en true if there are no naming differences and there are no geometric differences. \~
  */
  bool    AreItemsEqual() const;            

  size_t  NamesDifferencesCount() const;     ///< \ru Число различий в именовании. \en The number of naming differences. 
  bool    HaveGeometricDifferences() const;  ///< \ru Есть геометрические различия? \en Is there any geometric difference? 

  const PArray<PrimitiveDifference> & GetPrimitiveDifferences() const; ///< \ru Дать результаты сравнения. \en Get comparison results. 
private:
  CompareItemsResult( const CompareItemsResult & );
};


///////////////////////////////////////////////////////////////////////////
// 
// \ru Функции \en Functions 
// 
///////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
/** \brief \ru Способы "перемешивания".
           \en Ways of "mixing". \~
  \details \ru Способы "перемешивания" составляющих оболочки.
           \en Ways of shell components "mixing". \~
  \ingroup Algorithms_3D
*/
// ---
enum SolidMixUpMode {
  /// \ru Изменение порядка следования граней в массиве. \en A change of faces order in array. 
  smm_FacesReorder     = 1,  

  /// \ru Изменение порядок следования циклов на грянях (толко внутренних). \en A change of loops order in faces (internal only). 
  smm_LoopsReorder     = 2,   

  /// \ru Изменение начального ребра в циклах граней. \en A change of first edge in loops of faces.  
  smm_LoopsBegReset    = 4,                                                 //-V112
   
  /// \ru Изменение направления ребер в циклах граней. \en A change of edges directions in loops of faces. 
  smm_EdgesRedirection = 8,  

  /// \ru Разбивка ребер вставкой вершины. \en Splitting of edges by vertex insertion. 
  smm_EdgesSection     = 16,  
};


//------------------------------------------------------------------------------
/** \brief \ru Стрессовый тест тела.
           \en A stress test for solid. \~
  \details \ru Стрессовый тест тела, перемешивание составляющих оболочки.\n
           \en A stress test for solid, shell components mixing.\n \~
  \param[in] solid      - \ru Тестируемое тело.
                          \en The tested solid. \~
  \param[in] mixUpModes - \ru Флаги из SolidMixUpMode.
                          \en Flags from SolidMixUpMode. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC ( void ) SolidMixUp( MbSolid & solid, uint mixUpModes );


//------------------------------------------------------------------------------
/** \brief \ru Сравнение двух тел.
           \en Two solids comparison. \~
  \details \ru Сравнение двух тел.\n
           \en Two solids comparison.\n \~
  \param[in]  solid1             - \ru Первое тело.
                                   \en The first solid. \~
  \param[in]  solid2             - \ru Второе тело.
                                   \en The second solid. \~
  \param[in]  compareMassInertia - \ru Проверять сначала МЦХ тел.
                                   \en Check mass-inertial properties at first. \~
  \param[in]  checkSense         - \ru Проверять совпадение ориентаций рёбер и граней.
                                   \en Check orientations coincidence of edges and faces. \~
  \param[out] compareResult      - \ru Результат сравнения двух тел.
                                   \en The result of comparison between two solids. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (void) CompareSolids( const MbSolid & solid1,
                                const MbSolid & solid2, 
                                CompareItemsResult & compareResult,
                                bool compareMassInertia,
                                bool checkSense );


//------------------------------------------------------------------------------
/** \brief \ru Сравнение двух тел по именам.
           \en Comparison of two solids by name. \~
  \details \ru Сравнение двух тел по именам.\n
           \en Comparison of two solids by name.\n \~
  \param[in]  before        - \ru Тело до перестроения.
                              \en The solid before construction. \~
  \param[in]  after         - \ru Тело после перестроения.
                              \en The solid after construction. \~
  \param[out] compareResult - \ru Результат сравнения двух тел.
                              \en The result of comparison between two solids. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (bool) CompareSolidsByNames( const MbSolid & before, 
                                       const MbSolid & after, 
                                       CompareItemsResult & compareResult );


#endif // __ATS_CHECK_H
