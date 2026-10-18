////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Строитель трансформируемой оболочки.
         \en Constructor of a transformed shell.
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_TRANSFORMED_SOLID_H
#define __CR_TRANSFORMED_SOLID_H


#include <creator.h>
#include <op_shell_parameter.h>
#include <mb_cube.h>


//------------------------------------------------------------------------------
/** \brief \ru Строитель трансформируемой оболочки.
           \en Constructor of a transformed shell. \~
  \details \ru Строитель трансформируемой оболочки, матрица преобразования которой 
    получена по изменению положения контрольных точек габаритного куба. \n
           \en Constructor of a transformed shell the transformation matrix of which 
    is obtained due to the change of bounding box control points positions. \n \~ 
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbTransformedSolid : public MbCreator {
protected: 
  TransformValues       parameters; ///< \ru Параметры преобразования оболочки. \en Parameters of a shell transformation. 
  MbCube                cube;       ///< \ru Габаритный куб. \en Bounding box. 
  SArray<MbCartPoint3D> cubePoints; ///< \ru Контрольные точки куба. \en Control points of the bounding box. 

public: // \ru Конструктор по параметрам \en Constructor by parameters 
  MbTransformedSolid( const TransformValues &, const MbCube &, const MbSNameMaker & );
private: // \ru Конструктор дублирующий \en Duplication constructor 
  MbTransformedSolid( const MbTransformedSolid &, MbRegDuplicate * ireg );
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbTransformedSolid( const MbTransformedSolid & );

public: // \ru Деструктор \en Destructor 
  ~MbTransformedSolid();

public: // \ru Общие функции математического объекта \en Common functions of the mathematical object 
  MbeCreatorType  IsA() const override; // \ru Тип элемента \en A type of element 
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию \en Create a copy
  bool        IsSame( const MbCreator & other, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal? 
  bool        SetEqual ( const MbCreator & ) override; // \ru Сделать равным \en Make equal 
  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать по матрице \en Transform according to the matrix
  void        Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override; // \ru Сдвиг по вектору \en Translation by a vector
  void        Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate around an axis

  MbePrompt   GetPropertyName() override; // \ru Выдать заголовок свойства объекта \en Get a name of object property 
  void        GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of the object 
  void        GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  /// \ru Построение оболочки \en Creation of a shell 
  bool        CreateShell( MbFaceShell *&, MbeCopyMode sameShell,
                           RPArray<MbSpaceItem> * items = nullptr ) override;
  void        Refresh( MbFaceShell & ) override; ///< \ru Обновить форму оболочки \en Update shape of the shell 
  // \ru Добавить модификацию по матрице \en Add a modification by a matrix 
  void        AddMatrix( MbFaceShell &, const MbMatrix3D & );

  // \ru Дать параметры. \en Get the parameters. 
  void        GetParameters(       TransformValues & params ) const { params = parameters; }
  // \ru Установить параметры. \en Set the parameters. 
  void        SetParameters( const TransformValues & params )       { parameters = params; }

private:
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
  void        operator = ( const MbTransformedSolid & );

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbTransformedSolid )
}; // MbTransformedSolid

IMPL_PERSISTENT_OPS( MbTransformedSolid )


//------------------------------------------------------------------------------
/** \brief \ru Создание строителя масштабированной оболочки.
           \en Creation of constructor of a scaled shell. \~
  \details \ru Построить оболочку путём трансформации исходной оболочки по матрице преобразования, 
    полученной по изменению положения контрольных точек габаритного куба.\n
    Одновременно с построением оболочки функция создаёт её строитель.\n
           \en Create a shell by transformation of the source shell according to the transformation matrix 
    obtained due to the change of the bounding box control points positions.\n
    The function simultaneously creates the shell and its constructor.\n \~ 
  \param[in]  outer      - \ru Исходная оболочка.
                           \en The source shell. \~
  \param[in]  sameShell  - \ru Режим копирования исходной оболочки.
                           \en Mode of copying the source shell. \~
  \param[in]  parameters - \ru Параметры модификации.
                           \en Parameters of the modification. \~
  \param[in]  names      - \ru Именователь операции.
                           \en An object defining names generation in the operation. \~
  \param[out] res        - \ru Код результата операции выдавливания.
                           \en The extrusion operation result code. \~
  \param[out] shell      - \ru Построенная оболочка.
                           \en The resultant shell. \~
  \result \ru Возвращает строитель оболочки.
          \en Returns the shell constructor. \~
  \ingroup Model_Creators
*/
// ---
MATH_FUNC (MbCreator *) CreateTransformedSolid(       MbFaceShell *     outer, 
                                                      MbeCopyMode       sameShell, 
                                                const TransformValues & parameters,
                                                const MbSNameMaker &    names, 
                                                      MbResultType &    res,
                                                      MbFaceShell *&    shell );


#endif // __CR_TRANSFORMED_SOLID_H
