////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Строитель размноженного набора граней.
         \en Constructor of duplication face sets . \~
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef CR_DUPLICATION_SOLID_H
#define CR_DUPLICATION_SOLID_H


#include <creator.h>
#include <op_duplication_parameter.h>


class    MbFaceShell;
class              MbRegTransform;
class              MbRegDuplicate;


//------------------------------------------------------------------------------
/** \brief \ru Строитель размноженного набора граней.
           \en Constructor of duplication face sets . \~
  \details \ru Строитель выполняет размножение тела согласно параметрам и объединяет копии в одно тело\n
           \en Creator makes duplication of face sets according to parameters and unite its into a single face set\~ 
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbDuplicationSolid : public MbCreator {
protected:
  DuplicationValues * _parameters;  ///< \ru Параметры размножения. \en Parameters of duplication.
  c3d::ItemIndices    _faceIndices; ///< \ru Индексы выбранных граней оболочки. \en Indices of selected shell faces. 
  bool                _adaptMode;   ///< \ru Режим адаптации открытых оболочек. \en Open shell instances adaptation mode. 

public:
  /// \ru Конструктор по параметрам. \en Constructor by parameters. 
  MbDuplicationSolid( const DuplicationValues &, const MbSNameMaker & );
  /// \ru Конструктор по параметрам. \en Constructor by parameters. 
  MbDuplicationSolid( const DuplicationValues &, const c3d::ItemIndices & faceIndices, bool adaptMode, const MbSNameMaker & );
private:
  MbDuplicationSolid( const MbDuplicationSolid &, MbRegDuplicate * );
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbDuplicationSolid( const MbDuplicationSolid & );
public:
  virtual~MbDuplicationSolid();

  /** \ru \name Общие функции строителя оболочки.
      \en \name Common functions of the shell creator.
      \{ */
  /// \ru Получить регистрационный тип (для копирования, дублирования). \en Get the registration type (for copying, duplication). 
  MbeCreatorType IsA() const override;
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru сделать копию \en create a copy
  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override;          // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move( const MbVector3D &, MbRegTransform * = nullptr ) override;                 // \ru Сдвиг \en Translation
  void        Rotate( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate about an axis

  void        GetProperties( MbProperties & ) override; // \ru выдать свойства объекта \en get properties of the object 
  void        SetProperties( const MbProperties & ) override; // \ru записать свойства объекта \en set properties of the object 
  MbePrompt   GetPropertyName() override; // \ru выдать заголовок свойства объекта \en get a name of object property

  bool        IsSame( const MbCreator &, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        IsSimilar( const MbCreator & ) const override; // \ru являются ли объекты подобными \en whether the objects are similar
  bool        SetEqual ( const MbCreator & ) override;       // \ru сделать равным \en make equal

  bool        CreateShell( MbFaceShell *&, MbeCopyMode sameShell,
                           RPArray<MbSpaceItem> * items = nullptr ) override;

private :
// \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
  void         operator = ( const MbDuplicationSolid & );

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbDuplicationSolid )
}; // MbDuplicationSolid

IMPL_PERSISTENT_OPS( MbDuplicationSolid )


//------------------------------------------------------------------------------
/** \brief \ru Создать оболочку размножения исходной оболочки.
           \en Create a shell of duplication of original shell. \~ 
  \details \ru По данной оболочке и параметрам размножения построить оболочку как результат объединения копий.\n
    Одновременно с построением оболочки функция создаёт её строитель. \n
           \en For a given shell and duplication parameters construct a shell as a result of a union of copies. \n
    The function simultaneously constructs the shell and creates its constructor.\~
  \param[in]  solid          - \ru Исходная оболочка.
                               \en Original face set. \~
  \param[in]  params         - \ru Параметры размножения.
                               \en Parameters of duplication. \~
  \param[out] resCode        - \ru Код результата операции.
                               \en Operation result code. \~
  \param[out] resShell       - \ru Построенный набор граней.
                               \en Constructed set of faces. \~  
  \result \ru Возвращает строитель, если операция была выполнена успешно.
          \en Returns the constructor if the operation has been successfully performed. \~
  \ingroup Solid_Modeling
*/
MATH_FUNC (MbCreator *) CreateDuplication( const MbFaceShell              & solid, 
                                           const MbDuplicationSolidParams & params, 
                                                 MbResultType             & resCode,
                                                 c3d::ShellSPtr           & resShell );


#endif // CR_DUPLICATION_SOLID_H
