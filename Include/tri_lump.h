////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Математическая грань и ее расчитанная решетка.
         \en Mathematical face and its calculated grid. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TRI_LUMP_H
#define __TRI_LUMP_H

                      
#include <templ_sptr.h>
#include <topology.h>
#include <mesh_primitive.h>
#include <lump.h>


//----------------------------------------------------------------------------------------
/** \brief \ru Математическая грань и ее рассчитанная решетка.
           \en Mathematical face and its calculated grid. \~
  \ingroup Polygonal_Objects
*/
// ---
class MATH_CLASS MbFaceAndGrid {
  c3d::ConstFaceSPtr face; ///< \ru Грань. \en A face. 
public:
  c3d::ConstGridSPtr grid; ///< \ru Триангуляция грани. \en A face triangulation.

public:
  /** \brief  \ru Конструктор по грани и ее триангуляции.\n
              \en Constructor by face and its triangulation.\n \~
    \param[in] _face - \ru Грань.
                       \en A face. \~
    \param[in] _grid - \ru Соответствующая грани триангуляционная решетка.
                       \en A triangulation grid which is corresponded to a face. \~
  */
  MbFaceAndGrid( const MbFace & _face, const MbGrid & _grid )
    : face( &_face )
    , grid( &_grid )  
  {}

  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbFaceAndGrid( const MbFaceAndGrid & faceGrid )
    : face( faceGrid.face )
    , grid( faceGrid.grid )   
  {}
  
  const MbFace & Face() const { return *face; }  

  /** \brief \ru Получить в виде пары указателей.
             \en Get as a pair of pointers.
  */
  template<class FacePtr, class GridPtr>
  void GetAsPair( std::pair<FacePtr, GridPtr> & tied ) const
  { 
    tied.first = face, tied.second = grid;
  }

  MbFaceAndGrid & operator = ( const MbFaceAndGrid & faceGrid )
  {
    face = faceGrid.face;
    grid = faceGrid.grid;
    return *this;
  }
  };


//------------------------------------------------------------------------------     
/** \brief \ru Тело с массивом граней и триангуляционных решеток.
           \en A solid with an array of faces and triangulation grids. \~
  \details \ru Тело, определенное в системе координат,
    с массивом граней и триангуляционных решеток.
           \en A solid determined in the coordinate system
    with an array of faces and triangulation grids. \~
  \ingroup Polygonal_Objects
*/
// ---
struct MATH_CLASS MbLumpAndFaces 
{
public:
  const void  *               m_comp;   ///< \ru Указатель на компонент. \en A pointer to the component. 
  MbMatrix3D                  m_toWCS;  ///< \ru Матрица пересчета в мир. \en A matrix of transformation to the world coordinate system. 
  std::vector<MbFaceAndGrid>  faces;    ///< \ru Множество пар "грань и её решетка". \en An array of pairs "a face and its grid". 
  double                      sag;      ///< \ru Точность, с которой рассчитывали решетку. \en The tolerance which was used for the calculation of a grid. 

public:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по матрице и компоненту.\n Создается тело с пустым массивом граней.
             \en Constructor by matrix and component.\n A solid with an empty array of faces is created. \~  
    \param[in] _from - \ru Матрица преобразования.
                       \en A transform matrix. \~
    \param[in] _comp - \ru Указатель на компонент.
                       \en A pointer to the component. \~
    \param[in] _sag  - \ru Точность, с которой рассчитывали решетку.
                       \en The tolerance which was used for the calculation of a grid. \~
  */
  MbLumpAndFaces( const MbMatrix3D & _from, void * _comp, double _sag )
    : m_comp ( _comp )
    , m_toWCS( _from )
    , faces()
    , sag( _sag  )       
  {}

  /** \brief  \ru Добавить грань с триангуляцией. \en Add face with triangulation. \~
      \param[in] face - \ru Грань с триангуляционной решеткой.
                        \en A face with triangulation grid. \~
  */
  void AddFace( const MbFaceAndGrid & face ) { faces.push_back( face ); }

  // \ru Объявление конструктора копирования и оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration without implementation of the copy-constructor and assignment operator to prevent an assignment by default. 
OBVIOUS_PRIVATE_COPY(MbLumpAndFaces)
};


#endif // __TRI_LUMP_H
