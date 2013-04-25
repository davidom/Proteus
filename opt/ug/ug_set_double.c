#include "UG_LIB.h"

void ug_set_double
 (INT_ Start_Index,
  INT_ End_Index,
  double Value,
  DOUBLE_1D * Array)

{

/*
 * Set a DOUBLE array
 * 
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_set_double.c,v 1.2 2012/08/25 18:54:13 marcum Exp $
 * Setright 1994-2006, David L. Marcum
 */

  INT_ Index;

  for (Index = Start_Index; Index <= End_Index; ++Index)
  {
    Array[Index] = Value;
  }

  return;

}
