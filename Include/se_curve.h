////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Сущности для передачи кривых STEP.
         \en Entities for STEP curves.
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __SE_CURVE_H
#define __SE_CURVE_H

#include <vector>
#include <se_elementary.h>

namespace c3d
{
  namespace converter {

    //------------------------------------------------------------------------------
    /** \brief \ru Тип узлов B-сплайна STEP.
               \en B-spline knots vector type. \~
        \note \ru ЭКСПЕРИМЕНТАЛЬНАЯ.
              \en EXPEREIMENTAL. \~
    */
    enum class SeBKnotsType {
      UniformKnots,         ///< \ru Однородный узловой вектор. \en Uniform knots vector.
      QuasiUniformKnots,    ///< \ru Квазиоднородный узловой вектор. \en Quasi-uniform knots vector.
      PiecewiseBezierKnots, ///< \ru Узловой вектор кривой Безье. \en Bezier knots vector.
      Unspecified           ///< \ru Узловой вектор общего вида. \en General knots vector.
    };

    //------------------------------------------------------------------------------
    /** \brief \ru Форма B-сплайна STEP.
               \en Form of STEP B-spline. \~
        \note \ru ЭКСПЕРИМЕНТАЛЬНАЯ.
              \en EXPEREIMENTAL. \~
    */
    enum class SeBCurveForm {
      EllipticArc,  ///< \ru Дуга эллипса. \en Elliptic arc.
      PolylineForm, ///< \ru Ломаная. \en Polyline.
      ParabolicArc, ///< \ru Участок параболы. \en Part of parabola.
      CircularArc,  ///< \ru Дуга окружности. \en Circular arc.
      Unspecified,  ///< \ru Общего вида. \en General type.
      HyperbolicArc ///< \ru Участок гиперболы. \en Part of hyperbola.
    };

    //------------------------------------------------------------------------------
    /** \brief \ru Форма ограничения кривой STEP.
               \en . Trimming form of STEP curve\~
        \note \ru ЭКСПЕРИМЕНТАЛЬНАЯ.
              \en EXPEREIMENTAL. \~
    */
    enum class SeTrimForm {
      Parameter,    ///< \ru Усечение параметром. \en Trim by parameter.
      Unspecified,  ///< \ru Неизвестный тип. \en Trim undefined.
      Cartesian,    ///< \ru Усечение точкой. \en Trim by point.
    };

    //------------------------------------------------------------------------------
    /** \brief \ru Прямая STEP.
               \en . \~
        \note \ru ЭКСПЕРИМЕНТАЛЬНАЯ.
              \en EXPEREIMENTAL. \~
    */
    class CONV_CLASS SeLineCurve final : public SeBase {
      size_t m_point;     ///< \ru Точка. \en Point.
      size_t m_direction; ///< \ru Направление. \en Direction.
    public:
      /// \ru Получить точку. \en Get point.
      size_t GetPoint() const;

      /// \ru Получить направление. \en Get direction.
      size_t GetDirection() const;

      /// \ru Создать прямую. \en Create a line.
      static std::shared_ptr<SeLineCurve> Create(
        size_t const thisId,
        size_t const point,
        size_t const direction
      );

      VISITING_CLASS( SeLineCurve )

      void Accept( Visitor & visitor ) const override
      {
        auto * impl = dynamic_cast<VisitorImpl<const SeLineCurve> *>(&visitor);
        if( impl )
          impl->Visit( *this );
      }

    private:
      /// \ru Конструктор. \en Constructor.
      SeLineCurve(
        size_t const thisId,
        size_t const point,
        size_t const direction
      );
    };


    //------------------------------------------------------------------------------
    /** \brief \ru Коническое сечение STEP.
               \en Conic section STEP. \~
        \note \ru ЭКСПЕРИМЕНТАЛЬНАЯ.
              \en EXPEREIMENTAL. \~
    */
    class CONV_CLASS SeConic : public SeBase {
      size_t m_postion;     ///< \ru Локальная система координат. \en PLacement.
    public:
      /// \ru Получить локальную систему координат. \en Get placement.
      size_t GetPosition() const;

    protected:
      /// \ru Конструктор. \en Constructor.
      SeConic(
        size_t const thisId,
        size_t const position
      );
    };


    //------------------------------------------------------------------------------
    /** \brief \ru Окружность STEP.
               \en STEP circle. \~
        \note \ru ЭКСПЕРИМЕНТАЛЬНАЯ.
              \en EXPEREIMENTAL. \~
    */
    class CONV_CLASS SeCircleCurve final : public SeConic {
      double m_radius;  ///< \ru Радиус. \en Radius.
    public:

      /// \ru Получить радиус. \en Get radius.
      double GetRadius() const;

      /// \ru Создать окружность. \en Create a circle.
      static std::shared_ptr<SeCircleCurve> Create(
        size_t const thisId,
        size_t const center,
        double const radius
      );

      VISITING_CLASS( SeCircleCurve )

      void Accept( Visitor & visitor ) const override
      {
        auto * impl = dynamic_cast<VisitorImpl<const SeCircleCurve> *>(&visitor);
        if( impl )
          impl->Visit( *this );
      }

    private:
      /// \ru Конструктор. \en Constructor.
      SeCircleCurve(
        size_t const thisId,
        size_t const position,
        double const radius
      );
    };

    //------------------------------------------------------------------------------
    /** \brief \ru Ограниченная кривая STEP.
               \en Bounded curve. \~
        \note \ru ЭКСПЕРИМЕНТАЛЬНАЯ.
              \en EXPEREIMENTAL. \~
    */
    class CONV_CLASS SeBoundedCurve : public SeBase {
    protected:
      SeBoundedCurve(size_t const thisId);
    };


    //------------------------------------------------------------------------------
    /** \brief \ru B-сплайн с узлами STEP.
               \en . \~
        \note \ru ЭКСПЕРИМЕНТАЛЬНАЯ.
              \en EXPEREIMENTAL. \~
    */
    class CONV_CLASS SeBSplineWithKnotsCurve final : public SeBase {
      ptrdiff_t m_degree;                 ///< \ru . \en .
      std::vector<size_t> m_points;  ///< \ru Опорные точки. \en Base points.
      SeBCurveForm m_form;                ///< \ru Форма. \en Form.
      bool m_isClosed;                    ///< \ru Замкнутость. \en Is closed.
      bool m_isSelfIntersected;           ///< \ru Самопересечение. \en Is self-intersectin.
      std::vector<ptrdiff_t> m_knotMultiplicities;  ///< \ru Множители узлов. \en Knots multiplicities.
      std::vector<double> m_knots;                  ///< \ru Узлы. \en Knots.
      SeBKnotsType m_knotsType;                     ///< \ru Тип вектора. \en Type of knots vector.
      bool m_weightsDefined;                        ///< \ru Заданы ли веса точек. \en Wether the weights of points are defined.
      std::vector<double> m_weights;                ///< \ru Веса точек. \en Weights of points.

    public:

      /// \ru . \en .
      size_t GetDegree() const;

      /// \ru Получить количество контрольных точек. \en Get count of control points.
      size_t GetPointsCount() const;

      /// \ru Получить контрольную точку по индексу. \en Get control point by index.
      size_t GetPoint( size_t index ) const;

      /// \ru Получить форму кривой. \en Get curve's form.
      SeBCurveForm GetForm() const;

      /// \ru Получить признак замкнутости кривой. \en Is curve closed.
      bool IsClosed() const;

      /// \ru Получить признак самопересечения кривой. \en Is curve slef-intersecting.
      bool IsSelfIntersected() const;

      /// \ru Получить количество множителей узлов. \en Get count of knots multiplicities.
      size_t GetKnotMultiplicitiesCount() const;

      /// \ru Получить множитель узла по индексу. \en Get knots multiplicity by index.
      ptrdiff_t GetKnotMultiplicity( size_t index ) const;

      /// \ru Получить количество узлов. \en Get count of knots.
      size_t GetKnotsCount() const;

      /// \ru Получить узел по индексу. \en Get knot by index.
      double GetKnot( size_t index ) const;

      /// \ru Получить тип вектора. \en Get type of vector.
      SeBKnotsType GetKnotsType() const;

      /// \ru Заданы ли веса. \en Wether the weights defined.
      bool AreWeightsDefined() const;

      /// \ru Получить вес точки. \en Get weightf of a point.
      double GetWeight( size_t index );

      /// \ru Создать сплайн. \en Create a spline.
      static std::shared_ptr<SeBSplineWithKnotsCurve> Create(
        size_t const thisId,
        ptrdiff_t const degree,
        std::vector<size_t> && controlPointsList,
        SeBCurveForm const curveForm,
        bool const isClosed,
        bool const isSelfIntersected,
        std::vector<ptrdiff_t> && knotMultiplicities,
        std::vector<double> && knots,
        SeBKnotsType const knotsType
      );


      /// \ru Создать сплайн. \en Create a spline.
      static std::shared_ptr<SeBSplineWithKnotsCurve> Create(
        size_t const thisId,
        ptrdiff_t const degree,
        std::vector<size_t> && controlPointsList,
        SeBCurveForm const curveForm,
        bool const isClosed,
        bool const isSelfIntersected,
        std::vector<ptrdiff_t> && knotMultiplicities,
        std::vector<double> && knots,
        std::vector<double> && weights,
        SeBKnotsType const knotsType
      );

      VISITING_CLASS( SeBSplineWithKnotsCurve )

      void Accept( Visitor & visitor ) const override
      {
        auto * impl = dynamic_cast<VisitorImpl<const SeBSplineWithKnotsCurve> *>(&visitor);
        if( impl )
          impl->Visit( *this );
      }

    private:
      /// \ru Конструктор. \en Constructor.
      SeBSplineWithKnotsCurve(
        size_t const thisId,
        ptrdiff_t const degree,
        std::vector<size_t> && controlPointsList,
        SeBCurveForm const curveForm,
        bool const isClosed,
        bool const isSelfIntersected,
        std::vector<ptrdiff_t> && knotMultiplicities,
        std::vector<double> && knots,
        SeBKnotsType const knotsType
      );

      /// \ru Конструктор. \en Constructor.
      SeBSplineWithKnotsCurve(
        size_t const thisId,
        ptrdiff_t const degree,
        std::vector<size_t> && controlPointsList,
        SeBCurveForm const curveForm,
        bool const isClosed,
        bool const isSelfIntersected,
        std::vector<ptrdiff_t> && knotMultiplicities,
        std::vector<double> && knots,
        std::vector<double> && weights,
        SeBKnotsType const knotsType
      );
    };

    //------------------------------------------------------------------------------
    /** \brief \ru Ломаная STEP.
               \en STEP polyline. \~
        \note \ru ЭКСПЕРИМЕНТАЛЬНАЯ.
              \en EXPEREIMENTAL. \~
    */
    class CONV_CLASS SePolylineCurve final : public SeBoundedCurve {
      std::vector<size_t> m_points; ///< \ru Точки ломаной. \en Points of polyline.
    public:

      /// \ru Получить количество точек. \en Get count of points.
      size_t GetPointsCount() const;

      /// \ru Получить точку по индексу. \en Get point by index.
      size_t GetPoint( size_t index ) const;

      /// \ru Создать ломаную. \en Create polyline.
      static std::shared_ptr<SePolylineCurve> Create(
        size_t const thisId,
        std::vector<size_t> && points
      );

      VISITING_CLASS( SePolylineCurve )

      void Accept( Visitor & visitor ) const override
      {
        auto * impl = dynamic_cast<VisitorImpl<const SePolylineCurve> *>(&visitor);
        if( impl )
          impl->Visit( *this );
      }
      
    private:
      /// \ru Конструктор. \en Constructor.
      SePolylineCurve(
        size_t const thisId,
        std::vector<size_t> && points
      );
    };

    //------------------------------------------------------------------------------
    /** \brief \ru Ограниченная кривая STEP.
               \en . \~
        \note \ru ЭКСПЕРИМЕНТАЛЬНАЯ.
              \en EXPEREIMENTAL. \~
    */
    class CONV_CLASS SeTrimmedCurve final : public SeBoundedCurve {
      size_t m_baseCurve;           ///< \ru Усекаемая кривая. \en Trimmed curve.
      std::array<double,2> m_trimmingParameters; ///< \ru Пераметры усечения. \en Trimming parameters.
      bool m_sense; ///< \ru Признак сонаправленности. \en Is trimmed curve co-directed to the base one.
      SeTrimForm m_masterRepresentation;  ///< \ru . \en .
    public:
      /// \ru Получить усекаемую кривую. \en Get curve to trim.
      size_t GetBaseCurve() const;

      /// \ru Получить первый параметр усечения. \en Get first trimming parameter.
      double GetTrim1() const;

      /// \ru Получить второй параметр усечения. \en Get second trimming parameter.
      double GetTrim2() const;

      /// \ru Получить признак сонаправленности. \en Is trimmed curve co-directed to the base one.
      bool GetSense() const;

      /// \ru Получить главное представление. \en Get master representation.
      SeTrimForm GetMasterRepresentation() const;

      /// \ru Создать усечённую кривую. \en Create trimmed curve.
      static std::shared_ptr<SeTrimmedCurve> Create(
        size_t const thisId,
        size_t const baseCurve,
        double trim1,
        double trim2,
        bool const sense,
        SeTrimForm const masterRepresentation
      );

      VISITING_CLASS( SeTrimmedCurve )

      void Accept( Visitor & visitor ) const override
      {
        auto * impl = dynamic_cast<VisitorImpl<const SeTrimmedCurve> *>(&visitor);
        if( impl )
          impl->Visit( *this );
      }

    private:
      /// \ru Конструктор. \en Constructor.
      SeTrimmedCurve(
        size_t const thisId,
        size_t const baseCurve,
        double const trim1,
        double const trim2,
        bool const sense,
        SeTrimForm const masterRepresentation
      );
    };

    //------------------------------------------------------------------------------
    /** \brief \ru Составная кривая STEP.
               \en Composite curve STEP. \~
        \note \ru ЭКСПЕРИМЕНТАЛЬНАЯ.
              \en EXPEREIMENTAL. \~
    */
    class CONV_CLASS SeCompositeCurve final : public SeBoundedCurve {
      std::vector<size_t> m_segments; ///< \ru Сеггменты составной кривой. \en .
      bool m_isSelfIntersected;     ///< \ru Является ли кривая самопересекающейся. \en Wether the curve is slef-intersectng.
    public:
      /// \ru Получить количество сегментов. \en Get segments count.
      size_t GetSegmentsCount() const;

      /// \ru Получить сггмент по индексу. \en Get degment by index.
      size_t GetSegment( size_t index ) const;

      /// \ru Получить признак самопересечения. \en Wether the curve is slef-intersectng.
      bool IsSelfIntersected() const;

      /// \ru Создать составную кривую. \en Create composite curve.
      static std::shared_ptr<SeCompositeCurve> Create(
        size_t const thisId,
        std::vector<size_t> && segments,
        bool const isSelfIntersected
      );

      VISITING_CLASS( SeCompositeCurve )

      void Accept( Visitor & visitor ) const override
      {
        auto * impl = dynamic_cast<VisitorImpl<const SeCompositeCurve> *>(&visitor);
        if( impl )
          impl->Visit( *this );
      }

    private:
      /// \ru Конструктор. \en Constructor.
      SeCompositeCurve(
        size_t const thisId,
        std::vector<size_t> && segments,
        bool const isSelfIntersected
      );
    };

  }
}

#endif