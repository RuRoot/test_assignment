////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Идентификаторы частей тела.
         \en Identifiers of the parts of the solid. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __PART_SOLID_H
#define __PART_SOLID_H


#include <topology_item.h>
#include <mb_cart_point3d.h>
#include <mb_operation_result.h>


class    MbSolid;


//------------------------------------------------------------------------------
/** \brief \ru Идентификатор части тела.
           \en Identifier of the part of the solid. \~
  \details \ru Идентификатор части тела с точкой привязки и другой информацией. \n
           \en Identifier of solid part with the anchor point and other information. \n \~
  \ingroup Data_Structures
*/
// ---
class MATH_CLASS MbPartSolidIndex : public MbRefItem {
  friend class MbPartSolidIndices; // \ru Владелец индексов. \en Owner of indices. 

protected: // \ru Данные класса \en Data of class 
  uint          id;       ///< \ru Идентификатор тела. \en Solid identifier. 
  MbPath        path;     ///< \ru Путь. \en Path. 
  ptrdiff_t     ind;      ///< \ru Индекс части тела. \en Index of the part of the solid. 
  MbCartPoint3D tiePnt;   ///< \ru Точка привязки части тела. \en Anchor point of solid part. 
  double        diag;     ///< \ru Размер диагонали части тела. \en Diagonal size of the solid part. 
  MbCartPoint3D refPnt;   ///< \ru Базовая точка тела из частей. \en Base point of the solid from parts. 
  ptrdiff_t     allCnt;   ///< \ru Общее количество частей тела. \en Total count of solid parts. 
private:
  mutable bool  selected; ///< \ru Был ли выбран индекс. \en Whether index s selected. 
  mutable bool  changed;  ///< \ru Был ли изменен индекс части тела. \en Whether index of solid part is changed. 

public: // \ru Конструкторы \en Constructors 
  /// \ru Конструктор по умолчанию. \en Default constructor. 
  MbPartSolidIndex()
    : MbRefItem() 
  {
    Reset();
    SetChanged( false );
  }
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по данным.
             \en Constructor by data. \~
    \param[in] _id       - \ru Идентификатор тела.
                           \en Solid identifier. \~
    \param[in] _path     - \ru Путь.
                           \en Path. \~
    \param[in] _ind      - \ru Индекс части тела.
                           \en Index of the part of the solid. \~
    \param[in] _tiePnt   - \ru Точка привязки части тела.
                           \en Anchor point of solid part. \~
    \param[in] _diag     - \ru Размер диагонали части тела.
                           \en Diagonal size of the solid part. \~
    \param[in] _refPnt   - \ru Базовая точка тела из частей.
                           \en Base point of the solid from parts. \~
    \param[in] _allCount - \ru Общее количество частей тела.
                           \en Total count of solid parts. \~
  */
  MbPartSolidIndex(       uint            _id, 
                    const MbPath &        _path, 
                          ptrdiff_t       _ind, 
                    const MbCartPoint3D & _tiePnt, 
                          double          _diag,
                    const MbCartPoint3D & _refPnt, 
                          ptrdiff_t       _allCount ) 
    : MbRefItem()
  { 
    Init( _id, _path, _ind, _tiePnt, _diag, _refPnt, _allCount ); 
    SetChanged( false );
  }
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbPartSolidIndex();
        
public: // \ru Внешние функции. \en Eternal functions. 

  /// \ru Корректен ли индекс. \en Whether index is correct. 
  bool        IsValid()     const;
  /// \ru Использован ли индекс. \en Whether index is selected. 
  bool        IsSelected()  const { return selected; } 
  /// \ru Изменен ли индекс. \en Whether index is changed. 
  bool        IsChanged()   const { return changed;  } 

  /// \ru Получить идентификатор тела. \en Get solid identifier. 
  uint        GetId()       const { return id;       } 
  /// \ru Получить путь. \en Get path. 
  const MbPath & GetPath()  const { return path;     } 

  /// \ru Получить индекс части тела. \en Get index of the part of the solid. 
  ptrdiff_t   GetIndex()    const { return ind;      } 
  /// \ru Получить точку привязки части тела. \en Get anchor point of solid part. 
  const MbCartPoint3D & GetTiePoint() const { return tiePnt; }
  /// \ru Получить размер диагонали части тела. \en Get diagonal size of the solid part. 
  double      GetDiag()     const { return diag;     } 
  /// \ru Получить точку привязки тела из частей. \en Get anchor point of solid from parts. 
  const MbCartPoint3D & GetRefPoint() const { return refPnt; } 
  /// \ru Получить общее количество частей тела. \en Get total count of solid parts. 
  ptrdiff_t   GetAllCount() const { return allCnt;   } 

  /// \ru Сравнение индексов (по содержанию). \en Comparison of indices (by content). 
  bool        operator == ( const MbPartSolidIndex & obj ) const; 

protected: // \ru Внутренние функции. \en Internal functions. 
  /// \ru Сброс данных в неопределённое состояние. \en Reset data to undefined state. 
  void        Reset(); 

  /** \brief \ru Инициализация.
             \en Initialization. \~
    \details \ru Инициализация по данным.
             \en Initialize by data. \~
    \param[in] _id       - \ru Идентификатор тела.
                           \en Solid identifier. \~
    \param[in] _path     - \ru Путь.
                           \en Path. \~
    \param[in] _ind      - \ru Индекс части тела.
                           \en Index of the part of the solid. \~
    \param[in] _tiePnt   - \ru Точка привязки части тела.
                           \en Anchor point of solid part. \~
    \param[in] _diag     - \ru Размер диагонали части тела.
                           \en Diagonal size of the solid part. \~
    \param[in] _refPnt   - \ru Базовая точка тела из частей.
                           \en Base point of the solid from parts. \~
    \param[in] _allCount - \ru Общее количество частей тела.
                           \en Total count of solid parts. \~
  */
  bool        Init(       uint            _id, 
                    const MbPath &        _path, 
                          ptrdiff_t       _ind, 
                    const MbCartPoint3D & _tiePnt, 
                          double          _diag,  
                    const MbCartPoint3D & _refPnt, 
                          ptrdiff_t       _allCount );

  /// \ru Инициализация по другом индексу. \en The initialization by another index. 
  bool        Init( const MbPartSolidIndex & psInd ); 
  /// \ru Установить использованность. \en Set selected. 
  void        SetSelected( bool b ) const { selected = b; } 
  /// \ru Установить измененность. \en Set modification. 
  void        SetChanged ( bool b ) const { changed  = b; } 

  KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( MbPartSolidIndex, MATH_FUNC_EX )
  DECLARE_NEW_DELETE_CLASS( MbPartSolidIndex )
  OBVIOUS_PRIVATE_COPY    ( MbPartSolidIndex )
}; // MbPartSolidIndex


//------------------------------------------------------------------------------
// \ru Проверка корректности. \en Check for correctness. 
// ---
inline bool MbPartSolidIndex::IsValid() const
{
  return (ind > -1 && id != SYS_MAX_UINT32 && diag > METRIC_PRECISION && allCnt > 1);
}


//------------------------------------------------------------------------------
// \ru Оператор сравнения (по содержанию). \en Comparison operator (by content). 
// ---
inline bool MbPartSolidIndex::operator == ( const MbPartSolidIndex & obj ) const
{
  return ( id     == obj.id     && 
           ind    == obj.ind    && 
           path   == obj.path   && 
           tiePnt == obj.tiePnt && 
           diag   == obj.diag   &&
           refPnt == obj.refPnt &&
           allCnt == obj.allCnt );
}


//------------------------------------------------------------------------------
// \ru Инициализация по данным. \en Initialize by data. 
// ---
inline 
bool MbPartSolidIndex::Init( uint _id, 
                             const MbPath & _path, 
                             ptrdiff_t _ind, 
                             const MbCartPoint3D & _tiePnt, 
                             double _diag,
                             const MbCartPoint3D & _refPnt, 
                             ptrdiff_t _allCnt )
{
  if ( _ind > -1 && _id != SYS_MAX_UINT32 && ::fabs(_diag) > METRIC_PRECISION && _allCnt > 1 ) {
    id      = _id;
    path    = _path;

    ind     = _ind; 
    tiePnt  = _tiePnt; 
    diag    = ::fabs(_diag); 

    refPnt  = _refPnt;
    allCnt  = _allCnt;
    
    selected = false;
    changed  = true;
    return true;
  }

  C3D_ASSERT_UNCONDITIONAL( false );
  return false;
}


//------------------------------------------------------------------------------
// \ru Инициализация по константной ссылке на другой объект. \en Initialization by constant reference to another object.  
// ---
inline bool MbPartSolidIndex::Init( const MbPartSolidIndex & psInd )
{
  if ( psInd.IsValid() ) {
    id   = psInd.id;
    path = psInd.path;

    ind  = psInd.ind;
    tiePnt = psInd.tiePnt;
    diag = psInd.diag;

    refPnt = psInd.refPnt;
    allCnt = psInd.allCnt;
    
    selected = psInd.selected;
    changed = true;
    return true;
  }

  C3D_ASSERT_UNCONDITIONAL( false );
  return false;
}


//------------------------------------------------------------------------------
// \ru Сброс данных в неопределённое состояние. \en Reset data to undefined state. 
// ---
inline void MbPartSolidIndex::Reset() 
{ 
  id = SYS_MAX_UINT32;
  path.Flush();

  ind = -1;
  tiePnt.SetZero(); 
  tiePnt.x = UNDEFINED_DBL;
  diag = 0.0;
  refPnt.SetZero();
  refPnt.x = UNDEFINED_DBL;
  allCnt = 0;
  
  selected = false;
  changed  = true;
}


//------------------------------------------------------------------------------
/** \brief \ru Идентификаторы частей тела.
           \en Identifiers of the parts of the solid. \~
  \details \ru Множество идентификаторов частей тела. \n
           \en Identifier set of the parts of the solid. \n \~
  \ingroup Data_Structures
*/
// ---
class MATH_CLASS MbPartSolidIndices : private PArray<MbPartSolidIndex> {
public:
  /// \ru Состояние выбора. \en Selection state. 
  enum SelectionState {
    ss_Undefined = 0,       ///< \ru Состояние не определено. \en State is not defined. 
    ss_NoSelection,         ///< \ru Не выбрано ни одного идентификатора. \en Identifiers are not selected. 
    ss_AllSelection,        ///< \ru Выбраны все идентификаторы. \en All the identifiers are selected. 
    ss_AllMaxSizeSelection, ///< \ru Выбран идентификатор с самой большой частью. \en The identifier with the biggest part is selected. 
    // \ru Добавлять в конец! \en Add to the end! 
  };

protected: // \ru Данные класса. \en Data of class. 
  mutable SelectionState  selState;  ///< \ru Состояние выбора идентификаторов. \en State of identifiers selection. 
  mutable bool            anyMulti;  ///< \ru Рабочий флаг (наличие в наборе тел тела из частей). \en Working flag (solids from parts are in the set). 
  mutable bool            editState; ///< \ru Рабочий флаг (режим редактирования тела). \en Working flag (mode of solid editing). 

public: // \ru Конструкторы, деструктор \en Constructors, destructor 
  /// \ru Пустой конструктор. \en Empty constructor. 
  MbPartSolidIndices();
  /// \ru Конструктор копирования копирует данные. \en Copy-constructor copies data. 
  MbPartSolidIndices( const MbPartSolidIndices & ); 
  virtual ~MbPartSolidIndices();

public: // \ru Открытые константные функции базового класса. \en Open constant functions of base class. 
using  PArray<MbPartSolidIndex>::Count;    // \ru Количество идентификаторов. \en The count of identifiers.
using  PArray<MbPartSolidIndex>::MaxIndex; // \ru Номер последнего идентификатора. \en Index of the last identifier.
using  PArray<MbPartSolidIndex>::Reserve;  // \ru Зарезервировать память. \en Reserve memory.
using  PArray<MbPartSolidIndex>::Adjust;   // \ru Очистить лишнюю память. \en Clear the unnecessary memory.
using  PArray<MbPartSolidIndex>::IsExist;  // \ru Существует ли объект? \en Is there the object?

public: // \ru Внешние функции. \en External functions. 
  /// \ru Набор пуст? \en Whether set is empty? 
  bool        IsEmpty() const { return Count() < 1; } 

  /// \ru Корректен ли набор. \en Whether set is correct. 
  bool        IsAllValid() const;
  /// \ru Корректен ли k-й индекс части. \en Whether k-th index of part is correct.  
  bool        IsValidIndex( size_t k ) const; 
  /// \ru Существует ли k-й индекс части. \en Whether k-th index of part exists.  
  bool        IsExistIndex( size_t k ) const; 

  /// \ru Выбраны ли все индексы. \en Whether all the indices are selected. 
  bool        IsAllSelected()          const; 
  /// \ru Установить состояние выбора всех индексов. \en Set the selection state for all the indices. 
  bool        SetAllSelected( bool s ) const; 

  /// \ru Выбран ли индекс. \en Whether index is selected. 
  bool        IsSelected( size_t k ) const;   
  /// \ru Установить состояние выбора индекса. \en Set the selection state of index. 
  bool        SetSelected( size_t k, bool s ) const; 
  
  /// \ru Получить состояние выбора индексов. \en Get the selection state of indices. 
  SelectionState GetSelectionState()  const { return selState; }; 
  /// \ru Установить состояние выбора индексов. \en Set the selection state of indices. 
  bool        SetSelectionState( SelectionState selState ) const; 
  /// \ru Выставить общее состояние выбора по данным. \en Set total selection state from data. 
  bool        CheckSelectionState() const; 

  /// \ru Получить собственный индекс части тела. \en Get own index of the part of the solid. 
  ptrdiff_t   GetOwnIndex( size_t k ) const; 
  /// \ru Получить макс. собственный индекс для частей тела в наборе. \en Get max own index of solid parts in the set. 
  ptrdiff_t   GetMaxOwnIndex() const;       

  /// \ru Получить идентификатор и путь для k-го индекса части. \en Get identifier and path for k-th index of part. 
  bool        GetIdPath( size_t k, uint & id, MbPath & path ) const; 

  /// \ru Получить копию индекса. \en Get a copy of the index. 
  bool        GetPartIndex( size_t k, MbPartSolidIndex & partIndex ) const;
  /// \ru Положить копию индекса. \en Add a copy of the index. 
  bool        AddPartIndex( const MbPartSolidIndex & partIndex ); 
  /// \ru Отцепить индекс. \en Detach index. 
  MbPartSolidIndex * DetachPartIndex( size_t k );
  /// \ru Поглотить индекс. \en Absorb index. 
  bool        AbsorbPartIndex( MbPartSolidIndex *& );

  /// \ru Забрать индексы по идентификатору и пути. \en Detach indices by identifier and path. 
  bool        DetachPartIndices( uint id, const MbPath & path, MbPartSolidIndices & ); 
  /// \ru Удалить индексы по идентификатору и пути. \en Delete indices by identifier and path. 
  bool        DeletePartIndices( uint id, const MbPath & path ); 

  /// \ru Положить копии индексов. \en Add copies of indices. 
  bool        AddPartIndices( const MbPartSolidIndices & ); 
  ///< \ru Поглотить индексы. \en Absorb indices. 
  bool        AbsorbPartIndices( MbPartSolidIndices & );

  /// \ru Найти по адресу или содержанию. \en Find by address or content. 
  size_t      Find( const MbPartSolidIndex & ) const; 
  /// \ru Найти по идентификаторам. \en Find by identifiers. 
  size_t      Find( uint id, const MbPath & path, ptrdiff_t index ) const; 

  /// \ru Удалить плохие индексы. \en Remove bad indices. 
  void        RemoveBadIndices(); 
  /// \ru Удалить все индексы. \en Remove all the vertices. 
  void        RemoveAllIndices(); 

  /// \ru Удалить по существующему пути те индексы частей, которых нет в списке идентификаторов. \en Delete by existing path those indices of parts which are not contained in the list of identifiers. 
  void        RemoveLostIndices( const MbPath & existPath, SArray<uint> & existIds );

  /// \ru Получить флаг наличия в наборе тел из частей. \en Get flag (when solids from parts are in the set). 
  bool        IsAnyMultiSolid() const { return anyMulti; }
  /// \ru Установить флаг наличия в наборе тел из частей. \en Set flag (when solids from parts are in the set). 
  void        SetAnyMultiSolid( bool anyMulit, bool setAny ) const; 

  /// \ru Находимся в режиме редактирования? \en Is in the editing mode? 
  bool        IsEditState() const { return editState; } 
  /// \ru Установить флаг, находимся ли в режиме редактирования. \en Set flag "Editing mode". 
  void        SetEditState( bool b ) { editState = b; } 

private: // \ru Внутренние функции. \en Internal functions. 
  /// \ru Удалить не выбранные индексы (плохие тоже убирает). \en Remove unselected indices (also removes bad). 
  void        RemoveUnselectedIndices(); 
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
  void        operator = ( const MbPartSolidIndices & ); 

  KNOWN_OBJECTS_RW_REF_OPERATORS_EX_BASE( MbPartSolidIndices, MATH_FUNC_EX )
  DECLARE_NEW_DELETE_CLASS( MbPartSolidIndices )
  DECLARE_NEW_DELETE_CLASS_EX( MbPartSolidIndices )
};


//------------------------------------------------------------------------------
// \ru Установить флаг наличия в наборе тел из частей (флаг устанавливается извне, объект им не управляет). \en Set flag of presence of solids from parts in the set (the flag is set from the outside, the object does not control it). 
// ---
inline void MbPartSolidIndices::SetAnyMultiSolid( bool isMulti, bool setAny ) const
{
  if ( setAny )
    anyMulti = isMulti;
  else if ( !anyMulti )
    anyMulti = isMulti;
}


//------------------------------------------------------------------------------
/** \brief \ru Информация о части тела.
           \en Information about the solid part. \~
  \details \ru Информация о части тела и его состоянии. \n
           \en Information about the solid part and it state. \n \~
  \ingroup Data_Structures
*/
// ---
struct MATH_CLASS MbPartSolidData {
public:
  MbSolid       * part;     ///< \ru Часть тела. \en The part of the solid. 
  uint            id;       ///< \ru Идентификатор тела. \en Solid identifier. 
  ptrdiff_t       ind;      ///< \ru Номер части тела. \en Index of the solid part. 
  const MbPath &  path;     ///< \ru Путь. \en Path. 
  bool            selected; ///< \ru Состояние выбора части. \en State of part selection. 

public:
  /// \ru Конструктор по данным. \en Constructor by data. 
  MbPartSolidData( MbSolid *      _part, 
                   uint           _id, 
                   ptrdiff_t      _ind, 
                   const MbPath & _path, 
                   bool           _selected )
    : part    ( _part     )
    , id      ( _id       )
    , ind     ( _ind      )
    , path    ( _path     )
    , selected( _selected )
  {}
public:
  /// \ru Проверить данные на корректность. \en Check data for correctness. 
  bool        IsValid() const { return (part != nullptr && ind > -1 && id != SYS_MAX_UINT32); }
private:
  /// \ru Конструктор без параметров. \en Constructor without parameters. 
  MbPartSolidData();
  // \ru Объявление конструктора копирования и оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration without implementation of the copy-constructor and assignment operator to prevent an assignment by default. 
  OBVIOUS_PRIVATE_COPY( MbPartSolidData );
};


//------------------------------------------------------------------------------
/** \brief \ru Создать индексы частей тела.
           \en Create indices of the parts of the solid. \~
  \details \ru При создании индексов частей тела предполагается, что тело состоит из отдельных частей.
    Информационный массив должен соответствовать телу.
           \en When creating indices of solid parts it is assumed that the solid consists of parts.
    Information array must correspond to solid. \~
  \param[in] solid - \ru Состоящее из отдельных частей исходное тело.
                     \en The initial solid is consisting of separate parts. \~
  \param[in] partInfo - \ru Информация о частях тела.
                        \en Information about the solid parts. \~
  \param[out] partIndices - \ru Множество индексов частей тела.
                            \en Index set of the parts of the solid. \~
  \return \ru Созданы ли индексы частей тела.
          \en Whether indices of solid parts are created. \~
  \ingroup Algorithms_3D
*/
MATH_FUNC (bool) CreatePartSolidIndices( const MbSolid &                 solid, 
                                         const SArray<MbPartSolidData> & partInfo,
                                               MbPartSolidIndices &      partIndices );


#endif // __PART_SOLID_H
