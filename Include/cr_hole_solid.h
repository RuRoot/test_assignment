////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Строитель оболочки отверстия, кармана, фигурного паза.
         \en Constructor of shell of hole, pocket, groove. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_HOLE_SOLID_H
#define __CR_HOLE_SOLID_H


#include <cr_swept_solid.h>
#include <op_shell_parameter.h>
#include <mb_placement3d.h>


//------------------------------------------------------------------------------
/** \brief \ru Строитель оболочки отверстия, кармана, фигурного паза.
           \en Constructor of shell of hole, pocket, groove. \~
  \details \ru Строитель оболочки отверстия, кармана, фигурного паза. \n
    Построение отверстия происходит следующим образом : 
      создается контур сверла в конструктивной плоскости,
      вращением контура строится сверло, затем оно вычитается из присланного тела.
    Построение кармана/бобышки происходит следующим образом : 
      создается прямоугольный контур в конструктивной плоскости,
      который затем выдавливается в зависимости от типа объекта
      либо в положительном направлении Z, либо в отрицательном,
      затем скругляются боковые ребра и ребра на дне,
      далее вычитается карман из присланного тела или приклеивается бобышка.
    Построение фигурного паза происходит следующим образом : 
      создается контур паза в конструктивной плоскости,
      выдавливанием контура строится паз, затем он вычитается из присланного тела.
           \en Constructor of shell of hole, pocket, groove. \n
    Construction of a hole is performed as follows: 
      a contour of a drill is created in the constructive plane,
      and a drill is constructed by revolution of the contour; then the drill is subtracted from the given solid.
    A pocket/boss is constructed as follows: 
      a rectangular contour is created in the constructive plane
      which is extruded then
      either in the positive direction of Z or in the negative one subject to the object type;
      then the side edges and edges on the bottom are filleted;
      then the obtained pocket is subtracted from the given solid or the obtained boss is attached to the solid.
    The groove is constructed as follows: 
      a contour of a groove is created in the constructive plane;
      the groove is constructed by extrusion of the contour; then it is subtracted from the given solid. \~
  \ingroup Model_Creators
*/ 
// ---
class MATH_CLASS MbHoleSolid : public MbCurveSweptSolid {
protected :
  MbPlacement3D placement;  ///< \ru Плоскость отверстия. \en Plane of the hole. 
  HoleValues  * parameters; ///< \ru Параметры отверстия. \en The hole parameters. 

private :
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbHoleSolid( const MbHoleSolid & init ); 
  MbHoleSolid( const MbHoleSolid & init, MbRegDuplicate * ireg );
public :
  /// \ru Конструктор по параметрам. \en Constructor by parameters. 
  MbHoleSolid( const MbPlacement3D & pl, const HoleValues & p, 
               OperationType op, const MbSNameMaker & n ); 
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbHoleSolid();

public :

  // \ru Переопределение функций базового класса \en The base class functions override 

  MbeCreatorType  IsA() const override; // \ru Тип элемента \en A type of element 
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override;                       // \ru Сделать копию \en Create a copy
  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override;          // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move( const MbVector3D &, MbRegTransform * = nullptr ) override;               // \ru Сдвиг \en Translation
  void        Rotate( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate around an axis

  void        GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of the object 
  MbePrompt   GetPropertyName() override; // \ru Выдать заголовок свойства объекта \en Get a name of object property 
  void        GetBasisItems ( RPArray<MbSpaceItem> & ) override;  // \ru Дать базовые объекты \en Get the base objects 
  void        GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  bool        IsSame( const MbCreator &, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        IsSimilar( const MbCreator & ) const override; // \ru Являются ли объекты подобными \en Whether the objects are similar 
  bool        SetEqual ( const MbCreator & ) override;       // \ru Сделать равным \en Make equal 

  // \ru Общие функции твердого тела \en Common functions of solid 

  bool        CreateShell( MbFaceShell *& shell, MbeCopyMode sameShell, 
                           RPArray<MbSpaceItem> * items = nullptr ) override; // \ru Построение \en Construction

  MbFaceShell * InitShell( bool in ) override; 
  void        InitBasis( RPArray<MbSpaceItem> & items ) override; 
  bool        GetPlacement( MbPlacement3D & p ) const override;

private :
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
  void        operator = ( const MbHoleSolid & ); // \ru Не реализовано!!! \en Not implemented!!! 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbHoleSolid )
}; // MbHoleSolid

IMPL_PERSISTENT_OPS( MbHoleSolid )


//------------------------------------------------------------------------------
/** \brief \ru Создать оболочку с отверстием, карманом, или фигурным пазом.
           \en Create a shell with a hole, a pocket or a groove. \~
    \details \ru Для указанной оболочки построить оболочку с отверстием, карманом, или фигурным пазом. \n
                 Одновременно с построением оболочки функция создаёт её строитель. \n
             \en For a given shell construct a shell with a hole, a pocket or a groove. \n
                 The function simultaneously constructs the shell and creates its constructor. \n \~
    \param[in]  solid     - \ru Набор граней, к которым дополняется построение.
                            \en Face set the construction is complemented with respect to. \~
    \param[in]  sameShell - \ru Способ копирования граней.
                            \en The method of copying faces. \~
    \param[in]  params    - \ru Параметры операции.
                            \en Parameters of operation. ~
    \param[out] res       - \ru Код результата операции выдавливания.
                            \en The extrusion operation result code. \~
    \param[out] shell     - \ru Построенный набор граней.
                            \en Constructed set of faces. \~
    \result \ru Возвращает строитель.
            \en Returns the constructor. \~
    \ingroup Model_Creators
*/
// ---
MATH_FUNC (MbCreator *) CreateHole( const c3d::ShellSPtr &    solid,
                                          MbeCopyMode         sameShell,
                                    const MbHoleSolidParams & params,
                                          MbResultType &      res,
                                          c3d::ShellSPtr &    shell );


//------------------------------------------------------------------------------
/** \brief \ru Определить глубину отверстия "до указанной поверхности" при построении оболочки с отверстием.
           \en Determine the hole depth "to the specified surface" while creating a shell with a hole. \~
  \details \ru Определить глубину отверстия "до указанной поверхности" при построении оболочки с отверстием.
    Глубина отверстия "до поверхности" определяться расстоянием между 
    точкой привязки отверстия на поверхности расположения и 
    точкой пересечения оси отверстия с указанной поверхностью ограничения глубины.
           \en Determine the hole depth "to the specified surface" while creating a shell with a hole.
    The hole depth "to the surface" is defined by the distance between 
    the fasten point of the hole on the location surface and 
    a point of intersection of the hole axis with the given surface limiting the depth. \~
  \param[in]  face  - \ru Грань, до которой надо ограничить глубину.
                      \en A face terminating the depth. \~
  \param[in]  place - \ru Плоскость отверстия.
                      \en Plane of the hole. \~
  \param[in]  pars - \ru Параметры отверстия.
                     \en The hole parameters. \~
  \param[out] depth - \ru Глубина.
                      \en Depth. \~
  \return \ru true в случае , если расстояние было найдено \n false - если ось не пересекает грань
          \en True if the distance was found \n false - if the axis does not intersect the face \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (bool) GetDepthToFace( const MbFace &        face, 
                                 const MbPlacement3D & place, 
                                 const HoleValues &    pars, 
                                       double &        depth );


#endif // __CR_HOLE_SOLID_H
