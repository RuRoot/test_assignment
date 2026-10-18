////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Окружность, эллипс или их дуга в двумерном пространстве.
         \en Circle, ellipse or circular or elliptical arc in two-dimensional space. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CUR_ARC_H
#define __CUR_ARC_H


#include <curve.h>
#include <mb_placement.h>
#include <alg_base.h>
#include <alg_curve_distance.h>
#include <tool_mutex.h>
#include <mb_dim_accuracy.h>


class   MbLine;
class   MbContour;
class             MbTrimmedCurve;
class             MbRect1D;
class             DiskreteLengthData;


//------------------------------------------------------------------------------
/** \brief \ru Дуга эллипса в двумерном пространстве.
           \en Elliptical arc in two-dimensional space. \~
  \details \ru Дуга эллипса описывается двумя радиусами a и b и двумя параметрами trim1 и trim2, заданными в локальной системе координат position. \n
    Параметры trim1 и trim2 отсчитываются по дуге в направлении движения от оси position.axisX к оси position.axisY. 
    Параметры trim1 и trim2 будем называть параметрами усечения. 
    Значения параметров усечения, равные нулю и 2pi, соответствуют точке на оси position.axisX. \n
    Параметр кривой t принимает значения на отрезке: 0<=t<=trim2–trim1. 
    Кривая может быть замкнутой. У замкнутой кривой trim2–trim1=2pi. \n
    Радиус-вектор кривой в методе PointOn(double&t,MbCartPoint3D&r) описывается векторной функцией\n
    r(t) = position.origin + (a cos(trim1+(sense)t) position.axisX) + (b sin(trim1+(sense)t) position.axisY).\n
    Радиусы кривой должны быть больше нуля: a>0, b>0. \n
    Для параметров усечения должны соблюдаться неравенства: trim1<trim2 при sense==1 и trim1>trim2 при sense==-1. \n
    Локальная система координат position может быть как правой, так и левой. 
    Если локальная система координат правая и sense=+1 или локальная система координат левая и sense=–1, 
    то дуга направлена против движения часовой стрелки.\n
           \en The elliptical arc is described by two radii a and b and two parameters trim1 and trim2 given in the local coordinate system 'position'. \n
    Parameters 'trim1' and 'trim2' are measured along the arc in direction from position.axisX axis to position.axisY axis. 
    Parameters 'trim1' and 'trim2' will be called parameters of trimming. 
    Values of parameters of trimming equal to 0 and 2pi correspond to a point on position.axisX axis. \n
    Parameter t of curve possesses the values in the range: 0<=t<=trim2-trim1. 
    The curve can be closed. For closed curve: trim2-trim1=2pi. \n
    Radius-vector of the curve in the method PointOn(double&t,MbCartPoint3D&r) is described by the function\n
    r(t) = position.origin + (a cos(trim1+(sense)t) position.axisX) + (b sin(trim1+(sense)t) position.axisY).\n
    Radii of the curve must be positive: a>0, b>0. \n
    The following inequalities must be satisfied for the parameters of trimming: trim1<trim2 if sense==1 and trim1>trim2 if sense==-1. \n
    The local coordinate system 'position' can be both right and left. 
    If the local coordinate system is right and sense=+1 or the local coordinate system is left and sense=-1, 
    then the arc is oriented counterclockwise.\n \~
  \ingroup Curves_2D
*/
// ---
class MATH_CLASS MbArc : public MbCurve {
protected :
  MbPlacement position; ///< \ru Локальная система координат. \en Local coordinate system. 
  double      a;        ///< \ru Радиус полуоси вдоль X. \en Radius of semiaxis along X. 
  double      b;        ///< \ru Радиус полуоси вдоль Y. \en Radius of semiaxis along Y. 
  double      trim1;    ///< \ru Параметры начальной точки. \en The start point parameters. 
  double      trim2;    ///< \ru Параметры конечной  точки. \en The end point parameters. 
  int         sense;    ///< \ru Флаг совпадения с направлением от axisX к axisY (sense==0 не допускается). \en Flag of coincidence with direction from axisX to axisY (sense==0 is not allowed). 
  bool        circle;   ///< \ru Флаг, указывающий является объект окружностью (true) или эллипсом (false). \en Whether the object is a circle (true) or an ellipse (false).
  bool        closed;   ///< \ru Флаг, указывающий является объект замкнутой кривой (true) или дугой (false). \en Whether the object is a closed curve (true) or an arc (false).

  /** \brief \ru Вспомогательные данные. 
             \en Auxiliary data. \~
      \details \ru Вспомогательные данные служат для ускорения работы объекта.
               \en Auxiliary data are used for fast calculations. \n \~
  */
  mutable MbRect        rect;         ///< \ru Габаритный прямоугольник. \en Bounding rectangle. 
  mutable atomic_double metricLength; ///< \ru Метрическая длина кривой. \en Metric length of curve. 

public :
  /** \brief \ru Конструктор окружности с параметрами по умолчанию.
             \en Constructor of a circle with default parameters. \~
    \details \ru Создается окружность с центром в начале координат и с нулевым радиусом.
             \en A circle is created with center in the origin and zero radius. \~
  */
  MbArc(); // \ru Конструктор по умолчанию. \en Default constructor. 

  /** \brief \ru Конструктор окружности по радиусу.
             \en Constructor of a circle by radius. \~
    \details \ru Создается окружность с центром в начале координат и с заданным радиусом.
             \en A circle is created with center in the origin and the given radius. \~
    \param[in] rad - \ru Радиус.
                     \en Radius. \~
  */
  MbArc( double rad ); // \ru Конструктор окружности по радиусу с центром в начале координат. \en Constructor of a circle by the given radius and centered in the origin. 

  /** \brief \ru Конструктор окружности.
             \en Constructor of a circle. \~
    \details \ru Создается окружность с центром в точке p и с заданным радиусом.
             \en A circle is created with center in point 'p' and with the given radius. \~
    \param[in] p - \ru Центр окружности.
                   \en Center of circle. \~
    \param[in] rad - \ru Радиус.
                     \en Radius. \~
  */
  MbArc( const MbCartPoint & p, double rad ); // \ru Конструктор окружности \en Constructor of a circle. 

  /** \brief \ru Создать окружность.
             \en Create a circle. \~
    \details \ru Создается окружность с центром в точке pc. 
      Радиус определяется как расстояние между точками pc и on .
             \en A circle is created with center in point 'pc'. 
      The radius is determined as the distance between points 'pc' and 'on'. \~
    \param[in] pc - \ru Центр окружности.
                    \en Center of circle. \~
    \param[in] on - \ru Точка на окружности.
                    \en Point on circle. \~
  */
  MbArc( const MbCartPoint & pc, const MbCartPoint & on ); // \ru Конструктор окружности \en Constructor of a circle 

  /** \brief \ru Конструктор дуги окружности.
             \en Constructor of a circular arc. \~
    \details \ru Создается дуга окружности с центром в точке p и с заданным радиусом.
      Точки p1 и p2 определяют границы дуги. 
      Начальная точка дуги лежит на луче, выходящем из центра окружности и проходящем через точку p1.
      Конечная точка - на луче, проходящем через точку p2. 
      Параметр initSense определяет направление дуги. Если initSense > 0, то направление движения против часовой стрелки.
             \en A circular arc is created with a center in point 'p' and with a given radius.
      Points 'p1' and 'p2' specify the bounds of arc. 
      The start point of the arc lies on the ray starting from the circle center and passing through point 'p1'.
      The end point is on the ray passing through the point 'p2'. 
      Parameter 'initSense' specifies the arc direction. If initSense > 0, then the orientation is counterclockwise. \~
    \param[in] pc - \ru Центр окружности.
                    \en Center of circle. \~
    \param[in] rad - \ru Радиус.
                     \en Radius. \~
    \param[in] p1 - \ru Точка, определяющая начало дуги.
                    \en A point specifying the beginning of the arc. \~
    \param[in] p2 - \ru Точка, определяющая конец дуги.
                    \en A point specifying the end of the arc. \~
    \param[in] initSense - \ru Направление. initSense > 0 - движение против часовой стрелки, initSense < 0 - по часовой стрелке.
      initSense не должно быть равным нулю.
                           \en Direction. initSense > 0 - counterclockwise, initSense < 0 - clockwise.
      initSense cannot be equal to zero. \~
  */
  MbArc( const MbCartPoint & pc, double rad, const MbCartPoint & p1, const MbCartPoint & p2, int initSense ); // \ru Конструктор дуги \en The arc constructor 

  /** \brief \ru Конструктор дуги окружности.
             \en Constructor of a circular arc. \~
    \details \ru Создается дуга окружности с центром в точке p и с заданным радиусом.
      t1 и t2 определяют начальный и конечный углы дуги. Углы отсчитываются от оси OX против часовой стрелки. 
      Углы заданы в радианах.
      Параметр initSense определяет направление дуги. Если initSense > 0, то направление движения против часовой стрелки.
             \en A circular arc is created with a center in point 'p' and with a given radius.
      t1 and t2 specify the start and the end angles of the arc. The angles are measured from the OX axis counterclockwise. 
      The angles are given in radians.
      Parameter 'initSense' specifies the arc direction. If initSense > 0, then the orientation is counterclockwise. \~
    \param[in] pc - \ru Центр окружности.
                    \en Center of circle. \~
    \param[in] rad - \ru Радиус.
                     \en Radius. \~
    \param[in] t1 - \ru Угол, определяющий начало дуги.
                    \en An angle specifying the beginning of the arc. \~
    \param[in] t2 - \ru Угол, определяющий конец дуги.
                    \en An angle specifying the end of the arc. \~
    \param[in] initSense - \ru Направление. initSense > 0 - движение против часовой стрелки, initSense < 0 - по часовой стрелке.
      initSense не должно быть равным нулю.
                           \en Direction. initSense > 0 - counterclockwise, initSense < 0 - clockwise.
      initSense can't be equal to zero. \~
  */
  MbArc( const MbCartPoint & pc, double rad, double t1, double t2, int initSense ); // \ru Конструктор дуги \en The arc constructor 

  /** \brief \ru Конструктор дуги эллипса по образцу и концевым точкам.
             \en Constructor of an elliptic arc based on a sample and bounding points. \~
    \details \ru Создается дуга на основе образца данного эллипса или окружности.
      Точки p1 и p2 определяют границы дуги. 
      Начальная точка дуги лежит на луче, выходящем из центра окружности и проходящем через точку p1.
      Конечная точка - на луче, проходящем через точку p2.
      Параметр initSense определяет направление дуги. Если initSense > 0, то направление движения против часовой стрелки.
             \en An arc based on the given sample of circle or ellipse is created.
      Points 'p1' and 'p2' specify the bounds of arc. 
      The start point of the arc lies on the ray starting from the circle center and passing through point 'p1'.
      The end point is on the ray passing through the point 'p2'. 
      Parameter 'initSense' specifies the arc direction. If initSense > 0, then the orientation is counterclockwise. \~
    \param[in] init - \ru Образец окружности или эллипса.
                      \en A sample circle or ellipse. \~
    \param[in] p1 - \ru Точка, определяющая начало дуги.
                    \en A point specifying the beginning of the arc. \~
    \param[in] p2 - \ru Точка, определяющая конец дуги.
                    \en A point specifying the end of the arc. \~
    \param[in] initSense - \ru Направление. initSense > 0 - движение против часовой стрелки, initSense < 0 - по часовой стрелке.
      initSense не должно быть равным нулю.
                           \en Direction. initSense > 0 - counterclockwise, initSense < 0 - clockwise.
      initSense can't be equal to zero. \~
  */
  MbArc( const MbArc &init, const MbCartPoint & p1, const MbCartPoint & p2, int initSense ); // \ru Конструктор дуги \en The arc constructor 

  /** \brief \ru Конструктор дуги окружности.
             \en Constructor of a circular arc. \~
    \details \ru Создается дуга окружности с центром в точке pc.
      Радиус определяется как расстояние между точками pc и p1.
      Точки p1 и p2 определяют границы дуги. 
      Начальная точка дуги лежит на луче, выходящем из центра окружности и проходящем через точку p1.
      Конечная точка - на луче, проходящем через точку p2. 
      Параметр initSense определяет направление дуги. Если initSense > 0, то направление движения против часовой стрелки.
             \en An arc of a circle centered in point 'pc'.
      The radius is determined as the distance between points 'pc' and 'p1'.
      Points 'p1' and 'p2' specify the bounds of arc. 
      The start point of the arc lies on the ray starting from the circle center and passing through point 'p1'.
      The end point is on the ray passing through the point 'p2'. 
      Parameter 'initSense' specifies the arc direction. If initSense > 0, then the orientation is counterclockwise. \~
    \param[in] pc - \ru Центр окружности.
                    \en Center of circle. \~
    \param[in] p1 - \ru Точка, определяющая начало дуги и радиус.
                    \en A point determining the beginning of the arc and the radius. \~
    \param[in] p2 - \ru Точка, определяющая конец дуги.
                    \en A point specifying the end of the arc. \~
    \param[in] initSense - \ru Направление. initSense > 0 - движение против часовой стрелки, initSense < 0 - по часовой стрелке.
      initSense не должно быть равным нулю.
                           \en Direction. initSense > 0 - counterclockwise, initSense < 0 - clockwise.
      initSense can't be equal to zero. \~
  */
  MbArc( const MbCartPoint & pc, const MbCartPoint & p1, const MbCartPoint & p2, int initSense ); // \ru Конструктор дуги \en The arc constructor 

  /** \brief \ru Конструктор дуги эллипса.
             \en Constructor of an elliptical arc. \~
    \details \ru Создается дуга эллипса с заданными полуосями и локальной системой координат. 
      Точки p1 и p2 определяют границы дуги. 
      Начальная точка дуги лежит на луче, выходящем из центра окружности и проходящем через точку p1.
      Конечная точка - на луче, проходящем через точку p2. 
      Параметр initSense определяет направление дуги. Если initSense > 0, то направление движения против часовой стрелки.
             \en An elliptical arc is created with the given semiaxes and the local coordinate system. 
      Points 'p1' and 'p2' specify the bounds of arc. 
      The start point of the arc lies on the ray starting from the circle center and passing through point 'p1'.
      The end point is on the ray passing through the point 'p2'. 
      Parameter 'initSense' specifies the arc direction. If initSense > 0, then the orientation is counterclockwise. \~
    \param[in] aa - \ru Радиус полуоси вдоль X.
                    \en Radius of semiaxis along X. \~
    \param[in] bb - \ru Радиус полуоси вдоль Y.
                    \en Radius of semiaxis along Y. \~
    \param[in] place - \ru Локальная система координат эллипса.
                       \en The local coordinate system of the ellipse. \~
    \param[in] p1 - \ru Точка, определяющая начало дуги.
                    \en A point specifying the beginning of the arc. \~
    \param[in] p2 - \ru Точка, определяющая конец дуги.
                    \en A point specifying the end of the arc. \~
    \param[in] initSense - \ru Направление. initSense > 0 - движение против часовой стрелки, initSense < 0 - по часовой стрелке.
      initSense не должно быть равным нулю.
                           \en Direction. initSense > 0 - counterclockwise, initSense < 0 - clockwise.
      initSense can't be equal to zero. \~
  */
  MbArc( double aa, double bb, const MbPlacement & place, const MbCartPoint & p1, const MbCartPoint & p2, int initSense ); // \ru Конструктор дуги эллипса \en Constructor of an elliptical arc 

  /** \brief \ru Конструктор дуги эллипса.
             \en Constructor of an elliptical arc. \~
    \details \ru Создается дуга эллипса с заданными полуосями и локальной системой координат. 
      t1 и t2 определяют начальный и конечный углы дуги. Углы отсчитываются от оси OX против часовой стрелки. 
      Углы заданы в радианах.
      Параметр initSense определяет направление дуги. Если initSense > 0, то направление движения против часовой стрелки.
             \en An elliptical arc is created with the given semiaxes and the local coordinate system. 
      t1 and t2 specify the start and the end angles of the arc. The angles are measured from the OX axis counterclockwise. 
      The angles are given in radians.
      Parameter 'initSense' specifies the arc direction. If initSense > 0, then the orientation is counterclockwise. \~
    \param[in] aa - \ru Радиус полуоси вдоль X.
                    \en Radius of semiaxis along X. \~
    \param[in] bb - \ru Радиус полуоси вдоль Y.
                    \en Radius of semiaxis along Y. \~
    \param[in] place - \ru Локальная система координат эллипса.
                       \en The local coordinate system of the ellipse. \~
    \param[in] t1 - \ru Угол, определяющий начало дуги.
                    \en An angle specifying the beginning of the arc. \~
    \param[in] t2 - \ru Угол, определяющий конец дуги.
                    \en An angle specifying the end of the arc. \~
    \param[in] initSense - \ru Направление. initSense > 0 - движение против часовой стрелки, initSense < 0 - по часовой стрелке.
      initSense не должно быть равным нулю.
                           \en Direction. initSense > 0 - counterclockwise, initSense < 0 - clockwise.
      initSense can't be equal to zero. \~
  */
  MbArc( double aa, double bb, const MbPlacement & place, double t1, double t2, int initSense ); // \ru Конструктор дуги эллипса \en Constructor of an elliptical arc 

  /** \brief \ru Конструктор дуги эллипса.
             \en Constructor of an elliptical arc. \~
    \details \ru Создается дуга эллипса с локальной системой координат и полуосями заданного эллипса.
      t1 и t2 определяют начальный и конечный углы дуги. Углы отсчитываются от оси OX против часовой стрелки. 
      Углы заданы в радианах.
      Параметр initSense определяет направление дуги. Если initSense > 0, то направление движения против часовой стрелки.
             \en An elliptical arc is constructed with the local coordinate system and semiaxes of the given ellipse.
      t1 and t2 specify the start and the end angles of the arc. The angles are measured from the OX axis counterclockwise. 
      The angles are given in radians.
      Parameter 'initSense' specifies the arc direction. If initSense > 0, then the orientation is counterclockwise. \~
    \param[in] ellipse - \ru Эллипс - образец.
                         \en A pattern ellipse. \~
    \param[in] t1 - \ru Угол, определяющий начало дуги.
                    \en An angle specifying the beginning of the arc. \~
    \param[in] t2 - \ru Угол, определяющий конец дуги.
                    \en An angle specifying the end of the arc. \~
    \param[in] initSense - \ru Направление. initSense > 0 - движение против часовой стрелки, initSense < 0 - по часовой стрелке.
      initSense не должно быть равным нулю.
                           \en Direction. initSense > 0 - counterclockwise, initSense < 0 - clockwise.
      initSense can't be equal to zero. \~
  */
  MbArc( const MbArc & ellipse, double t1, double t2, int initSense );

  /** \brief \ru Конструктор эллипса.
             \en Constructor of an ellipse. \~
    \details \ru Создается эллипс с заданными локальной системой координат и полуосями.
             \en An ellipse is created with the given local coordinate system and semiaxes. \~
    \param[in] aa - \ru Радиус полуоси вдоль X.
                    \en Radius of semiaxis along X. \~
    \param[in] bb - \ru Радиус полуоси вдоль Y.
                    \en Radius of semiaxis along Y. \~
    \param[in] pos - \ru Локальная система координат эллипса.
                     \en The local coordinate system of the ellipse. \~
  */
  MbArc( double aa, double bb, const MbPlacement & pos ); // \ru Конструктор эллипса \en Constructor of an ellipse 

  /** \brief \ru Конструктор эллипса.
             \en Constructor of an ellipse. \~
    \details \ru Создается эллипс с заданными полуосями.
      Локальная система координат эллипса имеет начало в точке c и 
      ось OX локальной системы координат составляет с осью OX текущей системы координат угол angle.
      Направление поворота от оси текущей системы координат к оси новой системы координат.
             \en An ellipse is created with the given semiaxes.
      The local coordinate system of the ellipse has the origin in point 'c'; 
      OX axis of the local coordinate system forms angle 'angle' with the OX axis of the current coordinate system.
      Direction of turning from the current coordinate system axis to the axis of the new coordinate system. \~
    \param[in] aa - \ru Радиус полуоси вдоль X.
                    \en Radius of semiaxis along X. \~
    \param[in] bb - \ru Радиус полуоси вдоль Y.
                    \en Radius of semiaxis along Y. \~
    \param[in] c - \ru Начало локальной системы координат эллипса.
                   \en Origin of local coordinate system of ellipse. \~
    \param[in] angle - \ru Угол между осями OX локальной и текущей системами координат.
                       \en An angle between OX axes of the local and the current coordinate systems. \~
  */
  MbArc( double aa, double bb, const MbCartPoint & c, double angle ); // \ru Конструктор эллипса \en Constructor of an ellipse

//protected :
  /// \ru Конструктор копирования. \en Copy-constructor. 
  explicit MbArc( const MbArc & init );
public :
  /// \ru Деструктор \en Destructor 
  virtual ~MbArc();

public:
  /** \brief \ru Создать дугу окружности.
             \en Create circular arc. \~
    \details \ru Создается дуга окружности, проходящая через все 3 заданные точки.
      Точки p1 и p3 - крайние. Направление движения по дуге определяется так, чтобы точка p2 лежала на дуге.
             \en A circular arc is created passing through 3 given points.
      Points p1 and p3 are the end points. Direction of moving along the arc is defined so as point p2 lay on the arc. \~
    \param[in] p1 - \ru Начало дуги.
                    \en Beginning of the arc. \~
    \param[in] p2 - \ru Точка, лежащая на дуге.
                    \en A point on the arc. \~
    \param[in] p3 - \ru Конец дуги.
                    \en End of the arc. \~
  */
  static MbArc * Create( const MbCartPoint & p1, const MbCartPoint & p2, const MbCartPoint & p3 );

  /** \brief \ru Создать дугу окружности.
             \en Create circular arc. \~
    \details \ru Создается дуга окружности с концами в заданных точках.
      Радиус окружности определяется по заданному тангенсу 1/4 угла раствора дуги.
             \en An arc is created with ends at the given points.
      A circle radius is defined by the given tangent of 1/4 of arc opening angle. \~
    \param[in] p1 - \ru Начало дуги.
                    \en Beginning of the arc. \~
    \param[in] p2 - \ru Конец дуги.
                    \en End of the arc. \~
    \param[in] a4 - \ru Тангенс 1/4 угла раствора дуги.
                    \en Tangent of 1/4 of the arc opening angle. \~
  */
  static MbArc * Create( const MbCartPoint & p1, const MbCartPoint & p2, double a4 );

public:
  VISITING_CLASS( MbArc );

  /** \ru \name Общие функции геометрического объекта.
      \en \name Common functions of a geometric object.
      \{ */
  MbePlaneType  IsA() const override; // \ru Тип элемента \en A type of element 
  MbPlaneItem & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента \en Create a copy of the element
  bool        IsSame   ( const MbPlaneItem & other, double accuracy = LENGTH_EPSILON ) const override;
  bool        SetEqual ( const MbPlaneItem & ) override; // \ru Сделать равным \en Make equal 
  void        Transform( const MbMatrix & matr, MbRegTransform * ireg = nullptr, const MbSurface * newSurface = nullptr ) override; // \ru Преобразовать согласно матрице \en Transform according to the matrix
  void        Move     ( const MbVector &, MbRegTransform * = nullptr, const MbSurface * newSurface = nullptr ) override; // \ru Сдвиг \en Translation
  void        Rotate( const MbCartPoint & pnt, const MbDirection & angle, MbRegTransform * = nullptr, const MbSurface * newSurface = nullptr ) override; // \ru Поворот \en Rotation
  double      DistanceToPoint( const MbCartPoint & ) const override;// \ru Расстояние до точки \en Distance to a point 
  bool        DistanceToPointIfLess( const MbCartPoint & toP, double & d ) const override; // \ru Расстояние до точки, если оно меньше d \en Distance to a point if it is less than 'd' 
  void        AddYourGabaritTo( MbRect & r ) const override; // \ru Добавь свой габарит в прямой прям-к \en Add own bounding rectangle to an upright bounding rectangle
  void        CalculateGabarit( MbRect & r ) const override;
  bool        IsInRectForDeform( const MbRect & r ) const override; // \ru Виден ли объект в заданном прямоугольнике для деформации \en Whether the object is visible in the given rectangle for deformation 
  MbeState    Deformation( const MbRect &, const MbMatrix & ) override; // \ru Деформация \en Deformation 
  void        Refresh() override; // \ru Сбросить все временные данные \en Flush all the temporary data 
  void        PrepareIntegralData( const bool forced ) const override; // \ru Рассчитать временные (mutable) данные объекта. \en Calculate temporary (mutable) data of an object.
  bool        IsVisibleInRect( const MbRect & r, bool exact = false ) const override; // \ru Виден ли объект в заданном прямоугольнике \en Whether the object is visible in the given rectangle 
  using       MbCurve::IsVisibleInRect;    
  bool        IsCompleteInRect( const MbRect & r ) const override; // \ru Виден ли объект полностью в в заданном прямоугольнике \en Whether the object is completely visible in the given rectangle 
  /** \} */
  /** \ru \name Функции описания области определения кривой.
      \en \name Functions for curve domain description
      \{ */
  double      GetTMax()  const override; // \ru Вернуть максимальное значение параметра \en Get the maximum value of parameter 
  double      GetTMin()  const override; // \ru Вернуть минимальное значение параметра \en Get the minimum value of parameter 
  bool        IsClosed() const override; // \ru Проверка замкнутости кривой \en Check for curve closedness 
  double      GetPeriod() const override; // \ru Вернуть период \en Return the period  
  /** \} */
  /** \ru \name Функции для работы в области определения кривой. 
            Функции PointOn, FirstDer, SecondDer, ThirdDer,... корректируют параметр
            при выходе его за пределы области определения.
      \en \name Functions for working in the domain of a curve. 
            PointOn, FirstDer, SecondDer, ThirdDer,... functions correct parameter
            when it runs out the domain.
      \{ */
  void        PointOn  ( double & t, MbCartPoint & ) const override; // \ru Точка на кривой. \en Point on the curve.
  void        FirstDer ( double & t, MbVector & ) const override;  // \ru Первая производная. \en The first derivative.
  void        SecondDer( double & t, MbVector & ) const override;  // \ru Вторая производная. \en The Second derivative.
  void        ThirdDer ( double & t, MbVector & ) const override;  // \ru Третья производная. \en The third derivative with respect.
  virtual void        Normal   ( double & t, MbVector & ) const;// \ru Вектор главной нормали. \en Vector of the principal normal.
  /** \} */
  /** \ru \name Функции для работы внутри и вне области определения кривой. 
            Функции _PointOn, _FirstDer, _SecondDer, _ThirdDer,... не корректируют параметр
            при выходе за пределы области определения. Ограниченная кривая продолжается в соответствии с уравнениями кривой.
      \en \name Functions for working inside and outside of the curve domain. 
            _PointOn, _FirstDer, _SecondDer, _ThirdDer,... functions don't correct parameter
            when it runs out the domain. The bounded curve is extended due to the equations of curve.
      \{ */
  void       _PointOn  ( double  t, MbCartPoint & ) const override; // \ru Точка на кривой. \en Point on the curve.
  void       _FirstDer ( double  t, MbVector & ) const override;  // \ru Первая производная. \en The first derivative.
  void       _SecondDer( double  t, MbVector & ) const override;  // \ru Вторая производная. \en The Second derivative.
  void       _ThirdDer ( double  t, MbVector & ) const override;  // \ru Третья производная. \en The third derivative with respect.
  virtual void       _Normal   ( double  t, MbVector & ) const;// \ru Вектор главной нормали. \en Vector of the principal normal.
  /** \} */
  /** \ru \name Функции доступа к группе данных для работы внутри и вне области определения параметра кривой.
      \en \name Functions for get of the group of data inside and outside the curve's domain of parameter.
      \{ */
  // \ru Вычислить значения точки и производных для заданного параметра. \en Calculate point and derivatives of object for given parameter. \~
  void        Explore( double & t, bool ext,
                       MbCartPoint & pnt, MbVector & fir, MbVector * sec, MbVector * thir ) const override;
  /** \} */
  /** \ru \name Функции движения по кривой
      \en \name Functions of moving along the curve
    \{ */
  double      Step         ( double t, double sag    ) const override; // \ru Вычисление шага аппроксимации \en Calculation of the approximation step   
  double      DeviationStep( double t, double angle  ) const override;
  /** \} */
  /** \ru \name Общие функции кривой
      \en \name Common functions of the curve
      \{ */
  void        Inverse( MbRegTransform * iReg = nullptr ) override; // \ru Изменить направление \en Change direction

  double      GetMetricLength() const override; // \ru Выдать метрическую длину кривой \en Get the metric length of the curve  
  double      CalculateMetricLength() const override; // \ru Посчитать метрическую длину \en Calculate metric length 
  double      GetLengthEvaluation() const override;
  double      Curvature( double t ) const override; // \ru Кривизна по t \en Curvature by t 
  // \ru Посчитать метрическую длину дуги от параметра t1 до t2. \en Calculate the metric length of the arc from parameter 't1' to 't2'. 
  double      CalculateLength( double t1, double t2 ) const override;
  // \ru Сдвинуть параметр t на расстояние len по направлению \en Shift the parameter t by the distance 'len' in the direction 
  bool        DistanceAlong( double & t, double len, int curveDir, double eps = Math::LengthEps,
                             VERSION version = Math::DefaultMathVersion() ) const override; 

  double      PointProjection( const MbCartPoint & pnt ) const override; // \ru Проекция точки на кривую \en Projection of a point onto the curve 
  bool        NearPointProjection( const MbCartPoint & pnt, double xEpsilon, double yEpsilon, 
                                           double & t, bool ext, MbRect1D * tRange = nullptr ) const override; // \ru Проекция точки на кривую или её продолжение в области поиска проекции \en Projection of the point onto the curve or its extension in the projection region
  // \ru Вычисление всех касательных к кривой из данной точки \en Calculation of all the tangents to the curve from a given point 
  void        TangentPoint( const MbCartPoint & pnt, SArray<double> & tFind ) const override;
  // \ru Вычисление всех перпендикуляров к кривой из данной точки \en Calculation of all the perpendiculars to the curve from a given point 
  void        PerpendicularPoint( const MbCartPoint & pnt, SArray<double> & tFind ) const override;
  // \ru Нахождение ближайшего перпендикуляра к кривой из данной точки \en Calculation of the closest perpendicular to the curve from the given point 
  bool        SmallestPerpendicular( const MbCartPoint & pnt, double & tProj ) const override;

  void        IntersectHorizontal( double y, SArray<double> & cross ) const override; // \ru Пересечение с горизонтальной прямой \en Intersection with a horizontal line 
  void        IntersectVertical  ( double x, SArray<double> & cross ) const override; // \ru Пересечение с вертикальной прямой \en Intersection with a vertical line 

  bool        GetCentre( MbCartPoint & c ) const override; ///< \ru Вернуть центр эллипса или окружности. \en Return the center of an ellipse or a circle.
  virtual     const MbCartPoint & GetCentre() const;          ///< \ru Вернуть центр эллипса или окружности. \en Return the center of an ellipse or a circle.
  bool        GetMiddlePoint( MbCartPoint & p ) const override; // \ru Выдать среднюю точку дуги окружности \en Get the middle point of a circular arc
  bool        GetWeightCentre( MbCartPoint & p ) const override;  // \ru Выдать центр тяжести кривой \en Get the center of gravity of the curve
  double      GetRadius( double accuracy = PARAM_REGION ) const override; // \ru Дать физический радиус объекта или ноль, если это невозможно. \en Get the physical radius of the object or null if it impossible.
  bool        IsSimilarToCurve( const MbCurve & curve, double precision = PARAM_PRECISION ) const override; // \ru Подобные ли кривые для объединения (слива) \en Whether the curves for union (joining) are similar 
  size_t      GetCount() const override; // \ru Количество разбиений для прохода в операциях \en Count of splittings for pass in operations  
  void        GetPointsByEvenLengthDelta( size_t n,  std::vector<MbCartPoint> & pnts ) const override; // \ru Выдать n точек кривой с равными интервалами по длине дуги \en Get n points of curves equally spaced by the arc length 

  bool        HasLength( double & length ) const override; 
  bool        IsDegenerate( double eps = Math::LengthEps ) const override; // \ru Проверка вырожденности кривой \en Check for curve degeneracy
 
  MbNurbs *   NurbsCurve( const MbCurveIntoNurbsInfo & ) const override;
  MbCurve *   NurbsCurve( const MbNurbsParameters & ) const override; // \ru Построить Nurbs-копию кривой \en Construct NURBS-copy of the curve 
  MbContour * NurbsContour() const override;

  // \ru Построить усеченную кривую c учетом заданной двумерной точности. \en Construct a trimmed curve with the given two-dimensional accuracy.
  MbCurve *   Trimmed( double t1, double t2, int sense, const MbDimAccuracy & xyEps, bool saveParamLenAndLaw ) const override;
  using MbCurve::Trimmed; // \ru Метод базового класса MbCurve без учета двумерной точности. Точность по умолчанию - MbDimAccuracy::twoDimAcc.  \en Method of MbCurve base class without two-dimensional accuracy. The default accuracy is MbDimAccuracy::twoDimAcc.

  MbCurve *   Offset ( double rad ) const override;    // \ru Смещение дуги эллипса \en Elliptical arc offset 
  // \ru Положение точки относительно кривой. \en The point position relative to the curve. 
  // \ru Возвращает результат: \en Returning result: 
  // \ru iloc_InItem = 1 - точка находится слева по направлению обхода, \en Iloc_InItem = 1 - the point is on the left, 
  // \ru iloc_OnItem = 0 - точка находится на окружности, \en Iloc_OnItem = 0 - the point is on the circle, 
  // \ru iloc_OutOfItem = -1 - точка находится справа по направлению обхода. \en Iloc_OutOfItem = -1 - the point is on the right. 
  MbeItemLocation PointRelative( const MbCartPoint & pnt, double eps = Math::LengthEps ) const override;

  MbeState    DeletePart( double t1, double t2, MbCurve *& part2 ) override; // \ru Удалить часть усеченной кривой между параметрами t1 и t2 \en Delete a part of a trimmed curve between parameters t1 and t2 
  MbeState    TrimmPart ( double t1, double t2, MbCurve *& part2 ) override; // \ru Оставить часть усеченной кривой между параметрами t1 и t2 \en Keep a part of a trimmed curve between parameters t1 and t2 

  /** \brief \ru Модифицировать эллипс по характерной точке.
             \en Modify the ellipse by a characteristic point. \~
    \param[in] ind - \ru Номер характерной точки. Возможные значения:\n
      0 - Центр эллипса.\n
      1 - Точка на эллипсе, соответствующая 0 гр.\n
      2 - Точка на эллипсе, соответствующая 90 гр.\n
      3 - Точка на эллипсе, соответствующая 180 гр.\n
      4 - Точка на эллипсе, соответствующая 270 гр.
                     \en Index of a characteristic point. Possible values:\n
      0 - Ellipse center.\n
      1 - The point on ellipse corresponding to 0 degrees.\n
      2 - The point on ellipse corresponding to 90 degrees.\n
      3 - The point on ellipse corresponding to 180 degrees.\n
      4 - The point on ellipse corresponding to 270 degrees. \~
    \param[in] pnt - \ru Характерная точка.
                     \en Characteristic point. \~
    \return \ru true - если операция прошла успешно. Иначе возвращает false.
            \en True - if the operation succeeded. Otherwise returns false. \~
  */
  bool        ModifyByPoint( size_t ind, const MbCartPoint & pnt ); // \ru Модификация по характерным точкам \en Modification by the characteristic points 

  bool        GetSpecificPoint( const MbCartPoint & from, double & dmax, MbCartPoint & pnt ) const override;
  void        Isoclinal( const MbVector & angle, SArray <double> & tFind ) const override; // \ru Прямые, проходящие под углом к оси 0X и касательные к кривой \en Lines passing angularly to the 0X axis and tangent to the curve
  bool        GetAxisPoint( MbCartPoint & p ) const override; // \ru Точка для построения оси \en A point for the axis construction 

  // \ru Продлить кривую. \en Extend the curve. \~
  MbResultType Extend( const MbCurveExtensionParameters & parameters, c3d::PlaneCurveSPtr & resCurve ) const override;

  /// \ru Проверить с заданной точностью, является ли эллипс окружностью. \en Check whether the ellipse is a circle with a given tolerance. 
  bool        IsCircle( double eps = PARAM_REGION ) const;
  /** \} */
  /** \ru \name Функции в локальной системе координат плейсмента объекта.
      \en \name Functions in the local coordinate system of object placement.
    \{ */
  /// \ru Выдать локальную систему координат объекта. \en Get the local coordinate system of an object. 
  const   MbPlacement & GetPlacement() const { return position; }
  /// \ru Изменить локальную систему координат объекта. \en Modify the local coordinate system of the object. 
  void        SetPlacement( const MbPlacement & pl ) { position = pl; Refresh(); }
  /// \ru Определить, является ли локальная система координат ортонормированной. \en Determine whether the local coordinate system is orthonormalized. 
  bool        IsPositionNormal()    const { return ( position.IsNormal()   ); }
  /// \ru Определить, является ли локальная система координат ортогональной с равными по длине осями X,Y. \en Determine whether the local coordinate system is orthogonal with X and Y axes equal by length. 
  bool        IsPositionCircular()  const { return ( position.IsCircular() ); }
  /// \ru Определить, является ли локальная система координат ортогональной и изотропной по осям. \en Determine whether the local coordinate system is orthogonal and isotropic by the axes. 
  bool        IsPositionIsotropic() const { return ( position.IsIsotropic()); }

  /** \brief \ru Вычислить угол в локальной системе координат.
             \en Calculate the angle in the local coordinate system. \~
    \details \ru Вычислить угол между осью OX локальной системы координат и 
      лучом, выходящим из начала локальной системы координат и проходящим через точку p.
             \en Calculate the angle between OX axis of the local coordinate system and 
      the ray starting from the origin of local coordinate system and passing through point p. \~
    \param[in] p - \ru Заданная точка.
                   \en A given point. \~
    \return \ru Значение угла.
            \en Value of angle. \~
  */
  double      GetPositionAngle( const MbCartPoint & p ) const; // \ru Вычисление угла в локальной системе \en Calculation of angle in the local system  

  /** \brief \ru Инициализация параметров эллипса.
             \en Initialization of the ellipse parameters. \~
    \details \ru Параметры вычисляются в соответствии с направлением и углами, 
      соответствующими началу и концу дуги, вычисленными в локальной системе координат.
             \en Parameters are calculated subject to the directions and angles 
      corresponding to the beginning and the end of the arc calculated in the local coordinate system. \~
    \param[in] a1 - \ru Угол, соответствующий началу дуги.
                    \en The angle corresponding to the beginning of the arc. \~
    \param[in] a2 - \ru Угол, соответствующий концу дуги.
                    \en The angle corresponding to the end of the arc. \~
    \param[in] initSense - \ru Направление. initSense > 0 - движение против часовой стрелки, initSense < 0 - по часовой стрелке.
      initSense не должно быть равным нулю.
                           \en Direction. initSense > 0 - counterclockwise, initSense < 0 - clockwise.
      initSense can't be equal to zero. \~
    \param[in] xyEps - \ru Компонента xyEps.x соответствует точности по оси x, xyEps.y - по оси y.
                       \en The xyEps.x component corresponds to x-axis accuracy, xyEps.y corresponds to y-axis accuracy. \~
    \return \ru Значение угла.
            \en Value of angle. \~
  */
  void        InitByPositionAngles( double a1, double a2, int initSense, const MbDimAccuracy & xyEps = MbDimAccuracy::twoDimRgn ); // \ru Инициализация параметров по значениям углов в локальной системе \en Initialization of the parameters by values of angles in the local system 
  /** \} */
  /** \ru \name Функции для работы с данными.
      \en \name Functions for working with data.
    \{ */
  double      GetR()       const { return a; } ///< \ru Вернуть радиус или длину полуоси вдоль X для эллипса. \en Return the radius and the length of semiaxis along X for the ellipse 
  double      GetRadiusA() const { return a; } ///< \ru Вернуть длину полуоси вдоль X. \en Return the length of semiaxis along X. 
  double      GetRadiusB() const { return b; } ///< \ru Вернуть длину полуоси вдоль Y. \en Return the length of semiaxis along Y. 
  void        SetRadiusA( double aa ) { a = aa; Refresh(); }  ///< \ru Установить длину полуоси вдоль X. \en Set the length of semiaxis along X. 
  void        SetRadiusB( double bb ) { b = bb; Refresh(); }  ///< \ru Установить длину полуоси вдоль Y. \en Set the length of semiaxis along Y. 
  double      GetAngle() const { return (trim2 - trim1); } ///< \ru Вернуть угол раствора дуги. \en Return the arc opening angle. 
  /// \ru Установить угол раствора дуги. Начальная точка дуги остается неизменной. \en Set the arc opening angle. The start point of the arc remains unchanged. 
  void        SetAngle ( double ang ) { InitByPositionAngles( trim1, sense ? (trim1+ang) : (trim1-ang), sense ); } 
          
  /// \ru Вычислить угол между осями OX локальной и глобальной системой координат. \en Calculate the angle between OX axes of the local and the global coordinate systems. 
  double      GetMajorAxisAngle() const { return position.GetAxisX().DirectionAngle(); }

  double      GetTrim1() const { return trim1; } ///< \ru Вернуть параметр начальной точки. \en Return the parameter of the start point. 
  double      GetTrim2() const { return trim2; } ///< \ru Вернуть параметр конечной точки. \en Return the parameter of the end point. 
  int         GetSense() const { return trim2 > trim1 ? 1 : -1; } ///< \ru Определить флаг совпадения направления с направлением базовой кривой. \en Determine the flag of coincidence of the direction with the base curve direction. 
  void        SetTrim1( double t ) { trim1 = t; InitByPositionAngles( trim1, trim2, sense ); } ///< \ru Установить параметр начальной точки. \en Set the parameter of the start point. 
  void        SetTrim2( double t ) { trim2 = t; InitByPositionAngles( trim1, trim2, sense ); } ///< \ru Установить параметр конечной точки. \en Set the parameter of the end point. 

  /// \ru Установить радиус дуги окружности. \en Set the radius of the circular arc. 
  void        SetRadius( double rad ) { 
                a = rad;
                b = rad;
                Refresh();
              }
  /// \ru Установить центр. \en Set the center. 
  void        SetCentre( const MbCartPoint & c ) { 
                position.SetOrigin( c );  // \ru Установить центр \en Set the center 
                Refresh();
              }
  /// \ru Установить направление дуги. \en Set the arc orientation. 
  void        SetDirection( bool clockwise ) { 
                int newSense = clockwise ? - 1 : + 1;
                if ( newSense != GetSense() ) {
                  InitByPositionAngles( trim1, trim2, newSense );
                }
              }
  /// \ru Инициализировать дуги эллипса заданной дугой. \en Initialize elliptical arcs with the given arc. 
  void        Init( const MbArc & );
  /// \ru Инициализировать окружность по центру и радиусу \en Initialize a circle by the center and the radius  
  void        Init( const MbCartPoint & pc, double rad );
  /// \ru Инициализировать дугу по начальному и конечному параметрам. \en Initialize arc by parameters for begin point and end point. 
  void        Init( double t1, double t2 ); 

  /** \brief \ru Инициализировать дугу окружности.
             \en Initialize a circular arc. \~
    \details \ru Инициализировать дугу окружности, проходящую через все 3 заданные точки.
      Точки p1 и p3 - крайние. Если дуга не замкнута, то направление движения по дуге определяется так, 
      чтобы точка p2 лежала на дуге.
             \en Initialize a circular arc passing through all 3 given points.
      Points p1 and p3 are the end points. If the arc is not closed, then the direction of moving along the arc is defined so as 
      the point p2 lies on the arc. \~ 
    \param[in] p1 - \ru Начало дуги.
                    \en Beginning of the arc. \~
    \param[in] p2 - \ru Точка, лежащая на дуге.
                    \en A point on the arc. \~
    \param[in] p3 - \ru Конец дуги.
                    \en End of the arc. \~
    \param[in] cl - \ru Признак замкнутости.
                    \en Closedness attribute. \~
    \result \ru True, если инициализация завершилась успешно.
            \en True if initialization completed successfully. \~
  */
  bool        Init3Points( const MbCartPoint & p1, const MbCartPoint & p2, const MbCartPoint & p3, bool cl );

  /** \brief \ru Инициализировать дугу окружности.
             \en Initialize a circular arc. \~
    \details \ru Инициализировать дугу окружности, проходящую через все 3 заданные точки.
      Точки p1 и p3 - крайние. Направление движения по дуге определяется так, 
      чтобы точка p2 лежала на дуге.
             \en Initialize a circular arc passing through all 3 given points.
      Points p1 and p3 are the end points. The direction of moving along the arc is defined so as 
      the point p2 lies on the arc. \~ 
    \param[in] p1 - \ru Начало дуги.
                    \en Beginning of the arc. \~
    \param[in] p2 - \ru Точка, лежащая на дуге.
                    \en A point on the arc. \~
    \param[in] p3 - \ru Конец дуги.
                    \en End of the arc. \~
  */
  void        InitCircle( const MbCartPoint & p1, const MbCartPoint & p2, const MbCartPoint & p3 ); 

  /** \brief \ru Инициализировать дугу окружности.
             \en Initialize a circular arc. \~
    \details \ru Задается новое положение центра дуги, центр дуги будет расположен на биссектрисе угла раствора.
             \en A new position of the arc center is specified, the arc center will be located on the bisector of the opening angle. \~ 
    \param[in] pc - \ru Центр окружности.
                    \en Center of circle. \~
    \param[in] p1 - \ru Начало дуги.
                    \en Beginning of the arc. \~
    \param[in] p2 - \ru Конец дуги.
                    \en End of the arc. \~
  */
  void        InitArc( MbCartPoint & pc, const MbCartPoint & p1, const MbCartPoint & p2 ); 

  /** \brief \ru Инициализировать дугу окружности.
             \en Initialize a circular arc. \~
    \details \ru Изменяется центр и радиус окружности.
      Точки p1 и p2 определяют границы дуги. 
      Начальная точка дуги лежит на луче, выходящем из центра окружности и проходящем через точку p1.
      Конечная точка - на луче, проходящем через точку p2. 
      Параметр initSense определяет направление дуги. Если initSense > 0, то направление движения против часовой стрелки.
             \en The center and the radius of the circle is being modified.
      Points 'p1' and 'p2' specify the bounds of arc. 
      The start point of the arc lies on the ray starting from the circle center and passing through point 'p1'.
      The end point is on the ray passing through the point 'p2'. 
      Parameter 'initSense' specifies the arc direction. If initSense > 0, then the orientation is counterclockwise. \~
    \param[in] pc - \ru Центр окружности.
                    \en Center of circle. \~
    \param[in] rad - \ru Радиус.
                     \en Radius. \~
    \param[in] p1 - \ru Точка, определяющая начало дуги.
                    \en A point specifying the beginning of the arc. \~
    \param[in] p2 - \ru Точка, определяющая конец дуги.
                    \en A point specifying the end of the arc. \~
    \param[in] clockwise - \ru Направление. clockwise > 0 - движение против часовой стрелки, clockwise < 0 - по часовой стрелке.
      clockwise не должно быть равным нулю.
                           \en Direction. clockwise > 0 - moving counterclockwise, clockwise < 0 - clockwise.
      'clockwise' can't be equal to zero. \~
  */
  void        Init( const MbCartPoint & pc, double rad,
                            const MbCartPoint & p1, const MbCartPoint & p2, bool clockwise );

          // \ru Инициализация по центру и точке на дуге ( 360 градусов ) \en Initialization by the center and a point on the arc (360 degrees) 
  /** \brief \ru Инициализировать окружность.
             \en Initialize a circle. \~
    \details \ru Исходный объект изменяется на окружность с центром в точке pc. 
      Радиус определяется как расстояние между точками pc и p .
             \en The source object is changed to a circle with center in point 'pc'. 
      The radius is determined as a distance between points pc and p. \~
    \param[in] pc - \ru Центр окружности.
                    \en Center of circle. \~
    \param[in] p - \ru Точка на окружности.
                   \en Point on circle. \~
  */
  void        Init( const MbCartPoint & pc, const MbCartPoint & p );

          // \ru Первая точка, угол, радиус ( 360 градусов ) \en The first point, angle and radius (360 degrees) 
  /** \brief \ru Инициализировать окружность.
             \en Initialize a circle. \~
    \details \ru Исходный объект изменяется на окружность, проходящую через точку p1 с заданным радиусом.
      Угол angle определяет прямую, на которой лежит центр окружности. Это угол между лучем, 
      выходящим из точки p1 в сторону центра окружности и осью OX.
             \en The source object is changed to a circle passing through point p1 with the given radius.
      Angle 'angle' specifies a line the circle's center lies on. It is the angle between a ray 
      starting from point p1 and directed to the circle's center and OX axis. \~     
    \param[in] p1 - \ru Точка на окружности.
                    \en Point on circle. \~
    \param[in] angle - \ru Угол, определяющий положение центра окружности.
                       \en An angle specifying the position of the center on the circle. \~
    \param[in] rad - \ru Радиус.
                     \en Radius. \~
  */
  void        Init( const MbCartPoint & p1, double angle, double rad );

          // \ru центр, точка на окружности, начальный угол ( 360 градусов ) \en Center, a point on the circle, initial angle (360 degrees) 
  /** \brief \ru Инициализировать окружность.
             \en Initialize a circle. \~
    \details \ru Исходный объект изменяется на окружность с заданным центром, проходящую через точку pnt.
      Радиус определяется как расстояние между точками pc и pnt.
             \en The source object is changed to a circle with the given center and passing through point pnt.
      The radius is determined as the distance between points pc and pnt. \~
    \param[in] pc - \ru Центр окружности.
                    \en Center of circle. \~
    \param[in] pnt - \ru Точка на окружности.
                     \en Point on circle. \~
    \param[in] angle - \ru Начальный параметр.
                       \en Get the start parameter. \~
  */
  void        Init( const MbCartPoint & pc, const MbCartPoint & pnt, double angle );

          // \ru Центр, угол первой точки, угол второй точки, радиус, направление \en Centre, angle of the first point, angle of the second point, radius, direction 
  /** \brief \ru Инициализировать дугу окружность.
             \en Initialize a circular arc. \~
    \details \ru Исходный объект изменяется на дугу окружности с заданным центром и радиусом.
      angle1 и angle2 определяют начальный и конечный углы дуги. Углы отсчитываются от оси OX против часовой стрелки. 
      Углы заданы в радианах.
      Параметр clockwise определяет направление дуги. Если clockwise > 0, то направление движения против часовой стрелки.
             \en The source object is changed to a circular arc with the given center and radius.
      angle1 and angle2 specify the start and the end angles of the arc. The angles are measured from the OX axis counterclockwise. 
      The angles are given in radians.
      Parameter 'clockwise' specifies the direction of the arc. If clockwise > 0, then the direction of moving is counterclockwise. \~
    \param[in] pc - \ru Центр окружности.
                    \en Center of circle. \~
    \param[in] angle1 - \ru Угол, определяющий начало дуги.
                        \en An angle specifying the beginning of the arc. \~
    \param[in] angle2 - \ru Угол, определяющий конец дуги.
                        \en An angle specifying the end of the arc. \~
    \param[in] rad - \ru Радиус.
                     \en Radius. \~
    \param[in] clockwise - \ru Направление. clockwise > 0 - движение против часовой стрелки, clockwise < 0 - по часовой стрелке.
      clockwise не должно быть равным нулю.
                           \en Direction. clockwise > 0 - moving counterclockwise, clockwise < 0 - clockwise.
      'clockwise' can't be equal to zero. \~
  */
  void        Init( const MbCartPoint & pc, double angle1, double angle2, double rad, bool clockwise );

          // \ru центр, точка, номер точки, угол другой точки, направление \en Center, point, index of point, angle of another point, direction 
  /** \brief \ru Инициализировать дугу окружность.
             \en Initialize a circular arc. \~
    \details \ru Исходный объект изменяется на дугу окружности с заданным  и проходящую через точку pnt. 
      Радиус определяется как расстояние между точками pc и pnt.
      Параметр clockwise определяет направление дуги. Если clockwise > 0, то направление движения против часовой стрелки.
             \en The source object is changed to a circular arc with the given center and passing through point pnt. 
      The radius is determined as the distance between points pc and pnt.
      Parameter 'clockwise' specifies the direction of the arc. If clockwise > 0, then the direction of moving is counterclockwise. \~
    \param[in] pc - \ru Центр окружности.
                    \en Center of circle. \~
    \param[in] pnt - \ru Крайняя точка дуги окружности.
                     \en End point of the circular arc. \~
    \param[in] firstPoint - \ru Флаг, определяющий, является точка pnt начальной.
                            \en Flag determining whether the point pnt is the start point. \~
    \param[in] angle - \ru Угол между радиусом, идущим ко второй точки и осью OX.
                       \en The angle between the radius to the second point and the OX axis. \~
    \param[in] clockwise - \ru Направление. clockwise > 0 - движение против часовой стрелки, clockwise < 0 - по часовой стрелке.
      clockwise не должно быть равным нулю.
                           \en Direction. clockwise > 0 - moving counterclockwise, clockwise < 0 - clockwise.
      'clockwise' can't be equal to zero. \~
  */
  void        Init( const MbCartPoint & pc, const MbCartPoint & pnt, bool firstPoint, double angle, bool clockwise );

          // \ru центр, угол первой точки, вторая точка, радиус, направление \en Center, angle of the first point, the second point, radius, direction 
  /** \brief \ru Инициализировать дугу окружность.
             \en Initialize a circular arc. \~
    \details \ru Исходный объект изменяется на дугу окружности с заданным центром и радиусом. 
      Конечная точка дуги определяется как пересечение луча (pc, p2) и окружности.
      Параметр clockwise определяет направление дуги. Если clockwise > 0, то направление движения против часовой стрелки.
             \en The source object is changed to a circular arc with the given center and radius. 
      The end point of the arc is determined as intersection of ray (pc, p2) and the circle.
      Parameter 'clockwise' specifies the direction of the arc. If clockwise > 0, then the direction of moving is counterclockwise. \~
    \param[in] pc - \ru Центр окружности.
                    \en Center of circle. \~
    \param[in] angle1 - \ru Начальный параметр дуги.
                        \en The start parameter of the circle. \~
    \param[in] p2 - \ru Точка, определяющая угол конца дуги окружности.
                    \en A point specifying the angle of the circular arc end. \~
    \param[in] rad - \ru Радиус.
                     \en Radius. \~
    \param[in] clockwise - \ru Направление. clockwise > 0 - движение против часовой стрелки, clockwise < 0 - по часовой стрелке.
      initSense не должно быть равным нулю.
                           \en Direction. clockwise > 0 - moving counterclockwise, clockwise < 0 - clockwise.
      initSense can't be equal to zero. \~
  */
  void        Init( const MbCartPoint & pc, double angle1, const MbCartPoint & p2, double rad, bool clockwise );

          // \ru Окружность, первая точка, вторая точка, направление \en Circle, the first point, the second point, direction 
  /** \brief \ru Инициализировать дугу окружность.
             \en Initialize a circular arc. \~
    \details \ru Исходный объект изменяется на дугу окружности, соответствующей заданному объекту. 
      Начальная и конечная точки дуги определяются как пересечения лучей, направленных из центра к точкам p1 и p2 c окружностью.
      Конечная точка дуги определяется как пересечение луча (pc, p2) и окружности.
      Параметр initSense определяет направление дуги. Если initSense > 0, то направление движения против часовой стрелки.
             \en The source object is changed to a circular arc corresponding to a given object. 
      The start and the end point of the arc is determined as intersection of rays passing from the center to points p1 and p2 with the circle.
      The end point of the arc is determined as intersection of ray (pc, p2) and the circle.
      Parameter 'initSense' specifies the arc direction. If initSense > 0, then the orientation is counterclockwise. \~
    \param[in] obj - \ru Объект-образец.
                     \en A pattern object. \~
    \param[in] p1 - \ru Точка, определяющая угол начала дуги окружности.
                    \en A point determining the angle of the beginning of the circular arc. \~
    \param[in] p2 - \ru Точка, определяющая угол конца дуги окружности.
                    \en A point specifying the angle of the circular arc end. \~
    \param[in] initSense - \ru Направление. initSense > 0 - движение против часовой стрелки, initSense < 0 - по часовой стрелке.
      initSense не должно быть равным нулю.
                           \en Direction. initSense > 0 - counterclockwise, initSense < 0 - clockwise.
      initSense can't be equal to zero. \~
  */
  void        Init( MbArc * obj, const MbCartPoint & p1, const MbCartPoint & p2, int initSense );

          // \ru Первая точка, вторая точка, угол, номер угла, направление \en The first point, the second point, angle, index of angle, direction 
  /** \brief \ru Инициализировать дугу окружность.
             \en Initialize a circular arc. \~
    \details \ru В результате операции получаем дугу окружности, которая начинается в точке p1 и заканчивается в точке p2.
      Для одной из точек задан угол между направлением от нее до центра окружности и осью OX. 
      Параметр firstAngle определяет, для какой точке задан угол.
      Параметр clockwise определяет направление дуги. Если clockwise > 0, то направление движения против часовой стрелки.
             \en In the result of the operation the circular arc is obtained which starts at point p1 and ends at point p2.
      For one of points the angle between the direction from the point to the circle center and the OX axis is specified. 
      Parameter firstAngle determines for which of points the angle is specified.
      Parameter 'clockwise' specifies the direction of the arc. If clockwise > 0, then the direction of moving is counterclockwise. \~
    \param[in] p1 - \ru Начальная точка дуги окружности.
                    \en The start point of the circular arc. \~
    \param[in] p2 - \ru Конечная точка дуги окружности.
                    \en The end point of the circular arc. \~
    \param[in] angle - \ru Угол между направлением из точки на центр окружности осью OX.
                       \en The angle between the direction from the point to the circle center and OX axis. \~
    \param[in] firstAngle - \ru Флаг, определяющий для какой точки задан угол. firstAngle == true - для первой.
                            \en Flag determining for which point the angle is specified. firstAngle == true - for the first one. \~
    \param[in] clockwise - \ru Направление. clockwise > 0 - движение против часовой стрелки, clockwise < 0 - по часовой стрелке.
      clockwise не должно быть равным нулю.
                           \en Direction. clockwise > 0 - moving counterclockwise, clockwise < 0 - clockwise.
      'clockwise' can't be equal to zero. \~
  */
  void        Init( const MbCartPoint & p1, const MbCartPoint & p2, double angle, bool firstAngle, bool clockwise );

  // \ru Плавающий центр, угол первой точки, угол второй точки, точка, номер точки, направление \en Variable center, angle of the first point, angle of the second point, index of the point, direction 
  /** \brief \ru Инициализировать дугу окружность.
             \en Initialize a circular arc. \~
    \details \ru В результате операции получаем дугу окружности, одним из концов которой является точка pnt. 
      Для этой точки задан угол между прямой - направлением от точки к центру окружности и осью OX. 
      Для определения центра окружности находим проекцию pc на эту прямую.  
      Второй угол определяет второй конец дуги.
      Параметр clockwise определяет направление дуги. Если clockwise > 0, то направление движения против часовой стрелки.
             \en In the result of the operation a circular arc is obtained one of ends of which is point pnt. 
      For this point the angle between the line directed from the point to the circle center and OX axis is specified. 
      Find projection pc onto this line to determine the circle center.  
      The second angle specifies the second end of the arc.
      Parameter 'clockwise' specifies the direction of the arc. If clockwise > 0, then the direction of moving is counterclockwise. \~
    \param[in, out] pc - \ru На входе - заданная точка, на выходе - центр окружности.
                         \en On input - the given point, on output - the circle center. \~
    \param[in] angle1 - \ru Угол между направлением от начальной точки дуги на центр окружности и осью OX.
                        \en Angle between the direction from the starting point of the arc to the circle center and OX axis. \~
    \param[in] angle2 - \ru Угол между направлением от конечной точки дуги на центр окружности и осью OX.
                        \en Angle between the direction from the end point of the arc to the circle center and OX axis. \~
    \param[in] pnt - \ru Крайняя точка дуги окружности.
                     \en End point of the circular arc. \~
    \param[in] firstPoint - \ru Флаг, определяющий, является ли точка pnt начальной.
                            \en Flag determining whether the point pnt is a starting point. \~
    \param[in] clockwise - \ru Направление. clockwise > 0 - движение против часовой стрелки, clockwise < 0 - по часовой стрелке.
      clockwise не должно быть равным нулю.
                           \en Direction. clockwise > 0 - moving counterclockwise, clockwise < 0 - clockwise.
      'clockwise' can't be equal to zero. \~
  */
  void        Init( MbCartPoint & pc, double angle1, double angle2,
                    const MbCartPoint & pnt, bool firstPoint, bool clockwise );

  // \ru Плавающий центр, точка, номер точки, угол противоположной точки, радиус, направление \en Variable center, point, index of the point, angle of the opposite point, radius, direction 
  /** \brief \ru Инициализировать дугу окружность.
             \en Initialize a circular arc. \~
    \details \ru Исходный объект изменяется на дугу окружности с заданным радиусом и проходящую через точку p. 
      Надо определить центр окружности так, чтобы он был как можно ближе к заданной точке pc.
      Параметр clockwise определяет направление дуги. Если clockwise > 0, то направление движения против часовой стрелки.
             \en The source object is changed to the circular arc with the given radius and passing through point p. 
      The circle center should be defined so as it is as close to the given point pc as possible.
      Parameter 'clockwise' specifies the direction of the arc. If clockwise > 0, then the direction of moving is counterclockwise. \~
    \param[in, out] pc - \ru На входе - заданная точка, на выходе - центр окружности.
                         \en On input - the given point, on output - the circle center. \~
    \param[in] p - \ru Крайняя точка дуги окружности.
                   \en End point of the circular arc. \~
    \param[in] firstPoint - \ru Флаг, определяющий, является ли точка pnt начальной.
                            \en Flag determining whether the point pnt is a starting point. \~
    \param[in] angle - \ru Угол между радиусом, идущим ко второй точки и осью OX.
                       \en The angle between the radius to the second point and the OX axis. \~
    \param[in] rad - \ru Радиус.
                     \en Radius. \~
    \param[in] clockwise - \ru Направление. clockwise > 0 - движение против часовой стрелки, clockwise < 0 - по часовой стрелке.
      clockwise не должно быть равным нулю.
                           \en Direction. clockwise > 0 - moving counterclockwise, clockwise < 0 - clockwise.
      'clockwise' can't be equal to zero. \~
  */
  void        Init( MbCartPoint & pc, const MbCartPoint & p, bool firstPoint,
                    double angle, double rad, bool clockwise );

  /** \brief \ru Инициализировать дугу окружность.
             \en Initialize a circular arc. \~
    \details \ru В результате операции получаем дугу окружности с центром в точке pc. 
      Радиус определяется как расстояние между точками pc и p1. 
      Направления от центра к точкам p1 и p2 задают углы, определяющие начало и конец дуги.
      Параметр clockwise определяет направление дуги. Если clockwise > 0, то направление движения против часовой стрелки.
             \en In the result of the operation the circular arc is obtained with the center in point pc. 
      The radius is determined as the distance between points 'pc' and 'p1'. 
      Directions from the center to points p1 and p2 specify the angles determining the start and the end of the circle.
      Parameter 'clockwise' specifies the direction of the arc. If clockwise > 0, then the direction of moving is counterclockwise. \~
    \param[in] pc - \ru Центр окружности.
                    \en Center of circle. \~
    \param[in] p1 - \ru Точка, определяющая направление на начало дуги окружности.
                    \en A point specifying the direction to the beginning of the circular arc. \~
    \param[in] p2 - \ru Точка, определяющая направление на конец дуги окружности.
                    \en A point specifying the direction to the end of the circular arc. \~
    \param[in] initSense - \ru Направление. initSense > 0 - движение против часовой стрелки, initSense < 0 - по часовой стрелке.
      initSense не должно быть равным нулю.
                           \en Direction. initSense > 0 - counterclockwise, initSense < 0 - clockwise.
      initSense can't be equal to zero. \~
  */
  void        Init( const MbCartPoint & pc, const MbCartPoint & p1, const MbCartPoint & p2, int initSense );

  // \ru Инициализация по начальной и конечной точкам и 1/2 угла раствора дуги \en Initialization by the starting and end points and 1/2 of the arc opening angle 
  // \ru Если diskrData != nullptr, то округлить радиус и скорректировать первую \en If diskrData != nullptr, then round the radius and correct the first
  // \ru Или вторую точку (зависит от correctFirstPnt) \en Or the second point (depends on correctFirstPnt) 
  /** \brief \ru Инициализировать дугу окружность.
             \en Initialize a circular arc. \~
    \details \ru Инициализация происходит по начальной и конечной точкам и 1/2 угла раствора дуги. 
      Если diskrData != nullptr, радиус округляется и корректируется первая
      или вторая точка (зависит от correctFirstPnt).
             \en The initialization is performed by the starting and end points and 1/2 of the arc opening angle. 
      If diskrData != nullptr, the radius is rounded and the first
      or the second point is corrected (depends on correctFirstPnt). \~
    \param[in] a2 - \ru 1/2 угла раствора дуги окружности.
                    \en 1/2 of the circular arc opening angle. \~
    \param[in, out] p1 - \ru Начальная точка дуги. Может быть скорректирована после округления радиуса.
                         \en The starting point of the arc. Can be corrected after rounding the radius. \~
    \param[in, out] p2 - \ru Конечная точка дуги. Может быть скорректирована после округления радиуса.
                         \en The end point of the arc. Can be corrected after rounding the radius. \~
    \param[in] diskrData - \ru Структура для округления радиуса.
                           \en The structure for rounding the radius. \~
    \param[in] correctFirstPnt - \ru Определяет, какую точку корректировать после округления. 
      correctFirstPnt == true - корректируется первая точка.
                                 \en Determines which point to be corrected after the rounding. 
      correctFirstPnt == true - the first point is to be corrected. \~
    \result \ru True, если инициализация завершилась успешно.
            \en True if initialization completed successfully. \~
  */
  bool        Init( double a2, MbCartPoint & p1, MbCartPoint & p2, 
                    const DiskreteLengthData * diskrData = nullptr,
                    bool correctFirstPnt = true );

  // \ru Инициализация эллипса \en Ellipse initialization 
  /** \brief \ru Инициализировать эллипс.
             \en Initialize an ellipse. \~
    \details \ru В результате операции получаем эллипс с заданными локальной системой координат и полуосями.
             \en In the result of the operation the ellipse is obtained with the given local coordinate system and semiaxes. \~
    \param[in] aa - \ru Радиус полуоси вдоль X.
                    \en Radius of semiaxis along X. \~
    \param[in] bb - \ru Радиус полуоси вдоль Y.
                    \en Radius of semiaxis along Y. \~
    \param[in] place - \ru Локальная система координат эллипса.
                       \en The local coordinate system of the ellipse. \~
  */
  void        Init( double aa, double bb, const MbPlacement & place );

  /** \brief \ru Инициализировать эллипс.
             \en Initialize an ellipse. \~
    \details \ru В результате операции получаем эллипс с заданными полуосями.
      Локальная система координат эллипса имеет начало в точке pc и 
      ось OX локальной системы координат составляет с осью OX текущей системы координат угол ang.
      Направление поворота - от оси текущей системы координат к оси новой системы координат.
             \en In the result of the operation the ellipse is obtained with the specified semiaxes.
      The ellipse local coordinate system has origin in point pc; 
      OX axis of the local coordinate system forms angle ang with OX axis of the current coordinate system.
      The turning direction - from the axis of the current coordinate system to the axis of the new coordinate system. \~
    \param[in] aa - \ru Радиус полуоси вдоль X.
                    \en Radius of semiaxis along X. \~
    \param[in] bb - \ru Радиус полуоси вдоль Y.
                    \en Radius of semiaxis along Y. \~
    \param[in] pc - \ru Начало локальной системы координат эллипса.
                    \en Origin of local coordinate system of ellipse. \~
    \param[in] ang - \ru Угол между осями OX локальной и текущей системами координат.
                     \en An angle between OX axes of the local and the current coordinate systems. \~
  */
  void        Init( double aa, double bb, const MbCartPoint & pc, double ang );

          // \ru Различные варианты построения эллипса \en Different variants of ellipse construction 
  /** \brief \ru Инициализировать эллипс.
             \en Initialize an ellipse. \~
    \details \ru В результате операции получаем эллипс с центром в точке c.
      Длина полуоси эллипса, идущая вдоль оси X, определяется как расстояние между точками c и p1.
      Длина второй полуоси 0.
      Определяются угол между осями OX локальной системы координат эллипса и текущей системы координат.
             \en In the result of the operation an ellipse is obtained with center in point c.
      The length of ellipse semiaxis along X-axis is determined as the distance between points c and p1.
      The length of the second semiaxis is 0.
      The angle between OX axes of the ellipse's local coordinate system and the current coordinate system. \~
    \param[in] c - \ru Центр эллипса.
                   \en The ellipse center. \~
    \param[in] p1 - \ru Точка, лежащая на эллипсе.
                    \en A point on ellipse. \~
    \param[out] len - \ru Длина полуоси вдоль X.
                      \en The length of semiaxis along X. \~
    \param[out] angle - \ru Угол между осями OX локальной и текущей системами координат.
                        \en An angle between OX axes of the local and the current coordinate systems. \~
  */
  void        Init1( const MbCartPoint & c, const MbCartPoint & p1,
                     double & len, double & angle );

  /** \brief \ru Инициализировать эллипс.
             \en Initialize an ellipse. \~
    \details \ru В результате операции получаем эллипс с центром в точке c.
      Точка p1 определяет направление оси OX локальной системы координат эллипса и длину полуоси вдоль X.
      Точка p2 - длину полуоси вдоль Y, как расстояние от точки до оси OX локальной системы координат.
      Точка p2 изменяется так, чтобы она лежала на пересечении эллипса с осью OY локальной системы координат.
             \en In the result of the operation an ellipse is obtained with center in point c.
      Point p1 determines the direction of OX axis of ellipse's local coordinate system and the length of semiaxis along X.
      Point p2 determines the length of semiaxis along Y as the distance from the point to the OX axis of the local coordinate system.
      Point p2 is changed so as it lies on intersection of the ellipse with OY axis of the local coordinate system. \~
    \param[in] c - \ru Центр эллипса.
                   \en The ellipse center. \~
    \param[in] p1 - \ru Точка, лежащая на эллипсе, определяет ось OX.
                    \en A point on ellipse specifies OX axis. \~
    \param[in, out] p2 - \ru Определяет длину полуоси вдоль Y. На выходе - точка, лежащая на пересечении эллипса с осью OY локальной системы координат.
                         \en Specifies the length of semiaxis along Y. On output - point on intersection of ellipse with OY axis of the local coordinate system. \~
    \param[out] lenB - \ru Длина полуоси вдоль Y.
                       \en The length of semiaxis along Y. \~
  */
  void        Init2( const MbCartPoint & c, const MbCartPoint & p1,
                     MbCartPoint & p2, double & lenB );
  /** \brief \ru Инициализировать эллипс.
             \en Initialize an ellipse. \~
  \details \ru В результате операции получаем эллипс, вписанный в повернутый прямоугольник,
    заданный точкой центра c, вершиной p1 и углом наклона angle.
    Ось OX локальной системы координат эллипса будет направлена в соответствии с углом angle.
           \en In the result of the operation an ellipse is obtained inscribed into the rotated rectangle
    specified by the point of center c, the vertex p1 and the slope angle 'angle'.
    OX-axis of the local coordinate system of the ellipse will be oriented according to the angle 'angle'. \~
    \param[in] c0 - \ru Центр прямоугольника.
                    \en The center of the rectangle. \~
    \param[in] p1 - \ru Вершина прямоугольника.
                    \en The rectangle vertex. \~
    \param[in] angle - \ru Угол наклона прямоугольника.
                       \en Slope angle of the rectangle. \~
    \param[out] aa - \ru Длина полуоси вдоль X.
                     \en The length of semiaxis along X. \~
    \param[out] bb - \ru Длина полуоси вдоль Y.
                     \en The length of semiaxis along Y. \~
  */
  void        Init3( const MbCartPoint & c0, const MbCartPoint & p1,
                     double angle, double & aa, double & bb ); 

  /** \brief \ru Инициализировать эллипс.
             \en Initialize an ellipse. \~
  \details \ru В результате операции получаем эллипс, вписанный в повернутый прямоугольник,
    заданный двумя диагональными точками p1, p2 и углом наклона angle.
    Ось OX локальной системы координат эллипса будет направлена в соответствии с углом angle.
           \en In the result of the operation an ellipse is obtained inscribed into the rotated rectangle
    specified by two diagonal points p1 and p2 and the slope angle 'angle'.
    OX-axis of the local coordinate system of the ellipse will be oriented according to the angle 'angle'. \~
    \param[in] p1 - \ru Вершина прямоугольника.
                    \en The rectangle vertex. \~
    \param[in] p2 - \ru Вершина прямоугольника.
                    \en The rectangle vertex. \~
    \param[in] angle - \ru Угол наклона прямоугольника.
                       \en Slope angle of the rectangle. \~
    \param[out] aa - \ru Длина полуоси вдоль X.
                     \en The length of semiaxis along X. \~
    \param[out] bb - \ru Длина полуоси вдоль Y.
                     \en The length of semiaxis along Y. \~
  */
  void        Init4( const MbCartPoint & p1, const MbCartPoint & p2,
                     double angle, double & aa, double & bb );

  /** \brief \ru Инициализировать эллипс.
             \en Initialize an ellipse. \~
    \details \ru В результате операции получаем эллипс, вписанный в параллелограмм,
      заданный тремя точками: центром параллелограмма (c), серединой одной из его сторон (p1)
      и одной вершин этой стороны (p2).
      Ось OX локальной системы координат эллипса будет проходить через точку p2.
             \en In the result of the operation an ellipse is obtained inscribed into the parallelogram
      given by three points: center of parallelogram (c), middle of one of its sides (p1)
      and one of vertices of this side (p2).
      OX-axis of the local coordinate system of the ellipse will pass through the point p2. \~
    \param[in] c - \ru Центр параллелограмма.
                   \en The center of parallelogram. \~
    \param[in] p1 - \ru Середина стороны параллелограмма.
                    \en The middle of a side of the parallelogram. \~
    \param[in] p2 - \ru Вершина параллелограмма.
                    \en A vertex of the parallelogram. \~
    \param[out] aa - \ru Длина полуоси вдоль X.
                     \en The length of semiaxis along X. \~
    \param[out] bb - \ru Длина полуоси вдоль Y.
                     \en The length of semiaxis along Y. \~
    \param[out] angle - \ru Угол между осями OX локальной и текущей системами координат.
                        \en An angle between OX axes of the local and the current coordinate systems. \~
  */
  void        Init5( const MbCartPoint & c, const MbCartPoint & p1, const MbCartPoint & p2,
                     double & aa, double & bb, double & angle );

  /** \brief \ru Инициализировать эллипс.
             \en Initialize an ellipse. \~
    \details \ru В результате операции получаем эллипс, вписанный в параллелограмм,
      заданный тремя вершинами.
      Ось OX локальной системы координат эллипса будет параллельна отрезку [p1 p2].
             \en In the result of the operation an ellipse is obtained inscribed into the parallelogram
      given by three vertices.
      OX-axis of the local coordinate system of the ellipse will be parallel to the segment [p1 p2]. \~
    \param[in] p1 - \ru Вершина параллелограмма.
                    \en A vertex of the parallelogram. \~
    \param[in] p2 - \ru Вершина параллелограмма.
                    \en A vertex of the parallelogram. \~
    \param[in] p3 - \ru Вершина параллелограмма.
                    \en A vertex of the parallelogram. \~
    \param[out] aa - \ru Длина полуоси вдоль X.
                     \en The length of semiaxis along X. \~
    \param[out] bb - \ru Длина полуоси вдоль Y.
                     \en The length of semiaxis along Y. \~
    \param[out] angle - \ru Угол между осями OX локальной и текущей системами координат.
                        \en An angle between OX axes of the local and the current coordinate systems. \~
  */
  void        Init6( const MbCartPoint & p1, const MbCartPoint & p2, const MbCartPoint & p3,
                     double & aa, double & bb, double & angle );

  /** \brief \ru Инициализировать эллипс.
             \en Initialize an ellipse. \~
    \details \ru В результате операции получаем эллипс, построенный 
      по центру pc и трем точкам на нем p1, p2, p3
             \en In the result of the operation an ellipse is obtained constructed 
      by the center pc and three points on it p1, p2, p3 \~
    \param[in] pc - \ru Центр эллипса.
                    \en The ellipse center. \~
    \param[in] p1 - \ru Точка на эллипсе.
                    \en A point on ellipse. \~
    \param[in] p2 - \ru Точка на эллипсе.
                    \en A point on ellipse. \~
    \param[in] p3 - \ru Точка на эллипсе.
                    \en A point on ellipse. \~
    \param[out] aa - \ru Длина полуоси вдоль X.
                     \en The length of semiaxis along X. \~
    \param[out] bb - \ru Длина полуоси вдоль Y.
                     \en The length of semiaxis along Y. \~
    \param[out] angle - \ru Угол между осями OX локальной и текущей системами координат.
                        \en An angle between OX axes of the local and the current coordinate systems. \~
  */
  void        Init7( const MbCartPoint & pc,
                     MbCartPoint p1, MbCartPoint p2, MbCartPoint p3,
                     double & aa, double & bb, double & angle );

  /** \brief \ru Инициализировать эллипс.
             \en Initialize an ellipse. \~
    \details \ru Для построения эллипса имеем две точки на эллипсе и касательные в этих точках
      и третью точку, лежащую на эллипсе.
             \en For ellipse construction we have two points on ellipse and tangent lines at these points
      and the third point on the ellipse. \~
    \param[in] p1 - \ru Точка на эллипсе.
                    \en A point on ellipse. \~
    \param[in] dir1 - \ru Направление касательной к эллипсу в точке p1.
                      \en The direction of tangent line to ellipse at point p1. \~
    \param[in] p2 - \ru Точка на эллипсе.
                    \en A point on ellipse. \~
    \param[in] dir2 - \ru Направление касательной к эллипсу в точке p2.
                      \en The direction of tangent line to ellipse at point p2. \~
    \param[in] p3 - \ru Точка на эллипсе.
                    \en A point on ellipse. \~
    \param[out] aa - \ru Длина полуоси вдоль X.
                     \en The length of semiaxis along X. \~
    \param[out] bb - \ru Длина полуоси вдоль Y.
                     \en The length of semiaxis along Y. \~
    \param[out] angle - \ru Угол между осями OX локальной и текущей системами координат.
                        \en An angle between OX axes of the local and the current coordinate systems. \~
  */
  void        Init8( const MbCartPoint & p1, const MbDirection & dir1,
                     const MbCartPoint & p2, const MbDirection & dir2,
                     const MbCartPoint & p3,
                     double & aa, double & bb, double & angle );
          // \ru Различные варианты построения дуги эллипса \en Different variants of elliptical arc construction 
  /** \brief \ru Инициализировать дугу эллипса.
             \en Initialize an elliptical arc. \~
    \details \ru В результате операции получаем дугу эллипса с заданными полуосями и локальной системой координат. 
      t1 и t2 определяют начальный и конечный углы дуги. Углы отсчитываются от оси OX против часовой стрелки. 
      Углы заданы в радианах.
      Параметр initSense определяет направление дуги. Если initSense > 0, то направление движения против часовой стрелки.
             \en In the result of the operation an elliptical arc is obtained with the specified semiaxes and the local coordinate system. 
      t1 and t2 specify the start and the end angles of the arc. The angles are measured from the OX axis counterclockwise. 
      The angles are given in radians.
      Parameter 'initSense' specifies the arc direction. If initSense > 0, then the orientation is counterclockwise. \~
    \param[in] aa - \ru Радиус полуоси вдоль X.
                    \en Radius of semiaxis along X. \~
    \param[in] bb - \ru Радиус полуоси вдоль Y.
                    \en Radius of semiaxis along Y. \~
    \param[in] place - \ru Локальная система координат эллипса.
                       \en The local coordinate system of the ellipse. \~
    \param[in] t1 - \ru Угол, определяющий начало дуги.
                    \en An angle specifying the beginning of the arc. \~
    \param[in] t2 - \ru Угол, определяющий конец дуги.
                    \en An angle specifying the end of the arc. \~
    \param[in] initSense - \ru Направление. initSense > 0 - движение против часовой стрелки, initSense < 0 - по часовой стрелке.
      initSense не должно быть равным нулю.
                           \en Direction. initSense > 0 - counterclockwise, initSense < 0 - clockwise.
      initSense can't be equal to zero. \~
  */
  void        Init( double aa, double bb, const MbPlacement & place,
                    double t1, double t2, int initSense ); 

  /** \brief \ru Инициализировать дугу эллипса.
             \en Initialize an elliptical arc. \~
    \details \ru В результате операции получаем дугу эллипса с заданными полуосями и локальной системой координат. 
      Точки p1 и p2 определяют границы дуги. 
      Начальная точка дуги лежит на луче, выходящем из центра окружности и проходящем через точку p1.
      Конечная точка - на луче, проходящем через точку p2. 
      Параметр clockwise определяет направление дуги. Если clockwise > 0, то направление движения против часовой стрелки.
             \en In the result of the operation an elliptical arc is obtained with the specified semiaxes and the local coordinate system. 
      Points 'p1' and 'p2' specify the bounds of arc. 
      The start point of the arc lies on the ray starting from the circle center and passing through point 'p1'.
      The end point is on the ray passing through the point 'p2'. 
      Parameter 'clockwise' specifies the direction of the arc. If clockwise > 0, then the direction of moving is counterclockwise. \~
    \param[in] aa - \ru Радиус полуоси вдоль X.
                    \en Radius of semiaxis along X. \~
    \param[in] bb - \ru Радиус полуоси вдоль Y.
                    \en Radius of semiaxis along Y. \~
    \param[in] place - \ru Локальная система координат эллипса.
                       \en The local coordinate system of the ellipse. \~
    \param[in] p1 - \ru Точка, определяющая начало дуги.
                    \en A point specifying the beginning of the arc. \~
    \param[in] p2 - \ru Точка, определяющая конец дуги.
                    \en A point specifying the end of the arc. \~
    \param[in] clockwise - \ru Направление. clockwise > 0 - движение против часовой стрелки, clockwise < 0 - по часовой стрелке.
      clockwise не должно быть равным нулю.
                           \en Direction. clockwise > 0 - moving counterclockwise, clockwise < 0 - clockwise.
      'clockwise' can't be equal to zero. \~
  */
  void        Init( double aa, double bb, const MbPlacement & place,
                    const MbCartPoint & p1, const MbCartPoint & p2, bool clockwise ); 

  // \ru Эллипс вписан в прямоугольник, заданный двумя диагональными точками p1, p2, \en Ellipse is inscribed into the rectangle given by two diagonal points p1, p2, 
  // \ru проекции точек pB и pE на эллипс определяют начало и конец дуги, \en The projections of points pB and pE onto ellipse determine the start and the end of the arc, 
  // \ru clockwise определяет движение от начальноц точки к конечной по часовой стрелке или против \en 'clockwise' determines moving from the starting point to the end point clockwise or counterclockwise 
  /** \brief \ru Инициализировать дугу эллипса.
             \en Initialize an elliptical arc. \~
    \details \ru Эллипс вписан в прямоугольник, заданный двумя диагональными точками p1, p2.
      Стороны прямоугольника параллельны осям текущей системы координат.
      Проекции точек pB и pE на эллипс определяют начало и конец дуги.
      clockwise определяет движение от начальноц точки к конечной по часовой стрелке или против.
             \en Ellipse is inscribed into the rectangle given by two diagonal points p1 and p2.
      Sides of the rectangle are parallel to the axes of the current coordinate system.
      The projections of points pB and pE onto ellipse determine the start and the end of the arc.
      'clockwise' determines moving form the start point to the end point clockwise or counterclockwise. \~
    \param[in] p1 - \ru Вершина прямоугольника.
                    \en The rectangle vertex. \~
    \param[in] p2 - \ru Вершина прямоугольника.
                    \en The rectangle vertex. \~
    \param[in] pB - \ru Точка, определяющая начало дуги.
                    \en A point specifying the beginning of the arc. \~
    \param[in] pE - \ru Точка, определяющая конец дуги.
                    \en A point specifying the end of the arc. \~
    \param[in] clockwise - \ru Направление. clockwise > 0 - движение против часовой стрелки, clockwise < 0 - по часовой стрелке.
      clockwise не должно быть равным нулю.
                           \en Direction. clockwise > 0 - moving counterclockwise, clockwise < 0 - clockwise.
      'clockwise' can't be equal to zero. \~
  */
  void        Init4( const MbCartPoint & p1, const MbCartPoint & p2,
                     const MbCartPoint & pB, const MbCartPoint & pE, bool clockwise = false ); 

  bool        OnSector( const MbCartPoint & pnt ) const; ///< \ru Определить, находится ли луч от центра до точки в секторе дуги. \en Determine whether the ray from the center to the point is in the arc's sector. 

  /** \brief \ru Определить попадание в сектор дуги.
             \en Determine whether the ray hits the arc's sector. \~
    \details \ru Анализируется попадание в сектор дуги луча, выходящего из центра
      и имеющего с осью OX текущей системы координат угол angle.
             \en It is analyzed if the ray starting from the center and forming
      the angle 'angle' with OX-axis of the current coordinate system hits the arc's sector. \~
    \param[in] angle - \ru Угол между анализируемым направлением и осью OX текущей системы координат.
                       \en The angle between the direction being analyzed and the OX-axis of the current coordinate system. \~
    \result \ru true, если направление попадает в сектор дуги.
            \en True if the direction hits the arc's sector. \~
  */
  bool        OnSector( double angle ) const; // \ru Находится угол в секторе дуги ? \en Is the angle in the arc's sector? 

  /** \brief \ru Заменить точку дуги.
             \en Replace the arc's point. \~
    \details \ru Происходит построение дуги эллипса по крайним точкам с сохранением угла раствора дуги.
             \en The reconstruction of the elliptical arc by the end points is performed keeping the arc opening angle. \~
    \param[in] number - \ru Номер крайней точки дуги. 1 - начало дуги, 2 - конец дуги.
                        \en The index of end point of the arc. 1 - start of arc, 2 - end of arc. \~
    \param[in] pnt - \ru Новая точка.
                     \en A new point. \~
  */
  void        SetLimitPoint( ptrdiff_t number, const MbCartPoint & pnt ); // \ru Заменить точку дуги \en Replace the arc point 

  /// \ru Вернуть направление дуги: true  - по часовой стрелке; false - против часовой стрелки. \en Return the arc direction: true - clockwise, false - counterclockwise. 
  bool        IsClockwise() const { return ( position.IsLeft() == (GetSense() > 0) ); }

  /** \brief \ru Вернуть угол крайней точки дуги.
             \en Return the angle of the end point. \~
    \details \ru Угол крайней точки дуги считается относительно оси OX текущей системы координат.
             \en The angle of the end point of the arc is measured relative to the OX-axis of the current coordinate system. \~
    \param[in] number - \ru Номер крайней точки дуги. 1 - начало дуги, 2 - конец дуги.
                        \en The index of end point of the arc. 1 - start of arc, 2 - end of arc. \~
    \result \ru Угол между направлением от центра к крайней точке и осью OX текущей системы координат.
            \en The angle between the direction from the center to the end point and OX-axis of the current coordinate system. \~
  */
  double      GetLimitAngle( ptrdiff_t number ) const { 
                double ang = ( number == 1 ) ? trim1 : trim2; 
                if ( position.IsLeft() )
                  ang = -ang;
                ang += GetMajorAxisAngle(); // \ru Угол с осью X \en Angle with X axis 
                c3d::NormalizeAngle( ang);
                return ang;
              }

  /** \brief \ru Изменить граничный угол дуги.
             \en Modify the end angle of the arc. \~
    \param[in] number - \ru Номер крайней точки дуги. 1 - начало дуги, 2 - конец дуги.
                        \en The index of end point of the arc. 1 - start of arc, 2 - end of arc. \~
    \param[in] pnt - \ru Точка, определяющая направление на новый конец дуги.
                     \en The point specifying the direction to the new end of the arc. \~
    \result \ru Угол между направлением от центра к крайней точке и осью OX текущей системы координат.
            \en The angle between the direction from the center to the end point and OX-axis of the current coordinate system. \~
  */
  void        SetLimitAngle( ptrdiff_t number, const MbCartPoint & pnt ) {
                if ( number == 1 )
                  InitByPositionAngles( GetPositionAngle(pnt), trim2, GetSense() );
                else
                  InitByPositionAngles( trim1, GetPositionAngle(pnt), GetSense() );
                Refresh();
              }

  inline  double      CheckParam( double & t ) const; ///< \ru  Установить параметр в область допустимых значений. \en Set the parameter to the range of the allowable values. 
  inline  void        ParamToAngle( double & t ) const; ///< \ru Перевести параметр кривой в угол. \en Convert the parameter of the curve to the angle. 
  inline  void        AngleToParam( double & t ) const; ///< \ru Перевести угол кривой в параметр кривой. \en Convert the curve angle to the curve parameter. 


  void        ParameterInto( double &t ) const { AngleToParam( t ); } ///< \ru Перевести параметр базовой кривой в локальный параметр. \en Convert parameter of the base curve to the local parameter. 
  void        ParameterFrom( double &t ) const { ParamToAngle( t ); } ///< \ru Перевести локальный параметр в параметр базовой кривой. \en Convert the local parameter to the parameter of the base curve. 
  bool        IsBaseParamOn( double t, double eps = Math::paramEpsilon ) const; ///< \ru Определить, находится ли параметр базовой кривой в диапазоне усеченной кривой. \en Determine whether the parameter of the base curve is in range of the trimmed curve. 

          // \ru Работа с базовым эллипсом \en Work with the basic ellipse 
  /** \brief \ru Вычислить точку на эллипсе.
             \en Evaluate a point on ellipse. \~
    \details \ru Точка вычисляется на замкнутом эллипсе, независимо от того, является объект эллипсом или дугой эллипса.
             \en The point is evaluated on a closed ellipse regardless of whether the object is an ellipse or is an elliptical arc. \~
    \param[in] t - \ru Параметр.
                   \en Parameter. \~
    \param[out] pnt - \ru Искомая точка.
                      \en The required point. \~
  */
  void        PointOnBaseEllipse( double & t, MbCartPoint & pnt ) const; // \ru Точка на базовом эллипсе \en A point on the base ellipse 

  /** \brief \ru Найти проекцию точки на эллипс.
             \en Find the projection of a point onto the ellipse. \~
    \details \ru Точка проецируется на замкнутый эллипсе, независимо от того, является объект эллипсом или дугой эллипса.
             \en A point is projected onto the closed ellipse regardless of whether the object is an ellipse or an elliptical arc. \~
    \param[in] pnt - \ru Проецируемая точка.
                     \en A point to project. \~
    \result \ru Параметр, соответствующий точке проекции.
            \en Parameter corresponding to the projected point. \~
  */
  double      PointProjectionOnBaseEllipse( const MbCartPoint & pnt ) const; // \ru Проекция на базовом эллипсе \en Projection onto the base ellipse 

  void        MakeAsBaseEllipse(); ///< \ru Инициализировать как полный эллипс. \en Initialize as complete ellipse. 
  void        CopyBaseEllipse( const MbArc & init ); ///< \ru Скопировать базовый эллипс. \en Copy the base ellipse. 

  /** \brief \ru Определить, самопересекается ли эквидистанта от эллипса.
             \en Determine whether the ellipse offset has self-intersections. \~
    \param[in] d - \ru Расстояние эквидистанты.
                   \en Offset distance. \~
    \result \ru true, если самопересекается.
            \en True if it has self-intersections. \~
  */
  bool        IsSelfIntersectOffset( double d ) const; // \ru Есть ли самопересечения \en Whether there are self-intersections 

  // \ru Рассчитать коэффициенты неявного представления эллипса для IGES: Ax2 + Bxy + Cy2 + Dx + Ey + F = 0 \en Calculate coefficients of ellipse's implicit representation for IGES: Ax2 + Bxy + Cy2 + Dx + Ey + F = 0 
  bool        ParametricToCanonicConic( double & A, double & B, double & C,
                                        double & D, double & E, double & F,
                                        double & X1, double & Y1, double & X2, double & Y2 ) const;
  bool        Normalize(); ///< \ru Ортонормировать локальную систему координат. \en Orthonormalize the local coordinate system. 
  void        GetControlPoints( SArray<MbCartPoint> & points ); ///< \ru Заполнить массив контрольными точками. \en Fill the array with the control points. 
  void        NormalizeTransform( const MbMatrix & mt ); ///< \ru Ортонормировать плейсмент при трансформировании. \en Orthonormalize the placement when transforming. 

  /** \brief \ru Определить параметры пересечения прямой с эллипсом.
             \en Determine the parameters of intersection of a line with an ellipse. \~
    \param[in] pLine - \ru Прямая.
                       \en Line. \~
    \param[out] cross - \ru Массив с параметрами эллипса в точках пересечения.
                        \en Array with parameters of ellipse at intersection points. \~
    \result \ru Количество точек пересечения.
            \en Count of intersection points. \~
  */
  ptrdiff_t   EllipticIntersect( const MbLine & pLine, double cross[2], double eps0 = PARAM_PRECISION ) const; 
  const   MbArc &     operator = ( const MbArc & init ) { Init( init ); return *this; } ///< \ru Переопределяет оператор присваивания. \en Overrides the assignment operator. 

  void        GetProperties( MbProperties & properties ) override;  // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & properties ) override;  // \ru Записать свойства объекта \en Set properties of the object 
  void        GetBasisPoints( MbControlData & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.
  /** \} */

  void        ReadAsCircle( reader & in ); // \ru Чтение. 
  void        ReadAsEllipse( reader & in ); // \ru Чтение. 
  void        ReadAsEllipseArc( reader & in ); // \ru Чтение. 
  void        WriteAsCircle( writer & out ) const; // \ru Запись. 
  void        WriteAsEllipse( writer & out ) const; // \ru Запись. 
  void        WriteAsEllipseArc( writer & out ) const; // \ru Запись. 

protected :
          // \ru Инициализация параметров по значениям углов эллипса \en Initialization of parameters by values of ellipse angles. 
  inline  double      GetParamEpsilon( double eps = Math::LengthEps ) const; ///< \ru Получить погрешность параметра. \en Get the parameter accuracy 
  inline  void        SetClosed(); ///< \ru Сделать замкнутым. \en Make closed. 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbArc )
};

IMPL_PERSISTENT_OPS( MbArc )


//------------------------------------------------------------------------------
// \ru Установить параметр в область допустимых значений \en Set the parameter into the region of the legal values 
// ---
inline double MbArc::CheckParam( double & t ) const
{
  double tMax = ::fabs( trim2 - trim1 );
  if ( (t < 0.0) || (t > tMax) ) {
    if ( closed ) 
      t -= ::floor( t * Math::invPI2 ) * M_PI2;
    else if ( t < 0.0 )
      t = 0.0;
    else if ( t > tMax )
      t = tMax;
  }
  double w = t;
  ParamToAngle( w );
  return w;
}


//------------------------------------------------------------------------------
// \ru Перевод параметра кривой в угол \en Convert parameter of curve to the angle 
// ---
inline void MbArc::ParamToAngle( double & t ) const
{
  if ( trim2 < trim1 )
    t = -t;
  if ( ::fabs(trim1) > NULL_EPSILON ) 
    t += trim1;
  if ( (t < 0.0) || (t > M_PI2) ) 
    t -= ::floor( t * Math::invPI2 ) * M_PI2;
}


//------------------------------------------------------------------------------
// \ru Перевод угла кривой в параметр кривой \en Convert an angle of curve to a parameter of curve 
// ---
inline void MbArc::AngleToParam( double & t ) const
{
  double dtr = ( trim2 + trim1 - M_PI2 ) * 0.5;
  t -= ::floor( (t - dtr) * Math::invPI2 ) * M_PI2; // SKIP_SA
  t = ( trim2 > trim1 ) ? ( t - trim1 ) : ( trim1 - t );
}


//------------------------------------------------------------------------------
// \ru Погрешность параметра \en Parameter accuracy 
// ---
inline double MbArc::GetParamEpsilon( double eps ) const
{
  double r = ::fabs( a + b ) * 0.5;
  return ( (r > eps) ? (eps / r) : 1.0 );
}


//------------------------------------------------------------------------------
// \ru Сделать замкнутым \en Make closed 
// ---
inline void MbArc::SetClosed()
{
  trim2 = ( sense > 0 ) ? ( trim1 + M_PI2 ) : ( trim1 - M_PI2 );
  closed = true;
  rect.SetEmpty();
  metricLength = -1.0;
}


//------------------------------------------------------------------------------
// \ru Находится ли параметр базовой кривой в диапазоне \en Whether the parameter of the base curve is in the range 
// \ru Усеченной кривой \en Of the trimmed curve 
// ---
inline bool MbArc::IsBaseParamOn( double t, double eps ) const
{
  AngleToParam( t );
  return IsParamOn( t, eps );
}


//------------------------------------------------------------------------------
/** \brief \ru Создание эквидистантной кривой (дуги) эллипса c разными полуосями с учетом самопересечений.
           \en Creation of the offset curve (arc) of ellipse with different semiaxes subject to self-intersections. \~
  \details \ru Создание эквидистантной кривой (дуги) эллипса c разными полуосями с учетом самопересечений. \n
           Для (дуги) окружности не предназначена - выходит с флагом true, ничего не создавая. \n
           \en Creation of the offset curve (arc) of ellipse with different semiaxes subject to self-intersections. \n
           Not for (arc of) a circle - exits with flag true but constructs nothing. \n \~
  \ingroup Curves_2D
*/
// ---
MATH_FUNC (bool) CreateOffsetElliptic( const MbArc & curve, double rad, RPArray<MbCurve> & segments, size_t & count,
                                       bool setArcLimits = false );


//------------------------------------------------------------------------------
/** \brief \ru Запись в поток для старых версий.
           \en Writes to the stream for older versions. \~
  \details \ru Запись в поток для старых версий. \n
           \en Writes to the stream for older versions. \n \~
  \ingroup Curves_2D
*/
// ---
MATH_FUNC (void) EllipticWrite( writer & out, const MbArc * curve ); 


//------------------------------------------------------------------------------
/** \brief \ru Запись в поток для старых версий.
           \en Writes to the stream for older versions. \~
  \details \ru Запись в поток для старых версий. \n
           \en Writes to the stream for older versions. \n \~
  \ingroup Curves_2D
*/
// ---
MATH_FUNC (void) TrimmedWrite( writer & out, const MbTrimmedCurve * curve ); 


#endif // __CUR_ARC_H
