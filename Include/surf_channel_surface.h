////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Поверхность скругления с переменным радиусом обычная или с сохранением кромки.
         \en Fillet surface with variable radius is normal or with preservation of edges. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __SURF_CHANNEL_SURFACE_H
#define __SURF_CHANNEL_SURFACE_H


#include <surf_fillet_surface.h>


//------------------------------------------------------------------------------
/** \brief \ru Поверхность скругления с переменным радиусом обычная или с сохранением кромки.
           \en Fillet surface with variable radius is normal or with preservation of edges. \~
  \details \ru Поверхность скругления с переменным радиусом является NURBS-поверхностью,
    построенной по трём кривым: curve1, curve0, curve2. 
    Первый параметр поверхности совпадает с параметром кривых curve1, curve0, curve2. 
    Второй параметр изменяется от нуля (точки совпадают с curve1) до единицы (точки совпадают с curve2).
    Функция function определяет изменение радиуса и равна отношению текущего радиуса к заданному для поверхности радиусу. 
    Параметр функции радиуса совпадает с параметром кривых curve1, curve0, curve2. 
    Если коэффициент формы conic = _ARC_ ( 0 ), то вес каждой точки кривой curve0 задаётся функцией weights0 и
    вычислен так, что сечение поверхности вдоль её второго параметра будет дугой окружности,
    то есть при любом параметре u три точки curve1(u), curve0(u), curve2(u) определяют NURBS-кривую в форме дуги окружности. 
    Если коэффициент формы conic != _ARC_, то вес каждой точки кривой curve0 равен conic / ( 1.0 - conic ). 
    При conic = 0.5 сечение поверхности вдоль её второго параметра будет параболой. \n
           \en Fillet surface with variable radius is NURBS-surface
    constructed on three curves: curve1, curve0, curve2. 
    The first surface parameter coincides with the parameter of curve1, curve0, curve2 curves. 
    The second parameter is changed from zero (points coincide with curve1) to unit (points coincide with curve2).
    Function "function" determines the change of the radius and equals the ratio of the current radius to given for surface. 
    The parameter of radius function coincides with the parameter of curves curve1, curve0, curve2. 
    If coefficient of shape conic = _ARC_ ( 0 ), then the weight of each point of the curve0 curve determined by the function weights0 and
    calculated so that the section of surface along its second parameter is a circular arc 
    i.e. for any parameter u three points of curve1(u), curve0(u), curve2(u) determine the NURBS-curve with the shape of a circular arc. 
    If coefficient of shape conic != _ARC_, then the weight of each point of the curve0 curve is equal to conic / ( 1.0 - conic ). 
    If conic = 0.5, then the surface section along its second parameter is parabola. \n \~
  \ingroup Surfaces
*/
// ---
class MATH_CLASS MbChannelSurface : public MbFilletSurface {
private:
  MbFunction * function1; ///< \ru Функция изменения радиуса (переменный коэффициент). \en Function of change of the radius (variable coefficient). 
  MbFunction * function2; ///< \ru Функция изменения радиуса (переменный коэффициент). \en Function of change of the radius (variable coefficient). 

public:

  /** \brief \ru Конструктор по двум кривым и типу сопряжения.
             \en Constructor by two curves and type of mate. \~ 
    \details \ru Конструктор по двум кривым и типу сопряжения.
             \en Constructor by two curves and type of mate. \~ 
    \param[in] curv1 - \ru Опорная кривая на первой поверхности
                       \en Support curve on the first surface \~ 
    \param[in] curv2 - \ru Опорная кривая на второй поверхности
                       \en Support curve on the second surface \~ 
    \param[in] d1 - \ru Радиус скругления со знаком для поверхности кривой crve1
                    \en Fillet radius with sign for surface of crve1 curve \~
    \param[in] d2 - \ru Радиус скругления со знаком для поверхности кривой crve2
                    \en Fillet radius with sign for surface of crve2 curve \~
    \param[in] fm - \ru Тип сопряжения: \n 
               st_Span - скругление с заданной хордой \n
               st_Fillet - скругление с заданными радиусами
                    \en Mate type: \n 
               st_Span - fillet with a given chord \n
               st_Fillet - fillet with given radii. \~    
    \param[in] cn - \ru Коэффициент формы. Значение 0 соответствует профилю "дуга окружности".
                        Значения от 0.05 до 0.95 соответствуют коническому сечению.
                    \en Coefficient of shape. The value 0 corresponds to a "circular arc" profile.
                        Values between 0.05 and 0.95 represent conic section. \~
    \param[in] func - \ru Функция изменения радиуса
                      \en Function of change of the radius \~
    \param[in] ev - \ru Равномерная параметризация по дуге или нет
                    \en Uniform parametrization by arc or not \~
  */
  MbChannelSurface( MbSurfaceCurve & curv1, MbSurfaceCurve & curv2, 
                    double d1, double d2, MbeSmoothForm fm, double cn, MbFunction & func, bool ev ); 
  
  /** \brief \ru Конструктор по двум кривым и типу сопряжения.
             \en Constructor by two curves and type of mate. \~ 
    \details \ru Конструктор поверхности с сохранением кромки по двум кривым и типу сопряжения.
             \en Constructor of surface with preservation of edges by two curves and mate type. \~ 
    \param[in] curv1 - \ru Опорная кривая на первой поверхности
                       \en Support curve on the first surface \~ 
    \param[in] curv2 - \ru Опорная кривая на второй поверхности
                       \en Support curve on the second surface \~ 
    \param[in] d1 - \ru Радиус скругления со знаком для поверхности кривой crve1
                    \en Fillet radius with sign for surface of crve1 curve \~
    \param[in] d2 - \ru Радиус скругления со знаком для поверхности кривой crve2
                    \en Fillet radius with sign for surface of crve2 curve \~
    \param[in] fm - \ru Тип сопряжения: \n 
               st_Span - скругление с заданной хордой \n
               st_Fillet - скругление с заданными радиусами
                    \en Mate type: \n 
               st_Span - fillet with a given chord \n
               st_Fillet - fillet with given radii. \~    
    \param[in] cn - \ru Коэффициент формы. Значение 0 соответствует профилю "дуга окружности". 
                        Значения от 0.05 до 0.95 соответствуют коническому сечению.
                    \en Coefficient of shape. The value 0 corresponds to a "circular arc" profile. 
                        Values between 0.05 and 0.95 represent conic section. \~
    \param[in] func - \ru Функция изменения радиуса
                      \en Function of change of the radius \~
    \param[in] byFirst - \ru true - кривая curve2 является кромкой, false - кривая curve1 является кромкой
                         \en True - curve2 curve is edge, false - curve1 curve is edge \~
    \param[in] ev - \ru Равномерная параметризация по дуге или нет
                    \en Uniform parametrization by arc or not \~
  */
  MbChannelSurface( MbSurfaceCurve & curv1, MbSurfaceCurve & curv2, 
                    double d1, double d2, MbeSmoothForm fm, double cn, MbFunction & func, bool byFirst, bool ev ); 
  
  /** \brief \ru Конструктор по двум кривым и типу сопряжения.
             \en Constructor by two curves and type of mate. \~ 
    \details \ru Конструктор поверхности с сохранением кромки по двум кривым и типу сопряжения.
             \en Constructor of surface with preservation of edges by two curves and mate type. \~ 
    \param[in] surf1 - \ru Первая поверхность
                       \en First surface \~
    \param[in] curv1 - \ru Опорная кривая в параметрах первой поверхности
                       \en Support curve at parameters of the first surface \~ 
    \param[in] surf2 - \ru Вторая поверхность
                       \en Second surface \~
    \param[in] curv2 - \ru Опорная кривая в параметрах второй поверхности
                       \en Support curve at parameters of the second surface \~ 
    \param[in] curv0 - \ru Кривая пересеченния касательных к поверхностям
                       \en Intersection curve of tangents to surfaces \~
    \param[in] weig0 - \ru Функция изменения веса.
                       \en Function of change of the weights. \~
    \param[in] d1 - \ru Радиус скругления со знаком для поверхности кривой crve1
                    \en Fillet radius with sign for surface of crve1 curve \~
    \param[in] d2 - \ru Радиус скругления со знаком для поверхности кривой crve2
                    \en Fillet radius with sign for surface of crve2 curve \~
    \param[in] fm - \ru Тип сопряжения: \n 
               st_Span - скругление с заданной хордой \n
               st_Fillet - скругление с заданными радиусами
                    \en Mate type: \n 
               st_Span - fillet with a given chord \n
               st_Fillet - fillet with given radii. \~    
    \param[in] cn - \ru Коэффициент формы. Значение 0 соответствует профилю "дуга окружности".
                        Значения от 0.05 до 0.95 соответствуют коническому сечению.
                    \en Coefficient of shape. The value 0 corresponds to a "circular arc" profile.
                        Values between 0.05 and 0.95 represent conic section. \~
    \param[in] func - \ru Функция изменения радиуса.
                      \en Function of change of the radius. \~
    \param[in] byFirst - \ru true - кривая curve2 является кромкой, false - кривая curve1 является кромкой
                         \en True - curve2 curve is edge, false - curve1 curve is edge \~
    \param[in] ev - \ru Равномерная параметризация по дуге или нет
                    \en Uniform parametrization by arc or not \~
  */
  MbChannelSurface( MbSurface & surf1, MbCurve & curv1,
                    MbSurface & surf2, MbCurve & curv2, 
                    MbCurve3D & curv0, MbFunction & weig0, 
                    double d1, double d2, MbeSmoothForm fm, double cn, MbFunction & func, bool ev ); 
  
  /** \brief \ru Создать поверхность скругления по двум поверхностям по закону.
           \en Create a fillet surface from two surfaces according to the law. \~
  \details \ru В случае успеха создается поверхность скругления с версией >= fsv_Ver1. \n
           \en If successful, a fillet surface is created with version >= fsv_Ver1. \n\~
  \param[in] rad - \ru Радиус скругления.
                   \en Fillet radius. \~
  \param[in] law - \ru Функция домножения радиуса rad, зависящая от параметра u поверхности.
                    \en Multiplication function of radius rad, depending on the parameter u of the surface. \~
  \param[in] curv1 - \ru Опорная кривая в параметрах первой поверхности.
                     \en Support curve at parameters of the first surface. \~
  \param[in] curv2 - \ru Опорная кривая в параметрах второй поверхности.
                     \en Support curve at parameters of the second surface. \~
  \param[in] sgn1, sgn2 - \ru Ориентации опорных поверхностей. В случае true отступ выполняетя по нормали, в случае false - против нормали.
                          \en Orientations of the support surfaces. If true, the indentation is performed along the normal, 
                              if false, it is performed against the normal.\~
  \param[in] cn - \ru Функция коэффициента формы. Если функция не задана или значение коэффициента 0,то в качестве профиля используется "дуга окружности".
                      Значения от 0.05 до 0.95 соответствуют коническому сечению. Переменный коэффициент формы используется только для создания скругления граней.
                  \en A function of the shape coefficient. If the function is not specified or the coefficient value is 0, then the "circle arc" profile is used.
                      Values between 0.05 and 0.95 represent to the conic section. A variable shape coefficient is only used to create faces fillets. \~
  \param[in] vers - \ru Версия.
                     \en  Version. \~
  \return \ru Возвращает указатель на созданную поверхность скругления в случае успеха, иначе - nullptr.
          \en Returns a pointer to the created fillet surface if successful, nullptr otherwise. \~
  */
  static MbChannelSurface * CreateLawFillet( double rad, MbFunction & law, MbSurfaceCurve & curv1, bool sgn1,
                                             MbSurfaceCurve & curv2, bool sgn2, MbFunction * cn, VERSION vers );

  /** \brief \ru Создать поверхность скругления по двум поверхностям по закону.
           \en Create a fillet surface from two surfaces according to the law. \~
  \details \ru В случае успеха создается поверхность скругления с версией >= fsv_Ver1.\n
           \en If successful, a fillet surface is created with version >= fsv_Ver1. \n\~
  \param[in] curv1, curv2 - \ru Опорные кривые в параметрах поверхности
                            \en Support curves at parameters of surfaces \~
  \param[in] fRad1, fRad2 - \ru Функции радиуса, зависящая от параметра u поверхности.
                            \en Radii function, depending on the parameter u of the surface. \~
  \param[in] sgn1, sgn2 - \ru Ориентации опорных поверхностей. В случае true отступ выполняетя по нормали, в случае false - против нормали.
                          \en Orientations of the support surfaces. If true, the indentation is performed along the normal, 
                              if false, it is performed against the normal.\~
  \param[in] cn - \ru Функция коэффициента формы. Если функция не задана или значение коэффициента 0,то в качестве профиля используется "дуга окружности".
                      Значения от 0.05 до 0.95 соответствуют коническому сечению. Переменный коэффициент формы используется только для создания скругления граней.
                  \en A function of the shape coefficient. If the function is not specified or the coefficient value is 0, then the "circle arc" profile is used.
                      Values between 0.05 and 0.95 represent to the conic section. A variable shape coefficient is only used to create faces fillets. \~
  \param[in] vers - \ru Версия.
                     \en  Version. \~
  \return \ru Возвращает указатель на созданную поверхность скругления в случае успеха, иначе - nullptr.
          \en Returns a pointer to the created fillet surface if successful, nullptr otherwise. \~
  */
  static MbChannelSurface * CreateLawFillet( MbSurfaceCurve & curv1, bool sgn1, MbFunction & fRad1,
                                             MbSurfaceCurve & curv2, bool sgn2, MbFunction & fRad2,
                                             MbFunction * cn, VERSION vers );


  /** \brief \ru Создать поверхность скругления по двум поверхностям с переменной хордой.
           \en Create a fillet surface by two surfaces with a constant chord. \~
  \details \ru В случае успеха создается поверхность скругления с версией >= fsv_Ver1.\n
           \en If successful, a fillet surface is created with version >= fsv_Ver1. \n\~
  \param[in] curv1, curv2 - \ru Опорные кривые в параметрах поверхности.
                            \en Support curves at parameters of surfaces. \~
  \param[in] sgn1, sgn2 - \ru Ориентации опорных поверхностей. В случае true отступ выполняетя по нормали, в случае false - против нормали.
                          \en Orientations of the support surfaces. If true, the indentation is performed along the normal, 
                              if false, it is performed against the normal.\~
  \param[in] h - \ru Функция величины хорды.
                 \en Chord function. \~
  \param[in] cn - \ru Функция коэффициента формы. Если функция не задана или значение коэффициента 0,то в качестве профиля используется "дуга окружности".
                      Значения от 0.05 до 0.95 соответствуют коническому сечению. Переменный коэффициент формы используется только для создания скругления граней.
                  \en A function of the shape coefficient. If the function is not specified or the coefficient value is 0, then the "circle arc" profile is used.
                      Values between 0.05 and 0.95 represent to the conic section. A variable shape coefficient is only used to create faces fillets. \~
  \param[in] vers - \ru Версия.
                     \en  Version. \~
  \return \ru Возвращает указатель на созданную поверхность скругления в случае успеха, иначе - nullptr.
          \en Returns a pointer to the created fillet surface if successful, nullptr otherwise. \~
  */
  static MbChannelSurface * CreateChordLawFillet( MbSurfaceCurve & curv1, bool sgn1, 
                                                  MbSurfaceCurve & curv2, bool sgn2, 
                                                  MbFunction & hFunc, MbFunction * cn, VERSION vers );

  /** \brief \ru Создать поверхность скругления по поверхности и кромке с радиусом по закону
             \en Create fillet surface by support surface and by kerb-curve with a radius that varies according to the law. \~ 
    \details \ru В случае успеха создается поверхность скругления с версией >= fsv_Ver1.\n
             \en If successful, a fillet surface is created with version >= fsv_Ver1.\n\~ 
    \param[in] rad - \ru Радиус скругления.
                     \en Fillet radius. \~
    \param[in]  law - \ru Функция домножения радиуса rad, зависящая от параметра u поверхности.
                      \en Multiplication function of radius rad, depending on the parameter u of the surface. \~
    \param[in] curv1 - \ru Опорная кривая в параметрах первой поверхности.
                       \en Support curve at parameters of the first surface. \~ 
    \param[in] curv2 - \ru Опорная кривая в параметрах второй поверхности.
                       \en Support curve at parameters of the second surface. \~ 
    \param[in] sgn1, sgn2 - \ru Ориентации опорных поверхностей. В случае true отступ выполняетя по нормали, в случае false - против нормали.
                            \en Orientations of the support surfaces. If true, the indentation is performed along the normal, 
                                if false, it is performed against the normal.\~
    \param[in] byFirst - \ru true - кривая curve2 является кромкой, false - кривая curve1 является кромкой.
                         \en True - curve2 curve is edge, false - curve1 curve is edge. \~
    \param[in] vers - \ru Версия.
                       \en  Version. \~
    \return \ru Возвращает указатель на созданную поверхность скругления в случае успеха, иначе - nullptr.
            \en Returns a pointer to the created fillet surface if successful, nullptr otherwise. \~
  */
  static MbChannelSurface * CreateKerbLawFillet( double rad, MbFunction & law, MbSurfaceCurve & curv1, bool sgn1, 
                                                 MbSurfaceCurve & curv2, bool sgn2, bool byFirst, VERSION vers );

protected:
  /// \ru Конструктор для наследников обычной поверхности скругления. \en Constructor for inheritors of ordinary fillet surface.  
  MbChannelSurface( MbSurfaceCurve & curv1, double d1,
                    MbSurfaceCurve & curv2, double d2, MbFunction * cn, MbFunction & func );
  MbChannelSurface( MbSurfaceCurve & curv1, double d1, MbFunction & func1,
                    MbSurfaceCurve & curv2, double d2, MbFunction & func2, MbFunction * cn );
  MbChannelSurface( MbSurfaceCurve & curv1, double d1, 
                    MbSurfaceCurve & curv2, double d2, MbFunction & func, bool byFirst ); 
  MbChannelSurface( const MbChannelSurface &, MbRegDuplicate * );
  MbChannelSurface( const MbChannelSurface * ); // \ru Конструктор копирования с теми же опорными поверхностями для CurvesDuplicate() \en Copy constructor with the same support surfaces for CurvesDuplicate() 

private:
  MbChannelSurface( const MbChannelSurface & ); // \ru Не реализовано. \en Not implemented. 

public:
  virtual ~MbChannelSurface ();

public:
  VISITING_CLASS( MbChannelSurface );

  /** \ru \name Общие функции геометрического объекта
      \en \name Common functions of a geometric object
      \{ */
  MbeSpaceType    IsA() const override; // \ru Тип элемента. \en A type of element.
  MbSpaceItem   & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента. \en Create a copy of the element.
  bool    IsSame   ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const override; // \ru Является ли объект копией. \en Whether the object is a copy. 
  bool    SetEqual ( const MbSpaceItem & ) override;       // \ru Сделать равным. \en Make equal. 
  bool    IsSimilar( const MbSpaceItem & ) const override; // \ru Являются ли объекты подобными. \en Determine whether the objects are similar. 
  // \ru Подобные ли поверхности для объединения (слива). \en Whether the surfaces to union (joining) are similar. 
  bool    IsSimilarToSurface( const MbSurface & surf, VERSION version, double precision = METRIC_PRECISION ) const override;

  void    GetProperties( MbProperties & properties ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  /** \} */
  /** \ru \name Общие функции поверхности
      \en \name Common functions of surface
      \{ */
  MbSurface * Offset( double d, bool same ) const override; // \ru Создание эквидистантной поверхности. \en Creation of an offset surface. 
  double  GetFilletRadius( const MbCartPoint3D & p, double accuracy = METRIC_REGION ) const override;
  double  GetFilletRadius( double u ) const override;
  /** \} */
  /** \ru \name Функции поверхности сопряжения
      \en \name Functions of smooth surface
      \{ */  
  MbSmoothSurface & CurvesDuplicate() const override; // \ru Копия с теми же опорными поверхностями. \en Copy with the same support surfaces. 
  double  GetSmoothRadius() const override; // \ru Дать радиус. \en Get radius. 
  double  DistanceRatio( bool firstCurve, MbCartPoint3D & p, double distance ) const override;
  /** \} */
  /** \ru \name Функции поверхности скругления с переменным радиусом обычная или с сохранением кромки
      \en \name Functions of fillet surface with variable radius is normal or with preservation of edges
      \{ */        
          
  /** \brief \ru Добавить точку в опорные кривые границы.
              \en Add a point to the support curves of the boundary. \~ 
    \details \ru Добавить точку в опорные кривые границы.\n
      Точка будет добавлена в кривую, если она имеет тип pt_LineSegment, pt_CubicSpline или pt_Hermit.
              \en Add a point to the support curves of the boundary.\n
      A point will be added into a curve if it has a type pt_LineSegment, pt_CubicSpline or pt_Hermit. \~
    \param[out] t1 - \ru Параметр точки на первой кривой (если add1 = true)
                      \en Parameter of a point on the first curve (if add1 equals true) \~
    \param[in] p1 - \ru Точка на первой кривой
                    \en Point on the first curve \~
    \param[in] add1 - \ru Нужно ли добавлять точку в первую кривую
                      \en Whether to add a point to the first curve \~
    \param[out] t2 - \ru Параметр точки на второй кривой (если add2 = true)
                      \en Parameter of a point on the second curve (if add2 equals true) \~
    \param[in] p2 - \ru Точка на второй кривой
                    \en Point on the second curve \~
    \param[in] add2 - \ru Нужно ли добавлять точку во вторую кривую
                      \en Whether to add a point to the second curve \~
  */
  bool    InsertPoints( double & t1, const MbCartPoint & p1, bool add1,
                        double & t2, const MbCartPoint & p2, bool add2, VERSION vers ) override;

  /** \brief \ru Проверить наличие полюса.
              \en Check pole availability. \~ 
    \details \ru Проверить наличие полюса.
              \en Check pole availability. \~    
    \param[in] u - \ru Начальное приближение параметра по U для поиска полюса
                    \en Initial approximation of parameter U to search pole \~
    \param[in] bModify - \ru Флаг модификации поверхности \n
      если true, то поверхность корректирует свои параметры по U 
      и соответственно им опорные кривые curve1 и curve2
                          \en Flag of surface modification \n
      if true, then the surface corrects its parameters along U 
      and according to them the support curves curve1 and curve2 \~ 
    \return \ru true - если нашли полюс
            \en True - if pole has been found \~
  */
  bool    CheckPole( double & u, bool bModify = true ); 
  /// \ru Получить функцию изменения радиуса (0 - первая кривая, иначе -  вторая кривая) . \en Get a function of radius changing (0 - first curve, otherwise - second curve). 
  const MbFunction & GetFunction( size_t ind ) const { return ind == 0 ? *function1 : *function2; } 
  /// \ru Получить функцию изменения радиуса (0 - первая кривая, иначе -  вторая кривая) . \en Get a function of radius changing (0 - first curve, otherwise - second curve). 
  MbFunction & SetFunction( size_t ind ) { return ind == 0 ? *function1 : *function2; } 
  /// \ru Заменить функцию изменения радиуса (0 - первая кривая, иначе -  вторая кривая) . \en Set a function of radius changing (0 - first curve, otherwise - second curve). 
  void    SetFunction( size_t ind, MbFunction & funcNew ); // \ru (новая функция должна быть корректна) \en (new function must be correct) 
  /// \ru Построить функцию изменения радиуса от параметра u1 до параметра u2 (0 - первая кривая, иначе -  вторая кривая). 
  /// \en Construct a function for changing the radius from parameter u1 to parameter u2 (0 - first curve, otherwise - second curve). 
  MbFunction * MakeFunction( size_t ind, double u1, double u2 ) const;

  /** \} */
private:
  // \ru Дать коэффициент для радиуса (0 - первая кривая, иначе -  вторая кривая). 
  // \en Get coefficient for radius (0 - first curve, otherwise - second curve). 
  double  FunctionValue( size_t ind, double u ) const override; 
  void    CheckPole(); // \ru Проверить полюса \en Check poles 
          // \ru Добавить точку в опорные кривые границы поверхности с постоянной хордой. \en Add a point to the support curves of the boundary of surface with constant chord. \~ 
  bool    InsertForSpan( double & t1, const MbCartPoint & p1, bool add1,
                                 double & t2, const MbCartPoint & p2, bool add2, VERSION vers );
  void    operator = ( const MbChannelSurface & ); // \ru Не реализовано. \en Not implemented. 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbChannelSurface )
};

IMPL_PERSISTENT_OPS( MbChannelSurface )


//------------------------------------------------------------------------------
// \ru Создать поверхность переменного радиуса \en Create surface with variable radius 
// ---
MbSmoothSurface * CreateChannelSurface( const MbSurface & surface1, SArray<MbCartPoint> & points1,
                                        const MbSurface & surface2, SArray<MbCartPoint> & points2,
                                        MbeSmoothForm form, double distance1, double distance2, double conic, 
                                        SArray<double> & dFactor, SArray<double> & dTendency,
                                        bool even, VERSION vers ); 


//------------------------------------------------------------------------------
// \ru Создать поверхность переменного радиуса с сохранением кромки \en Create surface with variable radius with  preservation of edges 
// ---
MbSmoothSurface * CreateKerbChannelSurface( const MbSurface & surface1, SArray<MbCartPoint> & points1,
                                            const MbSurface & surface2, SArray<MbCartPoint> & points2,
                                            MbeSmoothForm form, double distance1, double distance2, double conic, 
                                            const MbSurfaceIntersectionCurve & guideCurve, SArray<double> & params,
                                            SArray<double> & dFactor, 
                                            bool byFirstSurface, bool even, VERSION version );
 

#endif // __SURF_CHANNEL_SURFACE_H
