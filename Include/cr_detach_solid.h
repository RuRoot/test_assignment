////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Разделение набора граней на связные части.
         \en Subdivision of face set into connected parts. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_DETACH_SOLID_H
#define __CR_DETACH_SOLID_H


#include <creator.h>


//------------------------------------------------------------------------------
/** \brief \ru Строитель, разделяющий набор граней на связные части.
           \en Constructor subdividing a set of faces into connected parts. \~
  \details \ru Строитель, разделяющий набор граней на связные части в виде оболочек и 
    сортирующий отдельные оболочки по убыванию диагоналей габаритных кубов частей. \n
           \en Constructor subdividing a set of faces into connected parts in form of shells and 
    sorting separate shells by decreasing of the parts's bounding boxes diagonals. \n \~ 
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbDetachSolid : public MbCreator {
protected :
  ptrdiff_t part; ///< \ru Номер оболочки, выделенной из общего набора граней. \en Number of a shell extracted from the common set of faces. 
  bool      sort; ///< \ru Сортированы ли оболочки по габаритам. \en Whether the shells are sorted by sizes. 

public :
  MbDetachSolid( ptrdiff_t p, bool s, const MbSNameMaker & n );
private :
  MbDetachSolid( const MbDetachSolid & init, MbRegDuplicate *ireg );
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbDetachSolid( const MbDetachSolid & init );
public :
  virtual ~MbDetachSolid();

  /** \ru \name Общие функции математического объекта.
      \en \name Common functions of the mathematical object.
      \{ */
  MbeCreatorType  IsA() const override; // \ru Тип элемента \en Type of element
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию \en Create a copy
  void        Transform( const MbMatrix3D & matr, MbRegTransform * = nullptr ) override;          // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move( const MbVector3D & to, MbRegTransform * = nullptr ) override;                 // \ru Сдвиг \en Translation
  void        Rotate( const MbAxis3D & axis, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate about an axis

  void        GetProperties( MbProperties & properties ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & properties ) override; // \ru Записать свойства объекта \en Set properties of the object 
  MbePrompt   GetPropertyName() override; // \ru Выдать заголовок свойства объекта \en Get a name of object property

  bool        IsSame( const MbCreator &, double accuracy ) const override; // \brief \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        IsSimilar( const MbCreator & ) const override; // \ru Являются ли объекты подобными? \en Whether the objects are similar?
  bool        SetEqual ( const MbCreator & ) override;       // \ru Сделать равным. \en Make equal.

  bool        CreateShell( MbFaceShell *& shell, MbeCopyMode sameShell,
                           RPArray<MbSpaceItem> * items = nullptr ) override; // \ru Построение \en Construction
  /** \} */
  /** \ru \name Функции строителя, разделяющие отдельные части оболочки.
      \en \name Functions of the creator subdividing separate parts of the shell.
      \{ */
  /// \ru Дать номер части, выделенной из общей оболочки. \en Get number of the part extracted from the common shell. 
  ptrdiff_t   GetPartNumber() const { return part; }
  /// \ru Установить номер части, выделенной из общей оболочки. \en Set number of the part extracted from the common shell. 
  void        SetPartNumber( ptrdiff_t p ) { part = p; }
  /// \ru Сортированы ли части по габаритам (диагоналям). \en Whether the parts are sorted by bounding boxes (diagonals). 
  bool        IsSort() const { return sort; }
  /** \} */
private :
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
  void         operator = ( const MbDetachSolid & );

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbDetachSolid )
}; // MbDetachSolid

IMPL_PERSISTENT_OPS( MbDetachSolid )


//------------------------------------------------------------------------------
/** \brief \ru Разделить несвязанные части набора граней на связанные наборы граней.
           \en Divide disconnected parts of a face set into connected sets of faces. \~
  \details \ru Разделить несвязанные части набора граней на связанные наборы граней - оболочки. 
    Одна связная оболочка (если sort=true, то наибольшая по диагонали габаритного куба) остаётся в исходном наборе граней solid.
    Отделенные наборы граней складываются в контейнер partSolid.
           \en Divide disconnected parts of a face set into connected sets of faces - shells. 
    One connected shell (if sort=true, then it is the greatest by the bounding box diagonal) remains in the initial set of faces 'solid'.
    Separated face sets are put into container partSolid. \~
  \param[in, out] solid     - \ru Исходный набор граней, на выходе - одна из связных оболочек.
                              \en Initial face set, in output - one of the connected shells. \~
  \param[out]     partSolid - \ru Набор всех связных частей кроме одной.
                              \en Set of all connected parts except one. \~
  \param[in]      sort      - \ru Если true, то в partSolid сортировать оболочки по убыванию диагоналей габаритного куба.
                              \en If true, then the shells should be sorted in partSolid by decreasing the bounding box diagonals. \~
  \result \ru Количество оболочек в контейнере partSolid.
          \en Number of shells in container partSolid. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (size_t) MakeDetachShells( MbFaceShell &          solid, 
                                     RPArray<MbFaceShell> & partSolid, 
                                     bool                   sort );


//------------------------------------------------------------------------------
/** \brief \ru Разделить несвязанные части набора граней на связанные наборы граней.
           \en Divide disconnected parts of a face set into connected sets of faces. \~
  \details \ru Разделить несвязанные части набора граней на связанные наборы граней - оболочки. 
    Одна связная оболочка (если sort=true, то наибольшая по диагонали габаритного куба) остаётся в исходном наборе граней solid.
    Отделенные наборы граней складываются в контейнер partSolid.
           \en Divide disconnected parts of a face set into connected sets of faces - shells. 
    One connected shell (if sort=true, then it is the greatest by the bounding box diagonal) remains in the initial set of faces 'solid'.
    Separated face sets are put into container partSolid. \~
  \param[in, out] solid     - \ru Исходный набор граней, на выходе - одна из связных оболочек.
                              \en Initial face set, in output - one of the connected shells. \~
  \param[out]     partSolid - \ru Набор всех связных частей кроме одной.
                              \en Set of all connected parts except one. \~
  \param[in]      sort      - \ru Если true, то в partSolid сортировать оболочки по убыванию диагоналей габаритного куба.
                              \en If true, then the shells should be sorted in partSolid by decreasing the bounding box diagonals. \~
  \result \ru Количество оболочек в контейнере partSolid.
          \en Number of shells in container partSolid. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (size_t) MakeDetachShells( MbFaceShell &       solid, 
                                     c3d::ShellsVector & partSolid, 
                                     bool                sort );


//------------------------------------------------------------------------------
/** \brief \ru Разделить несвязанные части набора граней на связанные наборы граней.
           \en Divide disconnected parts of a face set into connected sets of faces. \~
  \details \ru Разделить несвязанные части набора граней на связанные наборы граней - оболочки. 
    Одна связная оболочка (если sort=true, то наибольшая по диагонали габаритного куба) остаётся в исходном наборе граней solid.
    Отделенные наборы граней складываются в контейнер partSolid.
           \en Divide disconnected parts of a face set into connected sets of faces - shells. 
    One connected shell (if sort=true, then it is the greatest by the bounding box diagonal) remains in the initial set of faces 'solid'.
    Separated face sets are put into container partSolid. \~
  \param[in, out] solid     - \ru Исходный набор граней, на выходе - одна из связных оболочек.
                              \en Initial face set, in output - one of the connected shells. \~
  \param[out]     partSolid - \ru Набор всех связных частей кроме одной.
                              \en Set of all connected parts except one. \~
  \param[in]      sort      - \ru Если true, то в partSolid сортировать оболочки по убыванию диагоналей габаритного куба.
                              \en If true, then the shells should be sorted in partSolid by decreasing the bounding box diagonals. \~
  \result \ru Количество оболочек в контейнере partSolid.
          \en Number of shells in container partSolid. \~
  \ingroup Solid_Modeling
*/
// ---
MATH_FUNC (size_t) MakeDetachShells( MbFaceShell &           solid, 
                                     c3d::ShellsSPtrVector & partSolid, 
                                     bool                    sort );


//------------------------------------------------------------------------------
/** \brief \ru Разделить несвязанные части набора граней на связанные наборы граней.
           \en Divide disconnected parts of a face set into connected sets of faces. \~
  \details \ru Разделить несвязанные части набора граней на связанные наборы граней - оболочки. 
    Одновременно с построением оболочки функция создаёт её строитель.\n
           \en Divide disconnected parts of a face set into connected sets of faces - shells. 
    The function simultaneously creates the shell and its constructor.\n \~ 
  \param[in, out] solid     - \ru Исходная оболочка.
                              \en The initial shell. \~
  \param[out]     partSolid - \ru Набор всех связных частей - оболочек.
                              \en Set of all the connected parts - shells. \~
  \param[in]      sort      - \ru Если true, то в partSolid сортировать оболочки по убыванию диагоналей габаритного куба.
                              \en If true, then the shells should be sorted in partSolid by decreasing the bounding box diagonals. \~
  \param[in]      n         - \ru Именователь граней.
                              \en An object for naming faces. \~
  \param[out]     res       - \ru Код результата операции.
                              \en Operation result code. \~
  \result \ru Возвращает строитель.
          \en Returns the constructor. \~
  \ingroup Solid_Modeling
*/
// ---
template <class ShellsVector>
MbCreator * CreateDetach(       MbFaceShell &  solid,
                                ShellsVector & partSolid,
                                bool           sort,
                          const MbSNameMaker & n,
                                MbResultType & res )
{
  res = rt_Error;
  MbCreator * result = nullptr;

  ::MakeDetachShells( solid, partSolid, sort );

  if ( partSolid.size() > 0 ) {
    if ( !Math::SupressCreators() )
      result = new MbDetachSolid( 0, sort, n );
    res = rt_Success;
  }

  return result;
}


#endif // __CR_DETACH_SOLID_H
