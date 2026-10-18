////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Структура для хранения кривизн, их направлений и нормалей в вершине сетки.
         \en Struct to store curvatures, principal curvature directions and normals at mesh vertex. \~
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __M2B_MESH_CURVATURE_H
#define __M2B_MESH_CURVATURE_H

#include <math_define.h>
#include <mb_vector3d.h>


//------------------------------------------------------------------------------
/** \brief \ru Данные о кривизне и главных направлениях изменений кривизны.
           \en Curvature and principal curvature direction data. \~
  \details \ru Структура для хранения информации о кривизне и главных направлениях изменений кривизны поверхности,
               рассчитанной в вершине полигональной сетки.
           \en Structure for store curvature and principal curvature direction data calculated at the polygon vertex. \~
  \ingroup Polygonal_Objects
*/
// ---
struct MATH_CLASS MbCurvature
{
  double k_h;             ///< \ru Средняя кривизна. \en Mean curvature.
  double k_g;             ///< \ru Гауссова кривизна. \en Gaussian curvature.
  double k1;              ///< \ru Максимальная кривизна. \en Maximum principal curvature.
  double k2;              ///< \ru Минимальная кривизна. \en Minimum principal curvature.
  MbVector3D normal;      ///< \ru Нормаль (вычислена по оператору кривизны). \en Normal (calculated by curvature operator).
  MbVector3D meanNormal;  ///< \ru Нормаль (вычислена как взвешенное среднее нормалей соседних граней). \en Normal (calculated as weighted mean of the normals of neighboring faces).
  MbVector3D cdir1;       ///< \ru Направление максимальной кривизны. \en Maximum principal curvature direction.
  MbVector3D cdir2;       ///< \ru Направление минимальной кривизны. \en Minimum principal curvature direction.

  /// \ru Конструктор по умолчанию. \en Default constructor. 
  MbCurvature() : k_h( 0.0 ), k_g( 0.0 ), k1 ( 0.0 ), k2 ( 0.0 ) 
  {
    cdir1.Init( 0., 1., 0. );
    cdir2.Init( 0., 0., 1. );
    normal.Init( 1., 0., 0. );
  }
  /// \ru Конструктор копирования. \en Copy constructor.
  MbCurvature( const MbCurvature & that )
    : k_h       ( that.k_h        )
    , k_g       ( that.k_g        )
    , k1        ( that.k1         )
    , k2        ( that.k2         )
    , normal    ( that.normal     )
    , meanNormal( that.meanNormal )
    , cdir1     ( that.cdir1      )
    , cdir2     ( that.cdir2      )
  {}

  /// \ru Оператор присваивания. \en An assignment operator.
  MbCurvature & operator = ( const MbCurvature & that )
  {
    k_h = that.k_h;
    k_g = that.k_g;
    k1 = that.k1;
    k2 = that.k2;
    normal = that.normal;
    meanNormal = that.meanNormal;
    cdir1 = that.cdir1;
    cdir2 = that.cdir2;
    return ( *this );
  }
  /// \ru Функция копирования данных. \en Copy function of data. 
  void Init( const MbCurvature & that )
  {
    k_h = that.k_h;
    k_g = that.k_g;
    k1 = that.k1;
    k2 = that.k2;
    normal = that.normal;
    meanNormal = that.meanNormal;
    cdir1 = that.cdir1;
    cdir2 = that.cdir2;
  }
  /// \ru Очистка данных. \en Clear data. 
  void Clear()
  {
    k_h = 0.;
    k_g = 0.;
    k1 = 0.;
    k2 = 0.;
    normal = MbVector3D::zero;
    meanNormal = MbVector3D::zero;
    cdir1 = MbVector3D::zero;
    cdir2 = MbVector3D::zero;
  }
  /// \ru Определена ли кривизна. \en Is curvature defined. 
  bool IsDefined() const { return meanNormal.Length2() > PARAM_EPSILON; }
  /// \ru Получить индекс кривизны. \en Get curvature index. 
  double CurvatureIndex() const
  {
    const double delta = k1 - k2;
    if ( ::fabs( delta ) < PARAM_EPSILON )
      return 1.;

    return ::atan( ( k1 + k2 ) / delta ) / M_PI_2;
  }
  /// \ru Получить изогнутость. \en Get curvedness. 
  double Curvedness() const
  {
    return ::sqrt( ( k1 * k1 + k2 * k2 ) * c3d::ONE_HALF );
  }
  /// \ru Преобразовать согласно матрице.
  void Transform( const MbMatrix3D & matr )
  {
    normal.Transform( matr );
    meanNormal.Transform( matr );
    cdir1.Transform( matr );
    cdir2.Transform( matr );
  }
};


//------------------------------------------------------------------------------
/** \brief \ru Тип кривизны.
           \en Curvature type. \~
  \warning \ru В разработке.
           \en Under development. \~
*/
// ---
enum class MbeCurvatureType
{
  mean,       ///< \ru Средняя кривизна. \en Mean curvature.
  gaussian,   ///< \ru Гауссова кривизна. \en Gaussian curvature.
  maximum,    ///< \ru Максимальная кривизна. \en Maximum principal curvature.
  minimum,    ///< \ru Минимальная кривизна. \en Minimum principal curvature.
  index,      ///< \ru Индекс кривизны. \en Curvature indexr.
  curvedness, ///< \ru Изогнутость. \en Curvednessr.
};


/// \ru Получить значение кривизны заданного типа. \en Get the curvature value of a given type. 
inline double GetCurvatureValue( MbeCurvatureType type, const MbCurvature & curv )
{
  double value = 0.;
  switch ( type )
  {
    case MbeCurvatureType::mean:
      value = curv.k_h;
      break;
    case MbeCurvatureType::gaussian:
      value = curv.k_g;
      break;
    case MbeCurvatureType::maximum:
      value = curv.k1;
      break;
    case MbeCurvatureType::minimum:
      value = curv.k2;
      break;
    case MbeCurvatureType::index:
      value = curv.CurvatureIndex();
      break;
    case MbeCurvatureType::curvedness:
      value = curv.Curvedness();
      break;
    default:
      break;
  }
  return value;
}

#endif // __M2B_MESH_CURVATURE_H
