////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Строитель оболочки тела формообразующей операции.
         \en Constructor of a solid's shell of forming operation. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_SWEPT_SOLID_H
#define __CR_SWEPT_SOLID_H


#include <op_swept_parameter.h>
#include <creator.h>


//------------------------------------------------------------------------------
/** \brief \ru Строитель оболочки тела по формообразующим кривым.
           \en Constructor a solid's shell by forming curves. \~
  \details \ru Строитель оболочки тела, заданного формообразующими кривыми. \n
           \en Constructor a shell of a solid specified by forming curves. \n \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbCurveSweptSolid : public MbCreator {
protected :
  RPArray<MbSNameMaker> faceNames; ///< \ru Именователи граней. \en An object for naming faces. 
  RPArray<MbCreator>    creators;  ///< \ru Построители тела, используемого в опции "До ближайшего объекта". \en Creators of a solid used with option "To the nearest object (solid)". 
  OperationType         operation; ///< \ru Тип булевой операции над оболочками. \en Type of Boolean operation on shells. 
  double                buildSag;  ///< \ru Угловое отклонение при движении по кривым и поверхностям. \en Angular deviation while moving along curves and surfaces. 

protected :

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор.
             \en Constructor. \~
    \param[in] operNames - \ru Именователь операции.
                           \en An object defining names generation in the operation. \~
    \param[in] fNames    - \ru Именователь граней.
                           \en An object for naming faces. \~
    \param[in] oType     - \ru Тип булевой операции.
                           \en A Boolean operation type. \~
    \param[in] creators  - \ru Построители тела, используемого в опции "До ближайшего объекта".
                           \en Creators of a solid used with option "To the nearest object (solid)". \~
    \param[in] sameCreators  - \ru Признак использования оригиналов построителей.
                               \en Flag of using the original creators. \~
  */
  MbCurveSweptSolid( const MbSNameMaker &            operNames, 
                     const RPArray<MbSNameMaker> &   fNames, 
                           OperationType             oType,
                     const c3d::CreatorsSPtrVector * creators,
                           bool                      sameCreators = false );

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор.
             \en Constructor. \~
    \param[in] operNames - \ru Именователь операции.
                           \en An object defining names generation in the operation. \~
    \param[in] fNames    - \ru Именователи граней.
                           \en An object for naming faces. \~
    \param[in] oType     - \ru Тип булевой операции.
                           \en A Boolean operation type. \~
  */
  MbCurveSweptSolid( const MbSNameMaker & operNames, 
                     const MbSNameMaker & fNames, 
                           OperationType  oType );

  /// \ru Конструктор копии. \en Copy-constructor. 
  MbCurveSweptSolid( const MbCurveSweptSolid & init, MbRegDuplicate * );
public :
  virtual ~MbCurveSweptSolid();

  /** \ru \name Общие функции математического объекта.
      \en \name Common functions of the mathematical object.
      \{ */
  //virtual MbeCreatorType  IsA()  const = 0; // \ru Тип элемента \en A type of element
  MbeCreatorType  Type() const override;     // \ru Тип элемента \en A type of element 
  //virtual MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const = 0; // \ru Сделать копию \en Create a copy
  //virtual void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) = 0;             // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  //virtual void        Move     ( const MbVector3D &, MbRegTransform * = nullptr ) = 0;             // \ru Сдвиг \en Translation
  //virtual void        Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) = 0; // \ru Повернуть вокруг оси \en Rotate around an axis

  //virtual bool        IsSame( const MbCreator &, double accuracy ) const = 0; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  //virtual bool        IsSimilar( const MbCreator & ) const = 0;   // \ru Являются ли объекты подобными. \en Whether the objects are similar
  //virtual bool        SetEqual ( const MbCreator & ) = 0; // \ru Сделать равным \en Make equal

  //virtual void        GetProperties( MbProperties & ) = 0; // \ru Выдать свойства объекта \en Get properties of the object
  //virtual void        SetProperties( const MbProperties & ) = 0; // \ru Записать свойства объекта \en Set properties of the object
  //virtual MbePrompt   GetPropertyName() = 0; // \ru Выдать заголовок свойства объекта \en Get a name of object property                                              
  size_t      GetCreatorsCount( MbeCreatorType ct ) const override; // \ru Посчитать внутренние построители по типу. \en Count internal creators by type.
  bool        GetInternalCreators( MbeCreatorType, c3d::ConstCreatorsSPtrVector & ) const override; // \ru Получить внутренние построители по типу. \en Get internal creators by type.
  bool        SetInternalCreators( MbeCreatorType, c3d::CreatorsSPtrVector & ) override;            // \ru Получить внутренние построители по типу. \en Get internal creators by type.

  /** \} */
  /** \ru \name Общие функции твердого тела (формообразующей операции).
      \en \name Common functions of the rigid solid (forming operations).
      \{ */
  bool        CreateShell( MbFaceShell *& shell, MbeCopyMode sameShell, 
                           RPArray<MbSpaceItem> * items = nullptr ) override; // \ru Построение \en Construction

  virtual MbFaceShell * InitShell( bool in ) = 0; 
  virtual void        InitBasis( RPArray<MbSpaceItem> & ) = 0; 
  virtual bool        GetPlacement( MbPlacement3D & ) const = 0;
  void        SetYourVersion( VERSION version, bool forAll ) override;
          void        SetOperation( OperationType op ) { operation = op; }
  /** \} */
protected :
  /// \ru Удалить строители ближайшего тела. \en Delete internal creators.
  void        DeleteCreators();
private:
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
  void        operator = ( const MbCurveSweptSolid & ); 

  DECLARE_PERSISTENT_CLASS( MbCurveSweptSolid )
}; // MbCurveSweptSolid

IMPL_PERSISTENT_OPS( MbCurveSweptSolid )


#endif // __CR_SWEPT_SOLID_H
