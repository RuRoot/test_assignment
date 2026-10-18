////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Твердое тело.
         \en Solid solid. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __SOLID_H
#define __SOLID_H

#include <model_item.h>
#include <topology_faceset.h>

class  MbIngotSolidParams;
class  MbSolid;
namespace c3d // namespace C3D
{
typedef SPtr<MbSolid>                               SolidSPtr;
typedef SPtr<const MbSolid>                         ConstSolidSPtr;

typedef std::vector<MbSolid *>                      SolidsVector;
typedef std::vector<const MbSolid *>                ConstSolidsVector;

typedef std::vector<SolidSPtr>                      SolidsSPtrVector;
typedef std::vector<ConstSolidSPtr>                 ConstSolidsSPtrVector;

typedef std::set<MbSolid *>                         SolidsSet;
typedef SolidsSet::iterator                         SolidsSetIt;
typedef SolidsSet::const_iterator                   SolidsSetConstIt;
typedef std::pair<SolidsSetConstIt, bool>           SolidsSetRet;

typedef std::set<SolidSPtr>                         SolidsSPtrSet;
typedef SolidsSPtrSet::iterator                     SolidsSPtrSetIt;
typedef SolidsSPtrSet::const_iterator               SolidsSPtrSetConstIt;
typedef std::pair<SolidsSPtrSetConstIt, bool>       SolidsSPtrSetRet;

typedef std::set<const MbSolid *>                   ConstSolidsSet;
typedef ConstSolidsSet::iterator                    ConstSolidsSetIt;
typedef ConstSolidsSet::const_iterator              ConstSolidsSetConstIt;
typedef std::pair<ConstSolidsSetConstIt, bool>      ConstSolidsSetRet;

typedef std::set<ConstSolidSPtr>                    ConstSolidsSPtrSet;
typedef ConstSolidsSPtrSet::iterator                ConstSolidsSPtrSetIt;
typedef ConstSolidsSPtrSet::const_iterator          ConstSolidsSPtrSetConstIt;
typedef std::pair<ConstSolidsSPtrSetConstIt, bool>  ConstSolidsSPtrSetRet;
}

//------------------------------------------------------------------------------
/** \brief \ru Твердое тело.
           \en Solid solid. \~
  \details \ru Твердое тело, или тело, является объектом геометрической модели. 
    Тело состоит из множества граней MbFaceShell.\n
    Структура данных тела содержит указатель на набор граней outer и тип связности тела #multiState. 
    Тело может описывать одно или несколько связных множеств точек. Тип связности #multiState сообщает о том, 
    что тело описывает одно связное множество точек, или, что тело описывает несколько связных множеств точек 
    и может быть разбито на несколько тел.\n 
    Набор граней тела #outer->faceSet описывает одну или несколько однородных оболочек. 
    В зависимости от признака #outer->closed замкнутости оболочек тело может описывать 
    два принципиально разных множества точек.\n 
    Если все оболочки тела замкнутые, то тело описывает множество точек, расположенных с 
    внутренней стороны одной внешней и нескольких внутренних оболочек, расположенных внутри 
    внешней оболочки, в совокупности с точками этих оболочек.\n
    Если оболочки тела незамкнутые, то тело описывает множество точек, принадлежащих граням этих оболочек.\n 
    В зависимости от замкнутости оболочек тело будем называть замкнутым или незамкнутым.
    В частном случае, когда все оболочки являются замкнутыми, получим замкнутое тело.
    В общем случае оболочки тела могут быть незамкнутыми, тогда получим незамкнутое тело.\n
    Замкнутое тело и незамкнутое тело оперируют разными множествами точек и это различие влияет на булевы и другие операции с телами. \n 
    Над телами можно выполнять определённый набор действий.
    Эти действия записываются в журнал построения тела, элементами которого являются строители оболочек MbCreator.\n
    Тело может иметь атрибуты MbAttribute.\n
           \en Solid solid or solid is object of geometric model. 
    Solid consists of face set MbFaceShell. \n
    Data structure of solid contains pointer to face set outer and type of solid connectivity #multiState. 
    The solid can describe one or more sets of points. Connection type #multiState informs 
    that the solid describes one connected set of points or that the solid describes some connected sets of points 
    and can be split into multiple solids.\n 
    Face set of solid #outer->faceSet describes one or some homogeneous shells. 
    Depending on the attribute of #outer->closed shell closedness the solid can describe 
    two principally different sets of points.\n 
    If all the shells of solid are closed, then the solid describes a set of points which are located on  
    the inside of one external and several internal shells located inside 
    outer shell in combination with points of these shells.\n
    If shells of solid are closed, then the solid describes a set of points belonging to the faces of these shells.\n 
    Depending on the closedness of shells the solid is called closed or non-closed.
    In the special case when all the shells are closed, the solid is closed.
    In the general case shells of solid can be unclosed, the solid is unclosed. \n
    Closed solid and unclosed solid operate with different sets of points and this difference affects on boolean other operations with solids. \n 
    Certain set of actions can be performed with solids.
    These actions are recorded to history tree of solid, elements of which are creators of shells MbCreator. \n
    Solid can have attributes MbAttribute.\n \~
  \ingroup Model_Items
*/
// ---
class MATH_CLASS MbSolid : public MbItem {
public:
  /** \brief \ru Тип связности тела.
             \en The solid connection type. \~
    \details \ru Тело может состоять из одного или нескольких связных множеств точек.
             \en The solid can consist of one or several connected sets of points. \~
  */
  enum MultiState {
    ms_Undefined = 0, ///< \ru Связность тела не определена. \en Connectivity of solid is undefined. 
    ms_Single,        ///< \ru Тело описывает одно связное множество точек. \en The solid describes one connected set of points. 
    ms_Multiple,      ///< \ru Тело описывает несколько связных множеств точек и может быть разбито на несколько связных частей. \en The solid describes several connected set of points and can be split into multiple connected parts. 
  };
protected:
  MbFaceShell      * outer;      ///< \ru Оболочка тела. \en Shell of solid. 
  mutable MultiState multiState; ///< \ru Тип связности тела. \en The solid connection type. 
//#ifdef _MSC_VER 
//CRITICAL_SECTION   itemLock_;  // \ru Критическая секция для монопольного доступа к объекту. \en The critical section for exclusive access to the object. 
//#ifdef _MSC_VER

private:
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbSolid( const MbSolid & other ); 
  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  explicit MbSolid( const MbSolid & other, MbRegDuplicate * iReg );
public :
  /// \ru Конструктор по оболочке и строителю. \en Constructor by shell and creator. 
  explicit MbSolid( MbFaceShell * shell, MbCreator * creator );
  /// \ru Конструктор по оболочке и строителю. \en Constructor by shell and creator. 
  explicit MbSolid( MbFaceShell & shell, MbCreator & creator );

  /// \ru Конструктор по оболочке и набору строителей, флагу копирования строителей и регистратору дублирования объектов. \en Constructor by shell and set of creators, flag of creators copying and registrator of objects duplicating. 
  MbSolid( MbFaceShell & shell, RPArray<MbCreator> &      creators, bool sameCreators, MbRegDuplicate * iReg );
  /// \ru Конструктор по оболочке и набору строителей, флагу копирования строителей и регистратору дублирования объектов. \en Constructor by shell and set of creators, flag of creators copying and registrator of objects duplicating. 
  MbSolid( MbFaceShell & shell, c3d::CreatorsVector &     creators, bool sameCreators, MbRegDuplicate * iReg );
  /// \ru Конструктор по оболочке и набору строителей, флагу копирования строителей и регистратору дублирования объектов. \en Constructor by shell and set of creators, flag of creators copying and registrator of objects duplicating. 
  MbSolid( MbFaceShell & shell, c3d::CreatorsSPtrVector & creators, bool sameCreators, MbRegDuplicate * iReg );

  /// \ru Конструктор по оболочке, телу, у которого берутся строители, и строителю. \en Constructor by shells, creator and solid which has creators.  
  MbSolid( MbFaceShell & shell, const MbSolid & solid, MbCreator & creator );
  /// \ru Конструктор по оболочке, телу, у которого берутся строители, и строителю. \en Constructor by shells, creator and solid which has creators.  
  MbSolid( MbFaceShell & shell, const MbSolid & solid, MbCreator * creator );
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbSolid();

public :
  VISITING_CLASS( MbSolid ); 

  // \ru Общие функции геометрического объекта. \en Common functions of a geometric object. 

  MbeSpaceType  IsA()  const override; // \ru Тип объекта. \en A type of an object. 
  MbSpaceItem & Duplicate( MbRegDuplicate * iReg = nullptr ) const override; // \ru Создать копию. \en Create a copy.
  void    Transform( const MbMatrix3D &, MbRegTransform * iReg = nullptr ) override; // \ru Преобразовать согласно матрице. \en Transform according to the matrix.
  void    Move     ( const MbVector3D &, MbRegTransform * iReg = nullptr ) override; // \ru Сдвинуть вдоль вектора. \en Move along a vector.
  void    Rotate   ( const MbAxis3D &, double angle, MbRegTransform * iReg = nullptr ) override; // \ru Повернуть вокруг оси. \en Rotate around an axis.
  bool    IsSame   ( const MbSpaceItem &, double accuracy = LENGTH_EPSILON ) const override; // \ru Являются ли объекты равными? \en Determine whether objects are equal.
  bool    IsSimilar( const MbSpaceItem & ) const override; // \ru Являются ли объекты подобными? \en Determine whether objects are similar. 
  bool    SetEqual ( const MbSpaceItem & ) override; // \ru Сделать объекты равными. \en Make the objects equal. 
  double  DistanceToPoint( const MbCartPoint3D & ) const override; // \ru Вычислить расстояние до точки. \en Calculate the distance to a point. 
  void    AddYourGabaritTo( MbCube & ) const override; // \ru Добавить свой габарит в куб. \en Add own bounding box into a cube. 
  void    Refresh() override; // \ru Сбросить все временные данные. \en Reset all temporary data. 
  void    CalculateMesh( const MbStepData & stepData, const MbFormNote & note, MbMesh & mesh ) const override; // \ru Построить полигональную копию mesh. \en Build polygonal copy mesh.

  MbProperty & CreateProperty( MbePrompt ) const override;     // \ru Создать собственное свойство. \en Create a custom property.
  void    GetProperties( MbProperties & ) override;            // \ru Выдать свойства объекта. \en Get properties of the object. 
  void    SetProperties( const MbProperties & ) override;      // \ru Установить свойства объекта. \en Set properties of the object. 
  void    GetBasisItems ( RPArray<MbSpaceItem> & ) override;   // \ru Выдать базовые объекты. \en Get the base objects. 
  void    GetBasisPoints( MbControlData3D & ) const override;  // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void    SetBasisPoints( const MbControlData3D & ) override;  // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  // \ru Перестроить объект по журналу построения. \en Reconstruct object according to the history tree. 
  bool    RebuildItem( MbeCopyMode copyMode, RPArray<MbSpaceItem> * items, IProgressIndicator * progInd ) override;
  // \ru Достроить тело для последней невыполненной операции в журнале построения. \en Build a body by the last kept operation in the build log.
  virtual bool    FinishItem(); 

  // \ru Создать полигональный объект - упрощенную копию данного объекта. \en Create a polygonal object - a simplified copy of the given object. 
  MbItem * CreateMesh( const MbStepData & stepData, const MbFormNote & note, MbRegDuplicate * iReg ) const override; 

  /** \ru \name Функции тела.
      \en \name Functions of solid.
      \{ */

  /** \brief \ru Рассчитать габарит.
             \en Calculate bounding box. \~
    \details \ru Рассчитать габарит тела.
             \en Calculate bounding box of solid. \~
    \param[out] cube - \ru Рассчитанный габарит.
                       \en Calculated bounding box. \~
  */
  virtual void    CalculateGabarit( MbCube & cube ) const; 
 
 /** \brief \ru Получить габарит.
             \en Get bounding box. \~
    \details \ru Получить габарит тела.
             \en Get bounding box of solid. \~
    \return  \ru Габарит.
             \en Bounding box. \~
  */
  const MbCube    GetCube() const;

  /** \brief \ru Добавить свои строители в присланный массив.
             \en Add your own creators to the given array. \~
    \details \ru При отсутствии строителей создает строитель без истории 
      и добавляет его в присланный контейнер и в пустой журнал построения.
             \en If there are no creators, then creates a creator without history 
      and adds it to the given container and to empty history tree. \~
    \param[out] creators - \ru Контейнер для добавления своих строителей.
                           \en Container for adding of its creators. \~
    \return \ru Добавлены ли строители.
            \en Whether creators are added. \~
  */
  bool    GetCreators( RPArray<MbCreator> & creators ) const override;
  /** \brief \ru Добавить свои строители в присланный массив.
             \en Add your own creators to the given array. \~
    \details \ru При отсутствии строителей создает строитель без истории 
      и добавляет его в присланный контейнер и в пустой журнал построения.
             \en If there are no creators, then creates a creator without history 
      and adds it to the given container and to empty history tree. \~
    \param[out] creators - \ru Контейнер для добавления своих строителей.
                           \en Container for adding of its creators. \~
    \return \ru Добавлены ли строители.
            \en Whether creators are added. \~
  */
  bool    GetCreators( c3d::CreatorsSPtrVector & creators ) const override;

          /** \brief \ru Построить полигональную копию тела и положить её данные в присланный объект.
                     \en Create a polygonal copy of the solid and put its data to the given object. \~
            \details \ru Полигональная копия тела строится только путём триангуляции граней.
                     \en Polygonal copy of the solid is created only by triangulating the faces. \~
            \param[in] stepData - \ru Данные для вычисления шага при триангуляции.
                                  \en Data for step calculation during triangulation. \~
            \param[out] mesh - \ru Полигональная копия объекта.
                               \en Polygonal copy of the object. \~
          */
          void    CalculateGrid( const MbStepData & stepData, MbMesh & mesh ) const;

          /// \ru Заменить оболочку тела на присланную. \en Replace the shell of the solid by the given one. 
          void    SetShell( MbFaceShell * shell ); 
          /// \ru Отцепить оболочку. \en Detach the shell. 
          MbFaceShell * DetachShell(); 
          /// \ru Выдать оболочку. \en Get the shell. 
          MbFaceShell * GetShell() const; 
          /// \ru Имеется ли оболочка? \en Is there a shell? 
          bool    IsShellBuild() const; 

          /// \ru Переустановить в ребрах указатели на соединяемые ими грани. \en Reinstall pointers to mating faces in edges. 
          void    MakeRight(); 
          /// \ru Верно ли установлены в ребра указатели на соединяемые ими грани? \en Are the pointers in edges to the faces connected by them set correctly? 
          bool    IsRight() const; 

          /// \ru Выдать количество граней. \en Get the count of faces.  
          size_t  GetFacesCount() const { return outer ? outer->GetFacesCount() : 0; }
          /// \ru Заполнить контейнер вершинами тела. \en Fill container by solid vertices.  
          template <class VerticesVector>
          void    GetVertices( VerticesVector & vertices ) const { if ( outer != nullptr ) { outer->GetVertices<VerticesVector>( vertices ); }  }
          /// \ru Заполнить контейнер ориентированными ребрами тела. \en Fill container by oriented edges of the solid. 
          template <class EdgesVector>
          void    GetEdges( EdgesVector & edges ) const { if ( outer != nullptr ) { outer->GetEdges<EdgesVector>( edges ); } }
          /// \ru Заполнить контейнеры вершинами и ребрами тела. \en Fill containers by vertices and edges of the solid. 
          template <class VerticesVector, class EdgesVector>
          void    GetItems( VerticesVector & vertices, EdgesVector & edges ) const { if ( outer != nullptr ) { outer->GetItems<VerticesVector,EdgesVector>( vertices, edges ); } }
          /// \ru Заполнить контейнер гранями тела. \en Fill container by solid faces. 
          template <class FacesVector>
          void    GetFaces   ( FacesVector & faces ) const { if ( outer != nullptr ) { outer->GetFaces<FacesVector>( faces ); } }
          /// \ru Заполнить контейнер гранями тела. \en Fill container by solid faces. 
          template <class FacesSet>
          void    GetFacesSet( FacesSet & faces ) const { if ( outer != nullptr ) { outer->GetFacesSet<FacesSet>( faces ); } }
          /// \ru Заполнить контейнер вершинами, ребрами и гранями тела. \en Fill container by vertices, edges and faces of the solid. 
          template <class TopologyItemsVector>
          void    GetItems( TopologyItemsVector & items ) const { if ( outer != nullptr ) { outer->GetItems( items );  } }

          /// \ru Выдать вершину по её номеру. \en Get vertex by its index. 
          MbVertex    * GetVertex( size_t index ) const; 
          /// \ru Выдать ребро по его номеру. \en Get edge by its index. 
          MbCurveEdge * GetEdge  ( size_t index ) const; 
          /// \ru Выдать грань по её номеру. \en Get face by its index. 
          MbFace      * GetFace  ( size_t index ) const; 

          /// \ru Выдать номер вершины. \en Get the vertex index. 
          size_t  GetVertexIndex( const MbVertex & ) const; 
          /// \ru Выдать номер ребра. \en Get the edge index. 
          size_t  GetEdgeIndex  ( const MbCurveEdge & ) const; 
          /// \ru Выдать номер грани. \en Get the face index. 
          size_t  GetFaceIndex  ( const MbFace &  ) const; 
          /// \ru Выдать количество связных оболочек тела. \en Get the count of connected shells of the solid. 
          size_t  GetShellCount() const; 

          /// \ru Вывернуть тело наизнанку - переориентировать все грани. \en Revert the solid - reorientation of the whole set of faces. 
          bool    Reverse();

          /** \brief \ru Классифицировать точку.
                     \en Classify point. \~
            \details \ru Классификация заключается в определении положения точки относительно тела.
                     \en Classification consists in determining the point position relative to the solid. \~
            \param[in] p - \ru Классифицируемая точка.
                           \en Classified point. \~
            \param[in] epsilon - \ru Точность классификации.
                                 \en Classification tolerance. \~
            \return \ru #iloc_InItem    (+1) - Точка лежит внутри тела.\n
                    #iloc_OutOfItem (-1) - Точка лежит вне тела.\n
                    #iloc_OnItem    ( 0) - Точка лежит на поверхности тела.
                    \en #iloc_InItem    (+1) - Point is inside the solid.\n
                    #iloc_OutOfItem (-1) - Point is outside the solid.\n
                    #iloc_OnItem    ( 0) - Point is on the surface of the solid. \~
          */
          MbeItemLocation PointClassification( const MbCartPoint3D & p, double epsilon = Math::metricRegion ) const; 

          /** \brief \ru Классифицировать тело.
                     \en Classify solid. \~
            \details \ru Классификация заключается в определении положения присланного тела относительно данного тела.
                     \en Classification consists in determining a solid position relative to the this solid. \~
            \param[in] solid - \ru Классифицируемое тело.
                               \en Classified solid. \~
            \param[in] epsilon - \ru Точность классификации.
                                 \en Classification tolerance. \~
            \return \ru #iloc_OutOfItem (-1) - Классифицируемое тело лежит вне данного тела.\n
                        #iloc_OnItem    ( 0) - Классифицируемое тело пересекает данного тело.\n
                        #iloc_InItem    (+1) - Классифицируемое тело лежит внутри данного тела.\n
                        #iloc_ByItem    (+2) - Данное тело лежит внутри классифицируемого тела.\n 
                        #iloc_Undefined (-3) - Классификация не выполнялась.\n 
                    \en #iloc_OutOfItem (-1) - Classified solid is outside the this solid.\n
                        #iloc_OnItem    ( 0) - Classified solid intersects the this solid.\n
                        #iloc_InItem    (+1) - Classified solid is inside the this solid.\n
                        #iloc_ByItem    (+2) - This solid is inside the classified solid.\n 
                        #iloc_Undefined (-3) - Solid is not classified.\n \~
          */
          MbeItemLocation SolidClassification( const MbSolid & solid, double epsilon = Math::metricRegion ) const; 

          /** \brief \ru Найти номера граней.
                     \en Find indices of faces. \~
            \details \ru Найти номера граней и заполнить второй контейнер в соответствии с первым.
                     \en Find indices of faces and fill the second container in accordance with the first. \~
            \param[in] faces - \ru Множество граней.
                               \en A set of faces. \~
            \param[out] indices - \ru Найденное множество номеров этих граней в теле.
                                  \en Found set of indices of these faces in the solid. \~
            \return \ru Найдены ли все номера?
                    \en Whether all the indices are found? \~
          */
          bool    FindFacesIndexByFaces( const RPArray<MbFace> & faces, SArray<size_t> & indices ) const;

          /** \brief \ru Найти номера ребер и соединяемых ими граней.
                     \en Find indices of edges and faces connected by them. \~
            \details \ru Найти номера ребер и заполнить второй контейнер в соответствии с первым.
                     \en Find indices of edges and fill the second container in accordance with the first. \~
            \param[in] edges - \ru Множество ребер.
                               \en A set of edges. \~
            \param[out] indices - \ru Найденное множество комбинированных номеров ребер и соединяемых ими граней.
                                  \en Found set of combined indices of edges and faces connected by them. \~
            \return \ru Найдены ли номера?
                    \en Whether indices are found? \~
          */
          bool    FindFacesIndexByEdges( const RPArray<MbCurveEdge> & edges, SArray<MbEdgeFacesIndexes> & indices ) const; 

          /// \ru Выдать ближайшую к точке вершину. \en Get the nearest vertex to point. 
  const   MbVertex * FindNearestVertex( const MbCartPoint3D & p ) const; 

          /** \brief \ru Найти стыкующиеся в вершине ребра тела.
                     \en Find mating edges of the solid at the vertex. \~
            \details \ru Найти ребра тела, для которых данная вершина является начальной или конечной.
                     \en Find edges of the solid for which this vertex is start or end. \~
            \param[in] vertex - \ru Вершина.
                                \en Vertex. \~
            \param[out] findEdges - \ru Стыкующиеся в вершине рёбра.
                                    \en Mating edges at the vertex. \~
          */
          void    FindEdgesForVertex( const MbVertex & vertex, RPArray<MbCurveEdge> & findEdges ) const; 

          /** \brief \ru Найти стыкующиеся в вершине грани тела.
                     \en Find mating faces of the solid at the vertex. \~
            \details \ru Найти грани тела, для ребер которых данная вершина является конечной.
                     \en Find edges of the solid for which this vertex is end. \~
            \param[in] vertex - \ru Вершина.
                                \en Vertex. \~
            \param[out] findFaces - \ru Стыкующиеся в вершине грани.
                                    \en Mating faces at the vertex. \~
          */
          void    FindFacesForVertex( const MbVertex & vertex, RPArray<MbFace> & findFaces ) const; 

          /** \brief \ru Найти для ребра его номер грани, номер цикла и номер ребра в цикле.
                     \en Find a face index for the edge, loop index and edge index in the loop. \~
            \details \ru Для ребра найти номер грани, номер цикла и номер ребра в цикле. 
              Если номера не найдены, то номера сохраняют исходные значения.
                     \en Find face index for the edge, loop index and edge index in the loop. 
              If the indices are not found, then indices remain the same. \~
            \param[in] edge - \ru Ребро.
                              \en Edge. \~
            \param[out] faceN - \ru Найденный номер грани.
                                \en Found face index. \~
            \param[out] loopN - \ru Найденный номер цикла грани.
                                \en Found index of face loop. \~
            \param[out] edgeN - \ru Найденный номер ребра в цикле.
                                \en Found index of the edge in the loop. \~
            \return \ru Найдены ли номера?
                    \en Whether indices are found? \~
          */
          bool    FindEdgeNumbers ( const MbCurveEdge & edge, size_t & faceN, size_t & loopN, size_t & edgeN ) const; 

          /// \ru Найти вершину по имени. \en Find vertex by name. 
  const   MbVertex    * FindVertexByName( const MbName & ) const; 
          /// \ru Найти ребро по имени. \en Find edge by name. 
  const   MbCurveEdge * FindEdgeByName  ( const MbName & ) const; 
          /// \ru Найти грань по имени. \en Find face by name. 
  const   MbFace      * FindFaceByName  ( const MbName & ) const;

          /// \ru Найти вершину по имени. \en Find vertex by name. 
          MbVertex    * FindVertexByName( const MbName & ); 
          /// \ru Найти ребро по имени. \en Find edge by name. 
          MbCurveEdge * FindEdgeByName  ( const MbName & ); 
          /// \ru Найти грань по имени. \en Find face by name. 
          MbFace      * FindFaceByName  ( const MbName & );

          /// \ru Найти вершину по хешу имени. \en Find vertex by hash of a name. 
  const   MbVertex    * FindVertexByHash( const SimpleName h ) const; 
          /// \ru Найти ребро по хешу имени. \en Find edge by hash of a name. 
  const   MbCurveEdge * FindEdgeByHash  ( const SimpleName h ) const; 
          /// \ru Найти грань по хешу имени. \en Find face by hash of a name. 
  const   MbFace      * FindFaceByHash  ( const SimpleName h ) const;

          /// \ru Создать именователь тела. \en Create name-maker of solid. 
          SPtr<MbSNameMaker> GetYourNameMaker() const;

          /// \ru Установить заданный флаг измененности для всех граней, рёбер и вершин. \en Set flag of changes for all the faces, edges and vertices. 
          void    SetOwnChangedThrough( MbeChangedType );
          /// \ru Замкнуто ли тело (не имеет края)? \en Is solid closed (it has not boundary)? 
          bool    IsClosed() const; 

          /// \ru Является ли тело многочастным? \en Is a solid composed of several parts (is a multibody solid)? 
          bool    IsMultiSolid() const; 
          /** \brief \ru Установить флаг многочастности с минимальной проверкой.
                     \en Set flag of multibody with minimal checks. \~
            \details \ru Установить флаг многочастности с минимальной проверкой.
                         Неправильная установка флага может привести к непредсказуемым последствиям в операциях с телом.
                     \en Set flag of multibody with minimal checks.
                         Improper flag setting can lead to unpredictable results after operations with the solid. \~
            \param[in] ms - \ru При ms == true полагается, что тело является многочастностным.
                            \en Assumed that solid is multibody if ms==true. \~
            \param[in] setDirectly - \ru Установить без каких-либо проверок.
                                     \en Set without any checks. \~
            \return \ru Установлен ли флаг?
                    \en Whether flag was set? \~
          */
          bool    SetMultiSolidState( bool ms, bool setDirectly = false ) const;
          /// \ru Сбросить флаг многочастности в неопределённое состояние. \en Reset flag of multibody solid. 
          void    ResetMultiSolidState() const { multiState = ms_Undefined; }
          /// \ru Получить фактическое состояние флага многочастности. \en Get current state of multibody flag. 
          MultiState GetMultiSolidState() const { return multiState; }

          /// \ru Рассчитать габарит относительно локальной системы координат, заданной матрицей преобразования в неё. \en Calculate bounding box relative to the local coordinate system which is given by the matrix 'matrToLocal ' of transformation to it. 
  void    CalculateLocalGabarit( const MbMatrix3D & into, MbCube & cube ) const override; 
          /// \ru Рассчитать габарит относительно локальной системы координат. \en Calculate bounding box relative to the local coordinate system. 
          void    CalculateLocalGabarit( const MbPlacement3D & localPlace, MbCube & cube ) const; 
          /// \ru Выдать базовые объекты журнала построения. \en Get the base objects of history tree. 
          void    BreakToBasisItem( size_t c, RPArray<MbSpaceItem> & s ); 
          /// \ru Присвоить свой указатель глобальной переменной Math::selectSolid = this (для отладки). \en Assign the pointer to global variable Math::selectSolid = this (for debugging). 
          void    Math3DSelectSolid() const; 
  /** \} */

public:
  friend  MbResultType MakeIngot( bool, const MbIngotSolidParams &, c3d::SolidSPtr & );

private:
          void    SetMultiSolidStateDirectly( bool ms ) const { multiState = ms ? ms_Multiple : ms_Single; }
          // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
          void    operator = ( const MbSolid & ); 

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbSolid );
};

IMPL_PERSISTENT_OPS( MbSolid )


#endif // __SOLID_H
