////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Генератор скруглений граней. \en Maker of faces fillets. \~
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_FACE_FILLET_MAKER_H
#define __CR_FACE_FILLET_MAKER_H

#include <solid.h>
#include <op_shell_parameter.h>


class MbFilletInternalMaker;


//------------------------------------------------------------------------------
/** \brief \ru Генератор скруглений граней. \en Maker of faces fillets. \~
  \details \ru Класс для создания скруглений граней тела.
           \en Class for creating fillets of faces. \~
  \ingroup Shell_Modeling
  \warning \ru В разработке. \en Under development. \~
*/
// ---
class MATH_CLASS MbFaceFilletMaker
{
  std::unique_ptr<MbFilletInternalMaker> _impl;      ///< \ru Реализация алгоритма. \en The algorithm implementation.

public:
  /// \ru Конструктор устарел и будет удален. \en Constructor deprecated and will be deleted.
  //DEPRECATE_DECLARE
  explicit MbFaceFilletMaker( MbeCopyMode , const MbSectionData & data );
  /// \ru Конструктор. \en Constructor.
  explicit MbFaceFilletMaker( const MbSectionData & data );

  /// \ru Деструктор. \en Destructor.
  ~MbFaceFilletMaker();

  /// \ru Построить скругления и выдать количество частей (несвязанных оболочек) в результате. \en Create fillets and return a number of non-connected shells in a result.
  MbResultType CreateFillets( size_t & partsCount, MbSectionResults & result );

  /// \ru  Модифицировать базовые объекты указанными частями скругления. Если массив индексов пустой, то выполняется подрезка всеми частями скругления.
  /// \en Modify base objects by the defines parts of the fillet. If indices array is empty then performed cutting with the all parts of the fillet.
  /// 
  MbResultType ModifyBaseObjects( const c3d::IndicesVector & partsIndicies, std::vector<MbSectionResults> & result );

  /// \ru Получить параметры построения. \en Get the constructing parameters.
  const MbSectionData & GetSectionData() const;

OBVIOUS_PRIVATE_COPY( MbFaceFilletMaker )
};


#endif // __CR_FACE_FILLET_MAKER_H
