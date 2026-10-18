////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Эллипс в трёхмерном пространстве.
         \en Ellipse in three-dimensional space. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CUR_ARC3D_H
#define __CUR_ARC3D_H


#include <curve3d.h>
#include <mb_placement3d.h>
#include <mb_cube.h>
#include <mb_dim_accuracy.h>


constexpr size_t CONIC_COUNT = 32;
constexpr size_t LINES_COUNT = 10;


class   MbArc;


//------------------------------------------------------------------------------
/** \brief \ru Дуга эллипса в трёхмерном пространстве.
           \en Elliptical arc in three-dimensional space. \~
  \details \ru Дуга эллипса описывается двумя радиусами a и b и двумя параметрами trim1 и trim2, заданными в локальной системе координат position. \n
    Параметры trim1 и trim2 отсчитываются по дуге в направлении движения от оси position.axisX к оси position.axisY. 
    Параметры trim1 и trim2 будем называть параметрами усечения. 
    Значения параметров усечения, равные нулю и 2pi, соответствуют точке на оси position.axisX. \n
    Параметр кривой t принимает значения на отрезке: 0<=t<=trim2–trim1. 
    Кривая может быть замкнутой. У замкнутой кривой trim2–trim1=2pi. \n
    Радиус-вектор кривой в методе PointOn(double&t,MbCartPoint3D&r) описывается векторной функцией\n
    r(t) = position.origin + (a cos(trim1+t) position.axisX) + (b sin(trim1+t) position.axisY).\n
    Радиусы кривой должны быть больше нуля: a>0, b>0. \n
    Для параметров усечения должны соблюдаться неравенства: trim1<trim2. \n
    Локальная система координат position может быть как правой, так и левой. \n
           \en The elliptical arc is described by two radii a and b and two parameters trim1 and trim2 given in the local coordinate system 'position'. \n
    Parameters 'trim1' and 'trim2' are measured along the arc in direction from position.axisX axis to position.axisY axis. 
    Parameters 'trim1' and 'trim2' will be called parameters of trimming. 
    Values of parameters of trimming equal to 0 and 2pi correspond to a point on position.axisX axis. \n
    Parameter t of curve possesses the values in the range: 0<=t<=trim2-trim1. 
    The curve can be closed. For closed curve: trim2-trim1=2pi. \n
    Radius-vector of the curve in the method PointOn(double&t,MbCartPoint3D&r) is described by the function\n
    r(t) = position.origin + (a cos(trim1+t) position.axisX) + (b sin(trim1+t) position.axisY).\n
    Radii of the curve must be positive: a>0, b>0. \n
    The following inequalities must be satisfied for the parameters of trimming: trim1<trim2. \n
    Local coordinate system 'position' can be both right and left. \n \~
  \ingroup Curves_3D
*/
// ---
class MATH_CLASS MbArc3D : public MbCurve3D {
protected :
  MbPlacement3D position; ///< \ru Локальная система координат. \en Local coordinate system.  
  double        a;        ///< \ru Радиус полуоси вдоль X. \en Radius of semiaxis along X. 
  double        b;        ///< \ru Радиус полуоси вдоль Y. \en Radius of semiaxis along Y. 
  double        trim1;    ///< \ru Параметры начальной точки. \en The start point parameters. 
  double        trim2;    ///< \ru Параметры конечной  точки. \en The end point parameters. 
  bool          closed;   ///< \ru Замкнутость. \en Closedness. 
    // \ru Временные данные. \en Temporary data. 
  mutable MbCube cube;   ///< \ru Габаритный куб. \en Bounding box. 

protected:
  /** \brief \ru Конструктор окружности с параметрами по умолчанию.
             \en Constructor of a circle with default parameters. \~
    \details \ru Создается окружность с центром в начале координат и с нулевым радиусом.
             \en A circle is created with center in the origin and zero radius. \~
  */
  MbArc3D();

  /** \brief \ru Конструктор эллипса, окружности или их дуг.
             \en Constructor of an ellipse, a circle or an elliptical or circular arc. \~
    \details \ru Создается дуга с центром в точке pc. \n
      Первая полуось определяется как расстояние между точками pc и p1.
      Вторая полуось определяется как длина проекции вектора из pc в p2 на перпендикуляр к (p1 - pc).
      Начальная точка дуги лежит на луче, выходящем из центра окружности и проходящем через точку p1.
      Конечная точка - на луче из центра, проходящем через точку p2.  \n
      Параметр initSense определяет цельность и направление дуги.
      Если initSense == 0, то строится полный эллипс или окружность. \n
      Если initSense > 0, то направление движения против часовой стрелки, если смотреть навстречу векторному произведению (p1 - pc) и (p2 - pc). \n
      Если initSense < 0, то направление движения против часовой стрелки, если смотреть навстречу векторному произведению (p1 - pc) и (p2 - pc). \n
             \en An arc centered in point 'pc' is created. \n
      The first semiaxis is determines as the distance between points pc and p1.
      The second semiaxis is determined as the length of projection of the vector from pc to p2 onto the perpendicular to (p1 - pc).
      The start point of the arc lies on the ray starting from the circle center and passing through point 'p1'.
      The end point is on the ray from the center passing through the point 'p2'.  \n
      Parameter 'initSense' specifies completeness and the arc direction.
      If initSense == 0, then the complete ellipse or circle is constructed. \n
      If initSense > 0, then the direction of moving is counterclockwise if seeing against the vector product (p1 - pc) and (p2 - pc). \n
      If initSense < 0, , then the direction of moving is counterclockwise if seeing against the vector product (p1 - pc) and(p2 - pc). \n \~
    \deprecated \ru Метод устарел. \en The method is deprecated. \~
    \param[in] pc - \ru Центр эллипса или окружности.
                    \en Center of the ellipse or the circle. \~
    \param[in] p1 - \ru Точка, определяющая начало кривой и первую полуось.
                    \en A point determining the beginning of the curve and the first semiaxis. \~
    \param[in] p2 - \ru Точка, определяющая конец кривой и вторую полуось.
                    \en A point determining the end of the curve and the second semiaxis. \~
    \param[in] initSense - \ru Определяет цельность и направление. initSense == 0 - замкнутая кривая initSense > 0 - движение против часовой стрелки, initSense < 0 - по часовой стрелке.
                           \en Determines the completeness and the direction. initSense == 0 - closed curve initSense > 0 - moving counterclockwise, initSense < 0 - clockwise. \~
  */
  MbArc3D( const MbCartPoint3D & pc, const MbCartPoint3D & p1, const MbCartPoint3D & p2, int initSense = 0 );

  /** \brief \ru Конструктор окружности или дуги окружности.
             \en Constructor of a circle or a circular arc. \~
    \details \ru Конструктор окружности или дуга окружности одним из двух способов.
             \en Constructor of a circle or a circular arc by one of two methods.
    \deprecated \ru Метод устарел. \en The method is deprecated. \~
    \param[in] p0 - \ru Центр (n == 0) или начальная точка (n != 0).
                    \en Center (n == 0) or starting point (n != 0). \~
    \param[in] p1 - \ru Начальная точка (n == 0) или точка, через которую проходит окружность (n != 0).
                    \en Starting point (n == 0) or point the circle passes through (n == 1). \~
    \param[in] p2 - \ru Точка, определяющая конец кривой и вторую полуось.
                    \en A point determining the end of the curve and the second semiaxis. \~
    \param[in] n  - \ru Определяет способ построения окружности.
                        Если n == 0, то окружность или дуга имеют центр в точке p0.
                        Если n == 1, то окружность или дуга проходят по трем заданным точкам. \n
                        Если |n| == 2 и closed == false, то дуга будет дополнять до полной окружности дугу, проходящую по трем заданным точкам. \n
                    \en The parameter defines the method of arc construction. \~
                        If n == 0, then a circle or a circular arc have the center in point p0.
                        If n == 1, then a circle or an arc passes through the specified three points. \n
                        If |n| == 2 & closed == false, then an arc passes through p0 and p2 but not p1. \n \~
    \param[in] closed - \ru Определяет окружность (true) или дугу (false).
                        \en Specifies a circle (true) or an arc (false). \~
  */
  MbArc3D( const MbCartPoint3D & p0, const MbCartPoint3D & p1, const MbCartPoint3D & p2, int n, bool closed );

  /** \brief \ru Конструктор окружности или дуги окружности.
             \en Constructor of a circle or a circular arc. \~
    \details \ru Создается дуга окружности с центром в точке pc и с заданным радиусом.
      Радиус окружности или ее дуги определяется как расстояние между точками pc и p1.
      Точки pc, p1 и p2 определяют плоскость дуги.
      Точки p1 и p2 определяют границы дуги.
      Вектор aZ определяет направление оси Z локальной системы координат дуги окружности.
      Начальная точка дуги лежит в точке p1.
      Конечная точка дуги лежит на луче, выходящем из центра окружности и проходящем через точку p2.
      Параметр initSense определяет направление дуги.
             \en A circular arc is created with a center in point 'pc'.
      Points 'p1' and 'p2' specify the bounds of arc.
      The start point of the arc lies on point 'p1'.
      The end point is on the ray passing through the point 'p2'.
      Parameter 'initSense' specifies the arc direction. \~
    \deprecated \ru Метод устарел. \en The method is deprecated. \~
    \param[in] pc - \ru Центр окружности.
                    \en Center of circle. \~
    \param[in] p1 - \ru Точка, определяющая начало дуги.
                    \en A point specifying the beginning of the arc. \~
    \param[in] p2 - \ru Точка, определяющая конец дуги.
                    \en A point specifying the end of the arc. \~
    \param[in] aZ - \ru Направление оси Z локальной системы координат дуги окружности.
                    \en A direction of axis Z local coordinate system of the arc. \~
    \param[in] initSense - \ru Направление дуги.
                    Если initSense > 0, то направление движения дуги против часовой стрелки, если смотреть навстречу вектору aZ.
                    Если initSense < 0, то направление движения дуги по часовой стрелке, если смотреть навстречу вектору aZ.
                    Если initSense == 0, то будет построена полная окружность.
                           \en Arc direction.
                    If initSense > 0, then the orientation is counterclockwise if you look towards the vector aZ.
                    If initSense < 0, then the orientation is clockwise if you look towards the vector aZ.
                    If initSense = 0, then the circle is building. \~
  */
  MbArc3D( const MbCartPoint3D & pc, const MbCartPoint3D & p1, const MbCartPoint3D & p2,
                             const MbVector3D & aZ, int initSense );

  /** \brief \ru Конструктор дуги эллипса.
             \en Constructor of an elliptical arc. \~
    \details \ru Создается дуга эллипса с заданными полуосями и локальной системой координат.
      angle определяет угол дуги. Угол отсчитываются от оси OX против часовой стрелки.
      Угол задан в радианах.
             \en An elliptical arc is created with the given semiaxes and the local coordinate system.
      'angle' determines the arc angle. The angle is measured from the OX axis counterclockwise.
      The angle is given in radians. \~
    \deprecated \ru Метод устарел. \en The method is deprecated. \~
    \param[in] p0 - \ru Центр локальной системы координат эллипса.
                    \en The ellipse local coordinate system center. \~
    \param[in] vZ - \ru Ось Z локальной системы координат эллипса.
                    \en Z-axis of the local coordinate system of the ellipse. \~
    \param[in] vX - \ru Ось X локальной системы координат эллипса.
                    \en X-axis of the local coordinate system of the ellipse. \~
    \param[in] aa - \ru Радиус полуоси вдоль X.
                    \en Radius of semiaxis along X. \~
    \param[in] bb - \ru Радиус полуоси вдоль Y.
                    \en Radius of semiaxis along Y. \~
    \param[in] angle - \ru Угол, определяющий конец дуги.
                       \en An angle specifying the end of the arc. \~
  */
  MbArc3D( const MbCartPoint3D & p0, const MbVector3D & vZ, const MbVector3D & vX, double aa, double bb, double angle );

  /** \brief \ru Конструктор дуги окружности.
             \en Constructor of a circular arc. \~
    \details \ru Создается дуга окружности с концами в заданных точках.
      Радиус окружности определяется по заданному тангенсу 1/4 угла раствора дуги.
             \en An arc is created with ends at the given points.
      A circle radius is defined by the given tangent of 1/4 of arc opening angle. \~
    \deprecated \ru Метод устарел. \en The method is deprecated. \~
    \param[in] p1 - \ru Начало дуги.
                    \en Beginning of the arc. \~
    \param[in] p2 - \ru Конец дуги.
                    \en End of the arc. \~
    \param[in] a4 - \ru Тангенс 1/4 угла раствора дуги.
                    \en Tangent of 1/4 of the arc opening angle. \~
    \param[in] vZ - \ru Ось дуги.
                    \en Axis of the arc. \~
  */
  MbArc3D( const MbCartPoint3D & p1, const MbCartPoint3D & p2, double a_4, MbVector3D & vZ );

  /** \brief \ru Конструктор окружности по двум точкам и направлению к центру в одной из них.
             \en Constructor of a circle by two points and direction to the center from one of them. \~
    \details \ru Создается окружность по двум точкам и направлению в одной из них.
             \en A circle is created by two points and direction at one of them. \~
    \deprecated \ru Метод устарел. \en The method is deprecated. \~
    \param[in] p1 - \ru Начальная точка.
                    \en The starting point. \~
    \param[in] p2 - \ru Конечная точка.
                    \en The end point. \~
    \param[in] dirInPoint - \ru Направление из одной из точек (p1 или p2) к центру окружности.
                            \en Direction at one of points (p1 or p2) to the center of the circle. \~
    \param[in] insecond - \ru Направление из первой точки (insecond == true) к центру окружности.
                          \en Direction from the first point (insecond == true) to the circle center. \~
  */
  MbArc3D( const MbCartPoint3D & p1, const MbCartPoint3D & p2, const MbVector3D & dirInPoint, bool insecond );

public :
  /** \brief \ru Конструктор дуги эллипса.
             \en Constructor of an elliptical arc. \~
    \details \ru Создается дуга эллипса с заданными полуосями и локальной системой координат. 
      angle определяет угол дуги. Угол отсчитываются от оси OX против часовой стрелки. 
      Угол задан в радианах.
             \en An elliptical arc is created with the given semiaxes and the local coordinate system. 
      'angle' determines the arc angle. The angle is measured from the OX axis counterclockwise. 
      The angle is given in radians. \~
    \param[in] place - \ru Локальная система координат эллипса.
                       \en The local coordinate system of the ellipse. \~
    \param[in] aa - \ru Радиус полуоси вдоль X.
                    \en Radius of semiaxis along X. \~
    \param[in] bb - \ru Радиус полуоси вдоль Y.
                    \en Radius of semiaxis along Y. \~
    \param[in] angle - \ru Угол, определяющий конец дуги.
                       \en An angle specifying the end of the arc. \~
  */
  MbArc3D( const MbPlacement3D & place, double aa, double bb, double angle );

  /** \brief \ru Конструктор дуги эллипса.
             \en Constructor of an elliptical arc. \~
    \details \ru Создается дуга эллипса с локальной системой координат и полуосями заданного эллипса c учетом трехмерной точности.
      t1 и t2 определяют начальный и конечный углы дуги. Углы отсчитываются от оси OX против часовой стрелки. 
      Углы заданы в радианах.
      Параметр initSense определяет направление дуги. Если initSense > 0, то направление движения против часовой стрелки.
             \en An elliptical arc is constructed with the local coordinate system and semiaxes of the given ellipse.
      t1 and t2 specify the start and the end angles of the arc. The angles are measured from the OX axis counterclockwise. 
      The angles are given in radians.
      Parameter 'initSense' specifies the arc direction. If initSense > 0, then the direction of moving is counterclockwise. \~
    \param[in] init - \ru Эллипс - образец.
                      \en A pattern ellipse. \~
    \param[in] t1 - \ru Угол, определяющий начало дуги.
                    \en An angle specifying the beginning of the arc. \~
    \param[in] t2 - \ru Угол, определяющий конец дуги.
                    \en An angle specifying the end of the arc. \~
    \param[in] initSense - \ru Направление. initSense > 0 - движение против часовой стрелки, initSense < 0 - по часовой стрелке.
      initSense не должно быть равным нулю.
                       \en Direction. initSense > 0 - moving counterclockwise, clockwise < 0 - clockwise.
      'initSense' can't be equal to zero. \~
    \param[in] xyzEps - \ru Трехмерная точность.
                        \en Three-dimensional accuracy. \~
  */
  MbArc3D( const MbArc3D & init, double t1, double t2, int initSense, const MbDimAccuracy & xyzEps = MbDimAccuracy::threeDimAcc );

  /** \brief \ru Конструктор дуги эллипса.
             \en Constructor of an elliptical arc. \~
    \details \ru Создается дуга эллипса с локальной системой координат и полуосями заданного эллипса.
      Проекции p1 и p2 на init определяют начальный и конечный углы дуги. 
      Параметр initSense определяет направление дуги. Если initSense > 0, то направление движения против часовой стрелки.
             \en An elliptical arc is constructed with the local coordinate system and semiaxes of the given ellipse.
      Projections of p1 and p2 onto 'init' determines the starting and the end angles of the arc. 
      Parameter 'initSense' specifies the arc direction. If initSense > 0, then the direction of moving is counterclockwise. \~
    \param[in] init - \ru Эллипс - образец.
                      \en A pattern ellipse. \~
    \param[in] p1 - \ru Точка, определяющая начало дуги.
                    \en A point specifying the beginning of the arc. \~
    \param[in] p2 - \ru Точка, определяющая конец дуги.
                    \en A point specifying the end of the arc. \~
    \param[in] initSense - \ru Направление. initSense > 0 - движение против часовой стрелки, initSense < 0 - по часовой стрелке.
      initSense не должно быть равным нулю.
                       \en Direction. initSense > 0 - moving counterclockwise, clockwise < 0 - clockwise.
      'initSense' can't be equal to zero. \~
  */
  MbArc3D( const MbArc3D & init, MbCartPoint3D p1, MbCartPoint3D p2, int initSense );

  /** \brief \ru Конструктор по локальной системе и двумерной дуге эллипса.
             \en Constructor by a local system and two-dimensional elliptical arc. \~
    \details \ru Конструктор по локальной системе координат и двумерной дуге эллипса.
             \en Constructor by a local coordinate system and two-dimensional elliptical arc. \~ 
    \param[in] ellipse - \ru Двумерная дуга эллипса.
                         \en Two-dimensional elliptical arc. \~
    \param[in] place - \ru Локальная система координат.
                       \en A local coordinate system. \~
  */
  MbArc3D( const MbArc & ellipse, const MbPlacement3D & place );

//protected:
  explicit MbArc3D( const MbArc3D & init );
public :
  virtual ~MbArc3D();

public:
  /** \brief \ru Создать эллипс, окружность или их дугу.
             \en Create ellipse, a circle or an elliptical or circular arc. \~
    \details \ru Создается дуга с центром в точке pc. \n
      Первая полуось определяется как расстояние между точками pc и p1.
      Вторая полуось определяется как длина проекции вектора из pc в p2 на перпендикуляр к (p1 - pc).
      Начальная точка дуги лежит на луче, выходящем из центра окружности и проходящем через точку p1.
      Конечная точка - на луче из центра, проходящем через точку p2.  \n
      Параметр initSense определяет цельность и направление дуги.
      Если initSense == 0, то строится полный эллипс или окружность. \n
      Если initSense > 0, то направление движения против часовой стрелки, если смотреть навстречу векторному произведению (p1 - pc) и (p2 - pc). \n
      Если initSense < 0, то направление движения против часовой стрелки, если смотреть навстречу векторному произведению (p1 - pc) и (p2 - pc). \n
             \en An arc centered in point 'pc' is created. \n
      The first semiaxis is determines as the distance between points pc and p1.
      The second semiaxis is determined as the length of projection of the vector from pc to p2 onto the perpendicular to (p1 - pc).
      The start point of the arc lies on the ray starting from the circle center and passing through point 'p1'.
      The end point is on the ray from the center passing through the point 'p2'.  \n
      Parameter 'initSense' specifies completeness and the arc direction.
      If initSense == 0, then the complete ellipse or circle is constructed. \n
      If initSense > 0, then the direction of moving is counterclockwise if seeing against the vector product (p1 - pc) and (p2 - pc). \n
      If initSense < 0, , then the direction of moving is counterclockwise if seeing against the vector product (p1 - pc) and(p2 - pc). \n \~
    \param[in] pc - \ru Центр эллипса или окружности.
                    \en Center of the ellipse or the circle. \~
    \param[in] p1 - \ru Точка, определяющая начало кривой и первую полуось.
                    \en A point determining the beginning of the curve and the first semiaxis. \~
    \param[in] p2 - \ru Точка, определяющая конец кривой и вторую полуось.
                    \en A point determining the end of the curve and the second semiaxis. \~
    \param[in] initSense - \ru Определяет цельность и направление. initSense == 0 - замкнутая кривая initSense > 0 - движение против часовой стрелки, initSense < 0 - по часовой стрелке.
                           \en Determines the completeness and the direction. initSense == 0 - closed curve initSense > 0 - moving counterclockwise, initSense < 0 - clockwise. \~
    \return \ru Возвращает указатель на созданный объект или нулевой указатель в случае неудачи.
            \en Returns pointer to the created object or null pointer in case of failure. \~
  */
  static MbArc3D * Create( const MbCartPoint3D & pc, const MbCartPoint3D & p1, const MbCartPoint3D & p2, int initSense = 0 );

  /** \brief \ru Создать окружность или дугу окружности.
             \en Create circle or a circular arc. \~
    \details \ru Создать окружность или дугу окружности одним из двух способов.
             \en Constructor of a circle or a circular arc by one of two methods.
    \param[in] p0 - \ru Центр (n == 0) или начальная точка (n != 0).
                    \en Center (n == 0) or starting point (n != 0). \~
    \param[in] p1 - \ru Начальная точка (n == 0) или точка, через которую проходит окружность (n != 0).
                    \en Starting point (n == 0) or point the circle passes through (n == 1). \~
    \param[in] p2 - \ru Точка, определяющая конец кривой и вторую полуось.
                    \en A point determining the end of the curve and the second semiaxis. \~
    \param[in] n  - \ru Определяет способ построения окружности.
                        Если n == 0, то окружность или дуга имеют центр в точке p0.
                        Если n == 1, то окружность или дуга проходят по трем заданным точкам. \n
                        Если |n| == 2 и closed == false, то дуга будет дополнять до полной окружности дугу, проходящую по трем заданным точкам. \n
                    \en The parameter defines the method of arc construction. \~
                        If n == 0, then a circle or a circular arc have the center in point p0.
                        If n == 1, then a circle or an arc passes through the specified three points. \n
                        If |n| == 2 & closed == false, then an arc passes through p0 and p2 but not p1. \n \~
    \param[in] closed - \ru Определяет окружность (true) или дугу (false).
                        \en Specifies a circle (true) or an arc (false). \~
    \return \ru Возвращает указатель на созданный объект или нулевой указатель в случае неудачи.
            \en Returns pointer to the created object or null pointer in case of failure. \~
  */
  static MbArc3D * Create( const MbCartPoint3D & p0, const MbCartPoint3D & p1, const MbCartPoint3D & p2, int n, bool closed );

  /** \brief \ru Создать окружность или дугу окружности.
             \en Create circle or a circular arc. \~
    \details \ru Создается дуга окружности с центром в точке pc и с заданным радиусом.
      Радиус окружности или ее дуги определяется как расстояние между точками pc и p1.
      Точки pc, p1 и p2 определяют плоскость дуги.
      Точки p1 и p2 определяют границы дуги.
      Вектор aZ определяет направление оси Z локальной системы координат дуги окружности.
      Начальная точка дуги лежит в точке p1.
      Конечная точка дуги лежит на луче, выходящем из центра окружности и проходящем через точку p2.
      Параметр initSense определяет направление дуги.
             \en A circular arc is created with a center in point 'pc'.
      Points 'p1' and 'p2' specify the bounds of arc.
      The start point of the arc lies on point 'p1'.
      The end point is on the ray passing through the point 'p2'.
      Parameter 'initSense' specifies the arc direction. \~
    \param[in] pc - \ru Центр окружности.
                    \en Center of circle. \~
    \param[in] p1 - \ru Точка, определяющая начало дуги.
                    \en A point specifying the beginning of the arc. \~
    \param[in] p2 - \ru Точка, определяющая конец дуги.
                    \en A point specifying the end of the arc. \~
    \param[in] aZ - \ru Направление оси Z локальной системы координат дуги окружности.
                    \en A direction of axis Z local coordinate system of the arc. \~
    \param[in] initSense - \ru Направление дуги.
                    Если initSense > 0, то направление движения дуги против часовой стрелки, если смотреть навстречу вектору aZ.
                    Если initSense < 0, то направление движения дуги по часовой стрелке, если смотреть навстречу вектору aZ.
                    Если initSense == 0, то будет построена полная окружность.
                           \en Arc direction.
                    If initSense > 0, then the orientation is counterclockwise if you look towards the vector aZ.
                    If initSense < 0, then the orientation is clockwise if you look towards the vector aZ.
                    If initSense = 0, then the circle is building. \~
    \return \ru Возвращает указатель на созданный объект или нулевой указатель в случае неудачи.
            \en Returns pointer to the created object or null pointer in case of failure. \~
  */
  static MbArc3D * Create( const MbCartPoint3D & pc, const MbCartPoint3D & p1, const MbCartPoint3D & p2,
                           const MbVector3D & aZ, int initSense );

  /** \brief \ru Создать дугу эллипса.
             \en Create elliptical arc. \~
    \details \ru Создается дуга эллипса с заданными полуосями и локальной системой координат.
      angle определяет угол дуги. Угол отсчитываются от оси OX против часовой стрелки.
      Угол задан в радианах.
             \en An elliptical arc is created with the given semiaxes and the local coordinate system.
      'angle' determines the arc angle. The angle is measured from the OX axis counterclockwise.
      The angle is given in radians. \~
    \param[in] p0 - \ru Центр локальной системы координат эллипса.
                    \en The ellipse local coordinate system center. \~
    \param[in] vZ - \ru Ось Z локальной системы координат эллипса.
                    \en Z-axis of the local coordinate system of the ellipse. \~
    \param[in] vX - \ru Ось X локальной системы координат эллипса.
                    \en X-axis of the local coordinate system of the ellipse. \~
    \param[in] aa - \ru Радиус полуоси вдоль X.
                    \en Radius of semiaxis along X. \~
    \param[in] bb - \ru Радиус полуоси вдоль Y.
                    \en Radius of semiaxis along Y. \~
    \param[in] angle - \ru Угол, определяющий конец дуги.
                       \en An angle specifying the end of the arc. \~
    \return \ru Возвращает указатель на созданный объект или нулевой указатель в случае неудачи.
            \en Returns pointer to the created object or null pointer in case of failure. \~
  */
  static MbArc3D * Create( const MbCartPoint3D & p0, const MbVector3D & vZ, const MbVector3D & vX, double aa, double bb, double angle );

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
    \param[in] vZ - \ru Ось дуги.
                    \en Axis of the arc. \~
    \return \ru Возвращает указатель на созданный объект или нулевой указатель в случае неудачи.
            \en Returns pointer to the created object or null pointer in case of failure. \~
  */
  static MbArc3D * Create( const MbCartPoint3D & p1, const MbCartPoint3D & p2, double a_4, MbVector3D & vZ );

  /** \brief \ru Создать окружность по двум точкам и направлению к центру в одной из них.
             \en Create circle by two points and direction to the center from one of them. \~
    \details \ru Создается окружность по двум точкам и направлению в одной из них.
             \en A circle is created by two points and direction at one of them. \~
    \param[in] p1 - \ru Начальная точка.
                    \en The starting point. \~
    \param[in] p2 - \ru Конечная точка.
                    \en The end point. \~
    \param[in] dirInPoint - \ru Направление из одной из точек (p1 или p2) к центру окружности.
                            \en Direction at one of points (p1 or p2) to the center of the circle. \~
    \param[in] insecond - \ru Направление из первой точки (insecond == true) к центру окружности.
                          \en Direction from the first point (insecond == true) to the circle center. \~
    \return \ru Возвращает указатель на созданный объект или нулевой указатель в случае неудачи.
            \en Returns pointer to the created object or null pointer in case of failure. \~
  */
  static MbArc3D * Create( const MbCartPoint3D & p1, const MbCartPoint3D & p2, const MbVector3D & dirInPoint, bool insecond );

public:
  VISITING_CLASS( MbArc3D );

  void    Init( const MbArc3D & );
  void    Init( const MbPlacement3D &, double aa, double bb, double angle );
  void    Init( const MbArc3D & init, double t1, double t2, int initSense, const MbDimAccuracy & xyzEps = MbDimAccuracy::threeDimAcc );
          /// \ru Инициализация окружности или дуги окружности по трем точкам, (n == 0) - окружность или дуга по центру и двум точкам, (n == 1) - окружность или дуга по трем точкам. Возвращает true в случае удачи. \en Initialization of a circular arc by three points; (n == 0) - a circle or an arc by the center and two points, (n == 1) - a circle or an arc by three points. Return true if successful.
  bool    Init( const MbCartPoint3D & p0, const MbCartPoint3D & p1, const MbCartPoint3D & p2, int n, bool closed ); 
          /// \ru Инициализация дуги окружности по начальной и конечной точкам и 1/2 угла раствора дуги. Возвращает true в случае удачи. \en Initialization of a circular arc by the starting and the end points and 1/2 of the arc opening angle. Return true if successful. 
  bool    Init( double a_2, const MbCartPoint3D & p1, const MbCartPoint3D & p2, MbVector3D & vZ ); 
          /// \ru Инициализация дуги окружности по 2D-дуге и локальной системе координат. \en Initialization of an arc by 2D-arc and local coordinate system. 
  void    Init( const MbArc & ellipse, const MbPlacement3D & pos );
          /// \ru Инициализация окружности по двум точкам и направлению к центру в одной из них. Возвращает true в случае удачи. \en Initialization of a circle by two points and direction to the center in one of them. Returns true if successful.
  bool    Init( const MbCartPoint3D & p1, const MbCartPoint3D & p2, const MbVector3D & dirInPoint, bool insecond );

  // \ru Общие функции математического объекта \en Common functions of the mathematical object 
  /** \ru \name Общие функции геометрического объекта.
      \en \name Common functions of a geometric object.
      \{ */
  MbeSpaceType  IsA() const override; // \ru Тип элемента \en A type of element 
  MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента \en Create a copy of the element
  void    Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void    Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override;          // \ru Сдвиг \en Translation
  void    Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate around an axis
  bool    IsSame   ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const override;
  bool    SetEqual ( const MbSpaceItem & ) override; // \ru Сделать равным \en Make equal
  double  DistanceToPoint( const MbCartPoint3D & ) const override;// \ru Расстояние до точки \en Distance to a point
  bool    IsSpaceSame( const MbSpaceItem & item, double eps = METRIC_REGION ) const override; // \ru Являются ли объекты идентичными в пространстве \en Whether the objects are identical in the space

  void    GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта \en Get properties of the object
  void    SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of the object
  void    GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void    SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.
  /** \} */
  // \ru Общие функции кривой \en Common functions of the curve 
  /** \ru \name Функции описания области определения кривой.
      \en \name Functions for curve domain description
      \{ */
  double  GetTMax()  const override; // \ru Вернуть максимальное значение параметра \en Get the maximum value of parameter
  double  GetTMin()  const override; // \ru Вернуть минимальное значение параметра \en Get the minimum value of parameter 
  bool    IsClosed() const override; // \ru Проверка замкнутости кривой \en Check for curve closedness
  double  GetPeriod() const override; // \ru Вернуть период \en Return the period
  /** \} */
  /** \ru \name Функции для работы в области определения кривой. 
            Функции PointOn, FirstDer, SecondDer, ThirdDer,... корректируют параметр
            при выходе его за пределы области определения.
      \en \name Functions for working in the domain of a curve. 
            PointOn, FirstDer, SecondDer, ThirdDer,... functions correct parameter
            when it runs out the domain.
      \{ */
  void    PointOn  ( double & t, MbCartPoint3D & ) const override; // \ru Точка на кривой \en Point on the curve
  void    FirstDer ( double & t, MbVector3D & ) const override;    // \ru Первая производная \en The first derivative
  void    SecondDer( double & t, MbVector3D & ) const override;    // \ru Вторая производная \en The second derivative
  void    ThirdDer ( double & t, MbVector3D & ) const override;    // \ru Третья производная по t \en The third derivative with respect to t
  void    Normal   ( double & t, MbVector3D & ) const override;    // \ru Вектор главной нормали \en Vector of the principal normal
  /** \} */
  /** \ru \name Функции для работы внутри и вне области определения кривой. 
            Функции _PointOn, _FirstDer, _SecondDer, _ThirdDer,... не корректируют параметр
            при выходе за пределы области определения. Ограниченная кривая продолжается в соответствии с уравнениями кривой.
      \en \name Functions for working inside and outside of the curve domain. 
            _PointOn, _FirstDer, _SecondDer, _ThirdDer,... functions don't correct parameter
            when it runs out the domain. The bounded curve is extended due to the equations of curve.
      \{ */
  void   _PointOn  ( double t, MbCartPoint3D & ) const override; // \ru Точка на кривой \en Point on the curve
  void   _FirstDer ( double t, MbVector3D & ) const override;  // \ru Первая производная \en The first derivative
  void   _SecondDer( double t, MbVector3D & ) const override;  // \ru Вторая производная \en The second derivative
  void   _ThirdDer ( double t, MbVector3D & ) const override;  // \ru Третья производная по t \en The third derivative with respect to t
  void   _Normal   ( double t, MbVector3D & ) const override;// \ru Вектор главной нормали \en Vector of the principal normal
  /** \} */
  /** \ru \name Функции доступа к группе данных для работы внутри и вне области определения параметра кривой.
      \en \name Functions for get of the group of data inside and outside the curve's domain of parameter.
      \{ */
  // \ru Вычислить значения точки и производных для заданного параметра. \en Calculate point and derivatives of object for given parameter. \~
  void    Explore( double & t, bool ext,
                   MbCartPoint3D & pnt, MbVector3D & fir, MbVector3D * sec, MbVector3D * thir ) const override;
  /** \} */
  void    Inverse( MbRegTransform * iReg = nullptr ) override; // \ru Изменить направление \en Change direction
  double  Step         ( double t, double sag ) const override; // \ru Вычисление шага по стрелке прогиба. \en Calculation of parameter step by the sag.
  double  DeviationStep( double t, double angle ) const override; // \ru Вычисление шага по углу отклонения нормали. \en Calculation of parameter step by the deviation angle.
  double  MetricStep   ( double t, double length ) const override; // \ru Вычисление шага параметра по длине. \en Calculation of parameter step by the given length.

  // \ru Все проекции точки на кривую \en All the projections of a point onto the curve 
  bool    NearPointProjection( const MbCartPoint3D &, double & t, bool ext, MbRect1D * tRange = nullptr ) const override; // \ru Ближайшая проекция точки на кривую \en The closest projection of a point onto the curve

  MbNurbs3D * NurbsCurve( const MbCurveIntoNurbsInfo & ) const override;
  MbCurve3D * NurbsCurve( const MbNurbsParameters & ) const override; // \ru Построить Nurbs-копию кривой \en Construct NURBS-copy of the curve

  // \ru Построить усеченную кривую c учетом заданной трехмерной точности. \en Construct a trimmed curve with the given three-dimensional accuracy.
  MbCurve3D * Trimmed( double t1, double t2, int sense, const MbDimAccuracy & xyzEps, bool saveParamLenAndLaw ) const override;
  using MbCurve3D::Trimmed; // \ru Метод базового класса MbCurve3D без учета трехмерной точности. Точность по умолчанию - MbDimAccuracy::threeDimAcc.  \en Method of MbCurve3D base class without three-dimensional accuracy. The default accuracy is MbDimAccuracy::threeDimAcc.

  double  CalculateMetricLength() const override; // \ru Посчитать метрическую длину \en Calculate metric length
  double  GetLengthEvaluation() const override;
  double  Curvature( double t ) const override; // \ru Кривизна по t \en Curvature by t
  // \ru Сдвинуть параметр t на расстояние len по направлению \en Shift the parameter t by the distance 'len' in the direction 
  bool    DistanceAlong( double & t, double len, int curveDir, double eps = Math::metricPrecision,
                                 VERSION version = Math::DefaultMathVersion() ) const override;
  size_t  GetCount() const override;
  void    GetPointsByEvenLengthDelta( size_t n,  std::vector<MbCartPoint3D> & pnts ) const override; // \ru Выдать n точек кривой с равными интервалами по длине дуги \en Get n points of curves equally spaced by the arc length

  MbCurve * GetMap( const MbMatrix3D &, MbRect1D * pRgn = nullptr,
                    VERSION version = Math::DefaultMathVersion(), bool * coincParams = nullptr ) const override; // \ru Дать плоскую проекцию кривой \en Get a planar projection of a curve
  MbCurve * GetMapPsp( const MbMatrix3D &, double zNear, 
                       MbRect1D * pRgn = nullptr ) const override;

  double  GetRadius( double accuracy = METRIC_REGION ) const override; // \ru Дать физический радиус объекта или ноль, если это невозможно. \en Get the physical radius of the object or null if it impossible.
  bool    GetCircleAxis  ( MbAxis3D & ) const override; // \ru Дать ось кривой \en Get the axis of the curve
  // \ru Дать плоскую кривую и плейсмент, если пространственная кривая плоская (после использования вызывать DeleteItem на двумерную кривую) \en Get a planar curve and placement, if the spatial curve is planar (after using the DeleteItem must be called on a three-dimensional curve) 
  bool    GetPlaneCurve( MbCurve *& curve2d, MbPlacement3D & place, bool saveParams, PlanarCheckParams params = PlanarCheckParams() ) const override;

  void    AddYourGabaritTo( MbCube & ) const override; // \ru Добавить габарит кривой в куб. \en Add a bounding box of a curve to a cube.
  void    CalculateGabarit( MbCube & ) const override; // \ru Вычислить габарит кривой \en Calculate bounding box of curve
  void    CalculateLocalGabarit( const MbMatrix3D &, MbCube & ) const override; // \ru Рассчитать габарит относительно л.с.к. \en Calculate bounding box relative to local coordinate system
  /// \ru Является ли объект смещением \en Whether the object is a shift 
  bool    IsShift ( const MbSpaceItem &, MbVector3D &, bool & isSame, double accuracy = LENGTH_EPSILON ) const override;
  bool    IsSimilarToCurve( const MbCurve3D & curve, double precision = METRIC_PRECISION ) const override; // \ru Подобные ли кривые для объединения (слива) \en Whether the curves for union (joining) are similar

  void    SetRadiusA( double aa ) { a = aa; Refresh(); }     ///< \ru Установить большую полуось. \en Set the major semiaxis. 
  void    SetRadiusB( double bb ) { b = bb; Refresh(); }     ///< \ru Установить малую полуось. \en Set the minor semiaxis. 
  void    SetRadius( double r ) { a = r; b = r; Refresh(); } ///< \ru Установить радиус окружности. \en Set circle radius. 
  double  GetRadiusA() const { return a; }                   ///< \ru Получить большую полуось. \en Get the major semiaxis. 
  double  GetRadiusB() const { return b; }                   ///< \ru Получить малую полуось. \en Get the minor semiaxis. 

  void    SetLimitPoint( ptrdiff_t number, const MbCartPoint3D & );                  ///< \ru Заменить начальную (1) или конечную (2) точку дуги. \en Replace a start (1) or end (2) point of the arc. 
  double  GetAngle() const { return (trim2 - trim1); }                               ///< \ru Выдать граничный угол дуги \en Get the end angle of the arc. 
  void    SetAngle ( double ang ) { trim2 = trim1 + ang; CheckClosed(); Refresh(); } ///< \ru Изменить граничный угол дуги. \en Change the end angle of the arc. 

  bool    IsCircle( double eps = METRIC_REGION ) const; ///< \ru Является ли дуга эллипса дугой окружности. \en Whether the arc of an ellipse is an arc of a circle. 
          
  inline  double  CheckParam( double & t ) const;      ///< \ru Установить параметр в область допустимых значений \en Set the parameter into the region of the legal values 
  inline  void    ParamToAngle( double & t ) const;    ///< \ru Перевод параметра кривой в угол. \en Convert parameter of curve to the angle. 
  inline  void    AngleToParam( double & t ) const;    ///< \ru Перевод угла кривой в параметр кривой. \en Convert an angle of curve to a parameter of curve. 
  inline  double  GetTrim1() const { return trim1; }   ///< \ru Параметры начальной точки. \en Parameters of start point. 
  inline  double  GetTrim2() const { return trim2; }   ///< \ru Параметры конечной точки. \en Parameters of end point. 
  bool    MakeTrimmed( double t1, double t2 ); ///< \ru Установка параметров усечения с сохранением направления кривой. \en Setting of the parameters of trimming with keeping the curve direction. 
  void    AlignXAxis();                        ///< \ru Повернуть плейсмент круговой дуги так, чтобы ось ox указывала в начальную точку дуги. \en Rotate the placement of a circular arc so as the ox-axis points to the start point of the arc. 

  /// \ru Является ли кривая плоской? \en Whether the curve is planar? 
  bool    IsPlanar( double accuracy = METRIC_EPSILON ) const override;
  /// \ru Заполнить плейсемент, если кривая плоская. \en Fill the placement if a curve is planar. 
  bool    GetPlacement( MbPlacement3D & place, PlanarCheckParams params = PlanarCheckParams() ) const override;

  const   MbPlacement3D & GetPlacement() const { return position; }
          MbPlacement3D & SetPlacement()       { return position; }
          void   SetPlacement( const MbPlacement3D & pl ) { position = pl; }

  void    GetCentre( MbCartPoint3D & ) const override;
  void    GetWeightCentre( MbCartPoint3D & ) const override;

  // \ru Продлить кривую. \en Extend the curve. \~
  MbResultType Extend( const MbCurveExtensionParameters3D & parameters, c3d::SpaceCurveSPtr & resCurve ) const override;

  bool    Normalize(); ///< \ru Ортонормировать локальную систему координат. \en Orthonormalize the local coordinate system. 
  bool    IsPositionNormal()    const { return ( !position.IsAffine()   ); }
  bool    IsPositionCircular()  const { return (  position.IsCircular() ); }
  bool    IsPositionIsotropic() const { return (  position.IsIsotropic()); }

  const   MbCartPoint3D & GetCentre() const { return position.GetOrigin(); }

private:
  void    CheckClosed(); ///< \ru Проверить и установить признак замкнутости кривой. \en Check and set attribute of curve closedness. 

private:
  void    operator = ( const MbArc3D & ); // \ru Не реализовано. \en Not implemented. 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbArc3D )
};

IMPL_PERSISTENT_OPS( MbArc3D )


//------------------------------------------------------------------------------
// \ru Установить параметр в область допустимых значений \en Set the parameter into the region of the legal values 
// ---
inline
double MbArc3D::CheckParam( double & t ) const
{
  double tMax = trim2 - trim1;
  if ( (t < 0.0) || (t > tMax) ) {
    if ( closed ) 
      t -= ::floor( t * Math::invPI2 ) * M_PI2;
    else if ( t < 0.0 )
      t = 0.0;
    else if ( t > tMax )
      t = tMax;
  }
  double w = t;
  if ( ::fabs(trim1) > NULL_EPSILON ) {
    w = trim1 + w;
    if ( (w < 0.0) || (w > M_PI2) ) 
      w -= ::floor( w * Math::invPI2 ) * M_PI2;
  }
  return w;
}


//------------------------------------------------------------------------------
// \ru Перевод параметра кривой в угол \en Convert parameter of curve to the angle 
// ---
inline
void MbArc3D::ParamToAngle( double & t ) const
{
  if ( ::fabs(trim1) > NULL_EPSILON ) {
    t = trim1 + t;
    if ( (t < 0.0) || (t > M_PI2) ) 
      t -= ::floor( t * Math::invPI2 ) * M_PI2;
  }
}


//------------------------------------------------------------------------------
// \ru Перевод угла кривой в параметр кривой \en Convert an angle of curve to a parameter of curve 
// ---
inline
void MbArc3D::AngleToParam( double & t ) const
{
  if ( ::fabs(trim1) > NULL_EPSILON ) {
    double dtr = ( trim2 + trim1 - M_PI2 ) * 0.5;
    t -= ::floor( (t - dtr) * Math::invPI2 ) * M_PI2; // SKIP_SA
    t = t - trim1;
  }
}


#endif // __CUR_ARC3D_H
