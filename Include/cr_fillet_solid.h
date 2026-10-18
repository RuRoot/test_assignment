////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Строитель скругления ребер.
         \en Edges fillet constructor.
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_FILLET_SOLID_H
#define __CR_FILLET_SOLID_H


#include <cr_smooth_solid.h>
#include <function.h>


struct  MbEdgeFunction; 


//------------------------------------------------------------------------------
/** \brief \ru Строитель скругления ребер.
           \en Edges fillet constructor. \~
  \details \ru Строитель скругления ребер содержит параметры для выполнения операции, функции изменения радиуса, 
    идентификаторы граней остановки скруглений, идентификаторы скругляемых вершин. \n
    Скругление ребра заключается в его замене на грань, гладко сопрягающую соединяемые ребром грани.
    Построенная грань в сечении может иметь форму дуги окружности, эллипса, параболы и гиперболу.
    Дуга окружности может иметь постоянный или переменный радиус, а также постоянную хорду. \n
           \en Edges fillet constructor contains parameters for performing the operation, radius law, 
    identifiers of faces terminating fillets, fillet vertices identifiers. \n
    Edge fillet consists in its replacement with a face smoothly connecting the faces incident at the edge.
    The section of the constructed face can be an arc of circle, ellipse, parabola or hyperbola.
    A circular arc can have a constant or variable radius and also a constant chord. \n \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbFilletSolid : public MbSmoothSolid {
public :
  RPArray<MbFunction> functions;  ///< \ru Функции изменения радиусов сопряжения. \en Functions of changing conjugation radii. 
  RPArray<MbCurve3D>  slideways;  ///< \ru Опорные кривые сопряжения. \en Supporting curves of conjugation. 
  SArray<MbItemIndex> boundaries; ///< \ru Номера граней для обрезки краёв скругления / фаски. \en Indices of faces for trimming the fillet / chamfer boundaries. 
  SArray<MbItemIndex> vertices;   ///< \ru Номера скругляемых вершин. \en Indices of vertices to fillet. 
  CornerValues        cornerData; ///< \ru Параметры скругления вершин. \en Parameters of vertices fillet.  

public :
  MbFilletSolid( SArray<MbEdgeFacesIndexes> & inds, 
                 RPArray<MbFunction> & funcs, 
                 RPArray<MbCurve3D>  & slids,
                 SArray<MbItemIndex> & bounds, 
                 SArray<MbItemIndex> & verts,
                 const SmoothValues  & params, 
                 const CornerValues  & data, 
                 const MbSNameMaker  & n );
private :
  MbFilletSolid( const MbFilletSolid & init, MbRegDuplicate * iReg ); // \ru Конструктор копирования \en Copy-constructor 
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbFilletSolid( const MbFilletSolid & init ); 
public :
  virtual ~MbFilletSolid();

  // \ru Общие функции математического объекта. \en Common functions of the mathematical object. 

  MbeCreatorType  IsA() const override; // \ru Тип элемента. \en A type of element. 
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию \en Create a copy
  void        Transform( const MbMatrix3D &matr, MbRegTransform * = nullptr ) override; // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move( const MbVector3D &to, MbRegTransform * = nullptr ) override;        // \ru Сдвиг \en Translation
  void        Rotate( const MbAxis3D &axis, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate around an axis

  void        GetProperties( MbProperties & properties ) override; // \ru Выдать свойства объекта. \en Get properties of the object. 
  void        SetProperties( const MbProperties & properties ) override; // \ru Записать свойства объекта. \en Set properties of the object.
  MbePrompt   GetPropertyName() override; // \ru Выдать заголовок свойства объекта. \en Get a name of object property. 

  bool        IsSame( const MbCreator & other, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal? 
  bool        IsSimilar ( const MbCreator & ) const override; // \ru Являются ли объекты подобными \en Whether the objects are similar
  bool        SetEqual( const MbCreator & init ) override; // \ru Сделать равным. \en Make equal.

  // \ru Общие функции твердого тела \en Common functions of solid  

  bool        CreateShell( MbFaceShell *& shell, MbeCopyMode sameShell, 
                           RPArray<MbSpaceItem> * items = nullptr ) override; // \ru Построение \en Construction

private :
  void        ReadDistances ( reader &in ) override;
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
  void        operator = ( const MbFilletSolid & ); // \ru НЕЛЬЗЯ!!! \en NOT ALLOWED!!! 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbFilletSolid )
}; // MbFilletSolid

IMPL_PERSISTENT_OPS( MbFilletSolid )


//------------------------------------------------------------------------------
/** \brief \ru Создать оболочку со скруглением ребер.
           \en Create a shell with edges fillet. \~
  \details \ru Для указанной оболочки построить оболочку, в которой выполнено скругление рёбер.\n
            Одновременно с построением оболочки функция создаёт её строитель.
            Способ построения скругления определяется набором параметров в filletParams: \n
            - Скруглить указанные рёбра тела постоянным радиусом. \n
            - Скруглить вершины и примыкающие к ней рёбра тела постоянным радиусом.
              В вершинах должно стыковаться три ребра. \n
            - Скруглить указанные ребра тела переменным радиусом, задаваемым MbEdgeFunction.function.
              Параметры поперечного сечения могут изменяться по заданному закону. \n
           \en For a given shell create a shell with edges fillet.\n
            The function simultaneously creates the shell and its constructor. 
            The way of creating a fillet is determined by a set of parameters in filletParams:
            - Fillet the specified edges of the solid with a constant radius. \n
            - Create fillets on vertices and the edges of the solid adjacent to these vertices with a constant radius.
              Three edges must be incident to each vertex. \n
            - Fillet the given edges of the solid with a variable radius specified by MbEdgeFunction.function.
              The parameters of the cross-section can vary by the specified law. \n \~
  \param[in]  solid        - \ru Исходная оболочка.
                             \en The source shell. \~
  \param[in]  sameShell    - \ru Способ копирования граней исходной оболочки.
                             \en Method of copying the source shell faces. \~
  \param[in] filletParams   - \ru Параметры операции.
                              \en The operation parameters. \~
  \param[out] res           - \ru Код результата операции.
                              \en Operation result code. \~
  \param[out] shell         - \ru Построенный набор граней.
                              \en Constructed set of faces. \~
  \result \ru Возвращает строитель, если операция была выполнена успешно.
          \en Returns the constructor if the operation has been successfully performed. \~
  \ingroup Curve3D_Modeling
*/
// ---
MATH_FUNC( MbCreator * ) CreateFillet(        MbFaceShell *         solid,
                                              MbeCopyMode           sameShell,
                                       const  MbShellFilletValues & filletParams,
                                              MbResultType &        res,
                                              c3d::ShellSPtr &      shell,
                                              IProgressIndicator * progInd = nullptr );



#endif // __CR_FILLET_SOLID_H
