////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Геометрический объект в двумерном пространстве.
         \en Geometric object in two-dimensional space. \~ 

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __PLANE_ITEM_H
#define __PLANE_ITEM_H


#include <io_tape_define.h>
#include <reference_item.h>
#include <item_registrator.h>
#include <mb_variables.h>
#include <math_define.h>
#include <mb_property_title.h>


class   MbCartPoint;
class   MbVector;
class   MbDirection;
class   MbMatrix;
class   MbRect;
class   MbSurface;
class   MbProperties;
class   MbProperty;
struct  MbControlData;
class             MbRegDuplicate;
class             MbRegTransform;


//------------------------------------------------------------------------------
/** \brief \ru Типы геометрических объектов в двумерном пространстве.
           \en Types of geometric objects in two-dimensional space. \~
  \details \ru Геометрические объектыв группируются в семейства: 
    кривые, мультилиния, регион.
           \en Geometric objects are grouped by families: 
    curves, multiline, region. \~
  \ingroup Geometric_Items
*/
// ---
enum MbePlaneType {

  pt_Undefined         =   0,  ///< \ru Неизвестный объект. \en Unknown object. 
  pt_PlaneItem         =   1,  ///< \ru Произвольный двумерный объект. \en Arbitrary two-dimensional object. \n 

  // \ru Типы кривых. \en Types of curves.
  pt_Curve             = 201,  ///< \ru Произвольная кривая. \en Arbitrary curve. 
  pt_Line              = 202,  ///< \ru Прямая. \en Line. 
  pt_LineSegment       = 203,  ///< \ru Отрезок. \en Segment. 
  pt_Arc               = 204,  ///< \ru Окружность или эллипс или дуга окружности или дуга эллипса. \en Circle or ellipse or arc of circle or arc of ellipse. 
  pt_Cosinusoid        = 205,  ///< \ru Кривая-косинусоида. \en Cosine curve. 
  pt_PolyCurve         = 206,  ///< \ru Сплайновая кривая. \en Spline curve. 
  pt_Polyline          = 207,  ///< \ru Полилиния. \en Polyline. 
  pt_Bezier            = 208,  ///< \ru Безье-сплайн. \en Bezier spline. 
  pt_Hermit            = 209,  ///< \ru Составной кубический сплайн Эрмита. \en Composite cubic Hermite spline. 
  pt_Nurbs             = 210,  ///< \ru NURBS кривая. \en NURBS-curve. 
  pt_CubicSpline       = 211,  ///< \ru Кубический сплайн. \en Cubic spline. 
  pt_TrimmedCurve      = 212,  ///< \ru Усеченная кривая. \en Trimmed curve. 
  pt_OffsetCurve       = 213,  ///< \ru Эквидистантная продленная кривая. \en Extended offset curve. 
  pt_ReparamCurve      = 214,  ///< \ru Репараметризованная кривая. \en Reparametrized curve. 
  pt_PointCurve        = 215,  ///< \ru Кривая - точка. \en Point-curve. 
  pt_CharacterCurve    = 216,  ///< \ru Кривая, координатные функции которой заданы в символьном виде. \en Functionally defined curve. 
  pt_ProjCurve         = 217,  ///< \ru Проекционная кривая. \en Projection curves. 
  pt_SweptImageCurve   = 218,  ///< \ru Образ трехмерной кривой на поверхности при движении по направляющей. \en Image of three-dimensional curve on surface while moving along a guide curve. 
  pt_TransformedCurve  = 219,  ///< \ru Трансформированная кривая. \en Transformed curve. 
  pt_ConeBendedCurve   = 220,  ///< \ru Кривая в параметрической области конуса, соответствующая кривой в параметрической области плоскости при коническом сгибе. \en Curve in parametric region of a cone corresponding to curve in parametric region of plane at a conic bend. 
  pt_ConeUnbendedCurve = 221,  ///< \ru Кривая в параметрической области плоскости, соответствующая кривой в параметрической области конуса при коническом сгибе. \en Curve in parametric region of a plane corresponding to curve in parametric region of a cone at conic bend. 

  // \ru Типы сложных кривых. \en Types of complex curves.
  pt_Contour           = 301,  ///< \ru Контур - составная кривая. \en Contour - composite curve. 
  pt_ContourWithBreaks = 302,  ///< \ru Контур с разрывами . \en Contour with discontinuities. 
  pt_FreeCurve         = 400,  ///< \ru Тип для кривых, созданных пользователем. \en User-defined curve. \n 

  // \ru Типы сложных объектов. \en Types of complex objects. 
  pt_Multiline         = 401,  ///< \ru Мультилиния. \en Multiline. 

  // \ru Типы других объектов. \en Types of other objects. 
  pt_Region            = 501,  ///< \ru Регион. \en Region. 

  pt_FreeItem          = 600,  ///< \ru Тип для объектов, созданных пользователем. \en Type for the user-defined objects.

};


//------------------------------------------------------------------------------
/** \brief \ru Геометрический объект в двумерном пространстве.
           \en Geometric object in two-dimensional space. \~
  \details \ru Родительский класс геометрических объектов в двумерном пространстве. 
    Имеет счетчик ссылок. 
    Наследниками являются: кривая MbCurve, мультилиния MbMultiline, регион MbRegion.
           \en Parent class of geometric objects in two-dimensional space. 
    Has reference counter. 
    Inheritors are: MbCurve curve, MbMultiline multiline, MbRegion region. \~ 
  \ingroup Geometric_Items
*/ 
// ---
class MATH_CLASS MbPlaneItem : public TapeBase, public MbRefItem {
protected :
  /// \ru Конструктор. \en Constructor. 
  MbPlaneItem();
public :
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbPlaneItem();

  /** \ru \name Общие функции геометрического объекта
      \en \name Common functions of a geometric object 
      \{ */
  /// \ru Получить регистрационный тип (для копирования, дублирования). \en Get the registration type (for copying, duplication). 
  MbeRefType  RefType() const override;  
  /// \ru Получить тип объекта. \en Get the object type. 
  virtual MbePlaneType IsA() const = 0; 
  /// \ru Получить групповой тип объекта. \en Get the group type of the object. 
  virtual MbePlaneType Type() const = 0;
  /// \ru Получить семейство объекта. \en Get family of object. 
  virtual MbePlaneType Family() const = 0;

  /** \brief \ru Создать копию.
             \en Create a copy. \~
    \details \ru Создать копию объекта с использованием регистратора. 
      Регистратор используется для предотвращения многократного копирования объекта.
      Если объект содержит ссылки на другие объекты, то вложенные объекты так же копируются.
      Допустимо не передавать регистратор в функцию. Тогда будет создана новая копия объекта.
      При копировании одиночного объекта или набора не связанных между собой объектов допустимо не использовать регистратор.
      Регистратор необходимо использовать, если надо последовательно копировать несколько взаимосвязанных объектов.
      Возможно, что связь объектов обусловлена наличием в них ссылок на общие объекты. 
      Тогда, при копировании без использования регистратора, можно получить набор копий, 
      содержащих ссылки на разные копии одного и того же вложенного объекта, что ведет к потере связи между копиями.
             \en Create a copy of the object using the registrator. 
      The registrator is used for preventing multiple copying of an object.
      If the object contains references to other objects, then the included objects are copied too.
      It is allowed not to pass the registrator to a function. Then the new copy of the object will be created.
      It is allowed not to use the registrator while copying a single object or a set of disconnected objects.
      The registrator must be used to copy several correlated objects successively.
      It is possible that the objects' connection means that the objects contain references to the common objects. 
      Then, while copying without using the registrator, one can get a set of copies 
      which contain references to the different copies of a single included object, what leads to loss of connection between the copies. \~
    \param[in] iReg - \ru Регистратор.
                      \en Registrator. \~
    \return \ru Копия объекта.
            \en Copy of the object. \~
  */
  virtual MbPlaneItem & Duplicate( MbRegDuplicate * iReg = nullptr ) const = 0;

  /** \brief \ru Преобразовать согласно матрице.
             \en Transform according to the matrix. \~
    \details \ru Преобразовать исходный объект согласно матрице c использованием регистратора. 
      Если объект содержит ссылки на другие геометрические объекты, то вложенные объекты так же преобразуются согласно матрице.
      Регистратор служит для предотвращения многократного преобразования объекта.
      Допустимо использовать функцию без регистратора, если надо преобразовать одиночный объект.
      Если надо преобразовать набор взаимосвязанных объектов, необходимо использовать регистратор для 
      предотвращения повторного преобразования вложенных объектов, поскольку не исключена ситуация,
      когда несколько объектов из набора содержат ссылки на один или несколько общих объектов, подлежащих трансформации.
             \en Transform the initial object according to the matrix using the registrator. 
      If the object contains references to the other geometric objects, then the nested objects are transformed according to the matrix.
      The registrator is used for preventing multiple transformation of the object.
      The function can be used without the registrator to transform a single object.
      The registrator must be used to transform a set of interdependent objects to 
      prevent repeated transformation of the nested objects, since it is not ruled out
      that several objects from the set contain references to one or several common objects subject to transformation. \~
    \param[in] matr    - \ru Матрица преобразования.
                         \en A transformation matrix. \~
    \param[in] iReg    - \ru Регистратор.
                         \en Registrator. \~
    \param[in] surface - \ru Новая базовая поверхность объекта 
                         при условии, что matr - матрица преобразования из старой поверхности в новую.
                         Для трансформации проекционной кривой.
                         Не учитывается, если поверхность плоская.
                         \en New base surface of object 
                         provided that 'matr' is a transformation matrix from the old surface to a new one.
                         For transformation of projection curve.
                         It isn't considered if the surface is planar. \~
  */
  virtual void    Transform( const MbMatrix & matr, MbRegTransform * iReg = nullptr, const MbSurface * newSurface = nullptr ) = 0;

  /** \brief \ru Сдвинуть вдоль вектора.
             \en Translate along a vector. \~
    \details \ru Сдвинуть геометрический объект вдоль вектора с использованием регистратора. 
      Если объект содержит ссылки на другие геометрические объекты, то к вложенным объектам так же применяется операция сдвига.
      Регистратор служит для предотвращения многократного преобразования объекта.
      Допустимо использовать функцию без регистратора, если надо преобразовать одиночный объект.
      Если надо преобразовать набор взаимосвязанных объектов, необходимо использовать регистратор для 
      предотвращения повторного преобразования вложенных объектов, поскольку не исключена ситуация,
      когда несколько объектов из набора содержат ссылки на один или несколько общих объектов, подлежащих сдвигу.
             \en Translate a geometric object along the vector using the registrator. 
      If the object contains references to the other objects, then the translation operation is applied to the nested objects.
      The registrator is used for preventing multiple transformation of the object.
      The function can be used without the registrator to transform a single object.
      The registrator must be used to transform a set of interdependent objects to 
      prevent repeated transformation of the nested objects, since it is not ruled out
      that several objects from the set contain references to one or several common objects subject to translation. \~
    \param[in] to      - \ru Вектор сдвига.
                         \en Translation vector. \~
    \param[in] iReg    - \ru Регистратор.
                         \en Registrator. \~
    \param[in] surface - \ru Новая базовая поверхность объекта 
                         при условии, что matr - матрица преобразования из старой поверхности в новую.
                         Для трансформации проекционной кривой.
                         Не учитывается, если поверхность плоская.
                         \en New base surface of object 
                         provided that 'matr' is a transformation matrix from the old surface to a new one.
                         For transformation of projection curve.
                         It isn't considered if the surface is planar. \~
  */
  virtual void    Move  ( const MbVector & to, MbRegTransform * iReg = nullptr, const MbSurface * newSurface = nullptr ) = 0;

  /** \brief \ru Повернуть вокруг точки.
             \en Rotate about a point. \~
    \details \ru Повернуть объект вокруг точки на заданный угол с использованием регистратора. 
      Если объект содержит ссылки на другие геометрические объекты, то к вложенным объектам так же применяется операция поворота.
      Регистратор служит для предотвращения многократного преобразования объекта.
      Допустимо использовать функцию без регистратора, если надо преобразовать одиночный объект.
      Если надо преобразовать набор взаимосвязанных объектов, необходимо использовать регистратор для 
      предотвращения повторного преобразования вложенных объектов, поскольку не исключена ситуация,
      когда несколько объектов из набора содержат ссылки на один или несколько общих объектов, подлежащих повороту.
             \en Rotate an object about a point by the given angle using the registrator. 
      If the object contains references to the other geometric objects, then the rotation operation is applied to the nested objects too.
      The registrator is used for preventing multiple transformation of the object.
      The function can be used without the registrator to transform a single object.
      The registrator must be used to transform a set of interdependent objects to 
      prevent repeated transformation of the nested objects, since it is not ruled out
      that several objects from the set contain references to one or several common objects subject to rotation. \~
    \param[in] pnt     - \ru Неподвижная точка.
                         \en Fixed point. \~
    \param[in] angle   - \ru Угол поворота.
                         \en The rotation angle. \~
    \param[in] iReg    - \ru Регистратор.
                         \en Registrator. \~
    \param[in] surface - \ru Новая базовая поверхность объекта 
                         при условии, что matr - матрица преобразования из старой поверхности в новую.
                         Для трансформации проекционной кривой.
                         Не учитывается, если поверхность плоская.
                         \en New base surface of object 
                         provided that 'matr' is a transformation matrix from the old surface to a new one.
                         For transformation of projection curve.
                         It isn't considered if the surface is planar. \~
  */
  virtual void    Rotate( const MbCartPoint & pnt, const MbDirection & angle, 
                          MbRegTransform * iReg = nullptr, const MbSurface * newSurface = nullptr ) = 0;
          
  /** \brief \ru Повернуть вокруг точки.
             \en Rotate about a point. \~
    \details \ru Повернуть объект вокруг точки на заданный угол с использованием регистратора.
             \en Rotate an object about a point by the given angle using the registrator. \~ 
    \param[in] pnt     - \ru Неподвижная точка.
                         \en Fixed point. \~
    \param[in] angle   - \ru Угол поворота.
                         \en The rotation angle. \~
    \param[in] iReg    - \ru Регистратор.
                         \en Registrator. \~
    \param[in] surface - \ru Новая базовая поверхность объекта 
                         при условии, что matr - матрица преобразования из старой поверхности в новую.
                         Для трансформации проекционной кривой.
                         Не учитывается, если поверхность плоская.
                         \en New base surface of object 
                         provided that 'matr' is a transformation matrix from the old surface to a new one.
                         For transformation of projection curve.
                         It isn't considered if the surface is planar. \~
  */
  virtual void    Rotate( const MbCartPoint & pnt, double angle, 
                          MbRegTransform * iReg = nullptr, const MbSurface * newSurface = nullptr );

  /** \brief \ru Определить, являются ли объекты равными.
             \en Determine whether objects are equal. \~
    \details \ru Равными считаются однотипные объекты, все данные которых одинаковы (равны).
             \en Objects of the same types with similar (equal) data are considered to be equal. \~
    \param[in] item - \ru Объект для сравнения.
                      \en Object for comparison. \~
    \param[in] accuracy - \ru Точность сравнения.
                          \en The accuracy. \~
    \return \ru Равны ли объекты.
            \en Whether the objects are equal. \~
  */
  virtual bool    IsSame( const MbPlaneItem & item, double accuracy = LENGTH_EPSILON ) const = 0;

  /** \brief \ru Определить, являются ли объекты подобными.
             \en Determine whether the objects are similar. \~
    \details \ru Подобными считаются однотипные объекты, данные которых можно приравнять или данные так же являются подобными (указатели).
      Подобный объект можно инициализировать по данным подобного ему объекта (приравнять один другому без изменения адресов).
             \en Objects of the same type are considered to be similar if data of the objects can be equated or the data are also similar (pointers).
      Similar object can be initialized by data of object similar to it (equate one to another without changing of addresses). \~
    \param[in] item - \ru Объект для сравнения.
                      \en Object for comparison. \~
    \return \ru Подобны ли объекты.
            \en Whether the objects are similar. \~
  */
  virtual bool    IsSimilar( const MbPlaneItem & item ) const;

  /** \brief \ru Сделать объекты равным.
             \en Make the objects equal. \~
    \details \ru Равными можно сделать только подобные объекты.
      Подобный объект приравнивается присланному путем изменения численных данных.
             \en It is possible to make equal only similar objects.
      Similar object is equated to a given one by changing of numerical data. \~
    \param[in] item - \ru Объект для инициализации.
                      \en Object for initialization. \~
    \return \ru Сделан ли объект равным присланному.
            \en Whether the object is made equal to the given one. \~
  */
  virtual bool    SetEqual ( const MbPlaneItem & item ) = 0;

  /// \ru Расширить присланный габаритный прямоугольник так, чтобы он включал в себя данный объект. \en Extend the given bounding rectangle so that it encloses this object. 
  virtual void    AddYourGabaritTo( MbRect & r ) const = 0;

  /** \brief \ru Определить видимость объекта в прямоугольнике.
             \en Determine visibility of an object in rectangle. \~
    \details \ru Считается, что объект виден в прямоугольнике, если габариты объекта пересекаются с заданным прямоугольником
      или (при повышенных требованиях к точности exact = true) в прямоугольник попадает хотя бы одна точка объекта.
             \en It is considered that the object is visible in rectangle if bounds of an object is crossed with the given rectangle
      or (high requirements to accuracy, exact = true) at least one point of object is in the rectangle. \~
    \param[in] rect - \ru Прямоугольник, попадание в который проверяется.
                      \en Rectangle to check getting to. \~
    \param[in] exact - \ru Точность проверки. При exact = true в прямоугольник должна попасть хотя бы одна точка объекта. 
      При exact = false - достаточно пересечения габарита объекта с прямоугольником.
                       \en Check accuracy. If exact = true, then at least one point of object gets to the rectangle. 
      if exact = false, it is sufficient to find intersection between rectangle and bounding box of an object. \~
    \return \ru true - объект виден в прямоугольнике, иначе - false.
            \en true, if the object is visible in the rectangle, otherwise false. \~
  */
  virtual bool    IsVisibleInRect( const MbRect & rect, bool exact = false ) const = 0;  

  /// \ru Вычислить расстояние до точки. \en Calculate the distance to a point. 
  virtual double  DistanceToPoint( const MbCartPoint & to ) const = 0;

  /** \brief \ru Вычислить расстояние до точки.
             \en Calculate the distance to a point. \~
    \details \ru Вычислить расстояние от объекта до заданной точки вблизи объекта. 
      Расстояние вычисляется и записывается в переменную d, если оно меньше исходного значения d.
      Может быть получен выигрыш по времени выполнения по сравнению с функцией DistanceToPoint,
      за счет того, что сначала проверяется расстояние от точки до габаритного куба, 
      и только если это расстояние не больше заданного, выполняются дальнейшие вычисления.
             \en Calculate distance to object from a given point near the object. 
      Distance is calculated and stored to 'd' variable if it is less then initial value of 'd'.
      There can be performance benefit in comparison with DistanceToPoint function
      due to primarily checking the distance from point to bounding box 
      and performing the further calculations only if this distance is not greater than the given one. \~
    \param[in] to - \ru Точка.
                    \en Point. \~
    \param[in, out] d  - \ru На входе - заданная величина отступа от объекта. На выходе - расстояние от точки до объекта, если операция выполнена успешно.
                         \en Specified distance from object on input. Distance from point to object on output if operation succeeded. \~
    \return \ru true, если расстояние от точки до объекта меньше заданного, иначе - false.
            \en True if distance from point to the object is less than the given one, otherwise false. \~
  */

  /// \ru Рассчитать расстояние до точки и изменить его присланное значение, если расстояние окажется меньше присланного значения. \en Calculate the distance from a point and change the given value of distance if the distance is less than the given one. 
  virtual bool    DistanceToPointIfLess( const MbCartPoint & to, double & d ) const = 0;
  /// \ru Перевести все временные (mutable) данные объекта в неопределённое (исходное) состояние. \en Set all temporary (mutable) data of object to undefined (initial) state. 
  virtual void    Refresh();

  /// \ru Создать собственное свойство с заданием его имени. \en Create your own property with specified name. 
  virtual MbProperty & CreateProperty( MbePrompt name ) const = 0;

  /** \brief \ru Выдать свойства объекта.
             \en Get properties of the object. \~
    \details \ru Выдать внутренние данные (свойства) объекта для их просмотра и модификации.
             \en Get internal data (properties) of an object for viewing and modification. \~
    \param[in] properties - \ru Контейнер для внутренних данных объекта.
                            \en Container for internal data of an object. \~
  */
  virtual void    GetProperties( MbProperties & properties ) = 0;

  /** \brief \ru Изменить свойства объекта.
             \en Change properties of the object. \~
    \details \ru Изменение внутренних данных (свойств) объекта выполняется 
      копированием соответствующих значений из присланного объекта.
             \en Change internal data (properties) of object is performed 
      by copying of corresponding values from the given object. \~
    \param[in] properties - \ru Контейнер для внутренних данных объекта.
                            \en Container for internal data of an object. \~
  */
  virtual void    SetProperties( const MbProperties & properties ) = 0;

  /// \ru Выдать контрольные точки объекта. \en Get control points of object.
  virtual void    GetBasisPoints( MbControlData & ) const = 0;
  /// \ru Изменить объект по контрольным точкам. \en Change the object by control points.
  virtual void    SetBasisPoints( const MbControlData & ) = 0;
  /** \} */

          /** \brief \ru Регистрация объекта.
                     \en Object registration. \~
            \details \ru Регистрация объекта для предотвращения его многократной записи.
              Другие объекты могут содержать указатель на данный объект. 
              Функция взводит флаг, который позволяет записывать объект один раз, а в остальных записях ссылаться на записанный экземпляр.
              Чтение так же выполняется один раз, а в остальных случаях чтения подставляется адрес уже прочитанного объекта.
                     \en Object registration for preventing its multiple writing.
              Other objects may contain a pointer to the given object. 
              The function sets a flag that allow to write the object once and to use the references to the recorded instance in the other records. 
              Reading is performed once too, in other cases of reading the address of the already read object is used. \~
          */
          void    PrepareWrite() const { SetRegistrable( (GetUseCount() > 1) ? registrable : noRegistrable ); }

DECLARE_PERSISTENT_CLASS( MbPlaneItem )
OBVIOUS_PRIVATE_COPY( MbPlaneItem )
}; // MbPlaneItem

IMPL_PERSISTENT_OPS( MbPlaneItem )

#endif // __PLANE_ITEM_H
