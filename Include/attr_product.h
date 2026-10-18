////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Атрибуты изделий.
         \en Product attributes.
*/
////////////////////////////////////////////////////////////////////////////////

#include <attribute_item.h>
#include <math_define.h>
#include <legend.h>
#include <model_item.h>
#include <list>
#include <set>
#include <attr_common_attribute.h>
#include <tool_cstring.h>


#ifndef __ATTR_PRODUCT_H
#define __ATTR_PRODUCT_H


//------------------------------------------------------------------------------
/** \brief \ru Родительский класс атрибутов изделий.
           \en Base class of product attributes.
*/
// ---
class MATH_CLASS MbProductAttribute : public MbAttribute {
protected :
  MbProductAttribute(); // \ru Конструктор. \en Constructor.
public :
  // \ru Деструктор. \en Destructor.
  virtual ~MbProductAttribute();

public :
  MbeAttributeType AttributeFamily() const override;
  // Выдать подтип атрибута (временно). 
  //virtual MbeAttributeType AttributeType() const = 0;
  // Сделать копию элемента.
  //virtual MbAttribute & Duplicate( MbRegDuplicate * = nullptr ) const = 0;
  //virtual bool    IsSame( const MbAttribute &, double accuracy ) const = 0; // \ru Определить, являются ли объекты равными. \en Determine whether objects are equal.
  // Инициализировать данные по присланным.
  //virtual bool    Init( const MbAttribute & ) = 0;

  //virtual MbePrompt GetPropertyName() = 0;

  // Действия при изменении владельца, не связанное с другими действиями.
  void    OnChangeOwner( const MbAttributeContainer & owner ) override;  
  // Действия при конвертации владельца.
  void    OnConvertOwner( const MbAttributeContainer & owner, MbAttributeContainer & other ) override; 
  // Действия при трансформировании владельца.
  void    OnTransformOwner( const MbAttributeContainer & owner, const MbMatrix3D &, MbRegTransform * = nullptr ) override;
  // Действия при перемещении владельца.
  void    OnMoveOwner( const MbAttributeContainer & owner, const MbVector3D &, MbRegTransform * = nullptr ) override;
  // Действия при вращении владельца.
  void    OnRotateOwner( const MbAttributeContainer & owner, const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override;
  // Действия при копировании владельца.
  void    OnCopyOwner( const MbAttributeContainer & owner, MbAttributeContainer & other, MbRegDuplicate * = nullptr ) override;
  // Действия при объединении владельца.
  void    OnMergeOwner( const MbAttributeContainer & owner, MbAttributeContainer & other ) override; 
  // Действия при замене владельца.
  void    OnReplaceOwner( const MbAttributeContainer & owner, MbAttributeContainer & other ) override; 
  // Действия при разделении владельца.
  void    OnSplitOwner( const MbAttributeContainer & owner, const std::vector<MbAttributeContainer *> & others ) override; 
  // Действия при удалении владельца.
  void    OnDeleteOwner( const MbAttributeContainer & owner ) override; 

DECLARE_PERSISTENT_CLASS( MbProductAttribute )
OBVIOUS_PRIVATE_COPY( MbProductAttribute )
};

IMPL_PERSISTENT_OPS( MbProductAttribute )

//------------------------------------------------------------------------------
/** \brief \ru Сведения о лице в организации.
           \en Information related to a person and the organization he/she in.
*/
// ---
class MATH_CLASS MbPersonOrganizationInfo : public MbProductAttribute { 
  c3d::string_t personId;   ///< \ru Идентификатор лица. \en Identifier of the person.
  c3d::string_t lastName;   ///< \ru Фамилия. \en Last name.
  c3d::string_t firstName;  ///< \ru Имя. \en First name.
  std::list<c3d::string_t> middleNames;   ///< \ru Отчество/средние имена. \en Middle names.
  std::list<c3d::string_t> prefixTitles;  ///< \ru Титулы предшествующие. \en Prefix titles.
  std::list<c3d::string_t> suffixTitles;  ///< \ru Титулы завершающие. \en Suffix titles.
  c3d::string_t orgId;          ///< \ru Идентификатор организации. \en Identifier of the organization.
  c3d::string_t orgLabel;       ///< \ru Название организации. \en Label of the organization.
  c3d::string_t orgDescription; ///< \ru Описание организации. \en Description of the organization.
  std::set<c3d::string_t> roles;  ///< \ru Роли лица по отношению к изделию. \en The person's roles concerning a product.
protected :  
  // Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию.
  MbPersonOrganizationInfo( const MbPersonOrganizationInfo & );
public :
  // Конструктор без параметров для наследников.
  MbPersonOrganizationInfo();
  // Деструктор.
  virtual ~MbPersonOrganizationInfo();

public :
  // Выдать подтип атрибута (временно). 
  MbeAttributeType AttributeType() const override;
  // Сделать копию элемента.
  MbAttribute & Duplicate( MbRegDuplicate * = nullptr ) const override;
  bool    IsSame( const MbAttribute &, double accuracy ) const override; // Определить, являются ли объекты равными.
  // Инициализировать данные по присланным.
  bool    Init( const MbAttribute & ) override;
  void    GetProperties( MbProperties & ) override; // выдать свойства объекта

  MbePrompt GetPropertyName() override;

  /**
  \brief \ru Получить полное имя с префиксами и суффиксами. \en Get full name with prefixes and suffixes. \~  
  */
  c3d::string_t NameOneLine() const;

  /**
  \brief \ru Получить полное название организации. \en Get full name of organization. \~  
  */
  c3d::string_t OrganizationOneLine() const;

    /**
  \brief \ru Задать данные организации. \en Set organization's data. \~
  \param[in] oOrgId - \ru Идентификатор организации. \en Identifier of the organization. \~
  \param[in] oOrgLabel - \ru Название организации. \en Label of the organization. \~
  \param[in] oOrgDesc - \ru Описание организации. \en Description of the organization. \~
  */
  void SetOrganization( const c3d::string_t& initOrgId, const c3d::string_t& initOrgLabel, const c3d::string_t& initOrgDesc );

  /**
  \brief \ru Получить данные организации. \en Get organization data. \~
  \param[out] oOrgId - \ru Идентификатор организации. \en Identifier of the organization. \~
  \param[out] oOrgLabel - \ru Название организации. \en Label of the organization. \~
  \param[out] oOrgDesc - \ru Описание организации. \en Description of the organization. \~
  */
  void GetOrganizationDetails( c3d::string_t& oOrgId, c3d::string_t& oOrgLabel, c3d::string_t& oOrgDesc ) const;
     

  /**
  \brief \ru Задать данные лица и организации в упрощенной форме. \en Set person's and organization's simplified data. \~
  \param[in] person - \ru Фамилия автора. \en Author's second name. \~
  \param[in] organization - \ru Название организации. \en Label of the organization. \~
  */
  void SetPersonOrganization( const c3d::string_t& person, const c3d::string_t& organization );

  /// \ru Добавить роль автора. \en Add person's role.
  inline  void AddRole( const c3d::string_t& role ) { roles.insert( role ); }

  /// \ru Добавить роль автора. \en Add person's role.
  inline  void AddToRoles( const std::string& role ) { roles.insert( c3d::ToC3Dstring( role ) ); }

  /**
  \brief \ru Выдать данные лица. \en Swap person's data. \~
  \param[out] oPersonId - \ru Идентификатор лица. \en Identifier of the person. \~
  \param[out] oLast - \ru Фамилия. \en Last name. \~
  \param[out] oFirst - \ru Имя. \en First name. \~
  \param[out] oMiddles - \ru Список отчеств/средних имен. \en List of middle names. \~  
  \param[out] oPrefixes - \ru Список титулов предшествующих. \en List of prefix titles. \~  
  \param[out] oSuffixed - \ru Список титулов завершающих. \en List of suffix titles. \~
  */  
  void GetPersonDetails( c3d::string_t& oPersonId, c3d::string_t& oLast, c3d::string_t& oFirst,
                         std::list<c3d::string_t>& oMiddles,
                         std::list<c3d::string_t>& oPrefixes,
                         std::list<c3d::string_t>& oSuffixed ) const;

  /**
  \brief \ru Обменять данные лица. \en Swap person's data. \~
  \param[in] oPersonId - \ru Идентификатор лица. \en Identifier of the person. \~
  \param[in] oLast - \ru Фамилия. \en Last name. \~
  \param[in] oFirst - \ru Имя. \en First name. \~
  \param[in] oMiddles - \ru Список отчеств/средних имен. \en List of middle names. \~  
  \param[in] oPrefixes - \ru Список титулов предшествующих. \en List of prefix titles. \~  
  \param[in] oSuffixed - \ru Список титулов завершающих. \en List of suffix titles. \~
  */  
  void SwapPersonDetails( c3d::string_t& oPersonId, c3d::string_t& oLast, c3d::string_t& oFirst,
                  std::list<c3d::string_t>& oMiddles,
                  std::list<c3d::string_t>& oPrefixes,
                  std::list<c3d::string_t>& oSuffixed );

  
  /// \ru Получить роли автора. \en Get person's roles.
  void GetPersonRoles( std::vector<c3d::string_t>& ) const;  

private:
  MbPersonOrganizationInfo & operator = ( const MbPersonOrganizationInfo & ); // forbidden

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbPersonOrganizationInfo ) // Атрибуты писать ни к чему, они создаются только для конвертирования
};

IMPL_PERSISTENT_OPS( MbPersonOrganizationInfo )

//------------------------------------------------------------------------------
/** \brief \ru Данные об изделии. \en Product data.
*/
// ---
class MATH_CLASS MbProductInfo : public MbProductAttribute 
{
  c3d::string_t id;   ///< \ru Идентификатор. \en Identifier.
  c3d::string_t name; ///< \ru Название. \en Name.
  c3d::string_t description;  ///< \ru Описание. \en Description.
  bool          isAssembly; ///< \ru Является ли сборочной единицей. \en If the product is an assembly.

protected :  
  // Объявление (перегрузка) конструктора копирования без реализации, чтобы не было копирования по умолчанию.
  MbProductInfo( const MbProductInfo & );
public :
  MbProductInfo( c3d::StringTCRef initId, c3d::StringTCRef initName, c3d::StringTCRef initDesc, bool isAssm );

  MbProductInfo( const TCHAR* initId, const TCHAR* initName, const TCHAR* initDesc, bool isAssm );

  MbProductInfo( bool isAssm, const std::string & initId, const std::string & initName, const std::string & initDesc );
  // Деструктор.
  virtual ~MbProductInfo();

public :
  // Выдать подтип атрибута (временно). 
  MbeAttributeType AttributeType() const override;
  // Сделать копию элемента.
  MbAttribute & Duplicate( MbRegDuplicate * = nullptr ) const override;
  bool    IsSame( const MbAttribute &, double accuracy ) const override; // Определить, являются ли объекты равными.
  // Инициализировать данные по присланным.
  bool    Init( const MbAttribute & ) override;
  void    GetProperties( MbProperties & ) override; // выдать свойства объекта
  size_t  SetProperties( const MbProperties & ) override; // Установить свойства объекта.

  MbePrompt GetPropertyName() override;

          const c3d::string_t& GetId() const; ///< \ru Получить идентификатор. \en Get id.

          const c3d::string_t& GetName() const; ///< \ru Получить наименование. \en Get name.

          const c3d::string_t& GetDescription() const; ///< \ru Получить описание. \en Get description.

          /// \ru Получить данные. \en Get data.
          void    GetData( c3d::string_t & oId, c3d::string_t & oName, c3d::string_t & oDesc ) const;

  /// \ru Задать название. \en Set the name of the product.
          void    SetNameC3D( const c3d::string_t& oName );

  /// \ru Задать наименование. \en Set the designation of the product.
          void    SetId( const std::string& oId );
  /// \ru Задать название. \en Set the name of the product.
          void    SetName( const std::string& iName );
  /// \ru Задать описание. \en Set the description of the product.
          void    SetDescription( const std::string& oDesc );

          /// \ru Является ли изделие сборочной единицей. \en If the product is an assembly.
          bool IsAssembly() const;

private:
  MbProductInfo & operator = ( const MbProductInfo & );

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbProductInfo ) // Атрибуты писать ни к чему, они создаются только для конвертирования
};

IMPL_PERSISTENT_OPS( MbProductInfo )

#endif // __ATTR_PRODUCT_H
