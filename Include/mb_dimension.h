////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Определение размеров.
         \en Dimensions definition. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MB_DIMENSION_H
#define __MB_DIMENSION_H


#include <legend.h>
#include <mb_placement3d.h>
#include <mb_cart_point3d.h>


//------------------------------------------------------------------------------
/** \brief \ru Размер.
           \en Dimension. \~
  \details \ru Общий класс размеров.\n
           \en Common class of dimensions.\n \~
  \ingroup Legend
*/
// --- 
class MATH_CLASS MbDimension : public MbLegend {
public:
  /// \ru Конструктор. \en Constructor
  MbDimension();
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbDimension();

protected:
  MbDimension( const MbDimension & );  ///< \ru Конструктор копирования. \en Copy-constructor. 
  
public:
  /**\ru \name Общие функции геометрического объекта.
     \en \name Common functions of a geometric object.
      \{ */   
  MbeSpaceType    Type() const override;
  bool            IsSimilar ( const MbSpaceItem & ) const override;
 
private:  // \ru Не реализованные методы класса \en Non-implemented methods of class 
  void            operator = ( const MbDimension & );        // \ru Не реализовано \en Not implemented 

  DECLARE_PERSISTENT_CLASS( MbDimension )
};


//------------------------------------------------------------------------------
/** \brief \ru Линейный размер.
           \en Linear dimension. \~
  \ingroup Legend
*/ // ---
class MATH_CLASS MbLinearDimension : public MbDimension {
private:
  MbCartPoint3D base1;                ///< \ru Первая точка привязки размера. \en First dimension anchor point. 
  MbCartPoint3D base2;                ///< \ru Вторая точка привязки размера. \en Second dimension anchor point.
  MbCartPoint3D startDimensionCurve;  ///< \ru Точка начала размерной линии. \en Starting point of dimension line.

public:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор.\n
             \en Constructor.\n \~  
    \param[in] base1 - \ru Первая точка привязки размера.
                       \en First dimension anchor point. \~
    \param[in] base2 - \ru Вторая точка привязки размера.
                       \en Second dimension anchor point. \~
    \param[in] startDimension - \ru Точка начала размерной линии.
                                \en Starting point of dimension line. \~
  */
    MbLinearDimension( const MbCartPoint3D & base1, const MbCartPoint3D & base2, const MbCartPoint3D & startDimensionCurve );
protected:
    MbLinearDimension( const MbLinearDimension & );   ///< \ru Конструктор копирования. \en Copy-constructor. 

public:
    /**\ru \name Общие функции геометрического объекта.
     \en \name Common functions of a geometric object.
      \{ */   
    MbeSpaceType  IsA()  const override;
    MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const override;
    bool          IsSame( const MbSpaceItem &, double /*accuracy*/ = LENGTH_EPSILON ) const override;
    bool          SetEqual( const MbSpaceItem & ) override;
    void          Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override;
    void          Move( const MbVector3D &, MbRegTransform * = nullptr ) override;
    void          Rotate( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override;
    double        DistanceToPoint( const MbCartPoint3D & ) const override;
    void          AddYourGabaritTo( MbCube & ) const override;
    void          CalculateMesh( const MbStepData &, const MbFormNote &, MbMesh & ) const override; // \ru Построить полигональную копию mesh. \en Build polygonal copy mesh.

    // \ru Тестовые функции геометрического объекта \en Test functions of a geometric object 
    MbProperty &  CreateProperty( MbePrompt /*n*/ ) const override; // \ru Создать собственное свойство \en Create own property
    void          GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта \en Get properties of the object
    void          SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of object

public:
    /// \ru Инициализировать по двум точкам привязки и точке начала размерной линии. \en Initialize by two reference points and the starting point of the dimension line. 
    void          Init( const MbCartPoint3D & base1, const MbCartPoint3D & base2, const MbCartPoint3D & startDimensionCurve );

    /// \ru Получить первую точку привязки размера. \en Get the first dimension snap point.
    MbCartPoint3D GetBasePoint1() const { return base1; }
    void          SetBasePoint1( const MbCartPoint3D & val ) { base1 = val; }

    /// \ru Получить вторую точку привязки размера. \en Get the second dimension snap point.
    MbCartPoint3D GetBasePoint2() const { return base2; }
    void          SetBasePoint2( const MbCartPoint3D & val ) { base2 = val; }

    /// \ru Получить первую точку привязки размера. \en Get the first dimension snap point.
    MbCartPoint3D GetStartDimensionCurvePoint() const { return startDimensionCurve; }
    void          SetStartDimensionCurvePoint( const MbCartPoint3D & val ) { startDimensionCurve = val; }
    
private:  // \ru Не реализованные методы класса \en Non-implemented methods of class 
    void          operator = ( const MbLinearDimension & );        // \ru Не реализовано \en Not implemented 

  DECLARE_PERSISTENT_CLASS(MbLinearDimension)
};


//------------------------------------------------------------------------------
/** \brief \ru Угловой размер.
           \en Angular dimension. \~
  \ingroup Legend
*/ // ---
class MATH_CLASS MbAngularDimension : public MbDimension {
private:
  MbCartPoint3D base1;  ///< \ru Первая точка привязки размера. \en First dimension anchor point. 
  MbCartPoint3D base2;  ///< \ru Вторая точка привязки размера. \en Second dimension anchor point.
  MbCartPoint3D center; ///< \ru Точка центра. \en Center point.

public:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор.\n
             \en Constructor.\n \~  
    \param[in] base1  - \ru Первая точка привязки размера.
                        \en First dimension anchor point. \~
    \param[in] center - \ru Точка центра.
                        \en Center point. \~
    \param[in] base2  - \ru Вторая точка привязки размера.
                        \en Second dimension anchor point. \~
  */
    MbAngularDimension( const MbCartPoint3D & center, const MbCartPoint3D & base1, const MbCartPoint3D & base2 );
protected:
    MbAngularDimension( const MbAngularDimension & );   ///< \ru Конструктор копирования. \en Copy-constructor. 

public:
    /**\ru \name Общие функции геометрического объекта.
       \en \name Common functions of a geometric object.
      \{ */   
    MbeSpaceType  IsA()  const override;
    MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const override;
    bool          IsSame( const MbSpaceItem &, double /*accuracy*/ = LENGTH_EPSILON ) const override;
    bool          SetEqual( const MbSpaceItem & ) override;
    void          Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override;
    void          Move( const MbVector3D &, MbRegTransform * = nullptr ) override;
    void          Rotate( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override;
    double        DistanceToPoint( const MbCartPoint3D & ) const override;
    void          AddYourGabaritTo( MbCube & ) const override;
    void          CalculateMesh( const MbStepData &, const MbFormNote &, MbMesh & ) const override; // \ru Построить полигональную копию mesh. \en Build polygonal copy mesh.

    // \ru Тестовые функции геометрического объекта \en Test functions of a geometric object 
    MbProperty &  CreateProperty( MbePrompt /*n*/ ) const override; // \ru Создать собственное свойство \en Create own property
    void          GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта \en Get properties of the object
    void          SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of object

public:
    /// \ru Инициализировать по двум точкам привязки и точке центра. \en Initialize by two reference points and center point. 
    void          Init( const MbCartPoint3D & center, const MbCartPoint3D & base1, const MbCartPoint3D & base2 );

    /// \ru Получить первую точку привязки размера. \en Get the first dimension snap point.
    MbCartPoint3D GetBasePoint1() const { return base1; }
    void          SetBasePoint1( const MbCartPoint3D & val ) { base1 = val; }

    /// \ru Получить вторую точку привязки размера. \en Get the second dimension snap point.
    MbCartPoint3D GetBasePoint2() const { return base2; }
    void          SetBasePoint2( const MbCartPoint3D & val ) { base2 = val; }

    /// \ru Получить точку центра. \en Get center point.
    MbCartPoint3D GetCenterPoint() const { return center; }
    void          SetCenterPoint( const MbCartPoint3D & val ) { center = val; }

private:  // \ru Не реализованные методы класса \en Non-implemented methods of class 
    void          operator = ( const MbAngularDimension & );        // \ru Не реализовано \en Not implemented 

  DECLARE_PERSISTENT_CLASS(MbAngularDimension)
};


//------------------------------------------------------------------------------
/** \brief \ru Радиальный размер.
           \en Radial dimension. \~
  \ingroup Legend
*/ // ---
class MATH_CLASS MbRadialDimension : public MbDimension
{
private:
  MbCartPoint3D center; ///< \ru Точка центра окружности. \en Center point of circle. 
  MbCartPoint3D circle; ///< \ru Точка на окружности. \en Point on a circle.
  MbPlacement3D placement; ///< \ru Местная система координат размера. \en Local coordinate system of the dimension. 
  bool          diametral; ///< \ru Признак того, что размер диаметральный. \en Sign of the fact that the diametrical dimension.

public:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор.\n
             \en Constructor.\n \~  
    \param[in] center - \ru Точка центра окружности.
                        \en Center point of circle. \~
    \param[in] circle - \ru Точка на окружности.
                        \en Point on a circle. \~
    \param[in] dimensionPlacement - \ru Местная система координат размера.
                                    \en Local coordinate system of the dimension. \~
    \param[in] diametral - \ru Признак того, что размер диаметральный.
                           \en Sign of the fact that the diametrical dimension. \~
  */
    MbRadialDimension( const MbCartPoint3D & center, const MbCartPoint3D & circle, const MbPlacement3D & dimensionPlacement, bool diametral );
protected:
    MbRadialDimension( const MbRadialDimension & );   ///< \ru Конструктор копирования. \en Copy-constructor. 

public:
    /**\ru \name Общие функции геометрического объекта.
       \en \name Common functions of a geometric object.
      \{ */   
    MbeSpaceType  IsA()  const override;
    MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const override;
    bool          IsSame( const MbSpaceItem &, double /*accuracy*/ = LENGTH_EPSILON ) const override;
    bool          SetEqual( const MbSpaceItem & ) override;
    void          Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override;
    void          Move( const MbVector3D &, MbRegTransform * = nullptr ) override;
    void          Rotate( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override;
    double        DistanceToPoint( const MbCartPoint3D & ) const override;
    void          AddYourGabaritTo( MbCube & ) const override;
    void          CalculateMesh( const MbStepData &, const MbFormNote &, MbMesh & ) const override; // \ru Построить полигональную копию mesh. \en Build polygonal copy mesh.

    // \ru Тестовые функции геометрического объекта \en Test functions of a geometric object 
    MbProperty &  CreateProperty( MbePrompt /*n*/ ) const override; // \ru Создать собственное свойство \en Create own property
    void          GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта \en Get properties of the object
    void          SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of object

public:
    /// \ru Инициализировать по центру, точке на окружности, плейсменту. \en Initialize by Initialize by center, point on circle and placement. 
    void          Init( const MbCartPoint3D & center, const MbCartPoint3D & circle, const MbPlacement3D & dimensionPlacement, bool diametral );

    /// \ru Получить точку центра окружности. \en Get center point of circle. 
    MbCartPoint3D GetCenterPoint() const { return center; }
    void          SetCenterPoint( const MbCartPoint3D & val ) { center = val; }

    /// \ru Получить точку на окружности. \en Get point on a circle.
    MbCartPoint3D GetCirclePoint() const { return circle; }
    void          SetCirclePoint( const MbCartPoint3D & val ) { circle = val; }

    /// \ru Получить местная система координат размера. \en Get local coordinate system of the dimension. 
    MbPlacement3D GetPlacement() const { return placement; }
    void          SetPlacement( const MbPlacement3D & val ) { placement = val; }

    /// \ru Получить признак того что размер диаметральный. \en Get sign of the fact that the diametrical dimension.
    bool          IsDiametral() const { return diametral; }
    void          SetDiametral( bool val ) { diametral = val; }

private:  // \ru Не реализованные методы класса \en Non-implemented methods of class 
    void          operator = ( const MbRadialDimension & );        // \ru Не реализовано \en Not implemented 

  DECLARE_PERSISTENT_CLASS(MbRadialDimension)
};


#endif // __MB_DIMENSION_H
