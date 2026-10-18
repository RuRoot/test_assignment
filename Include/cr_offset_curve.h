////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Строитель эквидистантной кривой.
         \en Offset curve constructor.
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_OFFSET_CURVE_H
#define __CR_OFFSET_CURVE_H


#include <creator.h>
#include <templ_sptr.h>
#include <op_curve_parameter.h>
#include <vector>


class   MbCurve3D;


//------------------------------------------------------------------------------
/** \brief \ru Строитель эквидистантной кривой.
           \en Offset curve constructor. \~
  \details \ru Строитель эквидистантной кривой.\n
           \en Offset curve constructor.\n \~
  \ingroup Model_Creators
*/
// ---
// MATH_CLASS DEPRECATE_DECLARE_REPLACE ( MbOffsetWireFrameCreator with MbOffsetWireFrameParams )
class MATH_CLASS MbOffsetCurveCreator : public MbCreator {
private:
  // \ru Основные параметры \en The basic parameters 
  c3d::SpaceCurveSPtr     curve;           ///< \ru Исходная кривая. \en The initial curve. 
  MbVector3D              dir;             ///< \ru Направление смещения. \en The offset direction. 
  double                  dist;            ///< \ru Величина смещения. \en The offset distance. 
  bool                    fromBeg;         ///< \ru Вектор смещения привязан к началу кривой (иначе к концу). \en The translation vector is associated with the beginning (with the end otherwise). 

  // \ru Дополнительные параметры (эквидистанта в пространстве) \en Auxiliary parameters (spatial offset) 
  bool                    useFillet;       ///< \ru Заполнять ли разрывы скруглениями (иначе продлять сегменты). \en Whether to fill the gaps with fillets (extend segments otherwise). 
  bool                    keepRadius;      ///< \ru Сохранять ли радиусы в скруглениях. \en Whether to keep the radii at fillets. 
  bool                    bluntAngle;      ///< \ru Притуплять острые углы стыков сегментов \en Whether to blunt the sharp edges of segments joints. 
  bool                    bySurfaceNormal; ///< \ru Эквидистанта согласована с нормалью к поверхности. \en Offset point is moved according to surface normal. 
  c3d::SurfaceSPtr        surface;         ///< \ru Поверхность кривой или подобная ей. \en Curve's surface or similar to such surface. 

  // \ru Дополнительные параметры (эквидистанта на поверхности грани оболочки) \en Auxiliary parameters (offset on the shell face surface) 
  c3d::CreatorsSPtrVector shellCreators;   ///< \ru Журнал построения оболочки. \en The shell history tree. 

protected:
  /// \ru Конструктор копирования \en Copy-constructor 
  MbOffsetCurveCreator( const MbOffsetCurveCreator &, MbRegDuplicate * iReg );
private:
  MbOffsetCurveCreator( const MbOffsetCurveCreator & ); // \ru Не реализовано \en Not implemented 
  MbOffsetCurveCreator(); // \ru Не реализовано \en Not implemented 

public:
  /** \brief \ru Конструктор эквидистанты в пространстве.
             \en Constructor of offset in the space. \~    
    \details \ru Конструктор эквидистанты в пространстве. \n
             \en Constructor of offset in the space. \n \~
    \param[in] curve   - \ru Базовая кривая. 
                         \en The base curve. \~
    \param[in] params  - \ru Параметры.
                         \en Parameters. \~
    \param[in] snMaker - \ru Именователь с версия исполнения.
                         \en Names makers with a version. \~
  */
  MbOffsetCurveCreator( const MbCurve3D &                  curve, 
                        const MbSpatialOffsetCurveParams & params,
                        const MbSNameMaker &               snMaker );
  /** \brief \ru Конструктор эквидистанты на поверхности грани оболочки.
             \en Constructor of offset on the shell face surface. \~    
    \details \ru Конструктор эквидистанты на поверхности грани оболочки. \n
             \en Constructor of offset on the shell face surface. \n \~
    \param[in] curve         - \ru Базовая кривая. 
                               \en The base curve. \~
    \param[in] params        - \ru Параметры.
                               \en Parameters. \~
    \param[in] shellCreators - \ru Построители тела.
                               \en Creators of a solid. \~
    \param[in] sameCreators  - \ru Признак использования оригиналов построителей.
                               \en Flag of using the original creators. \~
    \param[in] snMaker - \ru Именователь с версия исполнения.
                         \en Names makers with a version. \~
  */
  MbOffsetCurveCreator( const MbCurve3D &                  curve, 
                        const MbSurfaceOffsetCurveParams & params,
                        const c3d::CreatorsSPtrVector &    shellCreators, 
                              bool                         sameCreators,
                        const MbSNameMaker &               snMaker );
public :
  virtual ~MbOffsetCurveCreator();

  // \ru Общие функции строителя. \en The common functions of the creator. 
  MbeCreatorType  IsA()  const override; // \ru Тип элемента \en A type of element 
  MbCreator & Duplicate( MbRegDuplicate * iReg = nullptr ) const override; // \ru Сделать копию \en Create a copy
  
  bool        IsSame   ( const MbCreator &, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        IsSimilar( const MbCreator & ) const override;            // \ru Являются ли объекты подобными \en Whether the objects are similar 
  bool        SetEqual ( const MbCreator & ) override;                  // \ru Сделать равным \en Make equal 

  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override; // \ru Сдвиг \en Translation
  void        Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate about an axis

  MbePrompt   GetPropertyName() override; // \ru Дать имя свойства объекта \en Get the object property name  
  void        GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of the object 
  void        GetBasisItems ( RPArray<MbSpaceItem> & ) override;  // \ru Дать базовые объекты \en Get the basis objects 
  void        GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.
  size_t      GetCreatorsCount( MbeCreatorType ct ) const override; // \ru Посчитать внутренние построители по типу. \en Count internal creators by type.
  bool        GetInternalCreators( MbeCreatorType, c3d::ConstCreatorsSPtrVector & ) const override; // \ru Получить внутренние построители по типу. \en Get internal creators by type.
  bool        SetInternalCreators( MbeCreatorType, c3d::CreatorsSPtrVector & ) override;            // \ru Получить внутренние построители по типу. \en Get internal creators by type.

  // \ru Построить кривую по журналу построения \en Create a curve from the history tree 
  bool        CreateWireFrame( MbWireFrame *&, MbeCopyMode, RPArray<MbSpaceItem> * items = nullptr ) override;

  /** \} */

private:
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation, to prevent an assignment by default. 
  void        operator = ( const MbOffsetCurveCreator & ); // \ru Не реализовано!!! \en Not implemented!!! 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbOffsetCurveCreator )
};

IMPL_PERSISTENT_OPS( MbOffsetCurveCreator )


//------------------------------------------------------------------------------
/** \brief \ru Создать офсетную кривую по трехмерной кривой и вектору направления.
           \en Create an offset curve from three-dimensional curve and direction. \~
  \deprecated \ru Функция устарела и будет удалена в версии 2026.
              \en The function is deprecated and will be removed in version 2026. \~
  \details \ru Создать офсетную кривую в пространстве по трехмерной кривой и вектору направления. \n
           \en Create an offset curve in space from three-dimensional curve and direction. \n \~
  \param[in] initCurve - \ru Пространственная кривая, к которой строится эквидистантная.
                         \en A space curve for which to construct the offset curve. \~
  \param[in] params    - \ru Параметры.
                         \en Parameters. \~
  \param[out] resType  - \ru Код результата операции
                         \en Operation result code \~
  \param[out] resCurve - \ru Эквидистантная кривая.
                         \en The offset curve. \~
  \return \ru Возвращает строитель.
          \en Returns the constructor. \~
  \ingroup Curve3D_Modeling
*/
//---
DEPRECATE_DECLARE_REPLACE ( CreateOffsetWireEdges with MbOffsetWireFrameParams )
MATH_FUNC (MbCreator *) CreateOffsetCurve( const MbCurve3D &                  initCurve, 
                                           const MbSpatialOffsetCurveParams & params,
                                                 MbResultType &               resType,
                                                 MbCurve3D *&                 resCurve );


//------------------------------------------------------------------------------
/** \brief \ru Создать офсетную кривую по поверхностной кривой и значению смещения.
           \en Create an offset curve from a spatial curve and offset value. \~
  \deprecated \ru Функция устарела и будет удалена в версии 2026.
              \en The function is deprecated and will be removed in version 2026. \~
  \details \ru Создать офсетную кривую по поверхностной кривой и значению смещения. \n
           \en Create an offset curve from a spatial curve and offset value. \n \~
  \param[in] curve      - \ru Кривая на поверхности грани face.
                          \en A curve on face 'face' surface. \~
  \param[in] params     - \ru Параметры.
                          \en Parameters. \~
  \param[out] resType   - \ru Код результата операции
                          \en Operation result code \~
  \param[out] resCurves - \ru Множество эквидистантных кривых.
                          \en Offset curve array. \~
  \return \ru Возвращает строитель.
          \en Returns the constructor. \~
  \ingroup Curve3D_Modeling
*/
//---
DEPRECATE_DECLARE_REPLACE ( CreateOffsetWireEdges with MbOffsetWireFrameParams )
MATH_FUNC (MbCreator *) CreateOffsetCurve( const MbCurve3D &                  curve, 
                                           const MbSurfaceOffsetCurveParams & params,
                                                 MbResultType &               resType,
                                                 RPArray<MbCurve3D> &         resCurves );


#endif // __CR_OFFSET_CURVE_H
