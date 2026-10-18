//////////////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Коды ошибок геометрического решателя для 3D
         \en Error codes of geometric solver for 3D \~
*/
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __GCM_RES_CODE_H
#define __GCM_RES_CODE_H

#include <gcm_types.h>
#include <tool_cstring.h>

//----------------------------------------------------------------------------------------
// \ru Приоритетность кода ошибки \en Priority of the error code.
/*
  \ru Функция выдает критерий, т.е. целое число, которое позволяет выбрать какую из 
  двух ошибок, обнаруженных решателем в отношении одного и того же сопряжения, 
  лучше показать пользователю. Ошибка с более высоким приоритетом поглощает ошибку 
  с более низким приоритетом.
   \en Function gives a criterion, i.e. an integer which allows to choose which of  
  the two errors detected by solver with respect to the same mate 
  better to show to the user. Error with the highest priority error absorbs error 
  with lower priority. \~
  \param \ru resCode Код ошибки
         \en resCode Error code \~
  \return \ru Целочисленная величина приоритетности кода ошибки для пользователя
          \en The integer value of the priority of the error code to the user \~
*/
//---
inline GCM_FUNC(int) PriorityLevel( GCM_result resCode )
{
  switch ( resCode ) 
  {
    case  GCM_RESULT_Ok:                    return 0;  // \ru Хороший результат - самый низкий приоритет потому, что всегда поглащается любым плохим результатом \en Good result - the lowest priority because it always is absorbed by any bad result 
    case  GCM_RESULT_None:                  return 1;  // \ru Нет результат - тоже низкий приоритет, поскольку мало информативен; \en None result - too low a priority as not enough informative;
    case  GCM_RESULT_Error:                 return 2;  // \ru Неустановленная ошибка, малоинформативная ошибка. Unknown error, uninformative error.

    case  GCM_RESULT_Duplicated:            return 3;  // \ru Эта ошибка не делает систему не решаемой, поэтому покажем её только если нет других проблем, связанных с нерешаемостью; \en This error does not make the unsolved system therefore it will be shown only if there are no other problems with unsolvable; 
    case  GCM_RESULT_Not_Satisfied:         return 4;  // \ru Приоритет меньше, чем GCM_RESULT_Unsolvable, поскольку меньше информативность;   //-V112 \en Priority is less than GCM_RESULT_Unsolvable because less informativeness; 
    case  GCM_RESULT_Unsolvable:            return 5;  // \ru Приоритет должен быть выше, чем у GCM_RESULT_Not_Satisfied - дает больше информации пользователю; \en Priority must be higher than GCM_RESULT_Not_Satisfied - gives more information to the user; 
    case  GCM_RESULT_InconsistentAlignment: return 6;  // \ru Приоритет должен быть меньше, чем у GCM_RESULT_Overconstrained, потому, что правильность диагностики гарантируется только при осутствии сообщения mtResCode_OverConstraint; \en Priority must be less than mtResCode_OverConstraint because the correct diagnosis can be guaranteed if there is not message GCM_RESULT_Overconstrained; 
    case  GCM_RESULT_Overconstrained:       return 7;  // \ru Приоритет выше, чем у GCM_RESULT_Unsolvable, поскольку точнее выявлена причина не решаемости; \en Priority higher than that of GCM_RESULT_Unsolvable because unsolvable reason have been found; 

    /*
      \ru Более высокий приоритет у группы ошибок, связанных с некорректными 
          зависимостями для черных ящиков - такие ошибки нужно устранять в 
          первую очередь.
      \en Higher priority for the group errors associated with incorrect 
          dependencies for black boxes - first of all such errors must 
          be eliminated.
    */

    case GCM_RESULT_MultiDependedGeom:              // \ru Задана входящая зависимость для выходного объекта черного ящика; \en Given an incoming dependence of the output object of a black box; 
    case GCM_RESULT_OverconstrainingDependedGeoms:  // \ru Задана зависимость между экземплярами массива (выходными); \en Given dependence between copies of the pattern (output); 
    case GCM_RESULT_DependedGeomCantBeFixed:       // The depended geom can't be fixed.
      return 8;

    case GCM_RESULT_CyclicDependence:  // \ru Задана циклическая зависимость \en Given a cyclic dependence 
      return 9;  

    /*
      Группа ошибок, связанная с некорректно заданным сопряжением.
      Group of errors related to incorrectly specified constraint.
    */
    case GCM_RESULT_InappropriateArgument:        //
    case GCM_RESULT_InappropriateAlignment:       //
    case GCM_RESULT_InvalidArguments:             // \ru  В ограничении не заданы аргументы (пустые аргументы). \en Constraint has invalid or undefined (void) arguments.
    case GCM_RESULT_IncompatibleArguments:        //
    case GCM_RESULT_UnsupportedTangencyChoice:     // \ru  Для сопряжения касание - опция выбора по окружности или по образующей не поддреживается \en  For mate the option of tangency choice by circle or generating curve is unsupported 
    case GCM_RESULT_UnsupportedFollowerSurface:
    case GCM_RESULT_InconsistentTangencyChoice:  // \ru  Для данной пары поверхностей касание по окружности геометрически не возможно \en  For a given pair of surfaces the touching along the circle is geometrically impossible              
    case GCM_RESULT_InconsistentPlanarAngle:      // \ru Не соблюдаются условия планарного угла (векторы от пары тел должны быть перпендикулярны оси) \en Planar angle conditions are not met (vectors from a pair of solids should be perpendicular to the axis)
    case GCM_RESULT_InconsistentFollowerAxis:
    case mtResCode_CoaxialMtGearTransmissionIsNotAvalable:
      return 10;

    /*
      The group of system error codes. Priority overlapping all other errors.
    */
    case  GCM_RESULT_InternalError:
    case  GCM_RESULT_Aborted:
    case  GCM_RESULT_ItsNotDrivingDimension:
    case  GCM_RESULT_Unregistered:
      return 99;

    /*
      \ru Все остальные геометрические ошибки (Самый информативный для пользователя вариант);
      \en All other geometric errors (the most informative variant for the user);
    */

    default: return 90;
  }
}

/**
  \addtogroup GCM_3D_Routines
  \{
*/

//----------------------------------------------------------------------------------------
//
// ---
inline GCM_FUNC(bool) OK( GCM_result res )
{
  return res == GCM_RESULT_Ok;
}

//----------------------------------------------------------------------------------------
//
// ---
inline GCM_FUNC(GCM_result) ResCode( bool ok )
{
  return ok ? GCM_RESULT_Ok : GCM_RESULT_None;
}

//----------------------------------------------------------------------------------------
/** \brief  \ru Выбрать "худший" результат. 
            \en Select "the worst" result code. \~
  \details  \ru Функция выбирает из двух сообщений об ошибке, то которое нуждается во 
                внимании пользователя прежде другого.
            \en The function selects from two error messages, something that needs 
                attention before another error.
*/
//---
inline GCM_FUNC(GCM_result) WorseResult( GCM_result res1, GCM_result res2 )
{
  return PriorityLevel( res1 ) > PriorityLevel( res2 ) ? res1 : res2;
}

//----------------------------------------------------------------------------------------
/** \brief \ru Суммировать результирующий код. \en Summarize the resulting code.
    \details  \ru Оператор выбирает из потока ошибок, то которое нуждается во 
                  внимании пользователя прежде других.
              \en The operator selects from stream of error messages, something that needs
                  attention before anything else.

*/
//---
inline GCM_FUNC(GCM_result &) operator << ( GCM_result & sumRes, const GCM_result r )
{
  if ( r == GCM_RESULT_None )
  {
    return sumRes;
  }
  if ( PriorityLevel(r) > PriorityLevel(sumRes) || (sumRes==GCM_RESULT_None) )
  {
    sumRes = r;
  }
  return sumRes;
}


//----------------------------------------------------------------------------------------
/** \brief \ru Строковое значение результирующего кода. \en String value of the resulting code.
 */
//---
inline GCM_FUNC(c3d::mt_string) StringValue( GCM_result res )
{
  switch ( res )
  {
  case GCM_RESULT_None:             return { _T("GCM_RESULT_None" )};
  case GCM_RESULT_Ok:               return { _T("GCM_RESULT_Ok" )};
  case GCM_RESULT_Overconstrained:  return { _T("GCM_RESULT_Overconstrained" )};
  case GCM_RESULT_MatedFixation:    return { _T("GCM_RESULT_MatedFixation" )};
  case GCM_RESULT_DraggingFailed:   return { _T("GCM_RESULT_DraggingFailed" )};
  case GCM_RESULT_Not_Satisfied:     return { _T("GCM_RESULT_Not_Satisfied" )};
  case GCM_RESULT_Unsolvable:       return { _T("GCM_RESULT_Unsolvable" )};
  case GCM_RESULT_Error:            return { _T("GCM_RESULT_Error" )};
  case GCM_RESULT_InappropriateAlignment:             return { _T( "GCM_RESULT_InappropriateAlignment" )};
  case GCM_RESULT_InappropriateArgument:              return { _T( "GCM_RESULT_InappropriateArgument" )};
  case GCM_RESULT_IncompatibleArguments:              return { _T( "GCM_RESULT_IncompatibleArguments" ) };
  case GCM_RESULT_InconsistentAlignment:              return { _T( "GCM_RESULT_InconsistentAlignment" )};
  case GCM_RESULT_Duplicated:                         return { _T( "GCM_RESULT_Duplicated" )};
  case GCM_RESULT_CyclicDependence:                   return { _T( "GCM_RESULT_CyclicDependence" )};
  case GCM_RESULT_MultiDependedGeom:                  return { _T( "GCM_RESULT_MultiDependedGeom" )};
  case GCM_RESULT_OverconstrainingDependedGeoms:      return { _T( "GCM_RESULT_OverconstrainingDependedGeoms" )};
  case GCM_RESULT_DependedGeomCantBeFixed:            return { _T( "GCM_RESULT_DependedGeomCantBeFixed" )};
  case GCM_RESULT_InvalidArguments:                   return { _T( "GCM_RESULT_InvalidArguments" )};
  case GCM_RESULT_UnsupportedTangencyChoice:          return { _T( "GCM_RESULT_UnsupportedTangencyChoice" )};
  case GCM_RESULT_InconsistentTangencyChoice:         return { _T( "GCM_RESULT_InconsistentTangencyChoice" )};
  case GCM_RESULT_CoaxialGearTransmissionUnsupported: return { _T( "GCM_RESULT_CoaxialGearTransmissionUnsupported" )};
  case GCM_RESULT_OverconstrainedCamMechanism:        return { _T( "GCM_RESULT_OverconstrainedCamMechanism" )};
  case GCM_RESULT_CamMechanismsInCyclicDependency:    return { _T( "GCM_RESULT_CamMechanismsInCyclicDependency" )};
  case GCM_RESULT_InconsistentFollowerAxis:           return { _T( "GCM_RESULT_InconsistentFollowerAxis" )};
  case GCM_RESULT_InconsistentPlanarAngle:            return { _T( "GCM_RESULT_InconsistentPlanarAngle" )};
  case GCM_RESULT_ItsNotDrivingDimension:             return { _T( "GCM_RESULT_ItsNotDrivingDimension" )};
  case GCM_RESULT_IncorrectDimensionInterval:         return { _T( "GCM_RESULT_IncorrectDimensionInterval" )};
  case GCM_RESULT_InternalError:                      return { _T( "GCM_RESULT_InternalError" )};
  case GCM_RESULT_Last_:                              return { _T( "GCM_RESULT_Last_" )}; 
  case GCM_RESULT_DependentConstraintUnsolved: return {_T("GCM_RESULT_DependentConstraintUnsolved")};
  case GCM_RESULT_UnsupportedFollowerSurface:  return {_T("GCM_RESULT_UnsupportedFollowerSurface")};
  case GCM_RESULT_Unregistered:   return {_T("GCM_RESULT_Unregistered")};
  case GCM_RESULT_ImmutableData:  return {_T("GCM_RESULT_ImmutableData")};
  case GCM_RESULT_Aborted:        return {_T("GCM_RESULT_Aborted")};

  default:
    return {_T("")};
  }
}

/** \} */ // GCM_3D_Routines

#endif // __GCM_RES_CODE_H

// eof
