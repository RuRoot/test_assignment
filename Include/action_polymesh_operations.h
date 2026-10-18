////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Классы для операций с полигональными геометрическими объектами с топологией.
         \en Classes for operations with polygonal geometric objects with topology. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __ACTION_POLYMESH_OPERATIONS_H
#define __ACTION_POLYMESH_OPERATIONS_H

#include <math_define.h>
#include <mb_operation_result.h>

class MbPolymesh;
class MbSegmentationParams;
class MbRepairSegmentationParams;


//------------------------------------------------------------------------------
/** \brief \ru Абстрактный класс для сегментации полигональных объектов.
           \en An abstract class for segmenting polygonal objects. \~
    \details \ru Абстрактный класс для сегментации полигональных объектов.
             \en An abstract class for segmenting polygonal objects. \~
    \warning \ru В разработке.
             \en Under development. \~
*/
// ---
class MATH_CLASS MbMeshSegmenter
{
protected:
  /// \ru Конструктор. \en Constructor.
  MbMeshSegmenter() = default;

public:
  /// \ru Деструктор. \en Destructor.
  virtual ~MbMeshSegmenter() = default;

  /** \brief \ru Создать экземпляр класса на основе MbPolymesh.
             \en Create an object by MbPolymesh. \~
    \param[in] mesh - \ru Полигональный геометрический объект.
                      \en A polygonal geometric object. \~
    \return \ru Возвращает указатель на созданный объект.
            \en Returns the pointer to a created object. \~
  */
  static MbMeshSegmenter * Create( MbPolymesh & mesh );
  /** \brief \ru Сегментировать полигональный объект.
             \en Segment the polygonal object. \~
    \param[in] params - \ru Параметры сегментации.
                        \en Segmentation parameters. \~
    \return \ru Код результата операции.
            \en Returns the operation result code. \~
  */
  virtual MbResultType Segment( const MbSegmentationParams & params ) = 0;
  /** \brief \ru Починить сегментацию полигонального объекта.
             \en Repair the segmentation of the polygonal object. \~
    \param[in] params - \ru Параметры лечения сегментации.
                        \en Parameters for repairing the segmentation. \~
  */
  virtual void RepairSegmentation( const MbRepairSegmentationParams & params ) = 0;
  /** \brief \ru Классифицировать регионы сегментации по типам аналитических поверхностей.
             \en Classify segmentation regions by surface types. \~
    \param[in] tolerance - \ru Точность вписывания поверхностей.
                           \en Surface fitting tolerance. \~
  */
  virtual void ClassifyRegions( double tolerance ) = 0;
};

#endif // __ACTION_POLYMESH_OPERATIONS_H
