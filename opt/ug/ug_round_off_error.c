#include "UG_LIB.h"

void ug_round_off_error
 (double *Round_Off_Error)

{

/*
 * Determine machine round-off error.
 * 
 * **************************************************
 * ****** THIS ROUTINE SHOULD NOT BE OPTIMIZED ******
 * **************************************************
 * 
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_round_off_error.c,v 1.6 2012/08/25 18:54:13 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ i;

  double Reduction_Factor, Round_Off_Error_i, dc1, dc1_w_Error;

  dc1 = 1.0;

  Reduction_Factor = 0.1;

  Round_Off_Error_i = 0.01;

  i = 2;

  do
  {
    *Round_Off_Error = Round_Off_Error_i;

    Round_Off_Error_i = (Reduction_Factor * Round_Off_Error_i);

    dc1_w_Error = (dc1 - Round_Off_Error_i);

    ++i;
  }
  while (i < 20 && dc1 != dc1_w_Error);

  return;

}
