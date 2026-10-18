////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Построение оболочки элементарного тела.
         \en Construction of shell for elementary solid. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_ELEMENTARY_SOLID_H
#define __CR_ELEMENTARY_SOLID_H

#include <creator.h>
#include <mb_enum.h>

class    MbElementarySurface;
class    MbElementarySolidParams;


//------------------------------------------------------------------------------
/** \brief \ru Строитель оболочки элементарного тела.
           \en Constructor of shell for elementary solid. \~
  \details \ru Строитель оболочки элементарного тела по набору опорных точек и типу: \n
    solidType = et_Sphere      - шар             (3 точки), \n
    solidType = et_Torus       - тор             (3 точки), \n
    solidType = et_Cylinder    - цилиндр         (3 точки), \n
    solidType = et_Cone        - конус           (3 точки), если конус не усеченный, 
                                                 (4 точки), если конус усеченный, \n
    solidType = et_Block       - блок            (4 точки), \n
    solidType = et_Wedge       - клин            (4 точки), \n
    solidType = et_Prism       - призма          (количество вершин основания+1 точка), \n
    solidType = et_Pyramid     - пирамида        (количество вершин основания+1 точка), \n
    solidType = et_Plate       - плита           (4 точки). \n
           \en Constructor of shell for elementary solid by a set of support points and a type: \n
    solidType = et_Sphere      - a sphere        (3 points), \n
    solidType = et_Torus       - a torus         (3 points), \n
    solidType = et_Cylinder    - a cylinder      (3 points), \n
    solidType = et_Cone        - a cone          (3 points), in the case of a non-frustum cone,
                                                 (4 points), in the case of a frustum cone, \n
    solidType = et_Block       - a block         (4 points), \n
    solidType = et_Wedge       - a wedge         (4 points), \n
    solidType = et_Prism       - a prism         (points count is equal to the base vertices count + 1), \n
    solidType = et_Pyramid     - a pyramid       (points count is equal to the base vertices count + 1), \n
    solidType = et_Plate       - a plate         (4 points). \n \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbElementarySolid : public MbCreator {
protected :
  SArray<MbCartPoint3D> points;      ///< \ru Опорные точки оболочки тела. \en Support points of a solid shell. 
  ElementaryShellType   type;        ///< \ru Тип тела. \en Type of a solid. 
  MbPlacement3D         position;    ///< \ru Локальная система координат тела. \en Local coordinate system of a solid.  
  double                radius;      ///< \ru Радиус основания тела. \en Radius of the base of the solid.   
  double                minorRadius; ///< \ru Малый радиус основания тела. \en Small radius of the base of the solid.
  double                height;      ///< \ru Высота тела. \en Height of a solid.
  double                length;      ///< \ru Длина тела. \en Length of a solid.
  double                minorLength; ///< \ru Малая длина тела. \en Small length of a solid.
  double                width;       ///< \ru Ширина тела. \en Width of a solid.
  double                angle;       ///< \ru Угол между осью position.axisZ и боковой образующей. \en Angle between position.axisZ axis and lateral generatrix.
  double                ratio;       ///< \ru Коэффициент растяжения. \en Stretch factor.
public :
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по точкам и типу тела. При этом, по массиву точек тела и его типу наполняются соответствующие его параметры.
             \en Constructor by points and type of solid. 
             At the same time, the corresponding parameters are filled in by the array of points of the body and its type. \~

    \param[in] pnts - \ru Опорные точки. \n
               pnts[0] определяет начало локальной системы координат. \n
               Для сферы, тора, цилиндра, конуса: \n
               pnts[1] определяет направление оси Z локальной системы. \n
               pnts[2] определяет направление оси X локальной системы. 
                       Для усеченного конуса (в случае 4-ех точек), если точка pnts[2] лежит на прямой, 
                       образованной точками pnts[0] и pnts[1], то направление оси X 
                       локальной системы определяет точка pnts[3]. \n
               Для блока, клина и плиты: \n
               pnts[1] определяет направление оси X локальной системы. \n
               pnts[2] определяет направление оси Y локальной системы. \n
               Кроме того, \n
               pnts[1] определяет высоту цилиндра, высоту конуса, высоту усеченного конуса,
                       большой радиус тора, длину блока, длину клина. \n
               pnts[2] определяет радиус цилиндра, радиус конуса (в случае трех точек), 
                       верхний или нижний радиус усеченного конуса (в случае четырех точек), 
                       радиус сферы, малый радиус тора, ширину блока, ширину клина. \n
               Последняя точка определяет высоту блока, клина, плиты, вершину пирамиды, 
               верхний или нижний радиус усеченного конуса (в случае четырех точек).
               \en Support points. \
               pnts[0] determines a local coordinate system origin. \n
               For a sphere, a torus, a cylinder, a cone: \n
               pnts[1] determines the direction of Z-axis of a local coordinate system. \n
               pnts[2] determines the direction of X-axis of a local coordinate system. 
                       For a frustum cone (in the case of 4 points), if point pnts[2] lies on a line that
                       formed by points pnts[0] and pnts[1], then the X axis
                       of a local system is determined by point pnts[3].\n
               For a block, a plate or a wedge: \n
               pnts[1] determines the direction of X-axis of a local coordinate system. \n
               pnts[2] determines the direction of Y-axis of a local coordinate system. \n
               Also, \n
               pnts[1] determines the height of a cylinder, a cone or a frustum cone,
                       the major radius of a torus, the length of a block or a wedge. \n
               pnts[2] determines the radius of a cylinder or a cone (in the case of 3 points), 
                       radius of the upper or lower base of a frustum cone (in the case of 4 points),
                       radius of a sphere, the minor radius of a torus, the width of a block or a wedge. \n
               The last point determines the height of a block, a wedge or a plate, the vertex of a pyramid,
               radius of the upper or lower base of a frustum cone (in the case of 4 points) \~
    \param[in] t - \ru Тип элементарного тела.
                   \en Elementary solid type. \~
    \param[in] n - \ru Именователь операции.
                   \en An object defining names generation in the operation. \~
  */
  template<class PointsVector>
  MbElementarySolid( const PointsVector & pnts, ElementaryShellType t, const MbSNameMaker & n );

private :
  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  MbElementarySolid( const MbElementarySolid &, MbRegDuplicate * ); 
  /// \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbElementarySolid( const MbElementarySolid & ); 
public :
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbElementarySolid();

  /** \ru \name Общие функции строителя оболочки.
      \en \name Common functions of the shell creator.
      \{ */
  MbeCreatorType IsA() const override; // \ru Тип элемента \en Type of element
  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move( const MbVector3D &, MbRegTransform * = nullptr ) override;                 // \ru Сдвиг \en Translation
  void        Rotate( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate about an axis
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию \en Create a copy

  MbePrompt   GetPropertyName() override; // \ru Выдать заголовок свойства объекта \en Get a name of object property
  void        GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of the object 
  void        GetBasisItems( RPArray<MbSpaceItem> & ) override;   // \ru Дать базовые объекты \en Get the basis objects 
  void        GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  bool        IsSame( const MbCreator &, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        IsSimilar ( const MbCreator & ) const override; // \ru Являются ли объекты подобными \en Whether the objects are similar
  bool        SetEqual( const MbCreator & ) override;       // \ru Сделать равным \en Make equal

  bool        CreateShell( MbFaceShell *&, MbeCopyMode sameShell,
                           RPArray<MbSpaceItem> * = nullptr ) override; // \ru Построение \en Construction
  /** \} */

private:
  /// \ru Установить параметры по типу и набору точек. \en Set parameters by type and points.
  bool        SetParameters();
private :
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
  void        operator = ( const MbElementarySolid & ); 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbElementarySolid )
}; // MbElementarySolid

IMPL_PERSISTENT_OPS( MbElementarySolid )


//------------------------------------------------------------------------------
// \ru Конструктор по точкам и типу тела. При этом, по массиву точек тела и его типу, заполняются соответствующие его параметры.
// \en Constructor by points and type of solid.
// ---
template<class PointsVector>
MbElementarySolid::MbElementarySolid( const PointsVector & pnts, ElementaryShellType t, const MbSNameMaker & n )
  : MbCreator  ( n   )
  , points     (     )
  , type       ( t   )
  , position   (     )
  , radius     ( 0.0 )
  , minorRadius( 0.0 )
  , height     ( 0.0 )
  , length     ( 0.0 )
  , minorLength( 0.0 )
  , width      ( 0.0 )
  , angle      ( 0.0 )
  , ratio      ( 0.0 )
{
  size_t cnt = pnts.size();
  points.reserve( cnt );
  for ( size_t k = 0; k < cnt; ++k ) {
    points.push_back( pnts[k] );
  }
  SetParameters();
}

#endif // __CR_ELEMENTARY_SOLID_H
