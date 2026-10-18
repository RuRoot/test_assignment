////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Базовый класс сущностей StepEntity и объекты для передачи примитивов.
         \en Base class StepEntity of entities and objects for geometric primitives
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef SE_ELEMENTARY_H
#define SE_ELEMENTARY_H

#include <math_define.h>
#include <templ_visitor.h>

#include <array>
#include <memory>

namespace c3d
{
  namespace converter
  {
//------------------------------------------------------------------------------
    /** \brief \ru Абстрактная сущность STEP.
               \en Abstract STEP entity. \~
        \note \ru ЭКСПЕРИМЕНТАЛЬНАЯ.
              \en EXPEREIMENTAL. \~
    */
    class CONV_CLASS SeBase
    {
      size_t m_thisId; ///< \ru Собственный идентификатор. \en Own identifier.
    public:
      /// \ru Предопределённое значнение некорректного идентификатора объекта. \en Pre-defined value for invalid identifier.
      static const size_t forbiddenId = 0;

      /// \ru Получить собственный идентификатор. \en Get own identifier.
      size_t ThisId() const;

      /// \ru Деструктор. \en Destructor.
      virtual ~SeBase() = default;

      /// \ru Принять посетителя. \en Accept a visitior. 
      virtual void Accept( Visitor & ) = 0;

      /// \ru Принять посетителя. \en Accept a visitior. 
      virtual void Accept( Visitor & ) const = 0;

    protected:
      /// \ru Конструктор. \en Constructor.
      SeBase( size_t thisId );

      /// \ru Конструктор копировани удалён. \en Copy constructor removed.
      SeBase( SeBase const & ) = delete;

      /// \ru Конструктор перемещения удалён. \en Move constructor removed.
      SeBase( SeBase && ) = delete;

      /// \ru Оператор присваивания удалён. \en Assignment operator removed.
      const SeBase& operator =( SeBase const & ) = delete;
    };


    //------------------------------------------------------------------------------
    /** \brief \ru Точка STEP.
               \en Point STEP. \~
        \note \ru ЭКСПЕРИМЕНТАЛЬНАЯ.
              \en EXPEREIMENTAL. \~
    */
    class CONV_CLASS SePoint : public SeBase
    {
      std::array<double,3> m_xyz; ///< \ru Координаты. \en Coordinates.
    public:

      /// \ru Получить значение координаты по индексу: 0 - x, 1- y, 2 - z. \en Get component by index: 0 - x, 1- y, 2 - z.
      double operator[]( size_t ) const;

      VISITING_CLASS( SePoint )

      void Accept( Visitor & visitor ) const override
      {
        auto * impl = dynamic_cast<VisitorImpl<const SePoint> *>(&visitor);
        if( impl )
          impl->Visit( *this );
      }

      /// \ru Создать точку. \en Create a point.
      static std::shared_ptr<SePoint> Create( size_t thisId, double x, double y, double z );

    private:
      /// \ru Конструктор. \en Constructor.
      SePoint( size_t thisId, double x, double y, double z );
    };


    //------------------------------------------------------------------------------
    /** \brief \ru Вектор STEP.
               \en Vector STEP. \~
        \note \ru ЭКСПЕРИМЕНТАЛЬНАЯ.
              \en EXPEREIMENTAL. \~
    */
    class CONV_CLASS SeVector : public SeBase
    {
      size_t m_direction; ///< \ru Направление. \en Direction.
      double m_magnitude; ///< \ru Длина. \en Magintude.
    public:

      /// \ru Получить направление. \en Get a direction.
      size_t GetDirction() const;

      /// \ru Получить длину. \en Get magnitude.
      double GetMagnitude() const;

      VISITING_CLASS( SeVector )

      void Accept( Visitor & visitor ) const override
      {
        auto * impl = dynamic_cast<VisitorImpl<const SeVector> *>(&visitor);
        if( impl )
          impl->Visit( *this );
      }

      /// \ru Создать вектор. \en Create a vector.
      static std::shared_ptr<SeVector> Create( size_t thisId, size_t direction, double magnitude );

    private:
      /// \ru Конструктор. \en Constructor.
      SeVector( size_t thisId, size_t direction, double magnitude );
    };


    //------------------------------------------------------------------------------
    /** \brief \ru Направление STEP.
               \en . \~
        \note \ru ЭКСПЕРИМЕНТАЛЬНАЯ.
              \en EXPEREIMENTAL. \~
    */
    class CONV_CLASS SeDirection : public SeBase
    {
      std::array<double,3> m_xyz; ///< \ru Координаты. \en Coordinates.
    public:

      /// \ru Получить значение координаты по индексу: 0 - x, 1- y, 2 - z, прочее - ислключение. \en Get component by index: 0 - x, 1- y, 2 - z, other - exception.
      double operator[]( size_t ) const;

      VISITING_CLASS( SeDirection )

      void Accept( Visitor & visitor ) const override
      {
        auto * impl = dynamic_cast<VisitorImpl<const SeDirection> *>(&visitor);
        if( impl )
          impl->Visit( *this );
      }

      /// \ru Создать направление. \en Create a direction.
      static std::shared_ptr<SeDirection> Create( size_t thisId, double x, double y, double z );

    private:
      /// \ru Конструктор. \en Constructor.
      SeDirection( size_t thisId, double x, double y, double z );
    };


    //------------------------------------------------------------------------------
    /** \brief \ru Локалькая система координат STEP.
               \en STEP placement. \~
        \note \ru ЭКСПЕРИМЕНТАЛЬНАЯ.
              \en EXPEREIMENTAL. \~
    */
    class CONV_CLASS SePlacement3D : public SeBase
    {
      size_t m_origin,///< \ru Начало. \en Origin.
             m_axisZ, ///< \ru Ось z. \en Z axis.
             m_axisX; ///< \ru Ось x. \en X axis.
    public:
      /// \ru Создать локальную систему координат. \en Create a placement.
      static std::shared_ptr<SePlacement3D> Create( size_t thisId, size_t origin, size_t axisZ, size_t axisX );

      /// \ru Получить начало. \en Get origin.
      size_t GetOrigin() const;

      /// \ru получить ось z. \en Get axis z.
      size_t GetAxisZ() const;

      /// \ru Получить ось x. \en Get axis x.
      size_t GetAxisX() const;

      VISITING_CLASS( SePlacement3D )

      void Accept( Visitor & visitor ) const override
      {
        auto * impl = dynamic_cast<VisitorImpl<SePlacement3D const> *>(&visitor);
        if( impl )
          impl->Visit( *this );
      }

    private:
      /// \ru Конструктор. \en Constructor.
      SePlacement3D( size_t thisId, size_t origin, size_t axisZ, size_t axisX );
    };

    
    //------------------------------------------------------------------------------
    /** \brief \ru Ось STEP.
               \en STEP axis. \~
        \note \ru ЭКСПЕРИМЕНТАЛЬНАЯ.
              \en EXPEREIMENTAL. \~
    */
    class CONV_CLASS SeAxis : public SeBase
    {
      size_t m_axis; ///< \ru Ось z. \en Z axis.
    public:
      /// \ru Создать локальную систему координат. \en Create a placement.
      static std::shared_ptr<SeAxis> Create( size_t thisId, size_t axis );

      /// \ru Получить ось. \en Get axis.
      size_t GetAxis() const;

      VISITING_CLASS( SeAxis )

      void Accept( Visitor & visitor ) const override
      {
        auto * impl = dynamic_cast<VisitorImpl<SeAxis const> *>(&visitor);
        if( impl )
          impl->Visit( *this );
      }

    private:
      /// \ru Конструктор. \en Constructor.
      SeAxis( size_t thisId, size_t axis );
    };
  }
};

#endif //SE_ELEMENTARY_H
