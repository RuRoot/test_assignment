////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Атрибуты для полигональной сетки в представлении half edge 
             для использования в сегментации.
         \en Polygonal mesh segmentation attributes.

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MESH_HALFEDGE_SEGM_ATTR_H
#define __MESH_HALFEDGE_SEGM_ATTR_H

#include <mesh_halfedge_attr_base.h>
#include <surface.h>

//------------------------------------------------------------------------------
/** \brief \ru Атрибут для поверхностей регионов сегментации.
           \en The surface segmentation region attribute.
  \ingroup Data_Structures
*/
// ---
class MATH_CLASS MbHalfedgeAttrSurfaces : public MbHalfedgeAttrBase
{
private:
  std::vector<SPtr<MbSurface>> _surfaces; //< \ru Массив поверхностей. \en The surfaces array.

public:
  /// \ru Конструктор. \en Constructor.
  MbHalfedgeAttrSurfaces( const MbHalfedge & topo ) : MbHalfedgeAttrBase( topo, MbeTypeAttr::ta_SegmentationSurface ) {}
  /// \ru Деструктор. \en Destructor.
  virtual ~MbHalfedgeAttrSurfaces() {}

  /// \ru Тип атрибута. \en Attribute type.
  static MbeTypeAttr         IsA() { return MbeTypeAttr::ta_SegmentationSurface; }
  /// \ru Зарезервировать память под массив вершин, ребер и фасетов. \en Reserve memory to store attribute's data.
  void Reserve( size_t /*nVx*/, size_t /*nEd*/, size_t nF ) override { _surfaces.reserve( nF ); }
  /// \ru Добавить фасет с дефолтным значением атрибута. \en Add facet with the default attribute.
  void AddFacet() override { _surfaces.push_back( SPtr<MbSurface>() ); }
  /// \ru Установить размер. \en Set size of the surfaces array.
  void SetSize( size_t  size ) { _surfaces.assign( size, SPtr<MbSurface>() ); }
  /// \ru Получить поверхность. \en Get surfaces for the facet.
  const SPtr<MbSurface> & GetSurface( size_t iF ) const { return _surfaces[iF]; }
  /// \ru Получить поверхность. \en Get surface for the facet.
  SPtr<MbSurface> & GetSurface( size_t iF ) { return _surfaces[iF]; }
  /// \ru Преобразовать согласно матрице. \en Transform by given matrix.
  void Transform( const MbMatrix3D & matr ) override;
  /// \ru Получить все поверхности. \en Get all surfaces.
  std::vector<SPtr<MbSurface>> & GetSurfaces() { return _surfaces; }
  /// \ru Получить все поверхности. \en Get all surfaces.
  const std::vector<SPtr<MbSurface>> & GetSurfaces() const { return _surfaces; }

  HALFEDGE_VISITING();
  DECLARE_PERSISTENT_CLASS( MbHalfedgeAttrSurfaces );
};
#endif // __MESH_HALFEDGE_SEGM_ATTR_H