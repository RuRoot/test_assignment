////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Строитель сплайна на поверхности по точками.
         \en Constructor of spline on a surface by points.
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_SURFACE_SPLINE_H
#define __CR_SURFACE_SPLINE_H


#include <creator.h>
#include <mb_nurbs_function.h>
#include <mb_point_mating.h>
#include <templ_s_array.h>
#include <templ_rp_array.h>


class   MbCartPoint;
class   MbSurface;


//------------------------------------------------------------------------------
/** \brief \ru Строитель пространственного сплайна.
           \en Spatial spline constructor. \~
  \details \ru Строитель пространственного сплайна.\n
           \en Spatial spline constructor.\n \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbSurfaceSplineCreator : public MbCreator {
private:
  c3d::SurfaceSPtr               surface;          ///< \ru Поверхность. \en Surface. 
  bool                           throughPnts;      ///< \ru через точки. \en Through points. 
  SArray<MbCartPoint>            paramPnts;        ///< \ru Параметрические точки. \en Parametric points. 
  SArray<double>                 paramWts;         ///< \ru Веса параметрических точек. \en Parametric points weights. 
  bool                           paramClosed;      ///< \ru Замкнуть параметрический сплайн. \en Make the parametric spline close. 
  RPArray<c3d::PntMatingData3D>  spaceTransitions; ///< \ru Сопряжения в точках. \en Tangents at the points. 

protected:
  /// \ru Конструктор копирования. \en Copy-constructor.
  MbSurfaceSplineCreator( const MbSurfaceSplineCreator &, MbRegDuplicate * iReg );
private:
  MbSurfaceSplineCreator( const MbSurfaceSplineCreator & ); // \ru Не реализовано \en Not implemented 
  MbSurfaceSplineCreator(); // \ru Не реализовано \en Not implemented 

public:
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  MbSurfaceSplineCreator( const MbSurface &                     surface,
                                bool                            sameSurf, 
                                bool                            thrPnts,
                          const SArray<MbCartPoint> &           pnts,
                          const SArray<double> &                wts, 
                                bool                            parCls,
                          const RPArray<c3d::PntMatingData3D> & transitions,
                          const MbSNameMaker &                  snMaker ); 
public :
  virtual ~MbSurfaceSplineCreator();

  // \ru Общие функции строителя. \en The common functions of the creator. 
  MbeCreatorType  IsA() const override; // \ru Тип элемента \en A type of element 
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

  // \ru Построить кривую по журналу построения \en Create a curve from the history tree 
  bool        CreateWireFrame( MbWireFrame *&, MbeCopyMode, RPArray<MbSpaceItem> * items = nullptr ) override;

  /** \} */

private:
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation, to prevent an assignment by default. 
  void        operator = ( const MbSurfaceSplineCreator & ); // \ru Не реализовано!!! \en Not implemented!!! 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbSurfaceSplineCreator )
};

IMPL_PERSISTENT_OPS( MbSurfaceSplineCreator )


//------------------------------------------------------------------------------
/** \brief \ru Создать кривую на поверхности.
           \en Create a curve on a surface. \~
  \details \ru Создать кривую на поверхности. \n
    Примечания: \n
    1. Если есть сопряжения, то количество сопряжений д.б. равно количеству точек. \n
       Отсутствующие сопряжения должны быть представлены нулевыми указателями в массиве \n
    2. Если сплайн строится через точки, то сопряжения могуть быть заданы произвольно. \n
    2. Если сплайн строится по полюсам и он незамкнут, то сопряжения могут быть только на концах. \n
    3. Если сплайн строится по полюсам и он замкнут, то сопряжения должны отсутствовать. \n
    4. Множество весов д.б. пуст или синхронизирован с массивом точек по количеству (с опцией throughPoints веса игнорируются). \n
           \en Create a curve on a surface. \n
    Notes: \n
    1. If the tangents are specified, then the number of tangents should be equal to the number of points. \n
       Missing tangents should be represented by null pointers in the array \n
    2. If the spline is created from points, arbitrary tangents can be defined. \n
    2. If the spline is created from poles and it is open, only the end tangents can be specified. \n
    3. If the spline is constructed from poles and it is closed, the tangents cannot be specified. \n
    4. The weight array should be empty or synchronized with the point array by size (with option throughPoints the weights are ignored). \n \~
  \param[in] surface        - \ru Поверхность.
                              \en The surface. \~
  \param[in] throughPoints  - \ru Провести сплайн через точки.
                              \en Create a spline through points. \~
  \param[in] paramPnts      - \ru Множество параметрических точкек.
                              \en Parametric point array. \~
  \param[in] paramWts       - \ru Множество весов параметрических точек.
                              \en An array of parametric point weights. \~
  \param[in] paramClosed    - \ru Строить замкнутый параметрический сплайн.
                              \en Create a closed parametric spline. \~
  \param[in] spaceTransitions - \ru Сопряжения в точках.
                                \en Tangents at the points. \~
  \param[in] snMaker        - \ru Именователь кривых каркаса.
                              \en An object defining the frame curves names. \~
  \param[out] resType - \ru Код результата операции
                        \en Operation result code \~
  \param[out] resCurves - \ru Множество эквидистантных кривых.
                          \en Offset curve array. \~
  \return \ru Возвращает строитель.
          \en Returns the constructor. \~
  \ingroup Curve3D_Modeling
*/
//---
MATH_FUNC (MbCreator *) CreateSurfaceSpline( const MbSurface &                     surface,
                                                   bool                            throughPoints,
                                                   SArray<MbCartPoint> &           paramPnts,
                                                   SArray<double> &                paramWts,
                                                   bool                            paramClosed,
                                                   RPArray<c3d::PntMatingData3D> & spaceTransitions,
                                             const MbSNameMaker &                  snMaker,
                                                   MbResultType &                  resType,
                                                   RPArray<MbCurve3D> &            resCurves );


#endif // __CR_SURFACE_SPLINE_H
