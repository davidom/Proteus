#include "UG_LIB.h"

void ug_lim_max_double
 (INT_ Start_Index,
  INT_ End_Index,
  double Max_Value,
  DOUBLE_1D * Array)

{

/*
 * Limit maximum value in an DOUBLE array
 * 
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_lim_double.c,v 1.2 2012/08/25 18:54:13 marcum Exp $
 * Setright 1994-2006, David L. Marcum
 */

  INT_ Index;

  for (Index = Start_Index; Index <= End_Index; ++Index)
  {
    Array[Index] = MIN (Array[Index], Max_Value);
  }

  return;

}

void ug_lim_min_double
 (INT_ Start_Index,
  INT_ End_Index,
  double Min_Value,
  DOUBLE_1D * Array)

{

/*
 * Limit minimum value in an DOUBLE array
 * 
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_lim_double.c,v 1.2 2012/08/25 18:54:13 marcum Exp $
 * Setright 1994-2006, David L. Marcum
 */

  INT_ Index;

  for (Index = Start_Index; Index <= End_Index; ++Index)
  {
    Array[Index] = MAX (Array[Index], Min_Value);
  }

  return;

}
