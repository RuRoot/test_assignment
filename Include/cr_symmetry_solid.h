////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Строитель симметричного тела.
         \en Constructor of a symmetric solid.
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_SYMMETRY_SOLID_H
#define __CR_SYMMETRY_SOLID_H


#include <creator.h>


class  MbSymmetrySolidParams;


//------------------------------------------------------------------------------
/** \brief \ru Строитель симметричного тела.
           \en Constructor of a symmetric solid. \~
  \details \ru Строитель симметричного тела разрезает тело плоскостью на две части, удаляет одну из них, 
    для оставшейся части строит симметричную относительно плоскости копию и склеивает её с оставшейся частью.
           \en Constructor of a symmetric solid cuts a solid by a plane onto to parts, deletes one of them, 
    for the remained part it builds a copy symmetric relative to the plane and glues it with the remained part. \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbSymmetrySolid : public MbCreator {
protected :
  MbCartPoint3D origin;   ///< \ru Начало плоскости симметрии. \en Symmetry plane origin. 
  MbVector3D    axisX;    ///< \ru Ось плоскости симметрии. \en Symmetry plane axis. 
  MbVector3D    axisY;    ///< \ru Ось плоскости симметрии. \en Symmetry plane axis. 
  int           side;     ///< \ru Оставляемая часть (если side>0, то оставляем часть тела со стороны нормали плоскости симметрии). \en Remained part (if side>0, then a part of solid from the side of symmetric plane's normal is to be remained). 
  double        buildSag; ///< \ru Угловое отклонение при движении по кривым и поверхностям. \en Angular deviation while moving along curves and surfaces.  

public :
  MbSymmetrySolid( const MbCartPoint3D & p, const MbVector3D & ax, const MbVector3D & ay, 
                   int s, const MbSNameMaker & n );
private :
  MbSymmetrySolid( const MbSymmetrySolid & init, MbRegDuplicate * iReg );
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbSymmetrySolid( const MbSymmetrySolid & init );
public :
  virtual ~MbSymmetrySolid();

  // \ru Общие функции математического объекта \en Common functions of the mathematical object 

  MbeCreatorType  IsA() const override; // \ru Тип элемента \en A type of element 
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию \en Create a copy
  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override;          // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move( const MbVector3D &, MbRegTransform * = nullptr ) override;                 // \ru Сдвиг \en Translation
  void        Rotate( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate around an axis

  void        GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of the object 
  MbePrompt   GetPropertyName() override; // \ru Выдать заголовок свойства объекта \en Get a name of object property 

  bool        IsSame( const MbCreator &, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        IsSimilar( const MbCreator & ) const override; // \ru Являются ли объекты подобными \en Whether the objects are similar 
  bool        SetEqual ( const MbCreator & ) override;       // \ru Сделать равным \en Make equal 

  // \ru Общие функции твердого тела \en Common functions of solid 

  bool        CreateShell( MbFaceShell *& shell, MbeCopyMode sameShell, 
                           RPArray<MbSpaceItem> * items = nullptr ) override; // \ru Построение \en Construction

private :
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
  void        operator = ( const MbSymmetrySolid & );

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbSymmetrySolid )
}; // MbSymmetrySolid

IMPL_PERSISTENT_OPS( MbSymmetrySolid )


//------------------------------------------------------------------------------
/** \brief \ru Создать симметричную оболочку.
           \en Create a symmetric shell. \~
  \details \ru Для указанной оболочки построить симметричную относительно указанной плоскости оболочку.
    Функция разрезает оболочку плоскостью на две части, удаляет одну из них, 
    для оставшейся части строит симметричную относительно плоскости копию и склеивает её с оставшейся частью. \n 
    Одновременно с построением оболочки функция создаёт её строитель. \n
           \en For a given shell build a shell symmetric relative to the specified plane.
    The function cuts the shell by the plane onto two parts, deletes one of them, 
    for the remained part it builds a copy symmetric relative to the plane and glues it with the remained part. \n 
    The function simultaneously constructs the shell and creates its constructor. \n \~
  \param[in]  solid          - \ru Исходная оболочка.
                               \en The source shell. \~
  \param[in] sameShell       - \ru Способ копирования граней исходной оболочки.
                               \en Method of copying the source shell faces. \~
  \param[in] symParams       - \ru Список параметров.
                               \en Parameters list. \~
  \param[out] res            - \ru Код результата операции выдавливания.
                               \en The extrusion operation result code. \~
  \param[out] shell          - \ru Построенный набор граней.
                               \en Constructed set of faces. \~  
  \result \ru Возвращает строитель, если операция была выполнена успешно.
          \en Returns the constructor if the operation has been successfully performed. \~
  \ingroup Model_Creators
*/
// ---
MATH_FUNC (MbCreator *) CreateSymmetry(       c3d::ShellSPtr &        solid, 
                                              MbeCopyMode             sameShell, 
                                        const MbSymmetrySolidParams & symParams,
                                              MbResultType &          res,
                                              c3d::ShellSPtr &        shell );


#endif // __CR_SYMMETRY_SOLID_H
