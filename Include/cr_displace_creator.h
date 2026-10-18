////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Строители перемещения объектов в пространстве.
         \en Constructors of displacement of an object.
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_DISPLACE_CREATOR_H
#define __CR_DISPLACE_CREATOR_H


#include <creator.h>
#include <mb_matrix3d.h>
#include <mb_axis3d.h>


//------------------------------------------------------------------------------
/** \brief \ru Перемещение объекта вдоль вектора.
           \en The shift of an object. \~
  \details \ru Строитель перемещает объект вдоль вектора на его длину. \n
           \en Constructor displaces an object along the vector by its length. \n \~ 
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbMotionMaker : public MbCreator {
protected: 
  MbVector3D vector; ///< \ru Вектор перемещения. \en The displacement vector. 

public: 
  /// \ru Конструктор по параметрам. \en Constructor by parameters. 
  MbMotionMaker( const MbVector3D & );
private:
  /// \ru Конструктор дублирующий. \en Duplication constructor.
  MbMotionMaker( const MbMotionMaker &, MbRegDuplicate * ireg );
  /// \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbMotionMaker( const MbMotionMaker & );

public: // \ru Деструктор \en Destructor 
  ~MbMotionMaker();

public: // \ru Общие функции математического объекта \en Common functions of the mathematical object 
  MbeCreatorType  IsA()  const override; // \ru Тип элемента \en A type of element 
  MbeCreatorType  Type() const override; // \ru Получить групповой тип объекта. \en Get the group type of the object. 
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию \en Create a copy
  bool        IsSame( const MbCreator & other, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal? 
  bool        SetEqual ( const MbCreator & ) override; // \ru Сделать равным \en Make equal 
  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать по матрице \en Transform according to the matrix
  void        Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override; // \ru Сдвиг по вектору \en Translation by a vector
  void        Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate around an axis

  MbePrompt   GetPropertyName() override; // \ru Выдать заголовок свойства объекта \en Get a name of object property 
  void        GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of the object 

  /// \ru Построение оболочки \en Creation of a shell.
  bool        CreateShell( MbFaceShell *& shell, MbeCopyMode sameShell, 
                           RPArray<MbSpaceItem> * items = nullptr ) override;
  /// \ru Построение каркаса кривых. \en Creation of a wire-frame. 
  bool        CreateWireFrame( MbWireFrame *& frame, MbeCopyMode sameShell,
                               RPArray<MbSpaceItem> * items = nullptr ) override;
  /// \ru Построение каркаса точек. \en Creation of a point-frame. 
  bool        CreatePointFrame( MbPointFrame *& frame, MbeCopyMode sameShell,
                                RPArray<MbSpaceItem> * items = nullptr ) override;
  /// \ru Создать полигональный объект. \en Create a polygonal object. 
  bool        CreateMesh( MbMesh *& mesh, MbeCopyMode sameShell,
                          RPArray<MbSpaceItem> * items = nullptr ) override;
  /// \ru Переместить строитель. \en Displace the creator.
  bool        Perform( MbCreator * ) const override;

  // \ru Добавить перемещение объекта вдоль вектора. \en Add a displacement vector. 
  void        AddVector( const MbVector3D & );
  // \ru Дать параметры. \en Get the parameters. 
  void        GetVector( MbVector3D & m ) const { m = vector; }
  // \ru Установить параметры. \en Set the parameters. 
  void        SetVector( const MbVector3D & m ) { vector = m; }

private:
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
  void        operator = ( const MbMotionMaker & );

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbMotionMaker )
}; // MbMotionMaker

IMPL_PERSISTENT_OPS( MbMotionMaker )


//------------------------------------------------------------------------------
/** \brief \ru Поворот объекта вокруг оси.
           \en Rotate an object around an axis. \~
  \details \ru Строитель поворачивает объект вокруг оси на заданный угол. \n
           \en Constructor rotates an object around the axis by the specified angle. \n \~ 
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbRotationMaker : public MbCreator {
protected: 
  MbAxis3D axis;  ///< \ru Ось вращения. \en The axis. 
  double   angle; ///< \ru Угол поворота. \en The angle of rotatation. 

public:
  /// \ru Конструктор по параметрам. \en Constructor by parameters. 
  MbRotationMaker( const MbAxis3D & ax, double an );
private:
  /// \ru Конструктор дублирующий. \en Duplication constructor.
  MbRotationMaker( const MbRotationMaker &, MbRegDuplicate * ireg );
  /// \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbRotationMaker( const MbRotationMaker & );

public: // \ru Деструктор \en Destructor 
  ~MbRotationMaker();

public: // \ru Общие функции математического объекта \en Common functions of the mathematical object 
  MbeCreatorType  IsA()  const override; // \ru Тип элемента \en A type of element 
  MbeCreatorType  Type() const override; // \ru Получить групповой тип объекта. \en Get the group type of the object. 
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию \en Create a copy
  bool        IsSame( const MbCreator & other, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal? 
  bool        SetEqual ( const MbCreator & ) override; // \ru Сделать равным \en Make equal 
  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать по матрице \en Transform according to the matrix
  void        Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override; // \ru Сдвиг по вектору \en Translation by a vector
  void        Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate around an axis

  MbePrompt   GetPropertyName() override; // \ru Выдать заголовок свойства объекта \en Get a name of object property 
  void        GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of the object 

  /// \ru Построение оболочки \en Creation of a shell.
  bool        CreateShell( MbFaceShell *& shell, MbeCopyMode sameShell, 
                           RPArray<MbSpaceItem> * items = nullptr ) override;
  /// \ru Построение каркаса кривых. \en Creation of a wire-frame. 
  bool        CreateWireFrame( MbWireFrame *& frame, MbeCopyMode sameShell,
                               RPArray<MbSpaceItem> * items = nullptr ) override;
  /// \ru Построение каркаса точек. \en Creation of a point-frame. 
  bool        CreatePointFrame( MbPointFrame *& frame, MbeCopyMode sameShell,
                                RPArray<MbSpaceItem> * items = nullptr ) override;
  /// \ru Создать полигональный объект. \en Create a polygonal object. 
  bool        CreateMesh( MbMesh *& mesh, MbeCopyMode sameShell,
                          RPArray<MbSpaceItem> * items = nullptr ) override;
  /// \ru Переместить строитель. \en Displace the creator.
  bool        Perform( MbCreator * ) const override;

  // \ru Добавить поворот вокруг оси. \en Add an angle of rotatation. 
  bool        AddAngle( const MbAxis3D & ax, double an );
  // \ru Дать параметры. \en Get the parameters. 
  void        GetAxis3D( MbAxis3D & m ) const { m = axis; }
  // \ru Установить параметры. \en Set the parameters. 
  void        SetAxis3D( const MbAxis3D & m ) { axis = m; }

private:
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
  void        operator = ( const MbRotationMaker & );

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbRotationMaker )
}; // MbRotationMaker

IMPL_PERSISTENT_OPS( MbRotationMaker )


//------------------------------------------------------------------------------
/** \brief \ru Трансформация объекта по матрице.
           \en Transformation of an object by matrix. \~
  \details \ru Строитель трансформирует объект по матрице. \n
           \en Constructor transforms an object by the matrix. \n \~ 
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbTransformationMaker : public MbCreator {
protected: 
  MbMatrix3D matrix; ///< \ru Матрица преобразования. \en The transform matrix. 

public:
  /// \ru Конструктор по параметрам. \en Constructor by parameters. 
  MbTransformationMaker( const MbMatrix3D & );
private:
  /// \ru Конструктор дублирующий. \en Duplication constructor.
  MbTransformationMaker( const MbTransformationMaker &, MbRegDuplicate * ireg );
  /// \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbTransformationMaker( const MbTransformationMaker & );

public: // \ru Деструктор \en Destructor 
  ~MbTransformationMaker();

public: // \ru Общие функции математического объекта \en Common functions of the mathematical object 
  MbeCreatorType  IsA()  const override; // \ru Тип элемента \en A type of element 
  MbeCreatorType  Type() const override; // \ru Получить групповой тип объекта. \en Get the group type of the object. 
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию \en Create a copy
  bool        IsSame( const MbCreator & other, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal? 
  bool        SetEqual ( const MbCreator & ) override; // \ru Сделать равным \en Make equal 
  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать по матрице \en Transform according to the matrix
  void        Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override; // \ru Сдвиг по вектору \en Translation by a vector
  void        Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate around an axis

  MbePrompt   GetPropertyName() override; // \ru Выдать заголовок свойства объекта \en Get a name of object property 
  void        GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of the object 

  /// \ru Построение оболочки \en Creation of a shell.
  bool        CreateShell( MbFaceShell *& shell, MbeCopyMode sameShell, 
                           RPArray<MbSpaceItem> * items = nullptr ) override;
  /// \ru Построение каркаса кривых. \en Creation of a wire-frame. 
  bool        CreateWireFrame( MbWireFrame *& frame, MbeCopyMode sameShell,
                               RPArray<MbSpaceItem> * items = nullptr ) override;
  /// \ru Построение каркаса точек. \en Creation of a point-frame. 
  bool        CreatePointFrame( MbPointFrame *& frame, MbeCopyMode sameShell,
                                RPArray<MbSpaceItem> * items = nullptr ) override;
  /// \ru Создать полигональный объект. \en Create a polygonal object. 
  bool        CreateMesh( MbMesh *& mesh, MbeCopyMode sameShell,
                          RPArray<MbSpaceItem> * items = nullptr ) override;
  /// \ru Переместить строитель. \en Displace the creator.
  bool        Perform( MbCreator * ) const override;

  // \ru Добавить модификацию по матрице \en Add a modification by a matrix 
  void        AddMatrix( const MbMatrix3D & );
  // \ru Дать параметры. \en Get the parameters. 
  void        GetMatrix( MbMatrix3D & m ) const { m = matrix; }
  // \ru Установить параметры. \en Set the parameters. 
  void        SetMatrix( const MbMatrix3D & m ) { matrix = m; }

private:
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
  void        operator = ( const MbTransformationMaker & );

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbTransformationMaker )
}; // MbTransformationMaker

IMPL_PERSISTENT_OPS( MbTransformationMaker )


#endif // __CR_DISPLACE_CREATOR_H
