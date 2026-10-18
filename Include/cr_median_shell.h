////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Построение срединной оболочки между гранями тела.
         \en Construction of a median shell between faces of solid. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_MEDIAN_SHELL_H
#define __CR_MEDIAN_SHELL_H


#include <creator.h>
#include <math_define.h>
#include <mb_operation_result.h>
#include <op_shell_parameter.h>


class    MbSNameMaker;
class    MedianShellFaces;


//------------------------------------------------------------------------------
/** \brief \ru Строитель срединной оболочки тела.
           \en Constructor of a median shell of solid. \~
  \details \ru Строитель осуществляет построение срединной оболочки между выбранными парами граней тела.
               Поверхности выбранные граней должны быть эквидистантны по отношению друг к другу. \n
           \en Constructor performs the building of a median shell between suitable selected face pairs of solid.
               Suitable face pairs should be equidistant from each other. \n \~ 
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbMedianShell : public MbCreator {
private :
  MedianShellFaces  faces;       ///< \ru Выбранные грани. \en Selected faces . 
  MedianShellValues parameters;  ///< \ru Параметры срединной оболочки. \en Parameters of median shell. 
  MbPrecision       precision;   ///< \ru Точность построения срединной оболочки. \en The precision of the construction of median shell.

public:
  /// \ru Конструктор по выбранным граням и параметрам срединной оболочки. \en Constructor by selected faces and parameters of median shell. 
  MbMedianShell( const MedianShellFaces & faces, const MedianShellValues & params, const MbSNameMaker & snMaker );
  /// \ru Конструктор по выбранным граням, параметрам срединной оболочки и точности построения. \en Constructor by selected faces, parameters of median shell and construction precision.
  MbMedianShell( const MedianShellFaces &  faces,
                 const MedianShellValues & params,
                 const MbSNameMaker &      snMaker,
                 const MbPrecision &       precision );
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbMedianShell();

private:
  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  MbMedianShell( const MbMedianShell &, MbRegDuplicate * );

public:
  MbeCreatorType  IsA()  const override; // \ru Тип элемента \en Type of element 
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию \en Create a copy
  bool        IsSame( const MbCreator &, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        IsSimilar( const MbCreator & ) const override;  // \ru Являются ли объекты подобными \en Whether the objects are similar 
  bool        SetEqual ( const MbCreator & ) override;        // \ru Сделать равным \en Make equal 
  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override; // \ru Сдвиг \en Translation
  void        Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate about an axis

  void        GetProperties( MbProperties & properties ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & properties ) override; // \ru Записать свойства объекта \en Set properties of the object 
  MbePrompt   GetPropertyName() override; // \ru Выдать заголовок свойства объекта \en Get a name of object property 

  // \ru Построение оболочки по исходным данным \en Construction of a shell from the given data 
  bool        CreateShell( MbFaceShell *& shell, MbeCopyMode sameShell, 
                           RPArray<MbSpaceItem> * items = nullptr ) override;
  
  const MbPrecision & GetPrecision() const override { return precision; } // \ru Выдать точность построения. \en Get the precision of object construction.

  /// \ru Дать параметры. \en Get the parameters. 
  void        GetParameters( MedianShellValues & params ) const { params = parameters; }
  /// \ru Установить параметры. \en Set the parameters. 
  void        SetParameters( const MedianShellValues & params ) { parameters = params; }

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbMedianShell )
  OBVIOUS_PRIVATE_COPY( MbMedianShell )
}; // MbMedianShell

IMPL_PERSISTENT_OPS( MbMedianShell )


//------------------------------------------------------------------------------
/** \brief \ru Построить срединную оболочку между выбранными парами граней тела.
           \en Build a median shell between selected faces of solid. \~
  \details \ru Построить срединную оболочку между выбранными парами граней тела.
               Грани должны принадлежать одному и тому же телу.
               Одновременно с построением оболочки функция создаёт её строитель.\n
           \en Build a median shell between suitable selected face pairs of solid.
               Suitable face pairs should be offset from each other.
               The faces must belong to the same body.
               The function simultaneously creates the shell and its constructor.\n \~
  \param[in]  solid       - \ru Исходное тело.
                            \en The initial solid. \~
  \param[in]  parameters  - \ru Параметры операции.
                            \en Parameters of operation. ~
  \param[out] resType     - \ru Код результата операции.
                            \en Operation result code. \~
  \param[out] results     - \ru Результат операции, включающий множество граней для создания срединной оболочки.
                            \en Operation result contains set of faces for build a median shell. \~
  \param[out] shell       - \ru Построенная срединная оболочка.
                            \en Constructed median shell. \~
  \result \ru Возвращает строитель оболочки.
          \en Returns the shell constructor. \~
  \ingroup Model_Creators
*/
// ---
MATH_FUNC (MbCreator *) CreateMedianShell( const MbFaceShell &          solid,
                                           const MbMedianShellParams &  params,
                                                 MbResultType &         resType,
                                                 MbMedianShellResults & results,
                                                 c3d::ShellSPtr &       shell );

#endif // __CR_MEDIAN_SHELL_H
