////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Свойства математических объектов.
         \en Properties of mathematical objects. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MB_PROPERTY_H
#define __MB_PROPERTY_H


#include <math_define.h>
#include <templ_sptr.h>
#include <stdio.h>
#include <stdlib.h>
#include <templ_p_array.h>
#include <reference_item.h>
#include <mb_property_title.h>
#include <mb_cart_point3d.h>
#include <mb_cart_point.h>
#include <name_item.h>


#define GET_PROPERTY_VALUE( v ) _GetPropertyValue( &(v), sizeof(v) )


class MbAttribute;
class MbCurve;
class MbCurve3D;
class MbMultiline;
class MbMatrix3D;
class MbFloatPoint;
class MbFloatPoint3D;
class MbFloatVector3D;
class MbTriangle;
class MbQuadrangle;
class MbElement;
class MbApex3D;
class MbPolygon3D;
class MbGrid;
class MbNamedAttributeContainer;
class MbPlacement3D;
class MbMarker;
class MbSurface;
class MbPoint3D;
class MbPointFrame;
class MbWireFrame;
class MbSolid;
class MbInstance;
class MbAssembly;
class MbConstraintSystem;
class MbMesh;            
class MbItem;
class MbSpaceInstance;
class MbPlaneInstance;
class MbAssistingItem;
class MbCollection;
class MbModel;
class MbRegion;
class MbDirection;
class MbPlacement;
class MbMatrix;
class MbMultiline;
class MbRegion;
class MbSymbol;
class MbThread;
class MbFunction;
class MbVertex;       
class MbEdge;         
class MbCurveEdge;    
class MbOrientedEdge; 
class MbLoop;         
class MbFace;         
class MbFaceShell;    
class MbName;         
class MbCreator;
class MbAttributeContainer;
class MbAttributeAction;
class MbTransactions;
template <class Vector>
class MbPntMatingData;
class MbProperties;
class MbPolymesh;


//----------------------------------------------------------------------------------------
  /** \brief \ru Типы свойств.
             \en Types of properties. \~
  \details \ru Типы свойств. \n
   Свойства дают доступ к внутренним данным объектов.
           \en Types of properties. \n
   Properties give access to internal data of objects. \~
  \ingroup Geometric_Items
  */
// ---
enum PrePropType 
{
  pt_UndefinedProp  = 0,                ///< \ru Свойство неизвестного типа данных. \en Property of unknown data type. \n

  // \ru Атомарные свойства. \en Atomic properties. 
  pt_BoolProp       = 1,                ///< \ru Логическое значение. \en Logical value. 
  pt_IntProp        = 2,                ///< \ru Целое значение. \en Integer value. 
  pt_UIntProp       = 3,                ///< \ru Беззнаковое целое значение. \en Unsigned integer value. 
  pt_DoubleProp     = 4,                ///< \ru Действительное значение. \en Real value. 
  pt_NDoubleProp    = 5,                ///< \ru Действительное значение с номером. \en Real value and index. 
  pt_StringProp     = 6,                ///< \ru Строковое значение. \en String value. 
  pt_CharProp       = 7,                ///< \ru Строковое значение. \en String value. 
  pt_VersionProp    = 8,                ///< \ru Свойство-версия. \en Version property. \n
  pt_AtomicPropLast = 20,               ///< \ru Атомарные свойства вставлять перед этим значением. \en Atomic properties should be inserted before this value. \n

  // \ru Комплексные свойства плоских объектов . \en Complex properties of planar objects. 
  pt_CartPointProp  = 21,               ///< \ru Свойство точки. \en Property of point. 
  pt_VectorProp     = 22,               ///< \ru Свойство вектора. \en Property of vector. 
  pt_DirectionProp  = 23,               ///< \ru Свойство вектора. \en Property of vector. 
  pt_PlacementProp  = 24,               ///< \ru Свойство системы координат. \en Property of coordinate system. 
  pt_MatrixProp     = 25,               ///< \ru Свойство матрицы. \en Property of matrix. 
  pt_CurveProp      = 26,               ///< \ru Свойство кривой. \en Property of curve. 
  pt_MultilineProp  = 27,               ///< \ru Свойство мультилинии. \en Property of multiline. 
  pt_RegionProp     = 28,               ///< \ru Свойство региона. \en Property of region. 
  pt_PntMatingProp  = 29,               ///< \ru Свойство сопряжения в точке. \en Property of conjugation at a point. \n
  pt_PlanarPropLast = 50,               ///< \ru Свойства плоских объектов вставлять перед этим значением. \en Properties of planar objects should be inserted before this value. \n
    
  // \ru Комплексные свойства пространственных объектов. \en Complex properties of spatial objects. 
  pt_CartPoint3DProp    = 51,           ///< \ru Свойство точки. \en Property of point. 
  pt_Vector3DProp       = 52,           ///< \ru Свойство вектора. \en Property of vector. 
  pt_Placement3DProp    = 53,           ///< \ru Свойство системы. \en Property of coordinate system. 
  pt_Matrix3DProp       = 54,           ///< \ru Свойство матрицы. \en Property of matrix. 
  pt_FloatPointProp     = 55,           ///< \ru Свойство параметра. \en Property of parameter. 
  pt_FloatPoint3DProp   = 56,           ///< \ru Свойство точки. \en Property of point. 
  pt_FloatVector3DProp  = 57,           ///< \ru Свойство вектора. \en Property of vector. 
  pt_TriangleProp       = 58,           ///< \ru Свойство треугольника. \en Property of triangle. 
  pt_QuadrangleProp     = 59,           ///< \ru Свойство четырехугольника. \en Property of quadrangle. 
  pt_ElementProp        = 60,           ///< \ru Свойство элемента. \en Property of element. 
  pt_Apex3DProp         = 61,           ///< \ru Свойство аперса. \en Property of apex. 
  pt_Polygon3DProp      = 62,           ///< \ru Свойство полигона. \en Property of polygon. 
  pt_GridProp           = 63,           ///< \ru Свойство триангуляции. \en Property of triangulation. \n
  pt_PolymeshRegionProp = 64,           ///< \ru Свойство региона. \en Property of polymesh region. \n
  pt_PolymeshSubProp    = 65,           ///< \ru Свойство подрегиона. \en Property of polymesh subregion. \n
  pt_PolymeshLoopProp   = 66,           ///< \ru Свойство цикла сегментации. \en Property of polymesh loop. \n
  pt_PolymeshEdgeProp   = 67,           ///< \ru Свойство ребра сегментации. \en Property of polymesh edge. \n
  pt_PolymeshVertexProp = 68,           ///< \ru Свойство вершины сегментации. \en Property of polymesh vertex. \n
  pt_SpatialPropLast    = 90,           ///< \ru Свойства пространственных объектов вставлять перед этим значением. \en Properties of spatial objects should be inserted before this value. \n

  // \ru Комплексные свойства геометрических объектов. \en Complex properties of geometric objects. 
  pt_FunctionProp       = 91,           ///< \ru Свойство функции. \en Property of function. 
  pt_Curve3DProp        = 92,           ///< \ru Свойство кривой. \en Property of curve. 
  pt_SurfaceProp        = 93,           ///< \ru Свойство поверхности. \en Property of surface. 
  pt_Point3DProp        = 94,           ///< \ru Свойство точки. \en Property of point.   
  pt_MarkerProp         = 95,           ///< \ru Свойство маркера ("точка присоединения"). \en Property of marker ("point of joint"). 
  pt_SymbolProp         = 96,           ///< \ru Свойство условного обозначения. \en Property of conventional notation. 
  pt_ThreadProp         = 97,           ///< \ru Свойство резьбы. \en Property of thread. 
  pt_Pnt3DMatingProp    = 98,           ///< \ru Свойство сопряжения в точке. \en Property of conjugation at a point. \n
  pt_GeomPropLast       = 120,          ///< \ru Свойства геометрических объектов вставлять перед этим значением. \en Properties of geometric objects should be inserted before this value. \n
    
  // \ru Комплексные свойства тел и топологических объектов. \en Complex properties of solids and topological objects. 
  pt_CreatorProp        = 121,          ///< \ru Свойство строителя тела. \en Property of solid creator. 
  pt_VertexProp         = 122,          ///< \ru Свойство вершины. \en Property of vertex. 
  pt_EdgeProp           = 123,          ///< \ru Свойство ребра-кривой. \en Property of edge curve. 
  pt_CurveEdgeProp      = 124,          ///< \ru Свойство ребра грани. \en Property of face edge. 
  pt_OrientedEdgeProp   = 125,          ///< \ru Свойство ориентированного ребра. \en Property of oriented edge. 
  pt_LoopProp           = 126,          ///< \ru Свойство цикла. \en Property of loop. 
  pt_FaceProp           = 127,          ///< \ru Свойство грани. \en Property of face.                            
  pt_FaceShellProp      = 128,          ///< \ru Свойство оболочки. \en Property of shell. 
  pt_NameProp           = 129,          ///< \ru Свойство имени. \en Property of name. \n
  pt_TopologyPropLast   = 150,          ///< \ru Свойства тел и топологических объектов вставлять перед этим значением. \en Properties of solids and topological objects should be inserted before this value. \n
    
  // \ru Комплексные свойства объектов модели. \en Complex properties of model objects. 
  pt_AssistingItemProp      = 151,      ///< \ru Свойство вспомогательного объекта. \en Property of assisting item. 
  pt_CollectionProp         = 152,      ///< \ru Свойство коллекции 3D элементов. \en Property of the collection of 3D elements. \n
  pt_PointFrameProp         = 153,      ///< \ru Свойство точечного каркаса. \en Property of point frame. 
  pt_WireFrameProp          = 154,      ///< \ru Свойство проволочного каркаса. \en Property of wire frame. 
  pt_SolidProp              = 155,      ///< \ru Свойство тела. \en Property of solid. 
  pt_InstanceProp           = 156,      ///< \ru Свойство вставки объекта. \en Property of object instance. 
  pt_AssemblyProp           = 157,      ///< \ru Свойство сборочной единицы. \en Property of assembly unit. 
  pt_ConstraintSystem       = 158,      ///< \ru Свойство системы ограничений. \en Property of constraint system.
  pt_MeshProp               = 159,      ///< \ru Свойство сетки. \en Property of mesh. 
  pt_SpaceInstanceProp      = 160,      ///< \ru Свойство объекта. \en Property of object. 
  pt_PlaneInstanceProp      = 161,      ///< \ru Свойство плоского объекта. \en Property of flat object. 
  pt_ConstraintModelProp    = 162,      ///< \ru Свойство схемы сопряжений. \en Property of conjugation scheme. 
  pt_ItemProp               = 163,      ///< \ru Свойство объекта. \en Property of object. 
  pt_ModelProp              = 164,      ///< \ru Свойство объектной модели. \en Property of object model. 
  pt_TransactionsProp       = 165,      ///< \ru Свойство журнала построения. \en Property of build log. 
  pt_AttributeContainerProp = 166,      ///< \ru Свойство контейнера атрибутов. \en Property of attribute container. 
  pt_AttributeProp          = 167,      ///< \ru Свойство атрибута. \en Property of attribute. 
  pt_NamedAttributeContainerProp = 168, ///< \ru Свойство именованного контейнера атрибутов. \en Property of named attribute container. 
  pt_AttributeActionProp    = 169,      ///< \ru Свойство атрибута. \en Property of attribute. \n
  pt_ModelPropLast          = 300,      ///< \ru Свойства объектов модели вставлять перед этим значением. \en Properties of model objects should be inserted before this value. \n

  pt_LastPropType           = 1000,     ///< \ru Последний тип свойства, все остальные добавлять перед ним. \en Last type of property, any other ones must be added before. 
};


//-----------------------------------------------------------------------------
/// \ru Структура соответствия типа объекта и типа свойства. \en Object type and property type correspondence structure. 
//---
template<class Type>
struct PropType { static const PrePropType propId = pt_UndefinedProp; };


//-----------------------------------------------------------------------------
// \ru Специализация шаблона PropType - организует таблицу соответствий \en PropType template specialization - organizes lookup table 
//---
template<> struct PropType<MbCartPoint>                  { static const PrePropType propId = pt_CartPointProp;               };
template<> struct PropType<MbVector>                     { static const PrePropType propId = pt_VectorProp;                  };
template<> struct PropType<MbDirection>                  { static const PrePropType propId = pt_DirectionProp;               };
template<> struct PropType<MbPlacement>                  { static const PrePropType propId = pt_PlacementProp;               };
template<> struct PropType<MbMatrix>                     { static const PrePropType propId = pt_MatrixProp;                  };
template<> struct PropType<MbCurve>                      { static const PrePropType propId = pt_CurveProp;                   };
template<> struct PropType<MbMultiline>                  { static const PrePropType propId = pt_MultilineProp;               };
template<> struct PropType<MbRegion>                     { static const PrePropType propId = pt_RegionProp;                  };
template<> struct PropType<MbSymbol>                     { static const PrePropType propId = pt_SymbolProp;                  };
template<> struct PropType<MbThread>                     { static const PrePropType propId = pt_ThreadProp;                  };
template<> struct PropType<MbCartPoint3D>                { static const PrePropType propId = pt_CartPoint3DProp;             };
template<> struct PropType<MbVector3D>                   { static const PrePropType propId = pt_Vector3DProp;                };
template<> struct PropType<MbPlacement3D>                { static const PrePropType propId = pt_Placement3DProp;             };
template<> struct PropType<MbMatrix3D>                   { static const PrePropType propId = pt_Matrix3DProp;                };
template<> struct PropType<MbFloatPoint>                 { static const PrePropType propId = pt_FloatPointProp;              };
template<> struct PropType<MbFloatPoint3D>               { static const PrePropType propId = pt_FloatPoint3DProp;            };
template<> struct PropType<MbFloatVector3D>              { static const PrePropType propId = pt_FloatVector3DProp;           };
template<> struct PropType<MbTriangle>                   { static const PrePropType propId = pt_TriangleProp;                };
template<> struct PropType<MbQuadrangle>                 { static const PrePropType propId = pt_QuadrangleProp;              };
template<> struct PropType<MbElement>                    { static const PrePropType propId = pt_ElementProp;                 };
template<> struct PropType<MbApex3D>                     { static const PrePropType propId = pt_Apex3DProp;                  };
template<> struct PropType<MbPolygon3D>                  { static const PrePropType propId = pt_Polygon3DProp;               };
template<> struct PropType<MbGrid>                       { static const PrePropType propId = pt_GridProp;                    };
template<> struct PropType<MbMarker>                     { static const PrePropType propId = pt_MarkerProp;                  };
template<> struct PropType<MbCurve3D>                    { static const PrePropType propId = pt_Curve3DProp;                 };
template<> struct PropType<MbSurface>                    { static const PrePropType propId = pt_SurfaceProp;                 };
template<> struct PropType<MbPoint3D>                    { static const PrePropType propId = pt_Point3DProp;                 };
template<> struct PropType<MbPointFrame>                 { static const PrePropType propId = pt_PointFrameProp;              };
template<> struct PropType<MbWireFrame>                  { static const PrePropType propId = pt_WireFrameProp;               };
template<> struct PropType<MbSolid>                      { static const PrePropType propId = pt_SolidProp;                   };
template<> struct PropType<MbInstance>                   { static const PrePropType propId = pt_InstanceProp;                };
template<> struct PropType<MbAssembly>                   { static const PrePropType propId = pt_AssemblyProp;                };
template<> struct PropType<MbConstraintSystem>           { static const PrePropType propId = pt_ConstraintSystem;            };
template<> struct PropType<MbMesh>                       { static const PrePropType propId = pt_MeshProp;                    };
template<> struct PropType<MbItem>                       { static const PrePropType propId = pt_ItemProp;                    };
template<> struct PropType<MbSpaceInstance>              { static const PrePropType propId = pt_SpaceInstanceProp;           };
template<> struct PropType<MbPlaneInstance>              { static const PrePropType propId = pt_PlaneInstanceProp;           };
template<> struct PropType<MbAssistingItem>              { static const PrePropType propId = pt_AssistingItemProp;           };
template<> struct PropType<MbCollection>                 { static const PrePropType propId = pt_CollectionProp;              };
template<> struct PropType<MbModel>                      { static const PrePropType propId = pt_ModelProp;                   };
template<> struct PropType<MbFunction>                   { static const PrePropType propId = pt_FunctionProp;                };
template<> struct PropType<MbVertex>                     { static const PrePropType propId = pt_VertexProp;                  };
template<> struct PropType<MbEdge>                       { static const PrePropType propId = pt_EdgeProp;                    };
template<> struct PropType<MbCurveEdge>                  { static const PrePropType propId = pt_CurveEdgeProp;               };
template<> struct PropType<MbOrientedEdge>               { static const PrePropType propId = pt_OrientedEdgeProp;            };
template<> struct PropType<MbLoop>                       { static const PrePropType propId = pt_LoopProp;                    };
template<> struct PropType<MbFace>                       { static const PrePropType propId = pt_FaceProp;                    };
template<> struct PropType<MbFaceShell>                  { static const PrePropType propId = pt_FaceShellProp;               };
template<> struct PropType<MbName>                       { static const PrePropType propId = pt_NameProp;                    };
template<> struct PropType<MbCreator>                    { static const PrePropType propId = pt_CreatorProp;                 };
template<> struct PropType<MbAttributeContainer>         { static const PrePropType propId = pt_AttributeContainerProp;      };
template<> struct PropType<MbAttribute>                  { static const PrePropType propId = pt_AttributeProp;               };
template<> struct PropType<MbAttributeAction>            { static const PrePropType propId = pt_AttributeActionProp;         };
template<> struct PropType<MbTransactions>               { static const PrePropType propId = pt_TransactionsProp;            };
template<> struct PropType<MbNamedAttributeContainer>    { static const PrePropType propId = pt_NamedAttributeContainerProp; };
template<> struct PropType<MbPntMatingData<MbVector> >   { static const PrePropType propId = pt_PntMatingProp;               };
template<> struct PropType<MbPntMatingData<MbVector3D> > { static const PrePropType propId = pt_Pnt3DMatingProp;             };


//------------------------------------------------------------------------------
/** \brief \ru Свойство. Только для внутреннего использования.
           \en Property. Internal use only. \~
  \details \ru Свойство является базовым классом для доступа к внутренним данным объектов.
    Наследники свойства могут содержать внутренние данные объектов или их копии.
    Свойства предназначены для просмотра и модификации внутренних данных объектов. \n
    Для хранения пользовательских данных объекта рекомендуем использовать атрибуты - объекты классов наследующих MbAttribute, уже имеющиеся в ядре или пользовательские. \n
           \en Property is the base class for access to internal data of objects.
    Inheritors of property may contain internal data of objects or its copies.
    Properties are intended for reading and changing internal data of objects. \n
    To store the user data of an object, we recommend using attributes - objects of classes that inherit MbAttribute, already available in the kernel or user-defined. \~
  \ingroup Model_Properties
*/
// ---
class MATH_CLASS MbProperty 
{
private:
  MbePrompt   prompt;     ///< \ru Номер подсказки. \en Number of hint string.
  const bool  changeable; ///< \ru Признак редактируемости. \en Attribute of editability.

public:
  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \details \ru Конструктор по параметрам. \n
             \en Constructor by parameters. \n \~
    \param[in] name      - \ru Номер подсказки.
                           \en Number of hint string. \~
    \param[in] change    - \ru Признак редактируемости.
                           \en The flag of edibility. \~
  */
  MbProperty( MbePrompt name, bool change = true ) : prompt( name ), changeable( change ) {}
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbProperty();

  /// \ru Выдать тип свойства. \en Get type of property. 
  virtual PrePropType IsA() const = 0;
  /// \ru Выдать строковое значение свойства. Устаревший метод, вместо него используйте GetCharValue_s() в сочетании с GetCharLen().
  /// \en Get string value of the property. Deprecated method, use GetCharValue_s() in combination with GetCharLen() instead.
  //DEPRECATE_DECLARE
    virtual void      GetCharValue( TCHAR * ) const = 0;
  /**
    \brief \ru Получить размер буфера, достаточный для размещения строкового значения свойства.
           \en Get buffer size, sufficient to accommodate the string value of the property. \~
  \details \ru Возвращает размер буфера, достаточный для размещения строкового значения свойства без учета нуль-терминатора. \n
           \en Returns buffer size, sufficient to accommodate the string value of the property, excluding the terminal null. \n \~
  */
  virtual size_t      GetCharLen() const;
  /**
    \brief \ru Выдать строковое значение свойства в виде строки с нуль-терминатором.
           \en Get the string value of the property as a string with a terminal null. \~
  \details \ru Выдать строковое значение свойства в виде строки с нуль-терминатором. \n
           \en Get the string value of the property as a string with a terminal null. \n \~
  \param[in] v    - \ru Указатель на символьный массив, куда копировать.
                    \en A pointer to a destination buffer to copy to. \~
  \param[in] size - \ru Размер символьного массива.
                    \en The size of the destination buffer. \~
  \return           \ru true в случае успеха, иначе -  false.
                    \en true if successful; otherwise, false. \~
  */
  virtual bool        GetCharValue_s( TCHAR * /* v */, size_t /* size */ ) const { return false; /* Not implemented. */ }

  /// \ru Выдать значение свойства. \en Get value of the property. 
  virtual void       _GetPropertyValue( void *, size_t size ) const = 0;
  /// \ru Установить новое значение свойства. \en Set the new value of the property. 
  virtual void        SetPropertyValue( TCHAR * ) = 0;
  /// \ru Выдать кортеж свойств составного свойства (не атомарный объект). \en Get tuple of the complex property (non-atomic object).
  virtual void        GetProperties( MbProperties & ) {}
  /// \ru Задать кортеж свойств составного свойства (не атомарный объект). \en Set tuple of the complex property (non-atomic object).
  virtual void        SetProperties( const MbProperties & ) {}
          /// \ru Выдать подсказку. \en Get a hint. 
  virtual size_t      GetPrompt() const { return prompt; }
          /// \ru Выдать подсказку. \en Get a hint. 
          MbePrompt & SetPrompt() { return prompt; }
          /// \ru Можно ли изменять данные. \en Is it possible to change data. 
          bool        IsChangeable() const { return changeable; }

OBVIOUS_PRIVATE_COPY( MbProperty )
}; // MbProperty


//------------------------------------------------------------------------------
/** \brief \ru bool свойство.
           \en Bool property. \~
  \details \ru bool свойство предназначено для просмотра и модификации данных типа bool.\n
           \en Bool property is intended for reading and changing data of boolean type.\n \~
  \ingroup Model_Properties
*/
// ---
class MATH_CLASS BoolProperty : public MbProperty {
public :
  bool value; ///< \ru Значение. \en Value. 

  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \details \ru Конструктор по параметрам. \n
             \en Constructor by parameters. \n \~
    \param[in] name      - \ru Номер подсказки.
                           \en Number of hint string. \~
    \param[in] initValue - \ru Значение.
                           \en Value. \~
    \param[in] change    - \ru Признак редактируемости.
                           \en The flag of edibility. \~
  */
  BoolProperty( MbePrompt name, bool initValue, bool change = true )
    : MbProperty( name, change )
    , value( initValue )
  {}
  /// \ru Деструктор. \en Destructor. 
  virtual ~BoolProperty();

  PrePropType IsA() const override; // \ru Выдать тип свойства. \en Get type of property.
  void        GetCharValue( TCHAR * v ) const override; // \ru Выдать строковое значение свойства. \en Get string value of the property.
  /**
    \brief \ru Получить размер буфера, достаточный для размещения строкового значения свойства.
           \en Get buffer size, sufficient to accommodate the string value of the property. \~
  \details \ru Возвращает размер буфера, достаточный для размещения строкового значения свойства без учета нуль-терминатора. \n
           \en Returns buffer size, sufficient to accommodate the string value of the property, excluding the terminal null. \n \~
  */
  size_t      GetCharLen() const override;
  /**
    \brief \ru Выдать строковое значение свойства в виде строки с нуль-терминатором.
           \en Get the string value of the property as a string with a terminal null. \~
  \details \ru Выдать строковое значение свойства в виде строки с нуль-терминатором. \n
           \en Get the string value of the property as a string with a terminal null. \n \~
  \param[in] v    - \ru Указатель на символьный массив, куда копировать.
                    \en A pointer to a destination buffer to copy to. \~
  \param[in] size - \ru Размер символьного массива.
                    \en The size of the destination buffer. \~
  \return           \ru true в случае успеха, иначе -  false.
                    \en true if successful; otherwise, false. \~
  */
  bool        GetCharValue_s( TCHAR * v, size_t size ) const override;   // \ru Выдать строковое значение свойства, возвращает true при успехе или false. \en Get string value of the property. Returns true if successful; otherwise, false.
  void       _GetPropertyValue( void * v, size_t size ) const override;  // \ru Выдать значение свойства. \en Get value of the property.
  void        SetPropertyValue( TCHAR * v ) override; // \ru Установить новое значение свойства. \en Set the new value of the property.

OBVIOUS_PRIVATE_COPY( BoolProperty )
}; // BoolProperty


//------------------------------------------------------------------------------
/** \brief \ru int свойство.
           \en Int property. \~
  \details \ru int свойство предназначено для просмотра и модификации данных типа int.\n
           \en Int property is intended for reading and changing data of integer type.\n \~
  \ingroup Model_Properties
*/
// ---
class MATH_CLASS IntProperty : public MbProperty {
public :
  int64 value; ///< \ru Значение. \en Value. 

  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \details \ru Конструктор по параметрам. \n
             \en Constructor by parameters. \n \~
    \param[in] name      - \ru Номер подсказки.
                           \en Number of hint string. \~
    \param[in] initValue - \ru Значение.
                           \en Value. \~
    \param[in] change    - \ru Признак редактируемости.
                           \en The flag of edibility. \~
  */
  IntProperty( MbePrompt name, int64 initValue, bool change = true )
    : MbProperty( name, change )
    , value( (int64)initValue )
  {}

  /// \ru Деструктор. \en Destructor. 
  virtual ~IntProperty();

  PrePropType IsA() const override; // \ru Выдать тип свойства. \en Get type of property.
  void        GetCharValue( TCHAR * v ) const override; // \ru Выдать строковое значение свойства. \en Get string value of the property.
  /**
    \brief \ru Получить размер буфера, достаточный для размещения строкового значения свойства.
           \en Get buffer size, sufficient to accommodate the string value of the property. \~
  \details \ru Возвращает размер буфера, достаточный для размещения строкового значения свойства без учета нуль-терминатора. \n
           \en Returns buffer size, sufficient to accommodate the string value of the property, excluding the terminal null. \n \~
  */
  size_t      GetCharLen() const override;
  /**
    \brief \ru Выдать строковое значение свойства в виде строки с нуль-терминатором.
           \en Get the string value of the property as a string with a terminal null. \~
  \details \ru Выдать строковое значение свойства в виде строки с нуль-терминатором. \n
           \en Get the string value of the property as a string with a terminal null. \n \~
  \param[in] v    - \ru Указатель на символьный массив, куда копировать.
                    \en A pointer to a destination buffer to copy to. \~
  \param[in] size - \ru Размер символьного массива.
                    \en The size of the destination buffer. \~
  \return           \ru true в случае успеха, иначе -  false.
                    \en true if successful; otherwise, false. \~
  */
  bool        GetCharValue_s( TCHAR * v, size_t size ) const override;  // \ru Выдать строковое значение свойства, возвращает true при успехе или false. \en Get string value of the property. Returns true if successful; otherwise, false.
  void       _GetPropertyValue( void * v, size_t size ) const override; // \ru Выдать значение свойства. \en Get value of the property.
  void        SetPropertyValue( TCHAR * v ) override; // \ru Установить новое значение свойства. \en Set the new value of the property.

OBVIOUS_PRIVATE_COPY( IntProperty )
}; // IntProperty


//------------------------------------------------------------------------------
/** \brief \ru uint свойство.
           \en Uint property. \~
  \details \ru uint свойство предназначено для просмотра и модификации данных типа uint64.\n
           \en Uint property is intended for reading and changing data of uint64 type.\n \~
  \ingroup Model_Properties
*/
// ---
class MATH_CLASS UIntProperty : public MbProperty {
public :
  uint64 value; ///< \ru Значение. \en Value. 
  
  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \details \ru Конструктор по параметрам. \n
             \en Constructor by parameters. \n \~
    \param[in] name      - \ru Номер подсказки.
                           \en Number of hint string. \~
    \param[in] initValue - \ru Значение.
                           \en Value. \~
    \param[in] change    - \ru Признак редактируемости.
                           \en The flag of edibility. \~
  */
  UIntProperty( MbePrompt name, size_t initValue, bool change = true )
    : MbProperty( name, change )
    , value( (uint64)initValue )
  {}

  /// \ru Деструктор. \en Destructor. 
  virtual ~UIntProperty();
  
  PrePropType IsA() const override; // \ru Выдать тип свойства. \en Get type of property.
  void        GetCharValue( TCHAR * v ) const override; // \ru Выдать строковое значение свойства. \en Get string value of the property.
  /**
    \brief \ru Получить размер буфера, достаточный для размещения строкового значения свойства.
           \en Get buffer size, sufficient to accommodate the string value of the property. \~
  \details \ru Возвращает размер буфера, достаточный для размещения строкового значения свойства без учета нуль-терминатора. \n
           \en Returns buffer size, sufficient to accommodate the string value of the property, excluding the terminal null. \n \~
  */
  size_t      GetCharLen() const override;
  /**
    \brief \ru Выдать строковое значение свойства в виде строки с нуль-терминатором.
           \en Get the string value of the property as a string with a terminal null. \~
  \details \ru Выдать строковое значение свойства в виде строки с нуль-терминатором. \n
           \en Get the string value of the property as a string with a terminal null. \n \~
  \param[in] v    - \ru Указатель на символьный массив, куда копировать.
                    \en A pointer to a destination buffer to copy to. \~
  \param[in] size - \ru Размер символьного массива.
                    \en The size of the destination buffer. \~
  \return           \ru true в случае успеха, иначе -  false.
                    \en true if successful; otherwise, false. \~
  */
  bool        GetCharValue_s( TCHAR * v, size_t size ) const override;  // \ru Выдать строковое значение свойства, возвращает true при успехе или false. \en Get string value of the property. Returns true if successful; otherwise, false.
  void       _GetPropertyValue( void * v, size_t size ) const override; // \ru Выдать значение свойства. \en Get value of the property.
  void        SetPropertyValue( TCHAR * v ) override; // \ru Установить новое значение свойства. \en Set the new value of the property.

OBVIOUS_PRIVATE_COPY( UIntProperty )
}; // UIntProperty


//------------------------------------------------------------------------------
/** \brief \ru double свойство.
           \en Double property. \~
  \details \ru double свойство предназначено для просмотра и модификации данных типа double.\n
           \en Double property is intended for reading and changing data of double type.\n \~
  \ingroup Model_Properties
*/
// ---
class MATH_CLASS DoubleProperty : public MbProperty {
public :
  double value; ///< \ru Значение. \en Value. 

  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \details \ru Конструктор по параметрам. \n
             \en Constructor by parameters. \n \~
    \param[in] name      - \ru Номер подсказки.
                           \en Number of hint string. \~
    \param[in] initValue - \ru Значение.
                           \en Value. \~
    \param[in] change    - \ru Признак редактируемости.
                           \en The flag of edibility. \~
  */
  DoubleProperty( MbePrompt name, double initValue, bool change = true )
    : MbProperty( name, change )
    , value( initValue )
  {}
  /// \ru Деструктор. \en Destructor. 
  virtual ~DoubleProperty();

  PrePropType IsA() const override; // \ru Выдать тип свойства. \en Get type of property.
  void        GetCharValue( TCHAR * v ) const override; // \ru Выдать строковое значение свойства. \en Get string value of the property.
  /**
    \brief \ru Получить размер буфера, достаточный для размещения строкового значения свойства.
           \en Get buffer size, sufficient to accommodate the string value of the property. \~
  \details \ru Возвращает размер буфера, достаточный для размещения строкового значения свойства без учета нуль-терминатора. \n
           \en Returns buffer size, sufficient to accommodate the string value of the property, excluding the terminal null. \n \~
  */
  size_t      GetCharLen() const override;
  /**
    \brief \ru Выдать строковое значение свойства в виде строки с нуль-терминатором.
           \en Get the string value of the property as a string with a terminal null. \~
  \details \ru Выдать строковое значение свойства в виде строки с нуль-терминатором. \n
           \en Get the string value of the property as a string with a terminal null. \n \~
  \param[in] v    - \ru Указатель на символьный массив, куда копировать.
                    \en A pointer to a destination buffer to copy to. \~
  \param[in] size - \ru Размер символьного массива.
                    \en The size of the destination buffer. \~
  \return           \ru true в случае успеха, иначе -  false.
                    \en true if successful; otherwise, false. \~
  */
  bool        GetCharValue_s( TCHAR * v, size_t size ) const override;  // \ru Выдать строковое значение свойства, возвращает true при успехе или false. \en Get string value of the property. Returns true if successful; otherwise, false.
  void       _GetPropertyValue( void * v, size_t size ) const override; // \ru Выдать значение свойства. \en Get value of the property.
  void        SetPropertyValue( TCHAR * v ) override; // \ru Установить новое значение свойства. \en Set the new value of the property.

OBVIOUS_PRIVATE_COPY( DoubleProperty )
}; // DoubleProperty


//------------------------------------------------------------------------------
/** \brief \ru double свойство с номером.
           \en Double property with number. \~
  \details \ru double свойство с номером предназначено для просмотра и модификации данных типа double, имеющих порядковый номер.\n
           \en Double property with number is intended for reading and changing data of double type which have number.\n \~
  \ingroup Model_Properties
*/
// ---
class MATH_CLASS NDoubleProperty : public DoubleProperty {
public :
  uint32 number; ///< \ru Номер. \en Number. 

  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \details \ru Конструктор по параметрам. \n
             \en Constructor by parameters. \n \~
    \param[in] name      - \ru Номер подсказки.
                           \en Number of hint string. \~
    \param[in] initValue - \ru Значение.
                           \en Value. \~
    \param[in] change    - \ru Признак редактируемости.
                           \en The flag of edibility. \~
    \param[in] n         - \ru Номер.
                           \en Number. \~
  */
  NDoubleProperty( MbePrompt name, double initValue, bool change = true, uint32 n = 0 )
    : DoubleProperty( name, initValue, change )
    , number( n )
  {}
  /// \ru Деструктор. \en Destructor. 
  virtual ~NDoubleProperty();

  PrePropType IsA() const override; // \ru Выдать тип свойства. \en Get type of property.
  void        GetCharValue( TCHAR * v ) const override; // \ru Выдать строковое значение свойства. \en Get string value of the property.
  /**
    \brief \ru Получить размер буфера, достаточный для размещения строкового значения свойства.
           \en Get buffer size, sufficient to accommodate the string value of the property. \~
  \details \ru Возвращает размер буфера, достаточный для размещения строкового значения свойства без учета нуль-терминатора. \n
           \en Returns buffer size, sufficient to accommodate the string value of the property, excluding the terminal null. \n \~
  */
  size_t      GetCharLen() const override;
  /**
    \brief \ru Выдать строковое значение свойства в виде строки с нуль-терминатором.
           \en Get the string value of the property as a string with a terminal null. \~
  \details \ru Выдать строковое значение свойства в виде строки с нуль-терминатором. \n
           \en Get the string value of the property as a string with a terminal null. \n \~
  \param[in] v    - \ru Указатель на символьный массив, куда копировать.
                    \en A pointer to a destination buffer to copy to. \~
  \param[in] size - \ru Размер символьного массива.
                    \en The size of the destination buffer. \~
  \return           \ru true в случае успеха, иначе -  false.
                    \en true if successful; otherwise, false. \~
  */
  bool        GetCharValue_s( TCHAR * v, size_t size ) const override;  // \ru Выдать строковое значение свойства, возвращает true при успехе или false. \en Get string value of the property. Returns true if successful; otherwise, false.
  void       _GetPropertyValue( void * v, size_t size ) const override; // \ru Выдать значение свойства. \en Get value of the property.
  void        SetPropertyValue( TCHAR * v ) override; // \ru Установить новое значение свойства. \en Set the new value of the property.

OBVIOUS_PRIVATE_COPY( NDoubleProperty )
}; // NDoubleProperty


//----------------------------------------------------------------------------------------
/** \brief \ru string свойство.
           \en String property. \~
  \details \ru string свойство предназначено для просмотра и модификации данных типа TCHAR *.\n
           \en String property is intended for reading and changing TCHAR * like data.\n \~
  \ingroup Model_Properties
*/
// ---
class MATH_CLASS StringProperty : public MbProperty
{
  TCHAR * value; ///< \ru Значение. \en Value.

public:
  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \details \ru Конструктор по параметрам.
             Максимальная длина строки ограничена 128 символами.
             Если длина строки инициализации больше, то строка-значение обрезается.\n
             \en Constructor by parameters.
             The maximum string length is limited to 128 characters.
             If the length of the initialization string is longer, the string-value is truncated. \n \~
    \param[in] name      - \ru Номер подсказки.
                           \en Number of hint string. \~
    \param[in] initValue - \ru Строка инициализации.
                           \en Initialization string. \~
    \param[in] change    - \ru Признак редактируемости.
                           \en The flag of editability. \~
  */
  StringProperty( MbePrompt name, const TCHAR * initValue, bool change = true );
  /// \ru Деструктор. \en Destructor. 
  virtual ~StringProperty();

  PrePropType IsA() const override; // \ru Выдать тип свойства. \en Get type of property.
  void        GetCharValue( TCHAR * v ) const override; // \ru Выдать строковое значение свойства. \en Get string value of the property.
  /**
    \brief \ru Получить размер буфера, достаточный для размещения строкового значения свойства.
           \en Get buffer size, sufficient to accommodate the string value of the property. \~
  \details \ru Возвращает размер буфера, достаточный для размещения строкового значения свойства без учета нуль-терминатора. \n
           \en Returns buffer size, sufficient to accommodate the string value of the property, excluding the terminal null. \n \~
  */
  size_t      GetCharLen() const override;
  /**
    \brief \ru Выдать строковое значение свойства в виде строки с нуль-терминатором.
           \en Get the string value of the property as a string with a terminal null. \~
  \details \ru Выдать строковое значение свойства в виде строки с нуль-терминатором. \n
           \en Get the string value of the property as a string with a terminal null. \n \~
  \param[in] v    - \ru Указатель на символьный массив, куда копировать.
                    \en A pointer to a destination buffer to copy to. \~
  \param[in] size - \ru Размер символьного массива.
                    \en The size of the destination buffer. \~
  \return           \ru true в случае успеха, иначе -  false.
                    \en true if successful; otherwise, false. \~
  */
  bool        GetCharValue_s( TCHAR * v, size_t size ) const override;  // \ru Выдать строковое значение свойства, возвращает true при успехе или false. \en Get string value of the property. Returns true if successful; otherwise, false.
  void       _GetPropertyValue( void * v, size_t size ) const override; // \ru Выдать значение свойства. \en Get value of the property.
  void        SetPropertyValue( TCHAR * v ) override; // \ru Установить новое значение свойства. \en Set the new value of the property.
  const TCHAR *       CharValue() const { return value; }
  
OBVIOUS_PRIVATE_COPY(StringProperty)
}; // StringProperty


typedef StringProperty CharProperty; 


//------------------------------------------------------------------------------
/** \brief \ru Version свойство.
           \en Version property. \~
  \details \ru Version свойство предназначено для просмотра и модификации данных типа VERSION.\n
           \en Version property is intended for reading and changing VERSION like data.\n \~
  \ingroup Model_Properties
*/
// ---
class MATH_CLASS VersionProperty : public MbProperty {
public :
  VERSION value; ///< \ru Значение. \en Value. 

  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \details \ru Конструктор по параметрам. \n
             \en Constructor by parameters. \n \~
    \param[in] name      - \ru Номер подсказки.
                           \en Number of hint string. \~
    \param[in] initValue - \ru Версия.
                           \en Version. \~
    \param[in] change    - \ru Признак редактируемости.
                           \en The flag of edibility. \~
  */
  VersionProperty( MbePrompt name, VERSION initValue, bool change = true )
    : MbProperty( name, change )
    , value( initValue )
  {}
  /// \ru Деструктор. \en Destructor. 
  virtual ~VersionProperty() {}

  PrePropType IsA() const override; // \ru Выдать тип свойства. \en Get type of property.
  void        GetCharValue( TCHAR * v ) const override; // \ru Выдать строковое значение свойства. \en Get string value of the property.
  size_t      GetCharLen() const override; // \ru Выдать размер буфера для строкового значения свойства (без учета нуль-терминатора). \en Get buffer size for string value of the property (excluding the terminal null).
  bool        GetCharValue_s( TCHAR * v, size_t size ) const override;  // \ru Выдать строковое значение свойства, возвращает true при успехе или false. \en Get string value of the property. Returns true if successful; otherwise, false.
  void       _GetPropertyValue( void * v, size_t size ) const override; // \ru Выдать значение свойства. \en Get value of the property.
  void        SetPropertyValue( TCHAR * v ) override; // \ru Установить новое значение свойства. \en Set the new value of the property.

OBVIOUS_PRIVATE_COPY( VersionProperty )
}; // IntProperty


//------------------------------------------------------------------------------
/** \brief \ru Выдать строковое значение данного свойства для данного его поля.
           \en Get string value of given property for its given field. \~
  \details \ru Функция определена для случая "по умолчанию", для конкретных типов FieldType, 
    следует перегрузить для статического сопоставления типов компилятором.\n
           \en In "default" case function is defined for explicit types FieldType, 
    it should be overloaded for static mapping of types by compiler.\n \~
  \ingroup Model_Properties
*/
//---
template <class PropType, class FieldType> 
inline void GetCharValue( const PropType *, const FieldType *, uint32 n, TCHAR * v )
{
  C3D_ASSERT( v != nullptr );
  if ( v != nullptr ) {
    if ( n == 0 ) {
      v[0] = _T(' ');
      v[1] = _T('\0');
    }
    else
      _sntprintf( v, 64, _T("%d\t"), n );
  }
}


//------------------------------------------------------------------------------
/** \brief \ru Выдать свойства двумерной точки.
           \en Get properties of two-dimensional point. \~
  \details \ru Выдать свойства двумерной точки MbCartPoint.\n
           \en Get properties of two-dimensional point MbCartPoint.\n \~
  \ingroup Model_Properties
*/
//---
template <class PropType> 
inline void GetCharValue( const PropType *, const MbCartPoint * value, uint32 n, TCHAR * v )
{
  C3D_ASSERT( value != nullptr && v != nullptr );
  if ( value != nullptr && v != nullptr ) {
    if ( n == 0 )
      _sntprintf( v, 64, _T("%.3f\t%.3f"), value->x, value->y );
    else
      _sntprintf( v, 64, _T("%d   %.3f\t%.3f"), n, value->x, value->y );
  }
}


//------------------------------------------------------------------------------
/** \brief \ru Выдать свойства двумерного вектора.
           \en Get properties of two-dimensional vector. \~
  \details \ru Выдать свойства двумерного вектора MbVector.\n
           \en Get properties of two-dimensional vector MbVector.\n \~
  \ingroup Model_Properties
*/
// ---
template <class PropType> 
inline void GetCharValue( const PropType *, const MbVector * value, uint32 n, TCHAR * v ) 
{
  C3D_ASSERT( value != nullptr && v != nullptr );
  if ( value != nullptr && v != nullptr ) {
    if ( n == 0 )
      _sntprintf( v, 64, _T("%.3f\t%.3f"), value->x, value->y );
    else
      _sntprintf( v, 64, _T("%d   %.3f\t%.3f"), n, value->x, value->y );
  }
}


//------------------------------------------------------------------------------
/** \brief \ru Выдать свойства двумерного нормированного вектора.
           \en Get properties of two-dimensional normalized vector. \~
  \details \ru Выдать свойства двумерного нормированного вектора MbDirection.\n
           \en Get properties of two-dimensional normalized vector MbDirection.\n \~
  \ingroup Model_Properties
*/
// ---
template <class PropType> 
inline void GetCharValue( const PropType *, const MbDirection * value, uint32 n, TCHAR * v ) 
{
  C3D_ASSERT( value != nullptr && v != nullptr );
  if ( value != nullptr && v != nullptr ) {
    double angle(0.0);
    if ( value->ax==0 && value->ay==0 )
      angle = 0.0;
    else
      angle = 180 / M_PI * atan2( value->ay, value->ax );
    if ( n == 0 )
      _sntprintf( v, 64, _T("%.3f\t"), angle );
    else
      _sntprintf( v, 64, _T("%d   %.3f\t"), n, angle );
  }
}


//------------------------------------------------------------------------------
/** \brief \ru Выдать свойства трёхмерной точки.
           \en Get properties of three-dimensional point. \~
  \details \ru Выдать свойства трёхмерной точки MbCartPoint3D.\n
           \en Get properties of three-dimensional point MbCartPoint3D.\n \~
  \ingroup Model_Properties
*/
// ---
template <class PropType> 
inline void GetCharValue( const PropType *, const MbCartPoint3D * value, uint32 n, TCHAR * v ) 
{
  C3D_ASSERT( value != nullptr && v != nullptr );
  if ( value != nullptr && v != nullptr ) {
    if ( n == 0 )
      _sntprintf( v, 64, _T("%.3f\t%.3f\t%.3f"), value->x, value->y, value->z );
    else
      _sntprintf( v, 64, _T("%d   %.3f\t%.3f\t%.3f"), n, value->x, value->y, value->z );
  }
}


//------------------------------------------------------------------------------
/** \brief \ru Выдать свойства трёхмерного вектора.
           \en Get properties of three-dimensional vector. \~
  \details \ru Выдать свойства трёхмерного вектора MbVector3D.\n
           \en Get properties of three-dimensional vector MbVector3D.\n \~
  \ingroup Model_Properties
*/
// ---
template <class PropType> 
inline void GetCharValue( const PropType *, const MbVector3D * value, uint32 n, TCHAR * v ) 
{
  C3D_ASSERT( value != nullptr && v != nullptr );
  if ( value != nullptr && v != nullptr ) {
    if ( n == 0 )
      _sntprintf( v, 64, _T("%.3f\t%.3f\t%.3f"), value->x, value->y, value->z );
    else
      _sntprintf( v, 64, _T("%d   %.3f\t%.3f\t%.3f"), n, value->x, value->y, value->z );
  }
}


//------------------------------------------------------------------------------
/** \brief \ru Выдать свойства имени объекта.
           \en Get properties of object name. \~
  \details \ru Выдать свойства имени объекта MbName.\n
           \en Get properties of object name MbName.\n \~
  \ingroup Model_Properties
*/
// ---
template <class PropType> 
inline void GetCharValue( const PropType *, const MbName * value, uint32 n, TCHAR * v ) 
{
  C3D_ASSERT( v != nullptr );
  if ( v != nullptr ) {
    if ( value != nullptr ) {
      c3d::string_t str;
      value->ToString( str );

      if ( n !=0 ) {
        _sntprintf( v, 64, _T("%d   "), n );
        _tcscat( v, str.c_str() );
      }
      else {
        _tcscpy( v, str.c_str() );
      }
    }
  }
}


//------------------------------------------------------------------------------
/** \brief \ru Свойство объекта.
           \en The property of the object. \~
  \details \ru Обертка, реализующая свойство объекта с настройкой владения им.\n
           \en Wrapper that implements property of an object with its ownership setting.\n \~
  \ingroup Model_Properties
*/
// ---
template <class Type>
class MathItemProperty : public MbProperty {
public :
  Type * value;  ///< \ru Объект. \en Object. 
  uint32 number; ///< \ru Номер.  \en Number. 
  
public :
  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \details \ru Конструктор по параметрам. \n
             \en Constructor by parameters. \n \~
    \param[in] name      - \ru Номер подсказки.
                           \en Number of hint string. \~
    \param[in] initValue - \ru Указатель на объект.
                           \en Object pointer. \~
    \param[in] change    - \ru Признак редактируемости.
                           \en The flag of edibility. \~
    \param[in] n         - \ru Номер.
                           \en Number. \~
  */
  MathItemProperty( MbePrompt name, Type * initValue, bool change, uint32 n = 0 ) 
    : MbProperty( name, change )
    , value( initValue )
    , number( n )
  {}
  /// \ru Деструктор. \en Destructor. 
  virtual ~MathItemProperty() {}
  
public :
  // \ru Выдать тип свойства. \en Get type of property. 
  PrePropType IsA() const override { return PropType<Type>::propId; }
  // \ru Выдать строковое значение свойства. \en Get string value of the property. 
  void        GetCharValue( TCHAR * v ) const override { ::GetCharValue( this, value, number, v ); }
  // \ru Выдать свойства неатомарного объекта. \en Get properties of the non-atomic object.
  void        GetProperties( MbProperties & ) override;
  // \ru Задать свойства неатомарного объекта. \en Set properties of the non-atomic object.
  void        SetProperties( const MbProperties & ) override;
  // \ru Выдать значение свойства. \en Get value of the property. 
  void       _GetPropertyValue( void * v, size_t /*size*/ ) const  override{ *(Type**)v = value; }
  // \ru Установить новое значение свойства. \en Set the new value of the property. 
  void        SetPropertyValue( TCHAR * ) override {}
  
  OBVIOUS_PRIVATE_COPY( MathItemProperty )
};


//------------------------------------------------------------------------------
/** \brief  \ru Свойство объекта.
            \en The property of the object. \~
  \details  \ru Обертка, реализующая свойство объекта с настройкой владения им.\n
            \en Wrapper that implements property of an object with its ownership setting.\n \~
  \ingroup Model_Properties
*/
// ---
template <class Type>
class MathItemCopyProperty : public MbProperty {
public:
  Type   value;  ///< \ru Объект. \en Object. 
  uint32 number; ///< \ru Номер.  \en Number. 

public:
  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \details \ru Конструктор по параметрам. \n
             \en Constructor by parameters. \n \~
    \param[in] name      - \ru Номер подсказки.
                           \en Number of hint string. \~
    \param[in] initValue - \ru Объект.
                           \en Object. \~
    \param[in] change    - \ru Признак редактируемости.
                           \en The flag of edibility. \~
    \param[in] n         - \ru Номер.
                           \en Number. \~
  */
  MathItemCopyProperty( MbePrompt name, const Type & initValue, bool change, uint32 n = 0 )
    : MbProperty( name, change )
    , value( initValue )
    , number( n )
  {}
  /// \ru Деструктор. \en Destructor. 
  virtual ~MathItemCopyProperty() {}

public:
  // \ru Выдать тип свойства. \en Get type of property. 
  PrePropType IsA() const override { return PropType<Type>::propId; }
  // \ru Выдать строковое значение свойства. \en Get string value of the property. 
  void        GetCharValue( TCHAR * v ) const override { ::GetCharValue( this, &value, number, v ); }
  // \ru Выдать свойства неатомарного объекта. \en Get properties of the non-atomic object.
  void        GetProperties( MbProperties & ) override;
  // \ru Выдать значение свойства. \en Get value of the property. 
  void       _GetPropertyValue( void * v, size_t /*size*/ ) const override { *(Type**)v = const_cast<Type*>(&value); }
  // \ru Установить новое значение свойства. \en Set the new value of the property. 
  void        SetPropertyValue( TCHAR * ) override {}

  OBVIOUS_PRIVATE_COPY( MathItemCopyProperty )
};


//------------------------------------------------------------------------------
/** \brief \ru Свойство объекта без изменения состояния (read only).
           \en Read-only reference item property without setter. \~
  \details \ru Обертка, реализующая свойство объекта со счетчиком ссылок.\n
           \en Wrapper that implements read-only property of a ref count object.\n \~
  \ingroup Model_Properties
*/
// ---
template <class Type>
class ConstRefItemProperty : public MbProperty 
{
protected:
  SPtr<Type> value;  ///< \ru Объект. \en Object. 
  uint32     number; ///< \ru Номер.  \en Number. 
  
protected: 
  // Inheritance-only constructor.
  ConstRefItemProperty( MbePrompt name, Type * initValue, bool change, uint32 n ) 
    : MbProperty( name, change )
    , value( initValue )
    , number( n )
  {}  

public :
  /** \brief \ru Конструктор свойства не допускающего изменение.
             \en The constructor of a non-modifiable property. \~
    \param[in] name      - \ru Номер подсказки.
                           \en Number of hint string. \~
    \param[in] initValue - \ru Объект.
                           \en Object. \~
    \param[in] n         - \ru Номер.
                           \en Number. \~
  */
  ConstRefItemProperty( MbePrompt name, Type * initValue, uint32 n = 0 ) 
    : MbProperty( name, false )
    , value( initValue    )
    , number( n )
  {} 

  // \ru Выдать тип свойства. \en Get type of property. 
  PrePropType IsA() const override { return PropType<Type>::propId; }
  // \ru Выдать строковое значение свойства. \en Get string value of the property. 
  void  GetCharValue( TCHAR * v ) const override { ::GetCharValue( this, value.get(), number, v ); }
  // \ru Выдать свойства неатомарного объекта. \en Get properties of the non-atomic object.
  void  GetProperties( MbProperties & ) override;
  // \ru Задать свойства неатомарного объекта. \en Set properties of the non-atomic object.
  //void  SetProperties( const MbProperties & ) override {}
  // \ru Выдать значение свойства. \en Get value of the property. 
  void _GetPropertyValue( void * v, size_t /*size*/ ) const override { *(Type**)v = value.get(); }
  // \ru Установить новое значение свойства. \en Set the new value of the property.
  void  SetPropertyValue( TCHAR * ) override {}
  
  OBVIOUS_PRIVATE_COPY( ConstRefItemProperty )
};

//------------------------------------------------------------------------------
/** \brief \ru Свойство объекта (read/write).
           \en Read/write property of ref count object. \~
  \details \ru Обертка, реализующая чтение и запись свойств объекта с подсчетом ссылок.\n
           \en Wrapper that implements the read/write properties of a ref count object.\n \~
  \ingroup Model_Properties
*/
// ---
template <class Type>
class RefItemProperty : public ConstRefItemProperty<Type>
{
public :
  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \details \ru Конструктор по параметрам. \n
             \en Constructor by parameters. \n \~
    \param[in] name      - \ru Номер подсказки.
                           \en Number of hint string. \~
    \param[in] initValue - \ru Объект.
                           \en Object. \~
    \param[in] n         - \ru Номер.
                           \en Number. \~
  */
  RefItemProperty( MbePrompt name, Type * initValue, uint32 n = 0 ) 
    : ConstRefItemProperty<Type>( name, initValue, true /*change*/, n)
  {}  
 
  // \ru Задать свойства неатомарного объекта объекта. \en Set properties of the non-atomic object.
  void  SetProperties( const MbProperties & ) override;
  // \ru Установить новое значение свойства. \en Set the new value of the property.
  void  SetPropertyValue( TCHAR * ) override {}

public:
  // Deprecated.
  RefItemProperty( MbePrompt name, Type * initValue, bool change, uint32 n ) 
    : ConstRefItemProperty<Type>( name, initValue, change, n)
  {} 
  
OBVIOUS_PRIVATE_COPY( RefItemProperty )
};


//------------------------------------------------------------------------------
/** \brief \ru Множество свойств объекта. Только для внутреннего использования.
           \en Set of object properties. Internal use only. \~
  \details \ru Множество свойств объекта представляет собой контейнер, вызывающий деструктор своих элементов. \n
               Для хранения пользовательских данных объекта рекомендуем использовать атрибуты - объекты классов наследующих MbAttribute, уже имеющиеся в ядре или пользовательские. \n
           \en Set of object properties is container that calls destructor of its elements. \n
               To store the user data of an object, we recommend using attributes, objects of classes that inherit MbAttribute, already available in the kernel or user-defined. \~
  \ingroup Model_Properties
*/
// ---
class MATH_CLASS MbProperties : public PArray<MbProperty> 
{
  MbePrompt name; ///< \ru Имя объекта. \en A name of an object. 

public:
  /// \ru Конструктор. \en Constructor. 
  MbProperties() 
    : PArray<MbProperty>()
    , name( IDS_ITEM_0000 ) 
  {}

public:
  /// \ru Выдать имя объекта. \en Get name of object. 
  MbePrompt &   SetName() { return name; } 
  /// \ru Выдать имя объекта. \en Get name of object. 
  size_t        GetName() const { return (size_t)name; } 
  /// \ru Выдать имя объекта. \en Get name of object. 
  MbePrompt     Name() const { return name; } 
  /** \brief \ru Установить имя объекта.
             \en Set name of the object. \~
    \details \ru Установить имя объекта. \n
             \en Set name of the object. \n \~
    \param[in] s - \ru Номер подсказки.
                   \en Number of hint string. \~
  */
  void          SetName( MbePrompt s ) { name = s; } 
  /** \brief \ru Установить имя объекта.
             \en Set name of the object. \~
    \details \ru Установить имя объекта. \n
             \en Set name of the object. \n \~
    \param[in] s - \ru Номер подсказки.
                   \en Number of hint string. \~
  */
  void          SetName( size_t s ) { name = (MbePrompt)s; } 
  /** \brief \ru Найти свойство.
             \en Find property. \~
    \details \ru Найти свойство по имени и типу. \n
             \en Find property by name and type. \n \~
    \param[in] p    - \ru Номер подсказки.
                      \en Number of hint string. \~
    \param[in] type - \ru Тип свойства.
                      \en Property type. \~
  */
  MbProperty *  FindByPrompt( MbePrompt p, uint type ) const; 
  /** \brief \ru Найти индекс свойства в массиве.
             \en Find index of property in array. \~
    \details \ru Найти индекс свойства в массиве по имени и типу. \n
             \en Find index of property in array by name and type. \n \~
    \param[in] type - \ru Тип свойства.
                      \en Property type. \~
    \param[in] p    - \ru Номер подсказки.
                      \en Number of hint string. \~
  */
  size_t        FindByPrompt( uint type, MbePrompt p ) const; 

OBVIOUS_PRIVATE_COPY( MbProperties )
}; // MbProperties

//----------------------------------------------------------------------------------------
// \ru Выдать свойства неатомарного объекта объекта. \en Get properties of the non-atomic object.
//---
template<class Type>
void MathItemProperty<Type>::GetProperties( MbProperties & props )
{
  if ( value )
    value->GetProperties( props );
}

//----------------------------------------------------------------------------------------
// \ru Задать свойства неатомарного объекта объекта. \en Set properties of the non-atomic object.
//---
template<class Type>
void MathItemProperty<Type>::SetProperties( const MbProperties & props )
{
  if ( value )
    value->SetProperties( props );
}

//----------------------------------------------------------------------------------------
// \ru Выдать свойства неатомарного объекта объекта. \en Get properties of the non-atomic object.
//---
template<class Type>
void MathItemCopyProperty<Type>::GetProperties( MbProperties & props )
{
  value.GetProperties( props );
}

//----------------------------------------------------------------------------------------
// \ru Выдать свойства неатомарного объекта объекта. \en Get properties of the non-atomic object.
//---
template<class Type>
void ConstRefItemProperty<Type>::GetProperties( MbProperties & props ) 
{
  if ( value )
    value->GetProperties( props );
}

//----------------------------------------------------------------------------------------
// \ru Выдать свойства неатомарного объекта объекта. \en Get properties of the non-atomic object.
//---
template<class Type>
void RefItemProperty<Type>::SetProperties( const MbProperties & props ) 
{
  if ( this->value )
    this->value->SetProperties( props );
}


//------------------------------------------------------------------------------
/** \brief  \ru Базовый класс для свойств полигонального объекта с топологией.
            \en The base class for polymesh properties. \~
  \details  \ru Обертка, реализующая свойство объекта с настройкой владения им.\n
            \en Wrapper that implements property of an object with its ownership setting.\n \~
  \ingroup Model_Properties
*/
// ---
class MATH_CLASS MbPolymeshPropertyBase : public MbProperty {
protected:
  MbPolymesh &     _topo;   ///< \ru Топология.  \en Topology. 
  PrePropType      _type;   ///< \ru Тип.  \en Type. 
  uint32           _number; ///< \ru Номер.  \en Number. 

public:
  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \details \ru Конструктор по параметрам. \n
             \en Constructor by parameters. \n \~
    \param[in] name      - \ru Номер подсказки.
                           \en Number of hint string. \~
    \param[in] topo - \ru Топология.
                      \en Topology. \~
    \param[in] type    - \ru Тип.
                         \en Type. \~
    \param[in] n         - \ru Номер.
                           \en Number. \~
  */
  MbPolymeshPropertyBase( MbePrompt name, MbPolymesh & topo, PrePropType type, uint32 n );
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbPolymeshPropertyBase() {}

public:
  // \ru Выдать тип свойства. \en Get type of property. 
  PrePropType IsA() const override { return _type; }
  // \ru Выдать строковое значение свойства. \en Get string value of the property. 
  void        GetCharValue( TCHAR * v ) const override;
  // \ru Задать свойства неатомарного объекта. \en Set properties of the non-atomic object.
  void        SetProperties( const MbProperties & ) override {}
  // \ru Установить новое значение свойства. \en Set the new value of the property. 
  void        SetPropertyValue( TCHAR * ) override {}
  // \ru Получить топологию. \en Get topology. 
  const MbPolymesh & GetTopo() const { return _topo; }
  // \ru Получить номер. \en Get number. 
  uint32 GetNumber() const { return _number; }

  OBVIOUS_PRIVATE_COPY( MbPolymeshPropertyBase )
};


//------------------------------------------------------------------------------
/** \brief  \ru Свойство региона.
            \en The property of the region. \~
  \details  \ru Обертка, реализующая свойство объекта с настройкой владения им.\n
            \en Wrapper that implements property of an object with its ownership setting.\n \~
  \ingroup Model_Properties
*/
// ---
class MATH_CLASS MbPolymeshRegionProperty : public MbPolymeshPropertyBase {
public:
  uint32 _facetCount;     ///< \ru Количество фасетов.  \en Facets count. 
  uint32 _subCount;       ///< \ru Количество подрегионов.  \en Subregions count. 

public:
  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \details \ru Конструктор по параметрам. \n
             \en Constructor by parameters. \n \~
    \param[in] name      - \ru Номер подсказки.
                           \en Number of hint string. \~
    \param[in] topo - \ru Топология.
                      \en Topology. \~
    \param[in] n         - \ru Номер.
                           \en Number. \~
  */
  MbPolymeshRegionProperty( MbePrompt name, MbPolymesh & topo, uint32 n );
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbPolymeshRegionProperty() {}

public:
  // \ru Выдать свойства неатомарного объекта. \en Get properties of the non-atomic object.
  void        GetProperties( MbProperties & ) override;
  // \ru Выдать значение свойства. \en Get value of the property. 
  void       _GetPropertyValue( void * v, size_t /*size*/ ) const override;

  OBVIOUS_PRIVATE_COPY( MbPolymeshRegionProperty )
};


//------------------------------------------------------------------------------
/** \brief  \ru Свойство подрегиона.
            \en The property of the subregion. \~
  \details  \ru Обертка, реализующая свойство объекта с настройкой владения им.\n
            \en Wrapper that implements property of an object with its ownership setting.\n \~
  \ingroup Model_Properties
*/
// ---
class MATH_CLASS MbPolymeshSubRegionProperty : public MbPolymeshPropertyBase {
public:
  uint32 _facetCount;     ///< \ru Количество фасетов.  \en Facets count. 
  uint32 _loopCount;      ///< \ru Количество циклов.  \en Loops count. 

public:
  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \details \ru Конструктор по параметрам. \n
             \en Constructor by parameters. \n \~
    \param[in] name      - \ru Номер подсказки.
                           \en Number of hint string. \~
    \param[in] topo - \ru Топология.
                      \en Topology. \~
    \param[in] n         - \ru Номер.
                           \en Number. \~
  */
  MbPolymeshSubRegionProperty( MbePrompt name, MbPolymesh & topo, uint32 n );
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbPolymeshSubRegionProperty() {}

public:
  // \ru Выдать свойства неатомарного объекта. \en Get properties of the non-atomic object.
  void        GetProperties( MbProperties & ) override;
  // \ru Выдать значение свойства. \en Get value of the property. 
  void       _GetPropertyValue( void * v, size_t /*size*/ ) const override;

  OBVIOUS_PRIVATE_COPY( MbPolymeshSubRegionProperty )
};


//------------------------------------------------------------------------------
/** \brief  \ru Свойство цикла.
            \en The property of the loop. \~
  \details  \ru Обертка, реализующая свойство объекта с настройкой владения им.\n
            \en Wrapper that implements property of an object with its ownership setting.\n \~
  \ingroup Model_Properties
*/
// ---
class MATH_CLASS MbPolymeshLoopProperty : public MbPolymeshPropertyBase {
public:
  uint32 _edgeCount;      ///< \ru Количество ребер.  \en Edges count. 

public:
  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \details \ru Конструктор по параметрам. \n
             \en Constructor by parameters. \n \~
    \param[in] name      - \ru Номер подсказки.
                           \en Number of hint string. \~
    \param[in] topo - \ru Топология.
                      \en Topology. \~
    \param[in] n         - \ru Номер.
                           \en Number. \~
  */
  MbPolymeshLoopProperty( MbePrompt name, MbPolymesh & topo, uint32 n );
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbPolymeshLoopProperty() {}

public:
  // \ru Выдать свойства неатомарного объекта. \en Get properties of the non-atomic object.
  void        GetProperties( MbProperties & ) override;
  // \ru Выдать значение свойства. \en Get value of the property. 
  void       _GetPropertyValue( void * v, size_t /*size*/ ) const override;

  OBVIOUS_PRIVATE_COPY( MbPolymeshLoopProperty )
};


//------------------------------------------------------------------------------
/** \brief  \ru Свойство ребра.
            \en The property of the edge. \~
  \details  \ru Обертка, реализующая свойство объекта с настройкой владения им.\n
            \en Wrapper that implements property of an object with its ownership setting.\n \~
  \ingroup Model_Properties
*/
// ---
class MATH_CLASS MbPolymeshEdgeProperty : public MbPolymeshPropertyBase {
public:
  uint32 _edgeCount;      ///< \ru Количество ребер.  \en Edges count. 

public:
  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \details \ru Конструктор по параметрам. \n
             \en Constructor by parameters. \n \~
    \param[in] name      - \ru Номер подсказки.
                           \en Number of hint string. \~
    \param[in] topo - \ru Топология.
                      \en Topology. \~
    \param[in] n         - \ru Номер.
                           \en Number. \~
  */
  MbPolymeshEdgeProperty( MbePrompt name, MbPolymesh & topo, uint32 n );
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbPolymeshEdgeProperty() {}

public:
  // \ru Выдать строковое значение свойства. \en Get string value of the property. 
  void        GetCharValue( TCHAR * v ) const override;
  // \ru Выдать свойства неатомарного объекта. \en Get properties of the non-atomic object.
  void        GetProperties( MbProperties & ) override;
  // \ru Выдать значение свойства. \en Get value of the property. 
  void       _GetPropertyValue( void * v, size_t /*size*/ ) const override;

  OBVIOUS_PRIVATE_COPY( MbPolymeshEdgeProperty )
};


//------------------------------------------------------------------------------
/** \brief  \ru Свойство вершины.
            \en The property of the vertex. \~
  \details  \ru Обертка, реализующая свойство объекта с настройкой владения им.\n
            \en Wrapper that implements property of an object with its ownership setting.\n \~
  \ingroup Model_Properties
*/
// ---
class MATH_CLASS MbPolymeshVertexProperty : public MbPolymeshPropertyBase {
public:
  uint32 _edge;     ///< \ru Ребро.  \en Edge. 

public:
  /** \brief \ru Конструктор по параметрам.
             \en Constructor by parameters. \~
    \details \ru Конструктор по параметрам. \n
             \en Constructor by parameters. \n \~
    \param[in] name      - \ru Номер подсказки.
                           \en Number of hint string. \~
    \param[in] topo - \ru Топология.
                      \en Topology. \~
    \param[in] n         - \ru Номер.
                           \en Number. \~
  */
  MbPolymeshVertexProperty( MbePrompt name, MbPolymesh & topo, uint32 n );
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbPolymeshVertexProperty() {}

public:
  // \ru Выдать свойства неатомарного объекта. \en Get properties of the non-atomic object.
  void        GetProperties( MbProperties & ) override;
  // \ru Выдать значение свойства. \en Get value of the property. 
  void       _GetPropertyValue( void * v, size_t /*size*/ ) const override;

  OBVIOUS_PRIVATE_COPY( MbPolymeshVertexProperty )
};
#endif // __PROPERTY_H
