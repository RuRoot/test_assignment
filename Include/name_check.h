////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Работа с именами топологических объектов.
         \en The treatment of names of topological objects. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __NAME_CHECK_H
#define __NAME_CHECK_H


#include <templ_s_array.h>
#include <templ_rp_array.h>
#include <mb_cart_point3d.h>
#include <name_item.h>
#include <topology.h>


class  MbFaceShell;
class  MbItem;


//------------------------------------------------------------------------------
/** \brief \ru Сравнение точек в пространстве.
           \en Points comparison in space. \~
  \details \ru Сравнение точек в пространстве с точностью Math::region: 
               по первой координате, по второй координате, по третьей координате.
           \en Points comparison with precision Math::region: 
               in the first coordinate, in the second coordinate, in the third coordinate. \~
  \param[in] p1 - \ru Первая точка.
                  \en An first point. \~
  \param[in] p2 - \ru Множество граней.
                  \en Вторая точка. \~
  \param[in] precision - \ru Точность сравнения.
                         \en The precision of comparison. \~
  \return \ru Возвращает: -1, если p1 < p2; +1, если p1 > p2; 0, если p1 == p2.
          \en Returns: -1 -if p1 < p2; +1 -if p1 > p2; 0 -if p1 == p2. \~
  \ingroup Names
*/
// ---
inline
int PointCompare3D ( const MbCartPoint3D & p1, const MbCartPoint3D & p2, double precision = Math::region )
{
  if ( p2.x + precision < p1.x ) // по X
    return 1;
  else if ( p1.x + precision < p2.x )
    return -1;
  else {
    if ( p2.y + precision < p1.y ) // по Y
      return 1;
    else if ( p1.y + precision < p2.y )
      return -1;
    else {
      if ( p2.z + precision < p1.z ) // по Z
        return 1;
      else if ( p1.z + precision < p2.z )
        return -1;
    }
  }

  return 0;
}


//------------------------------------------------------------------------------
/** \brief \ru Установить имена элементам оболочки.
           \en Set names for elements of shell. \~
  \details \ru Установить имена элементам оболочки: граням, ребрам, вершинам.
           \en Set names for elements of shell: for faces, edges, vertices. \~
  \param[in] edges - \ru Множество ребер.
                     \en An array of edges. \~
  \param[in] faces - \ru Множество граней.
                     \en An array of faces. \~
  \param[in] nameMaker - \ru Именователь.
                         \en An object for naming the new objects. \~
  \param[in] processVertexes - \ru Устанавливать ли имена вершинам.
                               \en Whether to set names to vertices. \~
  \ingroup Names
*/
// ---
MATH_FUNC (void) SetShellNames( const RPArray<MbCurveEdge> & edges,
                                const RPArray<MbFace> &      faces,
                                const MbSNameMaker &         nameMaker,
                                      bool                   processVertexes );


//------------------------------------------------------------------------------
/** \brief \ru Установить имена элементам оболочки.
           \en Set names for elements of shell. \~
  \details \ru Установить имена граням оболочки.
           \en Set names for faces of shell. \~
  \param[in] faces - \ru Множество граней.
                     \en An array of faces. \~
  \param[in] nameMaker - \ru Именователь.
                         \en An object for naming the new objects. \~
  \param[in] processVertexes - \ru Устанавливать ли имена вершинам.
                               \en Whether to set names to vertices. \~
  \ingroup Names
*/
// ---
MATH_FUNC (void) SetFacesNames( const RPArray<MbFace> & faces,
                                const MbSNameMaker &    nameMaker,
                                      bool              processVertexes );


//------------------------------------------------------------------------------
/** \brief \ru Установить имена элементам оболочки.
           \en Set names for elements of shell. \~
  \details \ru Установить имена граням оболочки.
           \en Set names for faces of shell. \~
  \param[in] faces - \ru Множество граней.
                     \en An array of faces. \~
  \param[in] nameMaker - \ru Именователь.
                         \en An object for naming the new objects. \~
  \param[in] processVertexes - \ru Устанавливать ли имена вершинам.
                               \en Whether to set names to vertices. \~
  \ingroup Names
*/
// ---
MATH_FUNC (void) SetFacesNames( const c3d::FacesVector & faces,
                                const MbSNameMaker &     nameMaker,
                                      bool               processVertexes );


//------------------------------------------------------------------------------
/** \brief \ru Установить имена элементам оболочки.
           \en Set names for elements of shell. \~
  \details \ru Установить имена элементам оболочки: граням, ребрам, вершинам.
           \en Set names for elements of shell: for faces, edges, vertices. \~
  \param[in] shell - \ru Оболочка.
                     \en A shell. \~
  \param[in] nameMaker - \ru Именователь.
                         \en An object for naming the new objects. \~
  \ingroup Names
*/
// ---
MATH_FUNC (void)  SetShellNames(       MbFaceShell &  shell, 
                                 const MbSNameMaker & nameMaker );


//------------------------------------------------------------------------------
/** \brief \ru Установить имена элементам объекта.
           \en Set names for elements of object. \~
  \details \ru Установить имена элементам объекта: граням, ребрам и вершинам тела, ребрам и вершинам проволочного каркаса, вершинам точечного каркаса.
           \en Set names for elements of object: faces, edges and vertices of an solid, edges and vertices of a wireframe, vertices of a point frame. \~
  \param[in] item - \ru Тело, вставка, проволочный каркас, точечный каркас.
                    \en A solid, wire frame, point frame, instance. \~
  \param[in] nameMaker - \ru Именователь.
                         \en An object for naming the new objects. \~
  \param[in] clearNames - \ru Очистить предварительно все имена у элементов объекта.
                          \en Clear all names of object elements beforehand. \~
  \return \ru Возвращает true, если было выполнено именование.
          \en Returns true if naming operation was performed. \~
  \ingroup Names
*/
// ---
MATH_FUNC (bool)  SetItemNames(       MbItem &       item, 
                                const MbSNameMaker & nameMaker,
                                      bool           clearNames );


//------------------------------------------------------------------------------
/** \brief \ru Имя со счётчиком совпадений.
           \en Name with hits counter. \~
  \details \ru Имя со счётчиком совпадений. \n
           \en Name with hits counter. \n \~
  \ingroup Names
*/
// ---
struct NameIntersectionInfo {
  const MbName * name;          ///< \ru Имя объектов. \en A name of objects. 
  size_t         intersections; ///< \ru Количество совпадений. \en The count of coincidences. 

  NameIntersectionInfo() : name( nullptr ), intersections( 0 ) {}
};


//------------------------------------------------------------------------------
/** \brief \ru Проверить имена на совпадение.
           \en Check names for coincidence. \~
  \details \ru Проверить имена составляющих элементов оболочке на совпадение. \n
           \en Check names of shell's components for coincidence. \n \~
  \param[in] shells - \ru Множество проверяемых оболочек.
                      \en An array of checked shells. \~
  \param[out] infos - \ru Множество совпадающих имен со счетчиком совпадений.
                      \en Array of coincident names with hits counter. \~
  \return \ru Возвращает true, если совпадающих имен не найдено.
          \en Returns true if no coincident names found. \~
  \ingroup Names
*/
// ---
MATH_FUNC (bool) CheckShellNames( const RPArray<const MbFaceShell> &   shells, 
                                        SArray<NameIntersectionInfo> & infos );


//-----------------------------------------------------------------------------
/** \brief \ru Выбрать имя объединяемых ребер.
           \en Select name for united edges. \~
  \details \ru Выбрать наиболее подходящее имя при объединении двух ребер, \n
           новое имя будет установлено первому ребру. \n
           \en Select the most suitable name while uniting two edges, \n
           the new name will be set to the first edge. \n \~
  \param[in,out] edge1 - \ru Первое ребро.
                         \en The first edge. \~
  \param[in]     edge2 - \ru Второе ребро.
                         \en The second edge. \~
  \param[in]     snMaker - \ru Именователь с версией исполнения.
                           \en Names maker with a version. \~
  \ingroup Names
*/
//---
MATH_FUNC (void) CombineNames(       MbCurveEdge &  edge1, 
                               const MbCurveEdge &  edge2, 
                               const MbSNameMaker & snMaker );


#endif // __NAME_CHECK_H
