////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Контейнер для ob объектов.
         \en Container for ob objects. \~
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef OB_CONTAINER_H
#define OB_CONTAINER_H

#include "system_cpp_standard.h"
#ifndef C3D_STANDARD_CXX_17
  #error "This header requires support for C++17"
#endif

#include <map>
#include <variant>
#include <math_define.h>
#include <filesystem>
#include <math_define.h>
#include <conv_res_type.h>
#include <solid.h>
#include <conv_exchange_settings.h>
#include "conv_ob_geometry.h"
#include "conv_ob_brep.h"
#include "conv_ob_init.h"

namespace c3d_conv_user
{
  using ObObject = std::variant<ObSegment2D, ObPolyline2D, ObArc2D, ObNURBSCurve2D,
    ObCompositeCurve2D, ObIntersectionCurve, ObCubicSpline2D, ObHermit2D,
    ObOffsetCurve2D, ObSegment3D, ObPolyline3D, ObArc3D, ObParabola3D,
    ObHyperbola3D, ObNURBSCurve3D, ObCompositeCurve3D, ObDataCubicSpline3D,
    ObHermit3D, ObOffsetCurve3D, ObPlane, ObCylinder, ObCone, ObSphere, ObTorus,
    ObExtrusion, ObRevolution, ObRuled, ObNURBSSurface, ObOffsetSurface,
    ObReplicaSurface, ObConeSpiral, ObTrimmedCurve2D, ObFifthDegreeSpline2D,
    ObTrimmedCurve3D, ObFifthDegreeSpline3D, ObReplicaCurve3D, ObShell, ObFace,
    ObLoop, ObCoedge, ObEdge, ObVertex, ObIdArray, ObDoubleArray,
    ObPrimitive2DArray, ObPrimitive3DArray>;

  //------------------------------------------------------------------------------
  /** \brief \ru Контейнер для хранения пользовательских объектов. Используется для последующего создания тела.
             \en Container for storing user objects. Used for subsequent body creation. \~
      \attention \ru Экспериментальный класс.
                \en Experimental class. 
  */
  // ---
  class CONV_CLASS ObContainer {

  protected:
    int rootId;
    std::map< int, ObObject > obObjects;
    
    //------------------------------------------------------------------------------
    /** \brief \ru Конструктор перемещения.
             \en Move constructor. \~
    */
    // ---
    ObContainer(std::shared_ptr<ObContainer const>&&);

    //------------------------------------------------------------------------------
    /** \brief \ru Оператор перемещения.
             \en Move operator. \~
    */
    // ---
    ObContainer& operator=(std::shared_ptr<ObContainer const>&&);

    //------------------------------------------------------------------------------
    /** \brief \ru Конструктор по id и набору объектов.
             \en Constructor by id and set of objects. \~
    */
    // ---
    ObContainer( const int rootId, const std::map< int, ObObject >& obObjects );

  public:

    //------------------------------------------------------------------------------
    /** \brief \ru Конструктор по умолчанию.
             \en Default constructor. \~
    */
    // ---
    ObContainer();
    ObContainer(ObContainer const&) = delete;
    ObContainer& operator= (ObContainer const&) = delete;
    virtual ~ObContainer() = default;

    //------------------------------------------------------------------------------
    /** \brief \ru Добавление нового объекта в контейнер.
               \en Adding a new object to a container. \~
    */
    // ---
    template < class ObObjectT >
    ObObjectT& AddObObject( int id ) {
      ObObjectT object;
      Init( object );
      obObjects.insert( {id, object } );
      return std::get<ObObjectT>( obObjects[id] );
    }

    //------------------------------------------------------------------------------
    /** \brief \ru Задать корневой id.
               \en Set root id. \~
    */
    // ---
    void SetRootId( const int id );

    //------------------------------------------------------------------------------
    /** \brief \ru Готов ли контейнер к использованию.
               \en Is the container ready for use. \~
    */
    // ---
    bool IsReady() const;

    //------------------------------------------------------------------------------
    /** \brief \ru Записать пользовательские объекты в файл формата ".ob".
               \en Write custom objects to an ".ob" file. \~
    */
    // ---
    bool WriteIntoFile( std::filesystem::path const & path ) const;

    //------------------------------------------------------------------------------
    /** \brief \ru Прочитать пользовательские объекты из файла формата ".ob".
               \en Read custom objects from an ".ob" file. \~
    */
    // ---
    static std::shared_ptr<ObContainer> ReadFromFile( std::filesystem::path const & path );
  };

  /** \brief \ru Построить тело по пользовательским объектам.
         \en Read a model document using a set of custom objects. \~
  \param[out] solid -    \ru Импортируемое тело.
                         \en The imported solid. \~
  \param[in] obContainer-\ru Контейнер пользовательских объектов.
                         \en Custom object container. \~
  \param[in] prop -      \ru Реализация интерфейса свойств конвертера.
                         \en Implementation of converter's properties interface. \~
  \param[in] indicator - \ru Индикатор хода процесса.
                         \en The process progress indicator. \~
  \return \ru Код завершения операции.
          \en Code of the operation termination. \~
  \note   \ru ЭКСПЕРИМЕНТАЛЬНАЯ.
          \en EXPEREIMENTAL \~.
  \ingroup Exchange_Interface
  */
 CONV_FUNC (MbeConvResType ) ImportSolid( c3d::SolidSPtr& solid,
                                          std::shared_ptr<ObContainer const> obContainer,
                                          IConvertorProperty3D * prop,
                                          IProgressIndicator* indicator = nullptr );

}

#endif //OB_CONTAINER_H
