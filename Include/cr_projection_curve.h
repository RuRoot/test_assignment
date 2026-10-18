////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Строитель проволочного каркаса из проекционных кривых.
         \en Projection wireframe constructor.
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_PROJECTION_CURVE_H
#define __CR_PROJECTION_CURVE_H


#include <creator.h>
#include <wire_frame.h>


//------------------------------------------------------------------------------
/** \brief \ru Строитель проволочного каркаса из проекционных кривых.
           \en Projection wireframe constructor. \~
  \details \ru Строитель проволочного каркаса из проекционных кривых.\n
           \en Projection wireframe constructor.\n \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbProjCurveCreator : public MbCreator {
private:
  MbWireFrame *      wireFrame;        // \ru Проецируемый проволочный каркас. \en Wireframe to project.
  RPArray<MbCreator> shellCreators;    // \ru Протокол построения оболочки, на которую выполняется проецирование \en History tree of the shell the projection is performed onto.
  MbVector3D         dir;              // \ru Вектор направления (если нулевой, то проекция по нормали) \en Direction vector (if zero, the normal projection).
  bool               createExact;      // \ru Создавать проекционную кривую при необходимости \en Create the projection curve if necessary.
  bool               truncateByBounds; // \ru Усечь границами \en Truncate by bounds.
  bool               projOnParallel;   // \ru Создавать проекции на поверхностях параллельных вектору проецирования. Такие проекции всегда будут усекаться границами вне зависимости от значения флага truncateByBounds. \en Project onto surfaces parallel to a direction of the vector projection. These projections will be truncated by bounds regardless of truncateByBounds value.
  MbPrecision        precision;        //< \ru Точность построения объекта. \en The precision of object construction.

protected:
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbProjCurveCreator( const MbProjCurveCreator &, MbRegDuplicate * iReg );
private:
  MbProjCurveCreator( const MbProjCurveCreator & ); // \ru Не реализовано \en Not implemented 
  MbProjCurveCreator(); // \ru Не реализовано \en Not implemented 
public:
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  MbProjCurveCreator( const MbCurve3D & curve,
                      const RPArray<MbCreator> & shellCreators, bool sameCreators,
                      const MbVector3D * dir, bool exact, bool truncate,
                      const MbSNameMaker & snMaker );
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  MbProjCurveCreator( const MbWireFrame & wf, const bool sameWire,
                      const RPArray<MbCreator> & shellCreators, bool sameCreators,
                      const MbVector3D * dir, bool exact, bool truncate,
                      const MbSNameMaker & snMaker );
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  MbProjCurveCreator( const MbWireFrame &             wf,
                      const bool                      sameWire,
                      const c3d::CreatorsSPtrVector & shellCreators,
                            bool                      sameCreators,
                      const MbVector3D *              dir,
                            bool                      exact,
                            bool                      truncate,
                            bool                      projOnParallel,
                      const MbPrecision &             prec,
                      const MbSNameMaker &            snMaker );
public:
  virtual ~MbProjCurveCreator();

  // \ru Общие функции строителя. \en The common functions of the creator. 
  MbeCreatorType  IsA()  const override; // \ru Тип элемента \en A type of element 
  MbCreator & Duplicate( MbRegDuplicate * iReg = nullptr ) const override; // \ru Сделать копию \en Create a copy

  bool        IsSame   ( const MbCreator &, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        IsSimilar( const MbCreator & ) const override;            // \ru Являются ли объекты подобными \en Whether the objects are similar 
  bool        SetEqual ( const MbCreator & ) override;                  // \ru Сделать равным \en Make equal 

  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override; // \ru Сдвиг \en Translation
  void        Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate about an axis

  MbePrompt   GetPropertyName() override; // \ru Дать имя свойства объекта \en Get the object property name  
  void        GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of the object 
  void        GetBasisItems ( RPArray<MbSpaceItem> & ) override;  // \ru Дать базовые объекты \en Get the basis objects 
  void        GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.
  size_t      GetCreatorsCount( MbeCreatorType ct ) const override; // \ru Посчитать внутренние построители по типу. \en Count internal creators by type.
  bool        GetInternalCreators( MbeCreatorType, c3d::ConstCreatorsSPtrVector & ) const override; // \ru Получить внутренние построители по типу. \en Get internal creators by type.
  bool        SetInternalCreators( MbeCreatorType, c3d::CreatorsSPtrVector & ) override;            // \ru Получить внутренние построители по типу. \en Get internal creators by type.

  const MbPrecision & GetPrecision() const override { return precision; } // \ru Выдать точность построения. \en Get the precision of object construction.

  // \ru Построить кривую по журналу построения \en Create a curve from the history tree 
  bool        CreateWireFrame( MbWireFrame *&, MbeCopyMode, RPArray<MbSpaceItem> * items = nullptr ) override;

  /** \} */

private:
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation, to prevent an assignment by default. 
  void        operator = ( const MbProjCurveCreator & ); // \ru Не реализовано!!! \en Not implemented!!! 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbProjCurveCreator )
};

IMPL_PERSISTENT_OPS( MbProjCurveCreator )


#endif // __CR_PROJECTION_CURVE_H
