////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __IGES_STRUCTURES_H
#define __IGES_STRUCTURES_H


#include <system_dependency.h>
#include <templ_s_array.h>
#include <templ_sp_array.h>
#include "iges_basic.h"


//-------------------------------------------------------------------------------
// функции сравнения двух наследников от BasicIGES, которые не содержат
// динамических данных
// ---
template <class Type>
inline bool Eq( const Type * t, const BasicIGES & o ) {
  if ( !t->Eq( o ) )
    return false;

  const Type * r = dynamic_cast<const Type*>(&o);
  if ( !r )
    return false;

  return ::IsEqualSArrayItems( t, r );
}


//-------------------------------------------------------------------------------
// функции сравнения двух наследников от BasicIGES, которые не содержат
// динамических данных
// ---
template <class Type>
inline bool Less( const Type * t, const BasicIGES & o ) {
  if ( !t->Eq( o ) )
    return t->Less( o );

  const Type * r = dynamic_cast<const Type*>(&o);
  if ( !r )
    return false;

  return t < r ;
}


//-------------------------------------------------------------------------------
// структура для сохранения типов линий
// ---
struct CONV_CLASS LTypeNameIGES {
  uint16 number;           // номер стиля в чертеже C3D
  ptrdiff_t   colorOrStr;  // цвет или номер строки цвета в файле IGES
  ptrdiff_t   width;       // толщина линии на бумаге * 1000
  ptrdiff_t   numOrStr;    // номер IGES-типа линии или номер строки типа в файле IGES

  LTypeNameIGES() : number(0), colorOrStr( 0 ), width(1), numOrStr(0){}

  bool operator == (const LTypeNameIGES & o) const { return number == o.number; }
  bool operator <  (const LTypeNameIGES & o) const { return number <  o.number; }

  void Assign( const LTypeNameIGES & o );
};


//-------------------------------------------------------------------------------
// структура цвета и места его хранения
// ---
struct CONV_CLASS ColorIGES : public BasicIGES {
  int32 trueColor;

  ColorIGES( int32 _color = 0 );
  virtual bool operator == ( const BasicIGES & o ) const;
  virtual bool operator <  ( const BasicIGES & o ) const;
};


//-------------------------------------------------------------------------------
// 
// ---
typedef       ColorIGES * PCOLORIGES;
typedef const ColorIGES * PCCOLORIGES;


//-------------------------------------------------------------------------------
// 
// ---
#ifdef _MSC_VER // C3D_WINDOWS
inline bool IsLessThanSArrayItems ( const PCCOLORIGES &obj1, const PCCOLORIGES &obj2 ) { return obj1 <  obj2; }
#else // C3D_WINDOWS
template<> bool IsLessThanSArrayItems< ColorIGES const* > ( ColorIGES const* const& obj1, ColorIGES const* const& obj2 );
#endif // C3D_WINDOWS


//------------------------------------------------------------------------------
//
// ---
struct CONV_CLASS ColourIGES : public BasicIGES {
  double red, green, blue;

  ColourIGES( double, double, double );
};


//-------------------------------------------------------------------------------
// точка
// ---
struct CONV_CLASS PointIGES : public BasicIGES {
  double x, y, z;

  PointIGES( double _x, double _y, double _z );
  PointIGES();

  virtual bool operator == ( const BasicIGES & o ) const;
  virtual bool operator <  ( const BasicIGES & o ) const;
};


//-------------------------------------------------------------------------------
// 
// ---
typedef       PointIGES * PPOINTIGES;
typedef const PointIGES * PCPOINTIGES;


//-------------------------------------------------------------------------------
// 
// ---
#ifdef _MSC_VER // C3D_WINDOWS
inline bool IsLessThanSArrayItems ( const PCPOINTIGES &obj1, const PCPOINTIGES &obj2 ) { return obj1 <  obj2; }
#else // C3D_WINDOWS
template<> bool IsLessThanSArrayItems< PointIGES const* > ( PointIGES const* const& obj1, PointIGES const* const& obj2 );
#endif // C3D_WINDOWS


//-------------------------------------------------------------------------------
// базовый curve примитив
// ---
struct CONV_CLASS BasicCurveIGES : public BasicIGES {
  LTypeNameIGES lt;      // стиль
  BasicCurveIGES( int32 _numType, int32 _form = 0 ) : BasicIGES( _numType, _form ), lt(){}
};


//-------------------------------------------------------------------------------
// структура отрезка
// ---
struct CONV_CLASS LineSegIGES : public BasicCurveIGES {
  double x1, y1, z1;     // координаты 1 точки
  double x2, y2, z2;     // координаты 2 точки

  LineSegIGES();

  LineSegIGES( double x1, double y1, double z1,    // 3D
               double x2, double y2, double z2 );
  LineSegIGES( double x1, double y1,               // 2D
               double x2, double y2 );

  virtual bool operator == ( const BasicIGES & o ) const;
  virtual bool operator <  ( const BasicIGES & o ) const;
};


//-------------------------------------------------------------------------------
// структура дуги и окружности
// ---
struct CONV_CLASS ArcOrCircleIGES : public BasicCurveIGES {
  double dir;        // напрвление
  double xc, yc;     // координаты центра
  double x1, y1;     // координаты 1 точки
  double x2, y2;     // координаты 2 точки

  ArcOrCircleIGES();

  virtual bool operator == ( const BasicIGES & o ) const;
  virtual bool operator <  ( const BasicIGES & o ) const;
};


//-------------------------------------------------------------------------------
// 104 IGS_CONIC_ARC коническая кривая ( эллипс, гипербола, парабола )
// ---
struct CONV_CLASS EllipsIGES : public BasicCurveIGES {
  double A, B, C, D, E, F, X1, Y1, X2, Y2, ZT;

  EllipsIGES();

  virtual bool operator == ( const BasicIGES & o ) const;
  virtual bool operator <  ( const BasicIGES & o ) const;
};


//-------------------------------------------------------------------------------
// 
// ---
typedef       EllipsIGES * PELLIPSIGES;
typedef const EllipsIGES * PCELLIPSIGES;


//-------------------------------------------------------------------------------
// 
// ---
#ifdef _MSC_VER // C3D_WINDOWS
inline bool IsLessThanSArrayItems ( const PCELLIPSIGES &obj1, const PCELLIPSIGES &obj2 ) { return obj1 <  obj2; }
#else // C3D_WINDOWS
template<> bool IsLessThanSArrayItems< EllipsIGES const* > ( EllipsIGES const* const& obj1, EllipsIGES const* const& obj2 );
#endif // C3D_WINDOWS


//-------------------------------------------------------------------------------
// структура элемента текста
// ---
struct CONV_CLASS TextItemIGES {
  double width;     // ширина
  double height;    // высота
  int32  fontCode;  // код шрифта
  double angleChar; // угол наклона букв
  double angleStr;  // угол наклона строки
  int32  flagMirror;// флаг зеркальности
  int32  horizont;  // 0 - отсчет от горизонали 1 - от вертикали
  double x, y, z;   // координаты
  std::string text;      // текст

  TextItemIGES();

  bool operator == ( const TextItemIGES & o ) const;
  bool operator <  ( const TextItemIGES & o ) const;
};


//-------------------------------------------------------------------------------
// структура текста
// ---
struct CONV_CLASS TextIGES : public BasicIGES {
  std::vector <TextItemIGES*> arr;

  TextIGES();

  virtual bool operator == ( const BasicIGES & o ) const;
  virtual bool operator <  ( const BasicIGES & o ) const;
private:
  TextIGES( const TextIGES& );
};


//-------------------------------------------------------------------------------
// 
// ---
typedef       TextIGES * PTEXTIGES;
typedef const TextIGES * PCTEXTIGES;


//-------------------------------------------------------------------------------
// 
// ---
#ifdef _MSC_VER // C3D_WINDOWS
inline bool IsLessThanSArrayItems ( const PCTEXTIGES &obj1, const PCTEXTIGES &obj2 ) { return obj1 <  obj2; }
#else // C3D_WINDOWS
template<> bool IsLessThanSArrayItems< TextIGES const* > ( TextIGES const* const& obj1, TextIGES const* const& obj2 );
#endif // C3D_WINDOWS


//-------------------------------------------------------------------------------
// 123 IGS_DIRECTION - вектор
// ---
struct CONV_CLASS DirectionIGES: public BasicIGES {
  double x, y, z;

  DirectionIGES( double _x, double _y, double _z );

  virtual bool operator == ( const BasicIGES & o ) const;
  virtual bool operator <  ( const BasicIGES & o ) const;
};


//-------------------------------------------------------------------------------
// 
// ---
typedef       DirectionIGES * PDIRECTIONIGES;
typedef const DirectionIGES * PCDIRECTIONIGES;


//-------------------------------------------------------------------------------
// 
// ---
#ifdef _MSC_VER // C3D_WINDOWS
inline bool IsLessThanSArrayItems ( const PCDIRECTIONIGES &obj1, const PCDIRECTIONIGES &obj2 ) { return obj1 <  obj2; }
#else // C3D_WINDOWS
template<> bool IsLessThanSArrayItems< DirectionIGES const* > ( DirectionIGES const* const& obj1, DirectionIGES const* const& obj2 );
#endif // C3D_WINDOWS


//-------------------------------------------------------------------------------
// 124 матрица трансформации
// ---
struct CONV_CLASS MatrixIGES : public BasicIGES {
  std::vector<double> matr;
  MatrixIGES();
  virtual bool operator == ( const BasicIGES & o ) const;
  virtual bool operator <  ( const BasicIGES & o ) const;
};


//-------------------------------------------------------------------------------
// 126 IGS_RATIONAL_B_SPLINE_CURVE
// ---
struct CONV_CLASS RationalBSplineCurveIGES : public BasicCurveIGES {
  ptrdiff_t upperIndexSum;              // верхний индекс суммы
  ptrdiff_t degree;                     // степень базовой функции
  int32 planar;                     // 0 - пространственная 1 - плоская
  int32 closed;                     // 1 - замкнутая 0 - незамкнутая
  int32 polynominal;                // 1 - Polynominal
                                    // 0 - Rational
  int32 periodic;                   // 1 - Периодическая
                                    // 0 - Непериодическая
// Значения последовательностей узлов
  std::vector<double> sequence;        // значения от -degree  до 1 + upperIndexSum

  // массив весовых коэффициентов размером 1 + upperIndexSum
  std::vector<double> weight;
  // массив координат контрольных точек размером 1 + upperIndexSum
  std::vector<double> x;
  std::vector<double> y;
  std::vector<double> z;
  double u0, u1;              // начальное и конечное значение параметрических координат
  double xNorm, yNorm, zNorm;

  RationalBSplineCurveIGES();

  virtual bool operator == ( const BasicIGES & o ) const;
  virtual bool operator <  ( const BasicIGES & o ) const;
};


//-------------------------------------------------------------------------------
// стрелка( или линия выноски ) IGS_LEADER
// ---
struct CONV_CLASS LeaderIGES : public BasicCurveIGES {
  double arrowLen;  // длина стрелки IGS_LENGTH_ARROW
  double arrowWidth;// ширина стрелки IGS_WIDTH_ARROW
  double zDepth;    // глубина по z
  // координаты стрелки
  double xHead, yHead;
  // координаты конца линии
  std::vector<double> x;
  std::vector<double> y;
  int formArrow;    // 0,4 никакой 1,2,3,11 обычная стрелка 9,10 засечка 5,6,7,8 точка

  LeaderIGES();

  virtual bool operator == ( const BasicIGES & o ) const;
  virtual bool operator <  ( const BasicIGES & o ) const;
};


//-------------------------------------------------------------------------------
// 
// ---
typedef       LeaderIGES * PLEADERIGES;
typedef const LeaderIGES * PCLEADERIGES;


//-------------------------------------------------------------------------------
// 
// ---
#ifdef _MSC_VER // C3D_WINDOWS
inline bool IsLessThanSArrayItems ( const PCLEADERIGES &obj1, const PCLEADERIGES &obj2 ) { return obj1 <  obj2; }
#else // C3D_WINDOWS
template<> bool IsLessThanSArrayItems< LeaderIGES const* > ( LeaderIGES const* const& obj1, LeaderIGES const* const& obj2 );
#endif // C3D_WINDOWS


//-------------------------------------------------------------------------------
// вспомогательная линия
// ---
struct CONV_CLASS WitnessLineIGES : public BasicCurveIGES {
  int32  interpretFlag;
  double z; // displacement
  // координаты конца линии N >= 3 и нечетное
  std::vector<double> x;
  std::vector<double> y;

  WitnessLineIGES();
  virtual bool operator == ( const BasicIGES & o ) const;
  virtual bool operator <  ( const BasicIGES & o ) const;
};


//-------------------------------------------------------------------------------
// 
// ---
typedef       WitnessLineIGES * PWITNESSLINEIGES;
typedef const WitnessLineIGES * PCWITNESSLINEIGES;


//-------------------------------------------------------------------------------
// 
// ---
#ifdef _MSC_VER // C3D_WINDOWS
inline bool IsLessThanSArrayItems ( const PCWITNESSLINEIGES &obj1, const PCWITNESSLINEIGES &obj2 ) { return obj1 <  obj2; }
#else // C3D_WINDOWS
template<> bool IsLessThanSArrayItems< WitnessLineIGES const* > ( WitnessLineIGES const* const& obj1, WitnessLineIGES const* const& obj2 );
#endif // C3D_WINDOWS


//-------------------------------------------------------------------------------
// структура линейного размера
// ---
struct CONV_CLASS LinDimensionIGES : public BasicIGES {
  int32 text;        // указатель на текст
  int32 firstArrow;  // первая стрелка( половина размерной линии )
  int32 secondArrow; // вторая стрелка( половина размерной линии )
  int32 firstLine;   // первая выносная линия
  int32 secondLine;  // вторая выносная линия
  LinDimensionIGES();

  virtual bool operator == ( const BasicIGES & o ) const;
  virtual bool operator <  ( const BasicIGES & o ) const;
};


//-------------------------------------------------------------------------------
// 
// ---
typedef       LinDimensionIGES * PLINDIMENSIONIGES;
typedef const LinDimensionIGES * PCLINDIMENSIONIGES;


//-------------------------------------------------------------------------------
// 
// ---
#ifdef _MSC_VER // C3D_WINDOWS
inline bool IsLessThanSArrayItems ( const PCLINDIMENSIONIGES &obj1, const PCLINDIMENSIONIGES &obj2 ) { return obj1 <  obj2; }
#else // C3D_WINDOWS
template<> bool IsLessThanSArrayItems< LinDimensionIGES const* > ( LinDimensionIGES const* const& obj1, LinDimensionIGES const* const& obj2 );
#endif // C3D_WINDOWS


//-------------------------------------------------------------------------------
// структура диаметрального размера
// ---
struct CONV_CLASS DimDimensionIGES : public BasicIGES {
  int32 text;        // указатель на текст
  int32 firstArrow;  // первая стрелка( половина размерной линии )
  int32 secondArrow; // вторая стрелка( половина размерной линии )
  double x, y;
  DimDimensionIGES();

  virtual bool operator == ( const BasicIGES & o ) const;
  virtual bool operator <  ( const BasicIGES & o ) const;
};


//-------------------------------------------------------------------------------
// 
// ---
typedef       DimDimensionIGES * PDIMDIMENSIONIGES;
typedef const DimDimensionIGES * PCDIMDIMENSIONIGES;


//-------------------------------------------------------------------------------
// 
// ---
#ifdef _MSC_VER // C3D_WINDOWS
inline bool IsLessThanSArrayItems ( const PCDIMDIMENSIONIGES &obj1, const PCDIMDIMENSIONIGES &obj2 ) { return obj1 <  obj2; }
#else // C3D_WINDOWS
template<> bool IsLessThanSArrayItems< DimDimensionIGES const* > ( DimDimensionIGES const* const& obj1, DimDimensionIGES const* const& obj2 );
#endif // C3D_WINDOWS


//-------------------------------------------------------------------------------
// структура диаметрального размера
// ---
struct CONV_CLASS RadDimensionIGES : public BasicIGES {
  int32 text;        // указатель на текст
  int32 arrow;       // первая стрелка( половина размерной линии )
  double x, y;
  RadDimensionIGES();

  virtual bool operator == ( const BasicIGES & o ) const;
  virtual bool operator <  ( const BasicIGES & o ) const;
};


//-------------------------------------------------------------------------------
// 
// ---
typedef       RadDimensionIGES * PRADDIMENSIONIGES;
typedef const RadDimensionIGES * PCRADDIMENSIONIGES;


//-------------------------------------------------------------------------------
// 
// ---
#ifdef _MSC_VER // C3D_WINDOWS
inline bool IsLessThanSArrayItems ( const PCRADDIMENSIONIGES &obj1, const PCRADDIMENSIONIGES &obj2 ) { return obj1 <  obj2; }
#else // C3D_WINDOWS
template<> bool IsLessThanSArrayItems< RadDimensionIGES const* > ( RadDimensionIGES const* const& obj1, RadDimensionIGES const* const& obj2 );
#endif // C3D_WINDOWS


//-------------------------------------------------------------------------------
// структура углового размера
// ---
struct CONV_CLASS AngDimensionIGES : public BasicIGES {
  int32 text;        // указатель на текст
  int32 firstLine;   // первая выносная линия
  int32 secondLine;  // вторая выносная линия
  double x, y, r;
  int32 firstArrow;  // первая стрелка( половина размерной линии )
  int32 secondArrow; // вторая стрелка( половина размерной линии )
  AngDimensionIGES();

  virtual bool operator == ( const BasicIGES & o ) const;
  virtual bool operator <  ( const BasicIGES & o ) const;
};


//-------------------------------------------------------------------------------
// 
// ---
typedef       AngDimensionIGES * PANGDIMENSIONIGES;
typedef const AngDimensionIGES * PCANGDIMENSIONIGES;


//-------------------------------------------------------------------------------
// 
// ---
#ifdef _MSC_VER // C3D_WINDOWS
inline bool IsLessThanSArrayItems ( const PCANGDIMENSIONIGES &obj1, const PCANGDIMENSIONIGES &obj2 ) { return obj1 <  obj2; }
#else // C3D_WINDOWS
template<> bool IsLessThanSArrayItems< AngDimensionIGES const* > ( AngDimensionIGES const* const& obj1, AngDimensionIGES const* const& obj2 );
#endif // C3D_WINDOWS


#endif // __IGES_STRUCTURES_H
