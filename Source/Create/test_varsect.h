////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief  \ru Пример реализации класса для получения сечений.
          \en Example implementation of a class for obtaining sections. \~
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEST_VARSECT_H
#define __TEST_VARSECT_H

#include <op_swept_parameter.h>
#include <func_sinus_function.h>
#include <mb_matrix.h>
#include <cur_contour.h>


//------------------------------------------------------------------------------
/** \brief \ru Реализация класса для получения сечений в кинематической операции с динамической параметризацией сечения с синусоидальной функцией изменения масштаба сечений.
           \en An implementation of a class for obtaining sections for sweeping operation with variable section with a sinusoidal function for scaling sections. \~
           \~
*/
// ---
class VariableSectionSinScale : public IVariableSectionData {
  protected:
    const c3d::PlaneContoursSPtrVector & _initContours; ///< \ru Исходные образующие контуры. \en Initial generating contours.
    double                               _tMin;         ///< \ru Минимальное значение параметра функции изменения масштаба сечения. \en Minimum value of parameter of a section scaling function.
    double                               _tMax;         ///< \ru Максимальное значение параметра функции изменения масштаба сечения. \en Maximum value of parameter of a section scaling function.
    c3d::ConstFunctionSPtrVector         _funcs;        ///< \ru Массив, содержащий функцию изменения масштаба сечения. \en An array containing a section scaling function.

  public: 

    /// \ru Конструктор по параметрам. \en Constructor by parameters.
    VariableSectionSinScale( const double                         tMin,
                             const double                         tMax,
                             const c3d::PlaneContoursSPtrVector & initContours )
      : _initContours ( initContours )
      , _tMin  ( std_min(tMin, tMax) )
      , _tMax  ( std_max(tMin, tMax) )
      , _funcs ( )
      
    {
      _funcs.emplace_back( new MbSinusFunction(0.0, 0.9, _tMin, (_tMax - _tMin) / M_PI2, _tMin, _tMax) );
    };

    /// \ru Деструктор. \en Destructor.
    ~VariableSectionSinScale() {};

    /// \ru Получить сечение. \en Obtain a section.
    bool GetSection(       double                         t,
                     const MbPlacement3D &                ,
                           c3d::PlaneContoursSPtrVector & contours ) const override {
      auto res = true;
      auto scale = _funcs[0]->Value( t );
      MbMatrix scaleMatr( MbCartPoint(0.0, 0.0), 1.0 + scale, 1.0 + scale);
      for ( auto & initContour : _initContours ) {
        if ( initContour != nullptr ) {
          contours.emplace_back( static_cast<MbContour *>(&initContour->Duplicate()) );
          contours.back()->Transform( scaleMatr );
        }
        else {
          res = false;
          break;
        }
      }
      return res;
    };

    /// \ru Получить функции. \en Get functions.
    const c3d::ConstFunctionSPtrVector * GetFuncs() const {
      return &_funcs;
    };
};

#endif // __TEST_VARSECT_H