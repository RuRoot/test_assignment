////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Данные для обеспечения дискретной длины/радиуса/расстояния в процессах пользовательского ввода кривых
         \en Data for support of discrete length/radius/distance in processes of input of curves by user. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __ALG_DISKRETE_LENGTH_DATA_H
#define __ALG_DISKRETE_LENGTH_DATA_H


#include <math_define.h>
#include <templ_parameter.h>


//------------------------------------------------------------------------------
/** \brief \ru Данные для обеспечения дискретной длины/радиуса/расстояния.
           \en Data for support of discrete length/radius/distance. \~
  \details \ru Данные для обеспечения дискретной длины/радиуса/расстояния в процессах 
    пользовательского ввода кривых.\n
    Для округления до числа, кратного значению шага курсора:\n
    Стандартное округление - значение округляется в меньшую сторону, если 
    разница между текущим значением и ближайшим кратным меньше половины шага курсора, 
    в противном случае округление выполняется в большую сторону.
           \en Data for support of discrete length/radius/distance in processes 
    of input of curves by user.\n
    For rounding to the multiple of value of the cursor step:\n
    Standard round-off - the value is rounded down if 
    the difference between the current value and the nearest multiple of the initial value is less than a half of cursor step, 
    the value is rounded up otherwise. \~
  \ingroup Algorithms_2D
*/
// ---
class MATH_CLASS DiskreteLengthData {
private:
  double factor; ///< \ru Число, которому должна быть кратна корректируемая величина. \en Number, which should be a multiple of the value to be corrected. 

public:
  /// \ru Конструктор. \en Constructor. 
  DiskreteLengthData( double fact );

  /// \ru Установить число, которому должна быть кратна корректируемая величина. \en Set the number, which should be a multiple of the value to be corrected. 
  void  SetFactor( double fact );
  /// \ru Скорректировать присланную величину. \en Correct the given value. 
  bool  CorrectLength( double & len ) const;
}; 


//------------------------------------------------------------------------------
/** \brief \ru Параметры построения синусоиды.
           \en Parameters of a sinusoid creation. \~
  \details \ru Параметры построения синусоиды для объекта "Волнистая линия". \n
           \en Parameters of sinusoid construction for object "Wavy line". \n \~
  \ingroup Data_Structures
  */
// ---
class MATH_CLASS CosinusoidPar {
public :
  static const double maxAmpl;           ///< \ru Максимальное значение амплитуды. \en Maximal value of amplitude.  
  static const double minAmpl;           ///< \ru Минимальное  значение амплитуды. \en Minimal value of amplitude.      

  Param<double> m_WaveLineAmpl;          ///< \ru Величина амплитуды. \en Amplitude value. 
  Param<bool>   m_WaveLineAmplByPercent; ///< \ru Амплитуда задается в процентах от длины волны. \en The amplitude is defined as a percentage of the wave length. 
  double        m_WaveLineLen;           ///< \ru Величина длины волны. \en Wave length value. 
  size_t        m_WaveLineCount;         ///< \ru Величина количество полуволн. \en Value of half-waves number. 
  bool          m_WaveLineByCount;       ///< \ru Построение волнистой линии по количеству волн. \en Construction of wavy line by the number of waves. 
  bool          m_WaveLineDir;           ///< \ru Направление первой полуволны вверх или вниз. \en Up or down direction of the first half-wave. 

 public :
  CosinusoidPar();
  CosinusoidPar( const CosinusoidPar & );
  virtual ~CosinusoidPar();

  void    Assign( const CosinusoidPar & );
  void    Read  ( reader & );
  void    Write ( writer & ) const;
};


#endif // __ALG_DISKRETE_LENGTH_DATA_H
