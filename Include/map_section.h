////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Модуль проецирования. Структуры данных, отображающие вид сечения множества тел.
         \en The projection module. Data structures which map a view of section of a solids set. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef  __MAP_SECTION_H
#define  __MAP_SECTION_H


#include <templ_ss_array.h>
#include <templ_p_array.h>
#include <cur_contour.h>
#include <mb_operation_result.h>
#include <topology_faceset.h>
#include <map_create.h>


struct MbVEFVestiges;


//------------------------------------------------------------------------------
/** \brief \ru Множество контуров, принадлежащих некоторому компоненту.
           \en An array of contours belonging to the certain component. \~
  \details \ru Множество контуров, принадлежащих некоторому компоненту. \n
           \en An array of contours belonging to the certain component. \n \~
  \ingroup Mapping
*/
// ---
class MATH_CLASS MbCompContourArray {
public:
  uint                 compHash;    ///< \ru Компонент. \en A component. 
  void               * lump;        ///< \ru Указатель на тело с матрицей. \en A pointer to a solid with a matrix. 
  PArray<MbContour>  * arContours;  ///< \ru Множество контуров. \en An array of contours. 

public:

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по компоненту и телу.\n
             \en Constructor by a component and a solid.\n \~    
    \param[in] comp_ - \ru Компонент.
                       \en A component. \~
    \param[in] lump_ - \ru Указатель на тело с матрицей MbLump.
                       \en A pointer to a solid with the MbLump matrix. \~
  */
  MbCompContourArray( uint comp_, void * lump_ );

  /// \ru Деструктор. \en Destructor. 
 ~MbCompContourArray();

public:

  /** \brief \ru Добавить контур.
             \en Add a contour. \~
    \details \ru Добавить контур в массив контуров, если массив не нулевой.\n
             \en Add a contour to the array of contours, if the array isn't null.\n \~    
    \param[in] contour - \ru Контур.
                         \en A contour. \~
  */
  void                  Add( MbContour * contour );

  /** \brief \ru Выдать массив контуров.
             \en Get the array of contours. \~
    \details \ru Выдать массив контуров, хранящихся в объекте,
      и обнулить поле объекта с массивом.
             \en Get the array of contours which are stored in the object
      and reset the field with the array of the object. \~
  */
  PArray<MbContour>   * CreateContoursArray();

  /** \brief \ru Выдать массив контуров.
             \en Get the array of contours. \~
    \details \ru Добавить контуры в присланный массив.
             \en Add contours to a given array. \~
    \param[out] arCont - \ru Множество для добавления контуров.
                         \en An array for contours adding. \~
  */
  void                  GetContoursArray( PArray<MbContour> & arCont );
  
  /** \brief \ru Удалить контуры.
             \en Remove contours. \~
    \details \ru Очистить массив с контурами, если он не нулевой.
             \en Clear the array of contours if it isn't null. \~       
  */
  void                  DetachContours() {      
    if ( arContours != nullptr )
      arContours->Flush( noDelete ); 
  }

private:
  MbCompContourArray( const MbCompContourArray & ); // \ru не реализовано \en not implemented 
  void operator = ( const MbCompContourArray & ); // \ru не реализовано \en not implemented 
};


//------------------------------------------------------------------------------
/** \brief \ru Данные для построения сечений и разрезов набора оболочек.
           \en A data for constructing a set of sections and cutaways of shells. \~
  \details \ru Данные для построения сечений и разрезов набора оболочек.\n
           \en A data for constructing a set of sections and cutaways of shells.\n \~
  \ingroup Mapping
*/
// ---
class MATH_CLASS MbSectionMap {

protected:
  MbVEFVestiges &             secMap;         ///< \ru Отображение. Набор следов. \en Mapping. A set of vestiges. 
  PArray<MbCompContourArray>  secBounds;      ///< \ru Неупорядоченный набор секущих контуров. \en Disordered set of cutting contours. 
  ptrdiff_t                   secIndex;       ///< \ru Текущий индекс. \en A current index. 
  MbResultType                secMessage;     ///< \ru Результат построения. \en A result of the construction. 
  RPArray<MbFaceShell>        secShells;      ///< \ru Отображаемые оболочки (усеченные). Заполнить в конце построения. \en Mapped shells (trimmed). Fill in the end of construction. 

protected:
  ItAnnObjectStore          * annObjStore;    ///< \ru Хранилище аннотационных объектов. \en A storage of annotation objects. 
  RPArray<MbSymbol>         * symbolObjects;  ///< \ru Условные обозначения. \en Conventional notations. 
  ItSymbolObjectStore       * symbolObjStore; ///< \ru Хранилище условных обозначений. \en A set of conventional notations. 

  RPArray<MbSpacePoints>    * pointsData;     ///< \ru Пространственные точки. \en Spatial points. 
  RPArray<MbSpaceCurves>    * curvesData;     ///< \ru Пространственные кривые. \en Spatial curves. 

public:
  /// \ru Конструктор пустого объекта. \en Constructor of an empty object. 
  MbSectionMap();
  /// \ru Деструктор. \en Destructor. 
 ~MbSectionMap();

public:

  /** \brief \ru Зарезервировать место под оболочки.
             \en Reserve a place for shells. \~
    \details \ru Зарезервировать место под отображаемые оболочки.
             \en Reserve a place for mapping shells. \~
    \param[in] count - \ru Количество мест для резервирования.
                       \en A number of places for reservation. \~
  */
  void                  ReserveShell( size_t count );

  /** \brief \ru Добавить оболочку.
             \en Add a shell. \~
    \details \ru Добавить оболочку в набор оболочек.\n
      Добавляется, даже если равна nullptr.\n
      Если не нулевая - захватывается.
             \en Add a shell into the set of shells.\n
      A shell is added even if it is equal to nullptr.\n
      If a shell isn't null it is captured. \~
    \param[in] secShell - \ru Оболочка.
                          \en A shell. \~
  */
  void                  AddShell( MbFaceShell * secShell ) { ::AddRefItem( secShell ); secShells.push_back( secShell ); }

  /** \brief \ru Объект пустой.
             \en Whether the object is empty. \~
    \details \ru Объект пустой.\n
             \en Whether the object is empty.\n \~     
    \return \ru true, если в объекте нет 
      ни оболочек, ни контуров, ни следов отображения.
            \en returns true if the object doesn't contain 
      shells, contours and mapping vestiges. \~
  */
  bool                  IsEmpty() const;

  /** \brief \ru Установить индекс.
             \en Set an index. \~
    \details \ru Установить индекс.\n
             \en Set an index.\n \~    
    \param[in] i - \ru Новое значение индекса.
                   \en New index value. \~
  */
  void                  SetIndex( ptrdiff_t i ) { secIndex = i; }
  
  /** \brief \ru Дать массив контуров.
             \en Get the array of contours. \~
    \details \ru Добавить в присланный массив контуры всех наборов.
             \en Add contours of all sets into a given array. \~
    \param[out] arCont - \ru Набор контуров.
                         \en A set of contours. \~ 
  */
  void                  GetContoursArray( PArray<MbContour> & arCont ) const;
  
  /// \ru Дать текущий индекс. \en Get the current index. 
  ptrdiff_t             GetIndex() const { return secIndex; }

  /** \brief \ru Отображение.
             \en Mapping. \~
    \details \ru Дать набор следов.
             \en Get the set of vestiges. \~
    \return \ru Набор массивов следов.
            \en A set of arrays of vestiges. \~
  */
  MbVEFVestiges               & GetSectionMap()  { return secMap; } 

  /// \ru Неупорядоченный набор контуров. \en Disordered set of contours. 
  PArray<MbCompContourArray>  & GetSectionBounds() { return secBounds; } 

  /** \brief \ru Очистить содержание вида сечения.
             \en Clean a section view. \~
    \details \ru Очистить массивы следов, наборы контуров,
      отпустить оболочки и очистить массив оболочек.
             \en Clean arrays of vestiges and sets of contours,
      release shells and clean the array of shells. \~    
  */
  void                  SetEmpty  (); 

  /** \brief \ru Установить код результата.
             \en Set a result code. \~
    \details \ru Установить код результата построения.
             \en Set a result code of the construction. \~
    \param[in] type - \ru Код результата операции.
                      \en Operation result code. \~   
  */
  void                  SetMessage( MbResultType & type ) { secMessage = type; }

  /** \brief \ru Дать код результата.
             \en Get the result code. \~
    \details \ru Дать код результата построения.
             \en Get the result code of the construction. \~
    \return \ru Код результата операции.
            \en Operation result code. \~   
  */
  MbResultType          GetMessage() const { return secMessage;}
  
  /** \brief \ru Преобразовать.
             \en Transform. \~
    \details \ru Преобразовать объект по матрице.
             \en Transform the object by a matrix. \~
    \param[in] matr - \ru Матрица преобразования.
                      \en A transformation matrix. \~  
  */
  void                  Transform( const MbMatrix & matr );
  
  /** \brief \ru Отцепить массив оболочек.
             \en Detach the array of shells. \~
    \details \ru Отцепить массив оболочек и переложить их в присланный массив.
             \en Detach the array of shells and put them into a given array. \~
    \param[out] shells - \ru Множество для оболочек.
                         \en An array for shells. \~  
  */
  template <class Shells>
  void                  DetachShellArray( Shells & shells )
  {
    shells.reserve( shells.size() + secShells.size() );
    for ( size_t i = 0, count = secShells.size(); i < count; ++i ) {
      MbFaceShell * shell = secShells[i];
      ::DecRefItem( shell );
      shells.push_back( shell );
    }
    secShells.clear();
  }
  
  /// \ru Дать массив оболочек. \en Get the array of shells. 
  const RPArray<MbFaceShell> & GetShellArray() const { return secShells; }

  /** \brief \ru Установить хранилище.
             \en Set a storage. \~
    \details \ru Установить хранилище аннотационных объектов.\n
      Отцепить старое хранилище, захватить новое.
             \en Set a storage of annotation objects.\n
      Detach the old storage and catch a new one. \~  
    \param[out] objStore - \ru Новое хранилище аннотационных объектов.
                           \en A new storage of annotation objects. \~  
  */
  void                    SetAnnObjectStore( ItAnnObjectStore * objStore ); 

  /// \ru Получить хранилище аннотационных объектов. \en Get the storage of annotation objects. 
  ItAnnObjectStore      * GetAnnObjectStore() const { return annObjStore; } 

  /** \brief \ru Получить условные обозначения.
             \en Get conventional notations. \~
    \details \ru Получить массив условных обозначений.\n
             \en Get the array of conventional notations.\n \~
    \return \ru Условные обозначения.
            \en Conventional notations. \~    
  */
  RPArray<MbSymbol>     * GetSymbolObjects    () const { return symbolObjects;  }; 
  
  /** \brief \ru Добавить условные обозначения.
             \en Add conventional notations. \~
    \details \ru Добавить информацию об условных обозначениях.\n
      Добавить в массив условных обозначений присланные условные обозначения.
             \en Add the information about conventional notations.\n
      Add the given conventional notations into the array of conventional notations. \~
    \param[in] arInit - \ru Условные обозначения.
                        \en Conventional notations. \~     
  */
  void                    SetSymbolObjects    ( RPArray<MbSymbol> & arInit ); 
  
  /// \ru Получить хранилище условных обозначений. \en Get the storage of conventional notations. 
  ItSymbolObjectStore   * GetSymbolObjectStore() const { return symbolObjStore; } 

  /** \brief \ru Установить хранилище.
             \en Set a storage. \~
    \details \ru Установить хранилище условных обозначений.\n
      Отцепить старое хранилище, захватить новое.
             \en Set a storage of conventional notations.\n
      Detach the old storage and catch a new one. \~  
    \param[out] objStore - \ru Новое хранилище условных обозначений.
                           \en A new storage of conventional notations. \~  
  */
  void                    SetSymbolObjectStore( ItSymbolObjectStore * objStore ); 

  /** \brief \ru Добавить пространственные точки.
             \en Add spatial points. \~
    \details \ru Добавить пространственные точки.\n
      Добавить в массив точек новые точки.
             \en Add spatial points.\n
      Add new points into the array of points. \~
    \param[in] points - \ru Точки.
                        \en Points. \~     
  */
  void                    SetSpacePoints( RPArray<MbSpacePoints> & points );

  /** \brief \ru Добавить пространственные кривые.
             \en Add spatial curves. \~
    \details \ru Добавить пространственные кривые.\n
      Добавить в массив кривых новые кривые.
             \en Add spatial curves.\n
      Add new curves into the array of curves. \~
    \param[in] curves - \ru Кривые.
                        \en Curves. \~     
  */
  void                    SetSpaceCurves( RPArray<MbSpaceCurves> & curves );
  
  /** \brief \ru Есть ли в объекте точки.
             \en Whether any point is in the object. \~
    \details \ru Есть ли в объекте пространственные точки.\n
             \en Whether any spatial point is in the object.\n \~       
    \return \ru true, если массив точек не нулевой и не пустой.
            \en returns true if the array of points isn't null and isn't empty. \~
  */
  bool                    IsSpacePoints() const { return (pointsData != nullptr && pointsData->size() > 0); }
  
  /** \brief \ru Есть ли в объекте кривые.
             \en Whether any curve is in an object. \~
    \details \ru Есть ли в объекте пространственные кривые.\n
             \en Whether any spatial curve is in an object.\n \~       
    \return \ru true, если массив кривых не нулевой и не пустой.
            \en returns true if the array of curves isn't null and isn't empty. \~
  */
  bool                    IsSpaceCurves() const { return (curvesData != nullptr && curvesData->size() > 0); }

  const RPArray<MbSpacePoints> * GetSpacePoints() const { return pointsData; } ///< \ru Получить указатель на пространственные точки. \en Get spatial points. 
  const RPArray<MbSpaceCurves> * GetSpaceCurves() const { return curvesData; } ///< \ru Получить указатель на пространственные кривые. \en Get spatial curves. 

OBVIOUS_PRIVATE_COPY( MbSectionMap )
};

//------------------------------------------------------------------------------
// \ru установить хранилище \en set a storage  
// ---
inline void MbSectionMap::SetAnnObjectStore( ItAnnObjectStore * _annObjStore )
{
  ::AddRefItem( _annObjStore );
  ::ReleaseItem( annObjStore );
  annObjStore = _annObjStore;
}

//-----------------------------------------------------------------------------
//
//---
inline void MbSectionMap::SetSymbolObjectStore( ItSymbolObjectStore * _symbolObjStore )
{
  ::AddRefItem( _symbolObjStore );
  ::ReleaseItem( symbolObjStore );
  symbolObjStore = _symbolObjStore;
}


#endif // __MAP_SECTION_H
