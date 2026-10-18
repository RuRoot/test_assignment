////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Строитель оболочки тела вращения.
         \en Constructor of a revolution shell. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_REVOLUTION_SOLID_H
#define __CR_REVOLUTION_SOLID_H


#include <mb_axis3d.h>
#include <cr_swept_solid.h>


//------------------------------------------------------------------------------
/** \brief \ru Строитель оболочки тела вращения.
           \en Constructor of a revolution shell. \~
  \details \ru Строитель оболочки тела путём вращения образующих кривых вокруг заданной оси на заданный угол. \n
           \en Constructor of a solid's shell by revolution of generating curves around the given axis at the given angle. \n \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbCurveRevolutionSolid : public MbCurveSweptSolid {
protected :
  MbSweptData      sweptData;  ///< \ru Данные об образующей. \en Generating curve data. 
  MbAxis3D         axis;       ///< \ru Ось вращения образующих кривых. \en Rotation axis of the generating curves. 
  RevolutionValues parameters; ///< \ru Параметры. \en Parameters. 

public :
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор.
             \en Constructor. \~
    \param[in] sweptData_    - \ru Данные об образующей.
                               \en The generating curve data. \~
    \param[in] axis_         - \ru Ось вращения.
                               \en Rotation axis. \~
    \param[in] parameters_   - \ru Параметры вращения.
                               \en The revolution parameters. \~
    \param[in] oType         - \ru Тип булевой операции.
                               \en A Boolean operation type. \~
    \param[in] operNames     - \ru Именователь операции.
                               \en An object defining names generation in the operation. \~
    \param[in] contoursNames - \ru Именователь контуров для именования граней.
                               \en An object defining contours' names for faces naming. \~
  */
  MbCurveRevolutionSolid( const MbSweptData &           sweptData_,
                          const MbAxis3D &              axis_, 
                          const RevolutionValues &      parameters_, 
                                OperationType           oType, 
                          const MbSNameMaker &          operNames, 
                          const RPArray<MbSNameMaker> & contoursNames ); 

private :
  MbCurveRevolutionSolid( const MbCurveRevolutionSolid & init, MbRegDuplicate * ireg  );
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbCurveRevolutionSolid( const MbCurveRevolutionSolid & init );
public :
  virtual ~MbCurveRevolutionSolid();

  /** \ru \name Общие функции математического объекта.
      \en \name Common functions of the mathematical object.
      \{ */
  MbeCreatorType  IsA() const override; // \ru Тип элемента \en A type of element 
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию \en Create a copy
  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override;          // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move( const MbVector3D &, MbRegTransform * = nullptr ) override;                 // \ru Сдвиг \en Translation
  void        Rotate( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate around an axis

  void        GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of the object 
  MbePrompt   GetPropertyName() override; // \ru Выдать заголовок свойства объекта \en Get a name of object property 
  void        GetBasisItems ( RPArray<MbSpaceItem>   & s ) override; // \ru Дать базовые объекты \en Get the base objects 
  void        GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  bool        IsSame( const MbCreator &, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        IsSimilar( const MbCreator & ) const override; // \ru Являются ли объекты подобными \en Whether the objects are similar 
  bool        SetEqual ( const MbCreator & ) override;       // \ru Сделать равным \en Make equal 
  
  /** \} */
  /** \ru \name Общие функции твердого тела (формообразующей операции).
      \en \name Common functions of the rigid solid (forming operations).
      \{ */
  MbFaceShell * InitShell( bool in ) override; 
  void        InitBasis( RPArray<MbSpaceItem> & items ) override; 
  bool        GetPlacement( MbPlacement3D & ) const override;
  /** \} */
  /** \ru \name Функции строителя оболочки тела вращения.
      \en \name Functions of the revolution solid's shell creator.
      \{ */
  const MbSurface *   GetSurface() const { return sweptData.GetSurface(); }  ///< \ru Поверхность двумерных контуров. \en Surface of two-dimensional contours. 
  const MbAxis3D &    GetAxis()    const { return axis; }                    ///< \ru Ось вращения. \en Rotation axis. 

  /// \ru Дать параметры. \en Get the parameters. 
  void        GetParameters(       RevolutionValues & p ) const { p = parameters; }
  /// \ru Установить параметры. \en Set the parameters. 
  void        SetParameters( const RevolutionValues & p )       { parameters = p; }
  /** \} */

private :
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
  void        operator = ( const MbCurveRevolutionSolid & ); 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbCurveRevolutionSolid )
}; // MbCurveRevolutionSolid

IMPL_PERSISTENT_OPS( MbCurveRevolutionSolid )


//------------------------------------------------------------------------------
/** \brief \ru Создать оболочку тела вращения.
           \en Create a shell of the revolution solid. \~
  \details \ru Построить оболочку тела путём вращения образующих кривых кривых вокруг заданной оси на заданный угол
    и выполнить булуву операцию с оболочкой, если последняя задана. \n
    Одновременно с построением оболочки функция создаёт её строитель.\n
           \en Construct a shell of a solid by rotating the generating curves around the given axis at the specified angle.
    and perform the Boolean operation with the shell if it is specified. \n
    The function simultaneously creates the shell and its constructor.\n \~ 
  \param[in]       solid          - \ru Набор граней, к которым дополняется построение.
                                    \en Face set the construction is complemented with respect to. \~
  \param[in]       sameShell      - \ru Способ копирования граней.
                                    \en The method of copying faces. \~
  \param[in]       sweptData      - \ru Данные об образующей.
                                    \en The generating curve data. \~
  \param[in]       axis           - \ru Ось вращения.
                                    \en Rotation axis. \~
  \param[in, out]  params         - \ru Параметры выдавливания.
    Возвращают информацию для построения элементов массива операция до поверхности.
                                    \en The extrusion parameters.
    Returns the information for construction of elements of operation-to-surface array. \~
  \param[in]       oType          - \ru Тип операции дополнения построения.
                                    \en Type of operation of construction complement. \~
  \param[in]       operNames      - \ru Именователь операции.
                                    \en An object defining names generation in the operation. \~
  \param[in]       contoursNames  - \ru Именователь контуров.
                                    \en An object defining the names of contours. \~
  \param[out]      resType        - \ru Код результата операции выдавливания.
                                    \en The extrusion operation result code. \~  
  \param[out]      shell          - \ru Построенный набор граней.
                                    \en Constructed set of faces. \~  
  \result \ru Возвращает строитель.
          \en Returns the constructor. \~
  \ingroup Model_Creators
*/
// ---
MATH_FUNC (MbCreator *) CreateCurveRevolution(       MbFaceShell *           solid, 
                                                     MbeCopyMode             sameShell, 
                                               const MbSweptData &           sweptData, 
                                               const MbAxis3D &              axis, 
                                               const RevolutionValues &      params, 
                                                     OperationType           oType, 
                                               const MbSNameMaker &          operNames, 
                                               const RPArray<MbSNameMaker> & contoursNames, 
                                                     MbResultType &          resType, 
                                                     c3d::ShellSPtr &        shell );


#endif // __CR_REVOLUTION_SOLID_H
