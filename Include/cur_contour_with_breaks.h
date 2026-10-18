////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Контур с разрывами.
         \en Contour with breaks. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CUR_CONTOUR_WITH_BREAKS_H
#define __CUR_CONTOUR_WITH_BREAKS_H


#include <templ_css_array.h>
#include <cur_contour.h>
#include <mb_cart_point3d.h>


class    MbBreaksPart;
class    MbBreak;


//------------------------------------------------------------------------------
/** \brief \ru Контур c разрывами.
           \en Contour with breaks. \~
  \details \ru Контур c разрывами.\n
      Для использования в мультилинии MbMultiline.\n
      Содержит разрывы MbBreak, видимые части - контуры MbContour.\n
      При использовании в мультилинии каждуй раз при перестроении контура в нем обновляется
    список номеров по количеству сегментов контура.
    Каждый номер показывает номер сегмента базовой кривой мультилинии,
    которому соответствует этот сегмент контура.\n
    Если сегмент контура является эквидистантой сегмента базовой кривой, 
    то ему соответствует номер этого сегмента.\n
    Если контур является сегментом обхода вершины мультилинии,
    то ему соответствует номер, соответствующий предыдущему сегменту контура.\n
      Разрывы в контуре не могут накладываться друг на друга.
    Если в результате перестроения один разрыв наложился на другой, то они объединяются.
           \en Contour with breaks.\n
      For using in the multiline MbMultiline.\n
      Contains breaks MbBreak, visible parts - contours MbContour.\n
      When using in a multiline every time when contour is rebuilding in it are updated
    list of numbers by the number of contour segments
    Each number indicates the segment number of base curve of multiline.
    which corresponds to this segment of contour.\n
    If segment of contour is equidistant of base curve segment, 
    then it corresponds to the number of this segment.\n
    If contour is segment of multiline vertices traverse.
    then it corresponds to the number corresponding the previous segment of the contour.\n
      Breaks in the contour can not be overlapped each other.
    If in the result of rebuilding a break overlapped on the other break then they are combined. \~
  \ingroup Curves_2D
*/ // ---
class MATH_CLASS MbContourWithBreaks : public MbContour 
{
private: 
  RPArray<MbBreak>   breaks;          // \ru Разрывы. \en Breaks. 
  RPArray<MbContour> visibleContours; // \ru Видимые контуры \en Visible contours.
  SArray<size_t>     baseSegNumbers;  // \ru Номера сегментов базового контура \en Numbers of segments of base contour
                                      // \ru для задания неподвижных точек разрыва \en to define the fixed points of break 
                                      // \ru заполняется при использовании контура в мультилинии. \en filled when using the contour in the multiline. 
public:

  /** \brief \ru Создание пустого контура.
             \en Creation of empty contour. \~
    \details \ru Создание пустого контура без сегментов.\n
             \en Creation of empty contour without segments. \n \~           
  */
  MbContourWithBreaks();   
  
  MbContourWithBreaks( const MbContour & cnt ); ///< \ru Копирующий конструктор. \en Copy-constructor. 

private:
  MbContourWithBreaks( const MbContourWithBreaks & ); // \ru не реализовано \en not implemented 
protected :
  MbContourWithBreaks( const MbContourWithBreaks &, MbRegDuplicate * );
public:
  virtual ~MbContourWithBreaks();

public :
  VISITING_CLASS( MbContourWithBreaks ); 
              
  /**\ru \name Общие функции геометрического объекта.
     \en \name Common functions of a geometric object.
      \{ */
  MbePlaneType  IsA() const override; // \ru Тип элемента. \en A type of element. 
  void          Transform( const MbMatrix & matr, MbRegTransform * ireg = nullptr, const MbSurface * newSurface = nullptr ) override; // \ru Преобразовать элемент согласно матрице. \en Transform element according to the matrix.
  void          Move( const MbVector & to, MbRegTransform * = nullptr, const MbSurface * newSurface = nullptr ) override; // \ru Сдвиг. \en Move.
  void          Rotate( const MbCartPoint & pnt, const MbDirection & angle, MbRegTransform * = nullptr, const MbSurface * newSurface = nullptr ) override; // \ru Поворот. \en Rotation.
  MbPlaneItem & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Сделать копию элемента. \en Create a copy of the element.
  /** \} */   
  /**\ru \name Функции доступа к данным: разрывы.
     \en \name Functions for access to data: breaks.
      \{ */   
          size_t      GetBreaksCount            ()              const { return breaks.Count(); } ///< \ru Число разрывов. \en The number of breaks. 
          
          /** \brief \ru Разрыв по номеру.
                     \en A break by the number. \~
            \details \ru Разрыв по номеру.\n  
              Номер не проверяется на корректность.
                     \en A break by the number. \n  
              A number isn't checked for correctness. \~ 
            \param[in] i - \ru Номер разрыва, должен быть меньше количества разрывов.
                           \en The number of break must be less than the number of breaks. \~
            \return \ru Указатель на разрыв.
                    \en Pointer to break. \~
          */
          MbBreak *   GetBreak                  ( size_t i )    const { return breaks[i];      } 

          /** \brief \ru Разрыв по номеру в параметрах контура.
                     \en A break by the number in contour parameters. \~
            \details \ru Разрыв по номеру в параметрах контура.\n  
              Номер проверяется на корректность. 
              В случае, если номер больше числа сегментов результат - вывернутая область.
                     \en A break by the number in contour parameters. \n  
              A number Is checked for correctness. 
              If number is more than numbers of segments then result is everted region. \~ 
            \param[in] i - \ru Номер разрыва, должен быть меньше количества разрывов.
                           \en The break number must be less than the number of all breaks. \~
            \return \ru Интервал разрыва в параметрах контура.
                    \en Break interval in contour parameters. \~
          */
          MbRect1D    GetBreaksRange            ( size_t i )    const;                           
  
  /** \} */   
  /**\ru \name Функции доступа к данным: видимые участки.
     \en \name Functions for access to data: visible regions.
      \{ */  

          /** \brief \ru Количество видимых частей.
                     \en The number of visible parts. \~
            \details \ru Количество видимых частей.\n      
              Если в контуре нет разрывов, то возвращает 0.
                     \en The number of visible parts. \n      
              If contour does not contain breaks, then returns 0. \~
            \return \ru Число видимых частей.
                    \en The number of visible parts. \~
          */
          size_t      GetVisibleCount           ()              const { return visibleContours.Count(); } 
          
  /** \brief \ru Видимая часть по номеру.
             \en A visible part by the number. \~
    \details \ru Видимая часть по номеру.\n     
      Номер не проверяется на корректность.
             \en A visible part by the number. \n     
      A number isn't checked for correctness. \~
    \param[in] i - \ru Номер видимой части, должен быть меньше количества видимых частей.
                   \en The number of visible part must be less than the number of visible parts. \~
    \return \ru Указатель не контур - видимую часть.
            \en A pointer to visible part of the contour. \~
  */
  const   MbContour * GetVisibleContour         ( size_t i )    const { return visibleContours[i];      } 
  
  /** \} */   
  /**\ru \name Функции доступа к данным: невидимые участки.
     \en \name Functions for access to data: Invisible regions.
      \{ */ 
          
          /** \brief \ru Невидимая часть по номеру разрыва.
                     \en Invisible part by the number of break. \~
            \details \ru Невидимая часть по номеру разрыва.\n
              Номер проверяется на корректность. 
              В случае, если номер не меньше числа разрывов, функция вернет nullptr.\n
              После использования полученный контур нужно удалить.
                     \en Invisible part by the number of break. \n
              A number Is checked for correctness. 
              If the number is not less than the number of breaks, the function returns nullptr. \n
              The resulting contour is to be deleted after use. \~
            \param[in] i - \ru Номер разрыва, должен быть меньше количества видимых частей.
                           \en The number of break must be less than the number of visible parts. \~
            \return \ru Указатель не контур - видимую часть.
                    \en A pointer to visible part of the contour. \~
          */
          MbContour * GetInvisibleContour       ( size_t i )    const;                           
  
  /** \} */   
  /**\ru \name Работа с разрывами: добавление.
     \en \name Working with breaks: addition.
      \{ */    

          /** \brief \ru Добавить разрыв между точками.
                     \en Add a break between points. \~
            \details \ru Добавить разрыв между точками.\n
                     \en Add a break between points. \n \~               
            \param[in] point1      - \ru Первая граница разрыва.
                                     \en The first boundary of break. \~
            \param[in] point2      - \ru Вторая граница разрыва.
                                     \en The second boundary of break. \~
            \param[in] point3      - \ru Точка, которая показывает удаляемую часть замкнутого контура,\n
                                     в случае разомкнутого контура она игнорируется.
                                     \en A point which indicates a removable part of the closed contour, \n
                                     in the case of open contour it is ignored. \~ 
            \param[in] invertBreak - \ru Признак добавления разрыва на противоположную часть контура.
                                     \en addition attribute of break on the opposite part of contour. \~
            \return \ru true, если разрыв был добавлен.
                    \en true if break has been added. \~
          */
          bool        AddBreak                  ( const MbCartPoint & point1, const MbCartPoint & point2,                     
                                                  const MbCartPoint & point3, bool invertBreak = false  );  

          /** \brief \ru Добавить разрыв между параметрами контура.
                     \en Add a break between contour parameters. \~
            \details \ru Добавить разрыв между параметрами контура.\n
                     \en Add a break between contour parameters. \n \~               
            \param[in] t1          - \ru Первая граница разрыва.
                                     \en The first boundary of break. \~
            \param[in] t2          - \ru Вторая граница разрыва.
                                     \en The second boundary of break. \~
            \param[in] t3          - \ru Параметр, который показывает удаляемую часть замкнутого контура,\n
                                     в случае разомкнутого контура он игнорируется.
                                     \en A parameter which indicates a removable part of the closed contour, \n
                                     in the case if contour is open it is ignored. \~ 
            \param[in] invertBreak - \ru Признак добавления разрыва на противоположную часть контура.
                                     \en addition attribute of break on the opposite part of contour. \~
            \return \ru true, если разрыв был добавлен.
                    \en true if break has been added. \~
          */
          bool        AddBreak                  ( double t1, double t2, double t3, bool invertBreak = false );         

          /** \brief \ru Добавить разрыв по интервалу параметров контура.
                     \en Add a break by interval of contour parameters. \~
            \details \ru Добавить разрыв по интервалу параметров контура.\n
                     \en Add a break by interval of contour parameters. \n \~               
            \param[in] range - \ru Интервал параметров контура,\n
                               если range.zmin больше range.zmax, то результат работы функции
                               будет корректным только в случае замкнутого контура - 
                               добавится разрыв, проходящий через начало контура.
                               \en An interval of contour parameters. \n
                               if range.zmin is greater than range.zmax then the function result
                               is correct only in the case of closed contour -  
                               added a break passing through the origin of the contour. \~          
            \return \ru true, если разрыв был добавлен.
                    \en true if break has been added. \~
          */
          bool        AddBreak                  ( const MbRect1D & range );       
  
  /** \} */   
  /**\ru \name Работа с разрывами: удаление.
     \en \name Working with breaks: removing.
      \{ */   

          /** \brief \ru Удалить разрывы.
                     \en Remove breaks. \~
            \details \ru Удалить все разрывы.\n
                     \en Remove all breaks. \n \~          
            \return \ru true, если хотя бы один разрыв был удален.
                    \en true, if at least one break has been removed. \~ 
          */
          bool        DeleteBreaks              (); ///< \ru Удалить разрывы. \en Remove breaks. 

          /** \brief \ru Удалить разрыв по номеру разрыва.
                     \en Remove a break by the number. \~
            \details \ru Удалить разрыв по номеру разрыва.\n
                     \en Remove a break by the number. \n \~               
            \param[in] breakIndex - \ru Номер разрыва,\n
                                    проверяется на корректность.\n
                                    \en The number of break. \n
                                    checked for correctness. \n \~
            \param[in] rebuild    - \ru Нужно ли перестроить контур после удаления разрыва.\n
                                    Если контур не перестраивать, видимые части контура не будут соответствовать разрывам.\n
                                    Перестроить контур можно отдельно вызовом RebuildBreaks.
                                    \en Is the contour to be rebuilt after removing break. \n
                                    If don't rebuild contour then the visible parts of contour do not match breaks. \n
                                    Rebuild contour you can be separately by call RebuildBreaks. \~ 
            \return \ru true, если разрыв был удален.
                    \en true if break has been removed. \~ 
          */
          bool        DeleteBreakAtNumber       ( size_t breakIndex, bool rebuild = false); 

          /** \brief \ru Удалить разрыв по параметру на контуре.
                     \en Remove a break by parameter on the contour. \~
            \details \ru Удалить разрыв по параметру на контуре.\n
                     \en Remove a break by parameter on the contour. \n \~          
            \return \ru true, если разрыв был удален
                    \en true if break has been removed \~ 
          */
          bool        DeleteBreakAtParam        ( double t );     

          /** \brief \ru Удалить разрывы на сегментах с соответствующим базовым номером.
                     \en Remove breaks on the segments with the corresponding base number. \~
            \details \ru Удалить разрывы на сегментах с соответствующим базовым номером.\n    
              Разрывы удаляться, если в контуре заполнены базовые номера сегментов.
              Номера заполняются при перестроении мультилинии, содержащей контур.\n  
              Если сегменту контура соответствует одна из частей разрыва, то весь разрыв будет удален.
                     \en Remove breaks on the segments with the corresponding base number. \n    
              Break are removed if base numbers of segments are filled in contour.
              The number are filled when multiline is rebuilt which contrains contour. \n  
              If segment of contour corresponds to one of break parts then entire break is removed. \~
            \param[in] baseNumber       - \ru Номер сегмента базовой кривой.
                                          \en A segment number of base curve. \~
            \param[in] delTracingBreaks - \ru Нужно ли удалять разрывы с сегментоа, соответствующих обходу вершины мультилинии.
                                          \en Is it necessary for breaks to be removed from segments which correspond to multiline vertices traverse. \~
            \param[in] delEquidBreaks   - \ru Нужно ли удалять разрывы с сегментов, соответствующих эквидистантам.
                                          \en breaks to be removed from segments which correspond to equidistants. \~
            \param[in] delInLineSeg     - \ru Удалять ли разрывы с прямолинейных сегментов.
                                          \en Is it necessary to remode breaks from straight segments. \~               
          */
          void        DeleteBreaksAtBaseNumber  ( size_t baseNumber, bool delTracingBreaks,
                                                  bool delEquidBreaks, bool delInLineSeg = true );       
  /** \} */   
  /**\ru \name Удаление разрывов и видимых частей малой метрической длины.
     \en \name Removing of breaks and visible parts of the small metric length.
      \{ */ 

          /** \brief \ru Удалить разрывы малой метрической длины.
                     \en Remove breaks of the small metric length. \~
            \details \ru Удалить разрывы малой метрической длины.\n
              В случае успеха видимые контуры перестраиваются соответственно разрывам.
                     \en Remove breaks of the small metric length. \n
              In case of success the visible contours are rebuilt by breaks. \~
            \param[in] length - \ru Минимальная длина невидимой части.
                                \en Minimal length of invisible part. \~
            \return \ru true, если хотя бы один разрыв был удален.
                    \en true, if at least one break has been removed. \~ 
          */
          bool        DeleteSmallBreaks         ( double length );
          
          /** \brief \ru Удалить видимые части малой метрической длины.
                     \en Remove visible parts of the small metric length. \~
            \details \ru Удалить видимые части малой метрической длины.\n  
              Соответствует объединению близких разрывов в один.
              В случае успеха видимые контуры перестраиваются соответственно разрывам.
                     \en Remove visible parts of the small metric length. \n  
              Corresponds to union of close discontinuities into one.
              In case of success the visible contours are rebuilt by breaks. \~
            \param[in] length - \ru Минимальная длина видимой части.
                                \en Minimal length of visible part. \~
            \return \ru true, если разрывы были изменены.
                    \en true if breaks have been changed. \~
          */
          bool        DeleteSmallVisContours    ( double length );

  /** \} */   
  /**\ru \name Работа с разрывами
     \en \name Working with breaks
      \{ */     

          /** \brief \ru Номер разрыва, край которого попал в окрестность точки.
                     \en The number of break the edge of which is into point neighbourhood. \~
            \details \ru Номер разрыва, край которого попал в окрестность точки.\n
                     \en The number of break the edge of which is into point neighbourhood. \n \~                     
            \param[in] p      - \ru Точка.
                                \en Point. \~
            \param[in] rad    - \ru Радиус окрестности точки для поиска разрыва.
                                \en Radius of point neighborhood to search the break. \~
            \param[out] index - \ru Номер разрыва.
                                \en The number of break. \~
            \return \ru true, если разрыв найден.
                    \en true if the break is found. \~
          */
          bool        GetBreakAtPoint           ( const MbCartPoint & p, double rad, 
                                                  size_t & index                         ) const; 

          /** \brief \ru Номера разрывов, которые хотя бы одним краем попадают в область.
                     \en Numbers of breaks which at least one edge fall into the region. \~
            \details \ru Номера разрывов, которые хотя бы одним краем попадают в область.\n
                     \en Numbers of breaks which at least one edge fall into the region. \n \~                     
            \param[in] rect           - \ru Область поиска.
                                        \en Region of search. \~             
            \param[out] breaksNumbers - \ru Номера разрывов.
                                        \en Numbers of breaks. \~
            \return \ru true, если хотя бы один разрыв найден.
                    \en true, if at least one break has been found. \~
          */
          bool        GetBreaksInRect           ( const MbRect & rect, 
                                                  SArray<size_t> & breaksNumbers          ) const; 

          /** \brief \ru Номера разрывов, которые хотя бы одним краем попадают в область.
                     \en Numbers of breaks which at least one edge fall into the region. \~
            \details \ru Номера разрывов, которые хотя бы одним краем попадают в область, заданную контуром.\n
                     \en Numbers of breaks which at least one edge fall into the region given contour. \n \~                     
            \param[in] contour        - \ru Контур должен быть замкнутым и иметь правильное направление.
                                        \en Contour must be closed and have the right direction. \~              
            \param[out] breaksNumbers - \ru Номера разрывов.
                                        \en Numbers of breaks. \~
            \return \ru true, если хотя бы один разрыв найден.
                    \en true, if at least one break has been found. \~
          */
          bool        GetBreaksInRect           ( const MbContour & contour, 
                                                  SArray<size_t> & breaksNumbers         ) const; 
          
          /** \brief \ru Определить попадает ли точка в любой из разрывов.
                     \en Determine whether a point it inside a break. \~
            \details \ru Определить попадает ли точка в любой из разрывов.\n
                     \en Determine whether a point it inside a break. \n \~                     
            \param[in] p - \ru Точка для проверки.
                           \en A point for the check. \~
            \return \ru true, если точка попадает в любой из разрывов.
                    \en true, if point falls into any break. \~
          */
          bool        HitToBreaks               ( const MbCartPoint & p                  ) const; 

          /** \brief \ru Находится ли интервал параметров на разрыве.
                     \en Whether the interval of parameters is on break. \~
            \details \ru Находится ли интервал параметров на разрыве.\n
                     \en Whether the interval of parameters is on break. \n \~                     
            \param[in] rect - \ru Интервал для проверки.
                              \en Interval to check. \~
            \return \ru true, если интервал полностью находится на разрыве или совпадает с ним.
                    \en true if interval entirely is on the break or coincides with it. \~
          */
          bool        IsRectInBreak             ( const MbRect1D & rect                  );       
          
          /** \brief \ru Обновить невидимые и видимые контуры.
                     \en Update visible and invisible contours. \~
            \details \ru Обновить невидимые и видимые контуры соответственно базовой кривой мультилинии.\n
              После перестроения разрывы должны соответствовать сегментам базовой кривой мультилинии.
                     \en Update visible and invisible contours by base curve of multiline respectively. \n
              After rebuilding the breaks must correspond to segments of base multiline curve. \~
            \param[in] oldBaseNumbers - \ru Старые номера базовых сегментов,\n
                                        должны быть запомнены до изменения мультилинии.
                                        \en Old numbers of base segments, \n
                                        must be saved before multiline is changed. \~
          */
          void        RebuildBreaks             ( SArray<size_t> & oldBaseNumbers        );            
          
          /** \brief \ru Обновить невидимые и видимые контуры.
                     \en Update visible and invisible contours. \~
            \details \ru Обновить невидимые и видимые контуры соответственно разрывам.
                     \en Update visible and invisible contours according to breaks. \~             
          */
          void        RebuildBreaks             (                                        );            
                       
          /** \brief \ru Преобразование в соответствии с матрицей.
                     \en Transform according to matrix. \~
            \details \ru Преобразование в соответствии с матрицей.\n
              Используется для преобразования мультилинии.
              Преобразует длину и расстояние от фиксированной точки прямолинейного разрыва,
              а так же фиксированную точку.
                     \en Transform according to matrix.\n
              Used to transform multiline.
              Transforms length and distance from fixed point of straight break,
              and fixed point. \~
            \param[in] matr - \ru Матрица преобразования.
                              \en A transformation matrix. \~
          */
          void        TransformMultlinesBreaks  ( const MbMatrix & matr                  );  
          
          /** \brief \ru Параметр привязки части разрыва.
                     \en A binding parameter of a break part. \~
            \details \ru Посчитать параметр привязки части разрыва в зависимости от типа сегмента контура.\n
                     \en Calculate binding parameter of a break part according to the type of contour segment. \n \~           
            \param[in]  brPart     - \ru Часть разрыва этого контура.
                                     \en A break part of this contour. \~
            \param[out] segNumber  - \ru Номер сегмента контура.
                                     \en A number of the contour segment. \~
            \return \ru Параметр центра разрыва на сегменте контура.
                    \en A parameter of the break center on the contour segment. \~
          */
          double      GetLocalBreaksParam       ( const MbBreaksPart & brPart, 
                                                  size_t & segNumber                     ) const;              
          
  /** \} */   
  /**\ru \name Работа с разрывами: изменение разрыва.
     \en \name Working with breaks: changing a break.
      \{ */      

          /** \brief \ru Фиксировать точку.
                     \en Fix the point. \~
            \details \ru Поставить фиксированную точку части разрыва.\n
                     \en Set the fixed point of a break part. \n \~           
            \param[in]  newPoint - \ru Новая фиксированная точка.
                                   \en A new fixed point. \~
            \param[out] part     - \ru Часть разрыва контура для изменения.
                                   \en A part of contour break to change. \~
          */
          void        SetBreakFixedPoint        ( const MbCartPoint & newPoint,
                                                  MbBreaksPart & part                    );     

          /** \brief \ru Фиксировать переменную.
                     \en Fix the variable. \~
            \details \ru Поставить фиксированную переменную части разрыва.\n
                     \en Set the fixed variable of a break part. \n \~           
            \param[in]  newFixedVar - \ru Новая фиксированная переменная.
                                      \en New fixed variable. \~
            \param[out] part        - \ru Часть разрыва контура для изменения.
                                      \en A part of contour break to change. \~
          */
          void        SetBreakFixedVar          ( double newFixedVar, MbBreaksPart & part); 

  /** \} */   
  /**\ru \name Работа с разрывами: отслеживание разрыва
     \en \name Working with breaks: tracking a break
      \{ */ 
          /// \ru Количество номеров сегментов базового контура \en Count of segments numbers of the base contour  
          // \ru (должно соответствовать количеству сегментов контура) \en (must be equal to count of contour segments) 
          size_t      GetBaseNumbersCount       () const { return baseSegNumbers.Count(); }       
          
          /// \ru Номера сегментов базового контура. \en Numbers of segments of base contour. 
          void        GetBaseNumbers            ( SArray<size_t> & baseNumbers           ) const; 
          
          /** \brief \ru Номер сегмента базового контура.
                     \en Segment number of the base contour. \~
            \details \ru Номер сегмента базового контура.\n        
              Номер сегмента будет найден, если массив номеров не пуст и
              корректно насчитан, то есть число номеров совпадает с числом сегментов контура
                     \en Segment number of the base contour. \n        
              Segment number will be found if the array of numbers is not empty and
              it was numbered correctly, ie count of numbers is equal to count of contour segments \~
            \param[in] i - \ru Индекс номера.
                           \en Index of number. \~
            \return \ru Номер сегмента из массива номеров.
                    \en Number of segment from the array of numbers. \~
          */
          size_t      GetBaseNumber             ( size_t i                               ) const; 
          
          /** \brief \ru Добавить номер базового сегмента.
                     \en Add number of the base segment. \~
            \details \ru Добавить номер сегмента базового контура в конец массива.\n
                     \en Add segment number of base contour to the end of array. \n \~                      
            \param[in] number - \ru Номер для добавления.
                                \en Number to add. \~
          */
          void        AddBaseSegNumber          ( size_t number                          ); 
          
          /** \brief \ru Добавить номер базового сегмента.
                     \en Add number of the base segment. \~
            \details \ru Добавить номер сегмента базового контура в начало массива.\n
                     \en Add segment number of base contour to the beginning of array. \n \~                      
            \param[in] number - \ru Номер для добавления.
                                \en Number to add. \~
          */
          void        AddBaseSegNumberAtBegin   ( size_t number                          );      
          
          /** \brief \ru Вставить номер после lastInd.
                     \en Insert number after the lastInd. \~
            \details \ru Вставить после элемента номер lastInd номер, соответствующий lastInd.\n
                     \en Insert number corresponding lastInd after element lastInd. \n \~                      
            \param[in] lastInd - \ru Индекс номера.
                                 \en Index of number. \~
          */
          void        InsertLastSegNumber       ( size_t lastInd                         ); 
          
          /** \brief \ru Удалить элемент по индексу.
                     \en Delete element by an index. \~
            \details \ru Удалить номер базового сегмента по индексу.\n
                     \en Remove number of the base segment by an index. \n \~                      
            \param[in] ind - \ru Индекс номера.
                             \en Index of number. \~
          */
          void        DeleteBaseSegNumber       ( size_t ind                             ); 
          
          /// \ru Очистить массив с номерами базовых сегментов. \en Clear the array with numbers of base segments. 
          void        ClearBaseSegNumbers       () { baseSegNumbers.HardFlush();         }        
          
          /** \brief \ru Изменить номера сегментов в разрывах.
                     \en Change numbers of segments in the breaks. \~
            \details \ru Изменить номера сегментов в разрывах.\n   
              Заданному номеру базового сегмента соответствует сегмент - эквидистанта на контуре.
              Для всех разрывов:
              если хотя бы часть разрыва находится на этом сегменте, 
              у всех его частей номер сегмента будет изменен.
                     \en Change numbers of segments in the breaks. \n   
              A given number of the base segments corresponds to the segment - equidistant on the contour.
              For all breaks:
              if at least one part of break is in this segment, 
              the segment number of all of its parts  will be changed. \~
            \param[in] begBaseNumber - \ru Номер базового сегмента.
                                       \en An index of the base segment. \~
            \param[in] deltaN        - \ru Величина изменение номера сегмента частей разрывов.
                                       \en The change value of segment number of breaks parts. \~
          */
          void        ChangeBreaksSegNumbers    ( size_t begBaseNumber, ptrdiff_t deltaN );  
          
          /** \brief \ru Изменить разрывы соотвестсвенно замкнутости.
                     \en Change breaks of closedness respectively. \~
            \details \ru Изменить разрывы соотвестсвенно замкнутости.\n   
              При изменении признака замкнутости контура нужно изменить разрывы
              соответственно новому значению замкнутости.\n
              Если контур стал разомкнутым - разрыв, находящийся на первом и последнем
              сегменте одновременно, делится на 2 части.\n
              Если контур стал замкнутым - разрывы, первый из который примыкает к левому краю
              первого сегмента контура, а второй из которых примыкает к правому краю последнего
              сегмента контура, объединяется в один.
                     \en Change breaks of closedness respectively. \n   
              When changing attribute of contour closedness breaks need to change
              respectively to the new value of closedness. \n
              If contour has become open - the break located on the first and last 
              segment is divided into 2 parts at the same time. \n
              If contour has become closed - breaks, the first of which is adjacent to the left boundary
              of the first contour segment and the second of which is adjacent to the right boundary of the last
              contour segment are united into one. \~
            \param[in] newClosed - \ru Новый признак замкнутости контура.
                                   \en The new closedness attribute of contour. \~
          */            
          void        ChangeBreaksAtClosed      ( bool newClosed                         ); 

          /** \brief \ru Изменить номера сегментов частей разрывов.
                     \en Change segments numbers of breaks parts. \~
            \details \ru Изменить номера сегментов частей разрывов.\n
                     \en Change segments numbers of breaks parts. \n \~             
            \param[in] deltaN - \ru Величина изменения.
                                \en A change value. \~
          */  
          void        MoveBreaksSegNumbers      ( ptrdiff_t deltaN                       );
               
          /** \brief \ru Находится ли часть разрыва этого контура на сегменте обхода вершин.
                     \en Is the break part of this contour located on the segment of vertices traverse. \~
            \details \ru Находится ли часть разрыва этого контура на сегменте обхода вершин.\n
                     \en Is the break part of this contour located on the segment of vertices traverse. \n \~             
            \param[in]  part       - \ru Часть разрыва этого контура.
                                     \en A break part of this contour. \~
            \param[out] vertNumber - \ru В случае успеха вернет номер вершины.
                                     \en In the case of success returns the vertex number. \~
            \return \ru true, если часть разрыва находится на сегменте обхода вершины.
                    \en true, if the break part is located on the segment of vertices traverse. \~
          */ 
          bool        IsTrasingBreaksPart       ( const MbBreaksPart & part, 
                                                  size_t & vertNumber                    ) const;
          
          /** \brief \ru Поменяться разрывами.
                     \en Swap breaks. \~
            \details \ru Поменяться разрывами.\n
                     \en Swap breaks. \n \~             
            \param[in] other - \ru Контур с разрывами для обмена.
                               \en A contour with breaks to swap. \~
          */
          void        SwapBreaksAndBaseNumbers  ( MbContourWithBreaks & other            );
          
          /** \brief \ru Добавить разрывы контура.
                     \en Add contour breaks. \~
            \details \ru Добавить разрывы контура.\n
                     \en Add contour breaks. \n \~             
            \param[in] other - \ru Контур с разрывами для добавления разрывов.
                               \en A contour with breaks for adding breaks. \~
          */
          void        AddContoursBreaks         ( const MbContourWithBreaks & other      ); 
                      
          /** \brief \ru Заменить номера базовых сегментов.
                     \en Replace the numbers of base segments. \~
            \details \ru Заменить номера базовых сегментов.\n
                     \en Replace the numbers of base segments. \n \~             
            \param[in] other - \ru Контур с новыми номерами.
                               \en A contour with new numbers. \~
          */
          void        ChangeBaseNumbers         ( const MbContourWithBreaks & other      );    
  /** \} */ 

private:     
          void        DeleteBreaks              ( size_t segmentIndex,
                                                  bool delInLineSeg = true               );       // \ru удалить разрывы на сегменте с номером index \en remove breaks from segment with number "index" 
          void        DeleteBreaksPartAtSegNum  ( size_t segNumber, size_t oldSegCount   );       // \ru часть разрыва по номеру сегмента \en part of break by the segment number 
          bool        DeleteBreaksPartOrBreak   ( size_t breakIndex, size_t partIndex,
                                                  size_t oldSegCount                     );       // \ru часть разрыва или разрыв \en part of break or break 
private:   
          void        CalculateVisibleContours  (                                        );       // \ru посчитать видимые части \en calculate visible parts           
          void        CalculateInvisibleContours( RPArray<MbContour> & invisibleContours );       // \ru посчитать невидимые части \en calculate invisible parts 
          void        CalculateContours         ( const CSSArray<MbRect1D> & ranges,              // \ru посчитать контуры по интервалам \en calculate contours by intervals 
                                                  RPArray<MbContour> & contours,
                                                  bool visible                           ) const; // \ru для видимых частей вызывать AddRef() \en call AddRef() for visible parts            
          void        CalculateInVisibleRanges  ( CSSArray<MbRect1D> & breaksRanges      );       // \ru посчитать невидимые интервалы \en calculate invisible intervals 
          void        CalculateVisibleRanges    ( CSSArray<MbRect1D> & breaksRanges,
                                                  CSSArray<MbRect1D> & visibleRanges     ) const; // \ru посчитать видимые интервалы \en Calculate visible intervals 
          void        CalculateRanges           ( const CSSArray<MbRect1D> & startingInt,         // \ru посчитать противоположные интервалы \en Calculate opposite intervals 
                                                        CSSArray<MbRect1D> & resultInt   ) const; // \ru второй массив по первому \en the second array by the first                            
          void        CalculateBreaksPart       ( const MbRect1D & segParams,
                                                  size_t segNumber, MbBreak & brRange    ) const; // \ru посчитать часть разрыва \en Calculate a part of the break 
          void        CalculateBreak            ( const MbRect1D & range, 
                                                  MbBreak & brRange                      ) const; // \ru посчитать разрыв по интервалу \en Calculate the break by interval 
          MbRect1D    GetLocalBreaksRange       ( const MbBreaksPart & part, 
                                                  double brParam, size_t segNumber       ) const; // \ru интервал по параметру привязки \en interval by the binding parameter 
          void        AddCalcBreak              ( const MbRect1D & range                 );          
          void        ChangeBreaksSegNumbers    ( const SArray<size_t> & oldBaseNumbers,
                                                  SArray<size_t> & oldEqCounts, 
                                                  SArray<size_t> & newEqCounts           );       // \ru изменить номера сегментов у разрывов \en change segments numbers of breaks           
          void        RedefineBreaksParts       ();                                               // \ru доопределить неопределенные части разрыва \en complete the definition of indefinite parts of the break  
          
          // \ru для преобразования контуров \en for transformation of contours 
          void        TransformBreaks           ( const MbMatrix & matr                  );       // \ru преобразование в соответствии с матрицей \en transform according to the matrix 
          
          size_t      GetLocalBreaksRange       ( const MbBreaksPart & part, 
                                                  MbRect1D & localRect                   ) const; // \ru разрыв по номеру в параметрах сегмента \en a break by the number in segment parameters 

         
          void        operator =                ( const MbContourWithBreaks &            );       // \ru не реализован \en not implemented 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbContourWithBreaks )
  
}; // MbContourWithBreaks

IMPL_PERSISTENT_OPS( MbContourWithBreaks )

//------------------------------------------------------------------------------
/** \brief \ru Часть разрыва.
           \en Part of break. \~
  \details \ru Часть разрыва контура мультилинии. Относится к одному сегменту контура.\n
    Для использования в разрыве MbBreak.
           \en Part of multiline contour break. Applicable to one segment of the contour.\n
    For using in the break MbBreak. \~
  \ingroup Algorithms_2D
*/ // ---
class MATH_CLASS MbBreaksPart {

private :
  size_t          segNumber;    // \ru Номер сегмента \en Number of the segment.
  double          fixedVar;     // \ru Фиксированная переменная \en Fixed variable
                                // \ru ( tMax - tCentre ) / ( tCentre - tMin ), tCentre - параметр центра разрыва \en ( tMax - tCentre ) / ( tCentre - tMin ), tCentre - center of the break 
                                // \ru для отрезка - расстояние до проекции неподвижной точки \en for segment - distance to projection of the fixed point 
  double          length;       // \ru Длина части разрыва (для отрезка) \en Length of the break part (for segment) 
                                // \ru для дуг и по умолчанию сохраняем параметрическую длину \en For arcs and save parametric length by default. 
                                // \ru (для дуги - угол) \en (for arc -  angle) 
  MbCartPoint     fixedPoint;   // \ru неподвижная точка ( корректное значение для отрезка ) \en fixed point (correct value for segment)  

public:  

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по номеру сегмента, фиксированной переменной, длине, неподвижной точке.
             \en Constructor by a number of segment, fixed variable, length, fixed point. \~     
    \param[in] sNumber - \ru Номер сегмента контура, на котором находится часть разрыва.
                         \en Number of contour segment where a part of break is located. \~
    \param[in] fixVar  - \ru Фиксированная переменная:\n
                         для отрезка - расстояние до проекции неподвижной точки,\n
                         в общем случае - величина, равная ( tMax - tCentre ) / ( tCentre - tMin ), где\n
                         tMin - минимальный параметр сегмента,\n
                         tMax - максимальный параметр сегмента,\n
                         tCentre - параметр центра части разрыва.
                         \en Fixed variable:\n
                         for segment - distance to projection of the fixed point,\n
                         In general case - a value which is equal to ( tMax - tCentre ) / ( tCentre - tMin ), where \n
                         tMin - minimal parameter of the segment,\n
                         tMax - maximal parameter of the segment,\n
                         tCentre - parameter of the break part center. \~ 
    \param[in] len     - \ru Длина части разрыва:\n
                         для отрезка - метрическая длина,\n
                         в общем случае - параметрическая длина.
                         \en Length of the break part:\n
                         for segment - metric length, \n
                         In the general case - parametric length. \~
    \param[in] p       - \ru Неподвижная точка:\n
                         для отрезка - используется для привязки части разрыва,\n
                         в общем случае - не имеет смысла.
                         \en Fixed point:\n
                         for segment - used to bind part of the break,\n
                         in general case - it is useless. \~
  */
  MbBreaksPart( size_t sNumber, double fixVar, double len, const MbCartPoint & p )
    : segNumber ( sNumber     ),  
      fixedVar  ( fixVar      ),
      length    ( len         ),
      fixedPoint( p           )    
  {
  }

  /// \ru Копирующий конструктор. \en Copy-constructor.    
  MbBreaksPart( const MbBreaksPart & other )
    : segNumber ( other.GetSegmentNumber() ),  
      fixedVar  ( other.GetFixedVar()      ),
      length    ( other.GetLength()        ),
      fixedPoint( other.GetFixedPoint()    )  
  {
  }

  ~MbBreaksPart(){};
       
  /**\ru \name Функции доступа к данным.
     \en \name Functions for access to data.
      \{ */
        size_t        GetSegmentNumber()  const { return segNumber;   } ///< \ru Номер сегмента контура. \en A number of the contour segment. 
        double        GetFixedVar()       const { return fixedVar;    } ///< \ru Фиксированная переменная. \en fixed variable 
        double        GetLength()         const { return length;      } ///< \ru Длина части разрыва. \en Length of the break part. 
  const MbCartPoint & GetFixedPoint()     const { return fixedPoint;  } ///< \ru Фиксированная точка. \en Fixed point.   

  /** \} */  
  /**\ru \name Функции изменения данных.
     \en \name Functions for changing data.
      \{ */
        
        /// \ru Изменить номер сегмента контура. \en Change a number of the contour segment. 
        void          SetSegmentNumber( size_t newNumber          ) { segNumber = newNumber;    }
        /// \ru Изменить фиксированную переменную. \en Change a fixed variable. 
        void          SetFixedFar     ( double newFixedVar        ) { fixedVar = newFixedVar;   }
        /// \ru Изменить длину части разрыва. \en Change the length of the break part. 
        void          SetLength       ( double newLength          ) { length = newLength;       }
        /// \ru Изменить фиксированную точку. \en Change a fixed point. 
        void          SetFixedPoint   ( const MbCartPoint & point ) { fixedPoint.Init( point ); }   
       
        /** \brief \ru Переместить.
                   \en Move. \~
          \details \ru Переместить на вектор.\n
                   \en Move by vector.\n \~    
          \param[in] to - \ru Вектор перемещения.
                          \en Movement vector. \~       
        */
        void          Move            ( const MbVector & to ) { fixedPoint.Move( to ); }
                     
        /** \brief \ru Повернуть.
                   \en Rotate. \~
          \details \ru Повернуть на угол вокруг точки.\n
                   \en Rotate at angle around a point.\n \~    
          \param[in] pnt   - \ru Точка - центр поворота.
                             \en A point is a rotation center. \~
          \param[in] angle - \ru Двумерный нормализованный вектор, задающий угол вращения.
                             \en A two-dimensional normalized vector which defines a rotation angle. \~
        */
        void          Rotate          ( const MbCartPoint & pnt, const MbDirection & angle ) { fixedPoint.Rotate( pnt, angle ); }
                 
        /** \brief \ru Преобразование.
                   \en Transformation. \~
          \details \ru Преобразование в соответствии с матрицей.\n
                   \en Transform according to matrix.\n \~
          \param[in] matr - \ru Матрица трансформации.
                            \en Transformation matrix. \~          
        */
        void          Transform       ( const MbMatrix & matr ) { fixedPoint.Transform( matr ); }

        /** \brief \ru Изменить номер сегмента.
                   \en Change a number of the segment. \~
          \details \ru Изменить номер сегмента на заданную величину.\n
            Номер сегмента не изменится, если величина изменения будет отрицательной и большей по модулю, чем номер.
                   \en Change a number of the segment by a given value. \n
            The number of the segment does not change if the amount of change is negative and greater in absolute value than the number. \~  
          \param[in] deltaN - \ru Величина увеличения номера сегмента.
                              \en Increase value of the segment number. \~         
        */
        void          ChangeSegNumber ( ptrdiff_t deltaN ) { if( deltaN >= 0 || (ptrdiff_t)segNumber >= -deltaN ) segNumber += deltaN; }
  /** \} */ 
private:
        void          operator =      ( const MbBreaksPart & ); // \ru не реализован \en not implemented 
}; // MbBreaksPart

   
//------------------------------------------------------------------------------
/** \brief \ru Разрыв.
           \en Break. \~
  \details \ru Разрыв контура.\n 
    Для использования в контуре с разрывом MbContourWithBreaks.\n
    Разрыв состоит из частей MbBreaksPart, каждая из которых находится на одном сегменте контура.\n     
    В разрыве может быть 1 или 2 части.
    Если разрыв должен располагаться более чем на трех сегментах, то он имеет 2 части, 
    соответствующие первому и последнему сегментам.
           \en Contour break.\n 
    For using in the contour with break MbContourWithBreaks.\n
    The break consists of parts MbBreaksPart all of which are on the same segment of the contour. \n     
    The break can have 1 or 2 parts.
    If the break must be located more than three segments it has two parts, 
    corresponding to the first and the last segments. \~
  \ingroup Algorithms_2D
*/ // ---
class MATH_CLASS MbBreak {

private:
  SArray<MbBreaksPart> parts; // \ru части разрыва: \en part of break: 
                              // \ru одна, если разрыв на одном сегменте, \en one if the break is on the one segment,  
                              // \ru две, если на нескольких сегментах - первая и последняя \en two if break is on the several segments - the first and the last 

public:

  /** \brief \ru Конструктор пустого разрыва.
             \en Constructor of an empty break. \~
    \details \ru Конструктор пустого разрыва.\n  
      Такой разрыв не может находиться в контуре с разрывом MbContourWithBreaks. 
      Он будет удален при перестроении.
             \en Constructor of an empty break. \n  
      Such break can not be in the contour with break MbContourWithBreaks. 
      It will be removed when rebuilding. \~
  */
  MbBreak(): parts () { }

  /// \ru Копирующий конструктор. \en Copy-constructor. 
  MbBreak( const MbBreak & other ): parts ( other.parts ) {}

  ~MbBreak() {}

public:

  /**\ru \name Функции доступа к данным.
     \en \name Functions for access to data.
  \{ */

  ///< \ru Количество частей. \en The number of parts. 
  size_t          PartsCount  ()                const { return parts.Count(); }     
  
  /** \brief \ru Часть по номеру.
             \en A part by the number. \~
    \details \ru Часть по номеру части разрыва.\n  
      Номер не проверяется на корректность.
             \en A part by the number of the break part. \n  
      A number isn't checked for correctness. \~ 
    \param[in] number - \ru Номер части разрыва, должен быть меньше количества частей.
                        \en The number of break part must be less than the number of parts. \~
    \return \ru Ссылку на часть разрыва.
            \en Reference to part of break. \~
  */
  MbBreaksPart &  GetPart     ( size_t number ) const { return parts[number]; }  
  
  /** \} */  
  /**\ru \name Функции изменения данных.
     \en \name Functions for changing data.
      \{ */

  /// \ru Добавить часть разрыва. \en Add a part of break. 
  void            AddPart     ( MbBreaksPart part )   { parts.Add( part );    }     
  /// \ru Удалить все части разрыва. \en Remove all parts of break. 
  void            DeleteParts ()                      { parts.HardFlush();    }     
  
  /** \brief \ru Удалить часть.
             \en Remove a part. \~
    \details \ru Удалить часть разрыва по номеру.\n  
      Номер проверяется на корректность. 
      Если номер не меньше количества частей, то разрыв не изменится.
             \en Remove a part of break by the number. \n  
      A number Is checked for correctness. 
      If the number isn't less than the number of parts the break doesn't change. \~
    \param[in] number - \ru Номер части разрыва, должен быть меньше количества частей.
                        \en The number of break part must be less than the number of parts. \~
  */
  void            DeletePart  ( size_t number     )   { if( number < PartsCount() ) parts.RemoveInd( number ); }
  
  /** \brief \ru Переместить.
             \en Move. \~
    \details \ru Переместить на вектор.\n
             \en Move by vector.\n \~    
    \param[in] to - \ru Вектор перемещения.
                    \en Movement vector. \~       
  */
  void Move( const MbVector & to )                                                  
  { 
    for( size_t i = 0, count = parts.Count(); i < count; ++i )  
      parts[i].Move( to );  
  }  
  
  /** \brief \ru Повернуть.
             \en Rotate. \~
    \details \ru Повернуть на угол вокруг точки.\n
             \en Rotate at angle around a point.\n \~    
    \param[in] pnt   - \ru Точка - центр поворота.
                       \en A point is a rotation center. \~
    \param[in] angle - \ru Двумерный нормализованный вектор, задающий угол вращения.
                       \en A two-dimensional normalized vector which defines a rotation angle. \~
  */
  void Rotate( const MbCartPoint & pnt, const MbDirection & angle )               
  { 
    for( size_t i = 0, count = parts.Count(); i < count; ++i )  
      parts[i].Rotate( pnt, angle );  
  }  
  
  /** \brief \ru Преобразовать.
             \en Transform. \~
    \details \ru Преобразовать в соответствии с матрицей.\n
             \en Transform according to matrix.\n \~
    \param[in] matr - \ru Матрица трансформации.
                      \en Transformation matrix. \~          
  */
  void Transform( const MbMatrix & matr )                                          
  {
    for( size_t i = 0, count = parts.Count(); i < count; ++i ) 
      parts[i].Transform( matr );
  } 
  /** \} */ 
private:
  void operator =( const MbBreak & ); // \ru не реализован \en not implemented 

}; // MbBreak

        
//------------------------------------------------------------------------------
/** \brief \ru Ближайшие проекции на контур.
           \en Nearest projections on the contour. \~
  \details \ru Ближайшие проекции точки на контур.\n
           \en Nearest projections of point on the contour. \n \~
  \param[in]  contour - \ru Контур.
                        \en A contour. \~
  \param[in]  pnt     - \ru Проецируемая точка.
                        \en Projecting point. \~
  \param[out] tProjs  - \ru Параметры ближайших проекций.
                        \en Parameters of nearest projections. \~ 
  \param[in]  isNear  - \ru Выбрать только проекции, 
                        находящиеся от проецируемой точки не дальше заданной точности.
                        \en Select only the projections 
                        which are located from projecting point within a given tolerance. \~
  \param[in]  mEps    - \ru Точность выбора ближайших точек.
                        \en A tolerance of nearest points selection. \~
  \ingroup Algorithms_2D
*/
// ---
MATH_FUNC (void) NearPointProjections( const MbContour & contour, const MbCartPoint & pnt, 
                                       SArray<double> & tProjs, bool isNear, double mEps = METRIC_REGION );



#endif // __CUR_CONTOUR_WITH_BREAKS_H
