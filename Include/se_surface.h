////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief Объекты для передачи поверхностей StepEntity.
*/
////////////////////////////////////////////////////////////////////////////////
#ifndef __SE_SURFACE_H
#define __SE_SURFACE_H

#include <se_elementary.h>

namespace c3d
{
  namespace converter
  {

    //------------------------------------------------------------------------------
    /** \brief \ru Форма тора STEP.
               \en STEP Torus form. \~
        \note \ru ЭКСПЕРИМЕНТАЛЬНАЯ.
              \en EXPEREIMENTAL. \~
    */
    enum class SeTorusForm {
      Donut,  ///< \ru Тор. \en Donut.
      Apple,  ///< \ru Яблоко. \en Apple.
      Lemon,  ///< \ru Лимон. \en Lemon.
    };

    
    //------------------------------------------------------------------------------
    /** \brief \ru Плоскость.
           \en Plane. \~
    */
    class CONV_CLASS SePlane : public SeBase
    {
      size_t m_placement; ///< \ru Локальная система координат. \en Location.
    public:
      /// \ru . \en .
      static std::shared_ptr<SePlane> Create( size_t thisId, size_t placement );

      /// \ru . \en .
      size_t GetPlacement() const;

      VISITING_CLASS( SePlane )

      void Accept( Visitor & visitor ) const override
      {
        auto * impl = dynamic_cast<VisitorImpl<const SePlane> *>(&visitor);
        if( impl )
          impl->Visit( *this );
      }

    private:
      /// \ru Конструктор. \en Constructor.
      SePlane( size_t thisId, size_t placement );
    };


    //------------------------------------------------------------------------------
    /** \brief \ru Цилиндр.
           \en Cylinder surface. \~
    */
    class CONV_CLASS SeCylinderSurface : public SeBase
    {
      size_t m_placement; ///< \ru Локальная система координат. \en Location.
      double m_radius;    ///< \ru . \en .
    public:
      /// \ru . \en .
      static std::shared_ptr<SeCylinderSurface> Create( size_t thisId, size_t placement, double radius );

      /// \ru . \en .
      size_t GetPlacement() const;

      /// \ru . \en .
      double GetRadius() const;

      VISITING_CLASS( SeCylinderSurface )

      void Accept( Visitor & visitor ) const override
      {
        auto * impl = dynamic_cast<VisitorImpl<const SeCylinderSurface> *>(&visitor);
        if( impl )
          impl->Visit( *this );
      }

    private:
      /// \ru Конструктор. \en Constructor.
      SeCylinderSurface( size_t thisId, size_t placement, double radius );
    };


    //------------------------------------------------------------------------------
    /** \brief \ru Конус.
           \en Cone surface. \~
    */
    class CONV_CLASS SeConeSurface : public SeBase
    {
      size_t m_placement; ///< \ru Локальная система координат. \en Location.
      double m_radius,    ///< \ru . \en .
             m_semiAngle; ///< \ru . \en .
    public:
      /// \ru . \en .
      static std::shared_ptr<SeConeSurface> Create( size_t thisId, size_t placement, double radius, double semiAngle );

      /// \ru . \en .
      size_t GetPlacement() const;

      /// \ru . \en .
      double GetRadius() const;

      /// \ru . \en .
      double GetSemiAngle() const;

      VISITING_CLASS( SeConeSurface )

      void Accept( Visitor & visitor ) const override
      {
        auto * impl = dynamic_cast<VisitorImpl<const SeConeSurface> *>(&visitor);
        if( impl )
          impl->Visit( *this );
      }

    private:
      /// \ru Конструктор. \en Constructor.
      SeConeSurface( size_t thisId, size_t placement, double radius, double semiAngle );
    };


    //------------------------------------------------------------------------------
    /** \brief \ru Сфера.
           \en Sphere surface. \~
    */
    class CONV_CLASS SeSphereSurface : public SeBase
    {
      size_t m_placement; ///< \ru Локальная система координат. \en Location.
      double m_radius;    ///< \ru . \en .
    public:
      /// \ru . \en .
      static std::shared_ptr<SeSphereSurface> Create( size_t thisId, size_t placement, double radius );

      /// \ru . \en .
      size_t GetPlacement() const;

      /// \ru . \en .
      double GetRadius() const;

      VISITING_CLASS( SeSphereSurface )

      void Accept( Visitor & visitor ) const override
      {
        auto * impl = dynamic_cast<VisitorImpl<const SeSphereSurface> *>(&visitor);
        if( impl )
          impl->Visit( *this );
      }

    private:
      /// \ru Конструктор. \en Constructor.
      SeSphereSurface( size_t thisId, size_t placement, double radius );
    };


    //------------------------------------------------------------------------------
    /** \brief \ru Тор.
           \en Torus surface. \~
    */
    class CONV_CLASS SeTorusSurface : public SeBase
    {
      size_t m_placement;       ///< \ru Локальная система координат. \en Location.
      double m_majorRadius,     ///< \ru Радиус. \en Radius.
             m_minorRadius;     ///< \ru Толщина. \en Thickness.
      SeTorusForm m_torusForm;  ///< \ru Torus form. \en Form.
    public:

      /// \ru Создать поверхность тора. \en Create toroidal surace.
      static std::shared_ptr<SeTorusSurface> Create( size_t thisId, size_t placement, double majorRadius, double minorRadius );

      /// \ru . \en .
      size_t GetPlacement() const;

      /// \ru Получить радиус. \en Get radius.
      double GetMajorRadius() const;

      /// \ru Получить толщину. \en GetThickness.
      double GetMinorRadius() const;

      /// \ru Получить форму. \en Get form.
      SeTorusForm GetTorusForm() const;

      VISITING_CLASS( SeTorusSurface )

      void Accept( Visitor & visitor ) const override
      {
        auto * impl = dynamic_cast<VisitorImpl<const SeTorusSurface> *>(&visitor);
        if( impl )
          impl->Visit( *this );
      }

    private:
      /// \ru Конструктор. \en Constructor.
      SeTorusSurface( size_t thisId, size_t placement, double majorRadius, double minorRadius, SeTorusForm torusForm );
    };


    //------------------------------------------------------------------------------
    /** \brief \ru Сплайновая поверхность.
           \en Spline surface. \~
    */
    class CONV_CLASS SeSplineSurface : public SeBase
    {
      size_t m_uDegree,  ///< \ru . \en .
             m_vDegree; ///< \ru . \en .
      std::vector<size_t> m_uMultipliers, ///< \ru . \en .
                          m_vMultipliers; ///< \ru . \en .
      std::vector<double> m_uKnots,       ///< \ru . \en .
                          m_vKnots;       ///< \ru . \en .
      std::vector<std::vector<size_t>> m_Points;
      std::vector<std::vector<double>> m_Weights;
      bool m_uClosed, m_vClosed;
    public:
      /// \ru . \en .
      static std::shared_ptr<SeSplineSurface> Create(size_t thisId, size_t uDegree, size_t vDegree,
                                                     std::vector<size_t>&& uMultipliers, std::vector<size_t>&& vMultipliers,
                                                     std::vector<double>&& uKnots, std::vector<double>&& vKnots,
                                                     std::vector<std::vector<size_t>>&& points,
                                                     std::vector<std::vector<double>>&& weights,
                                                     bool uClosed, bool vClosed);
      /// \ru . \en .
      size_t GetUDegree() const;

      /// \ru . \en .
      size_t GetVDegree() const;

      /// \ru . \en .
      size_t UMultipliersCount() const;

      /// \ru . \en .
      size_t GetUMultiplier( size_t index ) const;

      /// \ru . \en .
      size_t VMultipliersCount() const;

      /// \ru . \en .
      size_t GetVMultiplier( size_t index ) const;

      /// \ru . \en .
      size_t UKnotsCount() const;

      /// \ru . \en .
      double GetUKnot( size_t index ) const;

      /// \ru . \en .
      size_t VKnotsCount() const;

      /// \ru . \en .
      double GetVKnot( size_t index ) const;

      /// \ru . \en .
      size_t PointLinesCount() const;

      /// \ru . \en .
      size_t PointColumnsCount() const;

      /// \ru . \en .
      size_t GetPoint( size_t i, size_t j ) const;

      /// \ru . \en .
      size_t WeightLinesCount() const;

      /// \ru . \en .
      size_t WeightColumnsCount() const;

      /// \ru . \en .
      double GetWeight( size_t i, size_t j ) const;

      /// \ru . \en .
      bool IsUClosed() const;

      /// \ru . \en .
      bool IsVClosed() const;

      VISITING_CLASS( SeSplineSurface )

      void Accept( Visitor & visitor ) const override
      {
        auto * impl = dynamic_cast<VisitorImpl<const SeSplineSurface> *>(&visitor);
        if( impl )
          impl->Visit( *this );
      }

    private:
      /// \ru Конструктор. \en Constructor.
      SeSplineSurface( size_t thisId, size_t uDegree, size_t vDegree,
               std::vector<size_t> && uMultipliers, std::vector<size_t> && vMultipliers,
               std::vector<double> && uKnots, std::vector<double> && vKnots,
               std::vector<std::vector<size_t>> && points,
               std::vector<std::vector<double>> && weights,
               bool uClosed, bool vClosed );
    };


    //------------------------------------------------------------------------------
    /** \brief \ru Поверхность выдавливания.
           \en Extrusion surface. \~
    */
    class CONV_CLASS SeExtrusionSurface : public SeBase
    {
      size_t m_curve;
      size_t m_direction;
    public:
      /// \ru . \en .
      static std::shared_ptr<SeExtrusionSurface> Create( size_t thisId, size_t curve, size_t direction );

      /// \ru . \en .
      size_t GetCurve() const;

      /// \ru . \en .
      size_t GetDirection() const;

      VISITING_CLASS( SeExtrusionSurface )

      void Accept( Visitor & visitor ) const override
      {
        auto * impl = dynamic_cast<VisitorImpl<const SeExtrusionSurface> *>(&visitor);
        if( impl )
          impl->Visit( *this );
      }

    private:
      /// \ru Конструктор. \en Constructor.
      SeExtrusionSurface( size_t thisId, size_t curve, size_t direction );
    };


    //------------------------------------------------------------------------------
    /** \brief \ru Поверхность вращения.
           \en Revolution surface. \~
    */
    class CONV_CLASS SeRevolutionSurface : public SeBase
    {
      size_t m_curve;
      size_t m_axis;
    public:
      /// \ru . \en .
      static std::shared_ptr<SeRevolutionSurface> Create( size_t thisId, size_t curve, size_t axis );

      /// \ru . \en .
      size_t GetCurve() const;

      /// \ru . \en .
      size_t GetAxis() const;

      VISITING_CLASS( SeRevolutionSurface )

      void Accept( Visitor & visitor ) const override
      {
        auto * impl = dynamic_cast<VisitorImpl<const SeRevolutionSurface> *>(&visitor);
        if( impl )
          impl->Visit( *this );
      }

    private:
      /// \ru Конструктор. \en Constructor.
      SeRevolutionSurface( size_t thisId, size_t curve, size_t axis );
    };


    //------------------------------------------------------------------------------
    /** \brief \ru Эквидистантная поверхность.
           \en Offset surface. \~
    */
    class CONV_CLASS SeOffsetSurface : public SeBase
    {
      size_t m_basisSurface;
      double m_distance;
    public:
      /// \ru . \en .
      static std::shared_ptr<SeOffsetSurface> Create( size_t thisId, size_t basisSurface, double distance );

      /// \ru . \en .
      size_t GetBasisSurface() const;

      /// \ru . \en .
      double GetDistance() const;

      VISITING_CLASS( SeOffsetSurface )

      void Accept( Visitor & visitor ) const override
      {
        auto * impl = dynamic_cast<VisitorImpl<const SeOffsetSurface> *>(&visitor);
        if( impl )
          impl->Visit( *this );
      }

    private:
      /// \ru Конструктор. \en Constructor.
      SeOffsetSurface( size_t thisId, size_t basisSurface, double distance );
    };
  }
}

#endif //__SE_SURFACE_H