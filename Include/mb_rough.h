////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Условное обозначения шероховатости. Условное обозначение линия-выноска.
         \en Roughness conventional notation. Leader conventional notation. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MB_ROUGH_H
#define __MB_ROUGH_H


#include <topology_item.h>
#include <mb_symbol.h>


class    MbPlacement3D;


//------------------------------------------------------------------------------
/** \brief \ru Обозначение шероховатости.
           \en Roughness notation. \~
  \details \ru Обозначение шероховатости поверхности детали.\n
           \en Roughness notation of detail surface.\n \~
  \ingroup Legend
*/
// ---
class MATH_CLASS MbRough : public MbPointsSymbol 
{
private:
  MbTopologyItem * item; ///< \ru Топологический объект, которому принадлежит шероховатость (не владеет). \en Topological object which roughness belongs to (doesn't own). 

protected:
  /// \ru Конструктор-копия. \en Copy constructor. 
  MbRough( const MbRough & ); 
  /// \ru Умолчательный конструктор. \en Default constructor. 
  MbRough();
public:
  /** \brief \ru Конструктор.
             \en Constructor. \~   
    \details \ru Конструктор условного обозначения на базовых точках.
             \en Constructor of conventional notation on base points. \~
    \param[in] _points    - \ru Базовые точки условного обозначения в мировой системе координат.
                            \en Conventional notation base points in world coordinate system. \~
    \param[in] _name      - \ru Имя условного обозначения объекта.
                            \en Object's conventional notation name. \~
    \param[in] _component - \ru Компонент условного обозначения.
                            \en Component of conventional notation. \~
    \param[in] _item      - \ru Топологический объект, которому принадлежит шероховатость.
                            \en Topological object, which roughness belongs to. \~
    \param[in] _stateCalc - \ru Тип расчета видимости точек.
                            \en Points visibility calculation type. \~
  */
   MbRough( const SArray<MbCartPoint3D> & _points, MbName * _name,
            uint _component, MbTopologyItem * _item, StateCalc _stateCalc = st_strong );
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbRough();

public:
  /** \ru \name Общие функции геометрического объекта.
      \en \name Common functions of a geometric object.
      \{ */
  MbeSpaceType    IsA      () const override;
  MbSpaceItem &   Duplicate( MbRegDuplicate * = nullptr ) const override;
  bool            IsSame   ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const override;
  bool            SetEqual ( const MbSpaceItem & ) override;

  /** \} */
  /** \ru \name Собственные функции шероховатости.
      \en \name Own functions of roughness.
      \{ */
  
          /// \ru Получить топологический объект, которому принадлежит шероховатость. \en Get topological object, which roughness belongs to. 
          MbTopologyItem * GetTopologicItem() const; 
          /// \ru Установить топологический объект, которому принадлежит шероховатость. \en Set topological object, which roughness belongs to. 
          void             SetTopologicItem( MbTopologyItem * _item );
  /** \} */

private:
  MbRough & operator = ( const MbRough & );

  DECLARE_PERSISTENT_CLASS( MbRough )  
};

IMPL_PERSISTENT_OPS( MbRough )

//------------------------------------------------------------------------------
/** \brief \ru Условное обозначение линия-выноска.
           \en Leader conventional notation. \~
  \details \ru Условное обозначение линия-выноска.\n
           \en Leader conventional notation.\n \~
  \ingroup Legend
*/
// ---
class MATH_CLASS MbLeader : public MbSymbol {

protected:
  PArray<MbRough> branches;  ///< \ru Узлы обозначения линия-выноска (не владеет). \en Leader notation nodes (doesn't own). 

protected:
  /// \ru Конструктор-копия. \en Copy constructor. 
  MbLeader( const MbLeader & ); 
  /// \ru Умолчательный конструктор. \en Default constructor. 
  MbLeader();
public:
  /** \brief \ru Конструктор.
             \en Constructor. \~   
    \details \ru Конструктор условного обозначения на базовых точках.
             \en Constructor of conventional notation on base points. \~
    \param[in] _branches  - \ru Узлы обозначения линия-выноска.
                            \en Leader notation nodes. \~
    \param[in] _name      - \ru Имя условного обозначения.
                            \en Conventional notation name. \~
    \param[in] _component - \ru Компонент условного обозначения.
                            \en Component of conventional notation. \~        
    \param[in] _stateCalc - \ru Тип расчета видимости точек.
                            \en Points visibility calculation type. \~
  */
  MbLeader( const PArray<MbRough> & _branches, MbName * _name,
            uint _component, StateCalc _stateCalc = st_strong );
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbLeader();

public:
          /// \ru Инициализация. \en Initialization. 
          void            Init( const MbLeader & );
          
public:

  /** \ru \name Общие функции геометрического объекта
      \en \name Common functions of a geometric object
      \{ */

  MbeSpaceType    IsA          ()                                                      const override;
  MbSpaceItem &   Duplicate    ( MbRegDuplicate * = nullptr                             ) const override;
  bool            IsSame       ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const override;
  bool            SetEqual     ( const MbSpaceItem &                                 ) override;
  void            Transform    ( const MbMatrix3D &, MbRegTransform * = nullptr         ) override;
  void            Move         ( const MbVector3D &, MbRegTransform * = nullptr         ) override;
  void            Rotate       ( const MbAxis3D   &, double, MbRegTransform * = nullptr ) override;

  void            GetProperties( MbProperties &            ) override; 
  void            SetProperties( const MbProperties &            ) override; 
  void            GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void            SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  /** \} */  
  /** \ru \name Функции условного обозначения.
      \en \name Functions of conventional notation.
      \{ */       
 
  /// \ru Включить свой габарит в габаритный куб cube. \en Include own bounding box into 'cube' bounding box. 
  void            IncludeGab        ( MbCube &               ) const override;
  /// \ru Находится ли условное обозначение на плоскости OXY плейсмента place. \en Check if conventional notation on OXY plane of 'place' placement. 
  bool            IsSymbolOnPlace   ( const MbPlacement3D &  ) const override;
  /// \ru Находится ли условное обозначение на или под плоскостью OXY плейсмента place. \en Check if conventional notation on OXY plane of 'place' placement or under it. 
  bool            IsSymbolUnderPlace( const MbPlacement3D &  ) const override;
  /// \ru Находится ли условное обозначение на или внутри оболочки faceShell. \en Check if conventional notation on 'faceShell' shell or inside it. 
  bool            IsSymbolInShell   ( const MbFaceShell &    ) const override;
  /// \ru Получить массив условных обозначений на базовых точках, принадлежащих данному обозначению. \en Get an array of notation conventions on the base points which belong to this notation. 
  void            GetPointsSymbols  ( RPArray<MbSymbol> & ) const override;      

  /** \} */  
  /** \ru \name Собственные функции обозначения линия-выноска.
      \en \name Own functions of leader notation.
      \{ */   
  
  /// \ru Получить количество узлов обозначения линия-выноска. \en Get count of leader notation nodes. 
          ptrdiff_t       GetBranchesCount  ()              const;    

  /** \brief \ru Получить узел.
             \en Get node. \~   
    \details \ru Получить узел обозначения по индексу.
             \en Get notation node by index. \~
    \param[in] ind - \ru Индекс узла.
                     \en Node index. \~  
    \return \ru Узел по указанному индексу,\n
      в случае некорректного индекса - последний узел из списка.
            \en Node by specified index,\ n
      in case of incorrect index - the last node from list. \~
  */
  const   MbRough &       GetBranch( size_t ind ) const;       

  /** \brief \ru Установить узел.
             \en Set node. \~   
    \details \ru Установить узел обозначения с указанным индексом.
             \en Set notation node by given index. \~
    \param[in] rough - \ru Новый узел.
                       \en New node. \~
    \param[in] ind   - \ru Индекс узла.
                       \en Node index. \~      
  */
          void            SetBranch( const MbRough & rough, size_t ind );
  /** \} */
private:
                // \ru Не реализованные методы класса. \en Not implemented class methods. 
          void            operator = ( const MbLeader & );

  DECLARE_PERSISTENT_CLASS( MbLeader )
};

IMPL_PERSISTENT_OPS( MbLeader )


#endif // __MB_ROUGH_H
