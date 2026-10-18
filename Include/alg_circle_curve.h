////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Построение окружности, вычисление центра окружности.
         \en Circle construction, center of circle calculation. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __ALG_CIRCLE_CURVE_H
#define __ALG_CIRCLE_CURVE_H

#include <mb_cart_point.h>
#include <alg_base.h>
#include <alg_curve_distance.h>


class  MbCurve;
class  MbLine;
class  MbArc;


//------------------------------------------------------------------------------
/** \brief \ru Вспомогательная окружность.
           \en Auxiliary circle. \~
  \details \ru Вспомогательная окружность, заданная центром и радиусом. \n
           \en Auxiliary circle, defined by center and radius. \n \~
  \ingroup Data_Structures
*/
// ---
class MATH_CLASS MbTempCircle {
private:
  MbCartPoint centre; ///< \ru Центр. \en Center. 
  double      radius; ///< \ru Радиус. \en Radius. 

public :   
 
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор окружности нулевого радиуса с центром в начале координат.\n
             \en Constructor of a circle with zero radius, centered at the origin of coordinate system.\n \~ 
  */
  MbTempCircle() 
    : centre()
    , radius( 0 ) 
  {};

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор окружности с центром в начале координат.\n
             \en Constructor of a circle, centered at the origin of coordinate system.\n \~ 
    \param[in] rad - \ru Радиус окружности.
                     \en Radius of circle. \~
  */
  MbTempCircle( double rad ) 
    : centre()
    , radius( rad ) 
  {};

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по центру и радиусу.\n
             \en Constructor by center and radius.\n \~ 
    \param[in] p   - \ru Центр окружности.
                     \en Center of circle. \~
    \param[in] rad - \ru Радиус окружности.
                     \en Radius of circle. \~
  */
  MbTempCircle( const MbCartPoint & p, double rad ) 
    : centre( p )
    , radius( rad ) 
  {};

  /// \ru Копирующий конструктор. \en Copy-constructor. 
  MbTempCircle( const MbTempCircle & other ) 
    : centre( other.centre)
    , radius( other.radius ) 
  {};
  
public :
  ~MbTempCircle() {}

  /**\ru \name Функции инициализации.
     \en \name Initialization functions.
  \{ */

        /** \brief \ru Инициализация.
                   \en Initialization. \~
          \details \ru Инициализация по окружности.\n
                   \en Initialization by circle.\n \~ 
          \param[in] other - \ru Окружность.
                             \en Circle. \~
        */
        void          Init( const MbTempCircle & other ) { centre = other.centre; radius = other.radius; }
        
        /** \brief \ru Инициализация.
                   \en Initialization. \~
          \details \ru Инициализация по центру и радиусу.\n
                   \en Initialization by center and radius.\n \~ 
          \param[in] p   - \ru Центр.
                           \en Center. \~
          \param[in] rad - \ru радиус.
                           \en radius. \~
        */
        void          Init( const MbCartPoint & p, double rad ) { centre = p; radius = rad; }

  /** \} */   
  /**\ru \name Функции доступа к данным.
     \en \name Functions for access to data.
      \{ */ 

  const MbCartPoint & GetCentre() const { return centre; }   ///< \ru Центр окружности. \en Center of circle. 
  const double      & GetR()const { return radius; }         ///< \ru Радиус окружности. \en Radius of circle. 

  /** \} */   
  /**\ru \name Функции изменения данных.
     \en \name Functions for changing data.
      \{ */ 

        MbCartPoint & SetCentre() { return centre; }                      ///< \ru Выдать центр окружности для изменения. \en Get center of circle for editing.                   
        void          SetCentre( const MbCartPoint & c ) { centre = c; }  ///< \ru Изменить центр окружности. \en Set center of circle.  

        double      & SetRadius() { return radius; }                      ///< \ru Выдать радиус окружности для изменения. \en Get radius of circle for editing.                  
        void          SetRadius( double r ) { radius = r; }               ///< \ru Изменить радиус окружности. \en Set radius of circle. 
  
  /** \} */   
  /**\ru \name Функции расчета данных.
     \en \name Functions for calculating data.
      \{ */ 

        /** \brief \ru Точка на окружности.
                   \en Point on circle. \~
          \details \ru Точка на окружности по параметру.\n
                   \en Point on circle by parameter.\n \~ 
          \param[in]  t - \ru Параметр на окружности.
                          \en Parameter on circle. \~
          \param[out] p - \ru Точка на окружности.
                          \en Point on circle. \~
        */
        void          PointOn( double t, MbCartPoint & p ) const { 
                        p.x = ( centre.x + radius * ::cos(t) );
                        p.y = ( centre.y + radius * ::sin(t) );
                      }

        /** \brief \ru Первая производная.
                   \en First derivative. \~
          \details \ru Первая производная по параметру.\n
                   \en First derivative by parameter.\n \~ 
          \param[in]  t - \ru Параметр на окружности.
                          \en Parameter on circle. \~
          \param[out] v - \ru Вектор первой производной.
                          \en First derivative vector. \~
        */
        void          FirstDer( double t, MbVector & v ) const { 
                        v.x = -( radius * ::sin(t) );
                        v.y =  ( radius * ::cos(t) );
                      }

        /** \brief \ru Вычислить расстояние до точки.
                   \en Calculate distance to point. \~
          \details \ru Расстояние от окружности до точки.\n
                   \en Distance from circle to point.\n \~ 
          \param[in]  p - \ru Точка.
                          \en Point. \~
          \return \ru Вычислить расстояние до точки.
                  \en Calculate distance to point. \~
        */
        double        DistanceToPoint( const MbCartPoint & p ) const { 
                        return ::fabs( centre.DistanceToPoint(p) - radius );
                      }

        /** \brief \ru Проекция точки.
                   \en Point projection. \~
          \details \ru Проекция точки на окружность.\n
                   \en Point projection on circle.\n \~ 
          \param[in] p - \ru Точка.
                         \en Point. \~
          \return \ru Параметр проекции точки на окружности.
                  \en Parameter of point projection on circle. \~
        */
        double        PointProjection( const MbCartPoint & p ) const { 
                        return c3d::CalcAngle0X( centre, p );
                      }

        /** \brief \ru Лежит ли точка на окружности?
                   \en Is point on circle? \~
          \details \ru Проверка, лежит ли точка на окружности.\n
                   \en Check if the point is on circle.\n \~ 
          \param[in] p   - \ru Точка.
                           \en Point. \~
          \param[in] eps - \ru Погрешность.
                           \en Tolerance. \~
          \return \ru true, если точка лежит на окружности.
                  \en true if the point on circle. \~
        */
        bool          IsPointOn( const MbCartPoint & p, double eps = Math::LengthEps ) const { 
                        return ( DistanceToPoint(p) < eps );
                      }

        /** \brief \ru Проверить на вырожденность.
                   \en Check for degeneracy. \~
          \details \ru Проверить окружность на вырожденность.\n
                   \en Check circle for degeneracy. \~           
          \return \ru true, если окружность вырождена.
                  \en true if the circle is degenerate. \~
        */
        bool          IsDegenerate() const { // \ru проверка вырожденности окружности \en check for circle degeneracy 
                        return ( radius < Math::minRadius || radius > Math::maxRadius );
                      }
  /** \} */ 
private:
        void          operator = ( const MbTempCircle & other ) { centre = other.centre; radius = other.radius; }
};


//------------------------------------------------------------------------------
/** \brief \ru Вычислить центры окружностей.
           \en Calculate centers of circles. \~
  \details \ru Вычислить центры окружностей заданного радиуса rad, касающихся
    двух данных прямых pl1 и pl2.
           \en Calculate centers of circles with fixed radius rad, that touches
    two given lines pl1 and pl2. \~
  \param[in]  pl1 - \ru Первая прямая.
                    \en First line. \~
  \param[in]  pl2 - \ru Вторая прямая.
                    \en Second line \~
  \param[in]  rad - \ru Радиус окружности.
                    \en Radius of circle. \~
  \param[out] pc  - \ru Результат - массив окружностей с искомым центром.
                    \en Result - set of circles with required center. \~
  \return \ru Количество окружностей в массиве.
          \en The number of circles in array. \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (ptrdiff_t) CircleTanLineLineRad( const MbLine & pl1, const MbLine & pl2, double rad, MbTempCircle * pc );


//------------------------------------------------------------------------------
/** \brief \ru Вычислить центры окружностей.
           \en Calculate centers of circles. \~
  \details \ru Вычислить центры окружностей заданного радиуса rad, касающихся
    данных прямой pl1 и окружности pc1.
           \en Calculate centers of circles with fixed radius rad, that touches
    given line pl1 and circle pc1. \~
  \param[in]  pl1 - \ru Прямая.
                    \en Line. \~
  \param[in]  pc1 - \ru Окружность.
                    \en Circle. \~
  \param[in]  rad - \ru Радиус окружностей с искомым центром.
                    \en Result - set of circles with the required center. \~
  \param[out] pc  - \ru Результат - массив окружностей с искомым центром.
                    \en Result - set of circles with the required center. \~
  \return \ru Количество окружностей в массиве.
          \en The number of circles in array. \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (ptrdiff_t) CircleTanLineCircleRadius( const MbLine & pl1, const MbArc & pc1, double rad, 
                                                 MbTempCircle * pc );


//------------------------------------------------------------------------------
/** \brief \ru Вычислить центры окружностей.
           \en Calculate centers of circles. \~
  \details \ru Вычислить центры окружностей заданного радиуса rad, касающихся
    двух окружностей pc1 и pc2.
           \en Calculate centers of circles with fixed radius rad, that touches
    given circles pc1 and pc2. \~
  \param[in]  pc1 - \ru Первая окружность.
                    \en First circle. \~
  \param[in]  pc2 - \ru Вторая окружность.
                    \en Second circle. \~
  \param[in]  rad - \ru Радиус окружностей с искомым центром.
                    \en Radius of circles with the required center. \~
  \param[out] pc  - \ru Результат - массив окружностей с искомым центром.
                    \en Result - set of circles with the required center. \~
  \return \ru Количество окружностей в массиве.
          \en Count of circles in set. \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (ptrdiff_t) CircleTanCircleCircleRad( const MbArc & pc1, const MbArc & pc2, double rad, 
                                                MbTempCircle * pc );


//------------------------------------------------------------------------------
/** \brief \ru Построить окружности.
           \en Create circles. \~
  \details \ru Построить окружности с заданным центром,
    касающиеся заданной кривой.
           \en Create circles with given center,
    that touches given curve. \~
  \param[in]  pCurve  - \ru Кривая, касающаяся окружности.
                        \en Curve, that touches circle. \~
  \param[in]  pnt     - \ru Центр окружности.
                        \en Center of circle. \~
  \param[out] pCircle - \ru Набор окружностей.
                        \en Set of circles. \~   
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (void) CircleTanCurveCentre( const MbCurve & pCurve, const MbCartPoint & pnt,
                                       PArray<MbTempCircle> & pCircle );


//------------------------------------------------------------------------------
/** \brief \ru Построить окружности.
           \en Create circles. \~
  \details \ru Построить окружности, касающиеся заданной кривой,
    проходящие через две заданные точки.
           \en Create circles that touch given curve
    and pass through given two points. \~
  \param[in]  pCurve  - \ru Кривая, касающаяся окружности.
                        \en Curve, that touches circle. \~
  \param[in]  on1     - \ru Точка на окружности.
                        \en Point on circle. \~
  \param[in]  on2     - \ru Точка на окружности.
                        \en Point on circle. \~
  \param[out] pCircle - \ru Набор окружностей.
                        \en Set of circles. \~      
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (void) CircleTangentCurveTwoPoints( const MbCurve &              pCurve,
                                              const MbCartPoint &          on1, 
                                              const MbCartPoint &          on2,
                                                    PArray<MbTempCircle> & pCircle );


//------------------------------------------------------------------------------
/** \brief \ru Построить окружности.
           \en Create circles. \~
  \details \ru Построить окружности, касающиеся заданной кривой,
    с заданным радиусом, проходящие через заданную точку.
           \en Create circles that touch given curve
    and with a given radius, passing through a given point. \~
  \param[in]  pCurve  - \ru Кривая, касающаяся окружности.
                        \en Curve that touches circle. \~
  \param[in]  radius  - \ru Радиус.
                        \en Radius. \~
  \param[in]  on      - \ru Точка на окружности.
                        \en Point on circle. \~
  \param[out] pCircle - \ru Набор окружностей.
                        \en A set of circles. \~        
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (void) CircleTangentCurveRPointOn( const MbCurve &              pCurve, 
                                                   double                 radius,
                                             const MbCartPoint &          on,
                                                   PArray<MbTempCircle> & pCircle );


//------------------------------------------------------------------------------
/** \brief \ru Построить окружности.
           \en Create circles. \~
  \details \ru Построить окружности с заданным радиусом,
    касающиеся двух кривых.
           \en Create circles with a given radius
    that touch two curves. \~
  \param[in]  pCurve1 - \ru Первая кривая, касающаяся окружности.
                        \en The first curve that touches circle. \~  
  \param[in]  pCurve2 - \ru Вторая кривая, касающаяся окружности.
                        \en The second curve, that touches circle. \~ 
  \param[in]  rad     - \ru Радиус.
                        \en Radius. \~
  \param[out] pCircle - \ru Набор окружностей.
                        \en A set of circles. \~  
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (void) CircleTanTwoCurvesRadius( const MbCurve &              pCurve1, 
                                           const MbCurve &              pCurve2, 
                                                 double                 rad,
                                                 PArray<MbTempCircle> & pCircle );


//------------------------------------------------------------------------------
/** \brief \ru Построить окружности.
           \en Create circles. \~
  \details \ru Построить окружности, проходящие через заданную точку,
    касающиеся двух кривых.
           \en Create circles that pass through given point
    and touch two curves. \~
  \param[in]  pCurve1 - \ru Первая кривая, касающаяся окружности.
                        \en The first curve, that touches circle. \~  
  \param[in]  pCurve2 - \ru Вторая кривая, касающаяся окружности.
                        \en The second curve that touches circle. \~  
  \param[in]  pOn     - \ru Точка на окружности.
                        \en Point on circle. \~
  \param[out] pCircle - \ru Набор окружностей.
                        \en A set of circles. \~ 
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (void) CircleTanTwoCurvesPointOn( const MbCurve &              pCurve1, 
                                            const MbCurve &              pCurve2, 
                                            const MbCartPoint &          pOn,
                                                  PArray<MbTempCircle> & pCircle );


//------------------------------------------------------------------------------
/** \brief \ru Построить окружности.
           \en Create circles. \~
  \details \ru Построить окружности с центром на первой кривой,
    касательные ко второй кривой, проходящие через заданную точку.
           \en Create circles with center on the first curve
    that touches second curve and passes through the given point. \~ 
  \param[in]  pCurve1 - \ru Первая кривая, содержащая центр окружности.
                        \en The first curve which contains center of circle. \~  
  \param[in]  pCurve2 - \ru Вторая кривая, касающаяся окружности.
                        \en The second curve that touches circle. \~  
  \param[in]  pp      - \ru Точка на окружности.
                        \en Point on circle. \~
  \param[out] pCircle - \ru Набор окружностей.
                        \en A set of circles. \~ 
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (void) CircleOriginOneTangentTwo( const MbCurve &               pCurve1, 
                                            const MbCurve &               pCurve2, 
                                            const MbCartPoint &           pp,
                                                  RPArray<MbTempCircle> & pCircle );


//------------------------------------------------------------------------------
/** \brief \ru Построить окружности.
           \en Create circles. \~
  \details \ru Построить окружности, касательные заданной кривой,
    проходящие через заданную точку, составляющие в точке касания угол(p1, centre, ptan), 
    равный данному.
           \en Create circles that touch given curve
    and pass through given point, with angle(p1, centre, ptan) at tangent point 
    that equal to the given one. \~ 
  \param[in]  curve   - \ru Кривая, касающаяся окружности.
                        \en Curve that touches the circle. \~  
  \param[in]  p1      - \ru Точка на окружности.
                        \en Point on circle. \~
  \param[in]  angle   - \ru Угол, образованный тремя точками:\n
                        заданной точкой на окружности p1,\n
                        центром окружности,\n
                        точкой касания.
                        \en Angle formed by three points:\n
                        given point on circle p1,\n
                        center of circle,\n
                        tangent point. \~
  \param[out] circles - \ru Набор окружностей.
                        \en A set of circles. \~ 
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (void) CircleTanCurvePointOnAngle( const MbCurve & curve, const MbCartPoint & p1, double angle,
                                             PArray<MbTempCircle> & circles );


//------------------------------------------------------------------------------
/** \brief \ru Построить дуги окружностей.
           \en Create arcs of circles. \~
  \details \ru Построить дуги окружностей по двум точкам,
    касающиеся заданной кривой.
           \en Create arcs of circles by two points
    that touches given curve. \~
  \param[in]  pCurve - \ru Кривая, касающаяся дуг.
                       \en Curve that touches arcs. \~
  \param[in]  on1    - \ru Первая точка на дуге.
                       \en First point on arc. \~
  \param[in]  on2    - \ru Вторая точка на дуге.
                       \en Second point on arc. \~
  \param[out] arc    - \ru Набор дуг.
                       \en Set of arcs. \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (void) ArcTangentCurveTwoPoints( const MbCurve &       pCurve,
                                           const MbCartPoint &   on1, 
                                           const MbCartPoint &   on2,
                                                 PArray<MbArc> & arc ); 


//------------------------------------------------------------------------------
/** \brief \ru Построить дуги окружностей.
           \en Create arcs of circles. \~
  \details \ru Построить дуги окружностей по радиусу и точке,
    касающиеся заданной кривой.
           \en Create arcs of circles by radius and point
    that touch given curve. \~
  \param[in]  pCurve - \ru Кривая, касающаяся дуг.
                       \en Curve that touches arcs. \~
  \param[in]  radius - \ru Радиус.
                       \en Radius. \~
  \param[in]  on     - \ru Точка на дуге.
                       \en Point on arc. \~
  \param[out] arc    - \ru Набор дуг.
                       \en Set of arcs. \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (void) ArcTangentCurveRPointOn( const MbCurve &       pCurve,
                                                double          radius, 
                                          const MbCartPoint &   on,
                                                PArray<MbArc> & arc ); 


//------------------------------------------------------------------------------
/** \brief \ru Построить дугу окружности.
           \en Create a circle arc. \~
  \details \ru Построить дугу окружности, сопряженную с указанной ограниченной кривой
    в начальной или конечной точке. Дуга всегда выходит из кривой.
           \en Create arc of circle conjugated with the given bounded curve
    at the start point or at the end point. An arc always starts at curve. \~
  \param[in]  line - \ru Кривая для сопряжения.
                     \en Curve to conjugate with. \~
  \param[in]  p2   - \ru Точка на дуге.
                     \en Point on arc. \~
  \param[out] arc  - \ru Множество с дугой окружности.
                     \en A set that contains the arc of circle. \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (void) ArcTangentCurveContinue( const MbLine &        line, 
                                          const MbCartPoint &   p2,
                                                PArray<MbArc> & arc ); 


//------------------------------------------------------------------------------
/** \brief \ru Построить дугу окружности.
           \en Create a circle arc. \~
  \details \ru Построить дугу окружности заданного радиуса,
    сопряженной с указанной ограниченной кривой
    в начальной или конечной точке. Дуга всегда выходит из кривой.
           \en Create an arc of circle with the given radius
    conjugated with the given bounded curve
    at the start point or at the end point. An arc always starts at curve. \~
  \param[in]  line - \ru Кривая для сопряжения.
                     \en Curve to conjugate with. \~
  \param[in]  rad  - \ru Радиус.
                     \en Radius. \~
  \param[in]  p2   - \ru Точка на дуге.
                     \en Point on arc. \~
  \param[out] arc  - \ru Множество с дугой окружности.
                     \en A set that contains the arc of circle. \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (void) ArcTangentCurveRadContinue( const MbLine &        line,
                                                   double          rad,
                                             const MbCartPoint &   p2,
                                                   PArray<MbArc> & arc ); 


//------------------------------------------------------------------------------
/** \brief \ru Построить окружности.
           \en Create circles. \~
  \details \ru Построить окружности, касающиеся трех кривых.
           \en Create circles that touch three curves. \~
  \param[in] curve1  - \ru Первая кривая.
                       \en The first curve. \~
  \param[in] curve2  - \ru Вторая кривая.
                       \en The second curve. \~
  \param[in] curve3  - \ru Третья кривая.
                       \en The third curve. \~
  \param[in] pnt     - \ru Точка на перпендикуляре к точке касания кривой.\n
                       Используется для тех кривых их трех перечисленных,
                       которые не являются отрезком, прямой или полилинией.
                       \en A point on perpendicular to curve at the tangent point.\n
                       Used for those of the given three curves
                       which are not segment, line or polyline. \~
  \param[out] circle - \ru Набор окружностей.
                       \en A set of circles. \~
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (void) CircleTanThreeCurves( const MbCurve *              curve1, 
                                       const MbCurve *              curve2, 
                                       const MbCurve *              curve3,
                                       const MbCartPoint &          pnt,
                                             PArray<MbTempCircle> & circle );


//-----------------------------------------------------------------------------
/** \brief \ru Копировать временные окружности
           \en Copy temporary circles \~
  \details \ru Копировать временные окружности.\n
    Очищает временный массив cTmp.
           \en Copy temporary circles.\n
    Clear temporary array cTmp. \~
  \param[in, out]  cTmp    - \ru Набор временных окружностей. Множество очищается.
                             \en Set of temporary circles. Array will be cleared. \~
  \param[out]      pCircle - \ru Набор дуг окружностей, созданных соответственно временным окружностям.
                             \en Set of arcs of circles, created by temporary circles. \~ 
  \ingroup Curve_Modeling
*/
// ---
MATH_FUNC (void) CreateNewCircles( PArray<MbTempCircle> & cTmp, 
                                   PArray<MbArc> &        pCircle );  


#endif // __ALG_CIRCLE_CURVE_H
