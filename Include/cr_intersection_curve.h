////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Строитель кривой пересечения.
         \en Intersection curve constructor.
*/
////////////////////////////////////////////////////////////////////////////////
  
#ifndef __CR_INTERSECTION_CURVE_H
#define __CR_INTERSECTION_CURVE_H


#include <creator.h>


//------------------------------------------------------------------------------
/** \brief \ru Строитель кривой пересечения.
           \en Intersection curve constructor. \~
  \details \ru Строитель кривой пересечения.\n
           \en Intersection curve constructor.\n \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbIntCurveCreator : public MbCreator {
private:
  RPArray<MbCreator> creators1;    ///< \ru Журнал построения первой оболочки. \en The first shell history tree. 
  RPArray<MbCreator> creators2;    ///< \ru Журнал построения второй оболочки. \en The second shell history tree. 
  bool               mergeCurves;  ///< \ru Объединять кривые, разрезанные швом. \en Merge curves cut by a surface seam.
  bool               cutCurves;    ///< \ru Разрезать кривые в точках пересечения. \en Cut curves at intersection points.

protected:
  /// \ru Конструктор копирования. \en Copy-constructor.
  MbIntCurveCreator( const MbIntCurveCreator &, MbRegDuplicate * iReg );
private:
  MbIntCurveCreator( const MbIntCurveCreator & ); // \ru Не реализовано \en Not implemented 
  MbIntCurveCreator(); // \ru Не реализовано \en Not implemented 
public:
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  MbIntCurveCreator( const RPArray<MbCreator> & creators1, bool same1,
                     const RPArray<MbCreator> & creators2, bool same2,
                     bool mergeCurves, bool curCurves,
                     const MbSNameMaker & snMaker );
public:
  virtual ~MbIntCurveCreator();

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

  // \ru Построить кривую по журналу построения \en Create a curve from the history tree 
  bool        CreateWireFrame( MbWireFrame *&, MbeCopyMode, RPArray<MbSpaceItem> * items = nullptr ) override;

  /** \} */

private:
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation, to prevent an assignment by default. 
  void        operator = ( const MbIntCurveCreator & ); // \ru Не реализовано!!! \en Not implemented!!! 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbIntCurveCreator )
}; // MbIntCurveCreator

IMPL_PERSISTENT_OPS( MbIntCurveCreator )


#endif // __CR_INTERSECTION_CURVE_H
