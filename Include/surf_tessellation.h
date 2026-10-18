////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Функции вычисления шага движения по поверхности и кривой для методов разбивки поверхности GetTesselation(..) и AddTesselation(..).
         \en Functions for calculation step movement on the surface and the curve for methods of splitting the surface GetTesselation(..) and AddTesselation(..).

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __SURF_TESSELLATION_H
#define __SURF_TESSELLATION_H


#include <templ_s_array.h>
#include <templ_array2.h>
#include <math_define.h>
#include <mb_data.h>
#include <mb_rect1d.h>
#include <curve3d.h>
#include <surface.h>


//------------------------------------------------------------------------------
/** \brief \ru Точка и два нормализованных вектора с их длинами.
           \en Point and two normalized vectors, and their lengths. \~
  \details \ru Точка и два нормализованных вектора с их длинами.
           \en Point and two normalized vectors, and their lengths. \~ 
  \ingroup Data_Structures
*/ // ---
struct MATH_CLASS MbSpacePntTwoVects {
  MbCartPoint3D pnt;
  MbVector3D    tauU;
  MbVector3D    tauV;
  double        lenU;
  double        lenV;

  MbSpacePntTwoVects(); // \ru Конструктор по умолчанию \en Default constructor 
};


//------------------------------------------------------------------------------
/** \brief \ru Разбивка кривой.
           \en Curve tessellation. \~
  \details \ru Разбивка кривой используется для ускорения группового проецирования точек на кривую.
           \en Curve tessellation is used to accelerate the projection of a group of points on the curve. \~ 
  \ingroup Data_Structures
*/ // ---
class MATH_CLASS MbCurveTessellation : public MbRefItem {
private:
  MbStepData                 stepData; ///< \ru Тип шага. \en Step type and Step value. 
  std::vector<double>        params;   ///< \ru Массив параметров. \en The array of parameters.
  std::vector<MbCartPoint3D> pnts;     ///< \ru Массив точек. \en The array of points.

public:
  MbCurveTessellation(); // \ru Конструктор по умолчанию \en Default constructor 
  MbCurveTessellation( const MbCurveTessellation & st ); // \ru Конструктор копирования. \en Copy-constructor. 
  ~MbCurveTessellation() {} // \ru Деструктор. \en Destructor. 

public:
  bool          Init        ( MbStepData sData, const std::vector<double> & initParams, const std::vector<MbCartPoint3D> & initPnts ); // \ru Инициализация параметров. \en Initialization of parameters. 
  bool          IsSameParams( const MbStepData & sData, double eps ) const; // \ru Определить, являются ли объекты одинаковыми. \en Determine whether objects are equal.
  double        GetParam    ( size_t k ) const; // \ru Выдать параметр по индексу. \en Get parameter by index.
  MbCartPoint3D GetPoint    ( size_t k ) const; // \ru Выдать точку по индексу. \en Get point by index.

public:
  MbCurveTessellation & operator = ( const MbCurveTessellation & st );
};


//------------------------------------------------------------------------------
/** \brief \ru Разбивка поверхности.
           \en Surface tessellation. \~
  \details \ru Разбивка поверхности используется для ускорения группового проецирования точек на поверхность.
           \en Surface tessellation is used to accelerate the projection of a group of points on the surface. \~ 
  \ingroup Data_Structures
*/ // ---
class MATH_CLASS MbSurfaceTessellation {
private:
  MbStepData                 stepDataU; ///< \ru Тип шага и величина шага по U. \en Step type and Step value in U-direction. 
  MbStepData                 stepDataV; ///< \ru Тип шага и величина шага по V. \en Step type and Step value in V-direction. 
  MbRect1D                   uRange;    ///< \ru Диапазон по U. \en Range in u direction.
  MbRect1D                   vRange;    ///< \ru Диапазон по V. \en Range in v direction.
  std::vector<double>        uParams;   ///< \ru Массив параметров по U. \en The array of parameters in u direction.
  std::vector<double>        vParams;   ///< \ru Массив параметров по V. \en The array of parameters in u direction.
  Array2<MbSpacePntTwoVects> extPoints; ///< \ru Массив точек и первых производных поверхности. \en An array of points and the first derivatives of the surface.
  bool                       ext;       ///< \ru Флаг, определяющий, выполнять ли вычисления на продолжении поверхности (если true, то искать). \en A flag defining whether to do calculations on the extension of the surface.

public:
  MbSurfaceTessellation(); // \ru Конструктор по умолчанию \en Default constructor 
  MbSurfaceTessellation( const MbSurfaceTessellation & st ); // \ru Конструктор копирования. \en Copy-constructor. 
  ~MbSurfaceTessellation() {} // \ru Деструктор. \en Destructor. 

public:
  bool               IsFilled( double ueps, double veps, bool checkExtPoints ) const; // \ru Проверить, является ли разбивка заполненной. \en Check, whether the tessellation is filled. 
  void               Clear(); // \ru Очистить данные. \en Clear data. 
  bool               Init( MbStepData sdAlongU, MbStepData sdAlongV, bool ext,
                           double u1, double u2, const SArray<double> & uarr,
                           double v1, double v2, const SArray<double> & varr ); // \ru Инициализация данных. \en Data initialization.
  const MbStepData & GetStepAlongU() const { return stepDataU; } // \ru Дать тип шага вдоль параметра u. \en Give the step type along the parameter u.
  const MbStepData & GetStepAlongV() const { return stepDataV; } // \ru Дать тип шага вдоль параметра v. \en Give the step type along the parameter v.

  const MbRect1D &   GetURange () const { return uRange; } // \ru Дать диапазон по u. \en Give range by u.
  size_t             GetUCount () const { return uParams.size(); } // \ru Дать количество параметров по u. \en Give the number of parameters by u. 
  double             GetUParam ( size_t k ) const { return ((k < uParams.size()) ? uParams[k] : UNDEFINED_DBL ); } // \ru Дать u параметр по индексу. \en Give u parameter by index.
  void               GetUParams( SArray<double> & uArr ) const { uArr = uParams; } // \ru Дать массив u параметров. \en Give an array of u parameters.
                    
  const MbRect1D &   GetVRange () const { return vRange; } // \ru Дать диапазон по v. \en Give range by v.
  size_t             GetVCount () const { return vParams.size(); } // \ru Дать количество параметров по v. \en Give the number of parameters by v. 
  double             GetVParam ( size_t k ) const { return ((k < vParams.size()) ? vParams[k] : UNDEFINED_DBL ); } // \ru Дать v параметр по индексу. \en Give v parameter by index.
  void               GetVParams( SArray<double> & vArr ) const { vArr = vParams; } // \ru Дать массив v параметров. \en Give an array of v parameters.
                    
  bool               IsSameParams( const MbStepData & sdAlongU, const MbStepData & sdAlongV, bool ext,
                                   double u1, double u2, double ueps,
                                   double v1, double v2, double veps ) const; // \ru Проверить, являются ли параметры такими же. \en Check whether input parameters are the same.
                    
  bool               SetLinePoint( size_t lineIndex, size_t columnIndex, const MbSpacePntTwoVects & lineItem ); // \ru Установить точку. \en Set point. 
  bool               GetLinePoint( size_t lineIndex, size_t columnIndex, MbSpacePntTwoVects & lineItem ) const; // \ru Получить точку. \en Get point. 
                    
  void               Transform( const MbMatrix3D & matr ); // \ru Преобразовать элемент согласно матрице. \en Transform element according to the matrix. 
  void               Move     ( const MbVector3D & to ); // \ru Сдвинуть точку на вектор. \en Move point by vector. 
  void               Rotate   ( const MbAxis3D & a, double angle ); // \ru Повернуть вокруг оси на заданный угол. \en Rotate around an axis by angle. 

public:
  MbSurfaceTessellation & operator = ( const MbSurfaceTessellation & st ); // \ru Присвоить значение. \en Assign a value. 
};


//------------------------------------------------------------------------------
/** \brief \ru Точки и производные поверхности.
           \en Points and Derivative Surfaces. \~
  \details \ru Точки и производные поверхности.
           \en Points and Derivative Surfaces. \~ 
  \ingroup Data_Structures
*/ // ---
class MATH_CLASS MbSurfaceWorkingData {
private:
  MbCartPoint uv0;                ///< \ru Исходный параметр. \en Initial parameter.
  MbCartPoint uv;                 ///< \ru Модифицированный параметр. \en Modified parameter.
  bool        ext;                ///< \ru Флаг расчета на продолжении. \en Extension flag.
  MbVector3D  ders[sdt_CountDer]; ///< \ru Точка и производные. \en Point and derivatives.
  MbVector3D  norm;               ///< \ru Нормаль. \en Normal.

public:
  MbSurfaceWorkingData (); // \ru Конструктор по умолчанию \en Default constructor 
  MbSurfaceWorkingData ( const MbSurfaceWorkingData & ); // \ru Конструктор копирования. \en Copy-constructor. 
  ~MbSurfaceWorkingData() {} // \ru Деструктор. \en Destructor. 

public:
  void Init         (); // \ru Установить пустые значения. \en Set empty values. 
  void Init         ( const MbSurfaceWorkingData & ); // \ru Установить значения. \en Set values. 
  void Move         ( const MbVector3D & ); // \ru Сдвинуть значения по вектору. \en Move values by vector.
public:
  void SetPoint     ( double u0, double v0, bool ext, double u, double v, const MbCartPoint3D & pnt ); // \ru Установить точку. \en Set point. 
  bool GetPoint     ( double u0, double v0, bool ext, double & u, double & v, MbCartPoint3D & pnt ) const; // \ru Получить точку. \en Get point. 

  bool SetDerivative( double u0, double v0, bool ext, double u, double v, size_t k, const MbVector3D & der ); // \ru Установить производную. \en Set derivative. 
  bool GetDerivative( double u0, double v0, bool ext, double & u, double & v, size_t k, MbVector3D & der ) const; // \ru Получить производную. \en Get derivative. 

  void _SetParams0   ( double u0, double v0, bool ext0 ); // \ru Установить исходный параметр. \en Set initial parameter. 
  void _SetParams    ( double u, double v ); // \ru Установить модифицированный параметр. \en Set modified parameter. 
  bool _SetDerivative( size_t k, const MbVector3D & der ); // \ru Установить производную. \en Set derivative. 
  void _SetNormal    ( const MbVector3D & n );// \ru Установить нормаль. \en Set normal. 

  bool Explore      ( double u0, double v0, bool ext, double & u, double & v,
                      MbCartPoint3D & pnt, MbVector3D & uDer, MbVector3D & vDer,
                      MbVector3D * uuDer, MbVector3D * vvDer, MbVector3D * uvDer, MbVector3D * nor ) const;  // \ru Получить данные. \en Get data. 

private:
  MbSurfaceWorkingData & operator = ( const MbSurfaceWorkingData & ); // \ru Присвоить значение. \en Assign a value. 
};


//------------------------------------------------------------------------------
// \ru Конструктор по умолчанию \en Default constructor 
// ---
inline MbSurfaceWorkingData::MbSurfaceWorkingData()
  : uv0( UNDEFINED_DBL, UNDEFINED_DBL )
  , uv ( UNDEFINED_DBL, UNDEFINED_DBL )
  , ext( false )
{
  for ( size_t k = 0; k < sdt_CountDer; k++ )
    ders[k].Init( UNDEFINED_DBL, UNDEFINED_DBL, UNDEFINED_DBL );
  norm.Init( UNDEFINED_DBL, UNDEFINED_DBL, UNDEFINED_DBL );
}

//------------------------------------------------------------------------------
// \ru Конструктор копирования. \en Copy-constructor. 
// ---
inline MbSurfaceWorkingData::MbSurfaceWorkingData( const MbSurfaceWorkingData & init )
  : uv0( init.uv0 )
  , uv ( init.uv  )
  , ext( init.ext )
{
  for ( size_t k = 0; k < sdt_CountDer; k++ )
    ders[k].Init( init.ders[k] );
  norm.Init( init.norm );
}

//------------------------------------------------------------------------------
// \ru Установить пустые значения. \en Set empty values. 
// ---
inline void MbSurfaceWorkingData::Init()
{
  ext = false;
  uv0.x = UNDEFINED_DBL;
  uv.x = UNDEFINED_DBL;
  for ( size_t k = 0; k < sdt_CountDer; k++ )
    ders[k].x = UNDEFINED_DBL;
  norm.x = UNDEFINED_DBL;
}

//------------------------------------------------------------------------------
// \ru Установить значения. \en Set values. 
// ---
inline void MbSurfaceWorkingData::Init( const MbSurfaceWorkingData & init )
{
  ext = init.ext;
  uv0.Init( init.uv0 );
  uv.Init( init.uv );
  for ( size_t k = 0; k < sdt_CountDer; k++ )
    ders[k].Init( init.ders[k] );
  norm.Init( init.norm );
}

//------------------------------------------------------------------------------
// \ru Сдвинуть значения по вектору. \en Move values by vector.
// ---
inline void MbSurfaceWorkingData::Move( const MbVector3D & to )
{
  if ( ders[sdt_SurPoint].x != UNDEFINED_DBL )
    ders[sdt_SurPoint].Add( to );
}

//------------------------------------------------------------------------------
// \ru Установить точку. \en Set point. 
// ---
inline void MbSurfaceWorkingData::SetPoint( double u0, double v0, bool ext0, double u, double v, const MbCartPoint3D & pnt )
{
  bool resetOther = true;
  if ( (ext == ext0) && (u0 == uv0.x) && (v0 == uv0.y) ) {
    ders[sdt_SurPoint].Init( pnt );
    resetOther = false;
  }
  if ( resetOther ) {
    ext = ext0;
    uv0.Init( u0, v0 );
    uv.Init( u, v );
    
    for ( size_t i = 0; i < sdt_CountDer; i++ )
      ders[i].x = UNDEFINED_DBL;
    norm.x = UNDEFINED_DBL;

    ders[sdt_SurPoint].Init( pnt.x, pnt.y, pnt.z );
  }
}

//------------------------------------------------------------------------------
// \ru Получить точку. \en Get point. 
// ---
inline bool MbSurfaceWorkingData::GetPoint( double u0, double v0, bool ext0, double & u, double & v, MbCartPoint3D & pnt ) const
{
  if ( (ext == ext0) && (u0 == uv0.x) && (v0 == uv0.y) ) {
    if ( ders[sdt_SurPoint].x != UNDEFINED_DBL ) {
      u = uv.x;
      v = uv.y;
      pnt.Init( ders[sdt_SurPoint].x, ders[sdt_SurPoint].y, ders[sdt_SurPoint].z );
      return true;
    }
  }
  return false;
}

//------------------------------------------------------------------------------
// \ru Установить производную. \en Set derivative. 
// ---
inline bool MbSurfaceWorkingData::SetDerivative( double u0, double v0, bool ext0, double u, double v, size_t k, const MbVector3D & der )
{
  C3D_ASSERT( k < sdt_CountDer );
  if ( k >= sdt_CountDer )
    return false;

  bool resetOther = true;
  if ( (ext == ext0) && (u0 == uv0.x) && (v0 == uv0.y) ) {
    ders[k].Init( der );
    resetOther = false;
  }
  if ( resetOther ) {
    ext = ext0;
    uv0.Init( u0, v0 );
    uv.Init( u, v );

    for ( size_t i = 0; i < sdt_CountDer; i++ )
      ders[i].x = UNDEFINED_DBL;
    norm.x = UNDEFINED_DBL;

    ders[k].Init( der );
  }

  return true;
}

//------------------------------------------------------------------------------
// \ru Получить производную. \en Get derivative. 
// ---
inline bool MbSurfaceWorkingData::GetDerivative( double u0, double v0, bool ext0, double & u, double & v, size_t k, MbVector3D & der ) const
{
  if ( (k < sdt_CountDer) && (ext == ext0) && (u0 == uv0.x) && (v0 == uv0.y) ) {
    if ( ders[k].x != UNDEFINED_DBL ) {
      u = uv.x;
      v = uv.y;
      der.Init( ders[k].x, ders[k].y, ders[k].z );
      return true;
    }
  }
  return false;
}

//------------------------------------------------------------------------------
// \ru Установить исходный параметр. \en Set initial parameter. 
// ---
inline void MbSurfaceWorkingData::_SetParams0( double u0, double v0, bool ext0 )
{
  uv0.x = u0;
  uv0.y = v0;
  ext = ext0;
}

//------------------------------------------------------------------------------
// \ru Установить модифицированный параметр. \en Set modified parameter. 
// ---
inline void MbSurfaceWorkingData::_SetParams( double u, double v )
{
  uv.x = u;
  uv.y = v;
} 

//------------------------------------------------------------------------------
// \ru Установить нормаль. \en Set normal. 
// ---
inline void MbSurfaceWorkingData::_SetNormal( const MbVector3D & n )
{
  norm.Init( n );
} 

//------------------------------------------------------------------------------
// \ru Установить производную. \en Set derivative. 
// ---
inline bool MbSurfaceWorkingData::_SetDerivative( size_t k, const MbVector3D & der )
{ 
  if ( k < sdt_CountDer ) {
    ders[k].Init( der );
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
// \ru Получить данные. \en Get data. 
// ---
inline bool MbSurfaceWorkingData::Explore( double u0, double v0, bool ext0, double & u, double & v,
                                           MbCartPoint3D & pnt, MbVector3D & uDer, MbVector3D & vDer,
                                           MbVector3D * uuDer, MbVector3D * vvDer, MbVector3D * uvDer, MbVector3D * nor ) const
{
  bool res = false;

  // \\omega\Kernel\Models\Building\Konkurs_2012b\60114\кабина\бампер.c3d
  // if ( (ext == ext0) && (::fabs(u0 - uv0.x) < EXTENT_EQUAL) && (::fabs(v0 - uv0.y) < EXTENT_EQUAL) ) {
  if ( (ext == ext0) && (u0 == uv0.x) && (v0 == uv0.y) ) {
    if ( ders[sdt_SurPoint].x != UNDEFINED_DBL && ders[sdt_DeriveU].x != UNDEFINED_DBL && ders[sdt_DeriveV].x != UNDEFINED_DBL ) {
      bool resUU = false;
      bool resUV = false;
      bool resVV = false;

      if ( uuDer == nullptr )
        resUU = true;
      else if ( ders[sdt_DeriveUU].x != UNDEFINED_DBL ) {
        uuDer->Init( ders[sdt_DeriveUU] );
        resUU = true;
      }
      if ( uvDer == nullptr )
        resUV = true;
      else if ( ders[sdt_DeriveUV].x != UNDEFINED_DBL ) {
        uvDer->Init( ders[sdt_DeriveUV] );
        resUV = true;
      }
      if ( vvDer == nullptr )
        resVV = true;
      else if ( ders[sdt_DeriveVV].x != UNDEFINED_DBL ) {
        vvDer->Init( ders[sdt_DeriveVV] );
        resVV = true;
      }
      if ( resUU && resUV && resVV ) {
        if ( nor == nullptr )
          res = true;
        else if ( norm.x != UNDEFINED_DBL ) {
          nor->Init( norm );
          res = true;
        }
        if ( res ) {
          u = uv.x;
          v = uv.y;
          const MbVector3D & der0 = ders[sdt_SurPoint];
          pnt.Init( der0.x, der0.y, der0.z );
          uDer.Init( ders[sdt_DeriveU] );
          vDer.Init( ders[sdt_DeriveV] );
        }
      }
    }
  }

  return res;
}


//------------------------------------------------------------------------------
/** \brief \ru Дополнительные (сопутствующие) данные о поверхности.
           \en Additional (related) surface data. \~
  \details \ru Дополнительные (сопутствующие) данные о поверхности.
           \en Additional (related) surface data. \~ 
  \ingroup Data_Structures
*/ // ---
class MATH_CLASS MbSurfaceContiguousData {
private:
  MbeSurfacePoleType    poleUMin;     ///< \ru Флаг полюса на границе Umin. \en Pole flag on the border Umin. 
  MbeSurfacePoleType    poleUMax;     ///< \ru Флаг полюса на границе Umax. \en Pole flag on the border Umax. 
  MbeSurfacePoleType    poleVMin;     ///< \ru Флаг полюса на границе Vmin. \en Pole flag on the border Vmin. 
  MbeSurfacePoleType    poleVMax;     ///< \ru Флаг полюса на границе Vmax. \en Pole flag on the border Vmax. 
  MbRect1D              uExtRange;    ///< \ru Параметрический интервал U на продолженной поверхности. \en Parametric extended surface interval U. 
  MbRect1D              vExtRange;    ///< \ru Параметрический интервал V на продолженной поверхности. \en Parametric extended surface interval V. 
  ThreeStates           isLineU;      ///< \ru Флаг линейности вдоль параметра U. \en Flags of linearity along parameter U. 
  ThreeStates           isLineV;      ///< \ru Флаг линейности вдоль параметра V. \en Flags of linearity along parameter V.
  ThreeStates           isPlanar;     ///< \ru Флаг планарности. \en Planarity flag.
  size_t                uCount;       ///< \ru Число разбиений по U. \en The number of partitions along parameter U. 
  size_t                vCount;       ///< \ru Число разбиений по V. \en The number of partitions along parameter V.  
  MbSurfaceTessellation tessellation; ///< \ru Разбивка поверхности. \en Surface tessellation.

public:
  MbSurfaceContiguousData(); // \ru Конструктор по умолчанию \en Default constructor 
  MbSurfaceContiguousData( const MbSurfaceContiguousData & other ); // \ru Конструктор копирования. \en Copy-constructor. 

  void Reset                 (); // \ru Сбросить данные. \en Reset data. 
  void ResetExceptBordersData(); // \ru Сбросить данные. \en Reset data. 
  void Init                  ( const MbSurfaceContiguousData & other ); // \ru Установить данные. \en Set data. 

  MbSurfaceContiguousData & operator = ( const MbSurfaceContiguousData & other ); // \ru Присвоить значение. \en Assign a value. 

public:
  MbeSurfacePoleType &    PoleUMin    () { return poleUMin;     } // \ru Получить флаг полюса на границе Umin. \en Get the pole flag on the border Umin. 
  MbeSurfacePoleType &    PoleUMax    () { return poleUMax;     } // \ru Получить флаг полюса на границе Umax. \en Get the pole flag on the border Umax. 
  MbeSurfacePoleType &    PoleVMin    () { return poleVMin;     } // \ru Получить флаг полюса на границе Vmin. \en Get the pole flag on the border Vmin. 
  MbeSurfacePoleType &    PoleVMax    () { return poleVMax;     } // \ru Получить Флаг полюса на границе Vmax. \en Get the pole flag on the border Vmax. 
  MbRect1D &              URange      () { return uExtRange;    } // \ru Получить параметрический интервал U на продолженной поверхности. \en Get the parametric extended surface interval U. 
  MbRect1D &              VRange      () { return vExtRange;    } // \ru Получить параметрический интервал V на продолженной поверхности. \en Get the parametric extended surface interval V. 
  ThreeStates &           LineU       () { return isLineU;      } // \ru Получить флаг линейности вдоль параметра U. \en Get the flags of linearity along parameter U. 
  ThreeStates &           LineV       () { return isLineV;      } // \ru Получить флаг линейности вдоль параметра V. \en Get the flags of linearity along parameter V.
  ThreeStates &           Planar      () { return isPlanar;     } // \ru Получить флаг планарности. \en Get the planarity flag.
  size_t &                UCount      () { return uCount;       } // \ru Получить число разбиений по U. \en Get the number of partitions along parameter U. 
  size_t &                VCount      () { return vCount;       } // \ru Получить число разбиений по V. \en Get the number of partitions along parameter V.  
  MbSurfaceTessellation & Tessellation() { return tessellation; } // \ru Получить разбивку поверхности. \en Get the surface tessellation.

  void                    ClearTessellation() { tessellation.Clear(); } // \ru Очистить разбивку. \en Clear tessellation.
};


#endif // __SURF_TESSELLATION_H
