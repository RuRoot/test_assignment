////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Пользовательские атрибуты.
         \en User attributes. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __ATTR_USER_ATTRIBUT_H
#define __ATTR_USER_ATTRIBUT_H


#include <attribute_item.h>
#include <io_base.h>
#include <io_memory_buffer.h>
#include <math_define.h>
#include <attr_registry.h>
#include <tool_cstring.h>
#include <tool_mutex.h>

class  MbExternalAttribute;
class  MbUserAttribute;
class  MbFixAttrSet;


//------------------------------------------------------------------------------
/** \brief \ru Интерфейс определения атрибута.
           \en Attribute definition interface. \~ 
  \details \ru Интерфейс определения атрибута. Определение атрибута - объект используемый 
    для преобразования пользовательских внесистемных атрибутов в пользовательские системные, 
    а так же для разборки пользовательских системных атрибутов
    на составные части - другие атрибуты системные атрибуты, и обратной сборки.
           \en Attribute definition interface. Attribute definition - the object used 
    for converting user external attributes to user system attributes 
    and for a disassembly of user system attributes
    to their components - other system attributes, and for reassembly. \~
  \ingroup Model_Attributes
 */
class IAttrDefinition 
{
public:
  virtual ~IAttrDefinition() = default;

  /// \ru Преобразовать из пользовательского в "системный". \en Convert user attribute to "system" one. 
  virtual MbUserAttribute * ReduceUserAttrib ( const MbExternalAttribute & source ) = 0;

  /// \ru Преобразовать из "системного" в пользовательский. \en Convert "system" attribute to user one. 
  virtual MbExternalAttribute *  AdvanceUserAttrib( const MbUserAttribute & source ) = 0;

  /// \ru "Разобрать" на составляющие атрибуты. \en Disassemble on attributes. 
  virtual MbFixAttrSet *    DisassembleUserAttrib( const MbExternalAttribute & source ) = 0;

  /// \ru "Собрать" из составляющих атрибутов. \en Reassemble from attributes. 
  virtual bool              ReassembleUserAttrib ( const MbFixAttrSet & source, MbExternalAttribute & target ) = 0;
};


//------------------------------------------------------------------------------
/** \brief \ru Шаблон "определения" пользовательского атрибута.
           \en A template of user attribute definition. \~
  \details \ru Шаблонный класс "Определения" пользовательского атрибута - используется для создания
  стандартных определений, с предопределенным функционалом.
           \en Template class "Definition" of user attribute - used for creation
  of standard definitions with predefined functionality. \~ 
  \ingroup Model_Attributes
 */
template <typename AttrClass>
class UserAttrDefinition : public IAttrDefinition
{
public:
  /// \ru Преобразовать из пользовательского в "системный". \en Convert user attribute to "system" one. 
  virtual MbUserAttribute * ReduceUserAttrib ( const MbExternalAttribute & source );

  /// \ru Преобразовать из "системного" в пользовательский. \en Convert "system" attribute to user one. 
  virtual MbExternalAttribute * AdvanceUserAttrib( const MbUserAttribute & source );

  /// \ru "Разобрать" на составляющие атрибуты. \en Disassemble on attributes. 
  virtual MbFixAttrSet * DisassembleUserAttrib( const MbExternalAttribute & source );

  /// \ru "Собрать" из составляющих атрибутов. \en Reassemble from attributes. 
  virtual bool ReassembleUserAttrib( const MbFixAttrSet & source, MbExternalAttribute & target );
};


//------------------------------------------------------------------------------
/** \brief \ru Пользовательский системный атрибут.
           \en User system attribute. \~
  \details \ru Пользовательский системный атрибут. \n
           \en User system attribute. \n \~
  \ingroup Model_Attributes
 */
class MATH_CLASS MbUserAttribute : public MbAttribute, public MbSyncItem {
  typedef std::unique_ptr<membuf> UniqueMembufPtr;
protected :
  MbUserAttribType  userType_;  ///< \ru Тип пользовательского атрибута. \en Type of user attribute. 
  c3d::string_t     prompt_;    ///< \ru Строка описания. \en String of description. 
private:
  SPtr<MbExternalAttribute> extAttr;
  mutable UniqueMembufPtr   userBuf;

private: // public: // You must inherit from MbExternalAttribute only!!!
  /// \ru Конструктор. \en Constructor. 
  MbUserAttribute( const TCHAR *, const MbUserAttribType & );

public:
  MbeAttributeType AttributeFamily() const override; // \ru Дать тип атрибута. \en Get type of an attribute. 
  MbeAttributeType AttributeType() const override; // \ru Выдать подтип атрибута. \en Get subtype of an attribute.  

  /// \ru Выдать подтип пользовательского атрибута по пользовательскому типу. \en Get subtype of an user attribute by user-defined type. 
  static  MbeAttributeType AttributeType( const MbUserAttribType & userType ); 

  MbAttribute & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента. \en Create a copy of the element.
  bool    IsSame( const MbAttribute &, double accuracy ) const override; // \ru Определить, являются ли объекты равными. \en Determine whether objects are equal. 
  bool    Init( const MbAttribute & ) override; // \ru Инициализировать данные по присланным. \en Initialize data by given attribute. 

  // \ru Выполнить действия при изменении владельца не связанное с другими действиями \en Perform actions which are not associated with other actions when changing the owner 
  void    OnChangeOwner( const MbAttributeContainer & owner ) override;  
  // \ru Выполнить действия при конвертации владельца \en Perform actions when converting the owner 
  void    OnConvertOwner( const MbAttributeContainer & owner, MbAttributeContainer & other ) override; 
  // \ru Выполнить действия при трансформировании владельца \en Perform actions when transforming the owner 
  void    OnTransformOwner( const MbAttributeContainer & owner, const MbMatrix3D &, MbRegTransform * = nullptr ) override;
  // \ru Выполнить действия при перемещении владельца. \en Perform actions when moving the owner. 
  void    OnMoveOwner( const MbAttributeContainer & owner, const MbVector3D &, MbRegTransform * = nullptr ) override;
  // \ru Выполнить действия при вращении владельца. \en Perform actions when rotating the owner. 
  void    OnRotateOwner( const MbAttributeContainer & owner, const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override;
  // \ru Выполнить действия при копировании владельца \en Perform actions when copying the owner. 
  void    OnCopyOwner( const MbAttributeContainer & owner, MbAttributeContainer & other, MbRegDuplicate * = nullptr ) override;
  // \ru Выполнить действия при объединении владельца \en Perform actions when merging the owner. 
  void    OnMergeOwner( const MbAttributeContainer & owner, MbAttributeContainer & other ) override; 
  // \ru Выполнить действия при замене владельца. \en Perform actions when replacing the owner. 
  void    OnReplaceOwner( const MbAttributeContainer & owner, MbAttributeContainer & other ) override; 
  // \ru Выполнить действия при разделении владельца. \en Perform actions when splitting the owner. 
  void    OnSplitOwner( const MbAttributeContainer & owner, const std::vector<MbAttributeContainer *> & others ) override; 
  // \ru Выполнить действия при удалении владельца. \en Perform actions when deleting the owner. 
  void    OnDeleteOwner( const MbAttributeContainer & owner ) override; 

  /// \ru Выдать подсказку. \en Get a hint. 
  const   TCHAR * GetPrompt() const;
  /// \ru Выдать идентификатор хранимого атрибута. \en Get identifier of stored attribute. 
  void            GetUserAttribId( MbUserAttribType & attrId ) const;

          /// \ru Установить пользовательские данные. \en Set user data.
          void    SetUserData( const char * extAttrMemory );
          /// \ru Установить пользовательские данные. \en Set user data.
          void    SetUserData( const std::vector<char> & extAttrData );
          /// \ru Получить пользовательские данные. \en Get user data. 
          bool    GetUserData( membuf & ) const;
          /// \ru Создать пользовательский внесистемный атрибут по пользовательским данным. \en Make a user external attribute using user data. 
          bool    MakeExternalAttribute( bool keepExisting );
          /// \ru Обновить пользовательские данные по внесистемному атрибуту пользователя. \en Update user data using the user external attribute. 
          bool    UpdateByExternalAttribute() const;

          /// \ru Выдать пользовательский внесистемный атрибут. \en Get a user external attribute.
  const   MbExternalAttribute * GetExternalAttribute() const { return extAttr; }
          /// \ru Установить пользовательский внесистемный атрибут. \en Set a user external attribute.
          bool    SetExternalAttribute( MbExternalAttribute * );
          /// \ru Установить пользовательский внесистемный атрибут (его копию). \en Set a user external attribute (copy).
          void    SetExternalAttribute( const MbExternalAttribute & );

  void    GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  size_t  SetProperties( const MbProperties & ) override; // \ru Установить свойства объекта. \en Set properties of the object. 
  MbePrompt GetPropertyName() override; // \ru Выдать заголовок свойства объекта. \en Get a name of object property. 

  //warning: dependent nested name specifier 'UserAttrDefinition<AttrClass>::' for friend class declaration is not supported; turning off access control for 'MbUserAttribute'
  //template <typename AttrClass>
  //friend MbUserAttribute * UserAttrDefinition<AttrClass>::ReduceUserAttrib( const MbExternalAttribute & );

  template <typename AttrClass>
  friend class UserAttrDefinition;
  
protected:
  virtual ~MbUserAttribute(); // Use AddRef/Release or smart pointer SPtr<MbAttribute> to destruct it correctly.

  DECLARE_PERSISTENT_CLASS_NEW_DEL_BASE( MbUserAttribute, MATH_FUNC_EX )
OBVIOUS_PRIVATE_COPY( MbUserAttribute )
}; 

IMPL_PERSISTENT_OPS( MbUserAttribute )


//------------------------------------------------------------------------------
/** \brief \ru Пользовательский внесистемный атрибут - базовый класс.
           \en User external attribute - the base class. \~
  \details \ru Пользовательский внесистемный атрибут - базовый класс. \n
           \en User external attribute - the base class. \n \~
  \ingroup Model_Attributes
 */
class MATH_CLASS MbExternalAttribute : public MbAttribute
{
public :
  /// \ru Конструктор. \en Constructor. 
  MbExternalAttribute(); 
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbExternalAttribute();

  MbeAttributeType AttributeFamily() const override; // \ru Дать тип атрибута. \en Get type of an attribute. 
  MbeAttributeType AttributeType() const override; // \ru Выдать подтип атрибута. \en Get subtype of an attribute. 
  /// \ru Выдать подтип атрибута. \en Get subtype of an attribute. 
  virtual MbUserAttribType AttrTypeEx() const = 0;

  virtual MbAttribute & Duplicate( MbRegDuplicate * = nullptr ) const override = 0; // \ru Сделать копию элемента. \en Create a copy of the element.
  virtual bool    IsSame( const MbAttribute &, double accuracy ) const override = 0; // \ru Определить, являются ли объекты равными. \en Determine whether objects are equal. 
  virtual bool    Init( const MbAttribute & ) override = 0; // \ru Инициализировать данные по присланным. \en Initialize data. 

  // \ru Выполнить действия при изменении владельца, не связанное с другими действиями. \en Perform actions which are not associated with other actions when changing the owner. 
  virtual void    OnChangeOwner( const MbAttributeContainer & owner ) override;
  // \ru Выполнить действия при конвертации владельца. \en Perform actions when converting the owner.
  virtual void    OnConvertOwner( const MbAttributeContainer & owner, MbAttributeContainer & other ) override; 
  // \ru Выполнить действия при трансформировании владельца. \en Perform actions when transforming the owner. 
  virtual void    OnTransformOwner( const MbAttributeContainer & owner, const MbMatrix3D &, MbRegTransform * = nullptr ) override;
  // \ru Выполнить действия при перемещении владельца. \en Perform actions when moving the owner. 
  virtual void    OnMoveOwner( const MbAttributeContainer & owner, const MbVector3D &, MbRegTransform * = nullptr ) override;
  // \ru Выполнить действия при вращении владельца. \en Perform actions when rotating the owner. 
  virtual void    OnRotateOwner( const MbAttributeContainer & owner, const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override;
  // \ru Выполнить действия при копировании владельца. \en Perform actions when copying the owner. 
  virtual void    OnCopyOwner( const MbAttributeContainer & owner, MbAttributeContainer & other, MbRegDuplicate * = nullptr ) override;
  // \ru Выполнить действия при объединении владельца. \en Perform actions when merging the owner. 
  virtual void    OnMergeOwner( const MbAttributeContainer & owner, MbAttributeContainer & other ) override; 
  // \ru Выполнить действия при замене владельца. \en Perform actions when replacing the owner. 
  virtual void    OnReplaceOwner( const MbAttributeContainer & owner, MbAttributeContainer & other ) override; 
  // \ru Выполнить действия при разделении владельца. \en Perform actions when splitting the owner. 
  virtual void    OnSplitOwner( const MbAttributeContainer & owner, const std::vector<MbAttributeContainer *> & others ) override; 
  // \ru Выполнить действия при удалении владельца. \en Perform actions when deleting the owner. 
  virtual void    OnDeleteOwner( const MbAttributeContainer & owner ) override; 

  virtual void    GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  virtual size_t  SetProperties( const MbProperties & ) override; // \ru Установить свойства объекта. \en Set properties of the object. 
  MbePrompt GetPropertyName() override; // \ru Выдать заголовок свойства объекта. \en Get a name of object property. 

OBVIOUS_PRIVATE_COPY( MbExternalAttribute )
}; 


//------------------------------------------------------------------------------
/** \brief \ru Фиксированный набор атрибутов
           \en Fixed set of attributes. \~
  \details \ru Набор атрибутов, состав которого нельзя изменить, но никто не запрещает 
  менять значение самих атрибутов.
           \en A set of attributes the structure of which cannot be changed, but it is possible 
  to change values of the attributes. \~
  \ingroup Model_Attributes
*/
class MATH_CLASS MbFixAttrSet
{
private:
  MbUserAttribType  userAttrId; ///< \ru Идентификатор соответствующего пользовательского атрибута. \en Identifier of the corresponding external attribute. 
  c3d::AttrVector   attributes; ///< \ru Атрибуты. \en Attributes. 

private:
  /// \ru Конструктор. \en Constructor. 
  MbFixAttrSet( c3d::AttrVector & );
public:
  /// \ru Деструктор. \en Destructor. 
  ~MbFixAttrSet() { std::for_each( attributes.begin(), attributes.end(), ReleaseItem<MbAttribute> ); }

public:
  /// \ru Выдать идентификатор атрибута. \en Get attribute identifier. 
  const MbUserAttribType &  GetUserAttrId() const { return userAttrId;  }
  /// \ru Выдать атрибуты. \en Get attributes. 
  const MbAttribute *       GetAttribute( size_t k ) const { return ((k < attributes.size()) ? attributes[k] : nullptr);  }
  // \ru Выдать количество атрибутов. \en Get the number of attributes. 
  size_t                    AttributesCount() const { return attributes.size(); }

  friend class MbExternalAttribute;

OBVIOUS_PRIVATE_COPY( MbFixAttrSet )
};


//------------------------------------------------------------------------------
/** \brief \ru Шаблон явления "Определения" пользовательского атрибута.
           \en A template of "Definition" phenomenon of user attribute. \~
  \ingroup Model_Attributes
 */
template <typename AttrDefClass>
class UserAttrDefinitionInstance : public AttrDefInstance, public MbSyncItem
{
private:
  AttrDefClass * attrDef; ///< \ru "Определение" пользовательского атрибута. \en "Definition" of user attribute. 

public:
  /// \ru Конструктор. \en Constructor. 
  UserAttrDefinitionInstance( const MbUserAttribType & );
  /// \ru Деструктор. \en Destructor. 
  virtual ~UserAttrDefinitionInstance();

public:
  // \ru Дать "определение" пользовательского атрибута. \en Get a "definition" of user attribute. 
  virtual IAttrDefinition * GetAttrDefinition();
};


//------------------------------------------------------------------------------
/// \ru Преобразовать из пользовательского в "системный". \en Convert user attribute to "system" one. 
// ---
template <typename AttrClass>
MbUserAttribute * UserAttrDefinition<AttrClass>::ReduceUserAttrib( const MbExternalAttribute & source )
{
  MbUserAttribType attrId( source.AttrTypeEx() );

  MbUserAttribute * resAttr = new MbUserAttribute( _T("AttrClass"), attrId );
  resAttr->InitActions( source );
  {
    const char * charBuf = nullptr;
    size_t memLen = 0;
    {
      membuf memBuf;
      {
        const AttrClass * attrPtr = static_cast<const AttrClass *>(&source);
        writer::writer_ptr out = writer::CreateMemWriter( memBuf, io::out );
        if ( out->good() ) 
          *out << attrPtr;
      }
      memBuf.closeBuff(); // before memBuf.getMemLen!!!

      memLen = memBuf.getMemLen();
      charBuf = new char[memLen]; 
      memBuf.toMemory( charBuf, memLen );
    }
    resAttr->SetUserData( charBuf );
    delete [] charBuf;    
  }

  return resAttr;
}


//------------------------------------------------------------------------------
/// \ru Преобразовать из "системного" в пользовательский. \en Convert "system" attribute to user one. 
// ---
template <typename AttrClass>
MbExternalAttribute * UserAttrDefinition<AttrClass>::AdvanceUserAttrib( const MbUserAttribute & source )
{
  AttrClass * resAttr = nullptr;
  MbUserAttribType attrId;
  source.GetUserAttribId( attrId );
  {
    membuf memBuf;
    {
      bool canRead = true;
      if ( !source.GetUserData( memBuf ) ) {
        canRead = false;
        if ( source.UpdateByExternalAttribute() ) {
          canRead = source.GetUserData( memBuf );
        }
      }
      if ( canRead ) {
        reader::reader_ptr in = reader::CreateMemReader( memBuf, io::in );
        if ( in->good() ) 
          *in >> resAttr;
      }
    }
    memBuf.closeBuff();
  }
  return resAttr;
}


//------------------------------------------------------------------------------
/// \ru "Разобрать" на составляющие атрибуты. \en Disassemble on attributes. 
// ---
template <typename AttrClass>
MbFixAttrSet * UserAttrDefinition<AttrClass>::DisassembleUserAttrib( const MbExternalAttribute & /*source*/ ) {
  return nullptr;
}


//------------------------------------------------------------------------------
/// \ru "Собрать" из составляющих атрибутов. \en Reassemble from attributes. 
// ---
template <typename AttrClass>
bool UserAttrDefinition<AttrClass>::ReassembleUserAttrib( const MbFixAttrSet & /*source*/, MbExternalAttribute & /*target*/ ) {
  return false;
}


//------------------------------------------------------------------------------
// \ru Конструктор. \en Constructor. 
// ---
template <typename AttrDefClass>
UserAttrDefinitionInstance<AttrDefClass>::UserAttrDefinitionInstance( const MbUserAttribType & type ) 
  : AttrDefInstance( type )
  , attrDef( nullptr )
{
}


//------------------------------------------------------------------------------
// \ru Деструктор. \en Destructor. 
// ---
template <typename AttrDefClass>
UserAttrDefinitionInstance<AttrDefClass>::~UserAttrDefinitionInstance() 
{ 
  if ( attrDef != nullptr )
    delete attrDef; 
} 


//------------------------------------------------------------------------------
// \ru Дать "определение" пользовательского атрибута. \en Get a "definition" of user attribute. 
// ---
template <typename AttrDefClass>
IAttrDefinition * UserAttrDefinitionInstance<AttrDefClass>::GetAttrDefinition() 
{ 
  if ( attrDef == nullptr ) {
    ScopedLock ll( GetLock() );
    attrDef = new AttrDefClass();
  }
  return attrDef; 
}  


#endif // __ATTR_USER_ATTRIBUT_H
