////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Атрибут отбортовки листового тела.
         \en Swept flange attribute of a sheet solid. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __ATTR_FLANGE_ATTRIBUTE_H
#define __ATTR_FLANGE_ATTRIBUTE_H


#include <attr_common_attribute.h>
#include <sheet_metal_param.h>


//------------------------------------------------------------------------------
/** \brief \ru Атрибут отбортовки листового тела.
           \en Swept flange attribute of a sheet solid. \~
  \details \ru .
           \en  \n \~
  \ingroup Model_Attributes
 */
class MATH_CLASS MbSheetFlangingAttribute : public MbCommonAttribute {
protected :
  MbBendByEdgeValues  params; ///< \ru Параметры операции. \en The operation parameters.
  SPtr<MbSNameMaker>  names;  ///< \ru Именователь операции. \en An object defining names generation in the operation.

public :
  /// \ru Конструктор. \en Constructor. 
  MbSheetFlangingAttribute( const MbBendByEdgeValues & pars, const MbSNameMaker & n, const bool changeable ); 
  /// \ru Конструктор. \en Constructor. 
  MbSheetFlangingAttribute( const MbBendByEdgeValues & pars, const MbSNameMaker & n, const bool changeable, const c3d::string_t & itemPrompt ); 
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbSheetFlangingAttribute();

private:
  // \ru Конструктор копирования. \en Copy constructor. 
  MbSheetFlangingAttribute( const MbSheetFlangingAttribute & init, MbRegDuplicate * iReg );

public:
  // \ru Выдать подтип атрибута. \en Get subtype of an attribute. 
  MbeAttributeType AttributeType() const override;
  // \ru Сделать копию элемента. \en Create a copy of the element. 
  MbAttribute & Duplicate( MbRegDuplicate * = nullptr ) const override;
  // \ru Определить, являются ли объекты равными. \en Determine whether objects are equal. 
  bool    IsSame( const MbAttribute &, double accuracy ) const override;
  // \ru Инициализировать данные по присланным. \en Initialize data. 
  bool    Init( const MbAttribute & ) override;

  // \ru Выполнить действия при трансформировании владельца. \en Perform actions when transforming the owner. 
  void    OnTransformOwner( const MbAttributeContainer & owner, const MbMatrix3D &, MbRegTransform * = nullptr ) override;
  // \ru Выполнить действия при перемещении владельца. \en Perform actions when moving the owner. 
  void    OnMoveOwner( const MbAttributeContainer & owner, const MbVector3D &, MbRegTransform * = nullptr ) override;
  // \ru Выполнить действия при вращении владельца. \en Perform actions when rotating the owner. 
  void    OnRotateOwner( const MbAttributeContainer & owner, const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override;
  // \ru Выполнить действия при копировании владельца. \en Perform actions when copying the owner. 
  void    OnCopyOwner( const MbAttributeContainer & owner, MbAttributeContainer & other, MbRegDuplicate * = nullptr ) override;
  // \ru Выполнить действия при объединении владельца. \en Perform actions when merging the owner. 
  void    OnMergeOwner( const MbAttributeContainer & owner, MbAttributeContainer & other ) override;

  void    GetCharValue( TCHAR * v ) const override; // \ru Выдать строковое значение свойства. \en Get a string value of the property.
  void    GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  size_t  SetProperties( const MbProperties & ) override; // \ru Установить свойства объекта. \en Set properties of object. 
  MbePrompt GetPropertyName() override; // \ru Выдать заголовок свойства объекта. \en Get a name of object property.

          /// \ru Дать параметры операции. \en Get operation parameters.
          const  MbBendByEdgeValues & GetFlangeValues() const { return params; }
          /// \ru Дать именователь операции. \en Get an object defining a name of the operation.
          const  MbSNameMaker & GetNameMaker() const { return *names; } 

DECLARE_PERSISTENT_CLASS( MbSheetFlangingAttribute ) 
OBVIOUS_PRIVATE_COPY( MbSheetFlangingAttribute )
}; 

IMPL_PERSISTENT_OPS( MbSheetFlangingAttribute )

#endif // __ATTR_FLANGE_ATTRIBUTE_H
