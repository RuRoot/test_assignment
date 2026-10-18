////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Построение оболочки на сетке кривых.
         \en Construction of a shell from a mesh of curves. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_MESH_SHELL_H
#define __CR_MESH_SHELL_H


#include <math_define.h>
#include <mb_operation_result.h>
#include <creator.h>
#include <op_shell_parameter.h>


class   MbFaceShell;


//------------------------------------------------------------------------------
/** \brief \ru Строитель оболочки на сетке кривых.
           \en Constructor of a shell from a mesh of curves. \~
  \details \ru Строитель оболочки на сетке кривых, образованной двумя сечействами кривых. \n
           \en Constructor of a shell from a mesh of curves formed by two sets of curves. \n \~ 
  \ingroup Model_Creators
*/
//---
class MATH_CLASS MbMeshShell : public MbCreator {
private : 
  MeshSurfaceValues parameters; ///< \ru Параметры построения. \en Construction parameters. 
  mutable bool      changed;    ///< \ru Флаг изменения параметров. \en Flag of parameters modification. 
private:
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbMeshShell( const MbMeshShell & obj, MbRegDuplicate * ireg );
public:
  /// \ru Конструктор по параметрам операции и именователю на оригиналах кривых и копиях поверхностей. \en Constructor by operation parameters and name-maker for original curves and copies of surfaces. 
  MbMeshShell( const MeshSurfaceValues & pars, const MbSNameMaker & n );  
  virtual ~MbMeshShell();

public: // \ru Общие функции математического объекта \en Common functions of the mathematical object 
  MbeCreatorType  IsA() const override; ///< \ru Тип элемента \en Element type 
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override; ///< \ru Сделать копию \en Make a copy
  bool        IsSame( const MbCreator &, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        SetEqual ( const MbCreator & ) override; ///< \ru Сделать равным \en Make equal 
  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; ///< \ru Преобразовать по матрице \en Transform according to the matrix
  void        Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override; ///< \ru Сдвиг по вектору \en Translation by the vector
  void        Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; ///< \ru Поворот вокруг оси \en Rotation about an axis

  MbePrompt   GetPropertyName() override; ///< \ru Выдать заголовок свойства объекта \en Get name of object property 
  void        GetProperties( MbProperties & ) override; ///< \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & ) override; ///< \ru Записать свойства объекта \en Write properties of the object 
  void        GetBasisItems( RPArray<MbSpaceItem> & ) override;  // \ru Дать базовые объекты \en Get the basis objects 
  void        GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

public: 
  /// \ru Построение оболочки \en Creation of a shell 
  bool        CreateShell( MbFaceShell *& shell, MbeCopyMode sameShell, 
                           RPArray<MbSpaceItem> * items = nullptr ) override;

  // \ru Дать параметры. \en Get the parameters. 
  void        GetParameters(       MeshSurfaceValues & params ) const;
  // \ru Установить параметры. \en Set the parameters. 
  void        SetParameters( const MeshSurfaceValues & params );

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbMeshShell )
  OBVIOUS_PRIVATE_COPY( MbMeshShell )
}; // MbMeshShell

IMPL_PERSISTENT_OPS( MbMeshShell )


//------------------------------------------------------------------------------
/** \brief \ru Построить оболочку на сетке кривых.
           \en Construct a shell from a mesh of curves. \~
  \details \ru Построить оболочку на сетке кривых, образованной двумя семействами кривых. \n
               Одновременно с построением оболочки функция создаёт её строитель.\n
           \en Create a shell from a mesh of curves formed by two sets of curves. \n
               The function simultaneously creates the shell and its constructor.\n \~ 
  \param[in]  parameters  - \ru Параметры операции.
                            \en Parameters of operation. ~
  \param[out] res         - \ru Код результата операции.
                            \en Operation result code. \~
  \param[out] shell       - \ru Построенная оболочка.
                            \en Constructed mesh shell. \~
  \result \ru Возвращает строитель оболочки.
          \en Returns the shell constructor. \~
  \ingroup Model_Creators
*/
// ---
MATH_FUNC (MbCreator *) CreateMeshShell( const MbMeshShellParameters & parameters, 
                                               MbResultType          & res, 
                                               c3d::ShellSPtr        & shell );


#endif // __CR_MESH_SHELL_H
