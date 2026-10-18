////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Построение оболочки тела без истории.
         \en Constructor of solid shell without history. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_SIMPLE_CREATOR_H
#define __CR_SIMPLE_CREATOR_H


#include <creator.h>


class    MbFaceShell;
class    MbSolid;

//------------------------------------------------------------------------------
/** \brief \ru Строитель оболочки тела без истории.
           \en Constructor of a solid shell without history. \~
  \details \ru Строитель оболочки тела без истории построения. \n
           \en Constructor of a solid shell without history tree. \n \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbSimpleCreator : public MbCreator {
public :
  MbFaceShell * outer;     ///< \ru Набор граней без истории. \en Face set without history. 
  OperationType operation; ///< \ru Тип булевой операции. \en Boolean operation type. 
  double        buildSag;  ///< \ru Шаг построения булевой операции. \en Sag of Boolean operation construction. 

public :
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по оболочке с возможностью использования ее оригинала или копии.
             \en Constructor by a shell with possibility of using the original or a copy. \~
    \param[in] shell - \ru Оболочка.
                       \en A shell. \~
    \param[in] n     - \ru Именователь операции.
                       \en An object defining names generation in the operation. \~
    \param[in] same  - \ru Признак использования оригинала оболочки.
                       \en Flag of using the original shell. \~
  */
  MbSimpleCreator( const MbFaceShell & shell, const MbSNameMaker & n, bool same );

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по оболочке и типу операции.
             \en Constructor by a shell and a type of operation. \~
    \param[in] shell - \ru Оболочка.
                       \en A shell. \~
    \param[in] n     - \ru Именователь операции.
                       \en An object defining names generation in the operation. \~
    \param[in] op    - \ru Тип булевой операции.
                       \en A Boolean operation type. \~
  */
  MbSimpleCreator( const MbFaceShell & shell, const MbSNameMaker & n, OperationType op );

private :
  MbSimpleCreator( const MbSimpleCreator &, MbRegDuplicate * iReg ); // \ru Конструктор копирования с регистратором \en Copy-constructor with the registrator 
public :
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbSimpleCreator();

  /** \ru \name Общие функции строителя оболочки.
      \en \name Common functions of the shell creator.
      \{ */
  MbeCreatorType IsA() const override; // \ru Тип элемента \en Type of element
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию \en Create a copy
  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override;             // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override;             // \ru Сдвиг \en Translation
  void        Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate about an axis

  MbePrompt   GetPropertyName() override; // \ru Выдать заголовок свойства объекта \en Get a name of object property
  void        GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of the object 
  void        GetBasisItems  ( RPArray<MbSpaceItem> & ) override; // \ru Дать базовые объекты \en Get the basis objects 
  void        GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  bool        IsSame( const MbCreator &, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        IsSimilar ( const MbCreator & ) const override; // \ru Являются ли объекты подобными \en Whether the objects are similar
  bool        SetEqual( const MbCreator & ) override;       // \ru Сделать равным \en Make equal

  bool        CreateShell( MbFaceShell *&shell, MbeCopyMode sameShell,
                           RPArray<MbSpaceItem> * items = nullptr ) override; // \ru Построение \en Construction
  /** \} */

  const MbFaceShell * GetShell() const { return outer; }  /// \ru Дать оболочку. \en Get a shell. 
  void          SetShell( const MbFaceShell & );    /// \ru Заменить оболочку. \en Replace a shell. 
  OperationType GetOperationType() { return operation; }                /// \ru Дать оболочку. \en Get a shell. 
  void          SetOperationType( OperationType t ) { operation = t; }  /// \ru Заменить оболочку. \en Replace a shell.

  /// \ru Удалить копии оболочек в простых построителях (MbSimpleCreator). \en Delete shell copies in simple creators (MbSimpleCreator). 
  template <class CreatorsVector>
  static  bool          DeleteShellCopies( const CreatorsVector & );
  /// \ru Есть ли в каком-то простом построителе (MbSimpleCreator) заданная оболочка. \en Is there a simple builder (MbSimpleCreator) that contains a given shell?. 
  template <class CreatorsVector>
  static  bool          IsThisShell( const MbFaceShell &, const CreatorsVector & );
  /// \ru Есть ли в каком-то простом построителе (MbSimpleCreator) заданная оболочка. \en Is there a simple builder (MbSimpleCreator) that contains a given shell?. 
  static  bool          IsThisShell( const MbSolid & );

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbSimpleCreator )
  OBVIOUS_PRIVATE_COPY( MbSimpleCreator )
}; // MbSimpleCreator

IMPL_PERSISTENT_OPS( MbSimpleCreator )


//------------------------------------------------------------------------------
// Sort by shell pointers (ascending)
// ---
inline
bool SortByShellPointers( const c3d::IndexConstShell & is1, const c3d::IndexConstShell & is2 )
{
  if ( is1.second < is2.second )
    return true;
  return false;
}

//------------------------------------------------------------------------------
// Sort by shell pointers (ascending)
// ---
inline
bool AreEqualShellPointers( const c3d::IndexConstShell & is1, const c3d::IndexConstShell & is2 )
{
  if ( is1.second == is2.second )
    return true;
  return false;
}


//------------------------------------------------------------------------------
// Sort by index (ascending)
// ---
inline
bool SortByIndex( const c3d::IndexConstShell & is1, const c3d::IndexConstShell & is2 )
{
  if ( is1.first < is2.first )
    return true;
  return false;
}


//------------------------------------------------------------------------------
// \ru Удалить копии оболочек в простых построителях. \en Delete shell copies in creators MbSimpleCreator. 
// ---
template <class CreatorsVector>
bool MbSimpleCreator::DeleteShellCopies( const CreatorsVector & creators )
{ // KOMPAS-25604, KOMPAS-28405
  bool res = false;

  const size_t creatorsCnt = creators.size();
  std::vector<c3d::IndexConstShell> simpleShells;
  simpleShells.reserve( creatorsCnt );

  size_t i;
  for ( i = 0; i < creatorsCnt; ++i ) {
    MbCreator * creator = creators[i];
    if ( creator != nullptr ) {
      if ( creator->IsA() == ct_SimpleCreator ) {
        MbSimpleCreator * simpleCreator = static_cast<MbSimpleCreator *>(creator);
        simpleShells.push_back( std::make_pair( i, simpleCreator->GetShell() ) );
      }
    }
  }
  if ( simpleShells.size() > 1 ) {
    std::sort( simpleShells.begin(), simpleShells.end(), ::SortByShellPointers );
    simpleShells.erase( std::unique( simpleShells.begin(), simpleShells.end(), ::AreEqualShellPointers ), simpleShells.end() );
    std::sort( simpleShells.begin(), simpleShells.end(), ::SortByIndex );

    if ( simpleShells.size() > 1 ) {
      bool isReplaced = false;
      size_t checkCnt = simpleShells.size();
      for ( i = 0; i < checkCnt; ++i ) {
        size_t ind1 = simpleShells[i].first;
        MbSimpleCreator & sc1 = static_cast<MbSimpleCreator &>(*creators[ind1]);
        const MbFaceShell * shell1 = sc1.GetShell();
        for ( size_t j = i + 1; j < checkCnt; ++j ) {
          size_t ind2 = simpleShells[j].first;
          MbSimpleCreator & sc2 = static_cast<MbSimpleCreator &>(*creators[ind2]);
          const MbFaceShell * shell2 = sc2.GetShell();
          if ( shell1 && shell2 && (shell1 != shell2) ) {
            if ( shell1->IsSame( *shell2, LENGTH_EPSILON ) ) {
              sc2.SetShell( *shell1 );
              simpleShells[j].second = nullptr;
              isReplaced = true;
            }
          }
        }
      }
      if ( isReplaced && (simpleShells.size() > 1) ) {
        std::sort( simpleShells.begin(), simpleShells.end(), ::SortByShellPointers );
        simpleShells.erase( std::unique( simpleShells.begin(), simpleShells.end(), ::AreEqualShellPointers ), simpleShells.end() );
        if ( simpleShells.size() > 1 ) {
          if ( simpleShells.front().second == nullptr )
            simpleShells.erase( simpleShells.begin() );
        }
      }
    }
  }

  return res;
}


//------------------------------------------------------------------------------
// \ru Есть ли в каком-то простом построителе (MbSimpleCreator) заданная оболочка. \en Is there a simple builder (MbSimpleCreator) that contains a given shell?. 
// ---
template <class CreatorsVector>
bool MbSimpleCreator::IsThisShell( const MbFaceShell & shell, const CreatorsVector & creators )
{
  bool res = false;

  if ( creators.size() > 0 ) {
    for ( size_t i = creators.size(); i--; ) {
      const MbCreator * creator = creators[i];
      if ( (creator != nullptr) && (creator->IsA() == ct_SimpleCreator) ) {
        const MbSimpleCreator & simpleCreator = static_cast<const MbSimpleCreator &>(*creator);
        if ( simpleCreator.GetShell() == &shell ) {
          res = true;
          break;
        }
      }
    }
  }

  return res;
}


//------------------------------------------------------------------------------
/** \brief \ru Строитель вывернутого "наизнанку" тела.
           \en Constructor of a reversed solid. \~
  \details \ru Строитель вывернутого "наизнанку" тела. \n
           \en Constructor of a reversed solid. \n \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbReverseCreator : public MbCreator {

public :
  /// \ru Конструктор. \en Constructor. \~
  MbReverseCreator( const MbSNameMaker & );

private :
  MbReverseCreator( const MbReverseCreator &, MbRegDuplicate * iReg ); // \ru Конструктор копирования с регистратором \en Copy-constructor with the registrator 
public :
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbReverseCreator();

  /** \ru \name Общие функции строителя оболочки.
      \en \name Common functions of the shell creator.
      \{ */
  MbeCreatorType IsA() const override; // \ru Тип элемента \en Type of element
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию \en Create a copy
  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override;             // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override;             // \ru Сдвиг \en Translation
  void        Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate about an axis

  MbePrompt   GetPropertyName() override; // \ru Выдать заголовок свойства объекта \en Get a name of object property
  void        GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of the object 

  bool        IsSame( const MbCreator & other, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        SetEqual( const MbCreator & ) override;       // \ru Сделать равным \en Make equal
  bool        IsSimilar ( const MbCreator & ) const override; // \ru Являются ли объекты подобными \en Whether the objects are similar

  bool        CreateShell( MbFaceShell *& shell, MbeCopyMode sameShell,
                           RPArray<MbSpaceItem> * items = nullptr ) override; // \ru Построение \en Construction
  /** \} */

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbReverseCreator )
  OBVIOUS_PRIVATE_COPY( MbReverseCreator )
}; // MbReverseCreator

IMPL_PERSISTENT_OPS( MbReverseCreator )


#endif // __CR_SIMPLE_CREATOR_H
