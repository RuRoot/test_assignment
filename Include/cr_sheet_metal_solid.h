////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Построение оболочки из листового материала.
         \en Construction of a shell from sheet material. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_SHEET_METAL_SOLID_H
#define __CR_SHEET_METAL_SOLID_H


#include <cur_contour.h>
#include <action_sheet.h>
#include <creator.h>
#include <sheet_metal_param.h>


//------------------------------------------------------------------------------
/** \brief \ru Строитель оболочки из листового материала.
           \en Constructor of a shell from sheet material. \~
  \details \ru Строитель оболочки из листового материала операциями "Листовое тело", "Пластина", "Отверстие", "Вырез". \n 
    Листовое тело строится по замкнутому или разомкнутому контурам.
    В случае замкнутых контуров строится листовое тело, ограниченное этими контурами.
    Если контуров несколько, то один из них должен содержать внутри себя остальные,
    в этом случае он формирует внешнее очертание листа, а остальные контуры формируют 
    очертания вырезов в создаваемом листовом теле.
    Разомкнутый контур может быть только один и состоять из дуг и отрезков, причём дуги
    должны обязательно гладко стыковаться с соседними элементами контура. В процессе построения
    листового тела негладкие стыковки отрезков скругляются радиусом,
    заданным в параметрах операции (для построения фантома) или параметрах сгибов (в остальных случаях).
    В случае незамкнутого контура отрезки формируют плоские участки
    листового тела определённой в параметрах операции ширины, а дуги и скругления формируют сгибы.
    Операция "Пластина" расширяет плоский участок листового тела на заданные контуры.
    Операции "Отверстие" и "Вырез" с опцией "по толщине" строятся, как если бы вырезы выполнялись
    в полностью разогнутом теле с последующим выполнением необходимых сгибов.
           \en Constructor of a shell from sheet material using operations "Sheet solid", "Plate", "Hole", "Cutout \n 
    Sheet solid is built from a closed or open contour.
    In case of closed contours a sheet solid bounded by these contours are built.
    If there are several contours, then one of them should contain others inside itself,
    in this case it forms an external profile of a sheet, the other contours form 
    profiles of cutouts in the sheet solid being created.
    An open contour can be only one and it should consist of arcs and segments, besides arcs
    should be smoothly connected with the neighboring elements of the contour. During the construction
    of a sheet solid unsmooth joints of segments are filleted with radius
    specified in the operation parameters (for phantom construction) or in the bends parameters (in other cases).
    In case of open contour the segments form flat regions
    of a sheet solid which are specified in parameters of width operation, and arcs and fillets form bends.
    "Plate" operation extends flat part of a sheet solid to the given contours.
    Operations "Hole" and "Cutout" with option "By width" are constructed as if cutouts were performed
    in completely unbent solid with further performing the necessary bends. \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbSheetMetalSolid : public MbCreator {
protected:
  MbPlacement3D                 placement;  ///< \ru Локальная система координат образующих контуров. \en The local coordinate system of the generating contours. 
  std::vector<SPtr<MbContour> > curves;     ///< \ru Образующие контуры. \en Generating contours. 
  bool                          unbended;   ///< \ru Флаг построения листового тела в разогнутом виде. \en Flag of construction of a sheet solid in unbent state. 
  MbSheetMetalValues            parameters; ///< \ru Параметры построения. \en Construction parameters. 
  RPArray<MbSMBendNames>        bendParams; ///< \ru Параметры формируемых сгибов. \en Parameters of bends being formed. 
  OperationType                 operation;  ///< \ru Тип булевой операции. \en Boolean operation type. 
  double                        buildSag;   ///< \ru Максимальное отклонение нормали между соседними расчитанными точками линии пересечения. \en Maximal deviation of normal between the neighboring calculated points of the intersection line. 

public :
  MbSheetMetalSolid( const MbPlacement3D &           pl,
                     std::vector<SPtr<MbContour> > & c,
                           bool                      unbended,
                     const MbSheetMetalValues &      p, 
                     const RPArray<MbSMBendNames> &  bendNames,
                           OperationType             op,
                           double                    sag,
                     const MbSNameMaker &            n );
private :
  MbSheetMetalSolid( const MbSheetMetalSolid & init );
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbSheetMetalSolid( const MbSheetMetalSolid & init, MbRegDuplicate * ireg );
public :
  virtual ~MbSheetMetalSolid();

  // \ru Общие функции математического объекта \en Common functions of the mathematical object 

  MbeCreatorType  IsA() const override; // \ru Тип элемента \en Type of element 
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию \en Create a copy
  void        Transform( const MbMatrix3D & matr, MbRegTransform * = nullptr ) override;          // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move( const MbVector3D & to, MbRegTransform * = nullptr ) override;                 // \ru Сдвиг \en Translation
  void        Rotate( const MbAxis3D & axis, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate about an axis

  MbePrompt   GetPropertyName() override; // \ru Выдать заголовок свойства объекта \en Get a name of object property 
  void        GetProperties( MbProperties & properties ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & properties ) override; // \ru Записать свойства объекта \en Set properties of the object 
  void        GetBasisItems ( RPArray<MbSpaceItem> & ) override;   // \ru Дать базовые объекты \en Get the basis objects 
  void        GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  bool        IsSame   ( const MbCreator & other, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        IsSimilar( const MbCreator & item ) const override; // \ru Являются ли объекты подобными? \en Determine whether an object is similar?
  bool        SetEqual( const MbCreator & ) override;       // \ru Сделать равным \en Make equal

  // \ru Общие функции твердого тела \en Common functions of solid solid 

  bool        CreateShell( MbFaceShell *& shell, MbeCopyMode sameShell, 
                           RPArray<MbSpaceItem> * items = nullptr ) override; // \ru Построение \en Construction

  virtual MbFaceShell * InitShell( bool in ); 

  const MbPlacement3D & GetPlacement() const;

  // \ru Дать параметры. \en Get the parameters. 
  void        GetParameters(       MbSheetMetalValues & params ) const { params = parameters; }
  // \ru Установить параметры. \en Set the parameters. 
  void        SetParameters( const MbSheetMetalValues & params )       { parameters = params; }

private :
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
  void        operator = ( const MbSheetMetalSolid & );

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbSheetMetalSolid )
}; // MbSheetMetalSolid

IMPL_PERSISTENT_OPS( MbSheetMetalSolid )


//------------------------------------------------------------------------------
/** \brief \ru Построить оболочку из листового материала.
           \en Construct a shell from sheet material. \~
  \details \ru Построить оболочку из листового материала операциями "Листовое тело", "Пластина", "Отверстие", "Вырез". \n 
    Листовое тело строится по замкнутому или разомкнутому контурам.
    В случае замкнутых контуров строится листовое тело, ограниченное этими контурами.
    Если контуров несколько, то один из них должен содержать внутри себя остальные,
    в этом случае он формирует внешнее очертание листа, а остальные контуры формируют 
    очертания вырезов в создаваемом листовом теле.
    Одновременно с построением оболочки функция создаёт её строитель.\n
           \en Construct a shell from sheet material using operations "Sheet solid", "Plate", "Hole", "Cutout". \n 
    Sheet solid is built from a closed or open contour.
    In case of closed contours a sheet solid bounded by these contours are built.
    It there are several contours, then one of them should contain others inside itself,
    in this case it forms an external profile of a sheet, the other contours form 
    profiles of cutouts in the sheet solid being created.
    The function simultaneously creates the shell and its constructor.\n \~ 
  \param[in]  initialShell - \ru Исходная оболочка.
                             \en The initial shell. \~
  \param[in]  sameShell  - \ru Режим копирования исходной оболочки.
                           \en Mode of copying the initial shell. \~
  \param[in]  placement  - \ru Локальная система координат, в плоскости XY которй расположены контуры построния.
                           \en Local coordinate system the construction contours is located in XY plane of. \~
  \param[in]  contours   - \ru Контуры построния.
                           \en Construction contours. \~
  \param[in]  unbended   - \ru Флаг построения сгиба в разогнутом виде.
                           \en Flag of construction of a bend in unbent form. \~
  \param[in]  parameters - \ru Параметры построения.
                           \en Parameters of shell creation. \~
  \param[in]  oType      - \ru Тип булевой операции.
                           \en A Boolean operation type. \~
  \param[in]  sag        - \ru Угловой шаг для булевой операции.
                           \en Sag for a Boolean operation. \~
  \param[in]  nameMaker  - \ru Именователи граней.
                           \en An object for naming faces. \~
  \param[in]  resultBends - \ru Имена построенных сгибов.
                            \en Constructed bends names. \~
  \param[out] res - \ru Код результата операции.
                    \en Operation result code. \~
  \param[out] shell - \ru Построенная оболочка.
                      \en The resultant shell. \~
  \result \ru Возвращает строитель оболочки.
          \en Returns the shell constructor. \~
  \ingroup Model_Creators
*/
// ---
MATH_FUNC (MbCreator *) CreateSheetMetal(       SPtr<MbFaceShell> &      solid,
                                                MbeCopyMode              sameShell,
                                          const MbPlacement3D &          placement,
                                                RPArray<MbContour> &     contours,
                                                bool                     unbended,
                                          const MbSheetMetalValues &     parameters,
                                                OperationType            oType,
                                                double                   sag,
                                                RPArray<MbSNameMaker> *  nameMaker,
                                                RPArray<MbSMBendNames> & resultBends,
                                                MbResultType &           res,
                                                MbFaceShell *&           shell );


#endif // __CR_SHEET_METAL_SOLID_H
