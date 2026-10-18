////////////////////////////////////////////////////////////////////////////////
//
// DXF - конвертер.
// DXF - converter.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __DXF_CONVERTER_H
#define __DXF_CONVERTER_H

#include <templ_sptr.h>
#include <templ_psrt_array.h>
#include <solid.h>
#include <mb_placement3d.h>
#include <vector>


class  IConvertorProperty3D;
class  IProgressIndicator;
class  ColorProperties;
class  ItModelInstanceProperties;
class  ItModelDocument;
class  ItModelInstance;
class  DXFConverter;
class  DXFCompositeRef;
class  MbGrid;


//------------------------------------------------------------------------------
// \ru Уникальный (в пределах документа) идентификатор объекта.
// \en Unique (in the document) object identifier.
//---
class CONV_CLASS DXFHandle {

private:
  int64 thisId;    // \ru Уникальное 64-битное число, соответствующее модельному объекту. \en Unique 64-bit number corresponding to the model object. 

public:
  DXFHandle (                   );
  DXFHandle ( int64 & id );
  DXFHandle ( const unsigned char  id[8] );
  ~DXFHandle();
  
  const  DXFHandle & operator = ( const DXFHandle & id ); 
  
         bool        IsDefined() const { return thisId != -1 ; }
  
  // \ru операторы сравнения \en compare operators 
  friend bool operator >  ( const DXFHandle & left,  const DXFHandle & right );
  friend bool operator == ( const DXFHandle & left,  const DXFHandle & right );
};


//------------------------------------------------------------------------------
// \ru Сравнение thisId. \en Comparison of thisId. 
//---
inline
bool operator > ( const DXFHandle & left,  const DXFHandle & right ) {
  return left.thisId > right.thisId; 
}


//------------------------------------------------------------------------------
/// \ru Равенство thisId. \en Equality of thisId. 
//---
inline
bool operator ==( const DXFHandle & left,  const DXFHandle & right ) {
  return left.IsDefined() && right.IsDefined() && left.thisId == right.thisId;
}


//------------------------------------------------------------------------------
// \ru Тело. \en A solid. 
//---
class CONV_CLASS DXFSolidBody {
private:
  MbPlacement3D                placement; // \ru Локальная система координат. \en Local coordinate system. 
  std::vector<SPtr<MbItem> >   solids;    // \ru Тела. \en Solids. 
  std::vector< SPtr<MbFace>  > faces;     // \ru Грани. \en Faces. 
  
public:
  DXFSolidBody( MbPlacement3D & placement );
  DXFSolidBody( const DXFSolidBody &);
  ~DXFSolidBody();

public:
        bool          IsEmpty     () const;

        bool          IsSingle    () const;
        MbPlacement3D GetPlacement() const;

        void          MakePlacementIdentical( const MbPlacement3D& ownComponentLocation = MbPlacement3D::global );
        void          FillSolids    ( std::vector<SPtr<MbItem> > & solids ) const;
        void          Flush         ();
        void          AddSolids     ( const std::vector<SPtr<MbItem> > & mSolids );
        void          AddFaces      ( const std::vector< SPtr<MbFace> >  & mFaces  );
        void          SetPlacement  ( const MbPlacement3D & place );

        size_t        GetSolidsCount() const { return solids.size(); }
        void          GetSolids     ( std::vector<SPtr<MbItem> > & mSolids ) const;

        size_t        GetFacesCount () const { return faces.size(); }
        void          GetFaces      ( std::vector< SPtr<MbFace> > & mFaces ) const;
private:
  //DXFSolidBody              ( const DXFSolidBody & ); // \ru не реализовано \en not implemented 
  DXFSolidBody & operator = ( const DXFSolidBody & ); // \ru не реализовано \en not implemented 
  
};


//------------------------------------------------------------------------------
// 
// ---
inline 
void DXFSolidBody::SetPlacement( const MbPlacement3D & place ) {
  placement = place;
}


//------------------------------------------------------------------------------
// \ru Поверхность. \en The surface. 
//---
class CONV_CLASS DXFSurfaceBody {
private:
  MbPlacement3D               placement;  // \ru Локальная система координат. \en Local coordinate system. 
  std::vector< SRef<MbFace> > faces;      // \ru Грани. \en Faces. 
  SPtr<MbMesh>                mesh;
  
public:
  DXFSurfaceBody();
  DXFSurfaceBody( MbPlacement3D & placement );
  ~DXFSurfaceBody();

public:
        bool          IsEmpty     () const;
        bool          IsSingle    () const;
        MbPlacement3D GetPlacement() const;

        void          Flush();
        void          AddFace ( MbFace & face );
        void          AddGrid ( MbGrid & grid );
        void          AddFaces( const std::vector< SPtr<MbFace> > & mFaces );

        size_t        GetFacesCount() const { return faces.size(); }

private:
        void          FillFaces( RPArray<MbFace> & faces ) const;
        void          GetFaces( std::vector< SPtr<MbFace> > & mFaces ) const;
public:

        SPtr<MbMesh>  GetMesh();

        std::vector<SPtr<MbItem> > GenerateItems( bool stitch );

private:


  DXFSurfaceBody              ( const DXFSurfaceBody & ); // \ru не реализовано \en not implemented 
  DXFSurfaceBody & operator = ( const DXFSurfaceBody & ); // \ru не реализовано \en not implemented 
  
};


//------------------------------------------------------------------------------
// \ru Свойства блока.
// \en Block properties. 
//---
class CONV_CLASS DXFCompositeData {
private:
  DXFHandle  thisId;        // \ru Идентификатор блока. \en Block identifier. 
  MbVector3D m_scalesId;    // \ru Масштабы блока по осям координат, (нужны только для идентификация блока с thisId); связано с отказом от использования левых плейсментов в подсборках (err. 56646). \en Block scales by coordinate axes, (they are necessary only for the identification of the block with thisId); this is related with the decision not to use left placements in subassemblies (err. 56646). 56646). 
  c3d::string_t name;       // \ru Имя блока. \en Block name. 
  MbVector3D scales;        // \ru Масштабы блока по осям координат (нужны при создании сборки). \en Scales of block by coordinate axes (they are necessary for assembly creation). 

  MbMatrix3D m_TranslateRotate;  // \ru Преобразование блока для текущей вставки блока, но без учета масштабных коэффициентов самой вставки блока. \en Transformation of a block for the current block insertion without taking into account the scale factors of this block insertion. 
  MbMatrix3D m_sumTransform;     // \ru Преобразование всех внешних блоков. \en Transformation of all external blocks. 
  
public:
  DXFCompositeData();
  DXFCompositeData( const DXFHandle & chandle, const TCHAR * cname );
  DXFCompositeData( const DXFHandle & chandle, const TCHAR * cname,
                    const MbVector3D &,
                    const MbMatrix3D & tr,
                    const MbMatrix3D & sumTr );
  // DXFCompositeData( const TCHAR * cname );
  DXFCompositeData( const DXFCompositeData & cname );
  ~DXFCompositeData();

public:
  const TCHAR      * GetName()    const { return name.c_str(); }
  std::string        Name()       const { return c3d::ToSTDstring( name ); }
  c3d::string_t      NamePath()   const { return c3d::string_t( name.c_str() ); }
  const DXFHandle  & ThisId ()    const { return thisId; }
  const MbVector3D & Scales ()    const { return scales; }
        MbVector3D & Scales ()          { return scales; }
        size_t       NameLength() const { return name.length(); }
        MbVector3D & ScalesId()         { return m_scalesId; }
        MbVector3D   GetScalesId()const { return m_scalesId; }
  const MbMatrix3D & GetTranslateRotate() const { return m_TranslateRotate; }
  const MbMatrix3D & GetSumTransform   () const { return m_sumTransform; }

        /// \ru Сравнение с другими данными. \en Comparison with other data. 
        bool        operator == ( const DXFCompositeData & ) const ; 

        DXFCompositeData & operator=( const DXFCompositeData & ) { C3D_ASSERT_UNCONDITIONAL( false ); return *this; }
};


//------------------------------------------------------------------------------
/// \ru Сравнение с другими данными по thisId. \en Comparison with other data by thisId. 
//----
inline
bool DXFCompositeData::operator == ( const DXFCompositeData & comp ) const {
  // \ru для различения блока используется идентификационный номер и масштабный коэффициент \en an identification number and a scale factor are used to distinguish the block  
  return  thisId == comp.thisId && m_scalesId == comp.m_scalesId;
  // return  thisId == comp.thisId && scales == comp.scales;
}


//------------------------------------------------------------------------------
// \ru Блок \en Block   
//---
class CONV_CLASS DXFComposite : public MbRefItem {
private:
  PArray<DXFCompositeRef>        composites;    // \ru Составляющие. \en Components. 
  PArray<DXFSolidBody>           solid_bodies;  // \ru Тела. \en Solids. 
  std::vector< SPtr<MbCurve3D> > space_curves;  // \ru Кривые. \en Curves. 
  DXFSurfaceBody                 surface_body;  // \ru Поверхностное тело. \en Surface solid. 
  DXFCompositeData               data;          // \ru Данные для слива блока. \en Data of the block for union. 
  SPtr<ItModelInstanceProperties>     insert;        // \ru Готовая вставка блока в модельный документ. \en Prepared insert of the block to the model document. 

public:
  DXFComposite();
  DXFComposite( const DXFCompositeData & data );
  ~DXFComposite();

public:
  bool IsEmpty          () const;
  void Complete         ( DXFConverter & converter    );
  void AddComposite     ( DXFCompositeRef * composite );
  ptrdiff_t GetObjectsCount  ();
  void FlushSolidBodies ();

  void AddFace          ( MbFace & m_face );
  void AddGrid          ( MbGrid & m_grid );
  void AddSpaceCurve    ( MbCurve3D & space_curves );
  void AddSolidBody     ( MbPlacement3D & placement, const std::vector<SPtr<MbItem> > & m_solids,
                                                     const std::vector< SPtr<MbFace> > &  m_faces );

        size_t              GetSolidBodiesCount()  const { return solid_bodies.Count(); }
  const DXFSolidBody      * GetSolidBody( size_t k ) const { return ((k < solid_bodies.Count()) ? solid_bodies[k] : nullptr); }
        DXFSolidBody      * SetSolidBody( size_t k )       { return ((k < solid_bodies.Count()) ? solid_bodies[k] : nullptr); }

  const DXFCompositeData  & GetData() const { return data;}

        bool                SetToModel( const MbPlacement3D   & where, 
                                              ItModelInstance & instance );
        // \ru Высвободить и обнулить вставку. \en Free the insert and set it to null. 
        void                ReleaseInsert(); 

        // \ru Добавить геометрию (solid_bodies и surface_body) из ob. \en Add to geometry (solid_bodies and surface_body) from ob. 
        void                AddGeometryFrom( const DXFComposite & ob );
private:
  friend class DXFConverter;
  friend class DXFCompositeRef;
  void CompleteDocument ( ItModelDocument & model_document, DXFConverter & converter );
  void CompleteInstance ( ItModelInstance & model_instance, DXFConverter & converter, 
                          MbPlacement3D   * place, MbVector3D & scales );
  
  void CompleteComponent( const MbPlacement3D   & place,
                          ItModelInstance & model_instance, 
                          DXFConverter    & converter, const MbVector3D& scalesBase );

  DXFComposite * FindObj( const DXFCompositeData & );
  void CheckPlacementsByGabarits( MbPlacement3D & );

  void CheckIdenticalBaserSurfaces(); // Контроль одинаковых поверхностей в гранях.
  void CollectOwnItems( std::vector<SPtr<MbItem> >& ownItems ); // Собрать собственные элементы компонента
private:
  DXFComposite              ( const DXFComposite & ); // \ru не реализовано \en not implemented 
  DXFComposite & operator = ( const DXFComposite & ); // \ru не реализовано \en not implemented 
  
};


//------------------------------------------------------------------------------
// \ru Ссылка на блок \en Reference to a block 
//---
class CONV_CLASS DXFCompositeRef {
private:
  MbPlacement3D   place;     // \ru Локальная система координат. \en Local coordinate system. 
  DXFComposite  * composite; // \ru Блок. \en Block. 
public:
  DXFCompositeRef ( );
  DXFCompositeRef ( DXFComposite & composite , const MbPlacement3D & cplace );
  DXFCompositeRef ( DXFComposite & composite );
  ~DXFCompositeRef( );
  
        // \ru Заполнить документ модели. \en Fill the model document. 
        void            CompleteInstance ( ItModelInstance & model_instance, 
                                           DXFConverter    & converter,
                                           MbVector3D      & overallScales ); 
        // \ru Создать документ \en Create a document    
        void            CompleteDocument ( ItModelDocument & model_document, DXFConverter & converter );

  const MbPlacement3D & GetPlacement() const { return place; }
        void            SetPlacement( const MbPlacement3D & pl ) { place = pl; }
        DXFComposite  * operator->() { return composite; }
        DXFComposite  * operator* () { return composite; }

        // \ru Для явной записи. \en For explicit record. 
        DXFComposite * GetComposite() const { return composite; } 
};


//------------------------------------------------------------------------------
// \ru DXF-конвертер.
// DXF - converter. \~ 
//---
class CONV_CLASS DXFConverter {
private:
  PArray<DXFCompositeRef>      composites;      // \ru Составляющие блок подблоки, тела, поверхности. \en Subblocks, solids, surfaces forming the block. 
  DXFCompositeRef *            model_space;     // \ru Корневой блок. \en Root block. 
  PArraySort<DXFCompositeData> readComposites;  // \ru Идентификаторы прочтенных блоков. \en Identifiers of read blocks. 
  IConvertorProperty3D *       property;        // \ru Свойства конвертера. \en Converter properties. 
  int                          stitch;          // \ru Нет информации. \en No information. 
  double                       factor;          // \ru Нет информации. \en No information. 
  IProgressIndicator *         indicator;       // \ru Индикатор хода процесса преобразования. \en Transformation progress indicator. 
  ptrdiff_t                    indicator_delta; // \ru Приращение индикатора на одну условную операцию. \en Increment of the indicator by one unit operation. 
  ptrdiff_t                    indicator_count; // \ru Значение счётчика индикатора. \en Value of the indicator counter. 
  
#ifdef C3D_DEBUG
  uint32                       prev_mili_sec;
  uint32                       current_mili_sec;
  uint32                       delta_mili_sec;
#endif // C3D_DEBUG

public:
  DXFConverter();
  ~DXFConverter();

  // \ru Задать признак сшивки. \en Set a flag of stitching. 
  void                    SetStitch        ( bool stitch );
  // \ru Получить признак сшивки. \en Get flag of stitching. 
  bool                    IsStitch         () const; 
  /// \ru Задать значение множителя. \en Set a value of multiplier. 
  void                    SetFactor        ( double factor );
  // \ru Получить значение множителя. \en Get value of multiplier. 
  double                  GetFactor        () const;
  // \ru Задать свойства конвертера. \en Specify converter properties. 
  void                    SetProperty      ( IConvertorProperty3D * property );
  // \ru Получить свойства конвертера. \en Get converter properties. 
  IConvertorProperty3D *  GetProperty      ();
  // \ru Инициировать пустой блок. \en Initialize an empty block. 
  void                    BeginComposite   ();
  // \ru Инициировать блок с данными.
  //          \en Initialize block with data. \~
  // matr - \ru Матрица, преобразующая данные блока к СК объемлющего блока.
  //                  \en Matrix transforming block data to coordinate system of the enclosing block. \~
  // data - \ru Данные самого блока.
  //                  \en Data of the block. \~
  bool                    BeginComposite   ( MbMatrix3D &matr, const DXFCompositeData & data );

  // \ru Завершить создание составного элемента.
  // \en Complete creation of a composite element. \~
  // \ru Если у последнего составного элемента:\n
  // - нет идентификатора;\n
  // - внутри нет вставок,\n
  // то это признак того, что этот блок создан только для сшивки геометрии.
  // В этом случае последний блок объединяется с предпоследним:
  // в предпоследний переносится геометрия, последний удаляется.
  // \en If the last composite element has:\n
  //- no identifier;\n
  //- no inserts,\n
  //then it is a creterion that the block is created only for stitching the geometry.
  //In this case the last block is united with the last but one:
  //the geometry is moved to the last but one, the last one is deleted. \~
  bool                    EndComposite     (); 
  // \ru Очистить конвертер. \en Clear the converter. 
  void                    Reset            (); 
  // \ru Отобразить текующее состояние хода операции. \en Show the current state of operation progress. 
  bool                    Indicate         ( ptrdiff_t count );

  // \ru Завершить создание документа.
  // \en Complete the document creation. \~
  void                    CompleteDocument ( ItModelDocument & model_document, 
                                             IProgressIndicator * indicator = nullptr   );

  // \ru Отобразить текующее состояние хода операции. \en Show the current state of operation progress. 
  void                    ConvertLastComposite( uint32 defaultColor ); 

  // \ru Добавить модельную грань. \en Add the model face. 
  void                    AddFace          ( MbFace & m_face );  

  // \ru Добавить модельную грань. \en Add the model face. 
  void                    AddGrid          ( MbGrid & m_grid );  

  // \ru Добавить тело.
  // \en Add a solid. \~
  // placement - \ru Положение тела в ЛСК.
  //                       \en Position of the solid in LCS. \~
  // model_solids - \ru Модельные тела.
  //                          \en Model solids. \~
  // model_faces - \ru Модельные грани.
  //                       \en Model solids. \~
  void                    AddSolidBody     ( MbPlacement3D & placement, 
                                             const std::vector<SPtr<MbItem> > & m_solids,
                                             const std::vector< SPtr<MbFace> > &  m_faces );
  // \ru Добавить пространственную кривую. \en Add a spatial curves. 
  void                    AddSpaceCurve    ( MbCurve3D & spaceCurve );
  // \ru Удалить значение из списка прочитанных идентификатров. \en Delete a value from the list of read identifiers. 
  void                    RemoveData       ( const DXFCompositeData & data );

private:
  // \ru Создать модельные грани по свойствам старых модельных тел.
  // \en Create model faces from properties of old model solids. \~
  // solids - \ru Положение тела в ЛСК.
  //                    \en Position of the solid in LCS. \~
  // stitchedSolids - \ru Модельные тела.
  //                          \en Model solids. \~
  // defaultColor - \ru Цвет по умолчанию.
  //                        \en Default color. \~
  // model_faces - \ru Модельные грани.
  //                        \en Model solids. \~
  void                    CreateModelFacesFromOldSolids( const std::vector<SPtr<MbItem> > & solids, 
                                                         const std::vector< SPtr<MbSolid> > & stitchedSolids,
                                                               uint32                         defaultColor,
                                                               std::vector< SPtr<MbFace> >  & modelFaces ) ;
  private:
  DXFConverter              ( const DXFConverter & ); // \ru не реализовано \en not implemented 
  DXFConverter & operator = ( const DXFConverter & ); // \ru не реализовано \en not implemented 
  
};


#endif // __DXF_CONVERTER_H