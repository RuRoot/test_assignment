////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Контейнер атрибутов.
         \en An attribute container. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __ATTRIBUTE_CONTAINER_H
#define __ATTRIBUTE_CONTAINER_H


#include <attribute_item.h>
#include <mb_enum.h>
#include <vector>
#include <templ_multimap.h>
#include <attr_registry.h>


class    reader;
class    writer;
class    MbVector3D;
class    MbAxis3D;
class    MbMatrix3D;
class    MbAttribute;
class    MbUserAttribute;
class    MbExternalAttribute;
class    MbRGBA;
class    MbProperties;
class    MbRegDuplicate;
class    MbRegTransform;


//------------------------------------------------------------------------------
/** \brief \ru Контейнер атрибутов.
           \en An attribute container. \~
  \details \ru Контейнер атрибутов. \n
    От данного класса наследуются объекты модели геометрического ядра MbItem
    и топологические объекты с именем MbTopologyItem .\n
    Наследники данного класса содержат атрибуты.\n
    Методами данного класса выполняются действия над атрибутами объектов геометрического ядра.\n
    Атрибут может влиять на состояние атрибута через его владельца, 
    то есть геометрическое ядро предусматривает возможность передачи атрибутам информации об изменениях 
    их владельцев посредством вызовов предопределенных функций у самого атрибута.\n
    Кроме передачи самой информации об изменениях происходящих с владельцем, 
    предусмотрена возможность определять поведение атрибута при этих изменениях путем выбора 
    одного из предопределенных типов поведения на каждое изменения владельца.\n
    Типы действий, влияющих на состояние атрибутов.\n
    Копирование, например, при создании копии тела. Действие над атрибутом производится после копирования владеющего объекта.\n 
    Разделение, например, разделение грани на две части при вырезании. 
    Действие над атрибутом производится после разбиения владеющего объекта.\n
    Слияние, например, слияние граней при булевых операциях. 
    Действие над атрибутом производится перед выполнением слияния объектов. 
    Обрабатываются атрибуты всех объектов, участвующих в слиянии.\n
    Изменение, не связанное с разделением или слиянием. 
    Действие над атрибутом производится после изменения владеющего объекта.\n
    Преобразование, например, поворот или параллельный перенос. 
    Действие над атрибутом производится после преобразования владеющего объекта.\n
    Подмена, например замена одной грани тела на другую. Действие над атрибутом производится перед выполнением замены объектов. 
    Обрабатываются атрибуты всех объектов, участвующих в замене.\n
    Удаление объекта. Действие над атрибутом производится перед удалением объекта.\n
           \en An attribute container. \n
    The inheritors of this class are: objects of geometric kernel model of type MbItem
    and topological objects of type MbTopologyItem.\n
    Inheritors of this class contain attributes.\n
    Operations with attributes of geometric kernel objects are performed by methods of this class.\n
    Attribute can affect attribute state using its owner, 
    i.e. geometric kernel provides an opportunity for transmission to attributes the information about changes 
    of their owners by calling the predefined functions of the attribute.\n
    In addition to transfer of information about changes occurring with owner 
    provided a possibility to determine the behavior of attribute with these changes by selecting of 
    one of the predefined types of behavior for each changing of the owner.\n
    Types of actions that affect the states of attributes.\n
    Copying, for example, when creating a copy of solid. Action on attribute is performed after copying of owning object.\n 
    Splitting. For example, splitting of a face into two parts in cutting. 
    Action on attribute is performed after splitting of owning object.\n
    Merging. For example, merging of faces in boolean operations. 
    Action on attribute is performed after merging of owning object.\n 
    Attributes of all objects involved in merging are processed.\n
    Changing which is not associated with splitting or merging. 
    Action on attribute is performed after changing of owning object.\n
    Transformation. For example, rotation or parallel translation. 
    Action on attribute is performed after transformation of owning object.\n
    Replacement. For example, replacement of one face of a solid to another. Action on attribute is performed after replacement of objects. 
    Attributes of all objects involved in replacement are processed.\n
    Deletion of an object. Action on attribute is performed after deletion of an object.\n \~ 
  \ingroup Model_Attributes
*/
// ---
class MATH_CLASS MbAttributeContainer {
public:
typedef MultiMap<int, MbAttribute *> AttrMap_t;
private:
  AttrMap_t attributes; ///< \ru Множество атрибутов. \en Set of attributes.  

protected:
  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  MbAttributeContainer( const MbAttributeContainer &, MbRegDuplicate * );
public:
  /// \ru Конструктор без параметров. \en Constructor without parameters. 
  MbAttributeContainer();
  /// \ru Конструктор по атрибуту. \en Constructor by attribute. 
  MbAttributeContainer( MbAttribute & );
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbAttributeContainer();

public:

  /// \ru Выдать тип контейнера атрибутов. \en Get attribute container type. 
  virtual MbeImplicationType ImplicationType() const { return ace_AttribContainer; } 

  /** \ru \name Общие функции над атрибутами
      \en \name Common functions of attributes
      \{ */
          /** \brief \ru Дублировать атрибуты присланного объекта, свои отпустить.
                     \en Duplicate attributes of a given object, release existing attributes. \~
              \details \ru Дублировать атрибуты присланного объекта, свои отпустить. \n
                       \en Duplicate attributes of a given object, release existing attributes. \n \~
              \param[in] src - \ru Присланный контейнер атрибутов для копирования.
                               \en Attribute container for copying to this attribute container . \~
              \param[in] forcedAssign - \ru Если флаг равен false, то обобщенный атрибут не будет добавлен, если в настройках его поведения есть запрет на размножение.
                                        \en If the flag is false, then the generic attribute will not be added if its behavior settings prohibit replication. \~
          */
          void        AttributesAssign( const MbAttributeContainer & src, bool forcedAssign = false ); 
          /// \ru Выдать количество объектов. \en Get the number of objects. 
          size_t      AttributesCount() const { return attributes.Count(); } 
          /// \ru Удалить все атрибуты из контейнера. \en Delete all attributes from container. 
          bool        RemoveAttributes( bool onDeleteOwner = false ); 

          /** \brief \ru Добавить атрибут в контейнер.
                     \en Add attribute in container. \~
              \details \ru Добавить атрибут в контейнер (добавляет оригинал атрибута, если это возможно).
                           В случае простого атрибута, если он уже есть, его данные заменены данными входного атрибута.
                           Во избежание утечек рекомендуется владеть атрибутом по счетчику ссылок (явно или через SPtr). \n
                       \en Add attribute in container (adds the original attribute if it's possible).
                           In the case of a simple attribute, if it already exists, its data is replaced by the data of the input attribute. 
                           To avoid leaks, it is recommended to own the attribute by reference count (explicitly or via SPtr). \n \~
              \param[in] attr - \ru Атрибут.
                                \en Attribute. \~
              \param[in] checkSame - \ru Флаг поиска такого же по содержанию атрибута в контейнере. Атрибут не будет добавлен, если найден такой же.
                                     \en Search flag for an attribute containing the same content. Attribute will not be added if the same attribute is found. \~
              \param[in] forcedAssign - \ru Если флаг равен false, то обобщенный атрибут не будет добавлен, если в настройках его поведения есть запрет на размножение.
                                        \en If the flag is false, then the generic attribute will not be added if its behavior settings prohibit replication. \~
              \return \ru Возвращает указатель на добавленный атрибут или нулевой указатель.
                      \en Returns a pointer to the added attribute or null pointer. \~
          */
          MbAttribute * AddAttribute( MbAttribute * attr, bool checkSame = true, bool forcedAssign = false );

          /** \brief \ru Добавить атрибут в контейнер (добавляет копию атрибута, если его можно добавить).
                     \en Add attribute in container (adds a copy of the attribute if it can be added). \~
              \details \ru Добавить атрибут в контейнер (добавляет копию атрибута, если его можно добавить).
                           В случае простого атрибута, если он уже есть, его данные заменены данными входного атрибута. \n
                       \en Add attribute in container (adds a copy of the attribute if it can be added).
                           In the case of a simple attribute, if it already exists, its data is replaced by the data of the input attribute. \n \~
              \param[in] attr - \ru Атрибут.
                                \en Attribute. \~
              \param[in] checkSame - \ru Флаг поиска такого же по содержанию атрибута в контейнере. Атрибут не будет добавлен, если найден такой же.
                                     \en Search flag for an attribute containing the same content. Attribute will not be added if the same attribute is found. \~
              \param[in] forcedAssign - \ru Если флаг равен false, то обобщенный атрибут не будет добавлен, если в настройках его поведения есть запрет на размножение.
                                        \en If the flag is false, then the generic attribute will not be added if its behavior settings prohibit replication. \~
              \return \ru Возвращает указатель на добавленный атрибут или нулевой указатель.
                      \en Returns a pointer to the added attribute or null pointer. \~
          */
          MbAttribute * AddAttribute( const MbAttribute & attr, bool checkSame = true, bool forcedAssign = false );

          /** \brief \ru Выдать атрибуты заданного типа или семейства.
                     \en Get attributes of a given type or family. \~
              \details \ru Выдать атрибуты заданного типа или семейства атрибутов. Если тип атрибутов задан как at_Undefined, \n
                           то выдаются все атрибуты заданного семейства, иначе возвращаются атрибуты заданного типа.
                       \en Get attributes of a given type or family of attributes. If the attribute type is set to at_Undefined, \n
                           then all attributes of the given family are returned, otherwise the attributes of the given type are returned. \~
              \param[out] attrs - \ru Массив возвращаемых атрибутов.
                                  \en An array of returned attributes.\~
              \param[in] aFamily - \ru Выбранное семейство возвращаемых атрибутов.
                                   \en A family of returned attributes. \~
              \param[in] subType - \ru Выбранный тип возращаемых атрибутов.
                                   \en A type of returned attributes. \~
          */
          void        GetAttributes( c3d::AttrVector & attrs, MbeAttributeType aFamily, MbeAttributeType subType ) const;

          /** \brief \ru Выдать атрибуты заданного типа.
                     \en Get attributes of a given type. \~
              \details \ru Выдать атрибуты заданного типа. 
                       \en Get attributes of a given type. \~
              \param[out] attrs - \ru Массив возвращаемых атрибутов.
                                  \en An array of returned attributes.\~
              \param[in] aType - \ru Выбранный тип возращаемых атрибутов.
                                 \en A type of returned attributes. \~
          */
          void        GetAttributes( c3d::AttrVector & attrs, MbeAttributeType aType ) const;
          /** \brief \ru Выдать атрибуты.
                     \en Get attributes. \~
              \details \ru Выдать атрибуты.
                       \en Get attributes. \~
              \param[out] attrs - \ru Массив возвращаемых атрибутов.
                                  \en An array of returned attributes.\~
          */
          template <class AttrVector>
          void        GetAttributes( AttrVector & attrs ) const
          {
            AttrMap_t::Iterator curIter = attributes.LowerBound( at_Undefined );
            while ( !curIter.Empty() ) {
              MbAttribute * attr = curIter.Value();
              if ( attr != nullptr )
                attrs.push_back( attr );
              curIter++;
            }
          }
          /// \ru Выдать атрибуты по строке описания. \en Get attributes using sample of description string. 
          void        GetCommonAttributes( c3d::AttrVector &, const c3d::string_t & samplePrompt, MbeAttributeType subType = at_Undefined, bool firstFound = false ) const;
          /// \ru Выдать строковые атрибуты по строке содержания. \en Get string attributes using sample of contents of the string. 
          void        GetStringAttributes( c3d::AttrVector &, const c3d::string_t & sampleContent ) const;

          /// \ru Выдать атрибут заданного типа, если их несколько - то первый попавшийся. \en Get an attribute of a given type, the first one is returned if there are many. 
  //const   MbAttribute * GetAttribute( MbeAttributeType subType ) const;

          /// \ru Удалить атрибут из контейнера. \en Delete an attribute from container. 
          bool        RemoveAttribute( const MbAttribute *, bool checkAccuracySame = false, double accuracy = LENGTH_EPSILON );
          /// \ru Удалить атрибуты заданного типа. \en Delete attributes of a given type. 
          bool        RemoveAttributes( MbeAttributeType type, MbeAttributeType subType );

          /// \ru Выдать простой атрибут данного подтипа. \en Get a simple attribute of a given subtype. 
  const   MbAttribute * GetSimpleAttribute( MbeAttributeType ) const; 
          /// \ru Выдать простой атрибут данного подтипа. \en Get a simple attribute of a given subtype. 
          MbAttribute * SetSimpleAttribute( MbeAttributeType );
          /// \ru Установить простой атрибут данного подтипа. \en Set a simple attribute of a given subtype. 
          MbAttribute * SetSimpleAttribute( MbAttribute * simpAttr );
          /// \ru Установить простой атрибут данного подтипа (всегда копирует атрибут). \en Set a simple attribute of a given subtype (always copies the attribute). 
          MbAttribute * SetSimpleAttribute( const MbAttribute & simpAttr );  
          /// \ru Удалить простой атрибут(один и более) данного подтипа. \en Delete simple attributes (one or more) of a given subtype. 
          void          RemoveSimpleAttribute( MbeAttributeType );
          /// \ru Отдать простой атрибут данного подтипа. \en Detach a simple attribute of a given subtype. 
          MbAttribute * DetachSimpleAttribute( MbeAttributeType ); 

          /// \ru Выдать пользовательский атрибут данного подтипа. \en Get a user attribute of a given subtype. 
          void        GetUserAttributes( std::vector<MbUserAttribute *> & attrs, const MbUserAttribType & type ) const; 
          /// \ru Удалить пользовательский атрибут (один и более) данного подтипа. \en Delete user attributes (one or more) of a given subtype. 
          void        RemoveUserAttributes( const MbUserAttribType & type ); 
          /// \ru Отдать пользовательский атрибут данного подтипа. \en Detach a user attribute of a given subtype. 
          void        DetachUserAttributes( std::vector<MbUserAttribute *> & attrs, const MbUserAttribType & type ); 

          /// \ru Преобразовать из пользовательского в "системный" \en Convert user attribute to "system" one 
  static  MbUserAttribute *     ReduceUserAttrib ( const MbExternalAttribute & );
          /// \ru Преобразовать из "системного" в пользовательский \en Convert "system" attribute to user one 
  static  MbExternalAttribute * AdvanceUserAttrib( const MbUserAttribute & );

          /// \ru Выполнить действия при изменении  атрибутов. \en Perform actions when changing the attributes.  
          void        AttributesChange (); 
          /// \ru Выполнить действия при конвертации атрибутов. \en Perform actions when converting the attributes.  
          void        AttributesConvert( MbAttributeContainer & other ) const; 
          /// \ru Выполнить действия при трансформировании атрибутов. \en Perform actions when transforming the attributes.  
          void        AttributesTransform( const MbMatrix3D &, MbRegTransform * = nullptr );
          /// \ru Выполнить действия при перемещении атрибутов. \en Perform actions when moving the attributes.  
          void        AttributesMove   ( const MbVector3D &,   MbRegTransform * = nullptr );
          /// \ru Выполнить действия при вращении атрибутов. \en Perform actions when rotating the attributes.  
          void        AttributesRotate ( const MbAxis3D &, double angle, MbRegTransform * = nullptr );
          /// \ru Выполнить действия при копировании атрибутов. \en Perform actions when copying the attributes.  
          void        AttributesCopy   ( MbAttributeContainer & other, MbRegDuplicate * = nullptr ) const;
          /// \ru Выполнить действия при объединении атрибутов. \en Perform actions when merging the attributes.  
          void        AttributesMerge  ( MbAttributeContainer & other ); 
          /// \ru Выполнить действия при замене атрибутов. \en Perform actions when replacing the attributes. 
          void        AttributesReplace( MbAttributeContainer & other ); 
          /// \ru Выполнить действия при разделении атрибутов. \en Perform actions when splitting the attributes. 
          void        AttributesSplit  ( const std::vector<MbAttributeContainer *> & others ); 
          /// \ru Выполнить действия при удалении атрибутов. \en Perform actions when deleting the attributes. 
          void        AttributesDelete (); 
  /** \} */

  /** \ru \name Функции простых атрибутов объекта.
  \en \name Functions of object's simple attributes.
  \{ */
          /// \ru Установить плотность объекта. \en Set density of an object. 
          void        SetDensity( double );
          /// \ru Выдать плотность объекта. \en Get density of an object. 
          double      GetDensity() const;

          /// \ru Установить визуальные свойства объекта. \en Set visual properties of the object. 
          void        SetVisual( float a, float d, float sp, float sh, float t, float e, uint rgb = 0 );
          /// \ru Установить визуальные свойства объекта. \en Set visual properties of the object. 
          void        SetVisual( const MbRGBA & ambient, const MbRGBA & diffuse, const MbRGBA & specular,
                                 const MbRGBA & emission, uint8 shininess, uint8 opacity, uint8  chrom );
          /** \brief \ru Выдать визуальные свойства объекта.
                     \en Get visual properties of the object. \~
            \details \ru Выдать визуальные свойства объекта.
                     \en Get visual properties of the object. \~
            \param[out] a - \ru Коэффициент общего фона (рассеянного освещения)
                            \en Coefficient of backlighting \~
            \param[out] d - \ru Коэффициент диффузного отражения
                            \en Coefficient of diffuse reflection \~
            \param[out] s - \ru Коэффициент зеркального отражения
                            \en Coefficient of specular reflection \~
            \param[out] h - \ru Блеск (показатель степени в законе зеркального отражения)
                            \en Shininess (index according to the law of specular reflection) \~
            \param[out] t - \ru Коэффициент непрозрачности
                            \en Coefficient of total reflection (opacity coefficient) \~
            \param[out] e - \ru Коэффициент излучения
                            \en Emissivity coefficient \~
            \return \ru true если есть такой атрибут \n false в противном случае
                    \en True if there is the attribute MbVisual \n otherwise false. \~
          */
          bool        GetVisual( float & a, float & d, float & sp, float & sh, float & t, float & e, uint rgb = 0 ) const;

          /// \ru Есть ли у объекта свой цвет. \en . 

          /** \brief \ru Есть ли у объекта свой цвет.
                     \en Whether the object is colored. \~
            \details \ru Есть ли у объекта свой цвет.
                     \en Whether the object is colored. \~
            \return \ru true если есть такой атрибут \n false в противном случае
                    \en True if there is the attribute MbColor \n otherwise false. \~
          */
          bool        IsColored() const { return (GetSimpleAttribute( at_Visual ) != nullptr);  }
          /// \ru Изменить цвет объекта. \en Change color of the object.  
          void        SetColor( uint32 );
          /// \ru Изменить цвет объекта (0-255). \en Change color of the object (0-255).
          void        SetColor( int R, int G, int B );
          /// \ru Выдать цвет объекта. \en Get color of an object. 
          uint32      GetColor() const;

          /// \ru Установить толщину линий для отображения объекта. \en Set thickness of lines for object's representation. 
          void        SetWidth( int );
          /// \ru Выдать толщину линий для отображения объекта. \en Get thickness of lines for object's representation. 
          int         GetWidth() const;

          /// \ru Установить стиль линий для отображения объекта. \en Set style of lines for object's representation. 
          void        SetStyle( int );
          /// \ru Выдать стиль линий для отображения объекта. \en Get style of lines for object's representation. 
          int         GetStyle() const;

          /// \ru Выделить или не выделить объект. \en To allocate or not to allocate an object. 
          void        SetSelected( bool s = true );
          /// \ru Выделен ли объект? \en Is the object selected. 
          bool        IsSelected() const;
          /// \ru Инвертировать выделение объекта. \en Invert object selection. 
          bool        ReverseSelected();

          /// \ru Задать: объект изменен или не изменён. \en Set: the object is changed or isn't changed. 
          void        SetChanged( bool c = true );
          /// \ru Изменен ли объект? \en Is the object changed? 
          bool        IsChanged() const;

          /// \ru Установить видимость. \en Set visibility.  
          void        SetVisible( bool );
          /// \ru Видимый ли объект? \en Is the object visible? 
          bool        IsVisible() const;
          /// \ru Не видимый ли элемент? \en Is the object invisible? 
          bool        IsInvisible() const;
  /** \} */

          /// \ru Прочитать атрибуты из потока. \en Read attributes from stream. 
          void        AttributesRead ( reader & );
          /// \ru Записать атрибуты в поток. \en Writing attributes to stream. 
          void        AttributesWrite( writer & ) const; 
          /// \ru Выдать свойства атрибутов. \en Get properties of attributes. 
          void        GetProperties( MbProperties & );
          /// \ru Установить свойства атрибутов. \en Set properties of attributes. 
          void        SetProperties( const MbProperties & );

private:
          MbAttribute * SetColorAttribute( MbAttribute * );
          MbAttribute * SetColorAttribute( const MbAttribute & );

OBVIOUS_PRIVATE_COPY( MbAttributeContainer )
};


//------------------------------------------------------------------------------
/** \brief \ru Получить обобщенные атрибуты.
           \en Get common attributes. \~
  \details \ru Получить обобщенные атрибуты. \n
           \en Get common attributes. \n \~
  \param[in] attrItem - \ru Объект с атрибутами.
                        \en Object with attributes. \~
  \param[in] attrPrompt - \ru Подсказка атрибута для поиска.
                          \en Attribute prompt. \~
  \param[out] resAttrs - \ru Найденные атрибуты.
                         \en Found attributes. \~
  \result \ru Возвращает true, если что-то добавлено.
          \en Returns 'true' if the something was got. \~
  \ingroup Model_Attributes
*/
// ---
MATH_FUNC (bool) GetCommonAttributes( const MbAttributeContainer & attrItem, 
                                      const c3d::string_t &        attrPrompt, 
                                            c3d::ConstAttrVector & resAttrs );


//------------------------------------------------------------------------------
/** \brief \ru Установить обобщенные атрибуты в целевой объект из объекта-источника.
           \en Set common attributes in the destination object from the source object. \~
  \details \ru Установить обобщенные атрибуты в целевой объект из объекта-источника. \n
           \en Set common attributes in the destination object from the source object. \n \~
  \param[in] srcItem - \ru Объект-источник.
                       \en The source object. \~
  \param[in] attrType - \ru Тип атрибута.
                        \en Attribute type. \~
  \param[in] attrPrompt - \ru Подсказка атрибута для поиска.
                          \en Attribute prompt. \~
  \param[out] dstItem - \ru Целевой объект.
                        \en The destination object. \~
  \param[in,out] bufAttrs - \ru Буферный массив атрибутов.
                            \en Buffer attributes vector. \~
  \result \ru Возвращает true, если что-то добавлено.
          \en Returns 'true' if the something was added. \~
  \ingroup Model_Attributes
*/
// ---
MATH_FUNC (bool) AddCommonAttributes( const MbAttributeContainer & srcItem, 
                                            MbeAttributeType       attrType,
                                      const c3d::string_t &        attrPrompt,
                                            MbAttributeContainer & dstItem, 
                                            c3d::AttrVector *      bufAttrs = nullptr );

//------------------------------------------------------------------------------
/** \brief \ru Удалить обобщенные атрибуты.
           \en Delete common attributes. \~
  \details \ru Удалить обобщенные атрибуты. \n
           \en Delete common attributes. \n \~
  \param[in] attrItem - \ru Объект с атрибутами.
                        \en Object with attributes. \~
  \param[in] attrPrompt - \ru Подсказка атрибута для поиска.
                          \en Attribute prompt. \~
  \result \ru Возвращает true, если что-то добавлено.
          \en Returns 'true' if the something was deleted. \~
  \ingroup Model_Attributes
*/
// ---
MATH_FUNC (bool) RemoveCommonAttributes( MbAttributeContainer & attrItem, const c3d::string_t & attrPrompt );


//------------------------------------------------------------------------------
/** \brief \ru Преобразовать атрибуты цвета.
           \en Convert color attributes. \~
  \details \ru Преобразовать атрибуты цвета, исключив атрибут MbColor из контейнера.
               При наличии только атрибута MbColor, он заменяется на атрибут MbVisual, в котором
               компонент diffuse содержит значение атрибута MbColor, а остальные значения по умолчанию.
               При наличии пары атрибутов MbColor и MbVisual, компонент diffuse атрибута MbVisual
               заменяется на значение атрибута MbColor, а атрибут MbColor удаляется. \n
           \en Convert color attributes and exclude the attribute MbColor from the container.
               If there is only the MbColor attribute in the container, it is replaced by
               the MbVisual attribute, in which the diffuse component contains the value of
               the MbColor attribute, and the rest - default values.
               If there is a pair of MbColor and MbVisual attributes, the diffuse component
               of the MbVisual attribute is replaced with the value of the MbColor attribute,
               and the MbColor attribute is deleted. \n \~
  \param[in/out] attrItem - \ru Объект с атрибутами.
                            \en Object with attributes. \~
  \result \ru Возвращает true, если атрибуты были изменены.
          \en Returns 'true' if the attributes were was changed. \~
  \ingroup Model_Attributes
*/
// ---
MATH_FUNC( bool ) ConvertColorAttributes( MbAttributeContainer & attrItem );


#endif // __ATTRIBUTE_CONTAINER_H
