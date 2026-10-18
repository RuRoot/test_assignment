////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __IGES_WRITE_H
#define __IGES_WRITE_H


#include "iges_basic.h"
#include <tool_cstring.h>
#include <string>


class  ostream;
struct ColourIGES;
struct IGESData;
struct DirEntryParameter; // запись в DE
class  BasicIGES;
struct BasicCurveIGES;
struct LTypeNameIGES; // структура для сохранения типов линий
struct TextItemIGES; // структура элемента текста


//-------------------------------------------------------------------------------
// 
// ---
class CONV_CLASS IWIGES {
// тип функции - записи какого-то элемента - создан для передачи в параметрах
public:
  typedef ptrdiff_t (IWIGES::*WriteEntityFunc)( BasicIGES & );
private:
  IGESData * data;

public :
  IWIGES( std::ostream & _os );
  ~IWIGES();

  // добавить к строке преобразованый к строке и дополненый до 8 символов пробелами int32
  size_t AddLongToString( int32     l, char ch1 = 0 );
#if defined(PLATFORM_64)
  size_t AddLongToString( ptrdiff_t l, char ch1 = 0 );
#endif // PLATFORM_64

  // Преобразовать строковую константу к строковой константе IGES
  std::string & AddString( std::string & s, std::string & d, std::string & delimiter  );
  // Преобразовать double к строковой константе IGES
  std::string & AddDouble( double d, std::string & s, std::string & delimiter );
  // Преобразовать int32 к строковой константе IGES
  std::string & AddLong( int32     l, std::string & s, std::string & delimiter );
#if defined(PLATFORM_64)
  std::string & AddLong( ptrdiff_t l, std::string & s, std::string & delimiter );
#endif // PLATFORM_64

  // добавить к строке выовода другую строку, если длина превышает критическую
  // вывести строку, обнулить ее и добавить к ней остаток. Если стоит флаг вывода -
  // вывести остаток и обнулить строку
  // применяется для глобальной секции и секции комментария
  bool     AddValToStrAndOut( std::string  & outS,    // добавляемая строка
                              ptrdiff_t & numStr,       // номер строки в секции
                              char      section,      // символ секции
                              bool      out = false );// флаг вывода


  // добавить к строке выовода секции PD другую строку, если длина превышает критическую
  // вывести строку, обнулить ее и добавить к ней остаток. Если стоит флаг вывода -
  // вывести остаток и обнулить строку
  ptrdiff_t AddValToStrAndOutPD( std::string & outS,
                                bool   divide,   // строку можно разделять, числа - нежелательно
                                bool   out );
  // вывести в секцию PD int32 после него запятая
  ptrdiff_t WriteLong( int32 v );
#if defined(PLATFORM_64)
  ptrdiff_t WriteLong( ptrdiff_t v );
#endif // PLATFORM_64
  // вывести в секцию PD double после него запятая
  ptrdiff_t WriteDouble( double v );
  // вывести в секцию PD х, Y, и  z. после каждого запятая
  ptrdiff_t WriteXY0ZPD( double x, double y, double z = 0 );
  // вывести в секцию PD х, y. после каждого запятая
  ptrdiff_t WriteXYPD( double x, double y );
  // вывести в секцию PD дополнительные нулевые указатели, в конце - ";"
  ptrdiff_t WriteAddNULLPointerPD();
  // Ищет такую структуру в массиве записанных в IGES стркутур. Если находит
  // - уничтожает присланное и возвращает номер найденного, если нет - возвращает
  // 0, в случае ошибки - возвращает -1
  ptrdiff_t FindOrAddBasicIGES( BasicIGES * b );
  // запись примитива. в параметре - процедура записи этого примитива и его структура.
  // перед записью производится проверка - нет ли уже такой и если есть - присланная
  // структура уничтожается, если нет - запускается процедура записи,
  // возвращается номер строки DE
  ptrdiff_t WriteEntity( WriteEntityFunc func, BasicIGES * );
  // подготовка к записи - применять в паре с функцией FinishRecord - только
  // для записей, где не нужен стиль линии, уровень и номер формы
  // возвращает запомненый указатель- начало записи в PD
  ptrdiff_t PrepareRecord( BasicIGES & bi );
  // завершение записи - применять в паре с функцией PrepareRecord - только
  // для записей, где не нужен стиль линии
  // возвращает номер строки- начало записи в DE
  ptrdiff_t FinishRecord( BasicIGES & bi );
  // завершение записи - применять в паре с функцией PrepareRecord - только
  // для записей, где НУЖЕН стиль линии
  // возвращает номер строки- начало записи в DE
  ptrdiff_t FinishCurveRecord( BasicCurveIGES & bi );
  void     ClearBuffer();

  // заполнить структуру глобальной секции  и секции комментария
  ptrdiff_t Global( const c3d::path_string & fileName,
                    const double           & gabarit,
                    const std::string      & documentName, // Название документа
                    const std::string      & author,
                    const std::string      & organization,
                    const std::string      & productComments,
                    const std::string      & writingCADId, // Идентификация экспортирующей САПР
                    double lenUnits );
  // Запись завершения в файл IGES
  void     Terminate();

  // инициализировать DE
  DirEntryParameter & InitDirEntry( ptrdiff_t type,
                                    ptrdiff_t numPD,
                                    ptrdiff_t level,
                                    ptrdiff_t numForm,
                                    ptrdiff_t color,
                                    ptrdiff_t matrix,
                                    unsigned short vectorDE );
  // инициализировать геом. DE
  DirEntryParameter & InitCurveDirEntry( ptrdiff_t type,
                                         ptrdiff_t numPD,
                                         ptrdiff_t level,
                                         LTypeNameIGES & lt,
                                         ptrdiff_t numForm,
                                         ptrdiff_t matrix,
                                         unsigned short  vectorDE );
  // сформировать запись 2х строк DirEntry
  bool     DirEntry( DirEntryParameter & de );

  // записать цвет
  ptrdiff_t Color( BasicIGES * color );
  ptrdiff_t Colour( ColourIGES & );

  // вернуть структуру типа линии из массива
  ptrdiff_t GetTypeLine ( ptrdiff_t num, LTypeNameIGES & lt );
  // найти номер структуры типа линии в массиве
  ptrdiff_t FindTypeLine( LTypeNameIGES & lt );
  // добавить структуру типа линии в массив
  LTypeNameIGES * AddTypeLine ( LTypeNameIGES & lt );

  // вернуть буферную строку
  std::string & BuffStr     ();
  // разделитель
  std::string & Delimiter();
  // разделитель в записях
  std::string & RecordDelimiter();

  // число строк в записи
  ptrdiff_t GetCountRowInRec();
  // запомнить и обнулить число строк в записи
  void     KeepInMindAndResetCountRowInRec();
  // восстановить число строк в записи
  void     RestoreCountRowInRec();
  // обнулить число строк в записи
  void     ResetCountRowInRec();

  // счетчик строк секции DE
  ptrdiff_t GetCountStringDE();

  // счетчик строк секции PD
  ptrdiff_t GetCountStringPD();

  // вернуть массив пар номеров ресурса строк ( названия в Компасе и в IGES ) для формирования отчета 
//  void     GetReport( int32 *& report, int & size );

  // вернуть имя файла из которого пишется
  std::string & GetSourceFileName();

  // сброс геометрии

  // вектор
  ptrdiff_t Direction( BasicIGES & );
  // матрица трансформации
  ptrdiff_t Matrix( BasicIGES & );
  // Точка
  ptrdiff_t Point( BasicIGES & p );
  // Отрезок
  ptrdiff_t LineSeg( BasicIGES & ls );
  // Окружность
  ptrdiff_t ArcOrCircle( BasicIGES & acs );
  // 104 IGS_CONIC_ARC коническая кривая ( эллипс, гипербола, парабола )
  ptrdiff_t Ellipse( BasicIGES & );
  // 126 IGS_RATIONAL_B_SPLINE_CURVE
  ptrdiff_t RationalBSplineCurve( BasicIGES & b );
  // Элемент текста
  void     TextItem( TextItemIGES & ti );
  // Текст
  ptrdiff_t Text( BasicIGES & t );
  // стрелка( или линия выноски ) IGS_LEADER
  ptrdiff_t Leader( BasicIGES & l );
  // вспомогательная линия
  ptrdiff_t WitnessLine( BasicIGES & w );
  // линейный размер
  ptrdiff_t LinDimension( BasicIGES & b );
  // диаметральный размер
  ptrdiff_t DimDimension( BasicIGES & b );
  // радиальный размер
  ptrdiff_t RadDimension( BasicIGES & b );
  // угловой размер
  ptrdiff_t AngDimension( BasicIGES & b );
  // номер строки DE записанного примитива
  ptrdiff_t GetLastDE();
private:
  // записать глобальную секцию
  ptrdiff_t WriteGlobal();
};


#endif // __IGES_WRITE_H
