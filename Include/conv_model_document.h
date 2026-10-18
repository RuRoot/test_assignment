////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Сущности конвертерной модели: документ, деталь, сборка, вставка.
         \en Entities of converter-compatible model: document, part, assembly, instance. \~
  \details  \ru Интерфейсы сущностей и предопределённая реализация модельного документа C3dModelDocument.
            \en Interfaces of entities and pre-defined implementation of model document C3dModelDocument. \~
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CONV_MODEL_DOCUMENT_H
#define __CONV_MODEL_DOCUMENT_H

#include <model_item.h>
#include <attribute_item.h>
#include <instance_item.h>
#include <conv_annotation_item.h>
#include <conv_predefined.h>
#include <conv_requestor.h>
#include <tool_cstring.h>
#include <assisting_item.h>
#include <vector>
#include <map>

//class  IConverterMetadataReceiver;

class  MbPlacement3D;
class  MbName;
class  MbModel;

class  MbAttributeContainer;

class             ItModelAssembly;
class             ItModelPart; 
class             ItModelInstance; 

class IProgressIndicator;

typedef SPtr<ItModelAssembly> ModelAssemblyPtr;
typedef SPtr<ItModelPart>     ModelPartPtr;
typedef SPtr<ItModelInstance> ModelInstancePtr;



//------------------------------------------------------------------------------
/** \brief \ru Интерфейс документа модели сборки или детали.
\en Interface of document of an assembly model or a part model. \~
\ingroup Exchange_Interface
*/
// ---
class CONV_CLASS ItModelDocument : public MbRefItem
{
public:
  /// \ru Это сборка? \en Is it an assembly? 
  virtual bool              IsAssembly() const = 0;
  /// \ru Модельный документ не содержит данных. \en There is no data.
  virtual bool              IsEmpty() const = 0; 

  /** \brief \ru Прообраз новой интерфейсной функции - задать модель ЛСК, относительно которой позиционируется модель.
             \en Prototype of a new interface function - get the placement the model is defined in. \~
  */
  //virtual MbPlacement3D     GetOriginLocation() const = 0;

  /** \brief \ru Прообраз новой интерфейсной функции - задать модель для наполнения.
             \en Prototype of a new interface function - set a model to fill. \~
  */
  virtual void              SetContent( MbItem* /*content*/) = 0;

  /** \brief \ru Прообраз новой интерфейсной функции - получить наполнение.
  \en Prototype of a new interface function - get the filling. \~
  */
  virtual MbItem *          GetContent() /*{ return nullptr; }*/ = 0;

  /** \brief \ru Создать документ с новой сборкой при импорте.
             \en Create a document with a new assembly while importing. \~
  \details \ru Увеличить счётчик ссылок результирующего документа на 1.
           \en Increase the reference counter of the resultant document by 1. \~  
  \param[in] componentItems - \ru Элементы, добавляемые в сборку.
                              \en Items to add into the assembly. \~
  \param[in] fileName -       \ru Имя сборки.
                              \en Assembly name. \~
  \return \ru Экземпляр сборки, если операция прошла успешно, nullptr в противном случае.
          \en Instance of an assembly if the operation succeeded, nullptr - otherwise. \~
  */
  virtual ModelAssemblyPtr CreateAssembly( const c3d::ItemsSPtrVector & componentItems, const c3d::string_t& fileName ) = 0;


  /** \brief \ru Создать документ с новой деталью при импорте.
             \en Create a document with a new part while importing. \~
  \details \ru Увеличить счётчик ссылок результирующего документа на 1.
           \en Increase the reference counter of the resultant document by 1. \~  
  \param[in] componentItems - \ru Элементы, добавляемые в сборку.
                              \en Items to add into the assembly. \~
  \param[in] fileName -       \ru Имя детали.
                              \en A part name. \~
  \return \ru Экземпляр детали, если операция прошла успешно, nullptr в противном случае.
          \en Instance of the part if the operation succeeded, nullptr - otherwise. \~
  */
  virtual ModelPartPtr   CreatePart( const c3d::ItemsSPtrVector & componentItems, const c3d::string_t& fileName ) = 0;

  /** \brief \ru Получить сборку для экспорта.
             \en Get an assembly for export. \~
  \details \ru Увеличить счётчик ссылок результирующей сборки на 1.
           \en Increase the reference counter of the resultant assembly by 1. \~
  \return \ru Экземпляр сборки, если операция прошла успешно, nullptr в противном случае.
          \en Instance of an assembly if the operation succeeded, nullptr - otherwise. \~
  */
  virtual ModelAssemblyPtr GetInstanceAssembly( ) = 0;


  /** \brief \ru Получить деталь для экспорта.
             \en Get the detail for export. \~
  \details   \ru Увеличить счётчик ссылок результирующей детали на 1.
             \en Increase the reference counter of the resultant part by 1. \~
  \return    \ru Экземпляр детали, если операция прошла успешно, nullptr в противном случае.
             \en Instance of the part if the operation succeeded, nullptr - otherwise. \~
  */
  virtual ModelPartPtr   GetInstancePart( ) = 0;


  /** \brief \ru Получить обработчик метаданных.
             \en Get metadata provider. \~
  \details   \ru Метаданные используются в тех случаях, когда не вписываются не в геометрическую, ни в атрибутивную модели.
             \en Metadata are used in cases they don't fin in neigher geometric nor attribute-based models. \~
  \return    \ru Реализацию для приема метаданных с целью дальнейшей обработки.
             \en Implementation of metadata processor. \~
  */
  virtual SPtr<IConverterMetadataReceiver> GetMetadataReceiver()  { return {}; };


  /** \brief \ru Получить путь для записи метаданных.
             \en Get metadata write path. \~
  \details   \ru Метаданные используются в тех случаях, когда не вписываются не в геометрическую, ни в атрибутивную модели.
                 Этот метод является альтернативным для GetMetadataReceiver(), для случаев, когда для приложения предпочтительнее
                 записывать метаданные в файловую систему вместо передачи через IConverterMetadataReceiver.
                 Указанный каталог должен существовать, быть пустым и доступным для записи.
             \en Metadata are used in cases they don't fit in neigher geometric nor attribute-based models. 
                 This method is alternative for GetMetadataReceiver() for cases when it is more preferred for
                 user application to store metadata into the filesystem rather than transferring them via IConverterMetadataReceiver.
                 The directory must exist, be empty and writable.\~
  \return    \ru Путь для записи метаданных в файловую систему.
             \en Path for writing metadata into the filesystem. \~
  */
  virtual c3d::path_string GetMetadataWritePath()  { return {}; };


  /** \brief \ru Завершить импорт и сохранить документ.
             \en Complete the import and save the document. \~
  \return \ru true, если операция прошла успешно, false в противном случае.
          \en true if the operation succeeded, false - otherwise. \~
  \param[in] \ru indicator Объект для отображения хода процесса.
             \en indicator An object indicating a process progress. \~
  */
  virtual bool              FinishImport( IProgressIndicator * indicator  ) = 0;

  /** \brief \ru Получить элементы аннотации, соответствующие элементам геометрической модели.
             \en Get elements of annotation, corresponding items of geometric model. \~
  \param[in] eTextForm - \ru Форма представления текста.
                         \en Text representation form. \~  
  \return \ru Контейнер объектов аннотации.
          \en Vector of annotation objects. \~
  */
  DEPRECATE_DECLARE
  virtual map_of_visual_items GetAnnotationItems( eTextForm ) const = 0; 

  /// \ru Задать размеры. \en Set sizes. 
  DEPRECATE_DECLARE
  virtual void              SetAnnotationItems( const map_of_visual_items& ) = 0; 

  /// \ru Открыть документ. \en Open a document.  
  DEPRECATE_DECLARE 
  virtual void               OpenDocument() = 0; 

};


//------------------------------------------------------------------------------
/** \brief \ru Формирователь геометрического представления текста.
           \en Generator of text element's geometry shape. \~
    \note  \ru ДЛЯ РАЗРАБОТЧИКОВ.
           \en DEVELOPERS ONLY. \~
\ingroup Exchange_Interface
*/
// ---
class CONV_CLASS C3DSymbolToItem : public MbRefItem {
public:
  virtual SPtr<MbItem> TextToItem( const MaTextItem*, const MbPlacement3D& location ) const;
  virtual SPtr<MbItem> TerminatorToItem( const MaTerminatorSymbol*, const MbPlacement3D& location ) const;

  virtual ~C3DSymbolToItem();
};


class MaAtributePostRemover;


//------------------------------------------------------------------------------
/** \brief \ru Формирователь геометрического представления PMI.
           \en Generator of PMI's geometry shape. \~
    \note  \ru ДЛЯ РАЗРАБОТЧИКОВ.
           \en DEVELOPERS ONLY. \~

\ingroup Exchange_Interface
*/
// ---
class CONV_CLASS C3DPmiToItem : public MbRefItem {
  SPtr<C3DSymbolToItem> symToItem;
  MaAtributePostRemover* tempAttributeManager;
public:
  C3DPmiToItem( SPtr<C3DSymbolToItem> = SPtr<C3DSymbolToItem>() );
  
  virtual SPtr<MbItem> operator() ( const MaAnnotationItem* ) const;
  virtual SPtr<MaAnnotationItem> operator() ( const MbItem* ) const;
  
  void    AddPartItem( const MbItem& ); // Добавить элемент для установления связи PMI и элемента модели

  virtual ~C3DPmiToItem();
};


//------------------------------------------------------------------------------
/** \brief \ru Реализация документа модели, формирующая регулярную структуру.
\en Implementation of model document which has regular structure. \~
\ingroup Exchange_Interface
*/
// ---
class CONV_CLASS C3dModelDocument: public ItModelDocument {

  ModelPartPtr        m_part{};         ///< \ru Представление в виде детали. \en Representation as detail.
  ModelAssemblyPtr    m_assembly{};     ///< \ru Представление в виде сборки. \en Representation as assembly.
  map_of_visual_items m_visualItems{};  ///< \ru Элементы аннотации. \en Annotation items.
  c3d::ItemSPtr       m_rawContent{};   ///< \ru Передаваемый модельный элемент. \en Converted model item.
  SPtr<C3DPmiToItem>  m_pmiToItem{};    ///< \ru Включены ли элементы аннотации непосредственно в модельный элемент. \en Model item contains PMI.
  SPtr<IConverterMetadataReceiver> m_metadataReceiver{};  ///< \ru Обработчик метаданных. \en Metadata provider.
  c3d::path_string m_metadataWritePath{};  ///< \ru Путь для записи метаданных. \en Metadata write path.
public:

  C3dModelDocument( SPtr<C3DPmiToItem> pmiToContent = SPtr<C3DPmiToItem>() ); ///< \ru Конструктор. \en Conscructor.

  ~C3dModelDocument() override; ///< \ru Деструктор. \en Descructor.

  // Является ли сборкой.
  bool              IsAssembly() const override;
  // Пуст ли.
  bool              IsEmpty() const override;
  // Задать модель напрямую.
  void              SetContent( MbItem* /*content*/) override;
  // Выдать модель напрямую.
  MbItem *          GetContent() override;
  // Создать сборку.
  ModelAssemblyPtr  CreateAssembly( const c3d::ItemsSPtrVector & componentItems, const c3d::string_t& fileName ) override;
  // Создать деталь.
  ModelPartPtr      CreatePart( const c3d::ItemsSPtrVector & componentItems, const c3d::string_t& fileName ) override;
  // Выдать сборку.
  ModelAssemblyPtr  GetInstanceAssembly( ) override;
  // Выдать деталь.
  ModelPartPtr      GetInstancePart( ) override;
  // Завершить импорт.
  bool              FinishImport( IProgressIndicator *  ) override;

  // Выдать элементы аннотации.
  DEPRECATE_DECLARE
  map_of_visual_items GetAnnotationItems( eTextForm ) const override;
  // Задать элементы аннотации.
  DEPRECATE_DECLARE
  void              SetAnnotationItems( const map_of_visual_items& vi ) override;
  // Открыть документ.
  DEPRECATE_DECLARE
  void              OpenDocument() override;

  // Получить интерфейс передачи метаданных.
  SPtr<IConverterMetadataReceiver> GetMetadataReceiver() override;

  // Получить путь для записи метаданных.
  c3d::path_string GetMetadataWritePath() override;

  /// \ru Включены ли PMI в элемент модели. \en If PMI is included into model item.
  SPtr<C3DPmiToItem>        PmiInContent() const;

  /// \ru Зарегистрировать элемент аннотации. \en Register annotation object.
  DEPRECATE_DECLARE
  void              RegisterAnnotation( c3d::ItemSPtr component, const AnnotationSptrVector& annotation, const AnnotationSptrVector& requirements );

  /// \ru Сбросить итераторы вставок. \en Reset instances itrerators.
  void ResetInstanceIterators();

  /// \ru Задать особенный обработчик метаданных. \en Set specific metadata provider.
  void SetMetadataReceiver( SPtr<IConverterMetadataReceiver> const & );

  //  Задать путь для записи метаданных, каталог должен существовать, быть пустым и доступным для записи. \en. Set the path to write metadata to, the directory must exist, be empty and writable
  void SetMetadataWritePath( c3d::path_string const & );

  /// \ru Инициализировать документ по двойнику. \en Build document by twin.
  void InitDocumentByTwin( MbModel const& );

  /// \ru Преобразовать все сборки и вставки в уникальные. \en Convert all assemblies and instances to unique ones.
  bool MakeAssembliesUnique();
};


typedef C3dModelDocument RegularModelDocument;
typedef C3dModelDocument ConvModelDocument;


//------------------------------------------------------------------------------
/** \brief \ru Интерфейс свойств вставки, подсборки или детали.
\en Interface of properties of an instance, a subassembly or a part. \~
\ingroup Exchange_Interface
*/
// ---
class ItModelInstanceProperties : public MbRefItem
{
public:
  /// \ru Задать атрибуты. \en Set attributes. 
  virtual bool SetAttributes( const c3d::AttrSPtrVector& /*attributes*/ ) = 0;

  /// \ru Получить атрибуты. \en Get attributes. 
  virtual c3d::AttrSPtrVector GetAttributes() const = 0;// { return c3d::AttrSPtrVector(); }

  /// \ru Технические требования. \en Technical requirements. 
  /// \ru Получить технические требования. \en Get technical requirements. 
  DEPRECATE_DECLARE
  virtual void          GetRequirements( AnnotationSptrVector &, eTextForm ) const = 0;

  /// \ru Задать технические требования. \en Set technical requirements. 
  DEPRECATE_DECLARE
  virtual void          SetRequirements( const AnnotationSptrVector & ) = 0; 

};


//------------------------------------------------------------------------------
/** \brief \ru Интерфейс вставки компоненты.
\en Interface of the component instance. \~
\ingroup Exchange_Interface
*/
// ---
class ItModelInstance : public ItModelInstanceProperties
{
public:
  // \ru Выдать идентификатор сборки или детали \en Get identifier of an assembly or a part 
  DEPRECATE_DECLARE
  virtual void            * GetId() = 0;
  /// \ru Выдать расположение этой вставки в координатах родителя. \en Get the placement of this instance in parent's coordinates. 
  virtual bool              GetPlacement( MbPlacement3D & ) const = 0; 
  /// \ru Это сборка? \en Is it an assembly? 
  virtual bool              IsAssembly() const = 0; 
  /// \ru Это ни сборка, ни деталь? \en Is it neither an assembly nor a part? 
  virtual bool              IsEmpty() const = 0; 

  /** \brief \ru Создать пустую сборку при импорте и увеличить счётчик ссылок на 1.
             \en Create an empty assembly while importing and increase the reference counter by 1. \~  
  \param[in] place -          \ru ЛСК сборки в родительской модели.
                              \en LCS of the assembly in the parent's model. \~
  \param[in] componentItems - \ru Элементы, добавляемые в сборку.
                              \en Items to add into the assembly. \~
  \param[in] fileName -       \ru Имя сборки.
                              \en Assembly name. \~
  \return \ru Экземпляр сборки, если операция прошла успешно, nullptr в противном случае.
          \en Instance of an assembly if the operation succeeded, nullptr - otherwise. \~
  */
  virtual ModelAssemblyPtr CreateAssembly( const MbPlacement3D &place, const c3d::ItemsSPtrVector & componentItems, const c3d::string_t& fileName ) = 0;

  /** \brief \ru Создать деталь при импорте.
             \en Create a part while importing. \~
  \details \ru Увеличить счётчик ссылок детали на 1.
           \en Increase the reference counter of a part by 1. \~
  \param[in] place -          \ru ЛСК детали.
                              \en LCS of a part. \~
  \param[in] componentItems - \ru Элементы, добавляемые в сборку.
                              \en Items to add into the assembly. \~
  \param[in] fileName -       \ru Имя детали.
                              \en A part name. \~
  \return \ru Экземпляр детали, если операция прошла успешно, nullptr в противном случае.
          \en Instance of the part if the operation succeeded, nullptr - otherwise. \~
  */
  virtual ModelPartPtr   CreatePart( const MbPlacement3D &place, const c3d::ItemsSPtrVector & componentItems, const c3d::string_t& fileName ) = 0;

  /** \brief \ru Получить сборку для экспорта.
             \en Get an assembly for export. \~
  \return \ru Экземпляр сборки, если операция прошла успешно, nullptr в противном случае.
          \en Instance of an assembly if the operation succeeded, nullptr - otherwise. \~
  */
  virtual ModelAssemblyPtr GetInstanceAssembly( ) = 0;


  /** \brief \ru Получить деталь для экспорта.
             \en Get the detail for export. \~
  \return \ru Экземпляр детали, если операция прошла успешно, nullptr в противном случае.
          \en Instance of the part if the operation succeeded, nullptr - otherwise. \~
  */
  virtual ModelPartPtr   GetInstancePart( ) = 0;

  /** \brief \ru Создать подсборку при импорте, и её вставку.
             \en Create a subassembly and its instance while importing. \~
  \param[in] place -    \ru ЛСК сборки в родительской модели.
                        \en LCS of the assembly in the parent's model. \~
  \param[in] existing - \ru Сборка, подлежащая вставке.
                        \en An assembly to insert. \~
  \return \ru true, если операция прошла успешно, false в противном случае.
          \en true if the operation succeeded, false - otherwise. \~
  */
  virtual bool              SetAssembly( const MbPlacement3D & place, const ItModelAssembly * existing ) = 0;

  /** \brief \ru Создать деталь при импорте, и её вставку.
             \en Create a part while importing and its instance. \~
  \param[in] place -    \ru ЛСК детали в родительской модели.
                        \en LCS of a part in the parent's model. \~
  \param[in] existing - \ru Деталь, подлежащая вставке.
                        \en Detail to insert. \~
  \return \ru true, если операция прошла успешно, false в противном случае.
          \en true if the operation succeeded, false - otherwise. \~
  */
  virtual bool              SetPart( const MbPlacement3D & place, const ItModelPart * existing ) = 0;

};


//------------------------------------------------------------------------------
/** \brief \ru Тип объектов, которые необходимо выдать для экспорта или добавить при импорте.
\en Type of objects to be returned for export or to be added while importing. \~
\ingroup Data_Interface
*/
// ---
enum MbeGettingItemType {
  git_Item = 0,       ///< \ru Получить элементы всех типов. \en Get items of all types.
  git_Solid,          ///< \ru Получить тела. \en Get solids.  
  git_Surface,        ///< \ru Получить поверхности. \en Get surfaces.
  git_WireFrame,      ///< \ru Получить проволочные каркасы. \en Get wire frames.
  git_PlaneInstance,  ///< \ru Получить вставки плоских объектов (эскизы). \en Get plane instances (drafts).
  git_PointFrame,     ///< \ru Получить точечные каркасы. \en Get point frames.
  git_AssociatedGeometry, ///< \ru Получить ассоциированные геометрические объекты (резьбы). \en Get associated geometry objects (threads).
  git_Polygonal,      ///< \ru Получить полгональные объекты (MbMesh). !!! ЭКСПЕРИМЕНТАЛЬНЫЙ ФУНКЦИОНАЛ !!! \en Get polygonal objects (MbMesh). !!! EXPEREIMENTAL FUNCTIONALITY !!!
  git_PMI,            ///< \ru Получить размеры и иные элементы аннотации. \en Get dimensions and other annotation items.
  git_TechnicalRquirments, ///< \ru Получить технические требования. \en Get thecnical requirements.
  git_Placement,      ///< \ru Получить системы координат. \en Get placements.
  git_Count           ///< \ru Счетчик типов. \en Types counter.
};


//------------------------------------------------------------------------------
/** \brief \ru Интерфейс сборки.
\en Interface of the assembly. \~
\details \ru Экземпляр должен порождаться в методах CreateAssembly реализаций 
интерфейсов ItModelDocument и ItModelAInstance. Собственные элементы детали 
должны передаваться как параметры конструктора. \~ \en The object should be 
created in the CreateAssembly method of the implementations of the 
ItModelDocument and ItModelInstance interfaces. Own Items of the detail should 
be arguments of the constructor.
\ingroup Exchange_Interface
*/
// ---
class ItModelAssembly : public ItModelInstanceProperties
{
public:
  /** \brief \ru Получить имя файла сборки без пути и расширения  для экспорта.
             \en Get the file name of an assembly without the path and the extension for export. \~
  \return \ru Имя файла сборки.
          \en An assembly file name. \~
  */
  virtual  c3d::path_string PureFileName() const = 0;

  /** \brief \ru Получить пустой интерфейс вставки для создания подсборки или детали при импорте.
             \en Get an empty interface of the insertion for creation of subassembly or a part while importing. \~
  \details \ru Увеличить счётчик ссылок на 1.
           \en Increase the reference counter by 1. \~
  \return \ru Интерфейс вставки, если операция прошла успешно или nullptr в противном случае.
          \en Interface of the instance if the operation succeeded and nullptr otherwise. \~
  */
  virtual ModelInstancePtr PrepareInstance() = 0;

  /** \brief \ru Получить интерфейс следующей вставки для создания подсборки или детали при экспорте.
             \en Get the interface of the next insertion for creation of a subassembly or a part while exporting. \~
  \return    \ru Интерфейс вставки, если операция прошла успешно или nullptr в противном случае.
             \en Interface of the insertion if the operation succeeded and nullptr otherwise. \~
  */
  virtual ModelInstancePtr NextInstance( bool includeInvisible ) = 0;

  /// \ru Выдать ЛСК, общую для элементов компонента. \en Get the placement, which all the items of the component use for transformation.
  virtual bool GetPlacement( MbPlacement3D & ) const { return false; }; 

  /** \brief \ru Получить объекты из корня сборки при экспорте.
             \en Get objects from the assembly root while exporting. \~
  \param[out] items -           \ru Наполняемый массив (состоит из объектов классов MbSolid, MbCurve3D, MbCartPoint3D).
                                \en Array to fill (consist of objects of classes MbSolid, MbCurve3D, MbCartPoint3D). \~
  \param[in] includeInvisible - \ru Если true, то выдаются все тела, включая невидимые, если false - только видимые.
                                \en If true, then all the solids are returned, including invisible ones, if false - only visible ones. \~
  */
  virtual void              GetItems( c3d::ItemsSPtrVector & items, MbeGettingItemType itemType, bool includeInvisible ) const = 0;

  /** \brief \ru Добавить объекты в корень сборки при импорте.
             \en Add objects to the assembly root while importing. \~
  \param[in] items - \ru Объекты, добавляемые в модель (тела, кривые и точки).
                     \en Objects to add to the model (solids, curves and points). \~
  */
  virtual void              AddItems( const c3d::ItemsSPtrVector & items ) = 0;

  /** \brief \ru Получить элементы аннотации из сборки.
             \en Get elements of annotation from the assembly. \~
  \param[in] eTextForm - \ru Форма представления текста.
                         \en Text representation form. \~  
  \param[in] includeInvisible - \ru Если true, то выдаются все объекты аннотации, включая невидимые, если false - только видимые.
                                \en If true, all the annotation objects are returned, including invisible ones, if false - only visible ones. \~
  \return \ru Контейнер объектов аннотации.
          \en Vector of annotation objects. \~
  */
  DEPRECATE_DECLARE
  virtual AnnotationSptrVector GetAnnotationItems( eTextForm, bool ) const { return AnnotationSptrVector(); }; // Реализация будет удалена после того, как она будет осуществлена на стороне 3D

  DEPRECATE_DECLARE
  virtual AnnotationSptrVector GetAnnotationItems( eTextForm ) const  { return AnnotationSptrVector(); }; // Будет удалена после её реализации на стороне 3D

  /** \brief \ru Задать элементы аннотации в сборке.
             \en Set elements of annotation in the assembly. \~
  \param[in] sourceDim - \ru Элементы аннотации
                         \en Elements of annotation. \~
  */

  DEPRECATE_DECLARE
  virtual void          SetAnnotationItems( const AnnotationSptrVector & ) = 0;

  /** \brief \ru Добавить объект с заданной ЛСК в корень сборки при импорте.
             \en Add object with a placement to the assembly root while importing. \~
  \param[in] item - \ru Объект, добавляемые в модель.
                    \en Object to add to the model. \~
  \param[in] placement - \ru ЛСК.
                         \en Placement. \~
  */
  virtual void AddMappedItem( MbItem &, MbPlacement3D const &, c3d::AttrSPtrVector const& ) {};

  /** \brief \ru Получить следующий объект с заданной ЛСК.
             \en Get the next item with placement. \~
  \return \ru Объект с заданной ЛС или nullptr.
          \en Item with placement and nullptr otherwise. \~
  */
  virtual SPtr<MbInstance> NextMappedItem() const { return {}; }
  
  /** \brief \ru Получить следующую группу конструкционных осей.
             \en Get the next construction axes grid. \~
  \return \ru Очередная группа конструкционных осей или нуль в случае исчерпания.
          \en Next construction axes grid or null in case of depletion. \~
  */
  virtual c3d::ItemSPtr NextAxesGrid() const { return c3d::ItemSPtr{}; }
  
  
  /** \brief \ru Добавить следующую группу конструкционных осей.
             \en Add next construction axes grid. \~
   \param[in] group \ru Очередная группа конструкционных осей.
                    \en Next construction axes grid. \~
  */
  virtual void AddAxesGrid( c3d::ItemSPtr const & ) {}

  /** \brief \ru Получить следующую пространственную ось.
             \en Get the next spatial axis. \~
  \return \ru Очередная пространственная ось или нуль в случае исчерпания.
          \en Next spatial axis or null in case of depletion. \~
  */
  virtual c3d::ItemSPtr NextAxisSpatial() const { return c3d::ItemSPtr{}; }


  /** \brief \ru Добавить следующую пространственную ось.
             \en Get the next spatial axis. \~
   \param[in] group \ru Очередная пространственная ось.
                    \en Next spatial axis. \~
  */
  virtual void AddAxisSpatial( c3d::ItemSPtr const & ) {}

   /** \brief \ru Получить следующую систему координат.
              \en Get the next coordinate system. \~
  \return \ru Очередная система координат или нуль в случае исчерпания.
          \en Next coordinate system or null in case of depletion. \~
  */
  virtual SPtr<MbAssistingItem> NextCoordinateSystem() const { return {}; }
  
  
  /** \brief \ru Добавить систему координат.
             \en Add a coordinate system. \~
   \param[in] group \ru Система координат.
                    \en Coordinate system. \~
  */
  virtual void AddCoordinateSystem( SPtr<MbAssistingItem> const & ) {}


  /** \brief \ru Получить следующий объект, ограничивающий площадь или объем .
  \en Get the next item that is the boundary of an area or volume. \~
  \return \ru Очередной объект, ограничивающий площадь или объем, или нуль в случае исчерпания.
  \en Next item that is the boundary of an area or volume or null in case of depletion. \~
  */
  virtual c3d::ItemSPtr NextBoundaryItem() const { return c3d::ItemSPtr {}; }


  /** \brief \ru Добавить  объект, ограничивающий площадь или объем (IfcSpace ).
  \en Add an item that is the boundary of an area or volume. \~
   \param[in] group \ru Очередной объект, ограничивающий площадь или объем.
   \en Next item that is the boundary of an area or volume. \~
  */
  virtual void AddBoundaryItem( c3d::ItemSPtr const & ) {}
 };


//------------------------------------------------------------------------------
/** \brief \ru Интерфейс детали.
\en Interface of a part. \~
\details \ru Экземпляр должен порождаться в методах CreatePart реализаций 
интерфейсов ItModelDocument и ItModelAInstance. Собственные элементы детали 
должны передаваться как параметры конструктора. \~ \en The object should be 
created in the CreatePart method of the implementations of the 
ItModelDocument and ItModelInstance interfaces. Own Items of the detail should 
be arguments of the constructor.
\ingroup Exchange_Interface
*/
// ---
class ItModelPart : public ItModelInstanceProperties
{
public:
  /** \brief \ru Получить имя файла детали без пути и расширения  для экспорта.
             \en Get the file name of a part without the path and extension for export. \~
  \return \ru Имя файла детали.
          \en A part file name. \~
  */
  virtual c3d::path_string PureFileName() const = 0;

  /** \brief \ru Получить пустой интерфейс вставки для создания подсборки или детали при импорте.
             \en Get an empty interface of the insertion for creation of subassembly or a part while importing. \~
  \details \ru Увеличить счётчик ссылок на 1.
           \en Increase the reference counter by 1. \~
  \return \ru Интерфейс вставки, если операция прошла успешно или nullptr в противном случае.
          \en Interface of the instance if the operation succeeded and nullptr otherwise. \~
  */
  virtual ModelInstancePtr PrepareInstance() = 0;

  /** \brief \ru Получить интерфейс следующей вставки для создания подсборки или детали при экспорте.
             \en Get the interface of the next insertion for creation of a subassembly or a part while exporting. \~
  \return \ru Интерфейс вставки, если операция прошла успешно или nullptr в противном случае.
          \en Interface of the insertion if the operation succeeded and nullptr otherwise. \~
  */
  virtual ModelInstancePtr NextInstance( bool includeInvisible ) = 0;

  /// \ru Выдать ЛСК, общую для элементов компонента. \en Get the placement, which all the items of the component use for transformation.
  virtual bool GetPlacement( MbPlacement3D & ) const { return false; }; 

  /** \brief \ru Получить объекты из детали при экспорте.
             \en Get objects from the part while exporting. \~
  \param[out] items -   \ru Наполняемый массив (состоит из объектов классов MbSolid, MbWireFrame, MbPointFrame).
                        \en Array to fill (consists of objects of classes MbSolid, MbWireFrame, MbPointFrame). \~
  \param[in] itemType - \ru Тип объектов, которыми нужно наполнить массив.
                        \en Type of objects the array should be filled with. \~
  \param[in] includeInvisible - \ru Если true, то выдаются все тела, включая невидимые, если false - только видимые.
                                \en If true, all the solids are returned, including invisible ones, if false - only visible ones. \~
  */
  virtual void          GetItems( c3d::ItemsSPtrVector & items, MbeGettingItemType itemType, bool includeInvisible ) const = 0;

  /** \brief \ru Добавить объекты в деталь при импорте.
             \en Add objects to a part while importing. \~
  \param[in] items - \ru Объекты, добавляемые в модель (кривые и точки).
                     \en Objects to be added to the model (curves and points). \~
  */
  virtual void          AddItems( const c3d::ItemsSPtrVector & items ) = 0;

  /** \brief \ru Получить элементы аннотации из детали.
             \en Get elements of annotation from the detail. \~
  \param[in] eTextForm - \ru Форма представления текста.
                         \en Text representation form. \~  
  \param[in] includeInvisible - \ru Если true, то выдаются все объекты аннотации, включая невидимые, если false - только видимые.
                                \en If true, all the annotation objects are returned, including invisible ones, if false - only visible ones. \~
  \return \ru Контейнер объектов аннотации.
          \en Vector of annotation objects. \~
  */
  DEPRECATE_DECLARE
  virtual AnnotationSptrVector GetAnnotationItems( eTextForm, bool ) const { return AnnotationSptrVector(); }; // Реализация будет удалена после того, как она будет осуществлена на стороне 3D
  DEPRECATE_DECLARE
  virtual AnnotationSptrVector GetAnnotationItems( eTextForm ) const { return AnnotationSptrVector(); }; // Будет удалена после её реализации на стороне 3D


  /** \brief \ru Задать элементы аннотации в детали.
             \en Set elements of annotation in the part. \~
  \param[in] sourceDim - \ru Элементы аннотации
                         \en Elements of annotation. \~
  */
  DEPRECATE_DECLARE
  virtual void          SetAnnotationItems( const AnnotationSptrVector & ) = 0;

  /** \brief \ru Добавить объект с заданной ЛСК в корень сборки при импорте.
             \en Add object with a placement to the assembly root while importing. \~
  \param[in] item - \ru Объект, добавляемые в модель.
                    \en Object to add to the model. \~
  \param[in] placement - \ru ЛСК.
                         \en Placement. \~
  */
  virtual void AddMappedItem( MbItem &, MbPlacement3D const &, c3d::AttrSPtrVector const& ) {};

  /** \brief \ru Получить следующий объект с заданной ЛСК.
             \en Get the next item with placement. \~
  \return \ru Объект с заданной ЛС или nullptr.
          \en Item with placement and nullptr otherwise. \~
  */
  virtual SPtr<MbInstance> NextMappedItem() const { return {}; }
  
  /** \brief \ru Получить следующую группу конструкционных осей.
             \en Get the next conxtruction axes grid. \~
  \return \ru Очередная группа конструкционных осей или нуль в случае исчерпания.
          \en Next construction axes grid or null in case of depletion. \~
  */
  virtual c3d::ItemSPtr NextAxesGrid() const { return c3d::ItemSPtr{}; }
  
  
  /** \brief \ru Добавить следующую группу конструкционных осей.
             \en Get the next conxtruction axes grid. \~
   \param[in] group \ru Очередная группа конструкционных осей или нуль в случае исчерпания.
                    \en Next construction axes grid or null in case of depletion. \~
  */
  virtual void AddAxesGrid( c3d::ItemSPtr const & ) {}

  /** \brief \ru Получить следующую пространственную ось.
             \en Get the next spatial axis. \~
  \return \ru Очередная пространственная ось или нуль в случае исчерпания.
          \en Next spatial axis or null in case of depletion. \~
  */
  virtual c3d::ItemSPtr NextAxisSpatial() const { return c3d::ItemSPtr{}; }


  /** \brief \ru Добавить следующую пространственную ось.
             \en Get the next spatial axis. \~
   \param[in] group \ru Очередная пространственная ось.
                    \en Next spatial axis. \~
  */
  virtual void AddAxisSpatial( c3d::ItemSPtr const & ) {}

  /** \brief \ru Получить следующую систему координат.
             \en Get the next coordinate system. \~
  \return \ru Очередная система координат или нуль в случае исчерпания.
          \en Next coordinate system or null in case of depletion. \~
  */
  virtual SPtr<MbAssistingItem> NextCoordinateSystem() const { return {}; }
  
  
  /** \brief \ru Добавить систему координат.
             \en Add a coordinate system. \~
   \param[in] group \ru Система координат.
                    \en Coordinate system. \~
  */
  virtual void AddCoordinateSystem( SPtr<MbAssistingItem> const & ) {}

  /** \brief \ru Получить следующий объект, ограничивающий площадь или объем .
  \en Get the next item that is the boundary of an area or volume. \~
  \return \ru Очередной объект, ограничивающий площадь или объем, или нуль в случае исчерпания.
  \en Next item that is the boundary of an area or volume or null in case of depletion. \~
  */
  virtual c3d::ItemSPtr NextBoundaryItem() const { return c3d::ItemSPtr {}; }


  /** \brief \ru Добавить  объект, ограничивающий площадь или объем (IfcSpace ).
  \en Add an item that is the boundary of an area or volume. \~
   \param[in] group \ru Очередной объект, ограничивающий площадь или объем.
   \en Next item that is the boundary of an area or volume. \~
  */
  virtual void AddBoundaryItem( c3d::ItemSPtr const & ) {}
};


namespace c3d
{
  namespace converter {
  /** \brief \ru Проверить корректность набора осей.
             \en Validate axes grid. \~
  \details \ru Либо это объект типа MbPlaneInstance либо MbAssembly, содержащий только MbPlaneInstance с одинаковыми ЛСК плоскостей\~
           \en. The item is or MbPlaneInstance or MbAssembly containing only MbPlaneInstance items with same LCS of the plane.\~
  \return \ru Пройдена ли валидация.
          \en Wether validation passed. \~
  \param[in] item - \ru Группа конструкционных осей.
                    \en Construction axes grid. \~
  */
  CONV_FUNC (bool) IsAxesGridValid( c3d::ItemSPtr const & );
  }
}

#endif // __CONV_MODEL_DOCUMENT_H
