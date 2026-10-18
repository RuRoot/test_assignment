////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Строитель разрезанного тела.
         \en Cut solid constructor.
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_CUTTING_SOLID_H
#define __CR_CUTTING_SOLID_H

#include <creator.h>
#include <mb_placement3d.h>
#include <op_boolean_flags.h>
#include <op_shell_parameter.h>


class   MbSurface;
class   MbContour;


//------------------------------------------------------------------------------
/** \brief \ru Строитель разрезанного тела.
           \en Cut solid constructor. \~
  \details \ru Строитель тела, разрезанного поверхностью или набором граней, полученного выдавливанием плоского контура.\n
           \en Constructor of a solid cut by a surface or a set of faces obtained by extrusion of a planar contour.\n \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbCuttingSolid : public MbCreator {
private:
  typedef MbShellCuttingParams::ProlongState CuttingProlongState;
protected :
  // Surface
  c3d::SurfaceSPtr        surface;      ///< \ru Режущая поверхность. \en Cutting surface. 
  // Sketch contour
  c3d::PlaneContourSPtr   contour;      ///< \ru Режущий контур (вместо поверхности). \en Cutting contour (instead of surface). 
  MbPlacement3D           place;        ///< \ru Местная система координат контура. \en Local coordinate system of the contour. 
  MbVector3D              direction;    ///< \ru Направление и длина выдавливания контура. \en Direction and distance of the contour extrusion. 
  // Solid
  c3d::CreatorsSPtrVector creators;     ///< \ru Строители оболочки. \en Shell creators. 

  ThreeStates             part;         ///< \ru Оставляемая часть (если part больше 0, то оставляем часть тела со стороны нормали поверхности). \en A part to be kept (if part is bigger than 0, then keep a part of  solid from the side of surface normal). 
  CuttingProlongState     prolongState; ///< \ru Тип продления режущей поверхности. \en Prolongation type of cutter surface.

  bool                    closed;       ///< \ru Замкнутость оболочки разрезаемого объекта. \en Closedness of the shell of the object being cut.
  bool                    mergeFaces;   ///< \ru Сливать подобные грани (true). \en Whether to merge similar faces (true). 
  bool                    mergeEdges;   ///< \ru Сливать подобные ребра (true). \en Whether to merge similar edges (true). 
  double                  buildSag;     ///< \ru Угловое отклонение при движении по кривым и поверхностям. \en Angular deviation while moving along curves and surfaces. 

public :
  MbCuttingSolid( const MbShellCuttingParams & cuttingParams, bool sameCutterObject );
private :
  MbCuttingSolid( const MbCuttingSolid &, MbRegDuplicate * ireg ); 
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbCuttingSolid( const MbCuttingSolid & ); 
public :
  virtual ~MbCuttingSolid();

  // \ru Общие функции математического объекта \en Common functions of the mathematical object 

  MbeCreatorType  IsA() const override; // \ru Тип элемента \en A type of element 
  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move( const MbVector3D &, MbRegTransform * = nullptr ) override;                 // \ru Сдвиг \en Translation
  void        Rotate( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate around an axis
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию \en Create a copy

  MbePrompt   GetPropertyName() override; // \ru Выдать заголовок свойства объекта \en Get a name of object property 
  void        GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of the object 
  void        GetBasisItems ( RPArray<MbSpaceItem> & ) override; // \ru Дать базовые объекты \en Get the base objects 
  void        GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  bool        IsSame( const MbCreator &, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        IsSimilar ( const MbCreator & ) const override; // \ru Являются ли объекты подобными \en Whether the objects are similar
  bool        SetEqual( const MbCreator & ) override;       // \ru Сделать равным \en Make equal

  // \ru Общие функции твердого тела \en Common functions of solid 

  bool        CreateShell( MbFaceShell *&, MbeCopyMode sameShell, 
                           RPArray<MbSpaceItem> * = nullptr ) override; // \ru Построение \en Construction

  // \ru Оставляемая часть (если part больше 0, то оставляем часть тела со стороны нормали поверхности). \en A part to be kept (if part is bigger than 0, then keep a part of  solid from the side of surface normal). 
  ThreeStates GetPart() const { return part; } 
  void        SetPart( ThreeStates p ) { part = p; } 
  void        SetOppositePart() { if ( part == ts_negative ) 
                                    part = ts_positive; 
                                  else if ( part == ts_positive )
                                    part = ts_negative; } 

private :
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
  void        operator = ( const MbCuttingSolid & ); 

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbCuttingSolid )
}; // MbCuttingSolid

IMPL_PERSISTENT_OPS( MbCuttingSolid )


//------------------------------------------------------------------------------
/** \brief \ru Отрезать от оболочки некоторую её часть.
           \en Cut a part of the shell. \~
  \details \ru Для указанной оболочки построить оболочку без части граней, отрезанных от неё : 
    (1) указанной поверхностью, (2) набором граней, полученной выдавливанием плоского контура, (3) оболочкой. \n
    Одновременно с построением оболочки функция создаёт её строитель. \n
           \en For a given shell create a shell without a part of faces cut from it by :
     (1) the given surface, (2) a set of faces obtained by extrusion of a planar contour, (3) the given shell. \n
    The function simultaneously constructs the shell and creates its constructor. \n \~
  \param[in]  initShell      - \ru Исходная оболочка.
                               \en The source shell. \~
  \param[in]  sameShell      - \ru Способ копирования граней исходной оболочки.
                               \en Method of copying the source shell faces. \~
  \param[in]  cuttingParams  - \ru Параметры операции.
                               \en Operation parameters. \~
  \param[out] resCode        - \ru Код результата операции.
                               \en Operation result code. \~
  \param[out] resShell1      - \ru Построенный первый набор граней.
                               \en Constructed first set of faces. \~  
  \param[out] resShell2      - \ru Построенный второй набор граней.
                               \en Constructed second set of faces. \~  
  \result \ru Возвращает строитель, если операция была выполнена успешно.
          \en Returns the constructor if the operation has been successfully performed. \~
  \ingroup Model_Creators
*/
// ---
MATH_FUNC (MbCuttingSolid *) CreatePart(       c3d::ShellSPtr &       initShell, 
                                               MbeCopyMode            sameShell, 
                                         const MbShellCuttingParams & cuttingParams,
                                               MbResultType &         resCode, 
                                               c3d::ShellSPtr &       resShell1,
                                               c3d::ShellSPtr &       resShell2 );


#endif // __CR_CUTTING_SOLID_H
