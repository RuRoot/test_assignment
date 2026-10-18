////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief Вспомогательный интерфейс для полигональной топологии.

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MESH_PROVIDER_H
#define __MESH_PROVIDER_H

#include <mb_cart_point3d.h>

class MbAttributeContainer;
class MbPolymeshTopo;
class MbPolymesh;
class MbMesh;

//------------------------------------------------------------------------------
/** \brief \ru Абстрактный контейнер данных полигонального объекта.
           \en An abstract container for polygonal object data. \~
    \details \ru Абстрактный контейнер данных полигонального объекта.
             \en An abstract container for polygonal object data. \~ 
    \warning \ru В разработке.
             \en Under development. \~
*/
// ---
class MATH_CLASS IMeshProvider
{
protected:
  /// \ru Конструктор. \en Constructor.
  IMeshProvider() = default;

public:
  /// \ru Деструктор. \en Destructor.
  virtual ~IMeshProvider() = default;
  /** \brief \ru Создать экземпляр класса на основе MbMesh.
             \en Create an object by MbMesh. \~
    \param[in] mesh         - \ru Полигональный геометрический объект.
                              \en A polygonal geometric object. \~
    \return \ru Возвращает указатель на созданный объект.
            \en Returns pointer to created object. \~
  */
  static IMeshProvider * Create( const MbMesh & mesh );

  /// \ru Валидны ли данные. \en Returns true if the object is ready to be used.
  virtual bool IsReady() const = 0;
  /// \ru Получить количество точек \en Get the number of points.
  virtual size_t GetPointsCount() const = 0;
  /// \ru Получить количество нормалей \en Get the number of normals.
  virtual size_t GetNormalsCount() const = 0;
  /// \ru Получить точку с индексом index \en Get the point with given index.
  virtual void GetPoint( size_t index, MbCartPoint3D & point ) const = 0;
  /// \ru Получить нормаль с индексом index \en Get the normal with given index.
  virtual void GetNormal( size_t index, MbVector3D & normal ) const = 0;
  /// \ru Получить количество вершин \en Get the number of normals.
  virtual size_t GetVerticesCount() const { return GetPointsCount(); }
  /// \ru Получить количество граней \en Get the number of facets.
  virtual size_t GetFacetsCount() const = 0;
  /// \ru Получить индекс точки для вершины с индексом index \en Get point index for the vertex with the given index.
  virtual size_t GetVertexPointIndex( size_t index ) const = 0;
  /// \ru Получить количество вершин на грани с индексом index \en Get the number of vertices for the face with the given index.
  virtual size_t GetFacetVerticesCount( size_t index ) const = 0;
  /// \ru Получить индекс для вершины с порядковым номером number на грани с индексом index. \en Get the vertex index for the vertex on the facet with the given index with the given position number.
  virtual size_t GetFacetVertexIndex( size_t index, size_t number ) const = 0;
  /// \ru Получить флаг наличия нормалей граней. \en Returns true if facets normals exist.
  virtual bool HasFacetNormals() const { return false; }
  /// \ru Получить индекс нормали для грани с индексом index \en Get normal index for the facet with the given index.
  virtual size_t GetFacetNormalIndex( size_t index ) const { return index; }
  /// \ru Получить флаг наличия нормалей вершин граней \en Returns true if vertices normals exist.
  virtual bool HasFacetVertexNormals() const { return false; }
  /// \ru Получить индекс нормали для вершины с порядковым номером number на грани с индексом index. \en Get the vertex normal index for the vertex on the facet with the given index with the given position number.
  virtual size_t GetFacetVertexNormalIndex( size_t index, size_t number ) const = 0;
  /// \ru Получить количество групп граней \en Get the number of facet groups.
  virtual size_t GetFacetGroupsCount() const { return 1; }
  /// \ru Получить индекс группы для грани с индексом index. \en Get the facet group index for the facet with the given index.
  virtual size_t GetFacetGroup( size_t /*index*/ ) const { return 0; }
  /// \ru Получить флаг наличия атрибутов групп граней. \en Returns true if facets groups attributes exist.
  virtual bool HasFacetGroupAttributes() const { return false; }
  /// \ru Получить атрибуты для группы граней с индексом index \en Get facets group attributes.
  virtual void GetFacetGroupAttributes( size_t /*index*/, std::unique_ptr<MbAttributeContainer> & /*cont*/ ) const {}
  /** \brief \ru Установить атрибуты топологии.
             \en Set mesh topology attributes. \~
    \details \ru Предполагается, что объект polymesh содержит аттрибут маппинга от ребер сетки к порядковому номеру вершины на грани MbHalfedgeAttrSizetEdge
                 и атрибут для пометок перевернутых в процессе алгоритма фасетов MbHalfedgeAttrBoolFacet.
                 Среди всех атрибутов данная функция устанавливает только атрибуты нормалей вида MbHalfedgeAttrNormalsVertex.
                 Для установки других атрибутов следует создать наследника этого класса.
             \en It's assumed that the polymesh object contains MbHalfedgeAttrSizetEdge attribute with edge mapping to the vertex facet position number
                 and MbHalfedgeAttrBoolFacet attribute with bool marks if facets were inverted during topology creation.
                 Among all attributes this function sets up only normals through MbHalfedgeAttrNormalsVertex attribute.
                 To add more attributes an inherited class should be created.
    \param[in] bNormalsAveraging  - \ru Флаг усреднения нормалей по веерам объединенных вершин.
                                    \en A flag for normal vectors averaging around merged vertices fans. \~
    \param[in,out] polymesh - \ru Полигональный геометрический объект.
                              \en A polygonal geometric object. \~
  */
  virtual void SetMeshAttributes( bool bNormalsAveraging, MbPolymeshTopo & polymesh ) const;
  /** \brief \ru Установить атрибуты сегментации.
             \en Set attributes mesh segmentation attributes. \~
    \details \ru Данная функция сохраняет аттрибуты C3D оригинальных групп фасетов.
                 Для установки других атрибутов следует создать наследника этого класса.
             \en This function saves original C3D attributes from original facet groups.
                 To add more attributes an inherited class should be created.
  */
  virtual void SetSegmAttributes( MbPolymeshTopo & polymesh ) const;
  /** \brief \ru Установить атрибуты полигонального объекта.
             \en Set the polygonal object attributes. \~
  */    
  virtual void SetPolymeshAttributes( MbPolymesh & /*polymesh*/ ) const {}

protected:
  /** \brief \ru Установить нормали сетки без усреднения в вершинах.
             \en Set mesh normal vectors without averaging. \~
  */
  void InitFacetVertexNormals( MbPolymeshTopo & polymesh ) const;
  /** \brief \ru Установить нормали сетки с усреднением в вершинах.
             \en Set mesh normal vectors with averaging. \~
  */
  void InitFacetVertexNormalsWithAveraging( MbPolymeshTopo & polymesh ) const;
};
#endif // __MESH_PROVIDER_H