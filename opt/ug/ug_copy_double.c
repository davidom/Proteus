#include "UG_LIB.h"

void ug_copy_double
 (INT_ Start_Index,
  INT_ End_Index,
  DOUBLE_1D * Source,
  DOUBLE_1D * Destination)

{

/*
 * Copy a DOUBLE array
 * 
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_copy_double.c,v 1.2 2012/08/25 18:54:13 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ Index;

  for (Index = Start_Index; Index <= End_Index; ++Index)
  {
    Destination[Index] = Source[Index];
  }

  return;

}
