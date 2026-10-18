//////////////////////////////////////////////////////////////////////////////////////////
/** \file 
    \brief  \ru Реализация данных узла дерева модели 
            \en Implementation of data of Model Tree node \~
*/
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __MODEL_TREE_DATA_H
#define __MODEL_TREE_DATA_H


#include <io_tape_define.h>
#include <templ_multimap.h>
#include <mb_cube.h>
#include <attribute_item.h>
#include <attribute_container.h>
#include <space_item.h>
#include <mb_placement3d.h>
#include <instance_item.h>

//----------------------------------------------------------------------------------------
// \ru Реализация пользовательских данных узла дерева модели.
// \en Implementation of user data of the model tree node.
//----------------------------------------------------------------------------------------

namespace c3d // namespace C3D
{
//----------------------------------------------------------------------------------------
/** \brief   \ru Тип пользовательских данных узла дерева модели.
             \en A type of user data of the model tree node. \~
    \details \ru \ru Тип пользовательских данных узла дерева модели.
             \en A type of user data of the model tree node. \~
    \ingroup Base_Tools_IO
*/
// ---
enum MbeItemDataType
{
  // \ru Простые пользовательские данные. \en Simple user data.
  idtBool,            // bool
  idtInteger,         // int
  idtDouble,          // double
  idtString,          // c3d::string_t

  // \ru Данные атрибутов. \en Attributes data.

  // \ru Данные атрибутов, для которых хранится тип, подсказка и значение атрибута.
  // \en Attributes data which keeps attribute type, prompt and value.
  idtAttrBool,        // MbBoolAttribute   (bool)             at_BoolAttribute
  idtAttrInt,         // MbIntAttribute    (int)              at_IntAttribute
  idtAttrDouble,      // MbDoubleAttribute (double)           at_DoubleAttribute
  idtAttrString,      // MbStringAttribute (c3d::string_t)    at_StringAttribute
  idtAttrInt64,       // MbInt64Attribute  (int64)            at_Int64Attribute

  // \ru Данные атрибутов, для которых хранится тип и значение атрибута.
  // \en Attributes data which keeps attribute type and value.
  idtAttrIdentifier,  // MbIdentifier      (int32)            at_Identifier
  idtAttrColor,       // MbColor           (uint32)           at_Color
  idtAttrWidth,       // MbWidth           (int)              at_Width
  idtAttrStyle,       // MbStyle           (int)              at_Style
  idtAttrSelected,    // MbSelected        (bool)             at_Selected
  idtAttrVisible,     // MbVisible         (bool)             at_Visible
  idtAttrChanged,     // MbChanged         (bool)             at_Changed
  idtAttrDencity,     // MbDencity         (double)           at_Dencity
  idtAttrUpdateStamp, // MbUpdateStamp     (uint32)           at_UpdateStamp
  idtAttrAnchor,      // MbAnchorAttribute (uint8)            at_AnchorAttribute

  // \ru Данные сложных атрибутов, для которых хранится только тип.
  // \en Complex attributes data which keeps attribute type only.
  idtAttrVisual,                 // MbVisual                  at_Visual
  idtAttrWireCount,              // MbWireCount               at_WireCount
  idtAttrName,                   // MbNameAttribute           at_NameAttribute
  idtAttrGeom,                   // MbGeomAttribute           at_GeomAttribute
  idtAttrStampRib,               // MbStampRibAttribute       at_StampRibAttribute
  idtAttrModelInfo,              // MbModelInfo               at_ModelInfo
  idtAttrPersonOrganizationInfo, // MbPersonOrganizationInfo  at_PersonOrganizationInfo

  // \ru Специальная обработка атрибута - хранится свойство Идентификатор (Обозначение).
  // \en Special processing of attribute - keep property Identifier.
  idtAttrProductInfo,            // MbProductInfo             at_ProductInfo

  // \ru Данные сложных атрибутов, для которых хранится только тип (продолжение 1).
  // \en Complex attributes data which keeps attribute type only (continuation 1).
  idtAttrSTEPTextDescription,    // MbSTEPTextDescription     at_STEPTextDescription
  idtAttrSTEPReferenceHolder,    // MbSTEPReferenceHolder     at_STEPReferenceHolder
  idtAttrBinary,                 // MbBinaryAttribute         at_BinaryAttribute

  // \ru Специальный атрибут исполнения (хранится тип и пара значений).
  // \en Special attribute of embodiment (keeps type and values pair).
  idtAttrEmbodiment,             // MbEmbodimentAttribute     at_Embodiment

  // \ru Данные сложных атрибутов, для которых хранится только тип (продолжение 2).
  // \en Complex attributes data which keeps attribute type only (continuation 2).
  idtAttrStrains,               // MbStrains                  at_Strains
  idtAttrElasticity,            // MbElasticity               at_Elasticity
  idtAttrSheetFlanging,         // MbSheetFlangingAttribute   at_SweptFlangeAttribute
  idtAttrThickness,             // MbShellThickness           at_Thickness

  idtAttrInt32Vector,            // MbInt32VectorAttribute    at_Int32VectorAttribute
  idtAttrInt64Vector,            // MbInt64VectorAttribute    at_Int64VectorAttribute
  idtAttrDoubleVector,           // MbDoubleVectorAttribute   at_DoubleVectorAttribute

  // \ru Новый тип должен добавляться непосредственно перед idtCount (после всех определенных ранее типов).
  // \en New type should be added just before idtCount (after all types defined before).
  idtCount // \ru Число поддерживаемых типов данных. \en Number of supported data types
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Базовый класс для пользовательских данных узла дерева.
             \en A base class for user data of a tree node. \~
    \details \ru Базовый класс для пользовательских данных узла дерева.
             \en A base class for user data of a tree node. \~
    \ingroup Base_Tools_IO
*/
// ---
class MATH_CLASS ItemDataBase
{
// \ru Приведение объекта 'item' к типу данных 'Type' (тип проверен заранее).
// \en Cast object 'item' to the data type 'Type' (type already verified).
#define CAST(Type, item) dynamic_cast<Type*>(const_cast<ItemDataBase*>(item))

protected:
  bool m_ignoreValue; // \ru Игнорировать значение при сравнении. \en Ignore value when comparing.

public:

  ItemDataBase() : m_ignoreValue(false) {}
  virtual ~ItemDataBase() {}

  // \ru Тип данных. \en The data type.
  virtual MbeItemDataType Type() const = 0;

  // \ru Размер записи данных в поток. \en The data size in the stream.
  virtual size_t Size( writer& ) const = 0;

  // \ru Прочитать данные. \en Read data.
  virtual reader& operator << ( reader& in );

  // \ru Записать данные. \en Write data.
  virtual writer& operator >> ( writer& out ) const;

  // \ru Сравнить данные. \en Compare data.
  virtual bool operator == ( ItemDataBase* item2 ) const;

  /// \ru Сравнить данные. \en Compare data.
  virtual bool operator < ( ItemDataBase* item2 ) const;

  // \ru Выдать/установить флаг игнорирования значения при сравнении.
  // \en Get/set flag to ignore value when comparing.
  virtual bool IgnoreValue() const { return m_ignoreValue;  }
  virtual void SetIgnoreValue( bool ignore ) { m_ignoreValue = ignore; }

  // \ru Выдать/установить подсказку. \en Get/set a prompt.
  virtual c3d::string_t GetPrompt() const { return c3d::string_t();  }
  virtual void          SetPrompt( const c3d::string_t & ) {}

  // \ru Создать данные заданного типа. \en Create data of the given type.
  static ItemDataBase* Create( MbeItemDataType type );

  // \ru Создать копию данных. \en Create data copy.
  static ItemDataBase* Create( ItemDataBase* item );
};


//----------------------------------------------------------------------------------------
/** \brief   \ru Базовый класс для данных атрибутов с подсказкой.
             \en A base class for data of an attribute with a prompt. \~
    \details \ru Базовый класс для данных атрибутов с подсказкой.
             \en A base class for data of an attribute with a prompt. \~
    \ingroup Base_Tools_IO
*/
// ---
class MATH_CLASS PromptAttrBase : public ItemDataBase
{
  // \ru Приведение объекта 'item' к типу данных 'Type' (тип проверен заранее).
  // \en Cast object 'item' to the data type 'Type' (type already verified).
#define CAST2(Type, item) dynamic_cast<Type*>(const_cast<PromptAttrBase*>(item))

protected:
  c3d::string_t m_prompt; ///< \ru Подсказка. \en A prompt. 

public:

  PromptAttrBase() : ItemDataBase(), m_prompt() {}
  PromptAttrBase( const c3d::string_t & p ) : ItemDataBase(), m_prompt( p ) {}
  virtual ~PromptAttrBase() {}

  // \ru Прочитать данные. \en Read data.
  reader & operator << ( reader & in );

  // \ru Записать данные. \en Write data.
  writer & operator >> ( writer & out ) const override;

  // \ru Сравнить данные. \en Compare data.
  bool operator == ( ItemDataBase * item2 ) const override;

  /// \ru Сравнить данные. \en Compare data.
  bool operator < ( ItemDataBase * item2 ) const override;

  // \ru Выдать/установить подсказку. \en Get/set a prompt.
  c3d::string_t GetPrompt() const override { return m_prompt; }
  void          SetPrompt( const c3d::string_t & p ) override { m_prompt = p; }

  // \ru Создать данные заданного типа. \en Create data of the given type.
  static ItemDataBase * Create2( MbeItemDataType type );

  // \ru Создать копию данных. \en Create data copy.
  static ItemDataBase * Create2( ItemDataBase * item );
};


//----------------------------------------------------------------------------------------
/// \ru Создать объект пользовательских данных для атрибута. Возвращает nullptr, если данный атрибут не поддерживается деревом модели.
/// \en Create user data object for the attribute. Return nullptr if this attribute is not supported in the model tree.
//---
MATH_FUNC( ItemDataBase* ) CreateAttributeData( MbAttribute* attr );

//----------------------------------------------------------------------------------------
/// \ru Прочитать атрибуты для узла дерева.
/// \en Read attributes for the model tree node.
//---
MATH_FUNC( std::unique_ptr<MbAttributeContainer> ) GetTreeNodeAttributes( const IModelTreeNode * node, reader& in );


//----------------------------------------------------------------------------------------
// \ru Функции чтения и записи для данных атрибута, для которого хранится тип и значение атрибута.
// \en Functions of reading and writing an attribute data, which keeps attribute type and value.
//---
#define MTREE_PERSISTENT_DATA_OBJ(Class)                      \
  public:                                                            \
  static void Read(reader& in, Class* item) { in >> item->m_value; } \
  static void Write(writer& out, const Class* item) { out << item->m_value; }

//----------------------------------------------------------------------------------------
// \ru Функции чтения и записи для данных атрибута, для которого хранится тип, подсказка и значение атрибута.
// \en Functions of reading and writing an attribute data, which keeps attribute type, prompt and value.
//---
#define MTREE_PERSISTENT_DATA_OBJ2(Class)                      \
  public:                                                            \
  static void Read(reader& in, Class* item) { if(in.GetFormatVersion() >= 0x01000001L) in >> item->m_prompt; in >> item->m_value; } \
  static void Write(writer& out, const Class* item) {  if( out.GetFormatVersion() >= 0x01000001L ) out << item->m_prompt; out << item->m_value; }

//----------------------------------------------------------------------------------------
// \ru Определение типа для пользовательских данных атрибута.
// \en Type definition for attribute user data.
//---
#define MTREE_DEFINE_DATA_TYPE(type)  \
  public:                                   \
  virtual MbeItemDataType Type() const { return type; }

//----------------------------------------------------------------------------------------
/// \ru Определение размера записи пользовательских данных атрибута в поток, как sizeof.
/// \en Definition of data size of attribute user data in the stream as sizeof.
//---
#define MTREE_DEFINE_DATA_SIZE_STD(data) \
  public:                              \
  virtual size_t Size(writer&) const { return sizeof(data); }

//----------------------------------------------------------------------------------------
/// \ru Определение размера записи пользовательских данных атрибута с подсказкой в поток.
/// \en Definition of data size of user data of attribute with a prompt in the stream.
//---
#define MTREE_DEFINE_DATA_SIZE_STD2(prompt, data) \
  public:                                         \
  virtual size_t Size( writer& out ) const {      \
    return ( out.GetFormatVersion() >= 0x01000001L ) ? ( out.__lenWchar(m_prompt.c_str()) + sizeof(data) ) : sizeof( data );\
 }

//----------------------------------------------------------------------------------------
// \ru Макрос для объявления класса без данных (dataless) для атрибута.
// \en Macro for defining attribute dataless class.
//---
#define MTREE_ATTR_DATALESS_CLASS(Class,ClassType)            \
class Class : public ItemDataBase {                           \
public:                                                       \
  virtual ~Class() {}                                         \
  virtual size_t Size( writer& ) const { return 0; }          \
  virtual MbeItemDataType Type() const { return ClassType; }  \
  static void Read( reader&, Class* ) {}                      \
  static void Write( writer&, const Class* ) {}               \
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Свойство типа bool.
             \en Bool property. \~
    \details \ru Свойство типа bool.
             \en Bool property. \~
    \ingroup Base_Tools_IO
*/
// ---
class ItemDataBool : public ItemDataBase
{
public:
  bool m_value;

  ItemDataBool() : m_value( false ) {}
  ItemDataBool( bool value ) : m_value( value ) {}
  virtual ~ItemDataBool() {}

  MTREE_DEFINE_DATA_SIZE_STD(m_value)
  MTREE_DEFINE_DATA_TYPE(idtBool)
  MTREE_PERSISTENT_DATA_OBJ(ItemDataBool)
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Свойство типа integer.
             \en Integer property. \~
    \details \ru Свойство типа integer.
             \en Integer property. \~
    \ingroup Base_Tools_IO
*/
// ---
class ItemDataInteger : public ItemDataBase
{
public:
  int m_value;

  ItemDataInteger() : m_value( 0 ) {}
  ItemDataInteger( int value ) : m_value( value ) {}
  virtual ~ItemDataInteger() {}

  MTREE_DEFINE_DATA_SIZE_STD(int32)
  MTREE_DEFINE_DATA_TYPE(idtInteger)
  MTREE_PERSISTENT_DATA_OBJ(ItemDataInteger)
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Свойство типа double.
             \en Double property. \~
    \details \ru Свойство типа double.
             \en Double property. \~
    \ingroup Base_Tools_IO
*/
// ---
class ItemDataDouble : public ItemDataBase
{
public:
  double m_value;

  ItemDataDouble() : m_value( 0 ) {}
  ItemDataDouble( double value ) : m_value( value ) {}
  virtual ~ItemDataDouble() {}

  MTREE_DEFINE_DATA_SIZE_STD(m_value)
  MTREE_DEFINE_DATA_TYPE(idtDouble)
  MTREE_PERSISTENT_DATA_OBJ(ItemDataDouble)
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Свойство типа string.
             \en String property. \~
    \details \ru Свойство типа string.
             \en String property. \~
    \ingroup Base_Tools_IO
*/
// ---
class ItemDataString : public ItemDataBase
{
public:
  c3d::string_t m_value;

  ItemDataString() {}
  ItemDataString( c3d::string_t value ) : m_value( value ) {}
  virtual ~ItemDataString() {}

  /// \ru Размер записи данных в поток. \en The data size in the stream.
  virtual size_t Size( writer& out ) const { return out.__lenWchar( m_value.c_str() ); }

  MTREE_DEFINE_DATA_TYPE(idtString)
  MTREE_PERSISTENT_DATA_OBJ(ItemDataString)
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Данные атрибута bool.
             \en Data of Bool attribute. \~
    \details \ru Данные атрибута bool.
             \en Data of Bool attribute. \~
\ingroup Base_Tools_IO
*/
// ---
class ItemAttrBool : public PromptAttrBase
{
public:
  bool m_value;
  ItemAttrBool() : PromptAttrBase(), m_value( false ) {}
  ItemAttrBool( bool value ) : PromptAttrBase(), m_value( value ) {}
  ItemAttrBool( const c3d::string_t & p, bool value ) : PromptAttrBase( p ), m_value( value ) {}

  MTREE_DEFINE_DATA_SIZE_STD2(m_prompt, m_value)
  MTREE_DEFINE_DATA_TYPE(idtAttrBool)
  MTREE_PERSISTENT_DATA_OBJ2(ItemAttrBool)
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Данные атрибута integer.
             \en Data of Integer attribute. \~
    \details \ru Данные атрибута integer.
             \en Data of Integer attribute. \~
\ingroup Base_Tools_IO
*/
// ---
class ItemAttrInteger : public PromptAttrBase
{
public:
  int m_value;
  ItemAttrInteger() : PromptAttrBase(), m_value( 0 ) {}
  ItemAttrInteger( int value ) : PromptAttrBase(), m_value( value ) {}
  ItemAttrInteger( const c3d::string_t & p, int value ) : PromptAttrBase( p ), m_value( value ) {}

  MTREE_DEFINE_DATA_SIZE_STD2(m_prompt, m_value)
  MTREE_DEFINE_DATA_TYPE(idtAttrInt)
  MTREE_PERSISTENT_DATA_OBJ2(ItemAttrInteger)
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Данные атрибута double.
             \en Data of Double attribute. \~
    \details \ru Данные атрибута double.
             \en Data of Double attribute. \~
\ingroup Base_Tools_IO
*/
// ---
class ItemAttrDouble : public PromptAttrBase
{
public:
  double m_value;
  ItemAttrDouble() : PromptAttrBase(), m_value( 0.0 ) {}
  ItemAttrDouble( double value ) : PromptAttrBase(), m_value( value ) {}
  ItemAttrDouble( const c3d::string_t & p, double value ) : PromptAttrBase( p ), m_value( value ) {}

  MTREE_DEFINE_DATA_SIZE_STD2(m_prompt, m_value)
  MTREE_DEFINE_DATA_TYPE(idtAttrDouble)
  MTREE_PERSISTENT_DATA_OBJ2(ItemAttrDouble)
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Данные атрибута string.
             \en Data of String attribute. \~
    \details \ru Данные атрибута string.
             \en Data of String attribute. \~
\ingroup Base_Tools_IO
*/
// ---
class ItemAttrString : public PromptAttrBase
{
public:
  c3d::string_t m_value;
  ItemAttrString() : PromptAttrBase(), m_value() {}
  ItemAttrString( const c3d::string_t & value ) : PromptAttrBase(), m_value( value ) {}
  ItemAttrString( const c3d::string_t & p, const c3d::string_t & value ) : PromptAttrBase( p ), m_value( value ) {}

  /// \ru Размер записи данных в поток. \en The data size in the stream.
  virtual size_t Size( writer & out ) const {
    return ( out.GetFormatVersion() >= 0x01000001L ) ? ( out.__lenWchar(m_prompt.c_str()) + out.__lenWchar(m_value.c_str()) ) : out.__lenWchar( m_value.c_str() ); }

  MTREE_DEFINE_DATA_TYPE(idtAttrString)
  MTREE_PERSISTENT_DATA_OBJ2(ItemAttrString)
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Данные атрибута int64.
             \en Data of int64 attribute. \~
    \details \ru Данные атрибута int64.
             \en Data of int64 attribute. \~
\ingroup Base_Tools_IO
*/
// ---
class ItemAttrInt64 : public PromptAttrBase
{
public:
  int64 m_value;

  ItemAttrInt64() : m_value( 0 ) {}
  ItemAttrInt64( int64 value ) :  PromptAttrBase(), m_value( value ) {}
  ItemAttrInt64( const c3d::string_t & p, int64 value ) :  PromptAttrBase( p ), m_value( value ) {}

  MTREE_DEFINE_DATA_SIZE_STD2(m_prompt, m_value)
  MTREE_DEFINE_DATA_TYPE(idtAttrInt64)
  MTREE_PERSISTENT_DATA_OBJ2(ItemAttrInt64)
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Данные атрибута MbProductInfo.
             \en Data of MbProductInfo attribute. \~
    \details \ru Данные атрибута MbProductInfo.
             \en Data of MbProductInfo attribute. \~
\ingroup Base_Tools_IO
*/
// ---
class ItemAttrProductInfo : public ItemDataString
{
public:
  ItemAttrProductInfo() : ItemDataString() {}
  ItemAttrProductInfo( c3d::string_t value ) : ItemDataString( value ) {}

  MTREE_DEFINE_DATA_TYPE( idtAttrProductInfo )

  static void Read( reader &, ItemAttrProductInfo * ) {}
  static void Write( writer &, const ItemAttrProductInfo * ) {}
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Данные атрибута Идентификатор.
             \en Data of Identifier attribute. \~
    \details \ru Данные атрибута Идентификатор.
             \en Data of Identifier attribute. \~
\ingroup Base_Tools_IO
*/
// ---
class ItemAttrIdentifier : public ItemDataBase
{
public:
  int32 m_value;

  ItemAttrIdentifier() : m_value( 0 ) {}
  ItemAttrIdentifier( int32 value ) : m_value( value ) {}

  MTREE_DEFINE_DATA_SIZE_STD(m_value)
  MTREE_DEFINE_DATA_TYPE(idtAttrIdentifier)
  MTREE_PERSISTENT_DATA_OBJ(ItemAttrIdentifier)
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Данные атрибута Цвет.
             \en Data of Color attribute. \~
    \details \ru Данные атрибута Цвет.
             \en Data of Color attribute. \~
\ingroup Base_Tools_IO
*/
// ---
class ItemAttrColor : public ItemDataBase
{
public:
  uint32 m_value;

  ItemAttrColor() : m_value( 0 ) {}
  ItemAttrColor( uint32 value ) : m_value( value ) {}

  MTREE_DEFINE_DATA_SIZE_STD(m_value)
  MTREE_DEFINE_DATA_TYPE(idtAttrColor)
  MTREE_PERSISTENT_DATA_OBJ(ItemAttrColor)
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Данные атрибута Толщина.
             \en Data of Width attribute. \~
    \details \ru Данные атрибута Толщина.
             \en Data of Width attribute. \~
\ingroup Base_Tools_IO
*/
// ---
class ItemAttrWidth : public ItemDataInteger
{
public:
  ItemAttrWidth() : ItemDataInteger() {}
  ItemAttrWidth( int value ) : ItemDataInteger( value ) {}

  MTREE_DEFINE_DATA_TYPE(idtAttrWidth)
  MTREE_PERSISTENT_DATA_OBJ(ItemAttrWidth)
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Данные атрибута Стиль.
             \en Data of Style attribute. \~
    \details \ru Данные атрибута Стиль.
             \en Data of Style attribute. \~
\ingroup Base_Tools_IO
*/
// ---
class ItemAttrStyle : public ItemDataInteger
{
public:
  ItemAttrStyle() : ItemDataInteger() {}
  ItemAttrStyle( int value ) : ItemDataInteger( value ) {}

  MTREE_DEFINE_DATA_TYPE(idtAttrStyle)
  MTREE_PERSISTENT_DATA_OBJ(ItemAttrStyle)
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Данные атрибута Селектированность.
             \en Data of Selection attribute. \~
    \details \ru Данные атрибута Селектированность.
             \en Data of Selection attribute. \~
\ingroup Base_Tools_IO
*/
// ---
class ItemAttrSelected : public ItemDataBool
{
public:
  ItemAttrSelected() : ItemDataBool() {}
  ItemAttrSelected( bool value ) : ItemDataBool( value ) {}

  MTREE_DEFINE_DATA_TYPE(idtAttrSelected)
  MTREE_PERSISTENT_DATA_OBJ(ItemAttrSelected)
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Данные атрибута Видимость.
             \en Data of Visibility attribute. \~
    \details \ru Данные атрибута Видимость.
             \en Data of Visibility attribute. \~
\ingroup Base_Tools_IO
*/
// ---
class ItemAttrVisible : public ItemDataBool
{
public:
  ItemAttrVisible() : ItemDataBool() {}
  ItemAttrVisible( bool value ) : ItemDataBool( value ) {}

  MTREE_DEFINE_DATA_TYPE(idtAttrVisible)
  MTREE_PERSISTENT_DATA_OBJ(ItemAttrVisible)
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Данные атрибута Изменённость.
             \en Data of Modification attribute. \~
    \details \ru Данные атрибута Изменённость.
             \en Data of Modification attribute. \~
\ingroup Base_Tools_IO
*/
// ---
class ItemAttrChanged : public ItemDataBool
{
public:
  ItemAttrChanged() : ItemDataBool() {}
  ItemAttrChanged( bool value ) : ItemDataBool( value ) {}

  MTREE_DEFINE_DATA_TYPE(idtAttrChanged)
  MTREE_PERSISTENT_DATA_OBJ(ItemAttrChanged)
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Данные атрибута Плотность.
             \en Data of Dencity attribute. \~
    \details \ru Данные атрибута Плотность.
             \en Data of Dencity attribute. \~
\ingroup Base_Tools_IO
*/
// ---
class ItemAttrDencity : public ItemDataDouble
{
public:
  ItemAttrDencity() : ItemDataDouble( 0 ) {}
  ItemAttrDencity( double value ) : ItemDataDouble( value ) {}

  MTREE_DEFINE_DATA_TYPE(idtAttrDencity)
  MTREE_PERSISTENT_DATA_OBJ(ItemAttrDencity)
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Данные атрибута Метка времени обновления.
             \en Data of Update timestamp attribute. \~
    \details \ru Данные атрибута Метка времени обновления.
             \en Data of Update timestamp attribute. \~
\ingroup Base_Tools_IO
*/
// ---
class ItemAttrUpdateStamp : public ItemDataBase
{
public:
  uint32 m_value;

  ItemAttrUpdateStamp() : m_value( 0 ) {}
  ItemAttrUpdateStamp( uint32 value ) : m_value( value ) {}

  MTREE_DEFINE_DATA_SIZE_STD(m_value)
  MTREE_DEFINE_DATA_TYPE(idtAttrUpdateStamp)
  MTREE_PERSISTENT_DATA_OBJ(ItemAttrUpdateStamp)
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Данные атрибута Якорь.
             \en Data of Anchor attribute. \~
    \details \ru Данные атрибута Якорь.
             \en Data of Anchor attribute. \~
\ingroup Base_Tools_IO
*/
// ---
class ItemAttrAnchor : public ItemDataBase
{
public:
  uint8 m_value;

  ItemAttrAnchor() : m_value( 0 ) {}
  ItemAttrAnchor( uint8 value ) : m_value( value ) {}

  MTREE_DEFINE_DATA_SIZE_STD(m_value)
  MTREE_DEFINE_DATA_TYPE(idtAttrAnchor)
  MTREE_PERSISTENT_DATA_OBJ(ItemAttrAnchor)
};

//----------------------------------------------------------------------------------------
/** \brief \ru Данные атрибута исполнения.
           \en Data of embodiment attribute. \~
  \details \ru Данные атрибута исполнения.
           \en Data of embodiment attribute. \~
  \ingroup Base_Tools_IO
*/
// ---
class ItemAttrEmbodiment : public ItemDataBase
{
public:
  typedef std::pair<std::pair<SimpleName, SimpleName>, std::pair<bool, bool>> EmbData; // { <name, parentName>, <isCurrent, isVariant> }
  EmbData m_value;

  ItemAttrEmbodiment() : m_value( std::pair<SimpleName, SimpleName>(0, 0), std::pair<bool, bool>(false, false) ) {}
  ItemAttrEmbodiment( const EmbData& value ) : m_value( value ) {}

  MTREE_DEFINE_DATA_SIZE_STD( m_value )
  MTREE_DEFINE_DATA_TYPE( idtAttrEmbodiment )
  static void Read( reader& in, ItemAttrEmbodiment* item ) {
    item->m_value.first.first = ::ReadSimpleName( in );  // in >> item->m_value.first.first;
    item->m_value.first.second = ::ReadSimpleName( in ); // in >> item->m_value.first.second;
    if ( in.MathVersion() >= 0x13000010L ) {
      in >> item->m_value.second.first;
      if ( in.MathVersion() >= 0x1800100BL )
        in >> item->m_value.second.second;
    }
  }
  static void Write( writer& out, const ItemAttrEmbodiment* item ) {
    ::WriteSimpleName( out, item->m_value.first.first );  // out << item->m_value.first.first;
    ::WriteSimpleName( out, item->m_value.first.second ); // out << item->m_value.first.second;
    if ( out.MathVersion() >= 0x13000010L ) {
      out << item->m_value.second.first;
      if ( out.MathVersion() >= 0x1800100BL )
        out << item->m_value.second.second;
    }
  }
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Данные атрибута Толщина оболочки.
             \en Data of Thickness attribute. \~
    \details \ru Данные атрибута Толщина оболочки.
             \en Data of Thickness attribute. \~
\ingroup Base_Tools_IO
*/
// ---
class ItemAttrThickness : public ItemDataDouble
{
  public:
  ItemAttrThickness() : ItemDataDouble( 0 ) {}
  ItemAttrThickness( double value ) : ItemDataDouble( value ) {}

  MTREE_DEFINE_DATA_TYPE( idtAttrThickness )
  MTREE_PERSISTENT_DATA_OBJ( ItemAttrThickness )
};

//----------------------------------------------------------------------------------------
/// \ru Объявление классов без данных для атрибутов.
/// \en Definition of attribute dataless classes.
//---
MTREE_ATTR_DATALESS_CLASS(ItemAttrVisual, idtAttrVisual);
MTREE_ATTR_DATALESS_CLASS(ItemAttrWireCount, idtAttrWireCount);
MTREE_ATTR_DATALESS_CLASS(ItemAttrName, idtAttrName);
MTREE_ATTR_DATALESS_CLASS(ItemAttrGeom, idtAttrGeom);
MTREE_ATTR_DATALESS_CLASS(ItemAttrStampRib, idtAttrStampRib);
MTREE_ATTR_DATALESS_CLASS(ItemAttrModelInfo, idtAttrModelInfo);
MTREE_ATTR_DATALESS_CLASS(ItemAttrPersonOrganizationInfo, idtAttrPersonOrganizationInfo);
MTREE_ATTR_DATALESS_CLASS(ItemAttrSTEPTextDescription, idtAttrSTEPTextDescription);
MTREE_ATTR_DATALESS_CLASS(ItemAttrSTEPReferenceHolder, idtAttrSTEPReferenceHolder);
MTREE_ATTR_DATALESS_CLASS(ItemAttrBinary, idtAttrBinary);
MTREE_ATTR_DATALESS_CLASS(ItemAttrInt32Vector, idtAttrInt32Vector)
MTREE_ATTR_DATALESS_CLASS(ItemAttrInt64Vector, idtAttrInt64Vector)
MTREE_ATTR_DATALESS_CLASS(ItemAttrDoubleVector, idtAttrDoubleVector)
MTREE_ATTR_DATALESS_CLASS(ItemAttrStrains, idtAttrStrains);
MTREE_ATTR_DATALESS_CLASS(ItemAttrElasticity, idtAttrElasticity);
MTREE_ATTR_DATALESS_CLASS(ItemAttrSheetFlanging, idtAttrSheetFlanging);

//----------------------------------------------------------------------------------------
/** \brief   \ru Контейнер для пользовательских данных узла дерева.
             \en A container for user data of a tree node. \~
    \details \ru Контейнер для пользовательских данных узла дерева (владеет данными).
             \en A container for user data of a tree node (owns the data). \~
    \ingroup Base_Tools_IO
*/
// ---
class MATH_CLASS UserDataMap : public MultiMap<MbeItemDataType,ItemDataBase*>
{
public:
  UserDataMap() {}
  UserDataMap( const UserDataMap& other );

  ~UserDataMap(); // \ru Владеет данными. \en Owns the data.

  /// \ru Оператор ==. \en Operator ==.
  bool operator == ( const UserDataMap& other ) const;

  /// \ru Оператор <. \en Operator <.
  bool operator < ( const UserDataMap& other ) const;

  /// \ru Оператор =. \en Operator =.
  UserDataMap& operator = ( const UserDataMap& other );
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Данные узла дерева.
\en Tree node data. \~
\details \ru Данные узла дерева. \n
\en Tree node data. \n \~
\ingroup Base_Tools_IO
*/
// ---
struct MATH_CLASS MbItemData
{
  // \ru Признак наличия локальной системы координат.
  // \en Token of local coordinate system presence.
  enum PlacementPresenceToken
  {
    ppt_No = 0x01,
    ppt_Yes = 0x02
  };

  // \ru Уникальный ID узла в дереве модели. \en Unique id in the model tree.
  // \ru Не учитывается при сравнении. \en Not considered during comparison.
  mutable size_t id;
  // \ru Тип объекта. \en Object type.
  // \ru type==st_Undefined означает, что фильтр по типу не определен.
  // \en type==st_Undefined means that filter is undefined.
  MbeSpaceType type;
  // \ru иИмя объекта. \en Object name.
  // \ru name==SYS_MAX_UINT32 означает, что фильтр по имени не определен.
  // \en name==SYS_MAX_UINT32 means that filter is undefined.
  SimpleName   name;
  // \ru gabarit.IsEmpty()==true означает, что фильтр по габариту не определен.
  // \en gabarit.IsEmpty()==true means that filter is undefined.
  MbCube       gabarit;
  // \ru Позиция записи/чтения узла. \en Position for the node writing/reading. 
  // \ru position.IsValid()==false означает, что это поле не учитывается при сравнении.
  // \en position.IsValid()==false means that this field is not considered during comparison.
  ClusterReference position;
  // \ru Локальная система координат объекта. \en Local coordinate system for the object.
  // \ru Не учитывается при сравнении. \en Not considered during comparison.
  MbPlacement3D    placement;

  // \ru Кроме обязательных данных узла, описанных выше, можно задать пользовательские данные,
  //     которые содержатся в контейнере 'properties'.
  // \en In addition to the mandatory node data, described above, it is possible to define user data,
  //     which is kept in the 'properties' container.
  // \ru Контейнер для пользовательских данных узла.
  // \en Container for user data of a node.
  UserDataMap      properties;


  // \ru Конструкторы. \en Constructors.
  MbItemData() : id( SYS_MAX_T ), type( st_Undefined ), name( SYS_MAX_UINT32 ) {}
  MbItemData( MbeSpaceType t, SimpleName n, const MbCube& c, ClusterReference& pos ) : id( SYS_MAX_T ), type( t ), name( n ), gabarit( c ), position( pos ) {}
  MbItemData( MbeSpaceType t, SimpleName n, const MbCube& c, ClusterReference& pos, const UserDataMap& prop ) :
    id( SYS_MAX_T ), type( t ), name( n ), gabarit( c ), position( pos ), properties( prop ) {}
  MbItemData( const MbItemData& data ) : id( data.id ), type( data.type ),
    name( data.name ), gabarit( data.gabarit ), position( data.position ),
    placement( data.placement ), properties( data.properties ) {}

  // \ru Признак пустых (неинициализированных) данных. \en Indicator of empty (uninitialized) data.
  bool IsEmpty() const;

  // \ru Идентичность полей-фильтров (id не важен).
  // \en Equality of filtering fields (id is irrelevant).
  bool operator == ( const MbItemData& rt ) const;

  // \ru Сравнение полей-фильтров (id не важен).
  // \en Comparison of filtering fields (id is irrelevant).
  bool operator < ( const MbItemData& rt ) const;

  // \ru Специальное сравнение габаритов для сортировки объектов.
  // \en Special comparison of bounding boxes for object sorting.
  static bool CompareGabarits( const MbCube& a, const MbCube& b );

  // \ru Запись и чтение.
  // \en Writing and reading.
  writer& operator >> ( writer & out );
  reader& operator << ( reader & out );
};

//----------------------------------------------------------------------------------------
/// \ru Чтение UserDataMap. \en UserDataMap reading.
// ---
inline reader& operator >> ( reader & in, UserDataMap& itemmap )
{
  size_t typeCount = ::ReadCOUNT( in ); // \ru Количество типов данных в наборе. \en A number of types in the map.

  if ( in.good() ) {
    for ( size_t i = 0; i < typeCount; i++ ) {
      int t;
      in >> t;
      size_t typeSize = ::ReadCOUNT( in );
      C3D_ASSERT( t < (int)idtCount ); // SKIP_SA
      if ( t < (int)idtCount ) { // SKIP_SA
        MbeItemDataType type = (MbeItemDataType)t; // \ru Тип данных. \en A data type.
        ItemDataBase* item = ItemDataBase::Create( type );
        *item << in;
        itemmap.Associate( type, item );
      }
      else { // skip unknown data
        char* buff = new char[typeSize];
        in.readBytes( buff, typeSize );
        delete[] buff;
      }

      if ( !in.good() ) {
        in.setState( io::fail );
        break;
      }
    }
  }
  return in;
}

//----------------------------------------------------------------------------------------
/// \ru Запись UserDataMap. \en UserDataMap writing.
// ---
inline writer& operator << ( writer & out, const UserDataMap& itemmap )
{
  size_t mapCount = itemmap.Count();
  ::WriteCOUNT( out, mapCount ); // \ru Количество типов данных в наборе. \en A number of types in the map.

  if ( out.good() && mapCount ) {
    UserDataMap::Iterator curIter = itemmap.First();
    while ( !curIter.Empty() ) {
      MbeItemDataType type = curIter.Key();
      ItemDataBase* item = curIter.Value();
      C3D_ASSERT( type < idtCount && item != nullptr );
      if ( type < idtCount && item != nullptr ) {
        out << (int)type; // \ru Тип данных. \en A data type.
        size_t dataSize = item->Size( out );
        ::WriteCOUNT( out, dataSize );// \ru Размер данных. \en Data size.
        *item >> out;
      }
      if ( !out.good() ) {
        out.setState( io::fail );
        break;
      }
      curIter++;
    }
  }
  return out;
}

} //namespace c3d

#endif // __MODEL_TREE_DATA_H
