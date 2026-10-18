////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Поверхность, заданная прямоугольной матрицей точек.
         \en A surface specified by rectangular matrix of points. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __SURF_POLYSURFACE_H
#define __SURF_POLYSURFACE_H


#include <templ_array2.h>
#include <templ_three_states.h>
#include <surface.h>


struct            MbNurbsPointInfo;
class   MbSurfaceIntersectionCurve;


//------------------------------------------------------------------------------
/** \brief \ru Поверхность, заданная прямоугольной матрицей точек.
           \en A surface specified by rectangular matrix of points. \~
  \details \ru Поверхность, заданная прямоугольной матрицей контрольных точек размерности vcount ucount,  
    является родительским классом NURBS поверхности MbSplineSurface.
           \en A surface specified by rectangular matrix of  control points with dimension vcount * ucont  
    is a parent class of NURBS surface MbSplineSurface. \~
  \ingroup Surfaces
*/
// ---
class MATH_CLASS MbPolySurface : public MbSurface {
protected:
  Array2<MbCartPoint3D> points;  ///< \ru Матрица контрольных точек. \en A matrix of control points. 
  bool                  uclosed; ///< \ru Признак замкнутости по первому параметру u. \en An attribute of closedness by u-parameter. 
  bool                  vclosed; ///< \ru Признак замкнутости по второму параметру v. \en An attribute of closedness by v-parameter. 
  size_t                ucount;  ///< \ru Количество колонок. \en Count of columns. 
  size_t                vcount;  ///< \ru Количество строк. \en Count of rows. 

protected:
  /// \ru Пустой конструктор. \en Empty constructor. 
  MbPolySurface(); 
  /** \brief \ru Конструктор поверхности.
             \en Constructor of surface. \~
      \details \ru Конструктор поверхности по матрице точек и замкнутости по u и v.
               \en Constructor of surface by the matrix of points and closedness in directions of u and v. \~
      \param[in] vert - \ru Матрица точек.
                        \en Matrix of points. \~
      \param[in] ucl - \ru Замкнута ли поверхность по параметру u.
                       \en Whether a surface is closed in u-parameter direction. \~
      \param[in] vcl - \ru Замкнута ли поверхность по параметру v.
                       \en Whether a surface is closed in v-parameter direction. \~
  */
  MbPolySurface( Array2<MbCartPoint3D> & vert, bool ucl, bool vcl );
  /** \brief \ru Конструктор поверхности.
             \en Constructor of surface. \~
      \details \ru Конструктор поверхности по массиву точек и замкнутости по u и v. Формирует матрицу точек по заданному массиву. 
      Заполняет сначала первую колонку сверх вниз, затем вторую и т.д. В массиве должно быть nu*nv элементов.
               \en Surface constructor by points array and closedness in direction of u and v. It forms a points matrix by a given array. 
      It fills downward the first column at first, then the second column etc. Array should contain nu*nv elements. \~
      \param[in] nu - \ru Число колонок в матрице точек.
                      \en Columns count in points matrix. \~
      \param[in] nv - \ru Число колонок в матрице точек.
                      \en Columns count in points matrix. \~
      \param[in] vert - \ru Множество точек.
                        \en A set of points. \~
      \param[in] ucl - \ru Замкнута ли поверхность по параметру u.
                       \en Whether a surface is closed in u-parameter direction. \~
      \param[in] vcl - \ru Замкнута ли поверхность по параметру v.
                       \en Whether a surface is closed in v-parameter direction. \~
  */
  MbPolySurface( size_t nu, size_t nv, const SArray<MbCartPoint3D> & vert, bool ucl, bool vcl );
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbPolySurface( const MbPolySurface & ); 
public:
  virtual ~MbPolySurface();

public:
  VISITING_CLASS( MbPolySurface );

  /** \ru \name Общие функции геометрического объекта
      \en \name Common functions of a geometric object
      \{ */
  //virtual MbeSpaceType  IsA()  const = 0; // \ru Тип элемента \en Type of element
  MbeSpaceType  Type() const override;     // \ru Тип элемента \en Type of element 
  //virtual MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const = 0;                // \ru Сделать копию элемента \en Make a copy of an element.
  //virtual bool    IsSame   ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const = 0; // \ru Являются ли объекты равными \en Determine whether objects are equal
  //virtual bool    SetEqual ( const MbSpaceItem & ) = 0; // \ru Сделать равным \en Make equal
  void    Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix
  void    Move     ( const MbVector3D &, MbRegTransform * = nullptr ) override; // \ru Сдвиг \en Translation
  void    Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси \en Rotate about an axis

  //virtual void    GetProperties( MbProperties & ) = 0; // \ru Выдать свойства объекта \en Get properties of the object
  //virtual void    SetProperties( const MbProperties & ) = 0; // \ru Записать свойства объекта \en Set properties of the object
  //virtual void    GetBasisPoints( MbControlData3D & ) const = 0; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  //virtual void    SetBasisPoints( const MbControlData3D & ) = 0; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.
  /** \} */

  /** \ru \name Функции описания области определения поверхности
      \en \name Functions for surface domain description
      \{ */
  //virtual double  GetUMin() const = 0;
  //virtual double  GetVMin() const = 0;
  //virtual double  GetUMax() const = 0;
  //virtual double  GetVMax() const = 0;
  bool    IsUClosed() const override; // \ru Замкнута ли поверхность по параметру u. \en Whether a surface is closed in u-parameter direction. 
  bool    IsVClosed() const override; // \ru Замкнута ли поверхность по параметру v. \en Whether a surface is closed in v-parameter direction. 
  /** \} */

  /** \ru \name Функции для работы в области определения поверхности
            Функции PointOn, Derive... поверхностей корректируют параметры
            при выходе их за пределы прямоугольной области определения параметров.
      \en \name Functions for working at surface domain
            Functions PointOn, Derive... correct parameters
            when getting out of rectangular domain bounds.
      \{ */
  //virtual void    PointOn  ( double & u, double & v, MbCartPoint3D & ) const = 0; // \ru Точка на поверхности \en A point on surface.
  //virtual void    DeriveU  ( double & u, double & v, MbVector3D & ) const = 0; // \ru Первая производная по u \en First derivative with respect to u
  //virtual void    DeriveV  ( double & u, double & v, MbVector3D & ) const = 0; // \ru Первая производная по v \en First derivative with respect to v
  //virtual void    DeriveUU ( double & u, double & v, MbVector3D & ) const = 0; // \ru Вторая производная по u \en Second derivative with respect to u
  //virtual void    DeriveVV ( double & u, double & v, MbVector3D & ) const = 0; // \ru Вторая производная по v \en Second derivative with respect to v
  //virtual void    DeriveUV ( double & u, double & v, MbVector3D & ) const = 0; // \ru Вторая производная по uv \en Second derivative with respect to u and v
  //virtual void    DeriveUUU( double & u, double & v, MbVector3D & ) const = 0;
  //virtual void    DeriveUUV( double & u, double & v, MbVector3D & ) const = 0;
  //virtual void    DeriveUVV( double & u, double & v, MbVector3D & ) const = 0;
  //virtual void    DeriveVVV( double & u, double & v, MbVector3D & ) const = 0;
  /** \} */
  /** \ru \name Функции доступа к группе данных для работы внутри и вне области определения параметров поверхности.
      \en \name Functions for get of the group of data inside and outside the surface's domain of parameters.
      \{ */
  //virtual void    Explore( double & u, double & v, bool ext,
  //                         MbCartPoint3D & pnt, MbVector3D & uDer, MbVector3D & vDer,
  //                         MbVector3D * uuDer, MbVector3D * vvDer, MbVector3D * uvDer, MbVector3D * nor ) const = 0;
  /** \} */

  /** \ru \name Общие функции поверхности
      \en \name Common functions of surface.
      \{ */
  /// \ru Установить признак замкнутости по U. \en Set the attribute of closedness in direction of u. 
  virtual void    SetUClosed( bool cls ); 
  /// \ru Установить признак замкнутости по V. \en Set the attribute of closedness in direction of v. 
  virtual void    SetVClosed( bool cls ); 
  /// \ru Перестроить поверхность. \en Rebuild a surface. 
  virtual void    Rebuild() = 0; 

          /// \ru Вернуть количество строк в матрице точек. \en Return rows count in points matrix. 
  size_t  GetPointsLines  () const { return points.Lines();   } 
          /// \ru Вернуть количество столбцов в матрице точек. \en Return columns count in points matrix. 
  size_t  GetPointsColumns() const { return points.Columns(); } 
  /** \brief \ru Выдать точку, расположенную в i строке, j колонке.
              \en Get the point located at row i and column j. \~
      \details \ru Выдать точку, расположенную в i строке, j колонке.\n
                \en Get the point located at row i and column j.\n \~
      \param[in] i - \ru Строка.
                      \en String. \~
      \param[in] j - \ru Колонка.
                      \en Column. \~
      \param[in,out] pnt - \ru Запрашиваемая точка.
                            \en Requested point. \~
  */
  void    GetPoint ( size_t i, size_t j, MbCartPoint3D & pnt ) const { pnt = points( i, j ); }
  /** \brief \ru Изменить точку, расположенную в i строке, j колонке.
              \en Change the point located at row i and column j. \~
      \details \ru Изменить точку, расположенную в i строке, j колонке.\n
                \en Change the point located at row i and column j.\n \~
      \param[in] i - \ru Строка.
                      \en String. \~
      \param[in] j - \ru Колонка.
                      \en Column. \~
      \param[in] pnt - \ru Новая точка.
                        \en New point. \~
  */
  void    SetPoint( size_t i, size_t j, const MbCartPoint3D & pnt ) { points( i, j ).Init( pnt ); }
  /** \brief \ru Сдвинуть точку, расположенную в i строке, j колонке на заданный вектор.
              \en Translate the point located at row i and column j by the given vector. \~
      \details \ru Сдвинуть точку, расположенную в i строке, j колонке на заданный вектор.\n
                \en Translate the point located at row i and column j by the given vector.\n \~
      \param[in] i - \ru Строка.
                      \en String. \~
      \param[in] j - \ru Колонка.
                      \en Column. \~
      \param[in] v - \ru Вектор перемещения точки.
                      \en A vector of point translation. \~
  */
  void    MovePoint( size_t i, size_t j, const MbVector3D & v ) { points(i,j).Move(v); }

  /// \ru Получить количество колонок. \en Get count of columns. 
  size_t  GetPointsUCount() const { return ucount; } 
  /// \ru Получить количество строк. \en Get count of rows. 
  size_t  GetPointsVCount() const { return vcount; } 
  
  /** \brief \ru Заполнить матрицу точек.
              \en Fill points matrix. \~
      \details \ru Заполнить матрицу точек.\n
                \en Fill points matrix.\n \~
      \param[in] pnts - \ru Матрица точек.
                        \en Matrix of points. \~
  */
  bool    GetPoints( Array2<MbCartPoint3D> & pnts ) const { return pnts.Init( points ); }
  
  /** \brief \ru Выдать массив отрезков.
              \en Get the array of segments. \~
      \details \ru В функции строятся все горизонтальные отрезки между соседними точками и все вертикальные отрезки между соседними точками.
                \en The function constructs all horizontal segments between neighboring points and all vertical segments between neighboring points. \~
      \param[in] segments - \ru Множество для хранения отрезков.
                            \en Set for segments storage. \~
  */
  void    GetLineSegments( RPArray<MbCurve3D> & segments ) const; 
  
  /** \} */

  /** \ru \name Функции, предоставляющие интерфейс поверхности для сплайновой формы.
      \en \name Functions performing an interface for a surface of spline form.
      \{ */

  /** \brief \ru Получить узловой вектор по выбранному параметру.
             \en Get a knots vector by the chosen parameter. \~
      \details \ru Получить узловой вектор по выбранному параметру.\n
               \en Get a knots vector by the chosen parameter.\n \~
      \param[in] isU - \ru Определяет, по какой координате запрашивается узловой вектор: true - по u, false - по v.
                       \en Determines the requested coordinate of a knot vector: true - u, false - v. \~
      \param[in,out] knots - \ru Матрица для хранения узлового вектора.
                             \en Matrix for knot vector storage. \~
  */
  virtual void    GetKnots( bool isU, SArray<double> & knots ) const = 0;
  /** \brief \ru Получить матрицу весов вершин.
             \en Get the matrix of vertices weights. \~
      \details \ru Получить матрицу весов вершин.\n
               \en Get the matrix of vertices weights.\n \~
      \param[in,out] wts - \ru Матрица для заполнения значений весов.
                           \en A matrix for weights values filling. \~
  */
  virtual void    GetWeights( Array2<double> & wts ) const = 0;
  /** \brief \ru Вернуть массив узловых точек и их видимость для операции редактирования как сплайна.
             \en Return an array of knot points and their visibility for the operation of editing as spline. \~
      \details \ru Вернуть массив узловых точек и их видимость для операции редактирования как сплайна.\n
               \en Return an array of knot points and their visibility for the operation of editing as spline.\n \~
      \param[in,out] params - \ru Матрица контрольных точек с указанием видимости каждой контрольной точки для редактирования.
                              \en A matrix of control points with specifying of visibility of each control point for editing. \~
  */
  virtual void    GetPointsWithVisible ( Array2<MbNurbsPointInfo> & params ) const = 0; 
  /** \brief \ru Вычисление точек на поверхности, соответствующих узлам.
             \en Calculation of points on surface corresponding to knots. \~
      \details \ru Вычисление точек на поверхности, соответствующих узлам.\n
               \en Calculation of points on surface corresponding to knots.\n \~
      \param[in,out] params - \ru Матрица для хранения точек на поверхности, соответствующих контрольным точкам.
                              \en A matrix for keeping of points on surface corresponding to control points. \~
  */
  virtual void    CalculateUVParameters( Array2<MbCartPoint> & params ) const = 0;
  /** \brief \ru Вычисление точки на поверхности, соответствующей контрольной точке.
             \en Calculation of point on surface corresponding to control point. \~
      \details \ru Вычисление точки на поверхности, соответствующей контрольной точке.\n
               \en Calculation of point on surface corresponding to control point.\n \~
      \param[in] uIndex - \ru Столбец контрольной точки.
                          \en A column of control point. \~
      \param[in] vIndex - \ru Строка контрольной точки.
                          \en A row of control point. \~
      \param[in,out] point - \ru Точка на поверхности.
                             \en A point on surface. \~
      \return \ru true, если точка на поверхности успешно найдена.
              \en True if a point on surface was successfully found. \~
  */
  virtual bool    CalculateUVParameterForKnot( size_t uIndex, size_t vIndex, MbCartPoint & point ) const = 0;
  /** \brief \ru Удаление столбца контрольных точек без изменения поверхности.
             \en Deletion of a column of control points without changing of a surface. \~
      \details \ru Удаление столбца контрольных точек без изменения поверхности.\n
               \en Deletion of a column of control points without changing of a surface.\n \~
      \param[in] rowId - \ru Номер первого удаляемого столбца.
                         \en Index of the first deleted column. \~
      \param[in] num - \ru Количество удаляемых столбцов.
                       \en Count of deleted columns. \~
      \param[in] absEps - \ru Погрешность аппроксимации.
                          \en Approximation tolerance. \~
      \return \ru Число столбцов, которые удалось удалить.
              \en Count of columns which are succeeded to delete. \~
  */
  virtual size_t  RemoveUKnots( ptrdiff_t & rowId, ptrdiff_t num = 1, double absEps = Math::lengthEpsilon ) = 0; 

    /** \brief \ru Удаление строки контрольных точек без изменения поверхности.
               \en Deletion of a row of control points without changing of a surface. \~
      \details \ru Удаление строки контрольных точек без изменения поверхности.\n
               \en Deletion of a row of control points without changing of a surface.\n \~
      \param[in] rowId - \ru Номер первой удаляемой строки.
                         \en Index of the first deleted row. \~
      \param[in] num - \ru Количество удаляемых строк.
                       \en Count of deleted rows. \~
      \param[in] absEps - \ru Погрешность аппроксимации.
                          \en Approximation tolerance. \~
      \return \ru Число строк, которые удалось удалить.
              \en Count of rows which are succeeded to delete. \~
  */
  virtual size_t  RemoveVKnots( ptrdiff_t & rowId, ptrdiff_t num = 1, double absEps = Math::lengthEpsilon ) = 0; 
  /** \brief \ru Вставка ряда после ряда с номером idBegin без изменения поверхности по u.
             \en Insertion of a row after the row with the index idBegin without changing of a surface by u. \~
      \details \ru Вставка ряда после ряда с номером idBegin без изменения поверхности по u.\n
               \en Insertion of a row after the row with the index idBegin without changing of a surface by u.\n \~
      \param[in] idBegin - \ru Номер ряда, после которого будет вставлен новый ряд.
                           \en An index of the row a new row will be inserted after. \~
      \param[in] num - \ru Количество вставляемых рядов.
                       \en Count of inserted rows. \~
  */
  virtual void    InsertUKnotsInRegion( ptrdiff_t idBegin, ptrdiff_t num = 1 ) = 0;
  /** \brief \ru Вставка ряда после ряда с номером idBegin без изменения поверхности по v.
             \en Insertion of a row after the row with the index idBegin without changing of a surface by v. \~
      \details \ru Вставка ряда после ряда с номером idBegin без изменения поверхности по v.\n
               \en Insertion of a row after the row with the index idBegin without changing of a surface by v.\n \~
      \param[in] idBegin - \ru Номер ряда, после которого будет вставлен новый ряд.
                           \en An index of the row a new row will be inserted after. \~
      \param[in] num - \ru Количество вставляемых рядов.
                       \en Count of inserted rows. \~
  */
  virtual void    InsertVKnotsInRegion( ptrdiff_t idBegin, ptrdiff_t num = 1 ) = 0;
  /** \brief \ru Изменить порядок nurbs путем перестроения поверхности с помощью функции NurbsSurface.
             \en Change the order of NURBS by construction of a surface by the function NurbsSurface. \~
      \details \ru Изменить порядок nurbs путем перестроения поверхности с помощью функции NurbsSurface.\n
               \en Change the order of NURBS by construction of a surface by the function NurbsSurface.\n \~
      \param[in] newDegree - \ru Новый порядок поверхности по u.
                             \en New surface degree by u. \~
      \return \ru true, если аппроксимация выполнена успешно.
              \en True if approximation is succeeded. \~
  */
  virtual bool    ChangeUDegreeApprox ( size_t newDegree ) = 0;
  /** \brief \ru Изменить порядок nurbs путем перестроения поверхности с помощью функции NurbsSurface.
             \en Change the order of NURBS by construction of a surface by the function NurbsSurface. \~
      \details \ru Изменить порядок nurbs путем перестроения поверхности с помощью функции NurbsSurface.\n
               \en Change the order of NURBS by construction of a surface by the function NurbsSurface.\n \~
      \param[in] newDegree - \ru Новый порядок поверхности по v.
                             \en New surface degree by v. \~
      \return \ru true, если аппроксимация выполнена успешно.
              \en True if approximation is succeeded. \~
  */
  virtual bool    ChangeVDegreeApprox ( size_t newDegree ) = 0;

  /** \brief \ru Изменить порядок и количество узлов nurbs путем перестроения поверхности с помощью функции NurbsSurface.
             \en Change the order and the number of knots of NURBS by construction of a surface by the function NurbsSurface. \~
      \details \ru Изменить порядок и количество узлов nurbs путем перестроения поверхности с помощью функции NurbsSurface.\n
               \en Change the order and the number of knots of NURBS by construction of a surface by the function NurbsSurface.\n \~
      \param[in] nUDegree - \ru Новый порядок поверхности по u.
                            \en New surface degree by u. \~
      \param[in] nVDegree - \ru Новый порядок поверхности по v.
                            \en New surface degree by v. \~
      \param[in] nUCount - \ru Количество контрольных точек по u.
                           \en A number of control points in U direction. \~
      \param[in] nVCount - \ru Количество контрольных точек по v.
                           \en A number of control points in V direction. \~
      \return \ru true, если аппроксимация выполнена успешно.
              \en True if approximation is succeeded. \~
  */
  virtual bool    ChangeParametersApprox ( size_t nUDegree, size_t nVDegree, ptrdiff_t nUCount, ptrdiff_t nVCount ) = 0;
  /** \brief \ru Вычисление фиксированных контрольных точек.
             \en Calculation of fixed control points. \~
      \details \ru Вычисление узлов, которые должны быть неподвижны, чтобы при деформации поверхности кривые из 
        заданного массива не деформаровались.
               \en Calculation of knots which should be fixed to forbid the deformation of curves in the given array when a surface 
        deforms. \~
      \param[in] curves - \ru Множество кривых.
                          \en A set of curves. \~
      \param[in,out] fixedPoints - \ru Матрица, в которую заносятся данные о необходимости фиксации узлов 
        для сохранения кривых. Если элемент матрицы равен true - соответствующая ему 
        контрольная точка должна быть фиксирована.
              \en A matrix where the data about necessity of angles fixation is written 
        to save curves. If an element of matrix equals true then the control point 
        corresponding to it should be fixed. \~
      \return \ru true, вычисления выполнены успешно.
              \en True if calculations are successfully performed. \~
  */
  virtual bool    CalculateFixedPoints( const RPArray<const MbSurfaceIntersectionCurve> & curves, Array2<bool> & fixedPoints ) const = 0;
  /** \brief \ru Вычисление доли смещения узлов при перемещении со сглаживанием.
             \en Calculation of a shift part of knots during the translation with blending. \~
      \details \ru Известно перемещение одной контрольной точки. Перемещение остальных точек, помеченных как подвижные в 
        матрице movedPoints, зависит от направления ее перемещения, расстояния точки от линии перемещения (moveVector) 
        и функции сглаживания. Есть три режима сглаживания: выпуклый, вогнутый и плавный переход.
                \en A translation of one control point is known. Translation of other points which marked as movable in 
        the matrix movedPoints depends on the direction of its translation, the distance from the points to the translation line (moveVector) 
        and the function of blending. There are three modes of blending: convex, concave and smooth transition. \~
      \param[in] movedPoints - \ru Матрица, содержащая данные о перемещаемых точках. 
      Если элемент матрицы равен 1 - соответствующая ему контрольная точка может быть перемещена, 
      иначе - неподвижна.
                                \en A matrix containing data about moved points. 
      If an element of the matrix equals 1 then the corresponding control point can be moved, 
      otherwise - it is fixed. \~
      \param[in] uIndex - \ru Столбец перемещаемой контрольной точки, относительно которой будет сглаживание.
                          \en A column of a moved control point relative to which there will be the blending. \~
      \param[in] vIndex - \ru Строка перемещаемой контрольной точки, относительно которой будет сглаживание.
                          \en A row of a moved control point relative to which there will be the blending. \~
      \param[in] moveVector - \ru Вектор, по направлению которого смещается контрольная точка.
                              \en A vector in direction of which the control point is translated. \~
      \param[in] smoothType - \ru Тип сглаживания. \n
        dst_None - без сглаживания, dst_Convex - выпуклый, dst_Concave - вогнутый, dst_Smooth - плавный переход.
                              \en The type of blending. \n
        dst_None - no blending, dst_Convex - convex, dst_Concave - concave, dst_Smooth - smooth transition. \~
      \param[in] smoothDegree - \ru Степень функции сглаживания. Положительное число.
                                \en A degree of the blending function. A positive value. \~ 
      \param[in,out] partsPoints - \ru Матрица с данными о долях смещения каждой точки относительно смещения перемещаемой точки.
                                   \en A matrix with the data about a part of shift of each point relative to the moved point. \~
      \return \ru true, если вычисления проведены успешно.
              \en True if the calculations were successfully performed.. \~
  */
  virtual bool    CalculatePartsForSpecMove( const Array2<bool> & movedPoints, 
                                             size_t uIndex, size_t vIndex, 
                                             const MbVector3D & moveVector,
                                             MbeDirectSmoothType smoothType,
                                             double smoothDegree,
                                             Array2<double> & partsPoints ) const = 0;

private:
  void    operator = ( const MbPolySurface & ); // \ru Не реализовано. \en Not implemented. 
  /** \} */

  DECLARE_PERSISTENT_CLASS( MbPolySurface )
};

IMPL_PERSISTENT_OPS( MbPolySurface )


#endif // __SURF_POLYSURFACE_H
