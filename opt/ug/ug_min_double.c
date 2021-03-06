#include "UG_LIB.h"

double ug_min_double
 (INT_ Start_Index,
  INT_ End_Index,
  DOUBLE_1D * Data)

{

/*
 * Get minimum value in a DOUBLE array
 * 
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_min_double.c,v 1.4 2012/08/25 18:54:13 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ Index;

  double Min_Value;

  Min_Value = Data[End_Index];

  for (Index = Start_Index; Index < End_Index; ++Index)
  {
    Min_Value = MIN (Min_Value, Data[Index]);
  }

  return (Min_Value);

}
