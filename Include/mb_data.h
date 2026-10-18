////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Данные.
         \en Data. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MB_DATA_H
#define __MB_DATA_H


#include <mb_enum.h>
#include <mb_smooth_nurbs_fit_curve.h>
#include <io_base.h>
#include <mb_cart_point3d.h>
#include <mb_cart_point.h>
#include <templ_s_array.h>


#define C3D_SIGNAL_ENABLED // \ru Активация использования сигналов. \en Enabling use of signals

class  IProgressIndicator;


//------------------------------------------------------------------------------
/** \brief \ru Данные для вычисления шага.
           \en Data for step calculation. \~
  \details \ru Данные для вычисления шага при триангуляции поверхностей и граней. \n
           \en Data for step calculation during face triangulation. \n \~
  \ingroup Data_Structures
*/
// ---
class MATH_CLASS MbStepData {

private:
  /** \brief \ru Способ вычисления приращения параметра при движении по объекту. 
             \en The method of calculation of parameter increment by the object. \~
      \details \ru Способ вычисления приращения параметра при движении по кривой или поверхности.
        Для визуализации геометрической формы используется способ ist_SpaceStep. \n
        Для операций построения используется способ ist_DeviationStep. \n
        Способ ist_MetricStep используется при необходимости ограничения максимальной длины ребер получаемых треугольников,\n
        например для подготовки для последующей печати на 3D принтере. Способ может использоваться совместно с первыми двумя. \n
        Для привязки объектов к параметрам поверхности следует добавить способ ist_ParamStep,
        Для определения столкновений элементов модели используется способ ist_CollisionStep,
        Для вычисления инерционных характеристик используется способ ist_MipStep. 
               \en Methods of calculation of parameter increment by the object. \n \~
        Step by sag ist_SpaceStep is used for visualizations.
        Step by deviation angle ist_DeviationStep is used for calculation.
        Step by length ist_MetricStep is used if it is necessary to limit the maximum length of a side of a triangle,\n
        for example for a 3D printer. (plus by sag and by deviation angle). \n
        Special step ist_ParamStep is added for binding with surface parameters.
        Special step ist_CollisionStep is used for collision detection of model elements.
        Special step ist_MipStep is used for calculation of inertial characteristics. \~
  */
  uint8  stepType;
  double sag;      ///< \ru Максимально допустимый прогиб кривой или поверхности в соседних точках на расстоянии шага. \en The maximum permissible sag of the curve or surface at adjacent points away step. \~
  double angle;    ///< \ru Максимально допустимое угловое отклонение касательных кривой или нормалей поверхности в соседних точках на расстоянии шага. \en The maximum angular deviation of the curve or surface normal in the neighboring points on the distance of a step. \~
  double length;   ///< \ru Максимально допустимое расстояние между соседними точками на расстоянии шага. \en The maximum distance between points a step away. \~
  size_t maxCount; ///< \ru Максимальное количество ячеек в строке и ряду триангуляционной сетки (если 0, то не задано). \en Maximum count of cell in row and column for triangulation grid (if 0, then unlimited). \~
public:
  mutable IProgressIndicator * progress;  ///< \ru Индикатор прогресса выполнения операции. \en A progress indicator of the operation.

public:

  /// \ru Конструктор с заданным типом шага. \en Constructor by step type. 
  MbStepData( MbeStepType t, double s, IProgressIndicator * prog = nullptr );
  /// \ru Пустой конструктор. \en Empty constructor. 
  MbStepData()
    : stepType  ( ist_SpaceStep    )
    , sag       ( Math::visualSag  )
    , angle     ( Math::deviateSag )
    , length    ( MAXIMON          )
    , maxCount  ( 0 )
    , progress ( nullptr )
  {}
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbStepData( const MbStepData & other )
    : stepType  ( other.stepType )
    , sag       ( other.sag      )
    , angle     ( other.angle    )
    , length    ( other.length   )
    , maxCount  ( other.maxCount )
    , progress ( other.progress )
  {}
  /// \ru Деструктор. \en Destructor. 
  ~MbStepData() {}

public:
  /// \ru Установить способ вычисления шага. \en Set the method of calculation of parameter increment by the object. \~
  void    SetStepType( MbeStepType t, bool add = true ) { if ( add ) { stepType |= t; } else { stepType = (uint8)t; } }
  /// \ru Установить максимально допустимый прогиб на расстоянии шага. \en Set the maximum permissible sag at adjacent points away step. \~
  void    SetSag     (  double s ) { sag = s; }
  /// \ru Установить максимально допустимое угловое отклонение в соседних точках. \en Set the maximum angular deviation in the neighboring points on the distance of a step. \~
  void    SetAngle   (  double a ) { angle = a; }
  /// \ru Установить максимально допустимое расстояние между соседними точками на расстоянии шага. \en Set the maximum distance between points a step away. \~
  void    SetLength  ( double l ) { length = l; }
  /// \ru Установить максимально допустимое количество ячеек в строке или ряду триангуляционной сетки. \en Set the maximum count of cell in row and column for triangulation grid. \~
  void    SetMaxCount( size_t c ) { maxCount = c; }
  
  /// \ru Дать максимально допустимый прогиб на расстоянии шага. \en Get the maximum permissible sag at adjacent points away step. \~
  double  GetSag     () const { return sag; }
  /// \ru Дать максимально допустимое угловое отклонение в соседних точках. \en Get the maximum angular deviation in the neighboring points on the distance of a step. \~
  double  GetAngle   () const { return angle; }
  /// \ru Дать максимально допустимое расстояние между соседними точками на расстоянии шага. \en Get the maximum distance between points a step away. \~
  double  GetLength  () const { return length; }
  /// \ru Дать максимально допустимое количество ячеек в строке или ряду триангуляционной сетки. \en Get the maximum count of cell in row and column for triangulation grid. \~
  size_t  GetMaxCount() const { return maxCount; }

  /// \ru Указанный шаг задан. \en This step is set.
  bool    StepIs( MbeStepType sType ) const { return !!(stepType & sType); }

  /// \ru Задан шаг по максимальному прогибу. \en Step by maximum deflection defined. \~
  bool    SagIncluded() const { return //!!(stepType & ist_ParamStep) || 
                                       !!(stepType & ist_SpaceStep) || 
                                       !!(stepType & ist_CollisionStep); }
  /// \ru Задан шаг по угловому отклонению. \en Step by angular deviation defined. \~
  bool    AngleIncluded() const { return !!(stepType & ist_DeviationStep) || 
                                         !!(stepType & ist_MipStep); }
  /// \ru Задан шаг по максимальному расстоянию. \en Step by maximum distance defined. \~
  bool    LengthIncluded() const { return !!(stepType & ist_MetricStep); }

  /// \ru Установить данные для вычисления шага при триангуляции. \en Set data for step calculation during triangulation. 
  void    Init( MbeStepType t, double s, double a, double l, size_t c = 0 )
  { 
    stepType  = (uint8)t;
    sag       = s;
    angle     = a;
    length    = l;
    maxCount  = c;
}

  /// \ru Установить данные для вычисления шага при триангуляции. \en Set data for step calculation during triangulation. 
  void    InitStepBySag( double s )
  { 
    stepType  = (uint8)ist_SpaceStep;
    sag       = ::fabs(s);
    angle     = Math::deviateSag;
    length    = MAXIMON;
    maxCount  = 0;
  }

  /// \ru Функция копирования данных. \en Copy function of data. 
  void    Init( const MbStepData & other )
  { 
    stepType = other.stepType;
    sag      = other.sag;
    angle    = other.angle;
    length   = other.length;
    maxCount = other.maxCount;
  }

  /// \ru Оператор присваивания. \en Assignment operator. 
  MbStepData & operator = ( const MbStepData & other )
  {
    stepType = other.stepType;
    sag      = other.sag;
    angle    = other.angle;
    length   = other.length;
    maxCount = other.maxCount;
    return *this;
  }

  /// \ru Сбросить данные для вычисления шага. \en Reset data for step calculation. 
  void    Reset()
  {
    stepType = (uint8)ist_SpaceStep;
    sag = Math::visualSag;
    angle = Math::deviateSag;
    length = MAXIMON;
    maxCount = 0;
  }

  /// \ru Функция сравнения. \en Equal function. 
  bool    IsEqual( const MbStepData & other, double epsilon ) const;
  /// \ru Вырожденный ли объект? \en Is empty? 
  bool    IsEmpty( double epsilon ) const;

  KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( MbStepData, MATH_FUNC_EX ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class. 
}; // MbStepData


//------------------------------------------------------------------------------
/** \brief \ru Данные для построения полигонального объекта.
           \en Way for polygonal object constructing. \~
  \details \ru Дополнительные данные для построения полигонального объекта и триангуляции поверхностей и граней. \n
           \en Way for polygonal object constructing or face triangulation. \n \~
  \ingroup Data_Structures
*/
// ---
struct MATH_CLASS MbFormNote {

private:
  bool exact; ///< \ru Выполнить построение полигональных объектов на числах double (true) на числах float (false). \en Polygonal objects will created on double data (true) on float data (false).
  bool wire;  ///< \ru Строить изолинии поверхностей. \en Construct isolines of surfaces. \~
  bool grid;  ///< \ru Строить триангуляцию поверхностей. \en Construct triangulations of surfaces. \~
  bool seam;  ///< \ru Дублировать точки триангуляции на швах замкнутых поверхностей. \en Duplicate triangulation points at the seams of closed surfaces. \~
  bool quad;  ///< \ru Строить четырёхугольники (true) при триангуляции поверхностей (по возможности). \en Build quadrangles (true) in triangulations of surfaces (if possible). \~
  bool fair;  ///< \ru Удалить вырожденные треугольники (true). \en Degenerate triangles removing (if surface has pole). \~
  bool mere;  ///< \ru Использовать специальный алгоритм для плоской поверхности (true). \en Use a special algorithm for a flat surface. \~

public:

  /// \ru Пустой конструктор. \en Empty constructor. 
  MbFormNote()
    : exact( false )
    , wire( false )
    , grid( true )
    , seam( true )
    , quad( false )
    , fair( true )
    , mere( true )
  {}
  /// \ru Конструктор с заданным типом шага. \en Constructor by step type. 
  MbFormNote( bool w, bool g, bool s = true, bool e = false, bool q = false, bool f = true, bool m = true )
    : exact( e )
    , wire( w )
    , grid( g )
    , seam( s )
    , quad( q )
    , fair( f )
    , mere( m )
  {}
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbFormNote( const MbFormNote & other )
    : exact( other.exact )
    , wire( other.wire )
    , grid( other.grid )
    , seam( other.seam )
    , quad( other.quad )
    , fair( other.fair )
    , mere( other.mere )
  {}
  /// \ru Деструктор. \en Destructor. 
  ~MbFormNote() {}

public:
  /// \ru Выполнить построение полигональных объектов на числах double (true) на числах float (false). \en Polygonal objects will created on double data (true) on float data (false).
  void    SetExact( bool e ) { exact = e; }
  /// \ru Установить флаг построения изолиний поверхностей. \en Set flag construction isolines of surfaces. \~
  void    SetWire( bool w ) { wire = w; }
  /// \ru Установить флаг строить триангуляцию поверхностей. \en Set flag constructing triangulations of surfaces. \~
  void    SetGrid( bool g ) { grid = g; }
  /// \ru Установить флаг дублирования точек триангуляции на швах замкнутых поверхностей. \en Set the flag for duplicating triangulation points at the seams of closed surfaces. \~
  void    SetSeam( bool s ) { seam = s; }
  /// \ru Установить флаг строить четырёхугольники при триангуляции поверхностей (по возможности).. \en Set flag for build quadrangles in triangulations of surfaces (if possible). \~
  void    SetQuad( bool q ) { quad = q; }
  /// \ru Установить флаг удаления вырожденных треугольников. \en Set flag degenerate triangles removing. \~ 
  void    SetFair( bool f ) { fair = f; }
  /// \ru Установить флаг специального алгоритма для плоской поверхности. \en Set flag to use a special algorithm for a flat surface. \~ 
  void    SetMere( bool m ) { mere = m; }

  /// \ru Выполнить построение полигональных объектов на числах double (true) на числах float (false). \en Polygonal objects will created on double data (true) on float data (false).
  bool    DoExact() const { return exact; }
  /// \ru Дать флаг построения изолиний поверхностей. \en Whether to construct isolines of surfaces? \~
  bool    Wire() const { return wire;}
  /// \ru Строить триангуляцию поверхностей? \en Whether to construct triangulations of surfaces? \~
  bool    Grid() const { return grid; }
  /// \ru Дублировать точки триангуляции на швах замкнутых поверхностей? \en Whether to duplicate triangulation points at the seams of closed surfaces? \~
  bool    Seam() const { return seam; }
  /// \ru Строить четырёхугольники при триангуляции поверхностей (по возможности).? \en Whether to build quadrangles in triangulations of surfaces (if possible)? \~
  bool    Quad() const { return quad; }
  /// \ru Удалить вырожденные треугольники? \en Get flag for degenerate triangles removing. \~
  bool    Fair() const { return fair; }
  /// \ru Использовать специальный алгоритм для плоской поверхности? \en Use a special algorithm for a flat surface? \~
  bool    Mere() const { return mere; }

  /// \ru Установить Данные для вычисления шага при триангуляции. \en Set data for step calculation during triangulation. 
  void    Init( bool w, bool g, bool s, bool e = false, bool q = false, bool f = true, bool m = true ) {
    exact = e;
    wire = w;
    grid = g;
    seam = s;
    quad = q;
    fair = f;
    mere = m;
  }

  /// \ru Функция копирования данных. \en Copy function of data. 
  void    Init( const MbFormNote & other ) { 
    exact = other.exact;
    wire = other.wire;
    grid = other.grid;
    seam = other.seam;
    quad = other.quad;
    fair = other.fair;
    mere = other.mere;
  }

  /// \ru Оператор присваивания. \en Assignment operator. 
  MbFormNote & operator = ( const MbFormNote & other ) {
    exact = other.exact;
    wire = other.wire;
    grid = other.grid;
    seam = other.seam;
    quad = other.quad;
    fair = other.fair;
    mere = other.mere;
    return *this;
  }

  /// \ru Функция сравнения. \en Equal function. 
  bool    IsEqual( const MbFormNote & other ) const;

}; // MbFormNote


//------------------------------------------------------------------------------
/** \brief \ru Данные для управления двумерными объектами.
           \en The data for two-dimensional object control. \~
  \details \ru Данные содержат контрольные точки двумерных объектов. \n
           \en The data consist of two-dimensional control points for object. \n \~
\ingroup Data_Structures
*/
// ---
struct MATH_CLASS MbControlData {

private:
  SArray<MbCartPoint> total;      ///< \ru Точки, перемещаемые вместе. \en Points conveyed along. \~
  SArray<MbCartPoint> share;      ///< \ru Точки, перемещаемые по отдельности. \en Points transported separately. \~ 
  mutable size_t      totalIndex; ///< \ru Индекс текущей точки total. \en The index of current point total. \~
  mutable size_t      shareIndex; ///< \ru Индекс текущей точки share. \en The index of current point share. \~

public:
  /// \ru Пустой конструктор. \en Empty constructor. 
  MbControlData() : total( 0, 1 ), share(0, 1), totalIndex( 0 ), shareIndex(0) {}
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbControlData( const MbControlData & other )
    : total( other.total )
    , share( other.share )
    , totalIndex( other.totalIndex )
    , shareIndex( other.shareIndex )
  {}
  /// \ru Деструктор. \en Destructor. 
  ~MbControlData() {}

public:
  /// \ru Зарезервировать память. \en Size reserve. \~
  void    ReserveTotal( size_t c ) { total.Reserve( c ); }
  /// \ru Зарезервировать память. \en Size reserve. \~
  void    ReserveShare( size_t c ) { share.Reserve( c ); }

  /// \ru Добавить точку. \en Add a point conveyed along. \~
  void    AddTotal( const MbCartPoint & p ) { total.push_back(p); }
  /// \ru Добавить точки. \en Add points. \~
  template <class PointsVector>
  void    AddTotals( const PointsVector & points )
  {
    size_t addCnt = points.size();
    if ( addCnt > 0 ) {
      total.reserve( total.size() + addCnt );
      for ( size_t k = 0; k < addCnt; ++k )
        total.push_back( points[k] );
    }
  }
  /// \ru Добавить точку. \en Add a point. \~
  void    AddShare( const MbCartPoint & p ) { share.push_back(p); }
  /// \ru Добавить точки. \en Add points. \~
  template <class PointsVector>
  void    AddShares( const PointsVector & points )
  {
    size_t addCnt = points.size();
    if ( addCnt > 0 ) {
      share.reserve( share.size() + addCnt );
      for ( size_t k = 0; k < addCnt; ++k )
        share.push_back( points[k] );
    }
  }

  /// \ru Выдать количество точек. \en Get points count conveyed along. \~
  size_t  TotalCount() const { return total.Count(); }
  /// \ru Выдать количество точек. \en Get points count. \~
  size_t  ShareCount() const { return share.Count(); }

  /// \ru Обнулить индексы. \en Reset index.
  void    ResetIndex() const { totalIndex = 0; shareIndex = 0; }

  /// \ru Выдать очередную точку. \en Get current point for totalIndex++.
  bool    GetTotal( MbCartPoint & p ) const;
  /// \ru Выдать очередную точку. \en Get current point for shareIndex++.
  bool    GetShare( MbCartPoint & p ) const;
  /// \ru Выдать точку по индексу. \en Get point by index conveyed along.
  bool    GetTotal( size_t i, MbCartPoint & p ) const;
  /// \ru Выдать точку по индексу. \en Get point by index.
  bool    GetShare( size_t i, MbCartPoint & p ) const;
  /// \ru Выдать общее точек. \en Get all points count. \~
  size_t  Count() const { return total.Count() + share.Count(); }
  /// \ru Выдать точку по индексу. \en Get point by index conveyed along.
  bool    GetPoint( size_t i, MbCartPoint & p ) const;
  /// \ru Установить точку по индексу. \en Set point by index conveyed along.
  bool    SetPoint( size_t i, MbCartPoint & p );
  /// \ru Выдать все точки. \en Get points.
  SArray<MbCartPoint> & SetTotalPoints() { return total; }
  /// \ru Выдать все точки. \en Get points.
  SArray<MbCartPoint> & SetSharePoints() { return share; }
  /// \ru Освободить память. \en Free memory.
  void    HardFlush() { total.HardFlush(); share.HardFlush(); totalIndex = 0; shareIndex = 0; }

  /// \ru Преобразовать согласно матрице. \en Transform according to the matrix. 
  void    Transform( const MbMatrix & matrix );
  /// \ru Сдвинуть вдоль вектора. \en Translate along a vector. 
  void    Move( const MbVector & to );
  /// \ru Повернуть вокруг точки. \en Rotate around a point. 
  void    Rotate( const MbCartPoint & point, double angle );

  /// \ru Дать точку по индексу. \en Set point by index.
  MbCartPoint   & operator []( size_t i ) const;
  /// \ru Оператор присваивания. \en Assignment operator. 
  MbControlData & operator = ( const MbControlData & other )
  {
    total = other.total;
    share = other.share;
    totalIndex = other.totalIndex;
    shareIndex = other.shareIndex;
    return *this;
  }
  /// \ru Вырожденный ли объект? \en Is empty? 
  bool            IsEmpty() const { return ( total.Count() == 0 && share.Count() == 0 ); }
}; // MbControlData


//------------------------------------------------------------------------------
/** \brief \ru Данные для управления трехмерными объектами.
           \en The data for three-dimensional object control. \~
  \details \ru Данные содержат контрольные точки трехмерных объектов. \n
           \en The data consist of three-dimensional control points for object. \n \~
  \ingroup Data_Structures
*/
// ---
struct MATH_CLASS MbControlData3D {

private:
  SArray<MbCartPoint3D> total; ///< \ru Точки, перемещаемые вместе. \en Points conveyed along. \~
  SArray<MbCartPoint3D> share; ///< \ru Точки, перемещаемые по отдельности. \en Points transported separately. \~ 
  mutable size_t totalIndex;   ///< \ru Индекс текущей точки total. \en The index of current point total. \~
  mutable size_t shareIndex;   ///< \ru Индекс текущей точки share. \en The index of current point share. \~

public:
  /// \ru Пустой конструктор. \en Empty constructor. 
  MbControlData3D() : total( 0, 1 ), share(0, 1), totalIndex( 0 ), shareIndex(0) {}
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbControlData3D( const MbControlData3D & other )
    : total( other.total )
    , share( other.share )
    , totalIndex( other.totalIndex )
    , shareIndex( other.shareIndex )
  {}
  /// \ru Деструктор. \en Destructor. 
  ~MbControlData3D() {}

public:
  /// \ru Зарезервировать память. \en Size reserve. \~
  void    ReserveTotal( size_t c ) { total.Reserve( c ); }
  /// \ru Зарезервировать память. \en Size reserve. \~
  void    ReserveShare( size_t c ) { share.Reserve( c ); }

  /// \ru Добавить точку. \en Add a point conveyed along. \~
  void    AddTotal( const MbCartPoint3D & p ) { total.Add(p); }
  /// \ru Добавить точки. \en Add points. \~
  template <class PointsVector>
  void    AddTotals( const PointsVector & points )
  {
    size_t addCnt = points.size();
    if ( addCnt > 0 ) {
      total.reserve( total.size() + addCnt );
      for ( size_t k = 0; k < addCnt; ++k )
        total.push_back( points[k] );
    }
  }
  /// \ru Добавить точку. \en Add a point. \~
  void    AddShare( const MbCartPoint3D & p ) { share.Add(p); }
  /// \ru Добавить точки. \en Add points. \~
  template <class PointsVector>
  void    AddShares( const PointsVector & points )
  {
    size_t addCnt = points.size();
    if ( addCnt > 0 ) {
      share.reserve( share.size() + addCnt );
      for ( size_t k = 0; k < addCnt; ++k )
        share.push_back( points[k] );
    }
  }
  /// \ru Выдать количество точек. \en Get points count conveyed along. \~
  size_t  TotalCount() const { return total.Count(); }
  /// \ru Выдать количество точек. \en Get points count. \~
  size_t  ShareCount() const { return share.Count(); }

  /// \ru Обнулить индексы. \en Reset index.
  void    ResetIndex() const { totalIndex = 0; shareIndex = 0; }
  /// \ru Выдать очередную точку. \en Get current point for totalIndex++.
  bool    GetTotal( MbCartPoint3D & p ) const;
  /// \ru Выдать очередную точку. \en Get current point for shareIndex++.
  bool    GetShare( MbCartPoint3D & p ) const;
  /// \ru Выдать точку по индексу. \en Get point by index conveyed along.
  bool    GetTotal( size_t i, MbCartPoint3D & p ) const;
  /// \ru Выдать точку по индексу. \en Get point by index.
  bool    GetShare( size_t i, MbCartPoint3D & p ) const;
  /// \ru Выдать общее точек. \en Get all points count. \~
  size_t  Count() const { return total.Count() + share.Count(); }
  /// \ru Выдать точку по индексу. \en Get point by index conveyed along.
  bool    GetPoint( size_t i, MbCartPoint3D & p ) const;
  /// \ru Установить точку по индексу. \en Set point by index conveyed along.
  bool    SetPoint( size_t i, MbCartPoint3D & p );
  /// \ru Выдать все точки. \en Get points.
  SArray<MbCartPoint3D> & SetTotalPoints() { return total; }
  /// \ru Выдать все точки. \en Get points.
  SArray<MbCartPoint3D> & SetSharePoints() { return share; }
  /// \ru Освободить память. \en Free memory.
  void    HardFlush() { total.HardFlush(); share.HardFlush(); totalIndex = 0; shareIndex = 0; }
  ///< \ru Очистить данные без освобождения памяти. \en Clear the data without freeing up memory. 
  void    Flush() { total.Flush(); share.Flush(); totalIndex = 0; shareIndex = 0; }

  /// \ru Преобразовать согласно матрице. \en Transform according to the matrix. 
  void    Transform( const MbMatrix3D & matrix );
  /// \ru Сдвинуть вдоль вектора. \en Translate along a vector. 
  void    Move( const MbVector3D & to );
  /// \ru Повернуть вокруг оси. \en Rotate around an axis. 
  void    Rotate( const MbAxis3D & axis, double angle );

  /// \ru Дать точку по индексу. \en Set point by index.
  MbCartPoint3D & operator []( size_t i ) const;
  /// \ru Оператор присваивания. \en Assignment operator. 
  MbControlData3D & operator = ( const MbControlData3D & other )
  {
    total = other.total;
    share = other.share;
    totalIndex = other.totalIndex;
    shareIndex = other.shareIndex;
    return *this;
  }
  /// \ru Вырожденный ли объект? \en Is empty? 
  bool    IsEmpty() const { return ( total.Count() == 0 && share.Count() == 0 ); }
}; // MbControlData3D


//#define C3D_DEBUG_FAIR_CURVES ///< \ru Для отладочной печати данных гладких кривых. \en For debugging printing of smooth curve data.

//------------------------------------------------------------------------------
/** \brief \ru Параметры построения гладких кривых на базе трехмерной ломаной.
           \en The parameters of the construction of smooth curves based on a three-dimensional polyline. \~
  \details \ru Данные содержат параметры построения сплайнов с плавным изменением кривизны. \n
           \en The data contains parameters for constructing splines with smooth curvature changes. \n \~
  \ingroup Data_Structures
*/
// ---
//DEPRECATE_DECLARE
struct MATH_CLASS MbFairCurveData {
public:
  typedef std::vector<std::pair<size_t, MbVector3D> >  IndexVectorArray;

private:
  IndexVectorArray    fixData; /// \ru Массив данных для фиксации точек и касательных. \en Data array for fixing points and tangents.

public:
  MbeFairSmoothing    fairing;              ///< \ru Сглаживание (без сглаживания, со сглаживанием, со сглаживанием и исправлением острых углов, со сглаживанием зашумленных точек). \en Smoothing of curve: 0 - disable, 1 - enable, 2 - enable and correct acute angles \~
  bool                arrange;              ///< \ru Перераспределение точек по контуру (false - без перераспределения, true - с перераспределением). \en Redistribution of points (false - without of distribution, true - with  distribution) . \~
  MbeFairSubdivision  subdivision;          ///< \ru Коэффициент уплотнения кривой. \en Curve subdivision coefficient . \~
  MbeFairCurvature    accountCurvature;     ///< \ru Учет кривизны в концевых точках. \en Accounting for curvature at end points. \~
  MbeFairVector       accountInflexVector;  ///< \ru Учет вектора в точке перегиба при построении NURBzS кривой (0 - направление звена S-полигона, 1 - направление касательной). \en How to take into account the vector at the inflection point at creating NURBzS (0 - direction of segment of S-polygon, 1 - direction of tangent to curve). \~
  bool                tangentCorrectBspline;///< \ru Сохранение вектора в точке перегиба при построении B-кривой (0 - направление звена S-полигона, 1 - направление касательной). \en Save the vector at the inflection point at creating B-curve (0 - direction of segment of S-polygon, 1 - direction of tangent to curve). \~

  MbeFixPntTng        fixPntTng;            ///< \ru Фиксировать точки на касательных / касательные в точках.       \en Fix the points on tangents / the tangents on points.
  MbeFairApprox       approx;               ///< \ru Метод аппроксимации. \en Approx method. \~
  MbeFairMethod       create;               ///< \ru Исходные ГО: 1 - опорная ломаная, 2 - касательная ломаная. \en Initial GD: 1 - base polyline, 2 - tangent polyline. \~
  size_t              degreeBSpline;        ///< \ru Степень B-сплайновой кривой m (3<=m<=10). \en The degree m (3<=m<=10) of B-Spline curve. \~ 
  MbeFairSplineFormat outFormat;            ///< \ru Выходной формат сплайна (2 - S-полигон, 3 - GB-полигон). \en Output format of spline (2 - S-polygon, 3 - GB-polygon). \~
  size_t              nSegments;            ///< \ru Количество сегментов сплайна. \en Number of segments of spline. \~
  size_t              numSegment;           ///< \ru Номер сегмента. \en Number of segment. \~
  double              tParam;               ///< \ru Внутренний параметр точки сегмента сплайна. \en Point internal param on segment of spline. \~
  double              clothoidRMin;         ///< \ru Радиус кривизны на конце начального участка клотоиды. \en Curvature radius on end of initial part of Clothoid. \~
  double              clothoidLMax;         ///< \ru Максимальная длина начального участка клотоиды. \en Max length of initial part of Clothoid. \~
  size_t              clothoidSegms;        ///< \ru Количество сегментов аппроксимирующей клотоиду кривой. \en Number of segments of curve approximated the Clothoid. \~
  
  size_t              numberOfIterationsBSpl; ///< \ru Количество итераций построения B-сплайна (заданное и фактическое). \en The number of iterations for building the B-spline (given and actual).
  double              realAccuracyBSpl;       ///< \ru Точность построения B-сплайна (заданная и фактическая). \en The accuracy of creating the B-spline (given and actual). 

  /// \ru Параметры аппроксимации V-кривой. \en Params of Approximation of V-curve.
  bool                switchEndTangents;    ///< \ru Флаги учета значений концевых касательных. \en Flags accounting tangents values.
  bool                switchEndCurvature;   ///< \ru Флаги учета значений кривизны. \en Flags accounting curvature values.
  MbVector3D          firstTangent;         ///< \ru Касательная в начальной точке. \en Tangent in the first point.
  MbVector3D          lastTangent;          ///< \ru Касательная в конечной точке. \en Tangent in the last point.
  double              firstCurvature;       ///< \ru Значение кривизны в начальной точке. \en Curvature in the first point.
  double              lastCurvature;        ///< \ru Значение кривизны в конечной точке. \en Curvature in the last point.
  bool                smoothTorsion;        ///< \ru Флаг сглаживания кручения пространственной кривой (false - незамкнут, true - замкнут). \en Sign of smoothing of torsion of curve (false - not smoothed, true - smoothed).
  double              clearanceNoisy;       ///< \ru Коридор отклонения сглаживающей кривой от зашумленных точек;  \en Corridor of smoothing curve deviation from noisy points;
  size_t              clearanceNoisyIteration;    ///< \ru Количество итераций сглаживания зашумленных   точек.  \en Number of iterations of smoothing noisy points.
  double              scaleParam;                 ///< \ru Параметр масштабирования. \en Scaling parameter.    
  bool                switchFixFirstPointAtNoisy; ///< \ru Флаг фиксации сплайна в начальной точке. \en Flag of fixing a spline at the first point.
  bool                switchFixLastPointAtNoisy;  ///< \ru Флаг фиксации сплайна в конечной точке. \en Flag of fixing a spline at the last point.

  const MbNurbs3D *   referenceCurve; ///< \ru Кривая для сравнения с результатом аппроксимации. \en Curve for result comparing.

  /// \ru Выходные параметры. \en Output parameters.
  MbeFairWarning      warning;  ///< \ru Предупреждение о работе. \en The operation warning. \~
  MbResultType        error;    ///< \ru Ошибка о работе. \en The operation error. \~
  MbApproxError       errInfo;  ///< \ru Сведения об ошибке аппроксимации. \en Approximation error information.

#ifdef C3D_DEBUG_FAIR_CURVES
  /*DEBUG*/  FILE *prt;
#endif

public:
  /// \ru Пустой конструктор. \en Empty constructor. 
  MbFairCurveData() :
    fairing( fairSmooth_Yes ), arrange( false ), subdivision( fairSubdiv_Single ),
    accountCurvature( fairCur_No ), accountInflexVector( fairVector_Tangent ),
    tangentCorrectBspline( true ),
    fixPntTng( fixPntTng_NotFix ),
    //approx( fairApprox_KnotsSpline ), create( 1 ), degreeBSpline( 8 ),
    approx( fairApprox_IsoSpline ), create( fairMethod_Polyline ), degreeBSpline( 8 ), //DEBUG 2020 25
    outFormat( fairFormat_Close ),
    nSegments( 4 ), numSegment( 0 ), tParam( 0.1 ),
    clothoidRMin( 50.0 ), clothoidLMax( 200.0 ), clothoidSegms( 10 ), numberOfIterationsBSpl( 1000 ),
#ifdef C3D_DEBUG_FAIR_CURVES
    prt( nullptr ),
#endif
    realAccuracyBSpl( METRIC_ACCURACY*0.1 ),
    switchEndTangents( false ), switchEndCurvature( false ),
    firstCurvature( 0.0 ), lastCurvature( 0.0 ), smoothTorsion( false ),
    clearanceNoisy( 0.002 ), clearanceNoisyIteration( 10 ), scaleParam( 1.0 ),
    switchFixFirstPointAtNoisy( false ), switchFixLastPointAtNoisy( false ),
    referenceCurve( nullptr ),
    warning( fwarn_Success ), error( rt_Success ), errInfo() {}

  ~MbFairCurveData();

  // \ru Инициализировать данные для фиксации точек и касательных. \en Initialize данные для фиксации точек и касательных.
  void InitFixConstraints( const std::vector<std::pair<size_t, MbVector3D *>> & constraints );

  // \ru Инициализировать данные для фиксации точек и касательных. \en Initialize данные для фиксации точек и касательных.
  void InitFixConstraints( const IndexVectorArray & constraints );

  //< \ru Получить данные для фиксации точек и касательных. \en Get данные для фиксации точек и касательных.
  const IndexVectorArray & GetFixConstraints() const { return fixData; }

  ///< \ru Получить кривую для сравнения. \en Get curve for result comparing.
  const MbNurbs3D * GetReferenceCurve() const { return referenceCurve; }

  ///< \ru Установить кривую для сравнения. \en Set curve for result comparing.
  void SetReferenceCurve( const MbNurbs3D * pCurve ) { referenceCurve = pCurve; }

  /// \ru Оператор присваивания. \en Assignment operator. 
  MbFairCurveData & operator = ( const MbFairCurveData & other );

  // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool IsEqual( const MbFairCurveData &, double accuracy ) const;

  KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( MbFairCurveData, MATH_FUNC_EX ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class. 

}; // MbFairCurveData


//------------------------------------------------------------------------------
/** \brief \ru Определение метода построения гладких кривых.
           \en Definition of a method of smooth curves creation. \~
  \details \ru Структура содержит информацию о типе входных / выходных данных и методе построения или изменения гладких кривых. \n
           \en The structure contains information about types of input / output data and a method of constructing or modifying a fair curve. \n \~
  \ingroup Data_Structures
*/
// ---
struct MATH_CLASS MbFairCurveMethod
{
  MbeFairCurveType  _curveType;   ///<  \ru Тип создаваемой плавной кривой. \en Type of a fair curve to be created.
  MbeFairDataType   _dataType;    ///<  \ru Тип входных данных (метод построения). \en Type of input data (method of creating).
  MbeFairChangeType _changeType;  ///<  \ru Метод изменения плавной кривой. \en Method of modifying a fair curve.
public:
  /// \ru Конструктор по умолчанию. \en Default constructor. 
  MbFairCurveMethod() :
    _curveType( fairCrvType_None ), _dataType( fairData_None ), _changeType( fairChange_None ) {}

  /// \ru Конструктор создания плавной кривой. \en Constructor for creating a fair curve. 
  MbFairCurveMethod( const MbeFairCurveType & ct, const MbeFairDataType & dt = fairData_None ) :
    _curveType( ct ), _dataType( dt ), _changeType( fairChange_None ) {}

  /// \ru Конструктор изменения плавной кривой. \en Constructor for modifying a fair curve. 
  MbFairCurveMethod( const MbeFairChangeType & ct ) :
    _curveType( fairCrvType_None ), _dataType( fairData_None ), _changeType( ct ) {}

  /// \ru Конструктор копирования. \en Copy constructor. 
  MbFairCurveMethod( const MbFairCurveMethod & m ) :
    _curveType( m._curveType ), _dataType( m._dataType ), _changeType( m._changeType ) {}

  /// \ru Оператор присваивания. \en Assignment operator. 
  MbFairCurveMethod & operator = ( const MbFairCurveMethod & other ) {
    _curveType  = other._curveType;
    _dataType   = other._dataType;
    _changeType = other._changeType;
    return *this;
  }

  /// \ru Инициализация создания плавной кривой. \en Initializing a creation of a fair curve.
  void Init( const MbeFairCurveType & ct, const MbeFairDataType & dt = fairData_None ) {
    _curveType = ct; _dataType = dt; _changeType = fairChange_None; 
  }

  /// \ru Инициализация изменения плавной кривой. \en Initializing a modification of a fair curve. 
  void Init( const MbeFairChangeType & ct ) {
    _curveType = fairCrvType_None; _dataType = fairData_None; _changeType = ct;
  }

  /// \ru Получить тип создаваемой плавной кривой. \en Get the type of a fair curve to be created.
  const MbeFairCurveType & CurveType() const { return _curveType; }

  /// \ru Получить тип входных данных (метод построения плавной кривой). \en Get the type of input data( method of creating a fair curve ).
  const MbeFairDataType & DataType() const { return _dataType; }

  /// \ru Получить метод изменения плавной кривой. \en Get the method of modifying a fair curve.
  const MbeFairChangeType & ChangeType() const { return _changeType; }

  // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool IsEqual( const MbFairCurveMethod & ) const;

  KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( MbFairCurveMethod, MATH_FUNC_EX ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class. 

}; // MbFairCurveMethod


//------------------------------------------------------------------------------
/** \brief \ru Параметры для проверки, является ли объект плоским.
           \en Parameters for checking if the object is planar. \~
  \details \ru Параметры для проверки, является ли объект плоским.
           \en Parameters for checking if the object is planar. \~
*/
// ---
struct MATH_CLASS PlanarCheckParams {
  double  accuracy;
  VERSION version;

  /// \ru Конструктор по умолчанию. \en Default constructor. 
  PlanarCheckParams()
    : accuracy( METRIC_EPSILON )
    , version( Math::DefaultMathVersion() )
  {}

  /// \ru Конструктор. \en Constructor. 
  explicit PlanarCheckParams( double accuracy_ )
    : accuracy( accuracy_ )
    , version( Math::DefaultMathVersion() )
  {}

  /// \ru Конструктор. \en Constructor. 
  PlanarCheckParams( double accuracy_, VERSION version_ )
    : accuracy( accuracy_ )
    , version( version_ )
  {}

  /// \ru Конструктор копирования. \en Copy-constructor. 
  PlanarCheckParams( const PlanarCheckParams & other )
    : accuracy( other.accuracy )
    , version( other.version )
  {}
};


//------------------------------------------------------------------------------
/** \brief \ru Отступ от ребра пересечения.
           \en Offset from the edge of the intersection. \~
  \details \ru Отступ от ребра пересечения на грани.
           \en Offset from the edge of the intersection on face. \~
*/
// ---
class MATH_CLASS MbTraverse {

  /**
    \ru Сдвиг от пробной точки касательно поверхности по нормали от ребра пересечения на грани.
    \en The offset from the sample point touching the surface from the intersection edge on the face. \~
  */
  MbVector3D offset;
  /**
    \ru Нормализованный вектор нормали справа или слева от ребра пересечения на гранях. 
    \en The normalized normal vector to the right or left of the intersection edge on the faces. \~
  */
  MbVector3D normal;

public:
  /// \ru Конструктор по умолчанию. \en Default constructor. 
  MbTraverse()
    : offset()
    , normal()
  {}

  /// \ru Конструктор. \en Constructor. 
  MbTraverse( const MbVector3D & offset_, const MbVector3D & normal_ )
    : offset( offset_ )
    , normal( normal_ )
  {}

  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbTraverse( const MbTraverse & other )
    : offset( other.offset )
    , normal( other.normal )
  {}

  ~MbTraverse() {}

        /// \ru \en
        void         Init( const MbVector3D & offset_, const MbVector3D & normal_ );

  const MbVector3D & GetOffset()       const { return offset;        }  ///< \ru Выдать вектор сдвига. \en Get offset vector.
  const MbVector3D & GetNormal()       const { return normal;        }  ///< \ru Выдать вектор нормали. \en Get normal vector.

        MbVector3D & SetOffset()             { return offset;        }  ///< \ru Выдать вектор сдвига \en Get offset vector.
        MbVector3D & SetNormal()             { return normal;        }  ///< \ru Выдать вектор нормали \en Get normal vector.

        /// \ru Поменять направление нормали. \en Change direction of normal. \~
        void         InvertNormal() { normal.Invert(); }

        /// \        ru Обнулить координаты векторов. \en Set coordinates of vectors to zero.
        void         SetZero() { offset.SetZero(); normal.SetZero(); }

        /// \ru Присвоить отступу значения другого отступа. \en Set the offset to a different offset value.
        MbTraverse & operator = ( const MbTraverse & other )
        {
          offset = other.offset;
          normal = other.normal;
          return *this;
        }
};


//------------------------------------------------------------------------------
/** \brief \ru Отступы от ребра пересечения.
           \en Offsets from the edge of the intersection. \~
  \details \ru Отступы влево и вправо от ребра пересечения.
           \en Offsets to the left and right from the edge. \~
*/
// ---
class MATH_CLASS MbTwoTraverses {
  MbTraverse left;  ///< \ru Отступ влево. \en Left offset. \~
  MbTraverse right; ///< \ru Отступ вправо. \en Right offset. \~

public:
  /// \ru Конструктор по умолчанию. \en Default constructor. 
  MbTwoTraverses()
    : left()
    , right()
  {}

  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbTwoTraverses( const MbTwoTraverses & other )
    : left( other.left )
    , right( other.right )
  {}

  ~MbTwoTraverses() {}

  const MbTraverse & GetLeft()  const { return left;  }  ///< \ru Выдать отступ слева.  \en Get left offset.  \~
  const MbTraverse & GetRight() const { return right; }  ///< \ru Выдать отступ справа. \en Get right offset. \~
        MbTraverse & SetLeft()        { return left;  }  ///< \ru Выдать отступ слева.  \en Get left offset.  \~
        MbTraverse & SetRight()       { return right; }  ///< \ru Выдать отступ справа. \en Get right offset. \~

  /// \ru Поменять местами лево и право. \en Swap left and right. \~
  void Swap();

  /// \ru Поменять направление нормалей. \en Change direction of normals. \~
  void InvertNormals() { left.InvertNormal(); right.InvertNormal(); }

  /// \ru Обнулить координаты векторов. \en Set coordinates of vectors to zero.
  void SetZero() { left.SetZero(); right.SetZero(); }
};


//------------------------------------------------------------------------------
/** \brief \ru Тип изменений в объекте.
           \en Type of changes in object. \~
  \details \ru Тип изменений в объекте. Используется при передаче сигнала об изменениях.
           \en Type of changes in object. Used when signaling about changes. \~
  \ingroup Data_Structures
*/
// ---
enum MbeObjectChangeType
{
  oct_Move,
  oct_Rotate,
  oct_Inverse,
  oct_Transform,
  oct_CacheReset,
  oct_All
};


//------------------------------------------------------------------------------
/** \brief \ru Класс для уведомления объектов об изменениях определенного объекта.
           \en Class for notifying objects about changes of a certain object. \~
  \details \ru Класс для передачи сигналов об изменениях объекта в другие объекты.
               Объект, который хочет нотифицировать о своих изменениях, должен иметь экземпляр
               класса MbObjectChangeSignal и вызывать его метод Signal() при своей модификации.
               Объекты, которые хотят получать сигналы об изменениях определенного объекта, должны
               иметь метод Refresh( MbeObjectChangeType ) и подписаться на сигналы этого объекта.
           \en Class for sending signals about changes of an object to other objects.
               An object that wants to notify about its changes must have an instance of
               MbObjectChangeSignal class and call its Signal() method when gets modified.
               Objects that want to receive signals about changes of a certain object must
               have the Refresh( MbeObjectChangeType) method and subscribe to signals of this object. \~
  \ingroup Data_Structures
*/
// ---
template <class Object>
class MbObjectChangeSignal
{
  typedef std::set< Object * > ObjectSet;
  ObjectSet   _subscribers; // \ru Подписчики на сигнал. \en Subscribers to a signal.
  CommonMutex _lock;        // \ru Внутренняя блокировка. \en Internal lock.
public:

  /// \ru Подписаться на сигнал. \en Subscribe to a signal.
  void Subscribe( Object * c ) {
    ScopedLock l( &_lock );
    _subscribers.insert( c );
  }
  /// \ru Отписаться от сигнала. \en Unsubscribe from a signal.
  void Unsubscribe( Object * c ) {
    if ( _subscribers.size() ) {
      ScopedLock l( &_lock );
      typename ObjectSet::iterator i = _subscribers.find( c );
      if ( i != _subscribers.end() )
        _subscribers.erase( i );
    }
  }
  /// \ru Послать сигнал об изменениях подписчикам. \en Send a signal about changes to the subscribers.
  void Signal( MbeObjectChangeType type = oct_All ) {
    if ( _subscribers.size() ) {
      ScopedLock l( &_lock );
      typename ObjectSet::iterator i = _subscribers.begin();
      while ( i != _subscribers.end() ) {
        ( *i )->Refresh( type );
        i++;
      }
    }
  }
};


//------------------------------------------------------------------------------
/** \brief \ru Класс для уведомления одного объекта об изменениях определенного объекта.
           \en Class for notifying a single object about changes of a certain object. \~
  \details \ru Класс для передачи сигналов об изменениях объекта в другой объект.
               Объект, который хочет нотифицировать о своих изменениях, должен иметь экземпляр
               класса MbObjectChangeSignal и вызывать его метод Signal() при своей модификации.
               Объект, который будет получать сигналы об изменениях определенного объекта, должен
               иметь метод Refresh( MbeObjectChangeType ) и подписаться на сигналы этого объекта.
               Сигнал работает без блокировки.
           \en Class for sending signals about changes of an object to another object.
               An object which wants to notify about its changes must have an instance of
               MbObjectChangeSignal class and call its Signal() method when gets modified.
               An object which will receive signals about changes of a certain object must
               have the Refresh( MbeObjectChangeType) method and subscribe to signals of this object.
               Signal works without locking. \~
  \ingroup Data_Structures
*/
// ---
template <class Object>
class MbObjectChangeSignalOne
{
  Object * _subscriber; // \ru Подписчик на сигнал. \en Subscriber to a signal.
public:

  MbObjectChangeSignalOne() : _subscriber( nullptr ) {}

  /// \ru Подписаться на сигнал. \en Subscribe to a signal.
  void Subscribe( Object * c ) {
    _subscriber = c;
  }
  /// \ru Отписаться от сигнала. \en Unsubscribe from a signal.
  void Unsubscribe( Object * ) {
    _subscriber = nullptr;
  }
  /// \ru Послать сигнал об изменениях подписчикам. \en Send a signal about changes to the subscribers.
  void Signal( MbeObjectChangeType type = oct_All ) {
    if ( _subscriber != nullptr )
      _subscriber->Refresh( type );
  }
};


#endif // __MB_DATA_H
