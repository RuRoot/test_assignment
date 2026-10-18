////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Отладочное средство для сбора информации о времени выполнения алгоритмов.
         \en A debug tool for collection of the information about the algorithm running time. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TOOL_TIME_TEST_H
#define __TOOL_TIME_TEST_H


#define __TIMETEST__

#include <templ_pointer.h>
#include <tool_cstring.h>
#include <templ_p_array.h>
#include <templ_s_array.h>
#include <io_define.h>
#include <math_define.h>


//------------------------------------------------------------------------------
//
// ---
class MATH_CLASS TimeTestStruct  
{
public:
  c3d::string_t name;        // \ru имя замера \en a name of measurement 
  c3d::string_t attitude;    // \ru отношение действия \en a relation of the action 
  uint64        timeResult;  // \ru время замера в тактах процессора \en a time of the measurement in processor cycles 

public:
  TimeTestStruct( const c3d::string_t & _name, const c3d::string_t & _attitude, uint64 _timeResult );
  TimeTestStruct( const TimeTestStruct & );
  ~TimeTestStruct();
public:
  void    Init( const c3d::string_t & _name, const c3d::string_t & _attitude, uint64 _timeResult );
  void    Init( const TimeTestStruct & other );
  double  GetMiliseconds(); // \ru перевести timeResult в миллисекунды и выдать \en convert timeResult to milliseconds and return.  
private:
  const TimeTestStruct & operator = ( const TimeTestStruct & other ) { Init( other ); return *this; }
};


//------------------------------------------------------------------------------
//
// ---
struct MATH_CLASS TimeTestNode : public TimeTestStruct 
{
public:
  size_t  selfId;   // \ru собственный Id \en own Id 
  size_t  parentId; // \ru Id родителя \en Id of the parent 
  
public:
  explicit TimeTestNode( const c3d::string_t & _name, const c3d::string_t & _attitude, uint64 _timeResult );
  TimeTestNode( const TimeTestNode & );
  ~TimeTestNode();
public:
  void Init( const c3d::string_t & _name, const c3d::string_t & _attitude, uint64 _timeResult, size_t _selfId, size_t _parentId );
  void Init( const TimeTestNode & other );
public:
  const TimeTestNode & operator = ( const TimeTestNode & other ) { Init( other ); return *this; }
};


//------------------------------------------------------------------------------
//
// ---
struct MATH_CLASS TimeTestResult : public TimeTestStruct {
public:
  size_t  count;
  size_t  averageTime;

public:
  explicit TimeTestResult( const c3d::string_t & _name, const c3d::string_t & _attitude, uint64 _timeResult )
    : TimeTestStruct( _name, _attitude, _timeResult )
    , count      ( 1 )
    , averageTime( 0 )
  {
  }
  TimeTestResult( const TimeTestResult & other )
    : TimeTestStruct( other             )
    , count         ( other.count       )
    , averageTime   ( other.averageTime )
  {
  }
  ~TimeTestResult()
  {
  }
public:
  void Init( const c3d::string_t & _name, const c3d::string_t & _attitude, uint64 _timeResult )
  {
    TimeTestStruct::Init( _name, _attitude, _timeResult );
    count      = 1;
    averageTime= 0;
  }
  void Init( const TimeTestResult & other )
  {
    TimeTestStruct::Init( other );
    count = other.count;
    averageTime = other.averageTime;
  }
public:
  const TimeTestResult & operator = ( const TimeTestResult & other ) { Init( other ); return *this; }
};


//------------------------------------------------------------------------------
//
// ---
class TimeTestAsCalc : public TimeTestNode {
public:
  uint64 beginTime;  // \ru время начала замера в тактах процессора \en a time of the measurement start in processor cycles 

public:
  explicit TimeTestAsCalc( const c3d::string_t & _name, const c3d::string_t & _attitude, uint64 _beginTime )
    : TimeTestNode( _name, _attitude, 0 )
    , beginTime( _beginTime )
  {
  }
  TimeTestAsCalc( const TimeTestAsCalc & other ) 
    : TimeTestNode( other )
    , beginTime( other.beginTime )
  {
  }
  ~TimeTestAsCalc()
  {
  }
public:
  const TimeTestAsCalc & operator = ( const TimeTestAsCalc & other ) { TimeTestNode::Init( other ); beginTime = other.beginTime; return *this; }
};


//------------------------------------------------------------------------------
//
// ---
class MATH_CLASS TimeTest {
public:
  std::vector<TimeTestNode>   results;
  std::vector<TimeTestAsCalc> ttAsCalcs;
  size_t                      countEnd;
  bool                        searchErr; // \ru поиск ошибки \en an error search 

public:
  TimeTest();
  ~TimeTest();

public:
          TimeTest *                  GetTimeTest    ();
          std::vector<TimeTestNode> & GetListResult  () { return results; }
          void                        BeginTime      ( const TCHAR * name, const TCHAR * attitude ); // \ru начало замера \en start of the measurement 
          void                        EndTime        ( const TCHAR * name ); // \ru окончание замера \en end of the measurement 
          void                        ClearTime      ();

private:
  TimeTest( const TimeTest & );
  const TimeTest & operator = ( const TimeTest & );
};


//------------------------------------------------------------------------------
// \ru сброс \en reset 
// ---
MATH_FUNC(void) SetTimeTest( bool allow );


//------------------------------------------------------------------------------
// \ru проверить измерения \en check measurements 
// ---
MATH_FUNC(bool) CheckTimeTest();


//------------------------------------------------------------------------------
// \ru начало замера \en start of the measurement 
// ---
MATH_FUNC(void) BeginTime( const TCHAR * name, const TCHAR * attitude );


//------------------------------------------------------------------------------
// \ru окончание замера \en end of the measurement 
// ---
MATH_FUNC(void) EndTime( const TCHAR * name );


//------------------------------------------------------------------------------
// \ru Выдать результат в LOG_PATH + fileName \en Return the result in LOG_PATH + fileName 
// ---
MATH_FUNC(void) TimeTestReport( const TCHAR * fileName ); 


//------------------------------------------------------------------------------
// \ru выдать все результаты \en return all results 
// ---
MATH_FUNC(TimeTest *) GetTimeTestResults ();
MATH_FUNC(void) SortResultMeasuring( TimeTest &, std::vector<TimeTestResult> & );


////////////////////////////////////////////////////////////////////////////////
//
// \ru Как этим пользоваться \en The usage 
// \ru 1. вписать в начало функции BEGIN_TIME(name,group) (БЕЗ ; !!!) \en 1. inscribe to the begin of the function BEGIN_TIME(name,group) (WITHOUT ";" at the end !!!) 
// \ru где name   - имя_класса::имя_функции, \en where 'name' is name_of_class::name_of_function, 
// \ru group  - группа к которой относится эта функция \en 'group' - a group which the function belongs to 
//
// \ru 2. вписать в конец функции END_TIME(name) (БЕЗ ; !!!) \en 2. inscribe to the end of the function END_TIME(name) (WITHOUT ";" at the end !!!) 
// \ru так-же можно окружить любое место кода, но name должно быть уникальное ВО ВСЕЙ ЗАДАЧЕ!!! \en in the same way any part of the code may be bounded by these calls. Note that 'name' should be unique in THE ENTIRE SOLUTION!!! 
// \ru количество BEGIN_TIME должно быть строго равно количеству END_TIME осторожно \en the number of BEGIN_TIME calls should be equal to the number of END_TIME calls, be careful 
//
// \ru KYA K12 первому вызову замера верить нельзя, т.к. в BeginTime тратится время на выделение памяти! \en KYA K12 the result of first call of measurement is not significant, because in BeginTime function a lot of time consumes for the memory allocation! 
//
////////////////////////////////////////////////////////////////////////////////


#if defined(C3D_DEBUG)

  #define TB_OTHERS                   _T("Разные операции:")

  #define TB_MATH_BASE                _T("MATH_BASE:")
  #define TB_MATH_BUILDINGS           _T("MATH_BUILDINGS:")
  #define TB_MATH_COMPUTATIONS        _T("MATH_COMPUTATIONS:")
  #define TB_MATH_CONSTRAINTS         _T("MATH_CONSTRAINTS:")
  #define TB_MATH_CONVERTERS          _T("MATH_CONVERTERS:")
  #define TB_TEST                     _T("Test.exe:")

  #define SET_TIME_TEST(allow)        ::SetTimeTest( allow );
  #define CHECK_TIME_TEST()           ::CheckTimeTest();

  #define BEGIN_TIME1(name,attitude)  ::BeginTime( name, attitude  );  // \ru копирование оболочек, методы пересечения \en copying of shells, intersection methods 
  #define END_TIME1(name)             ::EndTime( name );

#else // C3D_DEBUG

  #define TB_OTHERS

  #define TB_MATH_BASE
  #define TB_MATH_BUILDINGS
  #define TB_MATH_COMPUTATIONS
  #define TB_MATH_CONSTRAINTS
  #define TB_MATH_CONVERTERS
  #define TB_TEST

  #define SET_TIME_TEST(allow)
  #define CHECK_TIME_TEST()
  
  #define BEGIN_TIME1(name,attitude)
  #define END_TIME1(name)

#endif // C3D_DEBUG > 0


#define SET_TIME_TEST_REL(allow)        ::SetTimeTest( allow );
#define BEGIN_TIME_REL(name, attitude)  ::BeginTime( name, attitude  );
#define END_TIME_REL(name)              ::EndTime( name );
#define TIME_TEST_REPORT_REL(filename)  ::TimeTestReport( filename );


#endif // __TOOL_TIME_TEST_H
