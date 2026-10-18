////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Параметры копирования.
         \en Parameters of duplication. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __OP_DUPLICATION_PARAMETERS_H
#define __OP_DUPLICATION_PARAMETERS_H

#include <templ_s_array.h>
#include <templ_dptr.h>
#include <math_define.h>
#include <math_version.h>
#include <mb_matrix3d.h>
#include <name_item.h>

class    MbAxis3D;
class              MbRegTransform;
class              MbRegDuplicate;
class    IProgressIndicator;


//------------------------------------------------------------------------------
/** \brief \ru Способы копирования.
           \en Types of parameters of duplication. \~
  \details \ru dt_Grid - Копии располагаются в узлах декартовой сетки, заданной двумя направлениями, 
               шагами и количеством шагов по каждому направлению, а также сдвигом относительно исходного положения.\n
               Исходное тело находится в центре сетки.\n
                      O---O---O \n
                     /   /   /  \n
                    O---O---O   \n
                   /   /   /    \n
                 [O]--O---O     \n
               dt_Polar - Копии располагаются в узлах полярной сетки, заданной вектором начального луча,
               вектором оси вращения, шагом по лучу, углом поворота, числом шагов по лучу и угловых шагов,
               а также сдвигом относительно исходного положения.\n
               Исходное тело находится в центре сетки.\n
                  O       O   \n
                   \     /    \n
                    O   O     \n
                     \ /      \n
                O--O-[O]-O--O \n
                dt_Matrix - Параметры копирования - массив матриц. Количество копий равно количеству матриц.
                Каждая копия получается из исходного тела трансформацией соответствующей матрицей. \n
           \en Dt_Grid - Copies locate in the nodes of the Cartesian grid, that define by two directions, 
               steps and numbers of steps along of each directions, and also by a shift from the initial position.\n
               The original solid locate in the center of the grid.\n
                      O---O---O \n
                     /   /   /  \n
                    O---O---O   \n
                   /   /   /    \n
                 [O]--O---O     \n
               dt_Polar - Copies locate in the nodes of the polar grid, that define by directions of the initial ray and the axis of rotation,
               steps on the ray, angle of rotation and number of steps on the ray and angular step,
               and also by a shift from the initial position.\n
               The original solid locate in the center of the grid.\n
                  O       O   \n
                   \     /    \n
                    O   O     \n
                     \ /      \n
                O--O-[O]-O--O \n
                dt_Matrix - Parameters of duplication is array of matrices. Number of copies equal to number of matrix.
                Each copy is obtained by transformation of corresponding matrix. \n \~
  \ingroup Model_Creators
 */
// ---
enum MbeDuplicatesType 
{
  dt_Grid  = 0 ,  ///< \ru Копии располагаются в узлах декартовой сетки. \en Copies locate in nodes of Cartesian grid.
  dt_Polar = 1 ,  ///< \ru Копии располагаются в узлах полярной сетки.\en Copies locate in nodes of a polar grid. 
  dt_Matrix = 2 , ///< \ru Копии трансформируются матрицами. \en Copies are transformed by matrices.
};


//------------------------------------------------------------------------------
/** \brief \ru Абстрактный класс параметров копирования.
           \en Abstract class of duplication parameters. \~
  \details \ru Родительский класс для всех видов параметров копирования. \n
           \en Parent class for all types of parameters of duplication. \n \~
  \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS DuplicationValues 
{
protected:
  /** \brief \ru Конструктор по умолчанию.
             \en Default constructor. \~
  */
  DuplicationValues() {}

public:  
  /** \brief \ru Деструктор.
             \en Destructor. \~
  */
  virtual ~DuplicationValues() {}

  /** \brief \ru Функция копирования.
             \en Copy function. \~
  */
  virtual bool        Init( const DuplicationValues & ) = 0;

  /** \brief \ru Тип параметров.
             \en Type of parameters \~
    \details \ru Возвращает способ копирования. \n
             \en Return type of parameters of duplication. \n \~
  */
  virtual MbeDuplicatesType Type() const = 0;

  /** \brief \ru Преобразовать параметры согласно матрице.
             \en Transform parameters according to the matrix. \~
  */
  virtual void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) = 0;

  /** \brief \ru Сдвинуть параметры вдоль вектора.
             \en Move parameters along a vector. \~
    \details \ru Сдвинуть параметры вдоль вектора.
             \en Move parameters along a vector. \n \~
   */
  virtual void        Move     ( const MbVector3D &, MbRegTransform * = nullptr ) = 0;

  /** \brief \ru Повернуть параметры вокруг оси на заданный угол.
             \en Rotate parameters at a given angle around an axis. \~
    \details \ru Повернуть параметры вокруг оси на заданный угол.
             \en Rotate parameters at a given angle around an axis. \n \~
   */
  virtual void        Rotate   ( const MbAxis3D &, double ang, MbRegTransform * = nullptr ) = 0;

  /** \brief \ru Выдать свойства объекта.
             \en Get properties of the object. \~
    \details \ru Выдать свойства объекта. \n
             \en Get properties of the object. \n \~
  */
  virtual void        GetProperties( MbProperties & ) = 0;

  /** \brief \ru Записать свойства объекта.
             \en Set properties of the object. \~
    \details \ru Записать свойства объекта. \n
             \en Set properties of the object. \n \~
  */
  virtual void        SetProperties( const MbProperties & ) = 0; 

  /** \brief \ru Являются ли объекты равными?
             \en Determine whether an object is equal? \~
    \details \ru Являются ли объекты равными? \n
             \en Determine whether an object is equal? \n \~
  */
  virtual bool        IsSame( const DuplicationValues &, double accuracy ) const = 0;

  /** \brief \ru Построить копию объекта.
             \en Create a copy of the object. \~
    \details \ru Построить копию объекта. \n
             \en Create a copy of the object. \n \~
  */
  virtual DuplicationValues & Duplicate( MbRegDuplicate * = nullptr ) const = 0;
  
  /** \brief \ru Сгенерировать матрицы трансформаций.
             \en Generate matrices of transformations. \~
    \details \ru Сгенерировать матрицы трансформаций согласно параметрам. \n
             \en Generate matrices of transformations according to parameters. \n \~
  */
  virtual void        GenerateTransformMatrices( std::vector<MbMatrix3D> & ) const = 0;

  /** \brief \ru Количество создаваемых копий.
             \en Number of of copies. \~
    \details \ru Количество создаваемых копий. \n
             \en Number of of copies. \n \~
  */
  virtual size_t      Count() const = 0;

OBVIOUS_PRIVATE_COPY( DuplicationValues )
};


//------------------------------------------------------------------------------
/** \brief \ru Копирование по сетке.
           \en Duplication by grid. \~
  \details \ru Параметры копирования по декартовой сетке или по полярной сетке.\n
               Тип сетки определяется флагом 'isPolar':\n 
               false - dt_Grid, true - dt_Polar.\n
               Исходное тело находится в центре сетки. \n
               Вектор 'axis1' задает одно из направлений декартовой сетки или направление луча в полярной сетке. \n
               Вектор 'axis2' задает другое направление декартовой сетки или ось вращения в полярной сетке, 
               точка оси не важна т.к. поворачиваются только вектора. \n
               'step1' и step2' задают шаги по направлениям декартовой сетки. \n
               'step1' - задает шаг по лучу полярной сетки, 'step2' задает угол поворота. \n
               'num1' и 'num2' задают кол-во шагов 'step1' и 'step2' соответственно. \n
               Ориентация тел на сетке определяется флагом 'isAlongAxis'. \n
               Если флаг 'isAlongAxis' = true, то тела ориентированы вдоль радиальной оси, false - параллельно исходному телу. \n
               'center' задает центр локальной системы координат. \n
               ВАЖНО! У векторов 'axis1', 'axis2' учитываются только направления, при инициализации или изменении происходит нормирование. \n
           \en Parameters of duplication by Cartesian grid or polar grid.\n
               Type of grid is determined by flag 'isPolar':\n 
               false - dt_Grid, true - dt_Polar.\n
               The original solid locate in the center of the grid. \n
               Vector 'axis1' define one of the directions of the Cartesian grid or direction of the ray in the polar grid. \n
               Vector 'axis2' define other directions of the Cartesian grid or the axis of rotation in the polar grid,
               point of axis doesn't matter because only vector will be rotated.
               'step1' and 'step2' define steps along the directions. \n
               'step1' define 'step' along the ray of polar grid, step2 define angle of rotation. \n
               'num1 and 'num2' define number of steps 'step1' and 'step2'. \n 
               Orientation of the bodies on the grid is controlled by 'isAlongAxis' flag. \n
               If 'isAlongAxis' = true, then bodies are oriented along radial axis, false - parallel to the origin body. \n
               'center' determines the origin of local coordinate system. \n \~
  \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS DuplicationMeshValues: public DuplicationValues 
{
protected:
  MbVector3D    axis1;        ///< \ru Направление копирования (dt_Grid и dt_Polar). \en Direction of duplication (dt_Grid and dt_Polar).
  MbVector3D    axis2;        ///< \ru Направление копирования (dt_Grid), направление оси вращения (dt_Polar). \en Direction of duplication (dt_Grid), direction of axis of rotation (dt_Polar).
  double        step1;        ///< \ru Шаг по направлению axis1 (dt_Grid и dt_Polar). \en Step along of axis1 (dt_Grid and dt_Polar).
  double        step2;        ///< \ru Шаг по направлению axis2 (dt_Grid), угол поворота (dt_Polar). \en Step along of axis1 (dt_Grid), angle of rotation (dt_Polar).
  uint          num1;         ///< \ru Кол-во шагов по направлению axis1 (dt_Grid и dt_Polar). \en Number of steps along of axis1 (dt_Grid and dt_Polar).
  uint          num2;         ///< \ru Кол-во шагов по направлению axis2 (dt_Grid), кол-во угловых шагов (dt_Polar). \en  Number of steps along of axis2 (dt_Grid), number of angular steps (dt_Polar).
  bool          isPolar;      ///< \ru Тип сетки, false - dt_Grid, true - dt_Polar. Type of grid, false - dt_Grid, true - dt_Polar. \en .
  bool          isAlongAxis;  ///< \ru true - тела расположены вдоль радиальной оси, false - параллельно исходному телу. \en true - along polar axis, false - parallel to initial body. 
  MbCartPoint3D center;       ///< \ru Центр локальной системы координат (и точка приложения оси вращения в случае полярной системы). \en Origin of the coordinate system.
public:
  /** \brief \ru Конструктор по типу.
             \en Constructor by a type. \~
    \details \ru Конструктор копирования по сетке.\n
             Вектора и значения инициализируются нулевыми, тип сетки инициализируется параметром, по умолчанию 'false'. \n
             \en Constructor of duplication by grid. \n
             Vectors and values are initialized to zero, type of grid is initialized by parameter, default 'false'. \n \~
  */
  DuplicationMeshValues( bool polar = false );

  /** \brief \ru Конструктор по параметрам и типу.
             \en Constructor by parameters and a type. \~
    \details \ru Конструктор копирования по сетке. \n
             Тип сетки, направления осей, шаги и кол-во шагов инициализируются параметрами. \n
             \en Constructor of duplication by grid. \n
             Type of grid, vectors, steps and numbers of steps are initialized to parameters. \n \~
  */
  DuplicationMeshValues( bool isPolar, const MbVector3D & dir1, const double step1, const uint num1, 
                                       const MbVector3D & dir2, const double step2, const uint num2,
                                       const MbCartPoint3D * center = nullptr,  bool isAlongAxis = false );

  /// \ru Деструктор. \en Destructor. 
  virtual ~DuplicationMeshValues();

  /// \ru Функция копирования. \en Copy function. 
          void        Init( const DuplicationMeshValues & );
  /// \ru Функция копирования. \en Copy function. 
  bool        Init( const DuplicationValues & ) override;
  /// \ru Тип параметров. \en Type of parameters. 
  MbeDuplicatesType Type() const override;
  /// \ru Преобразовать сетку согласно матрице. \en Transform grid according to the matrix. 
  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override;
  /// \ru Сдвинуть сетку вдоль вектора. \en Move grid along a vector. 
  void        Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override;
  /// \ru Повернуть сетку вокруг оси на заданный угол. \en Rotate grid at a given angle around an axis. 
  void        Rotate   ( const MbAxis3D &, double ang, MbRegTransform * = nullptr ) override;

  /// \ru Выдать свойства объекта \en Get properties of the object 
  void        GetProperties( MbProperties & ) override;
  /// \ru Записать свойства объекта \en Set properties of the object 
  void        SetProperties( const MbProperties & ) override; 

  // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        IsSame( const DuplicationValues &, double accuracy ) const override;

  /// \ru Построить копию объекта. \en Create a copy of the object. 
  DuplicationValues & Duplicate( MbRegDuplicate * = nullptr ) const override;
  
  /// \ru Сгенерировать матрицы трансформации. \en Generate matrix of transformation by. 
  void        GenerateTransformMatrices( std::vector<MbMatrix3D> & ) const override;
  
  /// \ru Количество создаваемых копий. \en Number of copies. 
  size_t      Count() const override;

  /** \brief \ru Установить одно из направлений сетки.
             \en Set one of the directions of grid. \~
    \details \ru Установить одно из направлений сетки. \n
             \en Set one of the directions of grid. \n \~
  */
          void        SetDirection( bool first, const MbVector3D & dir );
  
  /** \brief \ru Получить одно из направлений сетки.
             \en Get one of the directions of grid. \~
    \details \ru Получить одно из направлений сетки. \n
             \en Get one of the directions of grid. \n \~
  */
          void        GetDirection( bool first, MbVector3D & dir ) const;

  /** \brief \ru Установить шаг по одному из направлений сетки.
             \en Set the step along of one of the directions of grid. \~
    \details \ru Установить шаг по одному из направлений сетки. \n 
             \en Set the step along of one of the directions of grid. \n \~
  */
          void        SetStep( bool first, const double step );

  /** \brief \ru Получить шаг по одному из направлений сетки.
             \en Get the step along of one of the directions of grid. \~
    \details \ru Получить шаг по одному из направлений сетки. \n
             \en Get the step along of one of the directions of grid. \n \~
  */
          void        GetStep( bool first, double & step ) const;

  /** \brief \ru Установить количество шагов по одному из направлений сетки.
             \en Set the number of steps along of one of the directions of grid. \~
    \details \ru Установить количество шагов по одному из направлений сетки. \n
             \en Set the number of steps along of one of the directions of grid. \n \~
  */
          void        SetNumStep( bool first, const uint num );

  /** \brief \ru Полярная ли сетка?
             \en Is mesh polar? \~
    \details \ru Задана ли сетка в полярной системе координат? \n
             \en Is local system of mesh polar? \n \~
  */
          bool        IsPolar() const { return isPolar; }

  /** \brief \ru Задать тип сетки.
             \en Set mesh type. \~
    \details \ru Задать сетку в полярной или декартовой системе координат. \n
             \en Set local system of mesh. \n \~
  */
          void        SetPolar( bool p ) { isPolar = p; }

  /** \brief \ru Получить количество шагов по одному из направлений сетки.
             \en Get the number of steps along of one of the directions of grid. \~
    \details \ru Получить количество шагов по одному из направлений сетки. \n
             \en Get the number of steps along of one of the directions of grid. \n \~
  */
          void        GetNumStep( bool first, uint & num ) const;

            /** \brief \ru Тело вдоль радиальной оси?
             \en Is body along radial axis? \~
    \details \ru Тело вдоль радиальной оси? \n
             \en Is body along radial axis? \n \~
  */
          bool        IsAlongAxis() const { return isAlongAxis; }
 
  /** \brief \ru Вернуть центр полярной системы.
             \en Return center of the polar system. \~
    \details \ru Вернуть центр полярной системы. \n
             \en Return center of the polar system. \n \~
  */
  MbCartPoint3D        GetCenter() const { return center; }

    /** \brief \ru Установить центр полярной системы.
             \en Set center of the polar system. \~
    \details \ru Установить центр полярной системы. \n
             \en Set center of the polar system. \n \~
  */
  void                 SetCenter( MbCartPoint3D & cntr ) { center = cntr; }

KNOWN_OBJECTS_RW_REF_OPERATORS( DuplicationMeshValues ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class. 
OBVIOUS_PRIVATE_COPY( DuplicationMeshValues )
};


//------------------------------------------------------------------------------
/** \brief \ru Копирование с трансформациями по матрицам.
           \en Duplication by matrices. \~
  \details \ru Копирование задается набором матриц трансформаций. Каждая копия оригинального тела трансформируется соответствующей матрицей. \n
           \en Duplication is defined by set of transform matrices. Each copy of original solid is transforming by corresponding matrix. \n \~
  \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS DuplicationMatrixValues: public DuplicationValues 
{
public:
  std::vector<MbMatrix3D> matrices;
public:
  /// \ru Конструктор по умолчанию. \en Default constructor. 
  DuplicationMatrixValues();
  /// \ru Конструктор по матрице. \en Constructor by matrix. 
  DuplicationMatrixValues( const MbMatrix3D & );
  /// \ru Конструктор по набору матриц. \en Constructor by set of matrices. 
  DuplicationMatrixValues( const SArray<MbMatrix3D> & );
  /// \ru Конструктор по набору матриц. \en Constructor by set of matrices. 
  DuplicationMatrixValues( const std::vector<MbMatrix3D> & );

  /// \ru Деструктор. \en Destructor. 
  virtual ~DuplicationMatrixValues();

  /// \ru Функция копирования. \en Copy function. 
          void        Init( const DuplicationMatrixValues & );
  /// \ru Функция копирования. \en Copy function. 
  bool        Init( const DuplicationValues & ) override;
  /// \ru Тип параметров. \en Type of parameters. 
  MbeDuplicatesType Type() const override;
  /// \ru Преобразовать объект согласно матрице. \en Transform an object according to the matrix. 
  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override;
  /// \ru Сдвинуть объект вдоль вектора. \en Move an object along a vector. 
  void        Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override;
  /// \ru Повернуть объект вокруг оси на заданный угол. \en Rotate an object at a given angle around an axis. 
  void        Rotate   ( const MbAxis3D &, double ang, MbRegTransform * = nullptr ) override;

  /// \ru Выдать свойства объекта \en Get properties of the object 
  void        GetProperties( MbProperties & ) override;
  /// \ru Записать свойства объекта \en Set properties of the object 
  void        SetProperties( const MbProperties & ) override; 

  // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        IsSame( const DuplicationValues &, double accuracy ) const override;

  /// \ru Построить копию объекта. \en Create a copy of the object. 
  DuplicationValues & Duplicate( MbRegDuplicate * = nullptr ) const override;
  
  /// \ru Сгенерировать матрицы трансформации. \en Generate matrix of transformation. 
  void        GenerateTransformMatrices( std::vector<MbMatrix3D> & ) const override;
  
  /// \ru Количество создаваемых копий. \en Number of copies. 
  size_t      Count() const override;

KNOWN_OBJECTS_RW_REF_OPERATORS( DuplicationMatrixValues ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class. 
OBVIOUS_PRIVATE_COPY( DuplicationMatrixValues )
};


//------------------------------------------------------------------------------
/** \brief \ru Класс параметров копирования тел.
           \en Class with solid duplication parameters. \~
  \details \ru Класс параметров копирования тел.
           \en Class with solid duplication parameters. \~
  \ingroup Shell_Building_Parameters
*/
// ---
class MATH_CLASS MbDuplicationSolidParams
{
private:
  DPtr<DuplicationValues>       _duplicationValues; ///< \ru Параметры копирования. \en Copy parameters.
  c3d::IndicesVector            _selIndices;        ///< \ru Индексы выбранных граней оболочки. \en Indices of selected shell faces. 
  bool                          _adaptMode;         ///< \ru Режим адаптации открытых оболочек. \en Open shell instances adaptation mode. 
  MbSNameMaker                  _names;             ///< \ru Именователь операции. \en An object defining names generation in the operation.
public:
  mutable IProgressIndicator *  _progress;          ///< \ru Индикатор прогресса выполнения операции. \en A progress indicator of the operation.

private:
  /// \ru Конструктор по-умолчанию запрещён. \en The default constructor is forbidden.
  MbDuplicationSolidParams();

public:
  /// \ru Конструктор. \en Constructor.
  MbDuplicationSolidParams ( const DPtr<DuplicationValues> & values, IProgressIndicator * prog = nullptr )
    : _duplicationValues ( values ) // May be nullptr
    , _selIndices        (        )
    , _adaptMode         ( false  )
    , _names             (        )
    , _progress          ( prog   )
  {
    if ( _duplicationValues == nullptr )
    {
      C3D_ASSERT( _duplicationValues != nullptr );
      _duplicationValues = new DuplicationMatrixValues();
    }
  }

  /// \ru Конструктор. \en Constructor.
  MbDuplicationSolidParams ( const DPtr<DuplicationValues> & values,
                             const MbSNameMaker &            names,
                                   IProgressIndicator *      prog = nullptr  )
    : _duplicationValues ( values ) // May be nullptr
    , _selIndices        (        )
    , _adaptMode         ( false  )
    , _names             (        )
    , _progress          ( prog   )
  {
    if ( _duplicationValues == nullptr )
    {
      C3D_ASSERT( _duplicationValues != nullptr );
      _duplicationValues = new DuplicationMatrixValues();
    }
    _names.SetNameMaker( names, true );
  }

  /// \ru Конструктор на основе #DuplicationMeshValues для копирования по сетке. \en Constructor based on #DuplicationMeshValues to copy by grid.
  MbDuplicationSolidParams(       bool  isPolar, 
                            const MbVector3D & dir1, const double step1, const uint num1, 
                            const MbVector3D & dir2, const double step2, const uint num2,
                            const MbCartPoint3D * center,  bool isAlongAxis,
                                  IProgressIndicator * prog = nullptr )
    : _duplicationValues ( new DuplicationMeshValues(isPolar, dir1, step1, num1, dir2, step2, num2, center, isAlongAxis) )
    , _selIndices()
    , _adaptMode( false )
    , _names()
    , _progress( prog )
  { }

  /// \ru Конструктор на основе #DuplicationMatrixValues для копирования по матрицам. \en Constructor based on #DuplicationMatrixValues to copy by grid.
  MbDuplicationSolidParams( const MbMatrix3D & matr, IProgressIndicator * prog = nullptr )
    : _duplicationValues( new DuplicationMatrixValues(matr) )
    , _selIndices()
    , _adaptMode( false )
    , _names()
    , _progress( prog )
  { }

  /// \ru Конструктор на основе #DuplicationMatrixValues для копирования по матрицам. \en Constructor based on #DuplicationMatrixValues to copy by grid.
  MbDuplicationSolidParams( const SArray<MbMatrix3D> & matr, IProgressIndicator * prog = nullptr )
    : _duplicationValues( new DuplicationMatrixValues(matr) )
    , _selIndices()
    , _adaptMode( false )
    , _names()
    , _progress( prog )
  { }

  /// \ru Конструктор на основе #DuplicationMatrixValues для копирования по матрицам. \en Constructor based on #DuplicationMatrixValues to copy by grid.
  MbDuplicationSolidParams( const std::vector<MbMatrix3D> & matr, IProgressIndicator * prog = nullptr )
    : _duplicationValues( new DuplicationMatrixValues(matr) )
    , _selIndices()
    , _adaptMode( false )
    , _names()
    , _progress( prog )
  { }

  /// \ru Получить параметры копирования. \en Get the copy parameters.
  const DuplicationValues & GetDuplicationValues() const { return *_duplicationValues; }
  /// \ru Получить индексы выбранных граней оболочки. \en Get indices of selected shell faces.
        void                GetFacesIndices( c3d::IndicesVector & selIndices ) const { selIndices = _selIndices; }
  /// \ru Установить индексы выбранных граней оболочки. \en Set indices of selected shell faces.
        void                SetFacesIndices( const c3d::IndicesVector & selIndices ) { _selIndices = selIndices;  }
  /// \ru Режим адаптации открытых оболочек. \en Open shell instances adaptation mode. 
        bool                IsAdaptMode() const { return _adaptMode; }
  /// \ru Режим адаптации открытых оболочек. \en Open shell instances adaptation mode. 
        void                SetAdaptMode( bool enable ) { _adaptMode = enable; }
  /// \ru Получить именователь операции. \en Get the object defining names generation in the operation.
  const MbSNameMaker &      GetNameMaker() const { return _names; }
  /// \ru Установить именователь операции. \en Set the object defining names generation in the operation.
        void                SetNameMaker( const MbSNameMaker & names, bool version = true ) { _names.SetNameMaker( names, version ); }

OBVIOUS_PRIVATE_COPY( MbDuplicationSolidParams )
};


#endif // __OP_DUPLICATION_PARAMETERS_H
