////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Атрибут хот-точки.
         \en Attribute of hot-point. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __ATTR_HOTPOINT_ATTRIBUTE_H
#define __ATTR_HOTPOINT_ATTRIBUTE_H


#include <attr_common_attribute.h>
#include <mb_placement3d.h>

class  MbBaseHotPointAttribute;


namespace c3d // namespace C3D
{
  typedef SPtr<MbBaseHotPointAttribute> BaseHotPointAttrSPtr;
}


//------------------------------------------------------------------------------
/** \brief \ru Базовый класс атрибутов хот-точки.
           \en Base class of hot-point attributes. \~
  \details \ru Базовый класс атрибутов хот-точки.
           \en Base class of hot-point attributes. \n \~
  \ingroup Model_Attributes
  \warning \ru В разработке.
           \en Under development. \~
 */
class MATH_CLASS MbBaseHotPointAttribute : public MbCommonAttribute
{
public:
  //------------------------------------------------------------------------------
  /** \brief \ru Тип хот-точки.
             \en The hot point type. \~
    \details \ru Тип хот-точки по принадлежности операции.
             \en The hot point type by operation. \~
    \ingroup Data_Structures
  */
  //---
  enum class MbeHotPointType : uint16 {
    undefinedHP = 0,       ///< \ru Неопределенный тип. \en Undefined type.
    draftHP,               ///< \ru Хот-точка операции уклона. \en The draft operation hot point.
    conicSectionHP,        ///< \ru Хот-точка операции построения переменного конического сечения. \en The conic section operation hot point.
    DMFilletRadiusSetHP,   ///< \ru Хот-точка операции прямого моделирования по установке радиусов граням скругления. \en The direct modeling operation's hot point by set fillet face's radius.
    elementaryHP,          ///< \ru Хот-точка операции построения элементарного тела. \en The elementary solid operation hot point.
    DMReplaceFacesHP,      ///< \ru Хот-точка операции замены граней. \en Hot point of the face replacement operation.
    DMOffsetFacesHP,       ///< \ru Хот-точка операции замены граней на эквидистантные. \en Hot point of the operation of replacement faces with the offset ones.
    DMReplaceFacesGroupHP, ///< \ru Хот-точка операции замены группы граней. \en Hot point of the operation of replacement faces group.
    // \ru Вставлять новые типы перед этим типом. \en Insert new types before this type. 
    endHP = SYS_MAX_INT16
  };

private:
  MbeHotPointType _type; ///< \ru Тип хот-точки. \en The type of hot point.

protected:
  /// \ru Конструктор. \en Constructor. 
  MbBaseHotPointAttribute( const MbBaseHotPointAttribute & );
  /// \ru Конструктор. \en Constructor. 
  MbBaseHotPointAttribute( MbeHotPointType type, const c3d::string_t & prompt, const bool change );
public:
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbBaseHotPointAttribute();

public:
  MbeAttributeType AttributeFamily() const override;    // \ru Выдать тип атрибута. \en Get attribute type.
  MbeHotPointType  GetHotPointType() const { return _type; } // \ru Выдать тип хот-точки. \en Get hot point type.
  bool    IsSame( const MbAttribute &, double accuracy ) const override; // \ru Определить, являются ли объекты равными. \en Determine whether objects are equal. 
  // \ru Выполнить действия при трансформировании владельца. \en Perform actions when transforming the owner. 
  void    OnTransformOwner( const MbAttributeContainer & owner,
                            const MbMatrix3D &,
                                  MbRegTransform * = nullptr ) override;
  // \ru Выполнить действия при перемещении владельца. \en Perform actions when moving the owner. 
  void    OnMoveOwner( const MbAttributeContainer & owner,
                       const MbVector3D &,
                             MbRegTransform * = nullptr ) override;
  // \ru Выполнить действия при вращении владельца. \en Perform actions when rotating the owner. 
  void    OnRotateOwner( const MbAttributeContainer & owner,
                         const MbAxis3D &,
                               double                  angle,
                               MbRegTransform * = nullptr ) override;
  // \ru Выполнить действия при копировании владельца. \en Perform actions when copying the owner. 
  void    OnCopyOwner( const MbAttributeContainer & owner,
                             MbAttributeContainer & other,
                             MbRegDuplicate * = nullptr ) override;
  // \ru Выполнить действия при объединении владельца. \en Perform actions when merging the owner. 
  void    OnMergeOwner( const MbAttributeContainer & owner,
                              MbAttributeContainer & other ) override;
  void    GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта. \en Get properties of the object.
  size_t  SetProperties( const MbProperties & ) override; // \ru Установить свойства объекта. \en Set properties of object.
  MbePrompt GetPropertyName() override; // \ru Выдать заголовок свойства объекта. \en Get a name of object property.

private:
  /// \ru Оператор присваивания (не реализован). \en Assignment operator (not implemented). 
  MbBaseHotPointAttribute & operator = ( const MbBaseHotPointAttribute & );

  DECLARE_PERSISTENT_CLASS( MbBaseHotPointAttribute )
};

IMPL_PERSISTENT_OPS( MbBaseHotPointAttribute )


//------------------------------------------------------------------------------
/** \brief \ru Атрибут простой хот-точки.
           \en Attribute of a simple hot-point. \~
  \details \ru Атрибут параметров простой хот-точки содержит трехмерную точку
               и трехмерную локальную систему координат.
           \en Attribute of parameters of a simple hot-point contains three-dimensional point and
               three-dimensional coordinate system. \n \~
  \ingroup Model_Attributes
  \warning \ru В разработке.
           \en Under development. \~
 */
class MATH_CLASS MbHotPointAttribute : public MbBaseHotPointAttribute
{
protected:
  MbPlacement3D _place;    ///< \ru Трехмерная локальная система координат. \en Three-dimensional coordinate system.
  MbCartPoint3D _hotPoint; ///< \ru Хот-точка. \en Hot-point.
public:
  /** \brief \ru Конструктор атрибута хот-точки.
             \en Constructor of attribute of hot-point. \~
    \details \ru Создается атрибут хот-точки содержащий трехмерную точку и трехмерную локальную систему координат.
             \en Created attribute of hot-point contains three-dimensional point and three-dimensional coordinate system. \~
    \param[in] place - \ru Локальная система координат хот-точки.
                       \en The local coordinate system of hot-point. \~
    \param[in] hotPoint   - \ru Хот-точка.
                            \en The hot-point. \~
    \param[in] type       - \ru Тип хот-точки.
                            \en The type of hot-point. \~
    \param[in] changeable - \ru Признак редактируемости.
                            \en Attribute of editability. \~
  */
  MbHotPointAttribute( const MbPlacement3D & place, 
                       const MbCartPoint3D & hotPoint,
                             MbeHotPointType type,
                             bool            changeable );
public:
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbHotPointAttribute();

public:
  // \ru Выдать подтип атрибута. \en Get subtype of an attribute. 
  MbeAttributeType AttributeType() const override;
  // \ru Сделать копию элемента. \en Create a copy of the element. 
  MbAttribute &    Duplicate( MbRegDuplicate * = nullptr ) const override;
  // \ru Инициализировать данные по присланным. \en Initialize data. 
  bool             Init( const MbAttribute & ) override;
    // \ru Определить, являются ли объекты равными. \en Determine whether objects are equal. 
  bool             IsSame( const MbAttribute &, 
                                 double       accuracy ) const override;
  // \ru Выполнить действия при трансформировании владельца. \en Perform actions when transforming the owner. 
  void             OnTransformOwner( const MbAttributeContainer & owner,
                                     const MbMatrix3D &,
                                           MbRegTransform * = nullptr ) override;
  // \ru Выполнить действия при перемещении владельца. \en Perform actions when moving the owner. 
  void             OnMoveOwner( const MbAttributeContainer & owner,
                                const MbVector3D &,
                                      MbRegTransform * = nullptr ) override;
  // \ru Выполнить действия при вращении владельца. \en Perform actions when rotating the owner. 
  void             OnRotateOwner( const MbAttributeContainer & owner,
                                  const MbAxis3D &,
                                        double                  angle,
                                        MbRegTransform * = nullptr ) override;
  // \ru Выполнить действия при копировании владельца. \en Perform actions when copying the owner. 
  void             OnCopyOwner( const MbAttributeContainer & owner,
                                      MbAttributeContainer & other,
                                      MbRegDuplicate * = nullptr ) override;
  // \ru Выполнить действия при объединении владельца. \en Perform actions when merging the owner. 
  void             OnMergeOwner( const MbAttributeContainer & owner,
                                       MbAttributeContainer & other ) override;

  // \ru Выдать строковое значение свойства. \en Get a string value of the property.
  void                   GetCharValue( TCHAR * v ) const override; 
  /// \ru Дать систему координат. \en Get placement.
  const  MbPlacement3D & GetPlacement() const { return _place; }
  /// \ru Дать хот точку. \en Get hot - point.
  const  MbCartPoint3D & GetHotPoint() const { return _hotPoint; }


private:
  // \ru Конструктор копирования. \en Copy constructor. 
  MbHotPointAttribute( const MbHotPointAttribute & init, 
                             MbRegDuplicate *      iReg );
  
  DECLARE_PERSISTENT_CLASS( MbHotPointAttribute )
  OBVIOUS_PRIVATE_COPY( MbHotPointAttribute )
};

IMPL_PERSISTENT_OPS( MbHotPointAttribute )


//------------------------------------------------------------------------------
/** \brief \ru Атрибут композитной хот-точки (набора хот-точек).
           \en Attribute of a composite hot-point( hot point set ). \~
  \details \ru Атрибут параметров композитной хот-точки содержит набор пар : 
               трехмерная локальная система координат -трехмерная точка.
           \en Attribute of parameters of a composite hot-point contains a set of pairs
               of three-dimensional coordinate system and three-dimensional point. \n \~
  \ingroup Model_Attributes
  \warning \ru В разработке.
           \en Under development. \~
 */
class MATH_CLASS MbCompositeHotPointAttribute : public MbBaseHotPointAttribute
{
private:
  c3d::PlacePoint3DPairVector _hotpoints;

public:
  /** \brief \ru Конструктор атрибута хот-точки.
             \en Constructor of attribute of hot-point. \~
    \details \ru Создается атрибут хот-точки содержащий трехмерную точку и трехмерную локальную систему координат.
             \en Created attribute of hot-point contains three-dimensional point and three-dimensional coordinate system. \~
    \param[in] place - \ru Локальная система координат хот-точки.
                       \en The local coordinate system of hot-point. \~
    \param[in] hotPoint - \ru Хот-точка.
                          \en The hot-point. \~
  */
  explicit
  MbCompositeHotPointAttribute( MbeHotPointType type, 
                                bool            changeable );
public:
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbCompositeHotPointAttribute();

public:
  // \ru Выдать подтип атрибута. \en Get subtype of an attribute. 
  MbeAttributeType AttributeType() const override;
  // \ru Сделать копию элемента. \en Create a copy of the element. 
  MbAttribute &    Duplicate( MbRegDuplicate * = nullptr ) const override;
  // \ru Инициализировать данные по присланным. \en Initialize data. 
  bool             Init( const MbAttribute & ) override;
    // \ru Определить, являются ли объекты равными. \en Determine whether objects are equal. 
  bool             IsSame( const MbAttribute &, 
                                 double       accuracy ) const override;
  // \ru Выполнить действия при трансформировании владельца. \en Perform actions when transforming the owner. 
  void             OnTransformOwner( const MbAttributeContainer & owner,
                                     const MbMatrix3D &,
                                           MbRegTransform * = nullptr ) override;
  // \ru Выполнить действия при перемещении владельца. \en Perform actions when moving the owner. 
  void             OnMoveOwner( const MbAttributeContainer & owner,
                                const MbVector3D &,
                                      MbRegTransform * = nullptr ) override;
  // \ru Выполнить действия при вращении владельца. \en Perform actions when rotating the owner. 
  void             OnRotateOwner( const MbAttributeContainer & owner,
                                  const MbAxis3D &,
                                        double                  angle,
                                        MbRegTransform * = nullptr ) override;
  // \ru Выполнить действия при копировании владельца. \en Perform actions when copying the owner. 
  void             OnCopyOwner( const MbAttributeContainer & owner,
                                      MbAttributeContainer & other,
                                      MbRegDuplicate * = nullptr ) override;
  // \ru Выполнить действия при объединении владельца. \en Perform actions when merging the owner. 
  void             OnMergeOwner( const MbAttributeContainer & owner,
                                       MbAttributeContainer & other ) override;

  // \ru Выдать строковое значение свойства. \en Get a string value of the property.
  void   GetCharValue( TCHAR * v ) const override; 
  // \ru Выдать строковое значение свойства. \en Get a string value of the property.
  size_t GetHotPointsCount()       const { return _hotpoints.size(); }
  // \ru Выдать хот-точку по индексу. \en Get a hot point by index.
  void   GetHotPoint( size_t ind, MbPlacement3D & place, MbCartPoint3D & pnt ) const { place.Init( _hotpoints[ind].first ); pnt.Init( _hotpoints[ind].second ); }
  // \ru Добавить хот-точку в набор. \en Add a hot point to set.
  void   AddHotPoint( const MbPlacement3D & place, const MbCartPoint3D & pnt );
  // \ru Добавить набор хот-точек. \en Add a hot points set.
  void   AddHotPoints( const c3d::PlacePoint3DPairVector & data );
private:
  // \ru Конструктор копирования. \en Copy constructor. 
  MbCompositeHotPointAttribute( const MbCompositeHotPointAttribute & init,
                                      MbRegDuplicate *               iReg );

  DECLARE_PERSISTENT_CLASS( MbCompositeHotPointAttribute )
  OBVIOUS_PRIVATE_COPY( MbCompositeHotPointAttribute )
};

IMPL_PERSISTENT_OPS( MbCompositeHotPointAttribute )


#endif // __ATTR_HOTPOINT_ATTRIBUTE_H

