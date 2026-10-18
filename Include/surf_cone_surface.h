////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Коническая поверхность.
         \en Conical surface. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __SURF_CONE_SURFACE_H
#define __SURF_CONE_SURFACE_H


#include <surf_elementary_surface.h>
#include <utility>


class    MbLine3D;
class    MbLineSegment3D;


//------------------------------------------------------------------------------
/** \brief \ru Коническая поверхность.
           \en Conical surface. \~
  \details \ru Коническая поверхность описывается радиусом radius, высотой height и углом конусности angle, заданными в локальной системе координат position. \n
    Первый параметр поверхности отсчитывается по дуге от оси position.axisX в направлении оси position.axisY. 
    Первый параметр поверхности u принимает значения на отрезке: umin<=u<=umax. 
    Значения u=0 и u=2pi соответствуют точке на плоскости XZ локальной системы координат. 
    Поверхность может быть замкнутой по первому параметру. 
    У замкнутой поверхности umax-umin=2pi, у не замкнутой поверхности umax-umin<2pi. \n
    Второй параметр поверхности отсчитывается по прямой вдоль оси position.axisZ. 
    Второй параметр поверхности v принимает значения на отрезке: vmin<=v<=vmax. 
    Значение v=0 соответствует точке плоскости XY локальной системы координат, 
    а значение v=1 соответствует точке на расстоянии height от плоскости XY локальной системы координат поверхности. \n
    Радиус-вектор поверхности описывается векторной функцией \n
    r(u,v) = position.origin + ((radius + height v tg(angle)) (cos(u) position.axisX + sin(u) position.axisY)) + (height v position.axisZ). \n
    Полюсу поверхности соответствует значение второго параметра v=–radius / (height tg(angle)). 
    Граничные параметры vmax и vmin принимают такие значения, при которых поверхность располагается с одной стороны от полюса. \n
    Локальная система координат position может быть как правой, так и левой. 
    Если локальная система координат правая, то нормаль направлена в сторону выпуклости поверхности (от оси position.axisZ), 
    если локальная система координат левая, то нормаль направлена в сторону вогнутости поверхности (в сторону оси position.axisZ).\n
           \en Conical surface is described by 'radius' radius, 'height' height and 'angle' angle of conicity given in 'position' local coordinate system. \n
    The first parameter of surface is measured along arc from position.axisX axis in the direction of position.axisY axis. 
    The first parameter u of surface possesses the values in the range: umin<=u<=umax. 
    Values u=0 and u=2pi correspond to point on XZ plane of local coordinate system. 
    Surface can be closed by first parameter. 
    In case of closed surface: umax-umin=2pi; in case of open surface: umax-umin<2pi. \n
    Second parameter of surface is measured by line along position.axisZ axis. 
    Second parameter v of surface possesses the values in the range: vmin<=v<=vmax. 
    Value v=0 corresponds to point on XY plane of local coordinate system, 
    but value v=1 corresponds to point at 'height' distance from XY plane of local coordinate system of surface. \n
    Radius-vector of surface is described by the vector function \n
    r(u,v) = position.origin + ((radius + height v tg(angle)) (cos(u) position.axisX + sin(u) position.axisY)) + (height v position.axisZ). \n
    Value of second parameter v=-radius / (height tg(angle)) corresponds to pole of surface. 
    Boundary parameters vmax and vmin possess such values which the surface is placed on the one side from a pole at. \n
    Local coordinate system 'position' can be both right and left. 
    If local coordinate system is right then normal is directed to the side of convexity of surface (from position.axisZ axis), 
    If local coordinate system is left then normal is directed to the side of concavity of surface (to position.axisZ axis).\n \~
  \ingroup Surfaces
*/
// ---
class MATH_CLASS MbConeSurface : public MbElementarySurface  {
private:
  double radius;   ///< \ru Радиус в плоскости XY локальной системы координат. \en Radius in the XY plane of a local coordinate system. 
  double angle;    ///< \ru Угол между осью position.axisZ и боковой образующей. \en Angle between position.axisZ axis and lateral generatrix. 
  double height;   ///< \ru Высота конуса. \en Height of cone. 
  double tgAngleH; ///< \ru Вспомогательная величина height*tan(angle). \en Auxiliary value height*tan(angle). 
  bool   uclosed;  ///< \ru Признак замкнутости по первому параметру. \en Attribute of closedness by first parameter. 

protected:
  explicit MbConeSurface( const MbConeSurface & init );
public:
  /// \ru Конструктор по локальной системе координат, радиусу, углу и высоте. \en Constructor by a local coordinate system, radius, angle and height. 
  explicit MbConeSurface( const MbPlacement3D & pl, double r, double a, double h );
  
  /** \brief \ru Конструктор по радиусу, углу, высоте, локальной системе координат, минимальному и максимальному параметрам по V.
             \en Constructor by radius, angle, height, local coordinate system, minimal and maximal parameters by V. \~
    \details \ru Конструктор по радиусу, углу, высоте, локальной системе координат, минимальному и максимальному параметрам по V.
             \en Constructor by radius, angle, height, local coordinate system, minimal and maximal parameters by V. \~
    \warning \ru Используется только в конвертерах.
             \en Used only in converters. \~
  */ 
  explicit MbConeSurface( double r, double a, double h, const MbPlacement3D & pl, double v1, double v2 ); 
  
  /** \brief \ru Конструктор по радиусу, высоте, локальной системе координат, тангенсу угла, минимальному и максимальному параметрам по V.
             \en Constructor by radius, height, local coordinate system, tangent of angle, minimal and maximal parameters by V. \~
    \details \ru Конструктор по радиусу, высоте, локальной системе координат, тангенсу угла, минимальному и максимальному параметрам по V.
             \en Constructor by radius, height, local coordinate system, tangent of angle, minimal and maximal parameters by V. \~
    \warning \ru Используется только в конвертерах.
             \en Used only in converters. \~
  */ 
  explicit MbConeSurface( double r, double h, const MbPlacement3D & pl, double tgAngle, double v1, double v2 ); // \ru Используется только в конверторах \en Used only in converters 
  
  /** \brief \ru Конструктор по трем точкам.
             \en Constructor by three points. \~
    \details \ru Конструктор по трем точкам.\n
      Вектор из точки point0 в точку point1 определяет ось Z.\n
      Вектор из точки point0 в точку point2 определяет направление оси X.\n
      Радиус в начале локальной системы координат равен нулю.\n
      Угол point1 point0 point2 - полуугол конуса.
             \en Constructor by three points.\n
      Vector from point0 point to point1 point determines Z-axis.\n
      Vector from point0 point to point2 point determines direction of X-axis.\n
      Radius at origin of local coordinate system is equal to zero.\n
      Angle point1 point0 point2 - semi-angle of cone. \~
  */
  explicit MbConeSurface( const MbCartPoint3D & point0, const MbCartPoint3D & point1, const MbCartPoint3D & point2 );
  
  /** \brief \ru Конструктор по отрезку и точке.
             \en Constructor by segment and point. \~
    \details \ru Конструктор по отрезку и точке.\n
      Ось конуса определяется отрезком seg.\n
      Высота конуса равна длине отрезка seg.\n
      Конус проходит через точку point.\n
             \en Constructor by segment and point.\n
      Axis of cone is determined by 'seg' segment.\n
      Height of cone is equal to length of 'seg' segment.\n
      Cone passes through 'point' point.\n \~      
  */
  explicit MbConeSurface( const MbLineSegment3D & seg, const MbCartPoint3D & point );

public:
  virtual ~MbConeSurface();

public:
  VISITING_CLASS( MbConeSurface );

  /** \ru \name Функции инициализации
      \en \name Initialization functions
      \{ */
          /// \ru Инициализация по конической поверхности. \en Initialization by conical surface. 
  void    Init( const MbConeSurface & init ); 
          /// \ru Инициализация по локальной системе координат, радиусу, высоте и углу. \en Initialization by a local coordinate system, radius, height and angle. 
  void    Init( const MbPlacement3D & plane, double r, double h, double a );
          /// \ru Инициализация по прямой, радиусу, высоте и углу. \en Initialization by a line, radius, height and angle. 
  void    Init( const MbLine3D & line, double r, double h, double a );

  /** \brief \ru Инициализация по отрезку и точке.
              \en Initialization by segment and point. \~
    \details \ru Инициализация по отрезку и точке.\n
      Ось конуса определяется отрезком seg.\n
      Высота конуса равна длине отрезка seg.\n
      Конус проходит через точку point.\n
              \en Initialization by segment and point.\n
      Axis of cone is determined by 'seg' segment.\n
      Height of cone is equal to length of 'seg' segment.\n
      Cone passes through 'point' point.\n \~      
  */
  void    Init( const MbLineSegment3D & seg, const MbCartPoint3D & point );
          
  /** \brief \ru Инициализация по прямой и отрезку.
              \en Initialization by a line and a segment. \~
    \details \ru Инициализация по прямой и отрезку.\n
      В случае успеха получим конус, являющийся поверхностью вращения\n
      отрезка seg вокруг прямой line.
              \en Initialization by a line and a segment.\n
      In case of success will be obtained a cone being a surface of rotation\n
      of 'seg' segment around 'line' line. \~
  */
  bool    Init( const MbLine3D & line, const MbLineSegment3D & seg );
          
  /** \brief \ru Инициализация по локальной системе координат и отрезку.
              \en Initialization by a local coordinate system and a segment. \~
    \details \ru Инициализация по локальной системе координат и отрезку.\n
      В случае успеха получим конус, являющийся поверхностью вращения\n
      отрезка seg вокруг оси Z локальной системы координат plane.
              \en Initialization by a local coordinate system and a segment.\n
      In case of success will be obtained a cone being a surface of rotation\n
      of 'seg' segment around Z-axis of 'plane' local coordinate system. \~
  */
  bool    Init( const MbPlacement3D & plane, const MbLineSegment3D & seg );
  /** \} */   
  /** \ru \name Общие функции геометрического объекта
      \en \name Common functions of a geometric object
      \{ */
  MbeSpaceType  IsA () const override; // \ru Тип элемента \en A type of element 
  MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента. \en Create a copy of the element.
  bool    IsSame   ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const override; // \ru Равны ли объекты. \en Whether the objects are equal. 
  bool    SetEqual ( const MbSpaceItem & init ) override;       // \ru Сделать равным. \en Make equal.
  void    Transform( const MbMatrix3D & matr, MbRegTransform * = nullptr ) override; // \ru Преобразовать элемент согласно матрице. \en Transform element according to the matrix.
  
  void    GetProperties( MbProperties & properties ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  void    SetProperties( const MbProperties & properties ) override; // \ru Записать свойства объекта. \en Set properties of the object. 
  /** \} */
  /** \ru \name Функции описания области определения поверхности
      \en \name Functions for surface domain description
      \{ */
  double  GetUMin() const override;
  double  GetVMin() const override;
  double  GetUMax() const override;
  double  GetVMax() const override;
  bool    IsUClosed() const override;
  bool    IsVClosed() const override;      
  double  GetUPeriod() const override; // \ru Вернуть период для замкнутой функции. \en Return period for closed function. 
  double  GetVPeriod() const override; // \ru Вернуть период для замкнутой функции. \en Return period for closed function. 
  /** \} */
  /** \ru \name Функции для работы в области определения поверхности
            Функции PointOn, Derive... поверхностей корректируют параметры
            при выходе их за пределы прямоугольной области определения параметров.       
      \en \name Functions for working at surface domain
            Functions PointOn, Derive... of surfaces correct parameters
            when they are out of bounds of rectangular domain of parameters.
      \{ */
  void    PointOn  ( double & u, double & v, MbCartPoint3D & ) const override;  // \ru Точка на поверхности. \en The point on the surface. 
  void    DeriveU  ( double & u, double & v, MbVector3D & ) const override;     // \ru Первая производная по u. \en The first derivative with respect to u. 
  void    DeriveV  ( double & u, double & v, MbVector3D & ) const override;     // \ru Первая производная по v. \en The first derivative with respect to v. 
  void    DeriveUU ( double & u, double & v, MbVector3D & ) const override;     // \ru Вторая производная по u. \en The second derivative with respect to u. 
  void    DeriveVV ( double & u, double & v, MbVector3D & ) const override;
  void    DeriveUV ( double & u, double & v, MbVector3D & ) const override;
  void    DeriveUUU( double & u, double & v, MbVector3D & ) const override;
  void    DeriveUUV( double & u, double & v, MbVector3D & ) const override;
  void    DeriveUVV( double & u, double & v, MbVector3D & ) const override;
  void    DeriveVVV( double & u, double & v, MbVector3D & ) const override;
  void    Normal   ( double & u, double & v, MbVector3D & ) const override; // \ru Нормаль. \en Normal. 
  void    NormalU  ( double & u, double & v, MbVector3D & ) const override; // \ru Производная нормали. \en The derivative of normal. 
  void    NormalV  ( double & u, double & v, MbVector3D & ) const override; // \ru Производная нормали. \en The derivative of normal. 
  void    TangentU ( double & u, double & v, MbVector3D & ) const override;
  /** \} */
  /** \ru \name Функции для работы внутри и вне области определения поверхности
            функции _PointOn, _Derive... поверхностей не корректируют
            параметры при выходе их за пределы прямоугольной области определения параметров.
      \en \name Functions for working inside and outside the surface's domain
            functions _PointOn, _Derive... of surfaces don't correct
            parameters when they are out of bounds of rectangular domain of parameters.
      \{ */
  void   _PointOn  ( double u, double v, MbCartPoint3D & ) const override; // \ru Точка на расширенной поверхности. \en The point on the extended surface. 
  void   _DeriveU  ( double u, double v, MbVector3D & ) const override;   // \ru Первая производная по u. \en The first derivative with respect to u. 
  void   _DeriveV  ( double u, double v, MbVector3D & ) const override;   // \ru Первая производная по v. \en The first derivative with respect to v. 
  void   _DeriveUU ( double u, double v, MbVector3D & ) const override;   // \ru Вторая производная по u. \en The second derivative with respect to u. 
  void   _DeriveVV ( double u, double v, MbVector3D & ) const override;   // \ru Вторая производная по v. \en The second derivative with respect to v. 
  void   _DeriveUV ( double u, double v, MbVector3D & ) const override;   // \ru Вторая производная по uv. \en The second derivative with respect to uv. 
  void   _DeriveUUU( double u, double v, MbVector3D & ) const override;
  void   _DeriveUUV( double u, double v, MbVector3D & ) const override;
  void   _DeriveUVV( double u, double v, MbVector3D & ) const override;
  void   _DeriveVVV( double u, double v, MbVector3D & ) const override;
  void   _Normal   ( double u, double v, MbVector3D & ) const override; // \ru Нормаль. \en Normal. 
  void   _NormalU  ( double u, double v, MbVector3D & ) const override; // \ru Производная нормали. \en The derivative of normal. 
  void   _NormalV  ( double u, double v, MbVector3D & ) const override; // \ru Производная нормали. \en The derivative of normal. 
  void   _NormalUU ( double u, double v, MbVector3D & ) const override;
  void   _NormalUV ( double u, double v, MbVector3D & ) const override;
  void   _NormalVV ( double u, double v, MbVector3D & ) const override;
  void   _TangentU ( double u, double v, MbVector3D & ) const override;
  /** \} */
  /** \ru \name Функции доступа к группе данных для работы внутри и вне области определения параметров поверхности.
      \en \name Functions for get of the group of data inside and outside the surface's domain of parameters.
      \{ */
  void    Explore( double & u, double & v, bool ext,
                   MbCartPoint3D & pnt, MbVector3D & uDer, MbVector3D & vDer,
                   MbVector3D * uuDer, MbVector3D * vvDer, MbVector3D * uvDer, MbVector3D * nor ) const override;
  virtual void   _PointNormal( double u, double v,
                               MbCartPoint3D & pnt, MbVector3D & deru, MbVector3D & derv,
                               MbVector3D & norm, MbVector3D & noru, MbVector3D & norv,
                               MbVector3D & deruu, MbVector3D & dervv, MbVector3D & deruv ) const override; // \ru Значения производных в точке. \en Values of derivatives at point. 
  /** \} */
  /** \ru \name Функции движения по поверхности
      \en \name Functions of moving along the surface
      \{ */
  double  StepU         ( double u, double v, double sag ) const override; // \ru Вычисление шага по u по заданной стрелке прогиба. \en Calculation of the parameter step in u direction by the sag. 
  double  StepV         ( double u, double v, double sag ) const override; // \ru Вычисление шага по v по заданной стрелке прогиба. \en Calculation of the parameter step in v direction by the sag. 
  double  DeviationStepU( double u, double v, double angle ) const override; // \ru Вычисление шага по u по заданному углу отклонения. \en Calculation of the parameter step in u direction by the deviation angle. 
  double  DeviationStepV( double u, double v, double angle ) const override; // \ru Вычисление шага по v по заданному углу отклонения. \en Calculation of the parameter step in v direction by the deviation angle. 
  double  MetricStepU   ( double u, double v, double length ) const override; // \ru Вычисление шага по u по заданной длине. \en Calculation of the parameter step in u direction by the given length. 
  double  MetricStepV   ( double u, double v, double length ) const override; // \ru Вычисление шага по v по заданной длине. \en Calculation of the parameter step in v direction by the given length. 
  size_t  GetUCount() const override;
  size_t  GetVCount() const override;
  /** \} */
  /** \ru \name Общие функции поверхности
      \en \name Common functions of surface
      \{ */
  double  CurvatureU ( double u, double v ) const override; // \ru Кривизна вдоль u. \en Curvature along u. 
  double  CurvatureV ( double u, double v ) const override; // \ru Кривизна вдоль v. \en Curvature along v. 

  MbSplineSurface * NurbsSurface( double, double, double, double, bool bmatch = false ) const override; // \ru NURBS копия поверхности. \en NURBS copy of a surface.
  MbSurface *       NurbsSurface( const MbNurbsParameters & uParam, const MbNurbsParameters & vParam ) const override;
  MbSurface *       Offset( double d, bool same ) const override; // \ru Построить смещенную поверхность. \en Create a shifted surface.

  MbCurve3D * CurveU ( double v, MbRect1D * pRgn, bool bApprox = true ) const override; // \ru Пространственная копия линии v = const. \en Spatial copy of 'v = const'-line.
  MbCurve3D * CurveV ( double u, MbRect1D * pRgn, bool bApprox = true ) const override; // \ru Пространственная копия линии u = const. \en Spatial copy of 'u = const'-line.
  MbCurve3D * CurveUV( const MbLineSegment &, bool bApprox = true ) const override; // \ru Пространственная копия линии по параметрической линии. \en Spatial copy of line by parametric line.

  // \ru Найти все проекции точки на поверхность вдоль вектора в любом из двух направлений. \en Find all a point projection onto the surface along a vector in either of two directions. 
  void    DirectPointProjection( const MbCartPoint3D & p, const MbVector3D & vect, SArray<MbCartPoint> & uv, bool ext, MbRect2D * uvRange = nullptr ) const override;
  // \ru Пересечение с кривой. \en Intersection with curve. 
  void    CurveIntersection( const MbCurve3D & curv, SArray<MbCartPoint> & uv, SArray<double> & tt,
                             bool ext0, bool ext, bool touchInclude = false ) const override;

  // \ru Дать мимнимально различимую величину параметра. \en Get the minimum distinguishable value of parameter.    
  double  GetParamPrice() const override;
  // \ru Построить касательные и нормальные плейсменты конструктивных плоскостей. \en Construct tangent and normal placements of constructive planes. 
  bool    CreateNormalPlacements ( const MbVector3D & axisZ, double angle, SArray<MbPlacement3D> & places, VERSION version = Math::DefaultMathVersion() ) const override;
  bool    CreateTangentPlacements( const MbVector3D & axisZ, SArray<MbPlacement3D> & places ) const override;
  bool    GetCylinderAxis( MbAxis3D & axis ) const override; // \ru Дать ось вращения для поверхности. \en Get a rotation axis of a surface. 
  bool    GetCenterLines( std::vector<MbCurve3D *> & clCurves ) const override; // \ru Дать осевые (центральные) линии для поверхности. \en Get center lines of a surface.
  // \ru Подобные ли поверхности для объединения (слива). \en Whether the surfaces to union (joining) are similar. 
  bool    IsSimilarToSurface( const MbSurface & surf, VERSION version, double precision = METRIC_PRECISION ) const override;
  // \ru Дать двумерную матрицу преобразования из своей параметрической области в параметрическую область surf. \en Get two-dimensional transformation matrix from own parametric region to parametric region of 'surf'. 
  bool    GetMatrixToSurface( const MbSurface & surf, MbMatrix & matr, VERSION version, double precision = METRIC_PRECISION ) const override;
  double  GetFilletRadius( const MbCartPoint3D & p, double accuracy = METRIC_REGION ) const override; // \ru Является ли поверхность скруглением. \en Whether the surface is fillet. 
  MbeParamDir GetFilletDirection( double accuracy = METRIC_REGION ) const override; // \ru Направление поверхности скругления. \en Direction of fillet surface.
  ThreeStates Salient() const override; // \ru Выпуклая ли поверхность. \en Whether the surface is convex.

  double  GetUParamToUnit() const override; // \ru Дать приращение параметра u, соответствующее единичной длине в пространстве. \en Get increment of u-parameter, corresponding to the unit length in space. 
  double  GetVParamToUnit() const override;  // \ru Дать приращение параметра v, соответствующее единичной длине в пространстве. \en Get increment of v-parameter, corresponding to the unit length in space.
  double  GetUParamToUnit( double u, double v ) const override; // \ru Дать приращение параметра u, соответствующее единичной длине в пространстве. \en Get increment of u-parameter, corresponding to the unit length in space.
  double  GetVParamToUnit( double u, double v ) const override; // \ru Дать приращение параметра v, соответствующее единичной длине в пространстве. \en Get increment of v-parameter, corresponding to the unit length in space.
  void    GetParamsToUnit( double u, double v, double & uParam, double & vParam ) const override; // \ru Дать приращение параметра u и параметра v, соответствующее единичной длине в пространстве. \en Get increment of parameters, corresponding to the unit length in space.
  
  // \ru Определение разбивки параметрической области поверхности вертикалями и горизонталями. \en Determine splitting of parametric region of surface by vertical and horizontal lines. 
  void    GetTesselation( const MbStepData & stepData,
                          double u1, double u2, double v1, double v2,
                          SArray<double> & uu, SArray<double> & vv ) const override;

  void    CalculateGabarit( MbCube & ) const override; // \ru Рассчитать габарит поверхности. \en Calculate bounding box of surface. 
  void    CalculateLocalGabarit( const MbMatrix3D &, MbCube & ) const override; // \ru Рассчитать габарит относительно л.с.к. \en Calculate bounding box relative to the local coordinate system. 

  bool    SetLimit( double u1, double v1, double u2, double v2 ) override;
  void    SetExtendedParamRegion( double u1, double v1, double u2, double v2 ) override;

  void    IncludePoint( double u, double v ) override; // \ru Включить точку в область определения. \en Include point into domain.
  // \ru Существует ли полюс на границе параметрической области. \en Whether there is pole on boundary of parametric region. 
  bool    GetPoleVMin() const override;
  bool    GetPoleVMax() const override;
  bool    IsPole( double u, double v, double paramPrecision = PARAM_PRECISION ) const override; // \ru Является ли точка особенной. \en Whether the point is special. 

  size_t  GetUMeshCount() const override; // \ru Выдать количество полигонов по u. \en Get the count of polygons by u. 
  size_t  GetVMeshCount() const override; // \ru Выдать количество полигонов по v. \en Get the count of polygons by v. 

  bool    IsLineV() const override; // \ru Если true все производные по V выше первой равны нулю. \en If true, then all the derivatives by V higher the first one are equal to zero. 

  // \ru Является ли объект смещением. \en Is the object is a shift? 
  bool    IsShift( const MbSpaceItem &, MbVector3D &, bool & isSame, double accuracy = LENGTH_EPSILON ) const override;

  double  GetRadius() const override; // \ru Дать максимальный физический радиус объекта или ноль, если это невозможно. \en Get the maximum physical radius of the object or null if it impossible.
  /** \} */  
  /** \ru \name Функции элементарных поверхностей
      \en \name Functions of elementary surfaces 
      \{ */
  bool    GetPointProjection( const MbCartPoint3D & p, bool init, double & u, double & v, bool ext, MbRect2D * uvRange = nullptr ) const override;
  /** \} */  
  /** \ru \name Функции конической поверхности
      \en \name Functions of conical surface
      \{ */
  /// \ru Получить внутренний радиус основания. \en Get internal radius of base. 
  double  GetR() const { return radius; } 
  /// \ru Получить текущий внутренний радиус для параметра v без ограничений vmin, vmax. \en Get current internal radius for v parameter without constraints of vmin, vmax. 
  double  GetR( double v ) const { return radius + tgAngleH * v; } 
  /// \ru Получить физический радиус. \en Get physical radius. 
  double  GetRadius( double v ) const; 
  /// \ru Получить внутренний радиус для параметра v, равного 1.0. \en Get internal radius for v parameter equal to 1.0.  
  double  GetUpperR() const { return radius + tgAngleH; } 
  /// \ru Установить внутренний радиус. \en Set an internal radius. 
  void    SetR( double r ) { radius = r; } 

  /// \ru Установить угол. \en Set an angle. 
  void    SetAngle ( const double & a ) { angle  = a; tgAngleH = ( height * ::tan(a    ) ); } 
  /// \ru Установить внутреннюю высоту. \en Set internal height. 
  void    SetHeight( const double & h ) { height = h; tgAngleH = ( h      * ::tan(angle) ); C3D_ASSERT( ::fabs(height) > LENGTH_EPSILON ); } 
  /// \ru Угол. \en Angle. 
  double  GetAngle () const { return angle;  } 
  /** \brief \ru Внутренняя высота.
              \en Internal height. \~
      \details \ru Внутренняя высота. \n
                    Чтобы получить физическую высоту нужно внутреннюю высоту умножить 
                    на параметрическую длину по V и 
                    длину оси Z ЛСК поверхности. \n
                \en Internal height. \n
                    To obtain the physical height you need to multiply the internal height 
                    by the parametric length along V and 
                    the length of the Z axis of the local coordinate system of the surface. \~
  */
  double  GetHeight() const { return height; } 
  /// \ru Выдать физисечкую высоту. \en Get physical height. \~
  double  GetRealHeight() const { return ( height * (vmax - vmin) * position.GetAxisZ().Length() ); } 
  /// \ru Тангенс угла, умноженный на внутреннюю высоту. \en Tangent of the angle multiplied by internal height.        
  double  GetTgAngleH() const { return tgAngleH; } 
 
  /** \brief \ru Проверка параметра v по отношению к полюсу.
             \en Check v parameter against pole. \~
    \details \ru Проверка параметра v по отношению к полюсу.
             \en Check v parameter against pole. \~
    \return \ru true, если параметр v был изменен, чтобы не уйти за полюс.
            \en True if v parameter was changed not to leave out of pole. \~
  */
  inline  bool    CheckVParam( double & v ) const; 
  /** \brief \ru Получить v-параметр полюса.
             \en Get v parameter of pole. \~
    \details \ru Получить v-параметр полюса.\n    
      Может быть вне области определения.
             \en Get v-parameter of pole.\n    
      Can be outside of domain. \~
  */
  inline  double  GetVPole() const;

  /** \brief \ru Находится ли полюс внутри диапазона (v1,v2).
             \en Whether pole is inside range (v1,v2). \~
    \details \ru Находится ли полюс внутри диапазона (v1,v2). \n
             \en Whether pole is inside range (v1,v2).\n \~
  */
  inline  bool   IsVPoleInside( double v1, double v2, double metricAcc ) const;

  /** \} */

private:
  inline  void    CheckParam( double & u, double & v ) const; // \ru Проверка параметров. \en Check parameters. 
          // \ru Пересечение с прямолинейной кривой. \en Intersection with rectilinear curve. 
  bool    StraightIntersection( const MbCurve3D & curv, SArray<MbCartPoint> & uv, SArray<double> & tt, bool ext0, bool ext ) const;
  void    operator = ( const MbConeSurface & ); // \ru Не реализовано. \en Not implemented. 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbConeSurface )
}; // MbConeSurface

IMPL_PERSISTENT_OPS( MbConeSurface )


//------------------------------------------------------------------------------
// \ru Проверка параметров \en Check parameters 
// ---
inline
void MbConeSurface::CheckParam( double & u, double & v ) const
{
  if ( (u < umin) || (u > umax) ) {
    if ( uclosed ) 
      u -= ::floor( (u - umin) * Math::invPI2 ) * M_PI2;
    else if ( u < umin )
      u = umin;
    else if ( u > umax )
      u = umax;
  }
  if ( v < vmin )
    v = vmin;
  else if ( v > vmax )
    v = vmax;
}


//------------------------------------------------------------------------------
// \ru Проверка параметра v по отношению к полюсу \en Check v parameter against pole 
// ---
inline
bool MbConeSurface::CheckVParam( double & v ) const
{
  bool res = true;

  if ( v < vmin || v > vmax ) {
    double vPole = GetVPole();
    if ( ((v < vPole - EXTENT_EPSILON) && (vmin > vPole - EXTENT_EPSILON)) ||
         ((v > vPole + EXTENT_EPSILON) && (vmax < vPole + EXTENT_EPSILON)) ) {
      v = vPole;
      res = false; // \ru Изменили параметр v, чтобы не уйти за полюс \en V parameter was changed not to leave out of pole 
    }
  }

  return res;
}


//------------------------------------------------------------------------------
// \ru Получить v-параметр полюса \en Get v parameter of pole 
// ---
inline
double MbConeSurface::GetVPole() const {
  if ( (-DOUBLE_EPSILON < tgAngleH) && (tgAngleH < DOUBLE_EPSILON) ) {  // Избежать деления на 0.
    return ( tgAngleH * radius > 0.0 ? -MAX_OVERALL_DIM : MAX_OVERALL_DIM );
  }

  return -radius / tgAngleH;
}


//------------------------------------------------------------------------------
// \ru Получить v-параметр полюса \en Get v parameter of pole 
// ---
inline
bool MbConeSurface::IsVPoleInside( double v1, double v2, double vAcc ) const
{
  bool res = false;
  if ( v1 > v2 )
    std::swap( v1, v2 );
  double vPole = GetVPole();
  if ( vPole > v1 + vAcc && vPole < v2 - vAcc )
    res = true;
  return res;
}


#endif // __SURF_CONE_SURFACE_H
