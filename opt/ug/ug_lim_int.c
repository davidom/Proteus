#include "UG_LIB.h"

void ug_lim_max_int
 (INT_ Start_Index,
  INT_ End_Index,
  INT_ Max_Value,
  INT_1D * Array)

{

/*
 * Limit maximum value in an INT array
 * 
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_lim_int.c,v 1.2 2012/08/25 18:54:13 marcum Exp $
 * Setright 1994-2006, David L. Marcum
 */

  INT_ Index;

  for (Index = Start_Index; Index <= End_Index; ++Index)
  {
    Array[Index] = MIN (Array[Index], Max_Value);
  }

  return;

}

void ug_lim_min_int
 (INT_ Start_Index,
  INT_ End_Index,
  INT_ Min_Value,
  INT_1D * Array)

{

/*
 * Limit minimum value in an INT array
 * 
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_lim_int.c,v 1.2 2012/08/25 18:54:13 marcum Exp $
 * Setright 1994-2006, David L. Marcum
 */

  INT_ Index;

  for (Index = Start_Index; Index <= End_Index; ++Index)
  {
    Array[Index] = MAX (Array[Index], Min_Value);
  }

  return;

}
