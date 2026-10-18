////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Функции преобразования данных из устаревшего MaAnnotationItem в актуальный MbPmi и обратно.
         \en Functions for converting data from depicated MaAnnotationItem to current MbPmi and back. \~
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CONV_PMI_DEPRICATED_CONVERT_H
#define __CONV_PMI_DEPRICATED_CONVERT_H

#include <mb_pmi.h>
#include <conv_annotation_item.h>

class MaAnnotationItem;
class MbPMI;
class MaTextItem;
class MbTextItem;

//------------------------------------------------------------------------------
/** \brief \ru Преобразовать элемент аннотации.
           \en Transform annotation item. \~
*/
// ---
CONV_FUNC( SPtr<MbPMI> ) ConvertPMI( const MaAnnotationItem & it );


//------------------------------------------------------------------------------
/** \brief \ru Преобразовать элемент технического требования.
           \en Transform technical requirements. \~
*/
// ---
CONV_FUNC( SPtr<MbPMI> ) ConvertTechnicalRequirements( const MaAnnotationItem & it );


//------------------------------------------------------------------------------
/** \brief \ru Преобразовать элемент аннотации.
           \en Transform annotation item. \~
*/
// ---
CONV_FUNC( SPtr<MaAnnotationItem> ) ConvertPMI( const MbPMI & it );

//------------------------------------------------------------------------------
// Преобразовать текстовый элемент
// ---
SPtr<MbTextItem> CreateTextItem( const MaTextItem & it );

//------------------------------------------------------------------------------
// Преобразовать текстовый элемент
// ---
SPtr<MaTextItem> CreateTextItem( const MbTextItem & it );


#endif // __CONV_PMI_DEPRICATED_CONVERT_H