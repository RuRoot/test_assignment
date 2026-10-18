////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Строитель модифицированной оболочки.
         \en Constructor of a modified shell. 
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_MODIFIED_SOLID_H
#define __CR_MODIFIED_SOLID_H


#include <creator.h>
#include <op_shell_parameter.h>
#include <op_direct_mod_parameter.h>


//------------------------------------------------------------------------------
/** \brief \ru Строитель модифицированной оболочки.
           \en Constructor of a modified shell. \~
  \details \ru Строитель оболочки, выполняющий модификацию исходной оболочки.  
    Строитель выполняет следующие модификации исходной оболочки: \n 
    удаление из тела выбранных граней с окружением, \n 
    создание тела из выбранных граней с окружением, \n 
    перемещение выбранных граней с окружением относительно оставшихся граней тела, \n 
    замена выбранных граней тела эквидистантными гранями (перемещение по нормали, изменение радиуса), \n 
    замена выбранных граней тела деформируемыми  гранями (превращение в NURBS для редактирования).\n
    замена выбранных граней тела.\n
           \en Constructor of a shell performing modification of the source shell.  
    Constructor performs the following modifications of the source shell: \n 
    deletion the selected faces with neighborhood from the solid, \n 
    creation of the solid from the selected faces with the neighborhood, \n 
    translation of the selected faces with the neighborhood relative to the remained faces of the solid, \n 
    replacement of the selected faces of the solid with the offset faces (translation along the normal, changing the radius), \n 
    replacement of the specified faces of the solid with the deformable faces (conversion to the NURBS for editing).\n
    replacement of the specified faces of the solid.\n \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbFaceModifiedSolid : public MbCreator {
private:
  MbVector3D                 _direction;    ///< \ru Для обратной совместимости с dmt_Merger. \en For backward compatability with dmt_Merger.
protected: // \ru Данные класса. \en Data of class. 
  MbModifiedSolidParams      _parameters;   ///< \ru Параметры редактирования оболочки. \en Shell editing parameters. 

private: 
  // \ru Конструктор дублирующий. \en Duplicating constructor. 
  MbFaceModifiedSolid( const MbFaceModifiedSolid & init, MbRegDuplicate * ireg );

public: 
  // \ru Конструктор по параметрам. \en Constructor by parameters. 
  //DEPRECATE_DECLARE_REPLACE( MbFaceModifiedSolid with MbModifiedSolidParams )
  MbFaceModifiedSolid( const ModifyValues & p, const SArray<MbItemIndex> & faces, 
                       RPArray<MbSurface> & surfs, const MbSNameMaker & names );
  // \ru Конструктор по параметрам. \en Constructor by parameters. 
  //DEPRECATE_DECLARE_REPLACE( MbFaceModifiedSolid with MbModifiedSolidParams )
  MbFaceModifiedSolid( const ModifyValues & p, const SArray<MbEdgeFacesIndexes> edges, 
                       const MbSNameMaker & names );

  // \ru Конструктор по параметрам. \en Constructor by parameters. 
  MbFaceModifiedSolid( const MbModifiedSolidParams & params );

  // \ru Деструктор \en Destructor 
  ~MbFaceModifiedSolid();

public: 
  // \ru Общие функции математического объекта \en Common functions of the mathematical object 

  MbeCreatorType  IsA() const override; // \ru Тип элемента \en A type of element 
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию \en Create a copy
  bool        IsSame( const MbCreator & other, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal? 
  bool        SetEqual ( const MbCreator & ) override; // \ru Сделать равным \en Make equal 
  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать по матрице \en Transform according to the matrix
  void        Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override; // \ru Сдвиг по вектору \en Translation by the vector
  void        Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate around an axis

  MbePrompt   GetPropertyName() override; // \ru Выдать заголовок свойства объекта \en Get a name of object property 
  void        GetProperties( MbProperties & properties ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & properties ) override; // \ru Записать свойства объекта \en Set properties of the object 
  void        GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  void        SetYourVersion( VERSION version, bool forAll ) override; // \ru Установить версию объектов. \en Set the objects version.

  /// \ru Построение оболочки \en Creation of a shell 
  bool        CreateShell( MbFaceShell *& shell,
                           MbeCopyMode    sameShell, 
                           RPArray<MbSpaceItem> * = nullptr ) override;
  void        Refresh( MbFaceShell & outer ) override; ///< \ru Обновить форму оболочки \en Update shape of the shell

  // \ru Дать параметры. \en Get the parameters. 
  //DEPRECATE_DECLARE
  void GetParameters (       ModifyValues & params ) const;
  // \ru Установить параметры. \en Set the parameters. 
  //DEPRECATE_DECLARE
  void SetParameters ( const ModifyValues & params );


DECLARE_PERSISTENT_CLASS_NEW_DEL( MbFaceModifiedSolid )
OBVIOUS_PRIVATE_COPY( MbFaceModifiedSolid )
}; // MbFaceModifiedSolid

IMPL_PERSISTENT_OPS( MbFaceModifiedSolid )


//------------------------------------------------------------------------------
/** \brief \ru Построить модифицированную оболочку.
           \en Construct the modified shell. \~
  \details \ru Построить оболочку тела путём модификации исходной оболочки. 
    В зависимости от параметров модификации #ModifyValues метод выполняет одно из следующих действий: \n
    1.  Удаление из тела выбранных граней с окружением (param.way==dmt_Remove). \n
    2.  Создание тела из выбранных граней с окружением (param.way==dmt_Create). \n
    3.  Перемещение выбранных граней с окружением относительно оставшихся граней тела (param.way==dmt_Action). \n
    4.  Замена выбранных граней тела эквидистантными гранями (перемещение по нормали, изменение радиуса) (param.way==dmt_Offset). \n
    5.  Изменение радиуса выбранных граней скругления (param.way==dmt_Fillet). \n
    6.  Замена выбранных граней тела деформируемыми гранями (превращение в NURBS) для редактирования (param.way==dmt_Supple). \n
    7.  Удаление выбранных граней скругления тела (param.way==dmt_Purify). \n
    8.  Слияние вершин ребёр и удаление рёбер (param.way==dmt_Merger). \n
    9.  Замена гладко стыкующихся граней одной гранью (param.way==dmt_United). \n
    10. Поворот выбранных граней (param.way==dmt_Rotate). \n
    Одновременно с построением оболочки функция создаёт её строитель.\n
           \en Construct the solid's shell by modification the source shell. 
    The method performs one of the following actions depending on the parameter #ModifyValues: \n
    1.  Removal of the specified faces with the neighborhood from a solid (param.way==dmt_Remove). \n
    2.  Creation of a solid from the specified faces with the neighborhood (param.way==dmt_Create). \n
    3.  Translation of the specified faces with neighborhood relative to the other faces of the solid (param.way==dmt_Action). \n
    4.  Replacement of the specified faces by offset faces (param.way==dmt_Offset). \n
    5.  Radius change of the specified fillet faces (param.way==dmt_Fillet). \n
    6.  Replacement of the specified faces of a solid with a deformable NURBS faces for editing (param.way==dmt_Supple). \n 
    7.  Removal of the specified fillet faces from a solid (param.way==dmt_Purify). \n
    8.  Edge vertices merging and edges removal (param.way==dmt_Merger). \n
    9.  Replacement of the smoothly joined faces with a single face (param.way==dmt_United). \n
    10. Rotation of the chosen faces (param.way==dmt_Rotate). \n
    The function simultaneously creates the shell and its constructor.\n \~
  \deprecated \ru Метод устарел и будет удален в версии 2024.
              \en The method is deprecated and will be removed in version 2024. \~
  \param[in]  outer      - \ru Исходная оболочка.
                           \en The source shell. \~
  \param[in]  sameShell  - \ru Режим копирования исходной оболочки.
                           \en Mode of copying the source shell. \~
  \param[in]  parameters - \ru Параметры модификации.
                           \en Parameters of the modification. \~
  \param[in]  faces      - \ru Изменяемые грани тела.
                           \en Faces to be modified. \~
  \param[in]  edges      - \ru Изменяемые рёроа тела.
                           \en Edges to be modified. \~
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
//DEPRECATE_DECLARE
MATH_FUNC (MbCreator *) CreateFaceModifiedSolid(       MbFaceShell *          outer, 
                                                       MbeCopyMode            sameShell, 
                                                 const ModifyValues &         parameters, 
                                                 const RPArray<MbFace> &      faces, 
                                                 const RPArray<MbCurveEdge> & edges, 
                                                 const MbSNameMaker &         names, 
                                                       MbResultType &         res,
                                                       MbFaceShell *&         shell );
#endif // __CR_MODIFIED_SOLID_H
