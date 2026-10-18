////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Подтип элементарные атрибуты.
         \en Elementary attributes subtype. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __ATTR_ELEMENTARY_ATTRIBUTE_H
#define __ATTR_ELEMENTARY_ATTRIBUTE_H


#include <attribute_item.h>


//------------------------------------------------------------------------------
/** \brief \ru Элементарный атрибут - базовый класс.
           \en Elementary attribute - the base class. \~
  \details \ru Элементарный атрибут - базовый класс. \n
           \en Elementary attribute - the base class. \n \~
  \ingroup Model_Attributes
 */
class MATH_CLASS MbElementaryAttribute : public MbAttribute {
protected:
  MbElementaryAttribute();
public:
  virtual ~MbElementaryAttribute();

public :
  MbeAttributeType AttributeFamily() const override;    // \ru Тип атрибута \en Type of an attribute 
  //virtual MbeAttributeType AttributeType() const = 0; // \ru Выдать подтип атрибута \en Get subtype of an attribute.
  //virtual MbAttribute & Duplicate( MbRegDuplicate * = nullptr ) const = 0; // \ru Сделать копию элемента \en Create a copy of the element.
  //virtual bool    IsSame( const MbAttribute &, double accuracy ) const = 0; // \ru Определить, являются ли объекты равными. \en Determine whether objects are equal.
  //virtual bool    Init( const MbAttribute & ) = 0; // \ru Инициализировать данные по присланным \en Initialize data.

  // \ru Действия при изменении владельца, не связанное с другими действиями. \en Actions which are not associated with other actions when changing the owner. 
  void    OnChangeOwner( const MbAttributeContainer & owner ) override;
  // \ru Действия при конвертации владельца. \en Actions when converting the owner. 
  void    OnConvertOwner( const MbAttributeContainer & owner, MbAttributeContainer & other ) override;
  /// \ru Действия при трансформировании владельца. \en Actions when transforming the owner. 
  void    OnTransformOwner( const MbAttributeContainer & owner, const MbMatrix3D &, MbRegTransform * = nullptr ) override;
  // \ru Действия при перемещении владельца. \en Actions when moving the owner. 
  void    OnMoveOwner( const MbAttributeContainer & owner, const MbVector3D &, MbRegTransform * = nullptr ) override;
  // \ru Действия при вращении владельца. \en Actions when rotating the owner. 
  void    OnRotateOwner( const MbAttributeContainer & owner, const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override;
  // \ru Действия при копировании владельца. \en Actions when copying the owner. 
  void    OnCopyOwner( const MbAttributeContainer & owner, MbAttributeContainer & other, MbRegDuplicate * = nullptr ) override;
  // \ru Действия при объединении владельца. \en Actions when merging the owner. 
  void    OnMergeOwner( const MbAttributeContainer & owner, MbAttributeContainer & other ) override; 
  // \ru Действия при замене владельца. \en Actions when replacing the owner. 
  void    OnReplaceOwner( const MbAttributeContainer & owner, MbAttributeContainer & other ) override;
  // \ru Действия при разделении владельца. \en Actions when splitting the owner. 
  void    OnSplitOwner( const MbAttributeContainer & owner, const std::vector<MbAttributeContainer *> & others ) override;
  // \ru Действия при удалении владельца. \en Actions when merging the owner. 
  void    OnDeleteOwner( const MbAttributeContainer & owner ) override;

  void    GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта \en Get properties of the object
  size_t  SetProperties( const MbProperties & ) override; // \ru Установить свойства объекта \en Set properties of object
  MbePrompt GetPropertyName() override; // \ru Выдать заголовок свойства объекта \en Get a name of object property 

DECLARE_PERSISTENT_CLASS( MbElementaryAttribute )
OBVIOUS_PRIVATE_COPY( MbElementaryAttribute )
}; 

IMPL_PERSISTENT_OPS( MbElementaryAttribute )

#endif // __ATTR_ELEMENTARY_ATTRIBUTE_H
