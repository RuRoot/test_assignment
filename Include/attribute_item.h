////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Атрибуты объекта.
         \en Object attributes. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __ATTRIBUTE_ITEM_H
#define __ATTRIBUTE_ITEM_H


#include <io_tape_define.h>
#include <reference_item.h>
#include <math_define.h>
#include <mb_property_title.h>


class  MbVector3D;
class  MbAxis3D;
class  MbMatrix3D;
class  MbProperties;
class  MbAttributeContainer; 
class            MbRegDuplicate;
class            MbRegTransform;


class  MbAttribute;
namespace c3d // namespace C3D
{
typedef SPtr<MbAttribute>                 AttrSPtr;
typedef SPtr<const MbAttribute>           ConstAttrSPtr;

typedef std::vector<MbAttribute *>        AttrVector;
typedef std::vector<const MbAttribute *>  ConstAttrVector;

typedef std::vector<AttrSPtr>             AttrSPtrVector;
typedef std::vector<ConstAttrSPtr>        ConstAttrSPtrVector;
}


//------------------------------------------------------------------------------
/** \brief \ru Типы атрибутов.
           \en Types of attributes. \~
  \details \ru Типы атрибутов объектов геометрической модели.
     Атрибуты объектов группируются по семействам.
           \en Types of geometric model objects attributes.
     Objects attributes are grouped by families. \~
  \ingroup Model_Attributes
 */
enum MbeAttributeType
{
  at_Undefined              =   0,  ///< \ru Неопределенный - используется при поиске как "любой". \en Undefined - used as "any" in search. \n

  // \ru Типы простых атрибутов. \en Types of elementary attributes. 
  at_ElementaryAttribute    = 101,  ///< \ru Простой атрибут. \en Elementary attribute. 
  at_Identifier             = 102,  ///< \ru Идентификатор. \en Identifier. 
  at_Color                  = 103,  ///< \ru Цвет. \en Color. 
  at_Width                  = 104,  ///< \ru Ширина линий. \en Lines width. 
  at_Style                  = 105,  ///< \ru Стиль линий. \en Lines style.  
  at_Visual                 = 106,  ///< \ru Свойства материала (свойства для OpenGL). \en Material properties (properties for OpenGL).
  at_Selected               = 107,  ///< \ru Селектированность. \en Selection. 
  at_Visible                = 108,  ///< \ru Видимость. \en Visibility. 
  at_WireCount              = 109,  ///< \ru Количество u-линий и v-линий отрисовочной сетки. \en The number of u-mesh and v-mesh drawing lines. \~
  at_Changed                = 110,  ///< \ru Изменённость. \en Modification. 
  at_Dencity                = 111,  ///< \ru Плотность. \en Density. 
  at_NameAttribute          = 112,  ///< \ru Топологическое имя. \en Topological name. 
  at_UpdateStamp            = 113,  ///< \ru Метка времени обновления. \en Stamp of update time. 
  at_Embodiment             = 114,  ///< \ru Признак исполнения (варианта реализации модели). \en Indication of embodiment (variant of model implementation). 
  at_Elasticity             = 115,  ///< \ru Механические характеристики: модуль Юнга и коэффициент Пуассона. \en Mechanical properties: Young's modulus and Poisson's ratio.
  at_Strains                = 116,  ///< \ru Деформации. \en The strains.
  at_ShellThickness         = 117,  ///< \ru Толщина оболочки. \en The shell thickness.
  at_ElementaryLast         = 200,  ///< \ru Простые атрибуты вставлять перед этим значением. \en Elementary attributes should be inserted before this value. \n

  // \ru Типы обобщенных атрибутов. \en Types of common attributes. 
  at_CommonAttribute        = 201,  ///< \ru Обобщенный атрибут. \en Common attribute. 
  at_BoolAttribute          = 202,  ///< \ru Булев атрибут. \en Boolean attribute. 
  at_IntAttribute           = 203,  ///< \ru Целочисленный атрибут. \en Integer attribute. 
  at_DoubleAttribute        = 204,  ///< \ru Действительный атрибут. \en Double attribute. 
  at_StringAttribute        = 205,  ///< \ru Строковый атрибут. \en String attribute. 
  at_GeomAttribute          = 206,  ///< \ru Геометрический атрибут. \en Geometric attribute. \n
  at_StampRibAttribute      = 207,  ///< \ru Атрибут ребра жесткости листового тела. \en Attribute of reinforcement rib of sheet solid. \n
  at_Int64Attribute         = 208,  ///< \ru Атрибут int64. \en Int64 attribute. 
  at_BinaryAttribute        = 209,  ///< \ru Бинарный атрибут. \en Binary attribute. 
  at_SweptFlangeAttribute   = 210,  ///< \ru Атрибут отбортовки листового тела. \en Swept flange attribute of a sheet solid. \n
  at_Int32VectorAttribute   = 211,  ///< \ru Атрибут массив целочисленных значений типа int32. \en Array of integer (int32) values attribute.
  at_Int64VectorAttribute   = 212,  ///< \ru Атрибут массив целочисленных значений типа int64. \en Array of integer (int64) values attribute. 
  at_DoubleVectorAttribute  = 213,  ///< \ru Атрибут массив действительных чисел типа double. \en Array of real (double) values attribute.
  at_UuidInstanceAttribute  = 214,  ///< \ru Атрибут Uuid для вставок. \en Uuid attribute for instances.
  at_BaseHotPointAttribute  = 215,  ///< \ru Базовый атрибут хот-точки. \en Base attribute of hot-point. \n 
  at_HotPointAttribute      = 216,  ///< \ru Атрибут простой хот-точки. \en Attribute of the simple hot-point. \n 
  at_CompHotPointAttribute  = 217,  ///< \ru Композитный атрибут хот-точки. \en Composite attribute of the hot-point. \n 
  at_DoubleRangeAttribute   = 218,  ///< \ru Атрибут диапазона действительных чисел типа double. \en Double precision range attribute.
  at_RefItemAttribute       = 219,  ///< \ru Атрибут ссылочного объекта. \en Reference-counted object attribute. \n
  at_CommonLast             = 300,  ///< \ru Обобщенные атрибуты вставлять перед этим значением. \en Common attributes should be inserted before this value. \n

  // \ru Типы связующих атрибутов. \en Types of linking attributes. 
  at_LinkingAttribute       = 301,  ///< \ru Связующий атрибут. \en Linking attribute. 
  at_AnchorAttribute        = 302,  ///< \ru Якорь. \en Anchor. \n

  // \ru Типы директивных атрибутов. \en Types of directive attributes. 
  at_DirectiveAttribute     = 401,  ///< \ru Директивный атрибут. \en Directive attribute. 
  at_KeepUniqueKey          = 402,  ///< \ru Поддерживать уникальность ключей. \en Support unique keys. \n

  // \ru Типы изделия. \en Types of product attributes.
  at_ProductAttribute       = 501,  ///< \ru Атрибут конвертеров \en Converters attribute 
  at_ModelInfo              = 502,  ///< \ru Сведения о модели в целом. \en Information about model itself.
  at_PersonOrganizationInfo = 503,  ///< \ru Лицо и организация. \en Person and organization information.
  at_ProductInfo            = 504,  ///< \ru Сведения об изделии. \en Product info.
  at_STEPTextDescription    = 505,  ///< \ru Описание STEP. \en STEP description. 
  at_STEPReferenceHolder    = 506,  ///< \ru Обратная ссылка. \en Back reference. \n

  // \ru Типы пользовательских атрибутов. \en Types of user attributes. 
  at_UserAttribute          = 601,  ///< \ru Пользовательский атрибут. \en User attribute. 
  at_UserFirst              = 602,  ///< \ru Первый пользовательский атрибут. \en First user attribute.  
  at_UserLast               = 900,  ///< \ru Последний пользовательский атрибут. \en Last user attribute. \n

  // \ru Типы внешних (внесистемных) атрибутов. \en Types of external (off-system) attributes. 
  at_ExternalAttribute      = 901,  ///< \ru Внешний атрибут. \en External attribute. 
  at_ExternalAttributeImp   = 902,  ///< \ru Подтип - внешний атрибут \en Subtype - external attribute. 

  at_FreeItem               = 1000, ///< \ru Тип для прочих объектов. \en Type for the other objects.

};


//------------------------------------------------------------------------------
/** \brief \ru Типы контейнеров атрибутов.
           \en Types of attribute containers. \~
  \details \ru Типы контейнеров атрибутов наследников контейнера атрибутов.
     Каждый отдельный атрибут может содержать свой контейнер атрибутов.
           \en Types of attribute containers which are inheritors of attribute container.
     Each separate attribute may have its attribute container. \~
  \ingroup Model_Attributes
 */
enum MbeImplicationType 
{
  ace_Attribute,       ///< \ru Контейнер атрибутов, содержащий другие атрибуты. \en Attribute container which contains other attributes. 
  ace_ModelItem,       ///< \ru Контейнер атрибутов объектов геометрической модели. \en Container of geometric model objects attributes. 
  ace_TopItem,         ///< \ru Контейнер атрибутов именованных топологических объектов. \en Container of named topological objects attributes. 
  ace_MeshItem,        ///< \ru Контейнер атрибутов сеточных примитивов. \en Container of mesh primitives attributes. 
  ace_Model,           ///< \ru Контейнер атрибутов геометрической модели. \en Container of geometric model attributes. 
  ace_AttribContainer, ///< \ru Контейнер атрибутов. \en Attribute container. 
};


//------------------------------------------------------------------------------
/** \brief \ru Атрибуты объекта.
           \en Object attributes. \~
  \details \ru Атрибуты содержат информацию, дополняющую описание геометрической формы объекта.
    Атрибут не является неотъемлемой частью объекта, а является элементом данных, которыми может быть наделен объект.\n
    Атрибуты являются агентами передачи данных геометрического ядра от одного приложения другому приложению.\n
    Атрибуты могут быть следующих типов.\n
    Простой атрибут - атрибут несущий простую, однозначно интерпретируемую, информацию, например, цвет, признак выбора.\n
    Обобщенный атрибут - атрибут стандартного типа со строковым наименованием, 
    например, имя, целое число, вещественное число, строка, точка, вектор, указатель.\n 
    С помощью таких атрибутов приложения могут обмениваться какой либо специфичной информацией 
    без необходимости разработки дополнительных комплексных атрибутов.\n
    Комплексный атрибут - атрибут состоящий из предопределенного набора данных, 
    описывающих природу атрибута и его смысловую нагрузку, а так же способ его интерпретации. 
    Такими атрибутами могут описываться некоторые ограничения или простые зависимости а так же аннотационные объекты.\n
    Директивный атрибут - атрибут определяющий предназначения объекта или действия которые необходимо с ним произвести, 
    например атрибут "вычитание" подразумевает что некое тело предназначено для вычитания из другого тела, 
    и не важно из какого.
    Связующий атрибут - атрибут предназначенный для связи объекта геометрического ядра с абстрактным контейнером данных, 
    то есть набором данных, формат и смысловая нагрузка которых не может быть описана в рамках других атрибутов.\n
           \en Attributes contain information supplementing description of object geometric shape.
    Attribute is not an intrinsic part of the object, but it is an element of data which the object may contain. \n
    Attributes are geometric kernel agents for transferring data from one application to another. \n
    The possible types of attributes are the following. \n
    Elementary attribute - an attribute which reflects simple and clearly interpreted information, for example, color or selection attribute.\n
    Common attribute - attribute of standard type with string naming, 
    for example: name, integer value, double value, string, point, vector, pointer. \n 
    Applications may communicate any specific information using such attributes 
    without necessity of additional complex attributes developing. \n
    Complex attribute - an attribute which consists of predefined data set, 
    which describe a nature of attribute, its semantic meaning and a way of its interpretation. 
    Such attributes can describe some of constraints or simple dependences and annotation objects.\n
    Directive attribute - an attribute defining the purpose of object or actions which should be performed with it, 
    for example, an attribute "subtraction" implies that one solid is purposed for subtraction from another, 
    no matter from what exactly.
    Linking attribute - an attribute designed for linking of geometric kernel object with abstract container of data, 
    i.e. a set of data, which format and semantic meaning can not be described by other attributes. \n \~
  \ingroup Model_Attributes
*/
// ---
class MATH_CLASS MbAttribute : public MbRefItem, 
                               public TapeBase 
{
public:
  /**\ru Поведение атрибута при изменении владельца, не связанном с другими описанными действия.
     \en Behavior of attribute which is not associated with other described actions when changing the owner. \~ */
  enum OnChangeOwnerAction {
    chn_Self = 0, ///< \ru Поведение, определяемое виртуальной функцией OnChangeOwner. \en Behavior defined by the virtual function OnChangeOwner. 
    chn_Free,     ///< \ru Освободить атрибут, если это возможно, в противном случае удалить. \en Free attribute if it is possible, otherwise delete it. 
    chn_Keep,     ///< \ru Сохранить атрибут, т.е. ничего с ним не делать. \en Save attribute, i.e. do not do anything with it. 
    chn_ActCount, ///< \ru Количество элементов в перечислении (добавлять перед данным значением). \en The number of elements in enumeration (add before the given value). 
  };

  /**\ru Поведение атрибута при перерождении объекта в другой объект.
     \en Behavior of attribute when an object regenerates in other object. \~ */
  enum OnConvertOwnerAction {
    cnv_Self = 0, ///< \ru Поведение, определяемое виртуальной функцией OnConvertOwner. \en Behavior defined by the virtual function OnConvertOwner. 
    cnv_Free,     ///< \ru Освободить атрибут, если это возможно, в противном случае удалить. \en Free attribute if it is possible, otherwise delete it. 
    cnv_Keep,     ///< \ru Сохранить атрибут, т.е. ничего с ним не делать. \en Save attribute, i.e. do not do anything with it. 
    cnv_Copy,     ///< \ru Скопировать атрибут и прицепить его копию к копии владельца. \en Copy an attribute and attach its copy to an owner copy. 
    cnv_Convert,  ///< \ru Конвертировать атрибут и прицепить результат к копии владельца. \en Copy an attribute and attach the result to an owner copy. 
    cnv_ActCount, ///< \ru Количество элементов в перечислении (добавлять перед данным значением). \en The number of elements in enumeration (add before the given value). 
  };

  /**\ru Поведение атрибута при преобразовании владельца (по матрице).
     \en Behaviour of attribute when transforming the owner (by the matrix). \~ */
  enum OnTransformOwnerAction {
    trn_Self = 0, ///< \ru Поведение, определяемое виртуальной функцией OnTransformOwner. \en Behavior defined by the virtual function OnTransformOwner. 
    trn_Free,     ///< \ru Освободить атрибут, если это возможно, в противном случае удалить. \en Free attribute if it is possible, otherwise delete it. 
    trn_Keep,     ///< \ru Сохранить атрибут, т.е. ничего с ним не делать. \en Save attribute, i.e. do not do anything with it. 
    trn_ActCount, ///< \ru Количество элементов в перечислении (добавлять перед данным значением). \en The number of elements in enumeration (add before the given value). 
  };

  /**\ru Поведение атрибута при копировании владельца.
     \en Behaviour of attribute when copying the owner. \~ */
  enum OnCopyOwnerAction {
    cpy_Self = 0, ///< \ru Поведение, определяемое виртуальной функцией OnCopyOwner. \en Behavior defined by the virtual function OnCopyOwner. 
    cpy_Free,     ///< \ru Освободить атрибут, если это возможно, в противном случае удалить. \en Free attribute if it is possible, otherwise delete it. 
    cpy_Keep,     ///< \ru Сохранить атрибут, т.е. ничего с ним не делать. \en Save attribute, i.e. do not do anything with it. 
    cpy_Copy,     ///< \ru Скопировать атрибут и прицепить его копию к копии владельца. \en Copy an attribute and attach its copy to an owner copy. 
    cpy_ActCount, ///< \ru Количество элементов в перечислении (добавлять перед данным значением). \en The number of elements in enumeration (add before the given value). 
  };

  /**\ru Поведение атрибута при объединении владельца с другим объектом.
     \en Behaviour of attribute when merging of the owner with another object. \~ */
  enum OnMergeOwnerAction {
    mrg_Self = 0, ///< \ru Поведение, определяемое виртуальной функцией OnMergeOwner. \en Behavior is defined by the virtual function OnMergeOwner. 
    mrg_Free,     ///< \ru Освободить атрибут, если это возможно, в противном случае удалить. \en Free attribute if it is possible, otherwise delete it. 
    mrg_Keep,     ///< \ru Сохранить атрибут, т.е. ничего с ним не делать. \en Save attribute, i.e. do not do anything with it. 
    mrg_KeepAll,  ///< \ru Передать атрибут от поглощаемого объекта поглощающему объекту без замещения. \en Transmit attribute from absorbed object to absorbing object without replacing. 
    mrg_KeepRep,  ///< \ru Передать атрибут от поглощаемого объекта поглощающему объекту с замещением. \en Transmit attribute from absorbed object to absorbing object with replacing. 
    mrg_ActCount, ///< \ru Количество элементов в перечислении (добавлять перед данным значением). \en The number of elements in enumeration (add before the given value). 
  };

  /**\ru Поведение атрибута при замещении владельца с другим объектом.
     \en Behavior of attribute when replacing the owner by another object. \~ */
  enum OnReplaceOwnerAction {
    rep_Self = 0, ///< \ru Поведение, определяемое виртуальной функцией OnReplaceOwner. \en Behavior is defined by the virtual function OnReplaceOwner. 
    rep_Free,     ///< \ru Освободить атрибут, если это возможно, в противном случае удалить. \en Free attribute if it is possible, otherwise delete it. 
    rep_Keep,     ///< \ru Сохранить атрибут, т.е. ничего с ним не делать. \en Save attribute, i.e. do not do anything with it. 
    rep_KeepAll,  ///< \ru Передать атрибут от замещаемого объекта замещающему объекту без замещения. \en Transmit attribute from replaced object to substitutional object without replacing. 
    rep_KeepRep,  ///< \ru Передать атрибут от замещаемого объекта замещающему объекту с замещением. \en Transmit attribute from replaced object to substitutional object with replacing. 
    rep_ActCount, ///< \ru Количество элементов в перечислении (добавлять перед данным значением). \en The number of elements in enumeration (add before the given value). 
  };

  /**\ru Поведение атрибута при разделении владельца.
     \en Behavior of attribute when splitting the owner. \~ */
  enum OnSplitOwnerAction {
    spl_Self = 0, ///< \ru Поведение, определяемое виртуальной функцией OnSplitOwner. \en Behavior is defined by the virtual function OnSplitOwner. 
    spl_Free,     ///< \ru Освободить атрибут, если это возможно, в противном случае удалить. \en Free attribute if it is possible, otherwise delete it. 
    spl_Keep,     ///< \ru Сохранить атрибут, т.е. ничего с ним не делать. \en Save attribute, i.e. do not do anything with it. 
    spl_Copy,     ///< \ru Размножить(скопировать) атрибут для каждого результата разбиения. \en Duplicate (copy) attribute for each result of splitting. 
    spl_ActCount, ///< \ru Количество элементов в перечислении (добавлять перед данным значением). \en The number of elements in enumeration (add before the given value). 
  };

  /**\ru Поведение атрибута при удалении владельца.
     \en Behavior of attribute when deleting the owner. \~ */
  enum OnDeleteOwnerAction {
    del_Self = 0, ///< \ru Поведение, определяемое виртуальной функцией OnDeleteOwner. \en Behavior defined by the virtual function OnDeleteOwner. 
    del_Free,     ///< \ru Освободить атрибут, если это возможно, в противном случае удалить. \en Free attribute if it is possible, otherwise delete it. 
    del_ActCount, ///< \ru Количество элементов в перечислении (добавлять перед данным значением). \en The number of elements in enumeration (add before the given value). 
  };

private :
  uint8 forChange;    ///< \ru Поведение атрибута при изменении владельца. \en Behavior of attribute when changing the owner. 
  uint8 forConvert;   ///< \ru Поведение атрибута при конвертации владельца. \en Behavior of attribute when converting the owner. 
  uint8 forTransform; ///< \ru Поведение атрибута при трансформировании владельца. \en Behavior of attribute when transforming the owner. 
  uint8 forCopy;      ///< \ru Поведение атрибута при копировании владельца. \en Behavior of attribute when copying the owner. 
  uint8 forMerge;     ///< \ru Поведение атрибута при объединении владельца. \en Behavior of attribute when merging the owner. 
  uint8 forReplace;   ///< \ru Поведение атрибута при замене владельца. \en Behavior of attribute when replacing the owner. 
  uint8 forSplit;     ///< \ru Поведение атрибута при разделении владельца. \en Behavior of attribute when splitting the owner. 
  uint8 forDelete;    ///< \ru Поведение атрибута при удалении владельца. \en Behavior of attribute when deleting the owner. 
  bool  freeable;     ///< \ru Свободность атрибута. \en Attribute freeness 
  bool  copyable;     ///< \ru Разрешение копировать атрибут. \en Permission to copy attribute. 

protected :
  /// \ru Конструктор без параметров для наследников. \en Constructor without parameters for inheritors. 
  MbAttribute();
  /// \ru Конструктор копирования для наследников. \en Copy constructor for inheritors. 
  MbAttribute( const MbAttribute & );
private:
  /// \ru Оператор присваивания (не реализован. \en Assignment operator (not implemented). 
  const MbAttribute & operator = ( const MbAttribute & );
public :
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbAttribute();

public :
  /** \ru \name Общие функции атрибутов
      \en \name Common functions of attributes
      \{ */
  /// \ru Выдать регистрационный тип (для копирования, дублирования). \en Get registrational type (for copying, duplication) 
  MbeRefType RefType() const override;
  /// \ru Выдать тип контейнера атрибутов. \en Get attribute container type. 
  virtual MbeImplicationType ImplicationType() const; 
  /// \ru Выдать тип атрибута. \en Get attribute type. 
  virtual MbeAttributeType AttributeFamily() const = 0;    
  /// \ru Выдать подтип атрибута. \en Get subtype of an attribute.
  virtual MbeAttributeType AttributeType() const = 0; 
  /// \ru Сделать копию элемента. \en Create a copy of the element. 
  virtual MbAttribute & Duplicate( MbRegDuplicate * = nullptr ) const = 0;
  /** \brief \ru Определить, являются ли объекты равными.
             \en Determine whether objects are equal. \~
    \details \ru Равными считаются однотипные объекты, все данные которых одинаковы (равны).
             \en Objects of the same types with similar (equal) data are considered to be equal. \~
    \param[in] item - \ru Объект для сравнения.
                      \en Objects for comparison. \~
    \param[in] accuracy - \ru Точность сравнения.
                          \en The accuracy to compare. \~
    \return \ru Равны ли объекты.
            \en Whether objects are equal. \~
  */
  virtual bool    IsSame( const MbAttribute & item, double accuracy ) const = 0; 
  /// \ru Инициализировать данные по присланным. \en Initialize data. 
  virtual bool    Init( const MbAttribute & ) = 0; 

          /// \ru Проверить тип атрибута. \en Check an attribute type. 
          bool    IsA( MbeAttributeType t ) const { return t == AttributeFamily(); }
  /** \} */

  /** \ru \name Действия над объектами геометрического ядра, влияющие на состояние атрибутов
      \en \name Actions with objects of geometric kernel influencing on states of attributes.
      \{ */
  /** \brief \ru Выполнить действия при изменении владельца, не связанное с другими действиями.
             \en Perform actions which are not associated with other actions when changing the owner. \~
      \details \ru Действия при изменении владельца, не связанное с другими действиями. \n
      Вызывается после изменения владеющего объекта при условии GetActionForChange() == chn_Self.
               \en Actions which are not associated with other actions when changing the owner. \n
      This function is called after changing the owning object in a case when GetActionForChange() == chn_Self. \~ */
  virtual void    OnChangeOwner( const MbAttributeContainer & owner ) = 0;

  /**\ru Выполнить действия при конвертации владельца, \n
      Вызывается после конвертирования владеющего объекта при условии GetActionForConvert() == cnv_Self. \n
      В качестве входного параметра передается результат конвертирования объекта.
     \en Perform actions when converting the owner, \n
      This function is called after converting the owning object in a case when GetActionForConvert() == cnv_Self. \n
      The result of object converting is passed as input parameter. \~ */
  virtual void    OnConvertOwner( const MbAttributeContainer & owner, MbAttributeContainer & other ) = 0; 

  /**\ru Выполнить действия при трансформировании владельца, \n 
      Вызывается после трансформирования владеющего объекта при условии GetActionForTransform() == trn_Self.
      В качестве входного параметра может передаваться регистратор трансформированных объектов.
     \en Perform actions when transforming the owner, \n 
      This function is called after transforming the owning object in a case when GetActionForTransform() == trn_Self.
      The registrator of transformed objects may be passed as input parameter. \~ */
  virtual void    OnTransformOwner( const MbAttributeContainer & owner, const MbMatrix3D &, MbRegTransform * = nullptr ) = 0;

  /**\ru Выполнить действия при перемещении владельца. \n 
      Вызывается после перемещения владеющего объекта при условии GetActionForTransform() == trn_Self.
      В качестве входного параметра может передаваться регистратор трансформированных объектов.
     \en Perform actions when moving the owner. \n 
      This function is called after moving the owning object in a case when GetActionForTransform() == trn_Self.
      The registrator of transformed objects may be passed as input parameter. \~ */
  virtual void    OnMoveOwner( const MbAttributeContainer & owner, const MbVector3D &, MbRegTransform * = nullptr ) = 0;

  /**\ru Выполнить действия при вращении владельца. \n 
      Вызывается после вращения владеющего объекта при условии GetActionForTransform() == trn_Self.
      В качестве входного параметра может передаваться регистратор трансформированных объектов.
     \en Perform actions when rotating the owner. \n 
      This function is called after rotating the owning object in a case when GetActionForTransform() == trn_Self.
      The registrator of transformed objects may be passed as input parameter. \~ */
  virtual void    OnRotateOwner( const MbAttributeContainer & owner, const MbAxis3D &, double angle, MbRegTransform * = nullptr ) = 0;

  /**\ru Выполнить действия при копировании владельца. \n 
      Вызывается после копирования владеющего объекта при условии GetActionForCopy() == cpy_Self. \n
      В качестве входных параметров передаются: копия владеющего объекта и регистратор скопированных объектов.
     \en Perform actions when copying the owner. \n 
      This function is called after copying the owning object in a case when GetActionForCopy() == cpy_Self. \n
      The following objects are passed as input parameters: the owning object copy and registrator of copied objects. \~ */
  virtual void    OnCopyOwner( const MbAttributeContainer & owner, MbAttributeContainer & other, MbRegDuplicate * = nullptr ) = 0;
  
  /**\ru Выполнить действия при объединении владельца. \n 
      Вызывается перед слиянием владельца при условии GetActionForMerge() == mrg_Self. \n
      В качестве входного параметра передается объект который будет поглощен.
     \en Perform actions when merging the owner. \n 
      This function is called before merging the owner in a case when GetActionForMerge() == mrg_Self. \n
      The object which will be absorbed is passed as input parameter. \~ */
  virtual void    OnMergeOwner( const MbAttributeContainer & owner, MbAttributeContainer & other ) = 0; 

  /**\ru Выполнить действия при замене владельца. \n 
      Вызывается перед выполнением замены владельца при условии GetActionForReplace() == rep_Self. \n
      В качестве входного параметра передается объект - заместитель.
     \en Perform actions when replacing the owner. \n 
      This function is called before replacing the owner in a case when GetActionForReplace() == rep_Self. \n
      The substitutional object is passed as input parameter. \~ */
  virtual void    OnReplaceOwner( const MbAttributeContainer & owner, MbAttributeContainer & other ) = 0; 

  /**\ru Выполнить действия при разделении владельца. \n 
      Вызывается после разбиения владеющего объекта при условии GetActionForSplit() == spl_Self. \n
      В качестве входного параметра передается контейнер результатов разбиения.
     \en Perform actions when splitting the owner. \n 
      This function is called after splitting the owning object in a case when GetActionForSplit() == spl_Self. \n
      The container of splitting results is passed as input parameter. \~ */
  virtual void    OnSplitOwner( const MbAttributeContainer & owner, const std::vector<MbAttributeContainer*> & others ) = 0; 

  /**\ru Выполнить действия при удалении владельца. \n 
      Вызывается перед удалением объекта при условии GetActionForDelete() == spl_Self.
     \en Perform actions when deleting the owner. \n 
      This function is called before deleting the owner in a case when GetActionForDelete() == spl_Self. \~ */
  virtual void    OnDeleteOwner( const MbAttributeContainer & owner ) = 0; 
  /** \} */

  /// \ru Выдать поведение атрибута при изменении владельца. \en Get behavior of attribute when changing the owner. 
  OnChangeOwnerAction    GetActionForChange   () const { return static_cast<OnChangeOwnerAction>(forChange); } 
  /// \ru Выдать поведение атрибута при конвертации владельца. \en Get behavior of attribute when converting the owner. 
  OnConvertOwnerAction   GetActionForConvert  () const { return static_cast<OnConvertOwnerAction>(forConvert); }
  /// \ru Выдать поведение атрибута при трансформировании владельца. \en Get behavior of attribute when transforming the owner. 
  OnTransformOwnerAction GetActionForTransform() const { return static_cast<OnTransformOwnerAction>(forTransform); } 
  /// \ru Выдать поведение атрибута при копировании владельца. \en Get behavior of attribute when copying the owner. 
  OnCopyOwnerAction      GetActionForCopy     () const { return static_cast<OnCopyOwnerAction>(forCopy); } 
  /// \ru Выдать поведение атрибута при объединении владельца. \en Get behavior of attribute when merging the owner. 
  OnMergeOwnerAction     GetActionForMerge    () const { return static_cast<OnMergeOwnerAction>(forMerge); } 
  /// \ru Выдать поведение атрибута при замене владельца. \en Get behavior of attribute when replacing the owner. 
  OnReplaceOwnerAction   GetActionForReplace  () const { return static_cast<OnReplaceOwnerAction>(forReplace); } 
  /// \ru Выдать поведение атрибута при разделении владельца. \en Get behavior of attribute when splitting the owner. 
  OnSplitOwnerAction     GetActionForSplit    () const { return static_cast<OnSplitOwnerAction>(forSplit); } 
  /// \ru Выдать поведение атрибута при удалении владельца. \en Get behavior of attribute when deleting the owner. 
  OnDeleteOwnerAction    GetActionForDelete   () const { return static_cast<OnDeleteOwnerAction>(forDelete); } 

          /// \ru Задать поведение атрибута при изменении владельца. \en Set behavior of attribute when changing the owner. 
          void    SetActionForChange   ( OnChangeOwnerAction    a ) { forChange    = (uint8)a; } 
          /// \ru Задать поведение атрибута при конвертации владельца. \en Set behavior of attribute when converting the owner. 
          void    SetActionForConvert  ( OnConvertOwnerAction   a ) { forConvert   = (uint8)a; } 
          /// \ru Задать поведение атрибута при трансформировании владельца. \en Set behavior of attribute when transforming the owner. 
          void    SetActionForTransform( OnTransformOwnerAction a ) { forTransform = (uint8)a; } 
          /// \ru Задать поведение атрибута при копировании владельца. \en Set behavior of attribute when copying the owner. 
          void    SetActionForCopy     ( OnCopyOwnerAction      a ) { forCopy      = (uint8)a; } 
          /// \ru Задать поведение атрибута при объедении владельца. \en Set behavior of attribute when merging the owner. 
          void    SetActionForMerge    ( OnMergeOwnerAction     a ) { forMerge     = (uint8)a; } 
          /// \ru Задать поведение атрибута при замене владельца. \en Set behavior of attribute when replacing the owner. 
          void    SetActionForReplace  ( OnReplaceOwnerAction   a ) { forReplace   = (uint8)a; } 
          /// \ru Задать поведение атрибута при разбиении владельца. \en Set behavior of attribute when splitting the owner. 
          void    SetActionForSplit    ( OnSplitOwnerAction     a ) { forSplit     = (uint8)a; } 
          /// \ru Задать поведение атрибута при удалении владельца. \en Set behavior of attribute when deleting the owner. 
          void    SetActionForDelete   ( OnDeleteOwnerAction    a ) { forDelete    = (uint8)a; } 

          /// \ru Определить поведение атрибута по другому атрибуту. \en Define behavior of an attribute by another attribute. 
          void    InitActions          ( const MbAttribute & ); 

          bool    CanBeFree  () const { return freeable; }
          bool    CanBeCopied() const { return copyable; }

          void    SetCanBeFree  ( bool b ) { freeable = b; }
          void    SetCanBeCopied( bool b ) { copyable = b; }

  /// \ru Выдать свойства объекта. \en Get properties of the object. 
  virtual void    GetProperties( MbProperties & ); 
  /// \ru Установить свойства объекта. \en Set properties of object. 
  virtual size_t  SetProperties( const MbProperties & ); 
  /// \ru Выдать заголовок свойства объекта. \en Get a name of object property. 
  virtual MbePrompt GetPropertyName() = 0; 

          bool    IsFamilyRegistrable() const override;

DECLARE_PERSISTENT_CLASS( MbAttribute )
};

IMPL_PERSISTENT_OPS( MbAttribute )


//------------------------------------------------------------------------------
/** \brief \ru Объект для свойств.
           \en Object for properties. \~
  \details \ru Объект для свойств. \n
           \en Object for properties. \n \~
  \ingroup Model_Attributes
*/
// ---
class MATH_CLASS MbAttributeAction : public MbRefItem {
private :
  uint8 & forChange;    ///< \ru Поведение атрибута при изменении владельца. \en Behavior of attribute when changing the owner. 
  uint8 & forConvert;   ///< \ru Поведение атрибута при конвертации владельца. \en Behavior of attribute when converting the owner. 
  uint8 & forTransform; ///< \ru Поведение атрибута при трансформировании владельца. \en Behavior of attribute when transforming the owner. 
  uint8 & forCopy;      ///< \ru Поведение атрибута при копировании владельца. \en Behavior of attribute when copying the owner. 
  uint8 & forMerge;     ///< \ru Поведение атрибута при объединении владельца. \en Behavior of attribute when merging the owner. 
  uint8 & forReplace;   ///< \ru Поведение атрибута при замене владельца. \en Behavior of attribute when replacing the owner. 
  uint8 & forSplit;     ///< \ru Поведение атрибута при разделении владельца. \en Behavior of attribute when splitting the owner. 
  uint8 & forDelete;    ///< \ru Поведение атрибута при удалении владельца. \en Behavior of attribute when deleting the owner. 
  bool  & freeable;     ///< \ru Свободность атрибута. \en Attribute freeness 
  bool  & copyable;     ///< \ru Разрешение копировать атрибут. \en Permission to copy attribute. 

public:
  /// \ru Конструктор с параметрами. \en Constructor with parameters. 
  MbAttributeAction( uint8 & cha, uint8 & con, uint8 & tra, uint8 & cop, uint8 & mer, uint8 & rep, uint8 & spl, uint8 & del, 
                     bool & fre, bool & cob )
    : MbRefItem   (     )
    , forChange   ( cha )
    , forConvert  ( con )
    , forTransform( tra ) 
    , forCopy     ( cop )
    , forMerge    ( mer )
    , forReplace  ( rep )
    , forSplit    ( spl )
    , forDelete   ( del )
    , freeable    ( fre )
    , copyable    ( cob )
  {}
  /// \ru Деструктор. \en Destructor. 
  ~MbAttributeAction() {}

public:
  /// \ru Выдать свойства объекта. \en Get properties of the object. 
  void    GetProperties( MbProperties & ) const;
  /// \ru Установить свойства объекта. \en Set properties of object. 
  void    SetProperties( const MbProperties & );

OBVIOUS_PRIVATE_COPY( MbAttributeAction )
};


//------------------------------------------------------------------------------
// \ru Системные строки атрибутов. \en System strings of attributes. 
// ---
namespace c3d // namespace C3D
{
  /// \ru Подсказка для эквидистантной грани c нулевым значением эквидистанты. \en Hint for an offset face with the null value of offset. 
  constexpr TCHAR c3dStr_ShellFace[]   = _T( "c3d_ShellFace" );
  /// \ru Подсказка для эквидистантной грани. \en Hint for an offset face. 
  constexpr TCHAR c3dStr_OffsetFace[]  = _T( "c3d_OffsetFace" );
  /// \ru Подсказка для вскрываемой грани. \en Hint for an open face. 
  constexpr TCHAR c3dStr_OpenFace[]    = _T( "c3d_OpenFace" );
  /// \ru Подсказка для доп.эквидистантного смещения слипшейся грани. \en Hint for an offset of a stuck face. 
  constexpr TCHAR c3dStr_StuckOffset[] = _T( "c3d_StuckOffset" );
  /// \ru Подсказка для удаляемой слипшейся грани. \en Hint for a deleted stuck face. 
  constexpr TCHAR c3dStr_StuckDelete[] = _T( "c3d_StuckDelete" );

  /// \ru Подсказка для изначально граничного ребра. \en Hint for initially boundary edge. 
  constexpr TCHAR c3dStr_InitBoundaryEdge[] = _T( "c3d_InitBoundaryEdge" );
  /// \ru Подсказка для расшивки граней по ребру. \en Hint for separation neighbour faces by an edge. 
  constexpr TCHAR c3dStr_UnstitchByEdge[]      = _T( "c3d_UnstitchByEdge" );
  /// \ru Подсказка для проверки идентификатора боковой грани. \en Hint for checking flank's identifier. 
  constexpr TCHAR c3dStr_CheckFlankId[]        = _T( "c3d_CheckFlankId" );
  /// \ru Подсказка для порядкового номера оболочки. \en Hint for shell sequence number. 
  constexpr TCHAR c3dStr_ShellSequenceNumber[] = _T( "c3d_ShellSequenceNumber" );
  /// \ru Подсказка для сохраняемого объекта. \en Hint for kept object. 
  constexpr TCHAR c3dStr_KeptObject[]          = _T( "c3d_KeptObject" );
  /// \ru Подсказка для удаляемого объекта. \en Hint for deleting object. 
  constexpr TCHAR c3dStr_DeletingObject[]      = _T( "c3d_DeletingObject" );
  /// \ru Подсказка для временного объекта. \en Hint for temporal object. 
  constexpr TCHAR c3dStr_TemporalObject[]      = _T( "c3d_TemporalObject" );
  /// \ru Подсказка для временного маркера слитых атрибутов. \en Hint for a temporary marker of merged attributes. 
  constexpr TCHAR c3dStr_MergedStateMarker[]   = _T( "c3d_MergedStateMarker" );

  /**\ru Для плоской грани, сгибаемой в цилиндр - параметр u, который меньше соответствующего параметра любой точки грани,
                         сгибаемой в конус - угловой параметр луча, выходящего из начала координат плоскости параметров и не пересекающего контуры грани.
     \en For a planar face bended in cylinder - u-parameter which is less than corresponding parameter of any point on the face,
                         bended in cone - angular parameter of the ray which  goes out from the parameters plane origin and does not intersect contours of the face. \~*/
  constexpr TCHAR c3dStr_BendMinAnlge[]   = _T( "BendMinAnlge" );
  /// \ru Для цилиндрической и конической грани параметр u, который меньше соответствующего параметра любой точки грани. \en For a cylindrical and conical face - parameter u which is less than corresponding parameter of any point on the face. 
  constexpr TCHAR c3dStr_UnbendMinAngle[] = _T( "UnbendMinAngle" );
  /// \ru Подсказка для продленной грани. \en Hint for extended face.
  constexpr TCHAR c3dStr_ExtendedFace[]   = _T( "c3d_ExtendedFace" );
  /// \ru Подсказка замкнутости образующей к осевой. \en Hint for center line section closure.
  constexpr TCHAR c3dStr_CLSecClosure[]   = _T( "c3d_CLSecClosure" );

  /// \ru Подсказка для контрольного значения массы.                \en Prompt for the mass validation property.
  constexpr TCHAR c3dStr_ValidationPropertyMassExchange[]        = _T( "c3d_ValidationPropertyMassExchange" );
  /// \ru Подсказка для контрольного значения объёма.               \en Prompt for the volume validation property.
  constexpr TCHAR c3dStr_ValidationPropertyVolumeExchange[]      = _T( "c3d_ValidationPropertyVolumeExchange" );
  /// \ru Подсказка для контрольного значения площади поверхности.  \en Prompt for the surface area validation property.
  constexpr TCHAR c3dStr_ValidationPropertySurfaceAreaExchange[] = _T( "c3d_ValidationPropertySurfaceAreaExchange" );

  /// \ru Подсказка для идентификатора элемента при обмене данными. \en Prompt for the item identifier in model exchange operations.
  constexpr TCHAR c3dStr_ItemIdentifierExchange[]                = _T( "c3d_ItemIdentifierExchange" );
  /// \ru Подсказка для URL текстуры при обмене данными.            \en Prompt for the URL of texture in model exchange operations.
  constexpr TCHAR c3dStr_TextureUrl[]                            = _T( "C3D_Texture_URL" );
  /// \ru Подсказка для идентификатора компонента.                  \en Prompt for the item identifier.
  constexpr TCHAR c3dStr_UniqueCompId[]                          = _T( "c3d_UniqueCompID" );

  /// \ru Подсказка для типа замкнутости оболочки в исходных данных. \en Prompt for shell closure type in source data.
  constexpr TCHAR c3d_ShellOpenClosedOriginal[]                  = _T( "c3d_ShellOpenClosedOriginal" );

  /// \ru Подсказка для атрибута хот-точки в уклоне.                \en Prompt for attribute of hot point in draft operations.
  constexpr TCHAR c3d_DraftOperationHotPoint[]                   = _T( "c3d_DraftOperationHotPoint" );
  /// \ru Подсказка для атрибута простой хот-точки.                \en Prompt for attribute of simple hot point.
  constexpr TCHAR c3d_OperationSingleHotPoint[]                  = _T( "c3d_OperationSingleHotPoint" );
  /// \ru Подсказка для атрибута  композитной хот-точки.           \en Prompt for attribute of composite hot point.
  constexpr TCHAR c3d_OperationCompositeHotPoint[]               = _T("c3d_OperationCompositeHotPoint");

  /// \ru Подсказка для атрибута гладкости ребра. \en Hint for an edge smoothness attribute. 
  constexpr TCHAR c3dStr_EdgeSmoothnessInfo[]                    = _T( "c3d_EdgeSmoothnessInfo" );
  /// \ru Подсказка для атрибута резьбовой грани. \en Hint for an thread face carrier attribute. 
  constexpr TCHAR c3dStr_BorerThreadCarrier[]                    = _T( "c3d_BorerThreadCarrier" );

  /// \ru Подсказка для атрибута имени ребра, используемого для восстановления имён скруглений. \en Hint for the edge name attribute used to recover fillet names. 
  constexpr TCHAR c3dStr_EdgeForFilletNamesRecover[]             = _T( "c3d_EdgeEdgeForFilletNamesRecover" );

  /// \ru Подсказки для сохраненного текущего вида. \en Hints for the saved current view.

  /** \ru Подсказка для системы координат текущего вида. 
          Хранится в виде массива из 12 doubles (3D-точка начала координат, 3D-вектора осей X, Y, Z).
      \en Hint for the local coordinate system of the current view.
          Stored as an array of 12 doubles (3D-point of the coordinate system origin, 3D-vectors for X, Y, Z axes). \~ */
  constexpr TCHAR c3dStr_SavedViewPlacement[]                    = _T( "c3d_SavedView_Placement" );
  /** \ru Подсказка для вектора направления вверх текущего вида. Хранится в виде массива из 3 doubles.
      \en  Hint for the up-vector of the current view. Stored as an array of 3 doubles. \~*/
  constexpr TCHAR c3dStr_SavedViewUpVector[]                     = _T( "c3d_SavedView_UpVector" );
  /** \ru Подсказка для масштаба текущего вида. \en Hint for the scale of the current view. \~*/
  constexpr TCHAR c3dStr_SavedViewScale[]                        = _T( "c3d_SavedView_Scale" );
  /** \ru Подсказка для типа текущего вида (ортогональный/перспективный).  Хранится в виде значения bool.
      \en Hint for the type of the current view (orthogonal/perspective). Stored as a bool value. \~*/
  constexpr TCHAR c3dStr_SavedViewPerspective[]                  = _T( "c3d_SavedView_Perspective" );
} // namespace C3D


#endif // __ATTRIBUTE_ITEM_H
