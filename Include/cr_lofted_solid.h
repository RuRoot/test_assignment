////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Строитель оболочки тела по плоским сечениям.
         \en Constructor of a lofted shell. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_LOFTED_SOLID_H
#define __CR_LOFTED_SOLID_H


#include <cur_contour_on_plane.h>
#include <cr_swept_solid.h>
#include <templ_sptr.h>

class  MbLoftedSolidParams;

//------------------------------------------------------------------------------
/** \brief \ru Строитель оболочки тела по сечениям.
           \en Constructor of a lofted shell. \~
  \details \ru Строитель оболочки тела, проходящей по заданным сечениям и вдоль заданной осевой линии и направляющих. \n
           \en Constructor of solid's shell passing through the given sections along the specified spine curve and guide curves. \n \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbCurveLoftedSolid : public MbCurveSweptSolid {
protected :
  RPArray<MbContourOnSurface> curves;      ///< \ru Плоские сечения. \en Plane sections.
  SPtr<MbCurve3D>             spine;       ///< \ru Осевая линия (может отсутствовать). \en Spine curve (can be absent).
  LoftedValues                parameters;  ///< \ru Параметры. \en Parameters.
  RPArray<MbCurve3D> *        guideCurves; ///< \ru Массив направляющих кривых (может быть nullptr). \en An array of guide curves (can be nullptr).
  SArray<MbCartPoint3D> *     userPnts;    ///< \ru Пользовательские точки на сечениях. \en Custom points on the sections.

public:
  /// \ru Конструктор. \en Constructor. 
  MbCurveLoftedSolid( const RPArray<MbSurface> &    surfs, 
                      const RPArray<MbContour> &    cntrs,
                      const LoftedValues &          p,
                            OperationType           op,
                      const MbSNameMaker &          n,
                            RPArray<MbSNameMaker> & ns,
                            RPArray<MbCurve3D> *    guideCrvs,
                            SArray<MbCartPoint3D> * userPnts );
  /// \ru Конструктор. \en Constructor. 
  MbCurveLoftedSolid( const MbCurve3D &             s,
                      const RPArray<MbSurface> &    surfs, 
                      const RPArray<MbContour> &    cntrs,
                      const LoftedValues &          p,
                            OperationType           op,
                      const MbSNameMaker &          n,
                            RPArray<MbSNameMaker> & ns,
                            RPArray<MbCurve3D> *    guideCrvs,
                            SArray<MbCartPoint3D> * userPnts );

private :
  MbCurveLoftedSolid( const MbCurveLoftedSolid & init, MbRegDuplicate * ireg );
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbCurveLoftedSolid( const MbCurveLoftedSolid & init );

public :
  virtual ~MbCurveLoftedSolid();

  // \ru Общие функции математического объекта \en Common functions of the mathematical object 

  MbeCreatorType  IsA() const override; // \ru Тип элемента \en A type of element 
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию \en Create a copy
  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override;             // \ru Преобразовать \en Transform
  void        Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override;             // \ru Сдвиг \en Translation
  void        Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate around an axis

  void        GetProperties( MbProperties & ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & ) override; // \ru Записать свойства объекта \en Set properties of the object 
  MbePrompt   GetPropertyName() override; // \ru Выдать заголовок свойства объекта \en Get a name of object property 
  void        GetBasisItems ( RPArray<MbSpaceItem> & ) override; // \ru Дать базовые объекты \en Get the base objects 
  void        GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  bool        IsSame( const MbCreator &, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        IsSimilar( const MbCreator & ) const override; // \ru Являются ли объекты подобными \en Whether the objects are similar 
  bool        SetEqual ( const MbCreator & ) override;       // \ru Сделать равным \en Make equal 

  // \ru Общие функции твердого тела \en Common functions of solid  

  MbFaceShell * InitShell( bool /*in*/ ) override;
  void        InitBasis( RPArray<MbSpaceItem> & items ) override;
  bool        GetPlacement( MbPlacement3D & ) const override;

  /// \ru Дать параметры. \en Get the parameters. 
  void        GetParameters(       LoftedValues & params ) const { params = parameters; }
  /// \ru Установить параметры. \en Set the parameters. 
  void        SetParameters( const LoftedValues & params )       { parameters = params; }
  /// \ru Направляющая кривая. \en The spine curve. 
  const MbCurve3D * GetSpine() const { return spine.get(); }

private :
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
  void        operator = ( const MbCurveLoftedSolid & ); 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbCurveLoftedSolid )
}; // MbCurveLoftedSolid

IMPL_PERSISTENT_OPS( MbCurveLoftedSolid )


//------------------------------------------------------------------------------
/** \brief \ru Создать тело по плоским сечениям.
           \en Create a solid from a planar sections. \~
  \details \ru Построить оболочку тела, проходящую по заданным сечениям
               и выполнить Булеву операцию с оболочкой, если последняя задана. \n
               Одновременно с построением оболочки функция создаёт её строитель.\n
           \en Create a solid's shell passing through the given sections
               and perform the Boolean operation with the shell if it is specified. \n
               The function simultaneously creates the shell and its constructor.\n \~ 
  \param[in] srcSolid - \ru Первое тело для булевой операции.
                        \en The first solid for a boolean operation. \~
  \param[in] copyMode - \ru Режим копирования тела.
                        \en Whether to copy the solid. \~
  \param[in] loftSolParams - \ru Исходные данные для выполнения операции.
                             \en The source data to make the operation. \~
  \param[out] res - \ru Код результата операции.
                    \en Operation result code. \~
  \param[out] resShell - \ru Оболочка - результат построения.
                         \en Shell - the result of construction. \~
  \result \ru Возвращает строитель.
          \en Returns the constructor. \~
  \ingroup Model_Creators
*/
// ---
MATH_FUNC (c3d::CreatorSPtr) CreateCurveLofted(       c3d::ShellSPtr &      srcSolid,
                                                      MbeCopyMode           copyMode,
                                                const MbLoftedSolidParams & loftSolParams,
                                                      MbResultType &        res,
                                                      c3d::ShellSPtr &      resShell );



#endif // __CR_LOFTED_SOLID_H
