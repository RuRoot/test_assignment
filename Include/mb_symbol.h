////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Определение классов условных обозначений.
         \en Conventional notation classes definition. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MB_SYMBOL_H
#define __MB_SYMBOL_H


#include <legend.h>
#include <templ_pointer.h>


class    MbPlacement3D;
class    MbName;
class    MbFaceShell;


//------------------------------------------------------------------------------
/** \brief \ru Условное обозначение.
           \en Conventional notation. \~
  \details \ru Абстрактный базовый класс условного обозначения MbSymbol. \n
           \en Abstract base class of conventional notation MbSymbol. \n \~
  \ingroup Legend
*/
/*\ru Иерархия классов условных обозначений

              -MbSymbol (абстрактный базовый класс)
               тип расчета видимости
               имя
               компонент
               /                           \
              /                             \
             /                               \
          MbLeader (линия-выноска)        MbPointsSymbol (обозначение на базовых точках)
          + массив MbRough'ов             + массив MbCartPoint3D
                                                   /
                                                  /
                                                 /
                                           MbRough (обозначение шероховатости)
                                           + MbTopologyItem

  Пока для условных обозначений мы только определяем видимость при проецировании.
  Фактически это сводится к определению видимости MbPointsSymbol, а это в свою
  очередь - к определению видимости точки со следующими условиями:
  - для MbPointsSymbol точку не могут закрывать ребра
  - для MbRough точку закрывают все ребра, кроме тех, которые принадлежат MbTopologyItem
  \en Conventional notation classes hierarchy

              -MbSymbol (abstract base class)
               visibility calculation type
               name
               component
               /                           \
              /                             \
             /                               \
          MbLeader (leader)               MbPointsSymbol (notation on base points)
          + array of MbRough's               + array of MbCartPoint3D's
                                                   /
                                                  /
                                                 /
                                           MbRough (roughness notation)
                                           + MbTopologyItem

  Yet for conventional notations we only define visibility at projection.
  Actually it is reduced to determination of MbPointsSymbol visibility, so it, in
  turn, to definition of point visibility with following conditions:
  - in case of MbPointsSymbol edges can not hide a point
  - in case of MbRough point is hid by all edges except that belongs to MbTopologyItem \~
*/
// ---
class MATH_CLASS MbSymbol : public MbLegend {       

public:       
  /** \brief \ru Тип расчета видимости.
             \en Visibility calculation type. \~
    \details \ru Тип расчета видимости.
             \en Visibility calculation type. \~       
  */
  enum StateCalc {
    st_strong = 0,        ///< \ru Точно, все точки должны быть видимы. \en Strong, all points have to be visible. 
    st_loose  = 1,        ///< \ru Не строго, хотя бы одна точка должна быть видима. \en Not strong, at least one point has to be visible. 
  };                                            
                                                 
protected:                                       
  StateCalc           stateCalc; ///< \ru Тип расчета видимости. \en Visibility calculation type. 
  TOwnPointer<MbName> name;      ///< \ru Имя обозначения \en Notation name 
  uint                compHash;  ///< \ru Компонент условного обозначения (не владеет). \en Component of conventional notation (doesn't own). 
  size_t              ident;     ///< \ru Идентификатор нити. \en Thread identifier. 
                       
protected:
  /// \ru Умолчательный конструктор. \en Default constructor. 
  MbSymbol();
   
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор условного обозначения. \n
             \en Constructor of conventional notation. \n \~
    \param[in] _name      - \ru Имя обозначения.
                            \en Notation name. \~
    \param[in] _component - \ru Компонент условного обозначения
                            \en Component of conventional notation \~
    \param[in] _ident     - \ru Идентификатор нити.
                            \en A thread identifier. \~
    \param[in] _stateCalc - \ru Тип расчета видимости.
                            \en Visibility calculation type. \~
  */
  MbSymbol( MbName * _name, uint _component, StateCalc _stateCalc = st_strong );

  /// \ru Конструктор-копия. \en Copy constructor. 
  MbSymbol( const MbSymbol & ); 

public:
  virtual ~MbSymbol();

public:
  /** \ru \name Общие функции геометрического объекта.
      \en \name Common functions of a geometric object.
      \{ */
  //virtual MbeSpaceType    IsA() const = 0;
  MbeSpaceType    Type() const override;
  //virtual MbSpaceItem &   Duplicate        ( MbRegDuplicate * = nullptr                             ) const = 0;
  //virtual bool            IsSame           ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const = 0;
  bool            IsSimilar        ( const MbSpaceItem &                                 ) const override;
  //virtual bool            SetEqual         ( const MbSpaceItem &                                 )       = 0;
  //virtual void            Transform        ( const MbMatrix3D &, MbRegTransform * = nullptr         )       = 0;
  //virtual void            Move             ( const MbVector3D &, MbRegTransform * = nullptr         )       = 0;
  //virtual void            Rotate           ( const MbAxis3D   &, double, MbRegTransform * = nullptr )       = 0;
  double          DistanceToPoint  ( const MbCartPoint3D &                               ) const override;
  void            AddYourGabaritTo ( MbCube &                                            ) const  override{};
  void            CalculateMesh    ( const MbStepData & stepData, const MbFormNote & note, MbMesh & mesh ) const override;
 
  MbProperty &    CreateProperty   ( MbePrompt name ) const override;         // \ru Создать собственное свойство. \en Create own property. 
  //virtual void            GetProperties    ( MbProperties & ) = 0;           // \ru Выдать свойства объекта. \en Get properties of the object.
  //virtual void            SetProperties    ( const MbProperties & ) = 0;           // \ru Записать свойства объекта. \en Set properties of the object.
  //virtual void            GetBasisPoints( MbControlData3D & ) const = 0; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  //virtual void            SetBasisPoints( const MbControlData3D & ) = 0; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  /** \} */  
  /** \ru \name Собственные функции условного обозначения.
      \en \name Own functions of conventional notation.
      \{ */
  // \ru Функции, относящиеся к расчету видимости обозначения stateCalc \en Functions related to 'stateCalc' calculation of notation visibility 
          /// \ru Получить тип расчета видимости. \en Get visibility calculation type. 
          StateCalc       GetStateCalc      () const;
          /// \ru Точный ли тип расчета видимости. \en Check if visibility calculation type is strong. 
          bool            IsStrongStateCalc () const;
          /// \ru Установить тип расчета видимости. \en Set visibility calculation type. 
          void            SetStateCalc      ( StateCalc _stateCalc );

  // \ru Функции, относящиеся к имени name \en Functions related to 'name' name                          
          /// \ru Есть ли непустое имя. \en Is there a non-empty name. 
          bool            IsName            () const;
          /// \ru Получить имя обозначения. \en Get notation name. 
          MbName *        GetName           () const;
          /// \ru Установить имя обозначения. \en Set notation name. 
          void            SetName           ( MbName & );

  // \ru Функции, относящиеся к компоненту обозначения component \en Functions related to 'component' component of notation 
          /// \ru Получить компонент обозначения. \en Get notation component. 
          uint            GetComponent      () const;
          /// \ru Установить компонент обозначения. \en Set notation component. 
          void            SetComponent      ( uint );

  // \ru Функции, относящиеся к идентификатору нити ident \en Functions related to 'ident' thread identifier 
          /// \ru Получить идентификатор нити. \en Get the thread identifier. 
          size_t          GetIdentifier     () const;
          /// \ru Установить идентификатор нити. \en Set thread identifier. 
          void            SetIdentifier     ( size_t );

  // \ru Виртуальные функции, различающиеся у различных условных обозначений \en Virtual functions, that differs in different notations 
  /// \ru Включить свой габарит в габаритный куб cube. \en Include own bounding box into 'cube' bounding box. 
  virtual void            IncludeGab        ( MbCube &              ) const = 0;
  /// \ru Находится ли условное обозначение на плоскости OXY плейсмента. \en Check if conventional notation on the OXY plane of placement. 
  virtual bool            IsSymbolOnPlace   ( const MbPlacement3D & ) const = 0;
  /// \ru Находится ли условное обозначение на или под плоскостью OXY плейсмента. \en Check if conventional notation on the OXY plane of placement or under it. 
  virtual bool            IsSymbolUnderPlace( const MbPlacement3D & ) const = 0;
  /// \ru Находится ли обозначение внутри или на оболочке. \en Check if notation on shell or inside it. 
  virtual bool            IsSymbolInShell   ( const MbFaceShell &   ) const = 0;
  /// \ru Получить массив условных обозначений на базовых точках, принадлежащих данному обозначению. \en Get an array of notation conventions on the base points which belong to this notation. 
  virtual void            GetPointsSymbols  ( RPArray<MbSymbol> &   ) const = 0;
  /** \} */

private:
  MbSymbol & operator = ( const MbSymbol & );
  DECLARE_PERSISTENT_CLASS( MbSymbol )
};

IMPL_PERSISTENT_OPS( MbSymbol )

//------------------------------------------------------------------------------
/** \brief \ru Условное обозначение на базовых точках.
           \en Conventional notation on base points. \~
  \details \ru Условное обозначение на базовых точках. \n
           \en Conventional notation on base points. \n \~
  \ingroup Legend
*/
// ---
class MATH_CLASS MbPointsSymbol : public MbSymbol {

protected:
  SArray<MbCartPoint3D> points;  ///< \ru Базовые точки обозначения в мировой системе координат. \en Notation base points in world coordinate system. 

private:
  SArray<size_t> *      steps;   ///< \ru Данные об участках сложных разрезов. \en Data about the portions of complex sections.

protected:
  MbPointsSymbol( const MbPointsSymbol & ); ///< \ru Конструктор-копия. \en Copy-constructor. 
  MbPointsSymbol();                         ///< \ru Умолчательный конструктор. \en Default constructor. 

public:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор условного обозначения на базовых точках. \n
             \en Constructor of conventional notation on base points. \n \~
    \param[in] _points    - \ru Набор точек.
                            \en Set of points. \~ 
    \param[in] _name      - \ru Имя обозначения.
                            \en Notation name. \~
    \param[in] _component - \ru Компонент условного обозначения.
                            \en Component of conventional notation. \~  
    \param[in] _stateCalc - \ru Тип расчета видимости.
                            \en Visibility calculation type. \~
  */
  MbPointsSymbol( const SArray<MbCartPoint3D> & _points, MbName * _name,
                  uint _component, StateCalc _stateCalc = st_strong );
  /// \ru Деструктор. \en Destructor. 
  virtual     ~MbPointsSymbol();

public:

          void            Init( const MbPointsSymbol & ); ///< \ru Инициализация. \en Initialization.
          
  /** \ru \name Общие функции геометрического объекта.
      \en \name Common functions of a geometric object.
      \{ */
  MbeSpaceType    IsA          ()                                                      const override;
  MbSpaceItem &   Duplicate    ( MbRegDuplicate * = nullptr                             ) const override;
  bool            IsSame       ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const override;
  bool            SetEqual     ( const MbSpaceItem &                                 ) override;
  void            Transform    ( const MbMatrix3D &, MbRegTransform * = nullptr         ) override;
  void            Move         ( const MbVector3D &, MbRegTransform * = nullptr         ) override;
  void            Rotate       ( const MbAxis3D   &, double, MbRegTransform * = nullptr ) override;
                                              
  void            GetProperties( MbProperties &            ) override; // \ru Выдать свойства объекта. \en Get properties of the object.
  void            SetProperties( const MbProperties &            ) override; // \ru Записать свойства объекта. \en Set properties of the object.
  void            GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void            SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  /** \} */  
  /** \ru \name Функции условного обозначения.
      \en \name Functions of conventional notation.
      \{ */

  // \ru Виртуальные функции, различающиеся у различных условных обозначений \en Virtual functions that differs in different notations 
  // \ru Включить свой габарит в габаритный куб cube. \en Include own bounding box into 'cube' bounding box. 
  void            IncludeGab        ( MbCube &               ) const override;
  // \ru Находится ли условное обозначение на плоскости OXY плейсмента place. \en Check if conventional notation on OXY plane of 'place' placement. 
  bool            IsSymbolOnPlace   ( const MbPlacement3D &  ) const override;
  // \ru Находится ли условное обозначение на или под плоскостью OXY плейсмента place. \en Check if conventional notation on OXY plane of 'place' placement or under it. 
  bool            IsSymbolUnderPlace( const MbPlacement3D &  ) const override;
  // \ru Находится ли условное обозначение на или внутри оболочки faceShell. \en Check if conventional notation on 'faceShell' shell or inside it. 
  bool            IsSymbolInShell   ( const MbFaceShell &    ) const override;
  // \ru Получить массив условных обозначений на базовых точках, принадлежащих данному обозначению. \en Get an array of notation conventions on the base points which belong to this notation. 
  void            GetPointsSymbols  ( RPArray<MbSymbol> & ) const override;

  /** \} */  
  /** \ru \name Собственные функции обозначения на базовых точках.
      \en \name Own functions of conventional notation on base points.
      \{ */

  // \ru Функции, относящиеся к базовым точкам обозначения points \en Functions related to 'points' notation base points 
          /// \ru Получить базовые точки обозначения. \en Get notation base points. 
          const SArray<MbCartPoint3D> & GetPoints () const;
          /// \ru Получить количество базовых точек обозначения. \en Get count of notation base points. 
          ptrdiff_t       GetPointsCount() const; 
          /// \ru Получить базовую точку обозначения c индексом ind. \en Get notation base point by 'ind' index. 
          const MbCartPoint3D & GetPoint( size_t ind ) const;    
          /// \ru Установить базовую точку обозначения с указанным индексом. \en Set notation base point by given index. 
          void            SetPoint( const MbCartPoint3D &, size_t ); 

          /// \ru Принадлежит ли условное обозначение участку сложного разреза с указанным номером? \en Is symbol belong portion of complex section with the specified number?
          bool            StepFound( size_t mapper ) const;
          /// \ru Запомнить номер участка сложного разреза, которому принадлежит условное обозначение. \en Remember portion of complex section, which owns the symbol.
          void            SetStepIndex( size_t mapper );
          /// \ru Очистить данные об участках сложных разрезов. \en Clear data about the portions of complex sections.
          void            ClearSteps(); 
  /** \} */

private:
          // \ru Не реализованные методы класса. \en Not implemented class methods.
          void            operator = ( const MbPointsSymbol & );

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbPointsSymbol )
};

IMPL_PERSISTENT_OPS( MbPointsSymbol )

#endif // __MB_SYMBOL_H
