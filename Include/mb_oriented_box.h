//////////////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Ограничивающий параллелепипед с произвольной ориентацией в пространстве.
         \en Arbitrarily oriented bounding box (OBB). \~
*/
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __MB_ORIENTED_BOX_H
#define __MB_ORIENTED_BOX_H

#include <array>
#include <mb_cart_point3d.h>
#include <mb_matrix3d.h>

//----------------------------------------------------------------------------------------
/**  \brief \ru Ограничивающий параллелепипед с произвольной ориентацией в пространстве.
            \en An arbitrarily oriented bounding box (OBB) that includes some geometric object within box boundaries.\~

  \details \ru Ограничивающий параллелепипед определяет область 3D-пространства, более или менее плотно 
    обволакивающую некоторый геометрический объект.\n
    Используется для быстрой оценки близости или непересечения трёхмерных объектов,
    содержащихся в параллелепипеде.
           \en A bounding box (parallelepiped, PE) specifies a domain of 3D-space that more or 
    less tightly packs some geometric object.\n
    It is used for quick estimation of the proximity or non-intersection of three-dimensional objects 
    which are contained in the "box". \~
  \ingroup Mathematic_Base_3D
*/
//---
class MATH_CLASS MbOrientedBox
{
private:
  static constexpr size_t vertNb = 8;   ///< \ru Количество вершин параллелепипеда. \en. Number of vertices of the box.\~
  MbCartPoint3D m_center;                   ///< \ru Центр параллелепипеда. \en Center of the parallelepiped.\~
  MbVector3D    m_xAxis, m_yAxis, m_zAxis;  ///< \ru Ортогонормированная тройка векторов ориентации. \en The orthogonormal triplet of orientation vectors.\~

  /** \brief  \ru Границы векторного интервала ограничивающего объема, заданного по осям ящика, но от начала мировой СК.
              \en Lower and upper bounds of the OBB intervals. Specified on the box axies but via WCS origin.\~
  */
  MbVector3D    m_lBnd, m_uBnd;               
  double        m_xDim, m_yDim, m_zDim;     ///< \ru Половина длины, ширины, высоты. \en Half the box dimension values.
  std::array<MbCartPoint3D,vertNb> m_verts; ///< \ru Все восемь вершин. \en Eight vertices of the PE.\~

public:
  /// \ru Конструктор пустого пустого объема. \en Construct an empty box.
  MbOrientedBox();
  MbOrientedBox( const MbOrientedBox & );
  /// \ru Оператор присвоения. \en Assignment operator. \~
  MbOrientedBox & operator = ( const MbOrientedBox & );

public: /*  Functions of the box coordinate system. The bounding box has
            its proper orienting LCS that provides corresponding functions like #MbPlacement3D.
        */
  const MbCartPoint3D & GetOrigin() const { return m_center; }
  const MbVector3D &    GetAxisX() const { return m_xAxis; }
  const MbVector3D &    GetAxisY() const { return m_yAxis; }
  const MbVector3D &    GetAxisZ() const { return m_zAxis; }
  const MbVector3D &    GetAxis( size_t i ) const { return i == 0 ? m_xAxis : (i==1 ? m_yAxis : m_zAxis); }

public: /*  Properties of the oriented parallelepiped.
        */
        /** \brief \ru Геометрический центр ограничивающего объема находится в середине 
                       главной диагонали параллелепипеда.
                   \en Geometric center of the bounding box is in the middle of the main diagonal of PE.\~
        */
  const MbCartPoint3D & Centre() const { return m_center; }  

        /** \brief  \ru Вершина параллелепипеда по индексу от 0 до 7.
                    \en One of the eight vertices by 0-based index.\~
            \note   \ru Правило нумерации вершин приведено в "mb_oriented_box.h" ниже объявления функции.
                    \en The rule of numbering vertices is given in "mb_oriented_box.h" below the function declaration. \~
        */
  const MbCartPoint3D & Vertex( size_t i ) const { return m_verts[i]; }        
        /*
          The rule of the numbering is not equal to MbCube.
                 Z
                 |
                 4--------------6   
                /|             /|
               / |            / |
              /  |           /  |
             5---+----------7   |
             |   |          |   |
             |   0----------+---2 - Y
             |  /           |  /
             | /            | /
             |/             |/ 
             1--------------3 
            /
           X
        */

        /// \ru Половина наибольшей диагонали параллелепипеда в собственной ЛСК. \en Half of the longest diagonal of the parallelepiped in the proper coordinate system. 
        MbVector3D      SemiDiagonal() const { return MbVector3D( m_xDim, m_yDim, m_zDim ); }
        /// \ru Размер ящика, равный наибольшему из размеров по трем осям. \en The size of the box is equal to the maximum of the dimensions along its axes.
        double          SizeMax() const; 

public: /*  Set-theoretic functions. Теоретико-множественные функции.
        */

  /** \brief \ru Проверка на пустоту. \en The check for emptiness.\~
      \details \ru Параллелепипед считается пустым, если он не содержит ни одной точки 3D-пространства.
               \en A bounding box is empty if it doesn't contain any points of 3D-space. \~
  */
  bool  IsEmpty() const { return ( m_lBnd.x > m_uBnd.x ) || ( m_lBnd.y > m_uBnd.y ) || ( m_lBnd.z > m_uBnd.z ); }

  /** \brief \ru Пересекается ли параллелепипед с другим параллелепипедом. \en Whether the box intersects another box or not.\~
      \return \ru true, если пересекается.
              \en Returns true if it intersects. \~
  */  
  bool  Intersect( const MbOrientedBox & ) const;

  /// \ru Включить в подмножество ящика точку с сохранением ориентации. \en Include a point in the box subset with orientation preserved.\~
  void  Include( const MbCartPoint3D & ); 

public: /* Setting and transforming functions.
        */
  /** \brief \ru Задать положение и ориентацию параллелепипеда. \en Set the position and orientation of PE.\~
      \details \ru Функция задает положение и ориентацию, а именно координаты центра и векторную пару осей (X,Y).
               \en The func sets the position and orientation, namely coordinates of center and vector pair of (X,Y)-axies.\~
  */
  MbOrientedBox &   SetOXY( const MbCartPoint3D & centre, const MbVector3D &, const MbVector3D & );

  /// \ru Задать полу-размеры параллелепипеда по трем осям (X,Y,Z). \en Sets the semi-dimensions of the parallelepiped by the axies (X,Y,Z).\~
  MbOrientedBox &   SetSize( double wx, double wy, double wz ) { m_xDim = wx; m_yDim = wy; m_zDim = wz; return InitData(); }

  /// \ru Сделать параллелепипед пустым. \en Set the box empty.\~
  MbOrientedBox &   SetEmpty();

  /** \brief \ru Преобразовать согласно матрице. \en Transform according to the matrix.\~
      \note \ru Корректный результат гарантируется только для изометрических преобразований 
                (поворот,перенос,отражение) или при равномерных масштабированиях.
            \en The correct result is only guaranteed for isometric transformations 
                (rotations,translations and reflections) or uniform scalings.  \~
  */
  MbOrientedBox &   Transform( const MbMatrix3D & );

  /** \brief \ru Перенести на заданный вектор. \en Translate to the given vector.\~
  */
  MbOrientedBox &   Move( const MbVector3D & );

  /** \brief  \ru Нижняя граница интервального радиус-вектора, охватывающая область ящика (по проекциям осей, но через начало МСК).
              \en The lower bound of the interval radius-vector covering the box range (projections on the box axies but via the world origin).\~
  */
  const MbVector3D  LowerBnd() const { return m_lBnd; }

  /** \brief  \ru Верхняя граница интервального радиус-вектора, охватывающая область ящика (в глобальной СК).
              \en The upper bound of the interval radius-vector covering the box range (in the local CS).\~
  */
  const MbVector3D  UpperBnd() const { return m_uBnd; }

private: // Interval functions.
  double  LowerBnd( size_t i ) const { return m_lBnd[i]; }
  double  UpperBnd( size_t i ) const { return m_uBnd[i]; }

  /** \brief  \ru Инициализация внутреннего представления: интервальный вектор и вершины.
              \en Initializing internal representation: the interval vector and vertices. \~
  */
  MbOrientedBox & InitData() { return InitBounds().InitVertices(); }

  /** \brief  \ru Переинициализировать интервальный вектор.
              \en Re-initialize the interval vector. \~
  */
  MbOrientedBox & InitBounds();

  /** \brief  \ru Переинициализировать вершины всем остальным данным (вершины - зависимые).
              \en Re-initialize vertices as dependent data. \~
  */
  MbOrientedBox & InitVertices();
};


//---------------------------------------------------------------------------------------
/** \brief  \ru Матрица получения ограничивающего параллелепипеда из единичного куба. 
            \en Transformation matrix yielding bounding parallelepiped from the unit cube. \~
*/
//---
inline MbMatrix3D & GetMatrixFromUCube( const MbOrientedBox & bBox, MbMatrix3D & m )
{
  m = MbMatrix3D::identity;
  const MbVector3D & hDiag = bBox.SemiDiagonal();
  m.SetOrigin() = bBox.GetOrigin();
  m.SetAxisZ() = 2.0 * bBox.GetAxisZ() * hDiag.z;
  m.SetAxisX() = 2.0 * bBox.GetAxisX() * hDiag.x;
  m.SetAxisY() = 2.0 * bBox.GetAxisY() * hDiag.y;
  return m;
}

//------------------------------------------------------------------------------
//
//---
inline MbOrientedBox & MbOrientedBox::operator = ( const MbOrientedBox & box )
{
  m_center = box.m_center;
  m_xAxis  = box.m_xAxis;
  m_yAxis  = box.m_yAxis;
  m_zAxis  = box.m_zAxis;
  m_xDim = box.m_xDim;
  m_yDim = box.m_yDim;
  m_zDim = box.m_zDim;

  m_verts = box.m_verts;
  m_lBnd = box.m_lBnd;
  m_uBnd = box.m_uBnd;

  return *this;
}

//-----------------------------------------------------------------------------
// \ru Сделать параллелепипед пустым. \en Set the box empty.\~
//---
inline MbOrientedBox & MbOrientedBox::SetEmpty()
{
  m_center = MbCartPoint3D::origin;
  m_xAxis  = MbVector3D::xAxis;
  m_yAxis  = MbVector3D::yAxis;
  m_zAxis  = MbVector3D::zAxis;
  m_xDim = m_yDim = m_zDim = 0;

  m_verts.fill( m_center );
  m_lBnd.x = m_lBnd.y = m_lBnd.z =  MB_MAXDOUBLE;
  m_uBnd.x = m_uBnd.y = m_uBnd.z = -MB_MAXDOUBLE;
  return *this;
}

//-----------------------------------------------------------------------------
// Размер ящика, равный наибольшему размеру по трем осям.
//---
inline double MbOrientedBox::SizeMax() const
{
  return 2.0 * (std::max)( m_xDim, (std::max)(m_yDim,m_zDim) );
}

#endif // __MB_ORIENTED_BOX_H
