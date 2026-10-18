////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Точка, вектор и ось, основанные на числе одинарной точности (float).
         \en Point, vector and axis based on single precision floating point number (float). \~
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MESH_FLOAT_POINT3D_H
#define __MESH_FLOAT_POINT3D_H

#include <mb_axis3d.h>


constexpr float MB_MAXFLOAT = (float)MAXIMON; // \ru Максимальное значение. \en Maximum value.


class  MbFloatVector3D;


//------------------------------------------------------------------------------
/** \brief \ru Трехмерная точка.
           \en Three-dimensional point. \~
  \details \ru Класс MbFloatPoint3D служит для представления точки трехмерного 
  пространства, также как аналогичный класс #MbCartPoint3D, который отличается
  более высокой точностью представления. MbFloatPoint3D имеет структуру данных, 
  состоящей из трех чисел с плавающей точкой одинарной точности (float). Применяется 
  для полигонального представления трехмерных геометрических объектов в таких структурах 
  данных, как полигон (#MbPolygon3D) или триангуляция (#MbGrid).\n
           \en MbFloatPoint3D class is used for a three-dimensional point representation 
  as well as a similar #MbCartPoint3D class which differs
  by higher precision of representation. MbFloatPoint3D has data structure 
  consisting of a triple of a single precision floating point numbers (float). Used 
  for polygonal representation of a three-dimensional geometric objects in such data structures 
  as polygon (#MbPolygon3D) or triangulation (#MbGrid).\n \~
  \ingroup Polygonal_Objects
*/
// ---
class MATH_CLASS MbFloatPoint3D {
public:
  float x; ///< \ru Первая координата точки. \en First coordinate of point. 
  float y; ///< \ru Вторая координата точки. \en Second coordinate of point. 
  float z; ///< \ru Третья координата точки. \en Third coordinate of point. 

public :  
  /**
    \brief \ru Конструктор по умолчанию
           \en Default constructor \~ 
    \details \ru Конструирует точку с координатами (0.0, 0.0, 0.0).
             \en Constructs a point with coordinates (0.0, 0.0, 0.0). \~
  */
  MbFloatPoint3D() : x( 0.0 ), y( 0.0 ), z( 0.0 )  {}
  /// \ru Конструктор по евклидовой точке. \en Constructor by Euclidean point. 
  explicit MbFloatPoint3D( const MbCartPoint3D & );
  /// \ru Конструктор по радиус-вектору. \en Constructor by a radius-vector. 
  explicit MbFloatPoint3D( const MbVector3D & );
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbFloatPoint3D( const MbFloatPoint3D & p ) : x( p.x ), y( p.y ), z( p.z ) {}
  /// \ru Конструктор по радиус-вектору. \en Constructor by a radius-vector. 
  explicit MbFloatPoint3D( const MbFloatVector3D & );
  /// \ru Конструктор. \en Constructor. 
  MbFloatPoint3D( double xx, double yy, double zz ) : x( (float)xx ), y( (float)yy ), z( (float)zz ) {}

public:

  /** \ru \name Функции точки.
      \en \name Functions of point.
  \{ */        
  /// \ru Инициализация по точке. \en Initialize by point. 
  void    Init( const MbCartPoint3D & );
  /// \ru Инициализация по радиус-вектору. \en Initialization by a radius-vector. 
  void    Init( const MbVector3D & );
  /// \ru Инициализация по координатам. \en Initialization by coordinates. 
  void    Init( double xx, double yy, double zz );

  // \ru Общие функции объекта. \en Common functions of object. 

  void    Set( const MbFloatPoint3D & v1, float t1, const MbFloatPoint3D & v2, float t2 );
  /**
    \brief \ru Приравнять координаты сумме координат точки и вектора.
           \en Equate coordinates to sum of point coordinates and vector coordinates. \~
    \details \ru Приравнять координаты сумме координат точки v1 и вектора v2, умноженного на число t2.
             \en Equate coordinates to sum of v1 point coordinates and v2 vector coordinates multiplied by t2. \~
    \param[in] v1 - \ru Исходная точка.
                    \en The initial point. \~
    \param[in] v2 - \ru Исходный вектор.
                    \en The initial vector. \~
    \param[in] t2 - \ru Число, на которое умножаются координаты исходного вектора v2.
                    \en Factor the coordinates of the initial vector v2 are multiplied by. \~
  */
  MbFloatPoint3D & Set( const MbFloatPoint3D & v1, const MbFloatVector3D & v2, float t2 );

  void    Transform( const MbMatrix3D & );             ///< \ru Преобразовать согласно матрице. \en Transform according to the matrix. 
  void    Move     ( const MbVector3D & );             ///< \ru Сдвинуть вдоль вектора \en Translate along a vector. 
  void    Rotate   ( const MbAxis3D &, double angle ); ///< \ru Повернуть вокруг оси на угол. \en Rotate about an axis by an angle. 
  void    TransformAsVector( const MbMatrix3D & );     ///< \ru Преобразовать элемент согласно матрице как вектор (без учета смещения). \en Transform an element as vector according to the matrix (without taking translation into account). 
  void    RotateAsVector   ( const MbAxis3D &, double angle ); ///< \ru Повернуть вокруг оси на угол как вектор (без учета смещения). \en Rotate an element as a vector by an angle about an axis (without taking translation into account). 

  void    GetCartPoint( MbCartPoint3D &p ) const { p.x = x; p.y = y; p.z = z; } ///< \ru Выдать декартову точку \en Get the Cartesian point 
  void    GetVector   ( MbVector3D    &p ) const { p.x = x; p.y = y; p.z = z; } ///< \ru Выдать вектор \en Get the vector 

  void    operator =  ( const MbCartPoint3D  & );        ///< \ru Присвоить значение точки. \en Assign values of point. 
  void    operator =  ( const MbVector3D     & );        ///< \ru Присвоить значение вектора. \en Assign values of vector. 
  void    operator =  ( const MbFloatPoint3D & );        ///< \ru Присвоить значение точки. \en Assign values of point. 
  bool    operator == ( const MbFloatPoint3D & ) const;  ///< \ru Проверка на равенство. \en Check for equality. 

  float   DistanceToPoint ( const MbFloatPoint3D & ) const; ///< \ru Вычислить расстояние до точки. \en Calculate distance to point. 
  float   DistanceToPoint2( const MbFloatPoint3D & ) const; ///< \ru Вычислить квадрат расстояния до точки. \en Calculate squared distance to point. 

  void    operator += ( const MbFloatPoint3D & ); ///< \ru Добавить координаты точки. \en Add coordinates of point. 
  void    operator -= ( const MbFloatPoint3D & ); ///< \ru Вычесть координаты точки. \en Subtract coordinates of point. 
  void    operator += ( const MbFloatVector3D & ); ///< \ru Добавить координаты вектора. \en Add coordinates of vector. 
  void    operator -= ( const MbFloatVector3D & ); ///< \ru Вычесть координаты вектора. \en Subtract coordinates of vector. 

  MbFloatPoint3D  operator + ( const MbFloatVector3D & ) const; ///< \ru Сложение точки и вектора. \en Addition of point and vector. 
  MbFloatPoint3D  operator - ( const MbFloatVector3D & ) const; ///< \ru Вычитание вектора из точки. \en Subtraction of vector from point. 
  MbFloatVector3D operator + ( const MbFloatPoint3D & ) const; ///< \ru Сложение двух точек. \en Addition of two points. 
  MbFloatVector3D operator - ( const MbFloatPoint3D & ) const; ///< \ru Вычитание двух точек. \en Subtraction of two points. 

  /// \ru Выдать координату по её номеру. \en Get coordinate by its index. 
  float & operator[](size_t i)       { C3D_ASSERT( i < 3 ); return (&x)[i]; }
  /// \ru Выдать координату по её номеру. \en Get coordinate by its index. 
  float   operator[](size_t i) const { C3D_ASSERT( i < 3 ); return (&x)[i]; }

  /// \ru Равны ли координаты нулю с указанной точностью. \en Whether coordinates is equal to zero with specified tolerance. 
  bool    IsZero ( double eps = Math::lengthEpsilon ) const { return ::fabs(x) < eps &&
                                                                     ::fabs(y) < eps &&
                                                                     ::fabs(z) < eps; }

  /// \ru Выдать свойства объекта. \en Get properties of the object. 
  void    GetProperties( MbProperties & );
  /// \ru Записать свойства объекта. \en Set properties of the object. 
  void    SetProperties( const MbProperties & );
  /// \ru Являются ли объекты равными? \en Are the objects equal?
  bool    IsSame( const MbFloatPoint3D &, double accuracy ) const;

  /** \} */
  DECLARE_NEW_DELETE_CLASS( MbFloatPoint3D )
  DECLARE_NEW_DELETE_CLASS_EX( MbFloatPoint3D )
  KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE(MbFloatPoint3D, MATH_FUNC_EX);
  KNOWN_OBJECTS_RW_PTR_OPERATORS_EX_BASE(MbFloatPoint3D, MATH_FUNC_EX);
}; // MbFloatPoint3D


//------------------------------------------------------------------------------
/** \brief \ru Трехмерный вектор.
           \en Three-dimensional vector. \~
  \details \ru Трехмерный вектор, как тип данных, похож на #MbVector3D, однако основан 
           на более грубом представлении числа с плавающей точкой. Применяется в 
           структурах данных триангуляции (MbGrid) для аппроксимированного 
           представления объектов. \n
           \en Three-dimensional vector as data type is similar to #MbVector3D, however based 
           on more rough floating point number representation. It is used in 
           such data structures as triangulation (MbGrid) for approximated 
           representation of objects. \n \~
  \ingroup Polygonal_Objects
*/
// ---
class MATH_CLASS MbFloatVector3D {
public:
  float x; ///< \ru Первая компонента вектора. \en First component of vector. 
  float y; ///< \ru Вторая компонента вектора. \en Second component of vector. 
  float z; ///< \ru Третья компонента вектора. \en Third component of vector. 

public:
  /// \ru Конструктор. \en Constructor. 
  MbFloatVector3D();
  /// \ru Конструктор. \en Constructor. 
  explicit MbFloatVector3D( const MbCartPoint3D & );
  /// \ru Конструктор. \en Constructor. 
  explicit MbFloatVector3D( const MbFloatPoint3D & );
  /// \ru Конструктор. \en Constructor.
  explicit MbFloatVector3D( const MbVector3D & );
  /// \ru Конструктор. \en Constructor. 
  MbFloatVector3D( const MbFloatVector3D & );
  /// \ru Конструктор по разнице пары точек: this = p2 - p1. \en Constructor by difference of two points: this = p2 - p1. 
  MbFloatVector3D( const MbFloatPoint3D & p1, const MbFloatPoint3D & p2 ) : x( p2.x - p1.x ), y( p2.y - p1.y ), z( p2.z - p1.z ) {}  
  /// \ru Конструктор по разнице пары точек: this = p2 - p1. \en Constructor by difference of two points: this = p2 - p1. 
  MbFloatVector3D( const MbCartPoint3D & p1, const MbCartPoint3D & p2 ) : x( (float)(p2.x - p1.x) ), y( (float)(p2.y - p1.y) ), z( (float)(p2.z - p1.z) ) {}
  /// \ru Конструктор по координатам. \en Constructor by coordinates. 
  MbFloatVector3D( float xx, float yy, float zz );
  /// \ru Деструктор. \en Destructor. 
  ~MbFloatVector3D() {}

public:

  /** \ru \name Функции вектора.
      \en \name Functions of vector.
  \{ */        
  /// \ru Инициализация по координатам. \en Initialization by coordinates. 
  void    Init( float a, float b, float c ) { x = a; y = b; z = c; }
  /// \ru Инициализация по точке. \en Initialize by point. 
  void    Init( const MbFloatPoint3D & p ) { x = p.x; y = p.y; z = p.z; }
  /// \ru Инициализация по точкам. \en Initialize by points. 
  void    Init( const MbFloatPoint3D & p1, const MbFloatPoint3D & p2 );
  /// \ru Инициализация по вектору. \en Initialize by vector. 
  void    Init( const MbFloatVector3D & v ) { x = v.x; y = v.y; z = v.z; }
  /// \ru Задать векторное произведение двух заданных векторов. \en Set vector product of two given vectors. 
  void    SetVecM( const MbFloatVector3D & vF, const MbFloatVector3D & vS ) {
            x = vF.y * vS.z - vF.z * vS.y;
            y = vF.z * vS.x - vF.x * vS.z;
            z = vF.x * vS.y - vF.y * vS.x;
          }

  /// \ru Инициализация по сумме векторов с коэффициентами. \en Initialize by sum of vectors with coefficients 
  void    Set( const MbFloatVector3D &v1, float t1, const MbFloatVector3D &v2, float t2 );
  /// \ru Инициализация по сумме точек с коэффициентами. \en Initialize by sum of points with coefficients 
  void    Set( const MbFloatPoint3D &v1, float t1, const MbFloatPoint3D &v2, float t2 );

  void    Transform( const MbMatrix3D & );             ///< \ru Преобразовать согласно матрице. \en Transform according to the matrix. 
  void    Rotate   ( const MbAxis3D &, double angle ); ///< \ru Повернуть вокруг оси на угол. \en Rotate about an axis by an angle. 
  bool    Normalize(); ///< \ru Нормализовать вектор. \en Normalize a vector. 
  void    Invert(); ///< \ru Сменить направление вектора на противоположное. \en Change vector direction to opposite. 

  float   Length () const; ///< \ru Длина вектора. \en Length of vector. 
  float   Length2() const; ///< \ru Квадрат длины вектора. \en Squared length of vector. 

  MbFloatVector3D   operator -  () const; ///< \ru Оператор вычитания векторов. \en Operator of subtraction of vectors. 

  MbFloatVector3D   operator + ( const MbFloatVector3D & vector ) const; ///< \ru Сложение двух векторов; результат - вектор. \en Addition of two vectors; result is vector. 
  MbFloatVector3D   operator - ( const MbFloatVector3D & vector ) const; ///< \ru Вычитание двух векторов; результат - вектор. \en Subtraction of two vectors; result is vector. 

  MbFloatVector3D   operator + ( const MbFloatPoint3D & pnt ) const; ///< \ru Сложение вектора и точки. \en Addition of vector and point. 
  MbFloatVector3D   operator - ( const MbFloatPoint3D & pnt ) const; ///< \ru Вычитание из вектора точки. \en Subtraction of point from vector. 

  MbFloatVector3D & operator += ( const MbFloatVector3D & vector ); ///< \ru Добавить вектор. \en Add a vector. 
  MbFloatVector3D & operator -= ( const MbFloatVector3D & vector ); ///< \ru Вычесть вектор. \en Subtract a vector. 

  MbFloatVector3D & operator *= ( float f ); ///< \ru Умножить на коэффициент. \en Multiply by a factor. 
  MbFloatVector3D & operator /= ( float f ); ///< \ru Делить на коэффициент. \en Divide by a factor. 

  void    operator =  ( const MbCartPoint3D  & );        ///< \ru Присвоить значение точки. \en Assign values of point. 
  void    operator =  ( const MbVector3D     & );        ///< \ru Присвоить значение вектора. \en Assign values of vector. 
  void    operator =  ( const MbFloatPoint3D & );        ///< \ru Присвоить значение точки. \en Assign values of point. 
  bool    operator == ( const MbFloatVector3D & ) const; ///< \ru Проверить на равенство. \en

  float             operator * ( const MbFloatVector3D & ) const; ///< \ru Скалярное умножение двух векторов. \en Dot-product of two vectors. 
  MbFloatVector3D   operator | ( const MbFloatVector3D & ) const; ///< \ru Векторное умножение двух векторов. \en Vector-product of two vectors. 

  float & operator[](size_t i)       { C3D_ASSERT( i < 3 ); return (&x)[i]; }
  float   operator[](size_t i) const { C3D_ASSERT( i < 3 ); return (&x)[i]; }

  /// \ru Равен ли вектор нулю с указанной точностью. \en Whether vector is equal to zero with specified tolerance. 
  bool    IsZero ( double eps = Math::lengthEpsilon ) const { return ::fabs(x) < eps &&
                                                                     ::fabs(y) < eps &&
                                                                     ::fabs(z) < eps; }
  /// \ru Выдать вектор (по аналогии с MbFloatPoint3D). \en Get vector (by analogy with MbFloatPoint3D). 
  void    GetVector( MbVector3D & p ) const { p.x = x; p.y = y; p.z = z; }

  float   MaxFactor() const; ///< \ru Выдать максимальную по модулю компонент вектора. \en Get the largest absolute value of a vector.  

  /// \ru Выдать свойства объекта. \en Get properties of the object. 
  void    GetProperties( MbProperties & );
  /// \ru Записать свойства объекта. \en Set properties of the object. 
  void    SetProperties( const MbProperties & );
  /// \ru Являются ли объекты равными? \en Are the objects equal?
  bool    IsSame( const MbFloatVector3D &, double accuracy ) const;

  /** \} */
  DECLARE_NEW_DELETE_CLASS( MbFloatVector3D )
  DECLARE_NEW_DELETE_CLASS_EX( MbFloatVector3D )
  KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE(MbFloatVector3D, MATH_FUNC_EX);
  KNOWN_OBJECTS_RW_PTR_OPERATORS_EX_BASE(MbFloatVector3D, MATH_FUNC_EX);
}; // MbFloatVector3D


//------------------------------------------------------------------------------
/** \brief \ru Ось.
           \en Axis. \~
  \details \ru Ось представляет собой вектор, привязанный к фиксированной точке. \n
           \en Axis represents the vector attached to the fixed point. \n \~
  \ingroup Polygonal_Objects
*/ 
// ---
class MATH_CLASS MbFloatAxis3D 
{
  MbFloatPoint3D  origin; ///< \ru Положение начала. \en Position of the origin. 
  MbFloatVector3D axisZ;  ///< \ru Направление оси (вектор единичной длины). \en Axis direction (unit length vector). 

public :
  /// \ru Пустой конструктор, ось расположена в начале глобальных координат и совпадает с третьей осью глобальных координат. \en Empty constructor. The axis is in the origin of global coordinates and coincides with the third axis of global coordinates. 
  MbFloatAxis3D() : origin( 0, 0, 0 ), axisZ( 0, 0, 1 ) {}
  /// \ru Конструктор по точке и вектору. \en Constructor by a point and a vector. 
  MbFloatAxis3D( const MbFloatPoint3D & initOrigin, const MbFloatVector3D & initAxisZ ) : origin( initOrigin ), axisZ( initAxisZ ) { axisZ.Normalize(); }
  /// \ru Конструктор по точке и вектору. \en Constructor by a point and a vector. 
  explicit MbFloatAxis3D( const MbCartPoint3D & initOrigin, const MbVector3D & initAxisZ ) : origin( initOrigin ), axisZ( initAxisZ ) { axisZ.Normalize(); }
  /// \ru Конструктор по двум точкам. \en Constructor by two points. 
  MbFloatAxis3D( const MbFloatPoint3D & initOrigin, const MbFloatPoint3D & initPoint ) : origin( initOrigin ), axisZ( initOrigin, initPoint ) { axisZ.Normalize(); }
  /// \ru Конструктор по другой оси. \en Constructor by another axis. 
  MbFloatAxis3D( const MbFloatAxis3D & initAxis ) : origin( initAxis.origin ), axisZ( initAxis.axisZ ) {}
  /// \ru Конструктор по другой оси. \en Constructor by another axis. 
  explicit MbFloatAxis3D( const MbAxis3D & initAxis ) : origin( initAxis.GetOrigin() ), axisZ ( initAxis.GetAxisZ() ) {}
  /// \ru Деструктор \en Destructor 
 ~MbFloatAxis3D();

public :

  /// \ru Инициализация по другой оси. \en The initialization by another axis. 
  void    Init( const MbFloatAxis3D & initAxis )
  {
    origin = initAxis.origin;
    axisZ  = initAxis.axisZ;
  }
  /// \ru Инициализация по точке и вектору. \en The initialization by a point and a vector. 
  void    Init( const MbFloatPoint3D & initOrigin, const MbFloatVector3D & initAxisZ )
  {
    origin = initOrigin;
    axisZ = initAxisZ;
    axisZ.Normalize();
  }
  /// \ru Инициализация по точке и вектору. \en The initialization by a point and a vector. 
  void    Init( const MbCartPoint3D & initOrigin, const MbVector3D & initAxisZ )
  {
    origin.Init( MbFloatPoint3D( initOrigin ) );
    axisZ.Init( MbFloatVector3D( initAxisZ ) );
    axisZ.Normalize();
  }
  /// \ru Инициализация по двум точкам. \en The initialization by two points. 
  void    Init( const MbFloatPoint3D & initOrigin, const MbFloatPoint3D & initPoint )
  {
    origin = initOrigin;
    axisZ = initPoint - initOrigin;
    axisZ.Normalize();
  }

  /** \ru \name Функции трехмерного объекта
      \en \name Functions of a three-dimensional object
  \{ */  
  void    Transform( const MbMatrix3D & );             ///< \ru Преобразовать согласно матрице. \en Transform according to the matrix. 
  void    Move     ( const MbVector3D & );             ///< \ru Сдвинуть вдоль вектора \en Translate along a vector. 
  void    Rotate   ( const MbAxis3D &, double angle ); ///< \ru Повернуть вокруг оси на угол. \en Rotate about an axis by an angle. 
  MbFloatAxis3D & Duplicate() const;                   ///< \ru Сделать копию элемента. \en Create a copy of the element. 
  float   DistanceToPoint( const MbFloatPoint3D & ) const; ///< \ru Вычислить расстояние до точки. \en Calculate distance to point. 
  float   DistanceToSegment( const MbFloatPoint3D & p1, const MbFloatPoint3D & p2 ) const; ///< \ru Вычислить расстояние до отрезка. \en Calculate distance to segment.  
  bool    PointProjection( const MbFloatPoint3D & pnt, float & tRes ) const; ///< \ru Вычислить проекцию точки на ось. \en Calculate point projection to the exis.  
  /** \} */

  /** \ru \name Функции доступа к полям.
      \en \name Functions for access to fields.
  \{ */        
  const MbFloatPoint3D &  GetOrigin() const { return origin; } ///< \ru Получить начало оси. \en Get origin of axis. 
  const MbFloatVector3D & GetAxisZ () const { return axisZ;  } ///< \ru Получить вектор оси. \en Get vector of axis. 
        MbFloatPoint3D &  SetOrigin()       { return origin; } ///< \ru Изменить начало оси. \en Change origin of axis. 
        MbFloatVector3D & SetAxisZ ()       { return axisZ;  } ///< \ru Изменить вектор оси. \en Change vector of axis. 

        MbFloatAxis3D & operator = ( const MbFloatAxis3D & init ) { origin = init.origin; axisZ = init.axisZ; return *this; } ///< \ru Присвоение значений. \en Assignment of values. 
  /** \} */
  /// \ru Являются ли объекты равными? \en Are the objects equal?
  bool    IsSame( const MbFloatAxis3D & other, double accuracy ) const;
  /// \ru Дать пространственную точку по параметру на оси. \en Get the space point by a parameter on axis. 
  void    PointOn( const float & t, MbFloatPoint3D & p ) const { p.Set( origin, axisZ, t ); }

  KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( MbFloatAxis3D, MATH_FUNC_EX )
  DECLARE_NEW_DELETE_CLASS( MbFloatAxis3D )
  DECLARE_NEW_DELETE_CLASS_EX( MbFloatAxis3D )
}; // MbFloatAxis3D


////////////////////////////////////////////////////////////////////////////////
//
// \ru Трехмерная точка полигона. \en A three-dimensional point of a polygon. 
//
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
// \ru Конструктор по радиус-вектору. \en Constructor by a radius-vector. 
// ---
inline
MbFloatPoint3D::MbFloatPoint3D( const MbFloatVector3D & p )
  : x( p.x )
  , y( p.y )
  , z( p.z )
{}


//------------------------------------------------------------------------------
// \ru Конструктор. \en Constructor. 
// ---
inline
MbFloatPoint3D::MbFloatPoint3D( const MbCartPoint3D & p ) 
  : x( (float)p.x )
  , y( (float)p.y )
  , z( (float)p.z ) 
{}


//------------------------------------------------------------------------------
// \ru Конструктор \en Constructor 
// ---
inline
MbFloatPoint3D::MbFloatPoint3D( const MbVector3D & p ) 
  : x( (float)p.x )
  , y( (float)p.y )
  , z( (float)p.z )
{}


//------------------------------------------------------------------------------
// \ru Инициализация \en Initialization 
// ---
inline
void MbFloatPoint3D::Init( const MbCartPoint3D & p ) {
  x = (float)p.x; y = (float)p.y; z = (float)p.z;
}


//------------------------------------------------------------------------------
// \ru Инициализация \en Initialization 
// ---
inline
void MbFloatPoint3D::Init( const MbVector3D & p ) {
  x = (float)p.x; y = (float)p.y; z = (float)p.z;
}


//------------------------------------------------------------------------------
// \ru Инициализация \en Initialization 
// ---
inline
void MbFloatPoint3D::Init( double xx, double yy, double zz ) {
  x = (float)xx; y = (float)yy; z = (float)zz;
}


//------------------------------------------------------------------------------
// \ru Присвоение значений \en Values assignment 
// ---
inline
void MbFloatPoint3D::Set( const MbFloatPoint3D & v1, float t1, 
                                 const MbFloatPoint3D & v2, float t2 )
{
  x = v1.x * t1 + v2.x * t2;
  y = v1.y * t1 + v2.y * t2;
  z = v1.z * t1 + v2.z * t2;
}


//------------------------------------------------------------------------------
// \ru Присвоение значений \en Values assignment 
// ---
inline
MbFloatPoint3D & MbFloatPoint3D::Set( const MbFloatPoint3D & v1, const MbFloatVector3D & v2, float t2 )
{
  x = v1.x + v2.x * t2;
  y = v1.y + v2.y * t2;
  z = v1.z + v2.z * t2;
  return *this;
}


//------------------------------------------------------------------------------
// \ru Сдвиг \en Translation 
// ---
inline
void MbFloatPoint3D::Move( const MbVector3D & to )
{
  x += (float)to.x;
  y += (float)to.y;
  z += (float)to.z;
}


//------------------------------------------------------------------------------
// \ru Присвоение точке значений \en Assignment of values to point 
// ---
inline
void MbFloatPoint3D::operator = ( const MbCartPoint3D & v )
{
  x = (float)v.x;
  y = (float)v.y;
  z = (float)v.z;
}


//------------------------------------------------------------------------------
// \ru Присвоение точке значений \en Assignment of values to point 
// ---
inline
void MbFloatPoint3D::operator = ( const MbVector3D & v )
{
  x = (float)v.x;
  y = (float)v.y;
  z = (float)v.z;
}


//------------------------------------------------------------------------------
// \ru Присвоение точке значений вектора \en Assignment of vector values to point 
// ---
inline
void MbFloatPoint3D::operator = ( const MbFloatPoint3D & v )
{
  x = v.x;
  y = v.y;
  z = v.z;
}


//------------------------------------------------------------------------------
// \ru Проверка на равенство \en Check for equality 
// ---
inline
bool MbFloatPoint3D::operator == ( const MbFloatPoint3D & with ) const {
  return IsSame( with, Math::lengthEpsilon );
}


//------------------------------------------------------------------------------
// \ru Квадрат расстояния от точки до точки \en Squared distance from point to point 
// ---
inline
float MbFloatPoint3D::DistanceToPoint2( const MbFloatPoint3D & to ) const
{
  float coordDiff[3] = { ( x - to.x ), ( y - to.y ), ( z - to.z ) };
  coordDiff[0] *= coordDiff[0];
  coordDiff[1] *= coordDiff[1];
  coordDiff[2] *= coordDiff[2];
  return coordDiff[0] + coordDiff[1] + coordDiff[2];
}


//------------------------------------------------------------------------------
// \ru Добавить координаты точки \en Add coordinates of point 
// ---
inline
void MbFloatPoint3D::operator += ( const MbFloatPoint3D & with )
{
  x += with.x;
  y += with.y;
  z += with.z;
}


//------------------------------------------------------------------------------
// \ru Вычесть координаты точки \en Subtract coordinates of point 
// ---
inline
void MbFloatPoint3D::operator -= ( const MbFloatPoint3D & with )
{
  x -= with.x;
  y -= with.y;
  z -= with.z;
}


//------------------------------------------------------------------------------
// \ru Добавить координаты \en Add coordinates 
// ---
inline
void MbFloatPoint3D::operator += ( const MbFloatVector3D & with )
{
  x += with.x;
  y += with.y;
  z += with.z;
}


//------------------------------------------------------------------------------
// \ru Вычесть координаты \en Subtract coordinates 
// ---
inline
void MbFloatPoint3D::operator -= ( const MbFloatVector3D & with )
{
  x -= with.x;
  y -= with.y;
  z -= with.z;
}


//------------------------------------------------------------------------------
// \ru Умножение точки на число \en Multiplication of a point by a factor 
// ---
inline
MbFloatPoint3D operator * ( const MbFloatPoint3D &pnt, float factor ) {
  return MbFloatPoint3D( pnt.x * factor, pnt.y * factor, pnt.z * factor );
}


//------------------------------------------------------------------------------
// \ru Деление точки на число \en Division of a point by a factor 
// ---
inline
MbFloatPoint3D operator / ( const MbFloatPoint3D & pnt, float factor )
{
  // \ru Операция деления занимает 40 циклов процессора, а умножения 7, т.е. (/) 5.7 раза медленней (*) \en Division operation takes 40 CPU cycles and multiplication takes only 7, i.e. division is 5.7 times slower than multiplication 
  C3D_ASSERT( ::fabs(factor) > NULL_EPSILON );
  float invFactor = (float)( 1.0 / factor );
  return MbFloatPoint3D( pnt.x * invFactor, pnt.y * invFactor, pnt.z * invFactor );
}


//------------------------------------------------------------------------------
// \ru Умножение точки на число \en Multiplication of a point by a factor 
// ---
inline
MbFloatPoint3D operator * ( float factor, const MbFloatPoint3D & pnt ) {
  return MbFloatPoint3D( pnt.x * factor, pnt.y * factor, pnt.z * factor );
}


//------------------------------------------------------------------------------
// \ru Сложение точки и вектора \en Sum of a vector and a point 
// ---
inline
MbFloatPoint3D MbFloatPoint3D::operator + ( const MbFloatVector3D & vector ) const {
  return MbFloatPoint3D( x + vector.x, y + vector.y, z + vector.z );
}


//------------------------------------------------------------------------------
// \ru Вычитание вектора из точки \en Subtraction of a vector from a point 
// ---
inline
MbFloatPoint3D MbFloatPoint3D::operator - ( const MbFloatVector3D & vector ) const {
  return MbFloatPoint3D( x - vector.x, y - vector.y, z - vector.z );
}


//------------------------------------------------------------------------------
// \ru Сложение двух точек \en Sum of two points 
// ---
inline
MbFloatVector3D MbFloatPoint3D::operator + ( const MbFloatPoint3D & pnt ) const {
  return MbFloatVector3D( x + pnt.x, y + pnt.y, z + pnt.z );
}


//------------------------------------------------------------------------------
// \ru Вычитание двух точек \en Subtraction of two points 
// ---
inline
MbFloatVector3D MbFloatPoint3D::operator - ( const MbFloatPoint3D & pnt ) const {
  return MbFloatVector3D( x - pnt.x, y - pnt.y, z - pnt.z );
}


//------------------------------------------------------------------------------
/** \brief \ru Являются ли объекты равными?
           \en Are the objects equal? \~
  \details \ru Равными считаются объекты, данные которых равны с заданной точностью.
           \en The objects are equal if their data are equal with a given accuracy. \~
  \param[in] other -    \ru Объект для сравнения.
                        \en The object to compare. \~
  \param[in] accuracy - \ru Точность сравнения.
                        \en The accuracy to compare. \~
  \return \ru Равны ли объекты.
          \en Whether the objects are equal.
*/
inline
bool MbFloatPoint3D::IsSame( const MbFloatPoint3D & other, double accuracy ) const
{
  return ( (::fabs(x - other.x) < accuracy) && 
           (::fabs(y - other.y) < accuracy) && 
           (::fabs(z - other.z) < accuracy) );
}


////////////////////////////////////////////////////////////////////////////////
//
// \ru Трехмерный вектор нормали триангуляции. \en Three-dimensional vector of triangulation normal. 
//
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
// \ru Конструктор \en Constructor 
// ---
inline
MbFloatVector3D::MbFloatVector3D()
  : x( 0.0 )
  , y( 0.0 )
  , z( 0.0 )
{}


//------------------------------------------------------------------------------
// \ru Конструктор \en Constructor 
// ---
inline
MbFloatVector3D::MbFloatVector3D( const MbCartPoint3D & p )
  : x( (float)p.x )
  , y( (float)p.y )
  , z( (float)p.z )
{}


//------------------------------------------------------------------------------
// \ru Конструктор \en Constructor 
// ---
inline
MbFloatVector3D::MbFloatVector3D( const MbFloatPoint3D & p )
  : x( p.x )
  , y( p.y )
  , z( p.z )
{}


//------------------------------------------------------------------------------
// конструктор
// ---
inline
MbFloatVector3D::MbFloatVector3D( const MbVector3D & p )
  : x( (float)p.x )
  , y( (float)p.y )
  , z( (float)p.z )
{}


//------------------------------------------------------------------------------
// \ru Конструктор \en Constructor 
// ---
inline
MbFloatVector3D::MbFloatVector3D( const MbFloatVector3D & p )
  : x( p.x )
  , y( p.y )
  , z( p.z )
{}


//------------------------------------------------------------------------------
// \ru Конструктор \en Constructor 
// ---
inline
MbFloatVector3D::MbFloatVector3D( float xx, float yy, float zz )
  : x( xx )
  , y( yy )
  , z( zz )
{}


//------------------------------------------------------------------------------
// \ru Инициализация по двум точкам \en Initialization by two points 
// ---
inline
void MbFloatVector3D::Init( const MbFloatPoint3D & p1, const MbFloatPoint3D & p2 ) {
  x = p2.x - p1.x;
  y = p2.y - p1.y;
  z = p2.z - p1.z;
}


//------------------------------------------------------------------------------
// \ru Присвоение значений \en Values assignment 
// ---
inline
void MbFloatVector3D::Set( const MbFloatVector3D & v1, float t1, 
                           const MbFloatVector3D & v2, float t2 ) {
  x = v1.x * t1 + v2.x * t2;
  y = v1.y * t1 + v2.y * t2;
  z = v1.z * t1 + v2.z * t2;
}


//------------------------------------------------------------------------------
// \ru Присвоение значений \en Values assignment 
// ---
inline
void MbFloatVector3D::Set( const MbFloatPoint3D & v1, float t1, 
                           const MbFloatPoint3D & v2, float t2 ) {
  x = v1.x * t1 + v2.x * t2;
  y = v1.y * t1 + v2.y * t2;
  z = v1.z * t1 + v2.z * t2;
}


//------------------------------------------------------------------------------
// \ru Сложение двух векторов; результат - вектор \en Addition of two vectors; result is a vector 
// ---
inline
MbFloatVector3D MbFloatVector3D::operator + ( const MbFloatVector3D & vector ) const {
  return MbFloatVector3D( x + vector.x, y + vector.y, z + vector.z );
}


//------------------------------------------------------------------------------
// \ru Вычитание двух векторов; результат - вектор \en Subtraction of two vectors; result is a vector 
// ---
inline
MbFloatVector3D MbFloatVector3D::operator - ( const MbFloatVector3D & v2 ) const {
  return MbFloatVector3D ( x - v2.x, y - v2.y, z - v2.z );
}


//------------------------------------------------------------------------------
// \ru Сложение вектора и точки \en Addition of a vector and a point 
// ---
inline
MbFloatVector3D MbFloatVector3D::operator + ( const MbFloatPoint3D & pnt ) const {
  return MbFloatVector3D( x + pnt.x, y + pnt.y, z + pnt.z );
}


//------------------------------------------------------------------------------
// \ru Вычитание из вектора точки \en Subtraction of a point from a vector 
// ---
inline
MbFloatVector3D MbFloatVector3D::operator - ( const MbFloatPoint3D & pnt ) const {
  return MbFloatVector3D( x - pnt.x, y - pnt.y, z - pnt.z );
}


//------------------------------------------------------------------------------
// \ru Унарный минус \en Unary minus 
// ---
inline
MbFloatVector3D MbFloatVector3D::operator - () const {
  return MbFloatVector3D ( - x, - y, - z );
}


//------------------------------------------------------------------------------
// \ru Увеличение вектора \en Increase vector 
// ---
inline
MbFloatVector3D & MbFloatVector3D::operator += ( const MbFloatVector3D & v ) {
  x += v.x; y += v.y; z += v.z; 
  return *this;
}


//------------------------------------------------------------------------------
// \ru Уменьшение вектора \en Decrease vector 
// ---
inline
MbFloatVector3D & MbFloatVector3D::operator -= ( const MbFloatVector3D & v ) {
  x -= v.x; y -= v.y; z -= v.z; 
  return *this;
}


//------------------------------------------------------------------------------
// \ru Умножение вектора на число \en Multiplication of a vector by a factor 
// ---
inline
MbFloatVector3D & MbFloatVector3D::operator *= ( float f ) {
  x *= f; y *= f; z *= f; 
  return *this;
}


//------------------------------------------------------------------------------
// \ru Деление вектора на число \en Division of a vector by a factor 
// ---
inline
MbFloatVector3D & MbFloatVector3D::operator /= ( float f )
{
  C3D_ASSERT( ::fabs(f) > NULL_EPSILON );
  f = (float)( 1.0 / f );
  x *= f; y *= f; z *= f; 
  return *this;
}


//------------------------------------------------------------------------------
// \ru Скалярное умножение двух векторов \en Dot-product of two vectors 
// ---
inline
float MbFloatVector3D::operator * ( const MbFloatVector3D & vector ) const {
  return ( x * vector.x + y * vector.y + z * vector.z );
}


//------------------------------------------------------------------------------
// \ru Векторное умножение двух векторов \en Vector-product of two vectors 
// ---
inline
MbFloatVector3D MbFloatVector3D::operator | ( const MbFloatVector3D & vect2 ) const {
  return MbFloatVector3D( y * vect2.z - z * vect2.y,
                          z * vect2.x - x * vect2.z,
                          x * vect2.y - y * vect2.x );
}


//------------------------------------------------------------------------------
// \ru Присвоение точке значений \en Assignment of values to point 
// ---
inline
void MbFloatVector3D::operator = ( const MbCartPoint3D & v )
{
  x = (float)v.x;
  y = (float)v.y;
  z = (float)v.z;
}


//------------------------------------------------------------------------------
// \ru Присвоение точке значений \en Assignment of values to point 
// ---
inline
void MbFloatVector3D::operator = ( const MbVector3D & v )
{
  x = (float)v.x;
  y = (float)v.y;
  z = (float)v.z;
}


//------------------------------------------------------------------------------
// \ru Присвоение точке значений вектора \en Assignment of vector values to point 
// ---
inline
void MbFloatVector3D::operator = ( const MbFloatPoint3D & v )
{
  x = v.x;
  y = v.y;
  z = v.z;
}


//------------------------------------------------------------------------------
// Проверить на равенство.
// ---
inline
bool MbFloatVector3D::operator == ( const MbFloatVector3D & with ) const {
  return IsSame( with, Math::region );
}


//------------------------------------------------------------------------------
// \ru Длина вектора \en Length of vector 
// ---
inline
float MbFloatVector3D::Length () const {
  return (float)sqrt( Length2() );
} 


//------------------------------------------------------------------------------
// \ru Квадрат длины вектора \en Squared length of vector 
// ---
inline
float MbFloatVector3D::Length2() const {
  return x * x + y * y + z * z;
} 


//------------------------------------------------------------------------------
// \ru Нормализация вектора \en Normalize a vector 
// ---
inline
bool MbFloatVector3D::Normalize()
{
  double len = Length();
  bool res = ( len >= NULL_EPSILON );
  if ( res && ::fabs( len - 1.0 ) > NULL_EPSILON ) {
    double one_len = 1.0 / len;
    x *= (float)one_len;
    y *= (float)one_len;
    z *= (float)one_len;
  }
  return res;
}


//------------------------------------------------------------------------------
// \ru Умножение вектора на число \en Multiplication of a vector by a factor 
// ---
inline
MbFloatVector3D operator * ( const MbFloatVector3D & vector, float factor ) {
  return MbFloatVector3D( vector.x * factor, vector.y * factor, vector.z * factor );
}


//------------------------------------------------------------------------------
// \ru Деление вектора на число \en Division of a vector by a factor 
// ---
inline
MbFloatVector3D operator / ( const MbFloatVector3D & vector, float factor ) {
  // \ru Операция деления занимает 40 циклов процессора, а умножения 7, т.е. (/) 5.7 раза медленней (*) \en Division operation takes 40 CPU cycles and multiplication takes only 7, i.e. division is 5.7 times slower than multiplication 
  C3D_ASSERT( ::fabs(factor) > NULL_EPSILON );
  float invFactor = (float)( 1.0 / factor );
  return MbFloatVector3D( vector.x * invFactor, vector.y * invFactor, vector.z * invFactor );
}


//------------------------------------------------------------------------------
// \ru Умножение вектора на число \en Multiplication of a vector by a factor 
// ---
inline
MbFloatVector3D operator * ( float factor, const MbFloatVector3D &vector ) {
  return vector * factor;
}


//------------------------------------------------------------------------------
// \ru Сменить направление вектора на противоположное \en Change vector direction to the opposite one 
// ---
inline
void MbFloatVector3D::Invert()
{
  x = - x;
  y = - y;
  z = - z;
}


//-------------------------------------------------------------------------------
// \ru Максимальная по модулю компонента вектора \en The largest absolute value of a vector 
// ---
inline float MbFloatVector3D::MaxFactor() const
{
  float ax = ::fabs( x );
  float ay = ::fabs( y );
  float az = ::fabs( z );
  return ( ((ax > ay) && (ax > az)) ? ax : ((ay > az) ? ay : az) );
}


//------------------------------------------------------------------------------
/** \brief \ru Являются ли объекты равными?
           \en Are the objects equal? \~
  \details \ru Равными считаются объекты, данные которых равны с заданной точностью.
           \en The objects are equal if their data are equal with a given accuracy. \~
  \param[in] other -    \ru Объект для сравнения.
                        \en The object to compare. \~
  \param[in] accuracy - \ru Точность сравнения.
                        \en The accuracy to compare. \~
  \return \ru Равны ли объекты.
          \en Whether the objects are equal.
*/
inline bool MbFloatVector3D::IsSame( const MbFloatVector3D & other, double accuracy ) const
{
  return ( (::fabs(x - other.x) < accuracy) && 
           (::fabs(y - other.y) < accuracy) && 
           (::fabs(z - other.z) < accuracy) );
}


////////////////////////////////////////////////////////////////////////////////
//
// \ru Ось. \en The axis. 
//
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
/** \brief \ru Являются ли объекты равными?
           \en Are the objects equal? \~
  \details \ru Равными считаются объекты, данные которых равны с заданной точностью.
           \en The objects are equal if their data are equal with a given accuracy. \~
  \param[in] other -    \ru Объект для сравнения.
                        \en The object to compare. \~
  \param[in] accuracy - \ru Точность сравнения.
                        \en The accuracy to compare. \~
  \return \ru Равны ли объекты.
          \en Whether the objects are equal.
*/
inline bool MbFloatAxis3D::IsSame( const MbFloatAxis3D & other, double accuracy ) const
{
  return ( origin.IsSame( other.origin, accuracy ) &&
           axisZ.IsSame( other.axisZ, accuracy ) );
}


#endif // __MESH_FLOAT_POINT3D_H
