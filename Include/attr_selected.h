////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Атрибуты. Селектированность. Видимость. Изменённость.
         \en Attributes. Selection. Visibility. Modification. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __ATTR_SELECTED_H
#define __ATTR_SELECTED_H


#include <attr_elementary_attribute.h>


//------------------------------------------------------------------------------
/** \brief \ru Селектированность.
           \en Selection. \~
  \details \ru Селектированность. \n
           \en Selection. \n \~
  \ingroup Model_Attributes
*/
// ---
class MATH_CLASS MbSelected : public MbElementaryAttribute {
protected :
  bool selected; ///< \ru Селектированность. \en Selection. 

protected :
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbSelected( const MbSelected & init ); 
public :
  /// \ru Конструктор. \en Constructor. 
  MbSelected( bool init ); 
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbSelected();

  // \ru Общие функции объекта. \en Common functions of object. 
  MbeAttributeType AttributeType() const override; // \ru Дать подтип атрибута. \en Get subtype of an attribute. 
  MbAttribute & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента. \en Create a copy of the element.
  bool    IsSame( const MbAttribute &, double accuracy ) const override; // \ru Определить, являются ли объекты равными. \en Determine whether objects are equal. 
  bool    Init( const MbAttribute & ) override; // \ru Инициализировать данные по присланным. \en Initialize data by given attribute. 

          /// \ru Установить селектированность. \en Set selection. 
          void    Init( bool init ) { selected = init; }
          /// \ru Дать селектированность. \en Get selection. 
          bool    Selected() const  { return selected; }

  void    GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  size_t  SetProperties( const MbProperties & ) override; // \ru Установить свойства объекта. \en Set properties of the object. 
  MbePrompt GetPropertyName() override; // \ru Выдать заголовок свойства объекта. \en Get a name of object property. 

private:
          void    operator = ( const MbSelected & ); // \ru Не реализовано \en Not implemented 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbSelected ) 

}; // MbSelected

IMPL_PERSISTENT_OPS( MbSelected )

//------------------------------------------------------------------------------
/** \brief \ru Видимость.
           \en Visibility. \~
  \details \ru Видимость. \n
           \en Visibility. \n \~
  \ingroup Model_Attributes
*/
// ---
class MATH_CLASS MbVisible : public MbElementaryAttribute {
protected :
  bool visible; ///< \ru Видимость. \en Visibility. 

protected :
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbVisible( const MbVisible & init ); 
public :
  /// \ru Конструктор. \en Constructor. 
  MbVisible( bool init );
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbVisible();

  // \ru Общие функции объекта. \en Common functions of object. 

  MbeAttributeType AttributeType() const override; // \ru Дать подтип атрибута. \en Get subtype of an attribute. 
  MbAttribute & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента. \en Create a copy of the element.
  bool    IsSame( const MbAttribute &, double accuracy ) const override; // \ru Определить, являются ли объекты равными. \en Determine whether objects are equal. 
  bool    Init( const MbAttribute & ) override; // \ru Инициализировать данные по присланным. \en Initialize data by given attribute. 

          /// \ru Установить видимость. \en Set visibility. 
          void    Init( bool init ) { visible = init; }
          /// \ru Дать видимость. \en Get visibility. 
          bool    Visible() const   { return visible; }

  void    GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  size_t  SetProperties( const MbProperties & ) override; // \ru Установить свойства объекта. \en Set properties of the object. 
  MbePrompt GetPropertyName() override; // \ru Выдать заголовок свойства объекта. \en Get a name of object property. 

private:
          void    operator = ( const MbVisible & ); // \ru Не реализовано \en Not implemented 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbVisible ) 

}; // MbVisible

IMPL_PERSISTENT_OPS( MbVisible )

//------------------------------------------------------------------------------
/** \brief \ru Изменённость.
           \en Modification. \~
  \details \ru Изменённость. \n
           \en Modification. \n \~
  \ingroup Model_Attributes
*/
// ---
class MATH_CLASS MbChanged : public MbElementaryAttribute {
protected :
  bool changed; ///< \ru Изменённость. \en Modification. 

protected :
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbChanged( const MbChanged & init ); 
public :
  /// \ru Конструктор. \en Constructor. 
  MbChanged( bool init );
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbChanged();

  // \ru Общие функции объекта. \en Common functions of object. 

  MbeAttributeType AttributeType() const override; // \ru Дать подтип атрибута. \en Get subtype of an attribute. 
  MbAttribute & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента. \en Create a copy of the element.
  bool    IsSame( const MbAttribute &, double accuracy ) const override; // \ru Определить, являются ли объекты равными. \en Determine whether objects are equal. 
  bool    Init( const MbAttribute & ) override; // \ru Инициализировать данные по присланным. \en Initialize data by given attribute. 

          /// \ru Установить изменённость. \en Set modification. 
          void    Init( bool init ) { changed = init; }
          /// \ru Дать изменённость. \en Get modification. 
          bool    Changed() const   { return changed; }

  void    GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  size_t  SetProperties( const MbProperties & ) override; // \ru Установить свойства объекта. \en Set properties of the object. 
  MbePrompt GetPropertyName() override; // \ru Выдать заголовок свойства объекта. \en Get a name of object property. 

private:
          void    operator = ( const MbChanged & ); // \ru Не реализовано \en Not implemented 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbChanged ) 

}; // MbChanged

IMPL_PERSISTENT_OPS( MbChanged )

#endif // __ATTR_SELECTED_H
