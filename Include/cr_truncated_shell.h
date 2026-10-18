////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Построение усеченной оболочки.
         \en Construction of a truncated shell. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TRUNCATED_SHELL_H
#define __TRUNCATED_SHELL_H


#include <creator.h>
#include <cr_split_data.h>
#include <math_define.h>
#include <mb_enum.h>
#include <mb_operation_result.h>
#include <wire_frame.h>


class    MbCurve;
class    MbAxis3D;
class    MbSpaceItem;
class    MbPlacement3D;
class    MbCurve3D;
class    MbFace;
class    MbSolid;
class    MbSNameMaker;
class    MbTruncateShellParams;
struct   MbMergingFlags;


//------------------------------------------------------------------------------
/** \brief \ru Строитель усеченной оболочки.
           \en Constructor of a truncated shell. \~
  \details \ru Строитель усеченной оболочки режет исходную оболочку на части указанными элементами, 
    которыми могут служить двумерные кривые в локальной системе координат, трёхмерные кривые, поверхности, оболочка и каркас \n
           \en Constructor of a truncated shell cuts the initial shell into parts by the specified elements 
    which can be two-dimensional curves in the local coordinate system, three-dimensional curves, surfaces,  shell and wire frame. \n \~ 
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbTruncatedShell : public MbCreator {
private :
  std::vector<MbItemIndex>  selIndices; ///< \ru Идентификаторы выбранных граней усекаемой оболочки. \en Identifiers of selected faces of the shell being truncated. 
  MbSplitData               splitItems; ///< \ru Усекающие элементы c ориентациями. \en Truncating elements with orientations. 
  c3d::BoolVector           orients;    ///< \ru Ориентация усекающих элементов. \en Orientation of truncating elements. 
  bool                      mergeFaces; ///< \ru Сливать подобные грани (true). \en Whether to merge similar faces (true). 
  bool                      mergeEdges; ///< \ru Сливать подобные ребра (true). \en Whether to merge similar edges (true). 
  MbPrecision               precision;  ///< \ru Точность операции. \en The precision of operation.

public:
  /// \ru Конструктор по двумерным кривым. \en Constructor by two-dimensional curves. 
  MbTruncatedShell( const MbPlacement3D &, const RPArray<MbContour> &, bool same, 
                    const c3d::BoolVector & orients, const MbMergingFlags &, const MbSNameMaker & );
  /// \ru Конструктор по трехмерным кривым. \en Constructor by three-dimensional curves. 
  MbTruncatedShell( const RPArray<MbCurve3D> &, bool same, 
                    const c3d::BoolVector & orients, const MbMergingFlags &, const MbSNameMaker & );
  /// \ru Конструктор по поверхностям. \en Constructor by surfaces. 
  MbTruncatedShell( const RPArray<MbSurface> &, bool same, 
                    const c3d::BoolVector & orients, const MbMergingFlags &, const MbSNameMaker & );
  /// \ru Конструктор по строителям тела. \en Constructor by solid creators. 
  MbTruncatedShell( const MbSolid &, bool same, bool keepShell,
                    bool orient, const MbMergingFlags &, const MbSNameMaker & );
  /// \ru Конструктор по каркасу. \en Constructor by wire frame. \warning \ru В разработке. \en Under development.
  MbTruncatedShell( const MbWireFrame &, bool same, 
                    bool orient, const MbMergingFlags &, const MbSNameMaker &, const MbPrecision & );

  virtual ~MbTruncatedShell();

private:
  MbTruncatedShell( const MbTruncatedShell &, MbRegDuplicate * );

public:
  MbeCreatorType  IsA()  const override; // \ru Тип элемента \en Type of element 
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию \en Create a copy

  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override; // \ru Сдвиг \en Translation
  void        Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate about an axis

  void        GetProperties( MbProperties & properties ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & properties ) override; // \ru Записать свойства объекта \en Set properties of the object 
  MbePrompt   GetPropertyName() override;                        // \ru Выдать заголовок свойства объекта \en Get a name of object property 
  void        GetBasisItems ( RPArray<MbSpaceItem>   & ) override; // \ru Дать базовые объекты \en Get the base objects
  void        GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.
  size_t      GetCreatorsCount( MbeCreatorType ct ) const override; // \ru Посчитать внутренние построители по типу. \en Count internal creators by type.
  bool        GetInternalCreators( MbeCreatorType, c3d::ConstCreatorsSPtrVector & ) const override; // \ru Получить внутренние построители по типу. \en Get internal creators by type.
  bool        SetInternalCreators( MbeCreatorType, c3d::CreatorsSPtrVector & ) override;            // \ru Получить внутренние построители по типу. \en Get internal creators by type.
  
  const MbPrecision & GetPrecision() const override { return precision; }  // \ru Выдать  точность. \en Get the precision.

  bool        IsSame( const MbCreator &, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        IsSimilar ( const MbCreator & ) const override; // \ru Являются ли объекты подобными \en Whether the objects are similar
  bool        SetEqual( const MbCreator & ) override;         // \ru Сделать равным \en Make equal

  // \ru Построение оболочки по исходным данным \en Construction of a shell from the given data 
  bool        CreateShell( MbFaceShell *& shell, MbeCopyMode sameShell, 
                           RPArray<MbSpaceItem> * items = nullptr ) override;

  // \ru Установить номера выбранных граней усекаемого тела \en Set indices of selected faces of the solid being truncated. 
  void        SetSelIndices( const std::vector<MbItemIndex> & selInds );

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbTruncatedShell )
  OBVIOUS_PRIVATE_COPY( MbTruncatedShell )
}; // MbTruncatedShell

IMPL_PERSISTENT_OPS( MbTruncatedShell )
#endif // __TRUNCATED_SHELL_H
