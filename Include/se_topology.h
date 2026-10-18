////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief Объекты для передачи топологии StepEntity.
*/
////////////////////////////////////////////////////////////////////////////////
#ifndef __SE_TOPOLOGY_H
#define __SE_TOPOLOGY_H

#include <se_elementary.h>
namespace c3d
{
  namespace converter
  {
    //------------------------------------------------------------------------------
    /** \brief \ru Твердое тело.
           \en Solid. \~
    */
    class CONV_CLASS SeSolid : public SeBase
    {
      size_t m_shell;
    public:
      /// \ru . \en .
      static std::shared_ptr<SeSolid> Create( size_t thisId, size_t shell );

      /// \ru . \en .
      size_t GetShell() const;

      VISITING_CLASS( SeSolid )

      void Accept( Visitor & visitor ) const override
      {
        auto * impl = dynamic_cast<VisitorImpl<const SeSolid> *>(&visitor);
        if( impl )
          impl->Visit( *this );
      }

    private:
      /// \ru Конструктор. \en Constructor.
      SeSolid( size_t thisId, size_t shell );
    };


    //------------------------------------------------------------------------------
    /** \brief \ru Открытая оболочка.
           \en Open shell. \~
    */
    class CONV_CLASS SeOpenShell : public SeBase
    {
      std::vector<size_t> m_faces;
    public:

      /// \ru . \en .
      static std::shared_ptr<SeOpenShell> Create( size_t thisId, std::vector<size_t> && faces );

      /// \ru . \en .
      size_t FacesCount() const;

      /// \ru . \en .
      size_t GetFace( size_t index ) const;

      VISITING_CLASS( SeOpenShell )

      void Accept( Visitor & visitor ) const override
      {
        auto * impl = dynamic_cast<VisitorImpl<const SeOpenShell> *>(&visitor);
        if( impl )
          impl->Visit( *this );
      }

    private:
      /// \ru Конструктор. \en Constructor.
      SeOpenShell( size_t thisId, std::vector<size_t> && faces );
    };


    //------------------------------------------------------------------------------
    /** \brief \ru Замкнутая оболочка.
           \en Closed shell. \~
    */
    class CONV_CLASS SeClosedShell : public SeBase
    {
      std::vector<size_t> m_faces;
    public:

      /// \ru . \en .
      static std::shared_ptr<SeClosedShell> Create( size_t thisId, std::vector<size_t> && faces );

      /// \ru . \en .
      size_t FacesCount() const;

      /// \ru . \en .
      size_t GetFace( size_t index ) const;

      VISITING_CLASS( SeClosedShell )

      void Accept( Visitor & visitor ) const override
      {
        auto * impl = dynamic_cast<VisitorImpl<const SeClosedShell> *>(&visitor);
        if( impl )
          impl->Visit( *this );
      }

    private:
      /// \ru Конструктор. \en Constructor.
      SeClosedShell( size_t thisId, std::vector<size_t> && faces );
    };


    //------------------------------------------------------------------------------
    /** \brief \ru Грань.
           \en Face. \~
    */
    class CONV_CLASS SeAdvancedFace : public SeBase
    {
      size_t m_surface;
      std::vector<size_t> m_bounds;
      bool m_sameSense;
    public:
      /// \ru . \en .
      static std::shared_ptr<SeAdvancedFace> Create( size_t thisId, size_t surface, std::vector<size_t> && bounds, bool sameSense );

      /// \ru . \en .
      size_t GetSurface() const;

      /// \ru . \en .
      size_t BoundsCount() const;

      /// \ru . \en .
      size_t GetBound( size_t index ) const;

      /// \ru . \en .
      bool IsSameSense() const;

      VISITING_CLASS( SeAdvancedFace )

      void Accept( Visitor & visitor ) const override
      {
        auto * impl = dynamic_cast<VisitorImpl<const SeAdvancedFace> *>(&visitor);
        if( impl )
          impl->Visit( *this );
      }

    private:
      /// \ru Конструктор. \en Constructor.
      SeAdvancedFace( size_t thisId, size_t surface, std::vector<size_t> && bounds, bool sameSense );
    };


    //------------------------------------------------------------------------------
    /** \brief \ru Граница грани.
           \en Face bound. \~
    */
    class CONV_CLASS SeFaceBound : public SeBase
    {
      size_t m_loop;
      bool m_orientation;
    public:
      /// \ru . \en .
      static std::shared_ptr<SeFaceBound> Create( size_t thisId, size_t loop, bool orientation );

      /// \ru . \en .
      size_t GetLoop() const;

      /// \ru . \en .
      bool GetOrientation() const;

      VISITING_CLASS( SeFaceBound )

      void Accept( Visitor & visitor ) const override
      {
        auto * impl = dynamic_cast<VisitorImpl<const SeFaceBound> *>(&visitor);
        if( impl )
          impl->Visit( *this );
      }

    private:
      /// \ru Конструктор. \en Constructor.
      SeFaceBound( size_t thisId, size_t loop, bool orientation );
    };


    //------------------------------------------------------------------------------
    /** \brief \ru Цикл.
           \en Loop. \~
    */
    class CONV_CLASS SeLoop : public SeBase
    {
      std::vector<size_t> m_edgeList;
    public:
      /// \ru . \en .
      static std::shared_ptr<SeLoop> Create( size_t thisId, std::vector<size_t> && edgeList );

      /// \ru . \en .
      size_t EdgesCount() const;

      /// \ru . \en .
      size_t GetOrientedEdge( size_t index ) const;

      VISITING_CLASS( SeLoop )

      void Accept( Visitor & visitor ) const override
      {
        auto * impl = dynamic_cast<VisitorImpl<const SeLoop> *>(&visitor);
        if( impl )
          impl->Visit( *this );
      }

    private:
      /// \ru Конструктор. \en Constructor.
      SeLoop( size_t thisId, std::vector<size_t> && edgeList );
    };


    //------------------------------------------------------------------------------
    /** \brief \ru Ориентированное ребро.
           \en Oriented edge. \~
    */
    class CONV_CLASS SeOrientedEdge : public SeBase
    {
      size_t m_edge;
      bool m_orientation;
    public:
      /// \ru . \en .
      static std::shared_ptr<SeOrientedEdge> Create( size_t thisId, size_t edge, bool orientation );

      /// \ru . \en .
      size_t GetEdge() const;

      /// \ru . \en .
      bool GetOrientation() const;

      VISITING_CLASS( SeOrientedEdge )

      void Accept( Visitor & visitor ) const override
      {
        auto * impl = dynamic_cast<VisitorImpl<const SeOrientedEdge> *>(&visitor);
        if( impl )
          impl->Visit( *this );
      }

    private:
      /// \ru Конструктор. \en Constructor.
      SeOrientedEdge( size_t thisId, size_t edge, bool orientation );
    };


    //------------------------------------------------------------------------------
    /** \brief \ru Ребро.
           \en Edge. \~
    */
    class CONV_CLASS SeEdge : public SeBase
    {
      size_t m_curve;
      size_t m_begVertex, m_endVertex;
      bool m_sameSense;
    public:
      /// \ru . \en .
      static std::shared_ptr<SeEdge> Create( size_t thisId, size_t curve, size_t begVertex, size_t endVertex, bool sameSense );

      /// \ru . \en .
      size_t GetCurve() const;

      /// \ru . \en .
      size_t GetBegVertex() const;

      /// \ru . \en .
      size_t GetEndVertex() const;

      /// \ru . \en .
      bool IsSameSense() const;

      VISITING_CLASS( SeEdge )

      void Accept( Visitor & visitor ) const override
      {
        auto * impl = dynamic_cast<VisitorImpl<const SeEdge> *>(&visitor);
        if( impl )
          impl->Visit( *this );
      }

    private:
      /// \ru Конструктор. \en Constructor.
      SeEdge( size_t thisId, size_t curve, size_t begVertex, size_t endVertex, bool sameSense );
    };


    //------------------------------------------------------------------------------
    /** \brief \ru Вершина.
           \en Vertex. \~
    */
    class CONV_CLASS SeVertex : public SeBase
    {
      size_t m_point;
    public:
      /// \ru . \en .
      static std::shared_ptr<SeVertex> Create( size_t thisId, size_t point );

      /// \ru . \en .
      size_t GetPoint() const;

      VISITING_CLASS( SeVertex )

      void Accept( Visitor & visitor ) const override
      {
        auto * impl = dynamic_cast<VisitorImpl<const SeVertex> *>(&visitor);
        if( impl )
          impl->Visit( *this );
      }

    private:
      /// \ru Конструктор. \en Constructor.
      SeVertex( size_t thisId, size_t point );
    };
  }
}
#endif //__SE_TOPOLOGY_H