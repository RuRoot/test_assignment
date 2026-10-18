////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Диагностика оболочек и их составляющих.
         \en Diagnostics of shells and their components. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CHECK_GEOMETRY_H
#define __CHECK_GEOMETRY_H

#include <templ_rp_array.h>
#include <templ_sptr.h>
#include <mb_operation_result.h>
#include <mb_variables.h>
#include <attr_common_attribute.h>
#include <topology.h>
#include <solid.h>
#include <point_frame.h>
#include <templ_dptr.h>
#include <vector>
#include <iterator>
#include <curve3d.h>
#include <utility>


//------------------------------------------------------------------------------
/** \brief \ru Информация о пересечении двух тел.
           \en Information about two solids intersection. \~
  \details \ru Информация о пересечении двух тел при диагностике их оболочек. \n
           \en Information about intersection of two solids during diagnostics of their shells. \n \~
  \ingroup Algorithms_3D
*/
//---
struct MATH_CLASS MbShellsIntersectionData {
protected:
  c3d::EdgesSPtrVector edges;          ///< \ru Ребра пересечения (владеет по счетчику ссылок). \en Intersection edges (owns by reference counter). 
  c3d::IndicesVector   faceIndices1;   ///< \ru Номера касающихся граней первого тела. \en The numbers concerning faces of the first  solid.
  c3d::IndicesVector   faceIndices2;   ///< \ru Номера касающихся граней второго тела. \en The numbers concerning faces of the second solid.

  c3d::SolidSPtr       solid;          ///< \ru Тело пересечения  (владеет по счетчику ссылок). \en Intersection solid (owns by reference counter). 

  c3d::PointFrameSPtr  pointFrame;     ///< \ru Группа точек касания. \en Group of touch points. 

  bool                 isTangentCurve; ///< \ru Пересечения - это линии касания. \en Intersections are tangency lines. 
  bool                 isSolid;        ///< \ru Пересечения образуют тела. \en Intersections form solids.

public:
  /// \ru Конструктор. \en Constructor. 
  MbShellsIntersectionData();
  /// \ru Конструктор по ребру. \en Constructor by an edge. 
  MbShellsIntersectionData( const MbCurveEdge & );
  /// \ru Конструктор по ребрам. \en Constructor by edges. 
  template <class EdgesVector>
  MbShellsIntersectionData( const EdgesVector &, bool isSolidEdges );
  /// \ru Конструктор по ребрам. \en Constructor by edges. 
  template <class EdgesVector, class FaceIndicesVector>
  MbShellsIntersectionData( const EdgesVector &, const FaceIndicesVector & faceNumbers1, const FaceIndicesVector & faceNumbers2 );
  /// \ru Конструктор по ребрам. \en Constructor by edges. 
  template <class EdgesVector>
  MbShellsIntersectionData( const EdgesVector &, const c3d::IndicesPairsVector & faceNumbersPairs );
  /// \ru Конструктор по телу. \en Constructor by a solid. 
  explicit MbShellsIntersectionData( const MbSolid & );
  /// \ru Конструктор по точкам. \en Constructor by points. 
  explicit MbShellsIntersectionData( const std::vector<MbCartPoint3D> & );
  /// \ru Конструктор по вершинам и флагу использования этих объектов, а не их копий. \en Constructor by vertices and by flag of use of these objects instead of their copies. 
  explicit MbShellsIntersectionData( const c3d::ConstVerticesVector &, bool same );
  /// \ru Конструктор по вершинам и флагу использования этих объектов, а не их копий. \en Constructor by vertices and by flag of use of these objects instead of their copies. 
  explicit MbShellsIntersectionData( const c3d::ConstVerticesSPtrVector &, bool same );
  /// \ru Деструктор. \en Destructor. 
  ~MbShellsIntersectionData();

public:
        /// \ru Пересечение - есть тело. \en Intersection is a solid. 
        bool            IsSolid()   const { return ((solid != nullptr) || (isSolid && !edges.empty())); }
        /// \ru Пересечение касательной областью поверхности. \en Intersection by a tangent region of a surface. 
        bool            IsSurface() const { return !isTangentCurve && !edges.empty(); } 
        /// \ru Пересечение вдоль касательной линии. \en Intersection along a tangent line. 
        bool            IsCurve()   const { return  isTangentCurve && !edges.empty(); }
        /// \ru Пересечение точкой (еще не реализовано). \en Intersection is a point (not implemented yet). 
        bool            IsPoint()   const { return ((pointFrame != nullptr) && (pointFrame->GetVerticesCount() > 0)); }

        /// \ru Установить флаг пересечения вдоль касательной линии. \en Set the flag of intersection along a tangent line. 
        //void            SetTangent( bool b ) { isTangentCurve = b; } 

        /// \ru Отдать указатель для просмотра тела. \en Get a pointer for viewing the solid. 
  const MbSolid     *   GetSolid() const { return solid; } 
        /// \ru Отдать указатель для просмотра/модификации тела. \en Get a pointer for viewing/modification of the solid. 
        MbSolid     *   SetSolid()       { return solid; }

        /// \ru Количество кривых пересечения. \en The number of intersection curves. 
        size_t          GetCurvesCount() const { return edges.size(); } 
        /// \ru Получить массив кривых пересечения. \en Get the intersection curve array. 
        template <class EdgesVector>
        void            GetCurves( EdgesVector & curves ) const;
        /// \ru Получить указатель на кривую пересечения по индексу. \en Get a pointer to an intersection curve by the index. 
  const MbCurveEdge *   GetCurve( size_t k ) const { return ((k < edges.size()) ? edges[k].get() : nullptr); }
        /// \ru Получить номера касающихся граней первого/второго тела. \en Get numbers concerning faces of the first/second solid. 
        template <class OutputIndicesVector>
        void            GetFaceNumbers( bool first, OutputIndicesVector & ) const;
        /// \ru Получить номера касающихся граней первого и второго тел. \en Get numbers concerning faces of the first and second solids. 
        template <class OutputIndicesPairsVector>
        void            GetFaceNumbersPairs( OutputIndicesPairsVector & ) const;

        /// \ru Количество точек касания. \en The number of touch points. 
        size_t          GetPointsCount() const { return ((pointFrame != nullptr) ? pointFrame->GetVerticesCount() : 0); }
        /// \ru Получить набор точек касания. \en Get a set of touch points. 
  const MbPointFrame *  GetPointFrame() const { return pointFrame; }

OBVIOUS_PRIVATE_COPY( MbShellsIntersectionData ) // \ru Не реализовано \en Not implemented 
};


//------------------------------------------------------------------------------
// \ru Конструктор по ребрам. \en Constructor by edges. 
//---
template <class EdgesVector>
MbShellsIntersectionData::MbShellsIntersectionData( const EdgesVector & initEdges, bool isSolidEgdes )
  : edges         (              )
  , faceIndices1  (              )
  , faceIndices2  (              )
  , solid         ( nullptr      )
  , pointFrame    ( nullptr      )
  , isTangentCurve( false        )
  , isSolid       ( isSolidEgdes )
{
  size_t addCnt = initEdges.size();
  if ( addCnt > 0 ) {
    c3d::EdgeSPtr edge;
    edges.reserve( addCnt );
    for ( size_t k = 0; k < addCnt; ++k ) {
      if ( initEdges[k] != nullptr ) {
        edge = const_cast<MbCurveEdge *>( &(*initEdges[k]) );
        edges.push_back( edge );
      }
    }
  }
}


//------------------------------------------------------------------------------
// \ru Конструктор по ребрам. \en Constructor by edges. 
//---
template <class EdgesVector, class FaceIndicesVector>
MbShellsIntersectionData::MbShellsIntersectionData( const EdgesVector &       initEdges, 
                                                    const FaceIndicesVector & faceInds1,
                                                    const FaceIndicesVector & faceInds2 )
  : edges         (         )
  , faceIndices1  (         )
  , faceIndices2  (         )
  , solid         ( nullptr )
  , pointFrame    ( nullptr )
  , isTangentCurve( false   )    
  , isSolid       ( false   )
{ 
  size_t edgesCnt = initEdges.size();

  if ( edgesCnt > 0 ) {
    c3d::EdgeSPtr edge;
    edges.reserve( edgesCnt );
    for ( size_t k = 0; k < edgesCnt; ++k ) {
      if ( initEdges[k] != nullptr ) {
        edge = const_cast<MbCurveEdge *>(&(*initEdges[k]));
        edges.push_back( edge );
      }
    }
    std::copy( faceInds1.begin(), faceInds1.end(), std::back_inserter( faceIndices1 ) );
    std::copy( faceInds2.begin(), faceInds2.end(), std::back_inserter( faceIndices2 ) );
  }
}


//------------------------------------------------------------------------------
// \ru Конструктор по ребрам. \en Constructor by edges. 
//---
template <class EdgesVector>
MbShellsIntersectionData::MbShellsIntersectionData( const EdgesVector &             initEdges,
                                                    const c3d::IndicesPairsVector & faceIndicesPairs )
  : edges         (         )
  , faceIndices1  (         )
  , faceIndices2  (         )
  , solid         ( nullptr )
  , pointFrame    ( nullptr )
  , isTangentCurve( false   )
  , isSolid       ( false   )
{
  size_t edgesCnt = initEdges.size();

  if ( edgesCnt > 0 ) {
    c3d::EdgeSPtr edge;
    edges.reserve( edgesCnt );
    size_t k;
    for ( k = 0; k < edgesCnt; ++k ) {
      if ( initEdges[k] != nullptr ) {
        edge = const_cast<MbCurveEdge *>(&(*initEdges[k]));
        edges.push_back( edge );
      }
    }
    size_t facePairsCnt = faceIndicesPairs.size();
    faceIndices1.reserve( facePairsCnt );
    faceIndices2.reserve( facePairsCnt );
    for ( k = 0; k < facePairsCnt; ++k ) {
      faceIndices1.push_back( faceIndicesPairs[k].first );
      faceIndices2.push_back( faceIndicesPairs[k].second );
    }
  }
}


//------------------------------------------------------------------------------
// \ru Получить массив кривых пересечения. \en Get the intersection curve array. 
//---
template <class EdgesVector>
void MbShellsIntersectionData::GetCurves( EdgesVector & dstEdges ) const
{
  size_t addCnt = edges.size();
  c3d::EdgeSPtr edge;
  dstEdges.reserve( dstEdges.size() + addCnt );
  for ( size_t k = 0; k < addCnt; ++k ) {
    edge = const_cast<MbCurveEdge *>( &(*edges[k]) );
    dstEdges.push_back( edge );
    ::DetachItem( edge );
  }
}


//------------------------------------------------------------------------------
// \ru Получить номера касающихся граней первого/второго тела. \en Get numbers concerning faces of the first/second solid. 
//---
template <class OutputIndicesVector>
void MbShellsIntersectionData::GetFaceNumbers( bool first, OutputIndicesVector & outputIndices ) const
{ 
  const c3d::IndicesVector & faceIndices = first ? faceIndices1 : faceIndices2;
  size_t addCnt = faceIndices.size();
  if ( addCnt > 0 ) {
    outputIndices.reserve( outputIndices.size() + addCnt );
    for ( size_t k = 0; k < addCnt; ++k ) {
      outputIndices.push_back( faceIndices[k] );
    }
  }
}


//------------------------------------------------------------------------------
// \ru Получить номера касающихся граней первого и второго тел. \en Get numbers concerning faces of the first and second solids. 
//---
template <class OutputIndicesPairsVector>
void MbShellsIntersectionData::GetFaceNumbersPairs( OutputIndicesPairsVector & outputIndicesPairs ) const
{
  size_t addCnt = std_min( faceIndices1.size(), faceIndices2.size() );
  if ( addCnt > 0 ) {
    outputIndicesPairs.reserve( outputIndicesPairs.size() + addCnt );
    for ( size_t k = 0; k < addCnt; ++k ) {
      outputIndicesPairs.push_back( std::make_pair( faceIndices1[k], faceIndices2[k] ) );
    }
  }
}


//------------------------------------------------------------------------------
/** \brief \ru Проверка вырожденности кривой в трехмерном пространстве.
           \en Check for the curve degeneration in three-dimensional space. \~
  \details \ru Проверка вырожденности кривой в трехмерном пространстве. \n
           \en Check for the curve degeneration in three-dimensional space. \n \~
  \param[in] curve - \ru Кривая.
                     \en Curve. \~
  \param[in] eps - \ru Неразличимая метрическая область, критерий вырождения кривой.
                   \en Indistinguishable metric domain, curve degeneration criterion. \~
  \return \ru Возвращает состояние вырожденности кривой.
          \en Returns the state of the curve degeneration. \~
  \ingroup Algorithms_3D
*/ //---
MATH_FUNC (bool) IsDegeneratedCurve( const MbCurve3D & curve, double eps );


//------------------------------------------------------------------------------
/** \brief \ru Проверка вырожденности поверхности в точке.
           \en Checking the degeneracy of a surface at a point.\~
  \details \ru Проверка вырожденности поверхности в точке.
           \en Checking the degeneracy of a surface at a point.\~
  \param[in] surf -  \ru Поверхность.
                     \en Surface. \~
  \param[in] u, v -  \ru Координаты точки на поверхности.
                     \en The coordinates of a point on a surface.\~
  \param[in] eps -   \ru Точность оценки. \n
                         Для линейной оценивается отношение минимальной к максимальной длин первых производных.\n
                         Для угловой оценивается угол между первыми производными. \~
                     \en Estimation accuracy. \n
                         For a linear one, the ratio of the minimum to the maximum lengths of the first derivatives is estimated.\n
                         For a angular one, the angle between the first derivatives is estimated.\~
  \param[in] degRu - \ru Оценивает вырожденность поверхности по длине производной Ru.
                     \en Estimates the degeneracy of a surface by the length of the derivative Ru.\~
  \param[in] degRv - \ru Оценивает вырожденность поверхности по длине производной Rv.
                     \en Estimates the degeneracy of a surface by the length of the derivative Rv.\~
  \param[in] degRuv- \ru Оценивает вырожденность поверхности по углу между Ru и Rv.
                     \en Estimates the degeneracy of a surface by the angle between Ru and Rv.\~
  \return \ru Возвращает статус вырожденности.
          \en Returns the degeneracy status. \~
  \ingroup Algorithms_3D
*/ //---
MATH_FUNC (bool) CheckSurfaceDegeneracy( const MbSurface & surf, double u, double v, double eps, 
                                         bool & degRu, bool & degRv, bool & colUV );
  

//------------------------------------------------------------------------------
/** \brief \ru Проверка оболочки тела на замкнутость.
           \en Check of solid's shell for closedness. \~
  \details \ru Проверка оболочки тела на замкнутость. \n
           \en Check of solid's shell for closedness. \n \~
  \param[in] shell - \ru Оболочка.
                     \en A shell. \~
  \param[in] checkChangedOnly - \ru Проверять только измененные грани оболочки.
                                \en Only modified faces of a shell are to be checked. \~
  \return \ru Возвращает состояние замкнутости оболочки.
          \en Returns the state of shell closedness. \~
  \ingroup Algorithms_3D
*/ //---
MATH_FUNC (bool) CheckShellClosure( const MbFaceShell & shell, bool checkChangedOnly = false );


//------------------------------------------------------------------------------
/** \brief \ru Проверка оболочки тела на замкнутость.
           \en Check of solid's shell for closedness. \~
  \details \ru Проверка оболочки тела на замкнутость. \n
           \en Check of solid's shell for closedness. \n \~
  \ingroup Algorithms_3D
*/ //---
MATH_FUNC (bool) CheckSolidClosure( const MbSolid & solid );



////////////////////////////////////////////////////////////////////////////////
//
// \ru Функции для проверки элементов оболочки \en Functions for checking shell's elements 
//
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
/** \brief \ru Поиск краевых ребер замкнутой оболочки.
           \en Search for the boundary edges of a closed shell. \~
  \details \ru Поиск краевых ребер замкнутой оболочки. \n
               Краевое ребро - это ребро у которого нет ссылки на одну из смежных граней. \n
               Наличие краевых ребер замкнутой оболочки может приводит к отказу операций над оболочкой,
               если операцией будет затронута часть оболочки с краевыми ребрами. \n
               Наличие одиночных краевых ребер практически никак не влияет на правильность расчета МЦХ.
               Множественные краевые ребра, особенно в виде связных цепочек, являются серьезным дефектом замкнутой оболочки. \n
           \en Search for the boundary edges of a closed shell. \n
               Boundary edge is an edge that has no reference to one of the adjacent faces. \n
               The presence of boundary edges of a closed shell may lead to failure of operations on the shell, 
               if the operation affects a part of the shell with such edges. \n
               The presence of single boundary edges has practically no effect on the correctness of the MIP calculation. \n 
               Multiple boundary edges, especially in the form of related chains, is a serious defect of the closed shell. \n \~
  \param[in] allEdges - \ru Множество ребер оболочки.
                        \en Set of edges of a shell. \~
  \param[in] boundaryEdges - \ru Множество найденных краевых ребер.
                             \en Set of found boundary edges. \~
  \return \ru Возвращает true, если найдено хотя бы одно краевое ребро.
          \en Returns true if at least one boundary edges is found. \~
  \ingroup Algorithms_3D
*/ // ---
template <class EdgesVector>
bool CheckBoundaryEdges( const EdgesVector & allEdges, EdgesVector * boundaryEdges )
{
  bool isBoundary = false;
  C3D_ASSERT( boundaryEdges != &allEdges );

  if ( boundaryEdges != &allEdges ) {
    for ( size_t i = 0, cnt = allEdges.size(); i < cnt; ++i ) {
      if ( allEdges[i] != nullptr && allEdges[i]->IsBoundaryFace( METRIC_PRECISION ) ) {
        isBoundary = true;
        if ( boundaryEdges != nullptr )
          boundaryEdges->push_back( allEdges[i] );
        else
          break;
      }
    }
  }

  return isBoundary;
}


//------------------------------------------------------------------------------
/** \brief \ru Поиск некорректных ребер.
           \en Search of incorrect edges. \~
  \details \ru Поиск некорректных ребер. Не ищет краевые ребра замкнутой оболочки. \n
               Для поиска краевых ребер используйте функцию CheckBoundaryEdges. \n
               Функция проверяет следующие варианты некорректности ребер :
               1. Ребро с типом граница (cbt_Boundary) должно указывать только на одну грань \n
               2. Поверхности в кривой пересечения ребра должны быть такие же как и поверхности в смежных гранях ребра \n
               3. Граничные точки поверхностных кривых в кривой пересечения ребра должны совпадать с точностью не хуже 1e-6 или толерантности в вершинах ребра \n
               4. Опорные точки сплайнов поверхностных кривых в уточняемой кривой пересечения (cbt_Specific) должны совпадать в пространстве c точностью не хуже 1e-6 \n
               Наличие некорректных ребер является серьезным дефектом оболочки. \n
           \en Search of incorrect edges. Does not look for the boundary edges of a closed shell. \n
               Use function CheckBoundaryEdges for searching for boundary edges. \n 
               The function checks the next parameters of an edge as signs of its incorrectness :
               1. An edge with the border type cbt_Boundary must point to only one face. \n
               2. The surfaces of the intersection curve of the edge have to be the same as the surfaces in the adjacent faces of the edge. \n
               3. The boundary points of the surface curves in the curve of intersection of the edge must coincide with an accuracy not worse than 1e-6 or tolerance at the vertices of the edge. \n
               4. The reference points of the splines of the surface curves in the intersection curve with the border type cbt_Specific must coincide in space with an accuracy not worse than 1e-6. \n
               The presence of incorrect edges is a serious defect of the shell. \n \~
  \param[in] allEdges - \ru Множество ребер оболочки.
                        \en Set of edges of a shell. \~
  \param[in] badEdges - \ru Множество найденных некорректных ребер.
                        \en Set of found incorrect edges. \~
  \return \ru Возвращает true, если найдено хотя бы одно некорректное ребро.
          \en Returns true if at least one incorrect edge is found. \~
  \ingroup Algorithms_3D
*/ // ---
MATH_FUNC (bool) CheckBadEdges( const c3d::ConstEdgesVector & allEdges,
                                      c3d::ConstEdgesVector * badEdges );


//------------------------------------------------------------------------------
/** \brief \ru Поиск неточных вершин.
           \en Search for inexact vertices. \~
  \details \ru Поиск неточных вершин оболочки. \n
               Наличие неточных вершин не является серьезным дефектом оболочки.
               В большинстве случаев никак не влияет на работу операций с оболочкой.
               Не влияет на расчет МЦХ. \n
           \en Search for inexact vertices of a shell. \n 
               The presence of inaccurate vertices is not a serious shell defect. 
               In most cases, does not affect on the result of operations with this shell. 
               Does not affect the calculation of the MIP. \n \~
  \param[in] vertArr - \ru Множество вершин оболочки.
                       \en Set of shell's vertices. \~
  \param[in] mAcc - \ru Порог отбора неточных вершин.
                    \en Accuracy of inexact vertices filtration. \~
  \param[in] inexactVerts - \ru Множество для неточных вершин.
                            \en Set of inexact vertices. \~
  \return \ru Возвращает true, если найдена хотя бы одна неточная вершина.
          \en Returns true if at least one inexact vertex is found. \~
  \ingroup Algorithms_3D
*/ // ---
template <class VerticesVector>
bool CheckInexactVertices( const VerticesVector & vertArr, double mAcc, VerticesVector * inexactVerts )
{
  bool isInexactVertex = false;
  C3D_ASSERT( inexactVerts != &vertArr );

  if ( inexactVerts != &vertArr ) {
    for ( size_t i = 0, icnt = vertArr.size(); i < icnt; ++i ) {
      if ( vertArr[i] != nullptr && vertArr[i]->GetTolerance() > mAcc ) {
        isInexactVertex = true;
        if ( inexactVerts != nullptr ) {
          inexactVerts->push_back( vertArr[i] );
        }
        else {
          break;
        }
      }
    }
  }

  return isInexactVertex;
}


//------------------------------------------------------------------------------
/** \brief \ru Является ли кривая пересечения неточной.
           \en Is the curve of intersection inaccurate. \~
  \details \ru Является ли кривая пересечения неточной (оценочно). \n
               Наличие неточных кривых пересечения не является серьезным дефектом.
               В большинстве случаев никак не влияет на результат операций.
               Незначительно влияет на расчет МЦХ. \n
           \en Is the curve of intersection inaccurate (estimated). \n 
               The presence of inaccurate curves of intersection is not a serious defect.
               In most cases, does not affect on the result of operations.
               Can slightly affect the calculation of the MIP. \n \~
  \param[in] curve - \ru Кривая пересечения.
                    \en The curve of intersection. \~
  \param[in] mMaxAcc - \ru Порог отбора неточного ребра.
                       \en Accuracy selection inaccurate curves of intersection. \~
  \return \ru Возвращает true, если кривая пересечения неточная.
          \en Returns true, if the curve of intersection is inaccurate. \~
  \ingroup Algorithms_3D
*/ //---
MATH_FUNC (bool) IsInexactIntersectionCurve( const MbSurfaceIntersectionCurve & curve, double mMaxAcc );


//------------------------------------------------------------------------------
/** \brief \ru Является ли кривая пересечения ребра неточной.
           \en Is the curve of intersection edges inaccurate. \~
  \details \ru Является ли кривая пересечения ребра неточной (оценочно). \n
               Наличие неточных ребер (кривых пересечения) не является серьезным дефектом оболочки.
               В большинстве случаев никак не влияет на работу операций с оболочкой.
               Незначительно влияет на расчет МЦХ. \n
           \en Is the curve of intersection edges inaccurate (estimated). \n 
               The presence of inaccurate edges is not a serious shell defect.
               In most cases, does not affect on the result of operations with this shell.
               Can slightly affect the calculation of the MIP. \n \~
  \param[in] edge - \ru Ребро оболочки.
                    \en The edge of the shell. \~
  \param[in] mMaxAcc - \ru Порог отбора неточного ребра.
                       \en Accuracy selection inaccurate edges. \~
  \return \ru Возвращает true, если ребро неточное.
          \en Returns true, if the edge is inaccurate. \~
  \ingroup Algorithms_3D
*/ // ---
MATH_FUNC (bool) IsInexactEdge( const MbCurveEdge & edge, double mMaxAcc );


//------------------------------------------------------------------------------
/** \brief \ru Поиск неточных ребер оценочно.
           \en Approximate search of inexact edges. \~
  \details \ru Поиск неточных ребер оболочки оценочно. \n
               Наличие неточных ребер (кривых пересечения) не является серьезным дефектом оболочки.
               В большинстве случаев никак не влияет на работу операций с оболочкой.
               Незначительно влияет на расчет МЦХ. \n
           \en Approximate search of inexact edges of a shell. \n
               The presence of inaccurate edges is not a serious shell defect.
               In most cases, does not affect on the result of operations with this shell.
               Can slightly affect the calculation of the MIP. \n \~
  \param[in] allEdges - \ru Множество ребер оболочки.
                        \en Set of edges of a shell. \~
  \param[in] mAcc - \ru Порог отбора неточных ребер.
                    \en Accuracy of inexact edges filtration. \~
  \param[in] inexactEdges - \ru Множество найденных неточных ребер.
                            \en Set of found inexact edges. \~
  \return \ru Возвращает true, если найдено хотя бы одно неточное ребро.
          \en Returns true if at least one inexact edge is found. \~
  \ingroup Algorithms_3D
*/ // ---
template <class EdgesVector>
bool CheckInexactEdges( const EdgesVector & allEdges, double mAcc, EdgesVector * inexactEdges )
{
  bool isInexactEdge = false;

  for ( size_t i = 0, icnt = allEdges.size(); i < icnt; ++i ) {
    if ( allEdges[i] != nullptr) {
      bool isSpaceNear = !::IsInexactEdge( *allEdges[i], mAcc );

      if ( !isSpaceNear ) {
        isInexactEdge = true;
        if ( inexactEdges != nullptr )
          inexactEdges->push_back( allEdges[i] );
        else
          break;
      }
      else if ( !allEdges[i]->IsClosed() ) {
        const MbVertex & v1 = allEdges[i]->GetBegVertex();
        const MbVertex & v2 = allEdges[i]->GetEndVertex();
        if ( &v1 == &v2 ) {
          double mTol = v1.GetTolerance();
          double mLen = allEdges[i]->GetLengthEvaluation();
          if ( mLen > METRIC_PRECISION && mLen > mTol + mAcc ) {
            MbCartPoint3D p1, p2;
            allEdges[i]->Point( 0.0, p1 );
            allEdges[i]->Point( 1.0, p2 );
            double mMinAcc = std_min( mAcc, mTol );

            if ( !c3d::EqualPoints( p1, p2, mMinAcc ) ) { // SD#7353885
              isInexactEdge = true;
              if ( inexactEdges != nullptr )
                inexactEdges->push_back( allEdges[i] );
              else
                break;
            }
          }
        }
      }
    }
  }

  return isInexactEdge;
}


//------------------------------------------------------------------------------
/** \brief \ru Проверка подложек и указаний на грани.
           \en Check of substrates and pointers to faces. \~
  \details \ru Проверка подложек и указаний на грани оболочки. \n
               Наличие общих подложек (базовые поверхности в ограниченных кривыми поверхностях) 
               и неверных ссылок на грани в ребрах является серьезным дефектом оболочки. \n
           \en Check of substrates and pointers to faces of a shell. \n 
               The presence of common substrates (base surfaces in bounded curved surfaces) 
               and invalid references to faces in edges is a serious shell defect. \n \~
  \param[in] shell - \ru Проверяемая оболочка.
                     \en A shell to check. \~
  \param[out] areIdenticalBaseSurfaces - \ru Наличие общих подложек.
                                         \en Whether there are common substrates. \~
  \param[out] areBadFacePointers - \ru Наличие неверных указателей на соседние грани.
                                   \en Whether there are invalid pointers to neighboring faces. \~
  \ingroup Algorithms_3D
*/ // ---
MATH_FUNC (void) CheckBadFaces( const MbFaceShell & shell,
                                      bool &        areIdenticalBaseSurfaces,
                                      bool &        areBadFacePointers );


//------------------------------------------------------------------------------
/** \brief \ru Проверка взаимного расположения циклов грани.
           \en Check interposition of face loops. \~
  \details \ru Проверка взаимного расположения циклов грани.
               Функция проверять корректность ориентации циклов грани.
               Неправильная ориентация циклов граней является серьезным дефектом оболочки.
           \en Check interposition of face loops. \n 
               The function is to check the correctness of the orientation of the face loops (chains of oriented edges). 
               Incorrect orientation of face's loops is a serious defect in the shell. \n \~
  \param[in] face - \ru Грань.
                    \en Face. \~
  \return \ru Возвращает true, если расположение и ориентация циклов корректны.
          \en Returns true if interposition of loops and their orientations are correct. \~
  \ingroup Algorithms_3D
*/ //---
MATH_FUNC (bool) CheckLoopsInterposition( const MbFace & face );


//------------------------------------------------------------------------------
/** \brief \ru Проверка связности ребер цикла.
           \en Check for connectivity of loop edges. \~
  \details \ru Проверка связности ребер цикла грани.
               Возвращает максимальные метрическую и параметрическую (опционально) погрешности построения цикла. \n
               Наличие неточных стыковок в циклах грани не обязательно является серьезным дефектом оболочки. \n
           \en Check for connectivity of loop edges.
               Returns the maximal metric and parametric (optionally) tolerances of the loop construction. \n
               The presence of inaccurate connection in face loops (chains of oriented edges) is not necessarily a serious shell defect. \n \~
  \param[in] face - \ru Грань, содержащая проверяемый цикл.
                    \en Face containing the loop under test. \~
  \param[in] loop - \ru Цикл грани.
                    \en Face loop. \~
  \param[out] lengthTolerance - \ru Максимальное метрическое значение разрыва между ребрами.
                                \en The maximal metric value of a gap between edges. \~
  \param[out] paramTolerance  - \ru Максимальное параметрическое значение разрыва между ребрами.
                                \en The maximal parametric value of a gap between edges. \~
  \param[out] badLocs         - \ru Пары номеров ориентированных ребер с плохой связностью.
                                \en Edges pairs with bad connectivity. \~
  \return \ru Возвращает true, если связность ребер не нарушена.
          \en Returns true if the connectivity is good. \~
  \ingroup Algorithms_3D
*/ //---
MATH_FUNC (bool) CheckLoopConnection( const MbFace &                  face, 
                                      const MbLoop &                  loop, 
                                            double &                  lengthTolerance, 
                                            double &                  paramTolerance,
                                            c3d::IndicesPairsVector & badLocs );


//------------------------------------------------------------------------------
/** \brief \ru Проверка связности ребер цикла.
           \en Check for connectivity of a loop edges. \~
  \details \ru Проверка связности ребер цикла грани.
               Возвращает максимальные метрическую и параметрическую (опционально) погрешности построения цикла. \n
               Наличие неточных стыковок в циклах грани не обязательно является серьезным дефектом оболочки. \n
           \en Check for connectivity of a loop edges.
               Returns the maximal metric and parametric (optionally) tolerances of the loop construction. \n
               The presence of inaccurate connection in face loops (chains of oriented edges) is not necessarily a serious shell defect. \n \~
  \param[in] face - \ru Грань, содержащая проверяемый цикл.
                    \en Face containing the loop under test. \~
  \param[in] loop - \ru Цикл грани.
                    \en Face loop. \~
  \param[out] lengthTolerance - \ru Максимальное метрическое значение разрыва между ребрами.
                                \en The maximal metric value of a gap between edges. \~
  \param[out] paramTolerance  - \ru Максимальное параметрическое значение разрыва между ребрами.
                                \en The maximal parametric value of a gap between edges. \~
  \param[out] badConnectedEdges - \ru Ребра с плохой связностью.
                                  \en Edges with bad connectivity. \~
  \param[out] badVertexEdges - \ru Ребра с неправильными вершинами.
                               \en Edges with incorrect vertices. \~
  \return \ru Возвращает true, если связность ребер не нарушена.
          \en Returns true if the connectivity is good. \~
  \ingroup Algorithms_3D
*/ //---
MATH_FUNC (bool) CheckLoopConnection( const MbFace &                      face, 
                                      const MbLoop &                      loop, 
                                            double &                      lengthTolerance, 
                                            double *                      paramTolerance,
                                            c3d::ConstOrientEdgesVector & badConnectedEdges,
                                            c3d::ConstEdgesVector &       badVertexEdges );


//------------------------------------------------------------------------------
/** \brief \ru Проверка связности ребер цикла.
           \en Check for connectivity of a loop edges. \~
  \details \ru Проверка связности ребер цикла грани.
               Возвращает максимальные метрическую и параметрическую (опционально) погрешности построения цикла. \n
               Наличие неточных стыковок в циклах грани не обязательно является серьезным дефектом оболочки. \n
           \en Check for connectivity of a loop edges.
               Returns the maximal metric and parametric (optionally) tolerances of the loop construction. \n 
               The presence of inaccurate connection in face loops (chains of oriented edges) is not necessarily a serious shell defect. \n \~
  \param[in] face - \ru Грань, содержащая проверяемый цикл.
                    \en Face containing the loop under test. \~
  \param[in] loop - \ru Цикл грани.
                    \en Face loop. \~
  \param[out] lengthTolerance - \ru Максимальное метрическое значение разрыва между ребрами.
                                \en The maximal metric value of a gap between edges. \~
  \param[out] paramTolerance  - \ru Максимальное параметрическое значение разрыва между ребрами.
                                \en The maximal parametric value of a gap between edges. \~
  \param[out] badConnectedEdges - \ru Ребра с плохой связностью.
                                  \en Edges with bad connectivity. \~
  \return \ru Возвращает true, если связность ребер не нарушена.
          \en Returns true if the connectivity is good. \~
  \ingroup Algorithms_3D
*/ //---
MATH_FUNC (bool) CheckLoopConnection( const MbFace &                      face, 
                                      const MbLoop &                      loop, 
                                            double &                      lengthTolerance, 
                                            double *                      paramTolerance,
                                            c3d::ConstOrientEdgesVector & badConnectedEdges );


//------------------------------------------------------------------------------
/** \brief \ru Проверка связности ребер цикла.
           \en Check for connectivity of a loop edges. \~
  \details \ru Проверка связности ребер цикла грани.
               Возвращает максимальные метрическую и параметрическую (опционально) погрешности построения цикла. \n
               Наличие неточных стыковок в циклах грани не обязательно является серьезным дефектом оболочки. \n
           \en Check for connectivity of a loop edges.
               Returns the maximal metric and parametric (optionally) tolerances of the loop construction. \n
               The presence of inaccurate connection in face loops (chains of oriented edges) is not necessarily a serious shell defect. \n \~
  \param[in] face - \ru Грань, содержащая проверяемый цикл.
                    \en Face containing the loop under test. \~
  \param[in] loop - \ru Цикл грани.
                    \en Face loop. \~
  \param[out] lengthTolerance - \ru Максимальное метрическое значение разрыва между ребрами.
                                \en The maximal metric value of a gap between edges. \~
  \param[out] paramTolerance  - \ru Максимальное параметрическое значение разрыва между ребрами.
                                \en The maximal parametric value of a gap between edges. \~
  \return \ru Возвращает true, если связность ребер не нарушена.
          \en Returns true if the connectivity is good. \~
  \ingroup Algorithms_3D
*/ //---
MATH_FUNC (bool) CheckLoopConnection( const MbFace & face, 
                                      const MbLoop & loop, 
                                            double & lengthTolerance, 
                                            double * paramTolerance );


//------------------------------------------------------------------------------
/** \brief \ru Найти циклы грани с самопересечениями.
           \en Find face loops with self-intersections. \~
  \details \ru Найти циклы грани с самопересечениями.
               Возвращает найденные циклы с самопересечениям. \n
               Наличие самопересечений в циклах граней является серьезным дефектом оболочки. \n
           \en Find face loops with self-intersections.
               Returns the found loops with self-intersections. \n 
               The presence of self-intersections in face loops is a serious shell defect. \n \~
  \param[in] face - \ru Грань, содержащая проверяемые циклы.
                    \en Face containing loops under test. \~
  \param[in] nameMaker - \ru Именователь.
                         \en An object for naming the new objects. \~
  \param[in] checkInsideEdges - \ru Искать самопересечения внутри области определения двумерных кривых ребер.
                                \en Find edges with self-intersections inside. \~
  \param[out] loopPnts - \ru Точки самопересечения c номерами циклов.
                         \en Points of self-intersecting loops and the numbers of loops. \~
  \return \ru Возвращает true, если найдены самопересечения циклов.
          \en Returns true if the self-intersection has been found. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (bool) FindLoopsSelfIntersections( const MbFace & face, const MbSNameMaker & nameMaker, bool checkInsideEdges,
                                             std::vector< std::pair<c3d::IndicesPair, MbCartPoint3D> > * loopPnts );


//------------------------------------------------------------------------------
/** \brief \ru Проверка связности граней faces.
           \en Check for connectivity of faces 'faces'. \~
  \details \ru Проверка топологической связности граней faces. \n
           \en Check for topological connectivity of faces 'faces'. \n \~
  \param[in] faces - \ru Проверяемый набор граней.
                     \en Set of faces under check. \~
  \return \ru Возвращает true, все грани топологически связаны.
          \en Returns true if all the faces are topologically connected. \~
  \ingroup Algorithms_3D
*/ //---
MATH_FUNC (bool) CheckFacesConnection( const RPArray<MbFace> & faces );


//------------------------------------------------------------------------------
/** \brief \ru Проверка связности граней faces.
           \en Check for connectivity of faces 'faces'. \~
  \details \ru Проверка топологической связности граней faces. \n
           \en Check for topological connectivity of faces 'faces'. \n \~
  \param[in] faces - \ru Проверяемый набор граней.
                     \en Set of faces under check. \~
  \return \ru Возвращает true, все грани топологически связаны.
          \en Returns true if all the faces are topologically connected. \~
  \ingroup Algorithms_3D
*/ //---
MATH_FUNC (bool) CheckFacesConnection( const c3d::FacesVector & faces );


//------------------------------------------------------------------------------
/** \brief \ru Проверка связности граней faces.
           \en Check for connectivity of faces 'faces'. \~
  \details \ru Проверка топологической связности граней faces. \n
           \en Check for topological connectivity of faces 'faces'. \n \~
  \param[in] faces - \ru Проверяемый набор граней.
                     \en Set of faces under check. \~
  \return \ru Возвращает true, все грани топологически связаны.
          \en Returns true if all the faces are topologically connected. \~
  \ingroup Algorithms_3D
*/ //---
MATH_FUNC (bool) CheckFacesConnection( const c3d::ConstFacesVector & faces );


//------------------------------------------------------------------------------
/** \brief \ru Найти в исходной оболочке "родительские" грани производной оболочки.
           \en Find "parent" faces of a derived shell in the initial shell. \~
  \details \ru Найти в исходной оболочке "родительские" грани производной оболочки геометрическим поиском подобных граней с наложением. \n
               Флаг sameNormals установить false, если исходная оболочка участвовала в булевом вычитании тел вторым операндом. \n
           \en Find "parent" faces of a derived shell in the initial shell by geometric search of similar faces with overlapping. \n
               Flag sameNormals is to be set to false if the initial shell was involved in the boolean subtraction of solids as a second operand. \n \~
  \param[in] srcShell - \ru Исходная оболочка.
                        \en The source shell. \~
  \param[in] dstShell - \ru Производная оболочка.
                        \en The derived shell. \~
  \param[in] sameNormals - \ru Искать с одинаковым (true) или противоположным (false) направлением нормалей.
                           \en Search with the same (true) or the opposite (false) direction of normals. \~
  \param[out] simPairs - \ru Множество соответствий - номеров граней в исходной и производной оболочках.
                         \en Set of correspondences - indices of faces in the initial and the derived shells. \~
  \return \ru Возвращает true, все найдено хоть одно соответствие.
          \en Returns true if at least one correspondence is found. \~
  \ingroup Algorithms_3D
*/ //---
MATH_FUNC (bool) FindOverlappingSimilarFaces( const MbFaceShell &             srcShell, 
                                              const MbFaceShell &             dstShell, 
                                                    bool                      sameNormals,
                                                    c3d::IndicesPairsVector & simPairs );


//------------------------------------------------------------------------------
/** \brief \ru Найти на каких гранях исходной оболочки базируются ребра производной оболочки.
           \en Find faces edges of the derived shell are based on. \~
  \details \ru Найти на каких гранях исходной оболочки базируются ребра производной оболочки геометрическим поиском. 
    Поиск соответствия проводится по поверхностям из граней, на которые ссылается ребро, а не по поверхностям в кривой пересечения ребра. 
    Флаг sameNormals установить false, если исходная оболочка участвовала в булевом вычитании тел вторым операндом. \n
           \en Determine on which faces of the initial shell edges of the derived shell are based on by the geometric search. 
    Search of the correspondence is performed by surfaces from faces the edge refers to, but not by surfaces from the intersection curve of the edge. 
    Flag sameNormals is to be set to false if the initial shell was involved in the boolean subtraction of solids as a second operand. \n \~
  \param[in] edges - \ru Ребра производной оболочки.
                     \en Edges of an arbitrary shell. \~
  \param[in] shell - \ru Исходная оболочка.
                     \en The source shell. \~
  \param[in] sameNormals - \ru Искать с одинаковым (true) или противоположным (false) направлением нормалей.
                           \en Search with the same (true) or the opposite (false) direction of normals. \~
  \param[out] efPairs - \ru Множество соответствий - номеров ребер во входном массиве и номеров граней в исходной оболочке.
                        \en Set of correspondence - indices of edges in the input array and numbers of faces in the input shell. \~
  \return \ru Возвращает true, все найдено хоть одно соответствие.
          \en Returns true if at least one correspondence is found. \~
  \ingroup Algorithms_3D
*/ //---
MATH_FUNC (bool) FindFacesEdgesCarriers( const c3d::ConstEdgesVector &   edges, 
                                         const MbFaceShell &             shell, 
                                               bool                      sameNormals,
                                               c3d::IndicesPairsVector & efPairs );

//------------------------------------------------------------------------------
/** \brief \ru Починить некорректное ребро оболочки.
           \en Repair incorrect edge of a shell. \~
  \details \ru Починить некорректное ребро оболочки (псевдо-толерантное, псевдо-точное). \n
           \en Repair incorrect edge of a shell (pseudo-tolerant, pseudo-exact). \n \~
  \param[in,out] edge - \ru Ребро оболочки.
                        \en Shell edge. \~
  \param[in] updateFacesBounds - \ru Обновить границы поверхностей в гранях ребра.
                                 \en Update surface bounds of edge faces. \~
  \return \ru Возвращает true, если была выполнена модификация ребра.
          \en Returns true if edge modification was performed. \~
  \ingroup Algorithms_3D
*/ //---
MATH_FUNC( bool ) RepairEdge( MbCurveEdge & edge, bool updateFacesBounds );


//------------------------------------------------------------------------------
/** \brief \ru Починить некорректные ребра оболочки.
           \en Repair incorrect edges of a shell. \~
  \details \ru Починить некорректное ребро оболочки (псевдо-толерантное, псевдо-точное). \n
           \en Repair incorrect edge of a shell (pseudo-tolerant, pseudo-exact). \n \~
  \param[in,out] shell - \ru Оболочка.
                         \en Shell. \~
  \param[in] updateFacesBounds - \ru Обновить границы поверхностей в гранях ребра.
                                 \en Update surface bounds of edge faces. \~
  \return \ru Возвращает true, если была выполнена модификация ребра.
          \en Returns true if edge modification was performed. \~
  \ingroup Algorithms_3D
*/ //---
MATH_FUNC( bool ) RepairEdges( MbFaceShell & shell, bool updateFacesBounds = true );


//------------------------------------------------------------------------------
/** \brief \ru Починить некорректные вершины оболочки.
           \en Repair incorrect vertices of a shell. \~
  \details \ru Починить некорректные вершины оболочки : \n
               - установить одну общую вершину в средней точке стыковки ребёр, \n
               - уточнить толерантность вершины, если ее нет или она меньше реальной. \n
           \en Repair incorrect vertices of a shell : \n 
               - set single vertex at the midpoint of the edge junction, \n
               - clarify the tolerance of the vertex if it's not there or it 's less than the real one. \n ~
  \param[in] shell - \ru Оболочка.
                     \en Shell. \~
  \return \ru Возвращает true, если была выполнена модификация хотя бы одной вершины.
          \en Returns true if at least one vertex has been modified. \~
  \ingroup Algorithms_3D
*/ //---
MATH_FUNC( bool ) RepairVertices( MbFaceShell & shell );


//------------------------------------------------------------------------------
/** \brief \ru Устранить наличие общих подложек поверхностей.
           \en Remove common surface substrates. \~
  \details \ru Найти и устранить общие поверхности-подложки в гранях оболочки. \n
           \en Find and eliminate common underlying surfaces of a shell faces. \n \~
  \param[in,out] shell - \ru Модифицируемая оболочка.
                         \en A shell to be modified. \~
  \param[in] checkEdges - \ru Выполнить замену в ребрах.
                          \en Replace in shell edges. \~
  \return \ru Возвращает true, если была выполнена модификация оболочки.
          \en Returns true if the shell modification was performed. \~
  \ingroup Algorithms_3D
*/ //---
MATH_FUNC( bool ) RemoveCommonSurfaceSubstrates( MbFaceShell & shell, bool checkEdges = true );


//------------------------------------------------------------------------------
/** \brief \ru Сбросить габариты поверхностей.
           \en Reset bounding boxes of surfaces. \~
  \details \ru Сбросить габариты поверхностей и обновить параметрические пределы базовых поверхностей. \n
           \en Reset bounding boxes of surfaces and update parametric limits of basis surfaces. \n \~
  \param[in,out] shell - \ru Модифицируемая оболочка.
                         \en A shell to be modified. \~
  \param[in] updateParameticLimits - \ru Обновить параметрические пределы поверхностей.
                                     \en Update parametric limits of surfaces. \~
  \return \ru Возвращает true, если была выполнена модификация оболочки.
          \en Returns true if the shell modification was performed. \~
  \ingroup Algorithms_3D
*/ //---
MATH_FUNC( bool ) ResetSurfacesBoundingBoxes( MbFaceShell & shell, bool updateBasisParameticLimits = false );


//------------------------------------------------------------------------------
/** \brief \ru Сбросить габариты поверхности.
           \en Reset bounding boxes of a surface. \~
  \details \ru Сбросить габариты поверхности и обновить параметрические пределы ее базовой поверхности. \n
           \en Reset bounding boxes of a surface and update parametric limits of its basis surface. \n \~
  \param[in,out] surface - \ru Модифицируемая поверхность.
                           \en A surface to be modified. \~
  \param[in] updateParameticLimits - \ru Обновить параметрические пределы поверхностей.
                                     \en Update parametric limits of surfaces. \~
  \return \ru Возвращает true, если была выполнена модификация поверхности.
          \en Returns true if the surface modification was performed. \~
  \ingroup Algorithms_3D
*/ //---
MATH_FUNC( bool ) ResetSurfaceBoundingBoxes( MbSurface & surface, bool updateBasisParameticLimits = false );


//------------------------------------------------------------------------------
/** \brief \ru Сбросить все временные данные у кривых.
           \en Reset all temporary data of curves. \~
  \details \ru Сбросить все временные данные у кривых пересечения ребер оболочки. \n
           \en Reset all temporary data of curves of shell edges. \n \~
  \param[in,out] shell - \ru Модифицируемая оболочка.
                         \en A shell to be modified. \~
  \return \ru Возвращает true, если была выполнена модификация кривых.
          \en Returns true if the curve modification was performed. \~
  \ingroup Algorithms_3D
*/ //---
MATH_FUNC( bool ) ResetCurvesTemporaryData( MbFaceShell & shell );


//------------------------------------------------------------------------------
/** \brief \ru Проверка ориентированности оболочки наружу.
           \en Checking shell orientation. \~
  \details \ru Проверка ориентированности оболочки наружу по угловым точкам расширенного габарита. \n
           \en Checking the shell orientation outward by the corner points of the extended bounding box. \n \~
  \param[in] shell - \ru Оболочка.
                     \en A shell. \~
  \return \ru Возвращает состояние ориентации оболочки (ts_positive - наружу, ts_negative - внутрь, ts_neutral - не получилось определить).
          \en Returns the orientation state of the shell (ts_positive - outward, ts_negative - inward, ts_neutral - failed to determine). \~
  \ingroup Algorithms_3D
*/ //---
MATH_FUNC( ThreeStates ) IsOrientedOutward( const MbFaceShell & shell );


//------------------------------------------------------------------------------
/** \brief \ru Проверить, что имена вершин и рёбер содержат одно из главных имена родителей (рёбер и граней соответственно).
           \en Check whether vertices and edges names contain a main name of one of the parents (edges and faces corresponding). \~
  \details \ru Проверить, что имена вершин и рёбер содержат одно из главных имена родителей (рёбер и граней соответственно).
           \en Check whether vertices and edges names contain a main name of one of the parents (edges and faces corresponding). \~
  \param[in] shell - \ru Оболочка, которую проверяем.
                     \en Shell to check. \~
  \param[in] edges - \ru Выходной массив рёбер, не содержащих главное имя ни одной из смежных граней.
                     \en Resulting array of edges which do not contain a main name of any of the adjacent faces. \~
  \param[in] vertices - \ru Выходной массив вершин, не содержащих главное имя ни одного из смежных рёбер.
                        \en Resulting array of vertices which do not contain a main name of any of the adjacent edges. \~
  \return \ru Возвращает true, если все имена вершин и рёбер содержат одно из главных имена родителей (рёбер и граней соответственно).
          \en Returns true if all of the vertices and edges names contain a main name of one of the parents (edges and faces corresponding).\~
  \ingroup Algorithms_3D
*/ //---
MATH_FUNC (bool) CheckTopologyItemsMainNames( const MbFaceShell &              shell, 
                                                    c3d::ConstEdgesVector &    edges, 
                                                    c3d::ConstVerticesVector & vertices );


//------------------------------------------------------------------------------
/** \brief \ru Добавить в рёбра атрибуты гладкости ребра.
           \en Add edges smoothness attributes. \~
  \details \ru Добавить в рёбра атрибуты с информацией о гладкости стыковки граней в ребре.
           \en Add attributes to edges with information about the smoothness of joining faces in the edge. \~
  \param[in,out] shell - \ru Оболочка, которую обрабатываем.
                         \en Shell to processing. \~
  \param[in] skipExisting - \ru Пропустить уже существующие (не обновлять в них данные).
                            \en Skip existing ones (do not update data in them). \~
  \return \ru Возвращает true, если новые атрибуты были добавлены.
          \en Returns true if new attributes were added.\~
  \ingroup Algorithms_3D
*/ //---
MATH_FUNC (bool) AddEdgeSmoothnessAttributes( MbFaceShell & shell, bool skipExisting );

//------------------------------------------------------------------------------
/** \brief \ru Добавить в рёбра атрибуты гладкости ребра.
           \en Add edges smoothness attributes. \~
  \details \ru Добавить в рёбра атрибуты с информацией о гладкости стыковки граней в ребре.
           \en Add attributes to edges with information about the smoothness of joining faces in the edge. \~
  \param[in,out] shell - \ru Оболочка, которую обрабатываем.
                         \en Shell to processing. \~
  \param[in] skipExisting - \ru Пропустить уже существующие (не обновлять в них данные).
                            \en Skip existing ones (do not update data in them). \~
  \param[out] filledEdges - \ru Ребра с созданными или сохраненным атрибутами гладкости.
                            \en Edges with created or saved smoothness attributes. \~
  \param[out] emptyEdges - \ru Ребра без атрибутов гладкости (нет второй поверхности или сбой создания).
                           \en Edges without smoothness attributes (no second surface or creation failure). \~
  \return \ru Возвращает true, если новые атрибуты были добавлены.
          \en Returns true if new attributes were added.\~
  \ingroup Algorithms_3D
*/ //---
MATH_FUNC( bool ) AddEdgeSmoothnessAttributes( MbFaceShell & shell, bool skipExisting,
                                               c3d::ConstEdgesVector & filledEdges, 
                                               c3d::ConstEdgesVector & emptyEdges );

//------------------------------------------------------------------------------
/** \brief \ru Обновить данные в атрибутах гладкости ребра.
           \en Update edges smoothness attributes data. \~
  \details \ru Обновить в атрибутах информацию о гладкости стыковки граней в ребре.
           \en Update data in the attributes for the smoothness of the joining of faces in an edge. \~
  \param[in,out] shell - \ru Оболочка, которую обрабатываем.
                         \en Shell to processing. \~
  \return \ru Возвращает true, если атрибуты были обновлены.
          \en Returns true if attributes were updated.\~
  \ingroup Algorithms_3D
*/ //---
MATH_FUNC (bool) UpdateEdgeSmoothnessAttributes( MbFaceShell & shell );

//------------------------------------------------------------------------------
/** \brief \ru Удалить атрибуты гладкости ребра.
           \en Remove edges smoothness attributes. \~
  \details \ru Удалить из рёбер атрибуты с информацией о гладкости стыковки граней в ребре.
           \en Remove attributes from edges with information about the smoothness of the joining of faces in the edge. \~
  \param[in,out] shell - \ru Оболочка, которую обрабатываем.
                         \en Shell to processing. \~
  \return \ru Возвращает true, если атрибуты были удалены.
          \en Returns true if attributes were removed.\~
  \ingroup Algorithms_3D
*/ //---
MATH_FUNC (bool) RemoveEdgeSmoothnessAttributes( MbFaceShell & shell );


//------------------------------------------------------------------------------
/** \brief \ru Получить из ребра атрибут гладкости ребра.
           \en Get edge smoothness attribute from an edge. \~
  \details \ru Получить из ребра атрибут гладкости ребра.
           \en Get edge smoothness attribute from an edge. \~
  \param[in] edge - \ru Ребро, которое проверяем.
                    \en Edge to check. \~
  \return \ru Возвращает атрибут гладкости ребра, если он есть в ребре.
          \en Returns edge smoothness attribute if this attribute does exist.\~
  \ingroup Algorithms_3D
*/ //---
MATH_FUNC (const MbDoubleRangeAttribute *) GetEdgeSmoothnessAttribute( const MbCurveEdge & edge );

//------------------------------------------------------------------------------
/** \brief \ru Обновить атрибут гладкости ребра.
           \en Update edge smoothness attribute. \~
  \details \ru Обновить атрибут гладкости ребра.
           \en Update edge smoothness attribute. \~
  \param[in] edge - \ru Ребро, которое проверяем.
                    \en Edge to check. \~
  \return \ru Возвращает true, если атрибут есть в ребре и обновлен.
          \en Returns true if this attribute is present in the edge and is updated.\~
  \ingroup Algorithms_3D
*/ //---
MATH_FUNC( bool ) UpdateEdgeSmoothnessAttribute( MbCurveEdge & edge );


//------------------------------------------------------------------------------
/** \brief \ru Проверить, вырожденная ли поверхность.
           \en Check whether the surface is degenerated. \~
  \details \ru Проверить, вырожденная ли поверхность. В текущей реализации работает на элементарных поверхностях,
               наследованных от #MbElemetarySurface и на ограниченных кривыми поверхностях типа #CurveBoundedSurface.
           \en Check whether the surface is degenerated. Currently works on the elementary surfaces inherited from
               #MbElemetarySurface and on the surfaces bounded by curves #CurveBoundedSurface.\~
  \param[in] inputSurface - \ru Поверхность, которую проверяем.
                            \en Surface to check.\~
  \param[in] accuracy     - \ru Точность проверки.
                            \en Accuracy of the check.\~
  \return \ru Возвращает:
              rt_Success, если поверхность не вырожденная;
              rt_DegenerateSurface, если поверхность вырожденная;
              rt_ParameterError, если на вход дан не поддерживаемый тип поверхности.
          \en Returns:
              rt_Success, if the surface is not degenerated;
              rt_DegenerateSurface, the surface is degenerated;
              rt_ParameterError, if the input surface type is not supported.\~
  \ingroup Algorithms_3D
*/ //---
MATH_FUNC (MbResultType) IsDegeneratedElementary( const MbSurface & inputSurface, double accuracy = Math::metricPrecision );


//------------------------------------------------------------------------------
/** \brief \ru Параметры проверки на самопересечение поверхности.
           \en Parameters for checking surface self-intersection. \~
  \details \ru Параметры проверки на самопересечение поверхности. \n
           \en Parameters for checking surface self-intersection. \n \~
  \warning \ru В разработке.
           \en Under development. \~
  \ingroup Algorithms_3D
*/
//---
class MATH_CLASS MbSurfaceSelfIntParams {
public:
  /// \ru Метод проверки самопересечений. \en Method of self intersection checking.
  enum class MbeCheckSelfIntMethod {
    csim_Auto         , ///< \ru Автоматический режим. \en Auto mode.
    csim_UVLine       , ///< \ru Вдоль uv линий. \en Along uv lines.
    csim_SegmIntersect  ///< \ru Сегментированное пересечение. \en Segmented intersection.
  };

private:
  MbeCheckSelfIntMethod _method;  ///< \ru Метод проверки самопересечений. \en Method of self intersection checking.
  c3d::DoubleVector     _uParams; ///< \ru Набор u параметров поверхности. \en Set of u parameters of surface.
  c3d::DoubleVector     _vParams; ///< \ru Набор v параметров поверхности. \en Set of v parameters of surface.
  bool                  _ext;     ///< \ru Опция проверки на расширенной поверхности. \en Extended surface option.
  const MbRect2D *      _uvRange; ///< \ru Область uv для проверки. \en uv surface range for checking.
  VERSION               _version; ///< \ru Версия алгоритма. \en Algorithm version.
public:
  mutable IProgressIndicator * _progress;  ///< \ru Индикатор прогресса выполнения операции. \en A progress indicator of the operation.

public:
  /// \ru Конструктор по умолчанию(автоматический режим). \en Default constructor(auto mode). 
  MbSurfaceSelfIntParams();
  /** \brief \ru Конструктор проверки на самопересечение поверхности в автоматическом режиме.
             \en Constructor of checking for self-intersection of surface automatically. \~
    \details \ru Конструктор проверки на самопересечение поверхности в автоматическом режиме.
             \en Constructor of checking for self-intersection of surface automatically. \~
    \param[in] version - \ru Версия алгоритма.
                         \en Algorithm version. \~
    \param[in] prog    - \ru Индикатор прогресса выполнения операции.
                         \en A progress indicator of the operation.
  */
  MbSurfaceSelfIntParams( VERSION version, IProgressIndicator * prog = nullptr );
  /** \brief \ru Конструктор проверки на самопересечение поверхности вдоль вдоль uv линий.
             \en Constructor of checking for self-intersection of surface along uv lines. \~
    \details \ru Конструктор проверки на самопересечение поверхности вдоль вдоль uv линий.
             \en Constructor of checking for self-intersection of surface along uv lines. \~
    \param[in] uParams - \ru Набор u параметров поверхности.
                         \en Set of u parameters of surface. \~
    \param[in] vParams - \ru Набор v параметров поверхности.
                         \en Set of v parameters of surface. \~
    \param[in] ext     - \ru Опция проверки на расширенной поверхности.
                         \en Extended surface option. \~
    \param[in] uvRange - \ru Область uv для проверки.
                         \en uv surface range for checking \~
    \param[in] version - \ru Версия алгоритма.
                         \en Algorithm version. \~
    \param[in] prog    - \ru Индикатор прогресса выполнения операции.
                         \en A progress indicator of the operation.
  */
  MbSurfaceSelfIntParams( const c3d::DoubleVector &  uParams,
                          const c3d::DoubleVector &  vParams,
                                bool                 ext,
                          const MbRect2D *           uvRange,
                                VERSION              version,
                                IProgressIndicator * prog = nullptr );
  /** \brief \ru Конструктор проверки на самопересечение поверхности путем сегментированного пересечения.
             \en Constructor of checking for self-intersection of surface by segmented intersection. \~
    \details \ru Конструктор проверки на самопересечение поверхности путем сегментированного пересечения.
             \en Constructor of checking for self-intersection of surface by segmented intersection. \~
    \param[in] umin - \ru Минимальный u параметр поверхности.
                      \en Minimal u parameter of surface. \~
    \param[in] umax - \ru Максимальный u параметр поверхности.
                      \en Maximal u parameter of surface. \~
    \param[in] vmin - \ru Минимальный v параметр поверхности.
                      \en Minimal v parameter of surface. \~
    \param[in] vmax - \ru Максимальный v параметр поверхности.
                      \en Maximal v parameter of surface. \~
    \param[in] prog - \ru Индикатор прогресса выполнения операции.
                      \en A progress indicator of the operation.
  */
  MbSurfaceSelfIntParams( double umin, double umax, double vmin, double vmax, IProgressIndicator * prog = nullptr );
  /// \ru Деструктор. \en Destructor. 
  ~MbSurfaceSelfIntParams() {}
public:
  /// \ru Получить метод проверки самопересечений. \en Get the method of self intersection checking.
  MbeCheckSelfIntMethod     GetMethod()  const { return _method; }
  /// \ru Получить набор u параметров поверхности. \en Get the set of u parameters of surface.
  const c3d::DoubleVector & GetUParams() const { return _uParams; }
  /// \ru Получить набор v параметров поверхности. \en Get the set of v parameters of surface.
  const c3d::DoubleVector & GetVParams() const { return _vParams; }
  /// \ru Получить опцию проверки на расширенной поверхности. \en Get the extended surface option.
  bool                      GetExt()     const { return _ext; }
  /// \ru Получить uv область для проверки. \en Get uv surface range for checking.
  const MbRect2D *          GetUVRange() const { return _uvRange; }
  /// \ru Получить версию алгоритма. \en Get the algorithm version.
  VERSION                   GetVersion() const { return _version; }
  OBVIOUS_PRIVATE_COPY( MbSurfaceSelfIntParams )
};


//------------------------------------------------------------------------------
/** \brief \ru Проверить, имеет ли поверхность самопересечения.
           \en Check whether the surface is self-intersected. \~
  \details \ru Проверить, имеет ли поверхность самопересечения.
           \en Check whether the surface is self-intersected.\~
  \param[in] surface      - \ru Проверяемая поверхность.
                            \en Surface to check.\~
  \param[in] params       - \ru Параметры проверки на самопересечение.
                            \en Self-intersection checking parameters.\~
  \return \ru Возвращает true, если поверхность имеет самопересечения.
          \en Returns true if surface self-intersection is found. \~
  \warning \ru В разработке.
           \en Under development. \~
  \ingroup Algorithms_3D
*/ //---
MATH_FUNC( bool ) IsSelfIntersectSurface( const MbSurface &              surface,
                                          const MbSurfaceSelfIntParams & params );


//------------------------------------------------------------------------------
/** \brief \ru Параметры проверки на самопересечение оболочки.
           \en Parameters for checking shell self-intersection. \~
  \details \ru Параметры проверки на самопересечение оболочки. \n
           \en Parameters for checking shell self-intersection. \n \~
  \warning \ru В разработке.
           \en Under development. \~
  \ingroup Algorithms_3D
*/
//---
class MATH_CLASS MbShellSelfIntParams {
public:
  /// \ru Режим проверки самопересечений. \en Mode of self intersection checking.
  enum class MbeCheckSelfIntMode
  {
    Auto                , /// \ru Автоматический режим (проверять пересечение граней, включая самопересечения). \en Auto mode (check intersection of faces including self-intersections).
    SelfIntFacesOnly    , /// \ru Проверять только самопересечение граней. \en Check only self-intersection of faces.
    IntDfferentFacesOnly  /// \ru Проверять только пересечение различных граней. \en Check only the intersection of different faces.
  };

private:
  MbeCheckSelfIntMode          _mode;     ///< \ru Режим проверки самопересечений. \en Mode of self intersection checking.
  VERSION                      _version;  ///< \ru Версия алгоритма. \en Algorithm version.
  std::unique_ptr<MbStepData>  _stepData; ///< \ru Данные для вычисления шага при триангуляции. \en Data for step calculation during triangulation.

public:
  mutable IProgressIndicator * _progress; ///< \ru Индикатор прогресса выполнения операции. \en A progress indicator of the operation.

public:
  /// \ru Конструктор по умолчанию. \en Default constructor. 
  MbShellSelfIntParams();
  /** \brief \ru Конструктор проверки на самопересечение оболочки.
             \en Constructor of checking for self-intersection of shell. \~
    \details \ru Конструктор проверки на самопересечение оболочки.
             \en Constructor of checking for self-intersection of shell. \~
    \param[in] version - \ru Версия алгоритма.
                         \en Algorithm version. \~
    \param[in] prog    - \ru Индикатор прогресса выполнения операции.
                         \en A progress indicator of the operation.
  */
  MbShellSelfIntParams( VERSION version, IProgressIndicator * prog = nullptr );
  /** \brief \ru Конструктор проверки на самопересечение оболочки.
             \en Constructor of checking for self-intersection of shell. \~
    \details \ru Конструктор проверки на самопересечение оболочки.
             \en Constructor of checking for self-intersection of shell. \~
    \param[in] mode -    \ru Режим проверки самопересечений.
                         \en Mode of self intersection checking. \~
    \param[in] version - \ru Версия алгоритма.
                         \en Algorithm version. \~
    \param[in] prog    - \ru Индикатор прогресса выполнения операции.
                         \en A progress indicator of the operation.
  */
  MbShellSelfIntParams( MbeCheckSelfIntMode mode, VERSION version, IProgressIndicator * prog = nullptr );
  /** \brief \ru Конструктор проверки на самопересечение оболочки с помощью анализа сеток.
           \en Constructor of checking for self-intersection of shell by meshes check. \~
    \details \ru Конструктор проверки на самопересечение оболочки с помощью анализа сеток.
             \en Constructor of checking for self-intersection of shell by meshes check. \~
    \param[in] stepData - \ru Данные для вычисления шага при триангуляции.
                          \en Data for step calculation during triangulation. \~
    \param[in] version  - \ru Версия алгоритма.
                          \en Algorithm version. \~
    \param[in] prog     - \ru Индикатор прогресса выполнения операции.
                          \en A progress indicator of the operation.
    \warning \ru Пока не готово. \en Not yet ready.
*/
  MbShellSelfIntParams( const MbStepData & stepData, VERSION version = Math::DefaultMathVersion(), IProgressIndicator * prog = nullptr );

  /// \ru Деструктор. \en Destructor. 
  ~MbShellSelfIntParams() {}
public:
  /// \ru Получить режим проверки самопересечений. \en Get the mode of self intersection checking.
  MbeCheckSelfIntMode GetMode() const { return _mode; }
  /// \ru Установить режим проверки самопересечений. \en Set the mode of self intersection checking.
  void                SetMode( MbeCheckSelfIntMode mode ) { _mode = mode; }
  /// \ru Получить версию алгоритма. \en Get the algorithm version.
  VERSION GetVersion() const { return _version; }
  // \ru Получить данные для расчета шага. \en Get data for the step calculation. \~
  const MbStepData *  GetStepData() const { return _stepData.get(); }

  OBVIOUS_PRIVATE_COPY( MbShellSelfIntParams )
};


//------------------------------------------------------------------------------
/** \brief \ru Результаты проверки на самопересечение оболочки.
           \en Results for checking sell self-intersection. \~
  \details \ru Результаты проверки на самопересечение оболочки. \n
           \en Results for checking sell self-intersection. \n \~
  \warning \ru В разработке.
           \en Under development. \~
  \ingroup Algorithms_3D
*/
//---
class MATH_CLASS MbShellSelfIntResults {
private:
  /** \brief \ru Результат пересечения/самопересечения граней.
             \en Result of intersection/self-intersection of faces. \~
  \details   \ru Результат пересечения/самопересечения граней. Если '_facesIndices' содержат
                 одинаковые индексы, значит грань с данным индексом самопересекается.
                 Кривые пересечения не вычисляются для самопересечения и '_curves' будет пустым. \n
             \en The result of intersection/self-intersection of edges. If '_facesIndices' contains
                 identical indices, which means the face with this index intersects itself.
                 Intersection curves are not calculated for self-intersection and '_curves' will be empty. \n \~
  */
  struct SelfIntResult {
    c3d::IndicesPair _facesIndices;     ///< \ru Индексы пересекающихся/самопересекающихся граней. \en Indices of intersecting/self-intersecting faces.
    c3d::SpaceCurvesSPtrVector _curves; ///< \ru Кривые пересечения граней. \en Face intersection curves.

    /// \ru Конструктор по умолчанию. \en Default constructor.
    SelfIntResult() {};
    /// \ru Конструктор по копирования. \en Copy constructor.
    SelfIntResult( const SelfIntResult & other );
    /// \ru Оператор присваивания. \en Assignment operator.
    SelfIntResult & operator =( const SelfIntResult & other );
    /** \brief \ru Конструктор результата проверки на самопересечение пары граней.
             \en Constructor of the result of checking for self-intersection of a pair of faces. \~
    \details \ru Конструктор результата проверки на самопересечение пары граней.
             \en Constructor of the result of checking for self-intersection of a pair of faces. \~
    \param[in] faceIndex1 - \ru Индекс первой грани.
                            \en First face index. \~
    \param[in] faceIndex2 - \ru Индекс второй грани.
                            \en Second face index. \~
    \param[in] intCurves  - \ru Указатель на вектор кривых пересечения двух граней.
                            \en Pointer to a vector of curves where two faces intersect. \~
    */
    SelfIntResult( size_t faceIndex1, size_t faceIndex2, c3d::SpaceCurvesSPtrVector * intCurves = nullptr );
    /// \ru Обмен. \en Swap.
    void Swap( SelfIntResult & other );
  };

private:
  std::vector<SelfIntResult> _selfIntResults;

public:
  /// \ru Конструктор по умолчанию. \en Default constructor. 
  MbShellSelfIntResults() {};
  /// \ru Деструктор. \en Destructor. 
  ~MbShellSelfIntResults() {};
public:
  /// \ru Добавить результат самопересечения оболочки для пары граней. \en Add the result of self-intersection processing for a pair of faces.
  void AddResult( size_t faceIndex1, size_t faceIndex2, c3d::SpaceCurvesSPtrVector * intersectionCurves = nullptr );
  /** \brief \ru Получить результат самопересечения оболочки для пары граней по индексу.
             \en Get the result of self-intersection processing for a pair of faces. \~
    \details \ru Получить индексы пересекающихся/самопересекающихся граней и кривые пересечения
                 (для пары различных граней) по индексу. Если параметр 'intersectionCurves' - nullptr,
                 кривые пересечения не возвращаются. Если faceIndex1 == faceIndex2 значит грань
                 с индексом faceIndex1 самопересекается. Кривые пересечения не вычисляются и
                 'intersectionCurves' будет пустым.
             \en Get indices of intersecting/self-intersecting faces and intersection curves
                 (for a pair of different faces) by index. If the 'intersectionCurves' parameter is nullptr,
                 intersection curves are not returned. If faceIndex1 == faceIndex2 means face
                 with the index faceIndex1 self-intersects. Intersection curves are not calculated and
                 'intersectionCurves' will be empty. \~
    \param[in]  index                - \ru Индекс результата для пары граней.
                                       \en Result index for a pair of faces. \~
    \param[out] faceIndex1           - \ru Индекс первой грани.
                                       \en First face index. \~
    \param[out] faceIndex2           - \ru Индекс второй грани.
                                       \en Second face index. \~
    \param[out] intersectionCurves   - \ru Указатель на вектор кривых пересечения двух граней.
                                           Если intersectionCurves = nullptr - кривые пересечения
                                           не возвращаются.
                                       \en Pointer to a vector of curves where two faces intersect.
                                           If intersectionCurves = nullptr - intersection curves
                                           do not return. \~
    \return                            \ru Возвращает:
                                           false, если результата с заданным индексом не содержится;
                                           true, если результата с заданным индексом найден;
                                       \en Returns:
                                           false, if there is no result with the given index;
                                           true, if a result with the given index is found; \~
  */
  bool GetResult( size_t index, size_t & faceIndex1, size_t & faceIndex2, c3d::SpaceCurvesSPtrVector * intersectionCurves ) const;
  /// \ru Получить количество пар пересекающихся и самопересекающихся граней. \en Get the number of pairs of intersecting and self-intersecting faces.
  size_t Count() const { return _selfIntResults.size(); }
  /// \ru Установить значения по умолчанию. \en Set defaults.
  void Reset() { _selfIntResults.clear(); };
  /// \ru Зарезервировать размер массива пар пересекающихся и самопересекающихся граней. \en Reserve the size the array of pairs of intersecting and self-intersecting faces.
  void ReserveResult( size_t count ) { _selfIntResults.reserve( count ); }
  OBVIOUS_PRIVATE_COPY( MbShellSelfIntResults )
};


//------------------------------------------------------------------------------
/** \brief \ru Проверить оболочку на наличие самопересечений.
           \en Check shell for self-intersections. \~
  \details \ru Проверить оболочку на наличие самопересечений. Находит все кривые самопересечения оболочки.
           \en Check shell for self-intersections. Finds all shell self-intersection curves.\~
  \param[in]  shell       - \ru Оболочка, которую проверяем.
                            \en Shell to check.\~
  \param[in]  params      - \ru Параметры проверки на самопересечение.
                            \en Self-intersection checking parameters.\~
  \param[out] results     - \ru Результаты проверки на самопересечение.
                            \en Self-intersection checking results.\~
  \return \ru Возвращает:
              false, если оболочка не имеет самопересечений;
              true, если оболочка имеет самопересечения;
          \en Returns:
              false, if shell has no self-intersections;
              true, if shell has self-intersections.\~
  \warning \ru В разработке.
           \en Under development. \~
  \ingroup Algorithms_3D
*/ //---
MATH_FUNC( bool ) IsSelfIntersectShell( const MbFaceShell & shell,
                                        const MbShellSelfIntParams & params,
                                              MbShellSelfIntResults & results );

#endif // __CHECK_GEOMETRY_H
