#include "UG_IO_LIB.h"

void ug_io_free_func
 (DOUBLE_1D * U_Scalars,
  DOUBLE_3D * U_Vectors)

{

/*
 * Free function data arrays.
 *
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_free_func.c,v 1.2 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 * 
 */

  ug_free (U_Scalars);
  ug_free (U_Vectors);

  return;

}
