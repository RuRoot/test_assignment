////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Строитель оболочки из листового материала с подсечкой.
         \en Constructor of a shell from sheet material with a jog.
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_STAMP_JOG_SOLID_H
#define __CR_STAMP_JOG_SOLID_H


#include <templ_ss_array.h>
#include <creator.h>
#include <sheet_metal_param.h>


//------------------------------------------------------------------------------
/** \brief \ru Строитель оболочки из листового материала с подсечкой.
           \en Constructor of a shell from sheet material with a jog. \~
  \details \ru Строитель оболочки из листового материала с подсечкой. \n 
    С помощью подсечки плоская часть листового тела лежащая по левую или правую
    сторону отрезка смещается компланарно относительно своего изначального положения
    вдоль нормали или под углом к ней. Операция выполняется с помощью двух коллинеарных
    сгибов по линии и бывает двух типов: с добавлением и без добавления материала.
    Без добавления материала - это просто два сгиба по линии. С добавлением - материал
    наращивается таким образом, чтобы проекция поднятого участка совпала с контуром
    его первоначального положения.
           \en Constructor of a shell from sheet material with a jog. \n 
    Using a jog the planar part of a sheet solid lying on the left or on the right of
    the segment is shifted complanarly relative to its initial position
    along the normal or at angle to the normal. The operation is performed using two collinear
    bends by a line and can be of two types: with and without addition of the material.
    Without addition of the material - is simply two bends by a line. With addition of the material - the material
    is grown so as the projection of the raised part coincides with the contour of
    its initial state. \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbJogSolid : public MbCreator {
private:
  SArray<MbItemIndex> bendingFacesIndices;  ///< \ru Индексы подсекаемых граней (сортированы и не повторяются). \en Indices of faces for a jog (are sorted and not duplicated). 
  MbCurve3D *         curve;                ///< \ru Отрезок подсечки. \en A jog segment. 
  bool                unbended;             ///< \ru Флаг построения подсечки в разогнутом виде. \en Flag of a jog construction in unfolded state. 
  MbJogValues         jogParameters;        ///< \ru Параметры подсечки совместно с параметрами первого сгиба. \en Parameters of a jog together with the parameters of the first bend. 
  MbBendValues        secondBendParameters; ///< \ru Параметры второго сгиба. \en Parameters of the second bend. 

public :
  MbJogSolid( const SArray<MbItemIndex> & bendingFacesIndices,
                    MbCurve3D &           curve,
              const bool                  unbended,
              const MbJogValues &         jogPars,
              const MbBendValues &        secondBendPars,
              const MbSNameMaker &        names );
private:
  MbJogSolid( const MbJogSolid &, MbRegDuplicate *ireg );
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbJogSolid( const MbJogSolid & );

public:
  virtual ~MbJogSolid();

  // \ru Общие функции математического объекта \en Common functions of the mathematical object 

  MbeCreatorType  IsA()  const override; // \ru Тип элемента \en A type of element 
  MbCreator & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию \en Create a copy
  bool        IsSame   ( const MbCreator & other, double accuracy ) const override; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  bool        IsSimilar( const MbCreator & item ) const override; // \ru Являются ли объекты подобными? \en Determine whether an object is similar?
  bool        SetEqual ( const MbCreator & ) override; // \ru Сделать равным \en Make equal 
  void        Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void        Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override; // \ru Сдвиг \en Translation
  void        Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate around an axis

  void        GetProperties( MbProperties & properties ) override; // \ru Выдать свойства объекта \en Get properties of the object 
  void        SetProperties( const MbProperties & properties ) override; // \ru Записать свойства объекта \en Set properties of the object 
  MbePrompt   GetPropertyName() override; // \ru Выдать заголовок свойства объекта \en Get a name of object property 
  void        GetBasisItems ( RPArray<MbSpaceItem>   & ) override; // \ru Дать базовые объекты \en Get the base objects
  void        GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void        SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  // \ru Общие функции твердого тела \en Common functions of solid  

  bool        CreateShell( MbFaceShell *&         shell,
                           MbeCopyMode            sameShell, 
                           RPArray<MbSpaceItem> * items = nullptr ) override; // \ru Построение \en Construction

  // \ru Дать параметры. \en Get the parameters. 
  void        GetParameters(       MbJogValues & params ) const { params = jogParameters; }
  // \ru Установить параметры. \en Set the parameters. 
  void        SetParameters( const MbJogValues & params )       { jogParameters = params; }
  // \ru Дать параметры. \en Get the parameters. 
  void        GetParameters(       MbBendValues & params ) const { params = secondBendParameters; }
  // \ru Установить параметры. \en Set the parameters. 
  void        SetParameters( const MbBendValues & params )       { secondBendParameters = params; }

private:
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
  MbJogSolid & operator = ( const MbJogSolid & ); 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbJogSolid )
}; // MbJogSolid

IMPL_PERSISTENT_OPS( MbJogSolid )


//------------------------------------------------------------------------------
/** \brief \ru Построить оболочки из листового материала с подсечкой.
           \en Construct shells from the sheet material with a jog. \~
  \details \ru На базе исходной оболочки из листового материала построить оболочку с подсечкой.\n
    Одновременно с построением оболочки функция создаёт её строитель.\n
           \en A shell with a jog is to be created on the basis of the source shell from the sheet material.\n
    The function simultaneously creates the shell and its constructor.\n \~ 
  \param[in]  solid      - \ru Исходная оболочка.
                           \en The source shell. \~
  \param[in]  sameShell  - \ru Режим копирования исходной оболочки.
                           \en Mode of copying the source shell. \~
  \param[in]  bendingFaces - \ru Грани сгибов.
                             \en Faces of bends. \~
  \param[in]  curve      - \ru Кривая формы подсечки.
                           \en Curve of a jog shape. \~
  \param[in]  unbended   - \ru Флаг разогнутого состояния.
                           \en Flag of unfolded state. \~
  \param[in]  parameters - \ru Параметры подсечки.
                           \en The jog parameters. \~
  \param[in]  secondBendParams - \ru Параметры сгибов.
                                 \en The bends parameters. \~
  \param[in]  names      - \ru Именователь операции.
                           \en An object defining names generation in the operation. \~
  \param[in]  firstBendFaces  - \ru Грани первого сгиба.
                                \en The first bend faces. \~
  \param[in]  secondBendFaces - \ru Грани второго сгиба.
                                \en The second bend faces. \~
  \param[out] res        - \ru Код результата операции выдавливания.
                           \en The extrusion operation result code. \~
  \param[out] shell      - \ru Построенная оболочка.
                           \en The resultant shell. \~
  \result \ru Возвращает строитель оболочки.
          \en Returns the shell constructor. \~
  \ingroup Model_Creators
*/
// ---
MATH_FUNC (MbCreator *) CreateSheetSolidJog(       SPtr<MbFaceShell> & solid,
                                                   MbeCopyMode         sameShell,
                                             const RPArray<MbFace> &   bendingFaces,
                                                   MbCurve3D &         curve,
                                             const bool                unbended,
                                             const MbJogValues &       parameters,
                                             const MbBendValues &      secondBendParams,
                                             const MbSNameMaker &      names,
                                                   RPArray<MbFace> &   firstBendFaces,
                                                   RPArray<MbFace> &   secondBendFaces,
                                                   MbResultType &      res,
                                                   MbFaceShell *&      shell );


#endif // __CR_STAMP_JOG_SOLID_H
