////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Атрибут Uuid для вставок.
         \en Uuid attribute for instances. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __ATTR_UUID_ATTRIBUTE_H
#define __ATTR_UUID_ATTRIBUTE_H


#include <attr_common_attribute.h>
#include <tool_uuid.h>


//------------------------------------------------------------------------------
/** \brief \ru Атрибут Uuid для вставок (MbInstance).
           \en Uuid attribute for instances (MbInstance). \~
  \details \ru Атрибут Uuid должен применяться только для вставок (MbInstance).
           \en Uuid attribute should be applied to instances (MbInstance) only. \n \~
  \ingroup Model_Attributes
 */
class MATH_CLASS MbUuidInstanceAttribute : public MbCommonAttribute {
protected :
  MbUuid  _uuid; ///< ru Глобально уникальный идентификатор. \en Global unique identifier.

public :
  /// \ru Конструктор. \en Constructor. 
  MbUuidInstanceAttribute( const MbUuid & uuid, const bool changeable, const c3d::string_t & prompt );
  /// \ru Конструктор. \en Constructor. 
  MbUuidInstanceAttribute( const MbUuid & uuid, const bool changeable = true );
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbUuidInstanceAttribute();

private:
  // \ru Конструктор копирования. \en Copy constructor. 
  MbUuidInstanceAttribute( const MbUuidInstanceAttribute & init, MbRegDuplicate * iReg );

public:
  // \ru Выдать подтип атрибута. \en Get subtype of an attribute. 
  MbeAttributeType AttributeType() const override;
  // \ru Сделать копию элемента. \en Create a copy of the element. 
  MbAttribute & Duplicate( MbRegDuplicate * = nullptr ) const override;
  // \ru Определить, являются ли объекты равными. \en Determine whether objects are equal. 
  bool    IsSame( const MbAttribute &, double accuracy ) const override;
  // \ru Инициализировать данные по присланным. \en Initialize data. 
  bool    Init( const MbAttribute & ) override;

  // \ru Выполнить действия при изменении владельца, не связанное с другими действиями. \en Perform actions which are not associated with other actions when changing the owner. 
  void    OnChangeOwner( const MbAttributeContainer & owner ) override;
  // \ru Выполнить действия при конвертации владельца. \en Perform actions when converting the owner. 
  void    OnConvertOwner( const MbAttributeContainer & owner, MbAttributeContainer & other ) override;
  // \ru Выполнить действия при замене владельца. \en Perform actions when replacing the owner. 
  void    OnReplaceOwner( const MbAttributeContainer & owner, MbAttributeContainer & other ) override;
  // \ru Выполнить действия при разделении владельца. \en Perform actions when splitting the owner. 
  // \ru Выполнить действия при удалении владельца. \en Perform actions when deleting the owner. 
  void    OnDeleteOwner( const MbAttributeContainer & owner ) override;

  void    GetCharValue( TCHAR * v ) const override; // \ru Выдать строковое значение свойства. \en Get a string value of the property.
  void    GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  size_t  SetProperties( const MbProperties & ) override; // \ru Установить свойства объекта. \en Set properties of object. 
  MbePrompt GetPropertyName() override; // \ru Выдать заголовок свойства объекта. \en Get a name of object property.

 
  /// \ru Инициализировать значением Uuid. \en Initialize with Uuid value.
  void  Init( const MbUuid & uuid ) { _uuid = uuid; }
  /// \ru Задать значение Uuid. \en Set Uuid value.
  void  SetUuid( const MbUuid & uuid ) { _uuid = uuid; }
  /// \ru Получить значение Uuid. \en Get Uuid value.
  const MbUuid &  GetUuid() const { return _uuid; }

DECLARE_PERSISTENT_CLASS( MbUuidInstanceAttribute ) 
OBVIOUS_PRIVATE_COPY( MbUuidInstanceAttribute )
}; 

IMPL_PERSISTENT_OPS( MbUuidInstanceAttribute )

#endif // __ATTR_UUID_ATTRIBUTE_H
