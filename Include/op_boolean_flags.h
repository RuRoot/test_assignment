////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Флаги булевой операции и ее наследников.
         \en Flags of a boolean operation and its heirs. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __OP_BOOLEAN_FLAGS_H
#define __OP_BOOLEAN_FLAGS_H

#include <math_define.h>


//------------------------------------------------------------------------------
/** \brief \ru Управляющие флаги слияния элементов оболочки.
           \en Control flags of shell items merging. \~
  \details \ru Управляющие флаги слияния элементов оболочки. \n
           \en Control flags of shell items merging. \n \~
  \ingroup Shell_Building_Parameters
*/
// ---
struct MATH_CLASS MbMergingFlags {
protected:
  bool mergeFaces; ///< \ru Сливать подобные грани (true). \en Whether to merge similar faces (true). 
  bool mergeEdges; ///< \ru Сливать подобные ребра (true). \en Whether to merge similar edges (true). 
public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  MbMergingFlags() : mergeFaces( true ), mergeEdges( true ) {}
  /// \ru Конструктор по флагам слияния. \en Constructor by merging flags.
  MbMergingFlags( bool mFs, bool mEs ) : mergeFaces( mFs ), mergeEdges( mEs ) {}
  /// \ru Конструктор копирования. \en Copy-constructor.
  MbMergingFlags( const MbMergingFlags & f ) : mergeFaces( f.mergeFaces ), mergeEdges( f.mergeEdges ) {}
public:
  /// \ru Сливать подобные грани (true). \en Whether to merge similar faces (true). 
  bool MergeFaces() const { return mergeFaces; }
  /// \ru Сливать подобные ребра (true). \en Whether to merge similar edges (true). 
  bool MergeEdges() const { return mergeEdges; }
  /// \ru Сливать подобные грани (true). \en Whether to merge similar faces (true). 
  void SetMergingFaces( bool mf ) { mergeFaces = mf; }
  /// \ru Сливать подобные ребра (true). \en Whether to merge similar edges (true). 
  void SetMergingEdges( bool me ) { mergeEdges = me; }
  /// \ru Сливать подобные элементы. \en Whether to merge similar elements. 
  void SetMerging( const MbMergingFlags & f ) { mergeFaces = f.mergeFaces; mergeEdges = f.mergeEdges; }
public:
  MbMergingFlags & operator =  ( const MbMergingFlags & f ) { mergeFaces = f.mergeFaces; mergeEdges = f.mergeEdges; return *this; }
  bool             operator == ( const MbMergingFlags & f ) const { return  (mergeFaces == f.mergeFaces && mergeEdges == f.mergeEdges); }
};


//------------------------------------------------------------------------------
/** \brief \ru Управляющие флаги булевой операции.
           \en Control flags of Boolean operations. \~
  \details \ru Управляющие флаги булевой операции. \n
           \en Control flags of Boolean operations \n \~
  \ingroup Shell_Building_Parameters
*/
// ---
struct MATH_CLASS MbBooleanFlags {
protected:
  bool          mergeFaces;           ///< \ru Сливать подобные грани (true). \en Whether to merge similar faces (true). 
  bool          mergeEdges;           ///< \ru Сливать подобные ребра (true). \en Whether to merge similar edges (true). 
  bool          closed;               ///< \ru Замкнутость оболочек операндов. \en Closedness of operands' shells. 
  bool          enclosureCheck;       ///< \ru Проверять оболочки на вложенность. \en Check shell on nesting.
  bool          allowNonIntersecting; ///< \ru Выдавать конечную оболочку, если нет пересечений. \en Allow a final result if there is no intersection.
  bool          cutting;              ///< \ru Флаг резки оболочки при построении разрезов и сечений. \en Flag of cutting the shell in the construction of cuts and sections.
  bool          repairShellEdges;     ///< \ru Флаг починки ребер исходных оболочек. \en Flag of input shells edges repair.
public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  MbBooleanFlags()
    : mergeFaces          ( true  )
    , mergeEdges          ( true  )
    , closed              ( true  )
    , enclosureCheck      ( true  )
    , allowNonIntersecting( false )
    , cutting             ( false )
    , repairShellEdges    ( false )
  {}
public:
  /// \ru Конструктор копирования. \en Copy-constructor.
  MbBooleanFlags( const MbBooleanFlags & flags )
    : mergeFaces          ( flags.mergeFaces           )
    , mergeEdges          ( flags.mergeEdges           )
    , closed              ( flags.closed               )
    , enclosureCheck      ( flags.enclosureCheck       )
    , allowNonIntersecting( flags.allowNonIntersecting )
    , cutting             ( flags.cutting              )
    , repairShellEdges    ( flags.repairShellEdges     )
  {}
  /// \ru Конструктор копирования. \en Copy-constructor.
  MbBooleanFlags( const MbBooleanFlags & flags, bool _closed )
    : mergeFaces          ( flags.mergeFaces           )
    , mergeEdges          ( flags.mergeEdges           )
    , closed              ( _closed                    )
    , enclosureCheck      ( flags.enclosureCheck       )
    , allowNonIntersecting( flags.allowNonIntersecting )
    , cutting             ( flags.cutting              )
    , repairShellEdges    ( flags.repairShellEdges     )
  {}
protected:
  /// \ru Конструктор по флагам булевой операции. \en Constructor by Boolean flags.
  MbBooleanFlags( bool _mergeFaces, bool _mergeEdges, bool _closed, bool _enclosureCheck, bool _allowNonIntersecting, bool _cutting )
    : mergeFaces          ( _mergeFaces           )
    , mergeEdges          ( _mergeEdges           )
    , closed              ( _closed               )
    , enclosureCheck      ( _enclosureCheck       )
    , allowNonIntersecting( _allowNonIntersecting )
    , cutting             ( _cutting              )
    , repairShellEdges    ( false                 )
  {}

public:
  /// \ru Булева операция над оболочками. \en Boolean operation of shells.
  void InitBoolean( bool _closed, bool _allowNonIntersecting = false )
  {
    mergeFaces = true;
    mergeEdges = true;
    closed = _closed;
    enclosureCheck = _closed;
    allowNonIntersecting = _allowNonIntersecting;
    cutting = false;
  }
  /// \ru Сечение (или усечение) оболочки. \en The cutting (or truncation) of a shell.
  void InitCutting( bool _closed, bool _allowNonIntersecting = false )
  {
    mergeFaces = true;
    mergeEdges = true;
    closed = _closed;
    enclosureCheck = _closed;
    allowNonIntersecting = _allowNonIntersecting;
    cutting = true;
  }

  bool MergeFaces          () const { return mergeFaces;           } ///< \ru Сливать подобные грани (true)? \en Whether to merge similar faces (true)? 
  bool MergeEdges          () const { return mergeEdges;           } ///< \ru Сливать подобные ребра (true)? \en Whether to merge similar edges (true)? 
  bool DoClosed            () const { return closed;               } ///< \ru Замкнутость результата. \en Closedness of resulting shell. 
  bool CheckEnclosure      () const { return enclosureCheck;       } ///< \ru Проверять оболочки на вложенность. \en Check shell on nesting.
  bool AllowNonIntersecting() const { return allowNonIntersecting; } ///< \ru Выдавать конечную оболочку, если нет пересечений. \en Allow a final result if there is no intersection.
  bool IsCutting           () const { return cutting;              } ///< \ru Флаг резки оболочки при построении разрезов и сечений. \en Flag of cutting the shell in the construction of cuts and sections.
  bool ShellEdgesRepair    () const { return repairShellEdges;     } ///< \ru Флаг починки ребер исходных оболочек. \en Flag of input shells edges repair.

  /// \ru Получить флаги слияния подобных элементов. \en Get flags of merging. 
  MbMergingFlags GetMerging() const { return MbMergingFlags( mergeFaces, mergeEdges ); }

  /// \ru Проверить состояние флага. \en Check the flag's state.
  void CheckEnclosureState() { if ( closed ) { enclosureCheck = true; } }
  /// \ru Сливать подобные элементы. \en Whether to merge similar items. 
  void SetMerging( const MbMergingFlags & f ) { mergeFaces = f.MergeFaces(); mergeEdges = f.MergeEdges(); } 

  void SetMergingFaces( bool s ) { mergeFaces = s; } ///< \ru Сливать подобные грани. \en Whether to merge similar faces. 
  void SetMergingEdges( bool s ) { mergeEdges = s; } ///< \ru Сливать подобные ребра. \en Whether to merge similar edges. 
  void SetAllowNonIntersecting( bool s ) { allowNonIntersecting = s; } ///< \ru Выдавать конечную оболочку, если нет пересечений. \en Allow a final result if there is no intersection.
  void SetEnclosureCheck( bool s ) { enclosureCheck = s; } ///< \ru Проверять оболочки на вложенность. \en Check shell on nesting.
  void SetShellEdgesRepair( bool s ) { repairShellEdges = s; } ///< \ru Чинить ребера исходных оболочек. \en Repair edges of input shells.

  /// \ru Оператор присваивания. \en Assignment operator.
  MbBooleanFlags & operator = ( const MbBooleanFlags & flags )
  {
    mergeFaces           = flags.mergeFaces;
    mergeEdges           = flags.mergeEdges;
    closed               = flags.closed;
    enclosureCheck       = flags.enclosureCheck;
    allowNonIntersecting = flags.allowNonIntersecting;
    cutting              = flags.cutting;
    repairShellEdges     = flags.repairShellEdges;

    return *this;
  }
};


#endif // __OP_BOOLEAN_FLAGS_H
