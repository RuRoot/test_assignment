////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Подтип обобщенные атрибуты.
         \en Common attributes subtype. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __ATTR_COMMON_ATTRIBUE_H
#define __ATTR_COMMON_ATTRIBUE_H


#include <attribute_item.h>
#include <mb_rect1d.h>


//------------------------------------------------------------------------------
/** \brief \ru Обобщенный атрибут - базовый класс.
           \en Common attribute - the base class. \~
  \details \ru Обобщенный атрибут - базовый класс. \n
           \en Common attribute - the base class. \n \~
  \ingroup Model_Attributes
 */
class MATH_CLASS MbCommonAttribute : public MbAttribute {
protected :
  c3d::string_t prompt_;    ///< \ru Строка описания. \en String of description. 
  bool          changeable; ///< \ru Признак редактируемости. \en Attribute of editability. 

protected :
  /// \ru Конструктор. \en Constructor. 
  MbCommonAttribute( const MbCommonAttribute & );
  /// \ru Конструктор. \en Constructor. 
  MbCommonAttribute( const c3d::string_t & prompt, const bool change );
  /// \ru Конструктор. \en Constructor. 
  explicit MbCommonAttribute( const bool change );
private:
  /// \ru Оператор присваивания (не реализован. \en Assignment operator (not implemented). 
  MbCommonAttribute & operator = ( const MbCommonAttribute & );
public:
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbCommonAttribute();
public :
  MbeAttributeType AttributeFamily() const override;    // \ru Выдать тип атрибута. \en Get attribute type.
  //virtual MbeAttributeType AttributeType() const = 0; // \ru Выдать подтип атрибута. \en Get subtype of an attribute.
  //virtual MbAttribute & Duplicate( MbRegDuplicate * = nullptr ) const = 0; // \ru Сделать копию элемента. \en Create a copy of the element.
  bool    IsSame( const MbAttribute &, double accuracy ) const override; // \ru Определить, являются ли объекты равными. \en Determine whether objects are equal. 
  //virtual bool    Init( const MbAttribute & ) = 0; // \ru Инициализировать данные по присланным. \en Initialize data.

  // \ru Выполнить действия при изменении владельца, не связанное с другими действиями. \en Perform actions which are not associated with other actions when changing the owner. 
  void    OnChangeOwner( const MbAttributeContainer & owner ) override;  
  // \ru Выполнить действия при конвертации владельца. \en Perform actions when converting the owner. 
  void    OnConvertOwner( const MbAttributeContainer & owner, MbAttributeContainer & other ) override; 
  // \ru Выполнить действия при трансформировании владельца. \en Perform actions when transforming the owner. 
  void    OnTransformOwner( const MbAttributeContainer & owner, const MbMatrix3D &, MbRegTransform * = nullptr ) override;
  // \ru Выполнить действия при перемещении владельца. \en Perform actions when moving the owner. 
  void    OnMoveOwner( const MbAttributeContainer & owner, const MbVector3D &, MbRegTransform * = nullptr ) override;
  // \ru Выполнить действия при вращении владельца. \en Perform actions when rotating the owner. 
  void    OnRotateOwner( const MbAttributeContainer & owner, const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override;
  // \ru Выполнить действия при копировании владельца. \en Perform actions when copying the owner. 
  void    OnCopyOwner( const MbAttributeContainer & owner, MbAttributeContainer & other, MbRegDuplicate * = nullptr ) override;
  // \ru Выполнить действия при объединении владельца. \en Perform actions when merging he owner. 
  void    OnMergeOwner( const MbAttributeContainer & owner, MbAttributeContainer & other ) override; 
  // \ru Выполнить действия при замене владельца. \en Perform actions when replacing the owner. 
  void    OnReplaceOwner( const MbAttributeContainer & owner, MbAttributeContainer & other ) override; 
  // \ru Выполнить действия при разделении владельца. \en Perform actions when splitting the owner. 
  void    OnSplitOwner( const MbAttributeContainer & owner, const std::vector<MbAttributeContainer *> & others ) override; 
  // \ru Выполнить действия при удалении владельца. \en Perform actions when deleting the owner. 
  void    OnDeleteOwner( const MbAttributeContainer & owner ) override; 

  virtual void    GetCharValue( TCHAR * v ) const = 0; // \ru Выдать строковое значение свойства. \en Get a string value of the property. 
  void    GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта. \en Get properties of the object.
  size_t  SetProperties( const MbProperties & ) override; // \ru Установить свойства объекта. \en Set properties of object.
  MbePrompt GetPropertyName() override; // \ru Выдать заголовок свойства объекта. \en Get a name of object property. 

  /** \brief \ru Выдать подсказку атрибута. \en Get a prompt of attribute.
      \details \ru Строковое значение, которое может быть использовано, как совего рода тэг, имя или пометка атрибута.
               \en String value which can be used as some kind of tag, name or label of an attribute.
  */
  const c3d::string_t & GetPrompt() const;
  /// \ru Выдать признак изменяемости. \en Get an attribute of changeability. 
  bool IsChangeable() const;

DECLARE_PERSISTENT_CLASS( MbCommonAttribute ) 
}; 

IMPL_PERSISTENT_OPS( MbCommonAttribute )


//------------------------------------------------------------------------------
/** \brief \ru bool атрибут.
           \en Bool attribute. \~
  \details \ru bool атрибут. \n
           \en Bool attribute. \n \~
  \ingroup Model_Attributes
 */
class MATH_CLASS MbBoolAttribute : public MbCommonAttribute {
private:
  bool value_; ///< \ru Значение. \en The value. 

public:
  /// \ru Конструктор. \en Constructor. 
  explicit MbBoolAttribute( const c3d::string_t & prompt, bool change, bool initValue );
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbBoolAttribute();

public:
  MbeAttributeType  AttributeType() const override; // \ru Выдать подтип атрибута. \en Get subtype of an attribute. 
  void              GetCharValue( TCHAR * v ) const override; // \ru Выдать строковое значение свойства. \en Get a string value of the property. 
  MbAttribute &     Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента. \en Create a copy of the element.
  bool              IsSame( const MbAttribute &, double accuracy ) const override; // \ru Определить, являются ли объекты равными. \en Determine whether objects are equal. 
  bool              Init( const MbAttribute & ) override; // \ru Инициализировать данные по присланным. \en Initialize data. 
  void              GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  size_t            SetProperties( const MbProperties & ) override; // \ru Установить свойства объекта. \en Set properties of object. 
  MbePrompt         GetPropertyName() override; // \ru Выдать заголовок свойства объекта. \en Get a name of object property. 
public:
  bool              GetValue() const; ///< \ru Выдать значение. \en Get a value. 
  bool              SetValue( bool ); ///< \ru Установить новое значение. \en Set new value. 

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbBoolAttribute )
OBVIOUS_PRIVATE_COPY( MbBoolAttribute )
};

IMPL_PERSISTENT_OPS( MbBoolAttribute )


//------------------------------------------------------------------------------
/** \brief \ru int атрибут.
           \en Int attribute. \~
  \details \ru int атрибут. \n
           \en Int attribute. \n \~
  \ingroup Model_Attributes
*/
class MATH_CLASS MbIntAttribute : public MbCommonAttribute {
private:
  int value_; ///< \ru Значение. \en The value. 

public:
  /// \ru Конструктор. \en Constructor. 
  explicit MbIntAttribute( const c3d::string_t & prompt, bool change, int initValue );
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbIntAttribute();

public:
  MbeAttributeType  AttributeType() const override; // \ru Выдать подтип атрибута. \en Get subtype of an attribute. 
  void              GetCharValue( TCHAR * v ) const override; // \ru Выдать строковое значение свойства. \en Get a string value of the property. 
  MbAttribute &     Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента. \en Create a copy of the element.
  bool              IsSame( const MbAttribute &, double accuracy ) const override; // \ru Определить, являются ли объекты равными. \en Determine whether objects are equal. 
  bool              Init( const MbAttribute & ) override; // \ru Инициализировать данные по присланным. \en Initialize data. 
  void              GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  size_t            SetProperties( const MbProperties & ) override; // \ru Установить свойства объекта. \en Set properties of object. 
  MbePrompt         GetPropertyName() override; // \ru Выдать заголовок свойства объекта. \en Get a name of object property. 
public:
  int               GetValue() const; ///< \ru Выдать значение. \en Get a value. 
  bool              SetValue( int );  ///< \ru Установить новое значение. \en Set new value. 

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbIntAttribute ) 
OBVIOUS_PRIVATE_COPY( MbIntAttribute )
};

IMPL_PERSISTENT_OPS( MbIntAttribute )


//------------------------------------------------------------------------------
/** \brief \ru int64 атрибут.
           \en Int64 attribute. \~
  \details \ru int64 атрибут. \n
           \en Int64 attribute. \n \~
  \ingroup Model_Attributes
*/
class MATH_CLASS MbInt64Attribute : public MbCommonAttribute {
private:
  int64 value_; ///< \ru Значение. \en The value. 

public:
  /// \ru Конструктор. \en Constructor. 
  explicit MbInt64Attribute( const c3d::string_t & prompt, bool change, int64 initValue );
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbInt64Attribute();

public:
  MbeAttributeType  AttributeType() const override; // \ru Выдать подтип атрибута. \en Get subtype of an attribute. 
  void              GetCharValue( TCHAR * v ) const override; // \ru Выдать строковое значение свойства. \en Get a string value of the property. 
  MbAttribute &     Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента. \en Create a copy of the element.
  bool              IsSame( const MbAttribute &, double accuracy ) const override; // \ru Определить, являются ли объекты равными. \en Determine whether objects are equal. 
  bool              Init( const MbAttribute & ) override; // \ru Инициализировать данные по присланным. \en Initialize data. 
  void              GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  size_t            SetProperties( const MbProperties & ) override; // \ru Установить свойства объекта. \en Set properties of object. 
  MbePrompt         GetPropertyName() override; // \ru Выдать заголовок свойства объекта. \en Get a name of object property. 
public:
  int64             GetValue() const;   ///< \ru Выдать значение. \en Get a value. 
  bool              SetValue( int64 );  ///< \ru Установить новое значение. \en Set new value. 

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbInt64Attribute ) 
OBVIOUS_PRIVATE_COPY( MbInt64Attribute )
};

IMPL_PERSISTENT_OPS( MbInt64Attribute )


//------------------------------------------------------------------------------
/** \brief \ru double атрибут.
           \en Double attribute. \~
  \details \ru double атрибут. \n
           \en Double attribute. \n \~
  \ingroup Model_Attributes
 */
class MATH_CLASS MbDoubleAttribute : public MbCommonAttribute {
private:
  double value_; ///< \ru Значение. \en The value. 

public:
  /// \ru Конструктор. \en Constructor. 
  explicit MbDoubleAttribute( const c3d::string_t & prompt, bool change, double initValue );
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbDoubleAttribute();

public:
  MbeAttributeType  AttributeType() const override; // \ru Выдать подтип атрибута. \en Get subtype of an attribute. 
  void              GetCharValue( TCHAR * v ) const override; // \ru Выдать строковое значение свойства. \en Get a string value of the property. 
  MbAttribute &     Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента. \en Create a copy of the element.
  bool              IsSame( const MbAttribute &, double accuracy ) const override; // \ru Определить, являются ли объекты равными. \en Determine whether objects are equal. 
  bool              Init( const MbAttribute & ) override; // \ru Инициализировать данные по присланным. \en Initialize data. 
  void              GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  size_t            SetProperties( const MbProperties & ) override; // \ru Установить свойства объекта. \en Set properties of object. 
  MbePrompt         GetPropertyName() override; // \ru Выдать заголовок свойства объекта. \en Get a name of object property. 
public:
  double            GetValue() const;       ///< \ru Выдать значение. \en Get a value. 
  bool              SetValue( double val ); ///< \ru Установить новое значение. \en Set new value. 

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbDoubleAttribute ) 
OBVIOUS_PRIVATE_COPY( MbDoubleAttribute )
};

IMPL_PERSISTENT_OPS( MbDoubleAttribute )


//------------------------------------------------------------------------------
/** \brief \ru String атрибут.
           \en String attribute. \~
  \details \ru String атрибут. \n
           \en String attribute. \n \~
  \ingroup Model_Attributes
 */
class MATH_CLASS MbStringAttribute : public MbCommonAttribute {
private:
  c3d::string_t value_; ///< \ru Значение. \en The value. 

public:
  /// \ru Конструктор. \en Constructor. 
  explicit MbStringAttribute( const c3d::string_t & prompt, bool change, const c3d::string_t & string );

public:
  MbeAttributeType  AttributeType() const override; // \ru Выдать подтип атрибута. \en Get subtype of an attribute. 
  void              GetCharValue( TCHAR * v ) const override; // \ru Выдать строковое значение свойства. \en Get a string value of the property. 
  MbAttribute &     Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента. \en Create a copy of the element.
  bool              IsSame( const MbAttribute &, double accuracy ) const override; // \ru Определить, являются ли объекты равными. \en Determine whether objects are equal. 
  bool              Init( const MbAttribute & ) override; // \ru Инициализировать данные по присланным. \en Initialize data. 
  void              GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  size_t            SetProperties( const MbProperties & ) override; // \ru Установить свойства объекта. \en Set properties of object. 
  MbePrompt         GetPropertyName() override; // \ru Выдать заголовок свойства объекта. \en Get a name of object property. 
public:
  const c3d::string_t & GetValue() const { return value_; } ///< \ru Выдать значение. \en Get a value. 
  bool                  SetValue( const c3d::string_t & );        ///< \ru Установить новое значение. \en Set new value. 

protected:
  virtual ~MbStringAttribute();  // Use AddRef/Release or smart pointer SPtr<MbAttribute> to destruct it correctly.

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbStringAttribute ) 
OBVIOUS_PRIVATE_COPY( MbStringAttribute )
};

IMPL_PERSISTENT_OPS( MbStringAttribute )


//------------------------------------------------------------------------------
/** \brief \ru Бинарный атрибут.
    \en Binary attribute. \~
  \details \ru Бинарный атрибут. \n
           \en Binary attribute. \n \~
  \ingroup Model_Attributes
*/
class MATH_CLASS MbBinaryAttribute : public MbCommonAttribute {
private:
  std::vector<unsigned char> value_; ///< \ru Значение. \en The value. 

public:
  /// \ru Конструктор. \en Constructor. 
  explicit MbBinaryAttribute( const c3d::string_t & prompt, bool change, const std::vector<unsigned char> & value );

public:
  MbeAttributeType  AttributeType() const override; // \ru Выдать подтип атрибута. \en Get subtype of an attribute. 
  void              GetCharValue( TCHAR * v ) const override; // \ru Выдать строковое значение свойства. \en Get a string value of the property. 
  MbAttribute &     Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента. \en Create a copy of the element.
  bool              IsSame( const MbAttribute &, double accuracy ) const override; // \ru Определить, являются ли объекты равными. \en Determine whether objects are equal. 
  bool              Init( const MbAttribute & ) override; // \ru Инициализировать данные по присланным. \en Initialize data. 
  void              GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  size_t            SetProperties( const MbProperties & ) override; // \ru Установить свойства объекта. \en Set properties of object. 
  MbePrompt         GetPropertyName() override; // \ru Выдать заголовок свойства объекта. \en Get a name of object property. 
public:
  const std::vector<unsigned char> &  GetValue() const { return value_; }                   ///< \ru Выдать значение. \en Get a value. 
  bool                                SetValue( const std::vector<unsigned char> & );       ///< \ru Установить новое значение. \en Set new value. 
  size_t                              Count() const { return value_.size(); }               ///< \ru Выдать число элементов в массиве. \en Get a number of elements in the array.
  unsigned char                       operator [] ( size_t k ) const { return value_[k]; }  ///< \ru Доступ к элементу массива по индексу (без проверки на выход за границы). \en Access to array element by index (without bounds checking).

protected:
  virtual ~MbBinaryAttribute();  // Use AddRef/Release or smart pointer SPtr<MbAttribute> to destruct it correctly.

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbBinaryAttribute ) 
OBVIOUS_PRIVATE_COPY( MbBinaryAttribute )
};

IMPL_PERSISTENT_OPS( MbBinaryAttribute )


//------------------------------------------------------------------------------
/** \brief \ru Атрибут массив целочисленных значений типа int32.
           \en Array of integer (int32) values attribute. \~
  \details \ru Атрибут массив целочисленных значений типа int32. \n
           \en Array of integer (int32) values attribute. \n \~
  \ingroup Model_Attributes
*/
class MATH_CLASS MbInt32VectorAttribute : public MbCommonAttribute {
private:
  std::vector<int32> value_; ///< \ru Значение. \en The value. 

public:
  /// \ru Конструктор. \en Constructor. 
  explicit MbInt32VectorAttribute( const c3d::string_t & prompt, const bool change, const std::vector<int32> & value );
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbInt32VectorAttribute();

public:
  MbeAttributeType  AttributeType() const override; // \ru Выдать подтип атрибута. \en Get subtype of an attribute. 
  void              GetCharValue( TCHAR * v ) const override; // \ru Выдать строковое значение свойства. \en Get a string value of the property. 
  MbAttribute &     Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента. \en Create a copy of the element.
  bool              IsSame( const MbAttribute &, double accuracy ) const override; // \ru Определить, являются ли объекты равными. \en Determine whether objects are equal. 
  bool              Init( const MbAttribute & ) override; // \ru Инициализировать данные по присланным. \en Initialize data. 
  void              GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  size_t            SetProperties( const MbProperties & ) override; // \ru Установить свойства объекта. \en Set properties of object. 
  MbePrompt         GetPropertyName() override; // \ru Выдать заголовок свойства объекта. \en Get a name of object property. 
public:
  const std::vector<int32> &  GetValue() const;                                     ///< \ru Выдать значение. \en Get a value. 
  bool                        SetValue( const std::vector<int32> & );               ///< \ru Установить новое значение. \en Set new value. 
  size_t                      Count() const { return value_.size(); }               ///< \ru Выдать число элементов в массиве. \en Get a number of elements in the array.
  int32                       operator [] ( size_t k ) const { return value_[k]; }  ///< \ru Доступ к элементу массива по индексу (без проверки на выход за границы). \en Access to array element by index (without bounds checking).

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbInt32VectorAttribute ) 
OBVIOUS_PRIVATE_COPY( MbInt32VectorAttribute )
};

IMPL_PERSISTENT_OPS( MbInt32VectorAttribute )


//------------------------------------------------------------------------------
/** \brief \ru Атрибут массив целочисленных значений типа int64.
           \en Array of integer (int64) values attribute. \~
  \details \ru Атрибут массив целочисленных значений типа int64. \n
           \en Array of integer (int64) values attribute. \n \~
  \ingroup Model_Attributes
*/
class MATH_CLASS MbInt64VectorAttribute : public MbCommonAttribute {
private:
  std::vector<int64> value_; ///< \ru Значение. \en The value. 

public:
  /// \ru Конструктор. \en Constructor. 
  explicit MbInt64VectorAttribute( const c3d::string_t & prompt, const bool change, const std::vector<int64> & value );
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbInt64VectorAttribute();

public:
  MbeAttributeType  AttributeType() const override; // \ru Выдать подтип атрибута. \en Get subtype of an attribute. 
  void              GetCharValue( TCHAR * v ) const override; // \ru Выдать строковое значение свойства. \en Get a string value of the property. 
  MbAttribute &     Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента. \en Create a copy of the element.
  bool              IsSame( const MbAttribute &, double accuracy ) const override; // \ru Определить, являются ли объекты равными. \en Determine whether objects are equal. 
  bool              Init( const MbAttribute & ) override; // \ru Инициализировать данные по присланным. \en Initialize data. 
  void              GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  size_t            SetProperties( const MbProperties & ) override; // \ru Установить свойства объекта. \en Set properties of object. 
  MbePrompt         GetPropertyName() override; // \ru Выдать заголовок свойства объекта. \en Get a name of object property. 
public:
  const std::vector<int64> &  GetValue() const;                                     ///< \ru Выдать значение. \en Get a value. 
  bool                        SetValue( const std::vector<int64> & );               ///< \ru Установить новое значение. \en Set new value. 
  size_t                      Count() const { return value_.size(); }               ///< \ru Выдать число элементов в массиве. \en Get a number of elements in the array.
  int64                       operator [] ( size_t k ) const { return value_[k]; }  ///< \ru Доступ к элементу массива по индексу (без проверки на выход за границы). \en Access to array element by index (without bounds checking).

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbInt64VectorAttribute ) 
OBVIOUS_PRIVATE_COPY( MbInt64VectorAttribute )
};

IMPL_PERSISTENT_OPS( MbInt64VectorAttribute )


//------------------------------------------------------------------------------
/** \brief \ru Атрибут массив действительных чисел типа double.
           \en Array of real (double) values attribute. \~
  \details \ru Атрибут массив действительных чисел типа double. \n
           \en Array of real (double) values attribute. \n \~
  \ingroup Model_Attributes
*/
class MATH_CLASS MbDoubleVectorAttribute : public MbCommonAttribute {
private:
  std::vector<double> value_; ///< \ru Значение. \en The value. 

public:
  /// \ru Конструктор. \en Constructor. 
  explicit MbDoubleVectorAttribute( const c3d::string_t & prompt, const bool change, const std::vector<double> & value );
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbDoubleVectorAttribute();

public:
  MbeAttributeType  AttributeType() const override; // \ru Выдать подтип атрибута. \en Get subtype of an attribute. 
  void              GetCharValue( TCHAR * v ) const override; // \ru Выдать строковое значение свойства. \en Get a string value of the property. 
  MbAttribute &     Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента. \en Create a copy of the element.
  bool              IsSame( const MbAttribute &, double accuracy ) const override; // \ru Определить, являются ли объекты равными. \en Determine whether objects are equal. 
  bool              Init( const MbAttribute & ) override; // \ru Инициализировать данные по присланным. \en Initialize data. 
  void              GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  size_t            SetProperties( const MbProperties & ) override; // \ru Установить свойства объекта. \en Set properties of object. 
  MbePrompt         GetPropertyName() override; // \ru Выдать заголовок свойства объекта. \en Get a name of object property. 
public:
  const std::vector<double> & GetValue() const;                                     ///< \ru Выдать значение. \en Get a value. 
  bool                        SetValue( const std::vector<double> & val );          ///< \ru Установить новое значение. \en Set new value.
  size_t                      Count() const { return value_.size(); }               ///< \ru Выдать число элементов в массиве. \en Get a number of elements in the array.
  double                      operator [] ( size_t k ) const { return value_[k]; }  ///< \ru Доступ к элементу массива по индексу (без проверки на выход за границы). \en Access to array element by index (without bounds checking).

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbDoubleVectorAttribute ) 
OBVIOUS_PRIVATE_COPY( MbDoubleVectorAttribute )
};

IMPL_PERSISTENT_OPS( MbDoubleVectorAttribute )


//------------------------------------------------------------------------------
/** \brief \ru Атрибут диапазона действительных чисел типа double.
           \en Double precision range attribute. \~
  \details \ru Атрибут диапазона действительных чисел типа double. \n
           \en Double precision range attribute. \n \~
  \ingroup Model_Attributes
*/
class MATH_CLASS MbDoubleRangeAttribute : public MbCommonAttribute {
private:
  MbRect1D range_; ///< \ru Диапазон значений. \en Values range. 

public:
  /// \ru Конструктор. \en Constructor. 
  explicit MbDoubleRangeAttribute( const c3d::string_t & prompt, bool change, double begValue, double endValue );
  /// \ru Конструктор. \en Constructor. 
  explicit MbDoubleRangeAttribute( const c3d::string_t & prompt, bool change, const MbRect1D & range );
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbDoubleRangeAttribute();

public:
  MbeAttributeType  AttributeType() const override; // \ru Выдать подтип атрибута. \en Get subtype of an attribute. 
  void              GetCharValue( TCHAR * v ) const override; // \ru Выдать строковое значение свойства. \en Get a string value of the property. 
  MbAttribute &     Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента. \en Create a copy of the element.
  bool              IsSame( const MbAttribute &, double accuracy ) const override; // \ru Определить, являются ли объекты равными. \en Determine whether objects are equal. 
  bool              Init( const MbAttribute & ) override; // \ru Инициализировать данные по присланным. \en Initialize data. 
  void              GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  size_t            SetProperties( const MbProperties & ) override; // \ru Установить свойства объекта. \en Set properties of object. 
  MbePrompt         GetPropertyName() override; // \ru Выдать заголовок свойства объекта. \en Get a name of object property. 
public:
  const MbRect1D & GetRange() const { return range_; }        ///< \ru Выдать диапазон значений. \en Get values range. 
  bool             SetRange( const MbRect1D & );              ///< \ru Установить диапазон значений. \en Set values range. 
  bool             SetRange( double begVal, double endVal );  ///< \ru Установить диапазон значений. \en Set values range. 

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbDoubleRangeAttribute )
OBVIOUS_PRIVATE_COPY( MbDoubleRangeAttribute )
};

IMPL_PERSISTENT_OPS( MbDoubleRangeAttribute )


#endif // __ATTR_COMMON_ATTRIBUE_H
