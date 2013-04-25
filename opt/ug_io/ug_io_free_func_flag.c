#include "UG_IO_LIB.h"

void ug_io_free_func_flag
 (CHAR_21 * U_Scalar_Labels,
  CHAR_21 * U_Vector_Labels,
  INT_1D * U_Scalar_Flags,
  INT_1D * U_Vector_Flags)

{

/*
 * Free function label and flag arrays.
 *
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_free_func_flag.c,v 1.2 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 * 
 */

  ug_free (U_Scalar_Labels);
  ug_free (U_Vector_Labels);
  ug_free (U_Scalar_Flags);
  ug_free (U_Vector_Flags);

  return;

}
