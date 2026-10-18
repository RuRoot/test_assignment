////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Строитель оболочки тела выдавливания.
         \en Constructor of an extrusion solid's shell. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_EXTRUSION_SOLID_H
#define __CR_EXTRUSION_SOLID_H


#include <cr_swept_solid.h>


class    MbRect;


//------------------------------------------------------------------------------
/** \brief \ru Строитель оболочки тела выдавливания.
           \en Constructor of an extrusion solid's shell. \~
  \details \ru Строитель оболочки тела путём движения образующих кривых вдоль заданного вектора на заданное расстояние. \n
           \en Constructor of a solid's shell by moving generating curves along the given vector at the given distance. \n \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbCurveExtrusionSolid : public MbCurveSweptSolid {
protected:
  MbSweptData     sweptData;  ///< \ru Данные об образующей. \en Generating curve data. 
  MbVector3D      direction;  ///< \ru Направление выдавливания. \en Extrusion direction. 
  ExtrusionValues parameters; ///< \ru Параметры. \en Parameters. 

public :
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор.
             \en Constructor. \~
    \param[in] sweptData     - \ru Данные об образующей.
                               \en The generating curve data. \~
    \param[in] direction     - \ru Направление выдавливания.
                               \en An extrusion direction. \~
    \param[in] parameters    - \ru Параметры выдавливания.
                               \en The extrusion parameters. \~
    \param[in] oType         - \ru Тип булевой операции.
                               \en A Boolean operation type. \~
    \param[in] operNames     - \ru Именователь операции.
                               \en An object defining names generation in the operation. \~
    \param[in] contoursNames - \ru Именователь контуров для именования граней.
                               \en An object defining contours' names for faces naming. \~
    \param[in] creators      - \ru Построители тела, используемого в опции "До ближайшего объекта".
                               \en Creators of a solid used with option "To the nearest object (solid)". \~
    \param[in] sameCreators  - \ru Признак использования оригиналов построителей.
                               \en Flag of using the original creators. \~
  */
  MbCurveExtrusionSolid( const MbSweptData &             sweptData,
                         const MbVector3D &              direction,
                         const ExtrusionValues &         parameters, 
                               OperationType             oType, 
                         const MbSNameMaker &            operNames, 
                         const RPArray<MbSNameMaker> &   contoursNames,
                         const c3d::CreatorsSPtrVector * creators = nullptr,
                               bool                      sameCreators = true );

private :
  MbCurveExtrusionSolid( const MbCurveExtrusionSolid & init, MbRegDuplicate * ireg );
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbCurveExtrusionSolid( const MbCurveExtrusionSolid & ); 
public :
  virtual ~MbCurveExtrusionSolid();

  /** \ru \name Общие функции математического объекта.
      \en \name Common functions of the mathematical object.
      \{ */
  MbeCreatorType  IsA() const override; // \ru Тип элемента. \en A type of element. 
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию. \en Make a copy.
  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override;          // \ru Преобразовать элемент согласно матрице. \en Transform element according to the matrix.
  void        Move( const MbVector3D &, MbRegTransform * = nullptr ) override;                 // \ru Сдвиг. \en Move.
  void        Rotate( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси. \en Rotate around an axis.

  void        GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  void        SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта. \en Set properties of the object. 
  MbePrompt   GetPropertyName() override; // \ru Выдать заголовок свойства объекта. \en Get a name of object property. 
  void        GetBasisItems ( RPArray<MbSpaceItem> & ) override; // \ru Дать базовые объекты. \en Get the base objects. 
  void        GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  bool        IsSame( const MbCreator &, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        IsSimilar( const MbCreator & ) const override; // \ru Являются ли объекты подобными. \en Determine whether the objects are similar. 
  bool        SetEqual ( const MbCreator & ) override;       // \ru Сделать равным. \en Make equal. 
  
  /** \} */
  /** \ru \name Общие функции твердого тела (формообразующей операции).
      \en \name Common functions of the rigid solid (forming operations).
      \{ */
  bool        CreateShell( MbFaceShell *& shell, MbeCopyMode sameShell, 
                           RPArray<MbSpaceItem> * items = nullptr ) override; // \ru Построение. \en Construction.

  MbFaceShell * InitShell( bool in ) override; 
  void        InitBasis( RPArray<MbSpaceItem> & items ) override; 
  bool        GetPlacement( MbPlacement3D & ) const override;
  /** \} */
  /** \ru \name Функции строителя оболочки тела выдавливания.
      \en \name Functions of an extrusion solid's shell creator.
      \{ */
  /// \ru Поверхность двумерных контуров. \en A surface of two-dimensional contours. 
  const MbSurface *   GetSurface() const { return sweptData.GetSurface(); }
  /// \ru Направление выдавливания. \en An extrusion direction. 
  const MbVector3D &  GetDirection() const { return direction; }

  /// \ru Дать параметры. \en Get the parameters. 
  void        GetParameters(       ExtrusionValues & params ) const { params = parameters; }
  /// \ru Установить параметры. \en Set the parameters. 
  void        SetParameters( const ExtrusionValues & params )       { parameters = params; }
  /// \ru Дать габарит контуров на плейсменте. \en Get bounding boxes of contours in the placement. 
  void        AddPlacementRect( MbRect & r ) const;
  /** \} */

private :
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
  void        operator = ( const MbCurveExtrusionSolid & ); // \ru НЕЛЬЗЯ!!! \en NOT ALLOWED!!! 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbCurveExtrusionSolid )
}; // MbCurveExtrusionSolid

IMPL_PERSISTENT_OPS( MbCurveExtrusionSolid )


//------------------------------------------------------------------------------
/** \brief \ru Создать оболочку тела выдавливания.
           \en Create an extrusion solid's shell. \~
  \details \ru Построить оболочку тела путём движения образующих кривых вдоль заданного вектора на заданное расстояние
    и выполнить булеву операцию с оболочкой, если последняя задана. \n
    Одновременно с построением оболочки функция создаёт её строитель.\n
           \en Create a boy's shell by moving generating curves along the given vector at the given distance
    and perform the Boolean operation with the shell if it is specified. \n
    The function simultaneously creates the shell and its constructor.\n \~ 
  \param[in]       solid          - \ru Набор граней, к которым дополняется построение.
                                    \en Face set the construction is complemented with respect to. \~
  \param[in]       sameShell      - \ru Способ копирования граней.
                                    \en The method of copying faces. \~
  \param[in]       solidCreators  - \ru Строители тела solid.
                                    \en Creators of the solid. \~
  \param[in]       sweptData      - \ru Данные об образующей.
                                    \en The generating curve data. \~
  \param[in]       direction      - \ru Направление выдавливания
                                    \en Extrusion direction. \~
  \param[in, out]  params         - \ru Параметры выдавливания.
    Возвращают информацию для построения элементов массива операций до поверхности.
                                    \en The extrusion parameters.
    Returns the information for construction of the up-to-surface operation array elements. \~
  \param[in]       oType          - \ru Тип операции дополнения построения.
                                    \en Type of operation of construction complement. \~
  \param[in]       operNames      - \ru Именователь операции.
                                    \en An object defining names generation in the operation. \~
  \param[in]       contoursNames  - \ru Именователь контуров.
                                    \en An object defining the names of contours. \~
  \param[out]      resType        - \ru Код результата операции выдавливания.
                                    \en The extrusion operation result code. \~
  \param[out]      shell          - \ru Построенный набор граней.
                                    \en Constructed set of faces. \~  
  \result \ru Возвращает строитель.
          \en Returns the constructor. \~
  \ingroup Model_Creators
*/
// ---
MATH_FUNC (MbCreator *) CreateCurveExtrusion(       MbFaceShell *             solid, 
                                                    MbeCopyMode               sameShell,
                                              const c3d::CreatorsSPtrVector * solidCreators,
                                              const MbSweptData &             sweptData,
                                              const MbVector3D &              direction,
                                              const ExtrusionValues &         params, 
                                                    OperationType             oType, 
                                              const MbSNameMaker &            operNames, 
                                              const RPArray<MbSNameMaker> &   contoursNames, 
                                                    MbResultType &            resType, 
                                                    MbFaceShell *&            shell );


#endif // __CR_EXTRUSION_SOLID_H
