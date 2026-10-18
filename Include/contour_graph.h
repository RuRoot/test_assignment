////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Построение контуров.
         \en Contours construction. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CONTOUR_GRAPH_H
#define __CONTOUR_GRAPH_H


#include <templ_p_array.h>
#include <cur_contour.h>
#include <curve.h> // for PlaneCurveSPtr
#include <map>


class  MpEdge;
class  ProgressBarWrapper;
class            IProgressIndicator;


//------------------------------------------------------------------------------
/** \brief \ru Вершина.
           \en Vertex. \~
  \details \ru Вершина цикла. Соединяет два ребра цикла - предыдущее и следующее.\n
           \en Vertex of a loop. Connects two edges of a loop - the previous and the next one.\n \~
  \ingroup Algorithms_2D
*/
// ---
class MATH_CLASS MpVertex : public TapeBase {
private:
  MbCartPoint   point;   ///< \ru Точка. \en A point. 
  MpEdge      * begEdge; ///< \ru Предыдущее  ребро. \en The previous edge. 
  MpEdge      * endEdge; ///< \ru Последующее ребро. \en The next edge. 

public:
  /// \ru Конструктор по точке. \en Constructor by point. 
  MpVertex( const MbCartPoint & initP ) 
    : point( initP ) 
    , begEdge( nullptr )
    , endEdge( nullptr )
  {}

  /// \ru Деструктор. \en Destructor. 
  virtual ~MpVertex();    
  
  /**\ru \name Операции с вершиной.
     \en \name Operations on vertex.
      \{ */

  /// \ru Выдать декартову точку вершины. \en Get the Cartesian point of a vertex. 
  const MbCartPoint & GetCartPoint()                   const { return point; } 
  /// \ru Выдать декартову точку вершины. \en Get the Cartesian point of a vertex. 
        void          GetCartPoint( MbCartPoint & cp ) const { cp = point;   }  
  /// \ru Установить декартову точку вершины. \en Set the Cartesian point of a vertex. 
        void          SetCartPoint( MbCartPoint & cp )       { point = cp;   }  
  /** \} */  
  /**\ru \name Операции с указателями на ребра.
     \en \name Operations on pointers to edges.
      \{ */
        /// \ru Изменить предыдущее ребро. \en Change the previous edge. 
        void          SetBegEdge( MpEdge * edge ) { begEdge = edge; }  
        /// \ru Изменить последующее ребро. \en Change the next edge. 
        void          SetEndEdge( MpEdge * edge ) { endEdge = edge; } 
        /// \ru Предыдущее  ребро. \en The previous edge. 
        MpEdge      * GetBegEdge() const          { return begEdge; }  
        /// \ru Последующее ребро. \en The next edge. 
        MpEdge      * GetEndEdge() const          { return endEdge; }  
  /** \} */  
  /**\ru \name Операции преобразования.
     \en \name Transformation operations.
      \{ */

        /** \brief \ru Преобразовать.
                   \en Transform. \~
          \details \ru Преобразовать в соответствии с матрицей.\n
                   \en Transform according to the matrix.\n \~
          \param[in] matr - \ru Матрица трансформации.
                            \en Transformation matrix. \~          
        */
        void          Transform( const MbMatrix & matr ); 

        /** \brief \ru Переместить.
                   \en Move. \~
          \details \ru Переместить на вектор.\n
                   \en Move by a vector.\n \~    
          \param[in] to - \ru Вектор перемещения.
                          \en Movement vector. \~       
        */
        void          Move( const MbVector & to );      

        /** \brief \ru Повернуть.
                   \en Rotate. \~
          \details \ru Повернуть на угол вокруг точки.\n
                   \en Rotate at angle around a point.\n \~    
          \param[in] pnt   - \ru Точка - центр поворота.
                             \en A point is a rotation center. \~
          \param[in] angle - \ru Двумерный нормализованный вектор, задающий угол вращения.
                             \en A two-dimensional normalized vector determining the rotation angle. \~
        */
        void          Rotate( const MbCartPoint & pnt, const MbDirection & angle ); 
  /** \} */

private:
  MpVertex( const MpVertex & ); // \ru Не реализовано \en Not implemented 
  void operator = ( const MpVertex & ); // \ru Не реализовано \en Not implemented 

  DECLARE_PERSISTENT_CLASS_NEW_DEL ( MpVertex )
}; // MpVertex

IMPL_PERSISTENT_OPS( MpVertex )

//------------------------------------------------------------------------------
/** \brief \ru Ребро.
           \en Edge. \~
  \details \ru Ребро цикла.\n
           \en A loop edge.\n \~
  \ingroup Algorithms_2D
*/
// ---
class MATH_CLASS MpEdge : public TapeBase {
private :
  const MbCurve * baseCurve; ///< \ru Базовая кривая. \en The base curve. 
  ptrdiff_t       name;      ///< \ru Имя базовой кривой. \en The base curve name. 
  double          tBeg;      ///< \ru Параметр начала ребра. \en The edge start parameter. 
  double          tEnd;      ///< \ru Параметр конца ребра. \en The edge end parameter. 
  bool            sense;     ///< \ru Признак совпадения направления с кривой. \en Flag of coincidence of direction with the curve. 
  uint            type;      ///< \ru Тип кривой. \en Curve type. 
  MpVertex *      begVertex; ///< \ru Вершина-начало. \en The start vertex. 
  MpVertex *      endVertex; ///< \ru Вершина-конец. \en The end vertex. 

public:

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по кривой.\n
             \en Constructor by curve.\n \~  
    \param[in] c  - \ru Базовая кривая.
                    \en Base curve. \~
    \param[in] t1 - \ru Начальный параметр ребра.
                    \en The edge start parameter. \~
    \param[in] t2 - \ru Конечный параметр ребра.
                    \en The edge end parameter. \~
    \param[in] s  - \ru Признак совпадения направления с кривой.
                    \en Flag of coincidence of direction and the curve. \~
  */
  MpEdge( const MbCurve * c, double t1, double t2, bool s );

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по кривой.\n
             \en Constructor by curve.\n \~  
    \param[in] c - \ru Базовая кривая.
                   \en Base curve. \~      
    \param[in] s - \ru Признак совпадения направления с кривой.
                   \en Flag of coincidence of direction and the curve. \~
  */
  MpEdge( const MbCurve * c, bool s );

  /// \ru Копирующий конструктор. \en Copy-constructor. 
  MpEdge( const MpEdge & );

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор ребра с нулевой базовой кривой.\n
             \en Constructor of edge with null base curve.\n \~     
    \param[in] t1 - \ru Начальный параметр ребра.
                    \en The edge start parameter. \~
    \param[in] t2 - \ru Конечный параметр ребра.
                    \en The edge end parameter. \~
    \param[in] s  - \ru Признак совпадения направления с кривой.
                    \en Flag of coincidence of direction and the curve. \~
    \param[in] n  - \ru Имя базовой кривой.
                    \en The base curve name. \~
    \param[in] t  - \ru Тип кривой.
                    \en A curve type. \~
  */
  MpEdge( double t1, double t2, bool s, ptrdiff_t n, uint t );

  /// \ru Деструктор. \en Destructor. 
  virtual ~MpEdge();

  /**\ru \name Функции доступа к данным.
     \en \name Functions for access to data.
      \{ */
  /// \ru Выдать кривую, по которой проходит ребро. \en Get the curve the edge passes through.    
  const MbCurve * GetCurve() const { return baseCurve; }                
  /// \ru Имя базовой кривой. \en The base curve name.    
  ptrdiff_t   GetName() const { return name; }                      
  /// \ru Выдать направление по отношению к кривой. \en Get the direction relative to the curve.      
  bool        GetSense() const { return sense; }                    
  /// \ru Выдать вершину-начало. \en Get the start vertex. 
  MpVertex *  GetBegVertex() const { return begVertex; }            
  /// \ru Выдать вершину-конец. \en Get the end vertex.         
  MpVertex *  GetEndVertex() const { return endVertex; }            
  /// \ru Начальный параметр. \en Get the start parameter. 
  double      GetTBeg() const { return tBeg; }                      
  /// \ru Конечный параметр. \en End parameter. 
  double      GetTEnd() const { return tEnd; }    
  /// \ru Тип кривой. \en Curve type. 
  uint        GetType() const { return type; }

  /// \ru Выдать декартову точку вершины-начала. \en Get the Cartesian point of the start vertex. 
  void        GetBegPoint( MbCartPoint & cp ) const;                
  /// \ru Выдать декартову точку вершины-конца. \en Get the Cartesian point of the end vertex. 
  void        GetEndPoint( MbCartPoint & cp ) const;                
  /// \ru Выдать касательный вектор в начальной вершине. \en Get the tangent vector at the start point. 
  void        GetBegTangent( MbDirection & tan ) const;             
  /// \ru Выдать касательный вектор в конечной вершине. \en Get the tangent vector at the end vertex. 
  void        GetEndTangent( MbDirection & tan ) const;             
  /// \ru Выдать кривизну в начальной вершине. \en Get the curvature at the start point. 
  double      GetBegCurvature() const;                              
  /// \ru Выдать кривизну в конечной вершине. \en Get the curvature at the end point. 
  double      GetEndCurvature() const;                              
  /** \} */   
  /**\ru \name Функции изменения данных.
     \en \name Functions for changing data.
      \{ */
  /// \ru Выдать кривую, по которой проходит ребро. \en Get the curve the edge passes through.    
  void        SetCurve( const MbCurve * newCurve ) { baseCurve = newCurve; }
  /// \ru Установить имя базовой кривой. \en Set name of the base curve. 
  void        SetName( ptrdiff_t n ) { name = n; }                  
  /// \ru Установить направление по отношению к кривой. \en Set the direction relative to the curve. 
  void        SetSense( bool s ) { sense = s; }                     
  /// \ru Установить вершину-начало. \en Set the start vertex. 
  void        SetBegVertex( MpVertex * vert ) { begVertex = vert; } 
  /// \ru Установить вершину-конец. \en Set the end vertex. 
  void        SetEndVertex( MpVertex * vert ) { endVertex = vert; } 
  /// \ru Установить начальный параметр. \en Set the start parameter. 
  void        SetTBeg( double t ) { tBeg = t; }                     
  /// \ru Установить конечный параметр. \en Set the end parameter. 
  void        SetTEnd( double t ) { tEnd = t; }                     
 
  /// \ru Изменить ориентацию. \en Change the orientation. 
  void        Reverse();                                            
  /// \ru Создать кривую. \en Create a curve. 
  MbCurve *   MakeCurve() const;                                    
  /** \} */   
  /**\ru \name Операции преобразования.
     \en \name Transformation operations.
      \{ */

  /** \brief \ru Преобразовать.
             \en Transform. \~
    \details \ru Преобразовать в соответствии с матрицей.\n
             \en Transform according to the matrix.\n \~
    \param[in] matr - \ru Матрица трансформации.
                      \en Transformation matrix. \~          
  */
  void        Transform( const MbMatrix & matr ); 

  /** \brief \ru Переместить.
             \en Move. \~
    \details \ru Переместить на вектор.\n
             \en Move by a vector.\n \~    
    \param[in] to - \ru Вектор перемещения.
                    \en Movement vector. \~       
  */
  void        Move( const MbVector & to );  

  /** \brief \ru Повернуть.
             \en Rotate. \~
    \details \ru Повернуть на угол вокруг точки.\n
             \en Rotate at angle around a point.\n \~    
    \param[in] pnt   - \ru Точка - центр поворота.
                       \en A point is a rotation center. \~
    \param[in] angle - \ru Двумерный нормализованный вектор, задающий угол вращения.
                       \en A two-dimensional normalized vector determining the rotation angle. \~
  */
  void        Rotate( const MbCartPoint & pnt, const MbDirection & angle ); 
  /** \} */ 

private:
  void operator = ( const MpEdge & ); // \ru Не реализовано \en Not implemented 

  DECLARE_PERSISTENT_CLASS_NEW_DEL ( MpEdge )
}; // MpEdge

IMPL_PERSISTENT_OPS( MpEdge )


//------------------------------------------------------------------------------
/** \brief \ru Цикл.
           \en Loop. \~
  \details \ru Цикл. Набор ребер.\n
           \en Loop. Set of edges.\n \~
  \ingroup Algorithms_2D
*/
// ---
class MATH_CLASS MpLoop : public TapeBase {
private :
  PArray<MpEdge> edgeList;    ///< \ru Список ребер. \en List of edges. 
  bool           orientation; ///< \ru Ориентация цикла. \en Loop orientation. 
  int            mode;        ///< \ru Направление построения. \en Construction direction. 

public:  

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по ребру и направлению построения.\n
             \en Constructor by edge and the direction of construction.\n \~  
    \param[in] initEdge - \ru Ребро.
                          \en Edge. \~
    \param[in] m        - \ru Направление построения цикла:
                          если m > 0 - цикл строится против часовой стрелки,
                          если m < 0 - по часовой стрелке.
                          \en Direction of loop construction:
                          if m > 0 - the loop is constructed counterclockwise,
                          if m < 0 - clockwise. \~
  */
  MpLoop( MpEdge * initEdge, int m );
  
  /// \ru Копирующий конструктор. \en Copy-constructor. 
  MpLoop( const MpLoop & );

  /// \ru Деструктор. \en Destructor. 
  virtual ~MpLoop();

  /**\ru \name Функции доступа к данным.
     \en \name Functions for access to data.
      \{ */

  /// \ru Количество ребер. \en Count of edges. 
  ptrdiff_t   GetEdgesCount() const { return edgeList.Count(); }            
  
  /** \brief \ru Ребро по индексу.
             \en Edge by index. \~
    \details \ru Ребро по его индексу. Без проверки корректности индекса.\n
             \en Edge by its index. Without check for index correctness.\n \~    
    \param[in] index - \ru Индекс ребра.
                       \en An edge index. \~    
  */
  MpEdge    * GetEdge( ptrdiff_t index ) const { return edgeList[index]; }    

  /** \brief \ru Ребро по индексу для изменения.
             \en Edge by index to change. \~
    \details \ru Ребро по его индексу для изменения. Без проверки корректности индекса.\n
             \en Edge by its index to change. Without check for index correctness.\n \~
    \param[in] index - \ru Индекс ребра.
                       \en An edge index. \~
  */
  MpEdge   *& GetEdge( ptrdiff_t index )  { return edgeList[index]; }

  /// \ru Выдать последнее ребро. \en Get the last edge. 
  MpEdge    * GetEdge() const;                                              

  /// \ru Дать ориентацию. \en Get the orientation. 
  bool        GetOrientation() const { return orientation; }                
  /// \ru Направление построения. \en Construction direction. 
  int         GetMode() const { return mode; }                              
  /// \ru Выдать массив вершин. \en Get vertex array. 
  void        GetVerticesArray( RPArray<MpVertex> & vertices ) const;        
  /// \ru Выдать массив кривых. \en Get curve array. 
  void        GetCurvesArray  ( RPArray<const MbCurve> & curves ) const;
  /// \ru Выдать массив кривых. \en Get curve array. 
  void        SetCurvesArray  ( RPArray<MbCurve> & curves );

  /** \} */  
  /**\ru \name Функции изменения данных.
     \en \name Functions for changing data.
      \{ */

  /// \ru Добавить ребро. \en Add an edge. 
  void        AddEdge( MpEdge * edge ) { edgeList.Add(edge); }              
  /// \ru Удалить последнее ребро. \en Delete the last edge. 
  void        DeleteEdge();                                                 

  /** \brief \ru Удалить ребро по индексу.
             \en Delete an edge by index. \~
    \details \ru Удалить ребро по его индексу. Без проверки корректности индекса.\n
             \en Delete an edge by its index. Without check for index correctness.\n \~    
    \param[in] index - \ru Индекс ребра.
                       \en An edge index. \~    
  */
  void        DeleteEdge( ptrdiff_t index ) { edgeList.RemoveInd(index); }  
  
  /// \ru Установить ориентацию. \en Set the orientation. 
  void        SetOrientation( bool s ) { orientation = s; }                 

  /** \brief \ru Установить направление обхода.
             \en Set the traverse direction. \~
    \details \ru Установить направление обхода цикла.\n
             \en Set the direction of traversal of the loop.\n \~    
    \param[in] m - \ru Направление обхода.\n
                   Имеет значение знак числа m:\n
                   если m > 0, то обход против часовой стрелки,\n
                   если m < 0, то по часовой стрелке.
                   \en The traversal direction.\n
                   Has a value of sign of number m:\n
                   if m > 0, then traversal is counterclockwise,\n
                   if m < 0, then it is clockwise. \~
  */
  void        SetMode( int m ) { mode=m; }    

  /// \ru Изменить ориентацию ребра. \en Change the edge orientation. 
  void        Reverse() { orientation = !orientation; }                     
  /// \ru Построить вершины. \en Construct vertices. 
  void        CreateVertices();                                             
  /// \ru Создать контур по циклу. \en Create a contour by the loop. 
  MbContour * MakeContour( double epsilon = METRIC_ACCURACY ) const;
  
  /** \} */  
  /**\ru \name Операции преобразования.
     \en \name Transformation operations.
      \{ */

  /** \brief \ru Преобразование.
             \en Transformation. \~
    \details \ru Преобразование в соответствии с матрицей.\n
             \en Transform according to matrix.\n \~
    \param[in] matr - \ru Матрица трансформации.
                      \en Transformation matrix. \~          
  */
  void        Transform( const MbMatrix & matr ); 

  /** \brief \ru Переместить.
             \en Move. \~
    \details \ru Переместить на вектор.\n
             \en Move by a vector.\n \~    
    \param[in] to - \ru Вектор перемещения.
                    \en Movement vector. \~       
  */
  void        Move( const MbVector & to ); 

  /** \brief \ru Повернуть.
             \en Rotate. \~
    \details \ru Повернуть на угол вокруг точки.\n
             \en Rotate at angle around a point.\n \~    
    \param[in] pnt   - \ru Точка - центр поворота.
                       \en A point is a rotation center. \~
    \param[in] angle - \ru Двумерный нормализованный вектор, задающий угол вращения.
                       \en A two-dimensional normalized vector determining the rotation angle. \~
  */
  void        Rotate( const MbCartPoint & pnt, const MbDirection & angle ); 
  /** \} */  

private:
  void operator = ( const MpLoop & ); // \ru Не реализовано \en Not implemented 

  DECLARE_PERSISTENT_CLASS_NEW_DEL ( MpLoop )
}; // Loop

IMPL_PERSISTENT_OPS( MpLoop )


//------------------------------------------------------------------------------
/** \brief \ru Граф построения контуров.
           \en Contours construction graph. \~
  \details \ru Граф построения контуров.\n
    Содержит список границ - циклов.
           \en Contours construction graph.\n
    Contains list of boundaries - loops. \~
  \ingroup Algorithms_2D
*/
// ---
class MATH_CLASS MpGraph : public TapeBase {
private :
  PArray<MpLoop> loops;       ///< \ru Список границ. \en List of boundaries. 
  int            mode;        ///< \ru Направление обхода. \en Traversal direction. 
  ptrdiff_t      nameCount;   ///< \ru Количество имен ребер. \en Edge names count. 
  PArray<MpEdge> unusedEdges; ///< \ru Список ребер. \en List of edges. 

private:
  VERSION        version;     ///< \ru Версия чтения. // BUG_57224 \en Read version. // BUG_57224 

public:
  MpGraph();                          ///< \ru Конструктор. \en Constructor. 
  MpGraph( MpLoop * init );           ///< \ru Конструктор по циклу. \en Constructor by loop. 
  MpGraph( RPArray<MpLoop> & init );  ///< \ru Конструктор по набору циклов. \en Constructor by a set of loops. 
  MpGraph( const MpGraph & );         ///< \ru Копирующий конструктор. \en Copy-constructor. 
  virtual ~MpGraph();                 ///< \ru Деструктор. \en Destructor. 

  /**\ru \name Функции доступа к данным.
     \en \name Functions for access to data.
      \{ */

  /// \ru Количество границ. \en Count of boundaries. 
  size_t      GetLoopsCount() const { return loops.Count(); }         

  /** \brief \ru Цикл по индексу.
             \en Loop by index. \~
    \details \ru Цикл по индексу без проверки индекса.\n
             \en Loop by index without check of index.\n \~    
    \param[in] index - \ru Индекс цикла.
                       \en The loop index. \~     
  */
  MpLoop    * GetLoop( size_t index ) const { return loops[index]; }
        
  /// \ru Направление обхода. \en Traversal direction. 
  int         GetMode() const { return mode; }       
     
  /** \brief \ru Выдать точку.
             \en Get the point. \~
    \details \ru Выдать точку для сборки графа.\n
             \en Get the point for graph building.\n \~
    \param[in]  curveList - \ru Набор кривых (без совпадений) для создания графа.
                            \en A set of curves (without coincidences) for creating the graph. \~
    \param[in]  cross     - \ru Набор точек пересечения кривых из curveList.
                            \en Set of intersection points of curves from curveList. \~   
    \param[out] p         - \ru Результат - двумерная точка.
                            \en The result is a two-dimensional point. \~
  */
  bool        GetPointIn( const RPArray<MbCurve> & curveList, SArray<MbCrossPoint> & cross, MbCartPoint & p, 
                          double epsilon = Math::LengthEps*c3d::METRIC_DELTA ) const;
                                                                                  
  /** \brief \ru Выдать использованные кривые.
             \en Get used curves. \~
    \details \ru Выдать использованные кривые и переименовать в соответствии с последними.\n
             \en Get used curves and rename in compliance with the last ones.\n \~
    \param[in]  curveList  - \ru Набор кривых.
                             \en Set of curves. \~       
    \param[out] usedCurves - \ru Результат - использованные кривые.
                             \en The result are the used curves. \~
  */
  template<class CurvesArray, class CurvesArray2>
  void        GetUsedCurves( const CurvesArray & curveList, CurvesArray2 & usedCurves );

  /** \brief \ru Ориентация цикла по индексу.
             \en The orientation of a loop by its index. \~
    \details \ru Ориентация цикла по индексу без проверки индекса.\n
             \en The orientation of loop by its index without check of index.\n \~    
    \param[in] i - \ru Индекс цикла.
                   \en The loop index. \~
    \return \ru Ориентацию цикла.
            \en The loop orientation. \~
  */
  bool        GetLoopOrientation( size_t i ) const { return loops[i]->GetOrientation(); }

  /** \} */  
  /**\ru \name Функции изменения данных.
     \en \name Functions for changing data.
      \{ */

  /// \ru Добавить новую границу грани. \en Add a new boundary of the face. 
  void        AddLoop( MpLoop * newLoop );                          
  /// \ru Добавить новую границу грани в начало списка границ. \en Add a new boundary of the face to the beginning of face list. 
  void        InsertLoop( MpLoop * newLoop );                       
  /// \ru Установить направление обхода. \en Set the traverse direction. 
  void        SetMode( int m ) { mode = m; }                        
  /// \ru Выдать контуры циклов. \en Get loops' contours. 
  void        MakeContours( RPArray<MbContour> & contours ) const;   

  /** \brief \ru Запомнить неиспользованные ребра.
             \en Store unused edges. \~
    \details \ru Запомнить неиспользованные ребра, установив им имена.\n
             \en Store unused edges giving names for them.\n \~    
    \param[in] curveList - \ru Список неиспользованных кривых для установки имен.
                           \en The list of unused curves for setting the names. \~
    \param[in] g         - \ru Граф для поиска неиспользованных ребер.\n
                           Если в нем или в его массиве неиспользованных ребер
                           нашлось ребро с именем неиспользованной кривой, 
                           то оно запоминается в массиве неиспользованных ребер unusedEdges.\n
                           \en Graph for searching unused edges.\n
                           If there is an edge by the names of an unused curve
                           in the graph or its array of unused edges, 
                           then it is stored in the array of unused edges unusedEdges.\n \~
    \param[in] curvesNames - \ru Имена кривых.
                             \en Curves names. \n \~
  */
  template<class CurvesArray>
  void        SetAllName( const CurvesArray & curveList, MpGraph * g, std::map<const MbCurve *, ptrdiff_t> & curvesNames );

  /** \brief \ru Дать имена ребрам.
             \en Give names to edges. \~
    \details \ru Дать имена ребрам по списку кривых.\n
             \en Give names to edges by the list of curves.\n \~    
    \param[in] curveList - \ru Список кривых для именования.
                           \en The list of curves for naming. \~     
    \param[in] curvesNames - \ru Имена кривых.
                             \en Curves names. \n \~
  */
  template<class CurvesArray>
  void        SetEdgeName( const CurvesArray & curveList, std::map<const MbCurve *, ptrdiff_t> & curvesNames );

  /** \brief \ru Определить ориентацию контуров.
             \en Determine the contours' orientation. \~
    \details \ru Определить ориентацию контуров по их вложенности.\n
             \en Determine the contours' orientation by its inclusion.\n \~    
    \param[in] contourArray - \ru Список контуров, по которым строился граф.
                              \en List of contours the graph is built for. \~        
  */
  template<class ContourArray>
  void        SetLoopsOrientation( const ContourArray & contourArray );

  /** \brief \ru Перевести параметры ребер в параметры кривых.
             \en Convert edges' parameters to curves' parameters. \~
    \details \ru Перевести параметры ребер в параметры кривых, 
      если кривые ребер нашлись в списках.\n
             \en Convert parameters of edges to parameters of curves 
      if edges' curves are found in lists.\n \~    
    \param[in] unchangeCurve - \ru Список имен кривых для изменения.
                               \en List of curves' names for modification. \~   
    \param[in] changeCurve - \ru Список имен кривых для изменения.
                             \en List of curves' names for modification. \~
    \param[in] curveList - \ru Список кривых для изменения параметризации.
                           \en List of curves for modification of parametrization. \~
    \warning \ru Для внутреннего использования.
             \en For internal use only. \~      
  */
  // \ru Изменяется параметризация только у отрезков. \en Parametrization can be modified for line segments only. 
  // \ru Специально для исправления ошибки BUG_57224 \en Especially to fix BUG_57224       
  void        ConvertEdgesParams( const SArray<ptrdiff_t> & unchangeCurve, const SArray<ptrdiff_t> & changeCurve,
                                  const RPArray<MbCurve> & curveList ) const; 
    
  /** \} */  
  /**\ru \name Операции преобразования.
     \en \name Transformation operations.
      \{ */

  /** \brief \ru Преобразование.
             \en Transformation. \~
    \details \ru Преобразование в соответствии с матрицей.\n
             \en Transformation according to the matrix.\n \~
    \param[in] matr - \ru Матрица трансформации.
                      \en Transformation matrix. \~          
  */
  void        Transform( const MbMatrix & matr ); 

  /** \brief \ru Переместить.
             \en Move. \~
    \details \ru Переместить на вектор.\n
             \en Move by a vector.\n \~    
    \param[in] to - \ru Вектор перемещения.
                    \en Movement vector. \~       
  */
  void        Move( const MbVector & to );     

  /** \brief \ru Повернуть.
             \en Rotate. \~
    \details \ru Повернуть на угол вокруг точки.\n
             \en Rotate at angle around a point.\n \~    
    \param[in] pnt   - \ru Точка - центр поворота.
                       \en A point is a rotation center. \~
    \param[in] angle - \ru Двумерный нормализованный вектор, задающий угол вращения.
                       \en A two-dimensional normalized vector determining the rotation angle. \~
  */
  void        Rotate( const MbCartPoint & pnt, const MbDirection & angle ); 
  /** \} */ 
       
private:

  /** \brief \ru Направление обхода цикла.
             \en Loop traversal direction. \~
    \details \ru Направление обхода цикла, которому принадлежит вершина.\n
             \en Traversal direction of the loop the vertex belongs to.\n \~    
    \param[in] vert - \ru Вершина для поиска цикла.
                      \en The vertex for searching the loop. \~
    \return \ru Направление обхода.\n
            Важен знак числа:\n
            если > 0 - против часовой стрелки,\n
            если < 0 - по часовой стрелке.
            \en The traversal direction.\n
            Sign of the number is significant:\n
            if > 0 - counterclockwise,\n
            if < 0 - clockwise. \~
  */
  int         GetLoopMode( MpVertex * vert ) const;

  /** \brief \ru Выдать массив вершин.
             \en Get vertex array. \~
    \details \ru Выдать массив вершин всех циклов графа.\n
             \en Get vertex array of all the loops of the graph.\n \~    
    \param[out] vertices - \ru Результат - массив вершин.
                           \en The result is a vertex array. \~     
  */
  void        GetVerticesArray( RPArray<MpVertex> & vertices ) const; 

  /// \ru Количество имен ребер. \en The count of edges' names. 
  ptrdiff_t   GetNameCount() const { return nameCount; }

  /** \brief \ru Выдать имена ребер.
             \en Get edges' names. \~
    \details \ru Выдать имена ребер всех циклов графа.\n
      Имена складываются в массив без повторений, сортированные по возрастанию.
             \en Get names of edges of all the loops of the graph.\n
      Names are put to the array without duplications, sorted in the ascending order. \~
    \param[out] curveName - \ru Результат - массив имен.
                            \en The result is the array of names. \~     
  */
  void        GetEdgeName( SArray<ptrdiff_t> & curveName ) const;

  /** \brief \ru Ориентация ребра.
             \en Edge orientation. \~
    \details \ru Ориентация ребра по его имени с учетом направления цикла.\n
             \en Edge orientation by its name subject to the loop direction.\n \~
    \param[in]  n0 - \ru Имя ребра.
                     \en The edge name. \~
    \param[out] s  - \ru Ориентация ребра.
                     \en Edge orientation. \~  
    \return \ru true, если нашли нужное ребро.
            \en True if the required edge is found. \~
  */
  bool        GetCurveData( ptrdiff_t n0, int & s ) const;

  /** \brief \ru Ориентация ребра.
             \en Edge orientation. \~
    \details \ru Ориентация ребра по его имени без учета направления цикла.\n
             \en Edge orientation by its name without taking the loop direction into account.\n \~
    \param[in]  n0 - \ru Имя ребра.
                     \en The edge name. \~
    \param[out] s  - \ru Ориентация ребра.
                     \en Edge orientation. \~  
    \return \ru true, если нашли нужное ребро.
            \en True if the required edge is found. \~
  */
  bool        GetOldData  ( ptrdiff_t n0, int & s ) const;
                
  /** \brief \ru Выдать точку.
             \en Get the point. \~
    \details \ru Выдать точку для сборки графа.\n
             \en Get the point for graph building.\n \~
    \param[in]  vertex - \ru Начальная вершина ребра, которому соответствует кривая curve.
                         \en The start vertex of the edge the curve 'curve' corresponds to. \~
    \param[in]  curve  - \ru Кривая для расчета точки.
                         \en The curve for point calculation. \~
    \param[in]  t      - \ru Параметр на кривой.
                         \en A parameter on the curve. \~ 
    \param[out] p      - \ru Результат - двумерная точка.
                         \en The result is a two-dimensional point. \~
  */
  void        GetPoint( MpVertex * vertex, MbCurve * curve, double t, MbCartPoint & p ) const; 

  void        CurvesSort( const RPArray<MbCurve> & curveList, SArray<ptrdiff_t> & unchangeCurve, SArray<ptrdiff_t> & changeCurve ) const;  

private:
  void operator = ( const MpGraph & ); // \ru Не реализовано \en Not implemented 

  DECLARE_PERSISTENT_CLASS_NEW_DEL ( MpGraph )
}; // MpGraph

IMPL_PERSISTENT_OPS( MpGraph )


//------------------------------------------------------------------------------
/** \brief \ru Запомнить неиспользованные ребра.
           \en Store unused edges. \~
  \details \ru Запомнить неиспользованные ребра, установив им имена.\n
           \en Store unused edges giving names for them.\n \~
  \param[in] curveList - \ru Список неиспользованных кривых для установки имен.
                         \en The list of unused curves for setting the names. \~
  \param[in] g         - \ru Граф для поиска неиспользованных ребер.\n
                         Если в нем или в его массиве неиспользованных ребер
                         нашлось ребро с именем неиспользованной кривой,
                         то оно запоминается в массиве неиспользованных ребер unusedEdges.\n
                         \en Graph for searching unused edges.\n
                         If there is an edge by the names of an unused curve
                         in the graph or its array of unused edges,
                         then it is stored in the array of unused edges unusedEdges.\n \~
  \param[in] curvesNames - \ru Имена кривых.
                           \en Curves names. \n \~
*/
// ---
template<class CurvesArray>
void MpGraph::SetAllName( const CurvesArray & curveList, MpGraph * g, std::map<const MbCurve *, ptrdiff_t> & curvesNames )
{
  ptrdiff_t i, j, j0, k;
  MpLoop * loop = nullptr;
  MpEdge * edge = nullptr;
  bool s;

  unusedEdges.Flush();
  nameCount = curveList.size();

  // Сначала именуются ребра.
  for ( i = 0; i < (ptrdiff_t)loops.Count(); i++ ) {
    loop = GetLoop( i );
    for ( k = 0; k < (ptrdiff_t)loop->GetEdgesCount(); k++ ) {
      edge = loop->GetEdge( k );
      ptrdiff_t curveName = curvesNames[edge->GetCurve()]; // Это имя может быть у исходной кривой и у измененной в обработке наложений.
      if ( curveName >= 0 && curveName <= nameCount ) {
        edge->SetName( curveName );
        edge->SetCurve( nullptr );
      }
    }
  }

  // Теперь ищутся неиспользованные ребра.
  for ( j = 0; j < nameCount; j++ ) {
    j0 = -1;

    for ( i = 0; i < (ptrdiff_t)loops.Count(); i++ ) {
      loop = GetLoop( i );
      for ( k = 0; k < (ptrdiff_t)loop->GetEdgesCount(); k++ ) {
        edge = loop->GetEdge( k );
        if ( edge->GetName() == j )
          j0 = 1; // Нашли кривую в новом графе.
        // if ( curve0==edge->GetCurve() ) {
        //   edge->SetName( j );
        //   edge->SetCurve( nullptr );
        //   j0 = 1;
        // }
      }
    }

    if ( j0 < 0 && g != nullptr ) { // Этой кривой нет в новом графе this. Ищем ее в старом графе g.
      for ( i = 0; i < (ptrdiff_t)g->loops.Count(); i++ ) {
        loop = g->GetLoop( i );

        for ( k = 0; k < (ptrdiff_t)loop->GetEdgesCount(); k++ ) {
          edge = loop->GetEdge( k );

          if ( j == edge->GetName() ) { // Это ребро построено на кривой curveList[j].
            s = edge->GetSense();
            if ( loop->GetMode() < 0 )
              s = !s;

            unusedEdges.Add( new MpEdge( edge->GetTBeg(), edge->GetTEnd(), s, j, edge->GetType() ) );
            j0 = 1;
            i = g->loops.Count();
            break;
          }
        }
      }
    }

    if ( j0 < 0 && g != nullptr ) { // Если не нашли, ищем в неиспользованных ребрах.
      for ( k = 0; k < (ptrdiff_t)g->unusedEdges.Count(); k++ ) {
        edge = g->unusedEdges[k];
        if ( j == edge->GetName() ) {
          s = edge->GetSense();
          unusedEdges.Add( new MpEdge( edge->GetTBeg(), edge->GetTEnd(), s, j, edge->GetType() ) );
          break;
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
  /** \brief \ru Дать имена ребрам.
             \en Give names to edges. \~
    \details \ru Дать имена ребрам по списку кривых.\n
             \en Give names to edges by the list of curves.\n \~
    \param[in] curveList - \ru Список кривых для именования.
                           \en The list of curves for naming. \~
    \param[in] curvesNames - \ru Имена кривых.
                             \en Curves names. \n \~
  */
  // ---
template<class CurvesArray>
void MpGraph::SetEdgeName( const CurvesArray & curveList, std::map<const MbCurve *, ptrdiff_t> & curvesNames )
{
  // Именование ребер номерами кривых.
  nameCount = curveList.size();

  // for ( ptrdiff_t j = 0; j < nameCount; j++ ) {
  //   MbCurve *curve0 = curveList[j];
  // 
  //   for ( ptrdiff_t i = 0; i < (ptrdiff_t)loops.Count(); i++ ) {
  //     MpLoop *loop = GetLoop( i );
  // 
  //     for( ptrdiff_t k = 0; k < (ptrdiff_t)loop->edgeList.Count(); k++ ) {
  //       MpEdge *edge = loop->edgeList[k];
  // 
  //       if ( curve0==edge->GetCurve() ) {
  //         edge->SetName( j );
  //         edge->baseCurve = nullptr;
  //       }
  //     }
  //   }
  // }

  for ( ptrdiff_t i = 0; i < (ptrdiff_t)loops.Count(); i++ ) {
    MpLoop * loop = GetLoop( i );

    for ( ptrdiff_t k = 0; k < (ptrdiff_t)loop->GetEdgesCount(); k++ ) {
      MpEdge * edge = loop->GetEdge( k );

      ptrdiff_t curveName = curvesNames[edge->GetCurve()];

      if ( curveName >= 0 && curveName <= nameCount ) {
        edge->SetName( curveName );
        edge->SetCurve( nullptr );
      }
    }
  }
}

//------------------------------------------------------------------------------
  /** \brief \ru Выдать использованные кривые.
             \en Get used curves. \~
    \details \ru Выдать использованные кривые и переименовать в соответствии с последними.\n
             \en Get used curves and rename in compliance with the last ones.\n \~
    \param[in]  curveList  - \ru Набор кривых.
                             \en Set of curves. \~
    \param[out] usedCurves - \ru Результат - использованные кривые.
                             \en The result are the used curves. \~
  */
// ---
template<class CurvesArray, class CurvesArray2>
void MpGraph::GetUsedCurves( const CurvesArray & curveList, CurvesArray2 & usedCurves )
{
  SArray<ptrdiff_t> curveName( 0, 1 );
  GetEdgeName( curveName );

  usedCurves.reserve( curveName.Count() );
  for ( ptrdiff_t j = 0; j < (ptrdiff_t)curveName.Count(); j++ ) {
    ptrdiff_t n = curveName[j];
    if ( n < 0 || n >( ptrdiff_t )curveList.size() - 1 )
      continue;
    usedCurves.push_back( curveList[n] );

    for ( ptrdiff_t i = 0; i < (ptrdiff_t)loops.Count(); i++ ) {
      MpLoop * loop = GetLoop( i );

      for ( ptrdiff_t k = 0; k < (ptrdiff_t)loop->GetEdgesCount(); k++ )
        if ( loop->GetEdge( k )->GetName() == n )
          loop->GetEdge( k )->SetName( j );
    }
  }

  nameCount = curveName.Count();
}

//------------------------------------------------------------------------------
  /** \brief \ru Определить ориентацию контуров.
             \en Determine the contours' orientation. \~
    \details \ru Определить ориентацию контуров по их вложенности.\n
             \en Determine the contours' orientation by its inclusion.\n \~
    \param[in] contourArray - \ru Список контуров, по которым строился граф.
                              \en List of contours the graph is built for. \~
  */
  // ---
template<class ContourArray>
void MpGraph::SetLoopsOrientation( const ContourArray & contourArray )
{
  ptrdiff_t count = loops.Count();
  ptrdiff_t cnt = contourArray.size();

  cnt -= count;
  if ( cnt < 0 || count == 0 )
    return;

  double t;
  MbContour * contour = nullptr;
  MbContour * contour0 = nullptr;
  MbCartPoint p, p1;
  ptrdiff_t i, j;
  MbeLocation loc, loc1;
  const double kk = M_E / 8;

  SArray<ptrdiff_t> numb( count, 2 );
  for ( i = 0; i < count; i++ ) {
    numb.Add( 0 );
    contour = contourArray[cnt + i];
    double tmin = contour->GetTMin();
    double tmax = contour->GetTMax();
    double dt = kk * ( tmax - tmin );

    // Лучше брать иррациональный параметр BUG_24550 // t = contour->GetTMin();    
    t = tmin + dt;
    contour->PointOn( t, p );
    // Лучше брать иррациональный параметр BUG_24550 // t = (t+contour->GetTMax())*0.5;
    t = tmax - dt;
    contour->PointOn( t, p1 );

    for ( j = 0; j < count; j++ ) {
      if ( i == j )
        continue;
      contour0 = contourArray[cnt + j];
      loc = contour0->PointLocation( p );
      if ( loc == loc_Inside )
        numb[i] = numb[i] + 1;
      else {
        if ( loc == loc_OnCurve ) {
          loc1 = contour0->PointLocation( p1 );
          if ( loc1 == loc_Inside )
            numb[i] = numb[i] + 1;
          else if ( loc1 == loc_OnCurve && i < j )
            numb[i] = numb[i] + 1;
        }
      }
    }
  }

  for ( i = 0; i < count; i++ )
    loops[i]->SetOrientation( numb[i] % 2 == 0 );

}

//------------------------------------------------------------------------------
/** \brief \ru Удалить вершину.
           \en Delete a vertex. \~
  \details \ru Удалить вершину и обнулить указатель.\n
           \en Delete a vertex and set the pointer to null.\n \~
  \param[in, out] vertex - \ru Вершина для удаления.
                           \en A vertex to delete. \~  
*/ // ---
inline
void DeleteVertex( MpVertex *& vertex )
{ 
  delete vertex;
  vertex = nullptr;
}


//------------------------------------------------------------------------------
/** \brief \ru Удалить ребро.
           \en Delete an edge. \~
  \details \ru Удалить ребро и обнулить указатель.\n
           \en Delete an edge and set the pointer to null.\n \~
  \param[in, out] edge - \ru Ребро для удаления.
                         \en An edge to delete. \~  
*/ // ---
inline
void DeleteEdge( MpEdge *& edge )
{
  delete edge;
  edge = nullptr;
}


//------------------------------------------------------------------------------
/** \brief \ru Удалить цикл.
           \en Delete a loop. \~
  \details \ru Удалить цикл и обнулить указатель.\n
           \en Delete a loop and set the pointer to null.\n \~
  \param[in, out] loop - \ru Цикл для удаления.
                         \en A loop to delete. \~  
*/ // ---
inline
void DeleteLoop( MpLoop *& loop )
{
  delete loop;
  loop = nullptr;
}


//------------------------------------------------------------------------------
/** \brief \ru Удалить граф.
           \en Delete a graph. \~
  \details \ru Удалить граф и обнулить указатель.\n
           \en Delete a graph and set the pointer to null.\n \~
  \param[in, out] graph - \ru Граф для удаления.
                          \en A graph to delete. \~  
*/ // ---
inline
void DeleteGraph( MpGraph *& graph )
{ 
  delete graph;
  graph = nullptr;
}


//------------------------------------------------------------------------------
/** \brief \ru Устранить разрывы в контуре.
           \en Remove contour gaps. \~
  \details \ru Устранить разрывы в контуре.
           \en Remove contour gaps. \~
  \param[in]  contour - \ru Контур.
                        \en A contour. \~
  \param[in]  accuracy   - \ru Ограничение по размеру разрыва (для вставки сегмента и поиска пересечения соседей.
                           \en Upper gap size. \~
  \param[in]  canInsert  -  \ru Можно ли вставлять сегменты.
                           \en Allow insert segments. \~
  \param[in]  canReplace - \ru Можно ли заменять сегменты.
                           \en Allow replace segments. \~
  \return \ru true, если контур изменился.
          \en true, if something have changed. \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC (bool) RemoveContourGaps( MbContour & contour,      // контур 
                                    double      accuracy,     // размер разрывов
                                    bool        canInsert,    // разрешение на вставку сегментов
                                    bool        canReplace ); // разрешение на подмену сегментов


//------------------------------------------------------------------------------
/** \brief \ru Построить контуры вокруг заданной точки.
           \en Create contours around the given point. \~
  \details \ru Построить контуры вокруг заданной точки. 
    Строится один внешний и несколько внутренних контуров с одним уровнем вложенности. 
    На вход не должны приходить составные кривые (контуры и ломаные).
           \en Create contours around the given point.
    One outer and several inner loops are constructed with single nesting level. 
    Do not send composite curves (contours, polylines, polygons and etc) as input curves. Break them down into on the components. \~
  \param[in]  curveList    - \ru Список кривых для построения.
                             \en List of curves for construction. \~
  \param[in]  p            - \ru Точка, вокруг которой строятся контуры.
                             \en A point the contours are constructed around. \~
  \param[out] usedCurves   - \ru Использованные кривые.
                             \en Used curves. \~
  \param[out] contourArray - \ru Результат построения - набор контуров.
                             \en The result of construction is a set of contours. \~
  \param[in]  accuracy     - \ru Погрешность определения пересечения и близости кривых.
                             \en The accuracy of determining the intersection of curves and proximity. \~
  \param[in]  strict       - \ru Если false, сборка производится с загрубленной точностью.
                             \en If false, the construction is performed roughly. \~
  \param[in]  version      - \ru Версия построения. Последняя версия Math::DefaultMathVersion().
                             \en The version of construction. The last version Math::DefaultMathVersion(). \~
  \param[in]  progInd      - \ru Индикатора прогресса выполнения.
                             \en Execution progress indicator. \~
  \return \ru Граф построения контуров.
          \en Contours construction graph. \~
  \warning \ru При использовании функций EncloseContoursBuilder, OuterContoursBuilder, ContoursReconstructor
           состояние флага strict и версия version должно использоваться одно в одном процессе обработки.
           \en While using functions EncloseContoursBuilder, OuterContoursBuilder, ContoursReconstructor,
           a single state of 'strict' flag and version must be used in one treatment process. \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC (MpGraph *) EncloseContoursBuilder( const RPArray<MbCurve> &   curveList, 
                                              const MbCartPoint &        p,
                                                    PArray<MbCurve> &    usedCurves, 
                                                    PArray<MbContour> &  contourArray,
                                                    double               accuracy, 
                                                    bool                 strict, 
                                                    VERSION              version,
                                                    IProgressIndicator * progInd = nullptr );

//------------------------------------------------------------------------------
/** \brief \ru Построить контуры вокруг заданной точки.
           \en Create contours around the given point. \~
  \details \ru Построить контуры вокруг заданной точки. 
    Строится один внешний и несколько внутренних контуров с одним уровнем вложенности. 
    На вход не должны приходить составные кривые (контуры и ломаные).
           \en Create contours around the given point.
    One outer and several inner loops are constructed with single nesting level. 
    Do not send composite curves (contours, polylines, polygons and etc) as input curves. Break them down into on the components. \~
  \param[in]  curveList    - \ru Список кривых для построения.
                             \en List of curves for construction. \~
  \param[in]  p            - \ru Точка, вокруг которой строятся контуры.
                             \en A point the contours are constructed around. \~
  \param[out] usedCurves   - \ru Использованные кривые.
                             \en Used curves. \~
  \param[out] contourArray - \ru Результат построения - набор контуров.
                             \en The result of construction is a set of contours. \~
  \param[in]  accuracy     - \ru Погрешность определения пересечения и близости кривых.
                             \en The accuracy of determining the intersection of curves and proximity. \~
  \param[in]  strict       - \ru Если false, сборка производится с загрубленной точностью.
                             \en If false, the construction is performed roughly. \~
  \param[in]  version      - \ru Версия построения. Последняя версия Math::DefaultMathVersion().
                             \en The version of construction. The last version Math::DefaultMathVersion(). \~
  \param[in]  progInd      - \ru Индикатора прогресса выполнения.
                             \en Execution progress indicator. \~
  \return \ru Граф построения контуров.
          \en Contours construction graph. \~
  \warning \ru При использовании функций EncloseContoursBuilder, OuterContoursBuilder, ContoursReconstructor
           состояние флага strict и версия version должно использоваться одно в одном процессе обработки.
           \en While using functions EncloseContoursBuilder, OuterContoursBuilder, ContoursReconstructor,
           a single state of 'strict' flag and version must be used in one treatment process. \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC (MpGraph *) EncloseContoursBuilder( const c3d::PlaneCurvesSPtrVector &   curveList, 
                                              const MbCartPoint &                  p,
                                                    c3d::PlaneCurvesSPtrVector &   usedCurves,
                                                    c3d::PlaneContoursSPtrVector & contourArray,
                                                    double                         accuracy, 
                                                    bool                           strict, 
                                                    VERSION                        version,
                                                    IProgressIndicator *           progInd = nullptr );


//------------------------------------------------------------------------------
/** \brief \ru Построить объемлющие контуры на основе заданных кривых.
           \en Construct enclosing contours on the basis of the given curves. \~
  \details \ru Построить объемлющие контуры на основе заданных кривых. 
    Строятся внешние и внутренние контуры с произвольным уровнем вложенности. 
    На вход не должны приходить составные кривые (контуры и ломаные).
           \en Construct enclosing contours on the basis of the given curves. 
    Outer and inner loops are constructed with an arbitrary level of inclusion. 
    Do not send composite curves (contours, polylines, polygons and etc) as input curves. Break them down into on the components. \~
  \param[in]  curveList    - \ru Список кривых для построения.
                             \en List of curves for construction. \~
  \param[out] contourArray - \ru Результат построения - набор контуров.
                             \en The result of construction is a set of contours. \~
  \param[in]  accuracy     - \ru Погрешность определения пересечения и близости кривых.
                             \en The accuracy of determining the intersection of curves and proximity. \~
  \param[in]  strict       - \ru Если false, сборка производится с загрубленной точностью.
                             \en If false, the construction is performed roughly. \~
  \param[in]  version      - \ru Версия построения. Последняя версия Math::DefaultMathVersion().
                             \en The version of construction. The last version Math::DefaultMathVersion(). \~
  \param[in]  progInd      - \ru Индикатора прогресса выполнения.
                             \en Execution progress indicator. \~
  \return \ru Граф построения контуров.
          \en Contours construction graph. \~
  \warning \ru При использовании функций EncloseContoursBuilder, OuterContoursBuilder, ContoursReconstructor
           состояние флага strict и версия version должно использоваться одно в одном процессе обработки.
           \en While using functions EncloseContoursBuilder, OuterContoursBuilder, ContoursReconstructor,
           a single state of 'strict' flag and version must be used in one treatment process. \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC (MpGraph *) OuterContoursBuilder( const RPArray<MbCurve> &   curveList, 
                                                  PArray<MbContour> &  contourArray,
                                                  double               accuracy, 
                                                  bool                 strict, 
                                                  VERSION              version,
                                                  IProgressIndicator * progInd = nullptr );

//------------------------------------------------------------------------------
/** \brief \ru Построить объемлющие контуры на основе заданных кривых.
           \en Construct enclosing contours on the basis of the given curves. \~
  \details \ru Построить объемлющие контуры на основе заданных кривых.
    Строятся внешние и внутренние контуры с произвольным уровнем вложенности.
    На вход не должны приходить составные кривые (контуры и ломаные).
           \en Construct enclosing contours on the basis of the given curves.
    Outer and inner loops are constructed with an arbitrary level of inclusion.
    Do not send composite curves (contours, polylines, polygons and etc) as input curves. Break them down into on the components. \~
  \param[in]  curveList    - \ru Список кривых для построения.
                             \en List of curves for construction. \~
  \param[out] contourArray - \ru Результат построения - набор контуров.
                             \en The result of construction is a set of contours. \~
  \param[in]  accuracy     - \ru Погрешность определения пересечения и близости кривых.
                             \en The accuracy of determining the intersection of curves and proximity. \~
  \param[in]  strict       - \ru Если false, сборка производится с загрубленной точностью.
                             \en If false, the construction is performed roughly. \~
  \param[in]  version      - \ru Версия построения. Последняя версия Math::DefaultMathVersion().
                             \en The version of construction. The last version Math::DefaultMathVersion(). \~
  \param[in]  progInd      - \ru Индикатора прогресса выполнения.
                             \en Execution progress indicator. \~
  \return \ru Граф построения контуров.
          \en Contours construction graph. \~
  \warning \ru При использовании функций EncloseContoursBuilder, OuterContoursBuilder, ContoursReconstructor
           состояние флага strict и версия version должно использоваться одно в одном процессе обработки.
           \en While using functions EncloseContoursBuilder, OuterContoursBuilder, ContoursReconstructor,
           a single state of 'strict' flag and version must be used in one treatment process. \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC( MpGraph * ) OuterContoursBuilder( const c3d::PlaneCurvesSPtrVector &   curveList,
                                                   c3d::PlaneContoursSPtrVector & contourArray,
                                                   double                         accuracy,
                                                   bool                           strict,
                                                   VERSION                        version,
                                                   IProgressIndicator *           progInd = nullptr );

//------------------------------------------------------------------------------
/** \brief \ru Перестроить контуры, построенные ранее вокруг точки.
           \en Reconstruct contours constructed around the point before. \~
  \details \ru Перестроить контуры, построенные ранее вокруг точки.
    Функция перестраивает граф, построенный функцией EncloseContoursBuilder.
           \en Reconstruct contours constructed around the point before.
    The function reconstructs the graph constructed by EncloseContoursBuilder function. \~
  \param[in]  curveList    - \ru Список кривых для построения.
                             \en List of curves for construction. \~
  \param[in]  graph        - \ru Граф для перестроения.
                             \en A graph to reconstruct. \~
  \param[out] contourArray - \ru Результат построения - набор контуров.
                             \en The result of construction is a set of contours. \~
  \param[in]  accuracy     - \ru Погрешность определения пересечения и близости кривых.
                             \en The accuracy of determining the intersection of curves and proximity. \~
  \param[in]  strict       - \ru Если false, сборка производится с загрубленной точностью.
                             \en If false, the construction is performed roughly. \~
  \param[in]  version      - \ru Версия построения. Последняя версия Math::DefaultMathVersion().
                             \en The version of construction. The last version Math::DefaultMathVersion(). \~
  \param[in]  progInd      - \ru Индикатора прогресса выполнения.
                             \en Execution progress indicator. \~
  \return \ru Граф построения контуров.
          \en Contours construction graph. \~
  \warning \ru При использовании функций EncloseContoursBuilder, OuterContoursBuilder, ContoursReconstructor
           состояние флага strict и версия version должно использоваться одно в одном процессе обработки.
           \en While using functions EncloseContoursBuilder, OuterContoursBuilder, ContoursReconstructor,
           a single state of 'strict' flag and version must be used in one treatment process. \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC (MpGraph *) ContoursReconstructor( const RPArray<MbCurve> &   curveList, 
                                                   MpGraph *            graph, 
                                                   PArray<MbContour> &  contourArray,
                                                   double               accuracy, 
                                                   bool                 strict, 
                                                   VERSION              version,
                                                   IProgressIndicator * progInd = nullptr );

//------------------------------------------------------------------------------
/** \brief \ru Перестроить контуры, построенные ранее вокруг точки.
           \en Reconstruct contours constructed around the point before. \~
  \details \ru Перестроить контуры, построенные ранее вокруг точки.
    Функция перестраивает граф, построенный функцией EncloseContoursBuilder.
           \en Reconstruct contours constructed around the point before.
    The function reconstructs the graph constructed by EncloseContoursBuilder function. \~
  \param[in]  curveList    - \ru Список кривых для построения.
                             \en List of curves for construction. \~
  \param[in]  graph        - \ru Граф для перестроения.
                             \en A graph to reconstruct. \~
  \param[out] contourArray - \ru Результат построения - набор контуров.
                             \en The result of construction is a set of contours. \~
  \param[in]  accuracy     - \ru Погрешность определения пересечения и близости кривых.
                             \en The accuracy of determining the intersection of curves and proximity. \~
  \param[in]  strict       - \ru Если false, сборка производится с загрубленной точностью.
                             \en If false, the construction is performed roughly. \~
  \param[in]  version      - \ru Версия построения. Последняя версия Math::DefaultMathVersion().
                             \en The version of construction. The last version Math::DefaultMathVersion(). \~
  \param[in]  progInd      - \ru Индикатора прогресса выполнения.
                             \en Execution progress indicator. \~
  \return \ru Граф построения контуров.
          \en Contours construction graph. \~
  \warning \ru При использовании функций EncloseContoursBuilder, OuterContoursBuilder, ContoursReconstructor
           состояние флага strict и версия version должно использоваться одно в одном процессе обработки.
           \en While using functions EncloseContoursBuilder, OuterContoursBuilder, ContoursReconstructor,
           a single state of 'strict' flag and version must be used in one treatment process. \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC( MpGraph * ) ContoursReconstructor( const c3d::PlaneCurvesSPtrVector & curveList,
                                                    MpGraph *                    graph,
                                                    c3d::PlaneContoursSPtrVector contourArray,
                                                    double                       accuracy,
                                                    bool                         strict,
                                                    VERSION                      version,
                                                    IProgressIndicator *         progInd = nullptr );


#endif // __CONTOUR_GRAPH_H
