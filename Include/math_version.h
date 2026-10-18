////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Математическая версия.
         \en Mathematical version. \~

*/
////////////////////////////////////////////////////////////////////////////////

// \ru AS *nix: RC не переваривает BOM (пропадает #ifndef) \en AS *nix: RC does not understand BOM (#ifndef disappears) 
// \ru строка пропущена специально \en the line is skipped advisedly 
#ifndef __MATH_VERSION_H
#define __MATH_VERSION_H

#include <system_types.h>
#include <math_define.h>


#define MATH_5_11_R03_VERSION     0x0590005CL ///< \ru Версия файла - 5.11. \en The file version - 5.11.  \~ \ingroup Base_Tools
#define MATH_5_BC_VERSION         0x0590005FL ///< \ru Версия файла - 5.0 \en The file version - 5.0  \~ \ingroup Base_Tools

#define MATH_6_0_VERSION          0x06000009L ///< \ru Версия файла - 6.0. \en The file version - 6.0.  \~ \ingroup Base_Tools
#define MATH_6_PLUS_VERSION       0x06000032L ///< \ru Версия файла - 6.1. \en The file version - 6.1.  \~ \ingroup Base_Tools

#define MATH_7_0_VERSION          0x07000005L ///< \ru Версия файла - 7.0. \en The file version - 7.0.  \~ \ingroup Base_Tools
#define MATH_7_PLUS_VERSION       0x0701012CL ///< \ru Версия файла - 7.1. \en The file version - 7.1.  \~ \ingroup Base_Tools

#define MATH_8_0_VERSION          0x0800001AL ///< \ru Версия файла - 8.0. \en The file version - 8.0.  \~ \ingroup Base_Tools
#define MATH_8_PLUS_VERSION       0x08000133L ///< \ru Версия файла - 8.1. \en The file version - 8.1.  \~ \ingroup Base_Tools

#define MATH_9_0_VERSION          0x09000005L ///< \ru Версия файла - 9.0. \en The file version - 9.0.  \~ \ingroup Base_Tools
#define MATH_9_SP1_VERSION        0x09000008L ///< \ru Версия файла - 9.1. \en The file version - 9.1.  \~ \ingroup Base_Tools

#define LAST_NOUNICODE_VERSION    0x0A000018L ///< \ru Версия файла - 10.0 NoUnicode. \en The file version - 10.0 NoUnicode.  \~ \ingroup Base_Tools
#define UNICODE_VERSION           0x0A001000L ///< \ru Версия файла - 10.0 Unicode. \en The file version - 10.0 Unicode.  \~ \ingroup Base_Tools

#define MATH_10_VERSION           0x0A001021L ///< \ru Версия файла - 10.0. \en The file version - 10.0.  \~ \ingroup Base_Tools
#define MATH_10_SP1_VERSION       0x0A001023L ///< \ru Версия файла - 10.1. \en The file version - 10.1.  \~ \ingroup Base_Tools

#define MATH_11_VERSION           0x0B000031L ///< \ru Версия файла - 11.0. \en The file version - 11.0.  \~ \ingroup Base_Tools
#define MATH_11_SP1_VERSION       0x0B000032L ///< \ru Версия файла - 11.1. \en The file version - 11.1.  \~ \ingroup Base_Tools

#define MATH_12_VERSION           0x0C00004DL ///< \ru Версия файла - 12.0. \en The file version - 12.0.  \~ \ingroup Base_Tools
#define MATH_12_SP1_VERSION       0x0C00004EL  ///< \ru Версия файла - 12.1. \en The file version - 12.1.  \~ \ingroup Base_Tools

#define MATH_13_START_VERSION     0x0D000000L ///< \ru Версия файла - 13.0 (начало версии). \en The file version - 13.0 (start of version).  \~ \ingroup Base_Tools
#define MATH_13_VERSION           0x0D000060L ///< \ru Версия файла - 13.0. \en The file version - 13.0.  \~ \ingroup Base_Tools
#define MATH_13_SP1_VERSION       0x0D001016L ///< \ru Версия файла - 13.1. \en The file version - 13.1.  \~ \ingroup Base_Tools
#define MATH_13_SP2_VERSION       0x0D002004L ///< \ru Версия файла - 13.2. \en The file version - 13.2.  \~ \ingroup Base_Tools
#define MATH_13_SP3_START_VERSION 0x0D003000L ///< \ru Версия файла - 13.3 (начало версии). \en The file version - 13.3 (start of version).  \~ \ingroup Base_Tools
#define MATH_13_SP3_VERSION       0x0D003001L ///< \ru Версия файла - 13.3. \en The file version - 13.3.  \~ \ingroup Base_Tools

#define MATH_14_START_VERSION     0x0E000000L ///< \ru Версия файла - 14.0 (начало версии). \en The file version - 14.0 (start of version).  \~ \ingroup Base_Tools
#define MATH_14_VERSION           0x0E000021L ///< \ru Версия файла - 14.0. \en The file version - 14.0.  \~ \ingroup Base_Tools
#define MATH_14_SP1_START_VERSION 0x0E001000L ///< \ru Версия файла - 14.1 (начало версии). \en The file version - 14.1 (start of version).  \~ \ingroup Base_Tools
#define MATH_14_SP1_VERSION       0x0E001011L ///< \ru Версия файла - 14.1. \en The file version - 14.1.  \~ \ingroup Base_Tools
#define MATH_14_SP2_START_VERSION 0x0E002000L ///< \ru Версия файла - 14.2 (начало версии). \en The file version - 14.2 (start of version).  \~ \ingroup Base_Tools
#define MATH_14_SP2_VERSION       0x0E002001L ///< \ru Версия файла - 14.2. \en The file version - 14.2.  \~ \ingroup Base_Tools

#define MATH_15_START_VERSION     0x0F000000L ///< \ru Версия файла - 15.0 (начало версии). \en The file version - 15.0 (start of version).  \~ \ingroup Base_Tools
#define MATH_15_VERSION           0x0F000014L ///< \ru Версия файла - 15.0. \en The file version - 15.0.  \~ \ingroup Base_Tools
#define MATH_15_SP1_START_VERSION 0x0F001000L ///< \ru Версия файла - 15.1 (начало версии). \en The file version - 15.1 (start of version).  \~ \ingroup Base_Tools
#define MATH_15_SP1_VERSION       0x0F001010L ///< \ru Версия файла - 15.1. \en The file version - 15.1.  \~ \ingroup Base_Tools
#define MATH_15_SP2_START_VERSION 0x0F002000L ///< \ru Версия файла - 15.2 (начало версии). \en The file version - 15.2 (start of version).  \~ \ingroup Base_Tools
#define MATH_15_SP2_VERSION       0x0F002010L ///< \ru Версия файла - 15.2. \en The file version - 15.2.  \~ \ingroup Base_Tools

#define MATH_16_START_VERSION     0x10000000L ///< \ru Версия файла - 16.0 (начало версии). \en The file version - 16.0 (start of version).  \~ \ingroup Base_Tools
#define C3D_16_VERSION            0x10000008L ///< \ru Версия файла - C3D 16.0. \en The file version - C3D 16.0.  \~ \ingroup Base_Tools
#define MATH_16_VERSION           0x1000000DL ///< \ru Версия файла - 16.0. \en The file version - 16.0.  \~ \ingroup Base_Tools
#define MATH_16_SP1_START_VERSION 0x10001000L ///< \ru Версия файла - 16.1 (начало версии). \en The file version - 16.1 (start of version).  \~ \ingroup Base_Tools
#define MATH_16_SP1_VERSION       0x10001002L ///< \ru Версия файла - 16.1. \en The file version - 16.1.  \~ \ingroup Base_Tools

#define MATH_17_START_VERSION     0x11000000L ///< \ru Версия файла - 17.0 (начало версии). \en The file version - 17.0 (start of version).  \~ \ingroup Base_Tools
#define C3D_2016_VERSION          0x1100000FL ///< \ru Версия файла - C3D 2016. \en The file version - C3D 2016.  \~ \ingroup Base_Tools
#define MATH_17_VERSION           0x1100001FL ///< \ru Версия файла - 17.0. \en The file version - 17.0.  \~ \ingroup Base_Tools
#define C3D_2017_VERSION      MATH_17_VERSION ///< \ru Версия файла - C3D 2017. \en The file version - C3D 2017.  \~ \ingroup Base_Tools
#define MATH_17_SP1_VERSION       0x11001001L ///< \ru Версия файла - 17.1. \en The file version - 17.1.  \~ \ingroup Base_Tools

#define MATH_18_START_VERSION     0x12000000L ///< \ru Версия файла - 18.0 (начало версии). \en The file version - 18.0 (start of version).  \~ \ingroup Base_Tools
#define C3D_2018_VERSION          0x1200000DL ///< \ru Версия файла - C3D 2018. \en The file version - C3D 2018.  \~ \ingroup Base_Tools
#define MATH_18_VERSION           0x12000010L ///< \ru Версия файла - 18.0. \en The file version - 18.0.  \~ \ingroup Base_Tools

#define MATH_19_START_VERSION     0x13000000L ///< \ru Версия файла - 19.0 (начало версии). \en The file version - 19.0 (start of version).  \~ \ingroup Base_Tools
#define MATH_18_SP1_VERSION       0x13000005L ///< \ru Версия файла - 18.1. \en The file version - 18.1.  \~ \ingroup Base_Tools
#define C3D_2019_VERSION          0x1300000FL ///< \ru Версия файла - C3D 2019. \en The file version - C3D 2019.  \~ \ingroup Base_Tools
#define MATH_19_VERSION           0x13000101L ///< \ru Версия файла - 19.0. \en The file version - 19.0. \~ \ingroup Base_Tools
#define C3D_2020_VERSION          0x13001004L ///< \ru Версия файла - C3D 2020. \en The file version - C3D 2020.  \~ \ingroup Base_Tools
#define MATH_20_TR_VERSION        0x14000012L ///< \ru Версия файла - 20.0 TR. \en The file version - 20.0 TR. \~ \ingroup Base_Tools
#define MATH_20_VERSION           0x14001026L ///< \ru Версия файла - 20.0. \en The file version - 20.0. \~ \ingroup Base_Tools
#define C3D_2021_VERSION          0x14002001L ///< \ru Версия файла - C3D 2021. \en The file version - C3D 2021.  \~ \ingroup Base_Tools
#define MATH_21_VERSION           0x15000001L ///< \ru Версия файла - 21.0. \en The file version - 21.0.  \~ \ingroup Base_Tools
#define MATH_21_UHF_VERSION       0x15000011L ///< \ru Версия файла - 21.0 UHF (Upper Hot Fix). \en The file version - 21.0 UHF (Upper Hot Fix).  \~ \ingroup Base_Tools
#define C3D_2022_VERSION          0x15001001L ///< \ru Версия файла - C3D 2022. \en The file version - C3D 2022.  \~ \ingroup Base_Tools
#define MATH_22_VERSION           0x16000001L ///< \ru Версия файла - 22.0. \en The file version - 22.0.  \~ \ingroup Base_Tools
#define MATH_22_HF1_VERSION       0x16000002L ///< \ru Версия файла - 22.0 HF1. \en The file version - 22.0 HF1.  \~ \ingroup Base_Tools
#define MATH_22_HF2_VERSION       0x16000003L ///< \ru Версия файла - 22.0 HF2. \en The file version - 22.0 HF2.  \~ \ingroup Base_Tools
#define MATH_22_HF3_VERSION       0x16000004L ///< \ru Версия файла - 22.0 HF3. \en The file version - 22.0 HF3.  \~ \ingroup Base_Tools
#define MATH_22_HF4_VERSION       0x16000005L ///< \ru Версия файла - 22.0 HF4. \en The file version - 22.0 HF4.  \~ \ingroup Base_Tools
#define MATH_22_HF5_VERSION       0x16000006L ///< \ru Версия файла - 22.0 HF5. \en The file version - 22.0 HF5.  \~ \ingroup Base_Tools
#define MATH_22_HF6_VERSION       0x16000007L ///< \ru Версия файла - 22.0 HF6. \en The file version - 22.0 HF6.  \~ \ingroup Base_Tools
#define MATH_22_UHF_VERSION       0x16000011L ///< \ru Версия файла - 22.0 UHF (Upper Hot Fix). \en The file version - 22.0 UHF (Upper Hot Fix).  \~ \ingroup Base_Tools
#define C3D_2023_VERSION          0x16001001L ///< \ru Версия файла - C3D 2023. \en The file version - C3D 2023.  \~ \ingroup Base_Tools
#define MATH_23_VERSION           0x17000001L ///< \ru Версия файла - 23.0. \en The file version - 23.0.  \~ \ingroup Base_Tools
#define MATH_23_HF1_VERSION       0x17000003L ///< \ru Версия файла - 23.0 HF1. \en The file version - 23.0 HF1.  \~ \ingroup Base_Tools
#define MATH_23_HF2_VERSION       0x17000004L ///< \ru Версия файла - 23.0 HF2. \en The file version - 23.0 HF2.  \~ \ingroup Base_Tools
#define MATH_23_UHF_VERSION       0x17000101L ///< \ru Версия файла - 23.0 UHF (Upper Hot Fix). \en The file version - 23.0 UHF (Upper Hot Fix).  \~ \ingroup Base_Tools
#define C3D_2024_VERSION          0x17001002L ///< \ru Версия файла - C3D 2024. \en The file version - C3D 2024.  \~ \ingroup Base_Tools
#define MATH_24_VERSION           0x18000001L ///< \ru Версия файла - 24.0. \en The file version - 24.0.  \~ \ingroup Base_Tools
#define MATH_24_HF1_VERSION       0x18000003L ///< \ru Версия файла - 24.0 HF1. \en The file version - 24.0 HF1.  \~ \ingroup Base_Tools
#define MATH_24_HF2_VERSION       0x18000004L ///< \ru Версия файла - 24.0 HF2. \en The file version - 24.0 HF2.  \~ \ingroup Base_Tools
#define MATH_24_HF3_VERSION       0x18000005L ///< \ru Версия файла - 24.0 HF3. \en The file version - 24.0 HF3.  \~ \ingroup Base_Tools
#define MATH_24_HF4_VERSION       0x18000006L ///< \ru Версия файла - 24.0 HF4. \en The file version - 24.0 HF4.  \~ \ingroup Base_Tools
#define MATH_24_HF5_VERSION       0x18000007L ///< \ru Версия файла - 24.0 HF5. \en The file version - 24.0 HF5.  \~ \ingroup Base_Tools
#define MATH_24_HF6_VERSION       0x18000008L ///< \ru Версия файла - 24.0 HF6. \en The file version - 24.0 HF6.  \~ \ingroup Base_Tools
#define MATH_24_HF7_VERSION       0x18000009L ///< \ru Версия файла - 24.0 HF7. \en The file version - 24.0 HF7.  \~ \ingroup Base_Tools
#define MATH_24_HF8_VERSION       0x1800000AL ///< \ru Версия файла - 24.0 HF8. \en The file version - 24.0 HF8.  \~ \ingroup Base_Tools
#define MATH_24_HF9_VERSION       0x1800000BL ///< \ru Версия файла - 24.0 HF9. \en The file version - 24.0 HF9.  \~ \ingroup Base_Tools
#define MATH_24_UHF_VERSION       0x18000101L ///< \ru Версия файла - 24.0 UHF (Upper Hot Fix). \en The file version - 24.0 UHF (Upper Hot Fix).  \~ \ingroup Base_Tools
#define C3D_2025_VERSION          0x18001001L ///< \ru Версия файла - C3D 2025. \en The file version - C3D 2025.  \~ \ingroup Base_Tools

//------------------------------------------------------------------------------
/// \ru Является ли версия файла 16-битной. \en Whether there is a 16-bit file version.  \~ \ingroup Base_Tools
// ---
MATH_FUNC (bool) IsVersion16bit( VERSION version );

//------------------------------------------------------------------------------
/// \ru Является ли версия файла 32-битной. \en Whether there is a 32-bit file version.  \~ \ingroup Base_Tools
// ---
MATH_FUNC (bool) IsVersion32bit( VERSION version );

//------------------------------------------------------------------------------
/// \ru Является ли версия файла 64-битной. \en Whether there is a 64-bit file version.  \~ \ingroup Base_Tools
// ---
MATH_FUNC (bool) IsVersion64bit( VERSION version );


//------------------------------------------------------------------------------
/// \ru Текущая версия потока. \en The current version.  \~ \ingroup Base_Tools
// ---
MATH_FUNC (VERSION) GetCurrentMathFileVersion();


//------------------------------------------------------------------------------
/// \ru Можно ли потенциально сохранить в заданную версию? \en Can it be saved to this math version?  \~ \ingroup Base_Tools
// ---
MATH_FUNC( bool ) CanWriteToMathFileVersion( VERSION dstVertsion, bool * canUseWriterEx = nullptr );


//------------------------------------------------------------------------------
/** \brief \ru Список потенциально допустимых предыдущих версий для записи.
\en List of potentially writable previous releases. \~
\ingroup Base_Tools
*/
//---
enum MbeWritableReleaseVersion
{
  wrv_FirstRelease = MATH_17_SP1_VERSION, ///< \ru Версия потока первого допустимого релиза. \en Version of the first valid release.

  wrv_MATH_17_SP1  = MATH_17_SP1_VERSION, ///< \ru Версия файла - 17.1. \en The file version - 17.1.
  wrv_C3D_2018     = C3D_2018_VERSION,    ///< \ru Версия файла - C3D 2018. \en The file version - C3D 2018.
  wrv_MATH_18      = MATH_18_VERSION,     ///< \ru Версия файла - 18.0. \en The file version - 18.0.
  wrv_MATH_18_SP1  = MATH_18_SP1_VERSION, ///< \ru Версия файла - 18.1. \en The file version - 18.1.
  wrv_C3D_2019     = C3D_2019_VERSION,    ///< \ru Версия файла - C3D 2019. \en The file version - C3D 2019.
  wrv_MATH_19      = MATH_19_VERSION,     ///< \ru Версия файла - 19.0. \en The file version - 19.0.
  wrv_C3D_2020     = C3D_2020_VERSION,    ///< \ru Версия файла - C3D 2020. \en The file version - C3D 2020.
  wrv_MATH_20_TR   = MATH_20_TR_VERSION,  ///< \ru Версия файла - 20.0 TR2. \en The file version - 20.0 TR2.
  wrv_MATH_20      = MATH_20_VERSION,     ///< \ru Версия файла - 20.0. \en The file version - 20.0.
  wrv_C3D_2021     = C3D_2021_VERSION,    ///< \ru Версия файла - C3D 2021. \en The file version - C3D 2021.
  wrv_MATH_21      = MATH_21_VERSION,     ///< \ru Версия файла - 21.0. \en The file version - 21.0.
  wrv_C3D_2022     = C3D_2022_VERSION,    ///< \ru Версия файла - C3D 2022. \en The file version - C3D 2022.
  wrv_MATH_22      = MATH_22_VERSION,     ///< \ru Версия файла - 22.0. \en The file version - 22.0.
  wrv_C3D_2023     = C3D_2023_VERSION,    ///< \ru Версия файла - C3D 2023. \en The file version - C3D 2023.
  wrv_MATH_23      = MATH_23_VERSION,     ///< \ru Версия файла - 23.0. \en The file version - 23.0.
  wrv_C3D_2024     = C3D_2024_VERSION,    ///< \ru Версия файла - C3D 2024. \en The file version - C3D 2024.
  wrv_MATH_24      = MATH_24_VERSION,     ///< \ru Версия файла - 24.0. \en The file version - 24.0.
  wrv_C3D_2025     = C3D_2025_VERSION,    ///< \ru Версия файла - C3D 2025. \en The file version - C3D 2025.

  wrv_PrevRelease  = wrv_MATH_24,         ///< \ru Версия потока предпоследнего релиза. \en The previous release version.
  wrv_LastRelease  = wrv_C3D_2025,        ///< \ru Версия потока последнего релиза. \en The last release version.
  wrv_MaxPossible  = SYS_MAX_INT32        ///< \ru Использовать последнюю версия потока. \en Use current working version.
};


//------------------------------------------------------------------------------
/// \ru Версия потока предпоследнего релиза. \en The previous release version.  \~ \ingroup Base_Tools
// ---
inline VERSION GetPrevReleaseMathFileVersion() {
  return static_cast<VERSION>( wrv_PrevRelease );
}


//------------------------------------------------------------------------------
/// \ru Версия потока последнего релиза. \en The last release version.  \~ \ingroup Base_Tools
// ---
inline VERSION GetLastReleaseMathFileVersion() {
  return static_cast<VERSION>( wrv_LastRelease );
}


//------------------------------------------------------------------------------
/// \ru Предпоследняя версия расширенного формата файла.
//      Расширенный формат строит и использует дерево модели и позволяет выборочное чтение объектов.
//  \en The previous version of extended file format.
//      Extended file format builds and uses the model tree and allows selective reading of objects.
// \~ \ingroup Base_Tools
// ---
MATH_FUNC( VERSION ) GetPrevExtendedFormatVersion();


//------------------------------------------------------------------------------
/// \ru Текущая версия компактного формата файла.
//      Компактный формат записи объектов не использует дерево модели и пишет регистрируемые объекты внутри содержащих их объектов.
//  \en The current version of compact file format.
//      Compact file format does not use the model tree, and writes objects within the containing object.
// \~ \ingroup Base_Tools
// ---
MATH_FUNC (VERSION) GetCurrentCompactFormatVersion();


//------------------------------------------------------------------------------
/// \ru Текущая версия расширенного формата файла.
//      Расширенный формат строит и использует дерево модели и позволяет выборочное чтение объектов.
//  \en The current version of extended file format.
//      Extended file format builds and uses the model tree and allows selective reading of objects.
// \~ \ingroup Base_Tools
// ---
MATH_FUNC(VERSION) GetCurrentExtendedFormatVersion();


#endif // __MATH_VERSION_H
